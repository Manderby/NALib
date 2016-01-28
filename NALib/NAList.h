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


// The NAList struct is opaque, see notes below in the inline implementation.
typedef struct NAList NAList;
// The NAListPos is an opaque type for the NAListElement struct defined below.
// With this trick, the real type remains hidden to the user and the API is
// clean and easy to understand.
typedef const void* NAListPos;


// Creates an empty list.
NA_IAPI NAList* naInitList(NAList* list);

// Creates an exact copy of originallist by replicating all elements in the
// same order. The two list pointers must not be the same. If you are using
// NAListPos locators, the locators will keep the location of the original
// list.
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
NA_IAPI void    naForeachList  (NAList* list, NAFunc mutator);

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
//
// Note: The internal pointer is not altered at all when adding elements. If
// the internal pointer is not set, Before behaves the same as Last and After
// behaves the same as First.
//
// The return value is a position locator which the user can store (for example
// within the element itself) for later use to quickly access specific elements
// within a list or move an element within or even between lists. If you have
// no intention to use that value, you may simply ignore it. This locator will
// always reflect the current position and list the element is stored in, as
// long as the element is not explicitely removed from a list. You do not need
// to adjust it in any way.
//
// Beware: You should not add NULL pointers to a list. Iteration will not
// work properly then.
NA_IAPI NAListPos naAddListFirstConst    (NAList* list, const void* content);
NA_IAPI NAListPos naAddListFirstMutable  (NAList* list,       void* content);
NA_IAPI NAListPos naAddListLastConst     (NAList* list, const void* content);
NA_IAPI NAListPos naAddListLastMutable   (NAList* list,       void* content);
NA_IAPI NAListPos naAddListBeforeConst   (NAList* list, const void* content);
NA_IAPI NAListPos naAddListBeforeMutable (NAList* list,       void* content);
NA_IAPI NAListPos naAddListAfterConst    (NAList* list, const void* content);
NA_IAPI NAListPos naAddListAfterMutable  (NAList* list,       void* content);


// /////////////////////////////
// Removing elements
//
// Remove functions. Removes the desired element from the list.
// The advance parameter denotes how the internal pointer shall move in case
// the current element is the element which is removed. If advance is NA_FALSE
// The internal pointer will be moved to the previous element. If advance is
// NA_TRUE, the internal pointer will be moved to the next element.
// Note: Remove functions free the memory of the corresponding NAListElement but
// do not free the memory of the content! But the mutable variants return a
// pointer to the previously stored content such that you can erase it if
// you need to.
NA_IAPI void  naRemoveListFirstConst    (NAList* list, NABool advance);
NA_IAPI void* naRemoveListFirstMutable  (NAList* list, NABool advance);
NA_IAPI void  naRemoveListLastConst     (NAList* list, NABool advance);
NA_IAPI void* naRemoveListLastMutable   (NAList* list, NABool advance);
NA_IAPI void  naRemoveListPrevConst     (NAList* list);
NA_IAPI void* naRemoveListPrevMutable   (NAList* list);
NA_IAPI void  naRemoveListCurrentConst  (NAList* list, NABool advance);
NA_IAPI void* naRemoveListCurrentMutable(NAList* list, NABool advance);
NA_IAPI void  naRemoveListNextConst     (NAList* list);
NA_IAPI void* naRemoveListNextMutable   (NAList* list);


// /////////////////////////////////////////////
// Access functions
//
// Beware! These functions are no iteration functions. The internal pointer
// is NOT moved. Just the desired element is returned.
NA_IAPI const void* naGetListFirstConst     (const NAList* list);
NA_IAPI       void* naGetListFirstMutable   (const NAList* list);
NA_IAPI const void* naGetListLastConst      (const NAList* list);
NA_IAPI       void* naGetListLastMutable    (const NAList* list);
NA_IAPI const void* naGetListPrevConst      (const NAList* list);
NA_IAPI       void* naGetListPrevMutable    (const NAList* list);
NA_IAPI const void* naGetListCurrentConst   (const NAList* list);
NA_IAPI       void* naGetListCurrentMutable (const NAList* list);
NA_IAPI const void* naGetListNextConst      (const NAList* list);
NA_IAPI       void* naGetListNextMutable    (const NAList* list);


