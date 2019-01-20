
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



NA_HIDEF const NABuffer* naGetBufferIteratorBufferConst(const NABufferIterator* iter){
  return naGetPtrConst(iter->bufferptr);
}



NA_HIDEF NABuffer* naGetBufferIteratorBufferMutable(NABufferIterator* iter){
  return naGetPtrMutable(iter->bufferptr);
}



NA_IDEF NABool naLocateBufferRelative(NABufferIterator* iter, NAInt offset){
  NAInt abspos = naGetBufferLocation(iter);
  return naLocateBufferAbsolute(iter, abspos + offset);
}



NA_IDEF NABool naLocateBufferFromStart(NABufferIterator* iter, NAInt offset){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  return naLocateBufferAbsolute(iter, buffer->range.origin + offset);
}



NA_IDEF NABool naLocateBufferFromEnd(NABufferIterator* iter, NAInt offset){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  return naLocateBufferAbsolute(iter, naGetRangeiEnd(buffer->range) - offset);
}



NA_HIDEF NAInt naGetBufferIteratorPartOffset(NABufferIterator* iter){
  return iter->partoffset;
}



NA_HIDEF NABufferPart* naGetBufferPart(NABufferIterator* iter){
  return naGetTreeCurMutable(&(iter->partiter));
}



NA_IDEF NABool naIsBufferAtInitial(NABufferIterator* iter){
  return naIsTreeAtInitial(&(iter->partiter));
}



NA_IDEF NABool naIsBufferAtEnd(NABufferIterator* iter){
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
  if(!naIsBufferAtInitial(iter)){
    NABufferPart* part = naGetBufferPart(iter);
    if(iter->partoffset < 0 || iter->partoffset >= naGetBufferPartByteSize(part)){
      naLocateBufferAbsolute(iter, naGetBufferLocation(iter));
    }
  }
  return (naIsBufferAtInitial(iter) && (iter->partoffset == naGetRangeiEnd(buffer->range)));
}



NA_IDEF uint8 naGetBufferCurBit(NABufferIterator* iter){
  return iter->curbit;
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
