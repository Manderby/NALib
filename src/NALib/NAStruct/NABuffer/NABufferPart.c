
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "NABuffer.h"


// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////

// Buffers are implemented in an highly granular way. The most granular part
// is an NABufferPart. Consider the following two sentences as buffers:
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf1 = |     This is     |      very       |    exciting.    |
//                 +-----------------+-----------------+-----------------+
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf2 = | Don't question  |       her       |    authority.   |
//                 +-----------------+-----------------+-----------------+
//
// Now, consider the word "everywhere" which contains both the word "very" and
// the word "her" as a part of itself. With NABufferPart, it is possible that
// two NABuffers share a single memory block and use just the part which is
// needed.
//
//      +---------+------+-----------+
//      | This is | very | exciting. |
//      +---------/------\-----------+
//               /        \
//           +== BUFFER PART ==+
//           | bytesize: 4     |    = very
//           | byteoffset: 1 ------------------+
//        +--- pointer         |               |
//        |  +-----------------+               |
//        |                                    |
//        +-------> +=== POINTER ===+          |
//                  | refcount = 2  |       +==V=========+
//                  | data ---------------> | everywhere | (memoryblock)
//        +-------> +---------------+       +-------A----+
//        |                                         |
//        |  +== BUFFER PART ==+                    |
//        |  | bytesize: 3     |    = her           |
//        |  | byteoffset: 6 -----------------------+
//        +--- pointer         |
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
//    granular (the size NA_INTERNAL_BUFFER_PART_BYTESIZE defines the default
//    size). A sparse part can then reference a memory block, hence becoming
//    a filled part.

NA_HAPI void naDestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, naDestructBufferPart, NA_FALSE);



#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif



// Creates a memory block with sparse memory.
//
// A sparse buffer is initialized with a byteoffset of 0. This will possibly
// change when calling naReferenceBufferPart or naFillBufferSourcePart.
NA_HDEF NABufferPart* naNewBufferPartSparse(NAUInt bytesize){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefulu(bytesize))
      naError("naNewBufferPartSparse", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->byteoffset = 0;
  part->bytesize = bytesize;
  part->pointer = NA_NULL;
  return part;
}



// Creates a memory block with given const data buffer
NA_HDEF NABufferPart* naNewBufferPartConstData(const NAByte* data, NAUInt bytesize){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefulu(bytesize))
      naError("naNewBufferPartConstData", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->byteoffset = 0;
  part->bytesize = bytesize;
  part->pointer = naNewPointerConst(data);
  return part;
}



// Creates a memory block with given mutable data buffer
NA_HDEF NABufferPart* naNewBufferPartMutableData(NAByte* data, NAUInt bytesize, NAMutator destructor){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefulu(bytesize))
      naError("naNewBufferPartMutableData", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->byteoffset = 0;
  part->bytesize = bytesize;
  part->pointer = naNewPointerMutable(data, destructor);
  return part;
}



// This function makes a sparse part referencing the same memory block of an
// already filled part with a given offset and bytesize.
NA_HIDEF void naReferenceBufferPart(NABufferPart* part, NABufferPart* srcpart, NAUInt byteoffset, NAUInt bytesize){
  #ifndef NDEBUG
    if(part->pointer)
      naError("naReferenceBufferPart", "Part already is filled");
    if(byteoffset < srcpart->byteoffset)
      naError("naReferenceBufferPart", "Referenced offset too low");
    if(byteoffset >= srcpart->bytesize)
      naError("naReferenceBufferPart", "Referenced offset too high");
    if(byteoffset + bytesize >= srcpart->bytesize)
      naError("naReferenceBufferPart", "Referenced range too big");
  #endif
  part->byteoffset = byteoffset;
  part->bytesize = bytesize;
  part->pointer = naRetainPointer(srcpart->pointer);
}



// Allocates memory for the range defined in part.
NA_HIDEF void naAllocateBufferPartMemory(NABufferPart* part){
  #ifndef NDEBUG
    if(part->byteoffset != 0)
      naError("naAllocateBufferPartMemory", "Byteoffset should be zero");
    if(part->pointer)
      naError("naAllocateBufferPartMemory", "Part already has memory");
  #endif
  part->pointer = naNewPointerMutable(naMalloc((NAInt)part->bytesize), (NAMutator)naFree);
}



