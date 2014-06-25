
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_HEAP_INCLUDED
#define NA_HEAP_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"


typedef struct NAHeap NAHeap;
struct NAHeap{
  void* data;
  NAInt count;
  #ifndef NDEBUG
    NAInt maxcount;
  #endif
  NAInt (*movedown)(void*   data, const void* key, NAInt curindex);
  NAInt (*moveup)  (NAHeap* heap, const void* key, NAInt curindex);
};

// Use this flag in the constructor if you want the heap to be a max-heap.
// If this flag is not present, the heap will be a min-heap. This flag
// is currently not working.
#define NA_HEAP_IS_MAX_HEAP       0x0001
// Use this flag in the constructor if you want the heap to use int-keyvalues.
// If this flag is not present, the heap will use double-values.
#define NA_HEAP_USES_INT_KEY      0x0002

// Creates a new heap. The count parameter denotes the number of elements which
// the heap must hold and the flags denote a combination of the macros above.
NA_API NAHeap* naCreateHeap (NAHeap* heap, NAInt count, NAInt flags);

// Clears or deletes the given heap.
NA_API void    naClearHeap  (NAHeap* heap);
NA_API void    naDestroyHeap(NAHeap* heap);

// Adds a new element to the heap.
// Provide a pointer to your element as well as a pointer to the key value. The
// key value must be of the type you depicted with a flag when creating the
// heap. Each element is only referenced by the pointer, not stored! Therefore
// you must store your elements elsewhere, for example in a simple C-array.
//
// The backpointer argument depicts an optional pointer to an NAInt where the
// heap will store an implementation-defined value which will allow the heap
// structure to identify the element quickly when you alter the key value of
// an element which already is in the heap with naUpdateHeapElement. See below.
//
// You can use NA_NULL for the backpointer to not store this value. If you do
// store the value, you should probably provide a pointer to an NAInt which is
// stored somewhere within the element itself. You should NOT alter this value
// by yourself! Feel free to use the backpointer on none, all or only certain
// elements.
//
// Note that this API might slightly change in the near future. The backpointer
// might be implemented with a flag to allow the newptr argument to be const.
NA_API void naInsertHeapElement(NAHeap* heap,
                                  void* newptr,
                            const void* newkey,
                                 NAInt* newbackpointer);

// Returns the root element of the heap.
// The Remove-Function will additionally remove the root element of the heap. 
NA_API void*       naGetHeapRoot   (const NAHeap* heap);
NA_API void*       naRemoveHeapRoot(      NAHeap* heap);

// Use this function if the key of an element inside the heap has changed.
// The heap will re-order the element correctly. To identify the element, you
// need to provide the backpointer of the element which you have stored when
// inserting the element into the heap with naInsertHeapElement.
NA_API void        naUpdateHeapElement(   NAHeap* heap,
                                            NAInt backpointer);

// Returns NA_TRUE if the heap does not contain any elements.
NA_API NABool      naIsHeapEmpty   (const NAHeap* heap);







#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_HEAP_INCLUDED

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
