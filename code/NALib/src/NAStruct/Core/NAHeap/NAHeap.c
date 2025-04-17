
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

    #define NA_T_TYPE int64
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

    #define NA_T_TYPE int64
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

    #define NA_T_TYPE int64
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

    #define NA_T_TYPE int64
      #include "NAHeapT.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NADateTime
      #include "NAHeapT.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR
#undef NA_T_USE_BACKPOINTERS



NA_HDEF void na_GrowHeap(NAHeap* heap) {
  #if NA_DEBUG
    if(!heap->autoGrow)
      naError("Heap defined with a fixed count of elements.");
  #endif
  ptrdiff_t entryDiff = (NAByte*)(heap->root) - (NAByte*)(heap->data);
  #if NA_DEBUG
    if(entryDiff <= 0)
      naError("Invalid entry computation.");
  #endif
  size_t entrySize = (size_t)entryDiff;
  void* newData = naMalloc((heap->maxCount * 2 + 1) * entrySize);
  naCopyn(newData, heap->data, (heap->count + 1) * entrySize);
  naFree(heap->data);
  heap->data = newData;
  heap->root = (NAByte*)(heap->data) + entrySize;
  heap->maxCount *= 2;
}



NA_DEF void naShrinkHeapIfNecessary(NAHeap* heap) {
  #if NA_DEBUG
    if(!heap->autoGrow)
      naError("Heap defined with a fixed count of elements.");
  #endif
  if(heap->count < heap->maxCount / 4) {
    ptrdiff_t entryDiff = (NAByte*)(heap->root) - (NAByte*)(heap->data);
    #if NA_DEBUG
      if(entryDiff <= 0)
        naError("Invalid entry computation.");
    #endif
    size_t entrySize = (size_t)entryDiff;
    void* newData = naMalloc((heap->maxCount / 2 + 1) * entrySize);
    naCopyn(newData, heap->data, (heap->count + 1) * entrySize);
    naFree(heap->data);
    heap->data = newData;
    heap->root = (NAByte*)(heap->data) + entrySize;
    heap->maxCount /= 2;
  }
}



NA_HDEF void na_InsertHeapElementConstNoBack(NAHeap* heap, const void* data, const void* key, int64* backPointer) {
  int64 newIndex;
  NAHeapEntry* theData;
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    if(!heap->autoGrow && (heap->count == heap->maxCount))
      naCrash("Heap overflow.");
    if(backPointer)
      naError("Heap dos not store backPointers. packpointer should be nullptr. Ignored.");
  #endif
  if(heap->autoGrow && (heap->count == heap->maxCount)) {
    na_GrowHeap(heap);
  }
  newIndex = heap->moveDown(heap, key, (int64)(heap->count + 1));
  theData = (NAHeapEntry*)(heap->data);
  theData[newIndex].ptr = naMakePtrWithDataConst(data);
  theData[newIndex].key = key;
  heap->count++;
}



NA_HDEF void na_InsertHeapElementConstBack(NAHeap* heap, const void* data, const void* key, int64* backPointer) {
  int64 newIndex;
  NAHeapBackEntry* theData;
  #if NA_DEBUG
    if(!heap->autoGrow && (heap->count == heap->maxCount))
      naCrash("Heap overflow.");
  #endif
  if(heap->autoGrow && (heap->count == heap->maxCount)) {
    na_GrowHeap(heap);
  }
  newIndex = heap->moveDown(heap, key, (int64)(heap->count + 1));
  theData = (NAHeapBackEntry*)(heap->data);
  theData[newIndex].ptr = naMakePtrWithDataConst(data);
  theData[newIndex].key = key;
  if(backPointer) {
    theData[newIndex].backPointer = backPointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an int64.
    theData[newIndex].backPointer = heap->data;
    // Bad, bad programming style, but using this trick, a lot of conditional
    // code can be omitted.
  }
  *(theData[newIndex].backPointer) = newIndex;
  heap->count++;
}



NA_HDEF void na_InsertHeapElementMutableNoBack(NAHeap* heap, void* data, const void* key, int64* backPointer) {
  int64 newIndex;
  NAHeapEntry* theData;
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    if(!heap->autoGrow && (heap->count == heap->maxCount))
      naCrash("Heap overflow.");
    if(backPointer)
      naError("Heap does not store backPointers. packpointer should be nullptr. Ignored.");
  #endif
  if(heap->autoGrow && (heap->count == heap->maxCount)) {
    na_GrowHeap(heap);
  }
  newIndex = heap->moveDown(heap, key, (int64)(heap->count + 1));
  theData = (NAHeapEntry*)(heap->data);
  theData[newIndex].ptr = naMakePtrWithDataMutable(data);
  theData[newIndex].key = key;
  heap->count++;
}



