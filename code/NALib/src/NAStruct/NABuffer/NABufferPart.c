
#include "../../NABuffer.h"



NA_HAPI void na_DestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, na_DestructBufferPart, NA_FALSE);



// Creates a buffer part with sparse memory.
NA_HDEF NABufferPart* na_NewBufferPartSparse(NABufferSource* source, NARangei sourceRange){
  #if NA_DEBUG
    //if(!source)
    //  naError("source is Null");
    if(!naIsLengthValueUsefuli(sourceRange.length))
      naError("range length is not useful");
  #endif

  NABufferPart* part = naNew(NABufferPart);
  if(source){
    part->source = naRetain(source);
    part->sourceOffset = sourceRange.origin;
    if(na_HasBufferSourceCache(source)){
      NABuffer* sourceCache = na_GetBufferSourceCache(source);
      na_EnsureBufferRange(sourceCache, sourceRange.origin, naGetRangeiEnd(sourceRange));
    }
  }else{
    part->source = NA_NULL;
    part->sourceOffset = 0;
  }

  part->byteSize = (size_t)sourceRange.length;
  // A sparse buffer is initialized with a blockOffset of 0. This will likely
  // change when calling na_PrepareBufferPartCache.
  part->blockOffset = 0;
  part->memBlock = NA_NULL;

  return part;
}



// Creates a buffer part with constant data
NA_HDEF NABufferPart* na_NewBufferPartWithConstData(const void* data, size_t byteSize){
  #if NA_DEBUG
    if(!data)
      naError("data is Null");
    if(byteSize == 0)
      naError("byteSize is zero");
  #endif

  NABufferPart* part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceOffset = 0;
  part->byteSize = byteSize;
  part->blockOffset = 0;
  part->memBlock = na_NewMemoryBlockWithData(naMakePtrWithDataConst(data), byteSize, NA_NULL);
  return part;
}



// Creates a buffer part with mutable data
NA_HDEF NABufferPart* na_NewBufferPartWithMutableData(void* data, size_t byteSize, NAMutator destructor){

  #if NA_DEBUG
    if(!data)
      naError("data is Null");
    if(byteSize == 0)
      naError("byteSize is zero");
  #endif

  NABufferPart* part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceOffset = 0;
  part->byteSize = byteSize;
  part->blockOffset = 0;
  part->memBlock = na_NewMemoryBlockWithData(naMakePtrWithDataMutable(data), byteSize, destructor);
  return part;
}



// The destructor method which will automatically be called by naRelease.
NA_HDEF void na_DestructBufferPart(NABufferPart* part){
  if(part->source){naRelease(part->source);}
  if(part->memBlock){naRelease(part->memBlock);}
}



// Decouples the part from its current memory block by creating an independent
// mutable copy of the referenced contents. The part will lose the connection
// to the source.
NA_HDEF void na_DecoupleBufferPart(NABufferPart* part){
  #if NA_DEBUG
    if(!part)
      naCrash("part is Null");
  #endif
  NAMemoryBlock* newblock = na_NewMemoryBlock(part->byteSize);
  naCopyn(
    na_GetMemoryBlockDataPointerMutable(newblock, 0),
    na_GetMemoryBlockDataPointerConst(part->memBlock, part->blockOffset),
    part->byteSize);
  naRelease(part->memBlock);

  part->source = NA_NULL;
  part->sourceOffset = 0;
  part->blockOffset = 0;
  part->memBlock = newblock;
}



