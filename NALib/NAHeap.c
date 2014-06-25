
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAHeap.h"
#include "NAPointer.h"


typedef struct NAHeapEntry NAHeapEntry;
struct NAHeapEntry{
  void*       ptr;
  const void* key;
  NAInt*      backpointer;
};





// Movedown functions.
// Perform a reordering of the heap such that a new key can be inserted. The
// return value is the index in which the key can safely be inserted. The
// curindex points at the starting position and the element at curindex is
// assumed to be empty (or contain no useful information).

// Min Heap, double key
NA_HLP NAInt naHeapMoveDownMinDouble(void* data, const void* key, NAInt curindex){
  NAHeapEntry* thedata = (NAHeapEntry*)data;
  const double* thekey = (const double*)key;
  NAInt nextindex = curindex / 2;
  while((nextindex > 0) && (*((double*)thedata[nextindex].key) > *thekey)){
    thedata[curindex] = thedata[nextindex];
    *(thedata[curindex].backpointer) = curindex;
    curindex = nextindex;
    nextindex /= 2;
  }
  return curindex;
}

// Min Heap, NAInt key
NA_HLP NAInt naHeapMoveDownMinInt(void* data, const void* key, NAInt curindex){
  NAHeapEntry* thedata = (NAHeapEntry*)data;
  const NAInt* thekey = (const NAInt*)key;
  NAInt nextindex = curindex / 2;
  while((nextindex > 0) && (*((NAInt*)thedata[nextindex].key) > *thekey)){
    thedata[curindex] = thedata[nextindex];
    *(thedata[curindex].backpointer) = curindex;
    curindex = nextindex;
    nextindex /= 2;
  }
  return curindex;
}


// Moveup functions.
// Assumes the given index to point at an element which is empty (or contains
// no useful information). Performs a reordering of the heap such that the
// given key can be inserted at a suitable index and returns this index.

// Min Heap, double key
NA_HLP NAInt naHeapMoveUpMinDouble(NAHeap* heap, const void* key, NAInt curindex){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  const double* thekey = (const double*)key;

  NAInt indexl = curindex * 2 + 0;
  NAInt indexr = curindex * 2 + 1;
  // as long as there is at least one possible position...
  while(NA_TRUE){
    if(indexl > heap->count){
      // reached the leave
      break;
    }else if(indexr > heap->count){
      // only the left element must be checked.
      if(*(const double*)(thedata[indexl].key) < *thekey){
        thedata[curindex] = thedata[indexl];
        *(thedata[curindex].backpointer) = curindex;
        curindex = indexl;
      }
      // as the leafes have been reached, no need to perform another loop.
      break;
    }else if(thedata[indexl].key < thedata[indexr].key){
      // the left element is smaller than the right
      if(*(const double*)(thedata[indexl].key) < *thekey){
        // the left is smaller than the key
        thedata[curindex] = thedata[indexl];
        *(thedata[curindex].backpointer) = curindex;
        curindex = indexl;
      }else{
        // noone is smaller.
        break;
      }
    }else{
      // the right element is smaller than the left
      if(*(const double*)(thedata[indexr].key) < *thekey){
        // the right is smaller than the key
        thedata[curindex] = thedata[indexr];
        *(thedata[curindex].backpointer) = curindex;
        curindex = indexr;
      }else{
        // noone is smaller.
        break;
      }
    }
    indexl = curindex * 2 + 0;
    indexr = curindex * 2 + 1;
  }
  return curindex;
}



