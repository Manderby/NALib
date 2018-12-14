
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NABuffer.h"


// This is the internal function actually preparing and storing the bytes
// delivered in the parameters.
NA_HDEF void naStoreBufferBytes(NABufferIterator* iter, const void* data, NAInt bytesize, NABool prepare, NABool advance){
  const NABuffer* buffer;
  NAInt firstpartoffset;
  NATreeIterator firstbufiter;
  const NAByte* src = data;

  #ifndef NDEBUG
    if(!data)
      naError("naStoreBufferBytes", "data is Null pointer.");
    if(naGetBufferCurBit(iter) != 0)
      naError("naStoreBufferBytes", "Bit offset not 0.");
  #endif

  buffer = naGetBufferIteratorBufferConst(iter);
  
  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  if(prepare){
    naPrepareBuffer(iter, bytesize);
  }
  // After this function, all relevant parts should be present and filled with
  // memory. The iterator should point to the buffer part containing offset.

  // We store the current iterator to move back to it later on if necessary.
  firstpartoffset = iter->partoffset;
  firstbufiter = naMakeTreeAccessor(&(buffer->parts));
  naLocateTreeIterator(&firstbufiter, &(iter->partiter));

  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts.
  while(bytesize){
    NABufferPart* part;
    NAInt possiblelength;
    void* dst;

    #ifndef NDEBUG
      if(naIsBufferIteratorSparse(iter))
        naError("naStoreBufferBytes", "Cur part is sparse");
    #endif

    // The part pointed to by the iterator should be the one containing offset.
    part = naGetBufferPart(iter);

    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.

    // We get the data pointer where we can write bytes.
    dst = naGetBufferPartDataPointerMutable(iter);
    // We detect, how many bytes actually can be put into the current part.
    possiblelength = naGetBufferPartByteSize(part) - iter->partoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("naStoreBufferBytes", "possible length invalid");
    #endif

    if(possiblelength > bytesize){
      // If we can put in more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      possiblelength = bytesize;
      iter->partoffset += bytesize;
    }else{
      // We copy as many bytes as possible and advance to the next part.
      naLocateBufferNextPart(iter);
    }
    naCopyn(dst, src, possiblelength);
    src += possiblelength;
    bytesize -= possiblelength;
  }
  
  if(!advance){
    iter->partoffset = firstpartoffset;
    naLocateTreeIterator(&(iter->partiter), &firstbufiter);
  }
  naClearTreeIterator(&firstbufiter);  
}



// ////////////////////////////////////
// MULTI WRITING
// ////////////////////////////////////

