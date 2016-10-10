
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAByteArray.h
// Do not include this file directly! It will automatically be included when
// including "NAByteArray.h"

#include "NAMathOperators.h"


struct NAByteArray{
  NAPointer* storage;     // pointer to the storage of all bytes
  NAMemoryBlock memblock; // The memory block accessible to this NAByteArray.
  NAInt indx;             // internal index for iteration.
};
// Note that the memory block may not point to the same address as the data
// pointer stored in the storage!
//
// If the size of the memory block is zero, the NAByteArray is considered
// empty. In that case, the other fields contain no useful information.





NA_IDEF NAByteArray* naInitByteArray(NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitByteArray", "array is Null-Pointer."); return NA_NULL;}
  #endif
  array->storage = naNewNullPointer();
  array->memblock = naMakeMemoryBlock();
  array->indx = NA_INVALID_MEMORY_INDEX;
  return array;
}



NA_IDEF void naClearByteArray(NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naClearByteArray", "array is Null-Pointer."); return;}
  #endif
  naReleasePointer(array->storage);
}



NA_IAPI void naEmptyByteArray(NAByteArray* array){
  naClearByteArray(array);
  naInitByteArray(array);
}



NA_IDEF const NAByte* naGetByteArrayConstPointer(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayConstPointer", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsMemoryBlockEmpty(&(array->memblock)))
      naError("naGetByteArrayConstPointer", "array is empty. Result may be garbage");
  #endif
  return (const NAByte*)naGetMemoryBlockConstPointer(&(array->memblock));
}



NA_IDEF NAByte* naGetByteArrayMutablePointer(NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayMutablePointer", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsMemoryBlockEmpty(&(array->memblock)))
      naError("naGetByteArrayMutablePointer", "array is empty. Result may be garbage");
  #endif
  return (NAByte*)naGetMemoryBlockMutablePointer(&(array->memblock));
}



NA_IDEF const NAByte* naGetByteArrayConstByte(const NAByteArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayConstByte", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsMemoryBlockEmpty(&(array->memblock)))
      naError("naGetByteArrayConstByte", "array is empty. Result may be garbage");
  #endif
  if(indx < 0){indx += naGetByteArraySize(array);}
  #ifndef NDEBUG
    if(!naInsidei(0, naGetMemoryBlockMaxIndex(&(array->memblock)), indx))
      naError("naGetByteArrayMutableByte", "indx out of bounds");
  #endif
  return (const NAByte*)naGetMemoryBlockConstByte(&(array->memblock), (NAUInt)indx);
}



NA_IDEF NAByte* naGetByteArrayMutableByte(NAByteArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayMutableByte", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsMemoryBlockEmpty(&(array->memblock)))
      naError("naGetByteArrayMutableByte", "array is empty. Result may be garbage");
  #endif
  if(indx < 0){indx += naGetByteArraySize(array);}
  #ifndef NDEBUG
    if(!naInsidei(0, naGetMemoryBlockMaxIndex(&(array->memblock)), indx))
      naError("naGetByteArrayMutableByte", "indx out of bounds");
  #endif
  return (NAByte*)naGetMemoryBlockMutableByte(&(array->memblock), indx);
}



NA_IDEF NAUInt naGetByteArraySize(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArraySize", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return naGetMemoryBlockBytesize(&(array->memblock));
}



NA_IDEF NAUInt naGetByteArrayMaxIndex(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArraySize", "array is Null-Pointer.");
      return 0;
    }
    if(naIsByteArrayEmpty(array))
      naError("naGetByteArraySize", "array is empty.");
  #endif
  return naEndToMaxi(naGetByteArraySize(array));
}



NA_IDEF NABool naIsByteArrayEmpty(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsByteArrayEmpty", "array is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return naIsMemoryBlockEmpty(&(array->memblock));
}


#ifndef NDEBUG
  NA_IDEF NABool naIsByteArrayNullTerminated(const NAByteArray* array){
    if(!array){
      naCrash("naIsByteArrayNullTerminated", "array is Null-Pointer.");
      return NA_TRUE;
    }
    return naIsMemoryBlockNullTerminated(&(array->memblock));
  }
#endif








NA_IDEF void naFirstByteArray(const NAByteArray* array){
  NAByteArray* mutablearray = (NAByteArray*)array;
  #ifndef NDEBUG
    if(!array){
      naCrash("naFirstByteArray", "array is Null-Pointer.");
      return;
    }
  #endif
  mutablearray->indx = 0;
}



NA_IDEF void naLastByteArray(const NAByteArray* array){
  NAByteArray* mutablearray = (NAByteArray*)array;
  #ifndef NDEBUG
    if(!array){
      naCrash("naLastByteArray", "array is Null-Pointer.");
      return;
    }
  #endif
  mutablearray->indx = naGetByteArraySize(array) - 1;
}



