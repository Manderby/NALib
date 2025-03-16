
#include "../../NABuffer.h"

// /////////////////////////////////////
// Whole Buffer Functions
// /////////////////////////////////////

NA_DEF NAString* naNewStringWithBufferBase64Encoded(NABuffer* buffer, NABool appendEndSign) {
  int64 totalbyteSize;
  int64 triples;
  int64 sizeRemainder;
  NABuffer* dstBuffer;
  NABufferIterator srcIter;
  NABufferIterator dstIter;
  NAByte      srctriple[3];
  NAUTF8Char  dstTriple[4];
  NAString* retString;
  #if NA_DEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("Buffer has no determined range. Use naFixBufferRange");
  #endif
  totalbyteSize = buffer->range.length;
  triples = totalbyteSize / 3;
  sizeRemainder = totalbyteSize % 3;

  dstBuffer = naCreateBuffer(NA_FALSE);
  dstIter = naMakeBufferModifier(dstBuffer);
  srcIter = naMakeBufferMutator(buffer);

  while(triples) {
    naReadBufferBytes(&srcIter, srctriple, 3);
    dstTriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
    dstTriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4) | (NAUTF8Char)(srctriple[1] >> 4);
    dstTriple[2] = (NAUTF8Char)((srctriple[1] & 0x0f) << 2) | (NAUTF8Char)(srctriple[2] >> 6);
    dstTriple[3] = (NAUTF8Char) (srctriple[2] & 0x3f);
    naWriteBufferBytes(&dstIter, dstTriple, 4);
    triples--;
  }
  if(sizeRemainder == 1) {
    naReadBufferBytes(&srcIter, srctriple, 1);
    dstTriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
    dstTriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4);
    naWriteBufferBytes(&dstIter, dstTriple, 2);
  }
  if(sizeRemainder == 2) {
    naReadBufferBytes(&srcIter, srctriple, 2);
    dstTriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
    dstTriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4) | (NAUTF8Char)(srctriple[1] >> 4);
    dstTriple[2] = (NAUTF8Char)((srctriple[1] & 0x0f) << 2);
    naWriteBufferBytes(&dstIter, dstTriple, 3);
  }

  naClearBufferIterator(&srcIter);
  naClearBufferIterator(&dstIter);

  dstIter = naMakeBufferModifier(dstBuffer);
  while(!naIsBufferAtEnd(&dstIter)) {
    NAUTF8Char curChar = (NAUTF8Char)naGetBufferu8(&dstIter);
    NAUTF8Char newChar;
    if(curChar < 26) { newChar = curChar + 'A'; }
    else if(curChar < 52) { newChar = curChar + ('a' - 26); }
    else if(curChar < 62) { newChar = curChar + ('0' - 52); }
    else if(curChar == 62) { newChar = '+'; }
    else { newChar = '/'; }
    naWriteBufferu8(&dstIter, (uint8)newChar);
  }
  if(appendEndSign && sizeRemainder == 1) {
    naWriteBufferu8(&dstIter, '=');
    naWriteBufferu8(&dstIter, '=');
  }
  if(appendEndSign && sizeRemainder == 2) {
    naWriteBufferu8(&dstIter, '=');
  }

  naClearBufferIterator(&dstIter);
  retString = naNewStringWithBufferExtraction(dstBuffer, dstBuffer->range);
  naRelease(dstBuffer);
  return retString;
}



