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
NA_IAPI NAList* naInitList(NAList* list);

// Creates an exact copy of originallist by replicating all elements in the
// same order. The two list pointers must not be the same.
NA_IAPI NAList* naCopyList(NAList* list, NAList* originallist);

// Clears or empties the given list. Note: This will free all list elements
// but not the contents they store! Use naForeachList or iteration for that.
//
// The difference between naEmptyList and naClearList is that Clear should be
// called with a semantic of invalidating the list whereas Empty simply empties
// the list. Due to the implementation in NALib, this makes no difference but
// should nontheless be distinguished. For other datastructures like NAHeap,
// this equality does not hold.
NA_IAPI void    naClearList  (NAList* list);
NA_IAPI void    naEmptyList  (NAList* list);

// Traverses the whole list and calls the mutator on each element. A pointer
// to each element will be given to the mutator.
NA_IAPI void    naForeachList  (NAList* list, NAMutator mutator);

// Returns the informations about the number of elements in this list.
NA_IAPI NAUInt  naGetListCount(const NAList* list);
NA_IAPI NABool  naIsListEmpty(const NAList* list);

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
// Every list has an internal index denoting the current element. The
// programmer can controll and access this element with iteration functions.
// If no current element is set, NA_NULL is returned as a content. A typical
// example of iteration is the following:
//
// naFirstList(mylist);
// while((curelement = naIterateListMutable(mylist, 1))){
//   do stuff with curelement.
// }
//
// Note: You can safely use remove functions while iterating!

// With the following functions, you can initialize the internal pointer.
NA_IAPI void naFirstList                  (const NAList* list);
NA_IAPI void naLastList                   (const NAList* list);

// Returns the current element and then iterates the given steps forward or
// backwards by using positive or negative numbers. If step is 0, you simply
// access the current element. Note that for accessing the current element,
// the use of naGetListCurrent would be preferable.
// If no internal pointer is set, NA_NULL is returned and the internal pointer
// is not touched at all.
NA_IAPI const void* naIterateListConst    (const NAList* list, NAInt step);
NA_IAPI       void* naIterateListMutable  (      NAList* list, NAInt step);

// The locate-functions move the internal pointer to the element storing the
// desired content pointer or being located at the given index. If the given
// index is negative, it denotes the element from the end of the list, whereas
// -1 denotes the last element.
//
// If the element is not found or the index is not within the list range, the
// internal pointer will be unset.
// Returns NA_TRUE, if the element is found and NA_FALSE if not.
//
// Warning: These functions are very slow! And they are not inlined.
NA_API NABool naLocateListPointer        (const NAList* list, void* content);
NA_API NABool naLocateListIndex          (const NAList* list, NAInt indx);

// Moves the internal pointer forward or backwards.
// If no internal pointer is set, NA_NULL is returned and the internal pointer
// is not touched at all.
// Note: You can use naIterateListConst or naIterateListMutable to directly
// get the current element.
NA_IAPI void naIterateList                (const NAList* list, NAInt step);

// Returns whether the list is at a certain position
NA_IAPI NABool naIsListAtFirst            (const NAList* list);
NA_IAPI NABool naIsListAtLast             (const NAList* list);





// A helper function for the runtime system.
NA_HAPI void naPrepareListElementRuntime();








// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

#include "NARuntime.h"


// The following struct should be opaque. Or even better: Completely invisible
// to the programmer.
typedef struct NAListElement NAListElement;

struct NAListElement{
  NAPtr ptr;            // A pointer to the stored content
  NAListElement* next;  // A pointer to the next element
  NAListElement* prev;  // A pointer to the previous element
};

struct NAList{
  NAUInt count;            // The number of elements stored in this list.
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
  list->sentinel.ptr  = naMakePtr();
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
  list->cur           = &(list->sentinel);
  return list;
}


NA_IDEF NAList* naCopyList(NAList* list, NAList* originallist){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  list = naInitList(list);
  cur = originallist->sentinel.next;
  while(cur != &(originallist->sentinel)){
    NAListElement* next = cur->next;
    // Note that the following if will be optimized out when NDEBUG is defined.
    if(naIsPtrConst(&(cur->ptr))){
      naAddListLastConst(list, naGetPtrConst(&(cur->ptr)));
    }else{
      naAddListLastMutable(list, naGetPtrMutable(&(cur->ptr)));
    }
    cur = next;
  }
  return list;
}


NA_IDEF void naClearList(NAList* list){
  naEmptyList(list);
}


NA_IDEF void naEmptyList(NAList* list){
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
    naDelete(cur);
    cur = next;
  }
}



NA_IDEF void naForeachList(NAList* list, NAMutator mutator){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  #ifndef NDEBUG
    if(!list){
      naCrash("naForeachList", "list is Null-Pointer.");
      return;
    }
    if(!mutator)
      naCrash("naForeachList", "mutator is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    NAListElement* next = cur->next;
    mutator(naGetPtrMutable(&(cur->ptr)));
    cur = next;
  }
}