NA_IDEF const NAByte* naIterateByteArrayConst(const NAByteArray* array, NAInt step){
  const NAByte* retbyte;
  NAByteArray* mutablearray = (NAByteArray*)array;
  NABool isvalid;
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateByteArrayConst", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsByteArrayEmpty(array))
      naError("naIterateByteArrayConst", "array is empty.");
    if(!naInsidei(0, naGetByteArrayMaxIndex(array), array->indx))
      naError("naIterateByteArray", "iteration over- or underflow.");
  #endif
  isvalid = naInsidei(0, naGetByteArrayMaxIndex(array), array->indx);
  if(isvalid){
    retbyte = naGetByteArrayConstByte(mutablearray, mutablearray->indx);
    naIterateByteArray(mutablearray, step);
  }else{
    retbyte = NA_NULL;
  }
  return retbyte;
}



NA_IDEF NAByte* naIterateByteArrayMutable(NAByteArray* array, NAInt step){
  NAByte* retbyte;
  NAByteArray* mutablearray = (NAByteArray*)array;
  NABool isvalid;
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateByteArrayMutable", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsByteArrayEmpty(array))
      naError("naIterateByteArrayMutable", "array is empty.");
    if(!naInsidei(0, naGetByteArrayMaxIndex(array), array->indx))
      naError("naIterateByteArray", "iteration over- or underflow.");
  #endif
  isvalid = naInsidei(0, naGetByteArrayMaxIndex(array), array->indx);
  if(isvalid){
    retbyte = naGetByteArrayMutableByte(mutablearray, mutablearray->indx);
    naIterateByteArray(mutablearray, step);
  }else{
    retbyte = NA_NULL;
  }
  return retbyte;
}



NA_IDEF const void* naGetByteArrayCurrentConst(const NAByteArray* array){
  NAByteArray* mutablearray = (NAByteArray*)array;
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayCurrentConst", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(!naInsidei(0, naGetByteArrayMaxIndex(array), array->indx))
      naError("naIterateByteArray", "iteration over- or underflow.");
  #endif
  return naGetByteArrayConstByte(mutablearray, mutablearray->indx);
}



NA_IDEF void* naGetByteArrayCurrentMutable(const NAByteArray* array){
  NAByteArray* mutablearray = (NAByteArray*)array;
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayCurrentConst", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(!naInsidei(0, naGetByteArrayMaxIndex(array), array->indx))
      naError("naIterateByteArray", "iteration over- or underflow.");
  #endif
  return naGetByteArrayMutableByte(mutablearray, mutablearray->indx);
}



NA_IDEF NAUInt naGetByteArrayCurrentIndex(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayCurrentIndex", "array is Null-Pointer.");
      return 0;
    }
    if(array->indx < 0)
      naError("naGetByteArrayCurrentIndex", "Internal error: Index is negative.");
  #endif
  return array->indx;
}



NA_IDEF NAUInt naGetByteArrayRemainingSize(const NAByteArray* array){
  NAUInt retvalue;
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetByteArrayRemainingSize", "array is Null-Pointer.");
      return 0;
    }
  #endif
  retvalue = naGetByteArraySize(array) - array->indx;
  #ifndef NDEBUG
    if(!naInsidei(0, naGetByteArraySize(array), retvalue))
      naError("naGetByteArrayRemainingSize", "return value does not fit array bounds.");
  #endif
  return retvalue;
}



NA_IDEF NABool naLocateByteArrayIndex(const NAByteArray* array, NAInt indx){
  NAByteArray* mutablearray = (NAByteArray*)array;
  #ifndef NDEBUG
    if(!array){
      naCrash("naFirstByteArray", "array is Null-Pointer.");
      return 0;
    }
  #endif
  if(indx < 0){indx += naGetByteArraySize(array);}
  mutablearray->indx = indx;
  return naInsidei(0, naGetMemoryBlockMaxIndex(&(array->memblock)), indx);
}



NA_IDEF void naIterateByteArray(const NAByteArray* array, NAInt step){
  NAByteArray* mutablearray = (NAByteArray*)array;
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateByteArray", "array is Null-Pointer.");
      return;
    }
    if(naIsByteArrayEmpty(array))
      naError("naIterateByteArray", "array is empty.");
    if(!naInsidei(0, naGetByteArrayMaxIndex(array), array->indx))
      naError("naIterateByteArray", "iteration over- or underflow.");
  #endif
  mutablearray->indx += step;
}



NA_IDEF NABool naIsByteArrayAtFirst(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsByteArrayAtFirst", "array is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsByteArrayEmpty(array))
      naError("naIsByteArrayAtFirst", "array is empty. Result is garbage");
  #endif
  return (naGetByteArrayCurrentIndex(array) == 0);
}



NA_IDEF NABool naIsByteArrayAtLast(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsByteArrayAtLast", "array is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsByteArrayEmpty(array))
      naError("naIsByteArrayAtLast", "array is empty. Result is garbage");
  #endif
  return (naGetByteArrayCurrentIndex(array) == naGetByteArrayMaxIndex(array));
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
