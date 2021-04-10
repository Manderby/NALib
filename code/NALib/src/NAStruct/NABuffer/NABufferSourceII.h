
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



// Flags for the buffer source
#define NA_BUFFER_SOURCE_RANGE_LIMITED        0x01



struct NABufferSource{
  // automatic reference counting implemented as runtime type.
  NABufferFiller    bufFiller;      // Fill function filling memory.
  NABuffer*         cache;          // The underlying cache, if any.
  void*             data;           // data sent to filler and destructor.
  NAMutator         dataDestructor; // Data destructor.
  uint32            flags;          // Flags for the source
  NARangei          limit;          // Range limit (used if flag set)
};



NA_DEF void naSetBufferSourceData(NABufferSource* source, void* data, NAMutator dataDestructor){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
    if(!data)
      naError("data is Null");
    if(!source->bufFiller)
      naError("Adding data to a source without filler function is useless.");
    if(source->data)
      naError("Source already has data");
  #endif
  source->data = data;
  source->dataDestructor = dataDestructor;
}



NA_DEF void naSetBufferSourceLimit(NABufferSource* source, NARangei limit){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
    if(source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED)
      naError("Source already has a limit");
    if(!naIsRangeiUseful(limit))
      naError("Given limit is not useful");
  #endif
  source->flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
  source->limit = limit;
}



NA_HIDEF NABool na_HasBufferSourceCache(NABufferSource* source){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
  #endif
  return source->cache != NA_NULL;
}



NA_HIDEF NABuffer* na_GetBufferSourceCache(NABufferSource* source){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
    if(!na_HasBufferSourceCache(source))
      naError("source has no cache");
  #endif
  return source->cache;
}



// Returns NA_TRUE if the range is a valid limiting range.
NA_HIDEF NABool na_HasBufferSourceLimit(const NABufferSource* source){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
  #endif
  return (NABool)(source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
}



NA_HIDEF NARangei na_GetBufferSourceLimit(const NABufferSource* source){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
    if(!na_HasBufferSourceLimit(source))
      naError("source is not limited");
  #endif
  return source->limit;
}



NA_HIDEF void na_FillBufferSourceMemory(const NABufferSource* source, void* dst, NARangei range){
  #ifndef NDEBUG
    if(!source)
      naCrash("Source is Null");
    if(!dst)
      naCrash("dst is Null");
    if(!naIsRangeiUseful(range))
      naError("range is not useful");
    if(na_HasBufferSourceLimit(source) && !naEqualRangei(naMakeRangeiWithRangeIntersection(range, source->limit), range))
      naError("range is out of limit");
  #endif
  if(source && source->bufFiller){
    source->bufFiller(dst, range, source->data);
  }
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
