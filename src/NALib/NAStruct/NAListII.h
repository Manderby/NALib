// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAList.h
// Do not include this file directly! It will automatically be included when
// including "NAList.h"



#include "NAMemory.h"


// The following struct should be opaque. Or even better: Completely invisible
// to the programmer.
typedef struct NAListElement NAListElement;


// Note about NAListPos
// The NAListPos is an opaque type for the NAListElement struct.
// With this trick, the real type remains hidden to the user and the API is
// clean and easy to understand.

struct NAListElement{
  NAPtr ptr;            // A pointer to the stored content
  NAListElement* next;  // A pointer to the next element
  NAListElement* prev;  // A pointer to the previous element
  #ifndef NDEBUG
    NAList* list;       // A list pointer just for debugging.
  #endif
};


NA_RUNTIME_TYPE(NAListElement, NA_NULL);



struct NAList{
  NAUInt count;           // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
  NAListElement* cur;     // A pointer to the current element when iterating.
};



NA_IDEF NAList* naInitList(NAList* list){
  #ifndef NDEBUG
    if(!list)
      {naCrash("naInitList", "list is NULL"); return NA_NULL;}
  #endif
  list->count = 0;
  list->sentinel.ptr  = naMakeNullPtr();
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
  list->cur           = &(list->sentinel);
  return list;
}



NA_IDEF NAList* naCopyList(NAList* list, NAList* originallist){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  #ifndef NDEBUG
    if(list == originallist)
      naError("naCopyList", "the two lists are the same which they should not.");
  #endif
  list = naInitList(list);
  cur = originallist->sentinel.next;
  while(cur != &(originallist->sentinel)){
    // Note that the following if will be optimized out when NDEBUG is defined.
    if(naIsPtrConst(&(cur->ptr))){
      naAddListLastConst(list, naGetPtrConst(&(cur->ptr)));
    }else{
      naAddListLastMutable(list, naGetPtrMutable(&(cur->ptr)));
    }
    cur = cur->next;
  }
  return list;
}



NA_IDEF void naClearList(NAList* list){
  #ifndef NDEBUG
    if(!list)
      {naCrash("naClearList", "list is Null-Pointer."); return;}
  #endif
  naEmptyList(list);
}



NA_IDEF void naEmptyList(NAList* list){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  #ifndef NDEBUG
    if(!list)
      {naCrash("naEmptyList", "list is Null-Pointer."); return;}
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    NAListElement* next = cur->next;
    naDelete(cur);
    cur = next;
  }
}



NA_IDEF NAUInt naGetListCount(const NAList* list){
  return list->count;
}



NA_IDEF NABool naIsListEmpty(const NAList* list){
  return (list->count == NA_ZERO);
}



////////////////////////////
// ADDING ELEMENTS

// These are helper functions. They should be hidden.
NA_HIDEF void naInjectListElement(NAList* list, NAListElement* element){
  element->next->prev = element;
  element->prev->next = element;
  #ifndef NDEBUG
    element->list = list;
  #endif
  list->count++;
}
NA_HIDEF void naInjectConstListElement(NAList* list, NAListElement* element, const void* data){
  element->ptr = naMakePtrWithConstBuffer(data, NA_ZERO, NA_ZERO);
  naInjectListElement(list, element);
}
NA_HIDEF void naInjectMutableListElement(NAList* list, NAListElement* element, void* data){
  element->ptr = naMakePtrWithMutableBuffer(data, NA_ZERO, NA_ZERO, NA_MEMORY_CLEANUP_NONE);
  naInjectListElement(list, element);
}



NA_IDEF NAListPos naAddListFirstConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListFirstMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListLastConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListLastMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListBeforeConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListBeforeMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListAfterConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListAfterMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}



////////////////////////////
// REMOVING ELEMENTS

