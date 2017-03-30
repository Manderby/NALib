
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "../NAMemory.h"
#include "../NAString.h"
#include "../NAList.h"


// //////////////////////////////////////
// Implementation notes from the author about the Memory Pools.
//
// NALib uses memory allocation pools if configured in NAConfiguration.h
// with NA_RUNTIME_USES_MEMORY_POOLS. Here is an explanation on how it works:
//
// Each type which is marked with NA_RUNTIME_TYPE has a hidden struct called
// NACoreTypeInfo which is only visible to the programmer as NATypeInfo. In
// that struct, basic information about the type are stored like the size in
// bytes or the destructor to be called upon deletion.
//
// Most importantly, NACoreTypeInfo stores a pool of preallocated structs. This
// Pool is a double linked list of memory blocks with a bytesize defined in
// NAConfiguration.h with NA_COREPOOL_BYTESIZE. This size plus a corresponding
// address mask is stored in the global NARuntime struct na_runtime.
//
// Each memory block is allocated aligned to that bytesize and hence AND'ing an
// arbitrary address with the address mask returns the address of the first
// byte of the pool it is stored in.
//
// Each of this block is called a pool whereas the curpool field of the
// NACoreTypeInfo stores the pool which currently is expected to have a free
// struct. The pools are linked to a circular list but are implemented in a
// custom way and not with NAList. Pools at the beginning of the list (next to
// the current pool) are expected to have free space and pools at the end of
// the list (previous to the current pool) are expected to be full.
//
// Within one pool, the first bytes (8 times an addresssize to be precise) are
// reserved for a complete description of what that pool stores using the
// NACorePool struct.
//
// All remaining bytes are used for storing structs, as many as the remaining
// space can store. The NACorePool struct stores the first unused struct as a
// pointer. When naNew is called, that first unused pointer is used as the
// pointer being returned to the user. Before returning the pointer though,
// the next pointer to an unused struct must be identified.
//
// NALib does that by looking at the bytes currently stored at that pointer.
// Initially, there will only be garbage values. NALib keeps track of that by
// counting how many structs have ever been used inside the pool. If the new
// struct has never been used, the next available pointer will simply be one
// typesize ahead.
//
// Otherwise, when that struct has already been used once, it will contain an
// address to the next free struct. This can be ensured upon deletion of the
// struct: When calling naDelete, a struct destroys all of its contents and
// eventually only contains garbage values. When this is done, NALib puts the
// address of the next free pointer at the first bytes of the deleted struct
// and sets the next unused pointer of the NACorePool to this just deleted
// struct, hence ultimately creating a linear list. With this mechanism, the
// NACorePool can always keep track of which structs are still available.
//
// When finally the number of structs used in a pool equals the maximum number
// of structs, the pool has no more space. Then, the next pool in the list is
// selected in hope to find free structs there. If the next pool also has no
// space, a new pool needs to be created. The new pool gets added to the
// circular pool list and is used as the current pool.
//
// When deleting a struct from a completely full pool, that pool will get moved
// in the list right after the current pool hence making the pool available for
// new allocations as soon as possible.
//
// Upon deleting structs, a pool may become completely empty. If that is the
// case, the pool gets automatically deallocated, hence freeing all of the
// memory.


typedef struct NACorePool NACorePool;
typedef struct NACoreTypeInfo NACoreTypeInfo;
typedef struct NARuntime NARuntime;


struct NACoreTypeInfo{
  NACorePool*       curpool;
  NAUInt            typesize;
  NAMutator         destructor;
};


struct NACorePool{
  NACoreTypeInfo* coretypeinfo;
  NAUInt maxcount;
  NAUInt usedcount;
  NAUInt everusedcount;
  void* firstunused;
  NACorePool* prevpool;
  NACorePool* nextpool;
  // The following two field is a dummy entriy not used in release versions.
  // It shall not be removed though as the total amount of bytes used for
  // an NACorePool shall be 8 times an addresssize.
  void* dummy; // used in debugging. Points at first byte of the whole pool
};

struct NARuntime{
  NAUInt mempagesize;
  NAUInt poolsize;
  NAUInt poolsizemask;
};


NARuntime* na_runtime = NA_NULL;


