
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



// Flags for the buffer source:
#define NA_BUFFER_SOURCE_RANGE_LIMITED        0x01
#define NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE 0x80



struct NABufferSource{
  NABufferFiller    buffiller;      // Fill function filling memory.
  NABuffer*         buffer;         // The underlying buffer, if any.
  void*             data;           // data sent to filler and destructor.
  NAMutator         datadestructor; // Data destructor.
  NAUInt            flags;          // Flags for the source
  NARangei          limit;          // Source limit (used if flag set)
};
NA_EXTERN_RUNTIME_TYPE(NABufferSource);



NA_DEF void naSetBufferSourceData(NABufferSource* source, void* data, NAMutator datadestructor){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->data = data;
  source->datadestructor = datadestructor;
}



NA_DEF void naSetBufferSourceLimit(NABufferSource* source, NARangei limit){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED)
      naError("Source already has a limit");
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
  source->limit = limit;
}



NA_HHDEF NABuffer* na_GetBufferSourceUnderlyingBuffer(NABufferSource* source){
  return source->buffer;
}



// Returns NA_TRUE if the range is a valid limiting range.
NA_HHDEF NABool na_IsBufferSourceLimited(const NABufferSource* source){
  return (NABool)(source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
}



NA_HHDEF NARangei na_GetBufferSourceLimit(const NABufferSource* source){
  #ifndef NDEBUG
    if(!na_IsBufferSourceLimited(source))
      naError("source is not limited");
  #endif
  return source->limit;
}



NA_HHDEF void na_FillSourceBuffer(const NABufferSource* source, void* dst, NARangei range){
  if(source && source->buffiller){
    source->buffiller(dst, range, source->data);
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
