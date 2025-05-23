
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



// ////////////////////////////////////
// STORING
// ////////////////////////////////////

NA_HIDEF void na_StoreBufferi8(NABufferIterator* iter, int8 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness8(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 1, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferi16(NABufferIterator* iter, int16 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness16(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 2, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferi32(NABufferIterator* iter, int32 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness32(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 4, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferi64(NABufferIterator* iter, int64 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness64(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 8, NA_TRUE, advance);
}



NA_HIDEF void na_StoreBufferu8(NABufferIterator* iter, uint8 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness8(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 1, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferu16(NABufferIterator* iter, uint16 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness16(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 2, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferu32(NABufferIterator* iter, uint32 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness32(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 4, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferu64(NABufferIterator* iter, uint64 value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness64(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 8, NA_TRUE, advance);
}



NA_HIDEF void na_StoreBufferf(NABufferIterator* iter, float value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness32(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 4, NA_TRUE, advance);
}
NA_HIDEF void na_StoreBufferd(NABufferIterator* iter, double value, NABool advance) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  naConvertEndianness64(buffer->endianness, &value);
  na_StoreBufferBytes(iter, &value, 8, NA_TRUE, advance);
}



// ////////////////////////////////////
// SETTER
// ////////////////////////////////////

NA_IDEF void naSetBufferi8(NABufferIterator* iter, int8 value) {
  na_StoreBufferi8(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferi16(NABufferIterator* iter, int16 value) {
  na_StoreBufferi16(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferi32(NABufferIterator* iter, int32 value) {
  na_StoreBufferi32(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferi64(NABufferIterator* iter, int64 value) {
  na_StoreBufferi64(iter, value, NA_FALSE);
}



NA_IDEF void naSetBufferu8(NABufferIterator* iter, uint8 value) {
  na_StoreBufferu8(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferu16(NABufferIterator* iter, uint16 value) {
  na_StoreBufferu16(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferu32(NABufferIterator* iter, uint32 value) {
  na_StoreBufferu32(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferu64(NABufferIterator* iter, uint64 value) {
  na_StoreBufferu64(iter, value, NA_FALSE);
}



NA_IDEF void naSetBufferf(NABufferIterator* iter, float value) {
  na_StoreBufferf(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferd(NABufferIterator* iter, double value) {
  na_StoreBufferd(iter, value, NA_FALSE);
}



// ////////////////////////////////////
// WRITING
// ////////////////////////////////////

NA_IDEF void naWriteBufferi8(NABufferIterator* iter, int8 value) {
  na_StoreBufferi8(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferi16(NABufferIterator* iter, int16 value) {
  na_StoreBufferi16(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferi32(NABufferIterator* iter, int32 value) {
  na_StoreBufferi32(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferi64(NABufferIterator* iter, int64 value) {
  na_StoreBufferi64(iter, value, NA_TRUE);
}



NA_IDEF void naWriteBufferu8(NABufferIterator* iter, uint8 value) {
  na_StoreBufferu8(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferu16(NABufferIterator* iter, uint16 value) {
  na_StoreBufferu16(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferu32(NABufferIterator* iter, uint32 value) {
  na_StoreBufferu32(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferu64(NABufferIterator* iter, uint64 value) {
  na_StoreBufferu64(iter, value, NA_TRUE);
}



NA_IDEF void naWriteBufferf(NABufferIterator* iter, float value) {
  na_StoreBufferf(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferd(NABufferIterator* iter, double value) {
  na_StoreBufferd(iter, value, NA_TRUE);
}



// ////////////////////////////////////
// BLOCK WRITING
// ////////////////////////////////////

NA_IDEF void naWriteBufferBytes(NABufferIterator* iter, const void* data, size_t byteSize) {
  na_StoreBufferBytes(iter, data, byteSize, NA_TRUE, NA_TRUE);
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
