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
    NAUInt itercount;   // debugging iterator count.
  #endif
};


NA_RUNTIME_TYPE(NAListElement, NA_NULL);



struct NAList{
  NAUInt count;           // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
};



struct NAListIterator{
  NAPtr           listptr;
  NAListElement*  cur;
};



NA_API NAListElement* naNewListElement(NAListElement* prev, NAListElement* next);


NA_IDEF NAList* naInitList(NAList* list){
  #ifndef NDEBUG
    if(!list)
      {naCrash("naInitList", "list is NULL"); return NA_NULL;}
  #endif
  list->count = 0;
  list->sentinel.ptr  = naMakeNullPtr();
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
  #ifndef NDEBUG
    list->sentinel.list = list;
    list->sentinel.itercount = 0;
  #endif
  return list;
}



NA_IDEF NAList* naCopyList(NAList* list, NAList* originallist){
  // Declaration before implementation. Needed for C90.
  NAListElement* cur;
  #ifndef NDEBUG
    if(list == originallist)
      naError("naCopyList", "the two lists are the same.");
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
    if(list->sentinel.itercount)
      naError("naClearList", "Iterators still running on the list. Did you use naClearListIterator?");
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
    #ifndef NDEBUG
      if(cur->itercount)
        naError("naEmptyList", "Iterators still running on a list element. Did you use naClearListIterator?");
    #endif
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
    if(element->itercount)
      naError("naInjectExistingListElement", "Iterators are still using this element. Undefined behaviour.");
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
  NAListElement* newelement = naNewListElement(&(list->sentinel), list->sentinel.next);
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListFirstMutable(NAList* list, void* content){
  NAListElement* newelement = naNewListElement(&(list->sentinel), list->sentinel.next);
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListLastConst(NAList* list, const void* content){
  NAListElement* newelement = naNewListElement(list->sentinel.prev, &(list->sentinel));
  naInjectConstListElement(list, newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListLastMutable(NAList* list, void* content){
  NAListElement* newelement = naNewListElement(list->sentinel.prev, &(list->sentinel));
  naInjectMutableListElement(list, newelement, content);
  return newelement;
}



NA_IDEF NAListPos naGetListInitialPosition(NAList* list){
  return &(list->sentinel);
}


NA_IDEF NAListPos naGetListFirstPosition(NAList* list){
  return list->sentinel.next;
}


NA_IDEF NAListPos naGetListLastPosition(NAList* list){
  return list->sentinel.prev;
}


////////////////////////////
// REMOVING ELEMENTS

// These are helper functions. They should be hidden.
NA_HIDEF void naEjectList(NAList* list, NAListElement* element, NABool deleteelement){
  element->prev->next = element->next;
  element->next->prev = element->prev;
  #ifndef NDEBUG
    element->list = NA_NULL;  // This will capture some errors.
    if(element->itercount)
      naError("naEjectList", "Iterators still running on a list element. Did you use naClearListIterator?");
  #endif
  list->count--;
  if(deleteelement){naDelete(element);}
}
NA_HIDEF void naEjectListConst(NAList* list, NAListElement* element, NABool deleteelement){
  #ifndef NDEBUG
    if(element == &(list->sentinel))
      naError("naEjectListConst", "Trying to eject sentinel element. This should not happen.");
  #endif
  naEjectList(list, element, deleteelement);
}
#include <stdio.h>
NA_HIDEF void* naEjectListMutable(NAList* list, NAListElement* element, NABool deleteelement){
  void* contentpointer; // Declaration before Implementation. Needed for C90
  #ifndef NDEBUG
    if(element == &(list->sentinel))
      naError("naEjectListMutable", "Trying to eject sentinel element. This should not happen.");
  #endif
  contentpointer = naGetPtrMutable(&(element->ptr));
  naEjectList(list, element, deleteelement);
  return contentpointer;
}



NA_IDEF void naRemoveListFirstConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListFirstConst", "List is empty");
  #endif
  naEjectListConst(list, list->sentinel.next, NA_TRUE);
}


NA_IDEF void* naRemoveListFirstMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListFirstMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.next, NA_TRUE);
}


NA_IDEF void naRemoveListLastConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListLastConst", "List is empty");
  #endif
  naEjectListConst(list, list->sentinel.prev, NA_TRUE);
}


