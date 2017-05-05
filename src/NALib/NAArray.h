
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
// default C-like array. It has the big advantage though of knowing how many
// elements are stored and hence being able to check for under- and overflows
// as well as locating elements counting from the end (using negative indexes).
//
// This array implementaiton is more for complex datastructure building. If
// you are looking for a simple array of integers for example, better use the
// traditional [] arrays.
//
// Important: NAArray returns pointers to the elements. You have to typecast
// the returned element-pointers! Also be aware that if the array stores
// pointers, get functions will return a pointer to a pointer.


#include "NAMemory.h"


// The full type definition is in the file "NAArrayII.h"
typedef struct NAArray NAArray;
typedef struct NAArrayIterator NAArrayIterator;


// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Initializes an NAArray denoting an EMPTY array.
NA_IAPI NAArray* naInitArray(NAArray* array);

// Initializes an NAArray with enough space for the given typesize and count.
// Typesize denoes the number of bytes per element and count denotes the number
// of elements.
NA_IAPI NAArray* naInitArrayWithCount(        NAArray* array,
                                                NAInt typesize,
                                                NAInt count);

// Creates an NAArray which stores the given data WITHOUT copying.
//
// Use these functions to encapsulate your own raw buffers into an NAArray.
// There are two creation functions, one for const data and one for non-const.
//
// Typesize denoes the number of bytes per element and count denotes the number
// of elements. The programmer is responsible for the total number of bytes
// not overflowing the data range.
//
// When mutable data is stored, you can set the cleanup method which will clean
// up the given memory pointer automatically upon clearing the NAArray.
//
// Note that if you need to call constructors or destructors upon all elements,
// you can use naForeachArray.
NA_IAPI  NAArray* naInitArrayWithDataConst(   NAArray* array,
                                           const void* data,
                                                 NAInt typesize,
                                                 NAInt count);
NA_IAPI  NAArray* naInitArrayWithDataMutable( NAArray* array,
                                                 void* data,
                                                 NAInt typesize,
                                                 NAInt count,
                                       NAMemoryCleanup cleanup);

// Clears the given array.
NA_IAPI void naClearArray                    (NAArray* array);

// Traverses the whole array and calls the accessor or mutator on each element.
// A pointer to each element will be given to the accessor or mutator.
NA_IAPI void naForeachArrayConst  (NAArray* array, NAAccessor accessor);
NA_IAPI void naForeachArrayMutable(NAArray* array, NAMutator  mutator);

// Returns a pointer to the very first element of the raw data array.
// Notice: This function is speedy.
NA_IAPI const void* naGetArrayPointerConst  (const NAArray* array);
NA_IAPI       void* naGetArrayPointerMutable(      NAArray* array);

// Returns a pointer to the element at the given index. If indx is negative, it
// is searched from the end of the array. For example -1 denotes the last
// element.
//
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetArrayPointer or this
// function and use pointer arithmetic afterwards.
NA_IAPI const void* naGetArrayElementConst  (const NAArray* array, NAInt indx);
NA_IAPI       void* naGetArrayElementMutable(      NAArray* array, NAInt indx);

// Returns information about of nuber of elements in this array.
NA_IAPI NAInt naGetArrayCount    (const NAArray* array);
NA_IAPI NAInt naGetArrayMaxIndex (const NAArray* array);
NA_IAPI NAInt naGetArrayTypesize (const NAArray* array);
NA_IAPI NABool naIsArrayEmpty     (const NAArray* array);




// ////////////////////////////////////
// Iteration functions
//
// Before NALib verison 18, NAArray contained a built-in iterator. But starting
// with Version 18, there exists NAArrayIterator, a structure separate from an
// array. It gives the programmer way more flexibility to move freely within an
// array.
//
// To keep the API clean, the function names do not use "ArrayIterator" but
// instead simply use "Array" as if the iterators would denote the array per se.
//
// Having a separate iterator has several advantages. It may need a little bit
// more coding but you are in much more control over the array. This is
// especially useful in a multithreaded environment.
//
// The easiest way to implement an iteration is using a while loop:
//
// NAArray* myarray;
// NAArrayIterator iter = naMakeArrayIterator(myarray);
// while(naIterateArray(&iter, 1)){
//   void* curelement = naGetArrayCurrentMutable(iter);
//   Do stuff with curelement.
// }
// naClearArrayIterator(&iter);
//
// Be sure to not forget naClearArrayIterator. Otherwise when debugging, arrays
// will keep references to iterators which are no longer in use and will hence
// emit a warning when they are cleared. When NDEBUG is defined however, no
// such checks will be performed.


