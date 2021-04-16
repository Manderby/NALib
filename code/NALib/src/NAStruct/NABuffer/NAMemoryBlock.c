
#include "../../NABuffer.h"



NA_HAPI void na_DestructMemoryBlock(NAMemoryBlock* block);
NA_RUNTIME_TYPE(NAMemoryBlock, na_DestructMemoryBlock, NA_TRUE);



NA_HDEF NAMemoryBlock* na_NewMemoryBlock(size_t byteSize){
  #if NA_DEBUG
    if(byteSize == 0)
      naError("byteSize is zero");
  #endif

  NAMemoryBlock* block = naNew(NAMemoryBlock);
  block->data = naMakePtrWithDataMutable(naMalloc(byteSize));
  block->destructor = (NAMutator)naFree;
  #if NA_DEBUG
    block->byteSize = byteSize;
  #endif
  return block;
}



NA_HDEF NAMemoryBlock* na_NewMemoryBlockWithData(NAPtr data, size_t byteSize, NAMutator destructor){
  NAMemoryBlock* block;
  #if NA_DEBUG
    if(!naIsPtrValid(data))
      naError("Invalid data");
    if(byteSize == 0)
      naError("byteSize is zero");
    if(naIsPtrConst(data) && destructor != NA_NULL)
      naError("having a destructor for const data probably is not correct.");
  #else
    NA_UNUSED(byteSize);
  #endif
  block = naNew(NAMemoryBlock);
  block->data = data;
  block->destructor = destructor;
  #if NA_DEBUG
    block->byteSize = byteSize;
  #endif
  return block;
}



NA_HDEF void na_DestructMemoryBlock(NAMemoryBlock* block){
  if(block->destructor){
    block->destructor(naGetPtrMutable(block->data));
  }
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-comme3rcial, and by any
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
