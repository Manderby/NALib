// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemory.h"



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <Windows.h>
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include "malloc/malloc.h"
#endif

#include "NAMathOperators.h"
#include "NAValueHelper.h"


// //////////////////////////////////////
// Accessing informations about the memory system
// //////////////////////////////////////


NA_IDEF NAUInt naGetSystemMemoryPagesize(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (NAUInt)info.dwPageSize;
  #else
    return (NAUInt)sysconf(_SC_PAGESIZE);
  #endif
}



NA_IDEF NAUInt naGetSystemMemoryPagesizeMask(){
  return ~(NAUInt)(naGetSystemMemoryPagesize() - NA_ONE);
}



// Zero fill
//
// The following function returns for a given negative bytesize, how big a buffer
// must be, to include the appended zero bytes.
//
// The allocation functions of NALib expect a positive or negative bytesize. If the
// bytesize is negative, the absolute value is used to allocate sufficient space
// but a certain number of bytes is appended to the memory block which will
// be initialized with binary zero but will not be visible to the programmer.
// 
// The following holds true:
//
// - The additional bytes are all guaranteed to be initialized with binary
//   zero.
// - There are AT LEAST as many bytes appended as an address requires.
//   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
// - There are as many bytes allocated such that the total bytesize of the
//   allocated block is a multiple of an address bytesize, meaning 4 or 8 Bytes
//   depending on the system (32 Bit or 64 Bit).
// - The total bytesize (including the desired space) is at minimum 2 times
//   the number of bytes needed for an address.
// - The part without the additional bytes might partially become
//   initialized with binary zero.

NA_HIDEF NAInt naGetNullTerminationBytesize(NAInt bytesize){
  NAInt returnbytesize;
  #ifndef NDEBUG
    if((bytesize >= NA_ZERO))
      naError("naGetNullTerminationBytesize", "size is not negative");
    if(bytesize == NA_INVALID_MEMORY_BYTESIZE)
      naError("naGetNullTerminationBytesize", "invalid size given");
  #endif
  returnbytesize = -bytesize + (NA_SYSTEM_ADDRESS_BYTES << 1) - (-bytesize % NA_SYSTEM_ADDRESS_BYTES);
  #ifndef NDEBUG
    if(returnbytesize < NA_ZERO)
      naError("naGetNullTerminationBytesize", "given negative size is too close to the minimal integer value");
  #endif
  return returnbytesize;
}








// ////////////////////////////////////////////
// Basic Memory allocation and freeing
// ////////////////////////////////////////////


#ifndef NDEBUG
  extern NAInt  na_debug_mem_bytesize;
  extern NAInt  na_debug_mem_invisiblebytesize;
  extern NABool na_debug_mem_observe_bytes;
//  #include "NAThreading.h"
#endif



NA_IDEF void* naMalloc(NAInt bytesize){
  NAByte* ptr; // Declaration before implementation. Needed for C90.
  NAInt fullsize;
  // ptr is declared as NAByte to simplify accessing individual bytes later
  // in this functions.

  #ifndef NDEBUG
    if(bytesize == NA_ZERO)
      naError("naMalloc", "size is zero.");
  #endif

  if(bytesize > NA_ZERO){
    ptr = (NAByte*)malloc((size_t)bytesize);
    #ifndef NDEBUG
      if(na_debug_mem_observe_bytes){
        na_debug_mem_bytesize += bytesize;
      }
    #endif
  }else{
    #ifndef NDEBUG
      if(bytesize == NA_INT_MIN)
        naError("naMalloc", "given negative size owerflows NAInt type.");
    #endif
    fullsize = naGetNullTerminationBytesize(bytesize);
    #ifndef NDEBUG
      if(fullsize < -bytesize)
        naError("naMalloc", "given size including zero filled endbytes overflows NAInt type.");
    #endif
    ptr = (NAByte*)malloc((size_t)fullsize);
    #ifndef NDEBUG
    if (!ptr)
      {naCrash("naMalloc", "Out of memory."); return NA_NULL;}
    if(na_debug_mem_observe_bytes){
        na_debug_mem_bytesize += fullsize;
      }
    #endif
    *(NAUInt*)(&(ptr[fullsize - 2 * NA_SYSTEM_ADDRESS_BYTES])) = NA_ZERO;
    *(NAUInt*)(&(ptr[fullsize - 1 * NA_SYSTEM_ADDRESS_BYTES])) = NA_ZERO;
  }

  return ptr;
}


NA_IDEF void naFree(void* ptr){
  free(ptr);
}



