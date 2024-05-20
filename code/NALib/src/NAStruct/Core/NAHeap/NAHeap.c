
#include "../../NAHeap.h"
#include "../../../NAUtility/NAMemory.h"
#include "../../../NAUtility/NABinaryData.h"
#include "../../../NAMath/NAMathOperators.h"
#include "../../../NAUtility/NAKey.h"


// First note, that a heap structure is basically very simple and could
// probably be written with less than 30 lines of code. Which is one of the
// reasons, this datastructure is so fast.
//
// But as heaps are a very common utility for the authors daily programming,
// they had to be programmed in a somewhat general way which involves multiple
// key types as well as min- and max-heaps plus the ability to use backPointers
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
  #define NA_T_DONT_MOVE_UP_COMPARATOR Less
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

  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Less
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
  #define NA_T_DONT_MOVE_UP_COMPARATOR Less
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

  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Less
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



NA_HDEF void na_GrowHeap(NAHeap* heap) {
  ptrdiff_t entrysize = (NAByte*)(heap->root) - (NAByte*)(heap->data);
  void* newData = naMalloc((size_t)(-heap->maxcount * 2 + 1) * (size_t)entrysize);
  naCopyn(newData, heap->data, (size_t)((heap->count + 1) * entrysize));
  naFree(heap->data);
  heap->data = newData;
  heap->root = (NAByte*)(heap->data) + entrysize;
  heap->maxcount *= 2;
}



NA_DEF void naShrinkHeapIfNecessary(NAHeap* heap) {
  #if NA_DEBUG
    if(heap->maxcount > 0)
      naError("Heap defined with a fixed count of elements.");
  #endif
  if((NAInt)heap->count < -heap->maxcount / 4) {
    ptrdiff_t entrysize = (NAByte*)(heap->root) - (NAByte*)(heap->data);
    void* newData = naMalloc((size_t)(-heap->maxcount / 2 + 1) * (size_t)entrysize);
    naCopyn(newData, heap->data, (size_t)((heap->count + 1) * entrysize));
    naFree(heap->data);
    heap->data = newData;
    heap->root = (NAByte*)(heap->data) + entrysize;
    heap->maxcount /= 2;
  }
}



NA_HDEF void na_InsertHeapElementConstNoBack(NAHeap* heap, const void* data, const void* key, NAInt* backPointer) {
  NAInt newindex;
  NAHeapEntry* thedata;
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
    if(backPointer)
      naError("Heap dos not store backPointers. packpointer should be Null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)) {
    na_GrowHeap(heap);
  }
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataConst(data);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void na_InsertHeapElementConstBack(NAHeap* heap, const void* data, const void* key, NAInt* backPointer) {
  NAInt newindex;
  NAHeapBackEntry* thedata;
  #if NA_DEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)) {
    na_GrowHeap(heap);
  }
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataConst(data);
  thedata[newindex].key = key;
  if(backPointer) {
    thedata[newindex].backPointer = backPointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an NAInt.
    thedata[newindex].backPointer = heap->data;
    // Bad, bad programming style, but using this trick, a lot of conditional
    // code can be omitted.
  }
  *(thedata[newindex].backPointer) = newindex;
  heap->count++;
}



NA_HDEF void na_InsertHeapElementMutableNoBack(NAHeap* heap, void* data, const void* key, NAInt* backPointer) {
  NAInt newindex;
  NAHeapEntry* thedata;
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
    if(backPointer)
      naError("Heap dos not store backPointers. packpointer should be Null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)) {
    na_GrowHeap(heap);
  }
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataMutable(data);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void na_InsertHeapElementMutableBack(NAHeap* heap, void* data, const void* key, NAInt* backPointer) {
  NAInt newindex;
  NAHeapBackEntry* thedata;
  #if NA_DEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)) {
    na_GrowHeap(heap);
  }
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeapBackEntry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataMutable(data);
  thedata[newindex].key = key;
  if(backPointer) {
    thedata[newindex].backPointer = backPointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an NAInt.
    thedata[newindex].backPointer = heap->data;
    // Bad, bad programming style, but using this trick, a lot of conditional
    // code can be omitted.
  }
  *(thedata[newindex].backPointer) = newindex;
  heap->count++;
}



