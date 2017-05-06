
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

// ////////////////////////////////////////////////////////
//
// In this file, basic functionality of memory handling is implemented. There
// are multiple levels on which you can operate.
// The following things can be found here:
//
// - Accessing core informations about the memory system
// - Basic Memory allocation and freeing.
// - Base memory structs NAPtr, NAMemoryBlock
// - NARefCount, NAPtr, NASmartPtr, NAPointer: Handling of pointers with or
//   without reference counting with automatic deletion.
// - Handling of inifinte pools for new and delete functions.
//
// ////////////////////////////////////////////////////////


#ifndef NA_MEMORY_INCLUDED
#define NA_MEMORY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"



// //////////////////////////////////////
// Accessing core informations about the memory system
// //////////////////////////////////////

// Returns the number of bytes used per memory page as well as a mask which
// you can combine with & (AND) with a pointer to get the page-aligned address
// of that pointer.
NA_IAPI NAUInt naGetSystemMemoryPagesize();
NA_IAPI NAUInt naGetSystemMemoryPagesizeMask();



// NALib defines the following index and size to be the predefined "invalids".
// Often times in NALib, indexes and sizes can be negative (see the API for
// more details). As in todays computers the twos complement is usually used
// to store negative integers, the minimal integer number is a good choice for
// an invalid index as it can not be transformed into a positive value. But
// also in other encodings, the most negative integer is probably never used
// by any programmer. Any other number than this invalid number might have a
// valid meaning in NALib.
#define NA_INVALID_MEMORY_INDEX     NA_INT_MIN
#define NA_INVALID_MEMORY_BYTESIZE  NA_INT_MIN


// As all memory functions expect an NAInt, we make it easier to write code
// with a custom sizeof macro
#define naSizeof(x) ((NAInt)sizeof(x))


// //////////////////////////////////////
// Basic Memory allocation and freeing
// //////////////////////////////////////



// The following functions are the allocation functions of NALib. All functions
// in NALib use these functions when allocating memory on the heap.
//
// naMalloc             Allocates the given size of bytes on the heap using
//                      malloc. If bytesize is negative, zero-filled bytes are
//                      appended at the end. See explanation below.
// naAlloc              This is a macro which expands to naMalloc but can be
//                      used to allocate enough space for a struct type. For
//                      exampe, you can simply write array = naAlloc(NAArray);
// naFree               Deallocates a pointer previously allocated with one of
//                      the above functions or the default C memory allocation
//                      functions like malloc.
//
// naMallocAligned      The given bytesize is allocated and the returned pointer
//                      is guaranteed to be aligned on the given bound. Such
//                      a pointer must be freed with naFreeAligned. Beware
//                      that bytesize here can NOT be negative!
// naMallocPageAligned  Same thing but the bound is the memory-page boundary.
//                      Does not require NARuntimeSystem!
// naFreeAligned        Deallocates any aligned pointer previously allocated
//                      with naMallocAligned or naMallocPageAligned. Note: This
//                      differs with known implementations on unix-like systems.
//                      But unfortunately, not all systems follow the norm and
//                      so a custom solution had to be made. Just make sure
//                      You always free aligned what you alloced aligned. See
//                      implementation for more details.
//
// naNew                This is a macro replicating something like the new
//                      operator known from C++. But it is dependent on a
//                      custom runtime system described further below.
// naDelete             Deletes a pointer created with naNew by properly
//                      calling the correct desctructor.

NA_IAPI void* naMalloc(           NAInt bytesize);
#define       naAlloc(type)      (type*)naMalloc(sizeof(type))
NA_IAPI void  naFree(             void* ptr);

NA_IAPI void* naMallocAligned(    NAUInt bytesize, NAUInt align);
NA_IAPI void* naMallocPageAligned(NAUInt bytesize);
NA_IAPI void  naFreeAligned(      void* ptr);

// If you experience an error with naNew: Have you marked your type with
// NA_RUNTIME_TYPE? See below.
#define       naNew(type)        (type*)naNewStruct(&na_ ## type ## _typeinfo)
NA_API  void  naDelete(           void* pointer);

