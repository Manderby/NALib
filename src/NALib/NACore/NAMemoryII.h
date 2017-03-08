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

#include "../NASystem.h"
#include "../NAMathOperators.h"
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
// The following function returns for a given negative bytesize, how big a
// buffer must be, to include the appended zero bytes.
//
// The allocation functions of NALib expect a positive or negative bytesize.
// If the bytesize is negative, the absolute value is used to allocate
// sufficient space but a certain number of bytes is appended to the memory
// block which will be initialized with binary zero but will not be visible to
// the programmer.
// 
// The following holds true:
//
// - The additional bytes are all guaranteed to be initialized with binary
//   zero.
// - There are AT LEAST as many bytes appended as one address requires.
//   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
// - There are as many bytes allocated such that the total bytesize of the
//   allocated block is a multiple of an address bytesize, meaning 4 or 8 Bytes
//   depending on the system (32 Bit or 64 Bit).
// - The total bytesize (desired space plus null bytes) is at minimum 2 times
//   the number of bytes needed for an address.
// - When using naMalloc, the part without the additional bytes might partially
//   become initialized with binary zero.
// Example for a 32bit system:
// 0 is a zero-filled byte.
// x is a desired byte,
// z is a desired byte which initially gets overwritten with zero.
// 0:  0000 0000
// 1:  z000 0000
// 2:  zz00 0000
// 3:  zzz0 0000
// 4:  zzzz 0000
// 5:  xxxx z000 0000
// 8:  xxxx zzzz 0000
// 9:  xxxx xxxx z000 0000
// 12: xxxx xxxx zzzz 0000
// 13: xxxx xxxx xxxx z000 0000

NA_HIDEF NAInt naGetNullTerminationBytesize(NAInt bytesize){
  NAInt returnbytesize;
  #ifndef NDEBUG
    if((bytesize >= NA_ZERO))
      naError("naGetNullTerminationBytesize", "size is not negative");
    if(bytesize == NA_INVALID_MEMORY_BYTESIZE)
      naError("naGetNullTerminationBytesize", "invalid size given");
  #endif
  returnbytesize = (-bytesize - NA_ONE) + (NA_SYSTEM_ADDRESS_BYTES << 1) - ((-bytesize - NA_ONE) % NA_SYSTEM_ADDRESS_BYTES);
  #ifndef NDEBUG
    if(returnbytesize < NA_ZERO)
      naError("naGetNullTerminationBytesize", "given negative size is too close to the minimal integer value");
  #endif
  return returnbytesize;
}








// ////////////////////////////////////////////
// Basic Memory allocation and freeing
// ////////////////////////////////////////////



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







// //////////////////////////
// NARuntime
// //////////////////////////


NA_API void*              naNewStruct(NATypeInfo* typeidentifier);


// This needs to be in this inline implementation file as it needs to be
// accessible to the compiler at all times.
struct NATypeInfo{
  void*             curpool;  // The actual type of this entry is hidden.
  NAUInt            typesize;
  NAMutator         destructor;
};










// //////////////////////////
// NAMemoryCleanup
// //////////////////////////


// For the NAPointer struct, we need to have the information available about
// how the memory was originally alloced. We store it as the high-order part
// of the refcount which is why we need to bit-shift it to the left.
// To simplify matters, also the NAPtr struct stores this information as left-
// shifted bits in the flags when NDEBUG ist undefined.
#define NA_MEMORY_CLEANUP_BITS            3
#define NA_MEMORY_CLEANUP_MASK            ((1 << NA_MEMORY_CLEANUP_BITS) - 1)
#define NA_REFCOUNT_DATA_CLEANUP_BITSHIFT   (NA_SYSTEM_ADDRESS_BITS - NA_MEMORY_CLEANUP_BITS)
#define NA_REFCOUNT_STRUCT_CLEANUP_BITSHIFT (NA_SYSTEM_ADDRESS_BITS - 2 * NA_MEMORY_CLEANUP_BITS)



