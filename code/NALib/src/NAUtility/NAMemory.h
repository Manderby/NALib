
#ifndef NA_MEMORY_INCLUDED
#define NA_MEMORY_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// ////////////////////////////////////////////////////////
//
// In this file, basic functionality of memory handling is implemented. There
// are multiple levels on which you can operate.
// The following things can be found here:
//
// - Accessing fundamental informations about the memory system
// - Basic Memory allocation and freeing.
// - NARefCount, NAPtr, NASmartPtr, NAPointer: Handling of pointers with or
//   without reference counting with const and mutable distinction and
//   automatic deletion.
// - Handling of inifinte pools for naNew and naDelete functions.
//
// ////////////////////////////////////////////////////////




#include "../NABase/NABase.h"


// //////////////////////////////////////
// Accessing fundamental information about the memory system
// //////////////////////////////////////

// Returns the number of bytes used per memory page as well as a mask which
// you can combine with & (AND) with a pointer to get the page-aligned address
// of that pointer.
NA_IAPI size_t naGetSystemMemoryPagesize(void);
NA_IAPI size_t naGetSystemMemoryPagesizeMask(void);



// NALib defines the following index and size to be the predefined "invalids".
// Often times in NALib, indexes and sizes can be negative (see the API for
// more details). As in todays computers the twos complement is usually used
// to store negative integers, the minimal integer number is a good choice for
// an invalid index as it can not be transformed into a positive value. But
// also in other encodings, the most negative integer is probably never used
// by any programmer. Any other number than this invalid number might have a
// valid meaning in NALib.
#define NA_INVALID_MEMORY_INDEX     NA_MIN_i64



// //////////////////////////////////////
// Basic Memory allocation and freeing
// //////////////////////////////////////



// The following functions are the allocation functions of NALib. All functions
// in NALib use these functions when allocating memory on the heap.
//
// naMalloc             Allocates the given size of bytes on the heap using
//                      malloc. If byteSize is negative, zero-filled bytes are
//                      appended at the end. See explanation below.
// naAlloc              This is a macro which expands to naMalloc but can be
//                      used to allocate enough space for a struct type. For
//                      exampe, you can simply write array = naAlloc(NAArray);
// naFree               Deallocates a pointer previously allocated with one of
//                      the above functions or the default C memory allocation
//                      functions like malloc.
//
// naMallocAligned      The given byteSize is allocated and the returned pointer
//                      is guaranteed to be aligned on the given bound. Such
//                      a pointer must be freed with naFreeAligned. Beware
//                      that byteSize here can NOT be negative!
// naMallocPageAligned  Same thing but the bound is the memory-page boundary.
//                      Does NOT require NARuntimeSystem!
// naFreeAligned        Deallocates any aligned pointer previously allocated
//                      with naMallocAligned or naMallocPageAligned. Note: This
//                      differs with known implementations on unix-like systems.
//                      But unfortunately, not all systems follow the norm and
//                      so a custom solution had to be made. Just make sure
//                      You always free aligned what you alloced aligned. See
//                      implementation for more details.
//
// naMallocTmp          Allocates the given number of bytes with malloc and
//                      returns it as a mutable void pointer but is owned by
//                      the runtime system and will be freed automatically
//                      during a call to naCollectGarbage. Do not expect the
//                      content of such pointers to be valid for a long time
//                      and never call naFree or free on them manually!
//                      Beware: Only positive byte counts are allowed.
// naNew                This is a macro replicating something like the new
//                      operator known from C++. But it is dependent on a
//                      custom runtime system described further below.
// naDelete             Deletes a pointer created with naNew by properly
//                      calling the correct destructor.
//
// Authors note:
// Having only a handful allocation function helps detecting basic memory
// errors. Note however that there does not exist any exception handling
// in NALib, meaning an error might be detected though not resolved. And in
// favor of simplicity, NALib will not get exception handling soon.
//
// Note that the actual definitions of the macros are in NAMemoryII.h

NA_IAPI void*       naMalloc           (size_t byteSize);
#define             naAlloc            (type)
NA_IAPI void        naFree             (void* ptr);
      
NA_IAPI void*       naMallocAligned    (size_t byteSize, size_t align);
NA_IAPI void*       naMallocPageAligned(size_t byteSize);
NA_IAPI void        naFreeAligned      (void* ptr);
      
NA_API  void*       naMallocTmp        (size_t byteSize);
#define             naNew              (type)
NA_API  void        naDelete           (void* pointer);
      
