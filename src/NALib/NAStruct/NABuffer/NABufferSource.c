
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"


// //////////////////////////////////////
// Buffer Source
// //////////////////////////////////////



// Flags for the buffer source:
#define NA_BUFFER_SOURCE_RANGE_LIMITED    0x01
//#define NA_BUFFER_SOURCE_BUFFER           0x02
#define NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE 0x80


// Creates a buffer source with the given descriptor.
NA_HDEF NABufferSource* naCreateBufferSource(void* data, NAMutator datadestructor, NABufferSourceFiller filler){
  NABufferSource* source = naAlloc(NABufferSource);
  naInitRefCount(&(source->refcount));
  source->data = data;
  source->destructor = datadestructor;
  source->datafiller = filler;
  source->dataallocator = NA_NULL;
  source->datadeallocator = NA_NULL;
  source->flags = 0;
  source->limit = naMakeRangeiWithStartAndEnd(0, 0);
  return source;
}



NA_HDEF NABufferSource* naRetainBufferSource(NABufferSource* source){
  #ifndef NDEBUG
    source->flags |= NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE;
  #endif
  return (NABufferSource*)naRetainRefCount(&(source->refcount));
}



NA_HDEF void naDeallocBufferSource(NABufferSource* source){
  if(source->destructor){source->destructor(source->data);}
  naFree(source);
}



NA_HDEF void naReleaseBufferSource(NABufferSource* source){
  naReleaseRefCount(&(source->refcount), source, source->destructor);
}



NA_HDEF void naSetBufferSourceLimit(NABufferSource* source, NARangei limit){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceLimit", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
  source->limit = limit;
}



NA_HDEF void naSetBufferSourceDataFunctions(NABufferSource* source, NABufferDataAllocator allocator, NABufferDataDeallocator deallocator){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceDataFunctions", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->dataallocator = allocator;
  source->datadeallocator = deallocator;
}



//// NAMutator used in the naNewBufferSourceWithBuffer function.
//NA_HDEF void naDeallocBufferSourceBuffer(NABufferIterator* iter){
//  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
//  naRelease(buffer);
//  naClearBufferIterator(iter);
//  naFree(iter);
//}



//// This is one of the core concepts of buffer sources: Any NABuffer can again
//// be a source. An iterator retains the desired buffer and manages the access.
//NA_HIDEF NABufferSource* naNewBufferSourceWithBuffer(NABuffer* buffer){
//  NABufferSourceDescriptor desc;
//  NABufferIterator* iter;
//
//  naNulln(&desc, sizeof(desc));
//
//  // We store an iterator on the given buffer as the data. We must allocate
//  // space for it which later will be freed when the pointer is not in use
//  // anymore.
//  iter = naAlloc(NABufferIterator);
//  *iter = naMakeBufferModifier(buffer);
//  naRetain(buffer);
//  desc.data = naNewPointerMutable(iter, (NAMutator)naDeallocBufferSourceBuffer);  
//  desc.destructor = (NAMutator)naReleasePointer;
//  desc.flags |= NA_BUFFER_SOURCE_BUFFER;
//  if(naHasBufferFixedRange(buffer)){
//    desc.flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
//    desc.limit = naGetBufferRange(buffer);
//  }
//  
//  // The new buffer source gets created
//  return naNewBufferSource(desc);
//}



//#ifndef NDEBUG
//  // Returns NA_TRUE if the range is a valid limiting range.
//  NA_HIDEF NABool naIsBufferSourceLimited(const NABufferSource* source){
//    return (source->desc.flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
//  }
//#endif



//// Returns the limit range of this source
//NA_HIDEF NARangei naGetBufferSourceLimit(NABufferSource* source){
//  #ifndef NDEBUG
//    if(!naIsBufferSourceLimited(source))
//      naError("naGetBufferSourceLimit", "source is not limited");
//  #endif
//  return source->desc.limit;
//}
//
//
////
////// Returns the source data if there is a source and it is defined to be
////// an NABuffer source. returns NA_NULL otherwise.
////NA_HIDEF NABufferIterator* naGetBufferSourceBufferIterator(NABufferSource* source){
////  if(source && (source->desc.flags & NA_BUFFER_SOURCE_BUFFER)){
////    return naGetPointerMutable(source->desc.data);
////  }else{
////    return NA_NULL;
////  }
////}



// Calls the filler of the source descriptor for the range of the given part.
// Hence filling the memory with the desired content.
//void naFillBufferSourcePart(NABufferSource* source, NABufferPart* part){
//  #ifndef NDEBUG
//    if(naIsBufferSourceLimited(source)){
//      if(!naContainsRangeiRange(naGetBufferSourceLimit(source), naGetBufferPartRange(part)))
//        naError("naFillBufferSourcePart", "part overflows the limited range of the source");
//    }
//  #endif
//  // desc.filler can be null for non-secure memory as source
//  if(source && source->desc.filler){
//    source->desc.filler(source->desc.data, naGetBufferPartBaseDataPointerMutable(part), naGetBufferPartRange(part));
//  }
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