NA_HIDEF NABool naIsCleanupValid(NAMemoryCleanup cleanup){
  return ((cleanup > NA_MEMORY_CLEANUP_UNDEFINED) && (cleanup < NA_MEMORY_CLEANUP_COUNT));
}
NA_HIDEF NAMemoryCleanup naGetFlagCleanupData(NAUInt flag){
  return (NAMemoryCleanup)((flag >> NA_REFCOUNT_DATA_CLEANUP_BITSHIFT) & NA_MEMORY_CLEANUP_MASK);
}
NA_HIDEF NABool naHasFlagCleanupData(NAUInt flag){
  return (naGetFlagCleanupData(flag) != NA_MEMORY_CLEANUP_UNDEFINED);
}
NA_HIDEF NAMemoryCleanup naGetFlagCleanupStruct(NAUInt flag){
  return (NAMemoryCleanup)((flag >> NA_REFCOUNT_STRUCT_CLEANUP_BITSHIFT) & NA_MEMORY_CLEANUP_MASK);
}






// //////////////////////////
// NARefCount
// //////////////////////////

struct NARefCount{
  NAUInt count;      // Reference count. Including flags
  #ifndef NDEBUG
  NAMemoryCleanup structcleanup;
  NAMemoryCleanup datacleanup;
  #endif
};

// The refcount field also stores two cleanup hints in the 2*3 bits on the
// higher endian side. This is necessary due to the automatic deletion of a
// pointer and the struct the refcount is stored in when refcount reaches zero.
// It could be stored in a separate flag but the author decided to do it with
// a mask.
//
// Note that when NDEBUG is undefined, there is an additional flag just to
// check whether a reference counted value had been released properly.


#ifndef NDEBUG
  #define NA_REFCOUNT_DELETE_FROM_RELEASE (NA_ONE << (NA_REFCOUNT_STRUCT_CLEANUP_BITSHIFT - 1))
  #define NA_REFCOUNT_FLAG_BITS   (2 * NA_MEMORY_CLEANUP_BITS + 1)
#else
  #define NA_REFCOUNT_FLAG_BITS   (2 * NA_MEMORY_CLEANUP_BITS)
#endif

#define NA_REFCOUNT_FLAGS_BITSHIFT      (NA_SYSTEM_ADDRESS_BITS - NA_REFCOUNT_FLAG_BITS)
#define NA_REFCOUNT_MASK      ((NA_ONE << NA_REFCOUNT_FLAGS_BITSHIFT) - NA_ONE)


NA_IDEF NARefCount* naInitRefCount(NARefCount* refcount, NAMemoryCleanup structcleanup, NAMemoryCleanup datacleanup){
  #ifndef NDEBUG
    if(!naIsCleanupValid(structcleanup))
      naError("naInitRefCount", "structcleanup method invalid");
    if(!naIsCleanupValid(datacleanup))
      naError("naInitRefCount", "datacleanup method invalid");
    #ifdef __cplusplus 
      if(structcleanup == NA_MEMORY_CLEANUP_DELETE_BRACK)  
        naError("naInitRefCount", "This smartptrcleanup method does not make sense");
    #endif
  #endif
  refcount->count  = 1
               | ((NAUInt)datacleanup << NA_REFCOUNT_DATA_CLEANUP_BITSHIFT)
               | ((NAUInt)structcleanup << NA_REFCOUNT_STRUCT_CLEANUP_BITSHIFT);
  #ifndef NDEBUG
    refcount->structcleanup = structcleanup;
    refcount->datacleanup = datacleanup;
  #endif
  return refcount;
}



NA_HIDEF NAUInt naGetRefCountCount(const NARefCount* refcount){
  return refcount->count & NA_REFCOUNT_MASK;
}



