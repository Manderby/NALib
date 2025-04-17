
// This file contains inline implementations of the file NAArray.h
// Do not include this file directly! It will automatically be included when
// including "NAArray.h"



#define NA_ARRAY_INVALID_INDEX NA_MAX_s

#include "../../NAUtility/NAMemory.h"



struct NAArray{
  NAPtr   ptr;         // The pointer storing the data
  size_t   count;       // The number of elements.
  size_t   typeSize;    // The size in bytes of the stored type
  NAMutator destructor;
  #if NA_DEBUG
    size_t    iterCount;   // The number of iterators attached to this array.
  #endif
};



NA_IDEF NAArray* naInitArray(NAArray* array) {
  #if NA_DEBUG
    if(!array)
      naCrash("array is nullptr");
  #endif
  array->ptr = naMakePtrWithDataConst(NA_NULL);
  // The typeSize is set to 1 such that calls to naGetArrayCount will not
  // result in bad values.
  array->count = 0;
  array->typeSize = 1;
  array->destructor = NA_NULL;
  #if NA_DEBUG
    array->iterCount = 0;
  #endif
  return array;
}



NA_IDEF NAArray* naInitArrayWithCount(NAArray* array, size_t typeSize, size_t count) {
  #if NA_DEBUG
    if(!array)
      naCrash("array is nullptr");
    if(typeSize == 0)
      naError("typeSize must not be zero.");
    if(count == 0)
      naError("count must not be zero.");
  #endif
  array->typeSize = typeSize;
  array->count = count;
  array->destructor = (NAMutator)naFree;
  array->ptr = naMakePtrWithDataMutable(naMalloc(count * typeSize));
  #if NA_DEBUG
    array->iterCount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataConst(NAArray* array, const void* data, size_t typeSize, size_t count) {
  #if NA_DEBUG
    if(!array)
      naCrash("array is nullptr");
    if(typeSize < 1)
      naError("typeSize should not be < 1");
  #endif
  array->typeSize = typeSize;
  array->count = count;
  array->destructor = NA_NULL;
  array->ptr = naMakePtrWithDataConst(data);
  #if NA_DEBUG
    array->iterCount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataMutable(NAArray* array, void* data, size_t typeSize, size_t count, NAMutator destructor) {
  #if NA_DEBUG
    if(!array)
      naCrash("array is nullptr");
    if(typeSize < 1)
      naError("typeSize should not be < 1");
  #endif
  array->typeSize = typeSize;
  array->count = count;
  array->destructor = destructor;
  array->ptr = naMakePtrWithDataMutable(data);
  #if NA_DEBUG
    array->iterCount = 0;
  #endif

  return array;
}



NA_IDEF void naClearArray(NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
    }
    if(array->iterCount)
      naError("There are still some iterators operating upon this array. Did you use naClearArrayIterator?");
  #endif
  naCleanupPtr(&array->ptr, array->destructor);
}



NA_IDEF void naForeachArrayConst(NAArray* array, NAAccessor accessor) {
  #if NA_DEBUG
    if(!accessor)
      naCrash("Accessor is nullptr");
  #endif
  size_t count = naGetArrayCount(array);
  const NAByte* ptr = (const NAByte*)naGetPtrConst(array->ptr);
  while(count) {
    accessor(ptr);
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF void naForeachArrayMutable(NAArray* array, NAMutator mutator) {
  #if NA_DEBUG
    if(!mutator)
      naCrash("Mutator is nullptr");
  #endif
  size_t count = naGetArrayCount(array);
  NAByte* ptr = (NAByte*)naGetPtrMutable(array->ptr);
  while(count) {
    mutator(ptr);
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF void naForeachArraypConst(NAArray* array, NAAccessor accessor) {
  #if NA_DEBUG
    if(!accessor)
      naCrash("Accessor is nullptr");
  #endif
  size_t count = naGetArrayCount(array);
  const NAByte* ptr = (const NAByte*)naGetPtrConst(array->ptr);
  while(count) {
    accessor(*((const void**)ptr));
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF void naForeachArraypMutable(NAArray* array, NAMutator mutator) {
  #if NA_DEBUG
    if(!mutator)
      naCrash("Mutator is nullptr");
  #endif
  size_t count = naGetArrayCount(array);
  NAByte* ptr = (NAByte*)naGetPtrMutable(array->ptr);
  while(count) {
    mutator(*((void**)ptr));
    ptr += array->typeSize;
    count--;
  }
}



NA_IDEF const void* naGetArrayPointerConst(const NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty, returned pointer is nullptr");
    }
  #endif
  return naGetPtrConst(array->ptr);
}



NA_IDEF void* naGetArrayPointerMutable(NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty, returned pointer is nullptr");
    }
  #endif
  return naGetPtrMutable(array->ptr);
}



NA_IDEF const void* naGetArrayElementConst(const NAArray* array, size_t index) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
    if(index >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return &((NAByte*)naGetPtrConst(array->ptr))[index * array->typeSize];
}



NA_IDEF void* naGetArrayElementMutable(NAArray* array, size_t index) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
    if(index >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return &((NAByte*)naGetPtrMutable(array->ptr))[index * array->typeSize];
}



NA_IDEF const void* naGetArrayElementpConst(const NAArray* array, size_t index) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
    if(index >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return *((const void**)&((NAByte*)naGetPtrConst(array->ptr))[index * array->typeSize]);
}



NA_IDEF void* naGetArrayElementpMutable(NAArray* array, size_t index) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("array is empty");
    }
    if(index >= naGetArrayCount(array))
      naError("array overflow.");
  #endif
  return *((void**)&((NAByte*)naGetPtrMutable(array->ptr))[index * array->typeSize]);
}



NA_IDEF size_t naGetArrayCount(const NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return 0;
    }
    if(array->count > NA_MAX_s)
      naError("Array count looks like it was negative when interpreted as signed integer. Do not create Arrays with negative sizes!");
  #endif
  // Note that an empty array has a typeSize of 1.
  return array->count;
}



NA_IDEF size_t naGetArrayMaxIndex(const NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return 0;
    }
    if(naIsArrayEmpty(array))
      naError("array is empty.");
  #endif
  return naMakeMaxWithEnds(naGetArrayCount(array));
}



NA_IDEF size_t naGetArrayTypeSize(const NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return 0;
    }
  #endif
  return array->typeSize;
}



