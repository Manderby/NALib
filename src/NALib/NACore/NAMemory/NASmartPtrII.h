// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemoryII.h"



struct NASmartPtr{
  NARefCount refcount;
  NAPtr ptr;
};





NA_IDEF NASmartPtr* naInitSmartPtrConst(NASmartPtr* sptr, const void* data){
  sptr->ptr = naMakePtrWithDataConst(data);
  naInitRefCount(&(sptr->refcount));
  return sptr;
}



NA_IDEF NASmartPtr* naInitSmartPtrMutable(NASmartPtr* sptr, void* data){
  sptr->ptr = naMakePtrWithDataMutable(data);
  naInitRefCount(&(sptr->refcount));
  return sptr;
}



NA_IDEF NASmartPtr* naRetainSmartPtr(NASmartPtr* sptr){
  return (NASmartPtr*)naRetainRefCount(&(sptr->refcount));
}



NA_IDEF void naReleaseSmartPtr(NASmartPtr* sptr, NAMutator destructor, NABool onlydata){
  #ifndef NDEBUG
    if(!sptr)
      naCrash("naReleaseSmartPtr", "sptr is Null-Pointer.");
  #endif
  if(onlydata){
    naReleaseRefCount(&(sptr->refcount), naGetPtrMutable(sptr->ptr), destructor);
  }else{
    naReleaseRefCount(&(sptr->refcount), &(sptr->refcount), destructor);
  }
}



NA_IDEF const void* naGetSmartPtrConst(const NASmartPtr* sptr){
  return naGetPtrConst(sptr->ptr);
}



NA_IDEF void* naGetSmartPtrMutable(NASmartPtr* sptr){
  return naGetPtrMutable(sptr->ptr);
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
