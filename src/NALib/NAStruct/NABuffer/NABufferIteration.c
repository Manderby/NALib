

// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"



// /////////////////////////////////////
// Buffer Iteration
// /////////////////////////////////////








NA_HDEF const NABuffer* naGetBufferIteratorBufferConst(const NABufferIterator* iter){
  return naGetPtrConst(&(iter->bufferptr));
}



NA_HDEF NABuffer* naGetBufferIteratorBufferMutable(NABufferIterator* iter){
  return naGetPtrMutable(&(iter->bufferptr));
}




NA_DEF NABufferIterator naMakeBufferAccessor(const NABuffer* buffer){
  NABufferIterator iter;
  NABuffer* mutablebuffer = (NABuffer*)buffer;
  #ifndef NDEBUG
    if(!buffer)
      naCrash("naMakeBufferAccessor", "buffer is Null pointer");
  #endif
  iter.bufferptr = naMakePtrWithDataConst(naRetain(mutablebuffer));
  iter.partiter = naMakeTreeAccessor(&(buffer->parts));
  iter.partoffset = 0;
  iter.curbit = 0;
  iter.linenum = 0;
  #ifndef NDEBUG
//    mutablebuffer = (NABuffer*)buffer;
    mutablebuffer->itercount++;
  #endif
  return iter;
}



NA_DEF NABufferIterator naMakeBufferMutator(const NABuffer* buffer){
  NABufferIterator iter;
  NABuffer* mutablebuffer = (NABuffer*)buffer;
  #ifndef NDEBUG
    if(!buffer)
      naCrash("naMakeBufferMutator", "buffer is Null pointer");
  #endif
  iter.bufferptr = naMakePtrWithDataConst(naRetain(mutablebuffer));
  iter.partiter = naMakeTreeAccessor(&(buffer->parts));
  iter.partoffset = 0;
  iter.curbit = 0;
  iter.linenum = 0;
  #ifndef NDEBUG
//    mutablebuffer = (NABuffer*)buffer;
    mutablebuffer->itercount++;
  #endif
  return iter;
}



NA_DEF NABufferIterator naMakeBufferModifier(NABuffer* buffer){
  NABufferIterator iter;
  NABuffer* mutablebuffer = (NABuffer*)buffer;
  #ifndef NDEBUG
    if(!buffer)
      naCrash("naMakeBufferAccessor", "buffer is Null pointer");
  #endif
  iter.bufferptr = naMakePtrWithDataMutable(naRetain(mutablebuffer));
  iter.partiter = naMakeTreeModifier(&(buffer->parts));
  iter.partoffset = 0;
  iter.curbit = 0;
  iter.linenum = 0;
  #ifndef NDEBUG
//    mutablebuffer = (NABuffer*)buffer;
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
  naRelease(naGetPtrMutable(&(iter->bufferptr)));
}



