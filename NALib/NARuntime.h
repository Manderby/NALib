
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

typedef struct NATypeInfo NATypeInfo;
struct NATypeInfo{
  NAUInt typesize;
  NADestructor destructor;
};


typedef void* NATypeIdentifier;
extern NATypeIdentifier na_NAString_identifier;
extern NATypeIdentifier na_NAPointer_identifier;
extern NATypeIdentifier na_NAListElement_identifier;


NA_API void               naStartRuntime();
NA_API NATypeIdentifier   naManageRuntimeType(NATypeInfo* typeinfo);
NA_API void               naStopRuntime();

#define naNew(type)       (type*)naNewStruct(na_ ## type ## _identifier)
NA_API void*              naNewStruct(NATypeIdentifier typeidentifier);
NA_API void               naDelete(void* pointer);





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


// Creates an NAPointer struct with a newly allocated memory block (heap)
// of the given size in bytes. The memory is uninitialized. The reference
// counter is automatically set to 1. This NAPointer is owning the allocated
// memory and will delete it automatically using free() when the reference
// count reaches zero.
// You can also send negative sizes. See above for explanation.
NA_IAPI NAPointer* naNewPointerWithSize(NAInt size);

// Creates an NAPointer struct with the content of the given buffer.
// WILL NOT COPY the contents! The given buffer can be located anywhere in
// memory and can either contain const or non-const (mutable) data. A mutable
// buffer can either be owned or not owned by NAPointer: When this NAPointer
// takes the ownership of a mutable buffer, it will delete the memory using
// free() when the reference counter reaches 0. You can never take ownership
// of const buffers like for example a string literal "Hello World".
//
// Note: Use the const method whenever you can. The compiler and runtime system
// will help you detect const-safe-errors.
NA_IAPI NAPointer* naNewPointerWithLValue(    NALValue lvalue,
                                                NABool takeownership);

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
NA_IAPI const void* naGetPointerConstData  (const NAPointer* pointer);
NA_IAPI       void* naGetPointerMutableData(      NAPointer* pointer);
// Authors note:
// These two functions are equivalent when NDEBUG is defined.

// Additionally, you can access the data stored in the given pointer by
// getting one of the base memory structs:
NA_IAPI NAPtr         naGetPointerPtr(         NAPointer* pointer);
NA_IAPI NALValue      naGetPointerLValue(      NAPointer* pointer);
NA_IAPI NAMemoryBlock naGetPointerMemoryBlock( NAPointer* pointer);
NA_IAPI NACArray      naGetPointerCArray(      NAPointer* pointer);













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
//
// The author decided to put everything from the NARuntime into the separate
// implementation file and to not provide inline functions.
//
// But NAPointer is completely inlined:




// //////////////////////////
// NAPointer
// //////////////////////////

// Opaque type. typedef is above. See explanation in readme.txt
struct NAPointer{
  union{
    NAPtr         ptr;
    NALValue      lvalue;
    NAMemoryBlock memblock;
    NACArray      carray;
  } data;
  NAUInt refcount;      // Reference count.
  NAUInt flags;         // Various flags.
};

// Starting with NALib version 12, the author had to decide how to store the
// different base memory structs in an NAPointer. There is the possibility to
// store a pointer to such a struct which is very compact but would introduce
// a double-derefenciation, and the actual data had to be stored at another
// place, possibly again allocated with malloc.
//
// Therefore, the author decided to use a union-type to store the values
// directly. This is quite a bit costly in terms of memory consumption but
// as reference counted pointers are still a very rare thing in NALib, it
// seemd right to the author.
//
// Additionally, as this struct suddenly became way bigger than it was before
// version 12, the author decided, to separate refcount and flags. Before, they
// occupied the same integer and were filtered using bitmasks. Now, they occupy
// two integers but are easier to understand and debug.



// These are the flags stored in the flags field.
#define NA_POINTER_OWN_DATA         0x01
#define NA_POINTER_STORES_TYPESIZE  0x02
#define NA_POINTER_STORES_BYTESIZE  0x04





NA_IDEF NAPointer* naNewPointerWithPtr(NAPtr ptr, NABool takeownership){
  NAPointer* pointer;
  #ifndef NDEBUG
  if(takeownership && naIsPtrConst(&ptr))
    naError("naNewPointerWithPtr", "Taking ownership of const ptr.");
  #endif
  pointer = naNew(NAPointer);
  pointer->data.ptr = ptr;
  pointer->refcount = 1;
  pointer->flags = (takeownership * NA_POINTER_OWN_DATA);
  return pointer;
}



NA_IDEF NAPointer* naNewPointerWithLValue(NALValue lvalue, NABool takeownership){
  NAPointer* pointer;
  #ifndef NDEBUG
  if(takeownership && naIsLValueConst(&lvalue))
    naError("naNewPointerWithLValue", "Taking ownership of const lvalue.");
  #endif
  pointer = naNew(NAPointer);
  pointer->data.lvalue = lvalue;
  pointer->refcount = 1;
  pointer->flags = (takeownership * NA_POINTER_OWN_DATA) | NA_POINTER_STORES_TYPESIZE;
  return pointer;
}



