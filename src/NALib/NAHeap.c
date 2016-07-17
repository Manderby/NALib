
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAHeap.h"
#include "NAMemory.h"


// First note, that a heap structure is basically very simple and could
// probably be written with less than 30 lines of code. Which is one of the
// reasons, this datastructure is so fast.
//
// But as heaps are a very common utility for the authors daily programming,
// they had to be programmed in a somewhat general way which involves multiple
// key types as well as min- and max-heaps plus the ability to use backpointers
// or not.
//
// Therefore, this implementation uses a lot of function pointers which point
// to functions which all look very, very similar. Basically, there are two
// important things: Move an element up the heap and move an element down
// the heap such that the root element is guaranteed to be the min or max.
//
// Everything else is just administration. But boy, has this implementation
// become a mighty beast!




// Movedown functions.
// Perform a reordering of the heap such that a new key can be inserted. The
// return value is the index in which the key can safely be inserted. The
// curindex points at the starting position and the element at curindex is
// assumed to be empty (or contain no useful information).
//
// The core of the functions is implemented as a macro and reused in the
// different helper functions below to reduce writing effort and allow for
// future extensions with more types.
#define NA_HEAPMOVEDOWN_NOBACK(type, dontlowerkeyif)\
  NAHeapNoBackEntry* thedata = (NAHeapNoBackEntry*)(heap->data);\
  const type* thekey = (const type*)key;\
  NAInt nextindex = curindex / 2;\
  /* Go from the leaf to the root and test, where the new element shall lie. */\
  while((nextindex > 0) && (*((type*)thedata[nextindex].key) dontlowerkeyif *thekey)){\
    thedata[curindex] = thedata[nextindex];\
    curindex = nextindex;\
    nextindex /= 2;\
  }\
  return curindex;



#define NA_HEAPMOVEDOWN_BACK(type, dontlowerkeyif)\
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);\
  const type* thekey = (const type*)key;\
  NAInt nextindex = curindex / 2;\
  /* Go from the leaf to the root and test, where the new element shall lie. */\
  while((nextindex > 0) && (*((type*)thedata[nextindex].key) dontlowerkeyif *thekey)){\
    thedata[curindex] = thedata[nextindex];\
    *(thedata[curindex].backpointer) = curindex;\
    curindex = nextindex;\
    nextindex /= 2;\
  }\
  return curindex;

// These functions will be stored as function pointers according to the flags
// set on creation. Therefore, they are not marked as inline.
NA_HDEF NAInt naHeapMoveDownMinDoubleNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_NOBACK(double, >)
}
NA_HDEF NAInt naHeapMoveDownMinFloatNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_NOBACK(float, >)
}
NA_HDEF NAInt naHeapMoveDownMinIntNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_NOBACK(NAInt, >)
}

NA_HDEF NAInt naHeapMoveDownMaxDoubleNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_NOBACK(double, <)
}
NA_HDEF NAInt naHeapMoveDownMaxFloatNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_NOBACK(float, <)
}
NA_HDEF NAInt naHeapMoveDownMaxIntNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_NOBACK(NAInt, <)
}

NA_HDEF NAInt naHeapMoveDownMinDoubleBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_BACK(double, >)
}
NA_HDEF NAInt naHeapMoveDownMinFloatBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_BACK(float, >)
}
NA_HDEF NAInt naHeapMoveDownMinIntBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_BACK(NAInt, >)
}

NA_HDEF NAInt naHeapMoveDownMaxDoubleBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_BACK(double, <)
}
NA_HDEF NAInt naHeapMoveDownMaxFloatBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_BACK(float, <)
}
NA_HDEF NAInt naHeapMoveDownMaxIntBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEDOWN_BACK(NAInt, <)
}