NA_IDEF void* naMallocAligned(NAUInt bytesize, NAUInt align){
  void* retptr;
  // Usually, aligned memory can be created in unix like systems using the
  // following three methods. Unfortunately, none of them work reliably on
  // Mac OS X.
  // - aligned_alloc under C11 is unreliable when using clang
  // - posix_memalign returns misaligned pointers in Snow Leopard
  // - malloc_zone_memalign same thing as posix_memalign
  // Therefore, a custom implementation is used which is costly but what
  // the hell.
    

  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    retptr = _aligned_malloc(bytesize, align);
  #else
    // Allocate a full align and a pointer more than required.
    void* mem = malloc(bytesize + align + sizeof(void*));
    // make a pointer point to the first byte being aligned within the memory
    // allocated in mem which comes after align bytes and a pointer size.
    void** ptr = (void**)((size_t)((NAByte*)mem + align + sizeof(void*)) & ~(align - NA_ONE));
    // Store the pointer to the original allocation pointer in the element
    // right before the first returned byte.
    ptr[-1] = mem;
    retptr = ptr;
  #endif
  
  #ifndef NDEBUG
    if(na_debug_mem_observe_bytes){
      na_debug_mem_bytesize += bytesize;
      na_debug_mem_invisiblebytesize += align + sizeof(void*);
    }
  #endif

  #ifndef NDEBUG
    if(((NAUInt)retptr & (NAUInt)(align - NA_ONE)) != NA_ZERO)
      naError("naMallocAligned", "pointer unaligned.");
  #endif
  
  return retptr;
}



NA_IDEF void* naMallocPageAligned(NAUInt bytesize){
  // Note that due to some strange reason, aligned_alloc and valloc not work
  // with clang. Therefore, a simple call to naMallocAligned is used.
  return naMallocAligned(bytesize, naGetSystemMemoryPagesize());
}



NA_IDEF void naFreeAligned(void* ptr){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    _aligned_free(ptr);
  #else
    free(((void**)ptr)[-1]);
  #endif
}






NA_IDEF void naStartMemoryObservation(){
  #ifndef NDEBUG
    na_debug_mem_observe_bytes = NA_TRUE;
    na_debug_mem_bytesize = NA_ZERO;
    na_debug_mem_invisiblebytesize = NA_ZERO;
  #endif
}
NA_IDEF void naStopMemoryObservation(){
  #ifndef NDEBUG
    na_debug_mem_observe_bytes = NA_FALSE;
  #endif
}
NA_IDEF void naContinueMemoryObservation(){
  #ifndef NDEBUG
    na_debug_mem_observe_bytes = NA_TRUE;
  #endif
}
NA_IDEF NAInt naGetMemoryObservation(){
  #ifndef NDEBUG
    return na_debug_mem_bytesize + na_debug_mem_invisiblebytesize;
  #else
    return NA_ZERO;
  #endif
}
NA_IDEF NAInt naGetMemoryObservationVisible(){
  #ifndef NDEBUG
    return na_debug_mem_bytesize;
  #else
    return NA_ZERO;
  #endif
}
NA_IDEF NAInt naGetMemoryObservationInvisible(){
  #ifndef NDEBUG
    return na_debug_mem_invisiblebytesize;
  #else
    return NA_ZERO;
  #endif
}




// For the NAPointer struct, we need to have the information available about
// how the memory was originally alloced. We store it as the high-order part
// of the refcount which is why we need to bit-shift it to the left.
// To simplify matters, also the NAPtr struct stores this information as left-
// shifted bits in the flags when NDEBUG ist undefined.
#define NA_MEMORY_CLEANUP_BITSHIFT   (NA_SYSTEM_ADDRESS_BITS - 3)


NA_API void*              naNewStruct(NATypeInfo* typeidentifier);


