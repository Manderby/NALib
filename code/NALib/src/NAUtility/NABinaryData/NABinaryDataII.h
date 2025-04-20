
// This file contains inline implementations of the file NABinaryData.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryData.h"


struct NAChecksum{
  NAChecksumType type;
  void* data;
};



#include "NAEndiannessII.h"



#include <string.h>


NA_IDEF void naCopy8(void* NA_RESTRICT d, const void* NA_RESTRICT s) {
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer d is nullptr");
    if(!s)
      naCrash("Pointer s is nullptr");
    ptrdiff_t dist = (NAByte*)d - (NAByte*)s;
    if(dist > -1 && dist < 1)
      naError("Restrict pointers overlap.");
  #endif
  *(uint8*)d = *(uint8*)s;
}
NA_IDEF void naCopy16 (void* NA_RESTRICT d, const void* NA_RESTRICT s) {
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer d is nullptr");
    if(!s)
      naCrash("Pointer s is nullptr");
    ptrdiff_t dist = (NAByte*)d - (NAByte*)s;
    if(dist > -2 && dist < 2)
      naError("Restrict pointers overlap.");
  #endif
  *(uint16*)d = *(uint16*)s;
}
NA_IDEF void naCopy32 (void* NA_RESTRICT d, const void* NA_RESTRICT s) {
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer d is nullptr");
    if(!s)
      naCrash("Pointer s is nullptr");
    ptrdiff_t dist = (NAByte*)d - (NAByte*)s;
    if(dist > -4 && dist < 4)
      naError("Restrict pointers overlap.");
  #endif
  *(uint32*)d = *(uint32*)s;
}
NA_IDEF void naCopy64 (void* NA_RESTRICT d, const void* NA_RESTRICT s) {
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer d is nullptr");
    if(!s)
      naCrash("Pointer s is nullptr");
    ptrdiff_t dist = (NAByte*)d - (NAByte*)s;
    if(dist > -8 && dist < 8)
      naError("Restrict pointers overlap.");
  #endif
  *(uint64*)d = *(uint64*)s;
}
NA_IDEF void naCopy128(void* NA_RESTRICT d, const void* NA_RESTRICT s) {
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer d is nullptr");
    if(!s)
      naCrash("Pointer s is nullptr");
    ptrdiff_t dist = (NAByte*)d - (NAByte*)s;
    if(dist > -16 && dist < 16)
      naError("Restrict pointers overlap.");
  #endif
  *(uint64*)d = *(uint64*)s;
  d = ((NAByte*)d) + 8;
  s = ((NAByte*)s) + 8;
  *(uint64*)d = *(uint64*)s;
}

NA_IDEF void naCopyn(void* NA_RESTRICT d, const void* NA_RESTRICT s, size_t byteSize) {
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer d is nullptr");
    if(!s)
      naCrash("Pointer s is nullptr");
    if(byteSize < 1)
      naCrash("count is smaller than 1.");
  #endif
  memcpy(d, s, byteSize);
}




// /////////////////////////////
// Swap multibyte-values
// /////////////////////////////

NA_IDEF void naSwap8(void* NA_RESTRICT a, void* NA_RESTRICT b) {
  #if NA_DEBUG
    if(!a)
      naCrash("Pointer a is nullptr");
    if(!b)
      naCrash("Pointer b is nullptr");
    ptrdiff_t dist = (NAByte*)a - (NAByte*)b;
    if(dist > -1 && dist < 1)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint8*)a ^= *(uint8*)b;
  *(uint8*)b ^= *(uint8*)a;
  *(uint8*)a ^= *(uint8*)b;
}

