
// This file contains inline implementations of the file NAStack.h
// Do not include this file directly! It will automatically be included when
// including "NAStack.h"


#include "../NAMemory.h"
#include "../NAMathOperators.h"


struct NAStack{
  NAList          arrays;        // List of void*
  NAListIterator  curPos;        // The list position of the current array.
  NAInt           curIndex;      // The index of the current array in the list.
  NAInt           typeSize;      // The size in bytes of the stored type
  NAInt           usedCount;     // The used number of elements in the storage.
  NAInt           minimalExp;    // An exp of 1 denotes the minimal count of 2.
  #ifndef NDEBUG
    NAInt         iterCount;     // The number of iterators on this stack.
  #endif
};



struct NAStackIterator{
  const NAStack*  stack;         // The const stack
  NAListIterator  listIterator;  // the iterator on the array list
  NAInt           curArrayIndex; // The index of the current array.
  NAInt           cur;           // The index in the current array if available.
};


// Returns the number of elements which can be stored in the array with the
// given index.
NA_HIDEF NAInt naGetStackArrayAvailableCount(const NAStack* stack, NAInt index){
  return (NAInt)1 << (index + stack->minimalExp);
}

// Returns the element index of the first element stored in the array with the
// given index.
NA_HIDEF NAInt naGetStackArrayBaseIndex(const NAStack* stack, NAInt index){
  return ((NAInt)1 << (index + stack->minimalExp)) - ((NAInt)1 << stack->minimalExp);
}


// Returns the total amount of elements which can be stored with all arrays
// up including the one with the given index.
NA_HIDEF NAInt naGetStackTotalCount(const NAStack* stack, NAInt index){
  return naGetStackArrayBaseIndex(stack, index + 1);
}


// This is a helper function which has one purpose: Add another memory block
// to the stack which is double the element count of the previous block. The first block
// has the element count given to naInitStack
NA_HIDEF void naAddStackMoreMemory(NAStack* stack){
  void* newarray;
  newarray = naMalloc(naGetStackArrayAvailableCount(stack, naGetListCount(&(stack->arrays))) * stack->typeSize);
  naAddListLastMutable(&(stack->arrays), newarray);
}



NA_IDEF NAStack* naInitStack(NAStack* stack, NAInt typeSize, NAInt minimalCount){
  #ifndef NDEBUG
    if(!stack)
      naCrash("stack is Null-Pointer");
    if(typeSize <= 0)
      naError("typeSize is invalid.");
    // The following test states that 0 and 1 are invalid values. They could be
    // automatically converted to 2 but they are reserved for future use.
    if(minimalCount < 2)
      naError("minimalCount must be at least 2");
  #endif
  naInitList(&(stack->arrays));
  stack->curPos = naMakeListModifier(&(stack->arrays));
  stack->curIndex = -1;
  stack->typeSize = typeSize;
  stack->usedCount = 0;
  stack->minimalExp = naLog2i(minimalCount);
  #ifndef NDEBUG
    stack->iterCount = 0;
  #endif
  if(stack->minimalExp == 0){stack->minimalExp = 1;}
  naAddStackMoreMemory(stack);
  return stack;
}



NA_IDEF void naClearStack(NAStack* stack){
  #ifndef NDEBUG
    if(!stack)
      naCrash("stack is Null-Pointer.");
    if(stack->iterCount != 0)
      naError("There are still iterators on this stack. Did you forget naClearStackIterator?");
  #endif

  naClearListIterator(&(stack->curPos));

  // Note: We reuse the stack->curPos iterator here because otherwise, we
  // have to declare a new one on top of this function. That would be ugly.
  naBeginListMutatorIteration(void* array, &(stack->arrays), stack->curPos);
    naFree(array);
  naEndListIteration(stack->curPos);

  naClearList(&(stack->arrays));
}



NA_IDEF void* naPushStack(NAStack* stack){
  NAInt availablestack;

  stack->usedCount++;

  availablestack = naGetStackTotalCount(stack, stack->curIndex);

  if(stack->usedCount > availablestack){
    if(naGetListCount(&(stack->arrays)) <= (stack->curIndex + 1)){
      naAddStackMoreMemory(stack);
    }
    naIterateList(&(stack->curPos));
    stack->curIndex++;
  }

  return naTopStack(stack);
}



