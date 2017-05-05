
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



// This file contains inline implementations of the file NAStack.h
// Do not include this file directly! It will automatically be included when
// including "NAStack.h"


#include "../NAMemory.h"


struct NAStack{
  NAList          arrays;        // List of void*
  NAListIterator  curpos;        // The list position of the current array.
  NAInt           curindex;      // The index of the current array in the list.
  NAInt           typesize;      // The size in bytes of the stored type
  NAInt           usedcount;     // The used number of elements in the storage.
  NAInt           minimalexp;    // An exp of 1 denotes the minimal count of 2.
  #ifndef NDEBUG
    NAInt         itercount;     // The number of iterators on this stack.
  #endif
};



struct NAStackIterator{
  const NAStack*  stack;         // The const stack
  NAListIterator  listiterator;  // the iterator on the array list
  NAInt           curarrayindex; // The index of the current array.
  NAInt           cur;           // The index in the current array if available.
};


// Returns the number of elements which can be stored in the array with the
// given index.
NA_HIDEF NAInt naGetStackArrayAvailableCount(const NAStack* stack, NAInt indx){
  return (NAInt)1 << (indx + stack->minimalexp);
}

// Returns the element index of the first element stored in the array with the
// given index.
NA_HIDEF NAInt naGetStackArrayBaseIndex(const NAStack* stack, NAInt indx){
  return ((NAInt)1 << (indx + stack->minimalexp)) - ((NAInt)1 << stack->minimalexp);
}


// Returns the total amount of elements which can be stored with all arrays
// up including the one with the given index.
NA_HIDEF NAInt naGetStackTotalCount(const NAStack* stack, NAInt indx){
  return naGetStackArrayBaseIndex(stack, indx + 1);
}


// This is a helper function which has one purpose: Add another memory block
// to the stack which is double the element count of the previous block. The first block
// has the element count given to naInitStack
NA_HIDEF void naAddStackNewSpace(NAStack* stack){
  void* newarray;
  newarray = naMalloc(naGetStackArrayAvailableCount(stack, naGetListCount(&(stack->arrays))) * stack->typesize);
  naAddListLastMutable(&(stack->arrays), newarray);
}



NA_IDEF NAStack* naInitStack(NAStack* stack, NAInt typesize, NAInt minimalcount){
  #ifndef NDEBUG
    if(!stack)
      {naCrash("naInitStack", "stack is Null-Pointer"); return NA_NULL;}
    if(typesize <= 0)
      naError("naInitStack", "typesize is invalid.");
    // The following test states that 0 and 1 are invalid values. They could be
    // automatically converted to 2 but they are reserved for future use.
    if(minimalcount < 2)
      naError("naInitStack", "minimalcount must be at least 2");
  #endif
  naInitList(&(stack->arrays));
  stack->curpos = naMakeListModifier(&(stack->arrays));
  stack->curindex = -1;
  stack->typesize = typesize;
  stack->usedcount = 0;
  stack->minimalexp = naLog2i(minimalcount);
  #ifndef NDEBUG
    stack->itercount = 0;
  #endif
  if(stack->minimalexp == 0){stack->minimalexp = 1;}
  naAddStackNewSpace(stack);
  return stack;
}



NA_IDEF void naClearStack(NAStack* stack){

  #ifndef NDEBUG
    if(!stack){
      naCrash("naClearStack", "stack is Null-Pointer.");
      return;
    }
    if(stack->itercount != 0)
      naCrash("naClearStack", "There are still iterators on this stack. Did you forget naClearStackIterator?");
  #endif

  
  naClearListIterator(&(stack->curpos));
  stack->curpos = naMakeListMutator(&(stack->arrays));
  while(naIterateList(&(stack->curpos), 1)){
    void* curarray = naGetListCurrentMutable(&(stack->curpos));
    naFree(curarray);
  }
  naClearListIterator(&(stack->curpos));

  naClearList(&(stack->arrays));
}



NA_IDEF void* naPushStack(NAStack* stack){
  NAInt availablestack;

  stack->usedcount++;
  
  availablestack = naGetStackTotalCount(stack, stack->curindex);

  if(stack->usedcount > availablestack){
    if(naGetListCount(&(stack->arrays)) <= (stack->curindex + 1)){
      naAddStackNewSpace(stack);
    }
    naIterateList(&(stack->curpos), 1);
    stack->curindex++;
  }
  
  return naTopStack(stack);
}



NA_IDEF void* naTopStack(NAStack* stack){
  NAInt subindex;
  NAByte* array;
  subindex = stack->usedcount - naGetStackArrayBaseIndex(stack, stack->curindex) - 1;
  
  array = (NAByte*)naGetListCurrentMutable(&(stack->curpos));
  return &(array[subindex * stack->typesize]);
}



NA_IDEF void* naPopStack(NAStack* stack){
  NAInt baseindex;
  void* retvalue = naTopStack(stack);

  stack->usedcount--;
  
  baseindex = naGetStackArrayBaseIndex(stack, stack->curindex);
  if((stack->usedcount - 1) < baseindex){
    naIterateList(&(stack->curpos), -1);
    stack->curindex--;
  }

  return retvalue;
}



