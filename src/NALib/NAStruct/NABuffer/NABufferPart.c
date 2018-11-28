
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
// Buffer Source Part
// //////////////////////////////////////



NA_HAPI void naDestructBufferSourcePart(NABufferSourcePart* part);
NA_RUNTIME_TYPE(NABufferSourcePart, naDestructBufferSourcePart, NA_TRUE);



// Creates a buffer source part.
NA_HDEF NABufferSourcePart* naNewBufferSourcePart(NABufferSource* source, NARangei range, NAPtr data){
  #ifndef NDEBUG
    if(source && naIsBufferSourceLimited(source) && !naContainsRangeiRange(naGetBufferSourceLimit(source), range))
      naError("naNewBufferSourcePart", "range overflows the limited range of the source");
  #endif
  NABufferSourcePart* part = naNew(NABufferSourcePart);
  part->source = source ? naRetainBufferSource(source) : NA_NULL;
  part->range = range;
  part->data = data;
  return part;
}



NA_HDEF void naDestructBufferSourcePart(NABufferSourcePart* part){
  if(part->source){
    if(naGetPtrConst(&(part->data))){
      if(part->source->bufdeallocator){
        part->source->bufdeallocator(part->source->data, part->data);
      }else{
        part->source->bufsimpledestructor(naGetPtrMutable(&(part->data)));
      }
    }
    naReleaseBufferSource(part->source);
  }
}



// Returns NA_TRUE if the given part does not store memory.
NA_HDEF NABool naIsBufferSourcePartSparse(const NABufferSourcePart* part){
  return (naGetPtrConst(&(part->data)) == NA_NULL);
}



// Allocates memory for the range defined in part.
NA_HDEF void naAllocateBufferSourcePartMemory(NABufferSourcePart* part){
  #ifndef NDEBUG
    if(!naIsBufferSourcePartSparse(part))
      naError("naAllocateBufferSourcePartMemory", "Part already has memory");
  #endif
  if(part->source){
    if(part->source->bufallocator){
      part->data = part->source->bufallocator(part->source->data, part->range);
    }else{
      part->data = naMakePtrWithDataMutable(naMalloc((NAInt)part->range.length));
    }
  }
}



// Calls the filler of the source descriptor for the range of the given part.
// Hence filling the memory with the desired content.
NA_HDEF void naFillBufferSourcePart(NABufferSourcePart* part, NARangei range){
  if(part->source && part->source->buffiller){
    part->source->buffiller(part->source->data, naGetPtrMutable(&(part->data)), naMakeRangei(range.origin + part->range.origin, range.length));
  }
}


// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////



NA_RUNTIME_TYPE(NABufferPart, naDestructBufferPart, NA_FALSE);



// Creates a memory block with sparse memory.
// A sparse buffer is initialized with a byteoffset of 0. This will possibly
// change when calling naReferenceBufferPart or naFillBufferPart.
NA_HDEF NABufferPart* naNewBufferPartSparse(NABufferSource* source, NARangei range){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(range.length))
      naError("naNewBufferPartSparse", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->byteoffset = 0;
  part->bytesize = range.length;
  part->sourcepart = naNewBufferSourcePart(source, range, naMakePtrNull());
  return part;
}



// Creates a memory block with given data buffer
NA_HDEF NABufferPart* naNewBufferPartData(NABufferSource* source, NARangei range, NAPtr data){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(range.length))
      naError("naNewBufferPartConstData", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->byteoffset = 0;
  part->bytesize = range.length;
  part->sourcepart = naNewBufferSourcePart(source, range, data);
  return part;
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
NA_HDEF void naAllocateBufferPartMemory(NABufferPart* part){
  naAllocateBufferSourcePartMemory(part->sourcepart);
}



// Calls the filler of the source descriptor for the range of the given part.
// Hence filling the memory with the desired content.
NA_HDEF void naFillBufferPart(NABufferPart* part, NARangei range){
  naFillBufferSourcePart(part->sourcepart, range);
}



// The destructor method which will automatically be called by naDelete.
NA_HDEF void naDestructBufferPart(NABufferPart* part){
  naRelease(part->sourcepart);
}



// Returns NA_TRUE if the given part does not store memory.
NA_HDEF NABool naIsBufferPartSparse(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      naCrash("naIsBufferPartSparse", "part is Null pointer");
  #endif
  return naIsBufferSourcePartSparse(part->sourcepart);
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



// This function splits a sparse part such that there exists in the end a
// sparse part having precisely the desired range, possibly surrounded by
// other, newly created sparse parts. At the end of this function, the iterator
// points to that very part.
NA_HDEF void naSplitBufferSparsePart(NABufferIterator* iter, NAInt start, NAInt end){
  NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
  NABufferPart* newpart;

  #ifndef NDEBUG
    if(!part)
      naCrash("naSplitBufferSparsePart", "iterator must not be at initial position");
    if(!naIsBufferPartSparse(part))
      naError("naSplitBufferSparsePart", "current part is not sparse");
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

  NAInt length = end - start;

  // Only split, if the parts bytesize does not already equals the length.
  if(length != part->bytesize){
    NABufferSource* source = part->sourcepart->source;
    NARangei range = part->sourcepart->range;

    if(end < part->bytesize){
      // We need to add a new part at the end
      newpart = naNewBufferPartSparse(source, naMakeRangei(range.origin + end, range.length - end));
      naAddTreeNextMutable(&(iter->partiter), newpart);
    }

    if(start > 0){
      // We need to add a new part at the beginning.
      newpart = naNewBufferPartSparse(source, naMakeRangei(range.origin, start));
      naAddTreePrevMutable(&(iter->partiter), newpart);
      part->sourcepart->range.origin += start;
      iter->partoffset -= start;
    }

    part->sourcepart->range.length = length;
    part->bytesize = length;
  }
}



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
