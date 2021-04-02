
// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryData.h"


struct NAChecksum{
  NAChecksumType type;
  void* data;
};



#include "NAEndiannessII.h"



#include <string.h>


NA_IDEF void naCopy8  (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist;
    if(!d)
      naCrash("Pointer d is Null-Pointer.");
    if(!s)
      naCrash("Pointer s is Null-Pointer.");
    dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 1)
      naError("Restrict pointers overlap.");
  #endif
  *(uint8*)d = *(uint8*)s;
}
NA_IDEF void naCopy16 (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist;
    if(!d)
      naCrash("Pointer d is Null-Pointer.");
    if(!s)
      naCrash("Pointer s is Null-Pointer.");
    dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 2)
      naError("Restrict pointers overlap.");
  #endif
  *(uint16*)d = *(uint16*)s;
}
NA_IDEF void naCopy32 (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist;
    if(!d)
      naCrash("Pointer d is Null-Pointer.");
    if(!s)
      naCrash("Pointer s is Null-Pointer.");
    dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 4)
      naError("Restrict pointers overlap.");
  #endif
  *(uint32*)d = *(uint32*)s;
}
NA_IDEF void naCopy64 (void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist;
    if(!d)
      naCrash("Pointer d is Null-Pointer.");
    if(!s)
      naCrash("Pointer s is Null-Pointer.");
    dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 8)
      naError("Restrict pointers overlap.");
  #endif
  *(NAu64*)d = *(NAu64*)s;
}
NA_IDEF void naCopy128(void* NA_RESTRICT d, const void* NA_RESTRICT s){
  #ifndef NDEBUG
    NAInt dist;
    if(!d)
      naCrash("Pointer d is Null-Pointer.");
    if(!s)
      naCrash("Pointer s is Null-Pointer.");
    dist = (NAInt)((NAByte*)d - (NAByte*)s);
    if((NAByte*)d<(NAByte*)s){dist = -dist;};
    if(dist < 16)
      naError("Restrict pointers overlap.");
  #endif
  *(NAu64*)d = *(NAu64*)s;
  d = ((NAByte*)d) + 8;
  s = ((NAByte*)s) + 8;
  *(NAu64*)d = *(NAu64*)s;
}

NA_IDEF void naCopyn(void* NA_RESTRICT d, const void* NA_RESTRICT s, size_t byteSize){
  #ifndef NDEBUG
    if(!d)
      naCrash("Pointer d is Null-Pointer.");
    if(!s)
      naCrash("Pointer s is Null-Pointer.");
    if(byteSize < 1)
      naCrash("count is smaller than 1.");
  #endif
  memcpy(d, s, byteSize);
}




// /////////////////////////////
// Swap multibyte-values
// /////////////////////////////

NA_IDEF void naSwap8(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist;
    if(!a)
      naCrash("Pointer a is Null-Pointer");
    if(!b)
      naCrash("Pointer b is Null-Pointer");
    dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 1)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint8*)a ^= *(uint8*)b;
  *(uint8*)b ^= *(uint8*)a;
  *(uint8*)a ^= *(uint8*)b;
}

NA_IDEF void naSwap16(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist;
    if(!a)
      naCrash("Pointer a is Null-Pointer");
    if(!b)
      naCrash("Pointer b is Null-Pointer");
    dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 2)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint16*)a ^= *(uint16*)b;
  *(uint16*)b ^= *(uint16*)a;
  *(uint16*)a ^= *(uint16*)b;
}

NA_IDEF void naSwap32(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist;
    if(!a)
      naCrash("Pointer a is Null-Pointer");
    if(!b)
      naCrash("Pointer b is Null-Pointer");
    dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 4)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint32*)a ^= *(uint32*)b;
  *(uint32*)b ^= *(uint32*)a;
  *(uint32*)a ^= *(uint32*)b;
}

NA_IDEF void naSwap64(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist;
    if(!a)
      naCrash("Pointer a is Null-Pointer");
    if(!b)
      naCrash("Pointer b is Null-Pointer");
    dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a<(NAByte*)b){dist = -dist;};
    if(dist < 8)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(NAu64*)a = naXoru64(*(NAu64*)a, *(NAu64*)b);
  *(NAu64*)b = naXoru64(*(NAu64*)b, *(NAu64*)a);
  *(NAu64*)a = naXoru64(*(NAu64*)a, *(NAu64*)b);
}

NA_IDEF void naSwap128(void* NA_RESTRICT a, void* NA_RESTRICT b){
  #ifndef NDEBUG
    NAInt dist;
    if(!a)
      naCrash("Pointer a is Null-Pointer");
    if(!b)
      naCrash("Pointer b is Null-Pointer");
    dist = (NAInt)((NAByte*)a - (NAByte*)b);
    if((NAByte*)a < (NAByte*)b){dist = -dist;};
    if(dist < 16)
      naError("Restrict pointers overlap.");
  #endif
  naSwap64(a, b);
  a = ((NAByte*)a) + 8;
  b = ((NAByte*)b) + 8;
  naSwap64(a, b);
}



NA_IDEF void naSwap(double* NA_RESTRICT a, double* NA_RESTRICT b){
  naSwap64(a, b);
}

NA_IDEF void naSwapf(float* NA_RESTRICT a, float* NA_RESTRICT b){
  naSwap32(a, b);
}

NA_IDEF void naSwapi(NAInt* NA_RESTRICT a, NAInt* NA_RESTRICT b){
  #if NA_TYPE_NAINT_BITS == 32
    naSwap32(a, b);
  #elif NA_TYPE_NAINT_BITS == 64
    naSwap64(a, b);
  #endif
}