NA_DEF NABool naLocateBuffer(NABufferIterator* iter, NAInt offset){
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



NA_DEF NABool naLocateBufferFirstPart(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferFirstPart", "Buffer bitcount is not null.");
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



NA_DEF NABool naLocateBufferLastIndex(NABufferIterator* iter){
  #ifndef NDEBUG
    if(iter->curbit != 0)
      naError("naLocateBufferLastIndex", "Buffer bitcount is not null.");
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
    if(naIsTreeAtInitial(&(iter->partiter)))
      naError("naGetBufferLocation", "Iterator is at initial position.");
  #endif
  NABufferSearchToken token;
  token.searchoffset = 0;
  token.curoffset = 0;
  naBubbleTreeToken(&(iter->partiter), &token, naAccumulateBufferLocation);
  return iter->partoffset + token.curoffset;
}



NA_DEF void naSeekBufferAbsolute(NABufferIterator* iter, NAInt offset){
  NA_UNUSED(iter);
  NA_UNUSED(offset);
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  iter->curoffset = offset;
//  if(!naIsBufferEmpty(buffer)){
//    naLocateBufferPartOffset(&(iter->partiter), offset);
//  }
  return;
}



NA_DEF void naSeekBufferRelative(NABufferIterator* iter, NAInt offset){
  NA_UNUSED(iter);
  NA_UNUSED(offset);
//  NAInt absoffset = iter->curoffset + offset;
//  naSeekBufferAbsolute(iter, absoffset);
  return;
}



NA_DEF void naSeekBufferSource(NABufferIterator* iter, NAInt offset){
  NA_UNUSED(iter);
  NA_UNUSED(offset);
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  NAInt absoffset = buffer->srcoffset + offset;
//  naSeekBufferAbsolute(iter, absoffset);
  return;
}



NA_DEF void naSeekBufferFromStart(NABufferIterator* iter, NAInt offset){
  NA_UNUSED(iter);
  NA_UNUSED(offset);
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  NAInt absoffset = buffer->range.origin + offset;
//  naSeekBufferAbsolute(iter, absoffset);
  return;
}



NA_DEF void naSeekBufferFromEnd(NABufferIterator* iter, NAInt offset){
  NA_UNUSED(iter);
  NA_UNUSED(offset);
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  NAInt absoffset = naGetRangeiEnd(buffer->range) + offset;
//  naSeekBufferAbsolute(iter, absoffset);
  return;
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



NA_DEF uint8 naGetBufferCurBit(NABufferIterator* iter){
  return iter->curbit;
}



NA_HDEF NABufferPart* naGetBufferPart(NABufferIterator* iter){
  return naGetTreeCurMutable(&(iter->partiter));
}



// Returns NA_TRUE if the current part does not store memory. Repositions
// the iterator if there was a change in position since the last time.
NA_HDEF NABool naIsBufferPartSparse(NABufferIterator* iter){
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
    naLocateBuffer(iter, searchpos);
    part = naGetTreeCurConst(&(iter->partiter));
    #ifndef NDEBUG
      if(iter->partoffset >= naGetBufferPartByteSize(part))
        naError("naIsBufferPartSparse", "Still not found the correct part");
    #endif
  }

  // Reaching here, we can be sure, the iterator points at the correct part.
  return (naGetBufferPartMemoryBlock(part) == NA_NULL);
}




NA_HDEF void naRetrieveBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize, NABool advance){
  NAByte* dst = data;

  #ifndef NDEBUG
    if(!advance)
      naError("naRetrieveBufferBytes", "Not Advance is not implemented yet.");  // todo
    if(!data)
      naError("naRetrieveBufferBytes", "data is Null pointer.");
    if(naGetBufferCurBit(iter) != 0)
      naError("naRetrieveBufferBytes", "Bit offset not 0.");
  #endif

  if(naIsTreeAtInitial(&(iter->partiter))){
    NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
    naLocateBuffer(iter, buffer->range.origin);
//    #ifndef NDEBUG
//      if(naIsTreeAtInitial(&(iter->partiter)))
//        naError("naRetrieveBufferBytes", "iterator is still at initial.");
//    #endif
  }

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  naPrepareBuffer(iter, bytesize, NA_FALSE);
  // After this function, all relevant parts should be present and filled with
  // memory. The iterator should point to the buffer part containing offset.

  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts.
  while(bytesize){
    NABufferPart* part;
    NAInt possiblelength;

    #ifndef NDEBUG
      if(naIsBufferPartSparse(iter))
        naError("naRetrieveBufferBytes", "Cur part is sparse");
    #endif

    // The part pointed to by the iterator should be the one containing offset.
    part = naGetBufferPart(iter);

    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.

    // We get the data pointer where we can read bytes.
    const void* src = naGetBufferPartDataPointerConst(iter);
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
      naIterateTree(&(iter->partiter));
      iter->partoffset = 0;
    }
    naCopyn(dst, src, possiblelength);
    dst += possiblelength;
    bytesize -= possiblelength;
  }
}



