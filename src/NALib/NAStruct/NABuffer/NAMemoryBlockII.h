
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



struct NAMemoryBlock{
  NAPtr            data;
  NAMutator        destructor;
  #ifndef NDEBUG
    NAInt          bytesize;
  #endif
};
NA_EXTERN_RUNTIME_TYPE(NAMemoryBlock);



NA_HIDEF NAMemoryBlock* naNewMemoryBlock(NAInt bytesize){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(bytesize <= 0)
      naError("naNewMemoryBlock", "bytesize invalid");
  #endif
  block = naNew(NAMemoryBlock);
  block->data = naMakePtrWithDataMutable(naMalloc(bytesize));
  block->destructor = (NAMutator)naFree;
  #ifndef NDEBUG
    block->bytesize = bytesize;
  #endif
  return block;
}



NA_HIDEF NAMemoryBlock* naNewMemoryBlockWithData(NAPtr data, NAInt bytesize, NAMutator destructor){
  NAMemoryBlock* block;
  #ifndef NDEBUG
    if(bytesize <= 0)
      naError("naNewMemoryBlock", "bytesize invalid");
    if(naIsPtrConst(&data) && destructor != NA_NULL)
      naError("naNewMemoryBlock", "having a destructor for const data is probably wrong.");
  #else
    NA_UNUSED(bytesize);
  #endif
  block = naNew(NAMemoryBlock);
  block->data = data;
  block->destructor = destructor;
  #ifndef NDEBUG
    block->bytesize = bytesize;
  #endif
  return block;
}



NA_HIDEF const void* naGetMemoryBlockDataPointerConst(NAMemoryBlock* block, NAInt indx){
  return (const void*)&(((const NAByte*)naGetPtrConst(&(block->data)))[indx]);
}



NA_HIDEF void* naGetMemoryBlockDataPointerMutable(NAMemoryBlock* block, NAInt indx){
  return (void*)&(((const NAByte*)naGetPtrMutable(&(block->data)))[indx]);
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
