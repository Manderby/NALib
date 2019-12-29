
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_CIRCULAR_BUFFER_INCLUDED
#define NA_CIRCULAR_BUFFER_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// A circular buffer is a buffer with a maximal number of elements but which
// can be read like a list: First in First out (FIFO). You push elements which
// get appended at the end of the list and you pull elements which are at the
// beginning of the list.
//
// It is called a circular buffer as the buffer is allocated as an array of
// elements but which are wrapped around at the end such that the one element
// after the last one of the array will again be the first one.
//
// This implementation stores void pointers. The implementation allocates
// space for exactly count+1 elements. The additional element is required to
// guarantee proper function of the circular structure.


// The full type definition is in the file "NACircularBufferII.h"
typedef struct NACircularBuffer NACircularBuffer;



// Creates a circular buffer with sufficient space for the given number of
// void-Pointers.
NACircularBuffer* naInitCircularBuffer(NACircularBuffer* buffer, NAInt count);

// Clears the circular buffer. Does not deletes the content-pointers!
void naClearCircularBuffer(NACircularBuffer* buffer);

// Returns the beginning of the filled buffer and moves the buffer forward.
void* naPullCircularBuffer(NACircularBuffer* buffer);

// Puts one element at the tail of the buffer. Does not copy any content, only
// stores the pointer!
void naPushCircularBuffer(NACircularBuffer* buffer, void* newdata);




// Inline implementations are in a separate file:
#include "NAStruct/NACircularBufferII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_CIRCULAR_BUFFER_INCLUDED



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