// These are helper functions. They should be hidden.
NA_HIDEF void naEjectList(NAList* list, NAListElement* element, NABool advance, NABool deleteelement){
  if(element == list->cur){list->cur = (advance?(element->next):(element->prev));}
  element->prev->next = element->next;
  element->next->prev = element->prev;
  #ifndef NDEBUG
    element->list = NA_NULL;  // This will capture some errors.
  #endif
  list->count--;
  if(deleteelement){naDelete(element);}
}
NA_HIDEF void naEjectListConst(NAList* list, NAListElement* element, NABool advance, NABool deleteelement){
  #ifndef NDEBUG
    if(element == &(list->sentinel))
      naError("naEjectListConst", "Trying to eject sentinel element. This should not happen.");
  #endif
  naEjectList(list, element, advance, deleteelement);
}
#include <stdio.h>
NA_HIDEF void* naEjectListMutable(NAList* list, NAListElement* element, NABool advance, NABool deleteelement){
  void* contentpointer; // Declaration before Implementation. Needed for C90
  #ifndef NDEBUG
    if(element == &(list->sentinel))
      naError("naEjectListMutable", "Trying to eject sentinel element. This should not happen.");
  #endif
  contentpointer = naGetPtrMutable(&(element->ptr));
  naEjectList(list, element, advance, deleteelement);
  return contentpointer;
}



NA_IDEF void naRemoveListFirstConst(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListFirstConst", "List is empty");
  #endif
  naEjectListConst(list, list->sentinel.next, advance, NA_TRUE);
}


NA_IDEF void* naRemoveListFirstMutable(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListFirstMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.next, advance, NA_TRUE);
}


NA_IDEF void naRemoveListLastConst(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListLastConst", "List is empty");
  #endif
  naEjectListConst(list, list->sentinel.prev, advance, NA_TRUE);
}


NA_IDEF void* naRemoveListLastMutable(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListLastMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.prev, advance, NA_TRUE);
}


NA_IDEF void naRemoveListPrevConst(NAList* list){
  #ifndef NDEBUG
    if(list->cur == list->sentinel.next)
      naError("naRemoveListPrevConst", "There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListPrevConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur->prev, NA_TRUE, NA_TRUE);
}


NA_IDEF void* naRemoveListPrevMutable(NAList* list){
  #ifndef NDEBUG
    if(list->cur == list->sentinel.next)
      naError("naRemoveListPrevMutable", "There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListPrevMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur->prev, NA_TRUE, NA_TRUE);
}


NA_IDEF void naRemoveListCurrentConst(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentConst", "List is empty");
    if(list->cur == &(list->sentinel))
      naError("naRemoveListCurrentConst", "No current internal pointer is set. Major memory corruption expected...");
  #endif
  naEjectListConst(list, list->cur, advance, NA_TRUE);
}


NA_IDEF void* naRemoveListCurrentMutable(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentMutable", "List is empty");
    if(list->cur == &(list->sentinel))
      naError("naRemoveListCurrentMutable", "No current internal pointer is set. Major memory corruption expected...");
  #endif
  return naEjectListMutable(list, list->cur, advance, NA_TRUE);
}


NA_IDEF void naRemoveListNextConst(NAList* list){
  #ifndef NDEBUG
    if(list->cur == list->sentinel.prev)
      naError("naRemoveListNextConst", "There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListNextConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur->next, NA_TRUE, NA_TRUE);
}


NA_IDEF void* naRemoveListNextMutable(NAList* list){
  #ifndef NDEBUG
    if(list->cur == list->sentinel.prev)
      naError("naRemoveListNextMutable", "There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur->next, NA_TRUE, NA_TRUE);
}



// ///////////////////
// ACCESSING ELEMENTS

NA_IDEF const void* naGetListFirstConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrConst(&(list->sentinel.next->ptr));
}


NA_IDEF void* naGetListFirstMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->sentinel.next->ptr));
}


NA_IDEF const void* naGetListLastConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrConst(&(list->sentinel.prev->ptr));
}


NA_IDEF void* naGetListLastMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->sentinel.prev->ptr));
}


NA_IDEF const void* naGetListPrevConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrConst(&(list->cur->prev->ptr));
}


NA_IDEF void* naGetListPrevMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->cur->prev->ptr));
}


NA_IDEF const void* naGetListCurrentConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrConst(&(list->cur->ptr));
}


NA_IDEF void* naGetListCurrentMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->cur->ptr));
}


NA_IDEF const void* naGetListNextConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrConst(&(list->cur->next->ptr));
}


NA_IDEF void* naGetListNextMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->cur->next->ptr));
}


