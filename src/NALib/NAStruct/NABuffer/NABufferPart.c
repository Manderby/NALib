
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "NABuffer.h"



// Buffers are implemented in an highly granular way. The most granular parts
// are an NABufferPart and an NABufferSourcePart. Consider the following two
// sentences as buffers:
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf1 = |     This is     |      very       |    exciting.    |
//                 +-----------------+-----------------+-----------------+
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf2 = | Don't question  |       her       |    authority.   |
//                 +-----------------+-----------------+-----------------+
//
// Additionally, we have a text file source with the following content where
// the word "everywhere" with a bytesize of 10 is stored at byte position 16:
//
//                 +== SOURCE PART ==+= SOURCE PART ==+= SOURCE PART =+
// readme.txt      | You can see ads |   everywhere   |  these days.  |
//                 +-----------------+----------------+---------------+
//
// Now, consider the word "everywhere" which contains both the word "very" and
// the word "her" as a part of itself. Two NABuffers can share a single source
// part and use just the part which is needed:
//
//      +---------+------+-----------+
//      | This is | very | exciting. |
//      +---------/------\-----------+
//               /        \
//           +== BUFFER PART ==+
//           | bytesize: 4     |    = very
//           | byteoffset: 1 -------------------------+
//        +--- sourcepart      |                      |
//        |  +-----------------+                      |
//        |                                           |
//        |         +=== SOURCE PART ===+             |
//        +-------> | source            |             |
//                  | range = (16, 10)  |          +==V=========+
//        +-------> | data ----------------------> | everywhere |
//        |         +-------------------+          +-------A----+
//        |                                                |
//        |  +== BUFFER PART ==+                           |
//        |  | bytesize: 3     |    = her                  |
//        |  | byteoffset: 6 ------------------------------+
//        +--- sourcepart      |
//           +-----------------+
//                 \       /
// +----------------\-----/------------+
// | Don't question | her | authority. |
// +----------------+-----+------------+
//
// It is important to understand that it is the referenced data which is shared
// amongst NABuffers, NOT the NABufferParts!
//
// Buffer parts come in basically two forms:
// 1. Filled parts: The pointer field points to a const or mutable memory block
//    (which can be very large in size) which defines the referenced content.
//    Other buffer parts may reference that memory block too.
// 2. Sparse parts: The pointer field is NA_NULL. The referenced memory is yet
//    undefined. This defines a placeholder defining the bytesize which shall
//    be available where the NABufferPart is positioned inside the NABuffer.
//    This bytesize can be very large if needed. Later on, such a part can be
//    split into smaller parts of a specified size to make sparse memory more
//    granular (the size NA_BUFFER_PART_BYTESIZE in NAConfiguration.h defines
//    the default size). A sparse part can then reference a memory block,
//    hence becoming a filled part.



// //////////////////////////////////////
// Memory Block
// //////////////////////////////////////



#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif




NA_HAPI void naDestructMemoryBlock(NAMemoryBlock* block);
NA_RUNTIME_TYPE(NAMemoryBlock, naDestructMemoryBlock, NA_TRUE);



NA_HDEF NAMemoryBlock* naNewMemoryBlock(NAInt bytesize){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(bytesize <= 0)
      naError("naNewMemoryBlock", "bytesize invalid");
  #endif
  block = naNew(NAMemoryBlock);
  block->data = naMakePtrWithDataMutable(naMalloc(bytesize));
  block->deallocator = (NAMutator)naFree;
  #ifndef NDEBUG
    block->bytesize = bytesize;
  #endif
  return block;
}



NA_HDEF NAMemoryBlock* naNewMemoryBlockWithConstData(const void* data, NAInt bytesize){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(bytesize <= 0)
      naError("naNewMemoryBlock", "bytesize invalid");
  #else
    NA_UNUSED(bytesize);
  #endif
  block = naNew(NAMemoryBlock);
  block->data = naMakePtrWithDataConst(data);
  block->deallocator = NA_NULL;
  #ifndef NDEBUG
    block->bytesize = bytesize;
  #endif
  return block;
}