NA_IDEF void* naTopStack(NAStack* stack){
  NAInt subindex;
  NAByte* array;
  subindex = stack->usedCount - naGetStackArrayBaseIndex(stack, stack->curIndex) - 1;

  array = (NAByte*)naGetListCurMutable(&(stack->curPos));
  return &(array[subindex * stack->typeSize]);
}



NA_IDEF void* naPopStack(NAStack* stack){
  NAInt baseindex;
  void* retvalue = naTopStack(stack);

  stack->usedCount--;

  baseindex = naGetStackArrayBaseIndex(stack, stack->curIndex);
  if((stack->usedCount - 1) < baseindex){
    naIterateListBack(&(stack->curPos));
    stack->curIndex--;
  }

  return retvalue;
}



NA_IDEF void* naPeekStack(NAStack* stack, NAInt index){
  NAListIterator iter = naMakeListMutator(&(stack->arrays));
  NAInt arrayindex = 0;
  void* retvalue = NA_NULL;
  while(naIterateList(&iter)){
    NAInt nextbaseindex = naGetStackArrayBaseIndex(stack, arrayindex + 1);
    if(nextbaseindex > index)
    {
      NAByte* array = naGetListCurMutable(&iter);
      NAInt curbaseindex = naGetStackArrayBaseIndex(stack, arrayindex);
      retvalue = &(array[(index - curbaseindex) * stack->typeSize]);
      break;
    }
    arrayindex++;
  }
  naClearListIterator(&iter);
  return retvalue;
}



NA_IDEF NAInt naGetStackCount(const NAStack* stack){
  return stack->usedCount;
}



// Returns the total amount of elements which can be stored with all arrays
// up including the one with the given index.
NA_IDEF NAInt naGetStackReservedCount(const NAStack* stack){
  // Note that naGetStackTotalCount expects an index, not a count!
  return naGetStackTotalCount(stack, naGetListCount(&(stack->arrays)) - 1);
}



NA_IDEF void naShrinkStackIfNecessary(NAStack* stack, NABool aggressive){
  NAInt desiredcount;
  NAInt listcount;
  desiredcount = (aggressive) ? (stack->usedCount * 2) : (stack->usedCount * 4);
  listcount = naGetListCount(&(stack->arrays));
  while((listcount > 1) && (desiredcount < naGetStackTotalCount(stack, naGetListCount(&(stack->arrays)) - 1))){
    void* removed = naRemoveListLastMutable(&(stack->arrays));
    naFree(removed);
    listcount--;
  }
}



NA_IDEF NAStackIterator naMakeStackAccessor(const NAStack* stack){
  NAStackIterator iter;
  #ifndef NDEBUG
    NAStack* mutablestack;
  #endif
  iter.stack = stack;
  iter.listIterator = naMakeListAccessor(&(stack->arrays));
  iter.curArrayIndex = -1;
  iter.cur = 0;
  #ifndef NDEBUG
    mutablestack = (NAStack*)stack;
    mutablestack->iterCount++;
  #endif
  return iter;
}



NA_IDEF NAStackIterator naMakeStackMutator(const NAStack* stack){
  NAStackIterator iter;
  #ifndef NDEBUG
    NAStack* mutablestack;
  #endif
  iter.stack = stack;
  iter.listIterator = naMakeListMutator(&(stack->arrays));
  iter.curArrayIndex = -1;
  iter.cur = 0;
  #ifndef NDEBUG
    mutablestack = (NAStack*)stack;
    mutablestack->iterCount ++;
  #endif
  return iter;
}



NA_IDEF void naClearStackIterator(NAStackIterator* iterator){
  #ifndef NDEBUG
    NAStack* mutablestack;
  #endif
  naClearListIterator(&(iterator->listIterator));
  #ifndef NDEBUG
    mutablestack = (NAStack*)iterator->stack;
    mutablestack->iterCount --;
    if(mutablestack->iterCount < 0)
      naError("Too many cleared iterators on that stack.");
  #endif
}



