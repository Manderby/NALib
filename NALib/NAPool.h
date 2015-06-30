
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_POOL_INCLUDED
#define NA_POOL_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"
#include "NAPointer.h"


typedef struct NAPool NAPool;

NA_API NAPool* naCreatePoolEmpty(NAPool* pool, NAUInt count);
NA_API NAPool* naCreatePoolFilled(NAPool* pool, NAUInt count, const NAStructInfo* structinfo, NABool aspointerarray);
NA_API void naClearPool(NAPool* pool);
NA_API void naDestroyPool(NAPool* pool);


struct NAPool{
  void** drops;
  NAUInt count;
  NAUInt cur;

  void* storagearray;
  NABool storageispointerarray;
  const NAStructInfo* structinfo;
};



NA_DEF NAPool* naCreatePoolEmpty(NAPool* pool, NAUInt count){
  naAllocNALibStruct(pool, NAPool);
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = 0;
  pool->storagearray = NA_NULL;
}



NA_DEF NAPool* naCreatePoolFilled(NAPool* pool, NAUInt count, const NAStructInfo* structinfo, NABool aspointerarray){
  naAllocNALibStruct(pool, NAPool);
  pool->drops = naMalloc(count * sizeof(void*));
  pool->count = count;
  pool->cur = count;
  pool->structinfo = structinfo;
  pool->storageispointerarray = aspointerarray;
  if(pool->storageispointerarray){
    pool->storagearray = naMalloc(count * sizeof(void*));
    void** dropptr = pool->drops;
    void** storageptr = pool->storagearray;
    for(NAUInt i=0; i<pool->count; i++){
      *storageptr = naMalloc(structinfo->structsize);
      if(pool->structinfo->constructor){structinfo->constructor(*storageptr);}
      *dropptr = *storageptr;
      dropptr++;
      storageptr++;
    }
  }else{
    pool->storagearray = naMalloc(count * structinfo->structsize);
    void** dropptr = pool->drops;
    NAByte* storageptr = (NAByte*)pool->storagearray;
    for(NAUInt i=0; i<pool->count; i++){
      if(pool->structinfo->constructor){structinfo->constructor(storageptr);}
      *dropptr = storageptr;
      dropptr++;
      storageptr += structinfo->structsize;
    }
  }
}



NA_DEF void naClearPool(NAPool* pool){
  if(pool->storagearray){
    #ifndef NDEBUG
      if(pool->cur != pool->count)
        naError("naClearPool", "Pool was created filled but is not filled now.");
    #endif
    if(pool->storageispointerarray){
      void** storageptr = pool->storagearray;
      for(NAUInt i=0; i<pool->count; i++){
        if(pool->structinfo->destructor){pool->structinfo->destructor(*storageptr);}
        free(*storageptr);
        storageptr++;
      }
    }else{
      NAByte* storageptr = (NAByte*)pool->storagearray;
      for(NAUInt i=0; i<pool->count; i++){
        if(pool->structinfo->destructor){pool->structinfo->destructor(storageptr);}
        storageptr += pool->structinfo->structsize;
      }
    }
    free(pool->storagearray);
  }else{
    #ifndef NDEBUG
      if(pool->cur != 0)
        naError("naClearPool", "Pool was created empty but is not empty now.");
    #endif
  }
  free(pool->drops);
}



NA_DEF void naDestroyPool(NAPool* pool){
  naClearPool(pool);
  free(pool);
}



NA_DEF NAUInt naGetPoolRemainingCount(NAPool* pool){
  return pool->count - pool->cur;
}



NA_DEF void* naSuckPool(NAPool* pool){
  #ifndef NDEBUG
    if(pool->cur == 0)
      naError("naSuckPool", "Pool is empty");
  #endif
  pool->cur--;
  return pool->drops[pool->cur];
}



NA_DEF void naSpitPool(NAPool* pool, void* element){
  #ifndef NDEBUG
    if(pool->cur == pool->count)
      naError("naSpitPool", "Pool is full");
  #endif
  pool->drops[pool->cur] = element;
  pool->cur++;
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
