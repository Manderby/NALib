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
    if (!list)
      naCrash("list is Null-Pointer.");
    if (!accessor)
      naCrash("accessor is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      NAListElement* next = cur->next;
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
    if(!list)
      naCrash("list is Null-Pointer.");
    if(!mutator)
      naCrash("mutator is Null-Pointer.");
  #endif
  cur = list->sentinel.next;
  while(cur != &(list->sentinel)){
    #ifndef NDEBUG
      NAListElement* next = cur->next;
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



NA_IDEF void naResetListIterator(NAListIterator* iter){
  const NAList* list = (NAList*)naGetPtrConst(iter->listptr);
  #ifndef NDEBUG
    iter->cur->itercount--;
  #endif
  iter->cur = (NAListElement*)(&(list->sentinel));
  #ifndef NDEBUG
    iter->cur->itercount++;
  #endif
}



NA_IDEF void naClearListIterator(NAListIterator* iter){
  #ifndef NDEBUG
    const NAList* list;
    if(iter->cur->itercount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iter->cur->itercount--;
    list = (const NAList*)naGetPtrConst(iter->listptr);
    ((NAList*)list)->itercount--;
  #else
    NA_UNUSED(iter);
  #endif
}



NA_IDEF NABool naLocateListFirst(NAListIterator* iter){
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
    iter->cur->itercount--;
  #endif
  iter->cur = list->sentinel.next;
  #ifndef NDEBUG
    iter->cur->itercount++;
  #endif
  return NA_TRUE;
}



NA_IDEF NABool naLocateListLast(NAListIterator* iter){
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
    iter->cur->itercount--;
  #endif
  iter->cur = list->sentinel.prev;
  #ifndef NDEBUG
    iter->cur->itercount++;
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






NA_IDEF NABool naIterateList(NAListIterator* iter){
  #ifndef NDEBUG
    if(iter->cur->itercount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iter->cur->itercount--;
  #endif
  iter->cur = iter->cur->next;
  #ifndef NDEBUG
    iter->cur->itercount++;
  #endif

  return (iter->cur != &(((NAList*)naGetPtrConst(iter->listptr))->sentinel));
}



NA_IDEF NABool naIterateListBack(NAListIterator* iter){
  #ifndef NDEBUG
    if(iter->cur->itercount == 0)
      naError("No Iterator at this element. Did you do a double clear?");
    iter->cur->itercount--;
  #endif
  iter->cur = iter->cur->prev;
  #ifndef NDEBUG
    iter->cur->itercount++;
  #endif

  return (iter->cur != &(((NAList*)naGetPtrConst(iter->listptr))->sentinel));
}



// Note that this function looks extremely complicated but it is not. When
// this code is built in release, it all boils down to two loops just moving
// a pointer forward or backward.
NA_IDEF NABool naIterateListStep(NAListIterator* iter, NAInt step){
  #ifndef NDEBUG
    if(step == NA_ZERO){
      naError("step is zero.");
    }
  #endif
  while(step > NA_ZERO){
    #ifndef NDEBUG
      if(iter->cur->itercount == 0)
        naError("No Iterator at this element. Did you do a double clear?");
      iter->cur->itercount--;
    #endif
    iter->cur = iter->cur->next;
    step--;
    #ifndef NDEBUG
      iter->cur->itercount++;
      if((iter->cur == &(((NAList*)naGetPtrConst(iter->listptr))->sentinel)) && (step != NA_ZERO)){
        naError("The iteration overflows the number of elements.");
      }
    #endif
  }
  while(step < NA_ZERO){
    #ifndef NDEBUG
      if(iter->cur->itercount == 0)
        naError("No Iterator at this element. Did you do a double clear?");
      iter->cur->itercount--;
    #endif
    iter->cur = iter->cur->prev;
    step++;
    #ifndef NDEBUG
      iter->cur->itercount++;
      if((iter->cur == &(((NAList*)naGetPtrConst(iter->listptr))->sentinel)) && (step != NA_ZERO)){
        naError("The iteration underflows the number of elements.");
      }
    #endif
  }
  return (iter->cur != &(((NAList*)naGetPtrConst(iter->listptr))->sentinel));
}



NA_IDEF const void* naGetListPrevConst(const NAListIterator* iter){
  #ifndef NDEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrConst(iter->cur->prev->ptr);
}


NA_IDEF void* naGetListPrevMutable(NAListIterator* iter){
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrMutable(iter->cur->prev->ptr);
}


NA_IDEF const void* naGetListCurConst(const NAListIterator* iter){
  #ifndef NDEBUG
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
    // if(naIsListEmpty(naGetPtrConst(iter->listptr)))
    //   naError("List is empty");
  #endif
  return naGetPtrConst(iter->cur->ptr);
}


NA_IDEF void* naGetListCurMutable(NAListIterator* iter){
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    // Note that the empty check has been removed. Getting the current elem
    // of an empty list automatically returns NULL. This is a feature!
    // if(naIsListEmpty(naGetPtrConst(iter->listptr)))
    //   naError("List is empty");
  #endif
  return naGetPtrMutable(iter->cur->ptr);
}


NA_IDEF const void* naGetListNextConst(const NAListIterator* iter){
  #ifndef NDEBUG
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrConst(iter->cur->next->ptr);
}


NA_IDEF void* naGetListNextMutable(NAListIterator* iter){
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsListEmpty((const NAList*)naGetPtrConst(iter->listptr)))
      naError("List is empty");
  #endif
  return naGetPtrMutable(iter->cur->next->ptr);
}




NA_IDEF void naAddListBeforeConst(NAListIterator* iter, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur->prev, iter->cur);
  naInjectConstListElement((NAList*)naGetPtrConst(iter->listptr), newelement, content);
}


NA_IDEF void naAddListBeforeMutable(NAListIterator* iter, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur->prev, iter->cur);
  naInjectMutableListElement((NAList*)naGetPtrMutable(iter->listptr), newelement, content);
}


NA_IDEF void naAddListAfterConst(NAListIterator* iter, const void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur, iter->cur->next);
  naInjectConstListElement((NAList*)naGetPtrConst(iter->listptr), newelement, content);
}


NA_IDEF void naAddListAfterMutable(NAListIterator* iter, void* content){
  NAListElement* newelement;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  newelement = naNewListElement(iter->cur, iter->cur->next);
  naInjectMutableListElement((NAList*)naGetPtrMutable(iter->listptr), newelement, content);
}



NA_IDEF void naRemoveListPrevConst(NAListIterator* iter){
  NAList* list;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor iter");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #ifndef NDEBUG
    if(iter->cur == list->sentinel.next)
      naError("There is no previous element. Iterator is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  naEjectListConst(list, iter->cur->prev, NA_TRUE);
}


NA_IDEF void* naRemoveListPrevMutable(NAListIterator* iter){
  NAList* list;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #ifndef NDEBUG
    if(iter->cur == list->sentinel.next)
      naError("There is no previous element. Internal pointer is at first position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naEjectListMutable(list, iter->cur->prev, NA_TRUE);
}


NA_IDEF void naRemoveListCurConst(NAListIterator* iter, NABool advance){
  NAList* list;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
    if(iter->cur == &(list->sentinel))
      naError("No current internal pointer is set. Major memory corruption expected...");
  #endif
  newelem = advance ? iter->cur->next : iter->cur->prev;
  #ifndef NDEBUG
    iter->cur->itercount--;
  #endif
  naEjectListConst(list, iter->cur, NA_TRUE);
  iter->cur = newelem;
  #ifndef NDEBUG
    iter->cur->itercount++;
  #endif
}


NA_IDEF void* naRemoveListCurMutable(NAListIterator* iter, NABool advance){
  NAList* list;
  NAListElement* newelem;
  void* returncontent;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #ifndef NDEBUG
    if(list->count == 0)
      naError("List is empty");
    if(iter->cur == &(list->sentinel))
      naError("No current internal pointer is set. Major memory corruption expected...");
    if(iter->cur->itercount == 0)
      naError("Internal error. No iterators registered at element which iterator is located at now");
  #endif
  newelem = advance ? iter->cur->next : iter->cur->prev;
  #ifndef NDEBUG
    iter->cur->itercount--;
  #endif
  returncontent = naEjectListMutable(list, iter->cur, NA_TRUE);
  iter->cur = newelem;
  #ifndef NDEBUG
    iter->cur->itercount++;
  #endif
  return returncontent;
}


NA_IDEF void naRemoveListNextConst(NAListIterator* iter){
  NAList* list;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #ifndef NDEBUG
    if(iter->cur == list->sentinel.prev)
      naError("There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  naEjectListConst(list, iter->cur->next, NA_TRUE);
}


NA_IDEF void* naRemoveListNextMutable(NAListIterator* iter){
  NAList* list;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  list = (NAList*)naGetPtrMutable(iter->listptr);
  #ifndef NDEBUG
    if(iter->cur == list->sentinel.prev)
      naError("There is no next element. Internal pointer is at last position. Major memory corruption expected...");
    if(list->count == 0)
      naError("List is empty");
  #endif
  return naEjectListMutable(list, iter->cur->next, NA_TRUE);
}



// ////////////////////////////
// GETTING POSITION INFORMATION



NA_IDEF NABool naIsListAtFirst(const NAListIterator* iter){
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
  #endif
  return (list->sentinel.next == iter->cur);
}



NA_IDEF NABool naIsListAtLast(const NAListIterator* iter){
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  #ifndef NDEBUG
    if(naIsListEmpty(list))
      naError("List is empty");
  #endif
  return (list->sentinel.prev == iter->cur);
}



NA_IDEF NABool naIsListAtInitial(const NAListIterator* iter){
  const NAList* list = (const NAList*)naGetPtrConst(iter->listptr);
  return (&(list->sentinel) == iter->cur);
}




NA_IDEF void naMoveListCurToFirst(NAListIterator* srciter, NABool advance, NAList* dst){
  NAList* src;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!srciter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srciter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srciter->listptr);
  #ifndef NDEBUG
    if(srciter->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif

  newelem = advance ? srciter->cur->next : srciter->cur->prev;
  #ifndef NDEBUG
    srciter->cur->itercount--;
    if(srciter->cur->itercount)
      naError("element has other iterators running.");
  #endif
  naEjectListConst(src, srciter->cur, NA_FALSE);
  srciter->cur->next = dst->sentinel.next;
  srciter->cur->prev = &(dst->sentinel);
  naInjectExistingListElement(dst, srciter->cur);
  srciter->cur = newelem;
  #ifndef NDEBUG
    srciter->cur->itercount++;
  #endif

}



NA_IDEF void naMoveListCurToLast(NAListIterator* srciter, NABool advance, NAList* dst){
  NAList* src;
  NAListElement* newelem;
  #ifndef NDEBUG
    if(!srciter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srciter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srciter->listptr);
  #ifndef NDEBUG
    if(srciter->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif

  newelem = advance ? srciter->cur->next : srciter->cur->prev;
  #ifndef NDEBUG
    srciter->cur->itercount--;
    if(srciter->cur->itercount)
      naError("element has other iterators running.");
  #endif
  naEjectListConst(src, srciter->cur, NA_FALSE);
  srciter->cur->next = &(dst->sentinel);
  srciter->cur->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, srciter->cur);
  srciter->cur = newelem;
  #ifndef NDEBUG
    srciter->cur->itercount++;
  #endif

}



NA_IDEF void naMoveListRemainingToLast(NAListIterator* srciter, NAList* dst){
  NAList* src;
  NAListElement* element;
  #ifndef NDEBUG
    NAListElement* testelem;
    if(!srciter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srciter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srciter->listptr);

  if(!naIsListEmpty(src)){
    NAInt movecount = 1;

    #ifndef NDEBUG
      srciter->cur->itercount--;
    #endif

    // Move to the first element if the list is rewinded.
    element = srciter->cur;
    if(element == &(src->sentinel)){
      element = src->sentinel.next;
    }

    #ifndef NDEBUG
      // Test all remaining elements for iterators
      testelem = srciter->cur;
      while(testelem != &(src->sentinel)){
        if(testelem->itercount)
          naError("Element still has an iterator");
        testelem = testelem->next;
      }
    #endif

    // Reroute the cur element from src to dst
    element->prev->next = &(src->sentinel);
    src->sentinel.prev = element->prev;
    srciter->cur = &(src->sentinel);

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
      srciter->cur->itercount++;
    #endif
  }
}



NA_IDEF void naMoveListThisToFirst(NAListIterator* srciter, NAList* dst){
  NAList* src;
  #ifndef NDEBUG
    if(!srciter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srciter->listptr))
      naError("Trying to modify with no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srciter->listptr);
  #ifndef NDEBUG
    if(srciter->cur == &(src->sentinel))
      naError("List iterator does not point to any element. No ''This'' available.");
  #endif

  #ifndef NDEBUG
    if(!srciter->cur->itercount)
      naError("Count is already 0");
    if(!src->itercount)
      naError("Count is already 0");
    srciter->cur->itercount--;
    src->itercount--;
  #endif
  naEjectListConst(src, srciter->cur, NA_FALSE);
  srciter->cur->next = dst->sentinel.next;
  srciter->cur->prev = &(dst->sentinel);
  naInjectExistingListElement(dst, srciter->cur);
  naCleanupPtr(&(srciter->listptr), NA_NULL);
  srciter->listptr = naMakePtrWithDataMutable(dst);
  #ifndef NDEBUG
    srciter->cur->itercount++;
    dst->itercount++;
  #endif
}



NA_IDEF void naMoveListThisToLast(NAListIterator* srciter, NAList* dst){
  NAList* src;
  #ifndef NDEBUG
    if(!srciter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(srciter->listptr))
      naError("Trying to modify with no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(srciter->listptr);
  #ifndef NDEBUG
    if(srciter->cur == &(src->sentinel))
      naError("List iterator does not point to any element. No ''This'' available.");
  #endif

  #ifndef NDEBUG
    if(!srciter->cur->itercount)
      naError("Count is already 0");
    if(!src->itercount)
      naError("Count is already 0");
    srciter->cur->itercount--;
    src->itercount--;
  #endif
  naEjectListConst(src, srciter->cur, NA_FALSE);
  srciter->cur->next = &(dst->sentinel);
  srciter->cur->prev = dst->sentinel.prev;
  naInjectExistingListElement(dst, srciter->cur);
  naCleanupPtr(&(srciter->listptr), NA_NULL);
  srciter->listptr = naMakePtrWithDataMutable(dst);
  #ifndef NDEBUG
    srciter->cur->itercount++;
    dst->itercount++;
  #endif
}



NA_IDEF void naExchangeListParts(NAListIterator* iter){
  NAList* src;
  NAListElement* first;
  NAListElement* prev;
  #ifndef NDEBUG
    if(!iter->mutator)
      naError("Trying to mutate elements with an accessor");
    if(naIsPtrConst(iter->listptr))
      naError("Trying to modify list while iterator is no modifier");
  #endif
  src = (NAList*)naGetPtrMutable(iter->listptr);

  #ifndef NDEBUG
    if(iter->cur == &(src->sentinel))
      naError("List has no current element set.");
  #endif
  if((&(src->sentinel) != iter->cur) && (src->sentinel.next != iter->cur)){
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


