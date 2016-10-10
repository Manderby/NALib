// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "../NAList.h"






NA_DEF NABool naLocateListPointer(const NAList* list, void* content){
  NAList* mutablelist = (NAList*)list;
  NAListElement* curelement = list->sentinel.next;
  while(curelement != &(list->sentinel)){
    if(naGetPtrConst(&(curelement->ptr)) == content){
      mutablelist->cur = curelement;
      return NA_TRUE;
    }
    curelement = curelement->next;
  }
  // Reaching here, content could not be found. Do not change the internal
  // pointer but return NA_FALSE
  return NA_FALSE;
}


NA_DEF NABool naLocateListIndex(const NAList* list, NAInt indx){
  NAList* mutablelist = (NAList*)list;
  if(indx < 0){indx += list->count;}
  if(indx < 0){
    #ifndef NDEBUG
      naError("naLocateListIndex", "Negative index underflows the range of the list");
    #endif
    mutablelist->cur = &(mutablelist->sentinel);
    return NA_FALSE;
  }
  if(indx >= (NAInt)list->count){
    #ifndef NDEBUG
      naError("naLocateListIndex", "Index overflows the range of the list");
    #endif
    mutablelist->cur = &(mutablelist->sentinel);
    return NA_FALSE;
  }
  
  if(indx < ((NAInt)list->count / 2)){
    // Go from leading to trailing
    mutablelist->cur = list->sentinel.next;
    while(indx){
      mutablelist->cur = mutablelist->cur->next;
      indx--;
    }
  }else{
    // Go from trailing to leading
    mutablelist->cur = list->sentinel.prev;
    indx = indx - list->count + 1;
    while(indx){
      mutablelist->cur = mutablelist->cur->prev;
      indx++;
    }
  }

  return NA_TRUE;
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


