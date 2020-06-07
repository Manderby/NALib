
// This file contains inline implementations of the file NAPool.h
// Do not include this file directly! It will automatically be included when
// including "NAPool.h"


struct NAPool{
  NAByte** drops;       // The drops, speaking: Pointers.
  NAUInt count;         // The maximum count of drops in this pool.
  NAUInt cur;           // The current position in the drops array.
  void* storagearray;   // The storage of elements, if pool is created filled.
  #ifndef NDEBUG
    NAUInt typeSize;    // The typeSize is just for debugging.
  #endif
};



NA_IDEF NAPool* naInitPoolEmpty(NAPool* pool, NAUInt count){
  #ifndef NDEBUG
    if(!pool)
      naCrash("pool is Null-Pointer");
    if(count == 0)
      naError("count is 0");
  #endif
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = 0;
  pool->storagearray = NA_NULL;
  return pool;
}



NA_IDEF NAPool* naInitPoolFilled(NAPool* pool, NAUInt count, NAUInt typeSize){
  NAByte** dropptr;
  NAByte* storageptr;
  NAUInt i;

  #ifndef NDEBUG
    if(!pool)
      naCrash("pool is Null-Pointer");
    if(count == 0)
      naError("count is 0");
    if(typeSize == 0)
      naError("typeSize is 0");
  #endif
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = count;
  pool->storagearray = naMalloc(count * typeSize);
  #ifndef NDEBUG
    pool->typeSize = typeSize;
  #endif

  // Insert all elements to the drop array.
  dropptr = pool->drops;
  storageptr = (NAByte*)pool->storagearray;
  for(i = 0; i < pool->count; i++){
    *dropptr = storageptr;
    dropptr++;
    storageptr += typeSize;
  }
  return pool;
}



NA_IDEF void naClearPool(NAPool* pool){
  if(pool->storagearray){
    #ifndef NDEBUG
      if(pool->cur != pool->count)
        naError("Pool was created filled but is not filled now.");
    #endif
    free(pool->storagearray);
  }else{
    #ifndef NDEBUG
      if(pool->cur != 0)
        naError("Pool was created empty but is not empty now.");
    #endif
  }
  free(pool->drops);
}



NA_IDEF void* naSuckPool(NAPool* pool){
  #ifndef NDEBUG
    if(pool->cur == 0)
      naError("Pool is empty");
  #endif
  pool->cur--;
  return pool->drops[pool->cur];
}



NA_IDEF void naSpitPool(NAPool* pool, void* drop){
  #ifndef NDEBUG
    if(pool->cur == pool->count)
      naError("Pool is full");
    if(pool->storagearray && (!naInsidei((NAByte*)drop - (NAByte*)pool->storagearray, 0, pool->typeSize * pool->count)))
      naError("Pool was created filled. This drop does not seem to be a drop of this pool.");
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
NA_IDEF NABool na_IsPoolPartFull(NAPool* pool){
  return (pool->cur == pool->cur);
}



NA_IDEF void naForeachPool(const NAPool* pool, NAMutator mutator){
  NAUInt i;
  NAByte** curptr = pool->drops;
  for(i = 0; i < pool->cur; i++){
    mutator(*curptr);
    curptr++;
  }
}



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