NA_IDEF NAUInt naGetListCount(const NAList* list){
  return list->count;
}


NA_IDEF NABool naIsListEmpty(const NAList* list){
  return (list->count == NA_ZERO);
}


NA_IDEF void naFirstList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.next;
}


NA_IDEF void naLastList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.prev;
}




// Note: You can use naIterateListConst or naIterateListMutable to directly
// get the current element.
NA_IDEF void naIterateList(const NAList* list, NAInt step){
  NAList* mutablelist = (NAList*)list;
  while(step > 0){
    #ifndef NDEBUG
      if(!naGetPtrConst(&(list->cur->ptr)))
        naError("naIterateListConst", "Iteration overflows");
    #endif
    mutablelist->cur = list->cur->next;
    step--;
  }
  while(step < 0){
    #ifndef NDEBUG
      if(!naGetPtrConst(&(list->cur->ptr)))
        naError("naIterateListConst", "Iteration underflows");
    #endif
    mutablelist->cur = list->cur->prev;
    step++;
  }
}



NA_IDEF const void* naIterateListConst(const NAList* list, NAInt step){
  const void* returnptr;  // Declaration before implementation. C90
  if(list->cur == &(list->sentinel)){return NA_NULL;}
  returnptr = naGetPtrConst(&(list->cur->ptr));
  if(step){naIterateList(list, step);}
  return returnptr;
}



NA_IDEF void* naIterateListMutable(NAList* list, NAInt step){
  void* returnptr; // Declaration before implementation. C90
  if(list->cur == &(list->sentinel)){return NA_NULL;}
  returnptr = naGetPtrMutable(&(list->cur->ptr));
  if(step){naIterateList(list, step);}
  return returnptr;
}



NA_IDEF NABool naIsListAtFirst(const NAList* list){
  return (list->sentinel.next == list->cur);
}



NA_IDEF NABool naIsListAtLast(const NAList* list){
  return (list->sentinel.prev == list->cur);
}




// These are helper functions. They should be hidden.
NA_HIDEF void naInjectConstListElement(NAList* list, NAListElement* element, const void* data){
  element->ptr = naMakePtrWithConstBuffer(data, NA_ZERO, NA_ZERO);
  element->next->prev = element;
  element->prev->next = element;
  list->count++;
}
NA_HIDEF void naInjectMutableListElement(NAList* list, NAListElement* element, void* data){
  element->ptr = naMakePtrWithMutableBuffer(data, NA_ZERO, NA_ZERO);
  element->next->prev = element;
  element->prev->next = element;
  list->count++;
}


NA_IDEF void naAddListFirstConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListFirstMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListLastConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListLastMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListBeforeConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListBeforeMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListAfterConst(NAList* list, const void* content){
  NAListElement* newelement = naNew(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListAfterMutable(NAList* list, void* content){
  NAListElement* newelement = naNew(NAListElement);
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
  naDelete(element);
}
NA_HIDEF void* naEjectListMutable(NAList* list, NAListElement* element, NABool movenext){
  void* contentpointer; // Declaration before Implementation. Needed for C90
  if(element == &(list->sentinel)){return NA_NULL;}
  if(element == list->cur){
    list->cur = (movenext?(element->next):(element->prev));
  }
  element->prev->next = element->next;
  element->next->prev = element->prev;
  list->count--;
  contentpointer = naGetPtrMutable(&(element->ptr));
  naDelete(element);
  return contentpointer;
}


NA_IDEF void naRemoveListFirstConst(NAList* list, NABool movenext){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListFirstConst", "List is empty");
  #endif
  naEjectListConst(list, list->sentinel.next, movenext);
}

NA_IDEF void* naRemoveListFirstMutable(NAList* list, NABool movenext){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListFirstMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.next, movenext);
}

NA_IDEF void naRemoveListLastConst(NAList* list, NABool movenext){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListLastConst", "List is empty");
  #endif
  naEjectListConst(list, list->sentinel.prev, movenext);
}

NA_IDEF void* naRemoveListLastMutable(NAList* list, NABool movenext){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListLastMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.prev, movenext);
}

NA_IDEF void naRemoveListPrevConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListPrevConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur->prev, NA_TRUE);
}

NA_IDEF void* naRemoveListPrevMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListPrevMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur->prev, NA_TRUE);
}

NA_IDEF void naRemoveListCurrentConst(NAList* list, NABool movenext){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur, movenext);
}

NA_IDEF void* naRemoveListCurrentMutable(NAList* list, NABool movenext){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur, movenext);
}

NA_IDEF void naRemoveListNextConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur->next, NA_TRUE);
}

NA_IDEF void* naRemoveListNextMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur->next, NA_TRUE);
}






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