NA_IDEF NABool naIsArrayEmpty(const NAArray* array) {
  #if NA_DEBUG
    if(!array) {
      naCrash("array is nullptr");
      return NA_TRUE;
    }
  #endif
  return (array->count == 0);
}






// /////////////////////////////////////////
// Array Iterator
// /////////////////////////////////////////


struct NAArrayIterator{
  NAPtr array;
  size_t index;
  #if NA_DEBUG
    NABool mutator;
  #endif
};



NA_IDEF NAArrayIterator naMakeArrayAccessor(const NAArray* array) {
  NAArrayIterator iter;
  #if NA_DEBUG
    NAArray* mutablearray;
    if(!array)
      naCrash("array is nullptr");
    mutablearray = (NAArray*)array;
    mutablearray->iterCount++;
    iter.mutator = NA_FALSE;
  #endif
  iter.array = naMakePtrWithDataConst(array);
  iter.index = NA_ARRAY_INVALID_INDEX;

  return iter;
}



NA_IDEF NAArrayIterator naMakeArrayMutator(NAArray* array) {
  NAArrayIterator iter;
  #if NA_DEBUG
    NAArray* mutablearray;
    if(!array)
      naCrash("array is nullptr");
    mutablearray = (NAArray*)array;
    mutablearray->iterCount++;
    iter.mutator = NA_TRUE;
  #endif
  iter.array = naMakePtrWithDataMutable(array);
  iter.index = NA_ARRAY_INVALID_INDEX;

  return iter;
}



NA_IDEF void naClearArrayIterator(NAArrayIterator* iterator) {
  #if NA_DEBUG
    NAArray* mutablearray = (NAArray*)naGetPtrConst(iterator->array);
    if(mutablearray->iterCount == 0)
      naError("Too many clears. Iterator count is already zero.");
    mutablearray->iterCount--;
  #else
    NA_UNUSED(iterator);
  #endif
}



NA_IDEF NABool naLocateArrayFirst(NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(naIsArrayEmpty((const NAArray*)naGetPtrConst(iterator->array)))
      naError("Array is empty");
  #endif
  iterator->index = 0;
  return NA_TRUE;
}



NA_IDEF NABool naLocateArrayLast(NAArrayIterator* iterator) {
  const NAArray* array = (const NAArray*)naGetPtrConst(iterator->array);
  #if NA_DEBUG
    if(naIsArrayEmpty(array))
      naError("Array is empty");
  #endif
  iterator->index = naGetArrayMaxIndex(array);
  return NA_TRUE;
}



NA_IDEF NABool naLocateArrayData(NAArrayIterator* iterator, const void* data) {
  const NAArray* array = (const NAArray*)naGetPtrConst(iterator->array);
  const NAByte* ptr = (const NAByte*)naGetArrayPointerConst(array);
  size_t count = naGetArrayCount(array);
  size_t index = 0;
  while(count) {
    if(ptr == data) {
      iterator->index = index;
      return NA_TRUE;
    }
    ptr += naGetArrayTypeSize(array);
    count--;
    index++;
  }
  return NA_FALSE;
}



