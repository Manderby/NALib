// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_LIST_INCLUDED
#define NA_LIST_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// Following is the implementation of a double-chained list with built-in
// iteration.
//
// This implementation uses a so called sentinel which is stored in the list
// itself. Using such and using a double-chained list, implementation of the
// list becomes very easy and very fast, as there are almost no special cases
// to consider.
//
// Additionally, a list stores a pointer to the current element which allows
// you to iterate through the list easily and even remove elements while
// traversing the list. Changing the iterator is not considered changing the
// list. Be aware!
//
// Important: You have to typecast the returned content-pointers!


#include "NASystem.h"



typedef struct NAList NAList;



// Creates an empty list.
NA_IAPI NAList* naCreateList(NAList* list);

// Creates an exact copy of originallist by replicating all elements in the
// same order.
NA_IAPI NAList* naDuplicateList(NAList* list, NAList* originallist);

// Clears or destroys the given list. Note: This will free all list elements
// but not the contents they store! You can send a destructor though which
// might clear and destroy all elements.
NA_IAPI void    naClearList  (NAList* list, NADestructor destructor);
NA_IAPI void    naDestroyList(NAList* list, NADestructor destructor);

// Empties the list. See implementation for difference to naClearList
NA_IAPI void    naEmptyList  (NAList* list, NADestructor destructor);

// Returns the number of elements in this list.
NA_IAPI NAInt   naGetListCount(const NAList* list);


// ///////////////////////////
// Adding elements
//
// Adds the given content at the desired position of the list.
// First:   New element becomes first element of the list
// Last:    New element becomes last element of the list
// Before:  New element becomes element before the current element.
// After:   New element becomes element after the current element.
// Note: If the internal pointer is not set, Before behaves the same as Last
// and After behaves the same as First.
NA_IAPI void naAddListFirstConst    (NAList* list, const void* content);
NA_IAPI void naAddListFirstMutable  (NAList* list, void* content);
NA_IAPI void naAddListLastConst     (NAList* list, const void* content);
NA_IAPI void naAddListLastMutable   (NAList* list, void* content);
NA_IAPI void naAddListBeforeConst   (NAList* list, const void* content);
NA_IAPI void naAddListBeforeMutable (NAList* list, void* content);
NA_IAPI void naAddListAfterConst    (NAList* list, const void* content);
NA_IAPI void naAddListAfterMutable  (NAList* list, void* content);


// /////////////////////////////
// Removing elements
//
// Remove functions. Removes the desired element from the list.
// The movenext parameter denotes how the internal pointer shall move in case
// the current element is the element which is removed. If movenext is NA_FALSE
// The internal pointer will be moved to the previous element. If movenext is
// NA_TRUE, the internal pointer will be moved to the next element.
// Note: Remove functions free the memory of the corresponding NAListElement but
// do not free the memory of the content! But the mutable variants return a
// pointer to the previously stored content such that you can erase it if
// you need to.
NA_IAPI void  naRemoveListFirstConst    (NAList* list, NABool movenext);
NA_IAPI void* naRemoveListFirstMutable  (NAList* list, NABool movenext);
NA_IAPI void  naRemoveListLastConst     (NAList* list, NABool movenext);
NA_IAPI void* naRemoveListLastMutable   (NAList* list, NABool movenext);
NA_IAPI void  naRemoveListPrevConst     (NAList* list);
NA_IAPI void* naRemoveListPrevMutable   (NAList* list);
NA_IAPI void  naRemoveListCurrentConst  (NAList* list, NABool movenext);
NA_IAPI void* naRemoveListCurrentMutable(NAList* list, NABool movenext);
NA_IAPI void  naRemoveListNextConst     (NAList* list);
NA_IAPI void* naRemoveListNextMutable   (NAList* list);


