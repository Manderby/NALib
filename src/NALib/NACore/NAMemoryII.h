// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemory.h"



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <Windows.h>
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include "malloc/malloc.h"
#endif

#include "../NASystem.h"
#include "../NAMathOperators.h"
#include "NAValueHelper.h"


// //////////////////////////////////////
// Accessing informations about the memory system
// //////////////////////////////////////


NA_IDEF NAUInt naGetSystemMemoryPagesize(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (NAUInt)info.dwPageSize;
  #else
    return (NAUInt)sysconf(_SC_PAGESIZE);
  #endif
}



NA_IDEF NAUInt naGetSystemMemoryPagesizeMask(){
  return ~(NAUInt)(naGetSystemMemoryPagesize() - NA_ONE);
}



// Zero fill
//
// The following function returns for a given negative bytesize, how big a
// buffer must be, to include the appended zero bytes.
//
// The allocation functions of NALib expect a positive or negative bytesize.
// If the bytesize is negative, the absolute value is used to allocate
// sufficient space but a certain number of bytes is appended to the memory
// block which will be initialized with binary zero but will not be visible to
// the programmer.
// 
// The following holds true:
//
// - The additional bytes are all guaranteed to be initialized with binary
//   zero.
// - There are AT LEAST as many bytes appended as one address requires.
//   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
// - There are as many bytes allocated such that the total bytesize of the
//   allocated block is a multiple of an address bytesize, meaning 4 or 8 Bytes
//   depending on the system (32 Bit or 64 Bit).
// - The total bytesize (desired space plus null bytes) is at minimum 2 times
//   the number of bytes needed for an address.
// - When using naMalloc, the part without the additional bytes might partially
//   become initialized with binary zero.
// Example for a 32bit system:
// 0 is a zero-filled byte.
// x is a desired byte,
// z is a desired byte which initially gets overwritten with zero.
// 0:  0000 0000
// 1:  z000 0000
// 2:  zz00 0000
// 3:  zzz0 0000
// 4:  zzzz 0000
// 5:  xxxx z000 0000
// 8:  xxxx zzzz 0000
// 9:  xxxx xxxx z000 0000
// 12: xxxx xxxx zzzz 0000
// 13: xxxx xxxx xxxx z000 0000

NA_HIDEF NAInt naGetNullTerminationBytesize(NAInt bytesize){
  NAInt returnbytesize;
  #ifndef NDEBUG
    if((bytesize >= NA_ZERO))
      naError("naGetNullTerminationBytesize", "size is not negative");
    if(bytesize == NA_INVALID_MEMORY_BYTESIZE)
      naError("naGetNullTerminationBytesize", "invalid size given");
  #endif
  returnbytesize = (-bytesize - NA_ONE) + (NA_SYSTEM_ADDRESS_BYTES << 1) - ((-bytesize - NA_ONE) % NA_SYSTEM_ADDRESS_BYTES);
  #ifndef NDEBUG
    if(returnbytesize < NA_ZERO)
      naError("naGetNullTerminationBytesize", "given negative size is too close to the minimal integer value");
  #endif
  return returnbytesize;
}








// ////////////////////////////////////////////
// Basic Memory allocation and freeing
// ////////////////////////////////////////////




NA_IDEF void* naMalloc(NAInt bytesize){
  void* ptr;
  // ptr is declared as NAByte to simplify accessing individual bytes later
  // in this functions.

  #ifndef NDEBUG
    if(bytesize == NA_ZERO)
      {naCrash("naMalloc", "size is zero."); return NA_NULL;}
    if(bytesize < NA_ZERO)
      naError("naMalloc", "size is negative.");
  #endif

  ptr = malloc((size_t)bytesize);

  #ifndef NDEBUG
  if (!ptr)
    {naCrash("naMalloc", "Out of memory"); return NA_NULL;}
  #endif

  return ptr;
}


#undef naAlloc
#define naAlloc(type) (type*)naMalloc(sizeof(type))



