
// This file contains inline implementations of the file NAStack.h
// Do not include this file directly! It will automatically be included when
// including "NAStack.h"


#include "../../NAList.h"
#include "../../../NAUtility/NAMemory.h"
#include "../../../NAMath/NAMathOperators.h"



struct NAStack{
  size_t         typeSize;     // The size of the stored elements in bytes.
  NAInt          flags;        // Flags defining the behaviour of the stack.
  NAList         arrays;       // List of StackArray (void*)
  NAListIterator curArray;     // List position of current array
  size_t         curBaseIndex; // Absolute index of current array
  size_t         curCount;     // Number of used elements in current array
  #if NA_DEBUG
    NAInt        iterCount;     // The number of iterators on this stack.
  #endif
};



struct NAStackIterator{
  const NAStack* stack;         // The const stack
  NAListIterator curArray;      // the iterator on the array list
  size_t         cur;           // The index in the current array if available
  #if NA_DEBUG
    NABool mutator;             // Stores whether iter is mutable or const
    size_t curBaseIndex;        // Absolute index of current array. Only used
                                // for checking if iterator still valid.
  #endif
};



#define NA_STACK_GROW_MASK         0x0f
#define NA_STACK_SHRINK_MASK       0xf0



NA_HAPI void           na_GrowStack(NAStack* stack);
NA_HAPI void           na_ShrinkStack(NAStack* stack);
NA_HIAPI void*         na_AllocStackArray(size_t count, size_t typeSize);
NA_HIAPI void          na_DeallocStackArray(void* array);
NA_HIAPI size_t        na_GetStackArrayCount(const NAListIterator* iter);
NA_HIAPI const NAByte* na_GetStackArrayFirstConst(const NAListIterator* iter);
NA_HIAPI NAByte*       na_GetStackArrayFirstMutable(NAListIterator* iter);
NA_HIAPI void*         na_GetStackArrayAt(NAListIterator* iter, size_t index, size_t typeSize);
#if NA_DEBUG
  NA_HIAPI NABool na_IsStackIteratorPastEnd(NAStackIterator* iter);
#endif


NA_HIDEF void* na_AllocStackArray(size_t count, size_t typeSize) {
  #if NA_DEBUG
    if(!count)
      naError("count must be > 0");
    if(!typeSize)
      naError("typeSize must be > 0");
  #endif
  void* array = naMalloc(sizeof(size_t) + typeSize * count);
  *((size_t*)array) = count;
  return array;
}



NA_HIDEF void na_DeallocStackArray(void* array) {
  #if NA_DEBUG
    if(!array)
      naError("array is Null");
  #endif
  naFree(array);
}



NA_HIDEF size_t na_GetStackArrayCount(const NAListIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iter is Null");
    if(naIsListAtInitial(iter))
      naCrash("iterator is at initial position");
  #endif
  return *((size_t*)naGetListCurConst(iter));
}



NA_HIDEF const NAByte* na_GetStackArrayFirstConst(const NAListIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iter is Null");
    if(naIsListAtInitial(iter))
      naCrash("iterator is at initial position");
  #endif
  return (const NAByte*)naGetListCurConst(iter) + sizeof(size_t);
}



NA_HIDEF NAByte* na_GetStackArrayFirstMutable(NAListIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iter is Null");
    if(naIsListAtInitial(iter))
      naCrash("iterator is at initial position");
  #endif
  return (NAByte*)naGetListCurMutable(iter) + sizeof(size_t);
}



NA_HIDEF void* na_GetStackArrayAt(NAListIterator* iter, size_t index, size_t typeSize) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iter is Null");
    if(naIsListAtInitial(iter))
      naCrash("iterator is at initial position");
    if(index >= na_GetStackArrayCount(iter))
      naError("index out of bounds");
    if(!typeSize)
      naError("typeSize must be > 0");
  #endif
  return na_GetStackArrayFirstMutable(iter) + typeSize * index;
}



