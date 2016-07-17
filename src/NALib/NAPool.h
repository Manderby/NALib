
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_POOL_INCLUDED
#define NA_POOL_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This file contains a very simple pool structure which can hold a fixed
// amount drops. Drops are either arbitrary pointers or pointers to elements
// stored in an internal memory block.
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





// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
//


struct NAPool{
  NAByte** drops;       // The drops, speaking: Pointers.
  NAUInt count;         // The maximum count of drops in this pool.
  NAUInt cur;           // The current position in the drops array.
  void* storagearray;   // The storage of elements, if pool is created filled.
  #ifndef NDEBUG
    NAUInt typesize;    // The typesize is just for debugging.
  #endif
};



NA_IDEF NAPool* naInitPoolEmpty(NAPool* pool, NAUInt count){
  #ifndef NDEBUG
    if(!pool)
      {naCrash("naInitPoolEmpty", "pool is Null-Pointer"); return NA_NULL;}
    if(count == 0)
      naError("naInitPoolEmpty", "count is 0");
  #endif
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = 0;
  pool->storagearray = NA_NULL;
  return pool;
}



NA_IDEF NAPool* naInitPoolFilled(NAPool* pool, NAUInt count, NAUInt typesize){
  NAByte** dropptr;
  NAByte* storageptr;
  NAUInt i;

  #ifndef NDEBUG
    if(!pool)
      {naCrash("naInitPoolFilled", "pool is Null-Pointer"); return NA_NULL;}
    if(count == 0)
      naError("naInitPoolFilled", "count is 0");
    if(typesize == 0)
      naError("naInitPoolFilled", "typesize is 0");
  #endif
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = count;
  pool->storagearray = naMalloc(count * typesize);
  #ifndef NDEBUG
    pool->typesize = typesize;
  #endif

  // Insert all elements to the drop array.
  dropptr = pool->drops;
  storageptr = (NAByte*)pool->storagearray;
  for(i=0; i<pool->count; i++){
    *dropptr = storageptr;
    dropptr++;
    storageptr += typesize;
  }
  return pool;
}



NA_IDEF void naClearPool(NAPool* pool){
  if(pool->storagearray){
    #ifndef NDEBUG
      if(pool->cur != pool->count)
        naError("naClearPool", "Pool was created filled but is not filled now.");
    #endif
    free(pool->storagearray);
  }else{
    #ifndef NDEBUG
      if(pool->cur != 0)
        naError("naClearPool", "Pool was created empty but is not empty now.");
    #endif
  }
  free(pool->drops);
}



NA_IDEF void* naSuckPool(NAPool* pool){
  #ifndef NDEBUG
    if(pool->cur == 0)
      naError("naSuckPool", "Pool is empty");
  #endif
  pool->cur--;
  return pool->drops[pool->cur];
}



NA_IDEF void naSpitPool(NAPool* pool, void* drop){
  #ifndef NDEBUG
    if(pool->cur == pool->count)
      naError("naSpitPool", "Pool is full");
    if(pool->storagearray && (!naInsidei((NAByte*)drop - (NAByte*)pool->storagearray, 0, pool->typesize * pool->count)))
      naError("naSpitPool", "Pool was created filled. This drop does not seem to be a drop of this pool.");
  #endif
  pool->drops[pool->cur] = drop;
  pool->cur++;
}



NA_IDEF NAUInt naGetPoolCount(NAPool* pool){
  return pool->cur;
}
NA_IDEF NAUInt naGetPoolRemainingCount(NAPool* pool){
  return pool->count - pool->cur;
}
NA_IDEF NABool naIsPoolEmpty(NAPool* pool){
  return (pool->cur == 0);
}
NA_IDEF NABool naIsPoolFull(NAPool* pool){
  return (pool->cur == pool->cur);
}



NA_IDEF void naForeachPool(const NAPool* pool, NAFunc mutator){
  NAUInt i;
  NAByte** curptr = pool->drops;
  for(i=0; i<pool->cur; i++){
    mutator(*curptr);
    curptr++;
  }
}



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
