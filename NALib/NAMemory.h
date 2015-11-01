
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
// - NAPtr, NALValue, NAMemoryBlock, NACArray: Handling of C-pointers, being
//   mutable or const, denoting arrays or variables. With lots
//   of debugging helping goods.
//
// If you want reference counted values which additionally can be automatically
// created and deleted, you can include NARuntime.h instead of NAMemory.h. You
// then can additionally use the following things:
//
// - NAPointer: Handling of reference counted values with automatic deletion.
// - Handling of inifinte pools for new and delete functions.
//
// ////////////////////////////////////////////////////////


#ifndef NA_POINTER_INCLUDED
#define NA_POINTER_INCLUDED
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
NA_IAPI NAUInt naGetSystemMemoryPageSize();
NA_IAPI NAUInt naGetSystemMemoryPageSizeMask();



// NALib defines the following index and size to be the predefined "invalids".
// Often times in NALib, indexes and sizes can be negative (see the API for
// more details). As in todays computers the twos complement is usually used
// to store negative integers, the minimal integer number is a good choice for
// an invalid index as it can not be transformed into a positive value. But
// also in other encodings, the most negative integer is probably never used
// by any programmer. Any other number than this invalid number might have a
// valid meaning in NALib.
#define NA_INVALID_MEMORY_INDEX  NA_INT_MIN
#define NA_INVALID_MEMORY_SIZE   NA_INT_MIN



// //////////////////////////////////////
// Basic Memory allocation and freeing
// //////////////////////////////////////

// The following functions are the allocation functions of NALib. All functions
// in NALib use these functions when allocating memory on the heap.
//
// naMalloc             Allocates the given size of bytes on the heap using
//                      malloc. If size is negative, zero-filled bytes are
//                      appended at the end. See explanation below.
// naMallocIfNull       Same thing but only if the given ptr is NULL. If ptr is
//                      NA_NULL, memory is allocated and the pointer to that
//                      memory is returned. If the pointer is not NA_NULL,
//                      nothing is allocated and the same pointer is returned.
// naAlloc              This is a macro which expands to naMalloc but can be
//                      used to allocate enough space for a struct type. For
//                      exampe, you can simply write array = naAlloc(NAArray);
// naFree               Deallocates a pointer previously allocated with one of
//                      the above functions or the default C memory allocation
//                      functions like malloc.
//
// naMallocAligned      The given size is allocated and the returned pointer
//                      is guaranteed to be aligned on the given bound. Such
//                      a pointer must be freed with naFreePageAligned. Beware
//                      that size here can not be negative!
// naMallocPageAligned  Same thing but the bound is the memory-page boundary.
// naFreePageAligned    Deallocates a page-aligned pointer previously allocated
//                      with naMallocAligned or naMallocPageAligned.
//
NA_IAPI void* naMalloc(             NAInt size);
NA_IAPI void* naMallocIfNull(       void* ptr, NAInt size);
#define       naAlloc(structname)   (structname*)naMalloc(sizeof(structname))
NA_IAPI void  naFree(               void* ptr);

NA_IAPI void* naMallocAligned(      NAUInt size, NAUInt align);
NA_IAPI void* naMallocPageAligned(  NAUInt size);
NA_IAPI void  naFreePageAligned(    void* ptr);

// Authors note:
// Having only a handful allocation function helps detecting basic memory
// errors. Note however that there does not exist any exception handling
// in NALib, meaning an error might be detected though not resolved. And in
// favor of simplicity, NALib will not get exception handling soon.




// ///////////////////////////////
// Core memory structs: NAPtr, NALValue, NAMemoryBlock, NACArray
//
// Following are the declarations and definitions of the four base memory
// structs used in NALib. They seem confusing at first but are very simple
// to understand.
//
// Basically, they all store a C-Pointer. But a C-Pointer can denote anything
// from a single byte up to an array of large structs. The four base structs
// are here to distinguish between them. Basically, they store the following
// information:
//
//                | C-Pointer const and mutable  |  Typesize  |  Total bytesize
// ---------------+------------------------------+------------+----------------
// NAPtr          |               X              |      -     |        -
// NALValue       |               X              |      X     |        -
// NAMemoryBlock  |               X              |      -     |        X
// NACArray       |               X              |      X     |        X
//
// At the core, an NAPtr stores a C-Pointer and the information whether it
// is const or mutable.
//
// An NALValue stores a pointer which denotes a variable placed in memory.
// It has a typesize.
//
// An NAMemoryBlock stores a bunch of bytes. No typesize but a total size
// of bytes is available.
//
// An NACArray finally is the combination of all things. It stores a pointer,
// a total number of bytes available and the typesize of the units stored
// in that array. The number of elements in the array can be computed by
// dividing the total bytesize by the typesize. See API for the corresponding
// function call.
//
// ///////////////////////////////






// ////////////////////////
// NAPtr
//
// Certain structs like NAList or NAArray distinguish between const and
// non-const (mutable) data. The NAPtr struct delivers this distinction by
// providing both an Accessor and/or a Mutator to a C-pointer.
//
// This struct also serves as the core of memory consistency checking when
// debugging. An NAPtr stores many additional information which are not
// accessible to the programmer and would not be available when just using
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
// a non-const data pointer. This idea is not entirely type-save but is much
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
// NALValue, NAMemoryBlock and NACArray are all dependent on NAPtr and declare
// it as the first entry in their struct declaration. Therefore, it is safe
// to access any other memory struct typecastet as an NAPtr. See NAPointer for
// an example.