NA_API  void*       naRetain           (void* pointer);
NA_API  void        naRelease          (void* pointer);
NA_API  const void* naRetainConst      (const void* pointer);
NA_API  void        naReleaseConst     (const void* pointer);

NA_API size_t naGetRuntimeTypeRefCount(const void* pointer);


// ////////////////////////
// NARefCount
// ////////////////////////

// An NARefCount stores a reference count. When an NARefCount first is created,
// that count is 1 and it is increased everytime naRetainRefCount is called.
//
// The reference counter is decreased everytime naReleaseRefCount is called.
// When reaching 0, the data as well as the smart pointer struct itself is
// erased automatically from memory.
//
// Note that this struct serves as a raw implementation which is used in NALib
// as a base of specific structs which implement reference counting manually.
// If you are looking for a more comfortable way of using smart pointers, have
// a look at NASmartPtr and especially NAPointer below.
//
// Important: If you want to use NARefCount in your own struct, make sure it
// is the first field in your struct!

// The full type definition is in the file "NAMemoryII.h"
NA_PROTOTYPE(NARefCount);

// Initializes an NARefCount.
NA_IAPI NARefCount* naInitRefCount(     NARefCount* refCount);

// Returns true if the NATRefCount has reached zero.
NA_IAPI NABool naIsRefCountZero(        NARefCount* refCount);

// Retain and Release.
// You can send a destructor to Release which will be called when the reference
// count reaches 0.
//
// The destructor will be called with the given data pointer. You always must
// explicitely state that data pointer. If that data pointer is NA_NULL, the
// destructor will get NA_NULL.
//
// When destructor is NA_NULL itself, nothing will be called.
NA_IAPI NARefCount* naRetainRefCount( NARefCount* refCount);
NA_IAPI void        naReleaseRefCount(NARefCount* refCount,
                                            void* data,
                                        NAMutator destructor);

// ////////////////////////
// NAPtr
//
// Certain structs like NAList or NAArray distinguish between const and
// non-const (mutable) data. The NAPtr struct delivers this distinction by
// providing both an Accessor and/or a Mutator to a C-pointer.
//
// This struct also serves as the core of memory consistency checking when
// debugging. An NAPtr stores additional information which are not accessible
// to the programmer but would not be available when just using ordinary plain
// C pointers. During runtime, NALib checks if all accesses are fine.
//
// For example: If the programmer uses a mutator on const data when debugging,
// an error will be emitted. You can of course ignore these errors and hope
// that the application does not crash, but you should really make sure no
// const data gets mutated. When you see such an error message, something is
// fishy. Go check it!
//
// So, during debugging, using NAPtr can greatly improve your code-safety.
// Note however, that all the features of NAPtr are only avaliable when
// debugging. When compiling with NA_DEBUG = 1, all your code will still
// compile but NAPtr will optimize to a simple (mutable) C-Pointer. No
// additional information will be stored and no code is executed whatsoever.
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


// The full type definition is in the file "NAMemoryII.h"
NA_PROTOTYPE(NAPtr);

// Creates a NULL pointer
NA_IAPI NAPtr naMakePtrNull(void);

// Fills the given NAPtr struct with either a const or a non-const pointer
// without copying any bytes.
NA_IAPI NAPtr naMakePtrWithDataConst(       const void* data);
NA_IAPI NAPtr naMakePtrWithDataMutable(           void* data);
// Note that the creation functions of NAPtr are naMakeXXX functions which
// makes it easy to implement. But the remaining functions require to provide
// a pointer.

// Calls the given destructor with the data pointer stored in ptr.
NA_IAPI void naCleanupPtr(NAPtr* ptr, NAMutator destructor);

// The following functions return either a const or a mutable pointer.
//
// When NA_DEBUG is 1, NALib will check if a const value is accessed as a
// mutator and will emit a warning if so. If the content of the pointer is
// a NULL pointer, NO warning will be emitted.
//
// When NA_DEBUG is 0, the const and mutable functions behave equally and
// no test will be performed whatsoever.
NA_IAPI const void* naGetPtrConst   (NAPtr ptr);
NA_IAPI       void* naGetPtrMutable (NAPtr ptr);

// Returns true if there is a pointer stored, false if it stores NA_NULL
NA_IAPI NABool naIsPtrValid(NAPtr ptr);