// /////////////////////////////////////////////
// Access functions
//
// Beware! These functions are no iteration functions. The internal pointer
// is NOT moved. Just the desired element is returned.
NA_IAPI const void* naGetListFirstConst       (const NAList* list);
NA_IAPI       void* naGetListFirstMutable     (const NAList* list);
NA_IAPI const void* naGetListLastConst        (const NAList* list);
NA_IAPI       void* naGetListLastMutable      (const NAList* list);
NA_IAPI const void* naGetListPrevConst        (const NAList* list);
NA_IAPI       void* naGetListPrevMutable      (const NAList* list);
NA_IAPI const void* naGetListCurrentConst     (const NAList* list);
NA_IAPI       void* naGetListCurrentMutable   (const NAList* list);
NA_IAPI const void* naGetListNextConst        (const NAList* list);
NA_IAPI       void* naGetListNextMutable      (const NAList* list);



// ////////////////////////////////////
// Iteration functions
//
// Every list has an internal pointer denoting the current element. If no
// current element is set, NA_NULL is returned as a content. A typical example
// of iteration is the following:
//
// naFirstList(mylist);
// while((curelement = naIterateListMutable(mylist, 1))){
//   do stuff with curelement.
// }
//
// Note: You can safely use remove functions while iterating!

// Returns the current element and then iterates the given steps forward or
// backwards by using positive or negative numbers. If step is 0, you simply
// access the current element. Note that for accessing the current element,
// the use of naGetListCurrent would be preferable.
NA_IAPI const void* naIterateListConst    (const NAList* list, NAInt step);
NA_IAPI       void* naIterateListMutable  (const NAList* list, NAInt step);

// With the following functions, you can initialize the internal pointer.
NA_IAPI void naFirstList                  (const NAList* list);
NA_IAPI void naLastList                   (const NAList* list);
// The locate-functions move the internal pointer to the element storing the
// desired content pointer or being located at the given index. If the given
// index is negative, it denotes the element from the end of the list, whereas
// -1 denotes the last element.
//
// If the element is not found or the index is not within the list range, the
// internal pointer will be unset.
// Returns NA_TRUE, if the element is found and NA_FALSE if not.
//
// Warning: These functions are very slow!
NA_IAPI NABool naLocateListPointer        (const NAList* list, void* content);
NA_IAPI NABool naLocateListIndex          (const NAList* list, NAInt indx);

// Moves the internal pointer forward or backwards.
NA_IAPI void naIterateList                (const NAList* list, NAInt step);

// Returns whether the list is at a certain position
NA_IAPI NABool naIsListAtFirst            (const NAList* list);
NA_IAPI NABool naIsListAtLast             (const NAList* list);













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

#include "NAPointer.h"


// The following struct should be opaque. Or even better: Completely invisible
// to the programmer.
typedef struct NAListElement NAListElement;

struct NAListElement{
  NALValue lvalue;  // A pointer to the stored content
  NAListElement* next;  // A pointer to the next element
  NAListElement* prev;  // A pointer to the previous element
};

struct NAList{
  NAInt count;            // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
  NAListElement* cur;     // A pointer to the current element when iterating.
};


NA_IDEF NAList* naCreateList(NAList* list){
  list = naAllocNALibStruct(list, NAList);
  list->count = 0;
  naFillLValueMutable(&(list->sentinel.lvalue), NA_NULL);
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
  list->cur           = &(list->sentinel);
  return list;
}


NA_IDEF NAList* naDuplicateList(NAList* list, NAList* originallist){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  list = naCreateList(list);
  cur = originallist->sentinel.next;
  while(cur != &(originallist->sentinel)){
    NAListElement* next = cur->next;
    if(naIsLValueConst(&(cur->lvalue))){
      naAddListLastConst(list, naGetLValueConst(&(cur->lvalue)));
    }else{
      naAddListLastMutable(list, naGetLValueMutable(&(cur->lvalue)));
    }
    cur = next;
  }
  return list;
}


NA_IDEF void naClearList(NAList* list, NADestructor destructor){
  naEmptyList(list, destructor);
}


NA_IDEF void naDestroyList(NAList* list, NADestructor destructor){
  naClearList(list, destructor);
  free(list);
}


