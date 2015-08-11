
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
  NADestructor desctructor;
};


typedef void* NATypeIdentifier;
extern NATypeIdentifier na_NAString_identifier;
extern NATypeIdentifier na_NAPointer_identifier;


NA_API void               naStartRuntime();
NA_API NATypeIdentifier   naManageRuntimeType(NATypeInfo* typeinfo);
NA_API void               naStopRuntime();

#define naNew(type)       naNewStruct(na_ ## type ## _identifier)
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
// The last two functions are equivalent when NDEBUG is defined.












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
  NALValue lvalue;
  NAUInt refcount;      // Reference count.
                        // This field also stores some flags, embedded within
                        // the number.
};

// Quite often at the very base of all implementations, you have to let go
// all the beautiful code designs learned in school and university. Sometimes,
// there is just no other way than cheating and bit fiddling. In NALib, the
// ownership of the data is stored using some bits of the refcount field.
//
// This allows to create very fast code and condition-checks but yet saves lots
// of space while giving up only a small part of the value-range, which, as a
// sidenote, would in this implementation still be sufficient to theoretically
// fill the whole memory with references without ever overflowing.

// This is the flags stored in the refcount field. It occupies the most
// significant bit. Note that it would be possible to use a bit field for
// this but the author decided to use masks, as bit fields might introduce
// unnecessary algorithmic CPU operations when accessing refcount.
//
// Note that in previous versions, there had been additional flags but these
// have been eliminated or placed elsewhere.
#define NA_POINTER_OWN_DATA       ((NAUInt)1 << (NA_SYSTEM_ADDRESS_BITS - 1))
#define NA_POINTER_REFCOUNT_MASK  (~(NA_POINTER_OWN_DATA))





NA_IDEF NAPointer* naNewPointerWithLValue(NALValue lvalue, NABool takeownership){
  NAPointer* pointer;
  #ifndef NDEBUG
  if(takeownership && naIsLValueConst(&lvalue))
    naError("naNewPointerWithLValue", "Taking ownership of const lvalue.");
  #endif
  pointer = naNew(NAPointer);
  pointer->lvalue = lvalue;
  pointer->refcount = 1 | (takeownership * NA_POINTER_OWN_DATA);
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
      if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == 0)
        naError("naRetainPointer", "Retaining NAPointer with a refcount of 0");
    }
  #endif
  pointer->refcount++;
  #ifndef NDEBUG
    // If refcount now suddenly becomes zero, there was either an error earlier
    // or the object has been retained too many times. Overflow. This is a very
    // serious situation as the flags will be changed.
    if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == 0)
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
      if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == 0)
        naError("naReleasePointer", "Releasing NAPointer with a refcount of 0");
    }
  #endif
  // Note that the author decided to always count to zero, even if it is clear
  // that the pointer will eventually be freed and the data will be lost in
  // nirvana. But often times in debugging, when retaining and releasing is not
  // done correctly, an NAPointer is released too often. When refcount is 0 and
  // NDEBUG is not defined, this can be detected!
  pointer->refcount--;
  if((pointer->refcount & NA_POINTER_REFCOUNT_MASK) == 0){
    naDelete(pointer);
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist. You could write one though.
}



NA_IDEF const void* naGetPointerConstData(const NAPointer* pointer){
  return naGetLValueConst(&(pointer->lvalue));
}



NA_IDEF void* naGetPointerMutableData(NAPointer* pointer){
  return naGetLValueMutable(&(pointer->lvalue));
}



NA_HIDEF void naClearPointer(NAPointer* pointer){
  if(pointer->refcount & NA_POINTER_OWN_DATA){free(naGetLValueMutable(&(pointer->lvalue)));}
}



NA_HIDEF void naPreparePointerRuntime(){
  NATypeInfo typeinfo;
  typeinfo.typesize = sizeof(NAPointer);
  typeinfo.desctructor = (NADestructor)naClearPointer;
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
