// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAList.h
// Do not include this file directly! It will automatically be included when
// including "NAList.h"



#include "../NAMemory.h"


// The following struct should be opaque. Or even better: Completely invisible
// to the programmer.
typedef struct NAListElement NAListElement;


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
  NAInt count;            // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
};



struct NAListIterator{
  NAPtr           listptr;
  NAListElement*  cur;
  #ifndef NDEBUG
    NABool mutator;
  #endif
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
  NAListElement* cur;
  NAListElement* next;
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
    next = cur->next;
    naDelete(cur);
    cur = next;
  }
}



NA_IDEF NAInt naGetListCount(const NAList* list){
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
  element->ptr = naMakePtrWithDataConst(data);
  naInjectListElement(list, element);
}
NA_HIDEF void naInjectMutableListElement(NAList* list, NAListElement* element, void* data){
  element->ptr = naMakePtrWithDataMutable(data, NA_MEMORY_CLEANUP_NONE);
  naInjectListElement(list, element);
}



NA_IDEF void naAddListFirstConst(NAList* list, const void* content){
  NAListElement* newelement = naNewListElement(&(list->sentinel), list->sentinel.next);
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListFirstMutable(NAList* list, void* content){
  NAListElement* newelement = naNewListElement(&(list->sentinel), list->sentinel.next);
  naInjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListLastConst(NAList* list, const void* content){
  NAListElement* newelement = naNewListElement(list->sentinel.prev, &(list->sentinel));
  naInjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListLastMutable(NAList* list, void* content){
  NAListElement* newelement = naNewListElement(list->sentinel.prev, &(list->sentinel));
  naInjectMutableListElement(list, newelement, content);
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
  void* contentpointer;
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








// //////////////////////
// ITERATION FUNCTIONS

NA_IDEF void naForeachListConst(const NAList* list, NAAccessor accessor){
  NAListElement* cur;
  #ifndef NDEBUG
    NAListElement* next;
    if (!list)
      {naCrash("naForeachList", "list is Null-Pointer."); return;}
    if (!accessor)
      {naCrash("naForeachList", "accessor is Null-Pointer."); return;}
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      next = cur->next;
    #endif
    accessor(naGetPtrConst(&(cur->ptr)));
    #ifndef NDEBUG
      if(cur->next != next)
        naError("naForeachList", "List changed during iteration. Unexpected behaviour.");
    #endif
    cur = cur->next;
  }
}


NA_IDEF void naForeachListMutable(const NAList* list, NAMutator mutator){
  NAListElement* cur;
  #ifndef NDEBUG
    NAListElement* next;
    if(!list){
      naCrash("naForeachList", "list is Null-Pointer.");
      return;
    }
    if(!mutator)
      {naCrash("naForeachList", "mutator is Null-Pointer."); return;}
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      next = cur->next;
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
// the API, only specific elements can be transferred.
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






NA_IDEF NAListIterator naMakeListIteratorAccessor(const NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataConst(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iter.mutator = NA_FALSE;
    ((NAList*)list)->sentinel.itercount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListIteratorMutator(const NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataConst(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iter.mutator = NA_TRUE;
    ((NAList*)list)->sentinel.itercount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListIteratorModifier(NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataMutable(list, NA_MEMORY_CLEANUP_NONE);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iter.mutator = NA_TRUE;
    list->sentinel.itercount++;
  #endif
  return iter;
}



NA_IDEF void naResetListIterator(NAListIterator* iterator){
  NAList* list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    iterator->cur->itercount--;
  #endif
  iterator->cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif
}



NA_IDEF void naClearListIterator(NAListIterator* iterator){
  #ifndef NDEBUG
    if(iterator->cur->itercount == 0)
      naError("naClearListIterator", "No Iterator at this element. Did you do a double clear?");
    iterator->cur->itercount--;
  #else
    NA_UNUSED(iterator);
  #endif
}



NA_IDEF NABool naLocateListFirst(NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(&(iterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naLocateListFirst", "List is empty");
    iterator->cur->itercount--;
  #endif
  iterator->cur = list->sentinel.next;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif
  return NA_TRUE;
}



NA_IDEF NABool naLocateListLast(NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(&(iterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naLocateListFirst", "List is empty");
    iterator->cur->itercount--;
  #endif
  iterator->cur = list->sentinel.prev;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif
  return NA_TRUE;
}



NA_IAPI void naLocateListIterator(NAListIterator* dstiter, NAListIterator* srciter){
  #ifndef NDEBUG
    if(naGetPtrConst(&(dstiter->listptr)) != naGetPtrConst(&(srciter->listptr)))
      naError("naLocateListIterator", "Iterators do not share the same list");
    dstiter->cur->itercount--;
  #endif
  dstiter->cur = srciter->cur;
  #ifndef NDEBUG
    dstiter->cur->itercount++;
  #endif
}






// Note that this function looks extremely complicated but it is not. When
// this code is built in release, it all boils down to two loops just moving
// a pointer forward or backward.
NA_IDEF NABool naIterateList(NAListIterator* iterator, NAInt step){
  #ifndef NDEBUG
    if(step == NA_ZERO){
      naError("naIterateList", "step is zero.");
    }
  #endif
  while(step > NA_ZERO){
    #ifndef NDEBUG
      if(iterator->cur->itercount == 0)
        naError("naIterateList", "No Iterator at this element. Did you do a double clear?");
      iterator->cur->itercount--;
    #endif
    iterator->cur = iterator->cur->next;
    step--;
    #ifndef NDEBUG
      iterator->cur->itercount++;
      if((iterator->cur == &(((NAList*)naGetPtrConst(&(iterator->listptr)))->sentinel)) && (step != NA_ZERO)){
        naError("naIterateList", "The iteration overflows the number of elements.");
      }
    #endif
  }
  while(step < NA_ZERO){
    #ifndef NDEBUG
      if(iterator->cur->itercount == 0)
        naError("naIterateList", "No Iterator at this element. Did you do a double clear?");
      iterator->cur->itercount--;
    #endif
    iterator->cur = iterator->cur->prev;
    step++;
    #ifndef NDEBUG
      iterator->cur->itercount++;
      if((iterator->cur == &(((NAList*)naGetPtrConst(&(iterator->listptr)))->sentinel)) && (step != NA_ZERO)){
        naError("naIterateList", "The iteration underflows the number of elements.");
      }
    #endif
  }
  return (iterator->cur != &(((NAList*)naGetPtrConst(&(iterator->listptr)))->sentinel));
}



NA_IDEF const void* naGetListPrevConst(const NAListIterator* iterator){
  #ifndef NDEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(&(iterator->listptr))))
      naError("naGetListPrevConst", "List is empty");
  #endif
  return naGetPtrConst(&(iterator->cur->prev->ptr));
}


NA_IDEF void* naGetListPrevMutable(NAListIterator* iterator){
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naGetListPrevMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsListEmpty((const NAList*)naGetPtrConst(&(iterator->listptr))))
      naError("naGetListPrevMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(iterator->cur->prev->ptr));
}


NA_IDEF const void* naGetListCurrentConst(const NAListIterator* iterator){
  #ifndef NDEBUG
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
//    if(naIsListEmpty(naGetPtrConst(&(iterator->listptr))))
//      naError("naGetListCurrentConst", "List is empty");
  #endif
  return naGetPtrConst(&(iterator->cur->ptr));
}


NA_IDEF void* naGetListCurrentMutable(NAListIterator* iterator){
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naGetListCurrentMutable", "Trying to mutate elements with an accessor iterator");
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
//    if(naIsListEmpty(naGetPtrConst(&(iterator->listptr))))
//      naError("naGetListCurrentMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(iterator->cur->ptr));
}


NA_IDEF const void* naGetListNextConst(const NAListIterator* iterator){
  #ifndef NDEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(&(iterator->listptr))))
      naError("naGetListNextConst", "List is empty");
  #endif
  return naGetPtrConst(&(iterator->cur->next->ptr));
}


NA_IDEF void* naGetListNextMutable(NAListIterator* iterator){
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naGetListNextMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsListEmpty((const NAList*)naGetPtrConst(&(iterator->listptr))))
      naError("naGetListNextMutable", "List is empty");
  #endif
  return naGetPtrMutable(&(iterator->cur->next->ptr));
}




NA_IDEF void naAddListBeforeConst(NAListIterator* iterator, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naAddListBeforeConst", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naAddListBeforeConst", "Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur->prev, iterator->cur);
  naInjectConstListElement((NAList*)naGetPtrConst(&(iterator->listptr)), newelement, content);
}


NA_IDEF void naAddListBeforeMutable(NAListIterator* iterator, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naAddListBeforeMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naAddListBeforeMutable", "Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur->prev, iterator->cur);
  naInjectMutableListElement((NAList*)naGetPtrMutable(&(iterator->listptr)), newelement, content);
}


NA_IDEF void naAddListAfterConst(NAListIterator* iterator, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naAddListAfterConst", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naAddListAfterConst", "Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur, iterator->cur->next);
  naInjectConstListElement((NAList*)naGetPtrConst(&(iterator->listptr)), newelement, content);
}


NA_IDEF void naAddListAfterMutable(NAListIterator* iterator, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naAddListAfterMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naAddListAfterMutable", "Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur, iterator->cur->next);
  naInjectMutableListElement((NAList*)naGetPtrMutable(&(iterator->listptr)), newelement, content);
}



NA_IDEF void naRemoveListPrevConst(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naRemoveListPrevConst", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naRemoveListPrevConst", "Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.next)
      naError("naRemoveListPrevConst", "There is no previous element. Iterator is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListPrevConst", "List is empty");
  #endif
  naEjectListConst(list, iterator->cur->prev, NA_TRUE);
}


NA_IDEF void* naRemoveListPrevMutable(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naRemoveListPrevMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naRemoveListPrevMutable", "Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.next)
      naError("naRemoveListPrevMutable", "There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListPrevMutable", "List is empty");
  #endif
  return naEjectListMutable(list, iterator->cur->prev, NA_TRUE);
}


NA_IDEF void naRemoveListCurrentConst(NAListIterator* iterator, NABool advance){
  NAList* list;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naRemoveListCurrentConst", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naRemoveListPrevMutable", "Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentConst", "List is empty");
    if(iterator->cur == &(list->sentinel))
      naError("naRemoveListCurrentConst", "No current internal pointer is set. Major memory corruption expected...");
  #endif
  newelem = advance ? iterator->cur->next : iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount--;
  #endif
  naEjectListConst(list, iterator->cur, NA_TRUE);
  iterator->cur = newelem;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif
}


NA_IDEF void* naRemoveListCurrentMutable(NAListIterator* iterator, NABool advance){
  NAList* list;
  NAListElement* newelem;
  void* returncontent;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naRemoveListCurrentMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naRemoveListPrevMutable", "Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(list->count == 0)
      naError("naRemoveListCurrentMutable", "List is empty");
    if(iterator->cur == &(list->sentinel))
      naError("naRemoveListCurrentMutable", "No current internal pointer is set. Major memory corruption expected...");
    if(iterator->cur->itercount == 0)
      naError("naRemoveListCurrentMutable", "Internal error. No iterators registered at element which iterator is located at now");
  #endif
  newelem = advance ? iterator->cur->next : iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount--;
  #endif
  returncontent = naEjectListMutable(list, iterator->cur, NA_TRUE);
  iterator->cur = newelem;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif
  return returncontent;
}


NA_IDEF void naRemoveListNextConst(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naRemoveListNextConst", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naRemoveListNextConst", "Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.prev)
      naError("naRemoveListNextConst", "There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListNextConst", "List is empty");
  #endif
  naEjectListConst(list, iterator->cur->next, NA_TRUE);
}


NA_IDEF void* naRemoveListNextMutable(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naRemoveListNextMutable", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naRemoveListNextMutable", "Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.prev)
      naError("naRemoveListNextMutable", "There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("naRemoveListNextMutable", "List is empty");
  #endif
  return naEjectListMutable(list, iterator->cur->next, NA_TRUE);
}



// ////////////////////////////
// GETTING POSITION INFORMATION



NA_IDEF NABool naIsListAtFirst(const NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(&(iterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naIsListAtFirst", "List is empty");
  #endif
  return (list->sentinel.next == iterator->cur);
}



NA_IDEF NABool naIsListAtLast(const NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(&(iterator->listptr));
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("naIsListAtLast", "List is empty");
  #endif
  return (list->sentinel.prev == iterator->cur);
}



NA_IDEF NABool naIsListAtInitial(const NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(&(iterator->listptr));
  return (&(list->sentinel) == iterator->cur);
}




NA_IDEF void naMoveListCurToFirst(NAListIterator* iterator, NABool advance, NAList* dst){
  NAList* src;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naMoveListCurToFirst", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naMoveListCurToFirst", "Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("naMoveListCurToFirst", "List has no current element set.");
  #endif

  newelem = advance ? iterator->cur->next : iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount--;
  #endif
  naEjectListConst(src, iterator->cur, NA_FALSE);
  iterator->cur->next = dst->sentinel.next;
  iterator->cur->prev = &(dst->sentinel);
  naInjectExistingListElement(dst, iterator->cur);
  iterator->cur = newelem;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif

}



NA_IDEF void naMoveListCurToLast(NAListIterator* iterator, NABool advance, NAList* dst){
  NAList* src;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naMoveListCurToLast", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naMoveListCurToLast", "Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(&(iterator->listptr));
  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("naMoveCurToListLast", "List has no current element set.");
  #endif

  newelem = advance ? iterator->cur->next : iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount--;
  #endif
  naEjectListConst(src, iterator->cur, NA_FALSE);
  iterator->cur->next = &(dst->sentinel);
  iterator->cur->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, iterator->cur);
  iterator->cur = newelem;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif

}



NA_IDEF void naMoveListRemainingToLast(NAListIterator* iterator, NAList* dst){
  NAList* src;
  NAListElement* element;
  NAInt movecount = 1;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naMoveListRemainingToLast", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naMoveListRemainingToLast", "Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(&(iterator->listptr));

  
  if(naIsListEmpty(src)){return;}
  
  // Move to the first element if the list is rewinded.
  element = iterator->cur;
  if(element == &(src->sentinel)){
    element = src->sentinel.next;
  }

  // Reroute the cur element from src to dst
  element->prev->next = &(src->sentinel);
  src->sentinel.prev = element->prev;
  iterator->cur = &(src->sentinel);

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
      naError("naMoveListRemainingToLast", "Internal error: List count negative.");
  #endif
  
//  printf("%d, %d\n", (int)src->count, (int)movecount);
  src->count -= movecount;
  dst->count += movecount;
}



NA_IDEF void naExchangeListParts(NAListIterator* iterator){
  NAList* src;
  NAListElement* first;
  NAListElement* prev;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("naExchangeListParts", "Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(&(iterator->listptr)))
      naError("naMoveListCurToLast", "Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(&(iterator->listptr));

  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("naExchangeListParts", "List has no current element set.");
  #endif
  if(&(src->sentinel) == iterator->cur){return;}
  if(src->sentinel.next == iterator->cur){return;}
  first = src->sentinel.next;
  prev = iterator->cur->prev;
  first->prev = src->sentinel.prev;
  src->sentinel.prev->next = first;
  src->sentinel.next = iterator->cur;
  iterator->cur->prev = &(src->sentinel);
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


