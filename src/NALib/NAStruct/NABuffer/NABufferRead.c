
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"


NA_DEF NAByte naGetBufferByteAtIndex(const NABuffer* buffer, NAInt indx){
  #ifndef NDEBUG
    if(buffer->range.origin != 0)
      naError("naGetBufferByteAtIndex", "This function should only be used for buffers with origin 0.");
  #endif

  NAByte retbyte;
  NABufferIterator iter = naMakeBufferAccessor(buffer);  
  NABool found = naLocateBufferAbsolute(&iter, indx);
  if(found){
    retbyte = naGetBufferu8(&iter);
  }else{
    retbyte = '\0';
  }
  naClearBufferIterator(&iter);
  
  return retbyte;
}



// ////////////////////////////////////
// RETRIEVING
// ////////////////////////////////////

NA_DEF int8 naRetrieveBufferi8(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int8 value;
  naRetrieveBufferBytes(iter, &value, 1, NA_TRUE);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF int16 naRetrieveBufferi16(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int16 value;
  naRetrieveBufferBytes(iter, &value, 2, NA_TRUE);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF int32 naRetrieveBufferi32(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int32 value;
  naRetrieveBufferBytes(iter, &value, 4, NA_TRUE);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF int64 naRetrieveBufferi64(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  int64 value;
  naRetrieveBufferBytes(iter, &value, 8, NA_TRUE);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF uint8 naRetrieveBufferu8(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint8 value;
  naRetrieveBufferBytes(iter, &value, 1, advance);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF uint16 naRetrieveBufferu16(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint16 value;
  naRetrieveBufferBytes(iter, &value, 2, advance);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF uint32 naRetrieveBufferu32(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint32 value;
  naRetrieveBufferBytes(iter, &value, 4, advance);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF uint64 naRetrieveBufferu64(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  uint64 value;
  naRetrieveBufferBytes(iter, &value, 8, advance);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF float naRetrieveBufferf(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  float value;
  naRetrieveBufferBytes(iter, &value, 4, advance);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF double naRetrieveBufferd(NABufferIterator* iter, NABool advance){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  double value;
  naRetrieveBufferBytes(iter, &value, 8, advance);
  buffer->converter.convert64(&value);
  return value;
}



// ///////////////////////////////////////
// GETTING
// ///////////////////////////////////////

NA_DEF int8 naGetBufferi8(NABufferIterator* iter){
  return naRetrieveBufferi8(iter, NA_FALSE);
}
NA_DEF int16 naGetBufferi16(NABufferIterator* iter){
  return naRetrieveBufferi16(iter, NA_FALSE);
}
NA_DEF int32 naGetBufferi32(NABufferIterator* iter){
  return naRetrieveBufferi32(iter, NA_FALSE);
}
NA_DEF int64 naGetBufferi64(NABufferIterator* iter){
  return naRetrieveBufferi64(iter, NA_FALSE);
}



NA_DEF uint8 naGetBufferu8(NABufferIterator* iter){
  return naRetrieveBufferu8(iter, NA_FALSE);
}
NA_DEF uint16 naGetBufferu16(NABufferIterator* iter){
  return naRetrieveBufferu16(iter, NA_FALSE);
}
NA_DEF uint32 naGetBufferu32(NABufferIterator* iter){
  return naRetrieveBufferu32(iter, NA_FALSE);
}
NA_DEF uint64 naGetBufferu64(NABufferIterator* iter){
  return naRetrieveBufferu64(iter, NA_FALSE);
}



NA_DEF float naGetBufferf(NABufferIterator* iter){
  return naRetrieveBufferf(iter, NA_FALSE);
}
NA_DEF double naGetBufferd(NABufferIterator* iter){
  return naRetrieveBufferd(iter, NA_FALSE);
}



// ///////////////////////////////////////
// READING
// ///////////////////////////////////////

NA_DEF void naReadBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize){
  naRetrieveBufferBytes(iter, data, bytesize, NA_TRUE);
}



NA_DEF int8 naReadBufferi8(NABufferIterator* iter){
  return naRetrieveBufferi8(iter, NA_TRUE);
}
NA_DEF int16 naReadBufferi16(NABufferIterator* iter){
  return naRetrieveBufferi16(iter, NA_TRUE);
}
NA_DEF int32 naReadBufferi32(NABufferIterator* iter){
  return naRetrieveBufferi32(iter, NA_TRUE);
}
NA_DEF int64 naReadBufferi64(NABufferIterator* iter){
  return naRetrieveBufferi64(iter, NA_TRUE);
}


NA_DEF uint8 naReadBufferu8(NABufferIterator* iter){
  return naRetrieveBufferu8(iter, NA_TRUE);
}
NA_DEF uint16 naReadBufferu16(NABufferIterator* iter){
  return naRetrieveBufferu16(iter, NA_TRUE);
}
NA_DEF uint32 naReadBufferu32(NABufferIterator* iter){
  return naRetrieveBufferu32(iter, NA_TRUE);
}
NA_DEF uint64 naReadBufferu64(NABufferIterator* iter){
  return naRetrieveBufferu64(iter, NA_TRUE);
}


NA_DEF float naReadBufferf(NABufferIterator* iter){
  return naRetrieveBufferf(iter, NA_TRUE);
}
NA_DEF double naReadBufferd(NABufferIterator* iter){
  return naRetrieveBufferd(iter, NA_TRUE);
}



// /////////////////////////////////
// MULTI READING
// /////////////////////////////////

NA_DEF void naReadBufferi8v(NABufferIterator* iter, int8* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 1, NA_TRUE);
  buffer->converter.convert8v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi16v(NABufferIterator* iter, int16* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 2, NA_TRUE);
  buffer->converter.convert16v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi32v(NABufferIterator* iter, int32* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi64v(NABufferIterator* iter, int64* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
  buffer->converter.convert64v(dst, (NAUInt)count);
}


NA_DEF void naReadBufferu8v(NABufferIterator* iter, uint8* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 1, NA_TRUE);
  buffer->converter.convert8v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu16v(NABufferIterator* iter, uint16* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 2, NA_TRUE);
  buffer->converter.convert16v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu32v(NABufferIterator* iter, uint32* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu64v(NABufferIterator* iter, uint64* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
  buffer->converter.convert64v(dst, (NAUInt)count);
}


NA_DEF void naReadBufferfv(NABufferIterator* iter, float* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferdv(NABufferIterator* iter, double* dst, NAInt count){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  naRetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



// /////////////////////////////////
// BIT MANIPULATION
// /////////////////////////////////

NA_DEF NABool naReadBufferBit(NABufferIterator* iter){
  NA_UNUSED(iter);
  const NAByte* src;
  NABool bit;
  NABufferPart* part;

  if(iter->curbit == 0){
    naPrepareBuffer(iter, 1, NA_FALSE);
  }

  part = naGetBufferPart(iter);
  src = naGetBufferPartDataPointerConst(iter);
  bit = (*src >> iter->curbit) & 0x01;
  iter->curbit++;

  if(iter->curbit == 8){
    iter->curbit = 0;
    iter->partoffset++;
  }

  return bit;
}



NA_DEF NAUInt naReadBufferBits(NABufferIterator* iter, uint8 count){
  NAUInt retint = 0;
  NABool curbit;
  NAUInt curmask = 1;
  #ifndef NDEBUG
    if(count > NA_TYPE_NAINT_BITS)
      naError("naReadBufferBits", "Max bit readable per function call exceeded.");
  #endif
  while(count){
    curbit = naReadBufferBit(iter);
    retint |= curmask * (NAUInt)curbit;
    curmask <<= 1;
    count--;
  }
  return retint;
}



NA_DEF void naPadBufferBits(NABufferIterator* iter){
  if(iter->curbit != 0){
    iter->curbit = 0;
    iter->partoffset++;
  }
}



NA_DEF NABuffer* naReadBufferBuffer(NABufferIterator* iter, NAInt bytesize){
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
  NAInt abspos = naGetBufferLocation(iter);
  NABuffer* newbuffer = naNewBufferExtraction(buffer, naMakeRangei(abspos, bytesize));
  naLocateBufferAbsolute(iter, abspos + bytesize);
  return newbuffer;
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