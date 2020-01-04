
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NABuffer.h"



NA_HAPI void naDestructBufferSource(NABufferSource* source);
NA_RUNTIME_TYPE(NABufferSource, naDestructBufferSource, NA_TRUE);



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
