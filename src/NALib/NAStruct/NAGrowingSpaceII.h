
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



// This file contains inline implementations of the file NAGrowingSpace.h
// Do not include this file directly! It will automatically be included when
// including "NAGrowingSpace.h"


#include "NAMemory.h"


struct NAGrowingSpace{
  NAList        arrays;         // List of void*
  NAUInt        typesize;       // The size in bytes of the stored type
  NAUInt        usedcount;      // The used number of elements in the storage.
  NAUInt        minimalexp;     // An exponent of 1 denotes the minimal count of 2.
  #ifndef NDEBUG
    NAInt      itercount;      // The number of iterators on this space.
  #endif
};



struct NAGrowingSpaceIterator{
  const NAGrowingSpace* space;    // The const space
  NAListIterator  listiterator;   // the iterator on the array list
  NAInt           curarrayindex;  // The index of the current array.
  NAInt           cur;            // The index in the current array if available.
};


NA_HIDEF NAUInt naGetGrowingSpaceArrayAvailableSize(const NAGrowingSpace* space, NAInt indx){
  return NA_ONE << (indx + space->minimalexp);
}

NA_HIDEF NAUInt naGetGrowingSpaceArrayBaseIndex(const NAGrowingSpace* space, NAInt indx){
  return (NA_ONE << (indx + space->minimalexp)) - (NA_ONE << space->minimalexp);
}

NA_HIDEF NAUInt naGetGrowingSpaceTotalSize(const NAGrowingSpace* space){
  return (NA_ONE << (naGetListCount(&(space->arrays)) + space->minimalexp)) - (NA_ONE << space->minimalexp);
}


// This is a helper function which has one purpose: Add another memory block
// to the space which is double the element count of the previous block. The first block
// has the element count given to naInitGrowingSpace
NA_HIDEF void naAddGrowingSpaceNewSpace(NAGrowingSpace* space){
  void* newarray;
  newarray = naMalloc(naGetGrowingSpaceArrayAvailableSize(space, naGetListCount(&(space->arrays))) * space->typesize);
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
    space->itercount = 0;
  #endif
  if(space->minimalexp == 0){space->minimalexp = 1;}
  return space;
}



NA_IDEF void naClearGrowingSpace(NAGrowingSpace* space){
  NAListIterator iter;
  #ifndef NDEBUG
    if(!space){
      naCrash("naClearGrowingSpace", "space is Null-Pointer.");
      return;
    }
    if(space->itercount != 0)
      naCrash("naClearGrowingSpace", "There are still iterators on this space. Did you forget naClearGrowingSpaceIterator?");
  #endif

  iter = naMakeListIteratorMutator(&(space->arrays));
  while(naIterateList(&iter, 1)){
    void* curarray = naGetListCurrentMutable(&iter);
    naFree(curarray);
  }
  naClearListIterator(&iter);

  naClearList(&(space->arrays));
}



NA_IDEF void* naNewGrowingSpaceElement(NAGrowingSpace* space){
  // Declaration before Implementation. Needed for C90
  NAUInt subindex;
  NAUInt availablespace;

  space->usedcount++;
  
  availablespace = naGetGrowingSpaceTotalSize(space);

  if(space->usedcount > availablespace){
    naAddGrowingSpaceNewSpace(space);
  }
  
  subindex = space->usedcount - 1 - naGetGrowingSpaceArrayBaseIndex(space, naGetListCount(&(space->arrays)) - 1);
  return &(((NAByte*)naGetListLastMutable(&(space->arrays)))[subindex * space->typesize]);
}



NA_IDEF NAUInt naGetGrowingSpaceCount(const NAGrowingSpace* space){
  return space->usedcount;
}






NA_IDEF NAGrowingSpaceIterator naMakeGrowingSpaceIteratorAccessor(const NAGrowingSpace* space){
  NAGrowingSpaceIterator iter;
  #ifndef NDEBUG
    NAGrowingSpace* mutablespace;
  #endif
  iter.space = space;
  iter.listiterator = naMakeListIteratorAccessor(&(space->arrays));
  iter.curarrayindex = -1;
  iter.cur = 0;
  #ifndef NDEBUG
    mutablespace = (NAGrowingSpace*)space;
    mutablespace->itercount++;
  #endif
  return iter;
}



