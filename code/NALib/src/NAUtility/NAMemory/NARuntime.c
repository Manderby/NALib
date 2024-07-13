
#include "../NAMemory.h"
#include "../NABinaryData.h"

#if NA_DEBUG
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
// Most importantly, NA_TypeInfo stores a pointer "curPart". This
// pointer points to an allocated memory block with a byteSize defined in
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
// The curPart field of the NA_TypeInfo points to the part which is
// expected to have a free space for the desired runtime type. From there on,
// parts going forward (next to curPart) are expected to have free space
// and partss going backward (prev from curPart) are expected to be full.
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
// typeSize ahead.
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
// Note from the author: This little trick made the current implementation
// really fast!
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
// Its size is ALWAYS 8 times an addressSize, the rest of the block is
// available for the memory.
typedef struct NA_PoolPart NA_PoolPart;
struct NA_PoolPart{
  NA_TypeInfo* typeInfo;
  size_t maxCount;
  size_t usedCount;
  size_t everUsedCount;
  void* firstUnused;
  NA_PoolPart* prevPart;
  NA_PoolPart* nextPart;
  // The following field is a dummy entry and has no meaning in release code.
  // It shall not be removed though as the total amount of bytes used for
  // an NA_PoolPart shall be 8 times an addressSize.
  // It is used in debugging though. Points at first byte of the whole pool
  // for consistency check.
  void* dummy;
};

struct NA_TypeInfo{
  NA_PoolPart*      curPart;
  size_t            typeSize;
  NAMutator         destructor;
  NABool            refCounting;
  #if NA_DEBUG
    const char*     typeName;
  #endif
};



// The number of entries in a garbage pointer list part.  The 2 denotes the
// first two entries of the NAMallocGarbage struct.
#define NA_MALLOC_GARBAGE_POINTER_COUNT (NA_POOLPART_BYTESIZE / NA_ADDRESS_BYTES - 2)

struct NAMallocGarbage{
  NAMallocGarbage* next;
  size_t cur;
  void* pointers[NA_MALLOC_GARBAGE_POINTER_COUNT];
};



// The global runtime variable.
NARuntime* na_Runtime = NA_NULL;



// Security check: The pool byteSize must be big enough to store one struct
// of NA_PoolPart. Note that byteSize 0 has the meaning of using the
// memory page size.
#if(NA_POOLPART_BYTESIZE != 0) && (NA_POOLPART_BYTESIZE <= 8 * NA_ADDRESS_BYTES)
  #error "Memory pool size is too small"
#endif



// Registers a runtime type. Adds the typeInfo to the typeInfos found
// in na_Runtime. Note that when aggressive cleanup is turned on, any type
// which had been registered previously had already been unregistered.
NA_HIDEF void na_RegisterTypeInfo(NA_TypeInfo* typeInfo) {
  NA_TypeInfo** newinfos;

  #if NA_DEBUG
    if(typeInfo->curPart)
      naError("Newly registered type should have Null as current part.");
    if(typeInfo->typeSize < NA_ADDRESS_BYTES)
      naError("Size of type is too small");
    if(typeInfo->typeSize > (na_Runtime->partSize - sizeof(NA_PoolPart)))
      naError("Size of type is too big");
  #endif

  // As this is the first time, the runtime type gets used, we correct the
  // typeSize to incorporate reference counting, if any.
  if(typeInfo->refCounting) {
    typeInfo->typeSize += sizeof(NARefCount);
  }

  // We enlarge the na_Runtime info array by one. Yes, this is very bad
  // performance, but this code is usually called rather seldomly. If you
  // experience here a lot of memory allocations, you might want to check if
  // NA_MEMORY_POOL_AGGRESSIVE_CLEANUP is set to 1 and set it back to 0.
  newinfos = naMalloc(sizeof(NA_TypeInfo*) * (na_Runtime->typeInfoCount + NA_ONE_s));

  // We copy all previous infos to the newly allocated memory block.
  if(na_Runtime->typeInfos) {
    naCopyn(newinfos, na_Runtime->typeInfos, sizeof(NA_TypeInfo*) * na_Runtime->typeInfoCount);
  }

  // We add the new typeInfo as a pointer to the na_Runtime infos.
  newinfos[na_Runtime->typeInfoCount] = typeInfo;
  na_Runtime->typeInfoCount++;

  // We cleanup the old infos and set the new ones to be valid.
  naFree(na_Runtime->typeInfos);
  na_Runtime->typeInfos = newinfos;
}