NA_HDEF const void* na_RemoveHeapRootConstNoBack(NAHeap* heap) {
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  const void* returnvalue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrConst(thedata[1].ptr);
  heap->count--;
  if(heap->count) {
    NAInt curIndex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curIndex] = thedata[heap->count + 1];
  }
  return returnvalue;
}



NA_HDEF const void* na_RemoveHeapRootConstBack(NAHeap* heap) {
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  const void* returnvalue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrConst(thedata[1].ptr);
  *(thedata[1].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    NAInt curIndex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curIndex] = thedata[heap->count + 1];
    *(thedata[curIndex].backPointer) = curIndex;
  }
  return returnvalue;
}



NA_HDEF void* na_RemoveHeapRootMutableNoBack(NAHeap* heap) {
  NAHeapEntry* thedata = (NAHeapEntry*)(heap->data);
  void* returnvalue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrMutable(thedata[1].ptr);
  heap->count--;
  if(heap->count) {
    NAInt curIndex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curIndex] = thedata[heap->count + 1];
  }
  return returnvalue;
}


NA_HDEF void* na_RemoveHeapRootMutableBack(NAHeap* heap) {
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  void* returnvalue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnvalue = naGetPtrMutable(thedata[1].ptr);
  *(thedata[1].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    NAInt curIndex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curIndex] = thedata[heap->count + 1];
    *(thedata[curIndex].backPointer) = curIndex;
  }
  return returnvalue;
}



NA_HDEF const void* na_RemoveHeapPosConstNoBack(NAHeap* heap, NAInt backPointer) {
  NA_UNUSED(heap);
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    naError("Can not update heap as he stores no backPointers. Function has no effect");
  #endif
  return NA_NULL;
}



NA_HDEF const void* na_RemoveHeapPosConstBack(NAHeap* heap, NAInt backPointer) {
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  const void* returnvalue;
  #if NA_DEBUG
    if(backPointer > heap->count)
      naError("backPointer makes no sense.");
    if(backPointer == 0)
      naError("backPointer says that element is not part of the heap.");
  #endif
  returnvalue = naGetPtrConst(thedata[backPointer].ptr);
  *(thedata[backPointer].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    NAInt curIndex = heap->moveup(heap, thedata[heap->count + 1].key, backPointer);
    thedata[curIndex] = thedata[heap->count + 1];
    *(thedata[curIndex].backPointer) = curIndex;
  }
  return returnvalue;
}



NA_HDEF void* na_RemoveHeapPosMutableNoBack(NAHeap* heap, NAInt backPointer) {
  NA_UNUSED(heap);
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    naError("Can not update heap as he stores no backPointers. Function has no effect");
  #endif
  return NA_NULL;
}


NA_HDEF void* na_RemoveHeapPosMutableBack(NAHeap* heap, NAInt backPointer) {
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  void* returnvalue;
  #if NA_DEBUG
    if(backPointer > heap->count)
      naError("backPointer makes no sense.");
    if(backPointer == 0)
      naError("backPointer says that element is not part of the heap.");
  #endif
  returnvalue = naGetPtrMutable(thedata[backPointer].ptr);
  *(thedata[backPointer].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    NAInt curIndex = heap->moveup(heap, thedata[heap->count + 1].key, backPointer);
    thedata[curIndex] = thedata[heap->count + 1];
    *(thedata[curIndex].backPointer) = curIndex;
  }
  return returnvalue;
}


NA_HDEF void na_UpdateHeapElementNoBack(NAHeap* heap, NAInt backPointer) {
  NA_UNUSED(heap);
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    naError("Can not update heap as he stores no backPointers. Function has no effect");
  #endif
}



NA_HDEF void na_UpdateHeapElementBack(NAHeap* heap, NAInt backPointer) {
  NAHeapBackEntry* thedata = (NAHeapBackEntry*)(heap->data);
  NAHeapBackEntry tmp;
  NAInt curIndex;
  #if NA_DEBUG
    if(backPointer > heap->count)
      naError("backPointer makes no sense.");
    if(backPointer == 0)
      naError("backPointer says that element is not part of the heap.");
  #endif
  tmp = thedata[backPointer];
  curIndex = heap->moveup(heap, tmp.key, backPointer);
  curIndex = heap->movedown(heap, tmp.key, curIndex);
  thedata[curIndex] = tmp;
  *(thedata[curIndex].backPointer) = curIndex;
}