// This function splits a sparse part such that there exists in the end a
// sparse part having precisely the desired range, possibly surrounded by
// other, newly created sparse parts. At the end, this function moves the
// iterator to the desired part and returns that part.
// The start and end parameters must be positive definite.
NA_HDEF NABufferPart* na_SplitBufferPart(NATreeIterator* partIter, size_t start, size_t end){
  NABufferPart* part = naGetTreeCurLeafMutable(partIter);

  #if NA_DEBUG
    if(naIsTreeAtInitial(partIter))
      naError("Iterator is at initial position.");
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(end <= start)
      naError("start and end make no sense");
    if(start > part->byteSize)
      naError("start is too big");
  #endif

  size_t prevByteSize = part->byteSize;

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

  if(end < prevByteSize){
    // We need to add a new part at the end.
    part->byteSize = end;
    naUpdateTreeLeaf(partIter);

    NAInt sourceOffset = na_GetBufferPartSourceOffset(part);
    NABufferPart* newPart = na_NewBufferPartSparse(part->source, naMakeRangeiWithStartAndEnd(sourceOffset + (NAInt)end, sourceOffset + (NAInt)prevByteSize));
    naAddTreeNextMutable(partIter, newPart, NA_FALSE);
  }

  if(start > 0){
    // We need to add a new part which contains the bytes in the middle.
    // We change this parts size and add a new one after this one.
    part->byteSize = start;
    naUpdateTreeLeaf(partIter);

    NAInt sourceOffset = na_GetBufferPartSourceOffset(part);
    NABufferPart* newPart = na_NewBufferPartSparse(part->source, naMakeRangeiWithStartAndEnd(sourceOffset + (NAInt)start, sourceOffset + (NAInt)end));
    naAddTreeNextMutable(partIter, newPart, NA_TRUE);
    // Note that using the NA_TRUE, we automatically move to the new part.
    // This means that iter now points to the desired part.
  }

  return naGetTreeCurLeafMutable(partIter);
}