NA_HIDEF void na_UnregisterTypeInfo(NA_TypeInfo* typeInfo) {
  NA_TypeInfo** newinfos = NA_NULL;
  if(na_Runtime->typeInfoCount > 1) {
    newinfos = naMalloc(sizeof(NA_TypeInfo*) * (na_Runtime->typeInfoCount - NA_ONE_s));

    // We shrink the info array by one by omitting the one entry which equals
    // the given parameter. Again, just like na_RegisterTypeInfo, this is not
    // very fast, but does the job. See comment there.
    NAInt oldindex = 0;
    for(size_t i = 0; i < (na_Runtime->typeInfoCount - NA_ONE_s); ++i) {
      if(na_Runtime->typeInfos[i] == typeInfo) {
        oldindex++;
      }
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
  if(typeInfo->refCounting) {
    typeInfo->typeSize -= sizeof(NARefCount);
  }
}



NA_HDEF size_t na_GetTypeInfoAllocatedCount(NA_TypeInfo* typeInfo) {
  NA_PoolPart* firstpart = typeInfo->curPart;
  NA_PoolPart* curPart = firstpart->nextPart;
  size_t totalCount = firstpart->usedCount;
  while(curPart != firstpart) {
    totalCount += curPart->usedCount;
    curPart = curPart->nextPart;
  }
  return totalCount;
}



NA_HIDEF NABool na_IsPoolPartFull(NA_PoolPart* part) {
  return (part->usedCount == part->maxCount);
}



NA_HIDEF void na_AttachPoolPartAfterCurPoolPart(NA_TypeInfo* typeInfo, NA_PoolPart* part) {
  part->prevPart = typeInfo->curPart;
  part->nextPart = typeInfo->curPart->nextPart;
  part->prevPart->nextPart = part;
  part->nextPart->prevPart = part;
}

// This function gets called when no part has any more space.
// A new part is created and added to the list at the current position.
NA_HIDEF void na_EnhancePool(NA_TypeInfo* typeInfo) {
  NA_PoolPart* part;

  // We create a new part with the size of a full part but we type it as
  // NA_PoolPart to access the first bytes.
  part = (NA_PoolPart*)naMallocAligned(na_Runtime->partSize, na_Runtime->partSize);
  #if NA_DEBUG
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
  part->firstUnused = (void*)(((NAByte*)part) + sizeof(NA_PoolPart));

  // If we are in debug mode, we also set the dummy variable for a consistency
  // check.
  #if NA_DEBUG
    part->dummy = part;
  #endif

  // Add the new part after the current part or set the part as the first and
  // only part, if there is none available yet.
  if(typeInfo->curPart) {
    na_AttachPoolPartAfterCurPoolPart(typeInfo, part);
  }else{
    part->prevPart = part;
    part->nextPart = part;
  }

  // Set the newly created part to be the current part.
  typeInfo->curPart = part;
}



NA_DEF void* na_NewStructInternal(NATypeInfo* info) {
  #if NA_DEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!info)
      naCrash("Given type identifier is Null-Pointer. Do not call na_NewStruct directly. Use the naNew macro.");
    if(info->typeSize == 0)
      naError("Type size is zero. Is the type void?");
  #endif

  NA_TypeInfo* typeInfo = (NA_TypeInfo*)info;

  // If there is no current part, create a first one.
  // This happends either upon first naNew of this type ever or when aggressive
  // memory cleanup is activated. See Configuration.h
  if(!typeInfo->curPart) {
    // As this is the first one, we register the type to the runtime system.
    na_RegisterTypeInfo(typeInfo);
    na_EnhancePool(typeInfo);
    #if NA_DEBUG
      if(!typeInfo->curPart)
        naCrash("No part available even after enhancing.");
    #endif
  }

  // If the current part is full, we try the next in the part list.
  if(na_IsPoolPartFull(typeInfo->curPart)) {
    typeInfo->curPart = typeInfo->curPart->nextPart;
    // If the next in the part list is full too, no part in the list has any
    // space left and hence we must create a new part.
    if(na_IsPoolPartFull(typeInfo->curPart)) {
      na_EnhancePool(typeInfo);
    }
  }

  // Now, we can be sure that the current part has space.
  #if NA_DEBUG
    if(na_IsPoolPartFull(typeInfo->curPart))
      naCrash("Still no space after creating new space.");
  #endif

  // We get the pointer to the first currently unused space.
  void* pointer = typeInfo->curPart->firstUnused;
  void* retPointer = pointer;
  
  // In case this is a reference counting type, initialize the refCounter
  // and set the retPointer to the correct position.
  if(typeInfo->refCounting) {
    naInitRefCount(pointer);
    retPointer = (NAByte*)pointer + sizeof(NARefCount);
  }

  // We find out which will be the next pointer to return.
  if(typeInfo->curPart->usedCount == typeInfo->curPart->everUsedCount) {
    // The current space has not been used ever and is de facto the one unused
    // space with the lowest address in this part. Use the next address one
    // typeSize ahead for the next space.
    typeInfo->curPart->firstUnused = (NAByte*)(typeInfo->curPart->firstUnused) + typeInfo->typeSize;

    // Increase the number of ever used spaces in this part.
    typeInfo->curPart->everUsedCount++;
  }else{
    // The space has already been used and deleted before which means, it
    // currently stores a pointer to the next unused space.
    // Note that the next pointer is stored at retPointer, not pointer. This
    // is because reference counting types use the first bytes for the
    // NARefCount structure which still is useful for error checking. So one
    // can still detect if the programmer erroneously wants to retain or
    // release the pointer.
    typeInfo->curPart->firstUnused = *((void**)retPointer);
  }

  // Increase the number of spaces used in this part.
  typeInfo->curPart->usedCount++;

  #if NA_DEBUG
    #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
      naError("No native integer type to successfully run the runtime system.");
    #else
      if(typeInfo->curPart != (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask))
        naError("Pointer seems to be outside of part");
    #endif
  #endif

  return retPointer;
}



NA_DEF void* na_NewStruct(NATypeInfo* info) {
  #if NA_DEBUG
    NA_TypeInfo* typeInfo = (NA_TypeInfo*)info;
    if(typeInfo->refCounting)
      naError("Do not use naNew for reference-counting types. Use naCreate.");
  #endif
  return na_NewStructInternal(info);
}
NA_DEF void* na_CreateStruct(NATypeInfo* info) {
  #if NA_DEBUG
    NA_TypeInfo* typeInfo = (NA_TypeInfo*)info;
    if(!typeInfo->refCounting)
      naError("Do not use naCreate for non-reference-counting types. Use naNew.");
  #endif
  return na_NewStructInternal(info);
}



NA_HIDEF void na_EjectPoolPartObject(NA_PoolPart* part, void* pointer) {
  // The memory at pointer is expected to be erased and hence garbage.

  // We explicitely store a pointer to the next unused space at that
  // position, ultimately creating a list.
  // Note that for reference counting types, we store the next pointer at
  // the place where the actual content is stored, not the reference count.
  // With that, it is still possible to do some error checks when for example
  // the programmer wants to erroneously retain or release a pointer which
  // has already been erased.
  if(part->typeInfo->refCounting) {
    *((void**)((NAByte*)pointer + sizeof(NARefCount))) = part->firstUnused;
  }else{
    *((void**)pointer) = part->firstUnused;
  }
  part->firstUnused = pointer;

  // If the part was full up until now, we reattach it in the list such that
  // it comes one after the current part. But only if there are more than one
  // parts around and the current part of the typeInfo is not already the
  // current part.
  if(na_IsPoolPartFull(part) && (part->nextPart != part) && (part->typeInfo->curPart != part)) {
    part->nextPart->prevPart = part->prevPart;
    part->prevPart->nextPart = part->nextPart;
    na_AttachPoolPartAfterCurPoolPart(part->typeInfo, part);
  }

  // We reduce the number of used spaces in this part.
  part->usedCount--;

  // If no more spaces are in use, we can shrink that part away.
  if(!part->usedCount) {
    if(part->nextPart == part) {
      #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 1
        // If this part is the last part of the pool and the cleanup is set to
        // aggressive, we shrink it away and unregister the type.
        NA_TypeInfo* typeInfo = part->typeInfo;
        naFreeAligned(part);
        typeInfo->curPart = NA_NULL;
        na_UnregisterTypeInfo(typeInfo);
      #endif
    }else{
      // There are other parts in the pool. If the empty part is the one which
      // is the current part of the pool, we move to the next part.
      if(part->typeInfo->curPart == part) {
        part->typeInfo->curPart = part->nextPart;
      }

      // Now, one could think of checking whether there are more parts with
      // free space available and keep the part if there is absolutely none
      // available. But that would become a bit of a list nightmare and would
      // destroy the simplicity of the current approach. Still, might be an
      // idea.

      // We unlink the part from the list.
      part->prevPart->nextPart = part->nextPart;
      part->nextPart->prevPart = part->prevPart;
      // And delete its memory.
      naFreeAligned(part);
    }
  }
}



NA_DEF void naDelete(void* pointer) {
  NA_PoolPart* part;

  #if NA_DEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naError("Deleting nullptr. Use a guard.");
  #endif

  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    NA_UNUSED(part);
    NA_UNUSED(pointer);
  #else

    // Find the part entry at the beginning of the part by AND'ing the
    // address with the partSizeMask
    part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);

    #if NA_DEBUG
      if(part->dummy != part)
        naError("Pointer seems not to be from a pool.");
      if(part->typeInfo->refCounting)
        naError("Pointer belongs to a reference-counting entity. Use naRelease instead of naDelete!");
    #endif

    // Erase the content of the space with the destructor if applicable
    if(part->typeInfo->destructor) {
      part->typeInfo->destructor(pointer);
    }

    na_EjectPoolPartObject(part, pointer);

  #endif
}



NA_DEF void* naRetain(void* pointer) {
  #if NA_DEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naCrash("pointer is Null");

    // Find the part entry at the beginning of the part by AND'ing the
    // address with the partSizeMask
    #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
      naError("No native integer type to successfully run the runtime system.");
      NA_UNUSED(part);
    #else
      NA_PoolPart* part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);
      if(part->dummy != part)
        naError("Pointer seems not to be from a pool.");
      if(!part->typeInfo->refCounting)
        naError("Pointer belongs to a NON-reference-counting entity. You can't use naRetain!");
    #endif
  #endif

  // Retain the refCounter.
  NARefCount* refCount = (NARefCount*)((NAByte*)pointer - sizeof(NARefCount));
  naRetainRefCount(refCount);
  return pointer;
}



