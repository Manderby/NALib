
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



// This file contains inline implementations of the file NAGrowingSpace.h
// Do not include this file directly! It will automatically be included when
// including "NAGrowingSpace.h"



struct NAGrowingSpace{
  NAList        arrays;         // List of void*
  NAUInt        typesize;       // The size in bytes of the stored type
  NAUInt        usedcount;      // The used number of elements in the storage.
  NAUInt        minimalexp;     // The exponent of 2 denoting the minimal count.
  NAUInt        curarraycount;  // The element count of the current array.
  NAUInt        cur;            // The index in the current array if available.
};


// This is a helper function which has one purpose: Add another memory block
// to the space which is double the element count of the previous block. The first block
// has the element count given to naInitGrowingSpace
NA_HIDEF void naAddGrowingSpaceNewSpace(NAGrowingSpace* space){
  NAUInt newelementcount;
  void* newarray;
  NAUInt arraycount = naGetListCount(&(space->arrays));
  newelementcount = NA_ONE << (arraycount + space->minimalexp);
  newarray = naMalloc(newelementcount * space->typesize);
  naAddListLastMutable(&(space->arrays), newarray);
}



NA_IDEF NAGrowingSpace* naInitGrowingSpace(NAGrowingSpace* space, NAUInt typesize, NAUInt minimalcount){
  #ifndef NDEBUG
    if(!space)
      {naCrash("naInitGrowingSpace", "space is Null-Pointer"); return NA_NULL;}
    if(typesize <= 0)
      naError("naInitGrowingSpace", "typesize is invalid.");
    // The following test states that 0 and 1 are invalid values. They could be
    // automatically converted to 2 but they are reserved for future use.
    if(minimalcount < 2)
      naError("naInitGrowingSpace", "minimalcount must be at least 2");
  #endif
  naInitList(&(space->arrays));
  space->typesize = typesize;
  space->usedcount = 0;
  space->minimalexp = naLog2i(minimalcount);
  #ifndef NDEBUG
    if((NAUInt)naPow(2., (double)space->minimalexp) != minimalcount)  // todo make pow2i
      naError("naInitGrowingSpace", "minimalcount must be a power of 2.");
  #endif
  if(space->minimalexp == 0){space->minimalexp = 1;}
  return space;
}



NA_IDEF void naClearGrowingSpace(NAGrowingSpace* space){
  #ifndef NDEBUG
    if(!space){
      naCrash("naClearGrowingSpace", "space is Null-Pointer.");
      return;
    }
  #endif

  naRewindList(&(space->arrays));
  while(naIterateList(&(space->arrays), 1)){
    void* curarray = naGetListCurrentMutable(&(space->arrays));
    naFree(curarray);
  }

  naClearList(&(space->arrays));
}



NA_IDEF void* naNewGrowingSpaceElement(NAGrowingSpace* space){
  // Declaration before Implementation. Needed for C90
  NAUInt subindex;
  NAUInt baseindex;
  NAUInt arraycount;
  NAUInt availablespace;

  space->usedcount++;
  arraycount = naGetListCount(&(space->arrays));
  
  availablespace = (1 << (arraycount + space->minimalexp)) - (1 << space->minimalexp);

  if(space->usedcount > availablespace){
    naAddGrowingSpaceNewSpace(space);
    baseindex = (1 << (arraycount + space->minimalexp)) - (1 << space->minimalexp);
  }else{
    baseindex = (1 << (arraycount + space->minimalexp - 1)) - (1 << space->minimalexp);
  }
  
  subindex = space->usedcount - 1 - baseindex;
  return &(((NAByte*)naGetListLastMutable(&(space->arrays)))[subindex * space->typesize]);
}



NA_IDEF const void* naGetGrowingSpaceConstContent(const NAGrowingSpace* space){
  void* curarray = naGetListCurrentMutable(&(space->arrays));
  if(!curarray){return NA_NULL;}
  return &(((const NAByte*)curarray)[space->cur * space->typesize]);
}



NA_IDEF void* naGetGrowingSpaceMutableContent(const NAGrowingSpace* space){
  void* curarray;
  if(naGetListCount(&(space->arrays)) == NA_ZERO){return NA_NULL;}
  curarray = naGetListCurrentMutable(&(space->arrays));
  if(!curarray){return NA_NULL;}
  return &(((NAByte*)curarray)[space->cur * space->typesize]);
}



NA_IDEF const void* naGetGrowingSpaceArrayContent(const NAGrowingSpace* space, NAUInt* count){
  const void* curarray = naGetListCurrentConst(&(space->arrays));
  if(!curarray){return NA_NULL;}
  *count = space->curarraycount;
  return curarray;
}



NA_HIDEF void naCorrectGrowingSpaceCurArrayBytesize(NAGrowingSpace* space){
  if(naIsListAtLast(&(space->arrays))){
    NAUInt arraycount = naGetListCount(&(space->arrays));
    NAUInt baseindex;
    // This line is the reason why minimalcount shall not be 0 or 1. In that
    // case, the following << operator would have a negative input value which
    // is undefined.
    baseindex = (1 << (arraycount + space->minimalexp - 1)) - (1 << space->minimalexp);
    space->curarraycount = space->usedcount - baseindex;
  }
}


NA_HIDEF void naAdvanceGrowingSpaceToNextArray(NAGrowingSpace* space){
  #ifndef NDEBUG
    if(naIsListEmpty(&(space->arrays)))
      naError("naAdvanceGrowingSpaceToNextArray", "Growing space is empty");
  #endif
  naIterateList(&(space->arrays), 1);
  space->cur = 0;
  space->curarraycount <<= 1;
  naCorrectGrowingSpaceCurArrayBytesize(space);
}



NA_IDEF void naFirstGrowingSpace(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  naRewindList(&(mutablespace->arrays));
  mutablespace->cur = 0;
  mutablespace->curarraycount = NA_ONE << space->minimalexp;
  // Note that this is not correct if the growing space is empty. But this
  // will be captured in accessing functions by checking if there is anything
  // inside the list at all.
  naCorrectGrowingSpaceCurArrayBytesize(mutablespace);
}



NA_IDEF void naNextGrowingSpaceElement(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  const void* curarray = naGetListCurrentConst(&(space->arrays));
  if(!curarray){return;}
  
  mutablespace->cur++;
  if(mutablespace->cur == mutablespace->curarraycount){
    // We go outside the current array.
    naAdvanceGrowingSpaceToNextArray(mutablespace);
  }
}



NA_IDEF void naNextGrowingSpaceArray(const NAGrowingSpace* space){
  NAGrowingSpace* mutablespace = (NAGrowingSpace*)space;
  naAdvanceGrowingSpaceToNextArray(mutablespace);
}




NA_IDEF NAUInt naGetGrowingSpaceCount(const NAGrowingSpace* space){
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