NA_HDEF NAMemoryBlock* naNewMemoryBlockWithMutableData(void* data, NAInt bytesize, NAMutator deallocator){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(bytesize <= 0)
      naError("naNewMemoryBlock", "bytesize invalid");
  #else
    NA_UNUSED(bytesize);
  #endif
  block = naNew(NAMemoryBlock);
  block->data = naMakePtrWithDataMutable(data);
  block->deallocator = deallocator;
  #ifndef NDEBUG
    block->bytesize = bytesize;
  #endif
  return block;
}



NA_HDEF void naDestructMemoryBlock(NAMemoryBlock* block){
  if(block->deallocator){
    block->deallocator(naGetPtrMutable(&(block->data)));
  }
}



NA_HDEF const void* naGetMemoryBlockDataPointerConst(NAMemoryBlock* block, NAInt indx){
  return (const void*)&(((const NAByte*)naGetPtrConst(&(block->data)))[indx]);
}


// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////



// We enable Reference counting as there may be multiple iterators running
// on a certain part.
NA_RUNTIME_TYPE(NABufferPart, naDestructBufferPart, NA_FALSE);



// Creates a memory block with sparse memory.
// A sparse buffer is initialized with a byteoffset of 0. This will possibly
// change when calling naReferenceBufferPart or naFillBufferPart.
NA_HDEF NABufferPart* naNewBufferPartSparse(NABufferSource* source, NARangei range){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(range.length))
      naError("naNewBufferPartSparse", "range length is not useful");
    if(!source && range.origin != 0)
      naError("naNewBufferPartSparse", "origin unequal zero makes no sense without source");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->source = source ? naRetainBufferSource(source) : NA_NULL;
  part->sourceoffset = range.origin;
  part->blockoffset = 0;
  part->bytesize = range.length;
  part->memblock = NA_NULL;
  return part;
}



// Creates a memory block with constant data
NA_HDEF NABufferPart* naNewBufferPartWithConstData(const void* data, NAInt bytesize){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(bytesize))
      naError("naNewBufferPartWithConstData", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceoffset = 0;
  part->blockoffset = 0;
  part->bytesize = bytesize;
  part->memblock = naNewMemoryBlockWithConstData(data, bytesize);
  return part;
}



// Creates a memory block with mutable data
NA_HDEF NABufferPart* naNewBufferPartWithMutableData(void* data, NAInt bytesize, NAMutator deallocator){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(bytesize))
      naError("naNewBufferPartWithMutableData", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceoffset = 0;
  part->blockoffset = 0;
  part->bytesize = bytesize;
  part->memblock = naNewMemoryBlockWithMutableData(data, bytesize, deallocator);
  return part;
}



