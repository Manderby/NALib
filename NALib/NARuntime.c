
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "NARuntime.h"
#include "NAPointer.h"
#include "NAString.h"




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
  NAUInt pagesize;
  NAUInt pagesizemask;
};



NARuntime* na_runtime = NA_NULL;






NA_HIDEF void* naEnhanceCorePool(NACoreTypeInfo* coretypeinfo){
  #ifndef NDEBUG
    if(coretypeinfo->typeinfo.typesize < NA_SYSTEM_ADDRESS_BYTES)
      naError("naEnhanceCorePool", "Element is too small");
  #endif
  #ifndef NDEBUG
    if(coretypeinfo->typeinfo.typesize > (na_runtime->pagesize - sizeof(NACorePool)))
      naError("naEnhanceCorePool", "Element is too big");
  #endif
  NACorePool* corepool = (NACorePool*)naAllocatePageAligned(na_runtime->pagesize);
  corepool->coretypeinfo = coretypeinfo;

  // Reduce the elemcount by 1 to set the last entry to NULL
  corepool->usedcount = ((na_runtime->pagesize - sizeof(NACorePool)) / coretypeinfo->typeinfo.typesize) - 1;
  void** curunused = (void**)(((NAByte*)corepool) + sizeof(NACorePool));
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
  free(corepool);
}




NA_DEF void* naNew(NATypeIdentifier typeidentifier){
  NACoreTypeInfo* coretypeinfo = (NACoreTypeInfo*)typeidentifier;
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNew", "Runtime not running. Use naStartRuntime()");
    if(!coretypeinfo)
      {naCrash("naNew", "Given type identifier is Null"); return NA_NULL;}
    if(!coretypeinfo->curpool)
      {naCrash("naNew", "No pool present"); return NA_NULL;}
  #endif
  
  void* pointer = coretypeinfo->curpool->firstunused;
  if(!pointer){pointer = naEnhanceCorePool(typeidentifier);}
  coretypeinfo->curpool->usedcount++;
  coretypeinfo->curpool->firstunused = *((void**)coretypeinfo->curpool->firstunused);
  // Now, the pointer points to a space which can be constructed.
  return pointer;
}



NA_DEF void naDelete(void* pointer){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naNew", "Runtime not running. Use naStartRuntime()");
  #endif
  // Delete the struct with the destructor
  NACorePool* corepool = (NACorePool*)((NAUInt)pointer & na_runtime->pagesizemask);
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
  na_runtime = naAllocate(sizeof(NARuntime));
  na_runtime->pagesize = naGetSystemMemoryPageSize();
  na_runtime->pagesizemask = naGetSystemMemoryPageSizeMask();
  naPrepareStringRuntime();
}



NA_DEF NATypeIdentifier naManageRuntimeType(NATypeInfo* typeinfo){
  NACoreTypeInfo* coretypeinfo = naAllocate(sizeof(NACoreTypeInfo));
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
