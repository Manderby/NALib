
#include "../../NABuffer.h"



NA_HHAPI void na_DestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, na_DestructBufferPart, NA_FALSE);



// Creates a memory block with sparse memory.
// A sparse buffer is initialized with a byteoffset of 0. This will possibly
// change when calling naReferenceBufferPart or naFillBufferPart.
NA_HDEF NABufferPart* na_NewBufferPartSparse(NABufferSource* source, NARangei sourcerange){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(sourcerange.length))
      naError("range length is not useful");
//    if(!source && sourcerange.origin != 0)
//      naError("na_NewBufferPartSparse", "origin unequal zero makes no sense without source");
  #endif
  part = naNew(NABufferPart);
  if(source){
    NABuffer* sourcebuffer;
    part->source = naRetain(source);
    part->sourceoffset = sourcerange.origin;
    sourcebuffer = na_GetBufferSourceUnderlyingBuffer(source);
    if(sourcebuffer){
      na_EnsureBufferRange(sourcebuffer, sourcerange.origin, naGetRangeiEnd(sourcerange));
    }
  }else{
    part->source = NA_NULL;
    part->sourceoffset = 0;
  }
  part->bytesize = sourcerange.length;
  part->blockoffset = 0;
  part->memblock = NA_NULL;
  return part;
}



// Creates a memory block with constant data
NA_HDEF NABufferPart* na_NewBufferPartWithConstData(const void* data, NAInt bytesize){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(bytesize))
      naError("bytesize is not useful");
  #endif
  part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceoffset = 0;
  part->bytesize = bytesize;
  part->blockoffset = 0;
  part->memblock = na_NewMemoryBlockWithData(naMakePtrWithDataConst(data), bytesize, NA_NULL);
  return part;
}



// Creates a memory block with mutable data
NA_HDEF NABufferPart* na_NewBufferPartWithMutableData(void* data, NAInt bytesize, NAMutator destructor){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(bytesize))
      naError("bytesize is not useful");
  #endif
  part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceoffset = 0;
  part->bytesize = bytesize;
  part->blockoffset = 0;
  part->memblock = na_NewMemoryBlockWithData(naMakePtrWithDataMutable(data), bytesize, destructor);
  return part;
}



NA_HDEF void na_SeparateBufferPart(NABufferPart* part){
  NAMemoryBlock* newblock = na_NewMemoryBlock(part->bytesize);
  naCopyn(na_GetMemoryBlockDataPointerMutable(newblock, 0), na_GetMemoryBlockDataPointerConst(part->memblock, part->blockoffset), part->bytesize);
  naRelease(part->memblock);
  part->memblock = newblock;
}



// The destructor method which will automatically be called by naRelease.
NA_HDEF void na_DestructBufferPart(NABufferPart* part){
  if(part->source){naRelease(part->source);}
  if(part->memblock){naRelease(part->memblock);}
}



// This function splits a sparse part such that there exists in the end a
// sparse part having precisely the desired range, possibly surrounded by
// other, newly created sparse parts. At the end of this function, the iterator
// points to that very part.
// The start and end parameters must be positive definite.
// Moves the iterator to the desired part and returns that part.
NA_HDEF NABufferPart* na_SplitBufferPart(NATreeIterator* partiter, NAInt start, NAInt end){
  NABufferPart* part = naGetTreeCurLeafMutable(partiter);
  NABufferPart* newpart;
  NAInt prevbytesize;

  #ifndef NDEBUG
    if(naIsTreeAtInitial(partiter))
      naError("Iterator is at initial position.");
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(!naIsLengthValueUsefuli(end - start))
      naError("start and end make no sense");
    if(start > part->bytesize)
      naError("start is too big");
    if(end < 0)
      naError("end should not be negative");
  #endif

  prevbytesize = part->bytesize;

  if(start < 0){start = 0;}
  if(end > part->bytesize){end = part->bytesize;}

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
    part->bytesize = end;
    naUpdateTreeLeaf(partiter);
    newpart = na_NewBufferPartSparse(part->source, naMakeRangeiWithStartAndEnd(part->sourceoffset + end, part->sourceoffset + prevbytesize));
    naAddTreeNextMutable(partiter, newpart, NA_FALSE);
  }

  if(start > 0){
    // We need to add a new part which contains the bytes at the beginning.
    // We change this parts size and add a new one after this one.
    part->bytesize = start;
    naUpdateTreeLeaf(partiter);
    newpart = na_NewBufferPartSparse(part->source, naMakeRangeiWithStartAndEnd(part->sourceoffset + start, part->sourceoffset + end));
    naAddTreeNextMutable(partiter, newpart, NA_TRUE);
    // Note that using the NA_TRUE, we automatically move to the new part.
    // This means that iter now points to the desired part.
  }

  return naGetTreeCurLeafMutable(partiter);
}



