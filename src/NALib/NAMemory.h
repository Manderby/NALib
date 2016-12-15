
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
// - Base memory structs NAPtr, NALValue, NAMemoryBlock, NACArray, NABuf
// - NAPointer: Handling of reference counted values with automatic deletion.
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

// In order to work with specific types, each type trying to use the runtime
// system needs to register itself to the runtime system upon compile time.
// You can do so using the macro NA_RUNTIME_TYPE. Just write the typename
// (For example NAString) and the function to use for destructing the type.
//
// Note that the destructor must be declared and the type must not be opaque
// at the time of using NA_RUNTIME_TYPE!


#define NA_RUNTIME_TYPE(type, destructor)\
  static NA_LINKER_NO_EXPORT NATypeInfo na_ ## type ## _typeinfo =\
  {NA_NULL, sizeof(type), (NAFunc)destructor};

// Every type using the runtime system will get a global typeinfo variable
// which has the following type. The full type definition is in the file
// "NAMemoryII.h"
typedef struct NATypeInfo NATypeInfo;

// The following three functions are for debugging. You can start counting
// the number of bytes allocated using the start function and stop counting
// using the stop function. The start function will reset the byte counter
// to zero. Using the continue function, the counter will not be reset.
//
// These functions will all be NOP (no operation) when NDEBUG is defined.
NA_IAPI void naStartMemoryObservation();
NA_IAPI void naStopMemoryObservation();
NA_IAPI void naContinueMemoryObservation();
// Use the following functions to retrieve the number of bytes allocated. You
// can distinguish between aligned and unaligned allocations or just use the
// total sum. The aligned bytecount counts the bytes not visible to the
// programmer.
NA_IAPI NAInt naGetMemoryObservation();
NA_IAPI NAInt naGetMemoryObservationInvisible();
NA_IAPI NAInt naGetMemoryObservationVisible();




// ///////////////////////////////
// Core memory structs
//
// Following are the declarations and definitions of the very fundamental
// base memory structs used in NALib. They seem confusing at first but are
// very simple to understand.
//
// Basically, they all store a C-Pointer. But a C-Pointer can denote anything
// from a single byte up to an array of large structs, they can be const or
// mutable, they can be allocated with malloc, new or auto. The base structs
// are here to distinguish between them. Basically, they store the following
// information:
//
// NAPtr          A plain pointer. With lots of info when NDEBUG is undefined.
// NALValue       A pointer with a typesize, denoting a localizable value.
// NAMemoryBlock  A pointer to a bunch of bytes
// NACArray       A pointer to a bunch of values
// NABuf          A pointer to a bunch of bytes, only partially being used.
// NAPointer      A reference counted pointer which deallocates automatically
//                when the reference count reaches zero.
//
// At the core, an NAPtr stores a C-Pointer and the information whether it
// is const or mutable. It also stores information about the bytesize of the memory
// being pointed at and how it is null terminated. Even more, it stores, how
// the pointer had originally been allocated. All this information is just for
// debugging and can be omitted if necessary. When compiling with NDEBUG, no
// information is stored at all.
//
// An NALValue stores a pointer which denotes a variable placed in memory.
// It has a typesize.
//
// An NAMemoryBlock stores a bunch of bytes. No typesize but a total bytesize
// of bytes is available.
//
// An NACArray is the combination of the last two structs. It stores a pointer,
// a total number of bytes available and the typesize of the units stored
// in that array. The number of elements in the array can be computed by
// dividing the total bytesize by the typesize. See API for the corresponding
// function call.
//
// An NABuf is a buffer with a fixed maximal bytesize and a counter how many
// bytes actually are in use.
//
// An NAPointer stores a pointer with a reference count. You can increase and
// decrease that reference count and the pointer will automatically be erased
// when the reference count reaches zero. How it will be erased can be defined
// upon creation of the NAPointer.
//
// ///////////////////////////////



// NALib keeps track of how pointers are allocated when NDEBUG is undefined.
// This affects all of the memory structures in this file. When NDEBUG is
// defined, this information is not available anymore. Except for the NAPointer
// struct which needs to store the information to deallocate the stored pointer
// correctly once the reference count reaches zero.
//
// The comments denote what kind of cleanup-function of NAPtr you should call
// when you use one of the macros.
typedef enum{
  NA_MEMORY_CLEANUP_UNDEFINED       = 0x00,  // 0b000
  NA_MEMORY_CLEANUP_NONE            = 0x01,  // 0b001 // naClearPtr
  NA_MEMORY_CLEANUP_FREE            = 0x02,  // 0b010 // naFreePtr
  NA_MEMORY_CLEANUP_FREE_ALIGNED    = 0x03,  // 0b011 // naFreeAlignedPtr
  NA_MEMORY_CLEANUP_DELETE          = 0x04   // 0b100 // naDeletePtr
} NAMemoryCleanup;



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
// NAPtr is the base of all memory structs.
// NALValue, NAMemoryBlock, NACArray and NABuf are all dependent on NAPtr and
// declare it as the first entry in their struct declaration. Therefore, it is
// safe to access any other memory struct typecastet as an NAPtr. See NAPointer
// for an example.