NA_IDEF void* naTopStack(NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
    if(stack->curCount == 0)
      naError("stack is Empty");
  #endif

  return na_GetStackArrayAt(
    &(stack->curArray),
    stack->curCount - 1,
    stack->typeSize);
}



NA_IDEF void* naPushStack(NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
  #endif

  if(stack->curCount == na_GetStackArrayCount(&(stack->curArray))) {
    na_GrowStack(stack);
  }

  stack->curCount++;
  return naTopStack(stack);
}



NA_IDEF void* naPopStack(NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
    if(stack->curCount == 0)
      naError("stack is Empty");
  #endif

  void* retValue = naTopStack(stack);
  stack->curCount--;

  if(stack->curCount == 0) {
    na_ShrinkStack(stack);
  }

  return retValue;
}



NA_IDEF size_t naGetStackCount(const NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
  #endif
  return stack->curBaseIndex + stack->curCount;
}



NA_IDEF NAStackIterator naMakeStackAccessor(const NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
  #endif

  NAStackIterator iter;
  #if NA_DEBUG
    NAStack* mutablestack;
  #endif
  iter.stack = stack;
  iter.curArray = naMakeListMutator(&(stack->arrays));
  iter.cur = 0;
  #if NA_DEBUG
    iter.mutator = NA_FALSE;
    iter.curBaseIndex = 0;
    mutablestack = (NAStack*)stack;
    mutablestack->iterCount++;
  #endif
  return iter;
}



NA_IDEF NAStackIterator naMakeStackMutator(const NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
  #endif

  NAStackIterator iter;
  #if NA_DEBUG
    NAStack* mutablestack;
  #endif
  iter.stack = stack;
  iter.curArray = naMakeListMutator(&(stack->arrays));
  iter.cur = 0;
  #if NA_DEBUG
    iter.mutator = NA_TRUE;
    iter.curBaseIndex = 0;
    mutablestack = (NAStack*)stack;
    mutablestack->iterCount++;
  #endif
  return iter;
}



NA_IDEF void naClearStackIterator(NAStackIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null");
  #endif

  #if NA_DEBUG
    NAStack* mutablestack;
  #endif
  naClearListIterator(&(iter->curArray));
  #if NA_DEBUG
    mutablestack = (NAStack*)iter->stack;
    mutablestack->iterCount--;
    if(mutablestack->iterCount < 0)
      naError("Too many cleared iterators on that stack.");
  #endif
}



#if NA_DEBUG
  NA_HIDEF NABool na_IsStackIteratorPastEnd(NAStackIterator* iter) {
    return iter->curBaseIndex + iter->cur >= naGetStackCount(iter->stack);
  }
#endif



NA_IDEF NABool naIterateStack(NAStackIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null");
    if(!naIsStackAtInitial(iter) && na_IsStackIteratorPastEnd(iter))
      naError("iterator points past the end of the stack");
  #endif

  if(naIsListAtInitial(&(iter->curArray))) {
    naIterateList(&(iter->curArray));
  }else{
    iter->cur++;
  }

  if(naEqualListIterator(&(iter->curArray), &(iter->stack->curArray)) && iter->cur == iter->stack->curCount)
  {
    // We reached the end of iteration.
    naResetStackIterator(iter);
    return NA_FALSE;
  }

  if(iter->cur == na_GetStackArrayCount(&(iter->curArray))) {
    #if NA_DEBUG
      iter->curBaseIndex += na_GetStackArrayCount(&(iter->curArray));
    #endif
    naIterateList(&(iter->curArray));
    iter->cur = 0;
  }
  return NA_TRUE;
}



NA_IDEF void naResetStackIterator(NAStackIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null");
  #endif

  naResetListIterator(&(iter->curArray));
  iter->cur = 0;
  #if NA_DEBUG
    iter->curBaseIndex = 0;
  #endif
}



