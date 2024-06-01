
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



// The implementation of NABounds is deprectated. Use NABezel instead.
//
// The reason for the deprecation is that the sequence of the values has been
// clockwise. An anti-clockwise fashion is much more in line of the concept of
// NALib. Furthermore, NABounds started with "top", whereas NABezel starts with
// "right" like in a right handed coordinate system.



NA_IDEF NABounds4 naMakeBounds4(double top, double right, double bottom, double left) {
  NABounds4 newBounds;
  #if NA_DEBUG
    naError("NABounds4 is deprecated. Use NABezel4.");
    if(!naIsLengthValueValid(top) || !naIsLengthValueValid(right) || !naIsLengthValueValid(bottom) || !naIsLengthValueValid(left))
      naError("Invalid values given.");
  #endif
  newBounds.top = top;
  newBounds.right = right;
  newBounds.bottom = bottom;
  newBounds.left = left;
  return newBounds;
}

NA_IDEF NABounds4i naMakeBounds4i(NAInt top, NAInt right, NAInt bottom, NAInt left) {
  NABounds4i newBounds;
  #if NA_DEBUG
    naError("NABounds4i is deprecated. Use NABezel4i.");
    if(!naIsLengthValueValidi(top) || !naIsLengthValueValidi(right) || !naIsLengthValueValidi(bottom) || !naIsLengthValueValidi(left))
      naError("Invalid values given.");
  #endif
  newBounds.top = top;
  newBounds.right = right;
  newBounds.bottom = bottom;
  newBounds.left = left;
  return newBounds;
}

NA_IDEF NABounds4 naMakeBounds4WithBounds4i(NABounds4i bounds) {
  NABounds4 newBounds = naMakeBounds4((double)bounds.top, (double)bounds.right, (double)bounds.bottom, (double)bounds.left);
  #if NA_DEBUG
    naError("NABounds4 is deprecated. Use NABezel4.");
  #endif
  return newBounds;
}

NA_IDEF NABounds4i naMakeBounds4iWithBounds4(NABounds4 bounds) {
  NABounds4i newBounds = naMakeBounds4i((NAInt)bounds.top, (NAInt)bounds.right, (NAInt)bounds.bottom, (NAInt)bounds.left);
  #if NA_DEBUG
    naError("NABounds4i is deprecated. Use NABezel4i.");
  #endif
  return newBounds;
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
