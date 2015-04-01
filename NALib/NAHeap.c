
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAHeap.h"
#include "NAPointer.h"




// Movedown functions.
// Perform a reordering of the heap such that a new key can be inserted. The
// return value is the index in which the key can safely be inserted. The
// curindex points at the starting position and the element at curindex is
// assumed to be empty (or contain no useful information).
//
// These functions will be stored as function pointers according to the flags
// set on creation. Therefore, they are not marked as inline.
//
// The core of the functions is implemented as a macro and reused in the
// different helper functions to reduce writing effort and allow for future
// extensions with more types.
#define NA_HEAPMOVEDOWN(type, dontlowerkeyif)\
  NAHeapEntry* thedata = (NAHeapEntry*)data;\
  const type* thekey = (const type*)key;\
  NAInt nextindex = curindex / 2;\
  while((nextindex > 0) && (*((type*)thedata[nextindex].key) dontlowerkeyif *thekey)){\
    thedata[curindex] = thedata[nextindex];\
    *(thedata[curindex].backpointer) = curindex;\
    curindex = nextindex;\
    nextindex /= 2;\
  }\
  return curindex;

NA_HIDEF NAInt naHeapMoveDownMinDouble(void* data, const void* key, NAInt curindex){
  NA_HEAPMOVEDOWN(double, >)
}
NA_HIDEF NAInt naHeapMoveDownMinFloat(void* data, const void* key, NAInt curindex){
  NA_HEAPMOVEDOWN(float, >)
}
NA_HIDEF NAInt naHeapMoveDownMinInt(void* data, const void* key, NAInt curindex){
  NA_HEAPMOVEDOWN(NAInt, >)
}

NA_HIDEF NAInt naHeapMoveDownMaxDouble(void* data, const void* key, NAInt curindex){
  NA_HEAPMOVEDOWN(double, <)
}
NA_HIDEF NAInt naHeapMoveDownMaxFloat(void* data, const void* key, NAInt curindex){
  NA_HEAPMOVEDOWN(float, <)
}
NA_HIDEF NAInt naHeapMoveDownMaxInt(void* data, const void* key, NAInt curindex){
  NA_HEAPMOVEDOWN(NAInt, <)
}


// Moveup functions.
// Assumes the given index to point at an element which is empty (or contains
// no useful information). Performs a reordering of the heap such that the
// given key can be inserted at a suitable index and returns this index.
//
// These functions will be stored as function pointers according to the flags
// set on creation. Therefore, they are not marked as inline.
//
// The core of the functions is implemented as a macro and reused in the
// different helper functions to reduce writing effort and allow for future
// extensions with more types.

#define NA_HEAPMOVEUP(type, dontraisekeyif)\
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);\
  const type* thekey = (const type*)key;\
  \
  NAInt indexl = curindex * 2 + 0;\
  NAInt indexr = curindex * 2 + 1;\
  /* as long as there is at least one possible position... */ \
  while(NA_TRUE){\
    if(indexl > heap->count){\
      /* reached the leaf */\
      break;\
    }else if(indexr > heap->count){\
      /* only the left element must be checked. */ \
      if(*(const type*)(thedata[indexl].key) dontraisekeyif *thekey){\
        thedata[curindex] = thedata[indexl];\
        *(thedata[curindex].backpointer) = curindex;\
        curindex = indexl;\
      }\
      /* as the leaves have been reached, no need to perform another loop. */ \
      break;\
    }else if(*(const type*)(thedata[indexl].key) dontraisekeyif *(const type*)(thedata[indexr].key)){\
      /* the left element is more important than the right */ \
      if(*(const type*)(thedata[indexl].key) dontraisekeyif *thekey){\
        /* the left is more important than the key */ \
        thedata[curindex] = thedata[indexl];\
        *(thedata[curindex].backpointer) = curindex;\
        curindex = indexl;\
      }else{\
        /* noone is more important. */ \
        break;\
      }\
    }else{\
      /* the right element is more important than the left */ \
      if(*(const type*)(thedata[indexr].key) dontraisekeyif *thekey){\
        /* the right is more important than the key */ \
        thedata[curindex] = thedata[indexr];\
        *(thedata[curindex].backpointer) = curindex;\
        curindex = indexr;\
      }else{\
        /* noone is more important. */ \
        break;\
      }\
    }\
    indexl = curindex * 2 + 0;\
    indexr = curindex * 2 + 1;\
  }\
  return curindex;

NA_HIDEF NAInt naHeapMoveUpMinDouble(NAHeap* heap, const void* key, NAInt curindex){
  NA_HEAPMOVEUP(double, <)
}
NA_HIDEF NAInt naHeapMoveUpMinFloat(NAHeap* heap, const void* key, NAInt curindex){
  NA_HEAPMOVEUP(float, <)
}
NA_HIDEF NAInt naHeapMoveUpMinInt(NAHeap* heap, const void* key, NAInt curindex){
  NA_HEAPMOVEUP(NAInt, <)
}

