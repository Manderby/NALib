
#include "../../NABuffer.h"



NA_DEF NABufferIterator naMakeBufferAccessor(const NABuffer* buffer) {
  NABufferIterator iter;
  #if NA_DEBUG
    NABuffer* mutablebuffer;
    if(!buffer)
      naCrash("buffer is Null pointer");
    mutablebuffer = (NABuffer*)buffer;
  #endif
  iter.bufferPtr = naMakePtrWithDataConst(buffer);
  iter.partIter = naMakeTreeAccessor(&(buffer->parts));
  iter.partOffset = 0;
  iter.curBit = 0;
  iter.lineNum = 0;
  #if NA_DEBUG
    mutablebuffer->iterCount++;
  #endif
  return iter;
}



NA_DEF NABufferIterator naMakeBufferMutator(NABuffer* buffer) {
  NABufferIterator iter;
  #if NA_DEBUG
    NABuffer* mutablebuffer;
    if(!buffer)
      naCrash("buffer is Null pointer");
    mutablebuffer = (NABuffer*)buffer;
  #endif
  iter.bufferPtr = naMakePtrWithDataMutable(buffer);
  iter.partIter = naMakeTreeMutator(&(buffer->parts));
  iter.partOffset = 0;
  iter.curBit = 0;
  iter.lineNum = 0;
  #if NA_DEBUG
    mutablebuffer->iterCount++;
  #endif
  return iter;
}



NA_DEF NABufferIterator naMakeBufferModifier(NABuffer* buffer) {
  NABufferIterator iter;
  #if NA_DEBUG
    NABuffer* mutablebuffer;
    if(!buffer)
      naCrash("buffer is Null pointer");
    mutablebuffer = (NABuffer*)buffer;
  #endif
  iter.bufferPtr = naMakePtrWithDataMutable(buffer);
  iter.partIter = naMakeTreeModifier(&(buffer->parts));
  iter.partOffset = 0;
  iter.curBit = 0;
  iter.lineNum = 0;
  #if NA_DEBUG
    mutablebuffer->iterCount++;
  #endif
  return iter;
}



NA_DEF void naClearBufferIterator(NABufferIterator* iter) {
  #if NA_DEBUG
    NABuffer* mutablebuffer;
    mutablebuffer = (NABuffer*)na_GetBufferIteratorBufferConst(iter);
    if(mutablebuffer->iterCount == 0)
      naError("Too many clears: Buffer has no iterators to clear.");
  #endif
  naClearTreeIterator(&(iter->partIter));
  #if NA_DEBUG
    mutablebuffer->iterCount--;
  #endif
}



NA_HDEF NABool na_AccumulateBufferLocation(void* token, NAPtr nodeData, NAInt childIndex) {
  NABufferSearchToken* searchtoken = (NABufferSearchToken*)token;
  NABufferTreeNodeData* buffernodeData = (NABufferTreeNodeData*)naGetPtrConst(nodeData);
  if(childIndex == 1) {
    searchtoken->curOffset += buffernodeData->len1;
  }
  return NA_TRUE;
}



NA_DEF NAInt naGetBufferLocation(const NABufferIterator* iter) {
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  if(naIsTreeAtInitial(&(iter->partIter))) {
    return (NAInt)iter->partOffset;
  }else{
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    NABufferSearchToken token;
    token.searchOffset = 0;
    token.curOffset = 0;
    naBubbleTreeToken(&(iter->partIter), &token, na_AccumulateBufferLocation);
    // Reaching here, token.curOffset has accumulated the origin of the
    // current part.
    return buffer->range.origin + (NAInt)token.curOffset + (NAInt)iter->partOffset;
  }
}



// Callback for naLocateBufferAbsolute
NA_HDEF NAInt na_SearchBufferNode(void* token, NAPtr data) {
  NABufferSearchToken* searchtoken = (NABufferSearchToken*)token;
  NABufferTreeNodeData* nodeData = (NABufferTreeNodeData*)naGetPtrMutable(data);
  NAInt nextIndex;

  if((searchtoken->searchOffset < searchtoken->curOffset) || (searchtoken->searchOffset >= searchtoken->curOffset + (NAInt)nodeData->len1 + (NAInt)nodeData->len2)) {
    nextIndex = NA_TREE_SEARCH_PARENT;
  }else{
    if(searchtoken->searchOffset < searchtoken->curOffset + (NAInt)nodeData->len1) {
      nextIndex = 0;
    }else{
      searchtoken->curOffset += nodeData->len1;
      nextIndex = 1;
    }
  }
  return nextIndex;
}



