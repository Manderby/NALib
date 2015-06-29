
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// In this file, basic functionality of memory allocation and pointers is
// implemented.
//
// NALib usually uses the native pointers of C and C++ but for some types,
// special kinds of pointers are more appropriate.
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


// ZERO FILL RULES
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
//
// The following function returns for a given negative size, how big a buffer
// must be to include the appended zero bytes.
NA_IAPI NAUInt naGetNullTerminationSize(NAInt size);

// These functions are the allocation functions of NALib. All functions in
// NALib use these functions when allocating memory on the heap. The basic
// function simply allocates using malloc. The naAllocateIfNull function takes
// an additional ptr argument. If ptr is NA_NULL, memory is allocated and the
// pointer to that memory is returned. If the pointer is not NA_NULL, nothing
// is allocated, the same pointer is returned.
//
NA_IAPI void* naAllocate      (           NAInt size);
NA_IAPI void* naAllocateIfNull(void* ptr, NAInt size);
NA_IAPI void* naAllocatePageAligned(      NAInt size);
// Authors note:
//
// Having only one or two allocation function helps detecting basic memory
// errors. Note however that there does not exist any exception handling
// in NALib, meaning an error might be detected though not resolved. And in
// favor of simplicity, NALib will not get exception handling soon.

// The following macro is used in all NALib structs where creation is
// necessary. This macro should not be used by other people as it might
// (might!) be the start of a future garbage collection implementation.
#define naAllocNALibStruct(ptr, structname)\
  (structname*)naAllocateIfNull(ptr, sizeof(structname))



// ////////////////////////
// NALValue
//
// Certain structs like NAList or NAPointer distinguish between const and
// non-const (mutable) data. The NALValue struct delivers this
// distinction by providing both an Accessor or a Mutator to a pointer.
//
// Furthermore, NALValue allows you to tag a pointer with various flags
// like if it denotes an array or not and whether that array is null-terminated
// or not.
//
// When compiling with NDEBUG, NALValue will optimize to a simpe C-Pointer.
// No tags will be available and no code is executed. But during debugging,
// using NALValue can greatly improve your code-safety.
//
// The problem is that C has no concept of how to specifically handle a
// pointer, and especially how to use accessors and mutators. Even
// though the main difference is just the type returned, in C you can not
// overload functions and hence you can not hide the distinction to the user.
// There is the possibility to create two versions for each struct type storing
// either const or non-const data. This ensures type-safety, but forces the
// programmer to make the distinction at declaration level (which can become
// very cumbersome) and maybe even force him to convert between the two
// variants which might be very costly and not beautiful at all.
//
// In this implementation, the author decided to use a union type storing
// either a const or a non-const data pointer. Furthermore, when NDEBUG is
// not defined, an additional flag is stored with the pointer defining if
// the pointer stores const or non-const data. With this, NALib is able to
// detect const errors during debugging. When NDEBUG is defined, this flag
// will not exists an no checks will be performed whatsoever, hence always
// assuming a mutable pointer.
//
// This idea is not entirely type-save and not too beautiful as well, but is
// much easier to write programs with, as the programmer has to differentiate
// between the two uses only when necessary.
//
// If the programmer uses a mutator on const data, an error will be emitted.
// You can of course ignore these errors and hope that the application does not
// crash, but you should really make sure no const data gets mutated.
//
// Note that when NDEBUG is defined, this struct will be completely optimized
// out by modern compilers, resulting in nothing but a simple memory pointer.

// Opaque type. See explanation in readme.txt
typedef struct NALValue NALValue;

// Fills the given NALValue struct with either a const or a non-const
// pointer.
NA_IAPI void naFillLValueConst  (NALValue* lvalue, const void* data);
NA_IAPI void naFillLValueMutable(NALValue* lvalue,       void* data);
NA_IAPI void naFillLValueSub    ( NALValue* dstlvalue,
                                  const NALValue* srclvalue,
                                  NAUInt offset,
                                  NAUInt size);

