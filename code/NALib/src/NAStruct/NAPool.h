
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



#include "../NABase/NABase.h"


NA_PROTOTYPE(NAPool);


// Creates a new pointer pool with the given maximum count. The pool is empty
// and can be filled with whatever drops you like.
NA_IAPI NAPool* naInitPoolEmpty(NAPool* pool, size_t count);

// Creates a new pool pre-filled with count elements with the given typeSize.
NA_IAPI NAPool* naInitPoolFilled(NAPool* pool, size_t count, size_t typeSize);

// Clears the pool. Depending whether you created the pool empty or filled,
// your pool should be in the same state now.
NA_IAPI void naClearPool(NAPool* pool);

// Sucks a drop from the pool or spits one back.
NA_IAPI void* naSuckPool(NAPool* pool);
NA_IAPI void  naSpitPool(NAPool* pool, void* drop);

// Returns information about the number of elements stored in the pool.
NA_IAPI size_t naGetPoolCount(NAPool* pool);
NA_IAPI size_t naGetPoolRemainingCount(NAPool* pool);
NA_IAPI NABool naIsPoolEmpty(NAPool* pool);
NA_IAPI NABool na_IsPoolPartFull(NAPool* pool);

// Calls the mutator-function for each element inside the pool.
NA_IAPI void naForeachPool(const NAPool* pool, NAMutator mutator);





// Inline implementations are in a separate file:
#include "Core/NAPoolII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_POOL_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
