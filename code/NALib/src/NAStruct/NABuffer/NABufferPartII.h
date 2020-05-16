
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
      naError("part is not sparse");
  #endif
  part->bytesize += bytesatstart + bytesatend;
  part->sourceoffset -= bytesatstart;
}



// Returns a the number of bytes left in this part at given offet.
NA_HIDEF NAInt naGetBufferPartRemainingBytes(NABufferIterator* iter){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(naIsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
  #endif
  part = naGetBufferPart(iter);
  return part->bytesize - iter->partoffset;
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF const void* naGetBufferPartDataPointerConst(NABufferIterator* iter){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(naIsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
  #endif
  part = naGetBufferPart(iter);
  return naGetMemoryBlockDataPointerConst(part->memblock, part->blockoffset + iter->partoffset);
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF void* naGetBufferPartDataPointerMutable(NABufferIterator* iter){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(naIsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
  #endif
  part = naGetBufferPart(iter);
  return naGetMemoryBlockDataPointerMutable(part->memblock, part->blockoffset + iter->partoffset);
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