// Authors note:
// Having only a handful allocation function helps detecting basic memory
// errors. Note however that there does not exist any exception handling
// in NALib, meaning an error might be detected though not resolved. And in
// favor of simplicity, NALib will not get exception handling soon.



// //////////////////////
// Runtime system
// //////////////////////

// In order to make naNew and NADelete work, you need to run the NALib runtime
// system. It is very raw at the moment but has a very low footprint. You can
// simply start the runtime at application start and stop it at the end.

NA_API void               naStartRuntime();
NA_API void               naStopRuntime();

NA_API NAUInt             naGetRuntimeMemoryPageSize();
NA_API NAUInt             naGetRuntimePoolSize();

// In order to work with specific types, each type trying to use the runtime
// system needs to register itself to the runtime system upon compile time.
// You can do so using the macro NA_RUNTIME_TYPE. Just write the typename
// (For example NAString) and the function to use for destructing the type.
//
// Note that the destructor must be declared and the type must not be opaque
// at the time of using NA_RUNTIME_TYPE!

#ifndef NDEBUG
#define NA_RUNTIME_TYPE(type, destructor)\
    static NATypeInfo na_ ## type ## _typeinfo =\
    {NA_NULL, sizeof(type), (NAMutator)destructor, #type}
#else
  #define NA_RUNTIME_TYPE(type, destructor)\
    static NATypeInfo na_ ## type ## _typeinfo =\
    {NA_NULL, sizeof(type), (NAMutator)destructor}
#endif

// Every type using the runtime system will get a global typeinfo variable
// which has the following type. The full type definition is in the file
// "NAMemoryII.h"
typedef struct NATypeInfo NATypeInfo;





// ///////////////////////////////
// Memory cleanup
// ////////////////////////


// The comments denote what kind of cleanup-function will be called when you
// use one of the macros.
typedef enum{
  NA_MEMORY_CLEANUP_NONE            = 0x00,  // 0b000 // 
  NA_MEMORY_CLEANUP_FREE            = 0x01,  // 0b001 // free
  NA_MEMORY_CLEANUP_FREE_ALIGNED    = 0x02,  // 0b010 // _aligned_free or free
  NA_MEMORY_CLEANUP_NA_FREE         = 0x03,  // 0b011 // naFree
  NA_MEMORY_CLEANUP_NA_FREE_ALIGNED = 0x04,  // 0b100 // naFreeAligned
#ifdef __cplusplus 
  NA_MEMORY_CLEANUP_DELETE          = 0x05,  // 0b101 // delete
  NA_MEMORY_CLEANUP_DELETE_BRACK    = 0x06,  // 0b110 // delete []
#endif
  NA_MEMORY_CLEANUP_NA_DELETE       = 0x07,  // 0b111 // naDelete
} NAMemoryCleanup;


// Deallocates the given data pointer with the given cleanup method.
NA_IAPI void naCleanupMemory(void* data, NAMemoryCleanup cleanup);





// ////////////////////////
// NARefCount
// ////////////////////////

// An NARefCount stores a reference count. When an NARefCount first is created,
// that count is 1 and it is increased everytime, naRetainRefCount is called.
//
// The reference counter is decreased everytime naReleaseSmartPtr is called.
// When reaching 0, the data as well as the smart pointer struct itself is
// erased automatically from memory.
//
// Note that this struct serves as a raw implementation which is used in NALib
// at specific positions. If you are looking for a more comfortable way of
// using smart pointers, have a look at NAPointer below.
//
// Important: If you want to use NARefCount in your own struct, make sure it
// is the first field in your struct!

// The full type definition is in the file "NAMemoryII.h"
typedef struct NARefCount NARefCount;

// Initialize a smart pointer. structcleanup defines, how the smart pointer
// itself shall be cleaned up.
//
// The datacleanup defines an additional flag which you can store in the struct
// if you have a data pointer for which you need to know, how that data shall
// be cleaned up. You can access that value later in the destructor which will
// be called in naReleaseRefCount when the refcount reaches zero by calling
// naGetRefCountCleanupData.
//
// In most of the time, datacleanup can be set to NA_MEMORY_CLEANUP_NONE.
NA_IAPI NARefCount* naInitRefCount(     NARefCount* refcount,
                                    NAMemoryCleanup structcleanup,
                                    NAMemoryCleanup datacleanup);

// Retain and Release.
// You can send a destructor to Release which will be called when the reference
// count reaches 0.
//
// When refcount reaches zero, first, the destructor is called. It will either
// be called with the given pointer to data or with refcount if data is NA_NULL.
// Then the data is cleaned up if available and finally, the struct is cleaned
// up.
NA_IAPI NARefCount* naRetainRefCount( NARefCount* refcount);
NA_IAPI void        naReleaseRefCount(NARefCount* refcount,
                                            void* data,
                                        NAMutator destructor);

// When your destructor will be called during a call to naReleaseRefCount, you
// can use this function to query the datacleanup value you entered upon the
// call to naInitRefCount.
NA_IAPI NAMemoryCleanup naGetRefCountCleanupData(const NARefCount* refcount);





// ////////////////////////
// NAPtr
//
// Certain structs like NAList or NAArray distinguish between const and
// non-const (mutable) data. The NAPtr struct delivers this distinction by
// providing both an Accessor and/or a Mutator to a C-pointer.
//
// This struct also serves as the core of memory consistency checking when
// debugging. An NAPtr stores many additional information which are not
// accessible to the programmer but would not be available when just using
// plain C pointers. NALib tags NAPtr values with various flags like if the
// pointer denotes an array and whether that array is null-terminated or not.
// During runtime, NALib checks if all accesses are fine and no buffer
// overflows occur.
//
// For example: If the programmer uses a mutator on const data when debugging,
// an error will be emitted. You can of course ignore these errors and hope
// that the application does not crash, but you should really make sure no
// const data gets mutated. When you see such an error message, something is
// fishy. Go check it!
//
// So, during debugging, using NAPtr can greatly improve your code-safety.
// Note however, that all the features of NAPtr are only avaliable when
// debugging. When compiling with NDEBUG, all your code will still compile
// but NAPtr will optimize to a simple (mutable) C-Pointer. No additional
// information will be stored and no code is executed whatsoever.
//
// The problem is that C has no concept of how to specifically handle a
// pointer, and especially how to use accessors and mutators. Even
// though the main difference is just the type returned, in C you can not
// overload functions and hence you can not hide the distinction to the user.
//
// In NALib, the author decided to use a union type storing either a const or
// a non-const data pointer. This idea is not entirely type-safe but is much
// easier to write programs with, as the programmer has to differentiate
// between const and non-const only when necessary.
//
// Other implementations provide two versions for each type storing either
// const or non-const data. This ensures type-safety, but forces the
// programmer to make the distinction at declaration level (which can become
// very cumbersome) and maybe even force him to convert between the two
// variants which might be very costly and not beautiful at all.
//
// NAPtr is the base of many memory structs.
// NAMemoryBlock is dependent on NAPtr and declare it as the first
// entry in their struct declaration. Therefore, it is safe to access these
// memory structs typecasted as an NAPtr.
//
// NALib keeps track of how pointers are allocated when NDEBUG is undefined.
// This affects all of the memory structures in this file. When NDEBUG is
// defined, this information is not available anymore. Except for the NAPointer
// struct which needs to store the information to deallocate the stored pointer
// correctly once the reference count reaches zero.


// The full type definition is in the file "NAMemoryII.h"
typedef struct NAPtr NAPtr;

// Creates a NULL pointer
// The cleanuphint for this function is NA_MEMORY_CLEANUP_NONE.
NA_IAPI NAPtr naMakeNullPtr();

// Makes an NAPtr with a newly allocated memory block of the given bytesize.
// The bytesize parameter can be negative. See naMalloc function for more
// information Note that you maybe will not call this function directly but
// instead will use naMakeLValueWithTypesize or naMakeMemoryBlockWithBytesize.
// The cleanuphint for this function is NA_MEMORY_CLEANUP_NA_FREE.
NA_IAPI NAPtr naMakePtrWithBytesize(NAInt bytesize);

// Fills the given NAPtr struct with either a const or a non-const pointer.
//
// The bytesizehint is a hint when debugging. It denotes how many bytes are
// visible to the programmer in the given data. In the same way, the
// zerofillhint are the number of zero bytes available but invisible to the
// programmer. Both parameters must be positive. The zerofillhint parameter
// only is valid when bytesizehint is not zero.
//
// The mutable variant can take ownership of the given data based on the given
// cleanuphint and expects a call to the corresponding cleanup function (see
// below). The cleanuphint for the const variant is NA_MEMORY_CLEANUP_NONE.
NA_IAPI NAPtr naMakePtrWithDataConst(       const void* data);
NA_IAPI NAPtr naMakePtrWithDataMutable(           void* data,
                                        NAMemoryCleanup cleanuphint);

// Assumes srcptr to be an array of bytes and creates an NAPtr referencing an
// extraction thereof. DOES NOT COPY!
// The byteoffset and bytesize must be given in bytes and are always positive!
// The bytesizehint is only a hint which helps detecting errors during
// debugging. When NDEBUG is defined, this hint is optimized out.
// The cleanuphint for this function is NA_MEMORY_CLEANUP_NONE.
NA_IAPI NAPtr naMakePtrWithExtraction(     const NAPtr* srcptr,
                                                 NAUInt byteoffset,
                                                 NAUInt bytesizehint);
    
// Note that the creation functions of NAPtr are naMakeXXX functions which
// makes it easy to implement. But the remaining functions require to provide
// a pointer.

// Cleanup functions
// Frees the memory stored in ptr. You should always choose the appropriate
// function depending on what kind of pointer the NAPtr stores. See definition
// of NAMemoryCleanup to know which one.
NA_IAPI void naClearPtr(NAPtr* ptr);
NA_IAPI void naFreePtr(NAPtr* ptr);
NA_IAPI void naFreeAlignedPtr(NAPtr* ptr);
NA_IAPI void naNaFreePtr(NAPtr* ptr);
NA_IAPI void naNaFreeAlignedPtr(NAPtr* ptr);
#ifdef __cplusplus
NA_IAPI void naDeletePtr(NAPtr* ptr);
NA_IAPI void naDeleteBrackPtr(NAPtr* ptr);
#endif
NA_IAPI void naNaDeletePtr(NAPtr* ptr);

// The following functions return either a const or a mutable pointer.
//
// When NDEBUG is NOT defined, NALib will check if a const value is accessed
// as a mutator and will emit a warning if so. If the content of the pointer
// is a NULL pointer, NO warning will be emitted.
//
// When NDEBUG IS defined, the const and mutable functions behave equally and
// no test will be performed whatsoever.
NA_IAPI const void* naGetPtrConst   (const NAPtr* ptr);
NA_IAPI       void* naGetPtrMutable (      NAPtr* ptr);

// Returns NA_TRUE, if the pointer stores const data. This function only is
// useful when debugging. When NDEBUG is defined, this function always returns
// NA_FALSE.
NA_IAPI NABool naIsPtrConst(const NAPtr* ptr);






// ////////////////////////
// NASmartPtr
// ////////////////////////

// A smart pointer stores an arbitrary mutable pointer with a reference count.
// When the smart pointer first is created, that count is 1 and it is increased
// everytime, naRetainSmartPtr is called.
//
// The reference counter is decreased everytime naReleaseSmartPtr is called.
// When reaching 0, the data as well as the smart pointer struct itself is
// erased automatically from memory.
//
// Note that this struct serves as a raw implementation which is used in NALib
// at specific positions. If you are looking for a more comfortable way of
// using smart pointers, have a look at NAPointer below.

// Starting with NALib version 14, the actual data is stored as a pointer.
// This means that it must be allocated elsewhere. But this means, any pointer
// can become a reference counted pointer.
//
// To distinguish, how the pointer shall be handeled upon deletion, a new
// enumeration NAMemoryCleanup had been introduced. See above.


// The full type definition is in the file "NAMemoryII.h"
typedef struct NASmartPtr NASmartPtr;

// Initialize a smart pointer. Define how the smart pointer itself shall be
// cleaned up, define the data it shall store and define, how that data shall
// be cleaned up.
NA_IAPI NASmartPtr* naInitSmartPtrConst(    NASmartPtr* sptr,
                                        NAMemoryCleanup smartptrcleanup,
                                            const void* data);
NA_IAPI NASmartPtr* naInitSmartPtrMutable(  NASmartPtr* sptr,
                                        NAMemoryCleanup smartptrcleanup,
                                                  void* data,
                                        NAMemoryCleanup datacleanup);

// Retain and Release.
// You can send a destructor to Release which will be called with the data
// pointer if the reference count reaches 0. Further more, the release function
// will return NA_TRUE when the reference count reached zero.
//
// When refcount reaches zero, first, the destructor is called with a pointer
// to data, then the data is cleaned up and finally, the struct is cleaned up.
NA_IAPI NASmartPtr* naRetainSmartPtr( NASmartPtr* sptr);
NA_IAPI void        naReleaseSmartPtr(NASmartPtr* sptr,
                                        NAMutator desctructor);

// Returns either a const or a mutable pointer to the data stored.
NA_IAPI const void* naGetSmartPtrConst  (const  NASmartPtr* sptr);
NA_IAPI void*       naGetSmartPtrMutable(       NASmartPtr* sptr);









// ////////////////////////
// NAPointer
// ////////////////////////

// NALib usually uses the native pointers of C and C++ but for some types,
// special kinds of pointers are more appropriate. The following is the
// implementation of a pointer having a reference count, as used in many
// modern languages. In C and C++, this must be implemented manually.
//
// NAPointer stores a pointer which can be accessed by multiple codeblocks,
// each "retaining" the NAPointer at start and "releasing" him at the end. When
// finally, no codeblock uses an NAPointer anymore, the pointer deallocates
// itself automatically.
//
// Having a reference count is not always useful in C and C++. Only a few
// structs of NALib like NAList actually use the NAPointer structure.
//
// Note that starting with NALib version 10, NAPointers require the NARuntime
// system. Starting with verision 18, there also exists a more fundamental
// struct called NASmartPtr which does not necessarily use the runtime system
// but is a little more complicated to use.



// The full type definition is in the file "NAMemoryII.h"
typedef struct NAPointer NAPointer;



// Creates an NAPointer struct around the given data pointer.
//
// The NAPointer allows to use reference counting on any pointer and
// will automatically erase both the data using the given destructor
// as well as the provided data pointer itself according to the cleanup
// argument.
//
// If no destructor is needed, you can send NA_NULL.
//
// Notice the distinction: The destructor will be called with the data pointer
// such that any struct which is behind that C-pointer can be properly erased.
// The cleanup enum on the other hand defines, what will happen with the
// data pointer itself AFTER the destructor had been called. Depending on how
// that pointer had been created in the first place, it must be cleaned up with
// the appropriate deallocation function.
NA_IAPI NAPointer* naNewPointerConst(   const void* data);
NA_IAPI NAPointer* naNewPointerMutable(       void* data,
                                    NAMemoryCleanup datacleanup,
                                          NAMutator destructor);

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
// is no longer needed. The data will be freed automatically according to the
// destructor and cleanup enumeration given upon creation. The NAPointer struct
// itself will be deleted by the runtime system.
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
//
// The distinction between const and mutable is done to ensure no const pointer
// gets mutated. But this is only checked during debugging. These two functions
// behave equivalent when NDEBUG is defined.
NA_IAPI const void* naGetPointerConst  (const NAPointer* pointer);
NA_IAPI       void* naGetPointerMutable(      NAPointer* pointer);




// Inline implementations are in a separate file:
#include "NACore/NAMemoryII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_MEMORY_INCLUDED


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