NA_IDEF NAMemoryCleanup naGetRefCountCleanupData(const NARefCount* refcount){
  #ifndef NDEBUG
    if(!refcount)
      {naCrash("naGetRefCountCleanupData", "refcount is Null-Pointer."); return NA_MEMORY_CLEANUP_NONE;}
    if(naGetFlagCleanupData(refcount->count) != refcount->datacleanup)
      naError("naGetRefCountCleanupData", "Consistency problem: flags overwritten");
  #endif
  return naGetFlagCleanupData(refcount->count);
}



NA_IDEF NAMemoryCleanup naGetRefCountCleanupStruct(const NARefCount* refcount){
  #ifndef NDEBUG
    if(!refcount)
      {naCrash("naGetRefCountCleanupStruct", "refcount is Null-Pointer."); return NA_MEMORY_CLEANUP_NONE;}
    if(naGetFlagCleanupStruct(refcount->count) != refcount->structcleanup)
      naError("naGetRefCountCleanupStruct", "Consistency problem: flags overwritten");
  #endif
  return naGetFlagCleanupStruct(refcount->count);
}



NA_IDEF NARefCount* naRetainRefCount(NARefCount* refcount){
  #ifndef NDEBUG
    if(!refcount){
      naCrash("naRetainRefCount", "refcount is Null-Pointer.");
      return NA_NULL;
    }else{
      // The next test can detect some erroneous behaviour in the code. Note
      // however that most likely the true cause of the error did occur long
      // before reaching here.
      if(naGetRefCountCount(refcount) == NA_ZERO)
        {naCrash("naRetainRefCount", "Retaining NARefCount with a count of 0"); return NA_NULL;}
    }
  #endif
  refcount->count++;
  #ifndef NDEBUG
    // If refcount now suddenly becomes zero, there was either an error earlier
    // or the object has been retained too many times. Overflow.
    if(naGetRefCountCount(refcount) == NA_ZERO)
      naError("naRetainRefCount", "Reference count overflow");
  #endif
  return refcount;
}



