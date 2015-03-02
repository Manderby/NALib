
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAByteArray.h"
#include "NAMinMax.h"


NA_DEF NAByteArray* naCreateByteArrayWithSize(NAByteArray* array, NAInt size){
  NAInt fullsize;
  if(!size){  // if size is zero
    array = naCreateByteArray(array);
  }else{
    array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
    if(size>0){
      array->size = size;
      array->storage = naCreatePointerWithSize(NA_NULL, size);
      array->ptr.p = (NAByte*)naGetPointerMutableData(array->storage);
    }else{
      // When size is negative, a number of bytes is appended to the array which
      // will be initialized with binary zero but will not be visible to the
      // programmer. The following holds true:
      // - The additional bytes are all guaranteed to be initialized with binary
      //   zero.
      // - There are at least as many bytes appended as an address requires.
      //   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
      // - There are as many bytes allocated such that the total size is a
      //   multiple of an address size, meaning 4 or 8 Bytes depending on the
      //   system (32 Bit or 64 Bit).
      // - The total size (including the desired space) is at minimum 2 times
      //   the number of bytes needed for an address.
      // - The part without the additional bytes might partially become
      //   initialized with binary zero.
      array->size = -size;
      fullsize = array->size + 2 * NA_SYSTEM_ADDRESS_BYTES - (array->size % NA_SYSTEM_ADDRESS_BYTES);
      array->storage = naCreatePointerWithSize(NA_NULL, fullsize);
      array->ptr.p = (NAByte*)naGetPointerMutableData(array->storage);
      naNulln(&(array->ptr.p[fullsize - 2 * NA_SYSTEM_ADDRESS_BYTES]), 2 * NA_SYSTEM_ADDRESS_BYTES);
    }
  }
  
  return array;
}



NA_DEF NAByteArray* naCreateByteArrayWithConstBuffer( NAByteArray* array, const void* buffer, NAInt size){
  #ifndef NDEBUG
    if(size < 0)
      naError("naCreateByteArrayWithConstBuffer", "size is negative.");
    if(!buffer)
      naError("naCreateByteArrayWithConstBuffer", "buffer is Null-Pointer.");
  #endif
  if(!size){  // if size is zero
    array = naCreateByteArray(array);
  }else{
    array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
    array->storage = naCreatePointerWithConstBuffer(NA_NULL, buffer);
    array->ptr.constp = (const NAByte*)naGetPointerConstData(array->storage);
    array->size = size;
  }
  return array;
}



NA_DEF NAByteArray* naCreateByteArrayWithMutableBuffer(NAByteArray* array, void* buffer, NAInt size, NABool takeownership){
  #ifndef NDEBUG
    if(size < 0)
      naError("naCreateByteArrayWithBuffer", "size is negative.");
    if(!buffer)
      naError("naCreateByteArrayWithBuffer", "buffer is Null-Pointer.");
  #endif
  if(!size){  // if size is zero
    array = naCreateByteArray(array);
    if(takeownership){free(buffer);}
  }else{
    array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
    array->storage = naCreatePointerWithMutableBuffer(NA_NULL, buffer, takeownership);
    array->ptr.p = (NAByte*)naGetPointerMutableData(array->storage);
    array->size = size;
  }
  return array;
}



NA_DEF NAByteArray* naCreateByteArrayExtraction(NAByteArray* dstarray, const NAByteArray* srcarray, NAInt offset, NAInt size){
  NAByte* newptr;

  #ifndef NDEBUG
    if(!srcarray){
      naCrash("naCreateByteArrayExtraction", "Source is Null-Pointer.");
      return NA_NULL;
    }
  #endif

  dstarray = (NAByteArray*)naAllocateIfNull(dstarray, sizeof(NAByteArray));
  // Note that dstarray may be equal to srcarray.

  naMakePositiveiInSize(&offset, &size, naGetByteArraySize(srcarray));
  newptr = srcarray->ptr.p + offset;
  
  if(!size){
    // If the extraction results in an empty array...
    if(dstarray == srcarray){
      naClearByteArray(dstarray); // clear the old array.
      naCreateByteArray(dstarray);
    }else{
      naCreateByteArray(dstarray);
    }
  }else{
    // The resulting array has content!
    if(dstarray != srcarray){
      dstarray = naCreateByteArray(dstarray);
      dstarray->storage = naRetainPointer(srcarray->storage);
    }
    dstarray->ptr.p = newptr;
    dstarray->size = size;
  }
  
  return dstarray;
}



NA_DEF void naDecoupleByteArray(NAByteArray* array, NABool appendzerobytes){
  // Declaration before implementation. Needed for C90.
  NAInt arraysize;
  NAByteArray newarray;
  #ifndef NDEBUG
    if(!array){
      naCrash("naDecoupleByteArray", "array is Null-Pointer.");
      return;
    }
  #endif
  // Note: Do not use realloc as ptr may point to a subset of NAPointer.
  // Instead, create a new object and copy manually.
  arraysize = naGetByteArraySize(array);
  if(!arraysize){return;}
  if(appendzerobytes){
    naCreateByteArrayWithSize(&newarray, -arraysize);
  }else{
    naCreateByteArrayWithSize(&newarray, arraysize);
  }
  naCpyn(newarray.ptr.p, array->ptr.constp, arraysize);
  naClearByteArray(array);
  array->size = newarray.size;
  if(newarray.size > 0){
    array->ptr = newarray.ptr;
    array->storage = naRetainPointer(newarray.storage);
    naClearByteArray(&newarray);
  }
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