// Moveup functions.
// Assumes the given index to point at an element which is empty (or contains
// no useful information). Performs a reordering of the heap such that the
// given key can be inserted at a suitable index and returns this index.
//
// The core of the functions is implemented as a macro and reused in the
// different helper functions below to reduce writing effort and allow for
// future extensions with more types.
#define NA_HEAPMOVEUP_NOBACK(type, dontraisekeyif)\
  NAHeapNoBackEntry* thedata = (NAHeapNoBackEntry*)(heap->data);\
  const type* thekey = (const type*)key;\
  \
  NAUInt indexl = curindex * 2 + 0;\
  NAUInt indexr = curindex * 2 + 1;\
  /* as long as there is at least one possible position... */ \
  while(NA_TRUE){\
    if(indexl > heap->count){\
      /* reached the leaf */\
      break;\
    }else if(indexr > heap->count){\
      /* only the left element must be checked. */ \
      if(*(const type*)(thedata[indexl].key) dontraisekeyif *thekey){\
        thedata[curindex] = thedata[indexl];\
        curindex = indexl;\
      }\
      /* as the leaves have been reached, no need to perform another loop. */ \
      break;\
    }else if(*(const type*)(thedata[indexl].key) dontraisekeyif *(const type*)(thedata[indexr].key)){\
      /* the left element is more important than the right */ \
      if(*(const type*)(thedata[indexl].key) dontraisekeyif *thekey){\
        /* the left is more important than the key */ \
        thedata[curindex] = thedata[indexl];\
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



#define NA_HEAPMOVEUP_BACK(type, dontraisekeyif)\
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);\
  const type* thekey = (const type*)key;\
  \
  NAUInt indexl = curindex * 2 + 0;\
  NAUInt indexr = curindex * 2 + 1;\
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

// These functions will be stored as function pointers according to the flags
// set on creation. Therefore, they are not marked as inline.
NA_HDEF NAInt naHeapMoveUpMinDoubleNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_NOBACK(double, <)
}
NA_HDEF NAInt naHeapMoveUpMinFloatNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_NOBACK(float, <)
}
NA_HDEF NAInt naHeapMoveUpMinIntNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_NOBACK(NAInt, <)
}

NA_HDEF NAInt naHeapMoveUpMaxDoubleNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_NOBACK(double, >)
}
NA_HDEF NAInt naHeapMoveUpMaxFloatNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_NOBACK(float, >)
}
NA_HDEF NAInt naHeapMoveUpMaxIntNoBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_NOBACK(NAInt, >)
}

NA_HDEF NAInt naHeapMoveUpMinDoubleBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_BACK(double, <)
}
NA_HDEF NAInt naHeapMoveUpMinFloatBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_BACK(float, <)
}
NA_HDEF NAInt naHeapMoveUpMinIntBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_BACK(NAInt, <)
}

NA_HDEF NAInt naHeapMoveUpMaxDoubleBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_BACK(double, >)
}
NA_HDEF NAInt naHeapMoveUpMaxFloatBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_BACK(float, >)
}
NA_HDEF NAInt naHeapMoveUpMaxIntBack(NAHeap* heap, const void* key, NAUInt curindex){
  NA_HEAPMOVEUP_BACK(NAInt, >)
}



NA_HDEF void naGrowHeap(NAHeap* heap){
  NAUInt entrysize = (NAByte*)(heap->root) - (NAByte*)(heap->data);
  void* newdata = naMalloc((-heap->maxcount * 2 + 1) * entrysize);
  naCopyn(newdata, heap->data, (heap->count + 1) * entrysize);
  naFree(heap->data);
  heap->data = newdata;
  heap->root = (NAByte*)(heap->data) + entrysize;
  heap->maxcount *= 2;
}



NA_HDEF void naShrinkHeap(NAHeap* heap){
  NAUInt entrysize = (NAByte*)(heap->root) - (NAByte*)(heap->data);
  void* newdata = naMalloc((-heap->maxcount / 2 + 1) * entrysize);
  naCopyn(newdata, heap->data, (heap->count + 1) * entrysize);
  naFree(heap->data);
  heap->data = newdata;
  heap->root = (NAByte*)(heap->data) + entrysize;
  heap->maxcount /= 2;
}



