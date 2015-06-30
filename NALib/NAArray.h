
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_ARRAY_INCLUDED
#define NA_ARRAY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// /////////////////////////////////////
// NAArray
// /////////////////////////////////////

// Following is a very general implementation of an array, storing one type
// of data. Note that this implementation may be marginally slower than the
// default C-like array and uses quite a bit of space to store all metadata.
// It has the big advantage though of knowing how many elements are stored
// and hence being able to check for under- and overflows as well as being
// able to have arbitrary sub-arrays without copying the contents.
//
// This array implementaiton is more for complex datastructure building. If
// you are looking for a simple array of integers for example, better use the
// traditional [] arrays.
//
// Important: You have to typecast the returned element-pointers!


#include "NASystem.h"


typedef struct NAArray NAArray;


// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Creates an NAArray struct denoting an EMPTY array.
NA_IAPI NAArray* naCreateArray(NAArray* array);

// Creates or fills a new NAArray struct with a given typesize in bytes and a
// count. Calls the constructor on every element if not NA_NULL.
NA_API  NAArray* naCreateArrayWithCount  (NAArray* array,
                                            NAUInt typesize,
                                            NAUInt count,
                                     NAConstructor constructor);

// Creates or fills a new NAArray struct which contains the data of the given
// buffer WITHOUT copying. The count denotes the number of elements in the
// array, not bytes! The programmer is responsible for that count does not
// overflows the buffer.
// When takeownership is set to NA_TRUE, the array will deallocate the given
// buffer with free() when it is no longer used. Note that count can not be
// negative. If count is null, an empty array is created and the buffer will be
// deleted immediately, if takeownership is true. You can not take ownership of
// const buffers.
//
// Use these functions to encapsulate your own raw buffers into an NAArray.
// There are two creation functions, one for const data and one for non-const.
//
// Note that NALib does provide an API for calling constructors on preallocated
// buffers. If you really need to do that, you have to do this manually.
NA_API  NAArray* naCreateArrayWithConstBuffer(
                                          NAArray* array,
                                       const void* buffer,
                                            NAUInt typesize,
                                            NAUInt count);
NA_API  NAArray* naCreateArrayWithMutableBuffer(
                                          NAArray* array,
                                             void* buffer,
                                            NAUInt typesize,
                                            NAUInt count,
                                            NABool takeownership);

// Fills dstarray with a desired part of srcarray.
// offset and count can be negative: See naInitByteArrayExtraction for the
// explanation of the arguments.
NA_API  NAArray* naCreateArrayExtraction( NAArray* dstarray,
                                    const NAArray* srcarray,
                                             NAInt offset,
                                             NAInt count);

// Clears or destroys the given array. Calls the destructor on every element
// if not NA_NULL. See Readme file for more information.
NA_IAPI void naClearArray  (NAArray* array, NADestructor destructor);
NA_IAPI void naDestroyArray(NAArray* array, NADestructor destructor);


// COPIES the contents of the array to a separate storage and decouples it
// from the existing storage. See naDecoupleByteArray for more info. No
// additional bytes are appended. COPIES ALWAYS!
NA_IAPI void naDecoupleArray(NAArray* array);

// Traverses the whole array and calls the mutator on each element. A pointer
// to each element will be given to the mutator.
NA_IAPI void naForeachArray(NAArray* array, NAMutator mutator);

// Returns a pointer to the very first element of the raw data array. Warning:
// result is garbage if the array is empty. Notice: This function is speedy.
NA_IAPI const void* naGetArrayConstPointer  (const NAArray* array);
NA_IAPI       void* naGetArrayMutablePointer(      NAArray* array);

// Returns a POINTER to the element at the given index. If indx is negative, it
// is searched from the end of the array. For example -1 denotes the last
// element.
// Warning: result is garbage if the array is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetArrayPointer or this
// function and use pointer arithmetic afterwards.
// Returns the pointer to the desired element. Note that if the array stores
// pointers, this function will return a pointer to a pointer.
NA_IAPI const void* naGetArrayConstElement  (const NAArray* array, NAInt indx);
NA_IAPI       void* naGetArrayMutableElement(      NAArray* array, NAInt indx);