NA_IDEF void naSwap16(void* NA_RESTRICT a, void* NA_RESTRICT b) {
  #if NA_DEBUG
    if(!a)
      naCrash("Pointer a is nullptr");
    if(!b)
      naCrash("Pointer b is nullptr");
    ptrdiff_t dist = (NAByte*)a - (NAByte*)b;
    if(dist > -2 && dist < 2)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint16*)a ^= *(uint16*)b;
  *(uint16*)b ^= *(uint16*)a;
  *(uint16*)a ^= *(uint16*)b;
}

NA_IDEF void naSwap32(void* NA_RESTRICT a, void* NA_RESTRICT b) {
  #if NA_DEBUG
    if(!a)
      naCrash("Pointer a is nullptr");
    if(!b)
      naCrash("Pointer b is nullptr");
    ptrdiff_t dist = (NAByte*)a - (NAByte*)b;
    if(dist > -4 && dist < 4)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint32*)a ^= *(uint32*)b;
  *(uint32*)b ^= *(uint32*)a;
  *(uint32*)a ^= *(uint32*)b;
}

NA_IDEF void naSwap64(void* NA_RESTRICT a, void* NA_RESTRICT b) {
  #if NA_DEBUG
    if(!a)
      naCrash("Pointer a is nullptr");
    if(!b)
      naCrash("Pointer b is nullptr");
    ptrdiff_t dist = (NAByte*)a - (NAByte*)b;
    if(dist > -8 && dist < 8)
      naError("Restrict pointers overlap.");
  #endif
  // Note: Do not write the following 3 lines as 1 line. The compiler might
  // cache the result of the dereference operators!
  *(uint64*)a = naXoru64(*(uint64*)a, *(uint64*)b);
  *(uint64*)b = naXoru64(*(uint64*)b, *(uint64*)a);
  *(uint64*)a = naXoru64(*(uint64*)a, *(uint64*)b);
}

NA_IDEF void naSwap128(void* NA_RESTRICT a, void* NA_RESTRICT b) {
  #if NA_DEBUG
    if(!a)
      naCrash("Pointer a is nullptr");
    if(!b)
      naCrash("Pointer b is nullptr");
    ptrdiff_t dist = (NAByte*)a - (NAByte*)b;
    if(dist > -16 && dist < 16)
      naError("Restrict pointers overlap.");
  #endif
  naSwap64(a, b);
  a = ((NAByte*)a) + 8;
  b = ((NAByte*)b) + 8;
  naSwap64(a, b);
}



NA_IDEF NABool naEqual8(  void* NA_RESTRICT a, void* NA_RESTRICT b) {
  return (*((uint8*)a) == *((uint8*)b));
}
NA_IDEF NABool naEqual16( void* NA_RESTRICT a, void* NA_RESTRICT b) {
  return (*((uint16*)a) == *((uint16*)b));
}
NA_IDEF NABool naEqual32( void* NA_RESTRICT a, void* NA_RESTRICT b) {
  return (*((uint32*)a) == *((uint32*)b));
}
NA_IDEF NABool naEqual64( void* NA_RESTRICT a, void* NA_RESTRICT b) {
  return naEqualu64(*((uint64*)a), *((uint64*)b));
}
NA_IDEF NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b) {
  return naEqualu128(*((uint128*)a), *((uint128*)b));
}




// ///////////////////////////////////////////////////////////
// Fills all bytes with zero values
// ///////////////////////////////////////////////////////////

NA_IDEF void naZeron(void* d, size_t byteSize) {
  memset(d, 0, byteSize);
}



NA_IDEF void naSetn(void* d, size_t byteSize, NAByte value) {
  memset(d, value, byteSize);
}




// ///////////////////////////////////////////////////////////
// Set multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF void naFill8WithBytes(
  void* d,
  NAByte b0)
{
  NAByte* p;
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)d;
  *p = b0;
}

NA_IDEF void naFill16WithBytes(
  void* d,
  NAByte b0, NAByte b1)
{
  NAByte* p;
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p = b1;
}

NA_IDEF void naFill32WithBytes(
  void* d,
  NAByte b0, NAByte b1, NAByte b2, NAByte b3)
{
  NAByte* p;
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p = b3;
}