// The following two functions return either a const or a mutable pointer.
//
// Beware that the second variant will return a non-const pointer even if the
// data stored originally was const. When NDEBUG is not defined, NALib will
// check if such an invalid access of const data is occuring and will emit a
// warning. If the content of the pointer is a NULL pointer, no warning will
// be emitted.
//
// When NDEBUG is defined, the two functions are equal and no test will be
// performed whatsoever.
NA_IAPI const void* naGetLValueConst(      const NALValue* lvalue);
NA_IAPI       void* naGetLValueMutable(          NALValue* lvalue);
NA_IAPI const void* naGetLValueOffsetConst(const NALValue* lvalue, NAUInt indx);
NA_IAPI       void* naGetLValueOffsetMutable(    NALValue* lvalue, NAUInt indx);

// The following functions are only available when debugging.
#ifndef NDEBUG
  NA_IAPI void naMarkLValueWithVisibleSize(NALValue* lvalue, NAUInt visiblesize);
  NA_IAPI void naMarkLValueWithAccessibleSize(NALValue* lvalue, NAUInt visiblesize, NABool nullterminated);
  NA_IAPI NABool naIsLValueNullTerminated(const NALValue* lvalue);
#endif

// Returns NA_TRUE, if the pointer stores const data. This function only is
// useful when debugging. When NDEBUG is defined, this function always returns
// NA_FALSE.
NA_IAPI NABool naIsLValueConst(NALValue* lvalue);




// ////////////////////////
// NAPointer
// The following is the implementation of a pointer having a reference count,
// as used in many modern languages. In C and C++, this must be implemented
// manually.
//
// As having a reference count is not always useful in C and C++, only a few
// structs of NALib like the NAByteArray actually use the NAPointer structure.
//
// Quite often at the very base of all implementations, you have to let go
// all the beautiful code designs learned in school and university. Sometimes,
// there is just no other way than cheating and bit fiddling.
//
// This implementation defines the ownership of the data and the struct itself
// using some bits of the refcount field.
// This allows to create very fast code and condition-checks but yet saves lots
// of space while giving up only a small part of the value-range, which, as a
// sidenote, would in this implementation still be sufficient to theoretically
// fill the whole memory with references without ever overflowing.


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

NA_IAPI NALValue naGetPointerLValue(const NAPointer* pointer);





















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NABinaryData.h"



NA_IAPI NAUInt naGetNullTerminationSize(NAInt size){
  #ifndef NDEBUG
    if(size >= 0)
      naError("naGetNullTerminationSize", "size is positive");
  #endif
  return -size + 2 * NA_SYSTEM_ADDRESS_BYTES - (-size % NA_SYSTEM_ADDRESS_BYTES);
}