NA_DEF NABuffer* naCreateBufferWithStringBase64Decoded(NAString* string) {
  int64 totalCharSize;
  int64 triples;
  int64 sizeRemainder;
  NABuffer* dstBuffer;
  NABufferIterator dstIter;
  NAUTF8Char  ascTriple[4];
  NAByte      dstTriple[3];
  NABufferIterator srcIter = naMakeBufferMutator(naGetStringBufferMutable(string));
  NABuffer* ascbuffer = naCreateBuffer(NA_FALSE);
  NABufferIterator ascIter = naMakeBufferModifier(ascbuffer);

  while(!naIsBufferAtEnd(&srcIter)) {
    NAUTF8Char curChar = (NAUTF8Char)naReadBufferu8(&srcIter);
    NAUTF8Char newChar;
    if     (curChar == '+') {
      newChar = 62;
    }else if(curChar == '/') {
      newChar = 63;
    }else if(curChar <= '9') {
      newChar = curChar - ('0' - 52);
    }else if(curChar == '=') {
      break;
    }else if(curChar <= 'Z') {
      newChar = curChar - 'A';
    }else if(curChar <= 'z') {
      newChar = curChar - ('a' - 26);
    }else{
      #if NA_DEBUG
        naError("Invalid character. This does not seem to be a Base64 encoding");
      #endif
      newChar = '\0';
    }
    naWriteBufferu8(&ascIter, (uint8)newChar);
  }

  naClearBufferIterator(&srcIter);
  naClearBufferIterator(&ascIter);

  totalCharSize = ascbuffer->range.length;
  triples = totalCharSize / 4;
  sizeRemainder = totalCharSize % 4;

  dstBuffer = naCreateBuffer(NA_FALSE);
  dstIter = naMakeBufferModifier(dstBuffer);
  ascIter = naMakeBufferMutator(ascbuffer);

  while(triples) {
    naReadBufferBytes(&ascIter, ascTriple, 4);
    dstTriple[0] = (NAByte) (ascTriple[0] << 2)         | (NAByte)(ascTriple[1] >> 4);
    dstTriple[1] = (NAByte)((ascTriple[1] & 0x0f) << 4) | (NAByte)(ascTriple[2] >> 2);
    dstTriple[2] = (NAByte)((ascTriple[2] & 0x03) << 6) | (NAByte)(ascTriple[3]);
    triples--;
    naWriteBufferBytes(&dstIter, dstTriple, 3);
  }
  #if NA_DEBUG
  if(sizeRemainder == 1)
    naError("This sizeRemainder should not happen");
  #endif
  if(sizeRemainder == 2) {
    naReadBufferBytes(&ascIter, ascTriple, 2);
    dstTriple[0] = (NAByte) (ascTriple[0] << 2)         | (NAByte)(ascTriple[1] >> 4);
    #if NA_DEBUG
    if((ascTriple[1] & 0x0f) << 4)
      naError("Security breach: Data in unobserved bits of second character");
    #endif
    naWriteBufferBytes(&dstIter, dstTriple, 1);
  }
  if(sizeRemainder == 3) {
    naReadBufferBytes(&ascIter, ascTriple, 3);
    dstTriple[0] = (NAByte) (ascTriple[0] << 2)         | (NAByte)(ascTriple[1] >> 4);
    dstTriple[1] = (NAByte)((ascTriple[1] & 0x0f) << 4) | (NAByte)(ascTriple[2] >> 2);
    #if NA_DEBUG
    if((ascTriple[2] & 0x03) << 6)
      naError("Security breach: Data in unobserved bits of third character");
    #endif
    naWriteBufferBytes(&dstIter, dstTriple, 2);
  }

  naClearBufferIterator(&ascIter);
  naClearBufferIterator(&dstIter);
  naRelease(ascbuffer);
  return dstBuffer;
}



NA_DEF void naAccumulateChecksumBuffer(NAChecksum* checksum, NABuffer* buffer) {
  size_t byteSize = (size_t)buffer->range.length;
  NABufferIterator iter = naMakeBufferModifier(buffer);
  na_LocateBufferStart(&iter);

  while(byteSize) {
    #if NA_DEBUG
      if(na_IsBufferIteratorSparse(&iter))
        naError("Buffer contains sparse parts. Can not compute checksum. Use naCacheBuffer.");
    #endif

    size_t remainingBytes = na_GetBufferPartByteSize(na_GetBufferPart(&iter));
    const void* src = na_GetBufferPartDataPointerConst(&iter);

    if(byteSize > remainingBytes) {
      naAccumulateChecksum(checksum, src, remainingBytes);
      na_IterateBufferPart(&iter);
      byteSize -= remainingBytes;
    }else{
      naAccumulateChecksum(checksum, src, byteSize);
      byteSize = 0;
    }
  }

  naClearBufferIterator(&iter);
  return;
}



NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file) {
  int64 byteSize;
  NABufferIterator iter;

  #if NA_DEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("Buffer has no determined range. Use naFixBufferRange");
  #endif

  byteSize = buffer->range.length;
  if(byteSize) {
    iter = naMakeBufferAccessor(buffer);
    na_LocateBufferStart(&iter);

    while(byteSize) {
      NABufferPart* part = na_GetBufferPart(&iter);
      size_t remainingBytes = na_GetBufferPartByteSize(part);
      const NAByte* src = na_GetBufferPartDataPointerConst(&iter);

      #if NA_DEBUG
        if(na_IsBufferPartSparse(part))
          naError("Buffer contains sparse parts.");
      #endif

      naWriteFileBytes(file, src, (NAFileSize)remainingBytes);
      na_LocateBufferNextPart(&iter);
      byteSize -= remainingBytes;
    }

    naClearBufferIterator(&iter);
  }
  return;
}



NA_DEF void naWriteBufferToData(NABuffer* buffer, void* data) {
  naCacheBufferRange(buffer, naGetBufferRange(buffer));
  NABufferIterator iter = naMakeBufferModifier(buffer);

  while(na_IterateBufferPart(&iter)) {
    size_t byteSize = na_GetBufferPartByteSize(na_GetBufferPart(&iter));
    const void* src = na_GetBufferPartDataPointerConst(&iter);
    #if NA_DEBUG
      if(na_IsBufferIteratorSparse(&iter))
        naError("Buffer contains sparse parts. Use naCacheBuffer.");
    #endif
    naCopyn(data, src, byteSize);
    data = (NAByte*)data + byteSize;
  }

  naClearBufferIterator(&iter);
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
