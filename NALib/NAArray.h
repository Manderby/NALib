
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_ARRAY_INCLUDED
#define NA_ARRAY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAByteArray.h"

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


// Opaque type. See explanation in readme.txt
typedef struct NAArray NAArray;
struct NAArray{
  struct NAByteArray bytearray;  // The byte array
  NAInt              typesize;   // The size in bytes of the stored type
};
// Note that an empty array is marked with 0 in the the size field of
// bytearray. If that field is zero, all other fields are garbage.



// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Creates an EMPTY array.
NA_INLINE_API NAArray* naCreateArray(NAArray* array);

// Creates or fills a new array with a given typesize in bytes and a count.
NA_INLINE_API NAArray* naCreateArrayWithCount  (NAArray* array,
                                            NAInt typesize,
                                            NAInt count);

// Fills dstarray with a desired part of srcarray.
// See naCreateByteArrayExtraction for the explanation of all arguments.
NA_INLINE_API NAArray* naCreateArrayExtraction( NAArray* dstarray,
                                         NAArray* srcarray,
                                            NAInt offset,
                                            NAInt count);


// Clears or destroys the given array.
NA_INLINE_API void naClearArray  (NAArray* array);
NA_INLINE_API void naDestroyArray(NAArray* array);



// Returns the pointer to the desired element. Note that if the array stores
// pointers, this function will return a pointer to a pointer.
NA_INLINE_API const void* naGetArrayConstElement  (const NAArray* array, NAInt index);
NA_INLINE_API void*       naGetArrayMutableElement(      NAArray* array, NAInt index);

// Returns the number of elements in this array. Beware: This is different
// from naGetByteArraySize. The size returned by naGetByteArraySize is a size
// in bytes, not necessary the element count.
// Also note that this function requires a (costly) division operation. When
// using it a lot, for example in a loop, it might be a good idea to store the
// count in a variable.
NA_INLINE_API NAInt naGetArrayCount(NAArray* array);

















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

NA_INLINE_API NAArray* naCreateArray(NAArray* array){
  array = naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArray(&(array->bytearray));
  naOnei(&(array->typesize));
  return array;
}


NA_INLINE_API NAArray* naCreateArrayWithCount(NAArray* array,
                                                    NAInt typesize,
                                                    NAInt count){
  #ifndef NDEBUG
    if(typesize < 0)
      naError("naCreateArrayWithCount", "typesize is < 1.");
    if(count < 0)
      naError("naCreateArrayWithCount", "count is < 0.");
  #endif
  if(!count){return naCreateArray(array);}
  array = naAllocateIfNull(array, sizeof(NAArray));
  naCreateByteArrayWithSize(&(array->bytearray), typesize * count);
  // Note that if count was 0, the typesize is irrelevant.
  array->typesize = typesize;
  return array;
}


NA_INLINE_API NAArray* naCreateArrayExtraction(   NAArray* dstarray,
                                    NAArray* srcarray,
                                       NAInt offset,
                                       NAInt count){
  dstarray = naAllocateIfNull(dstarray, sizeof(NAArray));

  dstarray->typesize = srcarray->typesize;
  naCreateByteArrayExtraction(&(dstarray->bytearray),
                            &(srcarray->bytearray),
                            offset * srcarray->typesize,
                            count * srcarray->typesize);
  return dstarray;
}


NA_INLINE_API void naClearArray(NAArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naClearArray", "array is Null-Pointer."); return;}
  #endif
  naClearByteArray(&(array->bytearray));
}


NA_INLINE_API void naDestroyArray(NAArray* array){
  naClearArray(array);
  free(array);
}


NA_INLINE_API const void* naGetArrayConstElement(const NAArray* array,
                                                             NAInt index){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetArrayElement", "array is Null-Pointer."); return NA_NULL;}
    if(naIsByteArrayEmpty(&(array->bytearray)))
      naError("naGetArrayElement", "array is empty, typesize is garbage");
  #endif
  return naGetByteArrayConstByte( &(array->bytearray),
                                  index * array->typesize);
}

NA_INLINE_API void* naGetArrayMutableElement(NAArray* array, NAInt index){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetArrayElement", "array is Null-Pointer."); return NA_NULL;}
    if(naIsByteArrayEmpty(&(array->bytearray)))
      naError("naGetArrayElement", "array is empty, typesize is garbage");
  #endif
  return naGetByteArrayMutableByte( &(array->bytearray),
                                    index * array->typesize);
}



NA_INLINE_API NAInt naGetArrayCount(NAArray* array){
  NAInt bytesize;
  
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetArrayCount", "array is Null-Pointer."); return 0;}
  #endif
  bytesize = naGetByteArraySize(&(array->bytearray));
  return bytesize ? (bytesize / array->typesize) : 0;
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
