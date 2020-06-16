
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
void naPushCircularBuffer(NACircularBuffer* buffer, void* newData);




// Inline implementations are in a separate file:
#include "NAStruct/NACircularBufferII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_CIRCULAR_BUFFER_INCLUDED



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
