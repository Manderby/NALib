
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



NA_HDEF NABuffer* naGetBufferSourceUnderlyingBuffer(NABufferSource* source){
  return source->buffer;
}



// Returns NA_TRUE if the range is a valid limiting range.
NA_HDEF NABool naIsBufferSourceLimited(const NABufferSource* source){
  return (source->flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
}



// This function returns a memory block which contains the desired sourceoffset.
// The sourceoffset parameter is given in source coordinates (can be negative).
// The parameter blockoffset returns the (always positive) offset in the
// returned memory block which corresponds to the given sourceoffset.
NA_HDEF NAMemoryBlock* naPrepareBufferSource(NABufferSource* source, NAInt sourceoffset, NAInt* blockoffset, NAInt* blocksize){
  NABufferPart* preparedpart;
  #ifndef NDEBUG
    if(naIsBufferSourceLimited(source) && !naContainsRangeiOffset(source->limit, sourceoffset))
      naError("naPrepareBufferSource", "offset is not in source limits");
  #endif

  NABuffer* sourcebuffer = naGetBufferSourceUnderlyingBuffer(source);
  if(sourcebuffer){
    // We have an underlying buffer and hence use it to find or create a
    // suitable memory block by recursively preparing the underlying buffer.
    NABufferIterator iter = naMakeBufferModifier(sourcebuffer);
    NABool found = naLocateBuffer(&iter, sourceoffset);
    if(!found){
      // If we haven't found a suitable part, we must create a new one.
      naEnsureBufferRange(sourcebuffer, sourceoffset, sourceoffset + 1);
      // We now know that the new byte must either be at the beginning or
      // the end.
      if(sourcebuffer->range.origin == sourceoffset){
        naLocateBufferFirstPart(&iter);
      }else{
        naLocateBufferLastIndex(&iter);
      }
      #ifndef NDEBUG
        if(naGetBufferLocation(&iter) != sourceoffset)
          naError("naPrepareBufferSource", "unsuccessfully enlarged buffer");
      #endif
    }
    // Now we can be sure that the buffer iterator is at a part containing
    // sourceoffset.
    naPrepareBuffer(&iter, 1, NA_FALSE);
    preparedpart = naGetBufferPart(&iter);
    *blockoffset = naGetBufferPartBlockOffset(preparedpart) + naGetBufferIteratorPartOffset(&iter);
    *blocksize = naGetBufferPartByteSize(preparedpart) - naGetBufferIteratorPartOffset(&iter);
    naClearBufferIterator(&iter);

  }else{
    // We have no underlying buffer. Therefore we have no information about
    // any previous memory block used by this source and we create new blocks.
    //
    // We find out a suitable range to contain at least 1 byte at the desired
    // offset. This will result in a range which has a byte cout of
    // NA_INTERNAL_BUFFER_PART_BYTESIZE
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
    naSetBufferPartMemoryBlock(preparedpart, naNewMemoryBlock(naGetBufferPartByteSize(preparedpart)));
    if(source->buffiller){
      source->buffiller(naGetMemoryBlockDataPointerMutable(naGetBufferPartMemoryBlock(preparedpart), 0), normedrange, naGetBufferPartSource(preparedpart)->data);
    }
    *blockoffset = sourceoffset - normedrange.origin;
    *blocksize = naGetBufferPartByteSize(preparedpart) - *blockoffset;

  }

  #ifndef NDEBUG
    if(*blockoffset < 0)
      naError("naPrepareBufferSource", "returned blockoffset should be >= 0");
  #endif
  return naGetBufferPartMemoryBlock(preparedpart);
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
