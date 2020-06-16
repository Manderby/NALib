
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"




// ////////////////////////////////////
// RETRIEVING
// ////////////////////////////////////

NA_IDEF int8 naRetrieveBufferi8(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int8 value;
  na_RetrieveBufferBytes(iter, &value, 1, advance);
  naConvertEndianness8(buffer->endianness, &value);
  return value;
}
NA_IDEF int16 naRetrieveBufferi16(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int16 value;
  na_RetrieveBufferBytes(iter, &value, 2, advance);
  naConvertEndianness16(buffer->endianness, &value);
  return value;
}
NA_IDEF int32 naRetrieveBufferi32(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int32 value;
  na_RetrieveBufferBytes(iter, &value, 4, advance);
  naConvertEndianness32(buffer->endianness, &value);
  return value;
}
NA_IDEF NAi64 naRetrieveBufferi64(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NAi64 value;
  na_RetrieveBufferBytes(iter, &value, 8, advance);
  naConvertEndianness64(buffer->endianness, &value);
  return value;
}



NA_IDEF uint8 naRetrieveBufferu8(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint8 value;
  na_RetrieveBufferBytes(iter, &value, 1, advance);
  naConvertEndianness8(buffer->endianness, &value);
  return value;
}
NA_IDEF uint16 naRetrieveBufferu16(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint16 value;
  na_RetrieveBufferBytes(iter, &value, 2, advance);
  naConvertEndianness16(buffer->endianness, &value);
  return value;
}
NA_IDEF uint32 naRetrieveBufferu32(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint32 value;
  na_RetrieveBufferBytes(iter, &value, 4, advance);
  naConvertEndianness32(buffer->endianness, &value);
  return value;
}
NA_IDEF NAu64 naRetrieveBufferu64(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NAu64 value;
  na_RetrieveBufferBytes(iter, &value, 8, advance);
  naConvertEndianness64(buffer->endianness, &value);
  return value;
}



NA_IDEF float naRetrieveBufferf(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  float value;
  na_RetrieveBufferBytes(iter, &value, 4, advance);
  naConvertEndianness32(buffer->endianness, &value);
  return value;
}
NA_IDEF double naRetrieveBufferd(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  double value;
  na_RetrieveBufferBytes(iter, &value, 8, advance);
  naConvertEndianness64(buffer->endianness, &value);
  return value;
}



// ///////////////////////////////////////
// GETTING
// ///////////////////////////////////////

NA_IDEF int8 naGetBufferi8(NABufferIterator* iter){
  return naRetrieveBufferi8(iter, NA_FALSE);
}
NA_IDEF int16 naGetBufferi16(NABufferIterator* iter){
  return naRetrieveBufferi16(iter, NA_FALSE);
}
NA_IDEF int32 naGetBufferi32(NABufferIterator* iter){
  return naRetrieveBufferi32(iter, NA_FALSE);
}
NA_IDEF NAi64 naGetBufferi64(NABufferIterator* iter){
  return naRetrieveBufferi64(iter, NA_FALSE);
}



NA_IDEF uint8 naGetBufferu8(NABufferIterator* iter){
  return naRetrieveBufferu8(iter, NA_FALSE);
}
NA_IDEF uint16 naGetBufferu16(NABufferIterator* iter){
  return naRetrieveBufferu16(iter, NA_FALSE);
}
NA_IDEF uint32 naGetBufferu32(NABufferIterator* iter){
  return naRetrieveBufferu32(iter, NA_FALSE);
}
NA_IDEF NAu64 naGetBufferu64(NABufferIterator* iter){
  return naRetrieveBufferu64(iter, NA_FALSE);
}



NA_IDEF float naGetBufferf(NABufferIterator* iter){
  return naRetrieveBufferf(iter, NA_FALSE);
}
NA_IDEF double naGetBufferd(NABufferIterator* iter){
  return naRetrieveBufferd(iter, NA_FALSE);
}



// ///////////////////////////////////////
// READING
// ///////////////////////////////////////

NA_IDEF int8 naReadBufferi8(NABufferIterator* iter){
  return naRetrieveBufferi8(iter, NA_TRUE);
}
NA_IDEF int16 naReadBufferi16(NABufferIterator* iter){
  return naRetrieveBufferi16(iter, NA_TRUE);
}
NA_IDEF int32 naReadBufferi32(NABufferIterator* iter){
  return naRetrieveBufferi32(iter, NA_TRUE);
}
NA_IDEF NAi64 naReadBufferi64(NABufferIterator* iter){
  return naRetrieveBufferi64(iter, NA_TRUE);
}


NA_IDEF uint8 naReadBufferu8(NABufferIterator* iter){
  return naRetrieveBufferu8(iter, NA_TRUE);
}
NA_IDEF uint16 naReadBufferu16(NABufferIterator* iter){
  return naRetrieveBufferu16(iter, NA_TRUE);
}
NA_IDEF uint32 naReadBufferu32(NABufferIterator* iter){
  return naRetrieveBufferu32(iter, NA_TRUE);
}
NA_IDEF NAu64 naReadBufferu64(NABufferIterator* iter){
  return naRetrieveBufferu64(iter, NA_TRUE);
}


NA_IDEF float naReadBufferf(NABufferIterator* iter){
  return naRetrieveBufferf(iter, NA_TRUE);
}
NA_IDEF double naReadBufferd(NABufferIterator* iter){
  return naRetrieveBufferd(iter, NA_TRUE);
}



// /////////////////////////////////
// MULTI READING
// /////////////////////////////////

NA_IDEF void naReadBufferi8v(NABufferIterator* iter, int8* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 1, NA_TRUE);
  naConvertEndianness8v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferi16v(NABufferIterator* iter, int16* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 2, NA_TRUE);
  naConvertEndianness16v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferi32v(NABufferIterator* iter, int32* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
  naConvertEndianness32v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferi64v(NABufferIterator* iter, NAi64* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
  naConvertEndianness64v(buffer->endianness, dst, (NAUInt)count);
}


NA_IDEF void naReadBufferu8v(NABufferIterator* iter, uint8* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 1, NA_TRUE);
  naConvertEndianness8v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferu16v(NABufferIterator* iter, uint16* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 2, NA_TRUE);
  naConvertEndianness16v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferu32v(NABufferIterator* iter, uint32* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
  naConvertEndianness32v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferu64v(NABufferIterator* iter, NAu64* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
  naConvertEndianness64v(buffer->endianness, dst, (NAUInt)count);
}


NA_IDEF void naReadBufferfv(NABufferIterator* iter, float* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
  naConvertEndianness32v(buffer->endianness, dst, (NAUInt)count);
}
NA_IDEF void naReadBufferdv(NABufferIterator* iter, double* dst, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  na_RetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
  naConvertEndianness64v(buffer->endianness, dst, (NAUInt)count);
}



// /////////////////////////////////
// BLOCK READING
// /////////////////////////////////

NA_IDEF void naReadBufferBytes(NABufferIterator* iter, void* data, NAInt byteSize){
  na_RetrieveBufferBytes(iter, data, byteSize, NA_TRUE);
}



NA_IDEF NABuffer* naReadBufferBuffer(NABufferIterator* iter, NAInt byteSize){
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);
  NAInt abspos = naGetBufferLocation(iter);
  NABuffer* newbuffer = naNewBufferExtraction(buffer, abspos, byteSize);
  naLocateBufferAbsolute(iter, abspos + byteSize);
  return newbuffer;
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
