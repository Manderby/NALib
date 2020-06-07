
#include "../../NABuffer.h"



NA_HHDEF void na_RetrieveBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize, NABool advance){
  const NABuffer* buffer;
  NAInt firstpartoffset;
  NAByte* dst = data;
  NATreeIterator firstbufiter;

  #ifndef NDEBUG
    if(!data)
      naError("data is Null pointer.");
    if(naGetBufferCurBit(iter) != 0)
      naError("Bit offset not 0.");
  #endif

  buffer = naGetBufferIteratorBufferConst(iter);

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  na_PrepareBuffer(iter, bytesize);
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
      if(na_IsBufferIteratorSparse(iter))
        naError("Cur part is sparse");
    #endif

    // The part pointed to by the iterator should be the one containing offset.
    part = naGetBufferPart(iter);

    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.

    // We get the data pointer where we can read bytes.
    src = na_GetBufferPartDataPointerConst(iter);
    // We detect, how many bytes actually can be read from the current part.
    possiblelength = na_GetBufferPartByteSize(part) - iter->partoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("possible length invalid");
    #endif

    if(possiblelength > bytesize){
      // If we can get out more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      possiblelength = bytesize;
      iter->partoffset += bytesize;
    }else{
      // We copy as many bytes as possible and advance to the next part.
      na_LocateBufferNextPart(iter);
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



NA_DEF NAByte naGetBufferByteAtIndex(NABuffer* buffer, NAInt index){
  NAByte retbyte;
  NABufferIterator iter;
  NABool found;
  
  #ifndef NDEBUG
    if(buffer->range.origin != 0)
      naError("This function should only be used for buffers with origin 0.");
  #endif

  iter = naMakeBufferAccessor(buffer);  
  found = naLocateBufferAbsolute(&iter, index);
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
    na_PrepareBuffer(iter, 1);
  }

  src = na_GetBufferPartDataPointerConst(iter);
  bit = (*src >> iter->curbit) & 0x01;
  iter->curbit++;

  if(iter->curbit == 8){
    iter->curbit = 0;
    iter->partoffset++;
  }

  return bit;
}



NA_DEF NAUInt naReadBufferBits(NABufferIterator* iter, uint8 count){
  NAUInt retValuei = 0;
  NAUInt curmask = 1;
  #ifndef NDEBUG
    if(count > NA_TYPE_NAINT_BITS)
      naError("Max bit readable per function call exceeded.");
  #endif
  while(count){
    NABool curbit = naReadBufferBit(iter);
    retValuei |= curmask * (NAUInt)curbit;
    curmask <<= 1;
    count--;
  }
  return retValuei;
}



NA_DEF void naPadBufferBits(NABufferIterator* iter){
  if(iter->curbit != 0){
    iter->curbit = 0;
    iter->partoffset++;
  }
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
