
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
// - NALValue: Handling of Lvalues, being mutable or const pointers with lots
//   of debugging helping goods.
//
// If you want reference counted values which additionally can be automatically
// created and deleted, you can include NARuntime.h instead of NAMemory.h. You
// then can additionally use the following things:
//
// - NAPointer: Handling of reference counted Lvalues with automatic deletion.
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
//                      NA_NULL, memo ry is allocated and the pointer to that
//                      memory is returned. If the pointer is not NA_NULL,
//                      nothing is allocated and the same pointer is returned.
// naAlloc              This is a macro which expands to naMalloc but can be
//                      used to allocate enough space for a struct type. For
//                      exampe, you can simply write array = naAlloc(NAArray);
// naMallocAligned      The given size is allocated and the returned pointer
//                      is guaranteed to be aligned on the given bound. Such
//                      a pointer must be freed with naFreePageAligned. Beware
//                      that size here can not be negative!
// naMallocPageAligned  Same thing but the bound is the memory-page boundary.
// naFree               Deallocates a pointer previously allocated with one of
//                      the above functions or the default C memory allocation
//                      functions like malloc.
// naFreePageAligned    Deallocates a page-aligned pointer previously allocated
//                      with naMallocPageAligned.
//
NA_IAPI void* naMalloc(             NAInt size);
NA_IAPI void* naMallocIfNull(       void* ptr, NAInt size);
#define       naAlloc(structname)   (structname*)naMalloc(sizeof(structname))
NA_IAPI void* naMallocAligned(      NAUInt size, NAUInt align);
NA_IAPI void* naMallocPageAligned(  NAUInt size);
NA_IAPI void  naFree(               void* ptr);
NA_IDEF void  naFreePageAligned(    void* ptr);

// Authors note:
// Having only a handful allocation function helps detecting basic memory
// errors. Note however that there does not exist any exception handling
// in NALib, meaning an error might be detected though not resolved. And in
// favor of simplicity, NALib will not get exception handling soon.









// ////////////////////////
// NALValue
//
// Certain structs like NAList or NAArray distinguish between const and
// non-const (mutable) data. The NALValue struct delivers this
// distinction by providing both an Accessor or a Mutator to a pointer.
//
// In NALib, the author decided to use a union type storing either a const or
// a non-const data pointer. This idea is not entirely type-save but is much
// easier to write programs with, as the programmer has to differentiate
// between const and non-const only when necessary.
//
// If the programmer uses a mutator on const data, an error will be emitted.
// You can of course ignore these errors and hope that the application does not
// crash, but you should really make sure no const data gets mutated.
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
// Above all, NALib allows you to tag Lvalues with various flags like if the
// pointer denotes an array and whether that array is null-terminated or not.
// During runtime, NALib checks if all accesses are fine.
//
// During debugging, using NALValue can greatly improve your code-safety.
// Note however, that all the features of Lvalues are only avaliable when
// debugging. When compiling with NDEBUG, all your code will still compile just
// fine but NALValue will optimize to a simple (mutable) C-Pointer. No tags
// will be available and no code is executed.

// Opaque type. See explanation in readme.txt
typedef struct NALValue NALValue;

// Fills the given NALValue struct with either a const or a non-const pointer.
NA_IAPI NALValue naMakeLValueConst  (const void* data);
NA_IAPI NALValue naMakeLValueMutable(      void* data);

// Fills the given lvalue with a new memory block of the given size. The size
// parameter can be negative. See naMalloc function for more information.
NA_IDEF NALValue naMakeLValueWithSize(NAInt size);

// Assumes srclvalue to be a byte array and dstlvalue to become a sub-array.
// The offset and size must be given in bytes and are always positive!
NA_IAPI NALValue naMakeLValueSub( const NALValue* srclvalue,
                                           NAUInt offset,
                                           NAUInt size);

// Note that the creation function are naMakeXXX functions which makes it
// easy to implement. But the remaining functions require to provide a pointer.

