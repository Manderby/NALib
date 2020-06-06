
#include "../../NAMemory.h"
#include "../../NABinaryData.h"

#ifndef NDEBUG
  #include "stdio.h"
#endif

// //////////////////////////////////////
// Implementation notes from the author about the Memory Pools.
//
// NALib uses memory allocation pools for types allocated with the runtime
// system. Here is an explanation on how it works:
//
// Each type which is marked with NA_RUNTIME_TYPE has a hidden struct called
// NA_TypeInfo which is only visible to the programmer as NATypeInfo. In
// that struct, basic information about the type are stored like the size in
// bytes or the destructor to be called upon deletion.
//
// Most importantly, NA_TypeInfo stores a pointer "curpart". This
// pointer points to an allocated memory block with a bytesize defined in
// NAConfiguration.h with NA_POOLPART_BYTESIZE. That size and a corresponding
// address mask is stored in the global NARuntime struct na_Runtime.
//
// Each of these memory blocks is allocated aligned to NA_POOLPART_BYTESIZE and
// hence AND'ing an arbitrary address within that block with the address mask
// returns the address of the first byte of the memory block.
//
// At the first bytes of each of these memory blocks, a NA_PoolPart struct
// is embedded. Therefore addressing the first byte of such a memory block is
// equivalent to accessing its NA_PoolPart. All NA_PoolParts of all the
// memory blocks of one runtime type form together a cyclic double linked list
// with a prev and next pointer. (Note: This does not uses NAList but is a
// custom implementation)
//
// The curpart field of the NA_TypeInfo points to the part which is
// expected to have a free space for the desired runtime type. From there on,
// parts going forward (next to curpart) are expected to have free space
// and partss going backward (prev from curpart) are expected to be full.
//
// While the first bytes of a memory block are filled with the contents of
// NA_PoolPart, the remaining bytes are used for storing the actual values,
// as many as the remaining space can store. The NA_PoolPart struct stores
// the first unused space as a pointer. When naNew is called, that pointer is
// used as the pointer being returned to the user which called naNew. Before
// returning the pointer though, the next pointer to an unused space must be
// identified.
//
// NALib does that by looking at the bytes currently stored at that pointer.
// Initially, there will only be garbage values. NALib keeps track of that by
// counting how many spaces have ever been used inside the part. If the new
// space has never been used, the next available pointer will simply be one
// typeSize ahead. Note from the author: This little trick made the current
// implementation really fast!
//
// Otherwise, when that space has already been used once and has been deleted
// in the meantime, it will now contain an address to the next free space.
// This can be ensured upon deletion of the previos space: After calling the
// optional destructor, the values stored at that space become garbage values.
// Now, NALib puts the address of the next free space at the first bytes of the
// deleted space and sets the next-unused-pointer of the NA_PoolPart to this
// just deleted space, hence ultimately creating a linear list. With this
// mechanism, the NA_PoolPart can always keep track of which spacess are
// still available.
//
// When finally the number of spaces used in a part equals the maximum number
// of spaces, the part has no more space. Then, the next part in the list is
// selected in hope to find a free space there. If the next part also has no
// space, a new part needs to be created. The new part gets added to the
// circular part list and is used as the current part.
//
// When deleting a space from a completely full part, that part will get moved
// in the list right after the current part hence making the part available for
// new allocations as soon as the current part is filled.
//
// Upon deleting spaces, a part may become completely empty. If that is the
// case, the part gets automatically deallocated, hence freeing all of the
// memory.

// /////////////
// Garbage collection:
//
// NALib also has a small garbage collection mechanism: If you just need a
// temporary pointer which shall automatically be freed, use naMallocTmp.
// This will return a pointer which is also stored in a garbage pool. That
// pool is a simple, single-linked list of memory blocks the same size as
// the memory blocks above, but simply storing arrays of temporarily allocated
// pointers.
//
// When you call naCollectGarbage or if you stop the Runtime, that memory
// will be completely erased.



// This structure is stored in the first bytes of every part block.
// Its size is ALWAYS 8 times an addresssize, the rest of the block is
// available for the memory.
typedef struct NA_PoolPart NA_PoolPart;
struct NA_PoolPart{
  NA_TypeInfo* typeInfo;
  size_t maxCount;
  size_t usedCount;
  size_t everUsedCount;
  void* firstunused;
  NA_PoolPart* prevpart;
  NA_PoolPart* nextpart;
  // The following field is a dummy entry not in use.
  // It shall not be removed though as the total amount of bytes used for
  // an NA_PoolPart shall be 8 times an addresssize.
  void* dummy; // used in debugging. Points at first byte of the whole pool
               // for consistency check.
};

