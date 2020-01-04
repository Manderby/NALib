// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAMemory.h
// Do not include this file directly! It will automatically be included when
// including "NAMemory.h"




// An NAPointer stores a pointer with a reference count. You can increase and
// decrease that reference count and the pointer will automatically be erased
// when the reference count reaches zero. How it will be erased can be defined
// upon creation of the NAPointer.

struct NAPointer{
  NASmartPtr sptr;
  NAMutator destructor;
};



NA_IDEF NAPointer* naInitPointerConst(NAPointer* pointer, const void* data){
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refcount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrConst(&(pointer->sptr), data);
  pointer->destructor = NA_NULL;
  return pointer;
}



NA_IDEF NAPointer* naInitPointerMutable(NAPointer* pointer, void* data, NAMutator destructor){
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refcount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrMutable(&(pointer->sptr), data);
  pointer->destructor = destructor;
  return pointer;
}



NA_IDEF NAPointer* naRetainPointer(NAPointer* pointer){
  return (NAPointer*)naRetainSmartPtr(&(pointer->sptr));
}



// The implementation of this function is in the NARuntime.c file.
NA_HAPI void naDestructPointer(NAPointer* pointer);



NA_IDEF void naReleasePointer(NAPointer* pointer){
  #ifndef NDEBUG
    if(!pointer)
      naCrash("pointer is Null-Pointer.");
  #endif
  naReleaseSmartPtr(&(pointer->sptr), (NAMutator)naDestructPointer, NA_FALSE);
}




NA_IDEF const void* naGetPointerConst(const NAPointer* pointer){
  return naGetSmartPtrConst(&(pointer->sptr));
}



NA_IDEF void* naGetPointerMutable(NAPointer* pointer){
  return naGetSmartPtrMutable(&(pointer->sptr));
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