// This function prepares the current part by calling the prepare function
// of the cache and referencing the memory block.
NA_HDEF NABufferPart* na_PrepareBufferPartCache(NATreeIterator* partIter, NARangei partRange){  
  NABufferPart* returnPart = naGetTreeCurLeafMutable(partIter);

  #if NA_DEBUG
    if(!returnPart->source)
      naError("Part has no source");
    if(partRange.origin < 0)
      naError("range origin is negative");
  #endif

  NAInt sourceOffset = na_GetBufferPartSourceOffset(returnPart) + partRange.origin;
  NABuffer* sourceCache = na_GetBufferSourceCache(returnPart->source);

  #if NA_DEBUG
    if(!na_HasBufferSourceCache(returnPart->source))
      naCrash("source has no buffer");
    if(!na_IsBufferPartSparse(returnPart))
      naError("part is not sparse");
    if(na_HasBufferSourceLimit(returnPart->source) && !naContainsRangeiOffset(na_GetBufferSourceLimit(returnPart->source), sourceOffset))
      naError("offset is not in source limits");
  #endif

  NABufferIterator sourceIter = naMakeBufferModifier(sourceCache);

  // Try to locate the desired offset in the source. If we find the offset,
  // the iterator is at the correct position, but we need to ensure, the whole
  // part range is available.

  NABool found = naLocateBufferAbsolute(&sourceIter, sourceOffset);
  na_EnsureBufferRange(sourceCache, sourceOffset, sourceOffset + partRange.length);
  
  // If we haven't found a suitable part before ensuring the buffer range, we
  // need to relocate the iterator. We do not first ensure the buffer range
  // because this would  be less performant. Most of the time, the offset will
  // be found and if not found, the location is very easy:

  if(!found){
    // If the offset could not be found before ensuring the buffer range, we
    // know for sure now that the new byte must either be at the beginning or
    // the end.
    if(sourceCache->range.origin == sourceOffset){
      na_LocateBufferStart(&sourceIter);
    }else{
      na_LocateBufferMax(&sourceIter);
    }
    #if NA_DEBUG
      if(naGetBufferLocation(&sourceIter) != sourceOffset)
        naError("unsuccessfully enlarged buffer");
    #endif
  }
  
  // Now we can be sure that the buffer iterator is pointing at sourceOffset.
  
  // Recursive call to the source buffer to prepare the desired range.
  na_PrepareBuffer(&sourceIter, (size_t)partRange.length);

  // Reaching here, we know that the full range is prepared in the source
  // and sourceIter points at the desired byte in the source buffer.
  //
  // The following diagram explains, how the part references the source.
  // 
  // The part potentially needs to be split into different parts because the
  // source is potentially split into multiple parts already. When split at
  // the same boundaries as the source part, the exact same memory block can
  // be referenced with the same offset. Note that the memory block is drawn
  // as one big block but every source part might point to a different part.
  // 
  // Part 1 starts at sourcePart->blockOffset + sourceIter.partOffset and has
  // a length of the remaining bytes of the source part which is the same as
  // sourcePart->byteSize - sourceIter.partOffset.
  //
  // Now, we iterate sourceIter by the remaining bytes. Therefore sourceIter
  // points at the first byte referenced by the second source part.
  // 
  // Part 2 starts at the same blockOffset as the sourcePart and has the same
  // length. The same calculation as in part 1 can be applied.
  // 
  // Again, we iterate sourceIter by the remaining bytes (which is the full
  // size of the source part).
  // 
  // Part 3 starts again at the same blockOffset as the sourcePart but has
  // just the length needed.
  // 
  // |<------>| sourcePart->blockOffset
  //          |<--------------->| sourceIter.partOffset
  //                            |<----------->| remainingBytesInSourcePart
  // 
  // +-- MEMORY BLOCK ----------+------------------------------------ - - -
  // |                          X
  // +--------------------------+------------------------------------ - - -
  //          +-- SOURCE PART ----------------+---------+------------ - - -
  //          |                               |         |
  //          +-------------------------------+---------+------------ - - -
  //                            +-- PART -----+---------+--------+
  //                            | 1           | 2       | 3      |
  //                            +-------------+---------+--------+
  //
  // This covers all possible combinations.

  NATreeIterator curPartIter = naMakeTreeModifier(na_GetTreeIteratorTreeMutable(partIter));
  naLocateTreeIterator(&curPartIter, partIter);

  while(partRange.length){
    NABufferPart* sourcePart = na_GetBufferPart(&sourceIter);
    NABufferPart* curPart = naGetTreeCurLeafMutable(&curPartIter);

    #if NA_DEBUG
      if(na_IsBufferPartSparse(sourcePart))
        naError("source part is sparse");
      if(sourceIter.partOffset < 0)
        naError("part offset is negative.");
    #endif

    NAInt remainingBytesInSourcePart = sourcePart->byteSize - sourceIter.partOffset;
    #if NA_DEBUG
      if(remainingBytesInSourcePart < 0)
        naError("remaining Bytes count is negative");
    #endif

    if((size_t)remainingBytesInSourcePart < curPart->byteSize){
      na_SplitBufferPart(&curPartIter, 0, remainingBytesInSourcePart);
    }
    
    curPart->memBlock = naRetain(na_GetBufferPartMemoryBlock(sourcePart));
    curPart->blockOffset = sourcePart->blockOffset + sourceIter.partOffset;

    if(remainingBytesInSourcePart < partRange.length){
      partRange.origin += remainingBytesInSourcePart;
      partRange.length -= remainingBytesInSourcePart;
      naIterateBuffer(&sourceIter, remainingBytesInSourcePart);
      naIterateTree(&curPartIter, NA_NULL, NA_NULL);
    }else{
      partRange.length = 0;
    }
  }

//  NABufferPart* sourcePart = na_GetBufferPart(&sourceIter);
//#if NA_DEBUG
//  if(na_IsBufferPartSparse(sourcePart))
//    naError("source part is sparse");
//#endif

  //NAInt offsetInSourcePart = sourceIter.partOffset;
  //NAInt remainingBytesInSourcePart = sourcePart->byteSize - sourceIter.partOffset;
  //NAInt normedStart = partRange.origin - offsetInSourcePart;
  //NAInt normedEnd = (partRange.origin + remainingBytesInSourcePart);
  //part = na_SplitBufferPart(partIter, normedStart, normedEnd);

  //part->memBlock = naRetain(na_GetBufferPartMemoryBlock(sourcePart));
  //part->blockOffset = sourcePart->blockOffset + offsetInSourcePart;
    
  naClearBufferIterator(&sourceIter);
  naClearTreeIterator(&curPartIter);

  return returnPart;
}