NA_DEF void naRelease(void* pointer) {
  #if NA_DEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naCrash("pointer is Null");
  #endif

  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    NA_UNUSED(part);
    NA_UNUSED(pointer);
    NA_UNUSED(refCount);
    #if NA_DEBUG
      naError("No native integer type to successfully run the runtime system.");
    #endif
  #else

    // Find the pool entry at the beginning of the part by AND'ing the
    // address with the partSizeMask
    NA_PoolPart* part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);

    #if NA_DEBUG
      if(part->dummy != part)
        naError("Pointer seems not to be from a pool.");
      if(!part->typeInfo->refCounting)
        naError("Pointer belongs to a NON-reference-counting entity. Use naDelete instead of naRelease!");
    #endif

    // Release the space and delete it with the destructor if refCount is zero.
    NARefCount* refCount = (NARefCount*)((NAByte*)pointer - sizeof(NARefCount));
    naReleaseRefCount(refCount, pointer, part->typeInfo->destructor);
    // Note: The following test could also be achieved by using a special
    // mutator function in the previous naReleaseRefCount call. But this would
    // always cause a function call, even for types without a destructor.
    // Therefore, we do this here:
    if(!na_GetRefCountCount(refCount)) {
      na_EjectPoolPartObject(part, refCount);
    }

  #endif
}



