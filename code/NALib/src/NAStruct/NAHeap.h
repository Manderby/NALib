
#ifndef NA_HEAP2_INCLUDED
#define NA_HEAP2_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "../NABase/NABase.h"
#include <stdlib.h>


// A heap is a datastructure, storing arbirtrary elements with a key value.
// The elements are "partially" sorted in a highly efficient and very balanced
// tree-like structure whereas only the element at the root can be accessed by
// the programmer. It is guaranteed that the root element always is the minimal
// or maximal value of the whole heap. To define, what kind of key the heap
// stores and whether it shall be a min- or a max-heap, you use the appropriate
// flags when using naInitHeap.
//
// You use a heap by inserting elements one-by-one in any order. To do so, you
// provide a pointer to your element as well as a pointer to the key value.
// The key value must be of the type you depicted with a flag when creating the
// heap. Each element is only referenced by the pointer, not stored! Therefore
// you must store your elements elsewhere, for example in a simple C-array.
//
// Note that NAHeap allows you to store const and mutable pointers arbitrarily
// mixed but beware that only elements inserted as mutable elements can also
// be accessed as mutable elements. Otherwise, a warning is emitted when
// NA_DEBUG is 1.
//
// The implementation in NALib also allows you to store an optional backPointer
// for each element. The backPointer is a pointer to an (mutable!) int64 where
// the heap will store an implementation-defined value. Using this value, a
// heap can update the internal ordering when the key of an already inserted
// element has changed. You can do this with a call to naUpdateHeapElement.
//
// You are free to use the backPointer on none, all or only certain elements.
// You provide it with a call to naInsertHeapElement. If your heap was created
// without the BACKPOINTER flag, the backPointer is simply ignored. You should
// send NA_NULL nontheless as NALib will emit a warning otherwise if NA_DEBUG
// is 1.
//
// You can always use NA_NULL for the backPointer to not store this value for
// a particular element. If you do store the value, you should probably provide
// a pointer to an int64 which is stored somewhere within the element itself.
// But you may choose otherwise, for example if you yourself have no mutable
// access to the element and therefore can only provide an array of int64.
// Note that, most importantly, under no circumstances, you should *-krrrrt-*
// NOT alter these int64-values by yourself!
//
// A typical application of a heap is to insert a bunch of unsorted elements
// into the heap and removing them out of the heap one by one to get a sorted
// series. A heap is often times used in simulations with timestamps when an
// action is about to occur. The backPointer implementation is needed for
// example if you store several heaps containing the same elements.
//
// Note that heaps are a very fast datastructure!
// One of the authors favourites!



// The full type definition is in the file "NAHeapII.h"
NA_PROTOTYPE(NAHeap);

// Use the following flags to define what type the key is.
// If this flag is 0 or not present, the heap will use double-value keys.
#define NA_HEAP_USES_DOUBLE_KEY       0x0000
#define NA_HEAP_USES_FLOAT_KEY        0x0001
#define NA_HEAP_USES_INT64_KEY        0x0002
#define NA_HEAP_USES_DATETIME_KEY     0x0003
#define NA_HEAP_DATATYPE_MASK         0x00ff
// Use the following flags to define if the heap shall be a min- or a max-heap.
// If this flag is 0 or not present, the heap will be a min-heap.
#define NA_HEAP_IS_MIN_HEAP           0x0000
#define NA_HEAP_IS_MAX_HEAP           0x0010
// Set this flag if you want your heap elements to store backPointers.
#define NA_HEAP_STORES_BACKPOINTERS   0x0020

// Creates a new heap. The count parameter denotes the number of elements which
// the heap must hold and the flags denote a combination of the macros above.
//
// If count is zero, the heap grows automatically if needed. Grows in power
// of 2. Note that shrinking of the heap does NOT occur automatically. You need
// to call naShrinkHeapIfNecessary for that.
//
// Beware in multithreaded environments that growing and shrinking requires
// memory allocation and deallocation.
NA_API NAHeap* naInitHeap(NAHeap* heap, size_t count, uint32 flags);

// Clears the given heap. Deallocates all allocated memory.
NA_IAPI void naClearHeap(NAHeap* heap);

// Resets the internal counter to zero. Does not deallocate any memory!
NA_IAPI void naEmptyHeap(NAHeap* heap);

// Returns the number of elements stored
NA_IAPI size_t naGetHeapCount(const NAHeap* heap);

// Returns the maximum number of elements that can be stored. If heap was
// initialized with count 0, that size can change over time and reflects the
// currently reserved count.
NA_IAPI size_t naGetHeapMaxCount(const NAHeap* heap);

// Shrinks the heap if at least three quarters of the heap are unused. Can only
// be called if the initial count given to naInitHeap was negative.
NA_API void naShrinkHeapIfNecessary(NAHeap* heap);

// Adds a new element to the heap. See text above for more information.
NA_IAPI void naInsertHeapElementConst(
  NAHeap*     heap,
  const void* ptr,
  const void* key,
  int64*      backPointer);
NA_IAPI void naInsertHeapElementMutable(
  NAHeap*     heap,
  void*       ptr,
  const void* key,
  int64*      backPointer);

// Returns the root element of the heap.
// The Remove-Function will additionally remove that element such that the
// next one can take its place.
NA_IAPI const void* naGetHeapRootConst(     const NAHeap* heap);
NA_IAPI void*       naGetHeapRootMutable(   const NAHeap* heap);
NA_IAPI const void* naRemoveHeapRootConst(        NAHeap* heap);
NA_IAPI void*       naRemoveHeapRootMutable(      NAHeap* heap);

// Returns the key of the root element.
NA_IAPI const void* naGetHeapRootKey(const NAHeap* heap);

// The following function can only be used when a backPointer is stored:
//
// Use this function if the key of an element inside the heap has changed.
// The heap will re-order the element correctly. To identify the element, you
// need to provide the backPointer of the element which you have stored when
// inserting the element into the heap with naInsertHeapElement.
//
// If the stored backPointer is 0, the element is considered to not be in the
// heap.
NA_IAPI void naUpdateHeapElement(           NAHeap* heap, int64 backPointer);

// If you have such a backPointer, you can also remove an element.
NA_IAPI const void* naRemoveHeapPosConst(   NAHeap* heap, int64 backPointer);
NA_IAPI void*       naRemoveHeapPosMutable( NAHeap* heap, int64 backPointer);




// Inline implementations are in a separate file:
#include "Core/NAHeap/NAHeapII.h"





#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_HEAP2_INCLUDED



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