typedef struct NAPtr NAPtr;

// Creates a NULL pointer
NA_IAPI NAPtr naMakePtr();

// Makes an NAPtr with a newly allocated memory block of the given size. The
// size parameter can be negative. See naMalloc function for more information.
// Note that you maybe will not call this function directly but instead will
// use naMakeLValueWithTypesize or naMakeMemoryBlockWithBytesize.
NA_IAPI NAPtr naMakePtrWithSize(NAInt size);

// Fills the given NAPtr struct with either a const or a non-const pointer.
// The bytesizehint is a hint when debugging. For an explanation of that
// parameter, see naMakeMemoryBlockWithConstBuffer. In the same way, the
// zerofillhint are the number of zero bytes available but invisible to the
// programmer.
NA_IAPI NAPtr naMakePtrWithConstBuffer(  const void* data,
                                               NAInt bytesizehint,
                                              NAUInt zerofillhint);
NA_IAPI NAPtr naMakePtrWithMutableBuffer(      void* data,
                                               NAInt bytesizehint,
                                              NAUInt zerofillhint);

// Assumes srcptr to be a byte array and creates an NAPtr referencing an
// extraction thereof. DOES NOT COPY!
// The offset and size must be given in bytes and are always positive! The size
// is only a hint which helps detecting errors during debugging. When NDEBUG is
// defined, this hint is optimized out.
NA_IAPI NAPtr naMakePtrWithExtraction(  const NAPtr* srcptr,
                                              NAUInt offset,
                                              NAUInt bytesizehint);

// Note that the creation function are naMakeXXX functions which makes it
// easy to implement. But the remaining functions require to provide a pointer.

// The following functions return either a const or a mutable pointer.
//
// When NDEBUG is NOT defined, NALib will check if a const value is accessed
// as a mutator and will emit a warning if so. If the content of the pointer
// is a NULL pointer, no warning will be emitted.
//
// When NDEBUG IS defined, the const and mutable functions behave equally and
// no test will be performed whatsoever.
NA_IAPI const void* naGetPtrConst(const NAPtr* ptr);
NA_IAPI void* naGetPtrMutable(NAPtr* ptr);

// Returns NA_TRUE, if the pointer stores const data. This function only is
// useful when debugging. When NDEBUG is defined, this function always returns
// NA_FALSE.
NA_IAPI NABool naIsPtrConst(NAPtr* ptr);





// ////////////////////////
// NALValue
// ////////////////////////

// Opaque type. See explanation in readme.txt
typedef struct NALValue NALValue;

// Returns an lvalue which is empty.
NA_IAPI NALValue naMakeLValue();

// Create a new lvalue of the given size. The size parameter MUST be positive.
NA_IAPI NALValue naMakeLValueWithTypesize(NAUInt typesize);

// Uses the given buffer WITHOUT copying as an lvalue with the given typesize.
NA_IAPI NALValue naMakeLValueWithConstBuffer(   const void* buffer,
                                                     NAUInt typesize);
NA_IAPI NALValue naMakeLValueWithMutableBuffer(       void* buffer,
                                                     NAUInt typesize);

// Returns informations about the typesize of the given lvalue.
NA_IAPI NAUInt naGetLValueTypesize(const NALValue* lvalue);
NA_IAPI NABool naIsLValueEmpty(const NALValue* lvalue);

// Returns either a const or a mutable pointer to the lvalue.
NA_IAPI const void* naGetLValueConstPointer(const NALValue* lvalue);
NA_IAPI void* naGetLValueMutablePointer(NALValue* lvalue);






// ////////////////////////
// NAMemoryBlock
// ////////////////////////

typedef struct NAMemoryBlock NAMemoryBlock;

// Returns a memory block which is empty.
NA_IAPI NAMemoryBlock naMakeMemoryBlock();

// Creates a new memory block of the given size. The size
// parameter can be negative. See naMalloc function for more information.
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithBytesize(NAInt bytesize);

// Uses the given buffer WITHOUT copying as a memory block with the given size.
// The programmer is responsible that the given size is not overflowing the
// buffer.
// The bytesize can be negative. If so, the absolute value of bytesize is
// used but the given buffer is expected to to have one or more bytes appended
// which are filled with binary zero. Hence, the array can safely be assumed to
// be null-terminated. This also serves as a hint for functions like
// naMakePtrWithConstBuffer
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithConstBuffer( const void* buffer,
                                                              NAInt bytesize);
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithMutableBuffer(     void* buffer,
                                                              NAInt bytesize);

// Makes a new NAMemoryBlock struct containing a sub-part of the given source
// memory block. Does NOT copy! The offset and size are given in bytes and must
// be positive.
NA_IAPI NAMemoryBlock naMakeMemoryBlockWithExtraction(
                                            const NAMemoryBlock* srcmemblock,
                                                          NAUInt offset,
                                                          NAUInt bytesize);

// Returns size information about the memory block.
NA_IAPI NAUInt naGetMemoryBlockBytesize(const NAMemoryBlock* memblock);
NA_IAPI NABool naIsMemoryBlockEmpty(const NAMemoryBlock* memblock);

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
#ifndef NDEBUG
  NA_HIAPI NABool naIsMemoryBlockNullTerminated(const NAMemoryBlock* memblock);
#endif






// ////////////////////////
// NACArray
// ////////////////////////

typedef struct NACArray NACArray;