// Callback for naLocateBufferAbsolute
NA_HDEF NAInt na_SearchBufferLeaf(void* token, NAPtr data) {
  NABufferSearchToken* searchtoken = (NABufferSearchToken*)token;
  NABufferPart* part = (NABufferPart*)naGetPtrMutable(data);
  NAInt nextIndex;

  if((searchtoken->searchOffset >= searchtoken->curOffset) && (searchtoken->searchOffset < searchtoken->curOffset + (NAInt)na_GetBufferPartByteSize(part))) {
    nextIndex = NA_TREE_SEARCH_FOUND;
  }else{
    nextIndex = NA_TREE_SEARCH_ABORT;
  }
  return nextIndex;
}



NA_HDEF NABool na_LocateBufferStart(NABufferIterator* iter) {
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  iter->curBit = 0;
  iter->lineNum = 0;
  naLocateTreeFirst(&(iter->partIter));
  if(naIsTreeAtInitial(&(iter->partIter))) {
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    iter->partOffset = (NAInt)buffer->range.origin;
    return NA_FALSE;
  }else{
    iter->partOffset = 0;
    return NA_TRUE;
  }
}



NA_HDEF NABool na_LocateBufferLastPart(NABufferIterator* iter) {
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  iter->curBit = 0;
  iter->lineNum = 0;
  naLocateTreeLast(&(iter->partIter));
  if(naIsTreeAtInitial(&(iter->partIter))) {
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    iter->partOffset = (NAInt)naGetRangei64End(buffer->range);
    return NA_FALSE;
  }else{
    iter->partOffset = 0;
    return NA_TRUE;
  }
}



NA_HDEF NABool na_LocateBufferNextPart(NABufferIterator* iter) {
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  iter->curBit = 0;
  iter->lineNum = 0;
  naIterateTree(&(iter->partIter), NA_NULL, NA_NULL);
  if(naIsTreeAtInitial(&(iter->partIter))) {
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    iter->partOffset = (NAInt)naGetRangei64End(buffer->range);
    return NA_FALSE;
  }else{
    iter->partOffset = 0;
    return NA_TRUE;
  }
}



NA_HDEF NABool na_LocateBufferPrevPartMax(NABufferIterator* iter) {
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  iter->curBit = 0;
  iter->lineNum = 0;
  naIterateTreeBack(&(iter->partIter), NA_NULL, NA_NULL);
  if(naIsTreeAtInitial(&(iter->partIter))) {
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    iter->partOffset = (NAInt)buffer->range.origin;
    return NA_FALSE;
  }else{
    iter->partOffset = (NAInt)na_GetBufferPartByteSize(na_GetBufferPart(iter)) - 1;
    return NA_TRUE;
  }
}



NA_HDEF NABool na_LocateBufferMax(NABufferIterator* iter) {
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  naLocateTreeLast(&(iter->partIter));
  if(naIsTreeAtInitial(&(iter->partIter))) {
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    iter->partOffset = (NAInt)naGetRangei64End(buffer->range);
    return NA_FALSE;
  }else{
    iter->partOffset = (NAInt)na_GetBufferPartByteSize(na_GetBufferPart(iter)) - 1;
    return NA_TRUE;
  }
}



NA_HDEF NABool na_LocateBufferEnd(NABufferIterator* iter) {
  const NABuffer* buffer;
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
  #endif
  buffer = na_GetBufferIteratorBufferConst(iter);
  if(buffer->range.length) {
    iter->partOffset = (NAInt)naGetRangei64End(buffer->range);
  }else{
    iter->partOffset = 0;
  }
  return NA_FALSE;
}