// The full type definition is in the file "NAMemoryII.h"
typedef struct NAPtr NAPtr;

// Creates a NULL pointer
NA_IAPI NAPtr naMakeNullPtr();

// Makes an NAPtr with a newly allocated memory block of the given bytesize. The
// bytesize parameter can be negative. See naMalloc function for more information.
// Note that you maybe will not call this function directly but instead will
// use naMakeLValueWithTypesize or naMakeMemoryBlockWithBytesize.
// The cleanuphint for this function is NA_MEMORY_CLEANUP_FREE.
NA_IAPI NAPtr naMakePtrWithBytesize(NAInt bytesize);

// Fills the given NAPtr struct with either a const or a non-const pointer.
// The bytesizehint is a hint when debugging. For an explanation of that
// parameter, see naMakeMemoryBlockWithConstBuffer. In the same way, the
// zerofillhint are the number of zero bytes available but invisible to the
// programmer.
//
// The mutable variant takes ownership of the given data and expects a call
// to a cleanup function (see below) based on the cleanuphint. The cleanuphint
// for the const variant is NA_MEMORY_CLEANUP_NONE.
NA_IAPI NAPtr naMakePtrWithConstBuffer(  const void* data,
                                               NAInt bytesizehint,
                                               NAInt zerofillhint);
NA_IAPI NAPtr naMakePtrWithMutableBuffer(      void* data,
                                               NAInt bytesizehint,
                                               NAInt zerofillhint,
                                    NAMemoryCleanup cleanuphint);