NA_IDEF void naFill64WithBytes(void* d,
  NAByte b0, NAByte b1, NAByte b2, NAByte b3,
  NAByte b4, NAByte b5, NAByte b6, NAByte b7)
{
  NAByte* p;
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)d;
  *p++ = b0; *p++ = b1; *p++ = b2; *p++ = b3;
  *p++ = b4; *p++ = b5; *p++ = b6; *p   = b7;
}

NA_IDEF void naFill128WithBytes(void* d,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15)
{
  NAByte* p;
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)d;
  *p++ = b0;  *p++ = b1;  *p++ = b2;  *p++ = b3;
  *p++ = b4;  *p++ = b5;  *p++ = b6;  *p++ = b7;
  *p++ = b8;  *p++ = b9;  *p++ = b10; *p++ = b11;
  *p++ = b12; *p++ = b13; *p++ = b14; *p   = b15;
}

NA_IDEF void naFill256WithBytes(void* d,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15,
  NAByte b16, NAByte b17, NAByte b18, NAByte b19,
  NAByte b20, NAByte b21, NAByte b22, NAByte b23,
  NAByte b24, NAByte b25, NAByte b26, NAByte b27,
  NAByte b28, NAByte b29, NAByte b30, NAByte b31)
{
  NAByte* p;
  #if NA_DEBUG
    if(!d)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)d;
  *p++ = b0;  *p++ = b1;  *p++ = b2;  *p++ = b3;
  *p++ = b4;  *p++ = b5;  *p++ = b6;  *p++ = b7;
  *p++ = b8;  *p++ = b9;  *p++ = b10; *p++ = b11;
  *p++ = b12; *p++ = b13; *p++ = b14; *p++ = b15;
  *p++ = b16; *p++ = b17; *p++ = b18; *p++ = b19;
  *p++ = b20; *p++ = b21; *p++ = b22; *p++ = b23;
  *p++ = b24; *p++ = b25; *p++ = b26; *p++ = b27;
  *p++ = b28; *p++ = b29; *p++ = b30; *p   = b31;
}



// ///////////////////////////////////////////////////////////
// Compare multiple bytes to the contents of a given pointer
// ///////////////////////////////////////////////////////////

NA_IDEF NABool naEqual8WithBytes(
  const void* s,
  NAByte b0)
{
  NAByte* p;
  #if NA_DEBUG
    if(!s)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)s;
  if(*p   != b0) { return NA_FALSE; }
  return NA_TRUE;
}

NA_IDEF NABool naEqual16WithBytes(
  const void* s,
  NAByte b0, NAByte b1)
{
  NAByte* p;
  #if NA_DEBUG
    if(!s)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0) { return NA_FALSE; }
  if(*p   != b1) { return NA_FALSE; }
  return NA_TRUE;
}

NA_IDEF NABool naEqual32WithBytes(
  const void* s,
  NAByte b0, NAByte b1, NAByte b2, NAByte b3)
{
  NAByte* p;
  #if NA_DEBUG
    if(!s)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0) { return NA_FALSE; }
  if(*p++ != b1) { return NA_FALSE; }
  if(*p++ != b2) { return NA_FALSE; }
  if(*p   != b3) { return NA_FALSE; }
  return NA_TRUE;
}

NA_IDEF NABool naEqual64WithBytes(
  const void* s,
  NAByte b0, NAByte b1, NAByte b2, NAByte b3,
  NAByte b4, NAByte b5, NAByte b6, NAByte b7)
{
  NAByte* p;
  #if NA_DEBUG
    if(!s)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)s;
  if(*p++ != b0) { return NA_FALSE; }
  if(*p++ != b1) { return NA_FALSE; }
  if(*p++ != b2) { return NA_FALSE; }
  if(*p++ != b3) { return NA_FALSE; }
  if(*p++ != b4) { return NA_FALSE; }
  if(*p++ != b5) { return NA_FALSE; }
  if(*p++ != b6) { return NA_FALSE; }
  if(*p   != b7) { return NA_FALSE; }
  return NA_TRUE;
}

