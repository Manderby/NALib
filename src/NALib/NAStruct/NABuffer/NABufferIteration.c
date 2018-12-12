

// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"




NA_HDEF const NABuffer* naGetBufferIteratorBufferConst(const NABufferIterator* iter){
  return naGetPtrConst(&(iter->bufferptr));
}



NA_HDEF NABuffer* naGetBufferIteratorBufferMutable(NABufferIterator* iter){
  return naGetPtrMutable(&(iter->bufferptr));
}



NA_HDEF NABuffer* naGetBufferIteratorSourceBuffer(NABufferIterator* iter){
  NABufferSource* source;
  if(naIsTreeAtInitial(&(iter->partiter))){
    NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
    source = buffer->source;
  }else{
    NABufferPart* part = naGetBufferPart(iter);
    source = naGetBufferPartSource(part);
  }
  if(source){
    return naGetBufferSourceUnderlyingBuffer(source);
  }else{
    return NA_NULL;
  }
}



NA_DEF NABufferIterator naMakeBufferAccessor(NABuffer* buffer){
  NABufferIterator iter;
  #ifndef NDEBUG
    if(!buffer)
      naCrash("naMakeBufferAccessor", "buffer is Null pointer");
  #endif
  iter.bufferptr = naMakePtrWithDataMutable(buffer);
  iter.partiter = naMakeTreeAccessor(&(buffer->parts));
  iter.partoffset = 0;
  iter.curbit = 0;
  iter.linenum = 0;
  #ifndef NDEBUG
    NABuffer* mutablebuffer = (NABuffer*)buffer;
    mutablebuffer->itercount++;
  #endif
  return iter;
}



NA_DEF NABufferIterator naMakeBufferMutator(NABuffer* buffer){
  NABufferIterator iter;
  #ifndef NDEBUG
    if(!buffer)
      naCrash("naMakeBufferMutator", "buffer is Null pointer");
  #endif
  iter.bufferptr = naMakePtrWithDataMutable(buffer);
  iter.partiter = naMakeTreeMutator(&(buffer->parts));
  iter.partoffset = 0;
  iter.curbit = 0;
  iter.linenum = 0;
  #ifndef NDEBUG
    NABuffer* mutablebuffer = (NABuffer*)buffer;
    mutablebuffer->itercount++;
  #endif
  return iter;
}



NA_DEF NABufferIterator naMakeBufferModifier(NABuffer* buffer){
  NABufferIterator iter;
  #ifndef NDEBUG
    if(!buffer)
      naCrash("naMakeBufferAccessor", "buffer is Null pointer");
  #endif
  iter.bufferptr = naMakePtrWithDataMutable(buffer);
  iter.partiter = naMakeTreeModifier(&(buffer->parts));
  iter.partoffset = 0;
  iter.curbit = 0;
  iter.linenum = 0;
  #ifndef NDEBUG
    NABuffer* mutablebuffer = (NABuffer*)buffer;
    mutablebuffer->itercount++;
  #endif
  return iter;
}



NA_DEF void naClearBufferIterator(NABufferIterator* iter){
  naClearTreeIterator(&(iter->partiter));
  #ifndef NDEBUG
    NABuffer* mutablebuffer = (NABuffer*)naGetBufferIteratorBufferConst(iter);
    if(mutablebuffer->itercount == 0)
      naError("naClearBufferIterator", "Too many clears: Buffer has no iterators to clear.");
    mutablebuffer->itercount--;
  #endif
}



NA_HDEF NABool naAccumulateBufferLocation(void* token, NAPtr nodedata, NAInt* childindx){
  NABufferSearchToken* searchtoken = (NABufferSearchToken*)token;
  NABufferTreeNodeData* buffernodedata = (NABufferTreeNodeData*)naGetPtrConst(&nodedata);
  if(*childindx == 1){
    searchtoken->curoffset += buffernodedata->len1;
  }
  *childindx = -1;
  return NA_TRUE;
}



