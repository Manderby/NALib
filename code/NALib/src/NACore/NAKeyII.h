
// This file contains inline implementations of the file NAKey.h
// Do not include this file directly! It will automatically be included when
// including "NAKey.h"

#include "NACoord.h"
#include "NADateTime.h"



NA_IDEF void NA_KEY_OP(Assign, double)(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
}
NA_IDEF NABool NA_KEY_OP(LessEqual, double)(const void* a, const void* b){
  return *(const double*)a <= *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Less, double)(const void* a, const void* b){
  return *(const double*)a < *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, double)(const void* a, const void* b){
  return *(const double*)a == *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, double)(const void* a, const void* b){
  return *(const double*)a >= *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, double)(const void* 
a, const void* b){
  return *(const double*)a > *(const double*)b;
}

NA_IDEF void NA_KEY_OP(Assign, float)(void* dst, const void* src){
  *(float*)dst = *(const float*)src;
}
NA_IDEF NABool NA_KEY_OP(LessEqual, float)(const void* a, const void* b){
  return *(const float*)a <= *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Less, float)(const void* a, const void* b){
  return *(const float*)a < *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, float)(const void* a, const void* b){
  return *(const float*)a == *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, float)(const void* a, const void* b){
  return *(const float*)a >= *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, float)(const void* a, const void* b){
  return *(const float*)a > *(const float*)b;
}



NA_IDEF void NA_KEY_OP(Assign, NAInt)(void* dst, const void* src){
  *(NAInt*)dst = *(const NAInt*)src;
}
NA_IDEF NABool NA_KEY_OP(LessEqual, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a <= *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(Less, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a < *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a == *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a >= *(const NAInt*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, NAInt)(const void* a, const void* b){
  return *(const NAInt*)a > *(const NAInt*)b;
}



NA_IDEF void NA_KEY_OP(Assign, NAPos)(void* dst, const void* src){
  naCopyPos((NAPos*)dst, (const NAPos*)src);
}
NA_IDEF NABool NA_KEY_OP(Less, NAPos)(const void* a, const void* b){
  return naLessPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(LessEqual, NAPos)(const void* a, const void* b){
  return naLessEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, NAPos)(const void* a, const void* b){
  return naEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAPos)(const void* a, const void* b){
  return naGreaterEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, NAPos)(const void* a, const void* b){
  return naGreaterPos(*(const NAPos*)a, *(const NAPos*)b);
}



NA_IDEF void NA_KEY_OP(Assign, NAVertex)(void* dst, const void* src){
  naCopyVertex((NAVertex*)dst, (const NAVertex*)src);
}
NA_IDEF NABool NA_KEY_OP(Less, NAVertex)(const void* a, const void* b){
  return naLessVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(LessEqual, NAVertex)(const void* a, const void* b){
  return naLessEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, NAVertex)(const void* a, const void* b){
  return naEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAVertex)(const void* a, const void* b){
  return naGreaterEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, NAVertex)(const void* a, const void* b){
  return naGreaterVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}



NA_IDEF void NA_KEY_OP(Assign, NADateTime)(void* dst, const void* src){
  NADateTime* dstDateTime = (NADateTime*)dst; 
  const NADateTime* srcDateTime = (const NADateTime*)src; 
  *dstDateTime = *srcDateTime;
}
NA_IDEF NABool NA_KEY_OP(Less, NADateTime)(const void* a, const void* b){
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naSmalleri64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond < bDateTime->nanoSecond));
}
NA_IDEF NABool NA_KEY_OP(LessEqual, NADateTime)(const void* a, const void* b){
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naSmallerEquali64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond <= bDateTime->nanoSecond));
}
NA_IDEF NABool NA_KEY_OP(Equal, NADateTime)(const void* a, const void* b){
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond == bDateTime->nanoSecond);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NADateTime)(const void* a, const void* b){
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naGreaterEquali64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond >= bDateTime->nanoSecond));
}
NA_IDEF NABool NA_KEY_OP(Greater, NADateTime)(const void* a, const void* b){
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naGreateri64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond > bDateTime->nanoSecond));
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