// Returns the number of elements in this array. Beware: This is different
// from naGetByteArraySize. The size returned by naGetByteArraySize is a size
// in bytes, not necessary the element count.
// Also note that this function requires a (costly) division operation. When
// using it a lot, for example in a loop, it might be a good idea to store the
// count in a variable.
NA_IAPI NAUInt naGetArrayCount(const NAArray* array);

// Returns the number of bytes needed to store one element.
NA_IAPI NAUInt naGetArrayTypeSize(const NAArray* array);

// Returns true if the array is empty.
NA_IAPI NABool naIsArrayEmpty(const NAArray* array);

















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAByteArray.h"
#include "NAMinMax.h"


struct NAArray{
  struct NAByteArray  bytearray;  // The byte array
  NAUInt              typesize;   // The size in bytes of the stored type
};



// This is a small helper function used for the naCreate functions. Note that
// naInitByteArray is defined inline whereas other creation functions of
// NAByteArray are not.
NA_HIDEF NAArray* naInitializeEmptyArray(NAArray* array){
  array = naAllocNALibStruct(array, NAArray);
  naInitByteArray(&(array->bytearray));
  return array;
}



NA_IDEF NAArray* naCreateArray(NAArray* array){
  array = naInitializeEmptyArray(array);
  // The typesize is set to 1 such that calls to naGetArraySize will not
  // result in bad values.
  array->typesize = 1;
  return array;
}



NA_IDEF void naClearArray(NAArray* array, NADestructor destructor){
  #ifndef NDEBUG
    if(!array){
      naCrash("naClearArray", "array is Null-Pointer.");
      return;
    }
    // Note that NALib does not check, if you initially created the array
    // with a constructor callback. You are responsible to pair constructors
    // and destructors appropriately.
  #endif
  if(destructor){naForeachArray(array, destructor);}
  naClearByteArray(&(array->bytearray));
}



NA_IDEF void naDestroyArray(NAArray* array, NADestructor destructor){
  naClearArray(array, destructor);
  free(array);
}



NA_IDEF void naDecoupleArray(NAArray* array){
  naDecoupleByteArray(&(array->bytearray), NA_FALSE);
}



NA_IDEF void naForeachArray(NAArray* array, NAMutator mutator){
  // Declaration before implementation. Needed for C90
  NAUInt count;
  NAByte* ptr;
  #ifndef NDEBUG
    if(!mutator)
      naError("naForeachArray", "Mutator is Null");
  #endif
  count = naGetArrayCount(array);
  ptr = naGetByteArrayMutablePointer(&(array->bytearray));
  while(count){
    mutator(ptr);
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF const void* naGetArrayConstPointer(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayConstPointer", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayConstPointer", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetByteArrayConstPointer(&(array->bytearray));
}



NA_IDEF void* naGetArrayMutablePointer(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMutablePointer", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayMutablePointer", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetByteArrayMutablePointer(&(array->bytearray));
}



NA_IDEF const void* naGetArrayConstElement(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayConstElement", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayConstElement", "array is empty");
    }
  #endif
  return naGetByteArrayConstByte(&(array->bytearray), indx * array->typesize);
}



NA_IDEF void* naGetArrayMutableElement(NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMutableElement", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayMutableElement", "array is empty");
    }
  #endif
  return naGetByteArrayMutableByte(&(array->bytearray), indx * array->typesize);
}



NA_IDEF NAUInt naGetArrayCount(const NAArray* array){
  NAInt bytesize;
  
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCount", "array is Null-Pointer.");
      return 0;
    }
  #endif
  bytesize = naGetByteArraySize(&(array->bytearray));
  #ifndef NDEBUG
    if(bytesize < 0)
      naError("naGetArrayCount", "ByteArray count was negative. Do not create Arrays with negative sizes!");
  #endif
  // Note that an empty array has a typesize of 1.
  return bytesize / array->typesize;
}



NA_IDEF NAUInt naGetArrayTypeSize(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayTypeSize", "array is Null-Pointer.");
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
  return naIsByteArrayEmpty(&(array->bytearray));
}



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_ARRAY_INCLUDED



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
