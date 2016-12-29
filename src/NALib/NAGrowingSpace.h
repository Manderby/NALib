
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
// Implementation note:
// NAGrowingSpace stores elements in a list of arrays. Therefore, the addresses
// of your elements will not change as long as they are stored in the growing
// space. Traversing the space one-by-one can be done efficiently using an
// iterator. You can even get pointers to the arrays, but only const.
//
// The growing space grows exponentially. Starting with an initial count, the
// following arrays will always be double in size. Choose your initial size
// wisely.
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


// The full type definition is in the file "NAGrowingSpaceII.h"
typedef struct NAGrowingSpace NAGrowingSpace;
typedef struct NAGrowingSpaceIterator NAGrowingSpaceIterator;


// Creates a new NAGrowingSpace with the desired type size. The space will grow
// exponentially over time. With the minimalcount parameter, you can define,
// where the exponential growth shall start. The minimalcount parameter must
// be a power of 2 and at least 2. The values 0 and 1 are not allowed.
NA_IAPI NAGrowingSpace* naInitGrowingSpace(NAGrowingSpace* space,
                                                     NAUInt typesize,
                                                     NAUInt minimalcount);

// Clears the given space.
NA_IAPI void naClearGrowingSpace  (NAGrowingSpace* space);

// Grows the space by 1 element. The pointer to the new element is returned.
// The returned pointer points to an uninitialized space.
NA_IAPI void* naNewGrowingSpaceElement(NAGrowingSpace* space);

// Returns the number of elements actually stored in the space
NA_IAPI NAUInt naGetGrowingSpaceCount(const NAGrowingSpace* space);



// Iteration functions
//
// Creates and clears an interator working upon a given NAGrowingSpace. You
// have to choose whether it is an accessor or mutator. An accessor can not
// get a mutable pointers to the elements.
NA_IAPI NAGrowingSpaceIterator naMakeGrowingSpaceIteratorAccessor(const NAGrowingSpace* space);
NA_IAPI NAGrowingSpaceIterator naMakeGrowingSpaceIteratorMutator(const NAGrowingSpace* space);
NA_IAPI void naClearGrowingSpaceIterator(NAGrowingSpaceIterator* iterator);

// Iterates through the space one by one. Returns NA_FALSE, if there are no
// more elements.
NA_IAPI NABool      naIterateGrowingSpace(NAGrowingSpaceIterator* iterator);

// Returns a pointer to the current element. Note that if you store pointers
// you will get a pointer to a pointer.
NA_IAPI const void* naGetGrowingSpaceCurrentConst   (NAGrowingSpaceIterator* iterator);
NA_IAPI void*       naGetGrowingSpaceCurrentMutable (NAGrowingSpaceIterator* iterator);

// Will call the accessor or mutator on every element stored in the space.
// The Pointer variants expect this growing space to store pointers hence
// the accessor or mutator will directly be called with the stored pointer
// instead of a pointer to a pointer.
NA_IAPI void naForeachGrowingSpaceConst         (const NAGrowingSpace* space, NAAccessor accessor);
NA_IAPI void naForeachGrowingSpaceMutable       (const NAGrowingSpace* space, NAMutator  mutator);
NA_IAPI void naForeachGrowingSpacePointerConst  (const NAGrowingSpace* space, NAAccessor accessor);
NA_IAPI void naForeachGrowingSpacePointerMutable(const NAGrowingSpace* space, NAMutator  mutator);







// Inline implementations are in a separate file:
#include "NAStruct/NAGrowingSpaceII.h"




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
