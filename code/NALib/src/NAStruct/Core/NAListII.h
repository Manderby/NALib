
// This file contains inline implementations of the file NAList.h
// Do not include this file directly! It will automatically be included when
// including "NAList.h"



#include "../../NAUtility/NAMemory.h"


// The following struct should be opaque. Or even better: Completely invisible
// to the programmer.
typedef struct NAListElement NAListElement;


struct NAListElement{
  NAPtr ptr;            // A pointer to the stored content
  NAListElement* next;  // A pointer to the next element
  NAListElement* prev;  // A pointer to the previous element
  #if NA_DEBUG
    NAList* list;       // A list pointer just for debugging.
    size_t iterCount;   // debugging iterator count.
  #endif
};
NA_EXTERN_RUNTIME_TYPE(NAListElement);

struct NAList{
  size_t count;           // The number of elements stored in this list.
  NAListElement sentinel; // The sentinel of the list.
                          // Stores the first and last element of the list
                          // as next and prev pointer. The content is NA_NULL.
  #if NA_DEBUG
    size_t iterCount;     // debugging iterator count
  #endif
};




struct NAListIterator{
  NAPtr           listptr;
  NAListElement*  cur;
  #if NA_DEBUG
    NABool mutator;
  #endif
};

NA_IDEF NAListElement* naNewListElement(NAListElement* prev, NAListElement* next) {
  NAListElement* elem = naNew(NAListElement);
  elem->prev = prev;
  elem->next = next;
  #if NA_DEBUG
    elem->iterCount = 0;
  #endif
  return elem;
}


NA_IDEF NAList* naInitList(NAList* list) {
  #if NA_DEBUG
    if(!list)
      naCrash("list is Null");
  #endif
  list->count = 0;
  list->sentinel.ptr  = naMakePtrNull();
  list->sentinel.next = &(list->sentinel);
  list->sentinel.prev = &(list->sentinel);
  #if NA_DEBUG
    list->sentinel.list = list;
    list->sentinel.iterCount = 0;
    list->iterCount = 0;
  #endif
  return list;
}



NA_IDEF NAList* naCopyList(NAList* list, NAList* originalList) {
  NAListElement* cur;
  #if NA_DEBUG
    if(list == originalList)
      naError("the two lists are the same.");
  #endif
  list = naInitList(list);
  cur = originalList->sentinel.next;
  while(cur != &(originalList->sentinel)) {
    // Note that the following if will be optimized out when NA_DEBUG is 0.
    if(naIsPtrConst(cur->ptr)) {
      naAddListLastConst(list, naGetPtrConst(cur->ptr));
    }else{
      naAddListLastMutable(list, naGetPtrMutable(cur->ptr));
    }
    cur = cur->next;
  }
  return list;
}



NA_IDEF void naClearList(NAList* list) {
  #if NA_DEBUG
    if(!list)
      naCrash("list is Null-Pointer.");
    if(list->iterCount)
      naError("Iterators still running on the list. Did you use naClearListIterator?");
  #endif
  naEmptyList(list);
}



