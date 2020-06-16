
#include "../../NABuffer.h"



NA_HAPI void na_DestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, na_DestructBufferPart, NA_FALSE);



// Creates a memory block with sparse memory.
// A sparse buffer is initialized with a byteOffset of 0. This will possibly
// change when calling naReferenceBufferPart or naFillBufferPart.
NA_HDEF NABufferPart* na_NewBufferPartSparse(NABufferSource* source, NARangei sourceRange){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(sourceRange.length))
      naError("range length is not useful");
//    if(!source && sourceRange.origin != 0)
//      naError("na_NewBufferPartSparse", "origin unequal zero makes no sense without source");
  #endif
  part = naNew(NABufferPart);
  if(source){
    NABuffer* sourcebuffer;
    part->source = naRetain(source);
    part->sourceOffset = sourceRange.origin;
    sourcebuffer = na_GetBufferSourceUnderlyingBuffer(source);
    if(sourcebuffer){
      na_EnsureBufferRange(sourcebuffer, sourceRange.origin, naGetRangeiEnd(sourceRange));
    }
  }else{
    part->source = NA_NULL;
    part->sourceOffset = 0;
  }
  part->byteSize = sourceRange.length;
  part->blockoffset = 0;
  part->memblock = NA_NULL;
  return part;
}



// Creates a memory block with constant data
NA_HDEF NABufferPart* na_NewBufferPartWithConstData(const void* data, NAInt byteSize){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(byteSize))
      naError("byteSize is not useful");
  #endif
  part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceOffset = 0;
  part->byteSize = byteSize;
  part->blockoffset = 0;
  part->memblock = na_NewMemoryBlockWithData(naMakePtrWithDataConst(data), byteSize, NA_NULL);
  return part;
}



// Creates a memory block with mutable data
NA_HDEF NABufferPart* na_NewBufferPartWithMutableData(void* data, NAInt byteSize, NAMutator destructor){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(byteSize))
      naError("byteSize is not useful");
  #endif
  part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceOffset = 0;
  part->byteSize = byteSize;
  part->blockoffset = 0;
  part->memblock = na_NewMemoryBlockWithData(naMakePtrWithDataMutable(data), byteSize, destructor);
  return part;
}



