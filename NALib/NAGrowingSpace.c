
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAGrowingSpace.h"
#include "NAMathOperators.h"
#include <stdlib.h>



NAGrowingSpace* naCreateGrowingSpace(NAGrowingSpace* space, NAInt typesize, NAInt initialcount){
  space = naAllocateIfNull(space, sizeof(NAGrowingSpace));
  if(initialcount == 0){initialcount = 1;}
  // Note>: An initial count of 0 is not allowed as otherwise the array would
  // be initialized as an empty array which in turn does not stores the
  // typesize which is used later on to grow the array.
  naCreateArrayWithCount(&(space->array), typesize, initialcount);
  space->usedcount = 0;
  return space;
}


NAGrowingSpace* naDuplicateGrowingSpace(NAGrowingSpace* space, const NAGrowingSpace* originalspace){
  space = naAllocateIfNull(space, sizeof(NAGrowingSpace));
  space->usedcount = originalspace->usedcount;
  if(space->usedcount){
    naCreateArrayExtraction(&(space->array), &(originalspace->array), 0, space->usedcount);
  }else{
    naCreateArrayExtraction(&(space->array), &(originalspace->array), 0, 1);
  }
  naDecoupleArray(&(space->array));
  return space;
}


void naClearGrowingSpace(NAGrowingSpace* space, NADestructor destructor){
  #ifndef NDEBUG
    if(!space){
      naCrash("naClearGrowingSpace", "space is Null-Pointer.");
    }
  #endif

  if(destructor){
    NAByte* ptr = (NAByte*)naGetArrayMutablePointer(&(space->array));
    NAInt typesize = naGetArrayTypeSize(&(space->array));
    NAInt i;
    for(i=0; i<space->usedcount; i++){
      destructor(ptr);
      ptr += typesize;
    }
  }

  naClearArray(&(space->array), NA_NULL);
}


void naDestroyGrowingSpace(NAGrowingSpace* space, NADestructor destructor){
  naClearGrowingSpace(space, destructor);
  free(space);
}


NA_INLINE static void* naGrowGrowingSpaceByOne(NAGrowingSpace* space){
  void* dataptr;
  NAInt arraycount = naGetArrayCount(&(space->array));
  if(space->usedcount == arraycount){
    NAArray newarray;
    naCreateArrayWithCount(&newarray, space->array.typesize, arraycount * 2);
    naCpyn(naGetArrayMutablePointer(&newarray), naGetArrayMutablePointer(&(space->array)), space->usedcount * naGetArrayTypeSize(&(space->array)));
    naClearArray(&(space->array), NA_NULL);
    space->array = newarray;
  }
  dataptr = naGetArrayMutableElement(&(space->array), space->usedcount);
  space->usedcount++;
  return dataptr;
}


void* naNewGrowingSpaceElement(NAGrowingSpace* space, NAInt* indexptr){
  void* dataptr = naGrowGrowingSpaceByOne(space);
  if(indexptr){*indexptr = space->usedcount - 1;}
  return dataptr;
}


void* naAddGrowingSpaceElement(NAGrowingSpace* space, void* elemptr){
  void* dataptr = naGrowGrowingSpaceByOne(space);
  if(elemptr){naCpyn(dataptr, elemptr, space->array.typesize);}
  return dataptr;
}


const void* naGetGrowingSpaceConstPointer(const NAGrowingSpace* space){
  #ifndef NDEBUG
    if(!space){
      naCrash("naGetGrowingSpaceConstPointer", "space is Null-Pointer.");
    }else{
      if(naIsArrayEmpty(&(space->array)))
        naError("naGetGrowingSpaceConstPointer", "space is empty, returned pointer is NULL");
    }
  #endif
  return naGetArrayConstPointer(&(space->array));
}

void* naGetGrowingSpaceMutablePointer(NAGrowingSpace* space){
  #ifndef NDEBUG
    if(!space){
      naCrash("naGetGrowingSpaceMutablePointer", "space is Null-Pointer.");
    }else{
      if(naIsArrayEmpty(&(space->array)))
        naError("naGetGrowingSpaceMutablePointer", "space is empty, returned pointer is NULL");
    }
  #endif
  return naGetArrayMutablePointer(&(space->array));
}


const void* naGetGrowingSpaceConstElement(const NAGrowingSpace* space, NAInt indx){
  #ifndef NDEBUG
    if(!space){
      naCrash("naGetGrowingSpaceConstElement", "space is Null-Pointer.");
    }else{
      if(naIsArrayEmpty(&(space->array)))
        naError("naGetGrowingSpaceConstElement", "space is empty, typesize is garbage");
    }
  #endif
  return naGetArrayConstElement(&(space->array), indx);
}

void* naGetGrowingSpaceMutableElement(NAGrowingSpace* space, NAInt indx){
  #ifndef NDEBUG
    if(!space){
      naCrash("naGetGrowingSpaceMutableElement", "space is Null-Pointer.");
    }else{
      if(naIsArrayEmpty(&(space->array)))
        naError("naGetGrowingSpaceMutableElement", "space is empty, typesize is garbage");
    }
  #endif
  return naGetArrayMutableElement(&(space->array), indx);
}




NAArray* naCreateArrayOutOfGrowingSpace(NAArray* array, NAGrowingSpace* space, NABool copying){
  if(copying){
    array = naCreateArrayWithCount(array, space->array.typesize, space->usedcount);
    naCpyn(naGetByteArrayMutablePointer(&(array->bytearray)), naGetByteArrayMutablePointer(&(space->array.bytearray)), space->usedcount * space->array.typesize);
  }else{
    array = naCreateArrayExtraction(array, &(space->array), 0, space->usedcount);
  }
  return array;
}


NAInt naGetGrowingSpaceCount(const NAGrowingSpace* space){
  return space->usedcount;
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