// List position functions. Use the given NAListPos argument to retrieve the
// element it points to or to advance retrieve the previous or next element.
// If there is no previous or next element, NA_NULL will be returned.
NA_IAPI NAListPos   naGetListPosition       (const NAList* list);
NA_IAPI const void* naGetListPositionConst  (const NAList* list, NAListPos pos);
NA_IAPI       void* naGetListPositionMutable(const NAList* list, NAListPos pos);
NA_IAPI NAListPos   naGetListPositionNext   (const NAList* list, NAListPos pos);
NA_IAPI NAListPos   naGetListPositionPrev   (const NAList* list, NAListPos pos);


// /////////////////////////////////////////////
// Reordering functions
//
// The move functions remove the CURRENT element from the srclist and move them
// to dstlist at the position indicated by the function name. These functions
// are VERY FAST! Way faster than locating a specific element, removing it and
// re-adding it.
//
// Use these functions in conjunction with naLocateListPos. It is the fastest
// way to transfer specific elements from one list to another.
//
// The advance parameter behaves the same as with the remove functions.
//
// The dstlist can be the same as srclist.
//
// See implementation for more details.
NA_IAPI void naMoveListFirst (NAList* srclist, NABool advance, NAList* dstlist);
NA_IAPI void naMoveListLast  (NAList* srclist, NABool advance, NAList* dstlist);
NA_IAPI void naMoveListBefore(NAList* srclist, NABool advance, NAList* dstlist);
NA_IAPI void naMoveListAfter (NAList* srclist, NABool advance, NAList* dstlist);

// The exchange function splits the list into two parts BEFORE the current
// element and re-attaches the whole list before that at the end of the list.
// For example when D is current, ABCDEFG becomes DEFGABC.
// The current element will point to the first element after this function.
NA_IAPI void naExchangeListParts(NAList* list);


// ////////////////////////////////////
// Iteration functions
//
// Every list has an internal pointer denoting the current element. The
// programmer can control and access this element with iteration functions.
// If no current element is set, NA_NULL is returned as a pointer. The easiest
// way to implement an iteration is using a while loop:
//
// NAList* mylist;
// void* curelement;
// naFirstList(mylist);
// while((curelement = naIterateListMutable(mylist, 1))){
//   Do stuff with curelement.
// }
//
// You should enclose the while-condition in additional parantheses such that
// a compiler knows that the returned pointer must be evaluated as a condition.
//
// Note: You can safely use remove functions while iterating. But sure that
// you remove the correct one!
//
// When being inside the while scope, the list itself already points to the
// element AFTER iteration.
//
// Do NOT use a for-loop for iteration! The internal pointer may, depending on
// how you write the for-loop, not point to the element AFTER iteration which
// can be devastating when removing elements. Also, it is very hard to read.
// ////////////////////////////////////


// The following functions move the internal pointer to a desired element.
//
// You can simply set the position to the first or last element or you can
// position it using the position you got when adding new elements to the list.
// You can also search for specific elements by pointer or index with the
// Locate functions.
//
// Warning: While the First, Last and Position functions are VERY fast, the
// Locate functions are VERY slow! And they are not inlined. If you really need
// to locate elements within a list, try to go for the Position solution. But
// you need to store the NAListPos when adding elements.
//
// If the given index is negative, it denotes the element from the end of the
// list, whereas -1 denotes the last element.
//
// The Pointer and Index variants will return NA_FALSE, if the element has not
// been found and NA_TRUE if it has been found. If not found, the internal
// pointer will be unset. All other variants will always return NA_TRUE.
//
NA_IAPI NABool naFirstList           (const NAList* list);
NA_IAPI NABool naLastList            (const NAList* list);
NA_IAPI NABool naPositionList        (const NAList* list, NAListPos listpos);
NA_API  NABool naLocateListPointer   (const NAList* list, void* content);
NA_API  NABool naLocateListIndex     (const NAList* list, NAInt indx);

