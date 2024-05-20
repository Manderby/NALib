
// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



// An NAPtr stores a C-Pointer and the information whether it is const or
// mutable. It also stores information about the byteSize of the memory
// being pointed at and how it is null terminated. Even more, it stores,
// how the pointer had originally been allocated. All this information is just
// for debugging and can be omitted if necessary. When compiling with NA_DEBUG
// being 0, no information is stored at all.

struct NAPtr{
  union{                        // A union storing either ...
    const void* constd;         // ... const data or ...
    void*       d;              // ... non-const (mutable) data.
  } data;
  #if NA_DEBUG
    uint32 debugFlags;          // This field stores some flags.
  #endif
};
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense.
//
// Also note that NAPtr will be fully optimized to a simple C-pointer
// when NA_DEBUG is 0.
//
// Authors note:
// The decision to include debugging information arose over many
// iterations. Mainly NAString was the cause of pushing debugging information
// deeply into the core functionality of NALib. They should be here to help
// with very fundamental datastructures like an NAByteArray but not to distract
// on higher levels. Further more, questions like "Is String Null-Terminated"
// should not be decided during (release) runtime. Therefore, the NAPtr
// implementation both supports but also limits the use of debug information
// depending whether you compile for debug or release.
//
// There would be the possibility to add such debug information to any pointer
// returned by naMalloc and similar functions. The information could be hidden
// in fields not visible to the user. But as naMalloc is basically just a
// wrapper for malloc, the two must be interchangable. Trying to use debug
// fields of a pointer which is not allocated with naMalloc may be more
// devastating than helpful. Therefore, the NAPtr struct was added which
// provides both a useful extension to differentiate between const and mutable
// as well as additional debugging functions for all who want to really make
// sure their code is doing what is expected.
//
// If you simply want to store a pointer without any knowledge of const or
// mutable and without any debugging information, just use a normal C-pointer.





// The following macros and functions are only available when debugging and
// are defined to be helper functions. They are needed when debugging to
// tag NAPtr values with various information.
#if NA_DEBUG
  #define NA_PTR_CONST_DATA                 0x01
  #define NA_PTR_CLEANED                    0x02
#endif




NA_IDEF NAPtr naMakePtrNull(){
  NAPtr ptr;
  ptr.data.d = NA_NULL;
  #if NA_DEBUG
    ptr.debugFlags = NA_ZERO; // Do not mark a null pointer as const.
                              // Otherwise many more errors will spawn.
  #endif
  return ptr;
}



NA_IDEF void naCleanupPtr(NAPtr* ptr, NAMutator destructor){
  #if NA_DEBUG
    if(ptr->debugFlags & NA_PTR_CLEANED)
      naError("NAPtr has already been cleaned once.");
    if(destructor && ptr->debugFlags & NA_PTR_CONST_DATA)
      naError("Calling a destructor on const data. This smells fishy.");
  #endif
  
  if(destructor)
    destructor(ptr->data.d);
  
  #if NA_DEBUG
    ptr->debugFlags |= NA_PTR_CLEANED;
  #endif
}



NA_IDEF NAPtr naMakePtrWithDataConst(const void* data){
  NAPtr ptr;
  ptr.data.constd = data;
  #if NA_DEBUG
    ptr.debugFlags = NA_PTR_CONST_DATA;
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithDataMutable(void* data){
  NAPtr ptr;
  ptr.data.d = data;
  #if NA_DEBUG
    ptr.debugFlags = NA_ZERO;
  #endif
  return ptr;
}



NA_IDEF const void* naGetPtrConst(NAPtr ptr){
  return ptr.data.constd;
}



NA_IDEF void* naGetPtrMutable(NAPtr ptr){
  return ptr.data.d;
}



NA_IDEF NABool naIsPtrValid(NAPtr ptr){
  return ptr.data.d != NA_NULL;
}



NA_IDEF NABool naIsPtrConst(NAPtr ptr){
  #if NA_DEBUG
    return (NABool)(ptr.debugFlags & NA_PTR_CONST_DATA);
  #else
    NA_UNUSED(ptr);
    return NA_FALSE;
  #endif
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
