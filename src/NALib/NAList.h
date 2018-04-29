
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_LIST_INCLUDED
#define NA_LIST_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// Following is the implementation of a double-chained list.
//
// You provide a pointer to your content which will be stored in the list.
// Important: You have to typecast the returned content-pointers!
//
// This implementation uses a so called sentinel which is stored in the list
// itself. Using such and using a double-chained list, implementation of the
// list becomes very easy and very fast, as there are almost no special cases
// to consider.
//
// Additionally, there exists an NAListIterator which allows you to iterate
// through the list easily and even remove elements while traversing the
// list. See below for a detailed explanation of the iteration functions.


#include "NABase.h"


// This file defines the following types:
// - NAList          Defines the struct storing a list.
// - NAListIterator  Defines the struct holding the iterator of a list.
//
// The full type definitions are in the file "NAListII.h"
typedef struct NAList         NAList;
typedef struct NAListIterator NAListIterator;


// Creates an empty list.
NA_IAPI NAList* naInitList    (NAList* list);

// Creates an exact copy of originallist by replicating all elements in the
// same order. Note that the contents are again only referenced by a pointer
// and will still be marked as const or mutable. The two parameters must
// not be the same. If you are using NAListIterator's  on the originallist,
// these iterators will keep the location of the original list.
NA_IAPI NAList* naCopyList    (NAList* list, NAList* originallist);

// Clears or empties the given list. Note: This will free all list elements
// but not the contents they store! Use naForeachList or iteration for that.
//
// The difference between naEmptyList and naClearList is that Clear should be
// called with a semantic of invalidating the list whereas Empty simply empties
// the list. Due to the implementation in NALib, this makes no difference but
// should nontheless be distinguished. For other container structs like NAHeap,
// this equality does not hold.
NA_IAPI void    naClearList   (NAList* list);
NA_IAPI void    naEmptyList   (NAList* list);

// Returns informations about the number of elements in this list.
NA_IAPI NAInt   naGetListCount(const NAList* list);
NA_IAPI NABool  naIsListEmpty (const NAList* list);


// ///////////////////////////
// Adding elements
//
// Adds the given content at the desired position of the list.
// First:   New element becomes first element of the list
// Last:    New element becomes last element of the list
// There are more options when using iterators. See below.
//
// You have to define whether you want to store a const or mutable element.
// If later on, you try to get a mutable element which was stored const, you
// will get an error when NDEBUG is undefined.
NA_IAPI void naAddListFirstConst    (NAList* list, const void* content);
NA_IAPI void naAddListFirstMutable  (NAList* list,       void* content);
NA_IAPI void naAddListLastConst     (NAList* list, const void* content);
NA_IAPI void naAddListLastMutable   (NAList* list,       void* content);


// /////////////////////////////
// Removing elements
//
// Remove functions. Removes the desired element from the list.
//
// Note: Remove functions free the memory of the underlying list element
// structure but do not free the memory of the content! But the mutable
// variants return a pointer to the previously stored content such that you
// can erase it if you need to.
//
// The naRemoveListData function is a convenience function which creates
// an iterator, searches for the given content and removes it. This would
// take several lines to code but is much simpler when written as one function.
// Warning: This function is potentially very slow.
NA_IAPI void  naRemoveListFirstConst    (NAList* list);
NA_IAPI void* naRemoveListFirstMutable  (NAList* list);
NA_IAPI void  naRemoveListLastConst     (NAList* list);
NA_IAPI void* naRemoveListLastMutable   (NAList* list);
NA_IAPI void  naRemoveListData          (NAList* list, void* data);


// /////////////////////////////////////////////
// Accessing elements
//
// The desired element is returned.
NA_IAPI const void* naGetListFirstConst     (const NAList* list);
NA_IAPI       void* naGetListFirstMutable   (const NAList* list);
NA_IAPI const void* naGetListLastConst      (const NAList* list);
NA_IAPI       void* naGetListLastMutable    (const NAList* list);


// Traverses the whole list calling the accessor or mutator on each element.
// A pointer to each element will be given to the mutator. The list is
// traversed forward from start to end.
NA_IAPI void naForeachListConst  (const NAList* list, NAAccessor accessor);
NA_IAPI void naForeachListMutable(const NAList* list, NAMutator  mutator);


// Removes the first element of the src list and reattaches it as the last
// element of dst list.
//
// Note that there is no memory allocation or deallocation used in the process!
NA_IAPI void naMoveListFirstToLast(NAList* src, NAList* dst);






