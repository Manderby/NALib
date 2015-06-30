
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



NA_HDEF void naAddGrowingSpaceNewSpace(NAGrowingSpace* space){
  NAByteArray* newarray = naInitByteArrayWithSize(naAlloc(NAByteArray), NA_GROWING_SPACE_SINGLE_ARRAY_SIZE * space->typesize);
  if(space->constructor){
    NAByte* curptr = naGetByteArrayMutablePointer(newarray);
    NAUInt i;
    for(i=0; i<NA_GROWING_SPACE_SINGLE_ARRAY_SIZE; i++){
      space->constructor(curptr);
      curptr += space->typesize;
    }
  }
  naAddListLastMutable(&(space->arrays), newarray);
}



NA_DEF NAGrowingSpace* naInitGrowingSpace(NAGrowingSpace* space, NAUInt typesize, NAConstructor constructor){
  #ifndef NDEBUG
    if(!space)
      {naCrash("naInitGrowingSpace", "space is NULL"); return NA_NULL;}
    if(typesize <= 0)
      naError("naInitGrowingSpace", "typesize is invalid.");
  #endif
  naInitList(&(space->arrays));
  space->typesize = typesize;
  space->usedcount = 0;
  space->constructor = constructor;
  return space;
}



NA_DEF void naClearGrowingSpace(NAGrowingSpace* space, NADestructor destructor){
  NAByteArray* curarray = NA_NULL; // Declaration before Implementation.
  NAUInt arraycount;
  NAUInt i, k;
  #ifndef NDEBUG
    if(!space){
      naCrash("naClearGrowingSpace", "space is Null-Pointer.");
      return;
    }
  #endif

  naFirstList(&(space->arrays));
  arraycount = naGetListCount(&(space->arrays));
  for(i=0; i<arraycount; i++){
    NAUInt remainingcount;
    curarray = naIterateListMutable(&(space->arrays), 1);
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
    naClearByteArray(curarray);
    naFree(curarray);
  }

  naClearList(&(space->arrays), NA_NULL);
}



NA_DEF void* naNewGrowingSpaceElement(NAGrowingSpace* space){
  NAByteArray* lastarray; // Declaration before Implementation. Needed for C90
  NAUInt subindex;
  NAUInt fullcount = naGetListCount(&(space->arrays)) * NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  if(space->usedcount == fullcount){naAddGrowingSpaceNewSpace(space);}
  lastarray = naGetListLastMutable(&(space->arrays));
  subindex = space->usedcount % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
  space->usedcount++;
  return naGetByteArrayMutableByte(lastarray, subindex * space->typesize);
  
//  space->usedcount++;
//  NAUInt arraycount = naGetArrayCount(&(space->bytearrays));
//  NAUInt endindex = 1 << arraycount;
//  if(space->usedcount == endindex){
//    naAddGrowingSpaceNewSpace(space);
//    arraycount++;
//    endindex <<= 1;
//  }
//  lastarray = naGetArrayMutableElement(&(space->bytearrays), -1);
//  subindex = arraycount - (1 << (arraycount - 1));
}



NA_DEF const void* naGetGrowingSpaceConstContent(const NAGrowingSpace* space){
  const NAByteArray* curarray = naGetListCurrentConst(&(space->arrays));
  if(!curarray){return NA_NULL;}
  return naGetByteArrayConstByte(curarray, space->cur * space->typesize);
}



NA_DEF void* naGetGrowingSpaceMutableContent(NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  NAByteArray* curarray = naGetListCurrentMutable(&(mutablespace->arrays));
  if(!curarray){return NA_NULL;}
  return naGetByteArrayMutableByte(curarray, space->cur * mutablespace->typesize);
}



NA_DEF void naFirstGrowingSpaceElement(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  naFirstList(&(mutablespace->arrays));
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
    naIterateList(&(mutablespace->arrays), 1);
    mutablespace->cur = 0;
  }
}



NA_DEF NAArray* naInitArrayWithGrowingSpace(NAArray* array, NAGrowingSpace* space){
  // Declaration before Implementation. Needed for C90
  NAByte* arrayptr;
  NAUInt bytearraycount;
  NAUInt bytearraysize;
  NAUInt remainingcount;
  NAUInt i;
  array = naInitArrayWithCount(array, space->typesize, space->usedcount, NA_NULL);
  arrayptr = naGetArrayMutablePointer(array);
  bytearraycount = naGetListCount(&(space->arrays));
  if(bytearraycount){
    bytearraysize = NA_GROWING_SPACE_SINGLE_ARRAY_SIZE * space->typesize;
    naFirstList(&(space->arrays));
    // Copy all full arrays as a whole
    for(i=0; i<(bytearraycount-1); i++){
      const NAByteArray* curarray = naGetListCurrentConst(&(space->arrays));
      const NAByte* firstbyte = naGetByteArrayConstPointer(curarray);
      naCpyn(arrayptr, firstbyte, bytearraysize);
      arrayptr += bytearraysize;
      naIterateList(&(space->arrays), 1);
    }
    // Copy the contents of the last array, if any.
    remainingcount = space->usedcount % NA_GROWING_SPACE_SINGLE_ARRAY_SIZE;
    if(remainingcount){
      const NAByteArray* curarray = naGetListCurrentConst(&(space->arrays));
      const NAByte* firstbyte = naGetByteArrayConstPointer(curarray);
      naCpyn(arrayptr, firstbyte, remainingcount * space->typesize);
    }
  }

  return array;
}



NA_DEF NAUInt naGetGrowingSpaceCount(const NAGrowingSpace* space){
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