struct NA_TypeInfo{
  NA_PoolPart*   curpart;
  size_t            typeSize;
  NAMutator         destructor;
  NABool            refCounting;
  #ifndef NDEBUG
    const char*     typeName;
  #endif
};

#define NA_MALLOC_GARBAGE_POINTER_COUNT (NA_POOLPART_BYTESIZE / NA_ADDRESS_BYTES - 2)
// The 2 denotes the first two entries in this struct.

struct NAMallocGarbage{
  NAMallocGarbage* next;
  size_t cur;
  void* pointers[NA_MALLOC_GARBAGE_POINTER_COUNT];
};



// The global runtime variable.
NARuntime* na_Runtime = NA_NULL;



// Security check: The pool bytesize must be big enough to store one struct
// of NA_PoolPart. Note that bytesize 0 has the meaning of using the
// memory page size.
#if (NA_POOLPART_BYTESIZE != 0) && (NA_POOLPART_BYTESIZE <= 8 * NA_ADDRESS_BYTES)
  #error "Memory pool size is too small"
#endif



// Registers a runtime type. Adds the typeInfo to the typeInfos found
// in na_Runtime. Note that when aggressive cleanup is turned on, any type
// which had been registered previously had already been unregistered.
NA_HIDEF void naRegisterTypeInfo(NA_TypeInfo* typeInfo){
  NA_TypeInfo** newinfos;

  #ifndef NDEBUG
    if(typeInfo->curpart)
      naError("Newly registered type should have NULL as current part.");
    if(typeInfo->typeSize < NA_ADDRESS_BYTES)
      naError("Size of type is too small");
    if(typeInfo->typeSize > (na_Runtime->partSize - sizeof(NA_PoolPart)))
      naError("Size of type is too big");
  #endif

  // As this is the first time, the runtime type gets used, we correct the
  // typeSize to incorporate reference counting, if any.
  if(typeInfo->refCounting){typeInfo->typeSize += sizeof(NARefCount);}

  // We enlarge the na_Runtime info array by one. Yes, this is very bad
  // performance, but this code is usually called rather seldomly. If you
  // experience here a lot of memory allocations, you might want to check if
  // NA_MEMORY_POOL_AGGRESSIVE_CLEANUP is set to 1 and set it back to 0.
  newinfos = naMalloc(naSizeof(NA_TypeInfo*) * (na_Runtime->typeInfoCount + NA_ONE));

  // We copy all previous infos to the newly allocated memory block.
  if(na_Runtime->typeInfos){
    naCopyn(newinfos, na_Runtime->typeInfos, naSizeof(NA_TypeInfo*) * na_Runtime->typeInfoCount);
  }

  // We add the new typeinfo as a pointer to the na_Runtime infos.
  newinfos[na_Runtime->typeInfoCount] = typeInfo;
  na_Runtime->typeInfoCount++;

  // We cleanup the old infos and set the new ones to be valid.
  naFree(na_Runtime->typeInfos);
  na_Runtime->typeInfos = newinfos;
}



NA_HIDEF void naUnregisterTypeInfo(NA_TypeInfo* typeInfo){
  NARuntime* runtime = na_Runtime;
  NA_TypeInfo** newinfos = NA_NULL;
  if(na_Runtime->typeInfoCount > 1){
    NAInt i;
    NAInt oldindex = 0;
    newinfos = naMalloc(naSizeof(NA_TypeInfo*) * (na_Runtime->typeInfoCount - NA_ONE));

    // We shrink the info array by one by omitting the one entry which equals
    // the given parameter. Again, just like naRegisterTypeInfo, this is not
    // very fast, but does the job. See comment there.
    for(i = 0; i < (na_Runtime->typeInfoCount - NA_ONE); i++){
      if(na_Runtime->typeInfos[i] == typeInfo){oldindex++;}
      newinfos[i] = na_Runtime->typeInfos[oldindex];
      oldindex++;
    }
  }

  na_Runtime->typeInfoCount--;

  // We cleanup the old infos and set the new ones to be valid.
  naFree(na_Runtime->typeInfos);
  na_Runtime->typeInfos = newinfos;

  // We restore the original typeSize in case NA_MEMORY_POOL_AGGRESSIVE_CLEANUP
  // is set to 1 which means, the type might get re-registered.
  if(typeInfo->refCounting){typeInfo->typeSize -= sizeof(NARefCount);}
}