// Returns a C-array which is empty.
NA_IAPI NACArray naMakeCArray();

// Creates a new memory block with the given typesize and element count. The
// count parameter can be negative. See naMalloc function for more information.
NA_IAPI NACArray naMakeCArrayWithTypesizeAndCount(NAUInt typesize, NAInt count);

// Uses the given buffer WITHOUT copying as a C array with the given typesize
// and element count. The programmer is responsible that the given sizes are
// not overflowing the buffer.
// The count can be negative. If so, the absolute value of count is used but
// the given buffer is expected to to have at least typesize bytes appended
// which are filled with binary zero. Hence, the array can be assumed to
// be null-terminated. This also serves as a hint for functions like
// naMakePtrWithConstBuffer.
// Note that the additional typesize bytes is special. naMalloc only guarantees
// zero filling up to a certain number of bytes. You have to be careful if you
// want to use negative sizes with NACArray.
NA_IAPI NACArray naMakeCArrayWithConstBuffer(   const void* buffer,
                                                     NAUInt typesize,
                                                      NAInt count);
NA_IAPI NACArray naMakeCArrayWithMutableBuffer(       void* buffer,
                                                     NAUInt typesize,
                                                      NAInt count);

// Makes a new NACArray struct containing a sub-part of the given source
// C-array. Does NOT copy! The offset and count are given in units with the
// typesize stored in srcarray and must be positive.
NA_IAPI NACArray naMakeCArrayWithExtraction(const NACArray* srccarray,
                                                     NAUInt offset,
                                                     NAUInt count);

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
#ifndef NDEBUG
  NA_HIAPI NABool naIsCArrayNullTerminated(const NACArray* carray);
#endif










// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <Windows.h>
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
#endif

#include "NAMathOperators.h"


// //////////////////////////////////////
// Accessing informations about the memory system
// //////////////////////////////////////


NA_IDEF NAUInt naGetSystemMemoryPageSize(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (NAUInt)info.dwPageSize;
  #else
    return (NAUInt)sysconf(_SC_PAGESIZE);
  #endif
}



NA_IDEF NAUInt naGetSystemMemoryPageSizeMask(){
  return ~(NAUInt)(naGetSystemMemoryPageSize() - NA_ONE);
}



// Zero fill
//
// The following function returns for a given negative size, how big a buffer
// must be, to include the appended zero bytes.
//
// The allocation functions of NALib expect a positive or negative size. If the
// size is negative, the absolute value is used to allocate sufficient space
// but a certain number of bytes is appended to the memory block which will
// be initialized with binary zero but will not be visible to the programmer.
// 
// The following holds true:
//
// - The additional bytes are all guaranteed to be initialized with binary
//   zero.
// - There are AT LEAST as many bytes appended as an address requires.
//   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
// - There are as many bytes allocated such that the total size of the
//   allocated block is a multiple of an address size, meaning 4 or 8 Bytes
//   depending on the system (32 Bit or 64 Bit).
// - The total size (including the desired space) is at minimum 2 times
//   the number of bytes needed for an address.
// - The part without the additional bytes might partially become
//   initialized with binary zero.

NA_HIDEF NAUInt naGetNullTerminationSize(NAInt size){
  NAInt returnsize;
  #ifndef NDEBUG
    if(!naIsIntNegative(size))
      naError("naGetNullTerminationSize", "size is not negative");
    if(size == NA_INVALID_MEMORY_SIZE)
      naError("naGetNullTerminationSize", "invalid size given");
  #endif
  returnsize = -size + 2 * NA_SYSTEM_ADDRESS_BYTES - (-size % NA_SYSTEM_ADDRESS_BYTES);
  #ifndef NDEBUG
    if(returnsize < 0)
      naError("naGetNullTerminationSize", "given negative size is too close to the minimal integer value");
  #endif
  return (NAUInt)returnsize;
}








// ////////////////////////////////////////////
// Basic Memory allocation and freeing
// ////////////////////////////////////////////



NA_IDEF void* naMalloc(NAInt size){
  // ptr is declared as NAByte to simplify accessing individual bytes later
  // in this functions.
  NAByte* ptr; // Declaration before implementation. Needed for C90.
  NAUInt fullsize;
  #ifndef NDEBUG
    if(naIsIntZero(size))
      naError("naMalloc", "size is zero.");
  #endif

  if(naIsIntStrictlyPositive(size)){
    ptr = (NAByte*)malloc((size_t)size);
  }else{
    #ifndef NDEBUG
      if(size == NA_INT_MIN)
        naError("naMalloc", "given negative size owerflows NAInt type.");
    #endif
    fullsize = naGetNullTerminationSize(size);
    #ifndef NDEBUG
      if((NAInt)fullsize < -size)
        naError("naMalloc", "given size including zero filled endbytes overflows NAInt type.");
    #endif
    ptr = (NAByte*)malloc((size_t)fullsize);
    *(NAUInt*)(&(ptr[fullsize - 2 * NA_SYSTEM_ADDRESS_BYTES])) = NA_ZERO;
    *(NAUInt*)(&(ptr[fullsize - 1 * NA_SYSTEM_ADDRESS_BYTES])) = NA_ZERO;
  }

  #ifndef NDEBUG
    if(!ptr)
      {naCrash("naMalloc", "Out of memory."); return NA_NULL;}
  #endif
  
  return ptr;
}



