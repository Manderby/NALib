
#include "../../NAStack.h"



NA_DEF NAStack* naInitStack(
  NAStack* stack,
  size_t typeSize,
  size_t initialCount,
  uint32 flags)
{
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
    if(typeSize < 1)
      naCrash("typeSize must be at least 1");
    if((flags & NA_STACK_GROW_MASK) == NA_STACK_FIXED_SIZE && initialCount == 0)
      naError("You must specify an initialCount when using a fixed size stack.");
  #endif

  stack->typeSize = typeSize;
  stack->flags = flags;
  naInitList(&stack->arrays);

  if(!initialCount) {
    // Compute the initial count automatically such that the first array fits
    // inside a memory page if possible.
    initialCount = (naGetSystemMemoryPagesize() - sizeof(size_t)) / typeSize;
    initialCount = naMaxs(initialCount, 1);
  }
  void* newArray = na_AllocStackArray(initialCount, stack->typeSize);
  naAddListLastMutable(&stack->arrays, newArray);

  // Create the cur pointer and let it point to the array just created.
  stack->curArray = naMakeListMutator(&stack->arrays);
  naIterateList(&stack->curArray);

  stack->curBaseIndex = 0;
  stack->curCount = 0;

  #if NA_DEBUG
    stack->iterCount = 0;
  #endif
  return stack;
}



NA_DEF void naClearStack(NAStack* stack) {
  #if NA_DEBUG
  if(!stack)
    naCrash("stack is nullptr");
  if(stack->iterCount != 0)
    naError("There are still iterators on this stack. Did you forget naClearStackIterator?");
  #endif

  naClearListIterator(&stack->curArray);
  naClearList(&stack->arrays, (NAMutator)na_DeallocStackArray);
}



NA_DEF void* naPeekStack(NAStack* stack, size_t index) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
    if(index >= naGetStackCount(stack))
      naError("index out of bounds.");
  #endif

  NAListIterator iter = naMakeListMutator(&stack->arrays);
  size_t curBaseIndex = 0;
  void* retValue = NA_NULL;
  while(naIterateList(&iter)) {
    size_t nextBaseIndex = curBaseIndex + na_GetStackArrayCount(&iter);
    if(nextBaseIndex > index)
    {
      naClearListIterator(&iter);
      return na_GetStackArrayAt(&iter, index - curBaseIndex, stack->typeSize);
    }
    curBaseIndex = nextBaseIndex;
  }
  naClearListIterator(&iter);
  return retValue;
}



NA_HDEF void na_GrowStack(NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
  #endif

  size_t lastArrayCount = na_GetStackArrayCount(&stack->curArray);

  if(naIsListAtLast(&stack->curArray)) {
    // If there is no more array in the list, we need to grow the stack.
    size_t newArrayCount;

    switch(stack->flags & NA_STACK_GROW_MASK) {
    case NA_STACK_GROW_LINEAR:
      newArrayCount = lastArrayCount;
      break;
    case NA_STACK_GROW_AUTO:
    case NA_STACK_GROW_FIBONACCI:
      // Yes, this is cheating. You would properly add the count of the last
      // and second to last array, but it has been proven that with bigger
      // numbers, this is the same as the golden ratio. And finding out if
      // there actually is a second to last array is just a nuisance.
      newArrayCount = (size_t)naCeil((1 + NA_GOLD_RATIO_BIG) * (double)lastArrayCount);
      break;
    case NA_STACK_GROW_EXPONENTIAL:
      newArrayCount = 2 * lastArrayCount;
      break;
    case NA_STACK_FIXED_SIZE:
      #if NA_DEBUG
        naError("Stack overflow.");
      #endif
      newArrayCount = 0;
      break;
    default:
      #if NA_DEBUG
        naError("Unknown grow option.");
      #endif
      newArrayCount = 0;
      break;
    }

    void* newArray = na_AllocStackArray(newArrayCount, stack->typeSize);
    naAddListLastMutable(&stack->arrays, newArray);
  }

  // Now, we have an array with spare elements.
  naIterateList(&stack->curArray);
  stack->curBaseIndex += lastArrayCount;
  stack->curCount = 0;
}



NA_HDEF void na_ShrinkStack(NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
  #endif

  if(!naIsListAtFirst(&stack->curArray)) {
    // We remember the position of the old array
    NAListIterator oldArrayIter = naMakeListAccessor(&stack->arrays);
    naLocateListIterator(&oldArrayIter, &stack->curArray);

    // We iterate one array backwards
    naIterateListBack(&stack->curArray);
    stack->curCount = na_GetStackArrayCount(&stack->curArray);
    stack->curBaseIndex -= stack->curCount;

    // After we positioned the stack anew, we can shrink if necessary.
    switch(stack->flags & NA_STACK_SHRINK_MASK) {
    case NA_STACK_SHRINK_AUTO:
      // Remove and deallocate all arrays which are after the old array iter.
      while(!naIsListAtLast(&oldArrayIter)) {
        na_DeallocStackArray(naRemoveListLastMutable(&stack->arrays));
      }
      break;
    case NA_STACK_NO_SHRINKING:
      // Do nothing
      break;
    }

    naClearListIterator(&oldArrayIter);
  }
}



NA_DEF size_t naGetStackReservedCount(const NAStack* stack) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
  #endif
  size_t retSize = stack->curBaseIndex;

  // We us an iterator starting from the current array position of the stack
  // and add the count of all remaining arrays.
  NAListIterator curArray = naMakeListAccessor(&stack->arrays);
  naLocateListIterator(&curArray, &stack->curArray);
  retSize += na_GetStackArrayCount(&curArray);

  while(naIterateList(&curArray)) {
    retSize += na_GetStackArrayCount(&curArray);
  }

  naClearListIterator(&curArray);
  return retSize;
}



NA_DEF void naShrinkStackIfNecessary(NAStack* stack, NABool aggressive) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
  #endif

  // We locate an iterator at the currently used array
  NAListIterator arrayIter = naMakeListAccessor(&stack->arrays);
  naLocateListIterator(&arrayIter, &stack->curArray);

  // If not aggressive, we look one paste this array.
  if(!aggressive) {
    naIterateList(&arrayIter);
  }
  
  // Delete as long as there are arrays after this one.
  while(!naIsListAtInitial(&arrayIter) && !naIsListAtLast(&arrayIter)) {
    na_DeallocStackArray(naRemoveListLastMutable(&stack->arrays));
  }
  naClearListIterator(&arrayIter);
}



NA_DEF void naDumpStack(NAStack* stack, void* buf) {
  #if NA_DEBUG
    if(!stack)
      naCrash("stack is nullptr");
    if(!buf)
      naCrash("buf is nullptr");
  #endif
  NAByte* bufPtr = (NAByte*)buf;
  NAListIterator arrayIter = naMakeListAccessor(&stack->arrays);
  while(naIterateList(&arrayIter)) {
    size_t count = naEqualListIterator(&arrayIter, &stack->curArray) ?
      stack->curCount :
      na_GetStackArrayCount(&arrayIter);
    if(count) {
      count *= stack->typeSize;
      naCopyn(
        bufPtr,
        na_GetStackArrayFirstConst(&arrayIter),
        count);
      bufPtr += count;
    }
  }
  naClearListIterator(&arrayIter);
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
