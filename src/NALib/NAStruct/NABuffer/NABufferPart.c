
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "NABuffer.h"



// Buffers are implemented in an highly granular way. Each buffer is a series
// of buffer parts stitched together. Consider the following three sentences
// as buffers:
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf1 = |     This is     |      very       |    exciting     |
//                 +-----------------+-----------------+-----------------+
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf2 = | Don't question  |       her       |    authority    |
//                 +-----------------+-----------------+-----------------+
//
//                 +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
// NABuffer buf3 = | You can see ads |    everywhere   |   these days    |
//                 +-----------------+-----------------+-----------------+
//
// Let's look at a single buffer part in more detail: The buffer part
// containing the word "everywhere" looks like this:
//
// +==== BUFFER PART ====+
// | blockoffset :  0    |
// | bytesize    : 10    |       +==== MEMORY BLOCK ====+
// | memblock ---------------->  |      everywhere      |
// +=====================+       +======================+
//
// It stores its content in a memory block which is self-organizing. This means
// the memory block knows how and where to store or deallocate the memory
// necessary to store the word "everywhere" and knows how many instances still
// need the memory block to exist.
//
// A buffer part is only referencing such a memory block. It also defines
// which bytes of that memory block are used. In this case 10 bytes starting
// with index 0. 
//
// Now, consider the words "very" and "her" which both are substrings of the
// word "everywhere" (e-VERY-w-HER-e). The buffer parts can look like this:
//
// +==== BUFFER PART ====+
// | blockoffset :  1    |
// | bytesize    :  4    |
// | memblock --------------\
// +=====================+   \
//                            -->  +==== MEMORY BLOCK ====+
// +==== BUFFER PART ====+         |      everywhere      |
// | blockoffset :  6    |    -->  +======================+
// | bytesize    :  3    |   /
// | memblock --------------/
// +=====================+
//
// As you can see, the buffer parts share the same memory block. It is
// important to understand that it is the referenced memery blocks which
// are shared amongst NABuffers, NOT the buffer parts!
//
// Now consider the following C-like string written in source code and how
// we easily turn that into an NABuffer:
//
// const char* mystring = "This is an exciting trend these days.";
// NABuffer* buf4 = naNewBufferWithConstData(mystring, strlen(mystring));
//
// This results in a buf4 containing exactly one buffer part:
//
// +==== BUFFER PART ====+
// | blockoffset :  0    |
// | bytesize    : 37    |       +============ MEMORY BLOCK =============+
// | memblock ---------------->  | This is an exciting trend these days. |
// +=====================+       +=======================================+
//
// Note that the data stored in mystring will not be copied but referenced
// and the memory block remembers that this was a const array and therefore
// will NOT try to deallocate it if it is no longer in use.
//
// Going back to buf1, we can show the buffer a little more in detail:
//
//                 +=== BUFFER PART ==+=== BUFFER PART ==+=== BUFFER PART ==+
// NABuffer buf1 = |     This is      |       very       |     exciting.    |
//                 | blockoffset :  0 | blockoffset :  1 | blockoffset : 12 |
//                 | bytesize    :  7 | bytesize    :  4 | bytesize    :  8 |
//               +-- memblock         | memblock  --+    | memblock  --+    |
//               | +------------------+-------------|----+-------------|----+
//               |                                  |                  |
//               |    +==== MEMORY BLOCK ====+      |                  |
//               |    |      everywhere      | <----+                  |
//               |    +======================+                         |
//               |                                                     |
//               |       +============ MEMORY BLOCK =============+     |
//               +-----> | This is an exciting trend these days. | <---+
//                       +=======================================+
//
// So far so good but let's go further.
//
// Imagine, buf3 being read from a file. In NALib, the file content stays on
// disk as long as it is not explicitely used. Therefore at the beginning
// when first creating buf3, it looks something like this:
//
//        +==================== BUFFER PART ====================+
// buf3 = |                                                     |
//        +-----------------------------------------------------+
//
// That single buffer part knows that the file content has in total 37 bytes
// but it has not yet allocated a memory block. This is called a "sparse part".
// When reading the file, such sparse parts will subsequentially be turned
// into filled parts. For example when reading the file byte by byte, it
// might looke somewhat like this:
//
//        +==================== BUFFER PART ====================+
//   |    |                                                     |
//   |    +== BUFFER PART ==+=========== BUFFER PART ===========+
//   |    | You can see ads |                                   |
//   |    +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
//   |    | You can see ads |    everywhere   |                 |
//   |    +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
//   V    | You can see ads |    everywhere   |   these days    |
//        +-----------------+-----------------+-----------------+
//
// The obvious advantage of that is that memory is only allocated when really
// needed. This also works when for example only the first few bytes of a
// large file need to be read and the last few bytes. In our little example,
// this might look like this:
//
//        +== BUFFER PART ==+== BUFFER PART ==+== BUFFER PART ==+
//        | You can see ads |                 |   these days    |
//        +-----------------+-----------------+-----------------+
//
// The middle buffer part is sparse. Now, as good as the advantages sound,
// this poses a problem. When looking above, buf1 and buf2 require the middle
// buffer part to be filled with the word "everywhere".
//
// NALib solves this problem automatically by remembering, what the original
// source of a buffer part should be. This is not shown here in the diagrams
// of a buffer part, but whenever a buffer part is sparse, it remembers which
// bytes from which source should be stored in that part. And whenever needed,
// the sparse part will "ask" the source to fill up its own buffer part with
// content, hence creating a memory block which then in turn will finally be
// referenced.
//
// A source can for example be a file, a const char* array or another NABuffer.
// You can create custom sources with naNewBufferSource and the corresponding
// buffers by calling naNewBufferWithCustomSource. 
//


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



// The destructor method which will automatically be called by naRelease.
NA_HDEF void naDestructBufferPart(NABufferPart* part){
  if(part->source){naRelease(part->source);}
  if(part->memblock){naRelease(part->memblock);}
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