// This function splits a sparse part such that there exists in the end a
// sparse part having precisely the desired range, possibly surrounded by
// other, newly created sparse parts. At the end of this function, the iterator
// points to that very part.
// The start and end parameters must be positive definite.

NA_HDEF void naSplitBufferSparsePart(NABufferIterator* iter, NAInt start, NAInt end){
  NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
  NABufferPart* newpart;
  NAInt length = end - start;

  #ifndef NDEBUG
    if(naIsTreeAtInitial(&(iter->partiter)))
      naError("naSplitBufferSparsePart", "Iterator is at initial position.");
    if(!part)
      naCrash("naSplitBufferSparsePart", "iterator must not be at initial position");
    if(!naIsBufferPartSparse(iter))
      naError("naSplitBufferSparsePart", "current part is not sparse");
    if(!naIsLengthValueUsefuli(length))
      naError("naSplitBufferSparsePart", "start and end make no sense");
    if(start < 0)
      naError("naSplitBufferSparsePart", "start should not be negative");
    if(start > naGetBufferPartByteSize(part))
      naError("naSplitBufferSparsePart", "start is too big");
    if(end < 0)
      naError("naSplitBufferSparsePart", "end should not be negative");
    if(end > naGetBufferPartByteSize(part))
      naError("naSplitBufferSparsePart", "end is too big");
  #endif

  // Only split, if the parts bytesize does not already equals the length.
  if(length != naGetBufferPartByteSize(part)){
    NAInt prevbytesize = naGetBufferPartByteSize(part);

    // We try to split the full sparse part into two or three parts.
    // It is important to adjust the length of the current part BEFORE adding
    // any new part because the tree implementation will bubble a change
    // message which requires the correct length in any leaf.
    //
    // Very important: The current part MUST NOT change its offset at all
    // because there might be other buffer iterators visiting this part. In
    // order to work properly, they need to be able to compare its current
    // part offset with the offset of the part they are visiting. If that
    // would change, the iterators would confuse their position.

    if(end < prevbytesize){
      // We need to add a new part at the end.
      naSetBufferPartByteSize(part, end);
      naUpdateTreeLeaf(&(iter->partiter));
      newpart = naNewBufferPartSparse(naGetBufferPartSource(part), naMakeRangeiWithStartAndEnd(naGetBufferPartSourceOffset(part) + end, naGetBufferPartSourceOffset(part) + prevbytesize));
      naAddTreeNextMutable(&(iter->partiter), newpart, NA_FALSE);
    }

    if(start > 0){
      // We need to add a new part which contains the bytes at the beginning.
      // We change this parts size and add a new one after this one.
      naSetBufferPartByteSize(part, start);
      naUpdateTreeLeaf(&(iter->partiter));
      newpart = naNewBufferPartSparse(naGetBufferPartSource(part), naMakeRangeiWithStartAndEnd(naGetBufferPartSourceOffset(part) + start, naGetBufferPartSourceOffset(part) + end));
      naAddTreeNextMutable(&(iter->partiter), newpart, NA_TRUE);
      // Note that using the NA_TRUE, we automatically move to the new part.
      // This means that iter now points to the desired part.
    }
  }
}