NA_DEF const void* naRetainConst(const void* pointer) {
  void* mutablePointer = (void*)pointer;
  return naRetain(mutablePointer);
}



NA_DEF void naReleaseConst(const void* pointer) {
  void* mutablePointer = (void*)pointer;
  naRelease(mutablePointer);
}



NA_HIDEF void na_EnhanceMallocGarbage() {
  NAMallocGarbage* newGarbage = naAlloc(NAMallocGarbage);
#if NA_DEBUG
  if(!newGarbage)
    naCrash("No garbage memory allocated");
#endif
  newGarbage->next = na_Runtime->mallocGarbage;
  newGarbage->cur = 0;
  na_Runtime->mallocGarbage = newGarbage;
}



NA_DEF void* naMallocTmp(size_t byteSize) {
#if NA_DEBUG
  if(!naIsRuntimeRunning())
    naCrash("Runtime not running. Use naStartRuntime()");
#endif

#if NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT != 0
  if(na_Runtime->totalMallocGarbageByteCount > (size_t)NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT) {
    naCollectGarbage();
  }
#endif

  na_Runtime->totalMallocGarbageByteCount += byteSize;
  void* newPtr = naMalloc(byteSize);

  if(!na_Runtime->mallocGarbage || (na_Runtime->mallocGarbage->cur == NA_MALLOC_GARBAGE_POINTER_COUNT)) {
    na_EnhanceMallocGarbage();
  }

#if NA_DEBUG
  if(!na_Runtime->mallocGarbage)
    naCrash("Garbage struct is Null");
  if(na_Runtime->mallocGarbage->cur >= NA_MALLOC_GARBAGE_POINTER_COUNT)
    naCrash("Buffer overrun.");
#endif

  NAMallocGarbage* garbage = na_Runtime->mallocGarbage;
  garbage->pointers[garbage->cur] = newPtr;
  garbage->cur++;
  return newPtr;
}



