
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_HEAP_INCLUDED
#define NA_HEAP_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"


// A heap is a datastructure, storing arbirtrary elements with a key value.
// The elements are "partially" sorted in a highly efficient and very balanced
// tree-like structure whereas only the element at the root can be accessed by
// the programmer. It is guaranteed that the root element always is the minimal
// or maximal value of the whole heap. To define, what kind of key the heap
// stores and whether it shall be a min- or a max-heap, you use the appropriate
// flags when using naCreateHeap.
//
// You use a heap by inserting elements one-by-one in any order. To do so, you
// provide a pointer to your element as well as a pointer to the key value.
// The key value must be of the type you depicted with a flag when creating the
// heap. Each element is only referenced by the pointer, not stored! Therefore
// you must store your elements elsewhere, for example in a simple C-array.
//
// Note that NAHeap allows you to store const and mutable pointers arbitrarily
// mixed but beware that only elements inserted as mutable elements can also
// be accessed as mutable elements. Otherwise, a warning is emitted when NDEBUG
// is undefined.
//
// The implementation in NALib also allows you to store an optional backpointer
// for each element. The backpointer is a pointer to an (mutable!) NAInt where
// the heap will store an implementation-defined value. Using this value, a
// heap can update the internal ordering when the key of an already inserted
// element has changed. You can do this with a call to naUpdateHeapElement.
//
// You are free to use the backpointer on none, all or only certain elements.
// You provide it with a call to naInsertHeapElement. If your heap was created
// without the BACKPOINTER flag, the backpointer is simply ignored. You should
// send NA_NULL nontheless as NALib will emit a warning otherwise if NDEBUG is
// undefined.
//
// You can always use NA_NULL for the backpointer to not store this value for
// a particular element. If you do store the value, you should probably provide
// a pointer to an NAInt which is stored somewhere within the element itself.
// But you may choose otherwise, for example if you yourself have no mutable
// access to the element and therefore can only provide an array of NAInt.
// Note that, most importantly, under no circumstances, you should *-krrrrt-*
// NOT alter these NAInt-values by yourself!
//
// A typical application of a heap is to insert a bunch of unsorted elements
// into the heap and removing them out of the heap one by one to get a sorted
// series. A heap is often times used in simulations with timestamps when an
// action is about to occur. The backpointer implementation is needed for
// example if you store several heaps containing the same elements.
//
// Note that heaps are a very fast datastructure! One of the authors favourites!

typedef struct NAHeap NAHeap;

// Use the following flags to define what type the key is.
// If this flag is 0 or not present, the heap will use double-value keys.
#define NA_HEAP_USES_DOUBLE_KEY       0x0000
#define NA_HEAP_USES_FLOAT_KEY        0x0001
#define NA_HEAP_USES_INT_KEY          0x0002
#define NA_HEAP_DATATYPE_MASK         0x0003
// Use the following flags to define if the heap shall be a min- or a max-heap.
// If this flag is 0 or not present, the heap will be a min-heap.
#define NA_HEAP_IS_MIN_HEAP           0x0000
#define NA_HEAP_IS_MAX_HEAP           0x0004
// Set this flag if you want your heap elements to store backpointers.
#define NA_HEAP_STORES_BACKPOINTERS   0x0008

// Creates a new heap. The count parameter denotes the number of elements which
// the heap must hold and the flags denote a combination of the macros above.
NA_API NAHeap* naCreateHeap   (NAHeap* heap, NAInt count, NAInt flags);

// Clears or deletes the given heap.
NA_IAPI void   naClearHeap    (NAHeap* heap);
NA_IAPI void   naDestroyHeap  (NAHeap* heap);

// Empties the heap without deallocating the memory
NA_IAPI void   naEmptyHeap    (NAHeap* heap);

// Returns the number of elements stored
NA_IAPI NAUInt naGetHeapCount (const NAHeap* heap);

// Adds a new element to the heap. See text above for more information.
NA_IAPI void naInsertHeapElementConst(    NAHeap* heap,
                                      const void* ptr,
                                      const void* key,
                                           NAInt* backpointer);
NA_IAPI void naInsertHeapElementMutable(  NAHeap* heap,
                                            void* ptr,
                                      const void* key,
                                           NAInt* backpointer);

// Returns the root element of the heap.
// The Remove-Function will additionally remove that element such that the
// next one can take its place.
NA_IAPI const void* naGetHeapRootConst(     const NAHeap* heap);
NA_IAPI void*       naGetHeapRootMutable(   const NAHeap* heap);
NA_IAPI const void* naRemoveHeapRootConst(        NAHeap* heap);
NA_IAPI void*       naRemoveHeapRootMutable(      NAHeap* heap);

// Returns the key of the root element.
NA_IAPI const void* naGetHeapRootKey(const NAHeap* heap);

// The following function can only be used when a backpointer is stored:
//
// Use this function if the key of an element inside the heap has changed.
// The heap will re-order the element correctly. To identify the element, you
// need to provide the backpointer of the element which you have stored when
// inserting the element into the heap with naInsertHeapElement.
//
// If the stored backpointer is 0, the element is considered to not be in the
// heap.
NA_IAPI void naUpdateHeapElement(NAHeap* heap, NAInt backpointer);










// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

// The NAHeap structure. This would normally be hidden to the user.
// Note that this implementation uses a lot of function pointers. See
// implementation file for more information.
struct NAHeap{
  NAInt count;
  void* data;
  void* root; // Pointer to the first byte of the root element
  #ifndef NDEBUG
    NAInt maxcount;
  #endif
  void        (*insertConst)  (NAHeap*, const void*, const void*, NAInt*);
  void        (*insertMutable)(NAHeap*,       void*, const void*, NAInt*);
  const void* (*removeConst)  (NAHeap*);
  void*       (*removeMutable)(NAHeap*);
  void        (*updateBack)   (NAHeap*, NAInt);
  NAInt       (*movedown)     (NAHeap*, const void*, NAInt);
  NAInt       (*moveup)       (NAHeap*, const void*, NAInt);
};
// The root field is needed because the inline functions below have no idea
// whether the heap stores backpointers or not. But as the root element is
// stored at array index 1, a pointer to that location is needed as the two
// structs differ in size whether they do or do not store backpointers.

#include "NAPointer.h"

// The structures internally used to store a single entry. They whould normally
// be hidden to the user.
//
// There are two different structures. The second is used only when the
// backpointer-flag is set when calling naCreateHeap. Note that the distinction
// of the two structs is made using function pointers and a pointer to the root
// element which makes the implementation rather complicated to read. But it
// is blazingly fast.
//
// The important thing is that the ptr and key field are at the same position
// in the struct such that they can be accessed by the inlined functions
// below no matter what struct type is stored.
typedef struct NAHeapNoBackEntry NAHeapNoBackEntry;
struct NAHeapNoBackEntry{
  NAPointerContent  ptr;
  const void*       key;
};
typedef struct NAHeapBackEntry NAHeapBackEntry;
struct NAHeapBackEntry{
  NAPointerContent  ptr;
  const void*       key;
  NAInt*            backpointer;
};



NA_IDEF void naClearHeap(NAHeap* heap){
  #ifndef NDEBUG
    if(!heap){
      naCrash("naClearHeap", "heap is Null-Pointer.");
      return;
    }
  #endif
  free(heap->data);
}


NA_IDEF void naDestroyHeap(NAHeap* heap){
  #ifndef NDEBUG
    if(!heap){
      naCrash("naDestroyHeap", "heap is Null-Pointer.");
      return;
    }
  #endif
  naClearHeap(heap);
  free(heap);
}


NA_IDEF void naEmptyHeap(NAHeap* heap){
  heap->count = 0;
}


NA_IDEF NAUInt naGetHeapCount (const NAHeap* heap){
  #ifndef NDEBUG
    if(!heap){
      naCrash("naGetHeapCount", "heap is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return heap->count;
}


NA_IDEF void naInsertHeapElementConst(  NAHeap* heap,
                                    const void* ptr,
                                    const void* key,
                                         NAInt* backpointer){
  heap->insertConst(heap, ptr, key, backpointer);
}
NA_IDEF void naInsertHeapElementMutable( NAHeap* heap,
                                          void* ptr,
                                    const void* key,
                                         NAInt* backpointer){
  heap->insertMutable(heap, ptr, key, backpointer);
}


NA_IDEF const void* naGetHeapRootConst(const NAHeap* heap){
  // Note that it is irrelevant whether the heap stores elements with or
  // without backpoitners. The ptr and key field are always at the same
  // position.
  NAHeapNoBackEntry* rootelem = (NAHeapNoBackEntry*)(heap->root);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naGetHeapRootConst", "Heap is empty.");
  #endif
  return naGetPointerContentConst(&(rootelem->ptr));
}


NA_IDEF void* naGetHeapRootMutable(const NAHeap* heap){
  // Note that it is irrelevant whether the heap stores elements with or
  // without backpoitners. The ptr and key field are always at the same
  // position.
  NAHeapNoBackEntry* rootelem = (NAHeapNoBackEntry*)(heap->root);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naGetHeapRootMutable", "Heap is empty.");
  #endif
  return naGetPointerContentMutable(&(rootelem->ptr));
}


NA_IDEF const void* naRemoveHeapRootConst(NAHeap* heap){
  return heap->removeConst(heap);
}
NA_IDEF void* naRemoveHeapRootMutable(NAHeap* heap){
  return heap->removeMutable(heap);
}


NA_IDEF const void* naGetHeapRootKey(const NAHeap* heap){
  // Note that it is irrelevant whether the heap stores elements with or
  // without backpoitners. The ptr and key field are always at the same
  // position.
  NAHeapNoBackEntry* rootelem = (NAHeapNoBackEntry*)(heap->root);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naGetHeapRoot", "Heap is empty.");
  #endif
  return rootelem->key;
}


NA_IDEF void naUpdateHeapElement(NAHeap* heap, NAInt backpointer){
  heap->updateBack(heap, backpointer);
}



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
