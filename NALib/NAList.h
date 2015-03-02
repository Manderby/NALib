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
// list becomes very, very easy, as there are almost no special cases to
// consider.
//
// Additionally, a list stores a pointer to the current element which allows
// you to iterate through the list easily and even remove elements while
// traversing the list. Changing the iterator is not considered changing the
// list. Be aware!
//
// Important: You have to typecast the returned content-pointers!


#include "NASystem.h"


// The following struct should be opaque. Or even better: Completely invisible
// to the programmer.
typedef struct NAListElement NAListElement;
struct NAListElement{
  void* content;        // A pointer to the stored content
  NAListElement* next;  // A pointer to the next element
  NAListElement* prev;  // A pointer to the previous element
};



typedef struct NAList NAList;
struct NAList{
  NAInt count;            // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
  NAListElement* cur;     // A pointer to the current element when iterating.
};



// Creates an empty list.
NA_IAPI NAList* naCreateList(NAList* list);

// Creates an exact copy of originallist by replicating all elements in the
// same order.
NA_IAPI NAList* naDuplicateList(NAList* list, NAList* originallist);

// Clears or destroys the given list. Note: This will free all list elements
// but not the contents they store!
NA_IAPI void naClearList  (NAList* list, NADestructor destructor);
NA_IAPI void naDestroyList(NAList* list, NADestructor destructor);

// Empties the list. See implementation for difference to naClearList
NA_IAPI void naEmptyList  (NAList* list, NADestructor destructor);

// Returns the number of elements in this list.
NA_IAPI NAInt naGetListCount(const NAList* list);


// Iteration functions
//
// Every list has an internal pointer denoting the current element. You can
// directly access the content of that element. If no current element is
// set, NA_NULL is returned.
NA_IAPI const void* naGetListConstContent  (const NAList* list);
NA_IAPI       void* naGetListMutableContent(      NAList* list);

// The following functions move the internal pointer. At start, the internal
// pointer is not set. Reaching the head or tail of the list, the internal
// pointer will be unset.
NA_IAPI void naFirstListElement   (const NAList* list);
NA_IAPI void naLastListElement    (const NAList* list);
NA_IAPI void naNextListElement    (const NAList* list);
NA_IAPI void naPrevListElement    (const NAList* list);
// Moves the internal pointer to the element storing the desired content.
// Returns true, if found and false if not. If not found, the internal pointer
// will not be changed.
NA_IAPI NABool naLocateListElement(const NAList* list, void* content);
// Note: You can safely use remove functions while iterating!


// Adds the given content at the desired position of the list.
// First:   New element becomes first element of the list
// Last:    New element becomes last element of the list
// Before:  New element becomes element before the current element.
// After:   New element becomes element after the current element.
// Note: If the internal pointer is not set, Before behaves the same as Last
// and After behaves the same as First.
NA_IAPI void naAddListElementFirst  (NAList* list, void* content);
NA_IAPI void naAddListElementLast   (NAList* list, void* content);
NA_IAPI void naAddListElementBefore (NAList* list, void* content);
NA_IAPI void naAddListElementAfter  (NAList* list, void* content);


// Remove functions. Removes the desired element from the list.
// The movenext parameter denotes how the internal pointer shall move in case
// the current element is the element which is removed. If movenext is NA_FALSE
// The internal pointer will be moved to the previous element. If movenext is
// NA_TRUE, the internal pointer will be moved to the next element.
// Note: Remove functions free the memory of the corresponding NAListElement but
// do not free the memory of the content! A pointer to the previously stored
// content is returned.
NA_IAPI void* naRemoveListElementFirst   (NAList* list, NABool movenext);
NA_IAPI void* naRemoveListElementLast    (NAList* list, NABool movenext);
NA_IAPI void* naRemoveListElementPrev    (NAList* list);
NA_IAPI void* naRemoveListElementCurrent (NAList* list, NABool movenext);
NA_IAPI void* naRemoveListElementNext    (NAList* list);















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

#include "NAPointer.h"


NA_IDEF NAList* naCreateList(NAList* list){
  list = (NAList*)naAllocateIfNull(list, sizeof(NAList));
  list->count = 0;
  list->sentinel.content = NA_NULL;
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
    naAddListElementLast(list, cur->content);
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
    }
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    NAListElement* next = cur->next;
    if(destructor){destructor(cur->content);}
    free(cur);
    cur = next;
  }
}




NA_IDEF NAInt naGetListCount(const NAList* list){
  return list->count;
}


NA_IDEF const void* naGetListConstContent(const NAList* list){
  return list->cur->content;
}

NA_IDEF void* naGetListMutableContent(NAList* list){
  return list->cur->content;
}


NA_IDEF void naFirstListElement(const NAList* list){
  NAList* mutablelist = (NAList*)list;

  mutablelist->cur = list->sentinel.next;
}


NA_IDEF void naLastListElement(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.prev;
}


NA_IDEF void naNextListElement(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->cur->next;
}


NA_IDEF void naPrevListElement(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->cur->prev;
}


NA_IDEF NABool naLocateListElement(const NAList* list, void* content){
  NAList* mutablelist = (NAList*)list;
  NAListElement* curelement = list->sentinel.next;
  while(curelement != &(list->sentinel)){
    if(curelement->content == content){
      mutablelist->cur = curelement;
      return NA_TRUE;
    }
    curelement = curelement->next;
  }
  // Reaching here, content could not be found. Do not change the internal
  // pointer but return NA_FALSE
  return NA_FALSE;
}



// This is a helper function. It should be hidden.
NA_IHLP void naInjectListElement(NAList* list, NAListElement* element, void* content){
  element->content = content;
  element->next->prev = element;
  element->prev->next = element;
  list->count++;
}


NA_IDEF void naAddListElementFirst(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAllocate(sizeof(NAListElement));
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectListElement(list, newelement, content);
}


NA_IDEF void naAddListElementLast(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAllocate(sizeof(NAListElement));
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectListElement(list, newelement, content);
}


NA_IDEF void naAddListElementBefore(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAllocate(sizeof(NAListElement));
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectListElement(list, newelement, content);
}


NA_IDEF void naAddListElementAfter(NAList* list, void* content){
  NAListElement* newelement = (NAListElement*)naAllocate(sizeof(NAListElement));
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectListElement(list, newelement, content);
}



// This is a helper function. It should be hidden.
NA_IHLP void* naEjectListElement(NAList* list, NAListElement* element, NABool movenext){
  void* contentpointer; // Declaration before Implementation. Needed for C90
  if(element == &(list->sentinel)){return NA_NULL;}
  if(element == list->cur){list->cur = (movenext?(element->next):(element->prev));}
  element->prev->next = element->next;
  element->next->prev = element->prev;
  list->count--;
  contentpointer = element->content;
  free(element);
  return contentpointer;
}


NA_IDEF void* naRemoveListElementFirst(NAList* list, NABool movenext){
  return naEjectListElement(list, list->sentinel.next, movenext);
}

NA_IDEF void* naRemoveListElementLast(NAList* list, NABool movenext){
  return naEjectListElement(list, list->sentinel.prev, movenext);
}

NA_IDEF void* naRemoveListElementPrev(NAList* list){
  return naEjectListElement(list, list->cur->prev, NA_TRUE);
}

NA_IDEF void* naRemoveListElementCurrent(NAList* list, NABool movenext){
  return naEjectListElement(list, list->cur, movenext);
}

NA_IDEF void* naRemoveListElementNext(NAList* list){
  return naEjectListElement(list, list->cur->next, NA_TRUE);
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