// The following functions return either a const or a mutable pointer.
//
// When NDEBUG is not defined, NALib will check if a const value is accessed
// as a mutator and will emit a warning if so. If the content of the pointer
// is a NULL pointer, no warning will be emitted.
//
// When NDEBUG is defined, the const and mutable functions behave equally and
// no test will be performed whatsoever.
NA_IAPI const void* naGetLValueConst(      const NALValue* lvalue);
NA_IAPI       void* naGetLValueMutable(          NALValue* lvalue);
// Same thing but with lvalue denoting a byte-array whereas the returned
// pointer will be a pointer to the given (unsigned!) index in bytes.
// If NDEBUG is defined, the indx argument will be completely ignored.
NA_IAPI const void* naGetLValueOffsetConst(const NALValue* lvalue, NAUInt indx);
NA_IAPI       void* naGetLValueOffsetMutable(    NALValue* lvalue, NAUInt indx);

// Returns NA_TRUE, if the pointer stores const data. This function only is
// useful when debugging. When NDEBUG is defined, this function always returns
// NA_FALSE.
NA_IAPI NABool naIsLValueConst(NALValue* lvalue);

// The following functions are only available when debugging. Beware: You may
// have to encapsulate some of your own code with #ifndef NDEBUG to compile!
#ifndef NDEBUG
  // The visible size denotes the size which can be filled with content.
  NA_IAPI void naMarkLValueWithVisibleSize(NALValue* lvalue, NAUInt visiblebytecount);
  // The accessible size denotes the size which can be safely accessed without
  // creating a system signal. This is for example used when a pointer denotes
  // an offset inside an array or for zero-filled arrays (see above).
  NA_IAPI void naMarkLValueWithAccessibleSize(NALValue* lvalue, NAUInt visiblebytecount, NABool nullterminated);
  // Returns true if the lvalue denotes an array which is null-terminated.
  NA_IAPI NABool naIsLValueNullTerminated(const NALValue* lvalue);
#endif
// Authors note:
// Why have these last functions been encapsulated into NDEBUG clauses? Because
// behaviour is undefined when the information is not available. And the author
// does not want to propose any assumption in that case. You have to find a
// solution for yourself!













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <Windows.h>
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
#endif


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
// must be to include the appended zero bytes.
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
  #ifndef NDEBUG
    if(!naIsIntNegative(size))
      naError("naGetNullTerminationSize", "size is not negative");
  #endif
  return -size + 2 * NA_SYSTEM_ADDRESS_BYTES - (-size % NA_SYSTEM_ADDRESS_BYTES);
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
  #ifdef NA_C11
    return aligned_alloc(size, align);
  #else
    #if NA_SYSTEM == NA_SYSTEM_WINDOWS
      return _aligned_malloc(size, align);
    #else
      void* retptr;
      posix_memalign(&retptr, align, size);
      return retptr;
    #endif
  #endif
}



