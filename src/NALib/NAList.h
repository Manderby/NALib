// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_LIST_INCLUDED
#define NA_LIST_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// Following is the implementation of a double-chained list with built-in
// iteration. You provide a pointer to your content and this content will be
// stored in the list.
//
// Important: You have to typecast the returned content-pointers!
//
// This implementation uses a so called sentinel which is stored in the list
// itself. Using such and using a double-chained list, implementation of the
// list becomes very easy and very fast, as there are almost no special cases
// to consider.
//
// Additionally, a list stores an internal pointer to the current element which
// allows you to iterate through the list easily and even remove elements while
// traversing the list. Changing the iterator is not considered changing the
// list. Be aware!
//
// Iterating a list is can be done using various function calls.
// The easiest way to implement an iteration is using a while loop:
//
// NAList* mylist;
// naRewindList(mylist);
// while(naIterateList(mylist, 1)){
//   void* curelement = naGetListCurrentMutable(mylist);
//   Do stuff with curelement.
// }
//
// See below for a detailed explanation of the different functions.


#include "NASystem.h"


// The full type definitions are in the file "NAListII.h"
typedef struct NAList NAList;
typedef struct NAListIterator NAListIterator;
typedef const void* NAListPos;


// Creates an empty list.
NA_IAPI NAList* naInitList    (NAList* list);

// Creates an exact copy of originallist by replicating all elements in the
// same order. Not that the contents are again only referenced by a pointer
// and will still be marked as const or mutable. The two list pointers must
// not be the same. If you are using NAListPos locators on the originallist,
// the locators will keep the location of the original list.
NA_IAPI NAList* naCopyList    (NAList* list, NAList* originallist);

// Clears or empties the given list. Note: This will free all list elements
// but not the contents they store! Use naForeachList or iteration for that.
//
// The difference between naEmptyList and naClearList is that Clear should be
// called with a semantic of invalidating the list whereas Empty simply empties
// the list. Due to the implementation in NALib, this makes no difference but
// should nontheless be distinguished. For other datastructures like NAHeap,
// this equality does not hold.
NA_IAPI void    naClearList   (NAList* list);
NA_IAPI void    naEmptyList   (NAList* list);

// Returns informations about the number of elements in this list.
NA_IAPI NAUInt  naGetListCount(const NAList* list);
NA_IAPI NABool  naIsListEmpty (const NAList* list);


// ///////////////////////////
// Adding elements
//
// Adds the given content at the desired position of the list.
// First:   New element becomes first element of the list
// Last:    New element becomes last element of the list
// Before:  New element becomes element before the current element.
// After:   New element becomes element after the current element.
//
// If the internal pointer is not set, Before behaves the same as Last
// and After behaves the same as First. This is a convenience when during
// iteration, the internal pointer came to the end of the list but you
// still want to add an element.
//
// Note: The internal pointer is not altered at all when adding elements.
//
// The return value is a position locator which the user can store (for example
// within the element itself) for later use to quickly access specific elements
// within a list or move an element within or even between lists. If you have
// no intention to use that value, you may simply ignore it. This locator will
// always reflect the current position and list the element is stored in, as
// long as the element is not explicitely removed from a list. You do not need
// to adjust it in any way.
NA_IAPI NAListPos naAddListFirstConst    (NAList* list, const void* content);
NA_IAPI NAListPos naAddListFirstMutable  (NAList* list,       void* content);
NA_IAPI NAListPos naAddListLastConst     (NAList* list, const void* content);
NA_IAPI NAListPos naAddListLastMutable   (NAList* list,       void* content);

NA_IAPI NAListPos naGetListInitialPosition(NAList* list);
NA_IAPI NAListPos naGetListFirstPosition  (NAList* list);
NA_IAPI NAListPos naGetListLastPosition   (NAList* list);


