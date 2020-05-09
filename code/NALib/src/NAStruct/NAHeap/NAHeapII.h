
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAHeap.h
// Do not include this file directly! It will automatically be included when
// including "NAHeap.h"



// The NAHeap structure. This would normally be hidden to the user.
// Note that this implementation uses a lot of function pointers. See
// implementation file for more information.
struct NAHeap{
  NAInt count;
  void* data;
  void* root; // Pointer to the first byte of the root element
  NAInt maxcount; // heap holds max elements. If this value is < 0, the
                  // heap grows automatically.
  void        (*insertConst)      (NAHeap*, const void*, const void*, NAInt*);
  void        (*insertMutable)    (NAHeap*,       void*, const void*, NAInt*);
  const void* (*removeConst)      (NAHeap*);
  void*       (*removeMutable)    (NAHeap*);
  const void* (*removePosConst)   (NAHeap*, NAInt);
  void*       (*removePosMutable) (NAHeap*, NAInt);
  void        (*updateBack)       (NAHeap*, NAInt);
  NAInt       (*moveup)           (NAHeap*, const void*, NAInt);
  NAInt       (*movedown)         (NAHeap*, const void*, NAInt);
};
// The root field is needed because the inline functions below have no idea
// whether the heap stores backpointers or not. But as the root element is
// stored at array index 1, a pointer to that location is needed as the two
// structs differ in bytesize whether they do or do not store backpointers.

#include "../../NAMemory.h"

// The structures internally used to store a single entry. They whould normally
// be hidden to the user.
//
// There are two different structures. The second is used only when the
// backpointer-flag is set when calling naInitHeap. Note that the distinction
// of the two structs is made using function pointers and a pointer to the root
// element which makes the implementation rather complicated to read. But it
// is blazingly fast.
//
// The important thing is that the ptr and key field are at the same position
// in the struct such that they can be accessed by the inlined functions
// below no matter what struct type is stored.
typedef struct NAHeapEntry NAHeapEntry;
struct NAHeapEntry{
  const void*       key;
  NAPtr             ptr;
//  NABool            importantindex;
};
typedef struct NAHeapBackEntry NAHeapBackEntry;
struct NAHeapBackEntry{
  const void*       key;
  NAPtr             ptr;
//  NABool            importantindex;
  NAInt*            backpointer;
};



NA_IDEF void naClearHeap(NAHeap* heap){
  #ifndef NDEBUG
    if(!heap)
      naCrash("heap is Null-Pointer.");
  #endif
  free(heap->data);
}


NA_IDEF void naEmptyHeap(NAHeap* heap){
  heap->count = 0;
}


NA_IDEF NAInt naGetHeapCount (const NAHeap* heap){
  #ifndef NDEBUG
    if(!heap)
      naCrash("heap is Null-Pointer.");
  #endif
  return heap->count;
}


NA_IDEF NAInt naGetHeapMaxCount(const NAHeap* heap){
  #ifndef NDEBUG
    if(!heap)
      naCrash("heap is Null-Pointer.");
  #endif
  return heap->maxcount;
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
  NAHeapEntry* rootelem = (NAHeapEntry*)(heap->root);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  return naGetPtrConst(rootelem->ptr);
}


NA_IDEF void* naGetHeapRootMutable(const NAHeap* heap){
  // Note that it is irrelevant whether the heap stores elements with or
  // without backpoitners. The ptr and key field are always at the same
  // position.
  NAHeapEntry* rootelem = (NAHeapEntry*)(heap->root);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  return naGetPtrMutable(rootelem->ptr);
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
  NAHeapEntry* rootelem = (NAHeapEntry*)(heap->root);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  return rootelem->key;
}


NA_IDEF void naUpdateHeapElement(NAHeap* heap, NAInt backpointer){
  heap->updateBack(heap, backpointer);
}



NA_IDEF const void* naRemoveHeapPosConst(NAHeap* heap, NAInt backpointer){
  return heap->removePosConst(heap, backpointer);
}



NA_IDEF void* naRemoveHeapPosMutable(NAHeap* heap, NAInt backpointer){
  return heap->removePosMutable(heap, backpointer);
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