// This is the one function where all the function pointers of the NAHeap
// structure are set. After this function, these pointers can no longer be
// changed and therefore define the behaviour of the heap until its deletion.
NA_DEF NAHeap* naInitHeap(NAHeap* heap, NAInt count, NAInt flags) {
  #if NA_DEBUG
    if(!heap)
      naCrash("heap is Null-Pointer");
    // there is always count + 1 elements stored in the array.
    if(count >= NA_MAX_i) {
      naCrash("Heap count is too big.");
      return NA_NULL;
    }
    // Note that the following case count == 0 could be automatically converted
    // to -1. But there might be a different use in the future. Therefore, we
    // define these two values to be invalid now.
    if(count == 0)
      naError("count == 0 is not allowed.");
    // Make sure, the negative count is a power of 2.
    if(count < 0) {
      NAInt countpower = naLog2i(-count);
      if((NAInt)naPow(2., (double)countpower) != -count)
        naError("negative count must be a power of 2.");
    }
  #endif
  heap->maxcount = count;

  if(!(flags & NA_HEAP_STORES_BACKPOINTERS)) {
    // entries store no backPointers

    heap->count = 0;
    heap->data = naMalloc((size_t)(naAbsi(heap->maxcount) + 1) * sizeof(NAHeapEntry));
    heap->root = &(((NAHeapEntry*)heap->data)[1]);

    heap->insertConst = na_InsertHeapElementConstNoBack;
    heap->insertMutable = na_InsertHeapElementMutableNoBack;
    heap->removeConst = na_RemoveHeapRootConstNoBack;
    heap->removeMutable = na_RemoveHeapRootMutableNoBack;
    heap->removePosConst = na_RemoveHeapPosConstNoBack;
    heap->removePosMutable = na_RemoveHeapPosMutableNoBack;
    heap->updateBack = na_UpdateHeapElementNoBack;

    switch(flags & NA_HEAP_DATATYPE_MASK) {
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   double, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, double, 0);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, double, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   double, 0);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   float, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, float, 0);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, float, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   float, 0);
      }
      break;
    case NA_HEAP_USES_NAINT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   NAInt, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, NAInt, 0);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, NAInt, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   NAInt, 0);
      }
      break;
    case NA_HEAP_USES_DATETIME_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   NADateTime, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, NADateTime, 0);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, NADateTime, 0);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   NADateTime, 0);
      }
      break;
    default:
      #if NA_DEBUG
        naCrash("flag combination not implemented.");
        return NA_NULL;
      #else
        break;
      #endif
    }

  }else{
    // Entries store backPointers

    heap->count = 0;
    heap->data = naMalloc((size_t)(naAbsi(heap->maxcount) + 1) * sizeof(NAHeapBackEntry));
    heap->root = &(((NAHeapBackEntry*)heap->data)[1]);

    heap->insertConst = na_InsertHeapElementConstBack;
    heap->insertMutable = na_InsertHeapElementMutableBack;
    heap->removeConst = na_RemoveHeapRootConstBack;
    heap->removeMutable = na_RemoveHeapRootMutableBack;
    heap->removePosConst = na_RemoveHeapPosConstBack;
    heap->removePosMutable = na_RemoveHeapPosMutableBack;
    heap->updateBack = na_UpdateHeapElementBack;

    switch(flags & NA_HEAP_DATATYPE_MASK) {
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   double, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, double, 1);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, double, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   double, 1);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   float, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, float, 1);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, float, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   float, 1);
      }
      break;
    case NA_HEAP_USES_NAINT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   NAInt, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, NAInt, 1);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, NAInt, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   NAInt, 1);
      }
      break;
    case NA_HEAP_USES_DATETIME_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->movedown = NA_T3(na_HeapMoveDown, Less,   NADateTime, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Greater, NADateTime, 1);
      }else{
        heap->movedown = NA_T3(na_HeapMoveDown, Greater, NADateTime, 1);
        heap->moveup   = NA_T3(na_HeapMoveUp,   Less,   NADateTime, 1);
      }
      break;
    default:
      #if NA_DEBUG
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