NA_DEF NAInt naGetBufferLocation(const NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naGetBufferLocation", "Buffer bitcount is not null.");
  #endif
  if(naIsTreeAtInitial(&(iter->partiter))){
    return iter->partoffset;
  }else{
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    NABufferSearchToken token;
    token.searchoffset = 0;
    token.curoffset = 0;
    naBubbleTreeToken(&(iter->partiter), &token, naAccumulateBufferLocation);
    // Reaching here, token.curoffset has accumulated the origin of the
    // current part.
    return buffer->range.origin + token.curoffset + iter->partoffset;
  }
}



NA_DEF NABool naLocateBufferAbsolute(NABufferIterator* iter, NAInt offset){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  NABufferSearchToken token;
  token.searchoffset = offset;
  token.curoffset = buffer->range.origin;
  naResetTreeIterator(&(iter->partiter));
  NABool found = naLocateTreeToken(&(iter->partiter), &token);
  if(found){
    iter->partoffset = token.searchoffset - token.curoffset;
  }else{
    iter->partoffset = offset;
  }
  return found;
}



NA_DEF NABool naLocateBufferStart(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferStart", "Buffer bitcount is not null.");
  #endif
  iter->curbit = 0;
  iter->linenum = 0;
  naLocateTreeFirst(&(iter->partiter));
  if(naIsTreeAtInitial(&(iter->partiter))){
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    iter->partoffset = buffer->range.origin;
    return NA_FALSE;
  }else{
    iter->partoffset = 0;
    return NA_TRUE;
  }
}



NA_DEF NABool naLocateBufferLastPart(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferLastPart", "Buffer bitcount is not null.");
  #endif
  iter->curbit = 0;
  iter->linenum = 0;
  naLocateTreeLast(&(iter->partiter));
  if(naIsTreeAtInitial(&(iter->partiter))){
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    iter->partoffset = naGetRangeiEnd(buffer->range);
    return NA_FALSE;
  }else{
    iter->partoffset = 0;
    return NA_TRUE;
  }
}



NA_DEF NABool naLocateBufferNextPart(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferNextPart", "Buffer bitcount is not null.");
  #endif
  iter->curbit = 0;
  iter->linenum = 0;
  naIterateTree(&(iter->partiter));
  if(naIsTreeAtInitial(&(iter->partiter))){
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    iter->partoffset = naGetRangeiEnd(buffer->range);
    return NA_FALSE;
  }else{
    iter->partoffset = 0;
    return NA_TRUE;
  }
}



NA_DEF NABool naLocateBufferPrevPartMax(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferNextPart", "Buffer bitcount is not null.");
  #endif
  iter->curbit = 0;
  iter->linenum = 0;
  naIterateTreeBack(&(iter->partiter));
  if(naIsTreeAtInitial(&(iter->partiter))){
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    iter->partoffset = buffer->range.origin;
    return NA_FALSE;
  }else{
    iter->partoffset = naGetBufferPartByteSize(naGetBufferPart(iter)) - 1;
    return NA_TRUE;
  }
}



NA_DEF NABool naLocateBufferMax(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferMax", "Buffer bitcount is not null.");
  #endif
  naLocateTreeLast(&(iter->partiter));
  if(naIsTreeAtInitial(&(iter->partiter))){
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    iter->partoffset = naGetRangeiEnd(buffer->range);
    return NA_FALSE;
  }else{
    iter->partoffset = naGetBufferPartByteSize(naGetBufferPart(iter)) - 1;
    return NA_TRUE;
  }
}



NA_DEF NABool naLocateBufferEnd(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferMax", "Buffer bitcount is not null.");
  #endif
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  if(buffer->range.length){
    iter->partoffset = naGetRangeiEnd(buffer->range);
  }else{
    iter->partoffset = 0;
  }
  return NA_FALSE;
}



NA_HDEF NABool naIterateBufferPart(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naIterateBufferPart", "Buffer bitcount is not null.");
    if(iter->partoffset != 0)
      naError("naIterateBufferPart", "partoffset is not null.");
  #endif
  NABool success = naIterateTree(&(iter->partiter));
  if(!success){
    const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
    iter->partoffset = naGetRangeiEnd(buffer->range);
  }
  return success;
}



