
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAArray.h
// Do not include this file directly! It will automatically be included when
// including "NAArray.h"


#include "NACoord.h"


struct NAArray{
  NASmartPtr sptr;        // The pointer storing the data
  NAUInt     count;       // The number of elements
  NAUInt     typesize;    // The size in bytes of the stored type
  #ifndef NDEBUG
    NAInt    itercount;   // The number of iterators attached to this array.
  #endif
};



NA_IDEF NAArray* naInitArray(NAArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArray", "array is Null-Pointer"); return NA_NULL;}
  #endif
  naInitSmartPtrConst(&(array->sptr), NA_MEMORY_CLEANUP_NONE, NA_NULL);
  // The typesize is set to 1 such that calls to naGetArrayCount will not
  // result in bad values.
  array->count = 0;
  array->typesize = 1;
  #ifndef NDEBUG
    array->itercount = 0;
  #endif
  return array;
}



NA_IDEF NAArray* naInitArrayWithCount(NAArray* array, NAUInt typesize, NAUInt count){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArrayWithCount", "array is Null-Pointer"); return NA_NULL;}
    if(typesize < 1)
      naError("naInitArrayWithCount", "typesize is < 1.");
    if((NAInt)count < 0)
      naError("naInitArrayWithCount", "count looks like it was signed and < 0.");
  #endif
  array->typesize = typesize;
  array->count = count;
  naInitSmartPtrMutable(&(array->sptr), NA_MEMORY_CLEANUP_NONE, naMalloc(count * typesize), NA_MEMORY_CLEANUP_NA_FREE);
  #ifndef NDEBUG
    array->itercount = 0;
  #endif
  
  return array;
}



NA_IDEF NAArray* naInitArrayWithDataConst(NAArray* array, const void* data, NAUInt typesize, NAUInt count){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArrayWithDataConst", "array is Null-Pointer"); return NA_NULL;}
    if(typesize < 1)
      naError("naInitArrayWithDataConst", "typesize is < 1");
  #endif
  array->typesize = typesize;
  array->count = count;
  naInitSmartPtrConst(&(array->sptr), NA_MEMORY_CLEANUP_NONE, data);
  #ifndef NDEBUG
    array->itercount = 0;
  #endif

  return array;
}



NA_IDEF NAArray* naInitArrayWithDataMutable(NAArray* array, void* data, NAUInt typesize, NAUInt count, NAMemoryCleanup cleanup){
  #ifndef NDEBUG
    if(!naIsCleanupValid(cleanup))
      naError("naNewStringWithMutableUTF8Buffer", "invalid cleanup option");
    if(!array)
      {naCrash("naInitArrayWithDataMutable", "array is Null-Pointer"); return NA_NULL;}
    if(typesize < 1)
      naError("naInitArrayWithDataMutable", "typesize is < 1");
  #endif
  array->typesize = typesize;
  array->count = count;
  naInitSmartPtrMutable(&(array->sptr), NA_MEMORY_CLEANUP_NONE, data, cleanup);
  #ifndef NDEBUG
    array->itercount = 0;
  #endif

  return array;
}



NA_IDEF void naClearArray(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naClearArray", "array is Null-Pointer.");
      return;
    }
    if(array->itercount)
      naError("naClearArray", "There are still some iterators operating upon this array. Did you use naClearArrayIterator?");
  #endif
  naReleaseSmartPtr(&(array->sptr), NA_NULL);
}



