
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// In this file, basic functionality of pointers is implemented.
// NALib usually uses the native pointers of C and C++ but for some types, a
// special kind of pointer is more appropriate.
// 
// Other programming languages are based on pointers with reference counting
// which point to objects which are automatically allocated and deallocated
// by a clever runtime-system. In NALib, such pointers are only used where
// necessary. The native pointers are fine for most purposes.
//
// If somebody ever wants to write a runtime-system for NALib, this file
// would be a good place to start.


#ifndef NA_POINTER_INCLUDED
#define NA_POINTER_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"


// These functions are the allocation functions of NALib. All functions in
// NALib use these functions when allocating memory on the heap. The basic
// function simply allocates using malloc. The naAllocateIfNull function takes
// an additional ptr argument. If ptr is NA_NULL, memory is allocated and the
// pointer to that memory is returned. If the pointer is not NA_NULL, nothing
// is allocated, the same pointer is returned.
//
// When size is negative, a certain number of bytes is appended to the memory
// block which will be initialized with binary zero but will not be visible to
// the programmer. The following holds true:
// - The additional bytes are all guaranteed to be initialized with binary
//   zero.
// - There are at least as many bytes appended as an address requires.
//   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
// - There are as many bytes allocated such that the total size is a
//   multiple of an address size, meaning 4 or 8 Bytes depending on the
//   system (32 Bit or 64 Bit).
// - The total size (including the desired space) is at minimum 2 times
//   the number of bytes needed for an address.
// - The part without the additional bytes might partially become
//   initialized with binary zero.
NA_IAPI void* naAllocate      (           NAInt size);
NA_IAPI void* naAllocateIfNull(void* ptr, NAInt size);
// Authors note:
//
// Having only one or two allocation function helps detecting basic memory
// errors. Note however that there does not exist any exception handling
// in NALib, meaning an error might be detected though not resolved. And in
// favor of simplicity, NALib will not get exception handling soon.



// NAPointer
// The following is the implementation of a pointer having a reference count,
// as used in many modern languages. In C and C++, this must be implemented
// manually.
//
// As having a reference count is not always useful in C and C++, only a few
// structs of NALib like the NAByteArray actually use the NAPointer structure.
//
// This implementation defines the ownership of the data and the struct itself
// using some bits of the refcount field.


// Opaque type. See explanation in readme.txt
typedef struct NAPointer NAPointer;


// Creates an NAPointer struct with a newly allocated memory block (heap)
// of the given size in bytes. The memory is uninitialized. The reference
// counter is automatically set to 1. This NAPointer is owning the allocated
// memory and will delete it automatically using free() when the reference
// count reaches zero. When the given pointer argument was a Null-Pointer,
// the NAPointer struct itself will be deleted automatically as well.
//
// You can also send negative sizes. See naAllocate for explanation.
NA_IAPI NAPointer* naCreatePointerWithSize(NAPointer* pointer,
                                                NAInt size);


// Creates an NAPointer struct with the content of the given buffer.
// WILL NOT COPY the contents. The given buffer can be located anywhere in
// memory and can either contain const or non-const (mutable) data. The buffer
// itself can either be owned or not owned by NAPointer: When this NAPointer
// takes the ownership of a buffer, it will delete the memory using free() when
// the reference counter reaches 0. You can not take ownership of const buffers
// like for example a string literal "Hello World".
// When the reference counter reaches 0, the NAPointer struct itself will also
// be freed automatically if the pointer argument of this constructor was
// NA_NULL.
//
// Note: Use the const method whenever you can. The compiler will help you
// detect const-safe-errors.
NA_IAPI NAPointer* naCreatePointerWithConstBuffer(   NAPointer* pointer,
                                                    const void* buffer);
NA_IAPI NAPointer* naCreatePointerWithMutableBuffer( NAPointer* pointer,
                                                          void* buffer,
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
// automatically. If this NAPointer was originally created with a Null-Pointer
// argument, the struct itself will be freed as well.
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
// Note that the second variant will return a non-const pointer even if the
// data stored originally was const. This is not safe but nontheless programmed
// in a completely legal way using union types. Note however that NAPointer
// remembers how you first created the data: const or non-const. When NDEBUG
// is undefined, an error will be emitted while trying to mutate const data.
//
// See implementation below for more details and to hear what the author was
// thinking.























// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NABinaryData.h"


NA_IDEF void* naAllocate(NAInt size){
  NAByte* ptr; // Declaration before implementation. Needed for C90.
  NAInt fullsize;
  #ifndef NDEBUG
    if(size == 0)
      naError("naAllocate", "size is zero.");
  #endif

  if(size>0){
    ptr = (NAByte*)malloc(size);
  }else{
    fullsize = -size + 2 * NA_SYSTEM_ADDRESS_BYTES - (-size % NA_SYSTEM_ADDRESS_BYTES);
    ptr = (NAByte*)malloc(fullsize);
    naNulln(&(ptr[fullsize - 2 * NA_SYSTEM_ADDRESS_BYTES]), 2 * NA_SYSTEM_ADDRESS_BYTES);
  }

  
  // This is the one place where the debug version differs from the non-debug
  // version.
  #ifndef NDEBUG
    if(ptr == NA_NULL){
      naCrash("naAllocate", "Out of memory.");
      // Also note that a special macro is checked for clang analyzer as
      // newer versions tend to complain a lot more about failed mallocs than
      // before.
      #ifdef __clang_analyzer__
        exit(EXIT_FAILURE);
      #endif
    }
  #endif
  
  return ptr;
}



NA_IDEF void* naAllocateIfNull(void* ptr, NAInt size){
  #ifndef NDEBUG
    if(size == 0)
      naError("naAllocateIfNull", "size is zero.");
  #endif

  if(ptr == NA_NULL){
    return naAllocate(size);
  }else{
    return ptr;
  }
}





// //////////////////////////
// NAPointer
// //////////////////////////

// Opaque type. typedef is above. See explanation in readme.txt
struct NAPointer{
  union{                // A union storing either ...
    const void* constd; // ... const data or ...
    void*       d;      // ... non-const (mutable) data.
  } data;
  NAInt refcount;       // Reference count.
                        // This field also stores some flags, embedded within
                        // the number.
};



// These are the flags stored in the refcount field. They occupy the most
// significant bits. Note that it would be possible to use a bit field for
// this but the author decided to use masks, as bit fields might introduce
// unnecessary algorithmic CPU operations when accessing refcount.
#define NA_POINTER_OWN_STRUCT     ((NAInt)1 << (NA_SYSTEM_ADDRESS_BITS - 1))
#define NA_POINTER_OWN_DATA       ((NAInt)1 << (NA_SYSTEM_ADDRESS_BITS - 2))
#define NA_POINTER_CONST_DATA     ((NAInt)1 << (NA_SYSTEM_ADDRESS_BITS - 3))
#define NA_POINTER_REFCOUNT_MASK  (~(   NA_POINTER_OWN_STRUCT\
                                      | NA_POINTER_OWN_DATA\
                                      | NA_POINTER_CONST_DATA))