NA_IDEF void* naRemoveListLastMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListLastMutable", "List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.prev, NA_TRUE);
}





// ///////////////////
// ACCESSING ELEMENTS

NA_IDEF const void* naGetListFirstConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListFirstConst", "List is empty");
  #endif
  return naGetPtrConst(&(list->sentinel.next->ptr));
}


NA_IDEF void* naGetListFirstMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListFirstMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->sentinel.next->ptr));
}


NA_IDEF const void* naGetListLastConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListLastConst", "List is empty");
  #endif
  return naGetPtrConst(&(list->sentinel.prev->ptr));
}


NA_IDEF void* naGetListLastMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naGetListLastMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(list->sentinel.prev->ptr));
}



// Note that the list would not be necessary when NDEBUG is undefined. But we
// force the programmer to be specific what list he wants to access so we can
// so some consistency checks if this truely what he wants.
NA_IDEF const void* naGetListPositionConst(const NAList* list, NAListPos listpos){
  NAListElement* element = (NAListElement*)listpos;
  #ifndef NDEBUG
    if(!listpos){
      naCrash("naGetListPositionConst", "pos is a Null-Pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListPositionConst", "List is empty");
    if(element->list != list)
      naError("naGetListPositionConst", "The given position is not part of the given list");
  #else
    NA_UNUSED(list);
  #endif
  return naGetPtrConst(&(element->ptr));
}



NA_IDEF void* naGetListPositionMutable(const NAList* list, NAListPos listpos){
  NAListElement* element = (NAListElement*)listpos;
  #ifndef NDEBUG
    if(!listpos){
      naCrash("naGetListPositionMutable", "pos is a Null-Pointer");
      return NA_NULL;
    }
    if(list->count == 0)
      naError("naGetListPositionMutable", "List is empty");
    if(element->list != list)
      naError("naGetListPositionMutable", "The given position is not part of the given list");
  #else
    NA_UNUSED(list);
  #endif
  return naGetPtrMutable(&(element->ptr));
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
    if(!mutator)
      naError("naForeachList", "mutator is Null-Pointer.");
  #endif
  NAListElement* cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      NAListElement* next = cur->next;
    #endif
    mutator(naGetPtrMutable(&(cur->ptr)));
    #ifndef NDEBUG
      if(cur->next != next)
        naError("naForeachList", "List changed during iteration. Unexpected behaviour.");
    #endif
    cur = cur->next;
  }
}






// //////////////////////
// REORDERING ELEMENTS


// Helper Function for injection of existing list elements. Expects the
// prev and next pointer already to be set.
NA_HIDEF void naInjectExistingListElement(NAList* list, NAListElement* element){
  element->next->prev = element;
  element->prev->next = element;
  #ifndef NDEBUG
    element->list = list;
    if(element->itercount)
      naError("naInjectExistingListElement", "Iterators are still using this element. Undefined behaviour.");
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
NA_IDEF void naMoveListFirstToLast(NAList* src, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(naIsListEmpty(src))
      naError("naMoveListFirstToLast", "Src List has no first element because it is empty.");
  #endif
  element = src->sentinel.next;
  naEjectListConst(src, element, NA_FALSE);
  element->next = &(dst->sentinel);
  element->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, element);
}






