
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "NABuffer.h"





// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////



struct NABufferPart{
  NABufferSource*     source;       // The referenced source.
  NAInt               sourceoffset; // The source offset of the first byte
  NAInt               blockoffset;  // The byte offset in the block.
  NAInt               bytesize;     // The number of bytes referenced.
  NAMemoryBlock*      memblock;     // The referenced memory block.
};

NA_HAPI void naDestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, naDestructBufferPart, NA_FALSE);



// Creates a memory block with sparse memory.
// A sparse buffer is initialized with a byteoffset of 0. This will possibly
// change when calling naReferenceBufferPart or naFillBufferPart.
NA_HDEF NABufferPart* naNewBufferPartSparse(NABufferSource* source, NARangei sourcerange){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(sourcerange.length))
      naError("naNewBufferPartSparse", "range length is not useful");
    if(!source && sourcerange.origin != 0)
      naError("naNewBufferPartSparse", "origin unequal zero makes no sense without source");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  if(source){
    part->source = naRetain(source);
    part->sourceoffset = sourcerange.origin;
    NABuffer* sourcebuffer = naGetBufferSourceUnderlyingBuffer(source);
    if(sourcebuffer){
      naEnsureBufferRange(sourcebuffer, sourcerange.origin, naGetRangeiEnd(sourcerange));
    }
  }else{
    part->source = NA_NULL;
    part->sourceoffset = 0;
  }
  part->blockoffset = 0;
  part->bytesize = sourcerange.length;
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
  part->memblock = naNewMemoryBlockWithData(naMakePtrWithDataConst(data), bytesize, NA_NULL);
  return part;
}



// Creates a memory block with mutable data
NA_HDEF NABufferPart* naNewBufferPartWithMutableData(void* data, NAInt bytesize, NAMutator destructor){
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(bytesize))
      naError("naNewBufferPartWithMutableData", "bytesize is not useful");
  #endif
  NABufferPart* part = naNew(NABufferPart);
  part->source = NA_NULL;
  part->sourceoffset = 0;
  part->blockoffset = 0;
  part->bytesize = bytesize;
  part->memblock = naNewMemoryBlockWithData(naMakePtrWithDataMutable(data), bytesize, destructor);
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



// The destructor method which will automatically be called by naRelease.
NA_HDEF void naDestructBufferPart(NABufferPart* part){
  if(part->source){naRelease(part->source);}
  if(part->memblock){naRelease(part->memblock);}
}



NA_HDEF NABufferSource* naGetBufferPartSource(const NABufferPart* part){
  return part->source;
}



NA_HDEF NAInt naGetBufferPartByteSize(const NABufferPart* part){
  return part->bytesize;
}



NA_HDEF void naSetBufferPartByteSize(NABufferPart* part, NAInt bytesize){
  part->bytesize = bytesize;
}



NA_HDEF NAInt naGetBufferPartSourceOffset(const NABufferPart* part){
  return part->sourceoffset;
}



NA_HDEF void naSetBufferPartSourceOffset(NABufferPart* part, NAInt sourceoffset){
  part->sourceoffset = sourceoffset;
}



NA_HDEF NAInt naGetBufferPartBlockOffset(const NABufferPart* part){
  return part->blockoffset;
}



NA_HDEF void naSetBufferPartBlockOffset(NABufferPart* part, NAInt blockoffset){
  part->blockoffset = blockoffset;
}



NA_HDEF NAMemoryBlock* naGetBufferPartMemoryBlock(const NABufferPart* part){
  return part->memblock;
}



NA_HDEF void naSetBufferPartMemoryBlock(NABufferPart* part, NAMemoryBlock* memblock){
  part->memblock = memblock;
}



//// Returns the range of the given part.
//NA_HIDEF NARangei naGetBufferPartRange(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartRange", "part is Null pointer");
//  #endif
//  return part->range;
//}



//// Returns the start of the given part range.
//NA_HIDEF NAInt naGetBufferPartStart(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartStart", "part is Null pointer");
//  #endif
//  return part->range.origin;
//}



//// Returns the end of the given part range.
//NA_HIDEF NAInt naGetBufferPartEnd(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naGetBufferPartEnd", "part is Null pointer");
//  #endif
//  return naGetRangeiEnd(part->range);
//}



//NA_HIDEF NABool naContainsBufferPartOffset(const NABufferPart* part, NAInt offset){
//  #ifndef NDEBUG
//    if(!part)
//      naCrash("naContainsBufferPartOffset", "part is Null pointer");
//  #endif
//  return naContainsRangeiOffset(part->range, offset);
//}



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



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HDEF const void* naGetBufferPartDataPointerConst(NABufferIterator* iter){
  #ifndef NDEBUG
    if(!iter)
      naCrash("naGetBufferPartDataPointerConst", "iterator is Null pointer");
    if(naIsBufferPartSparse(iter))
      naError("naGetBufferPartDataPointerConst", "buffer part is sparse");
  #endif
  NABufferPart* part = naGetBufferPart(iter);
  return naGetMemoryBlockDataPointerConst(part->memblock, part->blockoffset + iter->partoffset);
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
