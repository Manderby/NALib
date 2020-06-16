
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



struct NAMemoryBlock{
  NAPtr            data;
  NAMutator        destructor;
  #ifndef NDEBUG
    NAInt          byteSize;
  #endif
};
NA_EXTERN_RUNTIME_TYPE(NAMemoryBlock);



NA_HIDEF NAMemoryBlock* na_NewMemoryBlock(NAInt byteSize){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(byteSize <= 0)
      naError("byteSize invalid");
  #endif
  block = naNew(NAMemoryBlock);
  block->data = naMakePtrWithDataMutable(naMalloc(byteSize));
  block->destructor = (NAMutator)naFree;
  #ifndef NDEBUG
    block->byteSize = byteSize;
  #endif
  return block;
}



NA_HIDEF NAMemoryBlock* na_NewMemoryBlockWithData(NAPtr data, NAInt byteSize, NAMutator destructor){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(byteSize <= 0)
      naError("byteSize invalid");
    if(naIsPtrConst(data) && destructor != NA_NULL)
      naError("having a destructor for const data is probably wrong.");
  #else
    NA_UNUSED(byteSize);
  #endif
  block = naNew(NAMemoryBlock);
  block->data = data;
  block->destructor = destructor;
  #ifndef NDEBUG
    block->byteSize = byteSize;
  #endif
  return block;
}



NA_HIDEF const void* na_GetMemoryBlockDataPointerConst(NAMemoryBlock* block, NAInt index){
  return (const void*)&(((const NAByte*)naGetPtrConst(block->data))[index]);
}



NA_HIDEF void* na_GetMemoryBlockDataPointerMutable(NAMemoryBlock* block, NAInt index){
  return (void*)&(((const NAByte*)naGetPtrMutable(block->data))[index]);
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