// Assumes srcptr to be an array of bytes and creates an NAPtr referencing an
// extraction thereof. DOES NOT COPY!
// The byteoffset and bytesize must be given in bytes and are always positive! The bytesize
// is only a hint which helps detecting errors during debugging. When NDEBUG is
// defined, this hint is optimized out. The cleanuphint for this function is
// NA_MEMORY_CLEANUP_NONE.
NA_IAPI NAPtr naMakePtrWithExtraction(  const NAPtr* srcptr,
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
NA_IAPI void naDeletePtr(NAPtr* ptr);

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
// NALValue
// ////////////////////////

// The full type definition is in the file "NAMemoryII.h"
typedef struct NALValue NALValue;

// Returns an lvalue which is empty.
NA_IAPI NALValue naMakeLValue();

// Create a new lvalue of the given size in bytes. The typesize parameter MUST be positive.
NA_IAPI NALValue naMakeLValueWithTypesize(NAUInt typesize);

// Uses the given bufptr WITHOUT copying as an lvalue with the given typesize.
NA_IAPI NALValue naMakeLValueWithConstBuffer(   const void* bufptr,
                                                      NAInt typesize);
NA_IAPI NALValue naMakeLValueWithMutableBuffer(       void* bufptr,
                                                      NAInt typesize,
                                           NAMemoryCleanup cleanuphint);

// Frees the memory for the value buffer.
NA_IAPI void naFreeLValue(NALValue* lvalue);

// Returns informations about the typesize of the given lvalue.
NA_IAPI NAUInt naGetLValueTypesize(const NALValue* lvalue);
NA_IAPI NABool naIsLValueEmpty(const NALValue* lvalue);

// Returns either a const or a mutable pointer to the lvalue.
NA_IAPI const void* naGetLValueConstPointer(const NALValue* lvalue);
NA_IAPI void* naGetLValueMutablePointer(NALValue* lvalue);






// ////////////////////////
// NAMemoryBlock
// ////////////////////////

// The full type definition is in the file "NAMemoryII.h"
typedef struct NAMemoryBlock NAMemoryBlock;

// Returns a memory block which is empty.
NA_IAPI NAMemoryBlock naMakeMemoryBlock();

// Creates a new memory block of the given bytesize. The bytesize
// parameter can be negative. See naMalloc function for more information.
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithBytesize(NAInt bytesize);

// Uses the given bufptr WITHOUT copying as a memory block with the given bytesize.
// The programmer is responsible that the given bytesize is not overflowing the
// buffer.
// The bytesize can be negative. If so, the absolute value of bytesize is
// used but the given bufptr is expected to to have one or more bytes appended
// which are filled with binary zero. Hence, the array can safely be assumed to
// be null-terminated. This also serves as a hint for functions like
// naMakePtrWithConstBuffer
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithConstBuffer( const void* bufptr,
                                                              NAInt bytesize);
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithMutableBuffer(     void* bufptr,
                                                              NAInt bytesize,
                                                   NAMemoryCleanup cleanuphint);

// Makes a new NAMemoryBlock struct containing a sub-part of the given source
// memory block. Does NOT copy! The byteoffset and bytesize are given in bytes and must
// be positive.
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithExtraction(
                                            const NAMemoryBlock* srcmemblock,
                                                          NAUInt byteoffset,
                                                          NAUInt bytesize);

// Frees the memory for the memory block.
NA_IAPI void naFreeMemoryBlock(NAMemoryBlock* memblock);

// Returns size information about the memory block. The maxindex returns
// bytesize-1 but will emit an error when NDEBUG is undefined and the bytesize is 0.
NA_IAPI NAUInt naGetMemoryBlockBytesize(const NAMemoryBlock* memblock);
NA_IAPI NAUInt naGetMemoryBlockMaxIndex(const NAMemoryBlock* memblock);
NA_IAPI NABool naIsMemoryBlockEmpty(const NAMemoryBlock* memblock);

// Invalidates the memory block by setting its bytesize to 0. No access can
// be performed anymore. But the bytesize can be retrieved.
NA_IAPI void naVoidMemoryBlock(NAMemoryBlock* memblock);

// Returns either a const or mutable pointer to the first byte of the given
// memory block
NA_IAPI const void* naGetMemoryBlockConstPointer(const NAMemoryBlock* memblock);
NA_IAPI void* naGetMemoryBlockMutablePointer(          NAMemoryBlock* memblock);

// Returns either a const or mutable pointer to the byte at the given index.
NA_IAPI const void* naGetMemoryBlockConstByte(   const NAMemoryBlock* memblock,
                                                               NAUInt indx);
NA_IAPI void* naGetMemoryBlockMutableByte(             NAMemoryBlock* memblock,
                                                               NAUInt indx);
// Returns true if this memory block stores const content. Only useful when
// debugging. When NDEBUG is defined, this function always returns NA_FALSE.
NA_IAPI NABool naIsMemoryBlockConst(NAMemoryBlock* memblock);

// A debugging function returning true, if the given memory block is declared
// to be null terminated. If NDEBUG is defined, this function is undefined
// and not available. The author does not want to propose any assumption.
// Therefore it is also marked as a helper function.
#ifndef NDEBUG
  NA_HIAPI NABool naIsMemoryBlockNullTerminated(const NAMemoryBlock* memblock);
#endif






// ////////////////////////
// NACArray
// ////////////////////////

// The full type definition is in the file "NAMemoryII.h"
typedef struct NACArray NACArray;

// Returns a C-array which is empty.
NA_IAPI NACArray naMakeCArray();

// Creates a new memory block with the given typesize and element count. The
// count parameter can be negative. See naMalloc function for more information.
NA_IAPI NACArray naMakeCArrayWithTypesizeAndCount(NAUInt typesize, NAInt count);

// Uses the given bufptr WITHOUT copying as a C array with the given typesize
// and element count. The programmer is responsible that the given sizes are
// not overflowing the buffer.
// The count can be negative. If so, the absolute value of count is used but
// the given bufptr is expected to to have at least typesize bytes appended
// which are filled with binary zero. Hence, the array can be assumed to
// be null-terminated. This also serves as a hint for functions like
// naMakePtrWithConstBuffer.
// Note that the additional typesize bytes is special. naMalloc only guarantees
// zero filling up to a certain number of bytes. You have to be careful if you
// want to use negative sizes with NACArray.
NA_IAPI NACArray naMakeCArrayWithConstBuffer(   const void* bufptr,
                                                      NAInt typesize,
                                                      NAInt count);
NA_IAPI NACArray naMakeCArrayWithMutableBuffer(       void* bufptr,
                                                     NAUInt typesize,
                                                      NAInt count,
                                           NAMemoryCleanup cleanuphint);

// Makes a new NACArray struct containing a sub-part of the given source
// C-array. Does NOT copy! The elemoffset and count are given in units with the
// typesize stored in srcarray and must be positive.
NA_IAPI NACArray naMakeCArrayWithExtraction(const NACArray* srccarray,
                                                     NAUInt elemoffset,
                                                     NAUInt elemcount);

// Frees the memory for the array.
NA_IAPI void naFreeCArray(NACArray* carray);

// Returns size information about the memory block.
NA_IAPI NAUInt naGetCArrayBytesize( const NACArray* carray);
NA_IAPI NAUInt naGetCArrayCount(    const NACArray* carray);
NA_IAPI NABool naIsCArrayEmpty(     const NACArray* carray);

// Returns either a const or mutable pointer to the first byte of the given
// C-array
NA_IAPI const void* naGetCArrayConstPointer(const NACArray* carray);
NA_IAPI void* naGetCArrayMutablePointer(          NACArray* carray);

// Returns either a const or mutable pointer to the element at the given index.
NA_IAPI const void* naGetCArrayConstByte(const NACArray* carray, NAUInt indx);
NA_IAPI void* naGetCArrayMutableByte(          NACArray* carray, NAUInt indx);

// Returns true if this C-array stores const content. Only useful when
// debugging. When NDEBUG is defined, this function always returns NA_FALSE.
NA_IAPI NABool naIsCArrayConst(NACArray* carray);

// A debugging function returning true, if the given memory block is declared
// to be null terminated. If NDEBUG is defined, this function is undefined
// and not available. The author does not want to propose any assumption.
// Therefore it is also marked as a helper function.
#ifndef NDEBUG
  NA_HIAPI NABool naIsCArrayNullTerminated(const NACArray* carray);
#endif





// ////////////////////////
// NABuf
// ////////////////////////

// The full type definition is in the file "NAMemoryII.h"
typedef struct NABuf NABuf;

// Returns a buf which is empty.
NA_IAPI NABuf naMakeBuf();

// Creates a new buf with the given sizes. In contrast to the other
// base memory structs, both bytesize parameter must be positive.
// usedsize can be 0 but maxsize must be greater than 0.
NA_IAPI NABuf naMakeBufWithBytesize(int64 maxbytesize, int64 usedbytesize);

// Frees the memory for the buffer.
NA_IAPI void naFreeBuf(NABuf* buf);

// Returns size information about the memory block.
NA_IAPI uint64 naGetBufMaxBytesize(const NABuf* buf);
NA_IAPI uint64 naGetBufUsedBytesize(const NABuf* buf);
NA_IAPI uint64 naGetBufRemainingBytesize(const NABuf* buf);
// Returns true if there is no used byte in the buf.
NA_IAPI NABool naIsBufEmpty(const NABuf* buf);

// Returns either a const or mutable pointer to the current byte of the given
// buf. The current byte is the byte yet not being used.
NA_IAPI const void* naGetBufConstUsedPointer(const NABuf* buf);
NA_IAPI void*       naGetBufMutableUsedPointer(    NABuf* buf);
// Increments the used bytesize by the given number of bytes. Will emit an error
// when the buffer overflows when NDEBUG is not defined.
NA_IAPI void        naAdvanceBuf(NABuf* buf, uint64 bytesize);

// Returns either a const or mutable pointer to the first byte of the full
// buf. Emits an error when the buf is empty when NDEBUG is undefined.
NA_IAPI const void* naGetBufConstFirstPointer(   const NABuf* buf);
NA_IAPI       void* naGetBufMutableFirstPointer(       NABuf* buf);

// Returns true if this buf stores const content. Only useful when debugging.
// When NDEBUG is defined, this function always returns NA_FALSE.
NA_IAPI NABool naIsBufConst(NABuf* buf);




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
// structs of NALib like the NAByteArray actually use the NAPointer structure.
//
// Note that starting with NALib version 10, NAPointers require the NARuntime
// system.



// The full type definition is in the file "NAMemoryII.h"
typedef struct NAPointer NAPointer;



// Creates an NAPointer referencing a NULL pointer. This is a pure convenience
// function if you do not want to care upon deletion, what exactly an NAPointer
// is storing.
NA_IAPI NAPointer* naNewNullPointer();


// Creates an NAPointer struct around the given data pointer.
//
// The NAPointer allows to use reference counting on any pointer and
// will automatically deallocate both the data using the given deallocator
// as well as the provided data pointer itself according to the cleanup
// argument.
//
// If no deallocator is needed, you can send NA_NULL.
//
// Notice the distinction: The deallocator will be called with the data pointer
// such that any struct which is behind that pointer can be properly cleaned
// up. The cleanup enum on the other hand defines, what will happen with the
// data pointer itself after the deallocator has been called. Depending on how
// that pointer had been created in the first place, it must be cleaned up with
// the appropriate free or delete function.
//
NA_IAPI NAPointer* naNewPointer(       void* data,
                            NAMemoryCleanup cleanup,
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

// Returns NA_TRUE if the pointer stored is const. Note that this function
// also works when NDEBUG is defined.
NA_IAPI NABool      naIsPointerConst   (const NAPointer* pointer);




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