// This is a helper function. It should be hidden as the user shall not use
// it as it is just a function which prepares the internal state of the
// NAPointer struct. Of course, in NALib hiding something has not much use
// but it shows where the hidden attribute would make sense if the function
// would not be inlined.
NA_IHLP NAPointer* naCreatePointerStruct(NAPointer* pointer){
  if(!pointer){
    pointer = (NAPointer*)naAllocate(sizeof(NAPointer));
    pointer->refcount = 1 | NA_POINTER_OWN_STRUCT;
  }else{
    pointer->refcount = 1;
  }
  return pointer;
}


NA_IDEF NAPointer* naCreatePointerWithSize(NAPointer* pointer, NAInt size){
  #ifndef NDEBUG
    if(size == 0)
      naError("naCreatePointerWithSize", "size is zero.");
  #endif
  pointer = naCreatePointerStruct(pointer);
  pointer->data.d = naAllocate(size);
  pointer->refcount |= NA_POINTER_OWN_DATA;
  return pointer;
}


NA_IDEF NAPointer* naCreatePointerWithConstBuffer(  NAPointer* pointer,
                                                   const void* buffer){
  #ifndef NDEBUG
    if(!buffer)
      naError("naCreatePointerWithConstBuffer", "buffer is Null-Pointer.");
  #endif
  pointer = naCreatePointerStruct(pointer);
  pointer->data.constd = buffer;
  pointer->refcount |= NA_POINTER_CONST_DATA;
  return pointer;
}



NA_IDEF NAPointer* naCreatePointerWithMutableBuffer(NAPointer* pointer,
                                                         void* buffer,
                                                        NABool takeownership){
  #ifndef NDEBUG
    if(!buffer)
      naError("naCreatePointerWithMutableBuffer", "buffer is Null-Pointer.");
    if((takeownership != NA_FALSE) && (takeownership != NA_TRUE))
      naError("naCreatePointerWithMutableBuffer", "invalid ownership");
  #endif
  pointer = naCreatePointerStruct(pointer);
  pointer->data.d = buffer;
  pointer->refcount |= (takeownership * NA_POINTER_OWN_DATA);
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
    if(pointer->refcount & NA_POINTER_OWN_DATA){free(pointer->data.d);}
    if(pointer->refcount & NA_POINTER_OWN_STRUCT){free(pointer);}
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist. You could write one though.
}



// Accessor and mutator
//
// Quite often at the very base of all implementations, you have to let go
// all the beautiful code designs learned in school and university. Sometimes,
// there is just no other way than cheating and bit fiddling.
//
// The problem here is that C has no concept of accessors and mutators. Even
// though the main difference is just the type returned, in C you can not
// overload functions and hence you can not hide the distinction to the user.
// There is the possibility to create two versions for each type storing either
// const or non-const data. This ensures type-safety, but forces the programmer
// to make the distinction at declaration level (which can become very
// cumbersome) and maybe even convert between the two variants which might be
// very costly and not beautiful at all.
//
// In this implementation, the author decided to use a union type storing
// either a const or a non-const data pointer. When calling a constructor,
// a specific flag defines what is stored.
//
// This is not entirely type-save, meaning you can use the mutator on const
// data. This is not beatuiful, but is much easier to write programs with, as
// the programmer has to differentiate between the two uses only when necessary.
// If the programmer uses a mutator on const data, an error will be emitted.
// You can of course ignore these errors and hope that the application does not
// crash, but you should really make sure no const data gets mutated.
//
// By the way:
// The const-flag is stored among other flags as part of the refcount field.
// This allows to create very fast code and condition-checks but yet saves lots
// of space while giving up only a small part of the value-range, which, as a
// sidenote, would in this implementation still be sufficient to theoretically
// fill the whole memory with references without ever overflowing.
//
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense. See NAPointer declaration.

NA_IDEF const void* naGetPointerConstData(const NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer){
      naCrash("naGetPointerData", "pointer is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return pointer->data.constd;
}

NA_IDEF void* naGetPointerMutableData(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer){
      naCrash("naGetPointerData", "pointer is Null-Pointer.");
      return NA_NULL;
    }else{
      if(pointer->refcount & NA_POINTER_CONST_DATA)
        naError("naGetPointerData", "Accessing const data as non-const.");
    }
  #endif
  return pointer->data.d;
}




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_POINTER_INCLUDED


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
