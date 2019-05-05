
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NAHeap2.h"
#include "../../NAMemory.h"
#include "../../NABinaryData.h"
#include "../../NAMathOperators.h"


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





NA_HIDEF NABool naGreaterKeydouble(const void* a, const void* b){
  return *(const double*)a > *(const double*)b;
}
NA_HIDEF NABool naLowerKeydouble(const void* a, const void* b){
  return *(const double*)a < *(const double*)b;
}
NA_HIDEF NABool naGreaterKeyfloat(const void* a, const void* b){
  return *(const float*)a > *(const float*)b;
}
NA_HIDEF NABool naLowerKeyfloat(const void* a, const void* b){
  return *(const float*)a < *(const float*)b;
}
NA_HIDEF NABool naGreaterKeyNAInt(const void* a, const void* b){
  return *(const NAInt*)a > *(const NAInt*)b;
}
NA_HIDEF NABool naLowerKeyNAInt(const void* a, const void* b){
  return *(const NAInt*)a < *(const NAInt*)b;
}

// The double indirection macro concatenation is a trick implied by the
// standard. The problem is that the ## directive will not evaluate macros
// therefore, one has to create a forwarding macro which evaluates it and
// then the evaluated macros get concatenated in a second macro. 
#define NA_CONCAT2(a1, a2)            a1 ## a2
#define NA_CONCAT3(a1, a2, a3)        a1 ## a2 ## a3
#define NA_CONCAT4(a1, a2, a3, a4)    a1 ## a2 ## a3 ## a4
#define NA_TOKENIZE2(a1, a2)          NA_CONCAT2(a1,a2)
#define NA_TOKENIZE3(a1, a2, a3)      NA_CONCAT3(a1,a2,a3)
#define NA_TOKENIZE4(a1, a2, a3, a4)  NA_CONCAT4(a1,a2,a3,a4)

#define NA_KEY_OPERATOR(operator, type) NA_CONCAT4(na, operator, Key, type)

// Template function generator. Use this macro to create the name of the
// function with the specified number of template arguments.
#define NA_T1(name, arg1) NA_TOKENIZE2(name, arg1)
#define NA_T2(name, arg1, arg2) NA_TOKENIZE3(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3) NA_TOKENIZE4(name, arg1, arg2, arg3)


#define NA_T_USE_BACKPOINTERS 0
  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Greater
  #define NA_T_DONT_MOVE_UP_COMPARATOR Lower
    #define NA_T_TYPE double
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeap2T.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR

  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Lower
  #define NA_T_DONT_MOVE_UP_COMPARATOR Greater
    #define NA_T_TYPE double
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeap2T.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR
#undef NA_T_USE_BACKPOINTERS

#define NA_T_USE_BACKPOINTERS 1
  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Greater
  #define NA_T_DONT_MOVE_UP_COMPARATOR Lower
    #define NA_T_TYPE double
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeap2T.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR

  #define NA_T_DONT_MOVE_DOWN_COMPARATOR Lower
  #define NA_T_DONT_MOVE_UP_COMPARATOR Greater
    #define NA_T_TYPE double
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE float
      #include "NAHeap2T.h"
    #undef NA_T_TYPE

    #define NA_T_TYPE NAInt
      #include "NAHeap2T.h"
    #undef NA_T_TYPE
  #undef NA_T_DONT_MOVE_DOWN_COMPARATOR
  #undef NA_T_DONT_MOVE_UP_COMPARATOR
#undef NA_T_USE_BACKPOINTERS



NA_HDEF void naGrowHeap2(NAHeap2* heap){
  NAInt entrysize;
  void* newdata;
  entrysize = (NASizeInt)((NAByte*)(heap->root) - (NAByte*)(heap->data));
  newdata = naMalloc((-heap->maxcount * 2 + 1) * entrysize);
  naCopyn(newdata, heap->data, ((heap->count + 1) * entrysize));
  naFree(heap->data);
  heap->data = newdata;
  heap->root = (NAByte*)(heap->data) + entrysize;
  heap->maxcount *= 2;
}



NA_DEF void naShrinkHeap2IfNecessary(NAHeap2* heap){
  #ifndef NDEBUG
    if(heap->maxcount > 0)
      naError("Heap2 defined with a fixed count of elements.");
  #endif
  if((NAInt)heap->count < -heap->maxcount / 4){
    NAInt entrysize = (NASizeInt)((NAByte*)(heap->root) - (NAByte*)(heap->data));
    void* newdata = naMalloc((-heap->maxcount / 2 + 1) * entrysize);
    naCopyn(newdata, heap->data, ((heap->count + 1) * entrysize));
    naFree(heap->data);
    heap->data = newdata;
    heap->root = (NAByte*)(heap->data) + entrysize;
    heap->maxcount /= 2;
  }
}



