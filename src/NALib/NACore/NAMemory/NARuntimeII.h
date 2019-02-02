// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



// This needs to be in this inline implementation file as it needs to be
// accessible to the compiler at all times.
struct NATypeInfo{
  void*             curpoolpart;    // The actual type of this entry is hidden.
  NAUInt            typesize;
  NAMutator         destructor;
  NABool            refcounting;
  #ifndef NDEBUG
    const char*     typename;
  #endif
};



#undef NA_EXTERN_RUNTIME_TYPE
#define NA_EXTERN_RUNTIME_TYPE(typename)\
  extern NATypeInfo na_ ## typename ## _typeinfo



#undef NA_RUNTIME_TYPE
#ifndef NDEBUG
  #define NA_RUNTIME_TYPE(typename, destructor, refcounting)\
    NATypeInfo na_ ## typename ## _typeinfo =\
    {NA_NULL,\
    naSizeof(typename),\
    (NAMutator)destructor,\
    refcounting,\
    #typename}
#else
  #define NA_RUNTIME_TYPE(typename, destructor, refcounting)\
    NATypeInfo na_ ## typename ## _typeinfo =\
    {NA_NULL,\
    naSizeof(typename),\
    (NAMutator)destructor,\
    refcounting}
#endif



#undef naNew
#define naNew(typename) (typename*)naNewStruct(&na_ ## typename ## _typeinfo)
// If you experience an error here with naNew: Have you marked your type with
// NA_RUNTIME_TYPE? See NA_RUNTIME_TYPE below.

NA_API void*        naNewStruct(NATypeInfo* typeidentifier);



typedef struct NACoreTypeInfo NACoreTypeInfo;
typedef struct NAMallocGarbage NAMallocGarbage;
typedef struct NARuntime NARuntime;

// The runtime struct stores base informations about the runtime.
struct NARuntime{
  NASizeUInt mempagesize;
  NASizeUInt partsize;
  NASizeUInt partsizemask;
  NAMallocGarbage* mallocGarbage;
  NASizeUInt totalmallocgarbagebytecount;
  NAInt typeinfocount;
  NACoreTypeInfo** typeinfos;
};

extern NARuntime* na_runtime;



NA_IDEF NAUInt naGetRuntimeGarbageBytesize(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naGetRuntimeGarbageBytesize", "Runtime not running. Use naStartRuntime()");
  #endif
  return na_runtime->totalmallocgarbagebytecount;
}



NA_IDEF NAUInt naGetRuntimeMemoryPageSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naGetRuntimeMemoryPageSize", "Runtime not running. Use naStartRuntime()");
  #endif
  return na_runtime->mempagesize;
}



NA_IDEF NAUInt naGetRuntimePoolPartSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("naGetRuntimePoolPartSize", "Runtime not running. Use naStartRuntime()");
  #endif
  return na_runtime->partsize;
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
