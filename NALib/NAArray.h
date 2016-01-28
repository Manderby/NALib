
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
#include "NARuntime.h"


typedef struct NAArray NAArray;


// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Creates an NAArray struct denoting an EMPTY array.
NA_IAPI NAArray* naInitArray(NAArray* array);

// Creates or fills a new NAArray struct with a given typesize in bytes and a
// count.
NA_API  NAArray* naInitArrayWithCount  (NAArray* array,
                                            NAUInt typesize,
                                            NAUInt count);

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
// Note that NALib does NOT provide an API for calling constructors on
// preallocated buffers. If you really need to do that, you have to do this
// manually.
NA_API  NAArray* naInitArrayWithConstBuffer(
                                          NAArray* array,
                                       const void* buffer,
                                            NAUInt typesize,
                                            NAUInt count);
NA_API  NAArray* naInitArrayWithMutableBuffer(
                                          NAArray* array,
                                             void* buffer,
                                            NAUInt typesize,
                                            NAUInt count,
                                  NAPointerCleanup ownership);

// Fills dstarray with a desired part of srcarray.
// offset and count can be negative: See naInitByteArrayExtraction for the
// explanation of the arguments.
NA_API  NAArray* naInitArrayExtraction( NAArray* dstarray,
                                    const NAArray* srcarray,
                                             NAInt offset,
                                             NAInt count);

// Clears the given array.
NA_IAPI void naClearArray  (NAArray* array);


// COPIES the contents of the array to a separate storage and decouples it
// from the existing storage. See naDecoupleByteArray for more info. No
// additional bytes are appended. COPIES ALWAYS!
NA_IAPI void naDecoupleArray(NAArray* array);

// Traverses the whole array and calls the mutator on each element. A pointer
// to each element will be given to the mutator.
NA_IAPI void naForeachArray(NAArray* array, NAFunc mutator);

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
NA_IAPI NAUInt naGetArrayMaxIndex(const NAArray* array);

// Returns the number of bytes needed to store one element.
NA_IAPI NAUInt naGetArrayTypeSize(const NAArray* array);

// Returns true if the array is empty.
NA_IAPI NABool naIsArrayEmpty(const NAArray* array);




// ////////////////////////////////////
// Iteration functions
//
// Every NAArray has an internal index denoting the current element.
// The programmer can control and access this element with iteration functions.
// If no current element is set, NA_NULL is returned as a pointer. A typical
// example of iteration is the following:
//
// NAArray* myarray;
// void* curelement;
// naFirstArray(myarray);
// while((curelement = naIterateArrayMutable(myarray, 1))){
//   Do stuff with curelement.
// }
//
// You should enclose the while-condition in additional parantheses such that
// a compiler knows that the returned pointer must be evaluated as a condition.
//
// Note that all iteration functions are inline. They are somewhat fast. But
// walking through an array using pointer arithmetic is still faster as there
// are quite a few if conditions to check when iterating. An iterator has some
// advantages in certain situations though, as the position within the array
// is stored persistently.
//
// When being inside the while scope, the array itself already points to the
// element AFTER iteration.
//
// Do NOT use a for-loop for iteration! Although it would not matter much with
// the NAArray structure, it nontheless should be kept consistent with other
// structures using iteration functions, see NAList for example.
// Also, it is very hard to read.

// With the following functions, you can initialize the internal pointer.
NA_IAPI void naFirstArray                  (const NAArray* array);
NA_IAPI void naLastArray                   (const NAArray* array);

// Returns a pointer to the current element in retelement and iterates the
// given steps forward or backwards by using positive or negative numbers. If
// the returned byte is valid (inside the range of the array), the function
// returns NA_TRUE. If it is not valid, the function returns NA_NULL, but the
// returned pointer in retbyte will be computed just as if it was valid.
//
// If step is 0, you simply access the current element. Note that for accessing
// the current element, the use of naGetArrayCurrent would be preferable.
NA_IAPI const void* naIterateArrayConst    (     const NAArray* array,
                                                     NAInt step);
NA_IAPI void* naIterateArrayMutable  (           NAArray* array,
                                                     NAInt step);

// Returns a pointer to the current element without moving the internal index.
NA_IAPI const void* naGetArrayCurrentConst     (const NAArray* array);
NA_IAPI       void* naGetArrayCurrentMutable   (const NAArray* array);

// Returns the current index
NA_IAPI NAUInt       naGetArrayCurrentIndex      (const NAArray* array);
// Returns the remaining number of elements starting at the current element.
// Will emit a warning if the current index is not set.
NA_IDEF NAUInt       naGetArrayRemainingCount    (const NAArray* array);

// The locate-function set the internal pointer to the index given. If the
// given index is negative, it denotes the element from the end of the array,
// whereas -1 denotes the last element.
//
// If the index is not within the array range, the internal pointer will be
// unset. Returns NA_TRUE if the index was inside range and NA_FALSE if not.
NA_IAPI NABool naLocateArrayIndex          (const NAArray* array,
                                                     NAInt indx);

