
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAArray.h
// Do not include this file directly! It will automatically be included when
// including "NAArray.h"


#include "NAByteArray.h"
#include "NACoord.h"


struct NAArray{
  struct NAByteArray  bytearray;  // The byte array
  NAUInt              typesize;   // The size in bytes of the stored type
};



NA_IDEF NAArray* naInitArray(NAArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naInitArray", "array is Null-Pointer"); return NA_NULL;}
  #endif
  naInitByteArray(&(array->bytearray));
  // The typesize is set to 1 such that calls to naGetArrayCount will not
  // result in bad values.
  array->typesize = 1;
  return array;
}



NA_IDEF void naClearArray(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naClearArray", "array is Null-Pointer.");
      return;
    }
    // Note that NALib does not check, if you initially created the array
    // with a constructor callback. You are responsible to pair constructors
    // and destructors appropriately.
  #endif
  naClearByteArray(&(array->bytearray));
}



NA_IDEF void naDecoupleArray(NAArray* array){
  naDecoupleByteArray(&(array->bytearray), NA_FALSE);
}



NA_IDEF void naForeachArray(NAArray* array, NAMutator mutator){
  // Declaration before implementation. Needed for C90
  NAUInt count;
  NAByte* ptr;
  #ifndef NDEBUG
    if(!mutator)
      {naCrash("naForeachArray", "Mutator is Null"); return;}
  #endif
  count = naGetArrayCount(array);
  ptr = naGetByteArrayMutablePointer(&(array->bytearray));
  while(count){
    mutator(ptr);
    ptr += array->typesize;
    count--;
  }
}



NA_IDEF const void* naGetArrayConstPointer(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayConstPointer", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayConstPointer", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetByteArrayConstPointer(&(array->bytearray));
}



NA_IDEF void* naGetArrayMutablePointer(NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMutablePointer", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayMutablePointer", "array is empty, returned pointer is NULL");
    }
  #endif
  return naGetByteArrayMutablePointer(&(array->bytearray));
}



NA_IDEF const void* naGetArrayConstElement(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayConstElement", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayConstElement", "array is empty");
    }
  #endif
  return naGetByteArrayConstByte(&(array->bytearray), indx * array->typesize);
}



NA_IDEF void* naGetArrayMutableElement(NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMutableElement", "array is Null-Pointer.");
      return NA_NULL;
    }else{
      if(naIsByteArrayEmpty(&(array->bytearray)))
        naError("naGetArrayMutableElement", "array is empty");
    }
  #endif
  return naGetByteArrayMutableByte(&(array->bytearray), indx * array->typesize);
}



NA_IDEF NAUInt naGetArrayCount(const NAArray* array){
  NAInt bytesize;
  
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCount", "array is Null-Pointer.");
      return 0;
    }
  #endif
  bytesize = naGetByteArrayBytesize(&(array->bytearray));
  #ifndef NDEBUG
    if(bytesize < 0)
      naError("naGetArrayCount", "ByteArray count was negative. Do not create Arrays with negative sizes!");
  #endif
  // Note that an empty array has a typesize of 1.
  return bytesize / array->typesize;
}



NA_IDEF NAUInt naGetArrayMaxIndex(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayMaxIndex", "array is Null-Pointer.");
      return 0;
    }
    if(naIsArrayEmpty(array))
      naError("naGetArrayMaxIndex", "array is empty.");
  #endif
  return naMakeMaxWithEndi(naGetArrayCount(array));
}



NA_IDEF NAUInt naGetArrayTypesize(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayTypesize", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return array->typesize;
}



NA_IDEF NABool naIsArrayEmpty(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayEmpty", "array is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return naIsByteArrayEmpty(&(array->bytearray));
}









NA_IDEF void naFirstArray(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naFirstArray", "array is Null-Pointer.");
      return;
    }
  #endif
  naFirstByteArray(&(array->bytearray));
}



NA_IDEF void naLastArray(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naLastArray", "array is Null-Pointer.");
      return;
    }
  #endif
  naLastByteArray(&(array->bytearray));
}



NA_IDEF const void* naIterateArrayConst(const NAArray* array, NAInt step){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateArrayConst", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsArrayEmpty(array))
      naError("naIterateArrayConst", "array is empty.");
  #endif
  return naIterateByteArrayConst(&(array->bytearray), step * naGetArrayTypesize(array));
}



NA_IDEF void* naIterateArrayMutable(NAArray* array, NAInt step){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateArrayMutable", "array is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsArrayEmpty(array))
      naError("naIterateArrayMutable", "array is empty.");
  #endif
  return naIterateByteArrayMutable(&(array->bytearray), step * naGetArrayTypesize(array));
}



NA_IDEF const void* naGetArrayCurrentConst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCurrentConst", "array is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteArrayCurrentConst(&(array->bytearray));
}



NA_IDEF void* naGetArrayCurrentMutable(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCurrentConst", "array is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteArrayCurrentMutable(&(array->bytearray));
}



NA_IDEF NAUInt naGetArrayCurrentIndex(const NAArray* array){
  NAUInt byteindex;
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayCurrentIndex", "array is Null-Pointer.");
      return NA_INVALID_MEMORY_INDEX;
    }
  #endif
  byteindex = naGetByteArrayCurrentIndex(&(array->bytearray));
  #ifndef NDEBUG
    if(byteindex % naGetArrayTypesize(array))
      naError("naGetArrayCurrentIndex", "Internal error: Internal index is misaligned.");
  #endif
  return byteindex / naGetArrayTypesize(array);
}



NA_IDEF NAUInt naGetArrayRemainingCount(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naGetArrayRemainingCount", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return naGetArrayCount(array) - naGetArrayCurrentIndex(array);
}


NA_IDEF NABool naLocateArrayIndex(const NAArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array){
      naCrash("naFirstArray", "array is Null-Pointer.");
      return 0;
    }
  #endif
  return naLocateByteArrayIndex(&(array->bytearray), indx * naGetArrayTypesize(array));
}



NA_IDEF void naIterateArray(const NAArray* array, NAInt step){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIterateArray", "array is Null-Pointer.");
      return;
    }
    if(naIsArrayEmpty(array))
      naError("naIterateArray", "array is empty.");
  #endif
  naIterateByteArray(&(array->bytearray), step * naGetArrayTypesize(array));
}



NA_IDEF NABool naIsArrayAtFirst(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayAtFirst", "array is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsArrayEmpty(array))
      naError("naIsArrayAtFirst", "array is empty. Result is garbage");
  #endif
  return (naGetArrayCurrentIndex(array) == 0);
}



NA_IDEF NABool naIsArrayAtLast(const NAArray* array){
  #ifndef NDEBUG
    if(!array){
      naCrash("naIsArrayAtLast", "array is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsArrayEmpty(array))
      naError("naIsArrayAtLast", "array is empty. Result is garbage");
  #endif
  return (naGetArrayCurrentIndex(array) == naGetArrayMaxIndex(array));
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
