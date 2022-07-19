
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif



// The meaning of sourceOffset and blockOffset: Quering the memBlock with
// blockOffset is the same as quering the source buffer with sourceOffset.
struct NABufferPart{
  NABufferSource*     source;       // The referenced source.
  NAInt               sourceOffset; // The source offset of the first byte
  size_t              byteSize;     // The number of bytes referenced.
  size_t              blockOffset;  // The byte offset in the block.
  NAMemoryBlock*      memBlock;     // The referenced memory block.
};



NA_HIDEF NAInt na_GetBufferPartNormedStart(NAInt start){
  NAInt signShift = (start < 0);   // Note that (start < 0) either results in 0 or 1.
  return ((((start + signShift) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - signShift) * NA_INTERNAL_BUFFER_PART_BYTESIZE);
  // Examples explain best how this behaves (assume default part size to be 10):
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



NA_HIDEF NAInt na_GetBufferPartNormedEnd(NAInt end){
  // Return the end coordinate, such that max (= end-1) is within the normed
  // range.
  return na_GetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



NA_HIDEF NABufferSource* na_GetBufferPartSource(const NABufferPart* part){
  #if NA_DEBUG
    if(!part)
      naCrash("part is Null");
  #endif
  return part->source;
}



NA_HIDEF NAInt na_GetBufferPartSourceOffset(const NABufferPart* part){
  #if NA_DEBUG
    if(!part)
      naCrash("part is Null");
    if(!part->source)
      naError("part has no source");
  #endif
  return part->sourceOffset;
}



NA_HIDEF size_t na_GetBufferPartByteSize(const NABufferPart* part){
#if NA_DEBUG
  if(!part)
    naCrash("part is Null");
#endif
  return part->byteSize;
}



NA_HIDEF NABool na_IsBufferPartSparse(const NABufferPart* part){
  #if NA_DEBUG
    if(!part)
      naCrash("part is Null");
  #endif
  return (part->memBlock == NA_NULL);
}



NA_HIDEF NAMemoryBlock* na_GetBufferPartMemoryBlock(const NABufferPart* part){
  #if NA_DEBUG
    if(!part)
      naCrash("part is Null");
    if(na_IsBufferPartSparse(part))
      naError("part is sparse");
  #endif
  return part->memBlock;
}



// Enlarges a sparse buffer part with a given number of bytes at the start and
// at the end.
NA_HIDEF void na_EnlargeBufferPart(NABufferPart* part, size_t bytesAtStart, size_t bytesAtEnd){
  #if NA_DEBUG
  if(!part)
    naCrash("part is Null");
  if(!na_IsBufferPartSparse(part))
    naError("part is not sparse");
#endif
  part->byteSize += bytesAtStart + bytesAtEnd;
  part->sourceOffset -= bytesAtStart;
}



// Returns a the number of bytes left in this part at given offet.
NA_HIDEF size_t na_GetBufferPartRemainingBytes(NABufferIterator* iter){
  NABufferPart* part;
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(na_IsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
    if(iter->partOffset < 0)
      naError("part offset is negative");
  #endif
  part = na_GetBufferPart(iter);
  return part->byteSize - (size_t)iter->partOffset;
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF const void* na_GetBufferPartDataPointerConst(NABufferIterator* iter){
  NABufferPart* part;
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(na_IsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
    if(iter->partOffset < 0)
      naError("part offset is negative");
  #endif
  part = na_GetBufferPart(iter);
  return na_GetMemoryBlockDataPointerConst(part->memBlock, part->blockOffset + (size_t)iter->partOffset);
}



// Returns a pointer to the raw data of this buffer part, given its offset.
NA_HIDEF void* na_GetBufferPartDataPointerMutable(NABufferIterator* iter){
  NABufferPart* part;
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null pointer");
    if(na_IsBufferIteratorSparse(iter))
      naError("buffer part is sparse");
    if(iter->partOffset < 0)
      naError("part offset is negative");
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
