
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_GROWING_SPACE_INCLUDED
#define NA_GROWING_SPACE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This data structure provides the functionality of storing multiple elements
// of the same type but with a yet unknown count. The number of elements
// stored is only limited by the available memory.
//
// Usage: Create a growing space and add each element one by one. When you
// are finished adding elements, dump everything into a desired structure
// like an array or a list. Or just leave the elements in this structure and
// delete them all together when you are finished using them. Any other usage
// is not encouraged.
//
// Implementation note: The growing space had previously been implemented
// with an array which got copied everytime the space had to grow. This is
// no more! NAGrowing space now stores elements in a list of arrays. Therefore,
// the addresses of your elements will not change as long as they are stored
// in the growing space. This is a huge advantage!
//
// There is also a disadvantage: Traversing the space one-by-one can only be
// done efficiently using an iterator.
//
// IMPORTANT:
// NAGrowingSpace is not a Pool structure! Deletion or recycling of elements
// is not provided. NAGrowingSpace is a strictly growing structure which is
// especially good at providing quick space for single elements which
// simply need to be stored somewhere where the programmer does not want to
// care about.
//
// Use this structure when you are building up collections, for example:
// - parsing files without prior knowledge of the number of elements stored.
// - Creating a bunch of particles in a physically based simulation.
// - Creating an unknown number of edges within a point cloud.
// And many more examples. Turns out the author uses this structure more and
// more.

#include "NAArray.h"
#include "NAList.h"

// Opaque type. See explanation in readme.txt
typedef struct NAGrowingSpace NAGrowingSpace;
struct NAGrowingSpace{
  NAList        arrays;     // List of NAByteArray*
  NAUInt        typesize;   // The size in bytes of the stored type
  NAUInt        usedcount;  // The used number of elements in the storage.
  NAConstructor constructor;
};


// Creates a new NAGrowingSpace with the desired type size.
NA_API NAGrowingSpace* naCreateGrowingSpace(NAGrowingSpace* space,
                                                     NAUInt typesize,
                                              NAConstructor constructor);

// Clears or destroys the given space.
NA_API void naClearGrowingSpace  (NAGrowingSpace* space, NADestructor destructor);
NA_API void naDestroyGrowingSpace(NAGrowingSpace* space, NADestructor destructor);

// Grows the space by 1 element. The pointer to the new element is returned.
// The returned pointer points to an uninitialized space.
NA_API void* naNewGrowingSpaceElement(NAGrowingSpace* space);

// Returns a POINTER to the element at the given index.
// The indx argument is treated the same way as with naGetArrayConstElement
// or naGetArrayMutableElement.
// Warning: Do not use the result as an interation pointer. The elements
// are NOT stored in a single array!
// These functions are slow! Use Iterators instead. Unfortunately, they are
// not yet implemented. todo
NA_API const void* naGetGrowingSpaceConstElement  (const NAGrowingSpace* space,
                                                                   NAInt indx);
NA_API void*       naGetGrowingSpaceMutableElement(      NAGrowingSpace* space,
                                                                   NAInt indx);

// Creates a new array with just the used content of the growing space.
// All used contents will be copied. If you stored pointers to the elements
// in the growing space somewhere in your code, these pointers will still point
// to the elements stored in the growing space!
NA_API NAArray* naCreateArrayOutOfGrowingSpace(    NAArray* array,
                                            NAGrowingSpace* space);

// Returns the number of elements actually stored in the space
NA_API NAInt naGetGrowingSpaceCount(const NAGrowingSpace* space);




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