NA_IDEF NAPointer* naNewPointerWithMemoryBlock(NAMemoryBlock memblock, NABool takeownership){
  NAPointer* pointer;
  #ifndef NDEBUG
  if(takeownership && naIsMemoryBlockConst(&memblock))
    naError("naNewPointerWithMemoryBlock", "Taking ownership of const memblock.");
  #endif
  pointer = naNew(NAPointer);
  pointer->data.memblock = memblock;
  pointer->refcount = 1;
  pointer->flags = (takeownership * NA_POINTER_OWN_DATA) | NA_POINTER_STORES_BYTESIZE;
  return pointer;
}



NA_IDEF NAPointer* naNewPointerWithCArray(NACArray carray, NABool takeownership){
  NAPointer* pointer;
  #ifndef NDEBUG
  if(takeownership && naIsCArrayConst(&carray))
    naError("naNewPointerWithCArray", "Taking ownership of const carray.");
  #endif
  pointer = naNew(NAPointer);
  pointer->data.carray = carray;
  pointer->refcount = 1;
  pointer->flags = (takeownership * NA_POINTER_OWN_DATA) | NA_POINTER_STORES_BYTESIZE | NA_POINTER_STORES_TYPESIZE;
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
      if(pointer->refcount == NA_ZERO)
        naError("naRetainPointer", "Retaining NAPointer with a refcount of 0");
    }
  #endif
  pointer->refcount++;
  #ifndef NDEBUG
    // If refcount now suddenly becomes zero, there was either an error earlier
    // or the object has been retained too many times. Overflow.
    if(pointer->refcount == NA_ZERO)
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
      if(pointer->refcount == NA_ZERO)
        naError("naReleasePointer", "Releasing NAPointer with a refcount of 0");
    }
  #endif
  // Note that the author decided to always count to zero, even if it is clear
  // that the pointer will eventually be freed and the data will be lost in
  // nirvana. But often times in debugging, when retaining and releasing is not
  // done correctly, an NAPointer is released too often. When refcount is 0 and
  // NDEBUG is not defined, this can be detected!
  pointer->refcount--;
  if(pointer->refcount == NA_ZERO){
    naDelete(pointer);
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist. NARuntime is a small example of such a system.
}



NA_IDEF const void* naGetPointerConstData(const NAPointer* pointer){
  return naGetPtrConst(&(pointer->data.ptr));
}



NA_IDEF void* naGetPointerMutableData(NAPointer* pointer){
  return naGetPtrMutable(&(pointer->data.ptr));
}



NA_IDEF NAPtr naGetPointerPtr(NAPointer* pointer){
  #ifndef NDEBUG
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerPtr", "This pointer stores an NALValue, not an NAPtr");
    if(!(pointer->flags & NA_POINTER_STORES_TYPESIZE) && (pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerPtr", "This pointer stores an NAMemoryBlock, not an NAPtr");
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && (pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerPtr", "This pointer stores an NACArray, not an NAPtr");
  #endif
  return pointer->data.ptr;
}



NA_IDEF NALValue naGetPointerLValue(NAPointer* pointer){
  #ifndef NDEBUG
    if(!(pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerLValue", "This pointer stores an NAPtr, not an NALValue");
    if(!(pointer->flags & NA_POINTER_STORES_TYPESIZE) && (pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerLValue", "This pointer stores an NAMemoryBlock, not an NALValue");
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && (pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerLValue", "This pointer stores an NACArray, not an NALValue");
  #endif
  return pointer->data.lvalue;
}



NA_IDEF NAMemoryBlock naGetPointerMemoryBlock(NAPointer* pointer){
  #ifndef NDEBUG
    if(!(pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerMemoryBlock", "This pointer stores an NAPtr, not an NAMemoryBlock");
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerMemoryBlock", "This pointer stores an NALValue, not an NAMemoryBlock");
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && (pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerMemoryBlock", "This pointer stores an NACArray, not an NAMemoryBlock");
  #endif
  return pointer->data.memblock;
}



NA_IDEF NACArray naGetPointerCArray(NAPointer* pointer){
  #ifndef NDEBUG
    if(!(pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerCArray", "This pointer stores an NAPtr, not an NACArray");
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerCArray", "This pointer stores an NALValue, not an NACArray");
    if((pointer->flags & NA_POINTER_STORES_TYPESIZE) && !(pointer->flags & NA_POINTER_STORES_BYTESIZE))
      naError("naGetPointerCArray", "This pointer stores an NALValue, not an NACArray");
  #endif
  return pointer->data.carray;
}




NA_HIDEF void naClearPointer(NAPointer* pointer){
  if(pointer->flags & NA_POINTER_OWN_DATA){
    free(naGetPtrMutable(&(pointer->data.ptr)));
  }
}



NA_HIDEF void naPreparePointerRuntime(){
  NATypeInfo typeinfo;
  typeinfo.typesize = sizeof(NAPointer);
  typeinfo.destructor = (NADestructor)naClearPointer;
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