// Returns NA_TRUE, if the pointer stores const data. This function only is
// useful when debugging. When NA_DEBUG is 0, this function always returns
// NA_FALSE.
NA_IAPI NABool naIsPtrConst(NAPtr ptr);




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
// Note that this struct is a very raw implementation which currently is only
// used as a base for the more comfortable NAPointer struct. You are free to
// use the implementation for your own evil scheme though. If you are lazy and
// want to use NAPointer, have a look below.
//
// To distinguish, how the pointer shall be handeled upon deletion, a
// NAMemoryCleanup can be used. See above.


// The full type definition is in the file "NAMemoryII.h"
NA_PROTOTYPE(NASmartPtr);

// Initialize a smart pointer. Define how the smart pointer itself shall be
// cleaned up, define the data it shall store and define, how that data shall
// be cleaned up when the Ptr is mutable.
//
// Note: Even if it is possible to send a NONE cleanup, it is not a good idea
// to use a pointer with a stack variable for sptr. Code sanity checks might
// mark this as a possible error.
NA_IAPI NASmartPtr* naInitSmartPtrConst(    NASmartPtr* sptr,
                                            const void* data);
NA_IAPI NASmartPtr* naInitSmartPtrMutable(  NASmartPtr* sptr,
                                                  void* data);

// Retaining and releasing an NASmartPtr.
// You can send a destructor to Release which will be called if the reference
// count reaches 0. Either the destructor will be called with the struct itself
// or only with the data pointer if onlyData is set to NA_TRUE.
//
// When refCount reaches zero, first, the destructor is called with a pointer
// to data, then the data is cleaned up and finally, the struct is cleaned up.
NA_IAPI NASmartPtr* naRetainSmartPtr (NASmartPtr* sptr);
NA_IAPI void        naReleaseSmartPtr(NASmartPtr* sptr,
                                        NAMutator destructor,
                                           NABool onlyData);

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
NA_PROTOTYPE(NAPointer);

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
NA_IAPI NAPointer* naInitPointerConst(    NAPointer* pointer,
                                         const void* data);
NA_IAPI NAPointer* naInitPointerMutable(  NAPointer* pointer,
                                               void* data,
                                           NAMutator destructor);

// Retains and releases the given NAPointer. If the refCount reaches 0 when
// releasing, this NAPointer is no longer needed. The data will be freed
// automatically according to the destructor and cleanup enumeration given
// upon creation. The NAPointer struct itself will be deleted by the runtime
// system.
NA_IAPI NAPointer*  naRetainPointer (NAPointer* pointer);
NA_IAPI void        naReleasePointer(NAPointer* pointer);

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
// behave equivalently when NA_DEBUG is 0.
NA_IAPI const void* naGetPointerConst  (const NAPointer* pointer);
NA_IAPI       void* naGetPointerMutable(      NAPointer* pointer);




// //////////////////////
// Runtime system
// //////////////////////

// In order to make naNew and NADelete work, you need to run the NALib runtime
// system. You can simply start the runtime at application start and stop it at
// the end.
//
// Additionally, you can call naCollectGarbage to collect all temporary memory
// which had been allocated with naMallocTmp.

NA_API  void   naStartRuntime(void);
NA_API  void   naStopRuntime(void);
NA_IAPI NABool naIsRuntimeRunning(void);

NA_API  void   naCollectGarbage(void);
NA_IAPI size_t naGetRuntimeGarbageByteSize(void);

NA_IAPI size_t naGetRuntimeMemoryPageSize(void);
NA_IAPI size_t naGetRuntimePoolPartSize(void);

// In order to work with specific types, each type trying to use the runtime
// system needs to register itself to the runtime system upon compile time.
// This is achieved by defining a very specific variable of type NATypeInfo.
// You can do so using the macro NA_RUNTIME_TYPE. Just write the typeName
// and the function to use for destructing the type.

#define NA_RUNTIME_TYPE(typeName, destructor, refCounting)

// But note that this macro results in a variable definition and hence must be
// written in an implementation file (.c). Also, the type must not be opaque
// and the destructor must be declared before this macro.
//
// As though you may want to write inlineable code in your header files which
// use naNew, you can use the NA_EXTERN_RUNTIME_TYPE macro to declare the
// variable beforehand:

#define NA_EXTERN_RUNTIME_TYPE(typeName)

// For a deeper understanding on how that macro does what it does, please refer
// to the definition of these macros in NAMemoryII.h and the implementation of
// the runtime system in NAMemory.c

// Every type using the runtime system will get a global typeinfo variable
// which has the following type. The full type definition is in the file
// "NAMemoryII.h"
NA_PROTOTYPE(NATypeInfo);





// Inline implementations are in separate files:
#include "NAMemory/NAMemoryII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_MEMORY_INCLUDED



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