struct NATypeInfo{
  void*             curpool;
  NAUInt            typesize;
  NAMutator         destructor;
};


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
    NAUInt visiblebytesize;    // nof bytes of the visible byte storage
    NAUInt accessiblebytesize; // nof bytes of the accessible byte storage
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
  #define NA_PTR_CONST_DATA                 0x01
  #define NA_PTR_HAS_VISIBLE_BYTECOUNT      0x02
  #define NA_PTR_HAS_ACCESSIBLE_BYTECOUNT   0x04
  #define NA_PTR_NULL_TERMINATED            0x08
  #define NA_PTR_CLEANED                    0x10



  NA_HIDEF void naMarkPtrCleanup(NAPtr* ptr, NAMemoryCleanup cleanup){
    if((cleanup < NA_MEMORY_CLEANUP_NONE) || (cleanup > NA_MEMORY_CLEANUP_DELETE))
      naError("naMarkPtrCleanup", "Invalid cleanup option");
    if(((ptr->flags >> NA_MEMORY_CLEANUP_BITSHIFT) >= NA_MEMORY_CLEANUP_NONE) && ((ptr->flags >> NA_MEMORY_CLEANUP_BITSHIFT) <= NA_MEMORY_CLEANUP_DELETE))
      naError("naMarkPtrCleanup", "ptr already marked with a cleanup");
    ptr->flags |= (NAUInt)(cleanup) << NA_MEMORY_CLEANUP_BITSHIFT;
  }
  
  

  NA_HIDEF void naMarkPtrWithVisibleBytesize(NAPtr* ptr, NAInt visiblebytesize){
    if(visiblebytesize < NA_ZERO)
      naError("naMarkPtrWithVisibleBytesize", "visiblebytesize should not be negative.");
    if(!ptr)
      {naCrash("naMarkPtrWithVisibleBytesize", "ptr is Null-Pointer"); return;}
    if(ptr->flags & NA_PTR_HAS_VISIBLE_BYTECOUNT)
      naError("naMarkPtrWithVisibleBytesize", "visible size already marked");
    ptr->flags |= NA_PTR_HAS_VISIBLE_BYTECOUNT;
    ptr->visiblebytesize = visiblebytesize;
  }
  
  

  NA_HIDEF void naMarkPtrWithAccessibleBytesize(NAPtr* ptr, NAInt accessiblebytesize, NABool nullterminated){
    NAUInt nullindx;
    if(!ptr)
      {naCrash("naMarkPtrWithAccessibleBytesize", "ptr is Null-Pointer"); return;}
    if(!(ptr->flags & NA_PTR_HAS_VISIBLE_BYTECOUNT))
      naError("naMarkPtrWithAccessibleBytesize", "visible size must be marked first");
    if(ptr->flags & NA_PTR_HAS_ACCESSIBLE_BYTECOUNT)
      naError("naMarkPtrWithAccessibleBytesize", "accessible size already marked");
    if(accessiblebytesize < NA_ZERO)
      naError("naMarkPtrWithAccessibleBytesize", "accessiblesize should not be negative.");
    ptr->flags |= NA_PTR_HAS_ACCESSIBLE_BYTECOUNT;
    ptr->accessiblebytesize = accessiblebytesize;
    if(nullterminated){ptr->flags |= NA_PTR_NULL_TERMINATED;}
    
    // Now, we check if the last bytes are indeed zero
    nullindx = ptr->visiblebytesize;
    while(nullindx < ptr->accessiblebytesize){
      if(((const NAByte*)(ptr->data.constd))[nullindx] != '\0')
        naError("naMarkPtrWithAccessibleBytesize", "promised null-termination is not null");
      nullindx++;
    }
  }



  NA_HIDEF NABool naIsPtrNullTerminated(const NAPtr* ptr){
    if(!ptr)
      {naCrash("naIsPtrNullTerminated", "ptr is Null-Pointer"); return NA_FALSE;}
    if(!(ptr->flags & NA_PTR_HAS_ACCESSIBLE_BYTECOUNT))
      naError("naIsPtrNullTerminated", "No accessible size information present. Ptr may or may not be null-terminated. Return value is always false.");
    return (ptr->flags & NA_PTR_NULL_TERMINATED);
  }
  
#endif




NA_IDEF NAPtr naMakeNullPtr(){
  NAPtr ptr;
  ptr.data.d = NA_NULL;
  #ifndef NDEBUG
    ptr.flags = NA_ZERO; // Do not mark a null pointer as const. Otherwise many
                         // more errors will spawn.
    naMarkPtrCleanup(&ptr, NA_MEMORY_CLEANUP_NONE);
    naMarkPtrWithVisibleBytesize(&ptr, NA_ZERO);
    naMarkPtrWithAccessibleBytesize(&ptr, NA_ZERO, NA_FALSE);
  #endif
  return ptr;
}



// Note: bytesize 0 ist not allowed. Allowing it would introduce an if statement
// which should not exist at this low level function. A debug error will fire
// and you have to deal with zero sizes in higher level functions.
NA_IDEF NAPtr naMakePtrWithBytesize(NAInt bytesize){
  NAPtr ptr;
  #ifndef NDEBUG
    if(bytesize == NA_ZERO)
      naError("naMakePtrWithBytesize", "size is zero.");
  #endif
  ptr.data.d = naMalloc(bytesize);
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
    naMarkPtrCleanup(&ptr, NA_MEMORY_CLEANUP_FREE);
    if(bytesize < NA_ZERO){
      naMarkPtrWithVisibleBytesize(&ptr, -bytesize);
      naMarkPtrWithAccessibleBytesize(&ptr, naGetNullTerminationBytesize(bytesize), NA_TRUE);
    }else{
      naMarkPtrWithVisibleBytesize(&ptr, bytesize);
      naMarkPtrWithAccessibleBytesize(&ptr, bytesize, NA_FALSE);
    }
  #endif
  return ptr;
}



