
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



NA_IDEF int8 naParseBufferi8(NABufferIterator* iter, NABool skipDelimiter) {
  NAi64 intValue;
  naParseBufferDecimalSignedInteger(iter, &intValue, 0, naMakei64WithLo(NA_MIN_i8), naMakei64WithLo(NA_MAX_i8));
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return naCasti64Toi8(intValue);
}
NA_IDEF int16 naParseBufferi16(NABufferIterator* iter, NABool skipDelimiter) {
  NAi64 intValue;
  naParseBufferDecimalSignedInteger(iter, &intValue, 0, naMakei64WithLo(NA_MIN_i16), naMakei64WithLo(NA_MAX_i16));
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return naCasti64Toi16(intValue);
}
NA_IDEF int32 naParseBufferi32(NABufferIterator* iter, NABool skipDelimiter) {
  NAi64 intValue;
  naParseBufferDecimalSignedInteger(iter, &intValue, 0, naMakei64WithLo(NA_MIN_i32), naMakei64WithLo(NA_MAX_i32));
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return naCasti64Toi32(intValue);
}
NA_IDEF NAi64 naParseBufferi64(NABufferIterator* iter, NABool skipDelimiter) {
  NAi64 intValue;
  naParseBufferDecimalSignedInteger(iter, &intValue, 0, NA_MIN_i64, NA_MAX_i64);
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return intValue;
}



NA_IDEF uint8 naParseBufferu8(NABufferIterator* iter, NABool skipDelimiter) {
  NAu64 uintValue;
  naParseBufferDecimalUnsignedInteger(iter, &uintValue, 0, naMakeu64WithLo(NA_MAX_u8));
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return naCastu64Tou8(uintValue);
}
NA_IDEF uint16 naParseBufferu16(NABufferIterator* iter, NABool skipDelimiter) {
  NAu64 uintValue;
  naParseBufferDecimalUnsignedInteger(iter, &uintValue, 0, naMakeu64WithLo(NA_MAX_u16));
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return naCastu64Tou16(uintValue);
}
NA_IDEF uint32 naParseBufferu32(NABufferIterator* iter, NABool skipDelimiter) {
  NAu64 uintValue;
  naParseBufferDecimalUnsignedInteger(iter, &uintValue, 0, naMakeu64WithLo(NA_MAX_u32));
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return naCastu64Tou32(uintValue);
}
NA_IDEF NAu64 naParseBufferu64(NABufferIterator* iter, NABool skipDelimiter) {
  NAu64 uintValue;
  naParseBufferDecimalUnsignedInteger(iter, &uintValue, 0, NA_MAX_u64);
  if(skipDelimiter) {
    naSkipBufferDelimiter(iter);
  }
  return uintValue;
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
