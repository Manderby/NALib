
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



struct NABufferPart{
  NABufferSource*     source;       // The referenced source.
  NAInt               sourceoffset; // The source offset of the first byte
  NAInt               bytesize;     // The number of bytes referenced.
  NAInt               blockoffset;  // The byte offset in the block.
  NAMemoryBlock*      memblock;     // The referenced memory block.
};
NA_EXTERN_RUNTIME_TYPE(NABufferPart);



NA_HIDEF NABufferSource* naGetBufferPartSource(const NABufferPart* part){
  return part->source;
}



NA_HIDEF NAInt naGetBufferPartByteSize(const NABufferPart* part){
  return part->bytesize;
}



NA_HIDEF NAMemoryBlock* naGetBufferPartMemoryBlock(const NABufferPart* part){
  return part->memblock;
}



NA_HIDEF NABool naIsBufferPartSparse(const NABufferPart* part){
  return (part->memblock == NA_NULL);
}



// Enlarges a sparse buffer part with a given number of bytes at the start and
// at the end.
NA_HIDEF void naEnlargeBufferPart(NABufferPart* part, NAInt bytesatstart, NAInt bytesatend){
  #ifndef NDEBUG
    if(!naIsBufferPartSparse(part))
      naError("naEnlargeBufferPart", "part is not sparse");
  #endif
  part->bytesize += bytesatstart + bytesatend;
  part->sourceoffset -= bytesatstart;
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF const void* naGetBufferPartDataPointerConst(NABufferIterator* iter){
  #ifndef NDEBUG
    if(!iter)
      naCrash("naGetBufferPartDataPointerConst", "iterator is Null pointer");
    if(naIsBufferIteratorSparse(iter))
      naError("naGetBufferPartDataPointerConst", "buffer part is sparse");
  #endif
  NABufferPart* part = naGetBufferPart(iter);
  return naGetMemoryBlockDataPointerConst(part->memblock, part->blockoffset + iter->partoffset);
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF void* naGetBufferPartDataPointerMutable(NABufferIterator* iter){
  #ifndef NDEBUG
    if(!iter)
      naCrash("naGetBufferPartDataPointerConst", "iterator is Null pointer");
    if(naIsBufferIteratorSparse(iter))
      naError("naGetBufferPartDataPointerConst", "buffer part is sparse");
  #endif
  NABufferPart* part = naGetBufferPart(iter);
  return naGetMemoryBlockDataPointerMutable(part->memblock, part->blockoffset + iter->partoffset);
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