NA_HDEF void na_InsertHeapElementMutableBack(NAHeap* heap, void* data, const void* key, int64* backPointer) {
  int64 newIndex;
  NAHeapBackEntry* theData;
  #if NA_DEBUG
    if(!heap->autoGrow && (heap->count == heap->maxCount))
      naCrash("Heap overflow.");
  #endif
  if(heap->autoGrow && (heap->count == heap->maxCount)) {
    na_GrowHeap(heap);
  }
  newIndex = heap->moveDown(heap, key, (int64)(heap->count + 1));
  theData = (NAHeapBackEntry*)(heap->data);
  theData[newIndex].ptr = naMakePtrWithDataMutable(data);
  theData[newIndex].key = key;
  if(backPointer) {
    theData[newIndex].backPointer = backPointer;
  }else{
    // The element 0 of the data field is a dummy field which is more than
    // wide enough to store an int64.
    theData[newIndex].backPointer = heap->data;
    // Bad, bad programming style, but using this trick, a lot of conditional
    // code can be omitted.
  }
  *(theData[newIndex].backPointer) = newIndex;
  heap->count++;
}



NA_HDEF const void* na_RemoveHeapRootConstNoBack(NAHeap* heap) {
  NAHeapEntry* theData = (NAHeapEntry*)(heap->data);
  const void* returnValue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnValue = naGetPtrConst(theData[1].ptr);
  heap->count--;
  if(heap->count) {
    int64 curIndex = heap->moveUp(heap, theData[heap->count + 1].key, 1);
    theData[curIndex] = theData[heap->count + 1];
  }
  return returnValue;
}



NA_HDEF const void* na_RemoveHeapRootConstBack(NAHeap* heap) {
  NAHeapBackEntry* theData = (NAHeapBackEntry*)(heap->data);
  const void* returnValue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnValue = naGetPtrConst(theData[1].ptr);
  *(theData[1].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    int64 curIndex = heap->moveUp(heap, theData[heap->count + 1].key, 1);
    theData[curIndex] = theData[heap->count + 1];
    *(theData[curIndex].backPointer) = curIndex;
  }
  return returnValue;
}



NA_HDEF void* na_RemoveHeapRootMutableNoBack(NAHeap* heap) {
  NAHeapEntry* theData = (NAHeapEntry*)(heap->data);
  void* returnValue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnValue = naGetPtrMutable(theData[1].ptr);
  heap->count--;
  if(heap->count) {
    int64 curIndex = heap->moveUp(heap, theData[heap->count + 1].key, 1);
    theData[curIndex] = theData[heap->count + 1];
  }
  return returnValue;
}


NA_HDEF void* na_RemoveHeapRootMutableBack(NAHeap* heap) {
  NAHeapBackEntry* theData = (NAHeapBackEntry*)(heap->data);
  void* returnValue;
  #if NA_DEBUG
    if(heap->count == 0)
      naError("Heap is empty.");
  #endif
  returnValue = naGetPtrMutable(theData[1].ptr);
  *(theData[1].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    int64 curIndex = heap->moveUp(heap, theData[heap->count + 1].key, 1);
    theData[curIndex] = theData[heap->count + 1];
    *(theData[curIndex].backPointer) = curIndex;
  }
  return returnValue;
}



NA_HDEF const void* na_RemoveHeapPosConstNoBack(NAHeap* heap, int64 backPointer) {
  NA_UNUSED(heap);
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    naError("Can not update heap as he stores no backPointers. Function has no effect");
  #endif
  return NA_NULL;
}



NA_HDEF const void* na_RemoveHeapPosConstBack(NAHeap* heap, int64 backPointer) {
  NAHeapBackEntry* theData = (NAHeapBackEntry*)(heap->data);
  const void* returnValue;
  #if NA_DEBUG
    if(backPointer > (int64)heap->count)
      naError("backPointer makes no sense.");
    if(backPointer == 0)
      naError("backPointer says that element is not part of the heap.");
  #endif
  returnValue = naGetPtrConst(theData[backPointer].ptr);
  *(theData[backPointer].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    int64 curIndex = heap->moveUp(heap, theData[heap->count + 1].key, backPointer);
    theData[curIndex] = theData[heap->count + 1];
    *(theData[curIndex].backPointer) = curIndex;
  }
  return returnValue;
}



