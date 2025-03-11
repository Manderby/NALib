
// This file contains inline implementations of the file NAPool.h
// Do not include this file directly! It will automatically be included when
// including "NAPool.h"


struct NAPool{
  NAByte** drops;       // The drops, speaking: Pointers.
  size_t count;         // The maximum count of drops in this pool.
  size_t cur;           // The current position in the drops array.
  void* storageArray;   // The storage of elements, if pool is created filled.
  #if NA_DEBUG
    size_t typeSize;    // The typeSize is just for debugging.
  #endif
};



NA_IDEF NAPool* naInitPoolEmpty(NAPool* pool, size_t count) {
  #if NA_DEBUG
    if(!pool)
      naCrash("pool is Null-Pointer");
    if(count == 0)
      naError("count is 0");
  #endif
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = 0;
  pool->storageArray = NA_NULL;
  return pool;
}



NA_IDEF NAPool* naInitPoolFilled(NAPool* pool, size_t count, size_t typeSize) {
  #if NA_DEBUG
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
  pool->storageArray = naMalloc(count * typeSize);
  #if NA_DEBUG
    pool->typeSize = typeSize;
  #endif

  // Insert all elements to the drop array.
  NAByte** dropptr = pool->drops;
  NAByte* storageptr = (NAByte*)pool->storageArray;
  for(size_t i = 0; i < pool->count; ++i) {
    *dropptr = storageptr;
    dropptr++;
    storageptr += typeSize;
  }
  return pool;
}



NA_IDEF void naClearPool(NAPool* pool) {
  if(pool->storageArray) {
    #if NA_DEBUG
      if(pool->cur != pool->count)
        naError("Pool was created filled but is not filled now.");
    #endif
    free(pool->storageArray);
  }else{
    #if NA_DEBUG
      if(pool->cur != 0)
        naError("Pool was created empty but is not empty now.");
    #endif
  }
  free(pool->drops);
}



NA_IDEF void* naSuckPool(NAPool* pool) {
  #if NA_DEBUG
    if(pool->cur == 0)
      naError("Pool is empty");
  #endif
  pool->cur--;
  return pool->drops[pool->cur];
}



NA_IDEF void naSpitPool(NAPool* pool, void* drop) {
  #if NA_DEBUG
    if(pool->cur == pool->count)
      naError("Pool is full");
    #if NA_TYPE_NAINT_BITS == 32
      if(pool->storageArray && (!naInsidei32((NAByte*)drop - (NAByte*)pool->storageArray, 0, pool->typeSize * pool->count)))
        naError("Pool was created filled. This drop does not seem to be a drop of this pool.");
    #elif NA_TYPE_NAINT_BITS == 64
      if(pool->storageArray && (!naInsidei64((NAByte*)drop - (NAByte*)pool->storageArray, 0, pool->typeSize * pool->count)))
        naError("Pool was created filled. This drop does not seem to be a drop of this pool.");
    #else
      #error "integer size not supported"
      return 0;
    #endif
  #endif
  pool->drops[pool->cur] = drop;
  pool->cur++;
}



NA_IDEF size_t naGetPoolCount(NAPool* pool) {
  return pool->cur;
}
NA_IDEF size_t naGetPoolRemainingCount(NAPool* pool) {
  return pool->count - pool->cur;
}
NA_IDEF NABool naIsPoolEmpty(NAPool* pool) {
  return (pool->cur == 0);
}
NA_IDEF NABool na_IsPoolPartFull(NAPool* pool) {
  return (pool->count == pool->cur);
}



NA_IDEF void naForeachPool(const NAPool* pool, NAMutator mutator) {
  NAByte** curptr = pool->drops;
  for(size_t i = 0; i < pool->cur; ++i) {
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