// /////////////////////////////
// Removing elements
//
// Remove functions. Removes the desired element from the list.
// The advance parameter denotes how the internal pointer shall move in case
// the internal pointer points to the element which is removed. If advance is
// NA_FALSE, the internal pointer will be moved to the previous element. If
// advance is NA_TRUE, the internal pointer will be moved to the next element.
//
// Very much the same as when adding, when the internal pointer is not set,
// Prev works the same as Last and Next works the same as First. But removing
// the current element does not work and a warning will be emitted if NDEBUG
// is undefined. If the Prev is called when the internal pointer is on the
// first element, or if Next is called when on the last element, a warning is
// emittet. Also, a warning will be emitted on any remove function if
// the list is empty.
//
// Note: Remove functions free the memory of the underlying list element
// structure but do not free the memory of the content! But the mutable
// variants return a pointer to the previously stored content such that you
// can erase it if you need to.
NA_IAPI void  naRemoveListFirstConst    (NAList* list);
NA_IAPI void* naRemoveListFirstMutable  (NAList* list);
NA_IAPI void  naRemoveListLastConst     (NAList* list);
NA_IAPI void* naRemoveListLastMutable   (NAList* list);



// /////////////////////////////////////////////
// Accessing elements
//
// Beware! These functions are no iteration functions. The internal pointer
// is NOT moved. Just the desired element is returned. For the Position
// functions, use the given NAListPos argument to retrieve the desired element.
//
// If the current internal pointer is at a position where the desired element
// does not exist (for example the next element when being at the last element),
// a Null-Pointer will be returned without warning.
//
// A warning will be emitted if the list is empty.
NA_IAPI const void* naGetListFirstConst     (const NAList* list);
NA_IAPI       void* naGetListFirstMutable   (const NAList* list);
NA_IAPI const void* naGetListLastConst      (const NAList* list);
NA_IAPI       void* naGetListLastMutable    (const NAList* list);

NA_IAPI const void* naGetListPositionConst  (const NAList* list, NAListPos listpos);
NA_IAPI       void* naGetListPositionMutable(const NAList* list, NAListPos listpos);


// Traverses the whole list and calls the mutator on each element. A pointer
// to each element will be given to the mutator. The list is traversed forward
// from first to last and the internal pointer will be rewinded.
NA_IAPI void    naForeachList       (NAList* list, NAFunc mutator); // done

NA_IAPI void naMoveListFirstToLast(NAList* src, NAList* dst);






// ////////////////////////////////////
// Iterating a list
//


// Creating and positioning an iterator:
//
// The naMakeListIteratorXXX functions will create a new iterator which is
// positioned at the initial position of the list. From there on, a list
// can be searched forward or backwards. You define, if the iterator can
// mutate the contents and return mutable elements from the list. If an
// iterator is created const, add or remove functions will emit a warning
// when NDEBUG is undefined.
//
// After you are done using the iterator, you should clear it with a call to
// naClearListIterator. NALib keeps track of where the iterators are when
// NDEBUG is undefined. Therefore, you will get lots of warnings if the
// iterators are not properly cleared. In the release code, no checks are
// performed.

NA_IAPI NAListIterator naMakeListIteratorConst(const NAList* list);
NA_IAPI NAListIterator naMakeListIteratorMutable(NAList* list);

NA_IAPI void naClearListIterator(NAListIterator* listiterator);

// After having created an iterator, you may position it to a desired element.
// By using one of the naLocateListXXX functions:
//
// Position:      The iterator moves to the desired location. Such a position
//                is returned for example when adding a new element to the list.
//                This function is extremely fast.
// Content:       A specific content pointer is searched within the list. This
//                function is very slow. If you often need to locate elements
//                within a list, try to go for the Position solution. But you
//                need to store the NAListPos when adding elements.
// Index:         The element with the specified index is searched. If the given
//                index is negative, it denotes the element from the end of the
//                list, whereas -1 denotes the last element. Note that this
//                function is slow, especially when searching for elements in
//                the middle of the list.
//
// The Content and Index variants will return NA_FALSE, if the element has not
// been found and NA_TRUE if it has been found. If not found, the iterator will
// point to the initial list position. The Position variant will always return
// NA_TRUE.

NA_IAPI NABool naLocateListFirst    (NAListIterator* listiterator);
NA_IAPI NABool naLocateListLast     (NAListIterator* listiterator);
NA_IAPI NABool naLocateListPosition (NAListIterator* listiterator, NAListPos listpos);
NA_API  NABool naLocateListContent  (NAListIterator* listiterator, const void* content);
NA_API  NABool naLocateListIndex    (NAListIterator* listiterator, NAInt indx);