NA_IDEF void naClearPtr(NAPtr* ptr){
  #ifndef NDEBUG
    if(ptr->flags & NA_PTR_CLEANED)
      naError("naClearPtr", "NAPtr has already been cleaned once.");
    if((ptr->flags >> NA_MEMORY_CLEANUP_BITSHIFT) != NA_MEMORY_CLEANUP_NONE)
      naError("naClearPtr", "Ptr has not the NONE cleanup hint");
  #endif
  // As this is the NONE cleanup, we do nothing when NDEBUG is defined.
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #else
    NA_UNUSED(ptr);
  #endif
}



NA_IDEF void naFreePtr(NAPtr* ptr){
  #ifndef NDEBUG
    if(ptr->flags & NA_PTR_CLEANED)
      naError("naFreePtr", "NAPtr has already been cleaned once.");
    if(naIsPtrConst(ptr))
      naError("naFreePtr", "Trying to free a const pointer");
    if((ptr->flags >> NA_MEMORY_CLEANUP_BITSHIFT) != NA_MEMORY_CLEANUP_FREE)
      naError("naFreePtr", "Ptr has not the FREE cleanup hint");
  #endif
  naFree(ptr->data.d);
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #endif
}



NA_IDEF void naFreeAlignedPtr(NAPtr* ptr){
  #ifndef NDEBUG
    if(ptr->flags & NA_PTR_CLEANED)
      naError("naFreeAlignedPtr", "NAPtr has already been cleaned once.");
    if(naIsPtrConst(ptr))
      naError("naFreeAlignedPtr", "Trying to free-aligned a const pointer");
    if((ptr->flags >> NA_MEMORY_CLEANUP_BITSHIFT) != NA_MEMORY_CLEANUP_FREE_ALIGNED)
      naError("naFreeAlignedPtr", "Ptr has not the FREE_ALIGNED cleanup hint");
  #endif
  naFreeAligned(ptr->data.d);
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #endif
}



NA_IDEF void naDeletePtr(NAPtr* ptr){
  #ifndef NDEBUG
    if(ptr->flags & NA_PTR_CLEANED)
      naError("naDeletePtr", "NAPtr has already been cleaned once.");
    if(naIsPtrConst(ptr))
      naError("naDeletePtr", "Trying to delete a const pointer");
    if((ptr->flags >> NA_MEMORY_CLEANUP_BITSHIFT) != NA_MEMORY_CLEANUP_DELETE)
      naError("naDeletePtr", "Ptr has not the DELETE cleanup hint");
  #endif
  naDelete(ptr->data.d);
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #endif
}



