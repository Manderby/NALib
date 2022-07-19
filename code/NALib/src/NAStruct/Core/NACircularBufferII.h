
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
  #if NA_DEBUG
    if(buffer->last == buffer->cur)
      naError("naNextCircularBuffer", "Buffer is empty");
  #endif
  void* retValue = buffer->data[buffer->cur];
  buffer->cur = buffer->cur % buffer->space;
  return retValue;
}


void naPushCircularBuffer(NACircularBuffer* buffer, void* newData){
  buffer->data[buffer->last] = newData;
  buffer->last = buffer->last % buffer->space;
  #if NA_DEBUG
    if(buffer->last == buffer->cur)
      naError("naPushCircularBuffer", "Buffer did just overflow");
  #endif
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