// Moves the internal pointer forward or backwards without accessing the
// content.
NA_IAPI void naIterateArray                (const NAArray* array,
                                                     NAInt step);

// Returns whether the array is at a certain position.
// This functions return garbage if the array is empty!
// Returns NA_FALSE when the internal index is not set.
NA_IAPI NABool naIsArrayAtFirst            (const NAArray* array);
NA_IAPI NABool naIsArrayAtLast             (const NAArray* array);














// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAByteArray.h"
#include "NACoord.h"


struct NAArray{
  struct NAByteArray  bytearray;  // The byte array
  NAUInt              typesize;   // The size in bytes of the stored type
};



NA_IDEF NAArray* naInitArray(NAArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArray", "array is NULL"); return NA_NULL;}
  #endif
  naInitByteArray(&(array->bytearray));
  // The typesize is set to 1 such that calls to naGetArrayCount will not
  // result in bad values.
  array->typesize = 1;
  return array;
}



NA_IDEF void naClearArray(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naClearArray", "array is Null-Pointer.");
      return;
    }
    // Note that NALib does not check, if you initially created the array
    // with a constructor callback. You are responsible to pair constructors
    // and destructors appropriately.
  #endif
  naClearByteArray(&(array->bytearray));
}



NA_IDEF void naDecoupleArray(NAArray* array){
  naDecoupleByteArray(&(array->bytearray), NA_FALSE);
}



NA_IDEF void naForeachArray(NAArray* array, NAFunc mutator){
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



NA_IDEF NAUInt naGetArrayMaxIndex(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArraySize", "array is Null-Pointer.");
      return 0;
    }
    if(naIsArrayEmpty(array))
      naError("naGetArraySize", "array is empty.");
  #endif
  return naEndToMaxi(naGetArrayCount(array));
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









NA_IDEF void naFirstArray(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naFirstArray", "array is Null-Pointer.");
      return;
    }
  #endif
  naFirstByteArray(&(array->bytearray));
}



NA_IDEF void naLastArray(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naLastArray", "array is Null-Pointer.");
      return;
    }
  #endif
  naLastByteArray(&(array->bytearray));
}



NA_IDEF const void* naIterateArrayConst(const NAArray* array, NAInt step){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateArrayConst", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsArrayEmpty(array))
      naError("naIterateArrayConst", "array is empty.");
  #endif
  return naIterateByteArrayConst(&(array->bytearray), step * naGetArrayTypeSize(array));
}



NA_IDEF void* naIterateArrayMutable(NAArray* array, NAInt step){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateArrayMutable", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsArrayEmpty(array))
      naError("naIterateArrayMutable", "array is empty.");
  #endif
  return naIterateByteArrayMutable(&(array->bytearray), step * naGetArrayTypeSize(array));
}



NA_IDEF const void* naGetArrayCurrentConst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCurrentConst", "array is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteArrayCurrentConst(&(array->bytearray));
}



NA_IDEF void* naGetArrayCurrentMutable(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCurrentConst", "array is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteArrayCurrentMutable(&(array->bytearray));
}



NA_IDEF NAUInt naGetArrayCurrentIndex(const NAArray* array){
  NAUInt byteindex;
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCurrentIndex", "array is Null-Pointer.");
      return NA_INVALID_MEMORY_INDEX;
    }
  #endif
  byteindex = naGetByteArrayCurrentIndex(&(array->bytearray));
  #ifndef NDEBUG
    if(byteindex % naGetArrayTypeSize(array))
      naError("naGetArrayCurrentIndex", "Internal error: Internal index is misaligned.");
  #endif
  return byteindex / naGetArrayTypeSize(array);
}



NA_IDEF NAUInt naGetArrayRemainingCount(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayRemainingSize", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return naGetArrayCount(array) - naGetArrayCurrentIndex(array);
}


NA_IDEF NABool naLocateArrayIndex(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naFirstArray", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return naLocateByteArrayIndex(&(array->bytearray), indx * naGetArrayTypeSize(array));
}



NA_IDEF void naIterateArray(const NAArray* array, NAInt step){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateArray", "array is Null-Pointer.");
      return;
    }
    if(naIsArrayEmpty(array))
      naError("naIterateArray", "array is empty.");
  #endif
  naIterateByteArray(&(array->bytearray), step * naGetArrayTypeSize(array));
}



NA_IDEF NABool naIsArrayAtFirst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayAtFirst", "array is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsArrayEmpty(array))
      naError("naIsArrayAtFirst", "array is empty. Result is garbage");
  #endif
  return (naGetArrayCurrentIndex(array) == 0);
}



NA_IDEF NABool naIsArrayAtLast(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayAtLast", "array is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsArrayEmpty(array))
      naError("naIsArrayAtLast", "array is empty. Result is garbage");
  #endif
  return (naGetArrayCurrentIndex(array) == naGetArrayMaxIndex(array));
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
