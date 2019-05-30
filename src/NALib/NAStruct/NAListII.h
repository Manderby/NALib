// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


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
NA_EXTERN_RUNTIME_TYPE(NAListElement);

struct NAList{
  NAInt count;            // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
  #ifndef NDEBUG
    NAUInt itercount;     // debugging iterator count
  #endif
};




struct NAListIterator{
  NAPtr           listptr;
  NAListElement*  cur;
  #ifndef NDEBUG
    NABool mutator;
  #endif
};


NA_IDEF NAListElement* naNewListElement(NAListElement* prev, NAListElement* next){
  NAListElement* elem = naNew(NAListElement);
  elem->prev = prev;
  elem->next = next;
  #ifndef NDEBUG
    elem->itercount = 0;
  #endif
  return elem;
}


NA_IDEF NAList* naInitList(NAList* list){
  #ifndef NDEBUG
    if(!list)
      naCrash("list is NULL");
  #endif
  list->count = 0;
  list->sentinel.ptr  = naMakePtrNull();
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
  #ifndef NDEBUG
    list->sentinel.list = list;
    list->sentinel.itercount = 0;
    list->itercount = 0;
  #endif
  return list;
}



NA_IDEF NAList* naCopyList(NAList* list, NAList* originallist){
  NAListElement* cur;
  #ifndef NDEBUG
    if(list == originallist)
      naError("the two lists are the same.");
  #endif
  list = naInitList(list);
  cur = originallist->sentinel.next;
  while(cur != &(originallist->sentinel)){
    // Note that the following if will be optimized out when NDEBUG is defined.
    if(naIsPtrConst(cur->ptr)){
      naAddListLastConst(list, naGetPtrConst(cur->ptr));
    }else{
      naAddListLastMutable(list, naGetPtrMutable(cur->ptr));
    }
    cur = cur->next;
  }
  return list;
}



NA_IDEF void naClearList(NAList* list){
  #ifndef NDEBUG
    if(!list)
      naCrash("list is Null-Pointer.");
    if(list->itercount)
      naError("Iterators still running on the list. Did you use naClearListIterator?");
  #endif
  naEmptyList(list);
}



NA_IDEF void naEmptyList(NAList* list){
  NAListElement* cur;
  NAListElement* next;
  #ifndef NDEBUG
    if(!list)
      naCrash("list is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      if(cur->itercount)
        naError("Iterators still running on a list element. Did you use naClearListIterator?");
    #endif
    next = cur->next;
    naDelete(cur);
    cur = next;
  }
  list->count = 0;
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
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
      naError("Iterators are still using this element. Undefined behaviour.");
  #endif
  list->count++;
}
NA_HIDEF void naInjectConstListElement(NAList* list, NAListElement* element, const void* data){
  element->ptr = naMakePtrWithDataConst(data);
  naInjectListElement(list, element);
}
NA_HIDEF void naInjectMutableListElement(NAList* list, NAListElement* element, void* data){
  element->ptr = naMakePtrWithDataMutable(data);
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

NA_HIDEF void naEjectList(NAList* list, NAListElement* element, NABool deleteelement){
  element->prev->next = element->next;
  element->next->prev = element->prev;
  #ifndef NDEBUG
    element->list = NA_NULL;  // This will capture some errors.
    if(element->itercount)
      naError("Iterators still running on a list element. Did you use naClearListIterator?");
  #endif
  list->count--;
  if(deleteelement){naDelete(element);}
}



NA_HIDEF void naEjectListConst(NAList* list, NAListElement* element, NABool deleteelement){
  #ifndef NDEBUG
    if(element == &(list->sentinel))
      naError("Trying to eject sentinel element. This should not happen.");
  #endif
  naEjectList(list, element, deleteelement);
}



NA_HIDEF void* naEjectListMutable(NAList* list, NAListElement* element, NABool deleteelement){
  void* contentpointer;
  #ifndef NDEBUG
    if(element == &(list->sentinel))
      naError("Trying to eject sentinel element. This should not happen.");
  #endif
  contentpointer = naGetPtrMutable(element->ptr);
  naEjectList(list, element, deleteelement);
  return contentpointer;
}



NA_IDEF void naRemoveListFirstConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  naEjectListConst(list, list->sentinel.next, NA_TRUE);
}