NA_IDEF void naEmptyList(NAList* list) {
  NAListElement* cur;
  NAListElement* next;
  #if NA_DEBUG
    if(!list)
      naCrash("list is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)) {
    #if NA_DEBUG
      if(cur->iterCount)
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



NA_IDEF size_t naGetListCount(const NAList* list) {
  return list->count;
}



NA_IDEF NABool naIsListEmpty(const NAList* list) {
  return (list->count == NA_ZERO);
}



////////////////////////////
// ADDING ELEMENTS

// These are helper functions. They should be hidden.
NA_HIDEF void na_InjectListElement(NAList* list, NAListElement* element) {
  element->next->prev = element;
  element->prev->next = element;
  #if NA_DEBUG
    element->list = list;
    if(element->iterCount)
      naError("Iterators are still using this element. Undefined behaviour.");
    if(list->count + 1 < list->count)
      naError("Integer overflow");
  #endif
  list->count++;
}
NA_HIDEF void na_InjectConstListElement(NAList* list, NAListElement* element, const void* data) {
  element->ptr = naMakePtrWithDataConst(data);
  na_InjectListElement(list, element);
}
NA_HIDEF void na_InjectMutableListElement(NAList* list, NAListElement* element, void* data) {
  element->ptr = naMakePtrWithDataMutable(data);
  na_InjectListElement(list, element);
}



NA_IDEF void naAddListFirstConst(NAList* list, const void* content) {
  NAListElement* newelement = naNewListElement(&(list->sentinel), list->sentinel.next);
  na_InjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListFirstMutable(NAList* list, void* content) {
  NAListElement* newelement = naNewListElement(&(list->sentinel), list->sentinel.next);
  na_InjectMutableListElement(list, newelement, content);
}


NA_IDEF void naAddListLastConst(NAList* list, const void* content) {
  NAListElement* newelement = naNewListElement(list->sentinel.prev, &(list->sentinel));
  na_InjectConstListElement(list, newelement, content);
}


NA_IDEF void naAddListLastMutable(NAList* list, void* content) {
  NAListElement* newelement = naNewListElement(list->sentinel.prev, &(list->sentinel));
  na_InjectMutableListElement(list, newelement, content);
}





////////////////////////////
// REMOVING ELEMENTS

NA_HIDEF void na_EjectList(NAList* list, NAListElement* element, NABool deleteElement) {
  element->prev->next = element->next;
  element->next->prev = element->prev;
  #if NA_DEBUG
    element->list = NA_NULL;  // This will capture some errors.
    if(element->iterCount)
      naError("Iterators still running on a list element. Did you use naClearListIterator?");
    if(list->count == 0)
      naError("List count is zero already");
  #endif
  list->count--;
  
  if(deleteElement)
    naDelete(element);
}



NA_HIDEF void na_EjectListConst(NAList* list, NAListElement* element, NABool deleteElement) {
  #if NA_DEBUG
    if(element == &(list->sentinel))
      naError("Trying to eject sentinel element. This should not happen.");
  #endif
  na_EjectList(list, element, deleteElement);
}



NA_HIDEF void* na_EjectListMutable(NAList* list, NAListElement* element, NABool deleteElement) {
  void* contentpointer;
  #if NA_DEBUG
    if(element == &(list->sentinel))
      naError("Trying to eject sentinel element. This should not happen.");
  #endif
  contentpointer = naGetPtrMutable(element->ptr);
  na_EjectList(list, element, deleteElement);
  return contentpointer;
}



NA_IDEF void naRemoveListFirstConst(NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  na_EjectListConst(list, list->sentinel.next, NA_TRUE);
}


NA_IDEF void* naRemoveListFirstMutable(NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return na_EjectListMutable(list, list->sentinel.next, NA_TRUE);
}


NA_IDEF void naRemoveListLastConst(NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  na_EjectListConst(list, list->sentinel.prev, NA_TRUE);
}


NA_IDEF void* naRemoveListLastMutable(NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return na_EjectListMutable(list, list->sentinel.prev, NA_TRUE);
}


NA_IAPI void naRemoveListData(NAList* list, void* data) {
  NAListIterator iter = naMakeListModifier(list);
  NABool found = naLocateListData(&iter, data);
  #if NA_DEBUG
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

NA_IDEF const void* naGetListFirstConst(const NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrConst(list->sentinel.next->ptr);
}


NA_IDEF void* naGetListFirstMutable(const NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrMutable(list->sentinel.next->ptr);
}


NA_IDEF const void* naGetListLastConst(const NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrConst(list->sentinel.prev->ptr);
}


NA_IDEF void* naGetListLastMutable(const NAList* list) {
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naGetPtrMutable(list->sentinel.prev->ptr);
}


NA_IDEF size_t naGetListElemIndex(const NAList* list, const void* elem) {
  size_t index = 0;
  NAListIterator iter = naMakeListAccessor(list);
  while(naIterateList(&iter))
  {
    const void* curElem = naGetListCurConst(&iter);
    if(curElem == elem) { break; }
    index++;
  }
  naClearListIterator(&iter);
  return index;
}






// //////////////////////
// ITERATION FUNCTIONS

NA_IDEF void naForeachListConst(const NAList* list, NAAccessor accessor) {
  NAListElement* cur;
  #if NA_DEBUG
    if(!list)
      naCrash("list is Null-Pointer.");
    if(!accessor)
      naCrash("accessor is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)) {
    #if NA_DEBUG
      NAListElement* next = cur->next;
    #endif
    accessor(naGetPtrConst(cur->ptr));
    #if NA_DEBUG
      if(cur->next != next)
        naError("List changed during iteration. Unexpected behaviour.");
    #endif
    cur = cur->next;
  }
}


NA_IDEF void naForeachListMutable(const NAList* list, NAMutator mutator) {
  NAListElement* cur;
  #if NA_DEBUG
    if(!list)
      naCrash("list is Null-Pointer.");
    if(!mutator)
      naCrash("mutator is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)) {
    #if NA_DEBUG
      NAListElement* next = cur->next;
    #endif
    mutator(naGetPtrMutable(cur->ptr));
    #if NA_DEBUG
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
NA_HIDEF void na_InjectExistingListElement(NAList* list, NAListElement* element) {
  element->next->prev = element;
  element->prev->next = element;
  #if NA_DEBUG
    element->list = list;
    if(element->iterCount)
      naError("Iterators are still using this element. Undefined behaviour.");
    if(list->count + 1 < list->count)
      naError("Integer overflow");
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
NA_IDEF void naMoveListToLast(NAList* src, NAList* dst) {
  NAListElement* firstelement;
  NAListElement* lastelement;

  #if NA_DEBUG
    if(src->iterCount)
      naError("Src list still has iterators operating upon the elements.");
  #endif

  if(!naIsListEmpty(src)) {
    firstelement = src->sentinel.next;
    lastelement = src->sentinel.prev;
    
    dst->sentinel.prev->next = firstelement;
    firstelement->prev = dst->sentinel.prev;
    lastelement->next = &(dst->sentinel);
    dst->sentinel.prev = lastelement;
    
    #if NA_DEBUG
      if(dst->count + src->count < dst->count)
        naError("Integer overflow");
    #endif
    dst->count += src->count;
    src->sentinel.ptr  = naMakePtrNull();
    src->sentinel.next = &(src->sentinel);
    src->sentinel.prev = &(src->sentinel);
    src->count = 0;
  }
}



NA_IDEF void naMoveListFirstToLast(NAList* src, NAList* dst) {
  NAListElement* element;
  #if NA_DEBUG
    if(naIsListEmpty(src))
      naError("Src List has no first element because it is empty.");
    if(src->sentinel.next->iterCount)
      naError("First element still has iterators running.");
  #endif
  element = src->sentinel.next;
  na_EjectListConst(src, element, NA_FALSE);
  element->next = &(dst->sentinel);
  element->prev = dst->sentinel.prev;
  na_InjectExistingListElement(dst, element);
}




#undef naBeginListAccessorIteration
#define naBeginListAccessorIteration(typedElem, list, iter)\
  iter = naMakeListAccessor(list);\
  while(naIterateList(&iter)) {\
    typedElem = naGetListCurConst(&iter)

#undef naBeginListMutatorIteration
#define naBeginListMutatorIteration(typedElem, list, iter)\
  iter = naMakeListMutator(list);\
  while(naIterateList(&iter)) {\
    typedElem = naGetListCurMutable(&iter)

#undef naBeginListModifierIteration
#define naBeginListModifierIteration(typedElem, list, iter)\
  iter = naMakeListModifier(list);\
  while(naIterateList(&iter)) {\
    typedElem = naGetListCurMutable(&iter)

#undef naEndListIteration
#define naEndListIteration(iter)\
  }\
  naClearListIterator(&iter)



NA_IDEF NAListIterator naMakeListAccessor(const NAList* list) {
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataConst(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #if NA_DEBUG
    if(!list)
      naCrash("list is Null pointer");
    iter.mutator = NA_FALSE;
    iter.cur->iterCount++;
    ((NAList*)list)->iterCount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListMutator(const NAList* list) {
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataConst(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #if NA_DEBUG
    iter.mutator = NA_TRUE;
    iter.cur->iterCount++;
    ((NAList*)list)->iterCount++;
  #endif
  return iter;
}



NA_IDEF NAListIterator naMakeListModifier(NAList* list) {
  NAListIterator iter;
  iter.listptr = naMakePtrWithDataMutable(list);
  iter.cur = (NAListElement*)(&(list->sentinel));
  #if NA_DEBUG
    iter.mutator = NA_TRUE;
    iter.cur->iterCount++;
    list->iterCount++;
  #endif
  return iter;
}



NA_IDEF void naResetListIterator(NAListIterator* iter) {
  const NAList* list = (NAList*)naGetPtrConst(iter->listptr);
  #if NA_DEBUG
    iter->cur->iterCount--;
  #endif
  iter->cur = (NAListElement*)(&(list->sentinel));
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif
}



NA_IDEF void naClearListIterator(NAListIterator* iter) {
  #if NA_DEBUG
    const NAList* list;
    if(iter->cur->iterCount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iter->cur->iterCount--;
    list = (const NAList*)naGetPtrConst(iter->listptr);
    ((NAList*)list)->iterCount--;
  #else
    NA_UNUSED(iter);
  #endif
}



NA_IDEF NABool naLocateListFirst(NAListIterator* iter) {
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #if NA_DEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
    iter->cur->iterCount--;
  #endif
  iter->cur = list->sentinel.next;
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif
  return NA_TRUE;
}



NA_IDEF NABool naLocateListLast(NAListIterator* iter) {
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #if NA_DEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
    iter->cur->iterCount--;
  #endif
  iter->cur = list->sentinel.prev;
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif
  return NA_TRUE;
}



NA_IAPI void naLocateListIterator(NAListIterator* dstIter, const NAListIterator* srcIter) {
  #if NA_DEBUG
    if(naGetPtrConst(dstIter->listptr) != naGetPtrConst(srcIter->listptr))
      naError("Iterators do not share the same list");
    dstIter->cur->iterCount--;
  #endif
  dstIter->cur = srcIter->cur;
  #if NA_DEBUG
    dstIter->cur->iterCount++;
  #endif
}



NA_IDEF NABool naEqualListIterator(
  const NAListIterator* iter1,
  const NAListIterator* iter2)
{
  return naGetListCurConst(iter1) == naGetListCurConst(iter2);
}



NA_IDEF NABool naIterateList(NAListIterator* iter) {
  #if NA_DEBUG
    if(iter->cur->iterCount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iter->cur->iterCount--;
  #endif
  iter->cur = iter->cur->next;
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif

  return (iter->cur != &(((NAList*)naGetPtrConst(iter->listptr))->sentinel));
}



NA_IDEF NABool naIterateListBack(NAListIterator* iter) {
  #if NA_DEBUG
    if(iter->cur->iterCount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iter->cur->iterCount--;
  #endif
  iter->cur = iter->cur->prev;
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif

  return (iter->cur != &(((NAList*)naGetPtrConst(iter->listptr))->sentinel));
}



// Note that this function looks extremely complicated but it is not. When
// this code is built in release, it all boils down to two loops just moving
// a pointer forward or backward.
NA_IDEF NABool naIterateListStep(NAListIterator* iter, NAInt step) {
  #if NA_DEBUG
    if(step == NA_ZERO) {
      naError("step is zero.");
    }
  #endif
  while(step > NA_ZERO) {
    #if NA_DEBUG
      if(iter->cur->iterCount == 0)
        naError("No Iterator at this element. Did you do a double clear?");
      iter->cur->iterCount--;
    #endif
    iter->cur = iter->cur->next;
    step--;
    #if NA_DEBUG
      iter->cur->iterCount++;
      if((iter->cur == &(((NAList*)naGetPtrConst(iter->listptr))->sentinel)) && (step != NA_ZERO)) {
        naError("The iteration overflows the number of elements.");
      }
    #endif
  }
  while(step < NA_ZERO) {
    #if NA_DEBUG
      if(iter->cur->iterCount == 0)
        naError("No Iterator at this element. Did you do a double clear?");
      iter->cur->iterCount--;
    #endif
    iter->cur = iter->cur->prev;
    step++;
    #if NA_DEBUG
      iter->cur->iterCount++;
      if((iter->cur == &(((NAList*)naGetPtrConst(iter->listptr))->sentinel)) && (step != NA_ZERO)) {
        naError("The iteration underflows the number of elements.");
      }
    #endif
  }
  return (iter->cur != &(((NAList*)naGetPtrConst(iter->listptr))->sentinel));
}



NA_IDEF const void* naGetListPrevConst(const NAListIterator* iter) {
  #if NA_DEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrConst(iter->cur->prev->ptr);
}


NA_IDEF void* naGetListPrevMutable(NAListIterator* iter) {
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrMutable(iter->cur->prev->ptr);
}


NA_IDEF const void* naGetListCurConst(const NAListIterator* iter) {
  #if NA_DEBUG
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns Null. This is a feature!
    // if(naIsListEmpty(naGetPtrConst(iter->listptr)))
    //   naError("List is empty");
  #endif
  return naGetPtrConst(iter->cur->ptr);
}


NA_IDEF void* naGetListCurMutable(NAListIterator* iter) {
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns Null. This is a feature!
    // if(naIsListEmpty(naGetPtrConst(iter->listptr)))
    //   naError("List is empty");
  #endif
  return naGetPtrMutable(iter->cur->ptr);
}


NA_IDEF const void* naGetListNextConst(const NAListIterator* iter) {
  #if NA_DEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrConst(iter->cur->next->ptr);
}


NA_IDEF void* naGetListNextMutable(NAListIterator* iter) {
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrMutable(iter->cur->next->ptr);
}




NA_IDEF void naAddListBeforeConst(NAListIterator* iter, const void* content) {
  NAListElement* newelement;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur->prev, iter->cur);
  na_InjectConstListElement((NAList*)naGetPtrConst(iter->listptr), newelement, content);
}


NA_IDEF void naAddListBeforeMutable(NAListIterator* iter, void* content) {
  NAListElement* newelement;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur->prev, iter->cur);
  na_InjectMutableListElement((NAList*)naGetPtrMutable(iter->listptr), newelement, content);
}


NA_IDEF void naAddListAfterConst(NAListIterator* iter, const void* content) {
  NAListElement* newelement;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur, iter->cur->next);
  na_InjectConstListElement((NAList*)naGetPtrConst(iter->listptr), newelement, content);
}


NA_IDEF void naAddListAfterMutable(NAListIterator* iter, void* content) {
  NAListElement* newelement;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur, iter->cur->next);
  na_InjectMutableListElement((NAList*)naGetPtrMutable(iter->listptr), newelement, content);
}



NA_IDEF void naRemoveListPrevConst(NAListIterator* iter) {
  NAList* list;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor iter");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #if NA_DEBUG
    if(iter->cur == list->sentinel.next)
      naError("There is no previous element. Iterator is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  na_EjectListConst(list, iter->cur->prev, NA_TRUE);
}


NA_IDEF void* naRemoveListPrevMutable(NAListIterator* iter) {
  NAList* list;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #if NA_DEBUG
    if(iter->cur == list->sentinel.next)
      naError("There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  return na_EjectListMutable(list, iter->cur->prev, NA_TRUE);
}


NA_IDEF void naRemoveListCurConst(NAListIterator* iter, NABool advance) {
  NAList* list;
  NAListElement* newelem;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
    if(iter->cur == &(list->sentinel))
      naError("No current internal pointer is set. Major memory corruption expected...");
  #endif
  newelem = advance ? iter->cur->next : iter->cur->prev;
  #if NA_DEBUG
    iter->cur->iterCount--;
  #endif
  na_EjectListConst(list, iter->cur, NA_TRUE);
  iter->cur = newelem;
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif
}


NA_IDEF void* naRemoveListCurMutable(NAListIterator* iter, NABool advance) {
  NAList* list;
  NAListElement* newelem;
  void* returncontent;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #if NA_DEBUG
    if(list->count == 0)
      naError("List is empty");
    if(iter->cur == &(list->sentinel))
      naError("No current internal pointer is set. Major memory corruption expected...");
    if(iter->cur->iterCount == 0)
      naError("Internal error. No iterators registered at element which iterator is located at now");
  #endif
  newelem = advance ? iter->cur->next : iter->cur->prev;
  #if NA_DEBUG
    iter->cur->iterCount--;
  #endif
  returncontent = na_EjectListMutable(list, iter->cur, NA_TRUE);
  iter->cur = newelem;
  #if NA_DEBUG
    iter->cur->iterCount++;
  #endif
  return returncontent;
}


NA_IDEF void naRemoveListNextConst(NAListIterator* iter) {
  NAList* list;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #if NA_DEBUG
    if(iter->cur == list->sentinel.prev)
      naError("There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  na_EjectListConst(list, iter->cur->next, NA_TRUE);
}


NA_IDEF void* naRemoveListNextMutable(NAListIterator* iter) {
  NAList* list;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #if NA_DEBUG
    if(iter->cur == list->sentinel.prev)
      naError("There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  return na_EjectListMutable(list, iter->cur->next, NA_TRUE);
}



// ////////////////////////////
// GETTING POSITION INFORMATION



NA_IDEF NABool naIsListAtFirst(const NAListIterator* iter) {
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #if NA_DEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
  #endif
  return (list->sentinel.next == iter->cur);
}



NA_IDEF NABool naIsListAtLast(const NAListIterator* iter) {
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #if NA_DEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
  #endif
  return (list->sentinel.prev == iter->cur);
}



NA_IDEF NABool naIsListAtInitial(const NAListIterator* iter) {
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  return (&(list->sentinel) == iter->cur);
}




NA_IDEF void naMoveListCurToFirst(NAListIterator* srcIter, NABool advance, NAList* dst) {
  NAList* src;
  NAListElement* newelem;
  #if NA_DEBUG
    if(!srcIter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srcIter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srcIter->listptr);
  #if NA_DEBUG
    if(srcIter->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif

  newelem = advance ? srcIter->cur->next : srcIter->cur->prev;
  #if NA_DEBUG
    srcIter->cur->iterCount--;
    if(srcIter->cur->iterCount)
      naError("element has other iterators running.");
  #endif
  na_EjectListConst(src, srcIter->cur, NA_FALSE);
  srcIter->cur->next = dst->sentinel.next;
  srcIter->cur->prev = &(dst->sentinel);
  na_InjectExistingListElement(dst, srcIter->cur);
  srcIter->cur = newelem;
  #if NA_DEBUG
    srcIter->cur->iterCount++;
  #endif

}



NA_IDEF void naMoveListCurToLast(NAListIterator* srcIter, NABool advance, NAList* dst) {
  NAList* src;
  NAListElement* newelem;
  #if NA_DEBUG
    if(!srcIter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srcIter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srcIter->listptr);
  #if NA_DEBUG
    if(srcIter->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif

  newelem = advance ? srcIter->cur->next : srcIter->cur->prev;
  #if NA_DEBUG
    srcIter->cur->iterCount--;
    if(srcIter->cur->iterCount)
      naError("element has other iterators running.");
  #endif
  na_EjectListConst(src, srcIter->cur, NA_FALSE);
  srcIter->cur->next = &(dst->sentinel);
  srcIter->cur->prev = dst->sentinel.prev;
  na_InjectExistingListElement(dst, srcIter->cur);
  srcIter->cur = newelem;
  #if NA_DEBUG
    srcIter->cur->iterCount++;
  #endif

}



NA_IDEF void naMoveListRemainingToLast(NAListIterator* srcIter, NAList* dst) {
  NAList* src;
  NAListElement* element;
  #if NA_DEBUG
    NAListElement* testelem;
    if(!srcIter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srcIter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srcIter->listptr);

  if(!naIsListEmpty(src)) {
    size_t movecount = 1;

    #if NA_DEBUG
      srcIter->cur->iterCount--;
    #endif

    // Move to the first element if the list is rewinded.
    element = srcIter->cur;
    if(element == &(src->sentinel)) {
      element = src->sentinel.next;
    }

    #if NA_DEBUG
      // Test all remaining elements for iterators
      testelem = srcIter->cur;
      while(testelem != &(src->sentinel)) {
        if(testelem->iterCount)
          naError("Element still has an iterator");
        testelem = testelem->next;
      }
    #endif

    // Reroute the cur element from src to dst
    element->prev->next = &(src->sentinel);
    src->sentinel.prev = element->prev;
    srcIter->cur = &(src->sentinel);

    // Reroute the cur element
    element->prev = dst->sentinel.prev;
    dst->sentinel.prev->next = element;

    // count the number of moved elements
    while(element->next != &(src->sentinel)) {
      movecount++;
      element = element->next;
    }

    // Reroute the last element from src to dst
    element->next = &(dst->sentinel);
    dst->sentinel.prev = element;

    #if NA_DEBUG
      if(src->count < movecount)
        naError("Internal error: List count negative.");
      if(src->count + movecount < src->count)
        naError("Integer overflow");
    #endif

    src->count -= movecount;
    dst->count += movecount;

    #if NA_DEBUG
      srcIter->cur->iterCount++;
    #endif
  }
}



NA_IDEF void naMoveListThisToFirst(NAListIterator* srcIter, NAList* dst) {
  NAList* src;
  #if NA_DEBUG
    if(!srcIter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srcIter->listptr))
      naError("Trying to modify with no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srcIter->listptr);
  #if NA_DEBUG
    if(srcIter->cur == &(src->sentinel))
      naError("List iterator does not point to any element. No ''This'' available.");
  #endif

  #if NA_DEBUG
    if(!srcIter->cur->iterCount)
      naError("Count is already 0");
    if(!src->iterCount)
      naError("Count is already 0");
    srcIter->cur->iterCount--;
    src->iterCount--;
  #endif
  na_EjectListConst(src, srcIter->cur, NA_FALSE);
  srcIter->cur->next = dst->sentinel.next;
  srcIter->cur->prev = &(dst->sentinel);
  na_InjectExistingListElement(dst, srcIter->cur);
  naCleanupPtr(&(srcIter->listptr), NA_NULL);
  srcIter->listptr = naMakePtrWithDataMutable(dst);
  #if NA_DEBUG
    srcIter->cur->iterCount++;
    dst->iterCount++;
  #endif
}



NA_IDEF void naMoveListThisToLast(NAListIterator* srcIter, NAList* dst) {
  NAList* src;
  #if NA_DEBUG
    if(!srcIter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srcIter->listptr))
      naError("Trying to modify with no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srcIter->listptr);
  #if NA_DEBUG
    if(srcIter->cur == &(src->sentinel))
      naError("List iterator does not point to any element. No ''This'' available.");
  #endif

  #if NA_DEBUG
    if(!srcIter->cur->iterCount)
      naError("Count is already 0");
    if(!src->iterCount)
      naError("Count is already 0");
    srcIter->cur->iterCount--;
    src->iterCount--;
  #endif
  na_EjectListConst(src, srcIter->cur, NA_FALSE);
  srcIter->cur->next = &(dst->sentinel);
  srcIter->cur->prev = dst->sentinel.prev;
  na_InjectExistingListElement(dst, srcIter->cur);
  naCleanupPtr(&(srcIter->listptr), NA_NULL);
  srcIter->listptr = naMakePtrWithDataMutable(dst);
  #if NA_DEBUG
    srcIter->cur->iterCount++;
    dst->iterCount++;
  #endif
}



NA_IDEF void naExchangeListParts(NAListIterator* iter) {
  NAList* src;
  NAListElement* first;
  NAListElement* prev;
  #if NA_DEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(iter->listptr);

  #if NA_DEBUG
    if(iter->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif
  if((&(src->sentinel) != iter->cur) && (src->sentinel.next != iter->cur)) {
    first = src->sentinel.next;
    prev = iter->cur->prev;
    first->prev = src->sentinel.prev;
    src->sentinel.prev->next = first;
    src->sentinel.next = iter->cur;
    iter->cur->prev = &(src->sentinel);
    src->sentinel.prev = prev;
    prev->next = &(src->sentinel);
  }
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
