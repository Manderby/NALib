
#include "../../NAHeap.h"
#include "../../NAMemory.h"
#include "../../NABinaryData.h"
#include "../../NAMathOperators.h"
#include "../../NAKey.h"


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



#define NA_T_USE_BACKPOINTERS 0
  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Greater
  #define NA_T_DONT_MOVE_UP_COMPARATOR Lower
    #define NA_T_TYPE double
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NADateTime
      #include "NAHeapT.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR

  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Lower
  #define NA_T_DONT_MOVE_UP_COMPARATOR Greater
    #define NA_T_TYPE double
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NADateTime
      #include "NAHeapT.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR
#undef NA_T_USE_BACKPOINTERS

#define NA_T_USE_BACKPOINTERS 1
  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Greater
  #define NA_T_DONT_MOVE_UP_COMPARATOR Lower
    #define NA_T_TYPE double
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NADateTime
      #include "NAHeapT.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR

  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Lower
  #define NA_T_DONT_MOVE_UP_COMPARATOR Greater
    #define NA_T_TYPE double
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NADateTime
      #include "NAHeapT.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR
#undef NA_T_USE_BACKPOINTERS



NA_HDEF void naGrowHeap(NAHeap* heap){
  ptrdiff_t entrysize = (NAByte*)(heap->root) - (NAByte*)(heap->data);
  void* newdata = naMalloc((-heap->maxcount * 2 + 1) * entrysize);
  naCopyn(newdata, heap->data, ((heap->count + 1) * entrysize));
  naFree(heap->data);
  heap->data = newdata;
  heap->root = (NAByte*)(heap->data) + entrysize;
  heap->maxcount *= 2;
}



NA_DEF void naShrinkHeapIfNecessary(NAHeap* heap){
  #ifndef NDEBUG
    if(heap->maxcount > 0)
      naError("Heap defined with a fixed count of elements.");
  #endif
  if((NAInt)heap->count < -heap->maxcount / 4){
    ptrdiff_t entrysize = (NAByte*)(heap->root) - (NAByte*)(heap->data);
    void* newdata = naMalloc((-heap->maxcount / 2 + 1) * entrysize);
    naCopyn(newdata, heap->data, ((heap->count + 1) * entrysize));
    naFree(heap->data);
    heap->data = newdata;
    heap->root = (NAByte*)(heap->data) + entrysize;
    heap->maxcount /= 2;
  }
}



NA_HDEF void naInsertHeapElementConstNoBack(NAHeap* heap, const void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapEntry* thedata;
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
    if(backpointer)
      naError("Heap dos not store backpointers. packpointer should be null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataConst(data);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void naInsertHeapElementConstBack(NAHeap* heap, const void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapBackEntry* thedata;
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataConst(data);
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
  NAHeapEntry* thedata;
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
    if(backpointer)
      naError("Heap dos not store backpointers. packpointer should be null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataMutable(data);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void naInsertHeapElementMutableBack(NAHeap* heap, void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeapBackEntry* thedata;
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataMutable(data);
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
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrConst(thedata[1].ptr);
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
  }
  return returnvalue;
}



NA_HDEF const void* naRemoveHeapRootConstBack(NAHeap* heap){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrConst(thedata[1].ptr);
  *(thedata[1].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  return returnvalue;
}



NA_HDEF void* naRemoveHeapRootMutableNoBack(NAHeap* heap){
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrMutable(thedata[1].ptr);
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
  }
  return returnvalue;
}


NA_HDEF void* naRemoveHeapRootMutableBack(NAHeap* heap){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrMutable(thedata[1].ptr);
  *(thedata[1].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  return returnvalue;
}



NA_HDEF const void* naRemoveHeapPosConstNoBack(NAHeap* heap, NAInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("Can not update heap as he stores no backpointers. Function has no effect");
  #endif
  return NA_NULL;
}



NA_HDEF const void* naRemoveHeapPosConstBack(NAHeap* heap, NAInt backpointer){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(backpointer > heap->count)
      naError("backpointer makes no sense.");
    if(backpointer == 0)
      naError("backpointer says that element is not part of the heap.");
  #endif
  returnvalue = naGetPtrConst(thedata[backpointer].ptr);
  *(thedata[backpointer].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, backpointer);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  return returnvalue;
}



NA_HDEF void* naRemoveHeapPosMutableNoBack(NAHeap* heap, NAInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("Can not update heap as he stores no backpointers. Function has no effect");
  #endif
  return NA_NULL;
}


NA_HDEF void* naRemoveHeapPosMutableBack(NAHeap* heap, NAInt backpointer){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(backpointer > heap->count)
      naError("backpointer makes no sense.");
    if(backpointer == 0)
      naError("backpointer says that element is not part of the heap.");
  #endif
  returnvalue = naGetPtrMutable(thedata[backpointer].ptr);
  *(thedata[backpointer].backpointer) = 0;
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, backpointer);
    thedata[curindex] = thedata[heap->count + 1];
    *(thedata[curindex].backpointer) = curindex;
  }
  return returnvalue;
}


