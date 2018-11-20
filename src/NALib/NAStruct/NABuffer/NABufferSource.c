
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"


// //////////////////////////////////////
// Buffer Source
// //////////////////////////////////////


// The other flag NA_BUFFER_SOURCE_RANGE_LIMITED is defined in NABuffer.h
#define NA_BUFFER_SOURCE_BUFFER           0x02



NA_HAPI void naDeallocBufferSource(NABufferSource* source);
NA_RUNTIME_TYPE(NABufferSource, naDeallocBufferSource, NA_TRUE);



// Creates a buffer source with the given descriptor.
NA_HIDEF NABufferSource* naNewBufferSource(NABufferSourceDescriptor descriptor){
  NABufferSource* source = naNew(NABufferSource);
  source->desc = descriptor;    // the whole struct is copied.
  return source;
}



NA_HDEF void naDeallocBufferSource(NABufferSource* source){
  if(source->desc.destructor){
    source->desc.destructor(source->desc.data);
  }
}



//// NAMutator used in the naNewBufferSourceWithBuffer function.
//NA_HDEF void naDeallocBufferSourceBuffer(NABufferIterator* iter){
//  naRelease(naGetBufferIteratorBufferMutable(iter));
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
////
//
////
////// Calls the filler of the source descriptor for the range of the given part.
////// Hence filling the memory with the desired content.
////void naFillBufferSourcePart(NABufferSource* source, NABufferPart* part){
////  #ifndef NDEBUG
////    if(naIsBufferSourceLimited(source)){
////      if(!naContainsRangeiRange(naGetBufferSourceLimit(source), naGetBufferPartRange(part)))
////        naError("naFillBufferSourcePart", "part overflows the limited range of the source");
////    }
////  #endif
////  // desc.filler can be null for non-secure memory as source
////  if(source && source->desc.filler){
////    source->desc.filler(source->desc.data, naGetBufferPartBaseDataPointerMutable(part), naGetBufferPartRange(part));
////  }
////}
////





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