NA_IDEF void* naMallocIfNull(void* ptr, NAInt size){
  #ifndef NDEBUG
    if(naIsIntZero(size))
      naError("naMallocIfNull", "size is zero.");
  #endif
  if(ptr == NA_NULL){
    return naMalloc(size);
  }else{
    return ptr;
  }
}



NA_IDEF void* naMallocAligned(NAUInt size, NAUInt align){
  // commented out because due to some strange reason, this does not work with
  // clang.
//  #if defined NA_C11 && !defined __cplusplus
//    return aligned_alloc(size, align);
//  #else
    #if NA_SYSTEM == NA_SYSTEM_WINDOWS
      return _aligned_malloc(size, align);
    #else
      void* retptr;
      posix_memalign(&retptr, align, size);
      return retptr;
    #endif
//  #endif
}



NA_IDEF void* naMallocPageAligned(NAUInt size){
  // commented out because due to some strange reason, this does not work with
  // clang.
//  #if defined NA_C11 && !defined __cplusplus
//    return aligned_alloc(size, naGetSystemMemoryPageSize());
//  #else
    #if NA_SYSTEM == NA_SYSTEM_WINDOWS
      return _aligned_malloc(size, naGetSystemMemoryPageSize());
    #else
      return valloc(size);
    #endif
//  #endif
}



NA_IAPI void naFree(void* ptr){
  free(ptr);
}



NA_IDEF void naFreePageAligned(void* ptr){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    _aligned_free(ptr);
  #else
    free(ptr);
  #endif
}





// //////////////////////////
// NAPtr
// //////////////////////////

struct NAPtr{
  union{                        // A union storing either ...
    const void* constd;         // ... const data or ...
    void*       d;              // ... non-const (mutable) data.
  } data;
  #ifndef NDEBUG
    NAUInt flags;               // This field stores some flags.
    NAUInt visiblebytecount;    // nof bytes of the visible byte storage
    NAUInt accessiblebytecount; // nof bytes of the accessible byte storage
  #endif
};
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense.
//
// Also note that NALValue will be fully optimized to a simple C-pointer
// when NDEBUG is defined.
//
// Authors note:
// The decision to include so many debugging information arose over many
// iterations. Mainly NAString was the cause of pushing debugging information
// deeply into the core functionality of NALib. They should be here to help
// with very fundamental datastructures like an NAByteArray but not to distract
// on higher levels. Further more, questions like "Is String Null-Terminated"
// should not be decided during (release) runtime. Therefore, the NALValue
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
  #define NA_PTR_CONST_DATA                0x01
  #define NA_PTR_HAS_VISIBLE_BYTECOUNT     0x02
  #define NA_PTR_HAS_ACCESSIBLE_BYTECOUNT  0x04
  #define NA_PTR_NULL_TERMINATED           0x08



  NA_HIDEF void naMarkPtrWithVisibleSize(NAPtr* ptr, NAUInt visiblebytecount){
    if(naIsIntNegative((NAInt)visiblebytecount))
      naError("naMarkPtrWithVisibleSize", "visiblebytecount seems to be negative but should be unsigned.");
    if(!ptr)
      {naCrash("naMarkPtrWithVisibleSize", "ptr is null"); return;}
    if(ptr->flags & NA_PTR_HAS_VISIBLE_BYTECOUNT)
      naError("naMarkPtrWithVisibleSize", "visible size already marked");
    if(naIsIntNegative((NAInt)visiblebytecount))
      naError("naMarkPtrWithVisibleSize", "visible size overflows NAInt range");
    ptr->flags |= NA_PTR_HAS_VISIBLE_BYTECOUNT;
    ptr->visiblebytecount = visiblebytecount;
  }
  
  

  NA_HIDEF void naMarkPtrWithAccessibleSize(NAPtr* ptr, NAUInt accessiblesize, NABool nullterminated){
    NAUInt nullindx;
    if(naIsIntNegative((NAInt)accessiblesize))
      naError("naMarkPtrWithAccessibleSize", "accessiblesize seems to be negative but should be unsigned.");
    if(!ptr)
      {naCrash("naMarkPtrWithAccessibleSize", "ptr is null"); return;}
    if(!(ptr->flags & NA_PTR_HAS_VISIBLE_BYTECOUNT))
      naError("naMarkPtrWithAccessibleSize", "visible size must be marked first");
    if(ptr->flags & NA_PTR_HAS_ACCESSIBLE_BYTECOUNT)
      naError("naMarkPtrWithAccessibleSize", "accessible size already marked");
    if(naIsIntNegative((NAInt)accessiblesize))
      naError("naMarkPtrWithAccessibleSize", "accessible size overflows NAInt range");
    ptr->flags |= NA_PTR_HAS_ACCESSIBLE_BYTECOUNT;
    ptr->accessiblebytecount = accessiblesize;
    if(nullterminated){ptr->flags |= NA_PTR_NULL_TERMINATED;}
    
    // Now, we check if the last bytes are indeed zero
    nullindx = ptr->visiblebytecount;
    while(nullindx < ptr->accessiblebytecount){
      if(((const NAByte*)(ptr->data.constd))[nullindx] != '\0')
        naError("naMarkPtrWithAccessibleSize", "promised null-termination is not null");
      nullindx++;
    }
  }



  NA_HIDEF NABool naIsPtrNullTerminated(const NAPtr* ptr){
    if(!ptr)
      {naCrash("naIsPtrNullTerminated", "ptr was null"); return NA_FALSE;}
    if(!(ptr->flags & NA_PTR_HAS_ACCESSIBLE_BYTECOUNT))
      naError("naIsPtrNullTerminated", "No accessible size information present. Ptr may or may not be null-terminated.");
    return (ptr->flags & NA_PTR_NULL_TERMINATED);
  }
  
