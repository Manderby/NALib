
// This file contains inline implementations of the file NAKey.h
// Do not include this file directly! It will automatically be included when
// including "NAKey.h"

#include "../../NAMath/NACoord.h"
#include "../../NAUtility/NADateTime.h"



NA_IDEF void NA_KEY_OP(Assign, double)(void* dst, const void* src) {
  *(double*)dst = *(const double*)src;
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, double)(const void* a, const void* b) {
  return *(const double*)a <= *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Smaller, double)(const void* a, const void* b) {
  return *(const double*)a < *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, double)(const void* a, const void* b) {
  return *(const double*)a == *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, double)(const void* a, const void* b) {
  return *(const double*)a >= *(const double*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, double)(const void* 
a, const void* b) {
  return *(const double*)a > *(const double*)b;
}

NA_IDEF void NA_KEY_OP(Assign, float)(void* dst, const void* src) {
  *(float*)dst = *(const float*)src;
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, float)(const void* a, const void* b) {
  return *(const float*)a <= *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Smaller, float)(const void* a, const void* b) {
  return *(const float*)a < *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, float)(const void* a, const void* b) {
  return *(const float*)a == *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, float)(const void* a, const void* b) {
  return *(const float*)a >= *(const float*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, float)(const void* a, const void* b) {
  return *(const float*)a > *(const float*)b;
}



NA_IDEF void NA_KEY_OP(Assign, int32)(void* dst, const void* src) {
  *(int32*)dst = *(const int32*)src;
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, int32)(const void* a, const void* b) {
  return *(const int32*)a <= *(const int32*)b;
}
NA_IDEF NABool NA_KEY_OP(Smaller, int32)(const void* a, const void* b) {
  return *(const int32*)a < *(const int32*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, int32)(const void* a, const void* b) {
  return *(const int32*)a == *(const int32*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, int32)(const void* a, const void* b) {
  return *(const int32*)a >= *(const int32*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, int32)(const void* a, const void* b) {
  return *(const int32*)a > *(const int32*)b;
}



NA_IDEF void NA_KEY_OP(Assign, uint32)(void* dst, const void* src) {
  *(uint32*)dst = *(const uint32*)src;
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, uint32)(const void* a, const void* b) {
  return *(const uint32*)a <= *(const uint32*)b;
}
NA_IDEF NABool NA_KEY_OP(Smaller, uint32)(const void* a, const void* b) {
  return *(const uint32*)a < *(const uint32*)b;
}
NA_IDEF NABool NA_KEY_OP(Equal, uint32)(const void* a, const void* b) {
  return *(const uint32*)a == *(const uint32*)b;
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, uint32)(const void* a, const void* b) {
  return *(const uint32*)a >= *(const uint32*)b;
}
NA_IDEF NABool NA_KEY_OP(Greater, uint32)(const void* a, const void* b) {
  return *(const uint32*)a > *(const uint32*)b;
}



NA_IDEF void NA_KEY_OP(Assign, int64)(void* dst, const void* src) {
  *(int64*)dst = *(const int64*)src;
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, int64)(const void* a, const void* b) {
  return naSmallerEquali64(*(const int64*)a, *(const int64*)b);
}
NA_IDEF NABool NA_KEY_OP(Smaller, int64)(const void* a, const void* b) {
  return naSmalleri64(*(const int64*)a, *(const int64*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, int64)(const void* a, const void* b) {
  return naEquali64(*(const int64*)a, *(const int64*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, int64)(const void* a, const void* b) {
  return naGreaterEquali64(*(const int64*)a, *(const int64*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, int64)(const void* a, const void* b) {
  return naGreateri64(*(const int64*)a, *(const int64*)b);
}



NA_IDEF void NA_KEY_OP(Assign, uint64)(void* dst, const void* src) {
  *(uint64*)dst = *(const uint64*)src;
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, uint64)(const void* a, const void* b) {
  return naSmallerEqualu64(*(const uint64*)a, *(const uint64*)b);
}
NA_IDEF NABool NA_KEY_OP(Smaller, uint64)(const void* a, const void* b) {
  return naSmalleru64(*(const uint64*)a, *(const uint64*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, uint64)(const void* a, const void* b) {
  return naEqualu64(*(const uint64*)a, *(const uint64*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, uint64)(const void* a, const void* b) {
  return naGreaterEqualu64(*(const uint64*)a, *(const uint64*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, uint64)(const void* a, const void* b) {
  return naGreateru64(*(const uint64*)a, *(const uint64*)b);
}



NA_IDEF void NA_KEY_OP(Assign, NAPos)(void* dst, const void* src) {
  naFillPosWithCopy((NAPos*)dst, (const NAPos*)src);
}
NA_IDEF NABool NA_KEY_OP(Smaller, NAPos)(const void* a, const void* b) {
  return naSmallerPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, NAPos)(const void* a, const void* b) {
  return naSmallerEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, NAPos)(const void* a, const void* b) {
  return naEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAPos)(const void* a, const void* b) {
  return naGreaterEqualPos(*(const NAPos*)a, *(const NAPos*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, NAPos)(const void* a, const void* b) {
  return naGreaterPos(*(const NAPos*)a, *(const NAPos*)b);
}



NA_IDEF void NA_KEY_OP(Assign, NAVertex)(void* dst, const void* src) {
  naFillVertexWithCopy((NAVertex*)dst, (const NAVertex*)src);
}
NA_IDEF NABool NA_KEY_OP(Smaller, NAVertex)(const void* a, const void* b) {
  return naSmallerVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, NAVertex)(const void* a, const void* b) {
  return naSmallerEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(Equal, NAVertex)(const void* a, const void* b) {
  return naEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NAVertex)(const void* a, const void* b) {
  return naGreaterEqualVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}
NA_IDEF NABool NA_KEY_OP(Greater, NAVertex)(const void* a, const void* b) {
  return naGreaterVertex(*(const NAVertex*)a, *(const NAVertex*)b);
}



NA_IDEF void NA_KEY_OP(Assign, NADateTime)(void* dst, const void* src) {
  NADateTime* dstDateTime = (NADateTime*)dst; 
  const NADateTime* srcDateTime = (const NADateTime*)src; 
  *dstDateTime = *srcDateTime;
}
NA_IDEF NABool NA_KEY_OP(Smaller, NADateTime)(const void* a, const void* b) {
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naSmalleri64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond < bDateTime->nanoSecond));
}
NA_IDEF NABool NA_KEY_OP(SmallerEqual, NADateTime)(const void* a, const void* b) {
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naSmallerEquali64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond <= bDateTime->nanoSecond));
}
NA_IDEF NABool NA_KEY_OP(Equal, NADateTime)(const void* a, const void* b) {
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond == bDateTime->nanoSecond);
}
NA_IDEF NABool NA_KEY_OP(GreaterEqual, NADateTime)(const void* a, const void* b) {
  const NADateTime* aDateTime = (const NADateTime*)a; 
  const NADateTime* bDateTime = (const NADateTime*)b; 
  return naGreaterEquali64(aDateTime->siSecond, bDateTime->siSecond)
    || (naEquali64(aDateTime->siSecond, bDateTime->siSecond) && (aDateTime->nanoSecond >= bDateTime->nanoSecond));
}
NA_IDEF NABool NA_KEY_OP(Greater, NADateTime)(const void* a, const void* b) {
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
