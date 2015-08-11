
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAByteArray.h"
#include "NACoord.h"



NA_DEF NAByteArray* naInitByteArrayWithSize(NAByteArray* array, NAInt size){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitByteArrayWithSize", "array is Null-Pointer."); return NA_NULL;}
  #endif
  if(!size){  // if size is zero
    array = naInitByteArray(array);
  }else{
    array->lvalue = naMakeLValueWithSize(size);
    array->storage = naNewPointerWithLValue(array->lvalue, NA_TRUE);
    array->size = naAbsi(size);
  }
  return array;
}



NA_DEF NAByteArray* naInitByteArrayWithConstBuffer(NAByteArray* array, const void* buffer, NAInt size){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitByteArrayWithConstBuffer", "array is Null-Pointer."); return NA_NULL;}
    if(!buffer)
      naError("naInitByteArrayWithConstBuffer", "buffer is Null-Pointer.");
  #endif
  if(naIsIntZero(size)){
    array = naInitByteArray(array);
  }else{
    array->lvalue = naMakeLValueConst(buffer);
    #ifndef NDEBUG
      // Note that the accessiblesize is the same as the visible size because
      // the true amount of null-terminating bytes is unknown.
      if(size<0){
        naMarkLValueWithVisibleSize(&(array->lvalue), -size);
        naMarkLValueWithAccessibleSize(&(array->lvalue), -size, NA_TRUE);
      }else{
        naMarkLValueWithVisibleSize(&(array->lvalue), size);
        naMarkLValueWithAccessibleSize(&(array->lvalue), size, NA_FALSE);
      }
    #endif
    array->storage = naNewPointerWithLValue(array->lvalue, NA_FALSE);
    array->size = naAbsi(size);
  }
  return array;
}



NA_DEF NAByteArray* naInitByteArrayWithMutableBuffer(NAByteArray* array, void* buffer, NAInt size, NABool takeownership){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitByteArrayWithMutableBuffer", "array is Null-Pointer."); return NA_NULL;}
    if(!buffer)
      naError("naInitByteArrayWithMutableBuffer", "buffer is Null-Pointer.");
  #endif
  if(!size){  // if size is zero
    array = naInitByteArray(array);
    if(takeownership){free(buffer);}
  }else{
    array->lvalue = naMakeLValueMutable(buffer);
    #ifndef NDEBUG
      // Note that we mark the lvalue but not the NAPointer. Also note that
      // the accessiblesize is the same as the visible size because the true
      // amount of null-terminating bytes is unknown.
      if(size<0){
        naMarkLValueWithVisibleSize(&(array->lvalue), -size);
        naMarkLValueWithAccessibleSize(&(array->lvalue), -size, NA_TRUE);
      }else{
        naMarkLValueWithVisibleSize(&(array->lvalue), size);
        naMarkLValueWithAccessibleSize(&(array->lvalue), size, NA_FALSE);
      }
    #endif
    array->storage = naNewPointerWithLValue(array->lvalue, takeownership);
    array->size = naAbsi(size);
  }
  return array;
}



NA_DEF NAByteArray* naInitByteArrayExtraction(NAByteArray* dstarray, const NAByteArray* srcarray, NAInt offset, NAInt size){
  NALValue newlvalue;
  NAUInt positiveoffset;
  NAUInt positivesize;

  #ifndef NDEBUG
    if(!dstarray)
      {naCrash("naInitByteArrayExtraction", "dstarray is Null-Pointer."); return NA_NULL;}
    if(!srcarray){
      naCrash("naInitByteArrayExtraction", "srcarray is Null-Pointer."); return NA_NULL;}
  #endif

  // Note that dstarray may be equal to srcarray. If so, dstarray is assumed
  // to be a valid bytearray. Otherwise, dstarray is expected to be
  // uninitialized.

  naMakePositiveiInSize(&positiveoffset, &positivesize, offset, size, srcarray->size);
  newlvalue = naMakeLValueSub(&(srcarray->lvalue), positiveoffset, positivesize);
  
  if(!positivesize){
    // If the extraction results in an empty array...
    if(dstarray == srcarray){
      naClearByteArray(dstarray); // clear the old array.
    }
    dstarray = naInitByteArray(dstarray);
  }else{
    // The resulting array has content!
    if(dstarray != srcarray){
      dstarray->storage = naRetainPointer(srcarray->storage);
    }
    dstarray->size = positivesize;
    dstarray->lvalue = newlvalue;
  }
  
  return dstarray;
}



NA_DEF void naDecoupleByteArray(NAByteArray* array, NABool appendnulltermination){
  // Declaration before implementation. Needed for C90.
  NAInt arraysize;
  NAByte* buf;
  #ifndef NDEBUG
    if(!array){
      naCrash("naDecoupleByteArray", "array is Null-Pointer.");
      return;
    }
  #endif
  // Note: Do not use realloc as ptr may point to a subset of NAPointer.
  // Instead, create a new object and copy manually.
  arraysize = (NAInt)array->size;
  if(!arraysize){return;}
  if(appendnulltermination){arraysize = -arraysize;}
  buf = naMalloc(arraysize);
  naCopyn(buf, naGetLValueConst(&(array->lvalue)), naAbsi(arraysize));
  naClearByteArray(array);
  naInitByteArrayWithMutableBuffer(array, buf, arraysize, NA_TRUE);
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
