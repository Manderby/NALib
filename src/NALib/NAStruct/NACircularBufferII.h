
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NACircularBuffer.h
// Do not include this file directly! It will automatically be included when
// including "NACircularBuffer.h"


#include "NAMemory.h"


struct NACircularBuffer{
  NAInt space;
  NAInt cur;
  NAInt last;
  void** data;
};


NACircularBuffer* naInitCircularBuffer(NACircularBuffer* buffer, NAInt count){
  buffer->space = count + 1;
  buffer->cur = 0;
  buffer->last = 0;
  buffer->data = naMalloc(sizeof(void*) * buffer->space);
}


void naClearCircularBuffer(NACircularBuffer* buffer){
  naFree(buffer->data);
}


void* naPullCircularBuffer(NACircularBuffer* buffer){
  #ifndef NDEBUG
    if(buffer->last == buffer->cur)
      naError("naNextCircularBuffer", "Buffer is empty");
  #endif
  void* retvalue = buffer->data[buffer->cur];
  buffer->cur = buffer->cur % buffer->space;
  return retvalue;
}


void naPushCircularBuffer(NACircularBuffer* buffer, void* newdata){
  buffer->data[buffer->last] = newdata;
  buffer->last = buffer->last % buffer->space;
  #ifndef NDEBUG
    if(buffer->last == buffer->cur)
      naError("naPushCircularBuffer", "Buffer did just overflow");
  #endif
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