NA_IDEF void naFree(void* ptr){
  free(ptr);
}



NA_IDEF void* naMallocAligned(NAUInt bytesize, NAUInt align){
  void* retptr;
  // Usually, aligned memory can be created in unix like systems using the
  // following three methods. Unfortunately, none of them work reliably on
  // Mac OS X.
  // - aligned_alloc under C11 is unreliable when using clang
  // - posix_memalign returns misaligned pointers in Snow Leopard
  // - malloc_zone_memalign same thing as posix_memalign
  // Therefore, a custom implementation is used which is costly but what
  // the hell!
    

  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    retptr = _aligned_malloc(bytesize, align);
  #else
    // Allocate a full align and a pointer more than required.
    void* mem = malloc(bytesize + align + sizeof(void*));
    // make a pointer point to the first byte being aligned within the memory
    // allocated in mem which comes after align bytes and a pointer size.
    void** ptr = (void**)((size_t)((NAByte*)mem + align + sizeof(void*)) & ~(align - NA_ONE));
    // Store the pointer to the original allocation pointer in the element
    // right before the first returned byte.
    ptr[-1] = mem;
    retptr = ptr;
  #endif
  
  #ifndef NDEBUG
    if(((NAUInt)retptr & (NAUInt)(align - NA_ONE)) != NA_ZERO)
      naError("naMallocAligned", "pointer unaligned.");
  #endif
  
  return retptr;
}



NA_IDEF void* naMallocPageAligned(NAUInt bytesize){
  // Note that due to some strange reason, aligned_alloc and valloc not work
  // with clang. Therefore, a simple call to naMallocAligned is used.
  return naMallocAligned(bytesize, naGetSystemMemoryPagesize());
}



NA_IDEF void naFreeAligned(void* ptr){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    _aligned_free(ptr);
  #else
    free(((void**)ptr)[-1]);
  #endif
}








// //////////////////////////
// NARuntime
// //////////////////////////



// This needs to be in this inline implementation file as it needs to be
// accessible to the compiler at all times.
struct NATypeInfo{
  void*             curpool;    // The actual type of this entry is hidden.
  NAUInt            typesize;
  NAMutator         destructor;
  NABool            refcounting;
  #ifndef NDEBUG
    const char*     typename;
  #endif
};



#undef NA_EXTERN_RUNTIME_TYPE
#define NA_EXTERN_RUNTIME_TYPE(type)\
  extern NATypeInfo na_ ## type ## _typeinfo