NA_HDEF NABool na_IterateBufferPart(NABufferIterator* iter) {
  NABool success;
  #if NA_DEBUG
    if(iter->curBit != 0)
      naError("Buffer bitCount is not Null.");
    if(iter->partOffset != 0)
      naError("partOffset is not Null.");
  #endif
  success = naIterateTree(&(iter->partIter), NA_NULL, NA_NULL);
  if(!success) {
    const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
    iter->partOffset = (NAInt)naGetRangei64End(buffer->range);
  }
  return success;
}



NA_DEF NABool naLocateBufferAbsolute(NABufferIterator* iter, NAInt offset) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  NABufferSearchToken token;
  NABool found;
  
  token.searchOffset = offset;
  token.curOffset = buffer->range.origin;
  naResetTreeIterator(&(iter->partIter));
  found = naLocateTreeToken(&(iter->partIter), &token, na_SearchBufferNode, na_SearchBufferLeaf);
  if(found) {
    iter->partOffset = (NAInt)(token.searchOffset - token.curOffset);
  }else{
    iter->partOffset = (NAInt)offset;
  }
  return found;
}



NA_HDEF NABuffer* na_GetBufferIteratorCache(NABufferIterator* iter) {
  NABufferSource* source;
  if(naIsTreeAtInitial(&(iter->partIter))) {
    NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);
    source = buffer->source;
  }else{
    NABufferPart* part = na_GetBufferPart(iter);
    source = na_GetBufferPartSource(part);
  }
  if(source) {
    return na_GetBufferSourceCache(source);
  }else{
    return NA_NULL;
  }
}



NA_DEF NABool naIterateBuffer(NABufferIterator* iter, NAInt step) {
  const NABufferPart* part;
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  #if NA_DEBUG
    if(!step)
      naError("step is zero");
  #endif
  if(naIsBufferEmpty(buffer)) {
    return NA_FALSE;
  }
  if(naIsTreeAtInitial(&(iter->partIter))) {
    if(step > 0) {
      naLocateTreeFirst(&(iter->partIter));
      part = naGetTreeCurLeafConst(&(iter->partIter));
      iter->partOffset = (NAInt)-1; // Oh yeah, that must be refactored definitely.
    }else{
      naLocateTreeLast(&(iter->partIter));
      part = naGetTreeCurLeafConst(&(iter->partIter));
      iter->partOffset = (NAInt)na_GetBufferPartByteSize(part);
    }
  }else{
    part = naGetTreeCurLeafConst(&(iter->partIter));
  }
  iter->partOffset += (size_t)step;
  if(step > 0) {
    while(!naIsTreeAtInitial(&(iter->partIter)) && iter->partOffset >= (NAInt)na_GetBufferPartByteSize(part)) {
      naIterateTree(&(iter->partIter), NA_NULL, NA_NULL);
      if(!naIsTreeAtInitial(&(iter->partIter))) {
        iter->partOffset -= na_GetBufferPartByteSize(part);
        part = naGetTreeCurLeafConst(&(iter->partIter));
      }else{
        iter->partOffset = naGetRangei64End(buffer->range);
        part = NA_NULL;
      }
    }
  }else{
    while(!naIsTreeAtInitial(&(iter->partIter)) && iter->partOffset < 0) {
      naIterateTreeBack(&(iter->partIter), NA_NULL, NA_NULL);
      if(!naIsTreeAtInitial(&(iter->partIter))) {
        iter->partOffset += na_GetBufferPartByteSize(part);
        part = naGetTreeCurLeafConst(&(iter->partIter));
      }else{
        iter->partOffset = naGetRangei64End(buffer->range);
        part = NA_NULL;
      }
    }
  }
  return (part != NA_NULL);
}