#endif




NA_IDEF NAPtr naMakePtr(){
  NAPtr ptr;
  ptr.data.d = NA_NULL;
  #ifndef NDEBUG
    ptr.flags = 0;    // Do not mark a null pointer as const. Otherwise many
                      // more errors will spawn.
    naMarkPtrWithVisibleSize(&ptr, NA_ZERO);
    naMarkPtrWithAccessibleSize(&ptr, NA_ZERO, NA_FALSE);
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithSize(NAInt size){
  NAPtr ptr;
  #ifndef NDEBUG
    if(naIsIntZero(size))
      naError("naMakePtrWithSize", "size is zero.");
  #endif
  ptr.data.d = naMalloc(size);
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
    if(naIsIntNegative(size)){
      naMarkPtrWithVisibleSize(&ptr, -size);
      naMarkPtrWithAccessibleSize(&ptr, naGetNullTerminationSize(size), NA_TRUE);
    }else{
      naMarkPtrWithVisibleSize(&ptr, size);
      naMarkPtrWithAccessibleSize(&ptr, size, NA_FALSE);
    }
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithConstBuffer(const void* data, NAInt bytesizehint, NAUInt zerofillhint){
  NAPtr ptr;
  ptr.data.constd = data;
  #ifndef NDEBUG
    ptr.flags = NA_PTR_CONST_DATA;
    if(naIsIntNegative(bytesizehint)){
      naMarkPtrWithVisibleSize(&ptr, -bytesizehint);
      naMarkPtrWithAccessibleSize(&ptr, -bytesizehint + zerofillhint, NA_TRUE);
    }else if(naIsIntStrictlyPositive(bytesizehint)){
      naMarkPtrWithVisibleSize(&ptr, bytesizehint);
      naMarkPtrWithAccessibleSize(&ptr, bytesizehint, NA_FALSE);
    }
  #else
    NA_UNUSED(bytesizehint);
    NA_UNUSED(zerofillhint);
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithMutableBuffer(void* data, NAInt bytesizehint, NAUInt zerofillhint){
  NAPtr ptr;
  ptr.data.d = data;
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
    // Note that the accessiblesize is the same as the visible size because
    // the true amount of null-terminating bytes is unknown.
    if(naIsIntNegative(bytesizehint)){
      naMarkPtrWithVisibleSize(&ptr, -bytesizehint);
      naMarkPtrWithAccessibleSize(&ptr, -bytesizehint + zerofillhint, NA_TRUE);
    }else if(naIsIntStrictlyPositive(bytesizehint)){
      naMarkPtrWithVisibleSize(&ptr, bytesizehint);
      naMarkPtrWithAccessibleSize(&ptr, bytesizehint, NA_FALSE);
    }
  #else
    NA_UNUSED(bytesizehint);
    NA_UNUSED(zerofillhint);
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithExtraction(const NAPtr* srcptr, NAUInt offset, NAUInt bytesizehint){
  NAPtr dstptr;
  #ifndef NDEBUG
    if(naIsIntNegative((NAInt)offset))
      naError("naMakePtrWithExtraction", "offset seems to be negative but should be unsigned.");
    if(naIsIntNegative((NAInt)bytesizehint))
      naError("naMakePtrWithExtraction", "bytesizehint seems to be negative but should be unsigned.");
  #endif
  dstptr.data.d = &(((NAByte*)(srcptr->data.d))[offset]);
  #ifndef NDEBUG
    // Now, we set the sizes and decide if certain flags still are valid.
    dstptr.flags = srcptr->flags;
    dstptr.visiblebytecount = bytesizehint;
    dstptr.accessiblebytecount = srcptr->accessiblebytecount - offset;
    if(!(srcptr->flags & NA_PTR_HAS_VISIBLE_BYTECOUNT)){
      // When no size information is available, the NAPtr struct has no
      // purpose.
      naError("naMakePtrWithExtraction", "No size information available. Cannot debug.");
    }else{
      if((offset + bytesizehint) < srcptr->visiblebytecount){
        // The (offset,size) pair implies the dst array to be smaller than the
        // src array. Therefore, null-termination can no more be guaranteed.
        dstptr.flags &= ~NA_PTR_NULL_TERMINATED;
      }else if((srcptr->flags & NA_PTR_HAS_ACCESSIBLE_BYTECOUNT) && ((offset + bytesizehint) > srcptr->accessiblebytecount)){
        // the (offset,size) pair overflows the accessible size. Very bad!
        naError("naMakePtrWithExtraction", "new offset and size overflows storage");
      }else if((offset + bytesizehint) > srcptr->visiblebytecount){
        // the (offset,size) pair overflows the visible size. Ok, but not clean.
        naError("naMakePtrWithExtraction", "new offset and size overflows visible storage");
      }else{
        // the (offset,size) pair ends precisely at where the src array ends.
        // The flags therefore do not change. Nothing to do here.
      }
    }
  #else
    NA_UNUSED(bytesizehint);
  #endif
  return dstptr;
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
      naCrash("naGetPtrMutable", "Mutable access to const ptr.");
  #endif
  return ptr->data.d;
}



NA_IDEF NABool naIsPtrConst(NAPtr* ptr){
  #ifndef NDEBUG
    return (ptr->flags & NA_PTR_CONST_DATA);
  #else
    NA_UNUSED(ptr);
    return NA_FALSE;
  #endif
}





// //////////////////////////
// NALValue
// //////////////////////////

// Opaque type. typedef is above. See explanation in readme.txt
struct NALValue{
  NAPtr   ptr;          // pointer to the first byte
  NAUInt  typesize;     // size of the unit in bytes. Always positive.
};




NA_IDEF NALValue naMakeLValue(){
  NALValue lvalue;
  // Note that we do not initialize the ptr with naMakePtr. Relying solely on
  // the size field is non-redundant, saves some time and sometimes even helps
  // detecting whether you initialized your structs correctly as certain
  // compilers will add guards to non-initialized pointers and will fire
  // with an exception.
  lvalue.typesize = NA_ZERO;
  return lvalue;
}



NA_IDEF NALValue naMakeLValueWithTypesize(NAUInt typesize){
  NALValue lvalue;
  #ifndef NDEBUG
    if(naIsIntZero(typesize))
      naError("naMakeLValueWithTypesize", "typesize is zero.");
    if(naIsIntNegative((NAInt)typesize))
      naError("naMakeLValueWithTypesize", "typesize seems to be negative but should be unsigned.");
  #endif
  lvalue.ptr = naMakePtrWithSize(typesize);
  lvalue.typesize = typesize;
  return lvalue;
}



NA_IDEF NALValue naMakeLValueWithConstBuffer(const void* buffer, NAUInt typesize){
  NALValue lvalue;
  #ifndef NDEBUG
    if(naIsIntZero(typesize))
      naError("naMakeLValueWithTypesize", "typesize is zero.");
  #endif
  lvalue.ptr = naMakePtrWithConstBuffer(buffer, typesize, NA_ZERO);
  lvalue.typesize = typesize;
  return lvalue;
}



NA_IDEF NALValue naMakeLValueWithMutableBuffer(void* buffer, NAUInt typesize){
  NALValue lvalue;
  #ifndef NDEBUG
    if(naIsIntZero(typesize))
      naError("naMakeLValueWithTypesize", "typesize is zero.");
  #endif
  lvalue.ptr = naMakePtrWithMutableBuffer(buffer, typesize, NA_ZERO);
  lvalue.typesize = typesize;
  return lvalue;
}



NA_IDEF NAUInt naGetLValueTypesize(const NALValue* lvalue){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueTypesize", "lvalue is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return lvalue->typesize;
}



NA_IDEF NABool naIsLValueEmpty(const NALValue* lvalue){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naIsLValueEmpty", "lvalue is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return (lvalue->typesize == NA_ZERO);
}



NA_IDEF const void* naGetLValueConstPointer(const NALValue* lvalue){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueConstPointer", "lvalue is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsLValueEmpty(lvalue))
      naError("naGetLValueConstPointer", "lvalue is empty.");
  #endif
  return naGetPtrConst(&(lvalue->ptr));
}



NA_IDEF void* naGetLValueMutablePointer(NALValue* lvalue){
  #ifndef NDEBUG
    if(!lvalue){
      naCrash("naGetLValueMutablePointer", "lvalue is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsLValueEmpty(lvalue))
      naError("naGetLValueMutablePointer", "lvalue is empty.");
  #endif
  return naGetPtrMutable(&(lvalue->ptr));
}



NA_IDEF NABool naIsLValueConst(NALValue* lvalue){
  #ifndef NDEBUG
    return naIsPtrConst(&(lvalue->ptr));
  #else
    NA_UNUSED(lvalue);
    return NA_FALSE;
  #endif
}








// //////////////////////////
// NAMemoryBlock
// //////////////////////////

struct NAMemoryBlock{
  NAPtr   ptr;          // pointer to the first byte
  NAUInt  bytesize;     // size of the block in bytes. Always positive.
};



NA_IDEF NAMemoryBlock naMakeMemoryBlock(){
  NAMemoryBlock memblock;
  // Note that we do not initialize the ptr with naMakePtr. Relying solely on
  // the size field is non-redundant, saves some time and sometimes even helps
  // detecting whether you initialized your structs correctly as certain
  // compilers will add guards to non-initialized pointers and will fire
  // with an exception.
  memblock.bytesize = NA_ZERO;
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithBytesize(NAInt bytesize){
  NAMemoryBlock memblock;
  #ifndef NDEBUG
    if(naIsIntZero(bytesize))
      naError("naMakeMemoryBlockWithBytesize", "bytesize is zero.");
  #endif
  memblock.ptr = naMakePtrWithSize(bytesize);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithConstBuffer(const void* buffer, NAInt bytesize){
  NAMemoryBlock memblock;
  #ifndef NDEBUG
    if(naIsIntZero(bytesize))
      naError("naMakeMemoryBlockWithBytesize", "bytesize is zero.");
  #endif
  memblock.ptr = naMakePtrWithConstBuffer(buffer, bytesize, 1);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithMutableBuffer(void* buffer, NAInt bytesize){
  NAMemoryBlock memblock;
  #ifndef NDEBUG
    if(naIsIntZero(bytesize))
      naError("naMakeMemoryBlockWithBytesize", "bytesize is zero.");
  #endif
  memblock.ptr = naMakePtrWithMutableBuffer(buffer, bytesize, 1);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithExtraction(const NAMemoryBlock* srcmemblock, NAUInt offset, NAUInt bytesize){
  NAMemoryBlock dstmemblock;
  #ifndef NDEBUG
    if(naIsMemoryBlockEmpty(srcmemblock))
      naError("naMakeMemoryBlockWithExtraction", "src memory block is empty");
    if(naIsIntNegative((NAInt)offset))
      naError("naMakeMemoryBlockWithExtraction", "offset seems to be negative but should be unsigned.");
    if(naIsIntNegative((NAInt)bytesize))
      naError("naMakeMemoryBlockWithExtraction", "bytesize seems to be negative but should be unsigned.");
    if((offset + bytesize) > srcmemblock->bytesize)
      naError("naMakeMemoryBlockWithExtraction", "offset and bytesize range out of bounds");
  #endif
  dstmemblock.ptr = naMakePtrWithExtraction(&(srcmemblock->ptr), offset, bytesize);
  dstmemblock.bytesize = bytesize;
  return dstmemblock;
}



NA_IDEF NAUInt naGetMemoryBlockBytesize(const NAMemoryBlock* memblock){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naGetMemoryBlockBytesize", "memblock is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return memblock->bytesize;
}



NA_IDEF NABool naIsMemoryBlockEmpty(const NAMemoryBlock* memblock){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naIsMemoryBlockEmpty", "memblock is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return (memblock->bytesize == NA_ZERO);
}



NA_IDEF const void* naGetMemoryBlockConstPointer(const NAMemoryBlock* memblock){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naGetMemoryBlockConstPointer", "memblock is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsMemoryBlockEmpty(memblock))
      naError("naGetMemoryBlockConstPointer", "memblock is empty.");
  #endif
  return naGetPtrConst(&(memblock->ptr));
}



NA_IDEF void* naGetMemoryBlockMutablePointer(NAMemoryBlock* memblock){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naGetMemoryBlockMutablePointer", "memblock is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsMemoryBlockEmpty(memblock))
      naError("naGetMemoryBlockMutablePointer", "memblock is empty.");
  #endif
  return naGetPtrMutable(&(memblock->ptr));
}



NA_IDEF const void* naGetMemoryBlockConstByte(const NAMemoryBlock* memblock, NAUInt indx){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naGetMemoryBlockConstByte", "memblock is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsIntNegative((NAInt)indx))
      naError("naGetMemoryBlockConstByte", "indx seems to be negative but should be unsigned.");
    if(naIsMemoryBlockEmpty(memblock))
      naError("naGetMemoryBlockConstByte", "memblock is empty.");
  #endif
  #ifndef NDEBUG
    if(indx >= memblock->bytesize)
      naError("naGetMemoryBlockConstByte", "indx out of bounds");
  #endif
  return &(((const NAByte*)(naGetPtrConst(&(memblock->ptr))))[indx]);
}



NA_IDEF void* naGetMemoryBlockMutableByte(NAMemoryBlock* memblock, NAUInt indx){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naGetMemoryBlockMutableByte", "memblock is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsIntNegative((NAInt)indx))
      naError("naGetMemoryBlockMutableByte", "indx seems to be negative but should be unsigned.");
    if(naIsMemoryBlockEmpty(memblock))
      naError("naGetMemoryBlockMutableByte", "memblock is empty.");
  #endif
  #ifndef NDEBUG
    if(indx >= memblock->bytesize)
      naError("naGetMemoryBlockMutableByte", "indx out of bounds");
  #endif
  return &(((NAByte*)(naGetPtrMutable(&(memblock->ptr))))[indx]);
}



NA_IDEF NABool naIsMemoryBlockConst(NAMemoryBlock* memblock){
  #ifndef NDEBUG
    return naIsPtrConst(&(memblock->ptr));
  #else
    NA_UNUSED(memblock);
    return NA_FALSE;
  #endif
}



#ifndef NDEBUG

  NA_HIDEF NABool naIsMemoryBlockNullTerminated(const NAMemoryBlock* memblock){
    #ifndef NDEBUG
      if(!memblock){
        naCrash("naIsMemoryBlockNullTerminated", "memblock is Null-Pointer.");
        return NA_TRUE;
      }
    #endif
    return naIsPtrNullTerminated(&(memblock->ptr));
  }

#endif







// //////////////////////////
// CArray
// //////////////////////////

struct NACArray{
  NAPtr   ptr;          // pointer to the first byte
  NAUInt  typesize;     // size of one unit in bytes. Always positive.
  NAUInt  bytesize;     // size of the array in bytes. Always positive.
};



NA_IDEF NACArray naMakeCArray(){
  NACArray carray;
  // Note that we do not initialize the ptr with naMakePtr. Relying solely on
  // the size field is non-redundant, saves some time and sometimes even helps
  // detecting whether you initialized your structs correctly as certain
  // compilers will add guards to non-initialized pointers and will fire
  // with an exception.
  carray.typesize = NA_ZERO;
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithTypesizeAndCount(NAUInt typesize, NAInt count){
  NACArray carray;
  #ifndef NDEBUG
    if(naIsIntNegative((NAInt)typesize))
      naError("naMakeCArrayWithTypesizeAndCount", "typesize seems to be negative but should be unsigned.");
    if(naIsIntZero(count))
      naError("naMakeCArrayWithTypesizeAndCount", "count is zero.");
  #endif
  carray.ptr = naMakePtrWithSize(typesize * count);
  carray.bytesize = naAbsi(typesize * count);
  carray.typesize = typesize;
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithConstBuffer(const void* buffer, NAUInt typesize, NAInt count){
  NACArray carray;
  #ifndef NDEBUG
    if(naIsIntNegative((NAInt)typesize))
      naError("naMakeCArrayWithConstBuffer", "typesize seems to be negative but should be unsigned.");
    if(naIsIntZero(count))
      naError("naMakeCArrayWithConstBuffer", "count is zero.");
  #endif
  carray.ptr = naMakePtrWithConstBuffer(buffer, typesize * count, typesize);
  carray.bytesize = naAbsi(typesize * count);
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithMutableBuffer(void* buffer, NAUInt typesize, NAInt count){
  NACArray carray;
  #ifndef NDEBUG
    if(naIsIntNegative((NAInt)typesize))
      naError("naMakeCArrayWithMutableBuffer", "typesize seems to be negative but should be unsigned.");
    if(naIsIntZero(count))
      naError("naMakeCArrayWithMutableBuffer", "count is zero.");
  #endif
  carray.ptr = naMakePtrWithMutableBuffer(buffer, typesize * count, typesize);
  carray.bytesize = naAbsi(typesize * count);
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithExtraction(const NACArray* srccarray, NAUInt offset, NAUInt count){
  NACArray dstcarray;
  #ifndef NDEBUG
    if(naIsCArrayEmpty(srccarray))
      naError("naMakeCArrayWithExtraction", "src memory block is empty");
    if(naIsIntNegative((NAInt)offset))
      naError("naMakeCArrayWithExtraction", "offset seems to be negative but should be unsigned.");
    if(naIsIntNegative((NAInt)count))
      naError("naMakeCArrayWithExtraction", "bytesize seems to be negative but should be unsigned.");
    if(((offset + count) * srccarray->typesize) > srccarray->bytesize)
      naError("naMakeCArrayWithExtraction", "offset and bytesize range out of bounds");
  #endif
  dstcarray.ptr = naMakePtrWithExtraction(&(srccarray->ptr), offset * srccarray->typesize, count * srccarray->typesize);
  dstcarray.bytesize = count * srccarray->typesize;
  return dstcarray;
}



NA_IDEF NAUInt naGetCArrayBytesize(const NACArray* carray){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naGetCArrayBytesize", "carray is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return carray->bytesize;
}



NA_IDEF NAUInt naGetCArrayCount(const NACArray* carray){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naGetCArrayBytesize", "carray is Null-Pointer.");
      return NA_TRUE;
    }
    if(naIsCArrayEmpty(carray))
      naError("naGetCArrayConstPointer", "carray is empty. Division by zero ahead...");
  #endif
  return carray->bytesize / carray->typesize;
}



NA_IDEF NABool naIsCArrayEmpty(const NACArray* carray){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naIsCArrayEmpty", "carray is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return (carray->typesize == NA_ZERO);
}



NA_IDEF const void* naGetCArrayConstPointer(const NACArray* carray){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naGetCArrayConstPointer", "carray is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsCArrayEmpty(carray))
      naError("naGetCArrayConstPointer", "carray is empty.");
  #endif
  return naGetPtrConst(&(carray->ptr));
}



NA_IDEF void* naGetCArrayMutablePointer(NACArray* carray){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naGetCArrayMutablePointer", "carray is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsCArrayEmpty(carray))
      naError("naGetCArrayMutablePointer", "carray is empty.");
  #endif
  return naGetPtrMutable(&(carray->ptr));
}



NA_IDEF const void* naGetCArrayConstByte(const NACArray* carray, NAUInt indx){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naGetCArrayConstByte", "carray is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsIntNegative((NAInt)indx))
      naError("naGetCArrayConstByte", "indx seems to be negative but should be unsigned.");
    if(naIsCArrayEmpty(carray))
      naError("naGetCArrayConstByte", "carray is empty.");
  #endif
  #ifndef NDEBUG
    if((indx * carray->typesize) >= carray->bytesize)
      naError("naGetCArrayConstByte", "indx out of bounds");
  #endif
  return &(((const NAByte*)(naGetPtrConst(&(carray->ptr))))[indx * carray->typesize]);
}



NA_IDEF void* naGetCArrayMutableByte(NACArray* carray, NAUInt indx){
  #ifndef NDEBUG
    if(!carray){
      naCrash("naGetCArrayMutableByte", "carray is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsIntNegative((NAInt)indx))
      naError("naGetCArrayMutableByte", "indx seems to be negative but should be unsigned.");
    if(naIsCArrayEmpty(carray))
      naError("naGetCArrayMutableByte", "carray is empty.");
  #endif
  #ifndef NDEBUG
    if((indx * carray->typesize) >= carray->bytesize)
      naError("naGetCArrayMutableByte", "indx out of bounds");
  #endif
  return &(((NAByte*)(naGetPtrMutable(&(carray->ptr))))[indx * carray->typesize]);
}



NA_IDEF NABool naIsCArrayConst(NACArray* carray){
  #ifndef NDEBUG
    return naIsPtrConst(&(carray->ptr));
  #else
    NA_UNUSED(carray);
    return NA_FALSE;
  #endif
}



#ifndef NDEBUG

  NA_HIDEF NABool naIsCArrayNullTerminated(const NACArray* carray){
    #ifndef NDEBUG
      if(!carray){
        naCrash("naIsCArrayNullTerminated", "carray is Null-Pointer.");
        return NA_TRUE;
      }
    #endif
    return naIsPtrNullTerminated(&(carray->ptr));
  }

#endif








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