NA_IDEF NABool naReleaseRefCount(NARefCount* refcount, void* data, NAMutator destructor){
  #ifndef NDEBUG
    if(!refcount){
      naCrash("naReleaseRefCount", "refcount is Null-Pointer.");
      return NA_FALSE;
    }
    // The next test can detect some erroneous behaviour in the code. Note
    // however that most likely the true cause of the error did occur long
    // before reaching here.
    if(naGetRefCountCount(refcount) == NA_ZERO)
      {naCrash("naReleaseRefCount", "Releasing NARefCount with a count of 0"); return NA_FALSE;}
  #endif
  // Note that the author decided to always count to zero, even if it is clear
  // that the pointer will eventually be freed and the data will be lost in
  // nirvana. But often times in debugging, when retaining and releasing is not
  // done correctly, an NAPointer is released too often. When refcount is 0 and
  // NDEBUG is not defined, this can be detected!
  refcount->count--;
  if(naGetRefCountCount(refcount) == NA_ZERO){
    #ifndef NDEBUG
      refcount->count |= NA_REFCOUNT_DELETE_FROM_RELEASE;
    #endif
    
    // Call the destructor on the data if available.
    if(destructor){
      if(data){
        destructor(data);
      }else{
        destructor(refcount);
      }
    }
        
    // Clear the struct as marked.
    switch(naGetRefCountCleanupStruct(refcount)){
      case NA_MEMORY_CLEANUP_NONE:
        break;
      case NA_MEMORY_CLEANUP_FREE:
        naFree(refcount);
        break;
      case NA_MEMORY_CLEANUP_FREE_ALIGNED:
        naFreeAligned(refcount);
        break;
#ifdef __cplusplus 
      case NA_MEMORY_CLEANUP_DELETE:
        delete refcount;
        break;
      case NA_MEMORY_CLEANUP_DELETE_BRACK:
        #ifndef NDEBUG
          naError("naReleaseRefCount", "This cleanup option does not make sense");
        #endif
        delete [] refcount;
        break;
#endif
      case NA_MEMORY_CLEANUP_NA_DELETE:
        naDelete(refcount);
        break;
      default:
        #ifndef NDEBUG
          naError("naReleaseRefCount", "Invalid cleanup");
        #endif
        break;
    }
    
    // Note that this is the only position in the whole NALib code where an
    // NAPointer is deleted. Therefore you could theoretically include the
    // code written in naDestructPointer directly here to save one function
    // call. But as NALib assumes that NAPointer is only used when really
    // needed for reference counting, it is nontheless put into a separate
    // function such that this function can be inlined for sure and thus
    // becomes very fast.
    return NA_TRUE;
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist and must be implemented manually. NARuntime is a small
  // example of such a system.
  return NA_FALSE;
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
    NAUInt flags;              // This field stores some flags.
    NAUInt visiblebytesize;    // nof bytes of the visible byte storage
    NAUInt accessiblebytesize; // nof bytes of the accessible byte storage
  #endif
};
// Note that this is one of the very, very rare situations, where a union type
// actually makes sense.
//
// Also note that NAPtr will be fully optimized to a simple C-pointer
// when NDEBUG is defined.
//
// Authors note:
// The decision to include so many debugging information arose over many
// iterations. Mainly NAString was the cause of pushing debugging information
// deeply into the core functionality of NALib. They should be here to help
// with very fundamental datastructures like an NAByteArray but not to distract
// on higher levels. Further more, questions like "Is String Null-Terminated"
// should not be decided during (release) runtime. Therefore, the NAPtr
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


  // Marks an NAPtr to know what the cleanup method shall be. This will later
  // on be used when cleaning the NAPtr. It the used method will not correspond
  // to the method marked, a debug warning will be emitted.
  NA_HIDEF void naMarkPtrCleanup(NAPtr* ptr, NAMemoryCleanup cleanup){
    if(!naIsCleanupValid(cleanup))
      naError("naMarkPtrCleanup", "Invalid cleanup option");
    if(naHasFlagCleanupData(ptr->flags))
      naError("naMarkPtrCleanup", "ptr already marked with a cleanup");
    ptr->flags |= (NAUInt)(cleanup) << NA_REFCOUNT_DATA_CLEANUP_BITSHIFT;
  }
  
  

  // Marks an NAPtr to know what the bytesize visible to the programmer is.
  // The visible size may be different than the accessible size which is what
  // the implementation can access without overflowing the allocated memory.
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
    if((NAUInt)accessiblebytesize < ptr->visiblebytesize)
      naError("naMarkPtrWithAccessibleBytesize", "accessiblesize should not be smaller than the visible size.");
    if(nullterminated && (accessiblebytesize == ptr->visiblebytesize))
      naError("naMarkPtrWithAccessibleBytesize", "Null termination is bogus when visible and accessible size are equal.");
    ptr->flags |= NA_PTR_HAS_ACCESSIBLE_BYTECOUNT;
    if(nullterminated){ptr->flags |= NA_PTR_NULL_TERMINATED;}
    ptr->accessiblebytesize = accessiblebytesize;
    
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
      naError("naMakePtrWithBytesize", "bytesize is zero.");
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
    if(naGetFlagCleanupData(ptr->flags) != NA_MEMORY_CLEANUP_NONE)
      naError("naClearPtr", "Ptr has NOT the NONE cleanup hint. Use the appropriate clear function naFreePtr, naFreeAlignedPtr or naDeletePtr.");
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
    if(naGetFlagCleanupData(ptr->flags) != NA_MEMORY_CLEANUP_FREE)
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
    if(naGetFlagCleanupData(ptr->flags) != NA_MEMORY_CLEANUP_FREE_ALIGNED)
      naError("naFreeAlignedPtr", "Ptr has not the FREE_ALIGNED cleanup hint");
  #endif
  naFreeAligned(ptr->data.d);
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #endif
}