NA_IDEF NABool naIsStackAtInitial(NAStackIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null");
  #endif

  return naIsListAtInitial(&(iter->curArray));
}



 const void* naGetStackCurConst(NAStackIterator* iter) {
  // If you experience an error somewhere after this function call and believe
  // that garbage values are returned, maybe you need to use the pointer-variant
  // of this function? naGetStackCurConst
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null");
    if(naIsStackAtInitial(iter))
      naCrash("iterator is at initial position.");
    if(na_IsStackIteratorPastEnd(iter))
      naError("iterator points past the end of the stack");
  #endif

  return na_GetStackArrayAt(&(iter->curArray), iter->cur, iter->stack->typeSize);
}



NA_IDEF void* naGetStackCurMutable(NAStackIterator* iter) {
  #if NA_DEBUG
  if(!iter)
    naCrash("iterator is Null");
  if(naIsStackAtInitial(iter))
    naCrash("iterator is at initial position.");
  if(!iter->mutator)
    naError("Trying to mutate with an accessor.");
  if(na_IsStackIteratorPastEnd(iter))
    naError("iterator points past the end of the stack");
#endif

  // If you experience an error somewhere after this function call and believe
  // that garbage values are returned, maybe you need to use the pointer-variant
  // of this function? naGetStackCurpMutable

  return na_GetStackArrayAt(&(iter->curArray), iter->cur, iter->stack->typeSize);
}



NA_IDEF const void* naGetStackCurpConst(NAStackIterator* iter) {
  #if NA_DEBUG
    if(!iter)
      naCrash("iterator is Null");
    if(naIsStackAtInitial(iter))
      naCrash("iterator is at initial position.");
    if(na_IsStackIteratorPastEnd(iter))
      naError("iterator points past the end of the stack");
  #endif

  const void** dataptr = (const void**)naGetStackCurConst(iter);
  return *dataptr;
}



NA_IDEF void* naGetStackCurpMutable(NAStackIterator* iter) {
  #if NA_DEBUG
  if(!iter)
    naCrash("iterator is Null");
  if(naIsStackAtInitial(iter))
    naCrash("iterator is at initial position.");
  if(!iter->mutator)
    naError("Trying to mutate with an accessor.");
  if(na_IsStackIteratorPastEnd(iter))
    naError("iterator points past the end of the stack");
#endif

  void** dataptr = (void**)naGetStackCurConst(iter);
  return *dataptr;
}



NA_IDEF void naForeachStackConst(const NAStack* stack, NAAccessor accessor) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
    if(!accessor)
      naCrash("accessor is Null");
  #endif

  // If you experience an error somewhere within the mutator called by this
  // function, maybe you need to use the pointer-variant of this function?
  // naForeachStackpConst

  NAStackIterator iter = naMakeStackAccessor(stack);
  while(naIterateStack(&iter)) {
    const void* data = naGetStackCurConst(&iter);
    accessor(data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackMutable(const NAStack* stack, NAMutator mutator) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
    if(!mutator)
      naCrash("mutator is Null");
  #endif

  // If you experience an error somewhere within the mutator called by this
  // function, maybe you need to use the pointer-variant of this function?
  // naForeachStackpMutable

  NAStackIterator iter = naMakeStackMutator(stack);
  while(naIterateStack(&iter)) {
    void* data = naGetStackCurMutable(&iter);
    mutator(data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackpConst(const NAStack* stack, NAAccessor accessor) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
    if(!accessor)
      naCrash("accessor is Null");
  #endif

  NAStackIterator iter = naMakeStackAccessor(stack);
  while(naIterateStack(&iter)) {
    const void* const* data = (const void* const*)naGetStackCurConst(&iter);
    accessor(*data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackpMutable(const NAStack* stack, NAMutator mutator) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is Null");
    if(!mutator)
      naCrash("mutator is Null");
  #endif

  NAStackIterator iter = naMakeStackMutator(stack);
  while(naIterateStack(&iter)) {
    void** data = (void**)naGetStackCurMutable(&iter);
    mutator(*data);
  }
  naClearStackIterator(&iter);
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
