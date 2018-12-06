
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"

struct NABufferSource{
  NABufferFiller    buffiller;      // Fill function filling memory.
  NABuffer*         buffer;         // The underlying buffer, if any.
  void*             data;           // data sent to filler and destructor.
  NAMutator         datadestructor; // Data destructor.
  NAUInt            flags;          // Flags for the source
  NARangei          limit;          // Source limit (used if flag set)
};

NA_HAPI void naDestructBufferSource(NABufferSource* source);
NA_RUNTIME_TYPE(NABufferSource, naDestructBufferSource, NA_TRUE);



// Flags for the buffer source:
#define NA_BUFFER_SOURCE_RANGE_LIMITED        0x01
#define NA_BUFFER_SOURCE_VOLATILE             0x02
#define NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE 0x80



NA_DEF NABufferSource* naNewBufferSource(NABufferFiller filler, NABuffer* buffer){
  NABufferSource* source = naNew(NABufferSource);

  source->buffiller = filler;
  source->buffer = buffer ? naRetain(buffer) : NA_NULL;
  source->data = NA_NULL;
  source->datadestructor = NA_NULL;
  source->flags = 0;
  source->limit = naMakeRangeiWithStartAndEnd(0, 0);

  return source;
}



NA_HDEF void naDestructBufferSource(NABufferSource* source){
  if(source->datadestructor){source->datadestructor(source->data);}
  if(source->buffer){naRelease(source->buffer);}
  naFree(source);
}



NA_DEF void naSetBufferSourceData(NABufferSource* source, void* data, NAMutator datadestructor){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceData", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->data = data;
  source->datadestructor = datadestructor;
}



NA_DEF void naSetBufferSourceLimit(NABufferSource* source, NARangei limit){
  #ifndef NDEBUG
    if(source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED)
      naError("naSetBufferSourceLimit", "Source already has a limit");
    if(source->flags & NA_BUFFER_SOURCE_DEBUG_FLAG_IMMUTABLE)
      naError("naSetBufferSourceLimit", "Source already used in a buffer. Mayor problems may occur in the future");
  #endif
  source->flags |= NA_BUFFER_SOURCE_RANGE_LIMITED;
  source->limit = limit;
}



NA_HDEF NABool naIsBufferSourceVolatile(const NABufferSource* source){
  return naTestFlagu(source->flags, NA_BUFFER_SOURCE_VOLATILE);
}



NA_DEF void naSetBufferSourceVolatile(NABufferSource* source){
  naSetFlagu(&(source->flags), NA_BUFFER_SOURCE_VOLATILE, NA_TRUE);
}



NA_HDEF NABool naHasBufferSourceUnderlyingBuffer(const NABufferSource* source){
  return (source->buffer != NA_NULL);
}



NA_HDEF NABuffer* naGetBufferSourceUnderlyingBuffer(NABufferSource* source){
  #ifndef NDEBUG
    if(!naHasBufferSourceUnderlyingBuffer(source))
      naError("naHasBufferSourceUnderlyingBuffer", "Source has no underlying buffer");
  #endif
  return source->buffer;
}



// Returns NA_TRUE if the range is a valid limiting range.
NA_HDEF NABool naIsBufferSourceLimited(const NABufferSource* source){
  return (source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
}



// Returns the limit range of this source
NA_HDEF NARangei naGetBufferSourceLimit(NABufferSource* source){
  #ifndef NDEBUG
    if(!naIsBufferSourceLimited(source))
      naError("naGetBufferSourceLimit", "source is not limited");
  #endif
  return source->limit;
}



// This function returns a memory block which contains the desired sourceoffset.
// The sourceoffset parameter is given in source coordinates (can be negative).
// The parameter blockoffset returns the offset in the returned memory block
// which corresponds to the desired sourceoffset.
NA_HDEF NAMemoryBlock* naPrepareBufferSource(NABufferSource* source, NAInt sourceoffset, NAInt* blockoffset){
  NABufferPart* preparedpart;
  #ifndef NDEBUG
    if(naIsBufferSourceLimited(source) && !naContainsRangeiOffset(source->limit, sourceoffset))
      naError("naPrepareBufferSource", "offset is not in source limits");
  #endif

  if(naHasBufferSourceUnderlyingBuffer(source)){
    NABufferIterator bufiter = naMakeBufferModifier(source->buffer);
    // We recursively prepare the first byte of the underlying buffer.
    NABool found = naLocateBuffer(&bufiter, sourceoffset);
    if(!found){
      naEnsureBufferRange(naGetBufferSourceUnderlyingBuffer(source), sourceoffset, sourceoffset + 1);
      found = naLocateBuffer(&bufiter, sourceoffset);
      #ifndef NDEBUG
        if(!found)
          naError("naPrepareBufferSource", "Did not found offset in source buffer");
      #endif
    }
    naPrepareBuffer(&bufiter, 1, NA_FALSE);
    preparedpart = naGetTreeCurMutable(naGetBufferIteratorPartIterator(&bufiter));
    *blockoffset = preparedpart->blockoffset + naGetBufferIteratorPartOffset(&bufiter);
    naClearBufferIterator(&bufiter);

  }else{
    // We have no underlying buffer. We find out a suitable range to contain
    // at least 1 byte at the desired offset.
    NAInt normedstart = naGetBufferPartNormedStart(sourceoffset);
    NAInt normedend = naGetBufferPartNormedEnd(sourceoffset + 1);
    NARangei normedrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);
    if(naIsBufferSourceLimited(source)){
      normedrange = naClampRangeiToRange(normedrange, source->limit);
      #ifndef NDEBUG
        if(!naContainsRangeiOffset(normedrange, sourceoffset))
          naError("naPrepareBufferSource", "limited range can not contain desired offset");
      #endif
    }
    // Now, we create a new sparse buffer and fill it with memory immediately.
    preparedpart = naNewBufferPartSparse(source, normedrange);
    preparedpart->memblock = naNewMemoryBlock(preparedpart->bytesize);
    if(source->buffiller){
      source->buffiller(naGetPtrMutable(&(preparedpart->memblock->data)), normedrange, preparedpart->source->data);
    }
    *blockoffset = sourceoffset - normedrange.origin;

  }

  return preparedpart->memblock;
}




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