NA_HDEF void naInsertHeapElementConstNoBack(NAHeap* heap, const void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapNoBackEntry* thedata;
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("naInsertHeapElementConstNoBack", "Heap overflow.");
    if(backpointer)
      naError("naInsertHeapElementConstNoBack", "Heap dos not store backpointers. packpointer should be null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapNoBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithConstBuffer(data, NA_ZERO, NA_ZERO);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void naInsertHeapElementConstBack(NAHeap* heap, const void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapBackEntry* thedata;
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("naInsertHeapElementConstBack", "Heap overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithConstBuffer(data, NA_ZERO, NA_ZERO);
  thedata[newindex].key = key;
  if(backpointer){
    thedata[newindex].backpointer = backpointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an NAInt.
    thedata[newindex].backpointer = heap->data;
    // Bad, bad programming style, but using this trick, a lot of conditional
    // code can be omitted.
  }
  *(thedata[newindex].backpointer) = newindex;
  heap->count++;
}



NA_HDEF void naInsertHeapElementMutableNoBack(NAHeap* heap, void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapNoBackEntry* thedata;
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("naInsertHeapElementMutableNoBack", "Heap overflow.");
    if(backpointer)
      naError("naInsertHeapElementMutableNoBack", "Heap dos not store backpointers. packpointer should be null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapNoBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithMutableBuffer(data, NA_ZERO, NA_ZERO);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void naInsertHeapElementMutableBack(NAHeap* heap, void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapBackEntry* thedata;
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("naInsertHeapElementMutableBack", "Heap overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithMutableBuffer(data, NA_ZERO, NA_ZERO);
  thedata[newindex].key = key;
  if(backpointer){
    thedata[newindex].backpointer = backpointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an NAInt.
    thedata[newindex].backpointer = heap->data;
    // Bad, bad programming style, but using this trick, a lot of conditional
    // code can be omitted.
  }
  *(thedata[newindex].backpointer) = newindex;
  heap->count++;
}



NA_HDEF const void* naRemoveHeapRootConstNoBack(NAHeap* heap){
  NAHeapNoBackEntry* thedata = (NAHeapNoBackEntry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naRemoveHeapRootConst", "Heap is empty.");
  #endif
  returnvalue = naGetPtrConst(&(thedata[1].ptr));
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
  }
  if(heap->maxcount < 0 && ((NAInt)heap->count < -heap->maxcount / 4)){naShrinkHeap(heap);}
  return returnvalue;
}



NA_HDEF const void* naRemoveHeapRootConstBack(NAHeap* heap){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naRemoveHeapRootConst", "Heap is empty.");
  #endif
  returnvalue = naGetPtrConst(&(thedata[1].ptr));
  *(thedata[1].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  if(heap->maxcount < 0 && ((NAInt)heap->count < -heap->maxcount / 4)){naShrinkHeap(heap);}
  return returnvalue;
}



NA_HDEF void* naRemoveHeapRootMutableNoBack(NAHeap* heap){
  NAHeapNoBackEntry* thedata = (NAHeapNoBackEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naRemoveHeapRootMutable", "Heap is empty.");
  #endif
  returnvalue = naGetPtrMutable(&(thedata[1].ptr));
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
  }
  if(heap->maxcount < 0 && ((NAInt)heap->count < -heap->maxcount / 4)){naShrinkHeap(heap);}
  return returnvalue;
}


NA_HDEF void* naRemoveHeapRootMutableBack(NAHeap* heap){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("naRemoveHeapRootMutable", "Heap is empty.");
  #endif
  returnvalue = naGetPtrMutable(&(thedata[1].ptr));
  *(thedata[1].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  if(heap->maxcount < 0 && ((NAInt)heap->count < -heap->maxcount / 4)){naShrinkHeap(heap);}
  return returnvalue;
}



NA_HDEF void naUpdateHeapElementNoBack(NAHeap* heap, NAUInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("naUpdateHeapElementNoBack", "Can not update heap as he stores no backpointers. Function has no effect");
  #endif
}



NA_HDEF void naUpdateHeapElementBack(NAHeap* heap, NAUInt backpointer){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  NAHeapBackEntry tmp;
  NAInt curindex;
  #ifndef NDEBUG
    if(backpointer > heap->count)
      naError("naUpdateHeapElement", "backpointer makes no sense.");
    if(backpointer == 0)
      naError("naUpdateHeapElement", "backpointer says that element is not part of the heap.");
  #endif
  tmp = thedata[backpointer];
  curindex = heap->moveup(heap, tmp.key, backpointer);
  curindex = heap->movedown(heap, tmp.key, curindex);
  thedata[curindex] = tmp;
  *(thedata[curindex].backpointer) = curindex;
}


// This is the one function where all the function pointers of the NAHeap
// structure are set. After this function, these pointers can no longer be
// changed and therefore define the behaviour of the heap until its deletion.
NA_DEF NAHeap* naInitHeap(NAHeap* heap, NAInt count, NAInt flags){
  NAInt entrysize;
  #ifndef NDEBUG
    if(!heap)
      {naCrash("naInitHeap", "heap is Null-Pointer"); return NA_NULL;}
    // there is always count + 1 elements stored in the array.
    if(count >= NA_INT_MAX){
      naCrash("naInitHeap", "Heap count %" NA_PRIi " is too big.", count);
      return NA_NULL;
    }
    // Note that the following case count==0 could be automatically converted
    // to -1. But there might be a different use in the future. Therefore, we
    // define these two values to be invalid now.
    if(count == 0)
      naError("naInitHeap", "count == 0 is not allowed.");
    // Make sure, the negative count is a power of 2.
    if(count < 0){
      NAInt countpower = naLog2i(-count);
      if((NAInt)naPow(2., (double)countpower) != -count)
        naError("naInitHeap", "negative count must be a power of 2.");
    }
  #endif
  heap->maxcount = count;
  
  if(!(flags & NA_HEAP_STORES_BACKPOINTERS)){
    // entries store no backpointers
    
    entrysize = sizeof(NAHeapNoBackEntry);
    heap->count = 0;
    heap->data = naMalloc((naAbsi(heap->maxcount) + 1) * entrysize);
    heap->root = &(((NAHeapNoBackEntry*)heap->data)[1]);

    heap->insertConst = naInsertHeapElementConstNoBack;
    heap->insertMutable = naInsertHeapElementMutableNoBack;
    heap->removeConst = naRemoveHeapRootConstNoBack;
    heap->removeMutable = naRemoveHeapRootMutableNoBack;
    heap->updateBack = naUpdateHeapElementNoBack;

    switch(flags & NA_HEAP_DATATYPE_MASK){
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = naHeapMoveDownMaxDoubleNoBack;
        heap->moveup   = naHeapMoveUpMaxDoubleNoBack;
      }else{
        heap->movedown = naHeapMoveDownMinDoubleNoBack;
        heap->moveup   = naHeapMoveUpMinDoubleNoBack;
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = naHeapMoveDownMaxFloatNoBack;
        heap->moveup   = naHeapMoveUpMaxFloatNoBack;
      }else{
        heap->movedown = naHeapMoveDownMinFloatNoBack;
        heap->moveup   = naHeapMoveUpMinFloatNoBack;
      }
      break;
    case NA_HEAP_USES_INT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = naHeapMoveDownMaxIntNoBack;
        heap->moveup   = naHeapMoveUpMaxIntNoBack;
      }else{
        heap->movedown = naHeapMoveDownMinIntNoBack;
        heap->moveup   = naHeapMoveUpMinIntNoBack;
      }
      break;
    default:
      #ifndef NDEBUG
        naCrash("naInitHeap", "flag combination %x" NA_PRIi " not implemented.", flags);
        return NA_NULL;
      #endif
      break;
    }

  }else{
    // Entries store backpointers
    
    entrysize = sizeof(NAHeapBackEntry);
    heap->count = 0;
    heap->data = naMalloc((naAbsi(heap->maxcount) + 1) * entrysize);
    heap->root = &(((NAHeapBackEntry*)heap->data)[1]);

    heap->insertConst = naInsertHeapElementConstBack;
    heap->insertMutable = naInsertHeapElementMutableBack;
    heap->removeConst = naRemoveHeapRootConstBack;
    heap->removeMutable = naRemoveHeapRootMutableBack;
    heap->updateBack = naUpdateHeapElementBack;
    
    switch(flags & NA_HEAP_DATATYPE_MASK){
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = naHeapMoveDownMaxDoubleBack;
        heap->moveup   = naHeapMoveUpMaxDoubleBack;
      }else{
        heap->movedown = naHeapMoveDownMinDoubleBack;
        heap->moveup   = naHeapMoveUpMinDoubleBack;
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = naHeapMoveDownMaxFloatBack;
        heap->moveup   = naHeapMoveUpMaxFloatBack;
      }else{
        heap->movedown = naHeapMoveDownMinFloatBack;
        heap->moveup   = naHeapMoveUpMinFloatBack;
      }
      break;
    case NA_HEAP_USES_INT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = naHeapMoveDownMaxIntBack;
        heap->moveup   = naHeapMoveUpMaxIntBack;
      }else{
        heap->movedown = naHeapMoveDownMinIntBack;
        heap->moveup   = naHeapMoveUpMinIntBack;
      }
      break;
    default:
      #ifndef NDEBUG
        naCrash("naInitHeap", "flag combination %x" NA_PRIi " not implemented.", flags);
        return NA_NULL;
      #endif
      break;
    }
  }
  return heap;
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