#undef NA_RUNTIME_TYPE
#ifndef NDEBUG
  #define NA_RUNTIME_TYPE(type, destructor, refcounting)\
    NATypeInfo na_ ## type ## _typeinfo =\
    {NA_NULL,\
    naSizeof(type),\
    (NAMutator)destructor,\
    refcounting,\
    #type}
#else
  #define NA_RUNTIME_TYPE(type, destructor, refcounting)\
    NATypeInfo na_ ## type ## _typeinfo =\
    {NA_NULL,\
    naSizeof(type),\
    (NAMutator)destructor,\
    refcounting}
#endif



#undef naNew
#define naNew(type) (type*)naNewStruct(&na_ ## type ## _typeinfo)
// If you experience an error here with naNew: Have you marked your type with
// NA_RUNTIME_TYPE? See NA_RUNTIME_TYPE below.



NA_API void*        naNewStruct(NATypeInfo* typeidentifier);






// //////////////////////////
// NARefCount
// //////////////////////////

struct NARefCount{
  NAUInt count;      // Reference count.
  #ifndef NDEBUG
    NAUInt dummy;
  #endif
};

// The count field also stores two cleanup hints in the 2*3 bits on the
// higher endian side. This is necessary due to the automatic deletion of a
// pointer and the struct the refcount is stored in when refcount reaches zero.
// It could be stored in a separate flag but the author decided to do it with
// a mask.
//
// This means that not the full range of the count integer can be used for
// reference counting. More precisely, the integer falls 6 Bits short. So in a
// 32 bit system, you can have a max of 2^26 = 67 million references and on a
// 64 bit system, you can have a max of 2^58 = [insert huge number here] refs. 
//
// Note that when NDEBUG is undefined, there are additional flags which make
// debugging easier and also serve to detect hard to find memory bugs. The
// dummy field stores a very specific number which should never be overwritten.
// If it is overwritten or otherwise not correct, the NARefCount has been used
// wrongly.


#define NA_REFCOUNT_DUMMY_VALUE (NAUInt)0xaaaaaaaaaaaaaaaaLL



NA_HIDEF NAUInt naGetRefCountCount(const NARefCount* refcount){
  return refcount->count;
}



NA_IDEF NARefCount* naInitRefCount(NARefCount* refcount){
  refcount->count = 1;
  #ifndef NDEBUG
    refcount->dummy = NA_REFCOUNT_DUMMY_VALUE;
  #endif
  return refcount;
}



NA_IDEF NARefCount* naRetainRefCount(NARefCount* refcount){
  #ifndef NDEBUG
    if(!refcount){
      naCrash("naRetainRefCount", "refcount is Null-Pointer.");
      return NA_NULL;
    }else{
      if(refcount->dummy != NA_REFCOUNT_DUMMY_VALUE)
        naError("naRetainRefCount", "Consistency problem: dummy value wrong. Is NARefCount really defined as the first field of this struct?");
      // The next test can detect some erroneous behaviour in the code. Note
      // however that most likely the true cause of the error did occur long
      // before reaching here.
      if(refcount->count == NA_ZERO)
        {naCrash("naRetainRefCount", "Retaining NARefCount with a count of 0"); return NA_NULL;}
    }
  #endif
  refcount->count++;
  #ifndef NDEBUG
    // If refcount now suddenly becomes zero, there was either an error earlier
    // or the object has been retained too many times. Overflow.
    if(refcount->count == NA_ZERO)
      naError("naRetainRefCount", "Reference count overflow");
  #endif
  return refcount;
}



NA_IDEF void naReleaseRefCount(NARefCount* refcount, void* data, NAMutator destructor){
  #ifndef NDEBUG
    if(!refcount){
      naCrash("naReleaseRefCount", "refcount is Null-Pointer.");
      return;
    }
      if(refcount->dummy != NA_REFCOUNT_DUMMY_VALUE)
        naError("naReleaseRefCount", "Consistency problem: dummy value wrong. Is NARefCount really defined as the first field of this struct?");
    // The next test can detect some erroneous behaviour in the code. Note
    // however that most likely the true cause of the error did occur long
    // before reaching here.
    if(refcount->count == NA_ZERO)
      {naCrash("naReleaseRefCount", "Releasing NARefCount with a count of 0"); return;}
  #endif
  // Note that the author decided to always count to zero, even if it is clear
  // that the pointer will eventually be freed and the data will be lost in
  // nirvana. But often times in debugging, when retaining and releasing is not
  // done correctly, an NAPointer is released too often. When refcount is 0 and
  // NDEBUG is not defined, this can be detected!
  refcount->count--;

  if(refcount->count == NA_ZERO){    
    // Call the destructor on the data if available.
    if(destructor){destructor(data);}
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist and must be implemented manually. NARuntime is a small
  // example of such a system.
  return;
}




// //////////////////////////
// NAPtr
// //////////////////////////

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




NA_IDEF NAPtr naMakeNullPtr(){
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
      naError("naMakePtrWithBytesize", "bytesize is zero.");
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
      naError("naCleanupPtr", "NAPtr has already been cleaned once.");
    if(destructor && ptr->flags & NA_PTR_CONST_DATA)
      naError("naCleanupPtr", "Calling a destructor on const data. This smells fishy.");
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
      naCrash("naGetPtrConst", "ptr is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return ptr->data.constd;
}



NA_IDEF void* naGetPtrMutable(NAPtr* ptr){
  #ifndef NDEBUG
    if(!ptr){
      naCrash("naGetPtrMutable", "ptr is Null-Pointer.");
      return NA_NULL;
    }
    if(ptr->flags & NA_PTR_CONST_DATA)
      naError("naGetPtrMutable", "Mutable access to const ptr.");
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








// //////////////////////////
// NASmartPtr
// //////////////////////////

struct NASmartPtr{
  NARefCount refcount;
  NAPtr ptr;
};





NA_IDEF NASmartPtr* naInitSmartPtrConst(NASmartPtr* sptr, const void* data){
  sptr->ptr = naMakePtrWithDataConst(data);
  naInitRefCount(&(sptr->refcount));
  return sptr;
}



NA_IDEF NASmartPtr* naInitSmartPtrMutable(NASmartPtr* sptr, void* data){
  sptr->ptr = naMakePtrWithDataMutable(data);
  naInitRefCount(&(sptr->refcount));
  return sptr;
}



NA_IDEF NASmartPtr* naRetainSmartPtr(NASmartPtr* sptr){
  return (NASmartPtr*)naRetainRefCount(&(sptr->refcount));
}



NA_IDEF void naReleaseSmartPtr(NASmartPtr* sptr, NAMutator destructor, NABool onlydata){
  #ifndef NDEBUG
    if(!sptr){
      naCrash("naReleaseSmartPtr", "sptr is Null-Pointer.");
      return;
    }
  #endif
  if(onlydata){
    naReleaseRefCount(&(sptr->refcount), naGetPtrMutable(&(sptr->ptr)), destructor);
  }else{
    naReleaseRefCount(&(sptr->refcount), &(sptr->refcount), destructor);
  }
}



NA_IDEF const void* naGetSmartPtrConst(const NASmartPtr* sptr){
  return naGetPtrConst(&(sptr->ptr));
}



NA_IDEF void* naGetSmartPtrMutable(NASmartPtr* sptr){
  return naGetPtrMutable(&(sptr->ptr));
}








// //////////////////////////
// NAPointer
// //////////////////////////

// An NAPointer stores a pointer with a reference count. You can increase and
// decrease that reference count and the pointer will automatically be erased
// when the reference count reaches zero. How it will be erased can be defined
// upon creation of the NAPointer.

struct NAPointer{
  NASmartPtr sptr;
  NAMutator destructor;
};
NA_EXTERN_RUNTIME_TYPE(NAPointer);



NA_IDEF NAPointer* naNewPointerConst(const void* data){
  NAPointer* pointer;
  pointer = naNew(NAPointer);
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refcount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrConst(&(pointer->sptr), data);
  pointer->destructor = NA_NULL;
  return pointer;
}



NA_IDEF NAPointer* naNewPointerMutable(void* data, NAMutator destructor){
  NAPointer* pointer;
  pointer = naNew(NAPointer);
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refcount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrMutable(&(pointer->sptr), data);
  pointer->destructor = destructor;
  return pointer;
}



NA_IDEF NAPointer* naRetainPointer(NAPointer* pointer){
  return (NAPointer*)naRetainSmartPtr(&(pointer->sptr));
}



NA_HAPI void naDestructPointer(NAPointer* pointer);



NA_IDEF void naReleasePointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer){
      naCrash("naReleasePointer", "pointer is Null-Pointer.");
      return;
    }
  #endif
  naReleaseSmartPtr(&(pointer->sptr), (NAMutator)naDestructPointer, NA_FALSE);
}




NA_IDEF const void* naGetPointerConst(const NAPointer* pointer){
  return naGetSmartPtrConst(&(pointer->sptr));
}



NA_IDEF void* naGetPointerMutable(NAPointer* pointer){
  return naGetSmartPtrMutable(&(pointer->sptr));
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