NA_HDEF size_t naGetTypeInfoAllocatedCount(NA_TypeInfo* typeInfo){
  NA_PoolPart* firstpart = typeInfo->curpart;
  NA_PoolPart* curpart = firstpart->nextpart;
  size_t totalcount = firstpart->usedCount;
  while(curpart != firstpart){
    totalcount += curpart->usedCount;
    curpart = curpart->nextpart;
  }
  return totalcount;
}



NA_HIDEF NABool naIsPoolPartFull(NA_PoolPart* part){
  return (part->usedCount == part->maxCount);
}



NA_HIDEF void naAttachPoolPartAfterCurPoolPart(NA_TypeInfo* typeInfo, NA_PoolPart* part){
  part->prevpart = typeInfo->curpart;
  part->nextpart = typeInfo->curpart->nextpart;
  part->prevpart->nextpart = part;
  part->nextpart->prevpart = part;
}



// This function gets called when no part has any more space.
// A new part is created and added to the list at the current position.
NA_HIDEF void naEnhancePool(NA_TypeInfo* typeInfo){
  NA_PoolPart* part;

  // We create a new part with the size of a full part but we type it as
  // NA_PoolPart to access the first bytes.
  part = (NA_PoolPart*)naMallocAligned((NAUInt)na_Runtime->partSize, (NAUInt)na_Runtime->partSize);
  #ifndef NDEBUG
    // Do you think the following check is not necessary? You'd be surprised
    // how many systems do not align memory correctly!
    if(((size_t)part & ~na_Runtime->partSizeMask) != 0)
      naError("pool part badly aligned");
  #endif

  // We initialize the basic fields of part.
  part->typeInfo = typeInfo;
  part->maxCount = ((na_Runtime->partSize - sizeof(NA_PoolPart)) / typeInfo->typeSize);
  part->usedCount = 0;
  part->everUsedCount = 0;

  // Implementation note from the author: In earlier versions, everUsedCount
  // did not exists and the whole memory block needed to be intialized either
  // with binary zero or with a precomputation step storing all next free
  // addresses. But this turned out to use quite some computing power and
  // hence this new method with the everUsedCount was created. Way better now.

  // We set the pointer to the first available space to the first byte right
  // after the NA_PoolPart.
  part->firstunused = (void*)(((NAByte*)part) + sizeof(NA_PoolPart));

  // If we are in debug mode, we also set the dummy variable for a consistency
  // check.
  #ifndef NDEBUG
    part->dummy = part;
  #endif

  // Add the new part after the current part or set the part as the first and
  // only part, if there is none available yet.
  if(typeInfo->curpart){
    naAttachPoolPartAfterCurPoolPart(typeInfo, part);
  }else{
    part->prevpart = part;
    part->nextpart = part;
  }

  // Set the newly created part to be the current part.
  typeInfo->curpart = part;
}



