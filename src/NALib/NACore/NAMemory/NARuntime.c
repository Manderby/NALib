
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NAMemory.h"
#include "../../NABinaryData.h"

// //////////////////////////////////////
// Implementation notes from the author about the Memory Pools.
//
// NALib uses memory allocation pools for types allocated with the runtime
// system. Here is an explanation on how it works:
//
// Each type which is marked with NA_RUNTIME_TYPE has a hidden struct called
// NACoreTypeInfo which is only visible to the programmer as NATypeInfo. In
// that struct, basic information about the type are stored like the size in
// bytes or the destructor to be called upon deletion.
//
// Most importantly, NACoreTypeInfo stores a pointer "curpart". This
// pointer points to an allocated memory block with a bytesize defined in
// NAConfiguration.h with NA_POOLPART_BYTESIZE. That size and a corresponding
// address mask is stored in the global NARuntime struct na_runtime.
//
// Each of these memory blocks is allocated aligned to NA_POOLPART_BYTESIZE and
// hence AND'ing an arbitrary address within that block with the address mask
// returns the address of the first byte of the memory block.
//
// At the first bytes of each of these memory blocks, a NACorePoolPart struct
// is embedded. Therefore addressing the first byte of such a memory block is
// equivalent to accessing its NACorePoolPart. All NACorePoolParts of all the
// memory blocks of one runtime type form together a cyclic double linked list
// with a prev and next pointer. (Note: This does not uses NAList but is a
// custom implementation)
//
// The curpart field of the NACoreTypeInfo points to the part which is
// expected to have a free space for the desired runtime type. From there on,
// parts going forward (next to curpart) are expected to have free space
// and partss going backward (prev from curpart) are expected to be full.
//
// While the first bytes of a memory block are filled with the contents of
// NACorePoolPart, the remaining bytes are used for storing the actual values,
// as many as the remaining space can store. The NACorePoolPart struct stores
// the first unused space as a pointer. When naNew is called, that pointer is
// used as the pointer being returned to the user which called naNew. Before
// returning the pointer though, the next pointer to an unused space must be
// identified.
//
// NALib does that by looking at the bytes currently stored at that pointer.
// Initially, there will only be garbage values. NALib keeps track of that by
// counting how many spaces have ever been used inside the part. If the new
// space has never been used, the next available pointer will simply be one
// typesize ahead. Note from the author: This little trick made the current
// implementation really fast!
//
// Otherwise, when that space has already been used once and has been deleted
// in the meantime, it will now contain an address to the next free space.
// This can be ensured upon deletion of the previos space: After calling the
// optional desctructor, the values stored at that space become garbage values.
// Now, NALib puts the address of the next free space at the first bytes of the
// deleted space and sets the next-unused-pointer of the NACorePoolPart to this
// just deleted space, hence ultimately creating a linear list. With this
// mechanism, the NACorePoolPart can always keep track of which spacess are
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
typedef struct NACorePoolPart NACorePoolPart;
struct NACorePoolPart{
  NACoreTypeInfo* coretypeinfo;
  NASizeUInt maxcount;
  NASizeUInt usedcount;
  NASizeUInt everusedcount;
  void* firstunused;
  NACorePoolPart* prevpart;
  NACorePoolPart* nextpart;
  // The following field is a dummy entry not in use.
  // It shall not be removed though as the total amount of bytes used for
  // an NACorePoolPart shall be 8 times an addresssize.
  void* dummy; // used in debugging. Points at first byte of the whole pool
               // for consistency check.
};

struct NACoreTypeInfo{
  NACorePoolPart*   curpart;
  NAUInt            typesize;
  NAMutator         destructor;
  NABool            refcounting;
  #ifndef NDEBUG
    const char*     typename;
  #endif
};

#define NA_MALLOC_GARBAGE_POINTER_COUNT (NA_POOLPART_BYTESIZE / NA_SYSTEM_ADDRESS_BYTES - 2)
// The 2 denotes the first two entries in this struct.

struct NAMallocGarbage{
  NAMallocGarbage* next;
  NASizeUInt cur;
  void* pointers[NA_MALLOC_GARBAGE_POINTER_COUNT];
};



// The global runtime variable.
NARuntime* na_runtime = NA_NULL;