NA_IDEF NAGrowingSpaceIterator naMakeGrowingSpaceIteratorMutator(const NAGrowingSpace* space){
  NAGrowingSpaceIterator iter;
  #ifndef NDEBUG
    NAGrowingSpace* mutablespace;
  #endif
  iter.space = space;
  iter.listiterator = naMakeListIteratorMutator(&(space->arrays));
  iter.curarrayindex = -1;
  iter.cur = 0;
  #ifndef NDEBUG
    mutablespace = (NAGrowingSpace*)space;
    mutablespace->itercount ++;
  #endif
  return iter;
}



NA_IDEF void naClearGrowingSpaceIterator(NAGrowingSpaceIterator* iterator){
  #ifndef NDEBUG
    NAGrowingSpace* mutablespace;
  #endif
  naClearListIterator(&(iterator->listiterator));
  #ifndef NDEBUG
    mutablespace = (NAGrowingSpace*)iterator->space;
    mutablespace->itercount --;
    if(mutablespace->itercount < 0)
      naError("naClearGrowingSpaceIterator", "Too many cleared iterators on that space.");
  #endif
}



NA_IDEF NABool naIterateGrowingSpace(NAGrowingSpaceIterator* iterator){
  iterator->cur++;
  if(iterator->cur >= (NAInt)naGetGrowingSpaceArrayAvailableSize(iterator->space, iterator->curarrayindex)){
    naIterateList(&(iterator->listiterator), 1);
    iterator->curarrayindex++;
    iterator->cur = 0;
  }
  return (naGetGrowingSpaceArrayBaseIndex(iterator->space, iterator->curarrayindex) + iterator->cur < iterator->space->usedcount);
}



NA_IDEF const void* naGetGrowingSpaceCurrentConst(NAGrowingSpaceIterator* iterator){
  const NAByte* curbase;
  if(naGetGrowingSpaceArrayBaseIndex(iterator->space, iterator->curarrayindex) + iterator->cur >= iterator->space->usedcount){return NA_NULL;}
  curbase = (const NAByte*)naGetListCurrentConst(&(iterator->listiterator));
  return &(curbase[iterator->cur * iterator->space->typesize]);
}



NA_IDEF void* naGetGrowingSpaceCurrentMutable(NAGrowingSpaceIterator* iterator){
  NAByte* curbase;
  if(naGetGrowingSpaceArrayBaseIndex(iterator->space, iterator->curarrayindex) + iterator->cur >= iterator->space->usedcount){return NA_NULL;}
  curbase = (NAByte*)naGetListCurrentMutable(&(iterator->listiterator));
  return &(curbase[iterator->cur * iterator->space->typesize]);
}




NA_IDEF void naForeachGrowingSpaceConst(const NAGrowingSpace* space, NAAccessor accessor){
  NAGrowingSpaceIterator iter = naMakeGrowingSpaceIteratorAccessor(space);
  while(naIterateGrowingSpace(&iter)){
    const void* data = naGetGrowingSpaceCurrentConst(&iter);
    accessor(data);
  }
  naClearGrowingSpaceIterator(&iter);
}



NA_IDEF void naForeachGrowingSpaceMutable(const NAGrowingSpace* space, NAMutator mutator){
  NAGrowingSpaceIterator iter = naMakeGrowingSpaceIteratorMutator(space);
  while(naIterateGrowingSpace(&iter)){
    void* data = naGetGrowingSpaceCurrentMutable(&iter);
    mutator(data);
  }
  naClearGrowingSpaceIterator(&iter);
}



NA_IDEF void naForeachGrowingSpacePointerConst(const NAGrowingSpace* space, NAAccessor accessor){
  NAGrowingSpaceIterator iter = naMakeGrowingSpaceIteratorAccessor(space);
  while(naIterateGrowingSpace(&iter)){
    const void* const * data = naGetGrowingSpaceCurrentConst(&iter);
    accessor(*data);
  }
  naClearGrowingSpaceIterator(&iter);
}



NA_IDEF void naForeachGrowingSpacePointerMutable(const NAGrowingSpace* space, NAMutator mutator){
  NAGrowingSpaceIterator iter = naMakeGrowingSpaceIteratorMutator(space);
  while(naIterateGrowingSpace(&iter)){
    void** data = naGetGrowingSpaceCurrentMutable(&iter);
    mutator(*data);
  }
  naClearGrowingSpaceIterator(&iter);
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