// The difference between naEmptyList and naClearList is that Clear should
// be called with a semantic of invalidating the list whereas Empty simply
// empties the list. Due to the implementation in NALib, this makes no
// difference but should nontheless be distinguished. For other datastructures
// like NAHeap, this equality does not hold.
NA_IDEF void naEmptyList(NAList* list, NADestructor destructor){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  #ifndef NDEBUG
    if(!list){
      naCrash("naClearList", "list is Null-Pointer.");
      return;
    }
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    NAListElement* next = cur->next;
    if(destructor){destructor(naGetLValueMutable(&(cur->lvalue)));}
    free(cur);
    cur = next;
  }
}




NA_IDEF NAInt naGetListCount(const NAList* list){
  return list->count;
}


NA_IDEF void naFirstList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.next;
}


NA_IDEF void naLastList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.prev;
}




NA_IDEF void naIterateList(const NAList* list, NAInt step){
  NAList* mutablelist = (NAList*)list;
  while(step > 0){
    #ifndef NDEBUG
      if(!naGetLValueConst(&(list->cur->lvalue)))
        naError("naIterateListConst", "Iteration overflows");
    #endif
    mutablelist->cur = list->cur->next;
    step--;
  }
  while(step < 0){
    #ifndef NDEBUG
      if(!naGetLValueConst(&(list->cur->lvalue)))
        naError("naIterateListConst", "Iteration underflows");
    #endif
    mutablelist->cur = list->cur->prev;
    step++;
  }
}



NA_IDEF const void* naIterateListConst(const NAList* list, NAInt step){
  const void* returnptr;  // Declaration before implementation. C90
  if(list->cur == &(list->sentinel)){return NA_NULL;}
  returnptr = naGetLValueConst(&(list->cur->lvalue));
  if(step){naIterateList(list, step);}
  return returnptr;
}



NA_IDEF void* naIterateListMutable(const NAList* list, NAInt step){
  void* returnptr; // Declaration before implementation. C90
  if(list->cur == &(list->sentinel)){return NA_NULL;}
  returnptr = naGetLValueMutable(&(list->cur->lvalue));
  if(step){naIterateList(list, step);}
  return returnptr;
}



NA_IDEF NABool naIsListAtFirst(const NAList* list){
  return (list->sentinel.next == list->cur);
}



NA_IDEF NABool naIsListAtLast(const NAList* list){
  return (list->sentinel.prev == list->cur);
}


NA_IDEF NABool naLocateListPointer(const NAList* list, void* content){
  NAList* mutablelist = (NAList*)list;
  NAListElement* curelement = list->sentinel.next;
  while(curelement != &(list->sentinel)){
    if(naGetLValueConst(&(curelement->lvalue)) == content){
      mutablelist->cur = curelement;
      return NA_TRUE;
    }
    curelement = curelement->next;
  }
  // Reaching here, content could not be found. Do not change the internal
  // pointer but return NA_FALSE
  return NA_FALSE;
}