#ifdef __cplusplus 

  NA_IDEF void naDeletePtr(NAPtr* ptr){
    #ifndef NDEBUG
      if(ptr->flags & NA_PTR_CLEANED)
        naError("naDeletePtr", "NAPtr has already been cleaned once.");
      if(naIsPtrConst(ptr))
        naError("naDeletePtr", "Trying to delete a const pointer");
      if(naGetFlagCleanupData(ptr->flags) != NA_MEMORY_CLEANUP_DELETE)
        naError("naDeletePtr", "Ptr has not the DELETE cleanup hint");
    #endif
    delete ptr->data.d;
    #ifndef NDEBUG
      ptr->flags |= NA_PTR_CLEANED;
    #endif
  }



  NA_IDEF void naDeleteBrackPtr(NAPtr* ptr){
    #ifndef NDEBUG
      if(ptr->flags & NA_PTR_CLEANED)
        naError("naDeleteBrackPtr", "NAPtr has already been cleaned once.");
      if(naIsPtrConst(ptr))
        naError("naDeleteBrackPtr", "Trying to delete a const pointer");
      if(naGetFlagCleanupData(ptr->flags) != NA_MEMORY_CLEANUP_DELETE_BRACK)
        naError("naDeleteBrackPtr", "Ptr has not the DELETE_BRACK cleanup hint");
    #endif
    delete [] ptr->data.d;
    #ifndef NDEBUG
      ptr->flags |= NA_PTR_CLEANED;
    #endif
  }

#endif


NA_IDEF void naNaDeletePtr(NAPtr* ptr){
  #ifndef NDEBUG
    if(ptr->flags & NA_PTR_CLEANED)
      naError("naNaDeletePtr", "NAPtr has already been cleaned once.");
    if(naIsPtrConst(ptr))
      naError("naNaDeletePtr", "Trying to delete a const pointer");
    if(naGetFlagCleanupData(ptr->flags) != NA_MEMORY_CLEANUP_NA_DELETE)
      naError("naNaDeletePtr", "Ptr has not the NA_DELETE cleanup hint");
  #endif
  naDelete(ptr->data.d);
  #ifndef NDEBUG
    ptr->flags |= NA_PTR_CLEANED;
  #endif
}



NA_IDEF NAPtr naMakePtrWithConstData(const void* data, NAInt bytesizehint, NAInt zerofillhint){
  NAPtr ptr;
  ptr.data.constd = data;
  #ifndef NDEBUG
    ptr.flags = NA_PTR_CONST_DATA;
    naMarkPtrCleanup(&ptr, NA_MEMORY_CLEANUP_NONE);
    if(bytesizehint < NA_ZERO)
      naError("naMakePtrWithConstData", "bytesizehint should be greater-equal zero.");
    if(zerofillhint < NA_ZERO)
      naError("naMakePtrWithConstData", "zerofillhint should be greater-equal zero.");
    // Note that when bytesizehint is zero, zerofillhint is obsolete.
    if((bytesizehint == NA_ZERO) && (zerofillhint != NA_ZERO))
      naError("naMakePtrWithConstData", "zerofillhint should be zero if bytesizehint is zero.");
    naMarkPtrWithVisibleBytesize(&ptr, bytesizehint);
    naMarkPtrWithAccessibleBytesize(&ptr, bytesizehint + zerofillhint, NA_FALSE);
  #else
    NA_UNUSED(bytesizehint);
    NA_UNUSED(zerofillhint);
  #endif
  return ptr;
}