NA_DEF void* naNewStruct(NATypeInfo* typeinfo){
  void* pointer;
  NA_TypeInfo* typeInfo;

  #ifndef NDEBUG
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!typeinfo)
      naCrash("Given type identifier is Null-Pointer. Do not call naNewStruct directly. Use the naNew macro.");
    if(typeinfo->typeSize == 0)
      naError("Type size is zero. Is the type void?");
  #endif

  typeInfo = (NA_TypeInfo*)typeinfo;

  // If there is no current part, create a first one.
  // This happends either upon first naNew of this type ever or when aggressive
  // memory cleanup is activated. See Configuration.h
  if(!typeInfo->curpart){
    // As this is the first one, we register the type to the runtime system.
    naRegisterTypeInfo(typeInfo);
    naEnhancePool(typeInfo);
    #ifndef NDEBUG
      if(!typeInfo->curpart)
        naCrash("No part available even after enhancing.");
    #endif
  }

  // If the current part is full, we try the next in the part list.
  if(naIsPoolPartFull(typeInfo->curpart)){
    typeInfo->curpart = typeInfo->curpart->nextpart;
    // If the next in the part list is full too, no part in the list has any
    // space left and hence we must create a new part.
    if(naIsPoolPartFull(typeInfo->curpart)){naEnhancePool(typeInfo);}
  }

  // Now, we can be sure that the current part has space.
  #ifndef NDEBUG
    if(naIsPoolPartFull(typeInfo->curpart))
      naCrash("Still no space after creating new space.");
  #endif

  // We get the pointer to the first currently unused space.
  pointer = typeInfo->curpart->firstunused;

  // We find out which will be the next pointer to return.
  if(typeInfo->curpart->usedCount == typeInfo->curpart->everUsedCount){
    // The current space has not been used ever. Use the next address one
    // typeSize ahead.
    typeInfo->curpart->firstunused = (NAByte*)(typeInfo->curpart->firstunused) + typeInfo->typeSize;
    // Increase the number of ever used spaces in this part.
    typeInfo->curpart->everUsedCount++;
  }else{
    // The space has already been used and deleted before which means, it
    // stores now a pointer to the next unused space.
    typeInfo->curpart->firstunused = *((void**)typeInfo->curpart->firstunused);
  }

  // Increase the number of spaces used in this part.
  typeInfo->curpart->usedCount++;

  #ifndef NDEBUG
    #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
      naError("No native integer type to successfully run the runtime system.");
    #else
      if(typeInfo->curpart != (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask))
        naError("Pointer seems to be outside of part");
    #endif
  #endif

  // Now, the pointer points to a space which can be constructed. Initialize
  // the refCounter if applicable and return the pointer.
  if(typeInfo->refCounting){
    naInitRefCount(pointer);
    return (void*)((NAByte*)pointer + sizeof(NARefCount));
  }else{
    return pointer;
  }
}



NA_HIDEF void naEjectPoolPartObject(NA_PoolPart* part, void* pointer){
  // The memory at pointer is expected to be erased and hence garbage.

  // We explicitely store a pointer to the next unused space at that
  // position, ultimately creating a list.
  *((void**)pointer) = part->firstunused;
  part->firstunused = pointer;

  // If the part was full up until now, we reattach it in the list such that
  // it comes one after the current part. But only if there are more than one
  // parts around and the current part of the typeInfo is not already the
  // current part.
  if((part->usedCount == part->maxCount) && (part->nextpart != part) && (part->typeInfo->curpart != part)){
    part->nextpart->prevpart = part->prevpart;
    part->prevpart->nextpart = part->nextpart;
    naAttachPoolPartAfterCurPoolPart(part->typeInfo, part);
  }

  // We reduce the number of used spaces in this part.
  part->usedCount--;

  // If no more spaces are in use, we can shrink that part away.
  if(!part->usedCount){
    if(part->nextpart == part){
      #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 1
        // If this part is the last part of the pool and the cleanup is set to
        // aggressive, we shrink it away and unregister the type.
        NA_TypeInfo* typeInfo = part->typeInfo;
        naFreeAligned(part);
        typeInfo->curpart = NA_NULL;
        naUnregisterTypeInfo(typeInfo);
      #endif
    }else{
      // There are other parts in the pool. If the empty part is the one which
      // is the current part of the pool, we move to the next part.
      if(part->typeInfo->curpart == part){
        part->typeInfo->curpart = part->nextpart;
      }
      // We unlink the part from the list.
      part->prevpart->nextpart = part->nextpart;
      part->nextpart->prevpart = part->prevpart;
      // And delete its memory.
      naFreeAligned(part);
    }
  }
}



NA_DEF void naDelete(void* pointer){
  NA_PoolPart* part;

  #ifndef NDEBUG
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif

  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    NA_UNUSED(part);
    NA_UNUSED(pointer);
  #else

    // Find the part entry at the beginning of the part by AND'ing the
    // address with the partSizeMask
    part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);

    #ifndef NDEBUG
      if(part->dummy != part)
        naError("Pointer seems not to be from a pool.");
      if(part->typeInfo->refCounting)
        naError("Pointer belongs to a reference-counting entity. Use naRelease instead of naDelete!");
    #endif

    // Erase the content of the space with the destructor if applicable
    if(part->typeInfo->destructor){part->typeInfo->destructor(pointer);}

    naEjectPoolPartObject(part, pointer);

  #endif
}