// ///////////////////////////////
// Creating and positioning an iterator:
//
// The naMakeArrayIteratorXXX functions will create a new iterator which is
// positioned at the initial position of the array. From there on, an array
// can be searched forward or backward. You define, if the iterator can
// only access the contents or mutate them.
//
// After you are done using the iterator, you should clear it with a call to
// naClearArrayIterator. NALib keeps track of where the iterators are when
// NDEBUG is undefined. Therefore, you will get lots of warnings if the
// iterators are not properly cleared. In the release code, no checks are
// performed.
NA_IAPI NAArrayIterator naMakeArrayAccessor(const NAArray* array);
NA_IAPI NAArrayIterator naMakeArrayMutator (      NAArray* array);

NA_IAPI void naClearArrayIterator(NAArrayIterator* iterator);

// After having created an iterator, it is at the initial position but you may
// position it to a desired element by using one of the Locate functions:
//
// First:         Moves iterator to first element. This function is very fast.
// Last:          Moves iterator to last element. This function is very fast.
// Content:       A specific content pointer is searched within the array. This
//                function is very slow.
// Index:         The element with the specified index is searched. If the given
//                index is negative, it denotes the element from the end of the
//                list, whereas -1 denotes the last element.
//
// The Content and Index variants will return NA_FALSE, if the element has not
// been found and NA_TRUE if it has been found. If not found, the iterator will
// point to the initial array position.
// 
// The other variants will always return NA_TRUE. That's because of speed.
// Erroneous behaviour will only be checked when NDEBUG is undefined and hence
// the programmer is already forced to do things right when debugging the code.
NA_IAPI NABool naLocateArrayFirst    (NAArrayIterator* iterator);
NA_IAPI NABool naLocateArrayLast     (NAArrayIterator* iterator);
NA_IAPI NABool naLocateArrayContent  (NAArrayIterator* iterator, const void* content);
NA_IAPI NABool naLocateArrayIndex    (NAArrayIterator* iterator, NAInt indx);


// /////////////////////////////////
// Iterating
// Moves the iterator forward or backward the given number of positive or
// negative steps respectively. If the step over- or underflows the array
// and NDEBUG is undefined, a warning is emitted.
//
// Returns NA_FALSE when one of the two ends of the array is reached.
NA_IAPI NABool  naIterateArray        (NAArrayIterator* iterator, NAInt step);

// /////////////////////////////////
// Returns the content of the previous, current or next element without moving
// the iterator.
//
// If the iterator is at a position where the desired element does not exist
// (for example the next element when being at the last element or the current
// element when being located at the initial position), a Null-Pointer
// will be returned without warning.
NA_IAPI const void* naGetArrayPrevConst      (const NAArrayIterator* iterator);
NA_IAPI       void* naGetArrayPrevMutable    (      NAArrayIterator* iterator);
NA_IAPI const void* naGetArrayCurrentConst   (const NAArrayIterator* iterator);
NA_IAPI       void* naGetArrayCurrentMutable (      NAArrayIterator* iterator);
NA_IAPI const void* naGetArrayNextConst      (const NAArrayIterator* iterator);
NA_IAPI       void* naGetArrayNextMutable    (      NAArrayIterator* iterator);

// /////////////////////////////////////////////
// Getting position informations around the current position.
// Note that indx can be negative, denoting the number of elements from the
// end of the array, whereas -1 denotes the last element.
NA_IAPI NABool    naIsArrayAtFirst           (const NAArrayIterator* iterator);
NA_IAPI NABool    naIsArrayAtLast            (const NAArrayIterator* iterator);
NA_IAPI NABool    naIsArrayAtIndex           (const NAArrayIterator* iterator, NAInt indx);
NA_IAPI NABool    naIsArrayAtInitial         (const NAArrayIterator* iterator);

NA_IAPI NAInt     naGetArrayCurrentIndex     (const NAArrayIterator* iterator);




// Inline implementations are in a separate file:
#include "NAStruct/NAArrayII.h"




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
