
// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



// This needs to be in this inline implementation file as it needs to be
// accessible to the compiler at all times.
struct NATypeInfo{
  void*             curpoolpart;    // The actual type of this entry is hidden.
  NAUInt            typeSize;
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

NA_API void* naNewStruct(NATypeInfo* typeinfo);



typedef struct NACoreTypeInfo NACoreTypeInfo;
typedef struct NAMallocGarbage NAMallocGarbage;
typedef struct NARuntime NARuntime;

// The runtime struct stores base informations about the runtime.
struct NARuntime{
  size_t mempagesize;
  size_t partsize;
  size_t partsizemask;
  NAMallocGarbage* mallocGarbage;
  size_t totalmallocgarbagebytecount;
  NAInt typeinfocount;
  NACoreTypeInfo** typeinfos;
};

extern NARuntime* na_runtime;



NA_IDEF size_t naGetRuntimeGarbageByteSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  return na_runtime->totalmallocgarbagebytecount;
}



NA_IDEF size_t naGetRuntimeMemoryPageSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  return na_runtime->mempagesize;
}



NA_IDEF size_t naGetRuntimePoolPartSize(){
  #ifndef NDEBUG
    if(!na_runtime)
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  return na_runtime->partsize;
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