NA_HDEF void na_SeparateBufferPart(NABufferPart* part){
  NAMemoryBlock* newblock = na_NewMemoryBlock(part->byteSize);
  naCopyn(na_GetMemoryBlockDataPointerMutable(newblock, 0), na_GetMemoryBlockDataPointerConst(part->memblock, part->blockoffset), part->byteSize);
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
NA_HDEF NABufferPart* na_SplitBufferPart(NATreeIterator* partIter, NAInt start, NAInt end){
  NABufferPart* part = naGetTreeCurLeafMutable(partIter);
  NABufferPart* newpart;
  NAInt prevbyteSize;

  #ifndef NDEBUG
    if(naIsTreeAtInitial(partIter))
      naError("Iterator is at initial position.");
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(!naIsLengthValueUsefuli(end - start))
      naError("start and end make no sense");
    if(start > part->byteSize)
      naError("start is too big");
    if(end < 0)
      naError("end should not be negative");
  #endif

  prevbyteSize = part->byteSize;

  if(start < 0){start = 0;}
  if(end > part->byteSize){end = part->byteSize;}

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

  if(end < prevbyteSize){
    // We need to add a new part at the end.
    part->byteSize = end;
    naUpdateTreeLeaf(partIter);
    newpart = na_NewBufferPartSparse(part->source, naMakeRangeiWithStartAndEnd(part->sourceOffset + end, part->sourceOffset + prevbyteSize));
    naAddTreeNextMutable(partIter, newpart, NA_FALSE);
  }

  if(start > 0){
    // We need to add a new part which contains the bytes at the beginning.
    // We change this parts size and add a new one after this one.
    part->byteSize = start;
    naUpdateTreeLeaf(partIter);
    newpart = na_NewBufferPartSparse(part->source, naMakeRangeiWithStartAndEnd(part->sourceOffset + start, part->sourceOffset + end));
    naAddTreeNextMutable(partIter, newpart, NA_TRUE);
    // Note that using the NA_TRUE, we automatically move to the new part.
    // This means that iter now points to the desired part.
  }

  return naGetTreeCurLeafMutable(partIter);
}



// This function recursively prepares the source of the current part. If the
// current part has no source, memory is prepared. In the end, the current part
// will become a non-sparse part.
NA_HDEF NABufferPart* na_PrepareBufferPartSourceBuffer(NATreeIterator* partIter, NARangei partrange){
  NABufferIterator iter;
  NABool found;
  NABufferPart* sourcepart;
  NAInt offsetinsourcepart;
  NAInt remainingbytesinsourcepart;
  NAInt normedstart;
  NAInt normedend;
  
  NABufferPart* part = naGetTreeCurLeafMutable(partIter);
  NABufferSource* source = part->source;
  NAInt sourceOffset = part->sourceOffset + partrange.origin;
  NABuffer* sourcebuffer = na_GetBufferSourceUnderlyingBuffer(source);

  #ifndef NDEBUG
    if(!na_GetBufferSourceUnderlyingBuffer(source))
      naCrash("source has no buffer");
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(na_IsBufferSourceLimited(source) && !naContainsRangeiOffset(na_GetBufferSourceLimit(source), sourceOffset))
      naError("offset is not in source limits");
  #endif

  iter = naMakeBufferModifier(sourcebuffer);

  found = naLocateBufferAbsolute(&iter, sourceOffset);
  if(!found){
    // If we haven't found a suitable part, we must ensure the desired range.
    na_EnsureBufferRange(sourcebuffer, sourceOffset, sourceOffset + partrange.length);
    // We now know that the new byte must either be at the beginning or
    // the end.
    if(sourcebuffer->range.origin == sourceOffset){
      na_LocateBufferStart(&iter);
    }else{
      na_LocateBufferMax(&iter);
    }
    #ifndef NDEBUG
      if(naGetBufferLocation(&iter) != sourceOffset)
        naError("unsuccessfully enlarged buffer");
    #endif
  }
  
  // Now we can be sure that the buffer iterator is at a part containing
  // sourceOffset.
  
  // Recursive call to the source buffer to prepare itself.
  na_PrepareBuffer(&iter, partrange.length);
  sourcepart = na_GetBufferPart(&iter);
  
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
  
  offsetinsourcepart = iter.partOffset;
  remainingbytesinsourcepart = sourcepart->byteSize - iter.partOffset;
  normedstart = partrange.origin - offsetinsourcepart;
  normedend = partrange.origin + remainingbytesinsourcepart;
  part = na_SplitBufferPart(partIter, normedstart, normedend);

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



NA_HIDEF NAInt na_GetBufferPartNormedStart(NAInt start){
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



NA_HIDEF NAInt na_GetBufferPartNormedEnd(NAInt end){
  // Return the end coordinate, such that end-1 (=max) is within it.
  return na_GetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



// This function expects a sparse buffer part, splits it such that a suitable
// range can be made non-sparse and that range is filled with memory.
NA_HDEF NABufferPart* na_PrepareBufferPartMemory(NATreeIterator* partIter, NARangei partrange){
  NAInt normedstart;
  NAInt normedend;
  void* dst;
  NABufferPart* part = naGetTreeCurLeafMutable(partIter);
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
  normedstart = na_GetBufferPartNormedStart(partrange.origin);
  normedend = na_GetBufferPartNormedEnd(partrange.origin + 1);

  // We split the sparse part as necessary.
  part = na_SplitBufferPart(partIter, normedstart, normedend);

  // Now, the part has been split in whatever manner it was necessary.
  // Let's create the memory block.
  part->memblock = na_NewMemoryBlock(part->byteSize);
  part->blockoffset = 0;
  
  // Fill the memory block according to the source.
  dst = na_GetMemoryBlockDataPointerMutable(part->memblock, 0);
  na_FillSourceBuffer(source, dst, naMakeRangeiWithStartAndEnd(normedstart, normedstart + part->byteSize));

  return part;
}



// This function pepares the current part such that the byte pointed to by
// iter is available as memory. This may create additional sparse parts around
// the current part but always results in iterator pointing to a part being
// completely prepared and the number of available bytes after the current byte
// is returned.
NA_HDEF NAInt na_PrepareBufferPart(NABufferIterator* iter, NAInt byteCount){
  NAInt preparedbyteCount;
  NABufferPart* part = na_GetBufferPart(iter);
  if(na_IsBufferPartSparse(part)){
    // We decide how to prepare the part.
    NABuffer* sourcebuffer = na_GetBufferIteratorSourceBuffer(iter);
    if(sourcebuffer){
      // There is a source buffer, so we try to fill the part with it.
      part = na_PrepareBufferPartSourceBuffer(&(iter->partIter), naMakeRangei(iter->partOffset, byteCount));
    }else{
      // We have no source or no source buffer, meaning, we prepare memory.
      part = na_PrepareBufferPartMemory(&(iter->partIter), naMakeRangei(iter->partOffset, byteCount));
    }
  }
  
  // Reaching here, the current part is a prepared part. We compute the number
  // of remaining bytes in the part and return it.
  preparedbyteCount = part->byteSize - iter->partOffset;
  #ifndef NDEBUG
    if(preparedbyteCount <= 0)
      naError("Returned value should be greater zero");
  #endif
  return preparedbyteCount;
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
