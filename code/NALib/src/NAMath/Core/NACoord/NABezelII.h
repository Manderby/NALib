
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



// The implementation of NABounds is deprectated. Use NABezel instead.
//
// The reason for the deprecation is that the sequence of the values has been
// clockwise. An anti-clockwise fashion is much more in line of the concept of
// NALib. Furthermore, NABounds started with "top", whereas NABezel starts with
// "right" like in a right handed coordinate system.



NA_IDEF NABezel4 naMakeBezel4(double right, double top, double left, double bottom) {
  NABezel4 newBezel;
  #if NA_DEBUG
    if(!naIsLengthValueValid(right) || !naIsLengthValueValid(top) || !naIsLengthValueValid(left) || !naIsLengthValueValid(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4f naMakeBezel4f(float right, float top, float left, float bottom) {
  NABezel4f newBezel;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(right) || !naIsLengthValueValidf(top) || !naIsLengthValueValidf(left) || !naIsLengthValueValidf(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4i32 naMakeBezel4i32(int32 right, int32 top, int32 left, int32 bottom) {
  NABezel4i32 newBezel;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(right) || !naIsLengthValueValidi32(top) || !naIsLengthValueValidi32(left) || !naIsLengthValueValidi32(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4i64 naMakeBezel4i64(int64 right, int64 top, int64 left, int64 bottom) {
  NABezel4i64 newBezel;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(right) || !naIsLengthValueValidi64(top) || !naIsLengthValueValidi64(left) || !naIsLengthValueValidi64(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4s naMakeBezel4s(size_t right, size_t top, size_t left, size_t bottom) {
  NABezel4s newBezel;
  #if NA_DEBUG
    if(!naIsLengthValueValids(right) || !naIsLengthValueValids(top) || !naIsLengthValueValids(left) || !naIsLengthValueValids(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}



NA_IDEF NABezel4 naMakeBezel4Zero() {
  NABezel4 newBezel = naMakeBezel4(0., 0., 0., 0.);
  return newBezel;
}

NA_IDEF NABezel4f naMakeBezel4fZero() {
  NABezel4f newBezel = naMakeBezel4f(0.f, 0.f, 0.f, 0.f);
  return newBezel;
}

NA_IDEF NABezel4i32 naMakeBezel4i32Zero() {
  NABezel4i32 newBezel = naMakeBezel4i32(0, 0, 0, 0);
  return newBezel;
}

NA_IDEF NABezel4i64 naMakeBezel4i64Zero() {
  NABezel4i64 newBezel = naMakeBezel4i64(0, 0, 0, 0);
  return newBezel;
}

NA_IDEF NABezel4s naMakeBezel4sZero() {
  NABezel4s newBezel = naMakeBezel4s(0, 0, 0, 0);
  return newBezel;
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
