
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


#include "NAMinMax.h"



// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Creates an EMPTY array.
NA_IAPI NAArray* naCreateArray(NAArray* array);

// Creates or fills a new array with a given typesize in bytes and a count.
NA_IAPI NAArray* naCreateArrayWithCount  (NAArray* array,
                                             NAInt typesize,
                                             NAInt count);

// Creates or fills a new Array which contains the data of the given buffer
// WITHOUT copying. The count denotes the number of elements in the array, not
// bytes! The programmer is responsible for that count does not overflows the
// buffer.
// When takeownership is set to NA_TRUE, the array will deallocate the given
// buffer with free() when it is no longer used. Note that count can not be
// negative. If count is null, an empty array is created and the buffer will be
// deleted immediately, if takeownership is true. You can not take ownership of
// const buffers.
//
// Use these functions to encapsulate your own raw buffers into an NAArray!
// There are two creation functions, one for const data and one for non-const.
NA_IAPI NAArray* naCreateArrayWithConstBuffer(
                                          NAArray* array,
                                       const void* buffer,
                                             NAInt typesize,
                                             NAInt count);
NA_IAPI NAArray* naCreateArrayWithMutableBuffer(
                                          NAArray* array,
                                             void* buffer,
                                             NAInt typesize,
                                             NAInt count,
                                            NABool takeownership);

// Fills dstarray with a desired part of srcarray.
// See naCreateByteArrayExtraction for the explanation of all arguments.
NA_IAPI NAArray* naCreateArrayExtraction( NAArray* dstarray,
                                    const NAArray* srcarray,
                                             NAInt offset,
                                             NAInt count);


// Clears or destroys the given array. Calls the destructor on every element.
// See Readme file for more information.
NA_IAPI void naClearArray  (NAArray* array, NADestructor destructor);
NA_IAPI void naDestroyArray(NAArray* array, NADestructor destructor);


// COPIES the contents of the array to a separate storage and decouples it
// from the existing storage. See naDecoupleByteArray for more info. No
// additional bytes are appended. COPIES ALWAYS!
NA_IAPI void naDecoupleArray(NAArray* array);

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
NA_IAPI void*       naGetArrayMutableElement(      NAArray* array, NAInt indx);

// Returns the number of elements in this array. Beware: This is different
// from naGetByteArraySize. The size returned by naGetByteArraySize is a size
// in bytes, not necessary the element count.
// Also note that this function requires a (costly) division operation. When
// using it a lot, for example in a loop, it might be a good idea to store the
// count in a variable.
NA_IAPI NAInt naGetArrayCount(const NAArray* array);

// Returns the number of bytes needed to store one element.
NA_IAPI NAInt naGetArrayTypeSize(const NAArray* array);

// Returns true if the array is empty.
NA_IAPI NABool naIsArrayEmpty(const NAArray* array);

















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAByteArray.h"


struct NAArray{
  struct NAByteArray bytearray;  // The byte array
  NAInt              typesize;   // The size in bytes of the stored type
};
// Note that an empty array is marked with 0 in the the size field of
// bytearray. If that field is zero, all other fields are garbage.


NA_IDEF NAArray* naCreateArray(NAArray* array){
  array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArray(&(array->bytearray));
  array->typesize = 1;
  return array;
}


NA_IDEF NAArray* naCreateArrayWithCount(NAArray* array,
                                           NAInt typesize,
                                           NAInt count){
  #ifndef NDEBUG
    if(typesize < 0)
      naError("naCreateArrayWithCount", "typesize is < 1.");
    if(count < 0)
      naError("naCreateArrayWithCount", "count is < 0.");
  #endif
  if(!count){return naCreateArray(array);}
  array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArrayWithSize(&(array->bytearray), typesize * count);
  // Note that if count was 0, the typesize is irrelevant.
  array->typesize = typesize;
  return array;
}


NA_IDEF NAArray* naCreateArrayWithConstBuffer(
                                          NAArray* array,
                                       const void* buffer,
                                             NAInt typesize,
                                             NAInt count){
  #ifndef NDEBUG
    if(typesize <= 0)
      naError("naCreateArrayWithConstBuffer", "typesize is smallerequal zero");
  #endif
  array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArrayWithConstBuffer(&(array->bytearray), buffer, typesize * count);
  array->typesize = typesize;
  return array;
}


NA_IDEF NAArray* naCreateArrayWithMutableBuffer(
                                          NAArray* array,
                                             void* buffer,
                                             NAInt typesize,
                                             NAInt count,
                                            NABool takeownership){
  #ifndef NDEBUG
    if(typesize <= 0)
      naError("naCreateArrayWithConstBuffer", "typesize is smallerequal zero");
  #endif
  array = (NAArray*)naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArrayWithMutableBuffer(&(array->bytearray), buffer, typesize * count, takeownership);
  array->typesize = typesize;
  return array;
}


NA_IDEF NAArray* naCreateArrayExtraction(   NAArray* dstarray,
                                      const NAArray* srcarray,
                                               NAInt offset,
                                               NAInt count){
  dstarray = (NAArray*)naAllocateIfNull(dstarray, sizeof(NAArray));
  dstarray->typesize = srcarray->typesize;

  naMakePositiveiInSize(&offset, &count, naGetArrayCount(srcarray));

  naCreateByteArrayExtraction(&(dstarray->bytearray),
                            &(srcarray->bytearray),
                            offset * srcarray->typesize,
                            count * srcarray->typesize);
  return dstarray;
}


NA_IDEF void naClearArray(NAArray* array, NADestructor destructor){
  #ifndef NDEBUG
    if(!array){
      naCrash("naClearArray", "array is Null-Pointer.");
    }
  #endif

  if(destructor){
    NAInt count = naGetArrayCount(array);
    NAByte* ptr = naGetByteArrayMutablePointer(&(array->bytearray));
    NAInt i;
    for(i=0; i<count; i++){
      destructor(ptr);
      ptr += array->typesize;
    }
  }

  naClearByteArray(&(array->bytearray));
}


NA_IDEF void naDestroyArray(NAArray* array, NADestructor destructor){
  naClearArray(array, destructor);
  free(array);
}


NA_IAPI void naDecoupleArray(NAArray* array){
  naDecoupleByteArray(&(array->bytearray), NA_FALSE);
}


NA_IAPI const void* naGetArrayConstPointer(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayConstPointer", "array is Null-Pointer.");
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayConstPointer", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetByteArrayConstPointer(&(array->bytearray));
}
NA_IAPI void* naGetArrayMutablePointer(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMutablePointer", "array is Null-Pointer.");
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
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayConstElement", "array is empty, typesize is garbage");
    }
  #endif
  return naGetByteArrayConstByte( &(array->bytearray),
                                  indx * array->typesize);
}

NA_IDEF void* naGetArrayMutableElement(NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMutableElement", "array is Null-Pointer.");
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayMutableElement", "array is empty, typesize is garbage");
    }
  #endif
  return naGetByteArrayMutableByte( &(array->bytearray),
                                    indx * array->typesize);
}



NA_IDEF NAInt naGetArrayCount(const NAArray* array){
  NAInt bytesize;
  
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCount", "array is Null-Pointer.");
    }
  #endif
  bytesize = naGetByteArraySize(&(array->bytearray));
  return bytesize ? (bytesize / array->typesize) : 0;
}


NA_IAPI NAInt naGetArrayTypeSize(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayTypeSize", "array is Null-Pointer.");
    }
  #endif
  return array->typesize;
}



NA_IAPI NABool naIsArrayEmpty(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayEmpty", "array is Null-Pointer.");
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