// The destructor method which will automatically be called by naDelete.
NA_HDEF void naDestructBufferPart(NABufferPart* part){
  if(part->pointer){naReleasePointer(part->pointer);}
}



// Returns NA_TRUE if the given part does not store memory.
NA_HDEF NABool naIsBufferPartSparse(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      naCrash("naIsBufferPartSparse", "part is Null pointer");
  #endif
  return (part->pointer == NA_NULL);
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
//NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
//  return (((start + (start < 0)) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - (start < 0)) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
//// Note that (start < 0) either results in 0 or 1.
//}
//
//
//
//NA_HIDEF NAInt naGetBufferPartNormedEnd(NAInt end){
//  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
//}
//
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
//    naDelete(prevpart);
//  }
//  if(nextpart && naAreBufferPartsEqualAndAdjacent(part, nextpart)){
//    // we can combine the two parts.
//    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), naGetBufferPartEnd(nextpart));
//    // then, we remove the next part and delete it.
//    naRemoveListNextMutable(iter);
//    naDelete(nextpart);
//  }
//}
//
//
//
//// This function splits a sparse part such that there exists in the end a
//// sparse part having precisely the desired range, possibly surrounded by
//// other parts. At the end of this function, the iterator points to that
//// very part.
//NA_HIDEF void naSplitBufferSparsePart(NAListIterator* iter, NARangei range){
//  NABufferPart* part = naGetListCurMutable(iter);
//
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naSplitBufferSparsePart", "iterator must not be at initial position");
//    if(!naIsBufferPartSparse(part))
//      naError("naSplitBufferSparsePart", "current part is not sparse");
//    if(!naContainsRangeiRange(naGetBufferPartRange(part), range))
//      naError("naSplitBufferSparsePart", "part range does not contain given range");
//  #endif
//
//  if(naEqualRangei(naGetBufferPartRange(part), range)){
//    // This sparse part fits the desired range already.
//    // Nothing to do.
//    
//  }else if(naGetBufferPartStart(part) == range.origin){
//    // The desired part will be put at the start of this sparse part.
//    part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part));
//    part->origin = part->range.origin;
//    part = naNewBufferPartSparse(range);
//    naAddListBeforeMutable(iter, part);
//    naIterateListBack(iter);
//    
//  }else if(naGetBufferPartEnd(part) == naGetRangeiEnd(range)){
//    // The desired part will be put at the end of this sparse part.
//    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
//    part->origin = part->range.origin;
//    part = naNewBufferPartSparse(range);
//    naAddListAfterMutable(iter, part);
//    naIterateList(iter);
//    
//  }else{
//    // The desired part will be put in the middle of this sparse part.
//    NABufferPart* lastpart = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part)));
//    naAddListAfterMutable(iter, lastpart);
//    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
//    part->origin = part->range.origin;
//    part = naNewBufferPartSparse(range);
//    naAddListAfterMutable(iter, part);
//    naIterateList(iter);
//  }
//}
//
//
//
//// Returns a direct pointer to the raw data of this buffer part, given its
//// absolute address.
//NA_HIDEF NAByte* naGetBufferPartDataPointerConst(const NABufferPart* part, NAInt offset){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartDataPointerConst", "buffer part is Null pointer");
//    if(naIsBufferPartSparse(part))
//      naError("naGetBufferPartDataPointerConst", "buffer part is sparse");
//    if(!naContainsRangeiOffset(part->range, offset))
//      naError("naGetBufferPartDataPointerConst", "offset not inside buffer part");
//    if(offset - part->origin < 0)
//      naError("naGetBufferPartDataPointerConst", "offset calculation wrong");
//  #endif
////  printf("getPointer %d %d %c\n", (int)offset, (int)part->origin, ((char*)part->data->sptr.ptr.data.d)[offset - part->origin]);
//  return &(((NAByte*)naGetPointerConst(part->pointer))[offset - part->origin]);
//}
//
//
//
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