NA_IDEF void* naRemoveListFirstMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.next, NA_TRUE);
}


NA_IDEF void naRemoveListLastConst(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  naEjectListConst(list, list->sentinel.prev, NA_TRUE);
}


NA_IDEF void* naRemoveListLastMutable(NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naEjectListMutable(list, list->sentinel.prev, NA_TRUE);
}


NA_IAPI void naRemoveListData(NAList* list, void* data){
  NAListIterator iter = naMakeListModifier(list);
  NABool found = naLocateListData(&iter, data);
  #ifndef NDEBUG
    if(!found)
      naError("data not found in this list");
  #else
    NA_UNUSED(found);
  #endif
  naRemoveListCurMutable(&iter, NA_FALSE);
  naClearListIterator(&iter);
}




// ///////////////////
// ACCESSING ELEMENTS

NA_IDEF const void* naGetListFirstConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrConst(list->sentinel.next->ptr);
}


NA_IDEF void* naGetListFirstMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrMutable(list->sentinel.next->ptr);
}


NA_IDEF const void* naGetListLastConst(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrConst(list->sentinel.prev->ptr);
}


NA_IDEF void* naGetListLastMutable(const NAList* list){
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrMutable(list->sentinel.prev->ptr);
}








// //////////////////////
// ITERATION FUNCTIONS

NA_IDEF void naForeachListConst(const NAList* list, NAAccessor accessor){
  NAListElement* cur;
  #ifndef NDEBUG
    NAListElement* next;
    if (!list)
      naCrash("list is Null-Pointer.");
    if (!accessor)
      naCrash("accessor is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      next = cur->next;
    #endif
    accessor(naGetPtrConst(cur->ptr));
    #ifndef NDEBUG
      if(cur->next != next)
        naError("List changed during iteration. Unexpected behaviour.");
    #endif
    cur = cur->next;
  }
}