NA_DEF NABool naLocateBufferRelative(NABufferIterator* iter, NAInt offset){
  NAInt abspos = naGetBufferLocation(iter);
  return naLocateBufferAbsolute(iter, abspos + offset);
}



NA_DEF NABool naLocateBufferFromStart(NABufferIterator* iter, NAInt offset){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  return naLocateBufferAbsolute(iter, buffer->range.origin + offset);
}



NA_DEF NABool naLocateBufferFromEnd(NABufferIterator* iter, NAInt offset){
  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
  return naLocateBufferAbsolute(iter, naGetRangeiEnd(buffer->range) - offset);
}



NA_DEF NABool naIterateBuffer(NABufferIterator* iter, NAInt step){
  NA_UNUSED(iter);
  NA_UNUSED(step);
//  const NABufferPart* part;
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  #ifndef NDEBUG
//    if(!step)
//      naError("naIterateBuffer", "step is zero");
//  #endif
//  if(naIsBufferEmpty(buffer)){
//    return NA_FALSE;
//  }
//  part = naGetListCurConst(&(iter->partiter));
//  if(!part){
//    if(step > 0){
//      naLocateListFirst(&(iter->partiter));
//      part = naGetListCurConst(&(iter->partiter));
//      iter->curoffset = buffer->range.origin;
//    }else{
//      naLocateListLast(&(iter->partiter));
//      part = naGetListCurConst(&(iter->partiter));
//      iter->curoffset = naGetRangeiEnd(buffer->range);
//    }
//  }
//  iter->curoffset += step;
//  if(step > 0){
//    while(part && !naContainsBufferPartOffset(part, iter->curoffset)){
//      naIterateList(&(iter->partiter));
//      part = naGetListCurConst(&(iter->partiter));
//    }
//  }else{
//    while(part && !naContainsBufferPartOffset(part, iter->curoffset)){
//      naIterateListBack(&(iter->partiter));
//      part = naGetListCurConst(&(iter->partiter));
//    }
//  }
//  return (part != NA_NULL);
  return NA_FALSE;
}



NA_HDEF NAInt naGetBufferIteratorPartOffset(NABufferIterator* iter){
  return iter->partoffset;
}



NA_DEF NABool naIsBufferAtInitial(NABufferIterator* iter){
  return naIsTreeAtInitial(&(iter->partiter));
}



NA_DEF NABool naIsBufferAtEnd(NABufferIterator* iter){
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
  if(!naIsBufferAtInitial(iter)){
    NABufferPart* part = naGetBufferPart(iter);
    if(iter->partoffset < 0 || iter->partoffset >= naGetBufferPartByteSize(part)){
      naLocateBufferAbsolute(iter, naGetBufferLocation(iter));
    }
  }
  return (naIsBufferAtInitial(iter) && (iter->partoffset == naGetRangeiEnd(buffer->range)));
}



NA_DEF uint8 naGetBufferCurBit(NABufferIterator* iter){
  return iter->curbit;
}



NA_HDEF NABufferPart* naGetBufferPart(NABufferIterator* iter){
  return naGetTreeCurMutable(&(iter->partiter));
}



// Returns NA_TRUE if the current part does not store memory. Repositions
// the iterator if there was a change in position since the last time.
NA_HDEF NABool naIsBufferIteratorSparse(NABufferIterator* iter){
  #ifndef NDEBUG
    if(naIsTreeAtInitial(&(iter->partiter)))
      naError("naIsBufferPartSparse", "Iterator is at initial position.");
    if(iter->partoffset < 0)
      naError("naIsBufferPartSparse", "Negative offset not allowed");
  #endif
  const NABufferPart* part = naGetTreeCurConst(&(iter->partiter));
  if(iter->partoffset >= naGetBufferPartByteSize(part)){
    // Something changed with the part in the meantime. We need to find the
    // correct position. We do this by looking for the absolute position in
    // the source.
    NAInt searchpos = naGetBufferLocation(iter);
    naLocateBufferAbsolute(iter, searchpos);
    part = naGetTreeCurConst(&(iter->partiter));
    #ifndef NDEBUG
      if(iter->partoffset >= naGetBufferPartByteSize(part))
        naError("naIsBufferPartSparse", "Still not found the correct part");
    #endif
  }

  // Reaching here, we can be sure, the iterator points at the correct part.
  return (naGetBufferPartMemoryBlock(part) == NA_NULL);
}



