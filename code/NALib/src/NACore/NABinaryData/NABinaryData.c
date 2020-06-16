
#include "../../NABinaryData.h"
#include "../../NAMemory.h"



// ////////////////////////////
// PNG CRC implementation
//
// Based on the polynom sum x^[32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]
//
// Code stolen and adapted from the PNG reference:
// http://www.w3.org/TR/PNG/#D-CRCAppendix

typedef struct NAChecksumCRC NAChecksumCRC;
struct NAChecksumCRC{
  uint32 value;
  uint32 table[256];
};



NA_HIDEF void na_PrepareCRCPNG(NAChecksumCRC* checksumcrc){
 int n, k;
 for (n = 0; n < 256; n++) {
   uint32 c = (uint32) n;
   for (k = 0; k < 8; k++) {
     if (c & 1)
       c = 0xedb88320 ^ (c >> 1);
     else
       c = c >> 1;
    }
    checksumcrc->table[n] = c;
  }
}



NA_HIDEF void na_AccumulateCRCPNG(NAChecksumCRC* checksumcrc, const NAByte* buf, NAInt byteSize){
 uint32 c = checksumcrc->value;
 NAInt n;
 for(n = 0; n < byteSize; n++){
   c = checksumcrc->table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
 }
 checksumcrc->value = c;
}



// ////////////////////////////
// Adler 32 Checksum implementation
//
// Code stolen and adapted from the RFC 1950:
// http://www.ietf.org/rfc/rfc1950.txt

#include "../../NAMathConstants.h"
#define NA_CHECKSUM_ADLER_BASE NA_PRIME_BEFORE_2_16;

typedef struct NAChecksumAdler NAChecksumAdler;
struct NAChecksumAdler{
  uint32 s1;
  uint32 s2;
};



NA_HIDEF void na_AccumulateAdler(NAChecksumAdler* checksumadler, const NAByte* buf, NAInt byteSize){
  NAInt n;
  for(n = 0; n < byteSize; n++){
    checksumadler->s1 = (checksumadler->s1 + buf[n]) % NA_CHECKSUM_ADLER_BASE;
    checksumadler->s2 = (checksumadler->s2 + checksumadler->s1) % NA_CHECKSUM_ADLER_BASE;
  }
}





// /////////////////////////////
// General NAChecksum implementation

NA_DEF NAChecksum* naInitChecksum(NAChecksum* checksum, NAChecksumType type){
  checksum->type = type;
  switch(type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    checksum->data = naAlloc(NAChecksumCRC);
    na_PrepareCRCPNG((NAChecksumCRC*)(checksum->data));
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    checksum->data = naAlloc(NAChecksumAdler);
    //naPrepareAdler((NAChecksumAdler*)(checksum->data)); // nothing to be prepared
    break;
  default:
    #ifndef NDEBUG
      naError("Checksum type invalid");
    #endif
    break;
  }
  naResetChecksum(checksum);
  return checksum;
}



NA_DEF void naClearChecksum(NAChecksum* checksum){
  naFree(checksum->data);
}



NA_DEF void naResetChecksum(NAChecksum* checksum){
  switch(checksum->type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    ((NAChecksumCRC*)(checksum->data))->value = 0xffffffff;
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    ((NAChecksumAdler*)(checksum->data))->s1 = 1 & 0xffff;
    ((NAChecksumAdler*)(checksum->data))->s2 = (1 >> 16) & 0xffff;
    break;
  default:
    #ifndef NDEBUG
      naError("Checksum type invalid");
    #endif
    break;
  }
}



NA_DEF void naAccumulateChecksum(NAChecksum* checksum, const NAByte* buf, NAInt byteSize){
  switch(checksum->type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    na_AccumulateCRCPNG(((NAChecksumCRC*)(checksum->data)), buf, byteSize);
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    na_AccumulateAdler(((NAChecksumAdler*)(checksum->data)), buf, byteSize);
    break;
  default:
    #ifndef NDEBUG
      naError("Checksum type invalid");
    #endif
    break;
  }
}



NA_DEF uint32 naGetChecksumResult(NAChecksum* checksum){
  uint32 retvalue;
  switch(checksum->type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    retvalue = ((NAChecksumCRC*)(checksum->data))->value ^ 0xffffffff;
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    retvalue = (((NAChecksumAdler*)(checksum->data))->s2 << 16) + ((NAChecksumAdler*)(checksum->data))->s1;
    break;
  default:
    #ifndef NDEBUG
      naError("Checksum type invalid");
    #endif
    retvalue = 0;
    break;
  }
  return retvalue;
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