NA_HDEF void naUpdateHeapElementNoBack(NAHeap* heap, NAInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("Can not update heap as he stores no backpointers. Function has no effect");
  #endif
}



NA_HDEF void naUpdateHeapElementBack(NAHeap* heap, NAInt backpointer){
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  NAHeapBackEntry tmp;
  NAInt curindex;
  #ifndef NDEBUG
    if(backpointer > heap->count)
      naError("backpointer makes no sense.");
    if(backpointer == 0)
      naError("backpointer says that element is not part of the heap.");
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
      naCrash("heap is Null-Pointer");
    // there is always count + 1 elements stored in the array.
    if(count >= NA_MAX_i){
      naCrash("Heap count is too big.");
      return NA_NULL;
    }
    // Note that the following case count == 0 could be automatically converted
    // to -1. But there might be a different use in the future. Therefore, we
    // define these two values to be invalid now.
    if(count == 0)
      naError("count == 0 is not allowed.");
    // Make sure, the negative count is a power of 2.
    if(count < 0){
      NAInt countpower = naLog2i(-count);
      if((NAInt)naPow(2., (double)countpower) != -count)
        naError("negative count must be a power of 2.");
    }
  #endif
  heap->maxcount = count;

  if(!(flags & NA_HEAP_STORES_BACKPOINTERS)){
    // entries store no backpointers

    entrysize = sizeof(NAHeapEntry);
    heap->count = 0;
    heap->data = naMalloc((naAbsi(heap->maxcount) + 1) * entrysize);
    heap->root = &(((NAHeapEntry*)heap->data)[1]);

    heap->insertConst = naInsertHeapElementConstNoBack;
    heap->insertMutable = naInsertHeapElementMutableNoBack;
    heap->removeConst = naRemoveHeapRootConstNoBack;
    heap->removeMutable = naRemoveHeapRootMutableNoBack;
    heap->removePosConst = naRemoveHeapPosConstNoBack;
    heap->removePosMutable = naRemoveHeapPosMutableNoBack;
    heap->updateBack = naUpdateHeapElementNoBack;

    switch(flags & NA_HEAP_DATATYPE_MASK){
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   double, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, double, 0);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, double, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   double, 0);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   float, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, float, 0);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, float, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   float, 0);
      }
      break;
    case NA_HEAP_USES_NAINT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   NAInt, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, NAInt, 0);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, NAInt, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   NAInt, 0);
      }
      break;
    case NA_HEAP_USES_DATETIME_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   NADateTime, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, NADateTime, 0);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, NADateTime, 0);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   NADateTime, 0);
      }
      break;
    default:
      #ifndef NDEBUG
        naCrash("flag combination not implemented.");
        return NA_NULL;
      #else
        break;
      #endif
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
    heap->removePosConst = naRemoveHeapPosConstBack;
    heap->removePosMutable = naRemoveHeapPosMutableBack;
    heap->updateBack = naUpdateHeapElementBack;

    switch(flags & NA_HEAP_DATATYPE_MASK){
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   double, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, double, 1);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, double, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   double, 1);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   float, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, float, 1);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, float, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   float, 1);
      }
      break;
    case NA_HEAP_USES_NAINT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   NAInt, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, NAInt, 1);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, NAInt, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   NAInt, 1);
      }
      break;
    case NA_HEAP_USES_DATETIME_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeapMoveDown, Lower,   NADateTime, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Greater, NADateTime, 1);
      }else{
        heap->movedown = NA_T3(naHeapMoveDown, Greater, NADateTime, 1);
        heap->moveup   = NA_T3(naHeapMoveUp,   Lower,   NADateTime, 1);
      }
      break;
    default:
      #ifndef NDEBUG
        naCrash("flag combination not implemented.");
        return NA_NULL;
      #else
        break;
      #endif
    }
  }
  return heap;
}



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
