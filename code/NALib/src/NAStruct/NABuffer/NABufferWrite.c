
#include "../../NABuffer.h"


// This is the internal function actually preparing and storing the bytes
// delivered in the parameters.
NA_HDEF void na_StoreBufferBytes(NABufferIterator* iter, const void* data, NAInt byteSize, NABool prepare, NABool advance){
  const NABuffer* buffer;
  NAInt firstpartOffset;
  NATreeIterator firstbufiter;
  const NAByte* src = data;

  #ifndef NDEBUG
    if(!data)
      naError("data is Null pointer.");
    if(naGetBufferCurBit(iter) != 0)
      naError("Bit offset not 0.");
  #endif

  buffer = na_GetBufferIteratorBufferConst(iter);
  
  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  if(prepare){
    na_PrepareBuffer(iter, byteSize);
  }
  // After this function, all relevant parts should be present and filled with
  // memory. The iterator should point to the buffer part containing offset.

  // We store the current iterator to move back to it later on if necessary.
  firstpartOffset = iter->partOffset;
  firstbufiter = naMakeTreeAccessor(&(buffer->parts));
  naLocateTreeIterator(&firstbufiter, &(iter->partIter));

  // do as long as there is a byteSize remaining. Remember that the data may
  // be split into different buffer parts.
  while(byteSize){
    NABufferPart* part;
    NAInt possiblelength;
    void* dst;

    #ifndef NDEBUG
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
    possiblelength = na_GetBufferPartByteSize(part) - iter->partOffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("possible length invalid");
    #endif

    if(possiblelength > byteSize){
      // If we can put in more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      possiblelength = byteSize;
      iter->partOffset += byteSize;
    }else{
      // We copy as many bytes as possible and advance to the next part.
      na_LocateBufferNextPart(iter);
    }
    naCopyn(dst, src, possiblelength);
    src += possiblelength;
    byteSize -= possiblelength;
  }
  
  if(!advance){
    iter->partOffset = firstpartOffset;
    naLocateTreeIterator(&(iter->partIter), &firstbufiter);
  }
  naClearTreeIterator(&firstbufiter);  
}



// ////////////////////////////////////
// MULTI WRITING
// ////////////////////////////////////