NA_IDEF NAPtr naMakePtrWithMutableData(void* data, NAInt bytesizehint, NAInt zerofillhint, NAMemoryCleanup cleanuphint){
  NAPtr ptr;
  ptr.data.d = data;
  #ifndef NDEBUG
    ptr.flags = NA_ZERO;
    naMarkPtrCleanup(&ptr, cleanuphint);
    if(zerofillhint < NA_ZERO)
      naError("naMakePtrWithMutableData", "zerofillhint should be greater-equal zero.");
    if(bytesizehint < NA_ZERO)
      naError("naMakePtrWithMutableData", "bytesizehint should be greater-equal zero.");
    // Note that when bytesizehint is zero, zerofillhint is obsolete.
    if((bytesizehint == NA_ZERO) && (zerofillhint != NA_ZERO))
      naError("naMakePtrWithMutableData", "zerofillhint should be zero if bytesizehint is zero.");
    naMarkPtrWithVisibleBytesize(&ptr, bytesizehint);
    naMarkPtrWithAccessibleBytesize(&ptr, bytesizehint + zerofillhint, NA_FALSE);
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
    dstptr.flags = srcptr->flags & NA_PTR_CONST_DATA;
    naMarkPtrCleanup(&dstptr, NA_MEMORY_CLEANUP_NONE);
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



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithConstData(const void* data, NAInt bytesize){
  NAMemoryBlock memblock;
  memblock.ptr = naMakePtrWithConstData(data, bytesize, 0);
  memblock.bytesize = naAbsi(bytesize);
  return memblock;
}



NA_IDEF NAMemoryBlock naMakeMemoryBlockWithMutableData(void* data, NAInt bytesize, NAMemoryCleanup cleanuphint){
  NAMemoryBlock memblock;
  memblock.ptr = naMakePtrWithMutableData(data, bytesize, 0, cleanuphint);
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
// NASmartPtr
// //////////////////////////

struct NASmartPtr{
  NARefCount refcount;      // Reference count. Including flags
  NAPtr ptr;
};





NA_IDEF NASmartPtr* naInitSmartPtrConst(NASmartPtr* sptr, NAMemoryCleanup smartptrcleanup, const void* data){
  #ifndef NDEBUG
    if(!naIsCleanupValid(smartptrcleanup))
      naError("naInitSmartPtr", "smartptrcleanup method invalid");
    #ifdef __cplusplus 
      if(smartptrcleanup == NA_MEMORY_CLEANUP_DELETE_BRACK)  
        naError("naInitSmartPtr", "This smartptrcleanup method does not make sense");
    #endif
  #endif
  sptr->ptr = naMakePtrWithConstData(data, NA_ZERO, NA_ZERO);
  naInitRefCount(&(sptr->refcount), smartptrcleanup, NA_MEMORY_CLEANUP_NONE);
  return sptr;
}



NA_IDEF NASmartPtr* naInitSmartPtrMutable(NASmartPtr* sptr, NAMemoryCleanup smartptrcleanup, void* data, NAMemoryCleanup datacleanup){
  #ifndef NDEBUG
    if(!naIsCleanupValid(smartptrcleanup))
      naError("naInitSmartPtr", "smartptrcleanup method invalid");
    #ifdef __cplusplus 
      if(smartptrcleanup == NA_MEMORY_CLEANUP_DELETE_BRACK)  
        naError("naInitSmartPtr", "This smartptrcleanup method does not make sense");
    #endif
    if(!naIsCleanupValid(datacleanup))
      naError("naInitSmartPtr", "datacleanup method invalid");
  #endif
  sptr->ptr = naMakePtrWithMutableData(data, NA_ZERO, NA_ZERO, datacleanup);
  naInitRefCount(&(sptr->refcount), smartptrcleanup, datacleanup);
  return sptr;
}




NA_IDEF NASmartPtr* naRetainSmartPtr(NASmartPtr* sptr){
  #ifndef NDEBUG
    if(!sptr){
      naCrash("naRetainSmartPtr", "sptr is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return (NASmartPtr*)naRetainRefCount(&(sptr->refcount));
}



typedef struct NASmartPtrDestructContainer NASmartPtrDestructContainer;
struct NASmartPtrDestructContainer{
  NASmartPtr* sptr;
  NAMutator destructor;
};



// When releasing a smart pointer with a custom destructor, you need to call
// the following function at the end of your destructor in order to properly
// release the data pointer.
NA_IDEF void naDestructSmartPtrData(NASmartPtrDestructContainer* container){

  if(container->destructor){
    container->destructor(naGetPtrMutable(&(container->sptr->ptr)));
  }

  // Clear the data based on the data cleanup
  switch(naGetRefCountCleanupData(&(container->sptr->refcount))){
    case NA_MEMORY_CLEANUP_NONE:
      break;
    case NA_MEMORY_CLEANUP_FREE:
      naFreePtr(&(container->sptr->ptr));
      break;
    case NA_MEMORY_CLEANUP_FREE_ALIGNED:
      naFreeAlignedPtr(&(container->sptr->ptr));
      break;
#ifdef __cplusplus 
    case NA_MEMORY_CLEANUP_DELETE:
      naDeletePtr(&(container->sptr->ptr));
      break;
    case NA_MEMORY_CLEANUP_DELETE_BRACK:
      naDeleteBrackPtr(&(container->sptr->ptr));
      break;
#endif
    case NA_MEMORY_CLEANUP_NA_DELETE:
      naNaDeletePtr(&(container->sptr->ptr));
      break;
  }
}



NA_IDEF NABool naReleaseSmartPtr(NASmartPtr* sptr, NAMutator destructor){
  #ifndef NDEBUG
    if(!sptr){
      naCrash("naReleaseSmartPtr", "sptr is Null-Pointer.");
      return NA_FALSE;
    }
  #endif

  NASmartPtrDestructContainer container = {sptr, destructor};
  
  return naReleaseRefCount(&(sptr->refcount), &container, (NAMutator)naDestructSmartPtrData);
}



NA_IDEF const void* naGetSmartPtrConst(const NASmartPtr* sptr){
  return naGetPtrConst(&(sptr->ptr));
}



NA_IDEF void* naGetSmartPtrMutable(NASmartPtr* sptr){
  return naGetPtrMutable(&(sptr->ptr));
}








// //////////////////////////
// NAPointer
// //////////////////////////

struct NAPointer{
  NASmartPtr sptr;
  NAMutator destructor;
};

// Prototype of the destruct function defined in the implementation file.
NA_RUNTIME_TYPE(NAPointer, NA_NULL);




NA_IDEF NAPointer* naNewPointerConst(const void* data){
  NAPointer* pointer; // Declaration before definition. Needed for C90
  pointer = naNew(NAPointer);
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refcount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrConst(&(pointer->sptr), NA_MEMORY_CLEANUP_NA_DELETE, data);
  pointer->destructor = NA_NULL;
  return pointer;
}



NA_IDEF NAPointer* naNewPointerMutable(void* data, NAMemoryCleanup datacleanup, NAMutator destructor){
  NAPointer* pointer; // Declaration before definition. Needed for C90
  #ifndef NDEBUG
    if(datacleanup < NA_MEMORY_CLEANUP_NONE || datacleanup >= NA_MEMORY_CLEANUP_COUNT)
      naError("naNewPointer", "cleanup method invalid");
  #endif
  pointer = naNew(NAPointer);
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refcount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrMutable(&(pointer->sptr), NA_MEMORY_CLEANUP_NA_DELETE, data, datacleanup);
  pointer->destructor = destructor;
  return pointer;
}



NA_HIDEF void naDestructPointer(NAPointer* pointer){
  if(pointer->destructor){
    pointer->destructor(naGetSmartPtrMutable(&(pointer->sptr)));
  }
}



NA_IDEF NAPointer* naRetainPointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer)
      {naCrash("naRetainPointer", "pointer is Null-Pointer."); return NA_NULL;}
  #endif
  naRetainSmartPtr(&(pointer->sptr));
  return pointer;
}



NA_IDEF NABool naReleasePointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer){
      naCrash("naReleasePointer", "pointer is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return naReleaseSmartPtr(&(pointer->sptr), pointer->destructor);
}




NA_IDEF const void* naGetPointerConst(const NAPointer* pointer){
  return naGetSmartPtrConst(&(pointer->sptr));
}



NA_IDEF void* naGetPointerMutable(NAPointer* pointer){
  return naGetSmartPtrMutable(&(pointer->sptr));
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