NA_IDEF NABool naIterateStack(NAStackIterator* iterator){
  iterator->cur++;
  if((iterator->curArrayIndex == -1) || (iterator->cur >= (NAInt)naGetStackArrayAvailableCount(iterator->stack, iterator->curArrayIndex))){
    naIterateList(&(iterator->listIterator));
    iterator->curArrayIndex++;
    iterator->cur = 0;
  }
  return (naGetStackArrayBaseIndex(iterator->stack, iterator->curArrayIndex) + iterator->cur < iterator->stack->usedCount);
}



NA_IDEF const void* naGetStackCurConst(NAStackIterator* iterator){
  // If you experience an error somewhere after this function call and believe
  // that garbage values are returned, maybe you need to use the pointer-variant
  // of this function? naGetStackCurConst
  const NAByte* curbase;
  if(naGetStackArrayBaseIndex(iterator->stack, iterator->curArrayIndex) + iterator->cur >= iterator->stack->usedCount){return NA_NULL;}
  curbase = (const NAByte*)naGetListCurConst(&(iterator->listIterator));
  return &(curbase[iterator->cur * iterator->stack->typeSize]);
}



NA_IDEF void* naGetStackCurMutable(NAStackIterator* iterator){
  // If you experience an error somewhere after this function call and believe
  // that garbage values are returned, maybe you need to use the pointer-variant
  // of this function? naGetStackCurpMutable
  NAByte* curbase;
  if(naGetStackArrayBaseIndex(iterator->stack, iterator->curArrayIndex) + iterator->cur >= iterator->stack->usedCount){return NA_NULL;}
  curbase = (NAByte*)naGetListCurMutable(&(iterator->listIterator));
  return &(curbase[iterator->cur * iterator->stack->typeSize]);
}



NA_IDEF const void* naGetStackCurpConst(NAStackIterator* iterator){
  const void** dataptr = (const void**)naGetStackCurConst(iterator);
  return *dataptr;
//
//  const NAByte* curbase;
//  if(naGetStackArrayBaseIndex(iterator->stack, iterator->curArrayIndex) + iterator->cur >= iterator->stack->usedCount){return NA_NULL;}
//  curbase = (const NAByte*)naGetListCurConst(&(iterator->listIterator));
//  return *((const void**)&(curbase[iterator->cur * iterator->stack->typeSize]));
}



NA_IDEF void* naGetStackCurpMutable(NAStackIterator* iterator){
  void** dataptr = (void**)naGetStackCurConst(iterator);
  return *dataptr;
//  const NAByte* curbase;
//  if(naGetStackArrayBaseIndex(iterator->stack, iterator->curArrayIndex) + iterator->cur >= iterator->stack->usedCount){return NA_NULL;}
//  curbase = (const NAByte*)naGetListCurMutable(&(iterator->listIterator));
//  return *((void**)&(curbase[iterator->cur * iterator->stack->typeSize]));
}



NA_IDEF void naForeachStackConst(const NAStack* stack, NAAccessor accessor){
  // If you experience an error somewhere within the mutator called by this
  // function, maybe you need to use the pointer-variant of this function?
  // naForeachStackpConst
  NAStackIterator iter = naMakeStackAccessor(stack);
  while(naIterateStack(&iter)){
    const void* data = naGetStackCurConst(&iter);
    accessor(data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackMutable(const NAStack* stack, NAMutator mutator){
  // If you experience an error somewhere within the mutator called by this
  // function, maybe you need to use the pointer-variant of this function?
  // naForeachStackpMutable
  NAStackIterator iter = naMakeStackMutator(stack);
  while(naIterateStack(&iter)){
    void* data = naGetStackCurMutable(&iter);
    mutator(data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackpConst(const NAStack* stack, NAAccessor accessor){
  NAStackIterator iter = naMakeStackAccessor(stack);
  while(naIterateStack(&iter)){
    const void* const* data = (const void* const*)naGetStackCurConst(&iter);
    accessor(*data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackpMutable(const NAStack* stack, NAMutator mutator){
  NAStackIterator iter = naMakeStackMutator(stack);
  while(naIterateStack(&iter)){
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