NA_IDEF NAListIterator naMakeListIteratorConst(const NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithConstBuffer(list, 0, 0);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    ((NAList*)list)->sentinel.itercount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListIteratorMutable(NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithMutableBuffer(list, 0, 0, NA_MEMORY_CLEANUP_NONE);
  iter.cur = &(list->sentinel);
  #ifndef NDEBUG
    list->sentinel.itercount++;
  #endif
  return iter;
}



NA_IDEF void naClearListIterator(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(listiterator->cur->itercount == 0)
      naError("naClearListIterator", "No Iterator at this element. Did you do a double clear?");
    listiterator->cur->itercount--;
  #else
    NA_UNUSED(listiterator);
  #endif
}



NA_IDEF NABool naLocateListFirst(NAListIterator* listiterator){
  const NAList* list = naGetPtrConst(&(listiterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naLocateListFirst", "List is empty");
  #endif
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  listiterator->cur = list->sentinel.next;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif
  return NA_TRUE;
}



NA_IDEF NABool naLocateListLast(NAListIterator* listiterator){
  const NAList* list = naGetPtrConst(&(listiterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naLocateListFirst", "List is empty");
  #endif
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  listiterator->cur = list->sentinel.prev;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif
  return NA_TRUE;
}



NA_IDEF NABool naLocateListPosition(NAListIterator* listiterator, NAListPos listpos){
  NAListElement* elem = (NAListElement*)listpos;
  #ifndef NDEBUG
    if(!listpos)
      naError("naLocateListPosition", "listpos is NULL. Use naGetListInitialPosition for an undefined listpos.");
    if(elem->list != naGetPtrConst(&(listiterator->listptr)))
      naError("naLocateListPosition", "The given position lies not within list.");
  #endif
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  listiterator->cur = elem;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif
  return NA_TRUE;
}






// Note that this function looks extremely complicated but it is not. When
// this code is built in release, it all boils down to two loops just moving
// a pointer forward or backward.
NA_IDEF NABool naIterateList(NAListIterator* listiterator, NAInt step){
  #ifndef NDEBUG
    if(step == NA_ZERO){
      naError("naIterateList", "step is zero.");
    }
  #endif
  while(step > NA_ZERO){
    #ifndef NDEBUG
      if(listiterator->cur->itercount == 0)
        naError("naIterateList", "No Iterator at this element. Did you do a double clear?");
      listiterator->cur->itercount--;
    #endif
    listiterator->cur = listiterator->cur->next;
    step--;
    #ifndef NDEBUG
      listiterator->cur->itercount++;
      if((listiterator->cur == &(((NAList*)naGetPtrConst(&(listiterator->listptr)))->sentinel)) && (step != NA_ZERO)){
        naError("naIterateList", "The iteration overflows the number of elements.");
      }
    #endif
  }
  while(step < NA_ZERO){
    #ifndef NDEBUG
      if(listiterator->cur->itercount == 0)
        naError("naIterateList", "No Iterator at this element. Did you do a double clear?");
      listiterator->cur->itercount--;
    #endif
    listiterator->cur = listiterator->cur->prev;
    step++;
    #ifndef NDEBUG
      listiterator->cur->itercount++;
      if((listiterator->cur == &(((NAList*)naGetPtrConst(&(listiterator->listptr)))->sentinel)) && (step != NA_ZERO)){
        naError("naIterateList", "The iteration underflows the number of elements.");
      }
    #endif
  }
  return (listiterator->cur != &(((NAList*)naGetPtrConst(&(listiterator->listptr)))->sentinel));
}



NA_IDEF const void* naGetListPrevConst(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsListEmpty(naGetPtrConst(&(listiterator->listptr))))
      naError("naGetListPrevConst", "List is empty");
  #endif
  return naGetPtrConst(&(listiterator->cur->prev->ptr));
}


NA_IDEF void* naGetListPrevMutable(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naGetListPrevMutable", "Trying to mutate elements of iterator which is const");
    if(naIsListEmpty(naGetPtrConst(&(listiterator->listptr))))
      naError("naGetListPrevMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(listiterator->cur->prev->ptr));
}


NA_IDEF const void* naGetListCurrentConst(NAListIterator* listiterator){
  #ifndef NDEBUG
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
//    if(naIsListEmpty(naGetPtrConst(&(listiterator->listptr))))
//      naError("naGetListCurrentConst", "List is empty");
  #endif
  return naGetPtrConst(&(listiterator->cur->ptr));
}


NA_IDEF void* naGetListCurrentMutable(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naGetListCurrentMutable", "Trying to mutate elements of iterator which is const");
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
//    if(naIsListEmpty(naGetPtrConst(&(listiterator->listptr))))
//      naError("naGetListCurrentMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(listiterator->cur->ptr));
}


NA_IDEF const void* naGetListNextConst(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsListEmpty(naGetPtrConst(&(listiterator->listptr))))
      naError("naGetListNextConst", "List is empty");
  #endif
  return naGetPtrConst(&(listiterator->cur->next->ptr));
}


NA_IDEF void* naGetListNextMutable(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naGetListNextMutable", "Trying to mutate elements of iterator which is const");
    if(naIsListEmpty(naGetPtrConst(&(listiterator->listptr))))
      naError("naGetListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(listiterator->cur->next->ptr));
}




NA_IDEF NAListPos naAddListBeforeConst(NAListIterator* listiterator, const void* content){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naAddListBeforeConst", "Trying to mutate list of iterator which is const");
  #endif
  NAListElement* newelement = naNewListElement(listiterator->cur->prev, listiterator->cur);
  naInjectConstListElement((NAList*)naGetPtrConst(&(listiterator->listptr)), newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListBeforeMutable(NAListIterator* listiterator, void* content){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naAddListBeforeMutable", "Trying to mutate list of iterator which is const");
  #endif
  NAListElement* newelement = naNewListElement(listiterator->cur->prev, listiterator->cur);
  naInjectMutableListElement(naGetPtrMutable(&(listiterator->listptr)), newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListAfterConst(NAListIterator* listiterator, const void* content){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naAddListAfterConst", "Trying to mutate list of iterator which is const");
  #endif
  NAListElement* newelement = naNewListElement(listiterator->cur, listiterator->cur->next);
  naInjectConstListElement((NAList*)naGetPtrConst(&(listiterator->listptr)), newelement, content);
  return newelement;
}


NA_IDEF NAListPos naAddListAfterMutable(NAListIterator* listiterator, void* content){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naAddListAfterMutable", "Trying to mutate list of iterator which is const");
  #endif
  NAListElement* newelement = naNewListElement(listiterator->cur, listiterator->cur->next);
  naInjectMutableListElement(naGetPtrMutable(&(listiterator->listptr)), newelement, content);
  return newelement;
}



NA_IDEF void naRemoveListPrevConst(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naRemoveListPrevConst", "Trying to mutate list of iterator which is const");
  #endif
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(listiterator->cur == list->sentinel.next)
      naError("naRemoveListPrevConst", "There is no previous element. Iterator is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListPrevConst", "List is empty");
  #endif
  naEjectListConst(list, listiterator->cur->prev, NA_TRUE);
}


NA_IDEF void* naRemoveListPrevMutable(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naRemoveListPrevMutable", "Trying to mutate list of iterator which is const");
  #endif
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(listiterator->cur == list->sentinel.next)
      naError("naRemoveListPrevMutable", "There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListPrevMutable", "List is empty");
  #endif
  return naEjectListMutable(list, listiterator->cur->prev, NA_TRUE);
}


NA_IDEF void naRemoveListCurrentConst(NAListIterator* listiterator, NABool advance){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naRemoveListPrevMutable", "Trying to mutate list of iterator which is const");
  #endif
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentConst", "List is empty");
    if(listiterator->cur == &(list->sentinel))
      naError("naRemoveListCurrentConst", "No current internal pointer is set. Major memory corruption expected...");
  #endif
  NAListElement* newelem = advance ? listiterator->cur->next : listiterator->cur->prev;
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  naEjectListConst(list, listiterator->cur, NA_TRUE);
  listiterator->cur = newelem;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif
}


NA_IDEF void* naRemoveListCurrentMutable(NAListIterator* listiterator, NABool advance){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naRemoveListPrevMutable", "Trying to mutate list of iterator which is const");
  #endif
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentMutable", "List is empty");
    if(listiterator->cur == &(list->sentinel))
      naError("naRemoveListCurrentMutable", "No current internal pointer is set. Major memory corruption expected...");
  #endif
  NAListElement* newelem = advance ? listiterator->cur->next : listiterator->cur->prev;
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  NAListElement* returnelem = naEjectListMutable(list, listiterator->cur, NA_TRUE);
  listiterator->cur = newelem;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif
  return returnelem;
}


NA_IDEF void naRemoveListNextConst(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naRemoveListNextConst", "Trying to mutate list of iterator which is const");
  #endif
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(listiterator->cur == list->sentinel.prev)
      naError("naRemoveListNextConst", "There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListNextConst", "List is empty");
  #endif
  naEjectListConst(list, listiterator->cur->next, NA_TRUE);
}


NA_IDEF void* naRemoveListNextMutable(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naRemoveListNextMutable", "Trying to mutate list of iterator which is const");
  #endif
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(listiterator->cur == list->sentinel.prev)
      naError("naRemoveListNextMutable", "There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naEjectListMutable(list, listiterator->cur->next, NA_TRUE);
}



// ////////////////////////////
// GETTING POSITION INFORMATION


NA_IDEF NABool naIsListIteratorEmpty (NAListIterator* listiterator){
  const NAList* list = naGetPtrConst(&(listiterator->listptr));
  return naIsListEmpty(list);
}



NA_IDEF NABool naIsListAtFirst(NAListIterator* listiterator){
  const NAList* list = naGetPtrConst(&(listiterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naIsListAtFirst", "List is empty");
  #endif
  return (list->sentinel.next == listiterator->cur);
}



NA_IDEF NABool naIsListAtLast(NAListIterator* listiterator){
  NAList* list = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naIsListAtLast", "List is empty");
  #endif
  return (list->sentinel.prev == listiterator->cur);
}



NA_IDEF NABool naIsListAtPosition(NAListIterator* listiterator, NAListPos listpos){
  NAListElement* element = (NAListElement*)listpos;
  #ifndef NDEBUG
    NAList* list = naGetPtrMutable(&(listiterator->listptr));
    if(naIsListEmpty(list))
      naError("naIsListAtPosition", "List is empty");
    if(element->list != list)
      naError("naIsListAtPosition", "The given position is not part of the given list");
  #endif
  return (listiterator->cur == element);
}



NA_IDEF NAListPos naGetListPrevPosition(NAListIterator* listiterator){
  #ifndef NDEBUG
    NAList* list = naGetPtrMutable(&(listiterator->listptr));
    if(naIsListEmpty(list))
      naError("naGetListNextPosition", "List is empty");
  #endif
  return listiterator->cur->prev;
}



NA_IDEF NAListPos naGetListCurrentPosition(NAListIterator* listiterator){
  #ifndef NDEBUG
    NAList* list = naGetPtrMutable(&(listiterator->listptr));
    if(naIsListEmpty(list))
      naError("naGetListNextPosition", "List is empty");
  #endif
  return listiterator->cur;
}



NA_IDEF NAListPos naGetListNextPosition(NAListIterator* listiterator){
  #ifndef NDEBUG
    NAList* list = naGetPtrMutable(&(listiterator->listptr));
    if(naIsListEmpty(list))
      naError("naGetListNextPosition", "List is empty");
  #endif
  return listiterator->cur->next;
}



NA_IDEF void naMoveListCurToFirst(NAListIterator* listiterator, NABool advance, NAList* dst){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naMoveListCurToFirst", "Trying to mutate list of iterator which is const");
  #endif
  NAList* src = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(listiterator->cur == &(src->sentinel))
      naError("naMoveListCurToFirst", "List has no current element set.");
  #endif

  NAListElement* newelem = advance ? listiterator->cur->next : listiterator->cur->prev;
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  naEjectListConst(src, listiterator->cur, NA_FALSE);
  listiterator->cur->next = dst->sentinel.next;
  listiterator->cur->prev = &(dst->sentinel);
  naInjectExistingListElement(dst, listiterator->cur);
  listiterator->cur = newelem;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif

}



NA_IDEF void naMoveListCurToLast(NAListIterator* listiterator, NABool advance, NAList* dst){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naMoveListCurToLast", "Trying to mutate list of iterator which is const");
  #endif
  NAList* src = naGetPtrMutable(&(listiterator->listptr));
  #ifndef NDEBUG
    if(listiterator->cur == &(src->sentinel))
      naError("naMoveCurToListLast", "List has no current element set.");
  #endif

  NAListElement* newelem = advance ? listiterator->cur->next : listiterator->cur->prev;
  #ifndef NDEBUG
    listiterator->cur->itercount--;
  #endif
  naEjectListConst(src, listiterator->cur, NA_FALSE);
  listiterator->cur->next = &(dst->sentinel);
  listiterator->cur->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, listiterator->cur);
  listiterator->cur = newelem;
  #ifndef NDEBUG
    listiterator->cur->itercount++;
  #endif

}



NA_IDEF void naMoveListTrailingToLast(NAListIterator* listiterator, NAList* dst){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naMoveListCurToLast", "Trying to mutate list of iterator which is const");
  #endif
  NAList* src = naGetPtrMutable(&(listiterator->listptr));

  NAListElement* element;
  NAUInt movecount = 1;
  
  if(naIsListEmpty(src)){return;}
  
  // Move to the first element if the list is rewinded.
  element = listiterator->cur;
  if(element == &(src->sentinel)){
    element = src->sentinel.next;
  }

  // Reroute the cur element from src to dst
  element->prev->next = &(src->sentinel);
  src->sentinel.prev = element->prev;
  listiterator->cur = &(src->sentinel);

  // Reroute the cur element
  element->prev = dst->sentinel.prev;
  dst->sentinel.prev->next = element;
  
  // count the number of moved elements
  while(element->next != &(src->sentinel)){
    movecount++;
    element = element->next;
  }
  
  // Reroute the last element from src to dst
  element->next = &(dst->sentinel);
  dst->sentinel.prev = element;
  
  #ifndef NDEBUG
    if(src->count < movecount)
      naError("naMoveListTrailingToLast", "Internal error: List count negative.");
  #endif
  
//  printf("%d, %d\n", (int)src->count, (int)movecount);
  src->count -= movecount;
  dst->count += movecount;
}



NA_IDEF void naExchangeListParts(NAListIterator* listiterator){
  #ifndef NDEBUG
    if(naIsPtrConst(&(listiterator->listptr)))
      naError("naMoveListCurToLast", "Trying to mutate list of iterator which is const");
  #endif
  NAList* src = naGetPtrMutable(&(listiterator->listptr));

  NAListElement* first;
  NAListElement* prev;
  #ifndef NDEBUG
    if(listiterator->cur == &(src->sentinel))
      naError("naExchangeListParts", "List has no current element set.");
  #endif
  if(&(src->sentinel) == listiterator->cur){return;}
  if(src->sentinel.next == listiterator->cur){return;}
  first = src->sentinel.next;
  prev = listiterator->cur->prev;
  first->prev = src->sentinel.prev;
  src->sentinel.prev->next = first;
  src->sentinel.next = listiterator->cur;
  listiterator->cur->prev = &(src->sentinel);
  src->sentinel.prev = prev;
  prev->next = &(src->sentinel);
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