NA_IDEF void naSwapu(NAUInt* NA_RESTRICT a, NAUInt* NA_RESTRICT b){
  #if NA_TYPE_NAINT_BITS == 32
    naSwap32(a, b);
  #elif NA_TYPE_NAINT_BITS == 64
    naSwap64(a, b);
  #endif
}





NA_IDEF NABool naEqual8(  void* NA_RESTRICT a, void* NA_RESTRICT b){
  return (*((uint8*)a) == *((uint8*)b));
}
NA_IDEF NABool naEqual16( void* NA_RESTRICT a, void* NA_RESTRICT b){
  return (*((uint16*)a) == *((uint16*)b));
}
NA_IDEF NABool naEqual32( void* NA_RESTRICT a, void* NA_RESTRICT b){
  return (*((uint32*)a) == *((uint32*)b));
}
NA_IDEF NABool naEqual64( void* NA_RESTRICT a, void* NA_RESTRICT b){
  return naEqualu64(*((NAu64*)a), *((NAu64*)b));
}
NA_IDEF NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b){
  return naEqualu128(*((NAu128*)a), *((NAu128*)b));
}




// ///////////////////////////////////////////////////////////
// Fills all bytes with zero values
// ///////////////////////////////////////////////////////////

NA_IDEF void naZeron(void* d, size_t byteSize){
  memset(d, 0, byteSize);
}



NA_IDEF void naSetn(void* d, size_t byteSize, NAByte value){
  memset(d, value, byteSize);
}




// ///////////////////////////////////////////////////////////
// Set multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF void naFill8WithBytes( void* d,
                    NAByte b0){
  NAByte* p;
  #ifndef NDEBUG
    if(!d)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)d;
  *p = b0;
}

NA_IDEF void naFill16WithBytes( void* d,
                     NAByte b0, NAByte b1){
  NAByte* p;
  #ifndef NDEBUG
    if(!d)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p = b1;
}

NA_IDEF void naFill32WithBytes( void* d,
                     NAByte b0, NAByte b1, NAByte b2, NAByte b3){
  NAByte* p;
  #ifndef NDEBUG
    if(!d)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p = b3;
}

NA_IDEF void naFill64WithBytes( void* d,
                     NAByte b0, NAByte b1, NAByte b2, NAByte b3,
                     NAByte b4, NAByte b5, NAByte b6, NAByte b7){
  NAByte* p;
  #ifndef NDEBUG
    if(!d)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
  *p++ = b4; *p++ = b5; *p++ = b6; *p   = b7;
}

NA_IDEF void naFill128WithBytes( void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                      NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                      NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                      NAByte b12, NAByte b13, NAByte b14, NAByte b15){
  NAByte* p;
  #ifndef NDEBUG
    if(!d)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)d;
  *p++ = b0;  *p++ = b1;  *p++ = b2;  *p++ = b3;
  *p++ = b4;  *p++ = b5;  *p++ = b6;  *p++ = b7;
  *p++ = b8;  *p++ = b9;  *p++ = b10; *p++ = b11;
  *p++ = b12; *p++ = b13; *p++ = b14; *p   = b15;
}



// ///////////////////////////////////////////////////////////
// Compare multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF NABool naEqual8WithBytes(const void* s, NAByte b0){
  NAByte* p;
  #ifndef NDEBUG
    if(!s)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)s;
  if(*p   != b0){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual16WithBytes(const void* s, NAByte b0, NAByte b1){
  NAByte* p;
  #ifndef NDEBUG
    if(!s)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0){return NA_FALSE;}
  if(*p   != b1){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual32WithBytes(const void* s,
                              NAByte b0, NAByte b1, NAByte b2, NAByte b3){
  NAByte* p;
  #ifndef NDEBUG
    if(!s)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0){return NA_FALSE;}
  if(*p++ != b1){return NA_FALSE;}
  if(*p++ != b2){return NA_FALSE;}
  if(*p   != b3){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual64WithBytes(const void* s,
                              NAByte b0, NAByte b1, NAByte b2, NAByte b3,
                              NAByte b4, NAByte b5, NAByte b6, NAByte b7){
  NAByte* p;
  #ifndef NDEBUG
    if(!s)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0){return NA_FALSE;}
  if(*p++ != b1){return NA_FALSE;}
  if(*p++ != b2){return NA_FALSE;}
  if(*p++ != b3){return NA_FALSE;}
  if(*p++ != b4){return NA_FALSE;}
  if(*p++ != b5){return NA_FALSE;}
  if(*p++ != b6){return NA_FALSE;}
  if(*p   != b7){return NA_FALSE;}
  return NA_TRUE;
}

NA_IDEF NABool naEqual128WithBytes(const void* s,
                               NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                               NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                               NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                               NAByte b12, NAByte b13, NAByte b14, NAByte b15){
  NAByte* p;
  #ifndef NDEBUG
    if(!s)
      naCrash("Pointer is Null-Pointer.");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0) {return NA_FALSE;}
  if(*p++ != b1) {return NA_FALSE;}
  if(*p++ != b2) {return NA_FALSE;}
  if(*p++ != b3) {return NA_FALSE;}
  if(*p++ != b4) {return NA_FALSE;}
  if(*p++ != b5) {return NA_FALSE;}
  if(*p++ != b6) {return NA_FALSE;}
  if(*p++ != b7) {return NA_FALSE;}
  if(*p++ != b8) {return NA_FALSE;}
  if(*p++ != b9) {return NA_FALSE;}
  if(*p++ != b10){return NA_FALSE;}
  if(*p++ != b11){return NA_FALSE;}
  if(*p++ != b12){return NA_FALSE;}
  if(*p++ != b13){return NA_FALSE;}
  if(*p++ != b14){return NA_FALSE;}
  if(*p   != b15){return NA_FALSE;}
  return NA_TRUE;
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
