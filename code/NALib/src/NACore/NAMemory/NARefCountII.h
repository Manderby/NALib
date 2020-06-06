
// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



struct NARefCount{
  NAUInt count;      // Reference count.
  #ifndef NDEBUG
    NAUInt dummy;
  #endif
};

// The count field also stores two cleanup hints in the 2*3 bits on the
// higher endian side. This is necessary due to the automatic deletion of a
// pointer and the struct the refCount is stored in when refCount reaches zero.
// It could be stored in a separate flag but the author decided to do it with
// a mask.
//
// This means that not the full range of the count integer can be used for
// reference counting. More precisely, the integer falls 6 Bits short. So in a
// 32 bit system, you can have a max of 2^26 = 67 million references and on a
// 64 bit system, you can have a max of 2^58 = [insert huge number here] refs.
//
// Note that when NDEBUG is undefined, there are additional flags which make
// debugging easier and also serve to detect hard to find memory bugs. The
// dummy field stores a very specific number which should never be overwritten.
// If it is overwritten or otherwise not correct, the NARefCount has been used
// wrongly.


#if NA_TYPE_NAINT_BITS == 64
  #define NA_REFCOUNT_DUMMY_VALUE (NAUInt)0xaaaaaaaaaaaaaaaaLL
#else
  #define NA_REFCOUNT_DUMMY_VALUE (NAUInt)0xaaaaaaaa
#endif



NA_HIDEF NAUInt naGetRefCountCount(const NARefCount* refCount){
  return refCount->count;
}



NA_IDEF NARefCount* naInitRefCount(NARefCount* refCount){
  refCount->count = 1;
  #ifndef NDEBUG
    refCount->dummy = NA_REFCOUNT_DUMMY_VALUE;
  #endif
  return refCount;
}



NA_IDEF NARefCount* naRetainRefCount(NARefCount* refCount){
  #ifndef NDEBUG
    if(!refCount){
      naCrash("refCount is Null-Pointer.");
      return NA_NULL;
    }else{
      if(refCount->dummy != NA_REFCOUNT_DUMMY_VALUE)
        naError("Consistency problem: dummy value wrong. Is NARefCount really defined as the first field of this struct?");
      // The next test can detect some erroneous behaviour in the code. Note
      // however that most likely the true cause of the error did occur long
      // before reaching here.
      if(refCount->count == NA_ZERO)
        naCrash("Retaining NARefCount with a count of 0");
    }
  #endif
  refCount->count++;
  #ifndef NDEBUG
    // If refCount now suddenly becomes zero, there was either an error earlier
    // or the object has been retained too many times. Overflow.
    if(refCount->count == NA_ZERO)
      naError("Reference count overflow");
  #endif
  return refCount;
}



NA_IDEF void naReleaseRefCount(NARefCount* refCount, void* data, NAMutator destructor){
  #ifndef NDEBUG
    if(!refCount)
      naCrash("refCount is Null-Pointer.");
    if(refCount->dummy != NA_REFCOUNT_DUMMY_VALUE)
      naError("Consistency problem: dummy value wrong. Is NARefCount really defined as the first field of this struct?");
    // The next test can detect some erroneous behaviour in the code. Note
    // however that most likely the true cause of the error did occur long
    // before reaching here.
    if(refCount->count == NA_ZERO)
      naCrash("Releasing NARefCount with a count of 0");
  #endif
  // Note that the author decided to always count to zero, even if it is clear
  // that the pointer will eventually be freed and the data will be lost in
  // nirvana. But often times in debugging, when retaining and releasing is not
  // done correctly, an NARefCount is released too often. When refCount is 0
  // and NDEBUG is not defined, this can be detected!
  refCount->count--;

  if(refCount->count == NA_ZERO){
    // Call the destructor on the data if available.
    if(destructor){destructor(data);}
  }
  // Note that other programming languages have incorporated this very idea
  // of self-organized reference-counting pointers deeply within its core.
  // Their runtime-systems keep giant pools of free objects at hand and take
  // care of detecting and collecting unused objects. In C and C++, no such
  // mechanisms exist and must be implemented manually. NARuntime is a small
  // example of such a system.
}



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
