
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_POOL_INCLUDED
#define NA_POOL_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This file contains a very simple pool structure which can hold a fixed
// amount of drops. Drops are either arbitrary pointers or pointers to
// elements stored in an internal memory block.
//
// You use the pool by sucking drops out of it or spitting drops back.
//
// You can either create the pool empty or filled. If you create the pool
// empty, the pool is a simple pointer pool to which you can spit any pointer
// you like. If you create the pool filled, a memory block is allocated large
// enough to hold the given number of elements. The drops are pointing inside
// this memory block which is at your service when sucking out drops. You
// should not spit back drops which are not part of that pre-allocated memory.



#include "NASystem.h"


typedef struct NAPool NAPool;


// Creates a new pointer pool with the given maximum count. The pool is empty
// and can be filled with whatever drops you like.
NA_IAPI NAPool* naInitPoolEmpty(NAPool* pool, NAUInt count);

// Creates a new pool pre-filled with count elements with the given typesize.
NA_IAPI NAPool* naInitPoolFilled(NAPool* pool, NAUInt count, NAUInt typesize);

// Clears the pool. Depending whether you created the pool empty or filled,
// your pool should be in the same state now.
NA_IAPI void naClearPool(NAPool* pool);

// Sucks a drop from the pool or spits one back.
NA_IAPI void* naSuckPool(NAPool* pool);
NA_IAPI void  naSpitPool(NAPool* pool, void* drop);

// Returns information about the number of elements stored in the pool.
NA_IAPI NAUInt naGetPoolCount(NAPool* pool);
NA_IAPI NAUInt naGetPoolRemainingCount(NAPool* pool);
NA_IAPI NABool naIsPoolEmpty(NAPool* pool);
NA_IAPI NABool naIsPoolFull(NAPool* pool);

// Calls the mutator-function for each element inside the pool.
NA_IAPI void naForeachPool(const NAPool* pool, NAFunc mutator);





// Inline implementations are in a separate file:
#include "NAStruct/NAPoolII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_POOL_INCLUDED


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
