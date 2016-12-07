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

NA_IAPI NAListPos naAddListBeforeConst   (NAList* list, const void* content);
NA_IAPI NAListPos naAddListBeforeMutable (NAList* list,       void* content);
NA_IAPI NAListPos naAddListAfterConst    (NAList* list, const void* content);
NA_IAPI NAListPos naAddListAfterMutable  (NAList* list,       void* content);


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

NA_IAPI const void* naGetListPrevConst      (const NAList* list);
NA_IAPI       void* naGetListPrevMutable    (const NAList* list);
NA_IAPI const void* naGetListCurrentConst   (const NAList* list);
NA_IAPI       void* naGetListCurrentMutable (const NAList* list);
NA_IAPI const void* naGetListNextConst      (const NAList* list);
NA_IAPI       void* naGetListNextMutable    (const NAList* list);
NA_IAPI const void* naGetListPositionConst  (const NAList* list, NAListPos listpos);
NA_IAPI       void* naGetListPositionMutable(const NAList* list, NAListPos listpos);


// /////////////////////////////////////////////
// Getting position informations around the internal pointer.
//
// The Prev and Next getter will return a Null-Pointer if the list is at the
// first or last element respectively.
//
// Note that threr are no First and Last getters as the returned NAListPos
// would be bound to the specific element rather to the position in the list.
// This would be misleading.
NA_IAPI NABool    naIsListAtFirst           (const NAList* list);
NA_IAPI NABool    naIsListAtLast            (const NAList* list);
NA_IAPI NABool    naIsListAtPosition        (const NAList* list, NAListPos listpos);

NA_IAPI NAListPos naGetListNextPosition     (const NAList* list, NAListPos listpos);
NA_IAPI NAListPos naGetListCurrentPosition  (const NAList* list);
NA_IAPI NAListPos naGetListPrevPosition     (const NAList* list, NAListPos listpos);


// ////////////////////////////////////
// Iterating a list
//
// Traverses the whole list and calls the mutator on each element. A pointer
// to each element will be given to the mutator. The list is traversed forward
// from first to last and the internal pointer will be rewinded.
NA_IAPI void    naForeachList       (NAList* list, NAFunc mutator); // done

// Moves the internal pointer forward or backwards the given number of positive
// or negative steps respectively. If the step over- or underflows the stored
// elements, the iteration stops. If NDEBUG is undefined, a warning is emitted.
//
// Returns NA_FALSE when one of the two ends of the list is reached.
NA_IAPI NABool  naIterateList       (const NAList* list, NAInt step);

// Positioning the internal pointer
//
// The following functions move the internal pointer to a desired element.
//
// RewindList:    The List is at its initial position. Iterating forward will
//                first access the first element, iterating backwards will
//                first access the last element.
// PositionList:  The internal pointer moves to the desired location. Such a
//                position is returned for example when adding a new element
//                to the list.
// LocateList:    A specific content pointer or index is searched within the
//                list. Note that these functions work very slow. If you really
//                need to locate elements within a list, try to go for the
//                position solution. But you need to store the NAListPos when
//                adding elements.
//
// If the given index is negative, it denotes the element from the end of the
// list, whereas -1 denotes the last element.
//
// The Pointer and Index variants will return NA_FALSE, if the element has not
// been found and NA_TRUE if it has been found. If not found, the list will be
// rewinded.
NA_IAPI void   naRewindList          (const NAList* list);
NA_IAPI void   naPositionList        (const NAList* list, NAListPos listpos);
NA_API  NABool naLocateListPointer   (const NAList* list, void* content);
NA_API  NABool naLocateListIndex     (const NAList* list, NAInt indx);


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
NA_IAPI void naMoveListCurToFirst (NAList* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListCurToLast  (NAList* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListCurToBefore(NAList* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListCurToAfter (NAList* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListFirstToLast(NAList* src, NABool advance, NAList* dst);
NA_IAPI void naMoveListTrailingToLast(NAList* src, NAList* dst);

// The exchange function splits the list into two parts BEFORE the current
// element and re-attaches the whole list before that at the end of the list.
// For example when D is current, ABCDEFG becomes DEFGABC.
// The current element will point to the first element after this function.
NA_IAPI void naExchangeListParts(NAList* list);






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


