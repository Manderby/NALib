
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "NABinaryData.h"
#include "NAMemory.h"



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



NA_HIDEF void naPrepareCRCPNG(NAChecksumCRC* checksumcrc){
 uint32 c;
 int n, k;
 for (n = 0; n < 256; n++) {
   c = (uint32) n;
   for (k = 0; k < 8; k++) {
     if (c & 1)
       c = 0xedb88320L ^ (c >> 1);
     else
       c = c >> 1;
    }
    checksumcrc->table[n] = c;
  }
}



NA_HIDEF void naAccumulateCRCPNG(NAChecksumCRC* checksumcrc, const NAByte* buf, NAInt size){
 uint32 c = checksumcrc->value;
 NAInt n;
 for(n = 0; n < size; n++){
   c = checksumcrc->table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
 }
 checksumcrc->value = c;
}



// ////////////////////////////
// Adler 32 Checksum implementation
//
// Code stolen and adapted from the RFC 1950:
// http://www.ietf.org/rfc/rfc1950.txt

#include "NAMathConstants.h"
#define NA_CHECKSUM_ADLER_BASE NA_PRIME_BEFORE_2_16;

typedef struct NAChecksumAdler NAChecksumAdler;
struct NAChecksumAdler{
  uint32 s1;
  uint32 s2;
};



NA_HIDEF void naAccumulateAdler(NAChecksumAdler* checksumadler, const NAByte* buf, NAInt size){
  NAInt n;
  for(n = 0; n < size; n++){
    checksumadler->s1 = (checksumadler->s1 + buf[n]) % NA_CHECKSUM_ADLER_BASE;
    checksumadler->s2 = (checksumadler->s2 + checksumadler->s1) % NA_CHECKSUM_ADLER_BASE;
  }
}





// /////////////////////////////
// General NAChecksum implementation

NA_HDEF NAChecksum* naInitChecksum(NAChecksum* checksum, NAChecksumType type){
  checksum->type = type;
  switch(type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    checksum->data = naAlloc(NAChecksumCRC);
    naPrepareCRCPNG((NAChecksumCRC*)(checksum->data));
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    checksum->data = naAlloc(NAChecksumAdler);
    //naPrepareAdler((NAChecksumAdler*)(checksum->data)); // nothing to be prepared
    break;
  default:
    #ifndef NDEBUG
      naError("naInitChecksum", "Checksum type invalid");
    #endif
    break;
  }
  naResetChecksum(checksum);
  return checksum;
}



NA_HDEF void naClearChecksum(NAChecksum* checksum){
  naFree(checksum->data);
}



NA_HDEF void naResetChecksum(NAChecksum* checksum){
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
      naError("naResetChecksum", "Checksum type invalid");
    #endif
    break;
  }
}



NA_HDEF void naAccumulateChecksum(NAChecksum* checksum, const NAByte* buf, NAInt size){
  switch(checksum->type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    naAccumulateCRCPNG(((NAChecksumCRC*)(checksum->data)), buf, size);
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    naAccumulateAdler(((NAChecksumAdler*)(checksum->data)), buf, size);
    break;
  default:
    #ifndef NDEBUG
      naError("naResetChecksum", "Checksum type invalid");
    #endif
    break;
  }
}



NA_HDEF uint32 naGetChecksumResult(NAChecksum* checksum){
  switch(checksum->type){
  case NA_CHECKSUM_TYPE_CRC_PNG:
    return ((NAChecksumCRC*)(checksum->data))->value ^ 0xffffffff;
    break;
  case NA_CHECKSUM_TYPE_ADLER_32:
    return (((NAChecksumAdler*)(checksum->data))->s2 << 16) + ((NAChecksumAdler*)(checksum->data))->s1;
    break;
  default:
    #ifndef NDEBUG
      naError("naGetChecksumResult", "Checksum type invalid");
    #endif
    break;
  }
  return 0;
}



// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