NA_HDEF void naInsertHeap2ElementConstNoBack(NAHeap2* heap, const void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeap2Entry* thedata;
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap2 overflow.");
    if(backpointer)
      naError("Heap2 dos not store backpointers. packpointer should be null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap2(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeap2Entry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataConst(data);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void naInsertHeap2ElementConstBack(NAHeap2* heap, const void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeap2BackEntry* thedata;
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap2 overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap2(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeap2BackEntry*)(heap->data);
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



NA_HDEF void naInsertHeap2ElementMutableNoBack(NAHeap2* heap, void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeap2Entry* thedata;
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap2 overflow.");
    if(backpointer)
      naError("Heap2 dos not store backpointers. packpointer should be null. Ignored.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap2(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeap2Entry*)(heap->data);
  thedata[newindex].ptr = naMakePtrWithDataMutable(data);
  thedata[newindex].key = key;
  heap->count++;
}



NA_HDEF void naInsertHeap2ElementMutableBack(NAHeap2* heap, void* data, const void* key, NAInt* backpointer){
  NAInt newindex;
  NAHeap2BackEntry* thedata;
  #ifndef NDEBUG
    if((heap->maxcount > 0) && ((NAInt)heap->count == heap->maxcount))
      naError("Heap2 overflow.");
  #endif
  if(heap->maxcount < 0 && ((NAInt)heap->count == -heap->maxcount)){naGrowHeap2(heap);}
  newindex = heap->movedown(heap, key, heap->count + 1);
  thedata = (NAHeap2BackEntry*)(heap->data);
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



NA_HDEF const void* naRemoveHeap2RootConstNoBack(NAHeap2* heap){
  NAHeap2Entry* thedata = (NAHeap2Entry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap2 is empty.");
  #endif
  returnvalue = naGetPtrConst(thedata[1].ptr);
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
  }
  return returnvalue;
}



NA_HDEF const void* naRemoveHeap2RootConstBack(NAHeap2* heap){
  NAHeap2BackEntry* thedata = (NAHeap2BackEntry*)(heap->data);
  const void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap2 is empty.");
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



NA_HDEF void* naRemoveHeap2RootMutableNoBack(NAHeap2* heap){
  NAHeap2Entry* thedata = (NAHeap2Entry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap2 is empty.");
  #endif
  returnvalue = naGetPtrMutable(thedata[1].ptr);
  heap->count--;
  if(heap->count){
    NAInt curindex = heap->moveup(heap, thedata[heap->count + 1].key, 1);
    thedata[curindex] = thedata[heap->count + 1];
  }
  return returnvalue;
}


NA_HDEF void* naRemoveHeap2RootMutableBack(NAHeap2* heap){
  NAHeap2BackEntry* thedata = (NAHeap2BackEntry*)(heap->data);
  void* returnvalue;
  #ifndef NDEBUG
    if(heap->count == 0)
      naError("Heap2 is empty.");
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



NA_HDEF const void* naRemoveHeap2PosConstNoBack(NAHeap2* heap, NAInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("Can not update heap as he stores no backpointers. Function has no effect");
  #endif
  return NA_NULL;
}



NA_HDEF const void* naRemoveHeap2PosConstBack(NAHeap2* heap, NAInt backpointer){
  NAHeap2BackEntry* thedata = (NAHeap2BackEntry*)(heap->data);
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



NA_HDEF void* naRemoveHeap2PosMutableNoBack(NAHeap2* heap, NAInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("Can not update heap as he stores no backpointers. Function has no effect");
  #endif
  return NA_NULL;
}


NA_HDEF void* naRemoveHeap2PosMutableBack(NAHeap2* heap, NAInt backpointer){
  NAHeap2BackEntry* thedata = (NAHeap2BackEntry*)(heap->data);
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


NA_HDEF void naUpdateHeap2ElementNoBack(NAHeap2* heap, NAInt backpointer){
  NA_UNUSED(heap);
  NA_UNUSED(backpointer);
  #ifndef NDEBUG
    naError("Can not update heap as he stores no backpointers. Function has no effect");
  #endif
}



NA_HDEF void naUpdateHeap2ElementBack(NAHeap2* heap, NAInt backpointer){
  NAHeap2BackEntry* thedata = (NAHeap2BackEntry*)(heap->data);
  NAHeap2BackEntry tmp;
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


// This is the one function where all the function pointers of the NAHeap2
// structure are set. After this function, these pointers can no longer be
// changed and therefore define the behaviour of the heap until its deletion.
NA_DEF NAHeap2* naInitHeap2(NAHeap2* heap, NAInt count, NAInt flags){
  NAInt entrysize;
  #ifndef NDEBUG
    if(!heap)
      naCrash("heap is Null-Pointer");
    // there is always count + 1 elements stored in the array.
    if(count >= NA_INT_MAX){
      naCrash("Heap2 count is too big.");
      return NA_NULL;
    }
    // Note that the following case count==0 could be automatically converted
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

    entrysize = sizeof(NAHeap2Entry);
    heap->count = 0;
    heap->data = naMalloc((naAbsi(heap->maxcount) + 1) * entrysize);
    heap->root = &(((NAHeap2Entry*)heap->data)[1]);
    heap->entryByteSize = naSizeof(NAHeap2Entry);

    heap->insertConst = naInsertHeap2ElementConstNoBack;
    heap->insertMutable = naInsertHeap2ElementMutableNoBack;
    heap->removeConst = naRemoveHeap2RootConstNoBack;
    heap->removeMutable = naRemoveHeap2RootMutableNoBack;
    heap->removePosConst = naRemoveHeap2PosConstNoBack;
    heap->removePosMutable = naRemoveHeap2PosMutableNoBack;
    heap->updateBack = naUpdateHeap2ElementNoBack;

    switch(flags & NA_HEAP_DATATYPE_MASK){
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeap2MoveDown, Lower,   double, 0);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Greater, double, 0);
      }else{
        heap->movedown = NA_T3(naHeap2MoveDown, Greater, double, 0);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Lower,   double, 0);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeap2MoveDown, Lower,   float, 0);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Greater, float, 0);
      }else{
        heap->movedown = NA_T3(naHeap2MoveDown, Greater, float, 0);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Lower,   float, 0);
      }
      break;
    case NA_HEAP_USES_NAINT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeap2MoveDown, Lower,   NAInt, 0);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Greater, NAInt, 0);
      }else{
        heap->movedown = NA_T3(naHeap2MoveDown, Greater, NAInt, 0);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Lower,   NAInt, 0);
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

    entrysize = sizeof(NAHeap2BackEntry);
    heap->count = 0;
    heap->data = naMalloc((naAbsi(heap->maxcount) + 1) * entrysize);
    heap->root = &(((NAHeap2BackEntry*)heap->data)[1]);
    heap->entryByteSize = naSizeof(NAHeap2BackEntry);

    heap->insertConst = naInsertHeap2ElementConstBack;
    heap->insertMutable = naInsertHeap2ElementMutableBack;
    heap->removeConst = naRemoveHeap2RootConstBack;
    heap->removeMutable = naRemoveHeap2RootMutableBack;
    heap->removePosConst = naRemoveHeap2PosConstBack;
    heap->removePosMutable = naRemoveHeap2PosMutableBack;
    heap->updateBack = naUpdateHeap2ElementBack;

    switch(flags & NA_HEAP_DATATYPE_MASK){
    case NA_HEAP_USES_DOUBLE_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeap2MoveDown, Lower,   double, 1);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Greater, double, 1);
      }else{
        heap->movedown = NA_T3(naHeap2MoveDown, Greater, double, 1);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Lower,   double, 1);
      }
      break;
    case NA_HEAP_USES_FLOAT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeap2MoveDown, Lower,   float, 1);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Greater, float, 1);
      }else{
        heap->movedown = NA_T3(naHeap2MoveDown, Greater, float, 1);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Lower,   float, 1);
      }
      break;
    case NA_HEAP_USES_NAINT_KEY:
      if(flags & NA_HEAP_IS_MAX_HEAP){
        heap->movedown = NA_T3(naHeap2MoveDown, Lower,   NAInt, 1);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Greater, NAInt, 1);
      }else{
        heap->movedown = NA_T3(naHeap2MoveDown, Greater, NAInt, 1);
        heap->moveup   = NA_T3(naHeap2MoveUp,   Lower,   NAInt, 1);
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
