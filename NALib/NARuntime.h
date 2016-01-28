
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_RUNTIME_INCLUDED
#define NA_RUNTIME_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// ////////////////////////////
// In this file you find a small runtime system capable of handling memory
// pools for a few structs where using automatic allocation and deallocation
// has proven to be useful.
//
// You find both the NARuntime struct as well as the NAPointer struct, which
// is a reference counted pointer.



#include "NAMemory.h"



// Following is the runtime system. It is very raw at the moment. Will be
// enhanced in the near future.
//
// The most important two functions are naNew and naDelete.

typedef void  (*NATypeDestructor) (void *);

typedef struct NATypeInfo NATypeInfo;
struct NATypeInfo{
  NAUInt typesize;
  NATypeDestructor destructor;
};


typedef void* NATypeIdentifier;
extern NATypeIdentifier na_NAString_identifier;
extern NATypeIdentifier na_NAPointer_identifier;
extern NATypeIdentifier na_NAListElement_identifier;
extern NATypeIdentifier na_NAPNG_identifier;


NA_API void               naStartRuntime();
NA_API NATypeIdentifier   naManageRuntimeType(NATypeInfo* typeinfo);
NA_API void               naStopRuntime();

#define naNew(type)       (type*)naNewStruct(na_ ## type ## _identifier)
NA_API void*              naNewStruct(NATypeIdentifier typeidentifier);
NA_API void               naDelete(void* pointer);





typedef enum{
  NA_POINTER_CLEANUP_NONE            = 0x00,
  NA_POINTER_CLEANUP_FREE            = 0x01,
  NA_POINTER_CLEANUP_FREE_ALIGNED    = 0x02,
  NA_POINTER_CLEANUP_DELETE          = 0x03
} NAPointerCleanup;


// ////////////////////////
// NAPointer
//
// NALib usually uses the native pointers of C and C++ but for some types,
// special kinds of pointers are more appropriate. The following is the
// implementation of a pointer having a reference count, as used in many
// modern languages. In C and C++, this must be implemented manually.
//
// NAPointer stores a pointer which can be accessed by multiple codeblock, each
// "retaining" the NAPointer at start and "releasing" him at the end. When
// finally, no codeblock uses an NAPointer anymore, the pointer deallocates
// itself automatically.
//
// Having a reference count is not always useful in C and C++. Only a few
// structs of NALib like the NAByteArray actually use the NAPointer structure.
//
// Note that starting with NALib version 10, NAPointers require the NARuntime
// system.

// Opaque type. See explanation in readme.txt
typedef struct NAPointer NAPointer;

// Creates an NAPointer struct around the given data pointer.
//
// The NAPointer then allows to use reference counting on that pointer and
// will automatically deallocate both the data using the given deallocator
// as well as the provided data pointer itself according to the cleanup
// argument.
//
// If no deallocator is needed, you can send NA_NULL.
NA_IAPI NAPointer* naNewPointerConst(   const void* data);
NA_IAPI NAPointer* naNewPointerMutable(       void* data,
                                   NAPointerCleanup cleanup,
                                             NAFunc deallocator);

// Retains the given pointer. Meaning: There is one more codeblock which is
// using this NAPointer. This NAPointer will not be freed as long as that
// codeblock does not releases this NAPointer.
//
// Of course, someone else might erroneously release this NAPointer, but that's
// the risk you take with reference-counted pointers.
//
// Returns the pointer again. Makes it easier to write code like follows:
// referenceofmyvalue = naRetainPointer(myvalue);
NA_IAPI NAPointer* naRetainPointer(NAPointer* pointer);

// Releases the given NAPointer. If the refcount reaches 0, this NAPointer
// is no longer needed. If this NAPointer owns the data, it will be freed
// automatically. The struct itself will be deleted by the runtime system.
NA_IAPI void naReleasePointer(NAPointer* pointer);

// The following two functions return a pointer to the data. This function is
// not particularily beautiful when it comes to readability or writeability but
// it should nontheless be used because of this:
// - First, note that if the NAPointer struct would be truely opaque, there
//   would be no other way of accessing the data.
// - Second, this illustrates how accessors and mutators can be handeled in C
//   and C++: Simply create two functions where one returns a const pointer
//   (or a reference in C++), the other one returns a non-const pointer (or
//   reference). The first variant is an accessor, the second one a mutator.
NA_IAPI NABool      naIsPointerConst   (const NAPointer* pointer);
NA_IAPI const void* naGetPointerConst  (const NAPointer* pointer);
NA_IAPI       void* naGetPointerMutable(      NAPointer* pointer);
// Authors note:
// These two functions are equivalent when NDEBUG is defined.

// Additionally, you can access the data stored in the given pointer by
// getting one of the base memory structs:
//NA_IAPI NAPtr         naGetPointerPtr(         NAPointer* pointer);
//NA_IAPI NALValue      naGetPointerLValue(      NAPointer* pointer);
//NA_IAPI NAMemoryBlock naGetPointerMemoryBlock( NAPointer* pointer);
//NA_IAPI NACArray      naGetPointerCArray(      NAPointer* pointer);
//NA_IAPI NABuf         naGetPointerBuf(         NAPointer* pointer);













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
//
// The author decided to put everything from the NARuntime into the separate
// implementation file and to not provide inline functions.
//
// But NAPointer is completely inlined:


