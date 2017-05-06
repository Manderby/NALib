
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_STACK_INCLUDED
#define NA_STACK_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This data structure provides the functionality of storing multiple elements
// of the same type ordered in a stack-wise order. The number of elements
// stored is only limited by the available memory.
//
// Implementation notes:
// NAStack was originally named NAGrowingSpace which was a strictly growing
// structure which provided very fast access to elements with a yet unknown
// count. With NALib version 18, the implementation was enhancet with a Pop
// function and hence became a fully featured stack implementation.
//
// NAStack stores elements in a list of arrays. Therefore, the addresses
// of your elements will not change as long as they are stored in the stack.
// Traversing the stack one-by-one can be done efficiently using an iterator.
//
// The stack grows exponentially. Starting with an initial count, the
// following arrays will always be double in size. Choose your initial size
// wisely.
//
// IMPORTANT:
// NAStack is not a Pool structure!
// NAStack is a growing and shrinking structure which is especially good at
// providing quick space for single elements which simply need to be stored
// somewhere where the programmer does not want to care about.
//
// Aside from the clear functionality if a stack, you can use this structure
// when you are building up collections, for example:
// - parsing files without prior knowledge of the number of elements stored.
// - Creating a bunch of particles in a physically based simulation.
// - Creating an unknown number of edges within a point cloud.
// And many more examples. Turns out the author uses this structure quite a
// lot.


#include "NAArray.h"
#include "NAList.h"


// The full type definition is in the file "NAStackII.h"
typedef struct NAStack NAStack;
typedef struct NAStackIterator NAStackIterator;


// Creates a new NAStack with the desired type size. The memory will grow
// exponentially over time. With the minimalcount parameter, you can define,
// where the exponential growth shall start. NAStack will always have at least
// that amount of memory prepared. The values 0 and 1 are reserved for future
// use, you have to use at least a minimalcount of 2.
NA_IAPI NAStack* naInitStack( NAStack* stack,
                                NAInt typesize,
                                NAInt minimalcount);

// Clears the given stack.
NA_IAPI void naClearStack  (NAStack* stack);

// Push:  Grows the stack by 1 element and returns a pointer to the new element.
// Top:   Accesses the topmost element by returning a pointer to that element.
// Pop:   Shrinks the stack by 1 element and returns a pointer to the element
//        which just had beed popped.
//
// Note that the Pop function returns the element which had been removed. It
// will still be available shortly after a call to this function as the stack
// will only shrink upon a call to naShrinkStackIfNecessary and hence the data
// remains in memory as long as it is not overwritten or freed.
NA_IAPI void* naPushStack(NAStack* stack);
NA_IAPI void* naTopStack(NAStack* stack);
NA_IAPI void* naPopStack(NAStack* stack);

// Returns the number of elements actually stored in the stack
NA_IAPI NAInt naGetStackCount(const NAStack* stack);
// Returns the number of elements reserved in memory.
NA_IAPI NAInt naGetStackReservedCount(const NAStack* stack);

// The stack will never shrink automatically. If you want the stack to shrink,
// you need to call this function. The stack will be shrinked so that not more
// than three quarters of the stack are unused. If you choose aggressive to be
// NA_TRUE, the stack will be shrinked even more so that not more than half
// of the stack are unused. If you observe an unusual high amount of repetitive
// memory allocations, you may want to set aggressive to NA_FALSE.
NA_IAPI void naShrinkStackIfNecessary(NAStack* stack, NABool aggressive);



// //////////////////////////
// Iteration functions
// //////////////////////////

// Creates and clears an interator working upon a given NAStack. You have to
// choose whether it is an accessor or mutator. An accessor can not get a
// mutable pointers to the elements.
NA_IAPI NAStackIterator naMakeStackAccessor(const NAStack* stack);
NA_IAPI NAStackIterator naMakeStackMutator (const NAStack* stack);
NA_IAPI void naClearStackIterator(NAStackIterator* iterator);

// Iterates through the stack from base to top. Returns NA_FALSE if there are
// no more elements.
NA_IAPI NABool      naIterateStack(NAStackIterator* iterator);

// Returns a pointer to the current element. Note that if you store pointers
// you will get a pointer to a pointer. If you want a pointer directly, just
// use the Pointer variant
NA_IAPI const void* naGetStackCurrentConst         (NAStackIterator* iterator);
NA_IAPI void*       naGetStackCurrentMutable       (NAStackIterator* iterator);
NA_IAPI const void* naGetStackCurrentpConst        (NAStackIterator* iterator);
NA_IAPI void*       naGetStackCurrentpMutable      (NAStackIterator* iterator);

// Will call the accessor or mutator on every element stored in the stack.
// The Pointer variants expect this stack to store pointers hence
// the accessor or mutator will directly be called with the stored pointer
// instead of a pointer to a pointer.
NA_IAPI void naForeachStackConst   (const NAStack* stack, NAAccessor accessor);
NA_IAPI void naForeachStackMutable (const NAStack* stack, NAMutator  mutator);
NA_IAPI void naForeachStackpConst  (const NAStack* stack, NAAccessor accessor);
NA_IAPI void naForeachStackpMutable(const NAStack* stack, NAMutator  mutator);







// Inline implementations are in a separate file:
#include "NAStruct/NAStackII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_STACK_INCLUDED



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