// This function pepares the current part such that the byte pointed to by
// iter is available as memory. This may create additional sparse parts around
// the current part but always results in iterator pointing to a part being
// completely prepared and the number of available bytes after the current byte
// is returned.
NA_HDEF NAInt naPrepareBufferPart(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile){
  NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
  #ifndef NDEBUG
    if(naIsTreeAtInitial(&(iter->partiter)))
      naError("naPrepareBufferPart", "Iterator is at initial position.");
    if(!part)
      naError("naPrepareBufferPart", "Part is null");
  #endif

  // If volatile is desired, we simply erase any existing memory block. This
  // could be ameliorated in the future.
  if(naGetBufferPartMemoryBlock(part) && naGetBufferPartSource(part) && (forcevolatile || naIsBufferSourceVolatile(naGetBufferPartSource(part)))){
    naRelease(naGetBufferPartMemoryBlock(part));
    naSetBufferPartMemoryBlock(part, NA_NULL);
  }

  if(naIsBufferPartSparse(iter)){
    part = naGetBufferPart(iter);
    if(naGetBufferPartSource(part)){
      // We first try to prepare the buffer source. The following function
      // returns a source part which contains at least the first desired byte.
      // We then split the current part of this function such that it fits
      // that buffer source part.
      NAInt blockoffset;
      NAInt blocksize;
      NAMemoryBlock* memblock = naPrepareBufferSource(naGetBufferPartSource(part), naGetBufferPartSourceOffset(part) + iter->partoffset, &blockoffset, &blocksize);
      NAInt normedlength = blocksize;
      if(naGetBufferPartByteSize(part) - naGetBufferPartBlockOffset(part) < normedlength){
        normedlength = naGetBufferPartByteSize(part) - naGetBufferPartBlockOffset(part);
      }

      naSplitBufferSparsePart(iter, iter->partoffset, iter->partoffset + normedlength);
      naSetBufferPartBlockOffset(part, blockoffset);
      naSetBufferPartMemoryBlock(part, naRetain(memblock));
//      naRemoveTreeCur(&(iter->partiter), NA_FALSE);
//
//      // Test if the previous block is a direct predecessor of the new block.
//      if(!naIsTreeAtInitial(&(iter->partiter))){
//        NABufferPart* prevpart = naGetTreeCurMutable(&(iter->partiter));
//        if((prevpart->memblock == sourcepart->memblock) && (prevpart->blockoffset + prevpart->bytesize == sourcepart->blockoffset)){
//          // We combine the new block with the previous one and use that one
//          // instead.
//          prevpart->bytesize += sourcepart->bytesize;
//          iter->blockoffset += prevpart->bytesize;
//          naDelete(sourcepart);
//        }else{
//          // The new block is independent of the previous block.
//          naAddTreeNextMutable(&(iter->partiter), sourcepart, NA_TRUE);
//        }
//      }else{
//        // The new block is the first block.
//        naAddTreeNextMutable(&(iter->partiter), sourcepart, NA_TRUE);
//      }
    }else{
      // We have no source, meaning, we prepare memory.
      #ifndef NDEBUG
        if(forcevolatile)
          naError("naPrepareBufferPart", "forcevolatile makes no sense without source");
      #endif

      // We create a suitable range for at least 1 byte which tries to be
      // aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE.
      NAInt normedstart = naGetBufferPartNormedStart(iter->partoffset);
      NAInt normedend = naGetBufferPartNormedEnd(iter->partoffset + 1);
      if(normedend > naGetBufferPartByteSize(part)){normedend = naGetBufferPartByteSize(part);}

      // We split the sparse buffer if necessary.
      naSplitBufferSparsePart(iter, normedstart, normedend);

      // Now, iter points at a sparse part containing the desired byte.
      // Let's create the memory block.
      naSetBufferPartMemoryBlock(part, naNewMemoryBlock(naGetBufferPartByteSize(part)));
      // As we have no source, we can not fill the memory.
    }
  }

  // Reaching here, the current part is a prepared part. We compute the number
  // of remaining bytes in the part and return it.
  NAInt preparedbytecount = naGetBufferPartByteSize(part) - iter->partoffset;
  #ifndef NDEBUG
    if(preparedbytecount <= 0)
      naError("naPrepareBufferPart", "Returned value should be greater zero");
  #endif
  return preparedbytecount;
}