NA_HDEF void naEnsureBufferRangeAndLocate(NABufferIterator* iter, NAInt abspos, NAInt bytecount){
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
  if(naIsRangeiEmpty(buffer->range) || !naContainsRangeiOffset(buffer->range, abspos)){
    // If the desired absolute offset was not inside the buffers range, we
    // enlarge the range.
    naEnsureBufferRange(buffer, abspos, abspos + bytecount);
  }
  naLocateBufferAbsolute(iter, abspos);
}



// This function prepares the given number of bytes in the given buffer. This
// means that after this function, it is guaranteed that all bytes are present
// in memory.
NA_HDEF void naPrepareBuffer(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile){
  #ifndef NDEBUG
    if(naGetBufferCurBit(iter))
      naError("naPrepareBuffer", "bitcount should be 0");
    if(bytecount <= 0)
      naError("naPrepareBuffer", "bytecount should be >= 1");
  #endif

  NATreeIterator firstbufiterator = naMakeTreeAccessor(&(naGetBufferIteratorBufferConst(iter)->parts));
  NAInt firstbufoffset = 0;

  // We perform the preparation as long as there are still bytes left. As we
  // split buffers into parts, it may be the case that there are many parts
  // to prepare, one after the other.
  while(bytecount){

    // First, let's make sure, the iterator is at a buffer part and the offset
    // does not overflow the range.
    
    if(naIsTreeAtInitial(&(iter->partiter))){
      // If this iterator is not at a part, we either have not ensured the
      // range yet or the iterator simply was not located at its desired
      // position yet. The desired buffer offset is defined by iter->partoffset.
      naEnsureBufferRangeAndLocate(iter, iter->partoffset, bytecount);
    }else if(iter->partoffset >= naGetBufferPartByteSize(naGetBufferPart(iter))){
      // If the range is overflown, some other iterator changed this part in
      // the meantime. Maybe it even became a non-sparse part. Therefore
      // we have to search for the correct part again.
      naEnsureBufferRangeAndLocate(iter, naGetBufferLocation(iter), bytecount);
    }

    // Reaching here, iter must be at a valid part.

    // //////////////////////////
    // We prepare the current part.
    NAInt preparedbytecount = naPrepareBufferPart(iter, bytecount, forcevolatile);
    // //////////////////////////

    // Reaching here, iter points at a part filled with memory. Now, we can
    // set the first iterator for later use.
    if(naIsTreeAtInitial(&firstbufiterator)){
      #ifndef NDEBUG
        if(naIsBufferIteratorSparse(iter))
          naError("naPrepareBuffer", "First part located on sparse part");
      #endif
      naLocateTreeIterator(&firstbufiterator, &(iter->partiter));
      firstbufoffset = iter->partoffset;
    }

    // We take as many bytes as we can. If there are enough bytes, we set
    // bytecount to zero.
    if(preparedbytecount > bytecount){
      // More than enough bytes have been prepared.
      bytecount = 0;
    }else{
      // The current part is exhausted. Go to the next one.
      bytecount -= preparedbytecount;
      naLocateBufferNextPart(iter);
    }
  }

  // Reaching here, we need to relocate the buffer iterator at the first
  // desired offset.
  iter->partoffset = firstbufoffset;
  naLocateTreeIterator(&(iter->partiter), &(firstbufiterator));
  naClearTreeIterator(&firstbufiterator);
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
