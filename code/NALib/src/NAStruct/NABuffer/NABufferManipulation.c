
#include "../../NABuffer.h"

// /////////////////////////////////////
// Whole Buffer Functions
// /////////////////////////////////////

NA_DEF NAString* naNewStringWithBufferBase64Encoded(NABuffer* buffer, NABool appendEndSign){
  NAInt totalbyteSize;
  NAInt triples;
  NAInt sizeremainder;
  NABuffer* dstbuffer;
  NABufferIterator srcIter;
  NABufferIterator dstIter;
  NAByte      srctriple[3];
  NAUTF8Char  dsttriple[4];
  NAString* retString;
  #if NA_DEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("Buffer has no determined range. Use naFixBufferRange");
  #endif
  totalbyteSize = buffer->range.length;
  triples = totalbyteSize / 3;
  sizeremainder = totalbyteSize % 3;

  dstbuffer = naNewBuffer(NA_FALSE);
  dstIter = naMakeBufferModifier(dstbuffer);
  srcIter = naMakeBufferMutator(buffer);

  while(triples){
    naReadBufferBytes(&srcIter, srctriple, 3);
    dsttriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
    dsttriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4) | (NAUTF8Char)(srctriple[1] >> 4);
    dsttriple[2] = (NAUTF8Char)((srctriple[1] & 0x0f) << 2) | (NAUTF8Char)(srctriple[2] >> 6);
    dsttriple[3] = (NAUTF8Char) (srctriple[2] & 0x3f);
    naWriteBufferBytes(&dstIter, dsttriple, 4);
    triples--;
  }
  if(sizeremainder == 1){
    naReadBufferBytes(&srcIter, srctriple, 1);
    dsttriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
    dsttriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4);
    naWriteBufferBytes(&dstIter, dsttriple, 2);
  }
  if(sizeremainder == 2){
    naReadBufferBytes(&srcIter, srctriple, 2);
    dsttriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
    dsttriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4) | (NAUTF8Char)(srctriple[1] >> 4);
    dsttriple[2] = (NAUTF8Char)((srctriple[1] & 0x0f) << 2);
    naWriteBufferBytes(&dstIter, dsttriple, 3);
  }

  naClearBufferIterator(&srcIter);
  naClearBufferIterator(&dstIter);

  dstIter = naMakeBufferModifier(dstbuffer);
  while(!naIsBufferAtEnd(&dstIter)){
    NAUTF8Char curchar = (NAUTF8Char)naGetBufferu8(&dstIter);
    NAUTF8Char newChar;
    if      (curchar < 26) {newChar = curchar + 'A';}
    else if (curchar < 52) {newChar = curchar + ('a' - 26);}
    else if (curchar < 62) {newChar = curchar + ('0' - 52);}
    else if (curchar == 62){newChar = '+';}
    else                   {newChar = '/';}
    naWriteBufferu8(&dstIter, (uint8)newChar);
  }
  if(appendEndSign && sizeremainder == 1){
    naWriteBufferu8(&dstIter, '=');
    naWriteBufferu8(&dstIter, '=');
  }
  if(appendEndSign && sizeremainder == 2){
    naWriteBufferu8(&dstIter, '=');
  }

  naClearBufferIterator(&dstIter);
  retString = naNewStringWithBufferExtraction(dstbuffer, dstbuffer->range);
  naRelease(dstbuffer);
  return retString;
}