NA_DEF void naWriteBufferi8v(NABufferIterator* iter, const int8* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int8 value;
  na_PrepareBuffer(iter, count * 1);
  while(count){
    value = *src;
    naConvertEndianness8(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi16v(NABufferIterator* iter, const int16* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int16 value;
  na_PrepareBuffer(iter, count * 2);
  while(count){
    value = *src;
    naConvertEndianness16(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi32v(NABufferIterator* iter, const int32* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  int32 value;
  na_PrepareBuffer(iter, count * 4);
  while(count){
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi64v(NABufferIterator* iter, const NAi64* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NAi64 value;
  na_PrepareBuffer(iter, count * 8);
  while(count){
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferu8v(NABufferIterator* iter, const uint8* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint8 value;
  na_PrepareBuffer(iter, count * 1);
  while(count){
    value = *src;
    naConvertEndianness8(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu16v(NABufferIterator* iter, const uint16* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint16 value;
  na_PrepareBuffer(iter, count * 2);
  while(count){
    value = *src;
    naConvertEndianness16(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu32v(NABufferIterator* iter, const uint32* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  uint32 value;
  na_PrepareBuffer(iter, count * 4);
  while(count){
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu64v(NABufferIterator* iter, const NAu64* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NAu64 value;
  na_PrepareBuffer(iter, count * 8);
  while(count){
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferfv(NABufferIterator* iter, const float* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  float value;
  na_PrepareBuffer(iter, count * 4);
  while(count){
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    na_StoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferdv(NABufferIterator* iter, const double* src, NAInt count){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  double value;
  na_PrepareBuffer(iter, count * 8);
  while(count){
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

NA_DEF void naWriteBufferBuffer(NABufferIterator* iter, const NABuffer* srcBuffer, NARangei srcRange){
  if(!naIsRangeiEmpty(srcRange)){
    NABuffer* dstbuffer;
    NABufferSource* tmpsource;
    NAInt tmpsourceOffset;
    NABuffer* mutablesrcBuffer;
    NAInt curPos;

    dstbuffer = na_GetBufferIteratorBufferMutable(iter);
    curPos = naGetBufferLocation(iter);
    mutablesrcBuffer = (NABuffer*)srcBuffer;

    tmpsource = dstbuffer->source;
    tmpsourceOffset = dstbuffer->sourceOffset;
    dstbuffer->source = naNewBufferSource(NA_NULL, mutablesrcBuffer);
    dstbuffer->sourceOffset = srcRange.origin - curPos;

    naCacheBufferRange(dstbuffer, naMakeRangei(curPos, srcRange.length));
    naLocateBufferAbsolute(iter, curPos + srcRange.length);

    naRelease(dstbuffer->source);
    dstbuffer->source = tmpsource;
    dstbuffer->sourceOffset = tmpsourceOffset;
  }
}



NA_DEF void naRepeatBufferBytes(NABufferIterator* iter, NAInt distance, NAInt byteSize, NABool useCopy){
  NABufferIterator readiter;
  NABuffer* buffer;
  NAByte* bufptr;

  // Create the read iterator
  NAInt writeoffset = naGetBufferLocation(iter);
  buffer = na_GetBufferIteratorBufferMutable(iter);
  readiter = naMakeBufferAccessor(buffer);
  naLocateBufferAbsolute(&readiter, writeoffset - distance);

  if(useCopy){
    NAInt remainingbyteSize;
    NABuffer* tmpbuffer;
    
    NAByte* buf = naMalloc(byteSize);
    
    NAInt segmentsize = naMini(distance, byteSize);
    naReadBufferBytes(&readiter, buf, segmentsize);
    remainingbyteSize = byteSize - segmentsize;
    
    bufptr = &(buf[segmentsize]);
    while(remainingbyteSize){
      *bufptr = bufptr[-segmentsize]; //todo
      remainingbyteSize--;
      bufptr++;
    }

    tmpbuffer = naNewBufferWithMutableData(buf, byteSize, naFree);
    naWriteBufferBuffer(iter, tmpbuffer, naMakeRangei(0, byteSize));
    naRelease(tmpbuffer);

  }else{
    // Now start copying the buffers.
    while(byteSize){
      NAInt remainingwrite;
      NAInt remainingread;
      NABufferPart* writepart;
      const NABufferPart* readpart;
      const void* src;
      void* dst;

      // Prepare the two iterators
      na_PrepareBuffer(iter, byteSize);
      na_PrepareBuffer(&readiter, byteSize);

      readpart = na_GetBufferPart(&readiter);
      writepart = na_GetBufferPart(iter);

      remainingread = na_GetBufferPartByteSize(readpart) - readiter.partOffset;
      remainingwrite = na_GetBufferPartByteSize(writepart) - iter->partOffset;

      // We reduce the remainingread such that it does not overflow either the
      // distance, the remainingwrite or the byteSize.
      remainingread = naMini(remainingread, distance);
      remainingread = naMini(remainingread, remainingwrite);
      remainingread = naMini(remainingread, byteSize);

      src = na_GetBufferPartDataPointerConst(&readiter);
      dst = na_GetBufferPartDataPointerMutable(iter);
      naCopyn(dst, src, remainingread);
      byteSize -= remainingread;
      iter->partOffset += remainingread;
      readiter.partOffset += remainingread;
    }

  }
  naClearBufferIterator(&readiter);
}



// ////////////////////////////////////
// STRING BUFFER WRITING
// ////////////////////////////////////

// Note that string buffer writing are not inlined in order to prevent circular
// dependencies

NA_DEF void naWriteBufferTab(NABufferIterator* iter){
  na_StoreBufferBytes(iter, NA_TAB, 1, NA_TRUE, NA_TRUE);
}



NA_DEF void naWriteBufferNewLine(NABufferIterator* iter){
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  switch(buffer->newlineEncoding){
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



NA_DEF void naWriteBufferString(NABufferIterator* iter, const NAString* string){
  naWriteBufferBuffer(iter, naGetStringBufferConst(string), naGetBufferRange(naGetStringBufferConst(string)));
}
NA_DEF void naWriteBufferLine(NABufferIterator* iter, const NAString* string){
  naWriteBufferString(iter, string);
  naWriteBufferNewLine(iter);
}



NA_DEF void naWriteBufferStringWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...){
  va_list argumentList;
  va_start(argumentList, format);
  naWriteBufferStringWithArguments(iter, format, argumentList);
  va_end(argumentList);
}
NA_DEF void naWriteBufferLineWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...){
  va_list argumentList;
  va_start(argumentList, format);
  naWriteBufferLineWithArguments(iter, format, argumentList);
  va_end(argumentList);
}



NA_DEF void naWriteBufferStringWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentList){
  NAString* string;
  string = naNewStringWithArguments(format, argumentList);
  naWriteBufferString(iter, string);
  naDelete(string);
}
NA_DEF void naWriteBufferLineWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentList){
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