NA_DEF void naWriteBufferi8v(NABufferIterator* iter, const int8* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int8 value;
  naPrepareBuffer(iter, count * 1);
  while(count){
    value = *src;
    naConvertEndianness8(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi16v(NABufferIterator* iter, const int16* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int16 value;
  naPrepareBuffer(iter, count * 2);
  while(count){
    value = *src;
    naConvertEndianness16(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi32v(NABufferIterator* iter, const int32* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int32 value;
  naPrepareBuffer(iter, count * 4);
  while(count){
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi64v(NABufferIterator* iter, const int64* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int64 value;
  naPrepareBuffer(iter, count * 8);
  while(count){
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferu8v(NABufferIterator* iter, const uint8* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint8 value;
  naPrepareBuffer(iter, count * 1);
  while(count){
    value = *src;
    naConvertEndianness8(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu16v(NABufferIterator* iter, const uint16* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint16 value;
  naPrepareBuffer(iter, count * 2);
  while(count){
    value = *src;
    naConvertEndianness16(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu32v(NABufferIterator* iter, const uint32* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint32 value;
  naPrepareBuffer(iter, count * 4);
  while(count){
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu64v(NABufferIterator* iter, const uint64* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint64 value;
  naPrepareBuffer(iter, count * 8);
  while(count){
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferfv(NABufferIterator* iter, const float* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  float value;
  naPrepareBuffer(iter, count * 4);
  while(count){
    value = *src;
    naConvertEndianness32(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferdv(NABufferIterator* iter, const double* src, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  double value;
  naPrepareBuffer(iter, count * 8);
  while(count){
    value = *src;
    naConvertEndianness64(buffer->endianness, &value);
    naStoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
    src++;
    count--;
  }
}



// ////////////////////////////////////
// BLOCK WRITING
// ////////////////////////////////////

NA_DEF void naWriteBufferBuffer(NABufferIterator* iter, const NABuffer* srcbuffer, NARangei srcrange){
  NABuffer* dstbuffer;
  NABufferSource* tmpsource;
  NAInt tmpsourceoffset;
  NABuffer* mutablesrcbuffer;

  if(!naIsRangeiEmpty(srcrange)){
    NAInt curpos;
    dstbuffer = naGetBufferIteratorBufferMutable(iter);
    curpos = naGetBufferLocation(iter);
    mutablesrcbuffer = (NABuffer*)srcbuffer;

    tmpsource = dstbuffer->source;
    tmpsourceoffset = dstbuffer->sourceoffset;
    dstbuffer->source = naNewBufferSource(NA_NULL, mutablesrcbuffer);
    dstbuffer->sourceoffset = srcrange.origin - curpos;

    naCacheBufferRange(dstbuffer, naMakeRangei(curpos, srcrange.length));
    naLocateBufferAbsolute(iter, curpos + srcrange.length);

    naRelease(dstbuffer->source);
    dstbuffer->source = tmpsource;
    dstbuffer->sourceoffset = tmpsourceoffset;
  }
}



NA_DEF void naRepeatBufferBytes(NABufferIterator* iter, NAInt distance, NAInt bytesize){
  NABufferPart* writepart;
  const NABufferPart* readpart;
  NABufferIterator readiter;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
  NAInt writeoffset = naGetBufferLocation(iter);

  // Create the read iterator
  readiter = naMakeBufferAccessor(buffer);
  naLocateBufferAbsolute(&readiter, writeoffset - distance);

  // Now start copying the buffers.
  while(bytesize){
    NAInt remainingwrite;
    NAInt remainingread;
    const void* src;
    void* dst;

    // Prepare the two iterators
    naPrepareBuffer(iter, bytesize);
    naPrepareBuffer(&readiter, bytesize);

    readpart = naGetBufferPart(&readiter);
    writepart = naGetBufferPart(iter);

    remainingread = naGetBufferPartByteSize(readpart) - readiter.partoffset;
    remainingwrite = naGetBufferPartByteSize(writepart) - iter->partoffset;

    // We reduce the remainingread such that it does not overflow either the
    // distance, the remainingwrite or the bytesize.
    remainingread = naMini(remainingread, distance);
    remainingread = naMini(remainingread, remainingwrite);
    remainingread = naMini(remainingread, bytesize);

    src = naGetBufferPartDataPointerConst(&readiter);
    dst = naGetBufferPartDataPointerMutable(iter);
    naCopyn(dst, src, remainingread);
    bytesize -= remainingread;
    iter->partoffset += remainingread;
    readiter.partoffset += remainingread;
  }

  naClearBufferIterator(&readiter);
}



// ////////////////////////////////////
// STRING BUFFER WRITING
// ////////////////////////////////////

// Note that string buffer writing are not inlined in order to prevent circular
// dependencies

NA_DEF void naWriteBufferTab(NABufferIterator* iter){
  naStoreBufferBytes(iter, NA_TAB, 1, NA_TRUE, NA_TRUE);
}



NA_DEF void naWriteBufferNewLine(NABufferIterator* iter){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  switch(buffer->newlineencoding){
  case NA_NEWLINE_UNIX:
    naStoreBufferBytes(iter, NA_NL_UNIX, 1, NA_TRUE, NA_TRUE);
    break;
  case NA_NEWLINE_MAC9:
    naStoreBufferBytes(iter, NA_NL_MAC9, 1, NA_TRUE, NA_TRUE);
    break;
  case NA_NEWLINE_WIN:
    naStoreBufferBytes(iter, NA_NL_WIN, 2, NA_TRUE, NA_TRUE);
    break;
  case NA_NEWLINE_NATIVE:
    naStoreBufferBytes(iter, NA_NL, naStrlen(NA_NL), NA_TRUE, NA_TRUE);
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
  va_list argumentlist;
  va_start(argumentlist, format);
  naWriteBufferStringWithArguments(iter, format, argumentlist);
  va_end(argumentlist);
}
NA_DEF void naWriteBufferLineWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...){
  va_list argumentlist;
  va_start(argumentlist, format);
  naWriteBufferLineWithArguments(iter, format, argumentlist);
  va_end(argumentlist);
}



NA_DEF void naWriteBufferStringWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentlist){
  NAString* string;
  string = naNewStringWithArguments(format, argumentlist);
  naWriteBufferString(iter, string);
  naDelete(string);
}
NA_DEF void naWriteBufferLineWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentlist){
  naWriteBufferStringWithArguments(iter, format, argumentlist);
  naWriteBufferNewLine(iter);
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