// Returns the current element and iterates the given steps forward or
// backwards by using positive or negative numbers. If there is no current
// element available, the function returns NA_NULL.
//
// If step is 0, you simply access the current element. Note that for
// accessing the current element, the use of naGetListCurrent would be
// preferable.
NA_IAPI const void* naIterateListConst    (const NAList* list, NAInt step);
NA_IAPI       void* naIterateListMutable  (      NAList* list, NAInt step);

// Moves the internal pointer forward or backwards without accessing content.
// If no internal pointer is set, the internal pointer is not touched at all.
NA_IAPI void naIterateList                (const NAList* list, NAInt step);

// Returns whether the list is at a certain position
NA_IAPI NABool naIsListAtFirst     (const NAList* list);
NA_IAPI NABool naIsListAtLast      (const NAList* list);
NA_IAPI NABool naIsListAtPosition  (const NAList* list, NAListPos listpos);





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
  #ifndef NDEBUG
    NAList* list;       // A list pointer just for debugging.
  #endif
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



NA_IDEF void naForeachList(NAList* list, NAFunc mutator){
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


NA_IDEF NABool naFirstList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.next;
  return NA_TRUE;
}


NA_IDEF NABool naLastList(const NAList* list){
  NAList* mutablelist = (NAList*)list;
  mutablelist->cur = list->sentinel.prev;
  return NA_TRUE;
}



NA_IDEF NABool naPositionList(const NAList* list, NAListPos listpos){
  NAList* mutablelist = (NAList*)list;
  NAListElement* element = (NAListElement*)listpos;
  #ifndef NDEBUG
    if(!listpos)
      naError("naLocateListPos", "The element given is a Null Pointer.");
    if(element->list != list)
      naError("naLocateListPos", "The element at the given pos is not part of the given list.");
  #endif
    mutablelist->cur = element;
  return NA_TRUE;
}



// Note: You can use naIterateListConst or naIterateListMutable to directly
// get the current element.
NA_IDEF void naIterateList(const NAList* list, NAInt step){
  NAList* mutablelist = (NAList*)list;
  while(step > 0){
    mutablelist->cur = list->cur->next;
    step--;
  }
  while(step < 0){
    mutablelist->cur = list->cur->prev;
    step++;
  }
}



NA_IDEF const void* naIterateListConst(const NAList* list, NAInt step){
  const void* retptr;
  retptr = naGetPtrConst(&(list->cur->ptr));
  naIterateList(list, step);
  return retptr;
}



NA_IDEF void* naIterateListMutable(NAList* list, NAInt step){
  void* retptr;
  retptr = naGetPtrMutable(&(list->cur->ptr));
  naIterateList(list, step);
  return retptr;
}



NA_IDEF NABool naIsListAtFirst(const NAList* list){
  return (list->sentinel.next == list->cur);
}



NA_IDEF NABool naIsListAtLast(const NAList* list){
  return (list->sentinel.prev == list->cur);
}



NA_IDEF NABool naIsListAtPosition(const NAList* list, NAListPos listpos){
  #ifndef NDEBUG
    NAListElement* element = (NAListElement*)listpos;
    if(element->list != list)
      naError("naIsListAtPosition", "The given position is not part of the given list");
  #endif
  return (list->cur == listpos);
}



// These are helper functions. They should be hidden.
NA_HIDEF void naInjectConstListElement(NAList* list, NAListElement* element, const void* data){
  element->ptr = naMakePtrWithConstBuffer(data, NA_ZERO, NA_ZERO);
  element->next->prev = element;
  element->prev->next = element;
  #ifndef NDEBUG
    element->list = list;
  #endif
  list->count++;
}
NA_HIDEF void naInjectMutableListElement(NAList* list, NAListElement* element, void* data){
  element->ptr = naMakePtrWithMutableBuffer(data, NA_ZERO, NA_ZERO);
  element->next->prev = element;
  element->prev->next = element;
  #ifndef NDEBUG
    element->list = list;
  #endif
  list->count++;
}


