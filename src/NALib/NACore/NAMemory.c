
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "../NAMemory.h"
#include "../NAString.h"
#include "../NAList.h"


// Turns out, the pagesize is far too small to result in good speed
// improvements. The custom bytesize can result in up to 2 times faster allocation
// and deallocation on some systems.
#define NA_COREPOOL_BYTESIZE_EQUALS_PAGESIZE
#define NA_CUSTOM_POOL_BYTESIZE (1 << 16)


typedef struct NACorePool NACorePool;
typedef struct NACoreTypeInfo NACoreTypeInfo;
typedef struct NARuntime NARuntime;


struct NACoreTypeInfo{
  NACorePool*       curpool;
  NAUInt            typesize;
  NAMutator            destructor;
};

struct NACorePool{
  NACoreTypeInfo* coretypeinfo;
  NAUInt usedcount;
  void* firstunused;
  void* firstrecycle;
  NACorePool* prevpool;
  NACorePool* nextpool;
  // The following two fields are dummy entries not used in release versions.
  // They shall not be removed though as the total amount of bytes used for
  // an NACorePool shall be 8 times an addresssize.
  void* dummy1; // used in debugging. Points at first byte of the whole pool
  void* dummy2; // used in debugging. Points at last byte of the whole pool
};

struct NARuntime{
  NAUInt poolsize;
  NAUInt poolsizemask;
};


NARuntime* na_runtime = NA_NULL;


#if (NA_RUNTIME_USES_MEMORY_POOLS == 1)


  NA_HIDEF void* naEnhanceCorePool(NACoreTypeInfo* coretypeinfo){
    NACorePool* corepool;
    void** curunused;
    #ifndef NDEBUG
      if(coretypeinfo->typesize < NA_SYSTEM_ADDRESS_BYTES)
        naError("naEnhanceCorePool", "Element is too small");
    #endif
    #ifndef NDEBUG
      if(coretypeinfo->typesize > (na_runtime->poolsize - sizeof(NACorePool)))
        naError("naEnhanceCorePool", "Element is too big");
    #endif

    // We create a new pool.
    corepool = (NACorePool*)naMallocAligned(na_runtime->poolsize, na_runtime->poolsize);
    corepool->coretypeinfo = coretypeinfo;

    #ifndef NDEBUG
      if(((NAUInt)corepool & ~na_runtime->poolsizemask) != 0)
        naError("naEnhanceCorePool", "pool badly aligned");
    #endif

    // Reduce the elemcount by 1 to later set the last entry to NULL
    corepool->usedcount = ((na_runtime->poolsize - sizeof(NACorePool)) / coretypeinfo->typesize) - 1;
    curunused = (void**)(((NAByte*)corepool) + sizeof(NACorePool));
    corepool->firstunused = curunused;
    // Enumerate all positions with the succeeding free position
    while(corepool->usedcount){
      void** nextunused = (void**)((NAByte*)curunused + coretypeinfo->typesize);
      *curunused = nextunused;
      curunused = nextunused;
      corepool->usedcount--;
    }
    // Set the last entry to NULL.
    *curunused = NA_NULL; // note that elemcount has been reduced by 1

    // There is no element ready to be recycled.
    corepool->firstrecycle = NA_NULL;
    
    #ifndef NDEBUG
      corepool->dummy1 = corepool;
      corepool->dummy2 = (NAByte*)((NAUInt)corepool + na_runtime->poolsize - 1);
    #endif

    // Add the new pool after the current pool or se the pool as the first and
    // only pool, if there is none available yet.
    if(coretypeinfo->curpool){
      corepool->prevpool = coretypeinfo->curpool;
      corepool->nextpool = coretypeinfo->curpool->nextpool;
      corepool->prevpool->nextpool = corepool;
      corepool->nextpool->prevpool = corepool;
    }else{
      corepool->prevpool = corepool;
      corepool->nextpool = corepool;
    }
    coretypeinfo->curpool = corepool;
      
    return corepool->firstunused;
  }



  NA_HIDEF void naShrinkCorePool(NACorePool* corepool){
    if(corepool->coretypeinfo->curpool == corepool){corepool->coretypeinfo->curpool = corepool->nextpool;}
    corepool->prevpool->nextpool = corepool->nextpool;
    corepool->nextpool->prevpool = corepool->prevpool;
    naFreeAligned(corepool);
  }

#endif


NA_DEF void* naNewStruct(NATypeInfo* typeinfo){

  void* pointer;
  NACoreTypeInfo* coretypeinfo = (NACoreTypeInfo*)typeinfo;
  
  #ifndef NDEBUG
    if(!na_runtime)
      {naCrash("naNew", "Runtime not running. Use naStartRuntime()"); return NA_NULL;}
    if(!coretypeinfo)
      {naCrash("naNew", "Given type identifier is Null-Pointer"); return NA_NULL;}
  #endif

  #if (NA_RUNTIME_USES_MEMORY_POOLS == 0)

    NACoreTypeInfo** basepointer = naMalloc(sizeof(NACoreTypeInfo*) + coretypeinfo->typesize);
    pointer = basepointer + 1;
    *basepointer = coretypeinfo;
    return pointer;
  
  #else

    if(!coretypeinfo->curpool){
        naEnhanceCorePool(coretypeinfo);
    }
  
    // We get the next unused pointer.
    pointer = coretypeinfo->curpool->firstunused;
    // If none is available, we enhance the pool.
    if(!pointer){pointer = naEnhanceCorePool(coretypeinfo);}
    
    // The current pool can now advance the first-pointer to the next one.
    coretypeinfo->curpool->firstunused = *((void**)coretypeinfo->curpool->firstunused);
    coretypeinfo->curpool->usedcount++;
    
    #ifndef NDEBUG
      if(coretypeinfo->curpool != (NACorePool*)((NAUInt)pointer & na_runtime->poolsizemask))
      naError("naNewStruct", "Pointer seems to be outside of pool");
    #endif
    
    // Now, the pointer points to a space which can be constructed.
    return pointer;
  
  #endif
}



