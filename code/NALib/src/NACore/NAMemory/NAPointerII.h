
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
  // object when refCount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrConst(&(pointer->sptr), data);
  pointer->destructor = NA_NULL;
  return pointer;
}



NA_IDEF NAPointer* naInitPointerMutable(NAPointer* pointer, void* data, NAMutator destructor){
  // As sptr is the first entry of pointer, we can trick sptr to delete the
  // object when refCount becomes zero and delete the whole NAPointer object
  // in the end.
  naInitSmartPtrMutable(&(pointer->sptr), data);
  pointer->destructor = destructor;
  return pointer;
}



NA_IDEF NAPointer* naRetainPointer(NAPointer* pointer){
  return (NAPointer*)naRetainSmartPtr(&(pointer->sptr));
}



// The implementation of this function is in the NARuntime.c file.
NA_HAPI void na_DestructPointer(NAPointer* pointer);



NA_IDEF void naReleasePointer(NAPointer* pointer){
  #if NA_DEBUG
    if(!pointer)
      naCrash("pointer is Null-Pointer.");
  #endif
  naReleaseSmartPtr(&(pointer->sptr), (NAMutator)na_DestructPointer, NA_FALSE);
}




NA_IDEF const void* naGetPointerConst(const NAPointer* pointer){
  return naGetSmartPtrConst(&(pointer->sptr));
}



NA_IDEF void* naGetPointerMutable(NAPointer* pointer){
  return naGetSmartPtrMutable(&(pointer->sptr));
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
