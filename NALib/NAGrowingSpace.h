
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_GROWING_SPACE_INCLUDED
#define NA_GROWING_SPACE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAArray.h"

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


// Opaque type. See explanation in readme.txt
typedef struct NAGrowingSpace NAGrowingSpace;
struct NAGrowingSpace{
  struct NAArray array;
  NAInt          usedcount;  // The used number of elements in the storage.
};


// Creates a new GrowingSpace with the desired type size.
NAGrowingSpace* naCreateGrowingSpace(NAGrowingSpace* space,
                                             NAInt typesize,
                                             NAInt initialcount);

// Grows the Space by 1 element and copies the contents of the given pointer to
// the new element. If element is NA_NULL, nothing is copied. The pointer to the
// new element is returned.
void* naAddGrowingSpaceElement(NAGrowingSpace* space, void* elemptr);

// Dumps the used content of the growing space to array. The given growingspace
// will automatically be freed and is invalid after this call.
NAArray* naCreateArrayOutOfGrowingSpace(NAArray* array, NAGrowingSpace* space);


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