NA_HIDEF NAInt naHeapMoveUpMaxDouble(NAHeap* heap, const void* key, NAInt curindex){
  NA_HEAPMOVEUP(double, >)
}
NA_HIDEF NAInt naHeapMoveUpMaxFloat(NAHeap* heap, const void* key, NAInt curindex){
  NA_HEAPMOVEUP(float, >)
}
NA_HIDEF NAInt naHeapMoveUpMaxInt(NAHeap* heap, const void* key, NAInt curindex){
  NA_HEAPMOVEUP(NAInt, >)
}



NA_DEF NAHeap* naCreateHeap(NAHeap* heap, NAInt count, NAInt flags){
  NAInt entrysize;
  #ifndef NDEBUG
    NAHeap* originalheapparameter = heap;
    // there is always count + 1 elements stored in the array.
    if(count >= NA_INT_MAX){
      naCrash("naCreateHeap", "Heap count is too big.");
      return NA_NULL;
    }
    if(count <= 0)
      naError("naCreateHeap", "Heap count smallerequal zero.");
  #endif

  heap = naAllocateIfNull(heap, sizeof(NAHeap));

  #ifndef NDEBUG
    heap->maxcount = count;
  #endif

  entrysize = sizeof(NAHeapEntry);
  heap->count = 0;
  heap->data = naAllocate((count + 1) * entrysize);
  
  switch(flags & NA_HEAP_DATATYPE_MASK){
  case NA_HEAP_USES_DOUBLE_KEY:
    if(flags & NA_HEAP_IS_MAX_HEAP){
      heap->movedown = naHeapMoveDownMaxDouble;
      heap->moveup   = naHeapMoveUpMaxDouble;
    }else{
      heap->movedown = naHeapMoveDownMinDouble;
      heap->moveup   = naHeapMoveUpMinDouble;
    }
    break;
  case NA_HEAP_USES_FLOAT_KEY:
    if(flags & NA_HEAP_IS_MAX_HEAP){
      heap->movedown = naHeapMoveDownMaxFloat;
      heap->moveup   = naHeapMoveUpMaxFloat;
    }else{
      heap->movedown = naHeapMoveDownMinFloat;
      heap->moveup   = naHeapMoveUpMinFloat;
    }
    break;
  case NA_HEAP_USES_INT_KEY:
    if(flags & NA_HEAP_IS_MAX_HEAP){
      heap->movedown = naHeapMoveDownMaxInt;
      heap->moveup   = naHeapMoveUpMaxInt;
    }else{
      heap->movedown = naHeapMoveDownMinInt;
      heap->moveup   = naHeapMoveUpMinInt;
    }
    break;
  default:
    #ifndef NDEBUG
      naCrash("naCreateHeap", "flag combination not implemented.");
      if(originalheapparameter){
        naClearHeap(heap);
      }else{
        naDestroyHeap(heap);
      }
      return NA_NULL;
    #endif
    break;
  }
  return heap;
}



NA_DEF void naInsertHeapElement(NAHeap* heap, void* newptr, const void* newkey, NAInt* newbackpointer){
  NAInt newindex;
  NAHeapEntry* thedata;
  #ifndef NDEBUG
    if(heap->count == heap->maxcount)
      naError("naInsertHeapElement", "Heap overflow.");
  #endif
  newindex = heap->movedown(heap->data, newkey, heap->count + 1);
  thedata = (NAHeapEntry*)(heap->data);
  thedata[newindex].ptr = newptr;
  thedata[newindex].key = newkey;
  if(newbackpointer){
    thedata[newindex].backpointer = newbackpointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an NAInt.
    thedata[newindex].backpointer = heap->data;
    // Using this trick, a lot of conditional code can be omitted.
  }
  *(thedata[newindex].backpointer) = newindex;
  heap->count++;
}



NA_DEF void* naRemoveHeapRoot(NAHeap* heap){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naRemoveHeapRoot", "Heap is empty.");
  #endif
  returnvalue = thedata[1].ptr;
  *(thedata[1].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  return returnvalue;
}



NA_DEF void naUpdateHeapElement(NAHeap* heap, NAInt backpointer){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  NAHeapEntry tmp;
  NAInt curindex;
  #ifndef NDEBUG
    if(backpointer > heap->count)
      naError("naUpdateHeapElement", "backpointer makes no sense.");
    if(backpointer < 0)
      naError("naUpdateHeapElement", "backpointer is invalid.");
    if(backpointer == 0)
      naError("naUpdateHeapElement", "backpointer says that element is not part of the heap.");
  #endif
  tmp = thedata[backpointer];
  curindex = heap->moveup(heap, tmp.key, backpointer);
  curindex = heap->movedown(thedata, tmp.key, curindex);
  thedata[curindex] = tmp;
  *(thedata[curindex].backpointer) = curindex;
}





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
