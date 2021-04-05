
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



struct NABufferPart{
  NABufferSource*     source;       // The referenced source.
  NAInt               sourceOffset; // The source offset of the first byte
  size_t              byteSize;     // The number of bytes referenced.
  size_t              blockOffset;  // The byte offset in the block.
  NAMemoryBlock*      memBlock;     // The referenced memory block.
};



NA_HIDEF NABufferSource* na_GetBufferPartSource(const NABufferPart* part){
  return part->source;
}



NA_HIDEF size_t na_GetBufferPartByteSize(const NABufferPart* part){
  return part->byteSize;
}



NA_HIDEF NAMemoryBlock* na_GetBufferPartMemoryBlock(const NABufferPart* part){
  return part->memBlock;
}



NA_HIDEF NABool na_IsBufferPartSparse(const NABufferPart* part){
  return (part->memBlock == NA_NULL);
}



// Enlarges a sparse buffer part with a given number of bytes at the start and
// at the end.
NA_HIDEF void na_EnlargeBufferPart(NABufferPart* part, size_t bytesAtStart, size_t bytesAtEnd){
  #ifndef NDEBUG
    if(!na_IsBufferPartSparse(part))
      naError("part is not sparse");
  #endif
  part->byteSize += bytesAtStart + bytesAtEnd;
  part->sourceOffset -= bytesAtStart;
}



// Returns a the number of bytes left in this part at given offet.
NA_HIDEF size_t na_GetBufferPartRemainingBytes(NABufferIterator* iter){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(na_IsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
  #endif
  part = na_GetBufferPart(iter);
  return part->byteSize - (size_t)iter->partOffset;
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF const void* na_GetBufferPartDataPointerConst(NABufferIterator* iter){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(na_IsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
  #endif
  part = na_GetBufferPart(iter);
  return na_GetMemoryBlockDataPointerConst(part->memBlock, part->blockOffset + (size_t)iter->partOffset);
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF void* na_GetBufferPartDataPointerMutable(NABufferIterator* iter){
  NABufferPart* part;
  #ifndef NDEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(na_IsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
  #endif
  part = na_GetBufferPart(iter);
  return na_GetMemoryBlockDataPointerMutable(part->memBlock, part->blockOffset + (size_t)iter->partOffset);
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