NA_DEF void naCollectGarbage() {
#if NA_DEBUG
  if(!naIsRuntimeRunning())
    naCrash("Runtime not running. Use naStartRuntime()");
#endif
  while(na_Runtime->mallocGarbage) {
    void** ptr = na_Runtime->mallocGarbage->pointers;
    for(size_t i = 0; i < na_Runtime->mallocGarbage->cur; ++i) {
      naFree(*ptr);
      ptr++;
    }
    NAMallocGarbage* nextGarbage = na_Runtime->mallocGarbage->next;

    // If this was the last part, we decide if we want to delete it depending
    // on the aggressive setting.
  #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 1
    naFree(na_Runtime->mallocGarbage);
    na_Runtime->mallocGarbage = nextGarbage;
  #else
    if(nextGarbage) {
      naFree(na_Runtime->mallocGarbage);
      na_Runtime->mallocGarbage = nextGarbage;
    }else{
      if(na_Runtime->mallocGarbage) {
        na_Runtime->mallocGarbage->cur = 0;
      }else{
        #if NA_DEBUG
          naError("This should not happen but code sanity checks require it.");
        #endif
      }
      break;
    }
  #endif
  }

  na_Runtime->totalMallocGarbageByteCount = 0;
}



NA_DEF void naStartRuntime() {
  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    #if NA_DEBUG
      naError("Unable to start runtime on system where no native int is able to store an address.");
    #endif
  #else

    #if NA_DEBUG
      if(naIsRuntimeRunning())
        naCrash("Runtime already running");
      if(sizeof(NA_PoolPart) != (8 * NA_ADDRESS_BYTES))
        naError("NA_PoolPart struct encoding misaligned");
    #endif
    na_Runtime = naAlloc(NARuntime);
    na_Runtime->memPageSize = naGetSystemMemoryPagesize();
    #if(NA_POOLPART_BYTESIZE == 0)
      na_Runtime->partSize = naGetSystemMemoryPagesize();
      na_Runtime->partSizeMask = naGetSystemMemoryPagesizeMask();
    #elif(NA_POOLPART_BYTESIZE < NA_MAX_i32)
      na_Runtime->partSize = (size_t)NA_POOLPART_BYTESIZE;
      na_Runtime->partSizeMask = ~(size_t)(NA_POOLPART_BYTESIZE - NA_ONE);
    #else
      #error "Pool part byteSize is too large"
    #endif
    na_Runtime->mallocGarbage = NA_NULL;
    na_Runtime->totalMallocGarbageByteCount = 0;
    na_Runtime->typeInfoCount = 0;
    na_Runtime->typeInfos = NA_NULL;
  #endif
}