#if (NA_RUNTIME_USES_MEMORY_POOLS == 1)

  // Security check: The pool bytesize must be big enough to store one struct
  // of NACorePool.
  #if (NA_COREPOOL_BYTESIZE != 0) && (NA_COREPOOL_BYTESIZE <= 8 * NA_SYSTEM_ADDRESS_BYTES)
    #error "Core memory pool size is too small"
  #endif



  NA_HIDEF NABool naIsPoolFull(NACorePool* corepool){
    return (corepool->usedcount == corepool->maxcount);
  }



  NA_HIDEF void naAttachPoolAfterCurrentPool(NACoreTypeInfo* coretypeinfo, NACorePool* corepool){
    corepool->prevpool = coretypeinfo->curpool;
    corepool->nextpool = coretypeinfo->curpool->nextpool;
    corepool->prevpool->nextpool = corepool;
    corepool->nextpool->prevpool = corepool;
  }



  // This function gets called when no pool has any more space.
  // A new pool is created and added to the pool list at the current position.
  NA_HIDEF void naEnhanceCorePool(NACoreTypeInfo* coretypeinfo){
    NACorePool* corepool;
    
    #ifndef NDEBUG
      if(coretypeinfo->typesize < NA_SYSTEM_ADDRESS_BYTES)
        naError("naEnhanceCorePool", "Element is too small");
    #endif
    #ifndef NDEBUG
      if(coretypeinfo->typesize > (na_runtime->poolsize - sizeof(NACorePool)))
        naError("naEnhanceCorePool", "Element is too big for core memory pool size");
    #endif

    // We create a new pool which the size of a full pool but we type it as
    // NACorePool to access the first bytes.
    corepool = (NACorePool*)naMallocAligned(na_runtime->poolsize, na_runtime->poolsize);
    #ifndef NDEBUG
      if(((NAUInt)corepool & ~na_runtime->poolsizemask) != 0)
        naError("naEnhanceCorePool", "pool badly aligned");
    #endif
    
    // We store a pointer to the coretypeinfo.
    corepool->coretypeinfo = coretypeinfo;

    // We initialize the basic fields of corepool.
    corepool->maxcount = ((na_runtime->poolsize - sizeof(NACorePool)) / coretypeinfo->typesize);
    corepool->usedcount = 0;
    corepool->everusedcount = 0;
    corepool->firstunused = (void**)(((NAByte*)corepool) + sizeof(NACorePool));
    #ifndef NDEBUG
      corepool->dummy = corepool;
    #endif

    // Implementation Note from the author: Originally, the everusedcount field
    // did not exists and the whole memory block needed to be intialized either
    // with binary zero or with a precomputation step storing all next free
    // addresses. But this turned out to use quite some computing power and
    // hence this new method with the everusedcount was created. Way better now.

    // Add the new pool after the current pool or set the pool as the first and
    // only pool, if there is none available yet.
    if(coretypeinfo->curpool){
      naAttachPoolAfterCurrentPool(coretypeinfo, corepool);
    }else{
      corepool->prevpool = corepool;
      corepool->nextpool = corepool;
    }
    coretypeinfo->curpool = corepool;
  }



  // This function gets called when all structs of a pool have been deleted.
  // Note that if the whole pool list only contains one pool, this function is
  // not called.
  NA_HIDEF void naShrinkCorePool(NACorePool* corepool){
    // We move to the next pool if the pool is set as the current pool.
    if(corepool->coretypeinfo->curpool == corepool){corepool->coretypeinfo->curpool = corepool->nextpool;}
    // We unlink the pool from the list.
    corepool->prevpool->nextpool = corepool->nextpool;
    corepool->nextpool->prevpool = corepool->prevpool;
    // And delete its memory.
    naFreeAligned(corepool);
  }

#endif