NA_DEF void naDelete(void* pointer){
  NACorePool* corepool; // Declaration before definition. Needed for C90

  #ifndef NDEBUG
    if(!na_runtime)
      {naCrash("naNew", "Runtime not running. Use naStartRuntime()"); return;}
  #endif

  #if (NA_RUNTIME_USES_MEMORY_POOLS == 0)
  
    NACoreTypeInfo** basepointer = (((NACoreTypeInfo**)pointer) - 1);
    if((*basepointer)->destructor){(*basepointer)->destructor(pointer);}
    naFree(basepointer);
  
  #else

    // Find the corepool entry at the beginning of the pool.
    corepool = (NACorePool*)((NAUInt)pointer & na_runtime->poolsizemask);
    #ifndef NDEBUG
      if(corepool->dummy1 != corepool)
        naError("naDelete", "Pointer seems not to be from a pool");
      if(corepool->dummy2 != (NAByte*)((NAUInt)corepool + na_runtime->poolsize - 1))
        naError("naDelete", "Pointer seems not to be from a pool");
    #endif
    // Delete the struct with the destructor
    if(corepool->coretypeinfo->destructor){corepool->coretypeinfo->destructor(pointer);}
    *((void**)pointer) = corepool->firstunused;
    corepool->firstunused = pointer;
    corepool->usedcount--;
    if((!corepool->usedcount) && (corepool->nextpool != corepool)){naShrinkCorePool(corepool);}

  #endif

}




NA_DEF void naStartRuntime(){
  #ifndef NDEBUG
    if(na_runtime)
      naCrash("naStartRuntime", "Runtime already running");
    if(sizeof(NACorePool) != (8 * NA_SYSTEM_ADDRESS_BYTES))
      naError("naStartRuntime", "NACorePool struct encoding misaligned");
  #endif
  na_runtime = naAlloc(NARuntime);
  #if defined NA_COREPOOL_BYTESIZE_EQUALS_PAGESIZE
    na_runtime->poolsize = naGetSystemMemoryPagesize();
    na_runtime->poolsizemask = naGetSystemMemoryPagesizeMask();
  #else
    na_runtime->poolsize = NA_CUSTOM_POOL_BYTESIZE;
    na_runtime->poolsizemask = ~(NAUInt)(NA_CUSTOM_POOL_BYTESIZE - NA_ONE);
  #endif
}





NA_DEF void naStopRuntime(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naStopRuntime", "Runtime not running");
  #endif
  // todo.
  na_runtime = NA_NULL;
}







// This function is not inlined as it must be called by naDelete.
NA_HDEF void naDestructPointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!(pointer->refcount & NA_SMARTPTR_DELETE_FROM_RELEASE))
      naError("naDestructPointer", "You should never call naDelete on a Pointer! Use naPointerRelease.");
  #endif
  if(pointer->destructor){
    pointer->destructor(naGetPtrMutable(&(pointer->ptr)));
  }
  NAMemoryCleanup cleanup = (NAMemoryCleanup)(pointer->refcount >> NA_MEMORY_CLEANUP_DATA_BITSHIFT);
  #ifndef NDEBUG
    if(cleanup < NA_MEMORY_CLEANUP_NONE || cleanup >= NA_MEMORY_CLEANUP_COUNT)
      naError("naDestructPointer", "invalid cleanup option");
  #endif
  switch(cleanup){
  case NA_MEMORY_CLEANUP_UNDEFINED:
    #ifndef NDEBUG
      naError("naDestructPointer", "invalid cleanup option");
    #endif
    break;
  case NA_MEMORY_CLEANUP_NONE:
    break;
  case NA_MEMORY_CLEANUP_FREE:
    naFreePtr(&(pointer->ptr));
    break;
  case NA_MEMORY_CLEANUP_FREE_ALIGNED:
    naFreeAlignedPtr(&(pointer->ptr));
    break;
#ifdef __cplusplus 
  case NA_MEMORY_CLEANUP_DELETE:
    naDeletePtr(pointer->ptr);
    break;
  case NA_MEMORY_CLEANUP_DELETE_BRACK:
    naDeleteBrackPtr(&(pointer->ptr));
    break;
#endif
  case NA_MEMORY_CLEANUP_NA_DELETE:
    naNaDeletePtr(&(pointer->ptr));
    break;
  default:
    // The default case is a security if this file is not compiled for C++.
    #ifndef NDEBUG
      naError("naDestructPointer", "Cleanup option is unavailable in C, use C++");
    #endif
    break;
  }
}




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