NA_DEF void naStopRuntime() {
  // First, we collect the garbage
  naCollectGarbage();
  #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 0
    if(na_Runtime->mallocGarbage) {
      naFree(na_Runtime->mallocGarbage);
      na_Runtime->mallocGarbage = NA_NULL;
    }
  #endif

  // Then, we detect, if there are any memory leaks.
  #if NA_DEBUG
    NABool leakMessagePrinted = NA_FALSE;
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");

    // Go through all registered types and output a leak message if necessary.
    for(size_t i = 0; i < na_Runtime->typeInfoCount; ++i) {
      size_t spaceCount = na_GetTypeInfoAllocatedCount(na_Runtime->typeInfos[i]);
      #if NA_DEBUG
        if(spaceCount) {
          if(!leakMessagePrinted) {
            printf(NA_NL "Memory leaks detected in NARuntime:" NA_NL);
            leakMessagePrinted = NA_TRUE;
          }
          printf("%s: %zu * %zu = %zu Bytes" NA_NL, na_Runtime->typeInfos[i]->typeName, spaceCount, na_Runtime->typeInfos[i]->typeSize, spaceCount * na_Runtime->typeInfos[i]->typeSize);
        }
      #endif
    }
  #endif

  // Go through all remaining registered types and completely erase them
  // from memory.
  while(na_Runtime->typeInfos) {
    NA_PoolPart* firstpart;
    NA_PoolPart* curPart;
    NA_PoolPart* nextPart;

    // Free all parts.
    firstpart = na_Runtime->typeInfos[0]->curPart;
    curPart = firstpart;
    while(curPart) {
      nextPart = curPart->nextPart;
      naFreeAligned(curPart);
      
      if(nextPart == firstpart)
        break;
      
      curPart = nextPart;
    }

    // Finally, unregister the type.
    na_Runtime->typeInfos[0]->curPart = NA_NULL;
    na_UnregisterTypeInfo(na_Runtime->typeInfos[0]);
  }

  naFree(na_Runtime);
  na_Runtime = NA_NULL;
}



// This is the lonely implimentation of the destruction function of a
// NAPointer. Needed a place to exists in a .c file so we gave the homeless
// a place here.
NA_HDEF void na_DestructPointer(NAPointer* pointer) {
  if(pointer->destructor) {
    pointer->destructor(naGetSmartPtrMutable(&pointer->sptr));
  }
  naDelete(pointer);
}



NA_HDEF size_t naGetRuntimeTypeRefCount(const void* pointer) {
  #if NA_DEBUG
    // Find the pool entry at the beginning of the part by AND'ing the
    // address with the partSizeMask
    if(!pointer)
      naCrash("pointer is Null");
    NA_PoolPart* part = (NA_PoolPart*)((size_t)pointer & na_Runtime->partSizeMask);
    if(part->dummy != part)
      naError("Pointer seems not to be from a pool.");
    if(!part->typeInfo->refCounting)
      naError("Pointer belongs to a NON-reference-counting entity. Use naDelete instead of naRelease!");
  #endif

  const NARefCount* refCount = (const NARefCount*)((const NAByte*)pointer - sizeof(NARefCount));
  return na_GetRefCountCount(refCount);
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