NA_HDEF void* na_RemoveHeapPosMutableNoBack(NAHeap* heap, int64 backPointer) {
  NA_UNUSED(heap);
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    naError("Can not update heap as he stores no backPointers. Function has no effect");
  #endif
  return NA_NULL;
}


NA_HDEF void* na_RemoveHeapPosMutableBack(NAHeap* heap, int64 backPointer) {
  NAHeapBackEntry* theData = (NAHeapBackEntry*)(heap->data);
  void* returnValue;
  #if NA_DEBUG
    if(backPointer > (int64)heap->count)
      naError("backPointer makes no sense.");
    if(backPointer == 0)
      naError("backPointer says that element is not part of the heap.");
  #endif
  returnValue = naGetPtrMutable(theData[backPointer].ptr);
  *(theData[backPointer].backPointer) = 0;
  heap->count--;
  if(heap->count) {
    int64 curIndex = heap->moveUp(heap, theData[heap->count + 1].key, backPointer);
    theData[curIndex] = theData[heap->count + 1];
    *(theData[curIndex].backPointer) = curIndex;
  }
  return returnValue;
}


NA_HDEF void na_UpdateHeapElementNoBack(NAHeap* heap, int64 backPointer) {
  NA_UNUSED(heap);
  NA_UNUSED(backPointer);
  #if NA_DEBUG
    naError("Can not update heap as he stores no backPointers. Function has no effect");
  #endif
}



NA_HDEF void na_UpdateHeapElementBack(NAHeap* heap, int64 backPointer) {
  NAHeapBackEntry* theData = (NAHeapBackEntry*)(heap->data);
  NAHeapBackEntry tmp;
  int64 curIndex;
  #if NA_DEBUG
    if(backPointer > (int64)heap->count)
      naError("backPointer makes no sense.");
    if(backPointer == 0)
      naError("backPointer says that element is not part of the heap.");
  #endif
  tmp = theData[backPointer];
  curIndex = heap->moveUp(heap, tmp.key, backPointer);
  curIndex = heap->moveDown(heap, tmp.key, curIndex);
  theData[curIndex] = tmp;
  *(theData[curIndex].backPointer) = curIndex;
}


// This is the one function where all the function pointers of the NAHeap
// structure are set. After this function, these pointers can no longer be
// changed and therefore define the behaviour of the heap until its deletion.
NA_DEF NAHeap* naInitHeap(NAHeap* heap, size_t count, uint32 flags) {
  #if NA_DEBUG
    if(!heap)
      naCrash("heap is nullptr");
    if(count == NA_MAX_s)
      naCrash("count is too big");
  #endif

  // there is always count + 1 elements stored in the array.

  heap->autoGrow = count == 0;
  heap->maxCount = heap->autoGrow ? 1 : count;

  if(!(flags & NA_HEAP_STORES_BACKPOINTERS)) {
    // entries store no backPointers

    heap->count = 0;
    heap->data = naMalloc((heap->maxCount + 1) * sizeof(NAHeapEntry));
    heap->root = &((NAHeapEntry*)heap->data)[1];

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
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   double, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, double, 0);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, double, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   double, 0);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   float, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, float, 0);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, float, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   float, 0);
      }
      break;
    case NA_HEAP_USES_INT64_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   int64, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, int64, 0);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, int64, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   int64, 0);
      }
      break;
    case NA_HEAP_USES_DATETIME_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   NADateTime, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, NADateTime, 0);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, NADateTime, 0);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   NADateTime, 0);
      }
      break;
    default:
      #if NA_DEBUG
        naCrash("flag combination not implemented.");
      #else
        break;
      #endif
    }

  }else{
    // Entries store backPointers

    heap->count = 0;
    heap->data = naMalloc((heap->maxCount + 1) * sizeof(NAHeapBackEntry));
    heap->root = &((NAHeapBackEntry*)heap->data)[1];

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
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   double, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, double, 1);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, double, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   double, 1);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   float, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, float, 1);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, float, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   float, 1);
      }
      break;
    case NA_HEAP_USES_INT64_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   int64, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, int64, 1);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, int64, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   int64, 1);
      }
      break;
    case NA_HEAP_USES_DATETIME_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP) {
        heap->moveDown = NA_T3(na_HeapMoveDown, Less,   NADateTime, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Greater, NADateTime, 1);
      }else{
        heap->moveDown = NA_T3(na_HeapMoveDown, Greater, NADateTime, 1);
        heap->moveUp   = NA_T3(na_HeapMoveUp,   Less,   NADateTime, 1);
      }
      break;
    default:
      #if NA_DEBUG
        naCrash("flag combination not implemented.");
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