NA_IDEF void naForeachArrayConst(NAArray* array, NAAccessor accessor){
  // Declaration before implementation. Needed for C90
  NAUInt count;
  const NAByte* ptr;
  #ifndef NDEBUG
    if(!accessor)
      {naCrash("naForeachArray", "Accessor is Null"); return;}
  #endif
  count = naGetArrayCount(array);
  ptr = naGetSmartPtrConst(&(array->sptr));
  while(count){
    accessor(ptr);
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF void naForeachArrayMutable(NAArray* array, NAMutator mutator){
  // Declaration before implementation. Needed for C90
  NAUInt count;
  NAByte* ptr;
  #ifndef NDEBUG
    if(!mutator)
      {naCrash("naForeachArray", "Mutator is Null"); return;}
  #endif
  count = naGetArrayCount(array);
  ptr = naGetSmartPtrMutable(&(array->sptr));
  while(count){
    mutator(ptr);
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF const void* naGetArrayPointerConst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayPointerConst", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("naGetArrayPointerConst", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetSmartPtrConst(&(array->sptr));
}



NA_IDEF void* naGetArrayPointerMutable(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayPointerMutable", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("naGetArrayPointerMutable", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetSmartPtrMutable(&(array->sptr));
}



NA_IDEF const void* naGetArrayElementConst(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayElementConst", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("naGetArrayElementConst", "array is empty");
    }
  #endif
  indx = naMakeIndexPositive(indx, naGetArrayCount(array));
  return ((NAByte*)naGetSmartPtrConst(&(array->sptr)))[indx * array->typesize];
}



NA_IDEF void* naGetArrayElementMutable(NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayElementMutable", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsArrayEmpty(array))
        naError("naGetArrayElementMutable", "array is empty");
    }
  #endif
  indx = naMakeIndexPositive(indx, naGetArrayCount(array));
  return ((NAByte*)naGetSmartPtrMutable(&(array->sptr)))[indx * array->typesize];
}



NA_IDEF NAUInt naGetArrayCount(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCount", "array is Null-Pointer.");
      return 0;
    }
    if((NAInt)(array->count) < 0)
      naError("naGetArrayCount", "Array count looks like it was negative when interpreted as signed integer. Do not create Arrays with negative sizes!");
  #endif
  // Note that an empty array has a typesize of 1.
  return array->count;
}



NA_IDEF NAUInt naGetArrayMaxIndex(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMaxIndex", "array is Null-Pointer.");
      return 0;
    }
    if(naIsArrayEmpty(array))
      naError("naGetArrayMaxIndex", "array is empty.");
  #endif
  return naMakeMaxWithEndi(naGetArrayCount(array));
}



NA_IDEF NAUInt naGetArrayTypesize(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayTypesize", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return array->typesize;
}



NA_IDEF NABool naIsArrayEmpty(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayEmpty", "array is Null-Pointer.");
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
  NAInt indx;
  #ifndef NDEBUG
    NABool mutator;
  #endif
};



NA_IDEF NAArrayIterator naMakeArrayIteratorAccessor(const NAArray* array){
  NAArrayIterator iter;
  #ifndef NDEBUG
    if(!array)
      naError("naMakeArrayIteratorAccessor", "array is Null pointer");
    NAArray* mutablearray = (NAArray*)array;
    mutablearray->itercount++;
    iter.mutator = NA_FALSE;
  #endif
  iter.array = naMakePtrWithDataConst(array);
  iter.indx = -1;
  
  return iter;
}



NA_IDEF NAArrayIterator naMakeArrayIteratorMutator(NAArray* array){
  NAArrayIterator iter;
  #ifndef NDEBUG
    if(!array)
      naError("naMakeArrayIteratorAccessor", "array is Null pointer");
    NAArray* mutablearray = (NAArray*)array;
    mutablearray->itercount++;
    iter.mutator = NA_TRUE;
  #endif
  iter.array = naMakePtrWithDataMutable(array, NA_MEMORY_CLEANUP_NONE);
  iter.indx = -1;
  
  return iter;
}



NA_IDEF void naClearArrayIterator(NAArrayIterator* iterator){
  #ifndef NDEBUG
    NAArray* mutablearray = (NAArray*)naGetPtrConst(&(iterator->array));
    if(mutablearray->itercount == 0)
      naError("naClearArrayIterator", "Too many clears. Iteratorcout is already zero.");
    mutablearray->itercount--;
  #endif
}



NA_IDEF NABool naLocateArrayFirst(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(&(iterator->array))))
      naError("naLocateArrayFirst", "Array is empty");
  #endif
  iterator->indx = 0;
}



NA_IDEF NABool naLocateArrayLast(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(&(iterator->array))))
      naError("naLocateArrayLast", "Array is empty");
  #endif
  iterator->indx = naGetArrayMaxIndex(naGetPtrConst(&(iterator->array)));
}



NA_IDEF NABool naLocateArrayContent(NAArrayIterator* iterator, const void* content){
  const void* ptr = naGetArrayPointerConst(naGetPtrConst(&(iterator->array)));
  NAInt count = naGetArrayCount(naGetPtrConst(&(iterator->array)));
  NAInt indx = 0;
  while(count){
    if(ptr == content){
      iterator->indx = indx;
      return NA_TRUE;
    }
    ptr += naGetArrayTypesize(naGetPtrConst(&(iterator->array)));
    count--;
    indx++;
  }
  return NA_FALSE;
}