// This function expects a sparse buffer part, splits it such that a suitable
// range can be made non-sparse and that range is filled with memory.
NA_HDEF NABufferPart* na_PrepareBufferPartMemory(NATreeIterator* partIter, NARangei partRange){
  NABufferPart* part = naGetTreeCurLeafMutable(partIter);

  #if NA_DEBUG
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
    if(partRange.origin < 0)
      naError("range origin is negative");
  #endif

  // We try to split the current sparse part such that in the end, there is
  // a part containing at least the byte pointed to by partRange.origin but
  // possibly a few bytes more. We do this by aligning start and end at
  // NA_INTERNAL_BUFFER_PART_BYTESIZE.
  NAInt normedStart = na_GetBufferPartNormedStart(partRange.origin);
  NAInt normedEnd = na_GetBufferPartNormedEnd(naGetRangeiEnd(partRange));
  #if NA_DEBUG
    if(normedStart < 0)
      naError("normed start is negative");
  #endif
  if(normedEnd > (NAInt)part->byteSize){normedEnd = (NAInt)part->byteSize;}

  // We split the sparse part as necessary.
  part = na_SplitBufferPart(partIter, (size_t)normedStart, (size_t)normedEnd);

  // Now, the part has been split in whatever was necessary.
  // Let's create the memory block.
  part->memBlock = na_NewMemoryBlock(part->byteSize);
  part->blockOffset = 0;
  
  // Fill the memory block according to the source.
  if(part->source){
    NAInt sourceOffset = na_GetBufferPartSourceOffset(part);
    void* dst = na_GetMemoryBlockDataPointerMutable(part->memBlock, 0);
    na_FillBufferSourceMemory(
      part->source,
      dst,
      naMakeRangeiWithStartAndEnd(sourceOffset, sourceOffset + part->byteSize));
  }

  return part;
}



// This function pepares the current part such that the byte pointed to by
// iter is available as memory. This may need to recursively prepare the cache
// of the source and may create additional sparse parts around the current
// part but always results in iterator pointing to a part being completely
// prepared and the number of available bytes after the current byte is
// returned.
NA_HDEF size_t na_PrepareBufferPart(NABufferIterator* iter, NAInt byteCount){
  #if NA_DEBUG
    if(iter->partOffset < 0)
      naError("part offset is negative.");
  #endif
  
  NABufferPart* part = na_GetBufferPart(iter);

  if(na_IsBufferPartSparse(part)){
    // We decide how to prepare the part.
    NABuffer* cache = na_GetBufferIteratorCache(iter);
    if(cache){
      // There is a cache, so we try to fill the part with it.
      part = na_PrepareBufferPartCache(
        &(iter->partIter),
        naMakeRangei(iter->partOffset, byteCount));
    }else{
      // We have no cache, meaning, we prepare memory ourselfes.
      part = na_PrepareBufferPartMemory(
        &(iter->partIter),
        naMakeRangei(iter->partOffset, byteCount));
    }
  }
  
  // Reaching here, the current part is a prepared part. We compute the number
  // of remaining bytes in the part and return it.
  NAInt preparedByteCount = (NAInt)part->byteSize - iter->partOffset;
  #if NA_DEBUG
    if(preparedByteCount <= 0)
      naError("Internal error: Returned value should be greater zero");
  #endif
  return (size_t)preparedByteCount;
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
