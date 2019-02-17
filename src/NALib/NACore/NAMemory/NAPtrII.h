// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



// At the core, an NAPtr stores a C-Pointer and the information whether it
// is const or mutable. It also stores information about the bytesize of the
// memory being pointed at and how it is null terminated. Even more, it stores,
// how the pointer had originally been allocated. All this information is just
// for debugging and can be omitted if necessary. When compiling with NDEBUG,
// no information is stored at all.

struct NAPtr{
  union{                        // A union storing either ...
    const void* constd;         // ... const data or ...
    void*       d;              // ... non-const (mutable) data.
  } data;
  #ifndef NDEBUG
    NAUInt flags;              // This field stores some flags.
  #endif
};
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense.
//
// Also note that NAPtr will be fully optimized to a simple C-pointer
// when NDEBUG is defined.
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
#ifndef NDEBUG
  #define NA_PTR_CONST_DATA                 0x01
  #define NA_PTR_CLEANED                    0x02
#endif




NA_IDEF NAPtr naMakePtrNull(){
  NAPtr ptr;
  ptr.data.d = NA_NULL;
  #ifndef NDEBUG
    ptr.flags = NA_ZERO; // Do not mark a null pointer as const. Otherwise many
                         // more errors will spawn.
  #endif
  return ptr;
}



// Note: bytesize 0 ist not allowed. Allowing it would introduce an if statement
// which should not exist at this low level function. A debug error will fire
// and you have to deal with zero sizes in higher level functions.
NA_IDEF NAPtr naMakePtrWithBytesize(NAInt bytesize){
  NAPtr ptr;
  #ifndef NDEBUG
    if(bytesize == NA_ZERO)
      naError("bytesize is zero.");
  #endif
  ptr.data.d = naMalloc(bytesize);
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
  #endif
  return ptr;
}



NA_IDEF void naCleanupPtr(NAPtr* ptr, NAMutator destructor){
  #ifndef NDEBUG
    if(ptr->flags & NA_PTR_CLEANED)
      naError("NAPtr has already been cleaned once.");
    if(destructor && ptr->flags & NA_PTR_CONST_DATA)
      naError("Calling a destructor on const data. This smells fishy.");
  #endif
  if(destructor){destructor(ptr->data.d);}
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #endif
}



NA_IDEF NAPtr naMakePtrWithDataConst(const void* data){
  NAPtr ptr;
  ptr.data.constd = data;
  #ifndef NDEBUG
    ptr.flags = NA_PTR_CONST_DATA;
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithDataMutable(void* data){
  NAPtr ptr;
  ptr.data.d = data;
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
  #endif
  return ptr;
}



NA_IDEF const void* naGetPtrConst(const NAPtr* ptr){
  #ifndef NDEBUG
    if(!ptr){
      naCrash("ptr is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return ptr->data.constd;
}



NA_IDEF void* naGetPtrMutable(NAPtr* ptr){
  #ifndef NDEBUG
    if(!ptr){
      naCrash("ptr is Null-Pointer.");
      return NA_NULL;
    }
    if(ptr->flags & NA_PTR_CONST_DATA)
      naError("Mutable access to const ptr.");
  #endif
  return ptr->data.d;
}



NA_IDEF NABool naIsPtrConst(const NAPtr* ptr){
  #ifndef NDEBUG
    return (ptr->flags & NA_PTR_CONST_DATA);
  #else
    NA_UNUSED(ptr);
    return NA_FALSE;
  #endif
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