NA_DEF void* naRetain(void* pointer){
  NARefCount* refCount;

  #ifndef NDEBUG
    NA_PoolPart* part;
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naCrash("pointer is null");

    // Find the part entry at the beginning of the part by AND'ing the
    // address with the partSizeMask
    #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
      naError("No native integer type to successfully run the runtime system.");
      NA_UNUSED(part);
    #else
      part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);
      if(part->dummy != part)
        naError("Pointer seems not to be from a pool.");
      if(!part->typeInfo->refCounting)
        naError("Pointer belongs to a NON-reference-counting entity. You can't use naRetain!");
    #endif
  #endif

  // Retain the refCounter.
  refCount = (NARefCount*)((NAByte*)pointer - sizeof(NARefCount));
  naRetainRefCount(refCount);
  return pointer;
}



NA_DEF void naRelease(void* pointer){
  NA_PoolPart* part;
  NARefCount* refCount;

  #ifndef NDEBUG
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naCrash("pointer is null");
  #endif

  // Find the corepool entry at the beginning of the part by AND'ing the
  // address with the partSizeMask
  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    NA_UNUSED(part);
    NA_UNUSED(pointer);
    NA_UNUSED(refCount);
    #ifndef NDEBUG
      naError("No native integer type to successfully run the runtime system.");
    #endif
  #else

    part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);

    #ifndef NDEBUG
      if(part->dummy != part)
        naError("Pointer seems not to be from a pool.");
      if(!part->typeInfo->refCounting)
        naError("Pointer belongs to a NON-reference-counting entity. Use naDelete instead of naRelease!");
    #endif

    // Release the space and delete it with the destructor if refCount is zero.
    refCount = (NARefCount*)((NAByte*)pointer - sizeof(NARefCount));
    naReleaseRefCount(refCount, pointer, part->typeInfo->destructor);
    // Note: The following test could also be achieved by using a special
    // mutator function in the previous naReleaseRefCount call. But this would
    // always cause a function call, even for types without a destructor.
    // Therefore, we do this here:
    if(!naGetRefCountCount(refCount)){
      naEjectPoolPartObject(part, refCount);
    }

  #endif
}



NA_DEF void naStartRuntime(){
  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    #ifndef NDEBUG
      naError("Unable to start runtime on system where no native int is able to store an address.");
    #endif
  #else

    #ifndef NDEBUG
      if(na_Runtime)
        naCrash("Runtime already running");
      if(sizeof(NA_PoolPart) != (8 * NA_ADDRESS_BYTES))
        naError("NA_PoolPart struct encoding misaligned");
    #endif
    na_Runtime = naAlloc(NARuntime);
    na_Runtime->memPageSize = naGetSystemMemoryPagesize();
    #if (NA_POOLPART_BYTESIZE == 0)
      na_Runtime->partSize = naGetSystemMemoryPagesize();
      na_Runtime->partSizeMask = naGetSystemMemoryPagesizeMask();
    #elif (NA_POOLPART_BYTESIZE < NA_MAX_i32)
      na_Runtime->partSize = (size_t)NA_POOLPART_BYTESIZE;
      na_Runtime->partSizeMask = ~(size_t)(NA_POOLPART_BYTESIZE - NA_ONE);
    #else
      #error "Pool part bytesize is too large"
    #endif
    na_Runtime->mallocGarbage = NA_NULL;
    na_Runtime->totalMallocGarbageByteCount = 0;
    na_Runtime->typeInfoCount = 0;
    na_Runtime->typeInfos = NA_NULL;
  #endif
}



