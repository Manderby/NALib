
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAGrowingSpace.h"
#include "NAMathOperators.h"
#include <stdlib.h>


// Currently, the NAGrowingSpace structure grows in blocks storing a predefined
// number of elements.
#define NA_GROWING_SPACE_SINGLE_ARRAY_SIZE 64
// Note that this is not generally optimal. Will be altered in the future to
// make space grow slowly. Fibonaccy seems to be the solution. todo



NA_HLP void naAddGrowingSpaceNewSpace(NAGrowingSpace* space){
  NAByteArray* newarray = naCreateByteArrayWithSize(NA_NULL, NA_GROWING_SPACE_SINGLE_ARRAY_SIZE * space->typesize);
  if(space->constructor){
    NAByte* curptr = naGetByteArrayMutablePointer(newarray);
    NAInt i;
    for(i=0; i<NA_GROWING_SPACE_SINGLE_ARRAY_SIZE; i++){
      space->constructor(curptr);
      curptr += space->typesize;
    }
  }
  naAddListElementLast(&(space->arrays), newarray);
}



NA_DEF NAGrowingSpace* naCreateGrowingSpace(NAGrowingSpace* space, NAInt typesize, NAConstructor constructor){
  #ifndef NDEBUG
    if(typesize <= 0)
      naError("naCreateGrowingSpace", "typesize is invalid.");
  #endif
  space = naAllocateIfNull(space, sizeof(NAGrowingSpace));
  naCreateList(&(space->arrays));
  space->typesize = typesize;
  space->usedcount = 0;
  space->constructor = constructor;
  return space;
}



NA_DEF void naClearGrowingSpace(NAGrowingSpace* space, NADestructor destructor){
  NAByteArray* curarray = NA_NULL; // Declaration before Implementation.
  NAInt arraycount;
  NAInt i, k;
  #ifndef NDEBUG
    if(!space){
      naCrash("naClearGrowingSpace", "space is Null-Pointer.");
      return;
    }
  #endif

  naFirstListElement(&(space->arrays));
  arraycount = naGetListCount(&(space->arrays));
  for(i=0; i<arraycount; i++){
    curarray = naGetListMutableContent(&(space->arrays));
    NAInt remainingcount;
    if(i == arraycount - 1){
      remainingcount = space->usedcount % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
    }else{
      remainingcount = NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
    }
    if(destructor){
      NAByte* curptr = naGetByteArrayMutablePointer(curarray);
      for(k=0; k<remainingcount; k++){
        destructor(curptr);
        curptr += space->typesize;
      }
    }
    naDestroyByteArray(curarray);
    naNextListElement(&(space->arrays));
  }

  naClearList(&(space->arrays), NA_NULL);
}



NA_DEF void naDestroyGrowingSpace(NAGrowingSpace* space, NADestructor destructor){
  naClearGrowingSpace(space, destructor);
  free(space);
}



NA_DEF void* naNewGrowingSpaceElement(NAGrowingSpace* space){
  NAByteArray* lastarray; // Declaration before Implementation. Needed for C90
  NAInt subindex;
  NAInt fullcount = naGetListCount(&(space->arrays)) * NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  if(space->usedcount == fullcount){naAddGrowingSpaceNewSpace(space);}
  naLastListElement(&(space->arrays));
  lastarray = naGetListMutableContent(&(space->arrays));
  subindex = space->usedcount % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  space->usedcount++;
  return naGetByteArrayMutableByte(lastarray, subindex * space->typesize);
}



NA_DEF const void* naGetGrowingSpaceConstContent(const NAGrowingSpace* space){
  const NAByteArray* curarray;
  curarray = naGetListConstContent(&(space->arrays));
  if(!curarray){return NA_NULL;}
  return naGetByteArrayConstByte(curarray, space->cur * space->typesize);
}



NA_DEF void* naGetGrowingSpaceMutableContent(NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  NAByteArray* curarray;
  curarray = naGetListMutableContent(&(mutablespace->arrays));
  if(!curarray){return NA_NULL;}
  return naGetByteArrayMutableByte(curarray, space->cur * mutablespace->typesize);
}