// Note that the list would not be necessary when NDEBUG is undefined. But we
// force the programmer to be specific what list he wants to access so we can
// so some consistency checks if this truely what he wants.
NA_IDEF const void* naGetListPositionConst(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(!pos){
      naCrash("naGetListPositionConst", "pos is a Null-Pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListPositionConst", "List is empty");
    if(element->list != list)
      naError("naGetListPositionConst", "The given position is not part of the given list");
  #else
    NA_UNUSED(list);
    if(!pos){return NA_NULL;} // security return. Only needed for code sanity checks. todo
  #endif
  return naGetPtrConst(&(element->ptr));
}



NA_IDEF void* naGetListPositionMutable(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(!pos){
      naCrash("naGetListPositionMutable", "pos is a Null-Pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListPositionMutable", "List is empty");
    if(element->list != list)
      naError("naGetListPositionMutable", "The given position is not part of the given list");
  #else
    NA_UNUSED(list);
    if(!pos){return NA_NULL;} // security return. Only needed for code sanity checks. todo
  #endif
  return naGetPtrMutable(&(element->ptr));
}




// ////////////////////////////
// GETTING POSITION INFORMATION

NA_IDEF NABool naIsListAtFirst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naIsListAtFirst", "List is empty");
  #endif
  return (list->sentinel.next == list->cur);
}



NA_IDEF NABool naIsListAtLast(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naIsListAtLast", "List is empty");
  #endif
  return (list->sentinel.prev == list->cur);
}



NA_IDEF NABool naIsListAtPosition(const NAList* list, NAListPos pos){
  #ifndef NDEBUG
    NAListElement* element = (NAListElement*)pos;
    if(list->count == 0)
      naError("naIsListAtLast", "List is empty");
    if(element->list != list)
      naError("naIsListAtPosition", "The given position is not part of the given list");
  #endif
  return (list->cur == pos);
}



NA_IDEF NAListPos naGetListCurrentPosition(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListNextPosition", "List is empty");
  #endif
  if(list->cur == &(list->sentinel)){
    return NA_NULL;
  }else{
    return list->cur;
  }
}



NA_IDEF NAListPos naGetListNextPosition(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(!pos){
      naCrash("naGetListNextPosition", "pos is a Null-Pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListNextPosition", "List is empty");
    if(element->list != list)
      naError("naGetListNextPosition", "The given position is not part of the given list");
  #endif
//  if(!pos){return NA_NULL;} // security return. Only needed for code sanity checks.
  if(element->next == &(list->sentinel)){
    return NA_NULL;
  }else{
    return (NAListPos)(element->next);
  }
}



NA_IDEF NAListPos naGetListPrevPosition(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(!pos){
      naCrash("naGetListPrevPosition", "pos is a Null-Pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListPrevPosition", "List is empty");
    if(element->list != list)
      naError("naGetListPrevPosition", "The given position is not part of the given list");
  #endif
  if(element->prev == &(list->sentinel)){
    return NA_NULL;
  }else{
    return (NAListPos)(element->prev);
  }
}





// //////////////////////
// ITERATION FUNCTIONS

NA_IDEF void naForeachList(NAList* list, NAFunc mutator){
  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!list){
      naCrash("naForeachList", "list is Null-Pointer.");
      return;
    }
    if(!mutator || (mutator == NA_NULLFUNC))
      naError("naForeachList", "mutator is Null-Pointer.");
  #endif
  list->cur = list->sentinel.next;
  while(list->cur != &(list->sentinel)){
    NAListElement* next = list->cur->next;
    mutator(naGetPtrMutable(&(list->cur->ptr)));
    list->cur = next;
  }
}



NA_IDEF NABool naIterateList(const NAList* list, NAInt step){
  NAList* mutablelist = (NAList*)list;
  while(step > NA_ZERO){
    mutablelist->cur = list->cur->next;
    step--;
    if((mutablelist->cur == &(mutablelist->sentinel)) && (step != NA_ZERO)){
      #ifndef NDEBUG
        naError("naIterateList", "The iteration overflows the number of elements. Stopping iteration.");
      #endif
      step = NA_ZERO;
    }
  }
  while(step < NA_ZERO){
    mutablelist->cur = list->cur->prev;
    step++;
    if((mutablelist->cur == &(mutablelist->sentinel)) && (step != NA_ZERO)){
      #ifndef NDEBUG
        naError("naIterateList", "The iteration underflows the number of elements. Stopping iteration.");
      #endif
      step = NA_ZERO;
    }
  }
  return (mutablelist->cur != &(mutablelist->sentinel));
}



NA_IDEF void naRewindList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = &(mutablelist->sentinel);
}



NA_IDEF void naPositionList(const NAList* list, NAListPos listpos){
  NAList* mutablelist = (NAList*)list;
  NAListElement* element = (NAListElement*)listpos;
  #ifndef NDEBUG
    if(!listpos)
      {naCrash("naPositionList", "The element given is a Null Pointer."); return;}
    if(naIsListEmpty(list))
      naError("naPositionList", "The list is empty.");
    if(element->list != list)
      naError("naPositionList", "The element at the given pos is not part of the given list.");
  #endif
  mutablelist->cur = element;
}





// //////////////////////
// REORDERING ELEMENTS


// Helper Function for injection of existing list elements.
NA_HIDEF void naInjectExistingListElement(NAList* list, NAListElement* element){
  element->next->prev = element;
  element->prev->next = element;
  #ifndef NDEBUG
    element->list = list;
  #endif
  list->count++;
}


// When transfering an element from one list to another, you basically have to
// locate it within the source list, remove it, adjust the internal pointer of
// the souce list if necessary and re-add it to the dstlist. This a very costly
// process as there is a delete and new call for the NAListElement.
//
// To simplify the whole transfer process, these move functions keep the
// existing NAListElement and simply adjust the pointers. To not overcomplicate
// the API, only the current element can be transferred. To locate it, you can
// use the NAListPos you get when adding elements to a list or any of the
// iteration functions.
//
// Note a detail: The NAListPos is defined const, but we take the liberty to
// use it as mutable. Bad coding style, but who cares when the type is opaque.
NA_IDEF void naMoveListCurToFirst(NAList* src, NABool advance, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(src->cur == &(src->sentinel))
      naError("naMoveListCurToFirst", "List has no current element set.");
  #endif
  element = src->cur;
  naEjectListConst(src, element, advance, NA_FALSE);
  element->next = dst->sentinel.next;
  element->prev = &(dst->sentinel);
  naInjectExistingListElement(dst, element);
}


NA_IDEF void naMoveListCurToLast(NAList* src, NABool advance, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(src->cur == &(src->sentinel))
      naError("naMoveCurToListLast", "List has no current element set.");
  #endif
  element = src->cur;
  naEjectListConst(src, element, advance, NA_FALSE);
  element->next = &(dst->sentinel);
  element->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, element);
}