NA_IDEF NAPtr naMakePtrWithConstBuffer(const void* data, NAInt bytesizehint, NAInt zerofillhint){
  NAPtr ptr;
  ptr.data.constd = data;
  #ifndef NDEBUG
    ptr.flags = NA_PTR_CONST_DATA;
    naMarkPtrCleanup(&ptr, NA_MEMORY_CLEANUP_NONE);
    if(bytesizehint < NA_ZERO){
      naMarkPtrWithVisibleBytesize(&ptr, -bytesizehint);
      naMarkPtrWithAccessibleBytesize(&ptr, -bytesizehint + zerofillhint, NA_TRUE);
    }else{
      if(zerofillhint < NA_ZERO)
        naError("naMakePtrWithConstBuffer", "zerofillhint should be greater-equal zero.");
      // Note that when bytesizehint is zero, zerofillhint is obsolete.
      if((bytesizehint == NA_ZERO) && (zerofillhint != NA_ZERO))
        naError("naMakePtrWithConstBuffer", "zerofillhint should be zero if bytesizehint is zero.");
      naMarkPtrWithVisibleBytesize(&ptr, bytesizehint);
      naMarkPtrWithAccessibleBytesize(&ptr, bytesizehint, NA_FALSE);
    }
  #else
    NA_UNUSED(bytesizehint);
    NA_UNUSED(zerofillhint);
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithMutableBuffer(void* data, NAInt bytesizehint, NAInt zerofillhint, NAMemoryCleanup cleanuphint){
  NAPtr ptr;
  ptr.data.d = data;
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
    naMarkPtrCleanup(&ptr, cleanuphint);
    // Note that the accessiblesize is the same as the visible size because
    // the true amount of null-terminating bytes is unknown.
    if(bytesizehint < NA_ZERO){
      naMarkPtrWithVisibleBytesize(&ptr, -bytesizehint);
      naMarkPtrWithAccessibleBytesize(&ptr, -bytesizehint + zerofillhint, NA_TRUE);
    }else{
      if(zerofillhint < NA_ZERO)
        naError("naMakePtrWithMutableBuffer", "zerofillhint should be greater-equal zero.");
      // Note that when bytesizehint is zero, zerofillhint is obsolete.
      if((bytesizehint == NA_ZERO) && (zerofillhint != NA_ZERO))
        naError("naMakePtrWithMutableBuffer", "zerofillhint should be zero if bytesizehint is zero.");
      naMarkPtrWithVisibleBytesize(&ptr, bytesizehint);
      naMarkPtrWithAccessibleBytesize(&ptr, bytesizehint, NA_FALSE);
    }
  #else
    NA_UNUSED(bytesizehint);
    NA_UNUSED(zerofillhint);
    NA_UNUSED(cleanuphint);
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithExtraction(const NAPtr* srcptr, NAUInt byteoffset, NAUInt bytesizehint){
  NAPtr dstptr;
  #ifndef NDEBUG
    if((NAInt)byteoffset < NA_ZERO)
      naError("naMakePtrWithExtraction", "offset seems to be negative but should be unsigned.");
    if((NAInt)bytesizehint < NA_ZERO)
      naError("naMakePtrWithExtraction", "bytesizehint seems to be negative but should be unsigned.");
  #endif
  dstptr.data.d = &(((NAByte*)(srcptr->data.d))[byteoffset]);
  #ifndef NDEBUG
    // Now, we set the sizes and decide if certain flags still are valid.
    dstptr.flags = srcptr->flags;
    dstptr.visiblebytesize = bytesizehint;
    dstptr.accessiblebytesize = srcptr->accessiblebytesize - byteoffset;
    if(!(srcptr->flags & NA_PTR_HAS_VISIBLE_BYTECOUNT)){
      // When no size information is available, the NAPtr struct has no
      // purpose.
      naError("naMakePtrWithExtraction", "No size information available. Cannot debug.");
    }else{
      if((byteoffset + bytesizehint) < srcptr->visiblebytesize){
        // The (offset,size) pair implies the dst array to be smaller than the
        // src array. Therefore, null-termination can no more be guaranteed.
        dstptr.flags &= ~NA_PTR_NULL_TERMINATED;
      }else if((srcptr->flags & NA_PTR_HAS_ACCESSIBLE_BYTECOUNT) && ((byteoffset + bytesizehint) > srcptr->accessiblebytesize)){
        // the (offset,size) pair overflows the accessible size. Very bad!
        naError("naMakePtrWithExtraction", "new offset and size overflows storage");
      }else if((byteoffset + bytesizehint) > srcptr->visiblebytesize){
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



NA_IDEF NABool naIsPtrConst(const NAPtr* ptr){
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

struct NALValue{
  NAPtr   ptr;          // pointer to the first byte
  NAInt   typesize;     // size of the unit in bytes. Always positive.
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



// Typesize must never be zero. When debugging, an error will be emitted and
// you have to deal with the problem in a higher level function.
NA_IDEF NALValue naMakeLValueWithTypesize(NAUInt typesize){
  NALValue lvalue;
  #ifndef NDEBUG
    if(typesize == NA_ZERO)
      naError("naMakeLValueWithTypesize", "typesize is zero.");
    if((NAInt)typesize < NA_ZERO)
      naError("naMakeLValueWithTypesize", "typesize appears to be negative when interpreted as a signed integer. This is not allowed.");
  #endif
  lvalue.ptr = naMakePtrWithBytesize((NAInt)typesize);
  lvalue.typesize = typesize;
  return lvalue;
}



NA_IDEF NALValue naMakeLValueWithConstBuffer(const void* buffer, NAInt typesize){
  NALValue lvalue;
  #ifndef NDEBUG
    if(typesize == NA_ZERO)
      naError("naMakeLValueWithTypesize", "typesize should be greater than zero.");
  #endif
  lvalue.ptr = naMakePtrWithConstBuffer(buffer, typesize, NA_ZERO);
  lvalue.typesize = typesize;
  return lvalue;
}



NA_IDEF NALValue naMakeLValueWithMutableBuffer(void* buffer, NAInt typesize, NAMemoryCleanup cleanuphint){
  NALValue lvalue;
  #ifndef NDEBUG
    if(typesize == NA_ZERO)
      naError("naMakeLValueWithMutableBuffer", "typesize should be greater than zero.");
  #endif
  lvalue.ptr = naMakePtrWithMutableBuffer(buffer, typesize, NA_ZERO, cleanuphint);
  lvalue.typesize = typesize;
  return lvalue;
}



NA_IDEF void naFreeLValue(NALValue* lvalue){
  naFreePtr(&(lvalue->ptr));
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



// bytesize must never be zero. When debugging, an error will be emitted and
// you have to deal with the problem in a higher level function.
NA_IDEF NAMemoryBlock naMakeMemoryBlockWithBytesize(NAInt bytesize){
  NAMemoryBlock memblock;
  #ifndef NDEBUG
    if(bytesize == NA_ZERO)
      naError("naMakeMemoryBlockWithBytesize", "bytesize is zero.");
  #endif
  memblock.ptr = naMakePtrWithBytesize(bytesize);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithConstBuffer(const void* buffer, NAInt bytesize){
  NAMemoryBlock memblock;
  memblock.ptr = naMakePtrWithConstBuffer(buffer, bytesize, 1);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithMutableBuffer(void* buffer, NAInt bytesize, NAMemoryCleanup cleanuphint){
  NAMemoryBlock memblock;
  memblock.ptr = naMakePtrWithMutableBuffer(buffer, bytesize, 1, cleanuphint);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithExtraction(const NAMemoryBlock* srcmemblock, NAUInt byteoffset, NAUInt bytesize){
  NAMemoryBlock dstmemblock;
  #ifndef NDEBUG
    if(naIsMemoryBlockEmpty(srcmemblock))
      naError("naMakeMemoryBlockWithExtraction", "src memory block is empty");
    if((NAInt)byteoffset < NA_ZERO)
      naError("naMakeMemoryBlockWithExtraction", "offset seems to be negative but should be unsigned.");
    if((NAInt)bytesize < NA_ZERO)
      naError("naMakeMemoryBlockWithExtraction", "bytesize seems to be negative but should be unsigned.");
    if((byteoffset + bytesize) > srcmemblock->bytesize)
      naError("naMakeMemoryBlockWithExtraction", "offset and bytesize range out of bounds");
  #endif
  dstmemblock.ptr = naMakePtrWithExtraction(&(srcmemblock->ptr), byteoffset, bytesize);
  dstmemblock.bytesize = bytesize;
  return dstmemblock;
}



NA_IDEF void naFreeMemoryBlock(NAMemoryBlock* memblock){
  naFreePtr(&(memblock->ptr));
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



NA_IDEF NAUInt naGetMemoryBlockMaxIndex(const NAMemoryBlock* memblock){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naGetMemoryBlockMaxIndex", "memblock is Null-Pointer.");
      return NA_TRUE;
    }
    if(naIsMemoryBlockEmpty(memblock))
      naError("naGetMemoryBlockConstPointer", "memblock is empty.");
  #endif
  return naMakeMaxWithEndi(memblock->bytesize);
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



NA_IDEF void naVoidMemoryBlock(NAMemoryBlock* memblock){
  #ifndef NDEBUG
    if(!memblock){
      naCrash("naIsMemoryBlockEmpty", "memblock is Null-Pointer.");
      return;
    }
  #endif
  memblock->bytesize = NA_ZERO;
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
    if((NAInt)indx < NA_ZERO)
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
    if((NAInt)indx < NA_ZERO)
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
    if(!memblock){
      naCrash("naIsMemoryBlockConst", "memblock is Null-Pointer.");
      return NA_TRUE;
    }
    if(naIsMemoryBlockEmpty(memblock))
      naError("naIsMemoryBlockConst", "memblock is empty.");
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
      if(naIsMemoryBlockEmpty(memblock))
        naError("naIsMemoryBlockNullTerminated", "memblock is empty.");
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



// Typesize must never be zero. When debugging, an error will be emitted and
// you have to deal with the problem in a higher level function.
NA_IDEF NACArray naMakeCArrayWithTypesizeAndCount(NAUInt typesize, NAInt count){
  NACArray carray;
  NAInt totalbytecount = (NAInt)typesize * count;;
  #ifndef NDEBUG
    if((NAInt)typesize < NA_ZERO)
      naError("naMakeCArrayWithTypesizeAndCount", "typesize appears to be negative when interpreted as a signed integer. This is not allowed.");
    if(count == NA_ZERO)
      naError("naMakeCArrayWithTypesizeAndCount", "count is zero.");
    if(naSigni(count) != naSigni(totalbytecount))
      naError("naMakeCArrayWithTypesizeAndCount", "count and typesize overflow the integer range.");
  #endif
  carray.ptr = naMakePtrWithBytesize(totalbytecount);
  carray.bytesize = naAbsi(totalbytecount);
  carray.typesize = typesize;
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithConstBuffer(const void* buffer, NAInt typesize, NAInt count){
  NACArray carray;
  #ifndef NDEBUG
    if(typesize <= NA_ZERO)
      naError("naMakeCArrayWithConstBuffer", "typesize should be greater than zero.");
    if(count <= NA_ZERO)
      naError("naMakeCArrayWithConstBuffer", "count should be greater than zero.");
  #endif
  carray.ptr = naMakePtrWithConstBuffer(buffer, typesize * count, typesize);
  carray.bytesize = naAbsi(typesize * count);
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithMutableBuffer(void* buffer, NAUInt typesize, NAInt count, NAMemoryCleanup cleanuphint){
  NACArray carray;
  #ifndef NDEBUG
    if(typesize <= NA_ZERO)
      naError("naMakeCArrayWithMutableBuffer", "typesize should be greater than zero.");
    if(count <= NA_ZERO)
      naError("naMakeCArrayWithMutableBuffer", "count should be greater than zero.");
  #endif
  carray.ptr = naMakePtrWithMutableBuffer(buffer, typesize * count, typesize, cleanuphint);
  carray.bytesize = naAbsi(typesize * count);
  return carray;
}



NA_IDEF NACArray naMakeCArrayWithExtraction(const NACArray* srccarray, NAUInt elemoffset, NAUInt elemcount){
  NACArray dstcarray;
  #ifndef NDEBUG
    if(naIsCArrayEmpty(srccarray))
      naError("naMakeCArrayWithExtraction", "src memory block is empty");
    if((NAInt)elemoffset < NA_ZERO)
      naError("naMakeCArrayWithExtraction", "offset seems to be negative but should be unsigned.");
    if((NAInt)elemcount < NA_ZERO)
      naError("naMakeCArrayWithExtraction", "bytesize seems to be negative but should be unsigned.");
    if(((elemoffset + elemcount) * srccarray->typesize) > srccarray->bytesize)
      naError("naMakeCArrayWithExtraction", "offset and bytesize range out of bounds");
  #endif
  dstcarray.ptr = naMakePtrWithExtraction(&(srccarray->ptr), elemoffset * srccarray->typesize, elemcount * srccarray->typesize);
  dstcarray.bytesize = elemcount * srccarray->typesize;
  return dstcarray;
}



NA_IDEF void naFreeCArray(NACArray* carray){
  naFreePtr(&(carray->ptr));
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
    if((NAInt)indx < NA_ZERO)
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
    if((NAInt)indx < NA_ZERO)
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












// //////////////////////////
// NABuf
// //////////////////////////


// Note that in contrast to other basic memory structs, an NABuf is always
// 64 bit. This is because it is used to store file contents which may be
// greater than 32 bits.
struct NABuf{
  NAPtr   ptr;          // pointer to the first byte
  uint64  bytesize;     // total size of the buf in bytes. Always positive.
  uint64  usedbytesize;     // size of the used bytes. Always positive.
};



NA_IDEF NABuf naMakeBuf(){
  NABuf buf;
  // Note that we do not initialize the ptr with naMakePtr. Relying solely on
  // the size field is non-redundant, saves some time and sometimes even helps
  // detecting whether you initialized your structs correctly as certain
  // compilers will add guards to non-initialized pointers and will fire
  // with an exception.
  buf.bytesize = NA_ZERO_64;
  buf.usedbytesize = NA_ZERO_64;
  return buf;
}



// maxsize must never be zero. When debugging, an error will be emitted and
// you have to deal with the problem in a higher level function.
NA_IDEF NABuf naMakeBufWithBytesize(int64 maxbytesize, int64 usedbytesize){
  NABuf buf;
  #ifndef NDEBUG
    if(maxbytesize == NA_ZERO_64)
      naError("naMakeBufWithBytesize", "maxsize is zero.");
    if(maxbytesize < NA_ZERO_64)
      naError("naMakeBufWithBytesize", "maxsize must be positive. Zero fill check might fail with NABuf");
    if(usedbytesize < NA_ZERO_64)
      naError("naMakeBufWithBytesize", "usedsize must be positive.");
    if(usedbytesize > maxbytesize)
      naError("naMakeBufWithBytesize", "maxsize can not be smaller than usedsize.");
  	#if (NA_SYSTEM_INT_BITS < 64)
      if(maxbytesize > NA_INT_MAX)
	      naError("naMakeBufWithBytesize", "maxsize overflows int range, while int has less than 64 bits in this configuration.");
    #endif
#endif
  buf.ptr = naMakePtrWithBytesize((NAInt)maxbytesize);
  buf.bytesize = maxbytesize;
  buf.usedbytesize = usedbytesize;
  return buf;
}



NA_IDEF void naFreeBuf(NABuf* buf){
  naFreePtr(&(buf->ptr));
}



NA_IDEF uint64 naGetBufMaxBytesize(const NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufMaxBytesize", "buf is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return buf->bytesize;
}



NA_IDEF uint64 naGetBufUsedBytesize(const NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufUsedBytesize", "buf is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return buf->usedbytesize;
}



NA_IDEF uint64 naGetBufRemainingBytesize(const NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufRemainingBytesize", "buf is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return buf->bytesize - buf->usedbytesize;
}



NA_IDEF NABool naIsBufEmpty(const NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naIsBufEmpty", "buf is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return (buf->usedbytesize == NA_ZERO_64);
}





NA_IDEF const void* naGetBufConstUsedPointer(const NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufConstUsedPointer", "buf is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return &(((const NAByte*)(naGetPtrConst(&(buf->ptr))))[buf->usedbytesize]);
}



NA_IDEF void* naGetBufMutableUsedPointer(NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufMutableUsedPointer", "buf is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return &(((NAByte*)(naGetPtrMutable(&(buf->ptr))))[buf->usedbytesize]);
}



NA_IDEF void naAdvanceBuf(NABuf* buf, uint64 bytesize){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naAdvanceBuf", "buf is Null-Pointer.");
      return;
    }
    if((buf->usedbytesize + bytesize) > buf->bytesize)
      naError("naAdvanceBuf", "buf overflows.");
  #endif
  buf->usedbytesize += bytesize;
}



NA_IDEF const void* naGetBufConstFirstPointer(const NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufConstUsedPointer", "buf is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsBufEmpty(buf))
      naError("naGetBufConstUsedPointer", "buf is empty.");
  #endif
  return naGetPtrConst(&(buf->ptr));
}



NA_IDEF void* naGetBufMutableFirstPointer(NABuf* buf){
  #ifndef NDEBUG
    if(!buf){
      naCrash("naGetBufMutableUsedPointer", "buf is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsBufEmpty(buf))
      naError("naGetBufMutableUsedPointer", "buf is empty.");
  #endif
  return naGetPtrMutable(&(buf->ptr));
}



NA_IDEF NABool naIsBufConst(NABuf* buf){
  #ifndef NDEBUG
    return naIsPtrConst(&(buf->ptr));
  #else
    NA_UNUSED(buf);
    return NA_FALSE;
  #endif
}





// //////////////////////////
// NAPointer
// //////////////////////////

struct NAPointer{
  NAPtr ptr;
  NAMutator deallocator;
  NAUInt refcount;      // Reference count. Including flags
};

// Prototype of the destruct function defined in the implementation file.
NA_HAPI void naDestructPointer(NAPointer* pointer);
NA_RUNTIME_TYPE(NAPointer, naDestructPointer);

// Starting with NALib version 14, the actual data is stored as a pointer.
// This means that it must be allocated elsewhere. But this means, any pointer
// can become a reference counted pointer.
//
// To distinguish, how the pointer shall be handeled upon deletion, a new
// enumeration NAMemoryCleanup had been introduced. See above.
//
// The refcoutn field also stores the cleanup hint in the 3 bits on the higher
// endian side. This is necessary due to the automatic deletion of the pointer
// when refcount reaches zero. It could be stored in a separate flag but the
// author decided to do it this way.
//
// Note that when NDEBUG is undefined, this cleanup hint is stored redundantly
// both here in NAPointer as well as a debugging hint in NAPtr.

#ifndef NDEBUG
  #define NA_POINTER_DELETE_FROM_DECREF (NA_ONE << (NA_MEMORY_CLEANUP_BITSHIFT - 1))
  #define NA_POINTER_REFCOUNT_MASK      (NA_POINTER_DELETE_FROM_DECREF - NA_ONE)
#else
  #define NA_POINTER_REFCOUNT_MASK      ((NA_ONE << NA_MEMORY_CLEANUP_BITSHIFT) - NA_ONE)
#endif



NA_IDEF NAPointer* naNewNullPointer(){
  return naNewPointer(NA_NULL, NA_MEMORY_CLEANUP_NONE, NA_NULL);
}



NA_IDEF NAPointer* naNewPointer(void* data, NAMemoryCleanup cleanup, NAMutator deallocator){
  NAPointer* pointer; // Declaration before definition. Needed for C90
  #ifndef NDEBUG
    if(cleanup < NA_MEMORY_CLEANUP_NONE || cleanup > NA_MEMORY_CLEANUP_DELETE)
      naError("naNewPointer", "cleanup method invalid");
  #endif
  pointer = naNew(NAPointer);
  pointer->ptr = naMakePtrWithMutableBuffer(data, 0, 0, cleanup);
  pointer->deallocator = deallocator;
  pointer->refcount = 1 | ((NAUInt)cleanup << NA_MEMORY_CLEANUP_BITSHIFT);
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
    #ifndef NDEBUG
      pointer->refcount |= NA_POINTER_DELETE_FROM_DECREF;
    #endif
    naDelete(pointer);
    // Note that this is the only position in the whole NALib code where an
    // NAPointer is deleted. Therefore you could theoretically include the
    // code written in naDestructPointer directly here to save one function
    // call. But as NALib assumes that NAPointer is only used when really
    // needed for reference counting, it is nontheless put into a separate
    // function such that this function naReleasePointer can be inlined for
    // sure and thus becomes very fast.
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist and must be implemented manually. NARuntime is a small
  // example of such a system.
}




NA_IDEF const void* naGetPointerConst(const NAPointer* pointer){
  return naGetPtrConst(&(pointer->ptr));
}



NA_IDEF void* naGetPointerMutable(NAPointer* pointer){
  #ifndef NDEBUG
    if(naIsPtrConst(&(pointer->ptr)))
      naError("naGetPointerMutable", "Mutating a const pointer.");
  #endif
  return naGetPtrMutable(&(pointer->ptr));
}



NA_IDEF NABool naIsPointerConst(const NAPointer* pointer){
  return naIsPtrConst(&(pointer->ptr));
}





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
