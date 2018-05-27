
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAPool.h
// Do not include this file directly! It will automatically be included when
// including "NAPool.h"


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
      naCrash("naInitPoolEmpty", "pool is Null-Pointer");
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
      naCrash("naInitPoolFilled", "pool is Null-Pointer");
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
NA_IDEF NABool naIsPoolPartFull(NAPool* pool){
  return (pool->cur == pool->cur);
}



NA_IDEF void naForeachPool(const NAPool* pool, NAMutator mutator){
  NAUInt i;
  NAByte** curptr = pool->drops;
  for(i=0; i<pool->cur; i++){
    mutator(*curptr);
    curptr++;
  }
}



// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
