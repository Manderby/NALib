
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"



NA_HDEF void naRetrieveBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize, NABool advance){
  const NABuffer* buffer;
  NAInt firstpartoffset;
  NAByte* dst = data;
  NATreeIterator firstbufiter;

  #ifndef NDEBUG
    if(!data)
      naError("naRetrieveBufferBytes", "data is Null pointer.");
    if(naGetBufferCurBit(iter) != 0)
      naError("naRetrieveBufferBytes", "Bit offset not 0.");
  #endif

  buffer = naGetBufferIteratorBufferConst(iter);

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  naPrepareBuffer(iter, bytesize);
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
    const void* src;

    #ifndef NDEBUG
      if(naIsBufferIteratorSparse(iter))
        naError("naRetrieveBufferBytes", "Cur part is sparse");
    #endif

    // The part pointed to by the iterator should be the one containing offset.
    part = naGetBufferPart(iter);

    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.

    // We get the data pointer where we can read bytes.
    src = naGetBufferPartDataPointerConst(iter);
    // We detect, how many bytes actually can be read from the current part.
    possiblelength = naGetBufferPartByteSize(part) - iter->partoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("naRetrieveBufferBytes", "possible length invalid");
    #endif

    if(possiblelength > bytesize){
      // If we can get out more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      possiblelength = bytesize;
      iter->partoffset += bytesize;
    }else{
      // We copy as many bytes as possible and advance to the next part.
      naLocateBufferNextPart(iter);
    }
    naCopyn(dst, src, possiblelength);
    dst += possiblelength;
    bytesize -= possiblelength;
  }
  
  if(!advance){
    iter->partoffset = firstpartoffset;
    naLocateTreeIterator(&(iter->partiter), &firstbufiter);
  }
  naClearTreeIterator(&firstbufiter);  
}



NA_DEF NAByte naGetBufferByteAtIndex(NABuffer* buffer, NAInt indx){
  NAByte retbyte;
  NABufferIterator iter;
  NABool found;
  
  #ifndef NDEBUG
    if(buffer->range.origin != 0)
      naError("naGetBufferByteAtIndex", "This function should only be used for buffers with origin 0.");
  #endif

  iter = naMakeBufferAccessor(buffer);  
  found = naLocateBufferAbsolute(&iter, indx);
  if(found){
    retbyte = naGetBufferu8(&iter);
  }else{
    retbyte = '\0';
  }
  naClearBufferIterator(&iter);
  
  return retbyte;
}




// /////////////////////////////////
// BIT MANIPULATION
// /////////////////////////////////

NA_DEF NABool naReadBufferBit(NABufferIterator* iter){
  const NAByte* src;
  NABool bit;
  NA_UNUSED(iter);

  if(iter->curbit == 0){
    naPrepareBuffer(iter, 1);
  }

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