NA_HDEF NAInt naGetBufferPartNormedStart(NAInt start){
  NAInt signshift = (start < 0);   // Note that (start < 0) either results in 0 or 1.
  return (((start + signshift) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - signshift) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
  // Examples explain best how this behaves (assume default partsize to be 10):
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



NA_HDEF NAInt naGetBufferPartNormedEnd(NAInt end){
  // Return the end coordinate, such that end-1 (=max) is within it.
  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



// This function pepares the current part such that the byte pointed to by
// iter is available as memory. This may create additional sparse parts around
// the current part but always results in iterator pointing to a part being
// completely prepared and the number of available bytes after the current byte
// is returned.
NA_HDEF NAInt naPrepareBufferPart(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile){
  NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
  #ifndef NDEBUG
    if(!part)
      naError("naPrepareBufferPart", "Part is null");
  #endif
  
  // If volatile is desired, we simply erase any existing memory block. This
  // could be ameliorated in the future.
  if(part->memblock && part->source && (forcevolatile || naIsBufferSourceVolatile(part->source))){
    naRelease(part->memblock);
    part->memblock = NA_NULL;
  }
  
  if(part->source){
    if(naIsBufferPartSparse(part)){
      // We first try to prepare the buffer source. The following function
      // returns a source part which contains at least the first desired byte.
      // We then split the current part of this function such that it fits
      // that buffer source part. 
      NAInt blockoffset;
      NABufferPart* sourcepart = naPrepareBufferSource(part->source, part->sourceoffset + iter->partoffset, &blockoffset);
      naSplitBufferSparsePart(iter, iter->partoffset, naMini(bytecount, sourcepart->bytesize - blockoffset));
      naRemoveTreeCur(&(iter->partiter), NA_FALSE);
      
      // Test if the previous block is a direct predecessor of the new block.
      if(!naIsTreeAtInitial(&(iter->partiter))){
        NABufferPart* prevpart = naGetTreeCurMutable(&(iter->partiter));
        if((prevpart->memblock == sourcepart->memblock) && (prevpart->blockoffset + prevpart->bytesize == sourcepart->blockoffset)){
          // We combine the new block with the previous one and use that one
          // instead.
          prevpart->bytesize += sourcepart->bytesize;
          iter->partoffset += prevpart->bytesize;
          naDelete(sourcepart);
        }else{
          // The new block is independent of the previous block.
          naAddTreeNextMutable(&(iter->partiter), sourcepart, NA_TRUE);
        }
      }else{
        // The new block is the first block.
        naAddTreeNextMutable(&(iter->partiter), sourcepart, NA_TRUE);
      }
    }
  
  }else{
    // We have no source, meaning, we prepare memory.
    #ifndef NDEBUG
      if(forcevolatile)
        naError("naPrepareBufferPart", "forcevolatile makes no sense without source");
    #endif
    if(naIsBufferPartSparse(part)){
      // We create a suitable range for at least 1 byte which tries to be
      // aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE.
      NAInt normedstart = naGetBufferPartNormedStart(iter->partoffset);
      NAInt normedend = naGetBufferPartNormedEnd(iter->partoffset + 1);
      if(normedend > part->bytesize){normedend = part->bytesize;}

      // We split the sparse buffer if necessary.
      naSplitBufferSparsePart(iter, normedstart, normedend);

      // Now, iter points at a sparse part containing the desired byte.
      // Let's create the memory block.
      part->memblock = naNewMemoryBlock(part->bytesize);
      // As we have no source, we can not fill the memory.
    }
  }
  
  // Reaching here, the current part is a prepared part. We compute the number
  // of remaining bytes in the part and return it.
  NAInt preparedbytecount = part->bytesize - iter->partoffset;
  #ifndef NDEBUG
    if(preparedbytecount <= 0)
      naError("naPrepareBufferPart", "Returned value should be greater zero");
  #endif
  return preparedbytecount;
}


NA_HDEF void naFillBufferPartConstAccess(NABufferPart* part, NAInt blockoffset, const void** src, NAInt* bytesize){
  NA_UNUSED(part);
  NA_UNUSED(blockoffset);
  NA_UNUSED(src);
  NA_UNUSED(bytesize);
//  #ifndef NDEBUG
//    if(blockoffset < 0)
//      naError("naFillBufferPartConstAccess", "desired blockoffset is negative");
//    if(part->bytesize - blockoffset <= 0)
//      naError("naFillBufferPartConstAccess", "desired blockoffset is out of range");
//  #endif
//  *bytesize = part->bytesize - blockoffset;
//  *src = naGetMemoryBlockConstAccess(part->memblock, blockoffset);
}


// This function makes a sparse part referencing the same memory block of an
// already filled part with a given offset and bytesize.
//NA_HIDEF void naReferenceBufferPart(NABufferPart* part, NABufferPart* srcpart, NAInt byteoffset, NAInt bytesize){
//  #ifndef NDEBUG
//    if(part->pointer)
//      naError("naReferenceBufferPart", "Part already is filled");
//    if(byteoffset < srcpart->byteoffset)
//      naError("naReferenceBufferPart", "Referenced offset too low");
//    if(byteoffset >= srcpart->bytesize)
//      naError("naReferenceBufferPart", "Referenced offset too high");
//    if(byteoffset + bytesize >= srcpart->bytesize)
//      naError("naReferenceBufferPart", "Referenced range too big");
//  #endif
//  part->byteoffset = byteoffset;
//  part->bytesize = bytesize;
//  part->pointer = naRetainPointer(srcpart->pointer);
//}



// Allocates memory for the range defined in part.
//NA_HDEF void naAllocateBufferPartMemory(NABufferPart* part){
//  naAllocateBufferSourcePartMemory(part->sourcepart);
//}



// Calls the filler of the source descriptor for the range of the given part.
// Hence filling the memory with the desired content.
NA_HDEF void naFillBufferPart(NABufferPart* part, NARangei range){
  part->source->buffiller(part->source->data, naGetPtrMutable(&(part->memblock->data)), range);
}



// The destructor method which will automatically be called by naRelease.
NA_HDEF void naDestructBufferPart(NABufferPart* part){
  if(part->source){naReleaseBufferSource(part->source);}
  if(part->memblock){naRelease(part->memblock);}
}



// Returns NA_TRUE if the given part does not store memory.
NA_HDEF NABool naIsBufferPartSparse(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      naCrash("naIsBufferPartSparse", "part is Null pointer");
  #endif
  return part->memblock == NA_NULL;
}



//// Returns the range of the given part.
//NA_HIDEF NARangei naGetBufferPartRange(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartRange", "part is Null pointer");
//  #endif
//  return part->range;
//}
//
//
//
//// Returns the start of the given part range.
//NA_HIDEF NAInt naGetBufferPartStart(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartStart", "part is Null pointer");
//  #endif
//  return part->range.origin;
//}
//
//
//
//// Returns the end of the given part range.
//NA_HIDEF NAInt naGetBufferPartEnd(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartEnd", "part is Null pointer");
//  #endif
//  return naGetRangeiEnd(part->range);
//}
//
//
//
//NA_HIDEF NABool naContainsBufferPartOffset(const NABufferPart* part, NAInt offset){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naContainsBufferPartOffset", "part is Null pointer");
//  #endif
//  return naContainsRangeiOffset(part->range, offset);
//}
//
//
//
//// This function returns NA_TRUE if both parts have the same data pointer
//// and the ranges are adjacent.
//NA_HIDEF NABool naAreBufferPartsEqualAndAdjacent(NABufferPart* startpart, NABufferPart* endpart){
//  // Note that parts may have the same data but do not have the same origin
//  // as parts always denote their origin relative to the source of the buffer.
//  // They only are adjacent if the range origin plus the src origin matches.
//  if((startpart->pointer == endpart->pointer) && ((naGetRangeiEnd(startpart->range) + startpart->origin) == (endpart->range.origin + endpart->origin))){
//    return NA_TRUE;
//  }else{
//    return NA_FALSE;
//  }
//}
//
//
//
//// This function combines the current part given by iter with its previous and
//// next neighbor if possible.
//NA_HIDEF void naCombineBufferPartAdjacents(NAListIterator* iter){
//  NABufferPart* part = naGetListCurMutable(iter);
//  NABufferPart* prevpart = naGetListPrevMutable(iter);
//  NABufferPart* nextpart = naGetListNextMutable(iter);
//
//  if(prevpart && naAreBufferPartsEqualAndAdjacent(prevpart, part)){
//    // we can combine the two parts.
//    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(prevpart), naGetBufferPartEnd(part));
//    // then, we remove the previous part and delete it.
//    naRemoveListPrevMutable(iter);
//    naRelease(prevpart);
//  }
//  if(nextpart && naAreBufferPartsEqualAndAdjacent(part, nextpart)){
//    // we can combine the two parts.
//    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), naGetBufferPartEnd(nextpart));
//    // then, we remove the next part and delete it.
//    naRemoveListNextMutable(iter);
//    naRelease(nextpart);
//  }
//}



// This function splits a sparse part such that there exists in the end a
// sparse part having precisely the desired range, possibly surrounded by
// other, newly created sparse parts. At the end of this function, the iterator
// points to that very part.
NA_HDEF void naSplitBufferSparsePart(NABufferIterator* iter, NAInt start, NAInt end){
  NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
  NABufferPart* newpart;
  NAInt length = end - start;

  #ifndef NDEBUG
    if(!part)
      naCrash("naSplitBufferSparsePart", "iterator must not be at initial position");
    if(!naIsBufferPartSparse(part))
      naError("naSplitBufferSparsePart", "current part is not sparse");
    if(!naIsLengthValueUsefuli(length))
      naError("naSplitBufferSparsePart", "start and end make no sense");
    if(start < 0)
      naError("naSplitBufferSparsePart", "start should not be negative");
    if(start > part->bytesize)
      naError("naSplitBufferSparsePart", "start is too big");
    if(end < 0)
      naError("naSplitBufferSparsePart", "end should not be negative");
    if(end > part->bytesize)
      naError("naSplitBufferSparsePart", "end is too big");
    if(!naIsTreeIteratorAlone(&(iter->partiter)))
      naError("naSplitBufferSparsePart", "there is another iterator at this part. Splitting might lead to problems");
  #endif

  // Only split, if the parts bytesize does not already equals the length.
  if(length != part->bytesize){
    if(end < part->bytesize){
      // We need to add a new part at the end
      newpart = naNewBufferPartSparse(part->source, naMakeRangei(part->sourceoffset + end, part->bytesize - end));
      naAddTreeNextMutable(&(iter->partiter), newpart, NA_FALSE);
    }

    if(start > 0){
      // We need to add a new part at the beginning.
      newpart = naNewBufferPartSparse(part->source, naMakeRangei(part->sourceoffset, start));
      naAddTreePrevMutable(&(iter->partiter), newpart, NA_FALSE);
      part->sourceoffset += start;
      iter->partoffset -= start;
    }

    part->bytesize = length;
  }
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HDEF const void* naGetBufferPartDataPointerConst(const NABufferPart* part, NAInt offset){
  #ifndef NDEBUG
    if(!part)
      naCrash("naGetBufferPartDataPointerConst", "buffer part is Null pointer");
    if(naIsBufferPartSparse(part))
      naError("naGetBufferPartDataPointerConst", "buffer part is sparse");
    if(offset < 0)
      naError("naGetBufferPartDataPointerConst", "offset is negative");
    if(offset >= part->bytesize)
      naError("naGetBufferPartDataPointerConst", "offset not inside buffer part");
  #endif
  return naGetMemoryBlockDataPointerConst(part->memblock, part->blockoffset + offset);
}



//// Returns a direct pointer to the raw data of this buffer part, given its
//// absolute address.
//NA_HIDEF NAByte* naGetBufferPartDataPointerMutable(const NABufferPart* part, NAInt offset){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartDataPointerMutable", "buffer part is Null pointer");
//    if(naIsBufferPartSparse(part))
//      naError("naGetBufferPartDataPointerMutable", "buffer part is sparse");
//    if(!naContainsRangeiOffset(part->range, offset))
//      naError("naGetBufferPartDataPointerMutable", "offset not inside buffer part");
//    if(offset - part->origin < 0)
//      naError("naGetBufferPartDataPointerMutable", "offset calculation wrong");
//  #endif
//  return &(((NAByte*)naGetPointerMutable(part->pointer))[offset - part->origin]);
//}
//
//
//
//#ifndef NDEBUG
//  // Returns a direct pointer to the raw data of this buffer part, given its
//  // absolute address.
//  NA_HIDEF const NAByte* naGetBufferPartBaseDataPointerConst(const NABufferPart* part){
//    #ifndef NDEBUG
//      if(!part)
//        naCrash("naGetBufferPartBaseDataPointerConst", "buffer part is Null pointer");
//      if(naIsBufferPartSparse(part))
//        naError("naGetBufferPartBaseDataPointerConst", "buffer part is sparse");
//    #endif
//    return (const NAByte*)naGetPointerConst(part->pointer);
//  }
//#endif
//
//
//
//// Returns a direct pointer to the raw data of this buffer part, given its
//// absolute address.
//NA_HIDEF NAByte* naGetBufferPartBaseDataPointerMutable(NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartBaseDataPointerMutable", "buffer part is Null pointer");
//    if(naIsBufferPartSparse(part))
//      naError("naGetBufferPartBaseDataPointerMutable", "buffer part is sparse");
//  #endif
//  return (NAByte*)naGetPointerMutable(part->pointer);
//}










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