NA_IDEF void* naMallocPageAligned(NAUInt size){
  #ifdef NA_C11
    return aligned_alloc(size, naGetSystemMemoryPageSize());
  #else
    #if NA_SYSTEM == NA_SYSTEM_WINDOWS
      return _aligned_malloc(size, naGetSystemMemoryPageSize());
    #else
      return valloc(size);
    #endif
  #endif
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
// NALValue
// //////////////////////////

// Opaque type. typedef is above. See explanation in readme.txt
struct NALValue{
  union{                    // A union storing either ...
    const void* constd;     // ... const data or ...
    void*       d;          // ... non-const (mutable) data.
  } data;
  #ifndef NDEBUG
    NAUInt flags;               // This field stores some flags.
    NAUInt visiblebytecount;    // nof bytes of the visible byte storage
    NAUInt accessiblebytecount; // nof bytes of the accessible byte storage
  #endif
};
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense. Also note that the pointers are here declared as
// NAByte pointers. This makes it easier to write the accessing functions.
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
// devastating than helpful. Therefore, the NALValue struct was added which
// provides both a useful extension to differentiate between const and mutable
// as well as additional debugging functions for all who want to really make
// sure their code is doing what is expected.
//
// If you simply want to store a pointer without any knowledge of const or
// mutable and without any debugging information, just use a normal C-pointer.

#ifndef NDEBUG
  #define NA_LVALUE_CONST_DATA                0x01
  #define NA_LVALUE_HAS_VISIBLE_BYTECOUNT     0x02
  #define NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT  0x04
  #define NA_LVALUE_NULL_TERMINATED           0x08
#endif



NA_IDEF NALValue naMakeLValueConst(const void* data){
  NALValue lvalue;
  lvalue.data.constd = data;
  #ifndef NDEBUG
    lvalue.flags = NA_LVALUE_CONST_DATA;
  #endif
  return lvalue;
}



NA_IDEF NALValue naMakeLValueMutable(void* data){
  NALValue lvalue;
  lvalue.data.d = data;
  #ifndef NDEBUG
    lvalue.flags = NA_ZERO;
  #endif
  return lvalue;
}



NA_IDEF NALValue naMakeLValueWithSize(NAInt size){
  NALValue lvalue;
  #ifndef NDEBUG
    if(naIsIntZero(size))
      naError("naMakeLValueWithSize", "size is zero.");
  #endif
  lvalue.data.d = naMalloc(size);
  #ifndef NDEBUG
    lvalue.flags = NA_ZERO;
    if(naIsIntNegative(size)){
      naMarkLValueWithVisibleSize(&lvalue, -size);
      naMarkLValueWithAccessibleSize(&lvalue, naGetNullTerminationSize(size), NA_TRUE);
    }else{
      naMarkLValueWithVisibleSize(&lvalue, size);
      naMarkLValueWithAccessibleSize(&lvalue, size, NA_FALSE);
    }
  #endif
  return lvalue;
}



NA_IDEF NALValue naMakeLValueSub(const NALValue* srclvalue, NAUInt offset, NAUInt size){
  NALValue dstlvalue;
  dstlvalue.data.constd = naGetLValueOffsetConst(srclvalue, offset);
  #ifndef NDEBUG
    // Now, we set the sizes and decide if certain flags still are valid.
    dstlvalue.flags = srclvalue->flags;
    dstlvalue.visiblebytecount = size;
    dstlvalue.accessiblebytecount = dstlvalue.accessiblebytecount - offset;
    if(!(srclvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT)){
      // When no size information is available, the NALValue struct has no
      // purpose.
      naError("naMakeLValueSub", "No size information available. Cannot debug.");
    }else{
      if((offset + size) < srclvalue->visiblebytecount){
        // The (offset,size) pair implies the dst array to be smaller than the
        // src array. Therefore, null-termination is no more.
        dstlvalue.flags &= ~NA_LVALUE_NULL_TERMINATED;
      }else if((srclvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT) && ((offset + size) > srclvalue->accessiblebytecount)){
        // the (offset,size) pair overflows the accessible size. Very bad!
        naError("naMakeLValueSub", "new offset and size overflows storage");
      }else if((offset + size) > srclvalue->visiblebytecount){
        // the (offset,size) pair overflows the visible size. Ok, but not clean.
        naError("naMakeLValueSub", "new offset and size overflows visible storage");
      }else{
        // the (offset,size) pair ends precisely at where the src array ends.
        // The flags therefore do not change. Nothing to do here.
      }
    }
  #else
    NA_UNUSED(size);
  #endif
  return dstlvalue;
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
    if(!lvalue)
      {naCrash("naGetLValueMutable", "lvalue is Null-Pointer."); return NA_NULL;}
    // An NALValue often stores Null-Pointers. In that case, just return
    // NA_NULL and do not emit any warning.
    if(lvalue->data.d == NA_NULL){return NA_NULL;}
    if(lvalue->flags & NA_LVALUE_CONST_DATA)
      naError("naGetLValueMutable", "Accessing const data as non-const.");
  #endif
  return lvalue->data.d;
}



NA_IDEF const void* naGetLValueOffsetConst(const NALValue* lvalue, NAUInt indx){
  #ifndef NDEBUG
    if(!lvalue)
      {naCrash("naGetLValueOffsetConst", "lvalue is Null-Pointer."); return NA_NULL;}
    if((lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT) && (indx >= lvalue->visiblebytecount)){
      if((lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT) && (indx >= lvalue->accessiblebytecount))
        naError("naGetLValueOffsetConst", "index out of accessible bounds");
      else
        naError("naGetLValueOffsetConst", "index out of visible bounds");
    }
  #endif
  return &(((const NAByte*)(lvalue->data.constd))[indx]);
}



NA_IDEF void* naGetLValueOffsetMutable(NALValue* lvalue, NAUInt indx){
  #ifndef NDEBUG
    if(!lvalue)
      {naCrash("naGetLValueOffsetMutable", "lvalue is Null-Pointer."); return NA_NULL;}
    // An NALValue often stores Null-Pointers. In that case, just return
    // NA_NULL and do not emit any warning.
    if(lvalue->data.d == NA_NULL){return NA_NULL;}
    if(lvalue->flags & NA_LVALUE_CONST_DATA)
      naError("naGetLValueOffsetMutable", "Accessing const data as non-const.");
    if((lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT) && (indx >= lvalue->visiblebytecount)){
      if((lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT) && (indx >= lvalue->accessiblebytecount))
        naError("naGetLValueOffsetMutable", "index out of accessible bounds");
      else
        naError("naGetLValueOffsetMutable", "index out of visible bounds");
    }
  #endif
  return &(((NAByte*)(lvalue->data.d))[indx]);
}



#ifndef NDEBUG

  NA_IDEF void naMarkLValueWithVisibleSize(NALValue* lvalue, NAUInt visiblebytecount){
    if(!lvalue)
      {naCrash("naMarkLValueWithVisibleSize", "lvalue is null"); return;}
    if(lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT)
      naError("naMarkLValueWithVisibleSize", "visible size already marked");
    if(naIsIntZero(visiblebytecount))
      naError("naMarkLValueWithVisibleSize", "visible size is zero");
    if(naIsIntNegative((NAInt)visiblebytecount))
      naError("naMarkLValueWithVisibleSize", "visible size overflows NAInt range");
    lvalue->flags |= NA_LVALUE_HAS_VISIBLE_BYTECOUNT;
    lvalue->visiblebytecount = visiblebytecount;
  }
  
  

  NA_IDEF void naMarkLValueWithAccessibleSize(NALValue* lvalue, NAUInt accessiblesize, NABool nullterminated){
    NAUInt nullindx;
    if(!lvalue)
      {naCrash("naMarkLValueWithAccessibleSize", "lvalue is null"); return;}
    if(!(lvalue->flags & NA_LVALUE_HAS_VISIBLE_BYTECOUNT))
      naError("naMarkLValueWithAccessibleSize", "visible size must be marked first");
    if(lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT)
      naError("naMarkLValueWithAccessibleSize", "accessible size already marked");
    if(naIsIntNegative((NAInt)accessiblesize))
      naError("naMarkLValueWithAccessibleSize", "accessible size overflows NAInt range");
    if(naIsIntZero(accessiblesize))
      naError("naMarkLValueWithAccessibleSize", "accessible size is zero");
    lvalue->flags |= NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT;
    lvalue->accessiblebytecount = accessiblesize;
    if(nullterminated){lvalue->flags |= NA_LVALUE_NULL_TERMINATED;}
    
    // Now, we check if the last bytes are indeed zero
    nullindx = lvalue->visiblebytecount;
    while(nullindx < lvalue->accessiblebytecount){
      if(((const NAByte*)(lvalue->data.constd))[nullindx] != '\0')
        naError("naMarkLValueWithAccessibleSize", "promised null-termination is not null");
      nullindx++;
    }
  }



  NA_IDEF NABool naIsLValueNullTerminated(const NALValue* lvalue){
    if(!lvalue)
      {naCrash("naIsLValueNullTerminated", "lvalue was null"); return NA_FALSE;}
    if(!(lvalue->flags & NA_LVALUE_HAS_ACCESSIBLE_BYTECOUNT))
      naError("naIsLValueNullTerminated", "No accessible size information present. LValue may or may not be null-terminated.");
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