// Min Heap, NAInt key
NA_HLP NAInt naHeapMoveUpMinInt(NAHeap* heap, const void* key, NAInt curindex){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  const NAInt* thekey = (const NAInt*)key;

  NAInt indexl = curindex * 2 + 0;
  NAInt indexr = curindex * 2 + 1;
  // as long as there is at least one possible position...
  while(NA_TRUE){
    if(indexl > heap->count){
      // reached the leave
      break;
    }else if(indexr > heap->count){
      // only the left element must be checked.
      if(*(const NAInt*)(thedata[indexl].key) < *thekey){
        thedata[curindex] = thedata[indexl];
        *(thedata[curindex].backpointer) = curindex;
        curindex = indexl;
      }
      // as the leafes have been reached, no need to perform another loop.
      break;
    }else if(thedata[indexl].key < thedata[indexr].key){
      // the left element is smaller than the right
      if(*(const NAInt*)(thedata[indexl].key) < *thekey){
        // the left is smaller than the key
        thedata[curindex] = thedata[indexl];
        *(thedata[curindex].backpointer) = curindex;
        curindex = indexl;
      }else{
        // noone is smaller.
        break;
      }
    }else{
      // the right element is smaller than the left
      if(*(const NAInt*)(thedata[indexr].key) < *thekey){
        // the right is smaller than the key
        thedata[curindex] = thedata[indexr];
        *(thedata[curindex].backpointer) = curindex;
        curindex = indexr;
      }else{
        // noone is smaller.
        break;
      }
    }
    indexl = curindex * 2 + 0;
    indexr = curindex * 2 + 1;
  }
  return curindex;
}




NA_DEF NAHeap* naCreateHeap(NAHeap* heap, NAInt count, NAInt flags){
  heap = naAllocateIfNull(heap, sizeof(NAHeap));
  #ifndef NDEBUG
    // there is always count + 1 elements stored in the array.
    if(count == NA_INT_MAX)
      naError("naCreateHeap", "Heap count is too big.");
    if(count <= 0)
      naError("naCreateHeap", "Heap count smallerequal zero.");
    heap->maxcount = count;
  #endif
  heap->count = 0;
  heap->data = naAllocate((count + 1) * sizeof(NAHeapEntry));
  
  switch(flags){
  case 0:
    heap->movedown = naHeapMoveDownMinDouble;
    heap->moveup   = naHeapMoveUpMinDouble;
    break;
  case NA_HEAP_USES_INT_KEY:
    heap->movedown = naHeapMoveDownMinInt;
    heap->moveup   = naHeapMoveUpMinInt;
    break;
  default:
    #ifndef NDEBUG
      naCrash("naCreateHeap", "flag combination not implemented yet.");
    #endif
    break;
  }
  return heap;
}


NA_DEF void naClearHeap(NAHeap* heap){
  #ifndef NDEBUG
    if(!heap)
      {naCrash("naClearHeap", "heap is Null-Pointer."); return;}
  #endif
  free(&(heap->data));
}


NA_DEF void naDestroyHeap(NAHeap* heap){
  naClearHeap(heap);
  free(heap);
}



NA_DEF void naInsertHeapElement(NAHeap* heap, void* newptr, const void* newkey, NAInt* newbackpointer){
  #ifndef NDEBUG
    if(heap->count == heap->maxcount)
      naError("naInsertHeapElement", "Heap overflow.");
  #endif
  NAInt newindex = heap->movedown(heap->data, newkey, heap->count + 1);
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
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



NA_DEF NABool naIsHeapEmpty (const NAHeap* heap){
  #ifndef NDEBUG
    if(!heap)
      {naCrash("naIsHeapEmpty", "heap is Null-Pointer."); return NA_TRUE;}
  #endif
  return (heap->count == 0);
}


NA_DEF void* naGetHeapRoot(const NAHeap* heap){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naGetHeapRoot", "Heap is empty.");
  #endif
  return thedata[1].ptr;
}


NA_DEF void* naRemoveHeapRoot(NAHeap* heap){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naRemoveHeapRoot", "Heap is empty.");
  #endif
  void* returnvalue = thedata[1].ptr;
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
  #ifndef NDEBUG
    if(backpointer > heap->count)
      naError("naUpdateHeapElement", "backpointer makes no sense.");
    if(backpointer < 0)
      naError("naUpdateHeapElement", "backpointer is invalid.");
    if(backpointer == 0)
      naError("naUpdateHeapElement", "backpointer says that element is not part of the heap.");
  #endif
  NAHeapEntry tmp = thedata[backpointer];
  NAInt curindex;
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