// Security check: The pool bytesize must be big enough to store one struct
// of NACorePoolPart. Note that bytesize 0 has the meaning of using the
// memory page size.
#if (NA_POOLPART_BYTESIZE != 0) && (NA_POOLPART_BYTESIZE <= 8 * NA_SYSTEM_ADDRESS_BYTES)
  #error "Core memory pool size is too small"
#endif



// Registers a runtime type. Adds the coretypeinfo to the typeinfos found
// in na_runtime. Note that when aggressive cleanup is turned on, any type
// which had been registered previously had already been unregistered.
NA_HIDEF void naRegisterCoreTypeInfo(NACoreTypeInfo* coretypeinfo){
  NACoreTypeInfo** newinfos;

  #ifndef NDEBUG
    if(coretypeinfo->curpart)
      naError("naRegisterCoreTypeInfo", "Newly registered type should have NULL as current part.");
    if(coretypeinfo->typesize < NA_SYSTEM_ADDRESS_BYTES)
      naError("naRegisterCoreTypeInfo", "Size of type is too small");
    if(coretypeinfo->typesize > (na_runtime->partsize - sizeof(NACorePoolPart)))
      naError("naRegisterCoreTypeInfo", "Size of type is too big");
  #endif

  // As this is the first time, the runtime type gets used, we correct the
  // typesize to incorporate reference counting, if any.
  if(coretypeinfo->refcounting){coretypeinfo->typesize += sizeof(NARefCount);}

  // We enlarge the na_runtime info array by one. Yes, this is very bad
  // performance, but this code is usually called rather seldomly. If you
  // experience here a lot of memory allocations, you might want to check if
  // NA_MEMORY_POOL_AGGRESSIVE_CLEANUP is set to 1 and set it back to 0.
  newinfos = naMalloc(naSizeof(NACoreTypeInfo*) * (na_runtime->typeinfocount + NA_ONE));

  // We copy all previous infos to the newly allocated memory block.
  if(na_runtime->typeinfos){
    naCopyn(newinfos, na_runtime->typeinfos, naSizeof(NACoreTypeInfo*) * na_runtime->typeinfocount);
  }

  // We add the new typeinfo as a pointer to the na_runtime infos.
  newinfos[na_runtime->typeinfocount] = coretypeinfo;
  na_runtime->typeinfocount++;

  // We cleanup the old infos and set the new ones to be valid.
  naFree(na_runtime->typeinfos);
  na_runtime->typeinfos = newinfos;
}



NA_HIDEF void naUnregisterCoreTypeInfo(NACoreTypeInfo* coretypeinfo){
  NACoreTypeInfo** newinfos = NA_NULL;
  if(na_runtime->typeinfocount > 1){
    NAInt i;
    NAInt oldindex = 0;
    newinfos = naMalloc(naSizeof(NACoreTypeInfo*) * (na_runtime->typeinfocount - NA_ONE));

    // We shrink the info array by one by omitting the one entry which equals
    // the given parameter. Again, just like naRegisterCoreTypeInfo, this is not
    // very fast, but does the job. See comment there.
    for(i = 0; i < (na_runtime->typeinfocount - NA_ONE); i++){
      if(na_runtime->typeinfos[i] == coretypeinfo){oldindex++;}
      newinfos[i] = na_runtime->typeinfos[oldindex];
      oldindex++;
    }
  }

  na_runtime->typeinfocount--;

  // We cleanup the old infos and set the new ones to be valid.
  naFree(na_runtime->typeinfos);
  na_runtime->typeinfos = newinfos;

  // We restore the original typesize in case NA_MEMORY_POOL_AGGRESSIVE_CLEANUP
  // is set to 1 which means, the type might get re-registered.
  if(coretypeinfo->refcounting){coretypeinfo->typesize -= sizeof(NARefCount);}
}



NA_HDEF NAUInt naGetCoreTypeInfoAllocatedCount(NACoreTypeInfo* coretypeinfo){
  NACorePoolPart* firstpart = coretypeinfo->curpart;
  NACorePoolPart* curpart = firstpart->nextpart;
  NAUInt totalcount = firstpart->usedcount;
  while(curpart != firstpart){
    totalcount += curpart->usedcount;
    curpart = curpart->nextpart;
  }
  return totalcount;
}