#include "NASystem.h"


// //////////////////////////
// NAPointer
// //////////////////////////

// Opaque type. typedef is above. See explanation in readme.txt
struct NAPointer{
  union{
    void*       d;
    const void* constd;
  } data;
  NAFunc deallocator;
  NAUInt refcount;      // Reference count. Including owning flags
};

// Starting with NALib version 14, the actual data is stored as a pointer.
// This means that it must be allocated elsewhere. But this means, any pointer
// can become a reference counted pointer.
//
// To distinguish, how the pointer shall be handeled upon deletion, a new
// enumeration NAPointerCleanup had been introduced.

#define NA_POINTER_CLEANUP_BITSHIFT   (NA_SYSTEM_ADDRESS_BITS - 2)
#define NA_POINTER_CONST_POINTER      (1LL << (NA_SYSTEM_ADDRESS_BITS - 3))
#define NA_POINTER_REFCOUNT_MASK      (NA_UINT_MAX << (NA_SYSTEM_ADDRESS_BITS - 3))







NA_IAPI NAPointer* naNewPointerConst(const void* dataptr){
  NAPointer* pointer;
  pointer = naNew(NAPointer);
  pointer->data.constd = dataptr;
  pointer->deallocator = NA_NULL;
  pointer->refcount = 1 | NA_POINTER_CONST_POINTER;
  return pointer;
}



NA_IAPI NAPointer* naNewPointerMutable(void* dataptr, NAPointerCleanup cleanup, NAFunc deallocator){
  NAPointer* pointer;
  pointer = naNew(NAPointer);
  pointer->data.d = dataptr;
  pointer->deallocator = deallocator;
  pointer->refcount = 1 | ((NAUInt)cleanup << NA_POINTER_CLEANUP_BITSHIFT);
  return pointer;
}





NA_IDEF NAPointer* naRetainPointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer){
      naCrash("naRetainPointer", "pointer is Null-Pointer.");
      return NA_NULL;
    }else{
      // The next test can detect some erroneous behaviour in the code. Note
      // however that most likely the true cause of the error did occur long
      // before reaching here.
      if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == NA_ZERO)
        naError("naRetainPointer", "Retaining NAPointer with a refcount of 0");
    }
  #endif
  pointer->refcount++;
  #ifndef NDEBUG
    // If refcount now suddenly becomes zero, there was either an error earlier
    // or the object has been retained too many times. Overflow.
    if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == NA_ZERO)
      naError("naRetainPointer", "Reference count overflow");
  #endif
  return pointer;
}



NA_IDEF void naReleasePointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer){
      naCrash("naReleasePointer", "pointer is Null-Pointer.");
      return;
    }else{
      // The next test can detect some erroneous behaviour in the code. Note
      // however that most likely the true cause of the error did occur long
      // before reaching here.
      if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == NA_ZERO)
        naError("naReleasePointer", "Releasing NAPointer with a refcount of 0");
    }
  #endif
  // Note that the author decided to always count to zero, even if it is clear
  // that the pointer will eventually be freed and the data will be lost in
  // nirvana. But often times in debugging, when retaining and releasing is not
  // done correctly, an NAPointer is released too often. When refcount is 0 and
  // NDEBUG is not defined, this can be detected!
  pointer->refcount--;
  if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == NA_ZERO){
    naDelete(pointer);
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist. NARuntime is a small example of such a system.
}



NA_HIDEF void naClearPointer(NAPointer* pointer){
  if(pointer->deallocator){
    pointer->deallocator(pointer->data.d);
  }
  switch((NAPointerCleanup)(pointer->refcount >> NA_POINTER_CLEANUP_BITSHIFT)){
  case NA_POINTER_CLEANUP_NONE:
    break;
  case NA_POINTER_CLEANUP_FREE:
    naFree(pointer->data.d);
    break;
  case NA_POINTER_CLEANUP_FREE_ALIGNED:
    naFreeAligned(pointer->data.d);
    break;
  case NA_POINTER_CLEANUP_DELETE:
    naDelete(pointer->data.d);
    break;
  }
}


NA_IDEF NABool naIsPointerConst(const NAPointer* pointer){
  return ((pointer->refcount & NA_POINTER_CONST_POINTER) == NA_POINTER_CONST_POINTER);
}



NA_IDEF const void* naGetPointerConst(const NAPointer* pointer){
  return pointer->data.constd;
}



NA_IDEF void* naGetPointerMutable(NAPointer* pointer){
  #ifndef NDEBUG
    if(naIsPointerConst(pointer))
      naError("naGetPointerMutable", "Accessing a const pointer");
  #endif
  return pointer->data.d;
}



NA_HIDEF void naPreparePointerRuntime(){
  NATypeInfo typeinfo;
  typeinfo.typesize = sizeof(NAPointer);
  typeinfo.destructor = (NATypeDestructor)naClearPointer;
  na_NAPointer_identifier = naManageRuntimeType(&typeinfo);
}





#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_RUNTIME_INCLUDED


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