NA_DEF void naStopRuntime(){
  #ifndef NDEBUG
    NAInt i;
    NABool leakmessageprinted;
  #endif

  // First, we collect the garbage
  naCollectGarbage();
  #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 0
    if(na_Runtime->mallocGarbage){
      naFree(na_Runtime->mallocGarbage);
      na_Runtime->mallocGarbage = NA_NULL;
    }
  #endif

  // Then, we detect, if there are any memory leaks.
  #ifndef NDEBUG
    leakmessageprinted = NA_FALSE;
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");

    // Go through all registered types and output a leak message if necessary.
    for(i = 0; i < na_Runtime->typeInfoCount; i++){
      size_t spacecount = naGetTypeInfoAllocatedCount(na_Runtime->typeInfos[i]);
      #ifndef NDEBUG
        if(spacecount){
          if(!leakmessageprinted){
            printf(NA_NL "Memory leaks detected in NARuntime:" NA_NL);
            leakmessageprinted = NA_TRUE;
          }
          printf("%s: %zu * %zu = %zu Bytes" NA_NL, na_Runtime->typeInfos[i]->typeName, spacecount, na_Runtime->typeInfos[i]->typeSize, spacecount * na_Runtime->typeInfos[i]->typeSize);
        }
      #endif
    }
  #endif

  // Go through all remaining registered types and completely erase them
  // from memory.
  while(na_Runtime->typeInfos){
    NA_PoolPart* firstpart;
    NA_PoolPart* curpart;
    NA_PoolPart* nextpart;

    // Free all parts.
    firstpart = na_Runtime->typeInfos[0]->curpart;
    curpart = firstpart;
    while(curpart){
      nextpart = curpart->nextpart;
      naFreeAligned(curpart);
      if(nextpart == firstpart){break;}
      curpart = nextpart;
    }

    // Finally, unregister the type.
    na_Runtime->typeInfos[0]->curpart = NA_NULL;
    naUnregisterTypeInfo(na_Runtime->typeInfos[0]);
  }

  naFree(na_Runtime);
  na_Runtime = NA_NULL;
}



NA_HIDEF void naEnhanceMallocGarbage(){
  NAMallocGarbage* newgarbage = naAlloc(NAMallocGarbage);
  #ifndef NDEBUG
    if(!newgarbage)
	    naCrash("No garbage memory allocated");
  #endif
  newgarbage->next = na_Runtime->mallocGarbage;
  newgarbage->cur = 0;
  na_Runtime->mallocGarbage = newgarbage;
}



NA_DEF void* naMallocTmp(size_t bytesize){
  void* newPtr;
  NAMallocGarbage* garbage;
  #ifndef NDEBUG
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  #if NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT != 0
    if(na_Runtime->totalMallocGarbageByteCount > (size_t)NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT){naCollectGarbage();}
  #endif
  na_Runtime->totalMallocGarbageByteCount += bytesize;
  newPtr = naMalloc((NAInt)bytesize);
  if(!na_Runtime->mallocGarbage || (na_Runtime->mallocGarbage->cur >= NA_MALLOC_GARBAGE_POINTER_COUNT)){
    naEnhanceMallocGarbage();
  }
  #ifndef NDEBUG
    if(!na_Runtime->mallocGarbage)
      naCrash("Garbage struct is null");
    if(na_Runtime->mallocGarbage->cur >= NA_MALLOC_GARBAGE_POINTER_COUNT)
      naCrash("Buffer overrun.");
  #endif
  garbage = na_Runtime->mallocGarbage;
  garbage->pointers[garbage->cur] = newPtr;
  garbage->cur++;
  return newPtr;
}



NA_DEF void naCollectGarbage(){
  #ifndef NDEBUG
    if(!na_Runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  while(na_Runtime->mallocGarbage){
    NAUInt i;
    NAMallocGarbage* nextgarbage;
    void** ptr = na_Runtime->mallocGarbage->pointers;
    for(i = 0; i < na_Runtime->mallocGarbage->cur; i++){
      naFree(*ptr);
      ptr++;
    }
    nextgarbage = na_Runtime->mallocGarbage->next;

    // If this was the last part, we decide if we want to delete it depending
    // on the aggressive setting.
    #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 1
      naFree(na_Runtime->mallocGarbage);
      na_Runtime->mallocGarbage = nextgarbage;
    #else
      if(nextgarbage){
        naFree(na_Runtime->mallocGarbage);
        na_Runtime->mallocGarbage = nextgarbage;
      }else{
        na_Runtime->mallocGarbage->cur = 0;
        break;
      }
    #endif

  }
  na_Runtime->totalMallocGarbageByteCount = 0;
}




// This is the lonely implimentation of the destruction function of a
// NAPointer. Needed a place to exists in a .c file so we gave the homeless
// a place here.
NA_HHDEF void na_DestructPointer(NAPointer* pointer){
  if(pointer->destructor){
    pointer->destructor(naGetSmartPtrMutable(&(pointer->sptr)));
  }
  naDelete(pointer);
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
