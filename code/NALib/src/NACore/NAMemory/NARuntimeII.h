
// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



// This needs to be in this inline implementation file as it needs to be
// accessible to the compiler at all times.
struct NATypeInfo{
  void*             curpoolpart;    // The actual type of this entry is hidden.
  size_t            typeSize;
  NAMutator         destructor;
  NABool            refCounting;
  #ifndef NDEBUG
    const char*     typeName;
  #endif
};



#undef NA_EXTERN_RUNTIME_TYPE
#define NA_EXTERN_RUNTIME_TYPE(typeName)\
  extern NATypeInfo na_ ## typeName ## TypeInfo



#undef NA_RUNTIME_TYPE
#ifndef NDEBUG
  #define NA_RUNTIME_TYPE(typeName, destructor, refCounting)\
    NATypeInfo na_ ## typeName ## TypeInfo =\
    {NA_NULL,\
    sizeof(typeName),\
    (NAMutator)destructor,\
    refCounting,\
    #typeName}
#else
  #define NA_RUNTIME_TYPE(typeName, destructor, refCounting)\
    NATypeInfo na_ ## typeName ## TypeInfo =\
    {NA_NULL,\
    sizeof(typeName),\
    (NAMutator)destructor,\
    refCounting}
#endif



#undef naNew
#define naNew(typeName) (typeName*)naNewStruct(&na_ ## typeName ## TypeInfo)
// If you experience an error here with naNew: Have you marked your type with
// NA_RUNTIME_TYPE? See NA_RUNTIME_TYPE below.

NA_API void* naNewStruct(NATypeInfo* info);



typedef struct NA_TypeInfo NA_TypeInfo;
typedef struct NAMallocGarbage NAMallocGarbage;
typedef struct NARuntime NARuntime;

// The runtime struct stores base informations about the runtime.
struct NARuntime{
  size_t memPageSize;
  size_t partSize;
  size_t partSizeMask;
  NAMallocGarbage* mallocGarbage;
  size_t totalMallocGarbageByteCount;
  size_t typeInfoCount;
  NA_TypeInfo** typeInfos;
};

extern NARuntime* na_Runtime;



NA_IDEF size_t naGetRuntimeGarbageByteSize(){
  #ifndef NDEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  return na_Runtime->totalMallocGarbageByteCount;
}



NA_IDEF size_t naGetRuntimeMemoryPageSize(){
  #ifndef NDEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  return na_Runtime->memPageSize;
}



NA_IDEF size_t naGetRuntimePoolPartSize(){
  #ifndef NDEBUG
    if(!naIsRuntimeRunning())
      naCrash("Runtime not running. Use naStartRuntime()");
  #endif
  return na_Runtime->partSize;
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
