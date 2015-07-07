
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "NARuntime.h"
#include "NAPointer.h"
#include "NAString.h"


//#define NA_POOL_SIZE_EQUALS_PAGESIZE
#define NA_CUSTOM_POOL_SIZE (1 << 16)


typedef struct NACorePool NACorePool;
typedef struct NACoreTypeInfo NACoreTypeInfo;
typedef struct NARuntime NARuntime;


struct NACoreTypeInfo{
  NACorePool* curpool;
  NATypeInfo typeinfo;
};

struct NACorePool{
  NACoreTypeInfo* coretypeinfo;
  NAUInt usedcount;
  void* firstunused;
  void* firstrecycle;
  NACorePool* prevpool;
  NACorePool* nextpool;
  void* dummy1;
  void* dummy2;
};

struct NARuntime{
  NAUInt poolsize;
  NAUInt poolsizemask;
};



NARuntime* na_runtime = NA_NULL;


// These are the (hidden) string type identifier for the NALib runtime system.
// Will be set when calling naPrepareStringRuntime or naStartRuntime. This
// will likely change in the near future.
NATypeIdentifier na_NAString_identifier = NA_NULL;
NATypeIdentifier na_NAPointer_identifier = NA_NULL;





NA_HIDEF void* naEnhanceCorePool(NACoreTypeInfo* coretypeinfo){
  NACorePool* corepool;
  void** curunused;
  #ifndef NDEBUG
    if(coretypeinfo->typeinfo.typesize < NA_SYSTEM_ADDRESS_BYTES)
      naError("naEnhanceCorePool", "Element is too small");
  #endif
  #ifndef NDEBUG
    if(coretypeinfo->typeinfo.typesize > (na_runtime->poolsize - sizeof(NACorePool)))
      naError("naEnhanceCorePool", "Element is too big");
  #endif
  corepool = (NACorePool*)naMallocAligned(na_runtime->poolsize, na_runtime->poolsize);
  corepool->coretypeinfo = coretypeinfo;

  // Reduce the elemcount by 1 to set the last entry to NULL
  corepool->usedcount = ((na_runtime->poolsize - sizeof(NACorePool)) / coretypeinfo->typeinfo.typesize) - 1;
  curunused = (void**)(((NAByte*)corepool) + sizeof(NACorePool));
  corepool->firstunused = curunused;
  while(corepool->usedcount){
    void** nextunused = (void**)((NAByte*)curunused + coretypeinfo->typeinfo.typesize);
    *curunused = nextunused;
    curunused = nextunused;
    corepool->usedcount--;
  }
  *curunused = NA_NULL; // note that elemcount has been reduced by 1

  corepool->firstrecycle = NA_NULL;
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
  naFreePageAligned(corepool);
}




NA_DEF void* naNewStruct(NATypeIdentifier typeidentifier){
  void* pointer;
  NACoreTypeInfo* coretypeinfo = (NACoreTypeInfo*)typeidentifier;
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNew", "Runtime not running. Use naStartRuntime()");
    if(!coretypeinfo)
      {naCrash("naNew", "Given type identifier is Null"); return NA_NULL;}
    if(!coretypeinfo->curpool)
      {naCrash("naNew", "No pool present"); return NA_NULL;}
  #endif
  
  pointer = coretypeinfo->curpool->firstunused;
  if(!pointer){pointer = naEnhanceCorePool(typeidentifier);}
  coretypeinfo->curpool->usedcount++;
  coretypeinfo->curpool->firstunused = *((void**)coretypeinfo->curpool->firstunused);
  // Now, the pointer points to a space which can be constructed.
  return pointer;
}



NA_DEF void naDelete(void* pointer){
  NACorePool* corepool;
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNew", "Runtime not running. Use naStartRuntime()");
  #endif
  // Delete the struct with the destructor
  corepool = (NACorePool*)((NAUInt)pointer & na_runtime->poolsizemask);
  if(corepool->coretypeinfo->typeinfo.desctructor){corepool->coretypeinfo->typeinfo.desctructor(pointer);}
  *((void**)pointer) = corepool->firstunused;
  corepool->firstunused = pointer;
  corepool->usedcount--;
  if((!corepool->usedcount) && (corepool->nextpool != corepool)){naShrinkCorePool(corepool);}
}




NA_DEF void naStartRuntime(){
  #ifndef NDEBUG
    if(na_runtime)
      naCrash("naStartRuntime", "Runtime already running");
    if(sizeof(NACorePool) != (8 * NA_SYSTEM_ADDRESS_BYTES))
      naError("naStartRuntime", "NACorePool struct encoding misaligned");
  #endif
  na_runtime = naAlloc(NARuntime);
  #if defined NA_POOL_SIZE_EQUALS_PAGESIZE
    na_runtime->poolsize = naGetSystemMemoryPageSize();
    na_runtime->poolsizemask = naGetSystemMemoryPageSizeMask();
  #else
    na_runtime->poolsize = NA_CUSTOM_POOL_SIZE;
    na_runtime->poolsizemask = ~(NAUInt)(NA_CUSTOM_POOL_SIZE - NA_ONE);
  #endif
  naPrepareStringRuntime();
  naPreparePointerRuntime();
}



NA_DEF NATypeIdentifier naManageRuntimeType(NATypeInfo* typeinfo){
  NACoreTypeInfo* coretypeinfo = naAlloc(NACoreTypeInfo);
  coretypeinfo->typeinfo = *typeinfo;
  coretypeinfo->curpool = NA_NULL;
  naEnhanceCorePool(coretypeinfo);
  return coretypeinfo;
}



NA_DEF void naStopRuntime(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naStopRuntime", "Runtime not running");
  #endif
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
