
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



// ////////////////////////////////////
// STORING
// ////////////////////////////////////

NA_HIDEF void naStoreBufferi8(NABufferIterator* iter, int8 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness8(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 1, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferi16(NABufferIterator* iter, int16 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness16(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 2, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferi32(NABufferIterator* iter, int32 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness32(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 4, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferi64(NABufferIterator* iter, int64 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness64(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 8, NA_TRUE, advance);
}



NA_HIDEF void naStoreBufferu8(NABufferIterator* iter, uint8 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness8(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 1, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferu16(NABufferIterator* iter, uint16 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness16(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 2, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferu32(NABufferIterator* iter, uint32 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness32(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 4, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferu64(NABufferIterator* iter, uint64 value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness64(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 8, NA_TRUE, advance);
}



NA_HIDEF void naStoreBufferf(NABufferIterator* iter, float value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness32(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 4, NA_TRUE, advance);
}
NA_HIDEF void naStoreBufferd(NABufferIterator* iter, double value, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naConvertEndianness64(buffer->endianness, &value);
  naStoreBufferBytes(iter, &value, 8, NA_TRUE, advance);
}



// ////////////////////////////////////
// SETTER
// ////////////////////////////////////

NA_IDEF void naSetBufferi8(NABufferIterator* iter, int8 value){
  naStoreBufferi8(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferi16(NABufferIterator* iter, int16 value){
  naStoreBufferi16(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferi32(NABufferIterator* iter, int32 value){
  naStoreBufferi32(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferi64(NABufferIterator* iter, int64 value){
  naStoreBufferi64(iter, value, NA_FALSE);
}



NA_IDEF void naSetBufferu8(NABufferIterator* iter, uint8 value){
  naStoreBufferu8(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferu16(NABufferIterator* iter, uint16 value){
  naStoreBufferu16(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferu32(NABufferIterator* iter, uint32 value){
  naStoreBufferu32(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferu64(NABufferIterator* iter, uint64 value){
  naStoreBufferu64(iter, value, NA_FALSE);
}



NA_IDEF void naSetBufferf(NABufferIterator* iter, float value){
  naStoreBufferf(iter, value, NA_FALSE);
}
NA_IDEF void naSetBufferd(NABufferIterator* iter, double value){
  naStoreBufferd(iter, value, NA_FALSE);
}



// ////////////////////////////////////
// WRITING
// ////////////////////////////////////

NA_IDEF void naWriteBufferi8(NABufferIterator* iter, int8 value){
  naStoreBufferi8(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferi16(NABufferIterator* iter, int16 value){
  naStoreBufferi16(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferi32(NABufferIterator* iter, int32 value){
  naStoreBufferi32(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferi64(NABufferIterator* iter, int64 value){
  naStoreBufferi64(iter, value, NA_TRUE);
}



NA_IDEF void naWriteBufferu8(NABufferIterator* iter, uint8 value){
  naStoreBufferu8(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferu16(NABufferIterator* iter, uint16 value){
  naStoreBufferu16(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferu32(NABufferIterator* iter, uint32 value){
  naStoreBufferu32(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferu64(NABufferIterator* iter, uint64 value){
  naStoreBufferu64(iter, value, NA_TRUE);
}



NA_IDEF void naWriteBufferf(NABufferIterator* iter, float value){
  naStoreBufferf(iter, value, NA_TRUE);
}
NA_IDEF void naWriteBufferd(NABufferIterator* iter, double value){
  naStoreBufferd(iter, value, NA_TRUE);
}



// ////////////////////////////////////
// BLOCK WRITING
// ////////////////////////////////////

NA_IDEF void naWriteBufferBytes(NABufferIterator* iter, const void* data, NAInt bytesize){
  naStoreBufferBytes(iter, data, bytesize, NA_TRUE, NA_TRUE);
}



// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