NA_DEF void naFirstGrowingSpaceElement(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  naFirstListElement(&(mutablespace->arrays));
  mutablespace->cur = 0;
}



NA_DEF void naNextGrowingSpaceElement(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  NAUInt remainingcount = NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  if(naIsListAtLast(&(mutablespace->arrays))){
    remainingcount = space->usedcount % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
    if(remainingcount == 0){remainingcount = NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;}
  }
  mutablespace->cur++;
  if(mutablespace->cur == remainingcount){
    naNextListElement(&(mutablespace->arrays));
    mutablespace->cur = 0;
  }
}



NA_DEF const void* naGetGrowingSpaceConstElement(const NAGrowingSpace* space, NAInt indx){
  // Declaration before Implementation. Needed for C90
  NAInt arrayindex;
  NAInt subindex;
  NAInt i;
  const NAByteArray* curarray;
  #ifndef NDEBUG
    if(!space){
      naCrash("naGetGrowingSpaceConstElement", "space is Null-Pointer.");
      return NA_NULL;
    }
    if(indx < 0)
      naError("naGetGrowingSpaceConstElement", "Given index is negative.");
    if(indx >= space->usedcount)
      naError("naGetGrowingSpaceConstElement", "Given index overflows the available space.");
  #endif
  arrayindex = indx / NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  subindex = indx % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  naFirstListElement(&(space->arrays));
  for(i=0; i<arrayindex; i++){naNextListElement(&(space->arrays));}
  curarray = naGetListConstContent(&(space->arrays));
  return naGetByteArrayConstByte(curarray, subindex * space->typesize);
}



NA_DEF void* naGetGrowingSpaceMutableElement(NAGrowingSpace* space, NAInt indx){
  // Declaration before Implementation. Needed for C90
  NAInt arrayindex;
  NAInt subindex;
  NAInt i;
  NAByteArray* curarray;
  #ifndef NDEBUG
    if(!space){
      naCrash("naGetGrowingSpaceMutableElement", "space is Null-Pointer.");
      return NA_NULL;
    }
    if(indx < 0)
      naError("naGetGrowingSpaceMutableElement", "Given index is negative.");
    if(indx >= space->usedcount)
      naError("naGetGrowingSpaceMutableElement", "Given index overflows the available space.");
  #endif
  arrayindex = indx / NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  subindex = indx % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  naFirstListElement(&(space->arrays));
  for(i=0; i<arrayindex; i++){naNextListElement(&(space->arrays));}
  curarray = naGetListMutableContent(&(space->arrays));
  return naGetByteArrayMutableByte(curarray, subindex * space->typesize);
}




NA_DEF NAArray* naCreateArrayOutOfGrowingSpace(NAArray* array, NAGrowingSpace* space){
  // Declaration before Implementation. Needed for C90
  NAByte* arrayptr;
  NAInt bytearraycount;
  NAInt bytearraysize;
  NAInt remainingcount;
  NAInt i;
  array = naCreateArrayWithCount(array, space->typesize, space->usedcount, NA_NULL);
  arrayptr = naGetArrayMutablePointer(array);
  bytearraycount = naGetListCount(&(space->arrays));
  bytearraysize = NA_GROWING_SPACE_SINGLE_ARRAY_SIZE * space->typesize;
  naFirstListElement(&(space->arrays));
  // Copy all full arrays as a whole
  for(i=0; i<(bytearraycount-1); i++){
    const NAByteArray* curarray = naGetListConstContent(&(space->arrays));
    const NAByte* firstbyte = naGetByteArrayConstPointer(curarray);
    naCpyn(arrayptr, firstbyte, bytearraysize);
    arrayptr += bytearraysize;
    naNextListElement(&(space->arrays));
  }
  // Copy the contents of the last array, if any.
  remainingcount = space->usedcount % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  if(remainingcount){
    const NAByteArray* curarray = naGetListConstContent(&(space->arrays));
    const NAByte* firstbyte = naGetByteArrayConstPointer(curarray);
    naCpyn(arrayptr, firstbyte, remainingcount * space->typesize);
  }

  return array;
}



NA_DEF NAInt naGetGrowingSpaceCount(const NAGrowingSpace* space){
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
