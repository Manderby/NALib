
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "../NAArray.h"


NA_DEF NAArray* naInitArrayWithCount(NAArray* array, NAUInt typesize, NAUInt count){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArrayWithCount", "array is Null-Pointer"); return NA_NULL;}
    if(typesize < 1)
      naError("naInitArrayWithCount", "typesize is < 1.");
  #endif
  array->typesize = typesize;
  if(!count){
    naInitByteArray(&(array->bytearray));
  }else{
    naInitByteArrayWithBytesize(&(array->bytearray), typesize * count);
  }
  
  return array;
}



NA_DEF NAArray* naInitArrayWithConstBuffer(NAArray* array, const void* buffer, NAUInt typesize, NAUInt count){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArrayWithConstBuffer", "array is Null-Pointer"); return NA_NULL;}
    if(typesize < 1)
      naError("naInitArrayWithConstBuffer", "typesize is < 1");
  #endif
  array->typesize = typesize;
  if(!count){
    naInitByteArray(&(array->bytearray));
  }else{
    naInitByteArrayWithConstBuffer(&(array->bytearray), buffer, typesize * count);
  }
  return array;
}



NA_DEF NAArray* naInitArrayWithMutableBuffer(NAArray* array, void* buffer, NAUInt typesize, NAUInt count, NAPointerCleanup ownership){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArrayWithMutableBuffer", "array is Null-Pointer"); return NA_NULL;}
    if(typesize < 1)
      naError("naInitArrayWithMutableBuffer", "typesize is < 1");
  #endif
  array->typesize = typesize;
  // Note that here, in contrast to naInitArrayWithConstBuffer, no test
  // is made if count is zero. With that, the takeownership parameter can
  // correctly be handeled and the buffer can be automatically free'd if count
  // is zero.
  naInitByteArray(&(array->bytearray));
  naInitByteArrayWithMutableBuffer(&(array->bytearray), buffer, typesize * count, ownership);
  return array;
}



NA_DEF NAArray* naInitArrayExtraction(NAArray* dstarray, const NAArray* srcarray, NAInt offset, NAInt count){
  NAUInt positiveoffset;
  NAUInt positivecount;
  #ifndef NDEBUG
    if(!dstarray)
      {naCrash("naInitArrayExtraction", "dstarray is Null-Pointer"); return NA_NULL;}
  #endif
  
  dstarray->typesize = srcarray->typesize;
  naInitByteArray(&(dstarray->bytearray));

  naMakeIntegerRangePositiveInLength(&positiveoffset, &positivecount, offset, count, naGetArrayCount(srcarray));

  naInitByteArrayExtraction(&(dstarray->bytearray),
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