// Returns NA_TRUE if the current part does not store memory. Repositions
// the iterator if there was a change in position since the last time.
NA_HDEF NABool na_IsBufferIteratorSparse(NABufferIterator* iter) {
  const NABufferPart* part;
  #if NA_DEBUG
    if(naIsTreeAtInitial(&(iter->partIter)))
      naError("Iterator is at initial position.");
    if(iter->partOffset < 0)
      naError("Negative offset not allowed");
  #endif
  part = naGetTreeCurLeafConst(&(iter->partIter));
  if(iter->partOffset >= (NAInt)na_GetBufferPartByteSize(part)) {
    // Something changed with the part in the meantime. We need to find the
    // correct position. We do this by looking for the absolute position in
    // the source.
    NAInt searchpos = naGetBufferLocation(iter);
    naLocateBufferAbsolute(iter, searchpos);
    part = naGetTreeCurLeafConst(&(iter->partIter));
    #if NA_DEBUG
      if(iter->partOffset >= (NAInt)na_GetBufferPartByteSize(part))
        naError("Still not found the correct part");
    #endif
  }

  // Reaching here, we can be sure, the iterator points at the correct part.
  return (na_IsBufferPartSparse(part));
}



NA_HDEF void na_EnsureBufferRangeAndLocate(NABufferIterator* iter, NAInt abspos, size_t byteCount) {
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);
  if(naIsRangei64Empty(buffer->range) || !naContainsRangei64Point(buffer->range, abspos)) {
    // If the desired absolute offset was not inside the buffers range, we
    // enlarge the range.
    na_EnsureBufferRange(buffer, abspos, abspos + (NAInt)byteCount);
  }
  naLocateBufferAbsolute(iter, abspos);
}



// This function prepares the given number of bytes in the given buffer. This
// means that after this function, it is guaranteed that all bytes are present
// in memory.
NA_HDEF void na_PrepareBuffer(NABufferIterator* iter, size_t byteCount) {
  NATreeIterator firstBufIterator;
  #if NA_DEBUG
    if(naGetBufferCurBit(iter))
      naError("bitCount should be 0");
    if(byteCount == 0)
      naError("byteCount should be >= 1");
  #endif

  firstBufIterator = naMakeTreeAccessor(&(na_GetBufferIteratorBufferConst(iter)->parts));
  size_t firstBufOffset = 0;

  // We perform the preparation as long as there are still bytes left. As we
  // split buffers into parts, it may be the case that there are many parts
  // to prepare, one after the other.
  while(byteCount) {
    // First, let's make sure, the iterator is at a buffer part and the offset
    // does not overflow the range.
    
    if(naIsTreeAtInitial(&(iter->partIter))) {
      // If this iterator is not at a part, we either have not ensured the
      // range yet or the iterator simply was not located at its desired
      // position yet. The desired buffer offset is defined by iter->partOffset.
      na_EnsureBufferRangeAndLocate(iter, (NAInt)iter->partOffset, byteCount);
    }else if(iter->partOffset >= (NAInt)na_GetBufferPartByteSize(na_GetBufferPart(iter))) {
      // If the range is overflown, some other iterator changed this part in
      // the meantime. Maybe it even became a non-sparse part. Therefore
      // we have to search for the correct part again.
      na_EnsureBufferRangeAndLocate(iter, (NAInt)naGetBufferLocation(iter), byteCount);
    }

    // Reaching here, iter must be at a valid part.

    // //////////////////////////
    // We prepare the current part.
    size_t preparedByteCount = na_PrepareBufferPart(iter, byteCount);
    // //////////////////////////

    // Reaching here, iter points at a part filled with memory. Now, we can
    // set the first iterator for later use.
    if(naIsTreeAtInitial(&firstBufIterator)) {
      #if NA_DEBUG
        if(na_IsBufferIteratorSparse(iter))
          naError("First part located on sparse part");
      #endif
      naLocateTreeIterator(&firstBufIterator, &(iter->partIter));
      firstBufOffset = (size_t)iter->partOffset;
    }

    // We take as many bytes as we can. If there are enough bytes, we set
    // byteCount to zero.
    if(preparedByteCount > byteCount) {
      // More than enough bytes have been prepared.
      byteCount = 0;
    }else{
      // The current part is exhausted. Go to the next one.
      byteCount -= preparedByteCount;
      na_LocateBufferNextPart(iter);
    }
  }

  // Reaching here, we need to relocate the buffer iterator at the first
  // desired offset.
  iter->partOffset = (NAInt)firstBufOffset;
  naLocateTreeIterator(&(iter->partIter), &(firstBufIterator));
  naClearTreeIterator(&firstBufIterator);
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