// Moves the internal pointer forward or backwards the given number of positive
// or negative steps respectively. If the step over- or underflows the stored
// elements and NDEBUG is undefined, a warning is emitted.
//
// Returns NA_FALSE when one of the two ends of the list is reached.
NA_IAPI NABool  naIterateList    (NAListIterator* listiterator, NAInt step);

// Returns the content.
NA_IAPI const void* naGetListPrevConst      (NAListIterator* listiterator);
NA_IAPI       void* naGetListPrevMutable    (NAListIterator* listiterator);
NA_IAPI const void* naGetListCurrentConst   (NAListIterator* listiterator);
NA_IAPI       void* naGetListCurrentMutable (NAListIterator* listiterator);
NA_IAPI const void* naGetListNextConst      (NAListIterator* listiterator);
NA_IAPI       void* naGetListNextMutable    (NAListIterator* listiterator);

// Adds elements relative to the current position within a list.
NA_IAPI NAListPos naAddListBeforeConst   (NAListIterator* listiterator, const void* content);
NA_IAPI NAListPos naAddListBeforeMutable (NAListIterator* listiterator,       void* content);
NA_IAPI NAListPos naAddListAfterConst    (NAListIterator* listiterator, const void* content);
NA_IAPI NAListPos naAddListAfterMutable  (NAListIterator* listiterator,       void* content);

// Removes elements relative from the current position within the list.
NA_IAPI void  naRemoveListPrevConst     (NAListIterator* listiterator);
NA_IAPI void* naRemoveListPrevMutable   (NAListIterator* listiterator);
NA_IAPI void  naRemoveListCurrentConst  (NAListIterator* listiterator, NABool advance);
NA_IAPI void* naRemoveListCurrentMutable(NAListIterator* listiterator, NABool advance);
NA_IAPI void  naRemoveListNextConst     (NAListIterator* listiterator);
NA_IAPI void* naRemoveListNextMutable   (NAListIterator* listiterator);



// /////////////////////////////////////////////
// Getting position informations around the internal pointer.
//
// The Prev and Next getter will return a Null-Pointer if the list is at the
// first or last element respectively.
//
// Note that threr are no First and Last getters as the returned NAListPos
// would be bound to the specific element rather to the position in the list.
// This would be misleading.
NA_IAPI NABool    naIsListIteratorEmpty     (NAListIterator* listiterator);
NA_IAPI NABool    naIsListAtFirst           (NAListIterator* listiterator);
NA_IAPI NABool    naIsListAtLast            (NAListIterator* listiterator);
NA_IAPI NABool    naIsListAtPosition        (NAListIterator* listiterator, NAListPos listpos);

NA_IAPI NAListPos naGetListPrevPosition     (NAListIterator* listiterator);
NA_IAPI NAListPos naGetListCurrentPosition  (NAListIterator* listiterator);
NA_IAPI NAListPos naGetListNextPosition     (NAListIterator* listiterator);

// /////////////////////////////////////////////
// Reordering functions
//
// The move functions remove the specified element from the src list and move
// them to the dst list at the position indicated by the function name. These
// functions are VERY FAST! Way faster than locating a specific element,
// removing it and re-adding it. Additionally, there is no memory allocation
// or deallocation used in the process.
//
// The Trailing function will move all elements starting with the current
// element till the last to the new list. If the list is rewinded, the whole
// list will be moved. The cur pointer of the src list will be unset.
//
// Use these functions in conjunction with naPositionList. It is the fastest
// way to transfer specific elements from one list to another.
//
// The advance parameter behaves the same as with the remove functions.
//
// The dstlist can be the same as srclist.
//
// See implementation for more details.
NA_IAPI void naMoveListCurToFirst (NAListIterator* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListCurToLast  (NAListIterator* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListTrailingToLast(NAListIterator* listiterator, NAList* dst);

// The exchange function splits the list into two parts BEFORE the current
// element and re-attaches the whole list before that at the end of the list.
// For example when D is current, ABCDEFG becomes DEFGABC.
// The current element will point to the first element after this function.
NA_IAPI void naExchangeListParts(NAListIterator* listiterator);



// Inline implementations are in a separate file:
#include "NAStruct/NAListII.h"




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