// ////////////////////////////////////
// Iterating a list
//
// Up to NALib verison 16, NAList contained a built-in iterator. But starting
// with NALib Version 17, there exists NAListIterator, a structure separate
// from a list. It gives the programmer way more flexibility to move freely
// within a list.
//
// To keep the API clean, the function names do not use "ListIterator" but
// instead simply use "List" as if the iterators would denote the list per se.
//
// Having a separate iterator has several advantages. It may need a little bit
// more coding but you can be way more flexible. Note that for example removing
// an element will test if there are other iterators using that element when
// NDEBUG is undefined. This is very useful in a multithreaded environment.
//
// The easiest way to implement an iteration is using a while loop:
//
// NAList* mylist;
// NALIstIterator iter = naMakeListIterator(mylist);
// while(naIterateList(&iter, 1)){
//   void* curelement = naGetListCurMutable(iter);
//   Do stuff with curelement.
// }
// naClearListIterator(&iter);
//
// Be sure to not forget naClearListIterator. Otherwise when debugging, lists
// will keep references to iterators which are no longer in use and will hence
// emit a warning when they are cleared. When NDEBUG is defined however, no
// such checks will be performed.


// ///////////////////////////////
// Creating and positioning an iterator:
//
// The naMakeListIteratorXXX functions will create a new iterator which is
// positioned at the initial position of the list. From there on, a list
// can be searched forward or backward. You define, if the iterator can
// only access the contents, mutate them or even modify the whole list.
// Add or remove functions for example will emit a warning if the iterator
// is not a modifier. Therefore, you can even savely transfer iterators
// to third-party programmers.
//
// You can reset any iterator to the initial state with the Reset function.
//
// After you are done using the iterator, you should clear it with a call to
// naClearListIterator. NALib keeps track of where the iterators are when
// NDEBUG is undefined. Therefore, you will get lots of warnings if the
// iterators are not properly cleared. In the release code, no checks are
// performed.
NA_IAPI NAListIterator naMakeListAccessor(const NAList* list);
NA_IAPI NAListIterator naMakeListMutator (const NAList* list);
NA_IAPI NAListIterator naMakeListModifier(      NAList* list);

NA_IAPI void naResetListIterator(NAListIterator* iterator);

NA_IAPI void naClearListIterator(NAListIterator* iterator);

// After having created an iterator, it is at the initial position but you may
// position it to a desired element by using one of the Locate functions:
//
// First:         Moves iterator to first element. This function is very fast.
// Last:          Moves iterator to last element. This function is very fast.
// Data:          A specific content pointer is searched within the list. This
//                function is very slow. If you often need to locate elements
//                within a list, try to store iterators.
// Index:         The element with the specified index is searched. If the given
//                index is negative, it denotes the element from the end of the
//                list, whereas -1 denotes the last element. Note that this
//                function is slow, especially when searching for elements in
//                the middle of the list.
//
// The Data and Index variants will return NA_FALSE, if the element has not
// been found and NA_TRUE if it has been found. If not found, the iterator will
// point to the initial list position.
// 
// The other variants will always return NA_TRUE. That's because of speed.
// Erroneous behaviour will only be checked when NDEBUG is undefined and hence
// the programmer is already forced to do things right when debugging the code.
NA_IAPI NABool naLocateListFirst    (NAListIterator* iter);
NA_IAPI NABool naLocateListLast     (NAListIterator* iter);
NA_API  NABool naLocateListData     (NAListIterator* iter, const void* data);
NA_API  NABool naLocateListIndex    (NAListIterator* iter, NAInt indx);

// Positions dstiter at the same element as srciter.
NA_IAPI void   naLocateListIterator (NAListIterator* dstiter,
                                     NAListIterator* srciter);


// /////////////////////////////////
// Iterating
// Moves the iterator forward or backward the given number of positive or
// negative steps respectively. If the step over- or underflows the stored
// elements and NDEBUG is undefined, a warning is emitted.
//
// Returns NA_FALSE when one of the two ends of the list is reached.
NA_IAPI NABool naIterateList        (NAListIterator* iter, NAInt step);