NA_IDEF NAListPos naAddListFirstConst(NAList* list, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListFirstConst", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListFirstMutable(NAList* list, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListFirstMutable", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = list->sentinel.next;
  newelement->prev = &(list->sentinel);
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListLastConst(NAList* list, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListLastConst", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListLastMutable(NAList* list, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListLastMutable", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = &(list->sentinel);
  newelement->prev = list->sentinel.prev;
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListBeforeConst(NAList* list, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListBeforeConst", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListBeforeMutable(NAList* list, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListBeforeMutable", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = list->cur;
  newelement->prev = list->cur->prev;
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListAfterConst(NAList* list, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListAfterConst", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListAfterMutable(NAList* list, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!content)
      naError("naAddListAfterMutable", "content is Null-Pointer. May lead to errors when iterating");
  #endif
  newelement = naNew(NAListElement);
  newelement->next = list->cur->next;
  newelement->prev = list->cur;
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}



// These are helper functions. They should be hidden.
NA_HIDEF void naEjectListConst(NAList* list, NAListElement* element, NABool advance, NABool deleteelement){
  if(element == &(list->sentinel)){
    #ifndef NDEBUG
      naError("naEjectListConst", "Trying to eject sentinel element. Will return without doing anything. Try to stop before that.");
    #endif
    return;
  }
  if(element == list->cur){list->cur = (advance?(element->next):(element->prev));}
  element->prev->next = element->next;
  element->next->prev = element->prev;
  #ifndef NDEBUG
    element->list = NA_NULL;  // This will capture some errors.
  #endif
  list->count--;
  if(deleteelement){naDelete(element);}
}
#include <stdio.h>
NA_HIDEF void* naEjectListMutable(NAList* list, NAListElement* element, NABool advance, NABool deleteelement){
  void* contentpointer; // Declaration before Implementation. Needed for C90
  if(element == &(list->sentinel)){
    #ifndef NDEBUG
      naError("naEjectListMutable", "Trying to eject sentinel element. Will return NULL without doing anything. Try to stop removing elements before that.");
    #endif
    return NA_NULL;
  }
  if(element == list->cur){
    list->cur = (advance?(element->next):(element->prev));
  }
  element->prev->next = element->next;
  element->next->prev = element->prev;
  #ifndef NDEBUG
    element->list = NA_NULL;  // This will capture some errors.
  #endif
  list->count--;
  contentpointer = naGetPtrMutable(&(element->ptr));
  if(deleteelement){naDelete(element);}
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
    if(list->count == 0)
      naError("naRemoveListPrevConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur->prev, NA_TRUE, NA_TRUE);
}

NA_IDEF void* naRemoveListPrevMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListPrevMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur->prev, NA_TRUE, NA_TRUE);
}

NA_IDEF void naRemoveListCurrentConst(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur, advance, NA_TRUE);
}

NA_IDEF void* naRemoveListCurrentMutable(NAList* list, NABool advance){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur, advance, NA_TRUE);
}

NA_IDEF void naRemoveListNextConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextConst", "List is empty");
  #endif
  naEjectListConst(list, list->cur->next, NA_TRUE, NA_TRUE);
}

NA_IDEF void* naRemoveListNextMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->cur->next, NA_TRUE, NA_TRUE);
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






NA_IDEF NAListPos naGetListPosition(const NAList* list){
  if(list->cur == &(list->sentinel)){
    return NA_NULL;
  }else{
    return list->cur;
  }
}