NA_IDEF void* naAllocate(NAInt size){
  NAByte* ptr; // Declaration before implementation. Needed for C90.
  NAInt fullsize;
  #ifndef NDEBUG
    if(size == 0)
      naError("naAllocate", "size is zero.");
  #endif

  if(size>0){
    #ifndef NDEBUG
    #endif
    ptr = (NAByte*)malloc((size_t)size);
  }else{
    #ifndef NDEBUG
      if(size == NA_INT_MIN)
        naError("naAllocate", "given negative size owerflows NAInt type.");
    #endif
    fullsize = naGetNullTerminationSize(size);
    #ifndef NDEBUG
      if(fullsize < -size)
        naError("naAllocate", "given size including zero filled endbytes overflows NAInt type.");
    #endif
    ptr = (NAByte*)malloc((size_t)fullsize);
    *(NAUInt*)(&(ptr[fullsize - 2 * NA_SYSTEM_ADDRESS_BYTES])) = 0;
    *(NAUInt*)(&(ptr[fullsize - 1 * NA_SYSTEM_ADDRESS_BYTES])) = 0;
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




NA_IDEF void* naAllocatePageAligned(NAInt size){
  #ifdef NA_C11
    return aligned_alloc(size, naGetSystemMemoryPageSize());
  #else
    return valloc(size);
  #endif
}




// //////////////////////////
// NALValue
// //////////////////////////

// Opaque type. typedef is above. See explanation in readme.txt
struct NALValue{
  union{                // A union storing either ...
    const void* constd; // ... const data or ...
    void*       d;      // ... non-const (mutable) data.
  } data;
  #ifndef NDEBUG
    NAUInt flags;               // This field stores some flags.
    NAUInt visiblesize;         // nof bytes of the visible byte storage
    NAUInt accessiblebytecount; // nof bytes of the accessible byte storage
  #endif
};
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense.
// Also note that NALValue will be fully optimized to a simple C-pointer
// when NDEBUG is defined.

#ifndef NDEBUG
  #define NA_LVALUE_CONST_DATA                0x01
  #define NA_LVALUE_HAS_VISIBLE_BYTECOUNT     0x02
  #define NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT  0x04
  #define NA_LVALUE_NULL_TERMINATED           0x08
#endif



NA_IDEF void naFillLValueConst(NALValue* lvalue, const void* data){
  lvalue->data.constd = data;
  #ifndef NDEBUG
    lvalue->flags = NA_LVALUE_CONST_DATA;
  #endif
}



NA_IDEF void naFillLValueMutable(NALValue* lvalue, void* data){
  lvalue->data.d = data;
  #ifndef NDEBUG
    lvalue->flags = 0;
  #endif
}



NA_IDEF void naFillLValueSub(NALValue* dstlvalue, const NALValue* srclvalue, NAUInt offset, NAUInt size){
  dstlvalue->data.constd = naGetLValueOffsetConst(srclvalue, offset);
  #ifndef NDEBUG
    dstlvalue->flags = srclvalue->flags;
    dstlvalue->visiblesize = size;
    if(offset + size < srclvalue->visiblesize){
      dstlvalue->flags &= ~NA_LVALUE_NULL_TERMINATED;
      dstlvalue->accessiblebytecount = dstlvalue->visiblesize;
    }else if(offset + size > srclvalue->visiblesize){
      naError("naFillLValueSub", "new offset and size overflows storage");
    }else{
      dstlvalue->accessiblebytecount -= offset;
    }
  #else
    NA_UNUSED(size);
  #endif
}



NA_IDEF const void* naGetLValueConst(const NALValue* lvalue){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueConst", "lvalue is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return lvalue->data.constd;
}



NA_IDEF void* naGetLValueMutable(NALValue* lvalue){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueMutable", "lvalue is Null-Pointer.");
      return NA_NULL;
    }
    // An NALValue often stores Null-Pointers. In that case, just return
    // Null and do not emit any warning.
    if(lvalue->data.d == NA_NULL){return NA_NULL;}
    if(lvalue->flags & NA_LVALUE_CONST_DATA)
      naError("naGetLValueMutable", "Accessing const data as non-const.");
  #endif
  return lvalue->data.d;
}



NA_IDEF const void* naGetLValueOffsetConst(const NALValue* lvalue, NAUInt indx){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueOffsetConst", "lvalue is Null-Pointer.");
      return NA_NULL;
    }
    if((lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT) && (indx >= lvalue->visiblesize))
      if((lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT) && (indx >= lvalue->accessiblebytecount))
        naError("naGetLValueOffsetConst", "index out of accessible bounds");
      else
        naError("naGetLValueOffsetConst", "index out of visible bounds");
  #endif
  return &(((const NAByte*)(lvalue->data.constd))[indx]);
}



NA_IDEF void* naGetLValueOffsetMutable(NALValue* lvalue, NAUInt indx){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueOffsetMutable", "lvalue is Null-Pointer.");
      return NA_NULL;
    }
    // An NALValue often stores Null-Pointers. In that case, just return
    // Null and do not emit any warning.
    if(lvalue->data.d == NA_NULL){return NA_NULL;}
    if(lvalue->flags & NA_LVALUE_CONST_DATA)
      naError("naGetLValueOffsetMutable", "Accessing const data as non-const.");
    if((lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT) && (indx >= lvalue->visiblesize))
      if((lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT) && (indx >= lvalue->accessiblebytecount))
        naError("naGetLValueOffsetMutable", "index out of accessible bounds");
      else
        naError("naGetLValueOffsetMutable", "index out of visible bounds");
  #endif
  return &(((NAByte*)(lvalue->data.d))[indx]);
}