// /////////////////////////////////
// Returns the content of the previous, current or next element without moving
// the iterator.
//
// If the iterator is at a position where the desired element does not exist
// (for example the next element when being at the last element), a Null-Pointer
// will be returned without warning.
NA_IAPI const void* naGetListPrevConst      (const NAListIterator* iter);
NA_IAPI       void* naGetListPrevMutable    (      NAListIterator* iter);
NA_IAPI const void* naGetListCurConst       (const NAListIterator* iter);
NA_IAPI       void* naGetListCurMutable     (      NAListIterator* iter);
NA_IAPI const void* naGetListNextConst      (const NAListIterator* iter);
NA_IAPI       void* naGetListNextMutable    (      NAListIterator* iter);


// /////////////////////////////////
// Adds elements relative to the current position within a list.
// Before:  New element becomes element before the current element.
// After:   New element becomes element after the current element.
//
// If the iterator is at initial position, Before behaves the same as Last
// and After behaves the same as First. This is a convenience when during
// iteration, the iterator came to one of the two ends of a list but you
// still want to add an element.
//
// Note: The iterator is not moved at all when adding elements.
NA_IAPI void naAddListBeforeConst   (NAListIterator* iter, const void* content);
NA_IAPI void naAddListBeforeMutable (NAListIterator* iter,       void* content);
NA_IAPI void naAddListAfterConst    (NAListIterator* iter, const void* content);
NA_IAPI void naAddListAfterMutable  (NAListIterator* iter,       void* content);


// /////////////////////////////////
// Removes elements relative from the current position within the list.
//
// The advance parameter denotes how the iterator shall move after the
// element has been removed. If advance is NA_FALSE, the iterator will be
// moved to the previous element. If advance is NA_TRUE, the iterator will
// be moved to the next element.
//
// The same way as when adding, when the iterator is at initial position,
// Prev works the same as Last and Next works the same as First.
NA_IAPI void  naRemoveListPrevConst     (NAListIterator* iter);
NA_IAPI void* naRemoveListPrevMutable   (NAListIterator* iter);
NA_IAPI void  naRemoveListCurConst      (NAListIterator* iter, NABool advance);
NA_IAPI void* naRemoveListCurMutable    (NAListIterator* iter, NABool advance);
NA_IAPI void  naRemoveListNextConst     (NAListIterator* iter);
NA_IAPI void* naRemoveListNextMutable   (NAListIterator* iter);


// /////////////////////////////////////////////
// Getting position informations around the current position.
//
// The Prev and Next getter will return a Null-Pointer if the list is at the
// first or last element respectively.
NA_IAPI NABool    naIsListAtFirst           (const NAListIterator* iter);
NA_IAPI NABool    naIsListAtLast            (const NAListIterator* iter);
NA_IAPI NABool    naIsListAtInitial         (const NAListIterator* iter);


// /////////////////////////////////////////////
// Reordering functions
//
// The move functions remove the specified element from the list pointed to by
// srciter and move them to the dst list at the position indicated by the
// function name. These functions are VERY FAST! Way faster than locating a
// specific element, removing it and re-adding it. Additionally, there is no
// memory allocation or deallocation used in the process.
//
// The Remaining function will move all elements starting with the current
// element till the last to the new list. If the iterator is at initial
// position, the whole list will be moved. The iterator of the src list will
// then be unset.
//
// The "This" functions will move the current element but also will alter the
// iterator itself to point to the new list. These functions are very useful
// in combination with an iterator stored within a struct which denotes the
// current position of that struct within a list. So you can arbitrarily move
// such elements from one list to another without the need to search for it.
//
// Use these functions in conjunction with naLocateList. It is the fastest
// way to transfer specific elements from one list to another.
//
// The advance parameter behaves the same as with the remove functions.
//
// The dstlist can be the same as srclist.
NA_IAPI void naMoveListCurToFirst (     NAListIterator* srciter,
                                                 NABool advance,
                                                NAList* dst);
NA_IAPI void naMoveListCurToLast  (     NAListIterator* srciter,
                                                 NABool advance,
                                                NAList* dst);
NA_IAPI void naMoveListRemainingToLast( NAListIterator* srciter,
                                                NAList* dst);
NA_IAPI void naMoveListThisToFirst (    NAListIterator* srciter,
                                                NAList* dst);
NA_IAPI void naMoveListThisToLast (     NAListIterator* srciter,
                                                NAList* dst);

// The exchange function splits the list into two parts right BEFORE the
// current element and re-attaches the whole list before that at the end of
// the list. For example when D is current, ABCDEFG becomes DEFGABC.
// The iterator will point to the first element after this function.
NA_IAPI void naExchangeListParts(       NAListIterator* iter);



// Inline implementations are in a separate file:
#include "NAStruct/NAListII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_LIST_INCLUDED


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


