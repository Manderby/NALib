
// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



struct NASmartPtr{
  NARefCount refCount;
  NAPtr ptr;
};





NA_IDEF NASmartPtr* naInitSmartPtrConst(NASmartPtr* sptr, const void* data){
  sptr->ptr = naMakePtrWithDataConst(data);
  naInitRefCount(&(sptr->refCount));
  return sptr;
}



NA_IDEF NASmartPtr* naInitSmartPtrMutable(NASmartPtr* sptr, void* data){
  sptr->ptr = naMakePtrWithDataMutable(data);
  naInitRefCount(&(sptr->refCount));
  return sptr;
}



NA_IDEF NASmartPtr* naRetainSmartPtr(NASmartPtr* sptr){
  return (NASmartPtr*)naRetainRefCount(&(sptr->refCount));
}



NA_IDEF void naReleaseSmartPtr(NASmartPtr* sptr, NAMutator destructor, NABool onlyData){
  #if NA_DEBUG
    if(!sptr)
      naCrash("sptr is Null-Pointer.");
  #endif
  if(onlyData){
    naReleaseRefCount(&sptr->refCount, naGetPtrMutable(sptr->ptr), destructor);
  }else{
    naReleaseRefCount(&sptr->refCount, sptr, destructor);
  }
}



NA_IDEF const void* naGetSmartPtrConst(const NASmartPtr* sptr){
  return naGetPtrConst(sptr->ptr);
}



NA_IDEF void* naGetSmartPtrMutable(NASmartPtr* sptr){
  return naGetPtrMutable(sptr->ptr);
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