// This is the core function of NABuffer.
//
// This function fills the specified number of bytes in the buffer with memory
// starting from the current position. If the forcevolatile parameter is set
// to NA_TRUE or the buffer source is defined to be volatile, all bytes in that
// range are filled anew. If locatestart is set to NA_TRUE, the iterator of
// buffer will point to the part containing the start offset of range.
// Otherwise, it will point at the part containing the end of the range,
// possibly resulting in the initial position of the list.
NA_HDEF void naPrepareBuffer(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile){
  NABuffer* buffer;
  NATreeIterator firstbufiterator;
  NAInt firstbufoffset = 0;

  #ifndef NDEBUG
//    if(naIsTreeAtInitial(&(iter->partiter)))
//      naError("naPrepareBuffer", "Iterator is at initial position.");
    if(naGetBufferCurBit(iter))
      naError("naPrepareBuffer", "bitcount should be 0");
    if(bytecount <= 0)
      naError("naPrepareBuffer", "bytecount should be >= 1");
  #endif

  buffer = naGetBufferIteratorBufferMutable(iter);

  if(naIsTreeAtInitial(&(iter->partiter))){
    // Reaching here means one of two things: Either the iterator has just not
    // been used yet or the buffer is completely empty. In the latter case, we
    // right now ensure the whole desired buffer range including the bytes
    // requested for preparing. In the former case, we just search for the
    // first buffer part and will enhace later if necessary.
    //
    // Note that enhancing and even just the testing if enhancing is required
    // uses a considerable amount of time hence should be avoided whenever
    // possible.
    if(naIsBufferEmpty(buffer)){
      NAInt normedstart = naGetBufferPartNormedStart(buffer->range.origin);
      if(naHasBufferFixedRange(buffer)){normedstart = naMaxi(normedstart, buffer->range.origin);}
      NAInt normedend = naGetBufferPartNormedEnd(buffer->range.origin + bytecount);
      if(naHasBufferFixedRange(buffer)){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
      naEnsureBufferRange(buffer, normedstart, normedend);
    }

    // We can be sure that there is a part. We start at the beginning.
    naLocateTreeFirst(&(iter->partiter));
    iter->partoffset = 0;
    #ifndef NDEBUG
      if(naIsTreeAtInitial(&(iter->partiter)))
        naError("naPrepareBuffer", "No Buffer part found after being in initial state");
    #endif
  }

  // We save the first iterator for later.
  firstbufiterator = naMakeTreeAccessor(&(buffer->parts));

  // We perform the preparation as long as there are still bytes left.
  while(bytecount){

    // We prepare the current part.
    NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
    #ifndef NDEBUG
      if(!part)
        naError("naPrepareBuffer", "No buffer part available.");
    #endif
    NAInt preparedbytecount = naPrepareBufferPart(iter, bytecount, forcevolatile);

    // Reaching here, the current part is filled with memory. Now, we can set
    // the first iterator for later use.
    if(naIsTreeAtInitial(&firstbufiterator)){
      #ifndef NDEBUG
        if(naIsBufferPartSparse(iter))
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
      naIterateTree(&(iter->partiter));
      iter->partoffset = 0;
    }

    // If there are still bytes required but there is no more part, we enlarge
    // the buffer range at the end.
    if(bytecount && naIsTreeAtInitial(&(iter->partiter))){
      // First, we place the iterator at the currently existing last part.
      naLocateTreeLast(&(iter->partiter));

      // Then, we enhance the end of the buffer.
      NAInt normedend = naGetBufferPartNormedEnd(buffer->range.origin + bytecount);
      if(naHasBufferFixedRange(buffer)){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
      naEnsureBufferRange(buffer, naGetRangeiEnd(buffer->range), normedend);
      // Then, we iterate one step and set the partoffset to the beginning
      // of that new part.

      naIterateTree(&(iter->partiter));
      iter->partoffset = 0;
      #ifndef NDEBUG
        if(naIsTreeAtInitial(&(iter->partiter)))
          naError("naPrepareBuffer", "No Buffer part available after enlarging range at end");
      #endif
    }
  }

  // Reaching here, iter points to byte right after the desired range. We
  // locate the buffer iterator at the first desired offset.
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