NA_IDEF NABool naLocateListIndex(const NAList* list, NAInt indx){
  NAList* mutablelist = (NAList*)list;
  if(indx < 0){indx += list->count;}
  if(indx < 0){
    #ifndef NDEBUG
      naError("naLocateListIndex", "Negative index underflows the range of the list");
    #endif
    mutablelist->cur = &(mutablelist->sentinel);
    return NA_FALSE;
  }
  if(indx >= list->count){
    #ifndef NDEBUG
      naError("naLocateListIndex", "Index overflows the range of the list");
    #endif
    mutablelist->cur = &(mutablelist->sentinel);
    return NA_FALSE;
  }
  
  if(indx < (list->count / 2)){
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



// These are helper functions. They should be hidden.
NA_HIDEF void naInjectConstListElement(NAList* list, NAListElement* element, const void* data){
  naFillLValueConst(&(element->lvalue), data);
  element->next->prev = element;
  element->prev->next = element;
  list->count++;
}
NA_HIDEF void naInjectMutableListElement(NAList* list, NAListElement* element, void* data){
  naFillLValueMutable(&(element->lvalue), data);
  element->next->prev = element;
  element->prev->next = element;
  list->count++;
}


NA_IDEF void naAddListFirstConst(NAList* list, const void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListFirstMutable(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListLastConst(NAList* list, const void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListLastMutable(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListBeforeConst(NAList* list, const void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListBeforeMutable(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListAfterConst(NAList* list, const void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListAfterMutable(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAlloc(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectMutableListElement(list, newelement, content);
}



// These are helper functions. They should be hidden.
NA_HIDEF void naEjectListConst(NAList* list, NAListElement* element, NABool movenext){
  if(element == &(list->sentinel)){return;}
  if(element == list->cur){list->cur = (movenext?(element->next):(element->prev));}
  element->prev->next = element->next;
  element->next->prev = element->prev;
  list->count--;
  free(element);
}
NA_HIDEF void* naEjectListMutable(NAList* list, NAListElement* element, NABool movenext){
  void* contentpointer; // Declaration before Implementation. Needed for C90
  if(element == &(list->sentinel)){return NA_NULL;}
  if(element == list->cur){list->cur = (movenext?(element->next):(element->prev));}
  element->prev->next = element->next;
  element->next->prev = element->prev;
  list->count--;
  contentpointer = naGetLValueMutable(&(element->lvalue));
  free(element);
  return contentpointer;
}


NA_IDEF void naRemoveListFirstConst(NAList* list, NABool movenext){
  naEjectListConst(list, list->sentinel.next, movenext);
}

NA_IDEF void* naRemoveListFirstMutable(NAList* list, NABool movenext){
  return naEjectListMutable(list, list->sentinel.next, movenext);
}

NA_IDEF void naRemoveListLastConst(NAList* list, NABool movenext){
  naEjectListConst(list, list->sentinel.prev, movenext);
}

NA_IDEF void* naRemoveListLastMutable(NAList* list, NABool movenext){
  return naEjectListMutable(list, list->sentinel.prev, movenext);
}

NA_IDEF void naRemoveListPrevConst(NAList* list){
  naEjectListConst(list, list->cur->prev, NA_TRUE);
}

NA_IDEF void* naRemoveListPrevMutable(NAList* list){
  return naEjectListMutable(list, list->cur->prev, NA_TRUE);
}

NA_IDEF void naRemoveListCurrentConst(NAList* list, NABool movenext){
  naEjectListConst(list, list->cur, movenext);
}

NA_IDEF void* naRemoveListCurrentMutable(NAList* list, NABool movenext){
  return naEjectListMutable(list, list->cur, movenext);
}

NA_IDEF void naRemoveListNextConst(NAList* list){
  naEjectListConst(list, list->cur->next, NA_TRUE);
}

NA_IDEF void* naRemoveListNextMutable(NAList* list){
  return naEjectListMutable(list, list->cur->next, NA_TRUE);
}






NA_IDEF const void* naGetListFirstConst(const NAList* list){
  return naGetLValueConst(&(list->sentinel.next->lvalue));
}


NA_IDEF void* naGetListFirstMutable(const NAList* list){
  return naGetLValueMutable(&(list->sentinel.next->lvalue));
}


NA_IDEF const void* naGetListLastConst(const NAList* list){
  return naGetLValueConst(&(list->sentinel.prev->lvalue));
}


NA_IDEF void* naGetListLastMutable(const NAList* list){
  return naGetLValueMutable(&(list->sentinel.prev->lvalue));
}


NA_IDEF const void* naGetListPrevConst(const NAList* list){
  return naGetLValueConst(&(list->cur->prev->lvalue));
}


NA_IDEF void* naGetListPrevMutable(const NAList* list){
  return naGetLValueMutable(&(list->cur->prev->lvalue));
}


NA_IDEF const void* naGetListCurrentConst(const NAList* list){
  return naGetLValueConst(&(list->cur->lvalue));
}


NA_IDEF void* naGetListCurrentMutable(const NAList* list){
  return naGetLValueMutable(&(list->cur->lvalue));
}


NA_IDEF const void* naGetListNextConst(const NAList* list){
  return naGetLValueConst(&(list->cur->next->lvalue));
}


NA_IDEF void* naGetListNextMutable(const NAList* list){
  return naGetLValueMutable(&(list->cur->next->lvalue));
}





#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_LIST_INCLUDED


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


