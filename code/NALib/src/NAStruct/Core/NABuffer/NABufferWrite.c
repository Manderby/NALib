
#include "../../NABuffer.h"
#include "../../../NAUtility/NAString.h"


// This is the internal function actually preparing and storing the bytes
// delivered in the parameters.
NA_HDEF void na_StoreBufferBytes(NABufferIterator* iter, const void* data, size_t byteSize, NABool prepare, NABool advance) {
  const NABuffer* buffer;
  NATreeIterator firstBufIter;
  const NAByte* src = data;

  #if NA_DEBUG
    if(!data)
      naError("data is Null pointer.");
    if(naGetBufferCurBit(iter) != 0)
      naError("Bit offset not 0.");
  #endif

  buffer = na_GetBufferIteratorBufferConst(iter);
  
  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  if(prepare) {
    na_PrepareBuffer(iter, byteSize);
  }
  // After this function, all relevant parts should be present and filled with
  // memory. The iterator should point to the buffer part containing offset.

  // We store the current iterator to move back to it later on if necessary.
  size_t firstPartOffset = (size_t)iter->partOffset;
  firstBufIter = naMakeTreeAccessor(&buffer->parts);
  naLocateTreeIterator(&firstBufIter, &iter->partIter);

  // do as long as there is a byteSize remaining. Remember that the data may
  // be split into different buffer parts.
  while(byteSize) {
    NABufferPart* part;
    void* dst;

    #if NA_DEBUG
      if(na_IsBufferIteratorSparse(iter))
        naError("Cur part is sparse");
    #endif

    // The part pointed to by the iterator should be the one containing offset.
    part = na_GetBufferPart(iter);

    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.

    // We get the data pointer where we can write bytes.
    dst = na_GetBufferPartDataPointerMutable(iter);
    // We detect, how many bytes actually can be put into the current part.
    size_t possibleLength = na_GetBufferPartByteSize(part) - (size_t)iter->partOffset;

    #if NA_DEBUG
      if(possibleLength <= 0)
        naError("possible length invalid");
    #endif

    if(possibleLength > byteSize) {
      // If we can put in more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      possibleLength = byteSize;
      iter->partOffset += byteSize;
    }else{
      // We copy as many bytes as possible and advance to the next part.
      na_LocateBufferNextPart(iter);
    }
    naCopyn(dst, src, possibleLength);
    src += possibleLength;
    byteSize -= possibleLength;
  }
  
  if(!advance) {
    iter->partOffset = (int64)firstPartOffset;
    naLocateTreeIterator(&iter->partIter, &firstBufIter);
  }
  naClearTreeIterator(&firstBufIter);  
}



// ////////////////////////////////////
// MULTI WRITING
// ////////////////////////////////////