NA_DEF NABuffer* naNewBufferWithStringBase64Decoded(NAString* string){
  NAInt totalcharsize;
  NAInt triples;
  NAInt sizeremainder;
  NABuffer* dstbuffer;
  NABufferIterator dstIter;
  NAUTF8Char  asctriple[4];
  NAByte      dsttriple[3];
  NABufferIterator srcIter = naMakeBufferMutator(naGetStringBufferMutable(string));
  NABuffer* ascbuffer = naNewBuffer(NA_FALSE);
  NABufferIterator asciter = naMakeBufferModifier(ascbuffer);

  while(!naIsBufferAtEnd(&srcIter)){
    NAUTF8Char curchar = (NAUTF8Char)naReadBufferu8(&srcIter);
    NAUTF8Char newChar;
    if      (curchar == '+'){newChar = 62;}
    else if (curchar == '/'){newChar = 63;}
    else if (curchar <= '9'){newChar = curchar - ('0' - 52);}
    else if (curchar == '='){break;}
    else if (curchar <= 'Z'){newChar = curchar - 'A';}
    else if (curchar <= 'z'){newChar = curchar - ('a' - 26);}
    else{
      #if NA_DEBUG
        naError("Invalid character. This does not seem to be a Base64 encoding");
      #endif
      newChar = '\0';
    }
    naWriteBufferu8(&asciter, (uint8)newChar);
  }

  naClearBufferIterator(&srcIter);
  naClearBufferIterator(&asciter);

  totalcharsize = ascbuffer->range.length;
  triples = totalcharsize / 4;
  sizeremainder = totalcharsize % 4;

  dstbuffer = naNewBuffer(NA_FALSE);
  dstIter = naMakeBufferModifier(dstbuffer);
  asciter = naMakeBufferMutator(ascbuffer);

  while(triples){
    naReadBufferBytes(&asciter, asctriple, 4);
    dsttriple[0] = (NAByte) (asctriple[0] << 2)         | (NAByte)(asctriple[1] >> 4);
    dsttriple[1] = (NAByte)((asctriple[1] & 0x0f) << 4) | (NAByte)(asctriple[2] >> 2);
    dsttriple[2] = (NAByte)((asctriple[2] & 0x03) << 6) | (NAByte)(asctriple[3]);
    triples--;
    naWriteBufferBytes(&dstIter, dsttriple, 3);
  }
  #if NA_DEBUG
  if(sizeremainder == 1)
    naError("This sizeremainder should not happen");
  #endif
  if(sizeremainder == 2){
    naReadBufferBytes(&asciter, asctriple, 2);
    dsttriple[0] = (NAByte) (asctriple[0] << 2)         | (NAByte)(asctriple[1] >> 4);
    #if NA_DEBUG
    if((asctriple[1] & 0x0f) << 4)
      naError("Security breach: Data in unobserved bits of second character");
    #endif
    naWriteBufferBytes(&dstIter, dsttriple, 1);
  }
  if(sizeremainder == 3){
    naReadBufferBytes(&asciter, asctriple, 3);
    dsttriple[0] = (NAByte) (asctriple[0] << 2)         | (NAByte)(asctriple[1] >> 4);
    dsttriple[1] = (NAByte)((asctriple[1] & 0x0f) << 4) | (NAByte)(asctriple[2] >> 2);
    #if NA_DEBUG
    if((asctriple[2] & 0x03) << 6)
      naError("Security breach: Data in unobserved bits of third character");
    #endif
    naWriteBufferBytes(&dstIter, dsttriple, 2);
  }

  naClearBufferIterator(&asciter);
  naClearBufferIterator(&dstIter);
  naRelease(ascbuffer);
  return dstbuffer;
}



NA_DEF void naAccumulateChecksumBuffer(NAChecksum* checksum, NABuffer* buffer){
  size_t byteSize = (size_t)buffer->range.length;
  NABufferIterator iter = naMakeBufferModifier(buffer);
  na_LocateBufferStart(&iter);

  while(byteSize){
    #if NA_DEBUG
      if(na_IsBufferIteratorSparse(&iter))
        naError("Buffer contains sparse parts. Can not compute checksum. Use naCacheBuffer.");
    #endif

    size_t remainingBytes = na_GetBufferPartByteSize(na_GetBufferPart(&iter));
    const void* src = na_GetBufferPartDataPointerConst(&iter);

    if(byteSize > remainingBytes){
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



NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file){
  NAInt byteSize;
  NABufferIterator iter;

  #if NA_DEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("Buffer has no determined range. Use naFixBufferRange");
  #endif

  byteSize = buffer->range.length;
  if(byteSize){
    iter = naMakeBufferAccessor(buffer);
    na_LocateBufferStart(&iter);

    while(byteSize){
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



NA_DEF void naWriteBufferToData(NABuffer* buffer, void* data){
  naCacheBufferRange(buffer, naGetBufferRange(buffer));
  NABufferIterator iter = naMakeBufferModifier(buffer);

  while(na_IterateBufferPart(&iter)){
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