NA_IDEF void naForeachListMutable(const NAList* list, NAMutator mutator){
  NAListElement* cur;
  #ifndef NDEBUG
    NAListElement* next;
    if(!list)
      naCrash("list is Null-Pointer.");
    if(!mutator)
      naCrash("mutator is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      next = cur->next;
    #endif
    mutator(naGetPtrMutable(cur->ptr));
    #ifndef NDEBUG
      if(cur->next != next)
        naError("List changed during iteration. Unexpected behaviour.");
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
      naError("Iterators are still using this element. Undefined behaviour.");
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
NA_IDEF void naMoveListToLast(NAList* src, NAList* dst){
  NAListElement* firstelement;
  NAListElement* lastelement;

  #ifndef NDEBUG
    if(src->itercount)
      naError("Src list still has iterators operating upon the elements.");
  #endif

  if(!naIsListEmpty(src)){
    firstelement = src->sentinel.next;
    lastelement = src->sentinel.prev;
    
    dst->sentinel.prev->next = firstelement;
    firstelement->prev = dst->sentinel.prev;
    lastelement->next = &(dst->sentinel);
    dst->sentinel.prev = lastelement;
    
    dst->count += src->count;
    src->sentinel.ptr  = naMakePtrNull();
    src->sentinel.next = &(src->sentinel);
    src->sentinel.prev = &(src->sentinel);
    src->count = 0;
  }
}



NA_IDEF void naMoveListFirstToLast(NAList* src, NAList* dst){
  NAListElement* element;
  #ifndef NDEBUG
    if(naIsListEmpty(src))
      naError("Src List has no first element because it is empty.");
    if(src->sentinel.next->itercount)
      naError("First element still has iterators running.");
  #endif
  element = src->sentinel.next;
  naEjectListConst(src, element, NA_FALSE);
  element->next = &(dst->sentinel);
  element->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, element);
}




#undef naBeginListAccessorIteration
#define naBeginListAccessorIteration(typedelem, list, iter)\
  iter = naMakeListAccessor(list);\
  while(naIterateList(&iter)){\
    typedelem = naGetListCurConst(&iter)

#undef naBeginListMutatorIteration
#define naBeginListMutatorIteration(typedelem, list, iter)\
  iter = naMakeListMutator(list);\
  while(naIterateList(&iter)){\
    typedelem = naGetListCurMutable(&iter)

#undef naBeginListModifierIteration
#define naBeginListModifierIteration(typedelem, list, iter)\
  iter = naMakeListModifier(list);\
  while(naIterateList(&iter)){\
    typedelem = naGetListCurMutable(&iter)

#undef naEndListIteration
#define naEndListIteration(iter)\
  }\
  naClearListIterator(&iter)



NA_IDEF NAListIterator naMakeListAccessor(const NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataConst(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    if(!list)
      naCrash("list is null pointer");
    iter.mutator = NA_FALSE;
    iter.cur->itercount++;
    ((NAList*)list)->itercount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListMutator(const NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataConst(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iter.mutator = NA_TRUE;
    iter.cur->itercount++;
    ((NAList*)list)->itercount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListModifier(NAList* list){
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataMutable(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iter.mutator = NA_TRUE;
    iter.cur->itercount++;
    list->itercount++;
  #endif
  return iter;
}



NA_IDEF void naResetListIterator(NAListIterator* iterator){
  const NAList* list = (NAList*)naGetPtrConst(iterator->listptr);
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
    const NAList* list;
    if(iterator->cur->itercount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iterator->cur->itercount--;
    list = (const NAList*)naGetPtrConst(iterator->listptr);
    ((NAList*)list)->itercount--;
  #else
    NA_UNUSED(iterator);
  #endif
}



NA_IDEF NABool naLocateListFirst(NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(iterator->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
    iterator->cur->itercount--;
  #endif
  iterator->cur = list->sentinel.next;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif
  return NA_TRUE;
}



NA_IDEF NABool naLocateListLast(NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(iterator->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
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
    if(naGetPtrConst(dstiter->listptr) != naGetPtrConst(srciter->listptr))
      naError("Iterators do not share the same list");
    dstiter->cur->itercount--;
  #endif
  dstiter->cur = srciter->cur;
  #ifndef NDEBUG
    dstiter->cur->itercount++;
  #endif
}






NA_IDEF NABool naIterateList(NAListIterator* iterator){
  #ifndef NDEBUG
    if(iterator->cur->itercount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iterator->cur->itercount--;
  #endif
  iterator->cur = iterator->cur->next;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif

  return (iterator->cur != &(((NAList*)naGetPtrConst(iterator->listptr))->sentinel));
}



NA_IDEF NABool naIterateListBack(NAListIterator* iterator){
  #ifndef NDEBUG
    if(iterator->cur->itercount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iterator->cur->itercount--;
  #endif
  iterator->cur = iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount++;
  #endif

  return (iterator->cur != &(((NAList*)naGetPtrConst(iterator->listptr))->sentinel));
}



// Note that this function looks extremely complicated but it is not. When
// this code is built in release, it all boils down to two loops just moving
// a pointer forward or backward.
NA_IDEF NABool naIterateListStep(NAListIterator* iterator, NAInt step){
  #ifndef NDEBUG
    if(step == NA_ZERO){
      naError("step is zero.");
    }
  #endif
  while(step > NA_ZERO){
    #ifndef NDEBUG
      if(iterator->cur->itercount == 0)
        naError("No Iterator at this element. Did you do a double clear?");
      iterator->cur->itercount--;
    #endif
    iterator->cur = iterator->cur->next;
    step--;
    #ifndef NDEBUG
      iterator->cur->itercount++;
      if((iterator->cur == &(((NAList*)naGetPtrConst(iterator->listptr))->sentinel)) && (step != NA_ZERO)){
        naError("The iteration overflows the number of elements.");
      }
    #endif
  }
  while(step < NA_ZERO){
    #ifndef NDEBUG
      if(iterator->cur->itercount == 0)
        naError("No Iterator at this element. Did you do a double clear?");
      iterator->cur->itercount--;
    #endif
    iterator->cur = iterator->cur->prev;
    step++;
    #ifndef NDEBUG
      iterator->cur->itercount++;
      if((iterator->cur == &(((NAList*)naGetPtrConst(iterator->listptr))->sentinel)) && (step != NA_ZERO)){
        naError("The iteration underflows the number of elements.");
      }
    #endif
  }
  return (iterator->cur != &(((NAList*)naGetPtrConst(iterator->listptr))->sentinel));
}



NA_IDEF const void* naGetListPrevConst(const NAListIterator* iterator){
  #ifndef NDEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(iterator->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrConst(iterator->cur->prev->ptr);
}


NA_IDEF void* naGetListPrevMutable(NAListIterator* iterator){
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsListEmpty((const NAList*)naGetPtrConst(iterator->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrMutable(iterator->cur->prev->ptr);
}


NA_IDEF const void* naGetListCurConst(const NAListIterator* iterator){
  #ifndef NDEBUG
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
    // if(naIsListEmpty(naGetPtrConst(iterator->listptr)))
    //   naError("List is empty");
  #endif
  return naGetPtrConst(iterator->cur->ptr);
}


NA_IDEF void* naGetListCurMutable(NAListIterator* iterator){
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
    // if(naIsListEmpty(naGetPtrConst(iterator->listptr)))
    //   naError("List is empty");
  #endif
  return naGetPtrMutable(iterator->cur->ptr);
}


NA_IDEF const void* naGetListNextConst(const NAListIterator* iterator){
  #ifndef NDEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(iterator->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrConst(iterator->cur->next->ptr);
}


NA_IDEF void* naGetListNextMutable(NAListIterator* iterator){
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsListEmpty((const NAList*)naGetPtrConst(iterator->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrMutable(iterator->cur->next->ptr);
}




NA_IDEF void naAddListBeforeConst(NAListIterator* iterator, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur->prev, iterator->cur);
  naInjectConstListElement((NAList*)naGetPtrConst(iterator->listptr), newelement, content);
}


NA_IDEF void naAddListBeforeMutable(NAListIterator* iterator, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur->prev, iterator->cur);
  naInjectMutableListElement((NAList*)naGetPtrMutable(iterator->listptr), newelement, content);
}


NA_IDEF void naAddListAfterConst(NAListIterator* iterator, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur, iterator->cur->next);
  naInjectConstListElement((NAList*)naGetPtrConst(iterator->listptr), newelement, content);
}


NA_IDEF void naAddListAfterMutable(NAListIterator* iterator, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  newelement = naNewListElement(iterator->cur, iterator->cur->next);
  naInjectMutableListElement((NAList*)naGetPtrMutable(iterator->listptr), newelement, content);
}



NA_IDEF void naRemoveListPrevConst(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.next)
      naError("There is no previous element. Iterator is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  naEjectListConst(list, iterator->cur->prev, NA_TRUE);
}


NA_IDEF void* naRemoveListPrevMutable(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.next)
      naError("There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naEjectListMutable(list, iterator->cur->prev, NA_TRUE);
}


NA_IDEF void naRemoveListCurConst(NAListIterator* iterator, NABool advance){
  NAList* list;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
    if(iterator->cur == &(list->sentinel))
      naError("No current internal pointer is set. Major memory corruption expected...");
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


NA_IDEF void* naRemoveListCurMutable(NAListIterator* iterator, NABool advance){
  NAList* list;
  NAListElement* newelem;
  void* returncontent;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
    if(iterator->cur == &(list->sentinel))
      naError("No current internal pointer is set. Major memory corruption expected...");
    if(iterator->cur->itercount == 0)
      naError("Internal error. No iterators registered at element which iterator is located at now");
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
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.prev)
      naError("There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  naEjectListConst(list, iterator->cur->next, NA_TRUE);
}


NA_IDEF void* naRemoveListNextMutable(NAListIterator* iterator){
  NAList* list;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  list = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == list->sentinel.prev)
      naError("There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naEjectListMutable(list, iterator->cur->next, NA_TRUE);
}



// ////////////////////////////
// GETTING POSITION INFORMATION



NA_IDEF NABool naIsListAtFirst(const NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(iterator->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
  #endif
  return (list->sentinel.next == iterator->cur);
}



NA_IDEF NABool naIsListAtLast(const NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(iterator->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
  #endif
  return (list->sentinel.prev == iterator->cur);
}



NA_IDEF NABool naIsListAtInitial(const NAListIterator* iterator){
  const NAList* list = (const NAList*)naGetPtrConst(iterator->listptr);
  return (&(list->sentinel) == iterator->cur);
}




NA_IDEF void naMoveListCurToFirst(NAListIterator* iterator, NABool advance, NAList* dst){
  NAList* src;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif

  newelem = advance ? iterator->cur->next : iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount--;
    if(iterator->cur->itercount)
      naError("element has other iterators running.");
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
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif

  newelem = advance ? iterator->cur->next : iterator->cur->prev;
  #ifndef NDEBUG
    iterator->cur->itercount--;
    if(iterator->cur->itercount)
      naError("element has other iterators running.");
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
    NAListElement* testelem;
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(iterator->listptr);

  if(!naIsListEmpty(src)){
    #ifndef NDEBUG
      iterator->cur->itercount--;
    #endif

    // Move to the first element if the list is rewinded.
    element = iterator->cur;
    if(element == &(src->sentinel)){
      element = src->sentinel.next;
    }

    #ifndef NDEBUG
      // Test all remaining elements for iterators
      testelem = iterator->cur;
      while(testelem != &(src->sentinel)){
        if(testelem->itercount)
          naError("Element still has an iterator");
        testelem = testelem->next;
      }
    #endif

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
        naError("Internal error: List count negative.");
    #endif

    src->count -= movecount;
    dst->count += movecount;

    #ifndef NDEBUG
      iterator->cur->itercount++;
    #endif
  }
}



NA_IDEF void naMoveListThisToFirst(NAListIterator* iterator, NAList* dst){
  NAList* src;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify with no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("List iterator does not point to any element. No ''This'' available.");
  #endif

  #ifndef NDEBUG
    if(!iterator->cur->itercount)
      naError("Count is already 0");
    if(!src->itercount)
      naError("Count is already 0");
    iterator->cur->itercount--;
    src->itercount--;
  #endif
  naEjectListConst(src, iterator->cur, NA_FALSE);
  iterator->cur->next = dst->sentinel.next;
  iterator->cur->prev = &(dst->sentinel);
  naInjectExistingListElement(dst, iterator->cur);
  naCleanupPtr(&(iterator->listptr), NA_NULL);
  iterator->listptr = naMakePtrWithDataMutable(dst);
  #ifndef NDEBUG
    iterator->cur->itercount++;
    dst->itercount++;
  #endif
}



NA_IDEF void naMoveListThisToLast(NAListIterator* iterator, NAList* dst){
  NAList* src;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify with no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(iterator->listptr);
  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("List iterator does not point to any element. No ''This'' available.");
  #endif

  #ifndef NDEBUG
    if(!iterator->cur->itercount)
      naError("Count is already 0");
    if(!src->itercount)
      naError("Count is already 0");
    iterator->cur->itercount--;
    src->itercount--;
  #endif
  naEjectListConst(src, iterator->cur, NA_FALSE);
  iterator->cur->next = &(dst->sentinel);
  iterator->cur->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, iterator->cur);
  naCleanupPtr(&(iterator->listptr), NA_NULL);
  iterator->listptr = naMakePtrWithDataMutable(dst);
  #ifndef NDEBUG
    iterator->cur->itercount++;
    dst->itercount++;
  #endif
}



NA_IDEF void naExchangeListParts(NAListIterator* iterator){
  NAList* src;
  NAListElement* first;
  NAListElement* prev;
  #ifndef NDEBUG
    if(!iterator->mutator)
      naError("Trying to mutate elements with an accessor iterator");
    if(naIsPtrConst(iterator->listptr))
      naError("Trying to modify list while iterator is no modify iterator");
  #endif
  src = (NAList*)naGetPtrMutable(iterator->listptr);

  #ifndef NDEBUG
    if(iterator->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif
  if((&(src->sentinel) != iterator->cur) && (src->sentinel.next != iterator->cur)){
    first = src->sentinel.next;
    prev = iterator->cur->prev;
    first->prev = src->sentinel.prev;
    src->sentinel.prev->next = first;
    src->sentinel.next = iterator->cur;
    iterator->cur->prev = &(src->sentinel);
    src->sentinel.prev = prev;
    prev->next = &(src->sentinel);
  }
}



// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