NA_HIDEF NABool naIsPoolPartFull(NACorePoolPart* part){
  return (part->usedcount == part->maxcount);
}



NA_HIDEF void naAttachPoolPartAfterCurPoolPart(NACoreTypeInfo* coretypeinfo, NACorePoolPart* part){
  part->prevpart = coretypeinfo->curpart;
  part->nextpart = coretypeinfo->curpart->nextpart;
  part->prevpart->nextpart = part;
  part->nextpart->prevpart = part;
}



// This function gets called when no part has any more space.
// A new part is created and added to the list at the current position.
NA_HIDEF void naEnhanceCorePool(NACoreTypeInfo* coretypeinfo){
  NACorePoolPart* part;

  // We create a new part with the size of a full part but we type it as
  // NACorePoolPart to access the first bytes.
  part = (NACorePoolPart*)naMallocAligned(na_runtime->partsize, na_runtime->partsize);
  #ifndef NDEBUG
    // Do you think the following check is not necessary? You'd be surprised
    // how many systems do not align memory correctly!
    if(((NASizeUInt)part & ~na_runtime->partsizemask) != 0)
      naError("naEnhanceCorePool", "pool part badly aligned");
  #endif

  // We initialize the basic fields of part.
  part->coretypeinfo = coretypeinfo;
  part->maxcount = ((na_runtime->partsize - sizeof(NACorePoolPart)) / coretypeinfo->typesize);
  part->usedcount = 0;
  part->everusedcount = 0;

  // Implementation note from the author: In earlier versions, everusedcount
  // did not exists and the whole memory block needed to be intialized either
  // with binary zero or with a precomputation step storing all next free
  // addresses. But this turned out to use quite some computing power and
  // hence this new method with the everusedcount was created. Way better now.

  // We set the pointer to the first available space to the first byte right
  // after the NACorePoolPart.
  part->firstunused = (void*)(((NAByte*)part) + sizeof(NACorePoolPart));

  // If we are in debug mode, we also set the dummy variable for a consistency
  // check.
  #ifndef NDEBUG
    part->dummy = part;
  #endif

  // Add the new part after the current part or set the part as the first and
  // only part, if there is none available yet.
  if(coretypeinfo->curpart){
    naAttachPoolPartAfterCurPoolPart(coretypeinfo, part);
  }else{
    part->prevpart = part;
    part->nextpart = part;
  }

  // Set the newly created part to be the current part.
  coretypeinfo->curpart = part;
}



NA_DEF void* naNewStruct(NATypeInfo* typeinfo){
  void* pointer;
  NACoreTypeInfo* coretypeinfo;

  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNewStruct", "Runtime not running. Use naStartRuntime()");
    if(!typeinfo)
      naCrash("naNewStruct", "Given type identifier is Null-Pointer. Do not call naNewStruct directly. Use the naNew macro.");
    if(typeinfo->typesize == 0)
      naError("naNewStruct", "Type size is zero. Is the type void?");
  #endif

  coretypeinfo = (NACoreTypeInfo*)typeinfo;

  // If there is no current part, create a first one.
  // This happends either upon first naNew of this type ever or when aggressive
  // memory cleanup is activated. See Configuration.h
  if(!coretypeinfo->curpart){
    // As this is the first one, we register the type to the runtime system.
    naRegisterCoreTypeInfo(coretypeinfo);
    naEnhanceCorePool(coretypeinfo);
    #ifndef NDEBUG
      if(!coretypeinfo->curpart)
        naCrash("naNewStruct", "No part available even after enhancing.");
    #endif
  }

  // If the current part is full, we try the next in the part list.
  if(naIsPoolPartFull(coretypeinfo->curpart)){
    coretypeinfo->curpart = coretypeinfo->curpart->nextpart;
    // If the next in the part list is full too, no part in the list has any
    // space left and hence we must create a new part.
    if(naIsPoolPartFull(coretypeinfo->curpart)){naEnhanceCorePool(coretypeinfo);}
  }

  // Now, we can be sure that the current part has space.
  #ifndef NDEBUG
    if(naIsPoolPartFull(coretypeinfo->curpart))
      naCrash("naNewStruct", "Still no space after creating new space.");
  #endif

  // We get the pointer to the first currently unused space.
  pointer = coretypeinfo->curpart->firstunused;

  // We find out which will be the next pointer to return.
  if(coretypeinfo->curpart->usedcount == coretypeinfo->curpart->everusedcount){
    // The current space has not been used ever. Use the next address one
    // typesize ahead.
    coretypeinfo->curpart->firstunused = (NAByte*)(coretypeinfo->curpart->firstunused) + coretypeinfo->typesize;
    // Increase the number of ever used spaces in this part.
    coretypeinfo->curpart->everusedcount++;
  }else{
    // The space has already been used and deleted before which means, it
    // stores now a pointer to the next unused space.
    coretypeinfo->curpart->firstunused = *((void**)coretypeinfo->curpart->firstunused);
  }

  // Increase the number of spaces used in this part.
  coretypeinfo->curpart->usedcount++;

  #ifndef NDEBUG
    #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
      naError("naNewStruct", "No native integer type to successfully run the runtime system.");
    #else
      if(coretypeinfo->curpart != (NACorePoolPart*)((NASizeUInt)pointer & na_runtime->partsizemask))
        naError("naNewStruct", "Pointer seems to be outside of part");
    #endif
  #endif

  // Now, the pointer points to a space which can be constructed. Initialize
  // the refcounter if applicable and return the pointer.
  if(coretypeinfo->refcounting){
    naInitRefCount(pointer);
    return (void*)((NAByte*)pointer + sizeof(NARefCount));
  }else{
    return pointer;
  }
}