NA_IDEF NABool naLocateArrayIndex(NAArrayIterator* iterator, size_t index) {
  const NAArray* array = (const NAArray*)naGetPtrConst(iterator->array);
  #if NA_DEBUG
    if(naIsArrayEmpty(array))
      naError("Array is empty");
  #endif
  iterator->index = index;
  if(index >= naGetArrayCount(array)) {
    iterator->index = NA_ARRAY_INVALID_INDEX;
    return NA_FALSE;
  }
  return NA_TRUE;
}



NA_IDEF NABool naIterateArray(NAArrayIterator* iterator, size_t step) {
  // todo: make IterateBack possible with a separate function
  iterator->index = (iterator->index == NA_ARRAY_INVALID_INDEX) ? step - 1 : iterator->index + step;
  if(iterator->index == naGetArrayCount((const NAArray*)naGetPtrConst(iterator->array))) {
    iterator->index = NA_ARRAY_INVALID_INDEX;
  }
#if NA_DEBUG
  //    if(iterator->index < NA_ARRAY_INVALID_INDEX)
  //      naError("Iterator underflows");
  //    if(iterator->index >= naGetArrayCount((const NAArray*)naGetPtrConst(iterator->array)))
  //      naError("Iterator overflows");
#endif
  return (iterator->index != NA_ARRAY_INVALID_INDEX);
}



NA_IDEF NABool naIterateArrayBack(NAArrayIterator* iterator, size_t step) {
  // todo: make IterateBack possible with a separate function
  size_t arrayCount = naGetArrayCount((const NAArray*)naGetPtrConst(iterator->array));
  if(naIsArrayAtInitial(iterator)) {
    iterator->index += arrayCount;
  }
  if(iterator->index < step) {
    iterator->index = NA_ARRAY_INVALID_INDEX;
  }else{
    iterator->index -= step;
    if(iterator->index == arrayCount) {
      iterator->index = NA_ARRAY_INVALID_INDEX;
    }
  }
#if NA_DEBUG
  //    if(iterator->index < NA_ARRAY_INVALID_INDEX)
  //      naError("Iterator underflows");
  //    if(iterator->index >= arrayCount)
  //      naError("Iterator overflows");
#endif
  return (iterator->index != NA_ARRAY_INVALID_INDEX);
}



NA_IDEF const void* naGetArrayPrevConst(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_NULL;
    }
  #endif
  const NAArray* array = (const NAArray*)naGetPtrConst(iterator->array);
  if(iterator->index == 0) {
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)) {
    return naGetArrayElementConst(array, array->count - 1);
  }else{
    return naGetArrayElementConst(array, iterator->index - 1);
  }
}



NA_IDEF void* naGetArrayPrevMutable(NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_NULL;
    }
  #endif
  NAArray* array = (NAArray*)naGetPtrMutable(iterator->array);
  if(iterator->index == 0) {
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)) {
    return naGetArrayElementMutable(array, array->count - 1);
  }else{
    return naGetArrayElementMutable(array, iterator->index - 1);
  }
}



NA_IDEF const void* naGetArrayCurConst(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)) {
    return NA_NULL;
  }else{
    return naGetArrayElementConst((const NAArray*)naGetPtrConst(iterator->array), iterator->index);
  }
}



NA_IDEF void* naGetArrayCurMutable(NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)) {
    return NA_NULL;
  }else{
    return naGetArrayElementMutable((NAArray*)naGetPtrMutable(iterator->array), iterator->index);
  }
}



NA_IDEF const void* naGetArrayNextConst(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_NULL;
    }
  #endif
  const NAArray* array = (const NAArray*)naGetPtrConst(iterator->array);
  if(iterator->index == naGetArrayMaxIndex(array)) {
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)) {
    return naGetArrayElementConst(array, 0);
  }else{
    return naGetArrayElementConst(array, iterator->index + 1);
  }
}



NA_IDEF void* naGetArrayNextMutable(NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_NULL;
    }
  #endif
  NAArray* array = (NAArray*)naGetPtrConst(iterator->array);
  if(iterator->index == naGetArrayMaxIndex(array)) {
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)) {
    return naGetArrayElementMutable(array, 0);
  }else{
    return naGetArrayElementMutable(array, iterator->index + 1);
  }
}



NA_IDEF NABool naIsArrayAtFirst(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == 0);
}



NA_IDEF NABool naIsArrayAtLast(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == naGetArrayMaxIndex((const NAArray*)naGetPtrConst(iterator->array)));
}



NA_IDEF NABool naIsArrayAtIndex(const NAArrayIterator* iterator, size_t index) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == index);
}



NA_IDEF NABool naIsArrayAtInitial(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
      return NA_FALSE;
    }
  #endif
  return (iterator->index == NA_ARRAY_INVALID_INDEX);
}



NA_IDEF size_t naGetArrayCurIndex(const NAArrayIterator* iterator) {
  #if NA_DEBUG
    if(!iterator) {
      naCrash("iterator is nullptr");
    }
    if(naIsArrayAtInitial(iterator))
      naError("iterator is at initial position.");
  #endif
  return iterator->index;
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
