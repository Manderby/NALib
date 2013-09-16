
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAGrowingSpace.h"
#include "NAMathOperators.h"
#include <stdlib.h>



NAGrowingSpace* naCreateGrowingSpace(NAGrowingSpace* space, NAInt typesize, NAInt initialcount){
  space = naAllocateIfNull(space, sizeof(NAGrowingSpace));
  naCreateArrayWithCount(&(space->array), typesize, naExp2i(initialcount));
  space->usedcount = initialcount;
  return space;
}



void* naAddGrowingSpaceElement(NAGrowingSpace* space, void* elemptr){
  NAInt arraycount = naGetArrayCount(&(space->array));
  if((space->usedcount + 1) > arraycount){
    NAArray newarray;
    naCreateArrayWithCount(&newarray, space->array.typesize, arraycount << 1);
    naCpyn(naGetByteArrayMutablePointer(&(newarray.bytearray)), naGetByteArrayMutablePointer(&(space->array.bytearray)), space->usedcount * space->array.typesize);
    naClearArray(&(space->array));
    space->array = newarray;
  }
  if(elemptr){
    naCpyn(naGetArrayMutableElement(&(space->array), space->usedcount), elemptr, space->array.typesize);
  }
  space->usedcount++;
  return naGetArrayMutableElement(&(space->array), space->usedcount - 1);
}



NAArray* naCreateArrayOutOfGrowingSpace(NAArray* array, NAGrowingSpace* space){
  array = naAllocateIfNull(array, sizeof(NAArray));
  naCreateArrayWithCount(array, space->array.typesize, space->usedcount);
  naCpyn(naGetByteArrayMutablePointer(&(array->bytearray)), naGetByteArrayMutablePointer(&(space->array.bytearray)), space->usedcount * space->array.typesize);
  return array;
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