NA_HIDEF void naEjectCorePoolPartObject(NACorePoolPart* part, void* pointer){
  // The memory at pointer is expected to be erased and hence garbage.

  // We explicitely store a pointer to the next unused space at that
  // position, ultimately creating a list.
  *((void**)pointer) = part->firstunused;
  part->firstunused = pointer;

  // If the part was full up until now, we reattach it in the list such that
  // it comes one after the current part. But only if there are more than one
  // parts around and the current part of the coretypeinfo is not already the
  // current part.
  if((part->usedcount == part->maxcount) && (part->nextpart != part) && (part->coretypeinfo->curpart != part)){
    part->nextpart->prevpart = part->prevpart;
    part->prevpart->nextpart = part->nextpart;
    naAttachPoolPartAfterCurPoolPart(part->coretypeinfo, part);
  }

  // We reduce the number of used spaces in this part.
  part->usedcount--;

  // If no more spaces are in use, we can shrink that part away.
  if(!part->usedcount){
    if(part->nextpart == part){
      #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 1
        // If this part is the last part of the pool and the cleanup is set to
        // aggressive, we shrink it away and unregister the type.
        NACoreTypeInfo* coretypeinfo = part->coretypeinfo;
        naFreeAligned(part);
        coretypeinfo->curpart = NA_NULL;
        naUnregisterCoreTypeInfo(coretypeinfo);
      #endif
    }else{
      // There are other parts in the pool. If the empty part is the one which
      // is the current part of the pool, we move to the next part.
      if(part->coretypeinfo->curpart == part){
        part->coretypeinfo->curpart = part->nextpart;
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
  NACorePoolPart* part;

  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naDelete", "Runtime not running. Use naStartRuntime()");
  #endif

  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    NA_UNUSED(part);
    NA_UNUSED(pointer);
  #else

    // Find the part entry at the beginning of the part by AND'ing the
    // address with the partsizemask
    part = (NACorePoolPart*)((NASizeUInt)pointer & na_runtime->partsizemask);

    #ifndef NDEBUG
      if(part->dummy != part)
        naError("naDelete", "Pointer seems not to be from a pool.");
      if(part->coretypeinfo->refcounting)
        naError("naDelete", "Pointer belongs to a reference-counting entity. Use naRelease instead of naDelete!");
    #endif

    // Erase the content of the space with the destructor if applicable
    if(part->coretypeinfo->destructor){part->coretypeinfo->destructor(pointer);}

    naEjectCorePoolPartObject(part, pointer);

  #endif
}



NA_DEF void* naRetain(void* pointer){
  NARefCount* refcount;

  #ifndef NDEBUG
    NACorePoolPart* part;
    if(!na_runtime)
      naCrash("naRetain", "Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naCrash("naRetain", "pointer is null");

    // Find the part entry at the beginning of the part by AND'ing the
    // address with the partsizemask
    #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
      naError("naRetain", "No native integer type to successfully run the runtime system.");
      NA_UNUSED(part);
    #else
      part = (NACorePoolPart*)((NASizeUInt)pointer & na_runtime->partsizemask);
      if(part->dummy != part)
        naError("naRetain", "Pointer seems not to be from a pool.");
      if(!part->coretypeinfo->refcounting)
        naError("naRetain", "Pointer belongs to a NON-reference-counting entity. You can't use naRetain!");
    #endif
  #endif

  // Retain the refcounter.
  refcount = (NARefCount*)((NAByte*)pointer - sizeof(NARefCount));
  naRetainRefCount(refcount);
  return pointer;
}



NA_DEF void naRelease(void* pointer){
  NACorePoolPart* part;
  NARefCount* refcount;

  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naRelease", "Runtime not running. Use naStartRuntime()");
    if(!pointer)
      naCrash("naRelease", "pointer is null");
  #endif

  // Find the corepool entry at the beginning of the part by AND'ing the
  // address with the partsizemask
  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    NA_UNUSED(part);
    NA_UNUSED(pointer);
    NA_UNUSED(refcount);
    #ifndef NDEBUG
      naError("naRelease", "No native integer type to successfully run the runtime system.");
    #endif
  #else

    part = (NACorePoolPart*)((NASizeUInt)pointer & na_runtime->partsizemask);

    #ifndef NDEBUG
      if(part->dummy != part)
        naError("naRelease", "Pointer seems not to be from a pool.");
      if(!part->coretypeinfo->refcounting)
        naError("naRelease", "Pointer belongs to a NON-reference-counting entity. Use naDelete instead of naRelease!");
    #endif

    // Release the space and delete it with the destructor if refcount is zero.
    refcount = (NARefCount*)((NAByte*)pointer - sizeof(NARefCount));
    naReleaseRefCount(refcount, pointer, part->coretypeinfo->destructor);
    // Note: The following test could also be achieved by using a special
    // mutator function in the previous naReleaseRefCount call. But this would
    // always cause a function call, even for types without a destructor.
    // Therefore, we do this here:
    if(!naGetRefCountCount(refcount)){
      naEjectCorePoolPartObject(part, refcount);
    }

  #endif
}



NA_DEF void naStartRuntime(){
  #if defined NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
    #ifndef NDEBUG
      naError("naStartRuntime", "Unable to start runtime on system where no native int is able to store an address.");
    #endif
  #else

    #ifndef NDEBUG
      if(na_runtime)
        naCrash("naStartRuntime", "Runtime already running");
      if(sizeof(NACorePoolPart) != (8 * NA_SYSTEM_ADDRESS_BYTES))
        naError("naStartRuntime", "NACorePoolPart struct encoding misaligned");
    #endif
    na_runtime = naAlloc(NARuntime);
    na_runtime->mempagesize = naGetSystemMemoryPagesize();
    #if (NA_POOLPART_BYTESIZE == 0)
      na_runtime->partsize = naGetSystemMemoryPagesize();
      na_runtime->partsizemask = naGetSystemMemoryPagesizeMask();
    #elif (NA_POOLPART_BYTESIZE < NA_INT32_MAX)
      na_runtime->partsize = (NASizeUInt)NA_POOLPART_BYTESIZE;
      na_runtime->partsizemask = ~(NASizeUInt)(NA_POOLPART_BYTESIZE - NA_ONE);
    #else
      #error "Corepoolpart bytesize is too large"
    #endif
    na_runtime->mallocGarbage = NA_NULL;
    na_runtime->totalmallocgarbagebytecount = 0;
    na_runtime->typeinfocount = 0;
    na_runtime->typeinfos = NA_NULL;
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
    if(na_runtime->mallocGarbage){
      naFree(na_runtime->mallocGarbage);
      na_runtime->mallocGarbage = NA_NULL;
    }
  #endif

  // Then, we detect, if there are any memory leaks.
  #ifndef NDEBUG
    leakmessageprinted = NA_FALSE;
    if(!na_runtime)
      naCrash("naStopRuntime", "Runtime not running. Use naStartRuntime()");

    // Go through all registered types and output a leak message if necessary.
    for(i=0; i< na_runtime->typeinfocount; i++){
      NAUInt spacecount = naGetCoreTypeInfoAllocatedCount(na_runtime->typeinfos[i]);
      if(spacecount){
        if(!leakmessageprinted){
          printf(NA_NL "Memory leaks detected in NARuntime:" NA_NL);
          leakmessageprinted = NA_TRUE;
        }
        printf("%s: %" NA_PRIu " * %" NA_PRIu " = %" NA_PRIu " Bytes" NA_NL, na_runtime->typeinfos[i]->typename, spacecount, na_runtime->typeinfos[i]->typesize, spacecount * na_runtime->typeinfos[i]->typesize);
      }
    }
  #endif

  // Go through all remaining registered types and completely erase them
  // from memory.
  while(na_runtime->typeinfos){
    NACorePoolPart* firstpart;
    NACorePoolPart* curpart;
    NACorePoolPart* nextpart;

    // Free all parts.
    firstpart = na_runtime->typeinfos[0]->curpart;
    curpart = firstpart;
    while(1){
      nextpart = curpart->nextpart;
      naFreeAligned(curpart);
      if(nextpart == firstpart){break;}
      curpart = nextpart;
    }

    // Finally, unregister the type.
    na_runtime->typeinfos[0]->curpart = NA_NULL;
    naUnregisterCoreTypeInfo(na_runtime->typeinfos[0]);
  }

  naFree(na_runtime);
  na_runtime = NA_NULL;
}



NA_HIDEF void naEnhanceMallocGarbage(){
  NAMallocGarbage* newgarbage = naAlloc(NAMallocGarbage);
  newgarbage->next = na_runtime->mallocGarbage;
  newgarbage->cur = 0;
  na_runtime->mallocGarbage = newgarbage;
}



NA_DEF void* naMallocTmp(NAUInt bytesize){
  void* newPtr;
  NAMallocGarbage* garbage;
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naMallocTmp", "Runtime not running. Use naStartRuntime()");
  #endif
  #if NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT != 0
    if(na_runtime->totalmallocgarbagebytecount > NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT){naCollectGarbage();}
  #endif
  na_runtime->totalmallocgarbagebytecount += bytesize;
  newPtr = naMalloc((NAInt)bytesize);
  if(!na_runtime->mallocGarbage || (na_runtime->mallocGarbage->cur == NA_MALLOC_GARBAGE_POINTER_COUNT)){
    naEnhanceMallocGarbage();
  }
  #ifndef NDEBUG
    if(na_runtime->mallocGarbage->cur >= NA_MALLOC_GARBAGE_POINTER_COUNT)
      naError("naMallocTmp", "Buffer overrun.");
    if(!na_runtime->mallocGarbage)
      naCrash("naMallocTmp", "Garbage struct is null");
  #endif
  garbage = na_runtime->mallocGarbage;
  garbage->pointers[garbage->cur] = newPtr;
  garbage->cur++;
  return newPtr;
}



NA_DEF void naCollectGarbage(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naCollectRuntimeGarbage", "Runtime not running. Use naStartRuntime()");
  #endif
  while(na_runtime->mallocGarbage){
    NAUInt i;
    NAMallocGarbage* nextgarbage;
    void** ptr = na_runtime->mallocGarbage->pointers;
    for(i=0; i<na_runtime->mallocGarbage->cur; i++){
      naFree(*ptr);
      ptr++;
    }
    nextgarbage = na_runtime->mallocGarbage->next;

    // If this was the last part, we decide if we want to delete it depending
    // on the aggressive setting.
    #if NA_MEMORY_POOL_AGGRESSIVE_CLEANUP == 1
      naFree(na_runtime->mallocGarbage);
      na_runtime->mallocGarbage = nextgarbage;
    #else
      if(nextgarbage){
        naFree(na_runtime->mallocGarbage);
        na_runtime->mallocGarbage = nextgarbage;
      }else{
        na_runtime->mallocGarbage->cur = 0;
        break;
      }
    #endif

  }
  na_runtime->totalmallocgarbagebytecount = 0;
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