NA_IDEF const void* naGetListPositionConst(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListPositionConst", "List is empty");
    if(element->list != list)
      naError("naGetListPositionConst", "The given position is not part of the given list");
  #else
    NA_UNUSED(list);
  #endif
  if(!pos){return NA_NULL;} // security return. Only needed for code sanity checks.
  return naGetPtrConst(&(element->ptr));
}


NA_IDEF void* naGetListPositionMutable(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(!pos){
      naCrash("naGetListPositionMutable", "pos is a NULL pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListPositionMutable", "List is empty");
    if(element->list != list)
      naError("naGetListPositionMutable", "The given position is not part of the given list");
  #else
    NA_UNUSED(list);
  #endif
  if(!pos){return NA_NULL;} // security return. Only needed for code sanity checks.
  return naGetPtrMutable(&(element->ptr));
}


NA_IDEF NAListPos naGetListPositionNext(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListPositionNext", "List is empty");
    if(element->list != list)
      naError("naGetListPositionNext", "The given position is not part of the given list");
  #endif
  if(!pos){return NA_NULL;} // security return. Only needed for code sanity checks.
  if(element->next == &(list->sentinel)){
    return NA_NULL;
  }else{
    return (NAListPos)(element->next);
  }
}


NA_IDEF NAListPos naGetListPositionPrev(const NAList* list, NAListPos pos){
  NAListElement* element = (NAListElement*)pos;
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListPositionPrev", "List is empty");
    if(element->list != list)
      naError("naGetListPositionPrev", "The given position is not part of the given list");
  #endif
  if(element->prev == &(list->sentinel)){
    return NA_NULL;
  }else{
    return (NAListPos)(element->prev);
  }
}


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
NA_IDEF void naMoveListFirst(NAList* srclist, NABool advance, NAList* dstlist){
  NAListElement* element;
  #ifndef NDEBUG
    if(srclist->cur == &(srclist->sentinel))
      naError("naMoveListFirst", "List has no current element set.");
  #endif
  element = srclist->cur;
  naEjectListConst(srclist, srclist->cur, advance, NA_FALSE);
  element->next = dstlist->sentinel.next;
  element->prev = &(dstlist->sentinel);
  naInjectExistingListElement(dstlist, element);
}


NA_IDEF void naMoveListLast(NAList* srclist, NABool advance, NAList* dstlist){
  NAListElement* element;
  #ifndef NDEBUG
    if(srclist->cur == &(srclist->sentinel))
      naError("naMoveListLast", "List has no current element set.");
  #endif
  element = srclist->cur;
  naEjectListConst(srclist, srclist->cur, advance, NA_FALSE);
  element->next = &(dstlist->sentinel);
  element->prev = dstlist->sentinel.prev;
  naInjectExistingListElement(dstlist, element);
}


NA_IDEF void naMoveListBefore(NAList* srclist, NABool advance, NAList* dstlist){
  NAListElement* element;
  #ifndef NDEBUG
    if(srclist->cur == &(srclist->sentinel))
      naError("naMoveListBefore", "List has no current element set.");
  #endif
  element = srclist->cur;
  naEjectListConst(srclist, srclist->cur, advance, NA_FALSE);
  element->next = dstlist->cur;
  element->prev = dstlist->cur->prev;
  naInjectExistingListElement(dstlist, element);
}


NA_IDEF void naMoveListAfter(NAList* srclist, NABool advance, NAList* dstlist){
  NAListElement* element;
  #ifndef NDEBUG
    if(srclist->cur == &(srclist->sentinel))
      naError("naMoveListAfter", "List has no current element set.");
  #endif
  element = srclist->cur;
  naEjectListConst(srclist, srclist->cur, advance, NA_FALSE);
  element->next = dstlist->cur->next;
  element->prev = dstlist->cur;
  naInjectExistingListElement(dstlist, element);
}



NA_IDEF void naExchangeListParts(NAList* list){
  NAListElement* first;
  NAListElement* prev;
//  #ifndef NDEBUG
//    if(list->cur == &(list->sentinel))
//      naError("naMoveListAfter", "List has no current element set.");
//  #endif
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