NA_IDEF void naMoveListCurToBefore(NAList* src, NABool advance, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(src->cur == &(src->sentinel))
      naError("naMoveListCurToBefore", "List has no current element set.");
  #endif
  element = src->cur;
  naEjectListConst(src, element, advance, NA_FALSE);
  element->next = dst->cur;
  element->prev = dst->cur->prev;
  naInjectExistingListElement(dst, element);
}


NA_IDEF void naMoveListCurToAfter(NAList* src, NABool advance, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(src->cur == &(src->sentinel))
      naError("naMoveListCurToAfter", "List has no current element set.");
  #endif
  element = src->cur;
  naEjectListConst(src, element, advance, NA_FALSE);
  element->next = dst->cur->next;
  element->prev = dst->cur;
  naInjectExistingListElement(dst, element);
}


NA_IDEF void naMoveListFirstToLast(NAList* src, NABool advance, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(naIsListEmpty(src))
      naError("naMoveListFirstToLast", "Src List has no first element because it is empty.");
  #endif
  element = src->sentinel.next;
  naEjectListConst(src, element, advance, NA_FALSE);
  element->next = &(dst->sentinel);
  element->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, element);
}



NA_IDEF void naExchangeListParts(NAList* list){
  NAListElement* first;
  NAListElement* prev;
  #ifndef NDEBUG
    if(list->cur == &(list->sentinel))
      naError("naExchangeListParts", "List has no current element set.");
  #endif
  if(&(list->sentinel) == list->cur){return;}
  if(list->sentinel.next == list->cur){return;}
  first = list->sentinel.next;
  prev = list->cur->prev;
  first->prev = list->sentinel.prev;
  list->sentinel.prev->next = first;
  list->sentinel.next = list->cur;
  list->cur->prev = &(list->sentinel);
  list->sentinel.prev = prev;
  prev->next = &(list->sentinel);
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