NA_IDEF NAInt naGetStackCount(const NAStack* stack){
  return stack->usedcount;
}



// Returns the total amount of elements which can be stored with all arrays
// up including the one with the given index.
NA_IDEF NAInt naGetStackReservedCount(const NAStack* stack){
  // Note that naGetStackTotalCount expects an index, not a count!
  return naGetStackTotalCount(stack, naGetListCount(&(stack->arrays)) - 1);
}



NA_IDEF void naShrinkStackIfNecessary(NAStack* stack, NABool aggressive){
  NAInt desiredcount;
  NAInt listcount;
  desiredcount = (aggressive) ? (stack->usedcount * 2) : (stack->usedcount * 4);
  listcount = naGetListCount(&(stack->arrays));
  while((listcount > 1) && (desiredcount < naGetStackTotalCount(stack, naGetListCount(&(stack->arrays)) - 1))){
    void* removed = naRemoveListLastMutable(&(stack->arrays));
    naFree(removed);
    listcount--;
  }
}



NA_IDEF NAStackIterator naMakeStackAccessor(const NAStack* stack){
  NAStackIterator iter;
  #ifndef NDEBUG
    NAStack* mutablestack;
  #endif
  iter.stack = stack;
  iter.listiterator = naMakeListAccessor(&(stack->arrays));
  iter.curarrayindex = -1;
  iter.cur = 0;
  #ifndef NDEBUG
    mutablestack = (NAStack*)stack;
    mutablestack->itercount++;
  #endif
  return iter;
}



NA_IDEF NAStackIterator naMakeStackMutator(const NAStack* stack){
  NAStackIterator iter;
  #ifndef NDEBUG
    NAStack* mutablestack;
  #endif
  iter.stack = stack;
  iter.listiterator = naMakeListMutator(&(stack->arrays));
  iter.curarrayindex = -1;
  iter.cur = 0;
  #ifndef NDEBUG
    mutablestack = (NAStack*)stack;
    mutablestack->itercount ++;
  #endif
  return iter;
}



NA_IDEF void naClearStackIterator(NAStackIterator* iterator){
  #ifndef NDEBUG
    NAStack* mutablestack;
  #endif
  naClearListIterator(&(iterator->listiterator));
  #ifndef NDEBUG
    mutablestack = (NAStack*)iterator->stack;
    mutablestack->itercount --;
    if(mutablestack->itercount < 0)
      naError("naClearStackIterator", "Too many cleared iterators on that stack.");
  #endif
}



NA_IDEF NABool naIterateStack(NAStackIterator* iterator){
  iterator->cur++;
  if((iterator->curarrayindex == -1) || (iterator->cur >= (NAInt)naGetStackArrayAvailableCount(iterator->stack, iterator->curarrayindex))){
    naIterateList(&(iterator->listiterator), 1);
    iterator->curarrayindex++;
    iterator->cur = 0;
  }
  return (naGetStackArrayBaseIndex(iterator->stack, iterator->curarrayindex) + iterator->cur < iterator->stack->usedcount);
}



NA_IDEF const void* naGetStackCurrentConst(NAStackIterator* iterator){
  const NAByte* curbase;
  if(naGetStackArrayBaseIndex(iterator->stack, iterator->curarrayindex) + iterator->cur >= iterator->stack->usedcount){return NA_NULL;}
  curbase = (const NAByte*)naGetListCurrentConst(&(iterator->listiterator));
  return &(curbase[iterator->cur * iterator->stack->typesize]);
}



NA_IDEF void* naGetStackCurrentMutable(NAStackIterator* iterator){
  NAByte* curbase;
  if(naGetStackArrayBaseIndex(iterator->stack, iterator->curarrayindex) + iterator->cur >= iterator->stack->usedcount){return NA_NULL;}
  curbase = (NAByte*)naGetListCurrentMutable(&(iterator->listiterator));
  return &(curbase[iterator->cur * iterator->stack->typesize]);
}



NA_IDEF void naForeachStackConst(const NAStack* stack, NAAccessor accessor){
  NAStackIterator iter = naMakeStackAccessor(stack);
  while(naIterateStack(&iter)){
    const void* data = naGetStackCurrentConst(&iter);
    accessor(data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackMutable(const NAStack* stack, NAMutator mutator){
  NAStackIterator iter = naMakeStackMutator(stack);
  while(naIterateStack(&iter)){
    void* data = naGetStackCurrentMutable(&iter);
    mutator(data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackpConst(const NAStack* stack, NAAccessor accessor){
  NAStackIterator iter = naMakeStackAccessor(stack);
  while(naIterateStack(&iter)){
    const void* const* data = (const void* const*)naGetStackCurrentConst(&iter);
    accessor(*data);
  }
  naClearStackIterator(&iter);
}



NA_IDEF void naForeachStackpMutable(const NAStack* stack, NAMutator mutator){
  NAStackIterator iter = naMakeStackMutator(stack);
  while(naIterateStack(&iter)){
    void** data = (void**)naGetStackCurrentMutable(&iter);
    mutator(*data);
  }
  naClearStackIterator(&iter);
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