NA_IDEF NABool naEqual128WithBytes(
  const void* s,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15)
{
  NAByte* p;
  #if NA_DEBUG
    if(!s)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)s;
  if(*p++ !=  b0) { return NA_FALSE; }
  if(*p++ !=  b1) { return NA_FALSE; }
  if(*p++ !=  b2) { return NA_FALSE; }
  if(*p++ !=  b3) { return NA_FALSE; }
  if(*p++ !=  b4) { return NA_FALSE; }
  if(*p++ !=  b5) { return NA_FALSE; }
  if(*p++ !=  b6) { return NA_FALSE; }
  if(*p++ !=  b7) { return NA_FALSE; }
  if(*p++ !=  b8) { return NA_FALSE; }
  if(*p++ !=  b9) { return NA_FALSE; }
  if(*p++ != b10) { return NA_FALSE; }
  if(*p++ != b11) { return NA_FALSE; }
  if(*p++ != b12) { return NA_FALSE; }
  if(*p++ != b13) { return NA_FALSE; }
  if(*p++ != b14) { return NA_FALSE; }
  if(*p   != b15) { return NA_FALSE; }
  return NA_TRUE;
}

NA_IDEF NABool naEqual256WithBytes(
  const void* s,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15,
  NAByte b16, NAByte b17, NAByte b18, NAByte b19,
  NAByte b20, NAByte b21, NAByte b22, NAByte b23,
  NAByte b24, NAByte b25, NAByte b26, NAByte b27,
  NAByte b28, NAByte b29, NAByte b30, NAByte b31)
{
  NAByte* p;
  #if NA_DEBUG
    if(!s)
      naCrash("Pointer is nullptr");
  #endif
  p = (NAByte*)s;
  if(*p++ !=  b0) { return NA_FALSE; }
  if(*p++ !=  b1) { return NA_FALSE; }
  if(*p++ !=  b2) { return NA_FALSE; }
  if(*p++ !=  b3) { return NA_FALSE; }
  if(*p++ !=  b4) { return NA_FALSE; }
  if(*p++ !=  b5) { return NA_FALSE; }
  if(*p++ !=  b6) { return NA_FALSE; }
  if(*p++ !=  b7) { return NA_FALSE; }
  if(*p++ !=  b8) { return NA_FALSE; }
  if(*p++ !=  b9) { return NA_FALSE; }
  if(*p++ != b10) { return NA_FALSE; }
  if(*p++ != b11) { return NA_FALSE; }
  if(*p++ != b12) { return NA_FALSE; }
  if(*p++ != b13) { return NA_FALSE; }
  if(*p++ != b14) { return NA_FALSE; }
  if(*p++ != b15) { return NA_FALSE; }
  if(*p++ != b16) { return NA_FALSE; }
  if(*p++ != b17) { return NA_FALSE; }
  if(*p++ != b18) { return NA_FALSE; }
  if(*p++ != b19) { return NA_FALSE; }
  if(*p++ != b20) { return NA_FALSE; }
  if(*p++ != b21) { return NA_FALSE; }
  if(*p++ != b22) { return NA_FALSE; }
  if(*p++ != b23) { return NA_FALSE; }
  if(*p++ != b24) { return NA_FALSE; }
  if(*p++ != b25) { return NA_FALSE; }
  if(*p++ != b26) { return NA_FALSE; }
  if(*p++ != b27) { return NA_FALSE; }
  if(*p++ != b28) { return NA_FALSE; }
  if(*p++ != b29) { return NA_FALSE; }
  if(*p++ != b30) { return NA_FALSE; }
  if(*p   != b31) { return NA_FALSE; }
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