#ifndef NDEBUG



  NA_IDEF void naMarkLValueWithVisibleSize(NALValue* lvalue, NAUInt visiblesize){
    if(!lvalue)
      {naCrash("naMarkLValueWithVisibleSize", "lvalue was null"); return;}
    if(lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT)
      naError("naMarkLValueWithVisibleSize", "visible size already marked");
    lvalue->flags |= NA_LVALUE_HAS_VISIBLE_BYTECOUNT;
    lvalue->visiblesize = visiblesize;
  }
  
  

  NA_IDEF void naMarkLValueWithAccessibleSize(NALValue* lvalue, NAUInt accessiblesize, NABool nullterminated){
    if(!lvalue)
      {naCrash("naMarkLValueWithVisibleSize", "lvalue was null"); return;}
    if(!(lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT))
      naError("naMarkLValueWithVisibleSize", "visible size must be marked first");
    if(lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT)
      naError("naMarkLValueWithVisibleSize", "accessible size already marked");
    lvalue->flags |= NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT;
    lvalue->accessiblebytecount = accessiblesize;
    if(nullterminated){lvalue->flags |= NA_LVALUE_NULL_TERMINATED;}
    NAUInt nullindx = lvalue->visiblesize;
    while(nullindx < lvalue->accessiblebytecount){
      if(((NAByte*)(lvalue->data.constd))[nullindx] != 0)
        naError("naMarkLValueWithVisibleSize", "promised null-termination is not null");
      nullindx++;
    }
  }



  NA_IDEF NABool naIsLValueNullTerminated(const NALValue* lvalue){
    if(!lvalue)
      {naCrash("naIsLValueNullTerminated", "lvalue was null"); return NA_FALSE;}
    if(!(lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT))
      naError("naIsLValueNullTerminated", "No size information present. Result is possibly wrong");
    return (lvalue->flags & NA_LVALUE_NULL_TERMINATED);
  }
  
  
  
#endif




NA_IDEF NABool naIsLValueConst(NALValue* lvalue){
  #ifndef NDEBUG
    return (lvalue->flags & NA_LVALUE_CONST_DATA);
  #else
    NA_UNUSED(lvalue);
    return NA_FALSE;
  #endif
}







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



// These are the flags stored in the refcount field. They occupy the most
// significant bits. Note that it would be possible to use a bit field for
// this but the author decided to use masks, as bit fields might introduce
// unnecessary algorithmic CPU operations when accessing refcount.
#define NA_POINTER_OWN_STRUCT     ((NAUInt)1 << (NA_SYSTEM_ADDRESS_BITS - 1))
#define NA_POINTER_OWN_DATA       ((NAUInt)1 << (NA_SYSTEM_ADDRESS_BITS - 2))
#define NA_POINTER_REFCOUNT_MASK  (~(   NA_POINTER_OWN_STRUCT\
                                      | NA_POINTER_OWN_DATA))


// This is a helper function. It should be hidden as the user shall not use
// it as it is just a function which prepares the internal state of the
// NAPointer struct. Of course, in NALib hiding something has not much use
// but it shows where the hidden attribute would make sense if the function
// would not be inlined.
NA_HIDEF NAPointer* naCreatePointerStruct(NAPointer* pointer){
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
  naFillLValueMutable(&(pointer->lvalue), naAllocate(size));
  #ifndef NDEBUG
    if(size<0){
      naMarkLValueWithVisibleSize(&(pointer->lvalue), -size);
      naMarkLValueWithAccessibleSize(&(pointer->lvalue), naGetNullTerminationSize(size), NA_TRUE);
    }else{
      naMarkLValueWithVisibleSize(&(pointer->lvalue), size);
      naMarkLValueWithAccessibleSize(&(pointer->lvalue), size, NA_FALSE);
    }
  #endif
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
  naFillLValueConst(&(pointer->lvalue), buffer);
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
  naFillLValueMutable(&(pointer->lvalue), buffer);
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
    if(pointer->refcount & NA_POINTER_OWN_DATA){free(naGetLValueMutable(&(pointer->lvalue)));}
    if(pointer->refcount & NA_POINTER_OWN_STRUCT){free(pointer);}
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


NA_IDEF NALValue naGetPointerLValue(const NAPointer* pointer){
  return pointer->lvalue;
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