NA_DEF void* naNewStruct(NATypeInfo* typeinfo){

  void* pointer;
  NACoreTypeInfo* coretypeinfo;
  
  #ifndef NDEBUG
    if(!na_runtime)
      {naCrash("naNew", "Runtime not running. Use naStartRuntime()"); return NA_NULL;}
    if(!typeinfo)
      {naCrash("naNew", "Given type identifier is Null-Pointer"); return NA_NULL;}
  #endif

  coretypeinfo = (NACoreTypeInfo*)typeinfo;

  #if (NA_RUNTIME_USES_MEMORY_POOLS == 0)

    // When not using memory pools, we allocate enough bytes for the struct
    // plus an additional address size before the actual struct to store a
    // pointer to the coretypeinfo which is needed when deleting the struct.
    
    NACoreTypeInfo** basepointer = naMalloc(sizeof(NACoreTypeInfo*) + coretypeinfo->typesize);
    pointer = basepointer + 1;
    *basepointer = coretypeinfo;
    return pointer;
  
  #else

    // If there is no current pool, create a first one.
    if(!coretypeinfo->curpool){naEnhanceCorePool(coretypeinfo);}
  
    // If the current pool is full, we try the next in the pool list.
    if(naIsPoolFull(coretypeinfo->curpool)){
      coretypeinfo->curpool = coretypeinfo->curpool->nextpool;
      // If the next in the pool list is full too, no pool in the list has any
      // space left and hence we must create a new pool.
      if(naIsPoolFull(coretypeinfo->curpool)){naEnhanceCorePool(coretypeinfo);}
    }
    // Now, we can be sure that the current pool has space.
  
    // We get the next unused pointer.
    pointer = coretypeinfo->curpool->firstunused;
    
    if(coretypeinfo->curpool->usedcount == coretypeinfo->curpool->everusedcount){
      // The current struct has not been used ever. Use the next address one
      // typesize ahead.
      coretypeinfo->curpool->firstunused = (NAByte*)(coretypeinfo->curpool->firstunused) + coretypeinfo->typesize;
      coretypeinfo->curpool->everusedcount++;
    }else{
      // The struct has already been used and stores a pointer to the next
      // unused.
      coretypeinfo->curpool->firstunused = *((void**)coretypeinfo->curpool->firstunused);
    }
    
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
  #if (NA_RUNTIME_USES_MEMORY_POOLS != 0)
    NACorePool* corepool;
  #endif

  #ifndef NDEBUG
    if(!na_runtime)
      {naCrash("naNew", "Runtime not running. Use naStartRuntime()"); return;}
  #endif

  #if (NA_RUNTIME_USES_MEMORY_POOLS == 0)
  
    // If no memory pools are used, we look at the coretypeinfo which is stored
    // right before the given pointer, call the appropriate destructor and
    // free the whole memory block.
    
    NACoreTypeInfo** basepointer = (((NACoreTypeInfo**)pointer) - 1);
    if((*basepointer)->destructor){(*basepointer)->destructor(pointer);}
    naFree(basepointer);
  
  #else

    // Find the corepool entry at the beginning of the pool by AND'ing the
    // address with the poolsizemask
    corepool = (NACorePool*)((NAUInt)pointer & na_runtime->poolsizemask);
    #ifndef NDEBUG
      if(corepool->dummy != corepool)
        naError("naDelete", "Pointer seems not to be from a pool. Test case 0");
    #endif
    
    // Delete the struct with the destructor
    if(corepool->coretypeinfo->destructor){corepool->coretypeinfo->destructor(pointer);}
    // Now, the memory at pointer is expected to be garbage.
    
    // We explicitely store a pointer to the next unused struct at that
    // position, ultimately creating a list.
    *((void**)pointer) = corepool->firstunused;
    corepool->firstunused = pointer;
    
    // If the pool was full up until now, we reattach it in the list such that
    // it comes one after the current pool. But only if there are more than one
    // pool around and the current pool of the coretypeinfo is not already that
    // pool.
    if((corepool->usedcount == corepool->maxcount) && (corepool->nextpool != corepool) && (corepool->coretypeinfo->curpool != corepool)){
      corepool->nextpool->prevpool = corepool->prevpool;
      corepool->prevpool->nextpool = corepool->nextpool;
      naAttachPoolAfterCurrentPool(corepool->coretypeinfo, corepool);
    }

    // We reduce the number of used structs in this pool. 
    corepool->usedcount--;
        
    // If no more structs are in use, we can shrink that core pool away. We do
    // not shrink away the last pool though.
    if((!corepool->usedcount) && (corepool->nextpool != corepool)){naShrinkCorePool(corepool);}

  #endif

}



// Note that the runtime system in NALib currently is rather small. But it may
// serve a greater purpose in a later version.
NA_DEF void naStartRuntime(){
  #ifndef NDEBUG
    if(na_runtime)
      naCrash("naStartRuntime", "Runtime already running");
    if(sizeof(NACorePool) != (8 * NA_SYSTEM_ADDRESS_BYTES))
      naError("naStartRuntime", "NACorePool struct encoding misaligned");
  #endif
  na_runtime = naAlloc(NARuntime);
  na_runtime->mempagesize = naGetSystemMemoryPagesize();
  #if (NA_COREPOOL_BYTESIZE == 0)
    na_runtime->poolsize = naGetSystemMemoryPagesize();
    na_runtime->poolsizemask = naGetSystemMemoryPagesizeMask();
  #else
    na_runtime->poolsize = NA_COREPOOL_BYTESIZE;
    na_runtime->poolsizemask = ~(NAUInt)(NA_COREPOOL_BYTESIZE - NA_ONE);
  #endif
}



NA_DEF void naStopRuntime(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naStopRuntime", "Runtime not running");
  #endif
  naFree(na_runtime);
  na_runtime = NA_NULL;
}



NA_DEF NAUInt naGetRuntimeMemoryPageSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      {naCrash("naGetRuntimeMemoryPageSize", "Runtime not running"); return 0;}
  #endif
  return na_runtime->mempagesize;
}



NA_DEF NAUInt naGetRuntimePoolSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      {naCrash("naGetRuntimePoolSize", "Runtime not running"); return 0;}
  #endif
  return na_runtime->poolsize;
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