// This function recursively prepares the source of the current part. If the
// current part has no source, memory is prepared. In the end, the current part
// will become a non-sparse part.
NA_HDEF NABufferPart* na_PrepareBufferPartSourceBuffer(NATreeIterator* partiter, NARangei partrange){
  NABufferIterator iter;
  NABool found;
  NABufferPart* sourcepart;
  NAInt offsetinsourcepart;
  NAInt remainingbytesinsourcepart;
  NAInt normedstart;
  NAInt normedend;
  
  NABufferPart* part = naGetTreeCurLeafMutable(partiter);
  NABufferSource* source = part->source;
  NAInt sourceoffset = part->sourceoffset + partrange.origin;
  NABuffer* sourcebuffer = na_GetBufferSourceUnderlyingBuffer(source);

  #ifndef NDEBUG
    if(!na_GetBufferSourceUnderlyingBuffer(source))
      naCrash("source has no buffer");
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(na_IsBufferSourceLimited(source) && !naContainsRangeiOffset(na_GetBufferSourceLimit(source), sourceoffset))
      naError("offset is not in source limits");
  #endif

  iter = naMakeBufferModifier(sourcebuffer);

  found = naLocateBufferAbsolute(&iter, sourceoffset);
  if(!found){
    // If we haven't found a suitable part, we must ensure the desired range.
    na_EnsureBufferRange(sourcebuffer, sourceoffset, sourceoffset + partrange.length);
    // We now know that the new byte must either be at the beginning or
    // the end.
    if(sourcebuffer->range.origin == sourceoffset){
      na_LocateBufferStart(&iter);
    }else{
      na_LocateBufferMax(&iter);
    }
    #ifndef NDEBUG
      if(naGetBufferLocation(&iter) != sourceoffset)
        naError("unsuccessfully enlarged buffer");
    #endif
  }
  
  // Now we can be sure that the buffer iterator is at a part containing
  // sourceoffset.
  
  // Recursive call to the source buffer to prepare itself.
  na_PrepareBuffer(&iter, partrange.length);
  sourcepart = naGetBufferPart(&iter);
  
  #ifndef NDEBUG
    if(na_IsBufferPartSparse(sourcepart))
      naError("source part is sparse");
  #endif

  // Reaching here, iter points at the desired byte in the source buffer.
  // The computation is a little complicated, so here is a diagram:
  //
  //                              |<--- remainingbytesinsourcepart --->|
  // +-- SOURCEPART --------------+------------------------------------+
  // |       offsetinsourcepart ->X                                    |
  // +----------------------------+------------------------------------+
  //                              |
  // +-- normedstart              |                        normedend --+
  // |                            |                                    |
  // V                            |                                    V
  // +      +- PART --------------+------------------------+           +
  // |      |  partrange.origin ->X                        |           |
  // +      +---------------------+------------------------+           +
  //
  // Note that in general, part may be larger than sourcepart in which case
  // na_SplitBufferPart will split part into pieces.
  //
  // Note that the na_SplitBufferPart function will ensure that normedstart
  // and normedend are clamped to the parts boundaries if part is smaller
  // than sourcepart.
  
  offsetinsourcepart = iter.partoffset;
  remainingbytesinsourcepart = sourcepart->bytesize - iter.partoffset;
  normedstart = partrange.origin - offsetinsourcepart;
  normedend = partrange.origin + remainingbytesinsourcepart;
  part = na_SplitBufferPart(partiter, normedstart, normedend);

  part->memblock = naRetain(na_GetBufferPartMemoryBlock(sourcepart));
  part->blockoffset = sourcepart->blockoffset + offsetinsourcepart;
    
  naClearBufferIterator(&iter);

  return part;
}



