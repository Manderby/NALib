
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "NAArray.h"


NA_DEF NAArray* naCreateArrayWithCount(NAArray* array, NAUInt typesize, NAUInt count, NAConstructor constructor){
  #ifndef NDEBUG
    if(typesize < 1)
      naError("naCreateArrayWithCount", "typesize is < 1.");
  #endif
  if(!count){
    array = naInitializeEmptyArray(array);
  }else{
    array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
    naCreateByteArrayWithSize(&(array->bytearray), typesize * count);
  }
  array->typesize = typesize;

  if(constructor){
    // Note that you shall not call naForeachArray with the constructor. The
    // function pointers are not generally compatible.
    NAByte* ptr = naGetByteArrayMutablePointer(&(array->bytearray));
    while(count){
      constructor(ptr);
      ptr += array->typesize;
      count--;
    }
  }

  return array;
}



NA_DEF NAArray* naCreateArrayWithConstBuffer(NAArray* array, const void* buffer, NAUInt typesize, NAUInt count){
  #ifndef NDEBUG
    if(typesize < 1)
      naError("naCreateArrayWithConstBuffer", "typesize is < 1");
  #endif
  if(!count){
    array = naInitializeEmptyArray(array);
  }else{
    array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
    naCreateByteArrayWithConstBuffer(&(array->bytearray), buffer, typesize * count);
  }
  array->typesize = typesize;
  return array;
}



NA_DEF NAArray* naCreateArrayWithMutableBuffer(NAArray* array, void* buffer, NAUInt typesize, NAUInt count, NABool takeownership){
  #ifndef NDEBUG
    if(typesize < 1)
      naError("naCreateArrayWithMutableBuffer", "typesize is < 1");
  #endif
  // Note that here, in contrast to naCreateArrayWithConstBuffer, no test
  // is made if count is zero. With that, the takeownership parameter can
  // correctly be handeled and the buffer can be automatically free'd if count
  // is zero.
  array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArrayWithMutableBuffer(&(array->bytearray), buffer, typesize * count, takeownership);
  array->typesize = typesize;
  return array;
}



NA_DEF NAArray* naCreateArrayExtraction(NAArray* dstarray, const NAArray* srcarray, NAInt offset, NAInt count){
  NAUInt positiveoffset;
  NAUInt positivecount;
  
  dstarray = (NAArray*)naAllocateIfNull(dstarray, sizeof(NAArray));
  dstarray->typesize = srcarray->typesize;

  naMakePositiveiInSize(&positiveoffset, &positivecount, offset, count, naGetArrayCount(srcarray));

  naCreateByteArrayExtraction(&(dstarray->bytearray),
                            &(srcarray->bytearray),
                            positiveoffset * srcarray->typesize,
                            positivecount * srcarray->typesize);
  return dstarray;
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