NA_IDEF NABool naLocateArrayIndex(NAArrayIterator* iterator, NAInt indx){
  #ifndef NDEBUG
    if(naIsArrayEmpty(naGetPtrConst(&(iterator->array))))
      naError("naLocateArrayIndex", "Array is empty");
  #endif
  indx = naMakeIndexPositive(indx, naGetArrayCount(naGetPtrConst(&(iterator->array))));
  iterator->indx = indx;
  if(indx < 0){
    iterator->indx = -1;
    return NA_FALSE;
  }else if(indx >= naGetArrayCount(naGetPtrConst(&(iterator->array)))){
    iterator->indx = -1;
    return NA_FALSE;
  }
  return NA_TRUE;
}



NA_IDEF NABool naIterateArray(NAArrayIterator* iterator, NAInt step){
  if(naIsArrayAtInitial(iterator) && step < 0){iterator->indx += naGetArrayCount(naGetPtrConst(&(iterator->array)));}
  iterator->indx += step;
  if(iterator->indx == naGetArrayCount(naGetPtrConst(&(iterator->array)))){iterator->indx = -1;}
  #ifndef NDEBUG
    if(iterator->indx < -1)
      naError("naIterateArray", "Iterator step underflows");
    if(iterator->indx >= naGetArrayCount(naGetPtrConst(&(iterator->array))))
      naError("naIterateArray", "Iterator step overflows");
  #endif
  return (iterator->indx != -1);
}



NA_IDEF const void* naGetArrayPrevConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayPrevConst", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == 0){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementConst(naGetPtrConst(&(iterator->array)), -1);
  }else{
    return naGetArrayElementConst(naGetPtrConst(&(iterator->array)), iterator->indx - 1);
  }
}



NA_IDEF void* naGetArrayPrevMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayPrevMutable", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == 0){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementMutable(naGetPtrMutable(&(iterator->array)), -1);
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(&(iterator->array)), iterator->indx - 1);
  }
}



NA_IDEF const void* naGetArrayCurrentConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayCurrentConst", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)){
    return NA_NULL;
  }else{
    return naGetArrayElementConst(naGetPtrConst(&(iterator->array)), iterator->indx);
  }
}



NA_IDEF void* naGetArrayCurrentMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayCurrentMutable", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsArrayAtInitial(iterator)){
    return NA_NULL;
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(&(iterator->array)), iterator->indx);
  }
}



NA_IDEF const void* naGetArrayNextConst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayPrevConst", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == naGetArrayMaxIndex(naGetPtrConst(&(iterator->array)))){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementConst(naGetPtrConst(&(iterator->array)), 0);
  }else{
    return naGetArrayElementConst(naGetPtrConst(&(iterator->array)), iterator->indx + 1);
  }
}



NA_IDEF void* naGetArrayNextMutable(NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayNextMutable", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(iterator->indx == naGetArrayMaxIndex(naGetPtrConst(&(iterator->array)))){
    return NA_NULL;
  }else if(naIsArrayAtInitial(iterator)){
    return naGetArrayElementMutable(naGetPtrMutable(&(iterator->array)), 0);
  }else{
    return naGetArrayElementMutable(naGetPtrMutable(&(iterator->array)), iterator->indx + 1);
  }
}



NA_IDEF NABool naIsArrayAtFirst(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naIsArratAtIndex", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return (iterator->indx == 0);
}



NA_IDEF NABool naIsArrayAtLast(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naIsArratAtIndex", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return (iterator->indx == naGetArrayMaxIndex(naGetPtrConst(&(iterator->array))));
}



NA_IDEF NABool naIsArrayAtIndex(const NAArrayIterator* iterator, NAInt indx){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naIsArratAtIndex", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  indx = naMakeIndexPositive(indx, naGetArrayCount(naGetPtrConst(&(iterator->array))));
  return (iterator->indx == indx);
}



NA_IDEF NABool naIsArrayAtInitial(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naIsArrayAtInitial", "iterator is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return (iterator->indx == -1);
}



NA_IDEF NAUInt naGetArrayCurrentIndex(const NAArrayIterator* iterator){
  #ifndef NDEBUG
    if(!iterator){
      naCrash("naGetArrayCurrentIndex", "iterator is Null-Pointer.");
      return NA_INVALID_MEMORY_INDEX;
    }
    if(naIsArrayAtInitial(iterator))
      naError("naGetArrayCurrentIndex", "iterator is at initial position.");
  #endif
  return iterator->indx;
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
