
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_GROWING_SPACE_INCLUDED
#define NA_GROWING_SPACE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This data structure provides the functionality of storing multiple elements
// of the same type but with a yet unknown count. This is useful when parsing
// files without prior knowledge of the number of elements stored.
//
// Usage: Create a growing space and add each element one by one. When you
// are finished adding elements, dump everything into a desired structure
// like an array or a list. Any other usage is not encouraged.
//
// Implementation note: The growing space is implemented as an array which
// grows in increments of the power of 2. First, the array is empty but
// provides space for 1 element. Later on it provides space for 2, 4, 8, ...
// elements. Everytime, the provided space increases, the whole array gets
// copied bit by bit.
//
// Warning: Do not store pointers to any of the contents. They might likely
// change.


#include "NAArray.h"

// Opaque type. See explanation in readme.txt
typedef struct NAGrowingSpace NAGrowingSpace;
struct NAGrowingSpace{
  NAArray array;
  NAInt   usedcount;  // The used number of elements in the storage.
};


// Creates a new GrowingSpace with the desired type size. If initialcount is
// 0, it will nontheless allocate space for 1 element
NAGrowingSpace* naCreateGrowingSpace(NAGrowingSpace* space,
                                             NAInt typesize,
                                             NAInt initialcount);

// Creates a duplicate of the given original space. Note that the new space
// may not have the same amount of remaining available space!
NAGrowingSpace* naDuplicateGrowingSpace(NAGrowingSpace* space,
                                  const NAGrowingSpace* originalspace);

// Clears or destroys the given array.
void naClearGrowingSpace  (NAGrowingSpace* space, NADestructor destructor);
void naDestroyGrowingSpace(NAGrowingSpace* space, NADestructor destructor);

// Grows the space by 1 element. In both versions, the pointer to the new
// element is returned. The "New" version leaves the new element uninitialized
// but returns the new index in the given indexptr if available. The "Add"
// version copies the contents of the given elemptr to the new element. If
// elemptr is NA_NULL, nothing is copied.
void* naNewGrowingSpaceElement(NAGrowingSpace* space, NAInt* indexptr);
void* naAddGrowingSpaceElement(NAGrowingSpace* space, void* elemptr);

// Returns a pointer to the very first element of the growing space. Warning:
// result is garbage if the space is empty. Notice: This function is speedy.
const void* naGetGrowingSpaceConstPointer  (const NAGrowingSpace* space);
void*       naGetGrowingSpaceMutablePointer(      NAGrowingSpace* space);

// Returns a POINTER to the element at the given index.
// The index argument is threated the same way as with naGetArrayConstElement
// or naGetArrayMutableElement.
const void* naGetGrowingSpaceConstElement  (const NAGrowingSpace* space,
                                                            NAInt indx);
void*       naGetGrowingSpaceMutableElement(      NAGrowingSpace* space,
                                                            NAInt indx);

// Creates a new array with just the used content of the growing space.
// If copying is NA_TRUE, the contents get copied to a new storage. If copying
// is NA_FALSE, the storage of the growing space is used. You preferably will use
// copying = NA_TRUE, when the growing space is only sparsely used and will
// preferably use copying = NA_FALSE when the growing space is likely full.
// NA_FALSE ist also useful if the resulting array will only be used shortly
// and will be erased soon.
// Note that either way, you can safely clear or destroy the growing space after
// this function while still preserving the array with the used contents.
NAArray* naCreateArrayOutOfGrowingSpace(    NAArray* array,
                                     NAGrowingSpace* space,
                                              NABool copying);

// Returns the number of elements actually stored in the space
NAInt naGetGrowingSpaceCount(const NAGrowingSpace* space);


#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_GROWING_SPACE_INCLUDED



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