#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif



NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
  NAInt signshift = (start < 0);   // Note that (start < 0) either results in 0 or 1.
  return (((start + signshift) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - signshift) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
  // Examples explain best how this behaves (assume default part size to be 10):
  //  11:  (( 11 + 0) / 10) - 0 * 10 =  10
  //  10:  (( 10 + 0) / 10) - 0 * 10 =  10
  //   9:  ((  9 + 0) / 10) - 0 * 10 =   0
  //   1:  ((  1 + 0) / 10) - 0 * 10 =   0
  //   0:  ((  0 + 0) / 10) - 0 * 10 =   0
  //  -1:  (( -1 + 1) / 10) - 1 * 10 = -10
  //  -9:  (( -9 + 1) / 10) - 1 * 10 = -10
  // -10:  ((-10 + 1) / 10) - 1 * 10 = -10
  // -11:  ((-11 + 1) / 10) - 1 * 10 = -20
}



NA_HIDEF NAInt naGetBufferPartNormedEnd(NAInt end){
  // Return the end coordinate, such that end-1 (=max) is within it.
  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



// This function expects a sparse buffer part, splits it such that a suitable
// range can be made non-sparse and that range is filled with memory.
NA_HDEF NABufferPart* na_PrepareBufferPartMemory(NATreeIterator* partiter, NARangei partrange){
  NAInt normedstart;
  NAInt normedend;
  void* dst;
  NABufferPart* part = naGetTreeCurLeafMutable(partiter);
  NABufferSource* source = part->source;

  #ifndef NDEBUG
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(partrange.origin < 0)
      naError("start is negative");
  #endif

  // We try to split the current sparse part such that in the end, there is
  // a part containing at least the byte pointed to by partrange.origin.
  // We do this by aligning start and end at NA_INTERNAL_BUFFER_PART_BYTESIZE.
  // Both endpoints do not need to be adjusted as the partrange is always
  // positive definite and more bytes in the block do not harm.
  normedstart = naGetBufferPartNormedStart(partrange.origin);
  normedend = naGetBufferPartNormedEnd(partrange.origin + 1);

  // We split the sparse part as necessary.
  part = na_SplitBufferPart(partiter, normedstart, normedend);

  // Now, the part has been split in whatever manner it was necessary.
  // Let's create the memory block.
  part->memblock = na_NewMemoryBlock(part->bytesize);
  part->blockoffset = 0;
  
  // Fill the memory block according to the source.
  dst = na_GetMemoryBlockDataPointerMutable(part->memblock, 0);
  na_FillSourceBuffer(source, dst, naMakeRangeiWithStartAndEnd(normedstart, normedstart + part->bytesize));

  return part;
}



// This function pepares the current part such that the byte pointed to by
// iter is available as memory. This may create additional sparse parts around
// the current part but always results in iterator pointing to a part being
// completely prepared and the number of available bytes after the current byte
// is returned.
NA_HDEF NAInt na_PrepareBufferPart(NABufferIterator* iter, NAInt bytecount){
  NAInt preparedbytecount;
  NABufferPart* part = naGetBufferPart(iter);
  if(na_IsBufferPartSparse(part)){
    // We decide how to prepare the part.
    NABuffer* sourcebuffer = na_GetBufferIteratorSourceBuffer(iter);
    if(sourcebuffer){
      // There is a source buffer, so we try to fill the part with it.
      part = na_PrepareBufferPartSourceBuffer(&(iter->partiter), naMakeRangei(iter->partoffset, bytecount));
    }else{
      // We have no source or no source buffer, meaning, we prepare memory.
      part = na_PrepareBufferPartMemory(&(iter->partiter), naMakeRangei(iter->partoffset, bytecount));
    }
  }
  
  // Reaching here, the current part is a prepared part. We compute the number
  // of remaining bytes in the part and return it.
  preparedbytecount = part->bytesize - iter->partoffset;
  #ifndef NDEBUG
    if(preparedbytecount <= 0)
      naError("Returned value should be greater zero");
  #endif
  return preparedbytecount;
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
