// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"




#if NA_OS == NA_OS_WINDOWS
  #include <Windows.h>
#elif NA_OS == NA_OS_MAC_OS_X
  #include <unistd.h>
  #include "malloc/malloc.h"
#endif
#include <stdlib.h>



NA_IDEF void* naMalloc(NAInt bytesize){
  void* ptr;

  #ifndef NDEBUG
    if(bytesize == NA_ZERO)
      naCrash("size is zero.");
    if(bytesize < NA_ZERO)
      naCrash("size is negative.");
  #endif

  ptr = malloc((size_t)bytesize);

  #ifndef NDEBUG
  if(!ptr)
    naCrash("Out of memory");
  #endif

  return ptr;
}


// We "prototyped" the naSizeof macro in the .h file, therefore the undef.
#undef naSizeof
#define naSizeof(type) ((NAInt)sizeof(type))


// We "prototyped" the naAlloc macro in the .h file, therefore the undef.
#undef naAlloc
#define naAlloc(type) (type*)naMalloc(naSizeof(type))



NA_IDEF void naFree(void* ptr){
  free(ptr);
}



NA_IDEF void* naMallocAligned(NAUInt bytesize, NAUInt align){
  void* retptr;

  #if NA_OS == NA_OS_WINDOWS
    retptr = _aligned_malloc(bytesize, align);
  #else
    #if NA_MEMALIGN == NA_MEMALIGN_USE_CUSTOM
      // Allocate a full align and a pointer more than required.
      void* mem = malloc(bytesize + align + sizeof(void*));
      #ifndef NDEBUG
      if(!mem)
        naCrash("naMallocAligned", "Out of memory");
      #endif
      // make a pointer point to the first byte being aligned within the memory
      // allocated in mem which comes after align bytes and a pointer size.
      void** ptr = (void**)((size_t)((NAByte*)mem + align + sizeof(void*)) & ~(align - NA_ONE));
      // Store the pointer to the original allocation pointer in the element
      // right before the first returned byte.
      ptr[-1] = mem;
      retptr = ptr;
    #elif NA_MEMALIGN == NA_MEMALIGN_USE_ALIGNED_ALLOC
      retptr = aligned_alloc(align, bytesize);
      #ifndef NDEBUG
      if(!retptr)
        naCrash("naMallocAligned", "Out of aligned memory");
      #endif
    #elif NA_MEMALIGN == NA_MEMALIGN_USE_POSIX
      int error = posix_memalign(&retptr, align, bytesize);
      #ifndef NDEBUG
      if(error)
        naCrash("Memory alignment failed");
      #else
        NA_UNUSED(error);
      #endif
    #else
      #error "Invalid aligned alloc method chosen"
    #endif
  #endif

  #ifndef NDEBUG
    if(((NAUInt)retptr & (NAUInt)(align - NA_ONE)) != NA_ZERO)
      naError("pointer misaligned.");
  #endif

  return retptr;
}



NA_IDEF void* naMallocPageAligned(NAUInt bytesize){
  // Note that due to some strange reason, aligned_alloc and valloc not work
  // with clang. Therefore, a simple call to naMallocAligned is used.
  return naMallocAligned(bytesize, naGetSystemMemoryPagesize());
}



NA_IDEF void naFreeAligned(void* ptr){
  #if NA_OS == NA_OS_WINDOWS
    _aligned_free(ptr);
  #else
    #if NA_MEMALIGN == NA_MEMALIGN_USE_CUSTOM
      free(((void**)ptr)[-1]);
    #elif NA_MEMALIGN == NA_MEMALIGN_USE_ALIGNED_ALLOC
      free(ptr);
    #elif NA_MEMALIGN == NA_MEMALIGN_USE_POSIX
      free(ptr);
    #else
      #error "Invalid aligned alloc method chosen"
    #endif
  #endif
}




// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