NA_DEF void naWriteBufferi8v(NABufferIterator* iter, const int8* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int8 value;
  na_PrepareBuffer(iter, count * 1);
  while(count) {
    value = *src;
    naConvertEndianness8(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi16v(NABufferIterator* iter, const int16* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int16 value;
  na_PrepareBuffer(iter, count * 2);
  while(count) {
    value = *src;
    naConvertEndianness16(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi32v(NABufferIterator* iter, const int32* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int32 value;
  na_PrepareBuffer(iter, count * 4);
  while(count) {
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi64v(NABufferIterator* iter, const NAi64* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NAi64 value;
  na_PrepareBuffer(iter, count * 8);
  while(count) {
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferu8v(NABufferIterator* iter, const uint8* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint8 value;
  na_PrepareBuffer(iter, count * 1);
  while(count) {
    value = *src;
    naConvertEndianness8(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu16v(NABufferIterator* iter, const uint16* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint16 value;
  na_PrepareBuffer(iter, count * 2);
  while(count) {
    value = *src;
    naConvertEndianness16(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu32v(NABufferIterator* iter, const uint32* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint32 value;
  na_PrepareBuffer(iter, count * 4);
  while(count) {
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu64v(NABufferIterator* iter, const NAu64* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NAu64 value;
  na_PrepareBuffer(iter, count * 8);
  while(count) {
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferfv(NABufferIterator* iter, const float* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  float value;
  na_PrepareBuffer(iter, count * 4);
  while(count) {
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferdv(NABufferIterator* iter, const double* src, size_t count) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  double value;
  na_PrepareBuffer(iter, count * 8);
  while(count) {
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



// ////////////////////////////////////
// BLOCK WRITING
// ////////////////////////////////////

NA_DEF void naWriteBufferBuffer(NABufferIterator* iter, const NABuffer* srcBuffer, NARangei64 srcRange) {
  if(!naIsRangei64Empty(srcRange)) {
    NABuffer* dstBuffer;
    NABufferSource* tmpsource;
    int64 tmpsourceOffset;
    NABuffer* mutableSrcBuffer;
    int64 curPos;

    dstBuffer = na_GetBufferIteratorBufferMutable(iter);
    curPos = naGetBufferLocation(iter);
    mutableSrcBuffer = (NABuffer*)srcBuffer;

    tmpsource = dstBuffer->source;
    tmpsourceOffset = dstBuffer->sourceOffset;
    dstBuffer->source = naCreateBufferSource(NA_NULL, mutableSrcBuffer);
    dstBuffer->sourceOffset = srcRange.origin - curPos;

    naCacheBufferRange(dstBuffer, naMakeRangei64(curPos, srcRange.length));
    naLocateBufferAbsolute(iter, curPos + srcRange.length);

    naRelease(dstBuffer->source);
    dstBuffer->source = tmpsource;
    dstBuffer->sourceOffset = tmpsourceOffset;
  }
}



NA_DEF void naRepeatBufferBytes(NABufferIterator* iter, int64 distance, size_t byteSize, NABool useCopy) {
  NABufferIterator readIter;
  NABuffer* buffer;
  NAByte* bufptr;

  // Create the read iterator
  int64 writeoffset = naGetBufferLocation(iter);
  buffer = na_GetBufferIteratorBufferMutable(iter);
  readIter = naMakeBufferAccessor(buffer);
  naLocateBufferAbsolute(&readIter, writeoffset - distance);

  if(useCopy) {
    size_t remainingByteSize;
    NABuffer* tmpbuffer;
    
    NAByte* buf = naMalloc(byteSize);
    
    int64 segmentSize = naMini64(distance, (int64)byteSize);
    naReadBufferBytes(&readIter, buf, (size_t)segmentSize);
    remainingByteSize = byteSize - (size_t)segmentSize;
    
    bufptr = &buf[segmentSize];
    while(remainingByteSize) {
      *bufptr = bufptr[-segmentSize]; //todo
      remainingByteSize--;
      bufptr++;
    }

    tmpbuffer = naCreateBufferWithMutableData(buf, byteSize, naFree);
    naWriteBufferBuffer(iter, tmpbuffer, naMakeRangei64(0, (int64)byteSize));
    naRelease(tmpbuffer);

  }else{
    // Now start copying the buffers.
    while(byteSize) {
      size_t remainingWrite;
      size_t remainingRead;
      NABufferPart* writepart;
      const NABufferPart* readpart;
      const void* src;
      void* dst;

      // Prepare the two iterators
      na_PrepareBuffer(iter, byteSize);
      na_PrepareBuffer(&readIter, byteSize);

      readpart = na_GetBufferPart(&readIter);
      writepart = na_GetBufferPart(iter);

      remainingRead = na_GetBufferPartByteSize(readpart) - (size_t)readIter.partOffset;
      remainingWrite = na_GetBufferPartByteSize(writepart) - (size_t)iter->partOffset;

      // We reduce the remainingRead such that it does not overflow either the
      // distance, the remainingWrite or the byteSize.
      remainingRead = (size_t)naMini64((int64)remainingRead, distance);
      remainingRead = naMins(remainingRead, remainingWrite);
      remainingRead = naMins(remainingRead, byteSize);

      src = na_GetBufferPartDataPointerConst(&readIter);
      dst = na_GetBufferPartDataPointerMutable(iter);
      naCopyn(dst, src, remainingRead);
      byteSize -= remainingRead;
      iter->partOffset += remainingRead;
      readIter.partOffset += remainingRead;
    }

  }
  naClearBufferIterator(&readIter);
}



// ////////////////////////////////////
// STRING BUFFER WRITING
// ////////////////////////////////////

// Note that string buffer writing are not inlined in order to prevent circular
// dependencies

NA_DEF void naWriteBufferTab(NABufferIterator* iter) {
  na_StoreBufferBytes(iter, NA_TAB, 1, NA_TRUE, NA_TRUE);
}



NA_DEF void naWriteBufferNewLine(NABufferIterator* iter) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  switch(buffer->newlineEncoding) {
  case NA_NEWLINE_UNIX:
    na_StoreBufferBytes(iter, NA_NL_UNIX, 1, NA_TRUE, NA_TRUE);
    break;
  case NA_NEWLINE_MAC9:
    na_StoreBufferBytes(iter, NA_NL_MAC9, 1, NA_TRUE, NA_TRUE);
    break;
  case NA_NEWLINE_WIN:
    na_StoreBufferBytes(iter, NA_NL_WIN, 2, NA_TRUE, NA_TRUE);
    break;
  case NA_NEWLINE_NATIVE:
    na_StoreBufferBytes(iter, NA_NL, naStrlen(NA_NL), NA_TRUE, NA_TRUE);
    break;
  }
}



NA_DEF void naWriteBufferString(NABufferIterator* iter, const NAString* string) {
  naWriteBufferBuffer(iter, naGetStringBufferConst(string), naGetBufferRange(naGetStringBufferConst(string)));
}
NA_DEF void naWriteBufferLine(NABufferIterator* iter, const NAString* string) {
  naWriteBufferString(iter, string);
  naWriteBufferNewLine(iter);
}



NA_DEF void naWriteBufferStringWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...) {
  va_list argumentList;
  va_start(argumentList, format);
  naWriteBufferStringWithArguments(iter, format, argumentList);
  va_end(argumentList);
}
NA_DEF void naWriteBufferLineWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...) {
  va_list argumentList;
  va_start(argumentList, format);
  naWriteBufferLineWithArguments(iter, format, argumentList);
  va_end(argumentList);
}



NA_DEF void naWriteBufferStringWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentList) {
  NAString* string;
  string = naNewStringWithArguments(format, argumentList);
  naWriteBufferString(iter, string);
  naDelete(string);
}
NA_DEF void naWriteBufferLineWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentList) {
  naWriteBufferStringWithArguments(iter, format, argumentList);
  naWriteBufferNewLine(iter);
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
