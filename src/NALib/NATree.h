
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_TREE_INCLUDED
#define NA_TREE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// ///////////////////////////////////////////
// Trees
//
// This is the implementation of a general tree structure for various kinds.
// You use a NATreeConfiguration to define how the tree shall behave. Please
// read carefully about the callback methods below.
//
// Note that there is a basic distinction between trees with and without keys.
// Some APIs are only allowed for one of the kind. You will get an error when
// using NDEBUG.
//
// With this structure, it is possible to...
// - Store keys with inner nodes and leafes to automatically sort the tree.
// - Automatically balance a binary tree using AVL.
// - Store data for each inner node and/or leaf.
// - Search according to keys or, if no keys are available, any other kind.
// - Use iterators on the leafes.
//
// Later on, it is planned to also combine this implementation with NAQuadTree
// and NAOctTree.

#include "NAMemory.h"



// ///////////////////////////////////////////
// CALLBACKS
//
// When creating an NATree, you can create various callbacks which are stored
// in an NATreeConfiguration structure. Here are the signatures for these
// callbacks:
// ///////////////////////////////////////////


// NATreeContructorCallback and NATreeDestructorCallback
// Will be called whenever a tree is created or destroyed. The given parameter
// is the same as the data provided in NATreeConfiguration.
//
// Use this callback to handle potential registration of configdata,
// for example reference counting.
//
// Note that a new tree can be for example be implicitely created when another
// tree gets duplicated.
//
// The constructor will be called before the tree is initialized and the
// destructor is called after the tree has been cleared.
typedef void (*NATreeContructorCallback) (NAPtr configdata);
typedef void (*NATreeDestructorCallback) (NAPtr configdata);

// NATreeNodeConstructor
// Very much the same way as NATreeLeafConstructor, this callback is called
// when a tree creates an internal tree node other than a leaf.
// Internal nodes can NOT be manipulated directly but you are allowed to store
// any data with every node if desired. You can return an NAPtr to any data,
// even a Null pointer. This data pointer will be available to
// NATreeNodeDestructor and NATreeChildChanged.
// An internal node internally stores pointers to childnodes or leafes. The
// key parameter denotes the key the childnodes and leafes are sorted by.
// Note that the key is just here for information. You may or may not use it.
// All the important callback functions will provide the key again.
typedef NAPtr (*NATreeNodeConstructor)( const void* key,
                                              NAPtr configdata);

// NATreeNodeDestructor
// The node destructor is called before a tree ultimately deletes an internal
// node.
// The NAPtr created with NATreeNodeConstructor will be sent to this function
// such that you can deallocate the memory if necessary.
typedef void (*NATreeNodeDestructor)(NAPtr nodedata,
                                     NAPtr configdata);

// Gets called whenever childs of a parent change. The parent data is the
// data stored in the parent node. The childdatas parameter contains an
// array of the datas stored in all children. The childindx parameter denotes
// which child caused the updating or is -1 if no particular child caused the
// update. The childmask parameter is a bitfield denoting 1 for the child
// being a leaf and 0 for being an inner node.
typedef NABool (*NATreeNodeChildUpdater)  (NAPtr parentdata,
                                          NAPtr* childdatas,
                                           NAInt childindx,
                                           NAInt childmask);

// NATreeLeafConstructor
// Function which constructs the data stored in a leaf.
//
// This callback will be called when creating a new leaf. It allows you to
// process the given leafdata before storing. The return value will be stored
// in the tree. If this callback is Null, a leaf will simply store the
// leafdata pointer without any processing.
//
// The parameters are:
// - key        The key under which this leaf will be stored. Its type depends
//              on the setting in NATreeConfiguration. For example, can be
//              casted to a const double* when the key is a double key.
// - configdata The data you provided in NATreeConfiguration
// - leafdata   The data you expect to be stored.
//
// Use this callback function for example, if you want to store a reference
// counted pointer which shall be incremented before storing it in the tree.
// Or you can use the configdata to allocate a memory block from a user defined
// pool-structure and store the leafdata within, ultimately returning the
// memory block. Or maybe configdata is just an integer which counts the number
// of elements in the tree. Many more examples exist.
//
// Note that key and configdata are just here for information, you may or may not
// use them. Note that all the important callback functions will provide the
// key again.
//
// Use naGetPtrConst and naGetPtrMutable to read configdata and leafdata.
// Use naMakePtrWithDataConst and naMakePtrWithDataMutable to create the
// return value.
typedef NAPtr (*NATreeLeafConstructor)( const void* key,
                                              NAPtr configdata,
                                              NAPtr leafdata);

// NATreeLeafDestructor
// The deallocation function of your leafs. The leafdata pointer is the pointer
// returned when creating the leaf with NATreeLeafConstructor. The configdata
// is the same as the data provided in NATreeConfiguration.
typedef void (*NATreeLeafDestructor)(   NAPtr leafdata,
                                        NAPtr configdata);

// NATreeNodeTokenSearcher and NATreeLeafTokenSearcher
// For trees without keys, you can search using an arbitrary token. By calling
// naLocateTreeToken(), the following two callbacks will be called either for
// a node or a leaf given its stored data.
// The node callback shall test where to search next by setting the nextindx
// to the child index or -1 if the parent node shall be searched.
// The leaf callback shall test if the given leaf is the one having searched
// for, resulting in matchfound to be either true or false.
// Both callbacks shall return NA_TRUE if the search must continue or NA_FALSE
// if it shall be aborted.
// When the node callback aborts, the leaf is considered to be not found.
// When the leaf callback does not abort, automatically, the parent node is
// tested, completely ignoring the matchfound parameter.
typedef NABool (*NATreeNodeTokenSearcher)(  void* token,
                                            NAPtr data,
                                           NAInt* nextindx);
typedef NABool (*NATreeLeafTokenSearcher)(  void* token,
                                            NAPtr data,
                                          NABool* matchfound);



// Flags for NATreeConfiguration
//
// KEY_NOKEY        Set this flag for your nodes and leafes to have no key.
// KEY_DOUBLE       Set this flag for your keys to have the double type.
// KEY_NAINT        Set this flag for your keys to have the NAInt type.
// BALANCE_AVL      Makes the tree a self-balancing tree using the AVL method
#define NA_TREE_KEY_NOKEY   0x00
#define NA_TREE_KEY_DOUBLE  0x01
#define NA_TREE_KEY_NAINT   0x02
#define NA_TREE_BALANCE_AVL 0x10

typedef struct NATreeConfiguration NATreeConfiguration;
typedef struct NATree NATree;
typedef struct NATreeIterator NATreeIterator;


// ////////////////////
// NATreeConfiguration
//
// This is the callback struct you can use to create an NATree. Please read the
// extensive comments at the appropriate callback signatures to understand how
// this configuration must be filled. The flags are a combination of the macros
// defined above.
//
// If you try to change a configuration after it has been used for at least
// one tree, you will get a warning if NDEBUG is undefined.

NA_API  NATreeConfiguration* naCreateTreeConfiguration(NAInt flags);
NA_IAPI void naReleaseTreeConfiguration(NATreeConfiguration* config);

NA_IAPI void naSetTreeConfigurationTreeCallbacks(
  NATreeConfiguration*       config,
  NATreeContructorCallback   treeconstructor,
  NATreeDestructorCallback   treedestructor);

NA_IAPI void naSetTreeConfigurationLeafCallbacks(
  NATreeConfiguration*       config,
  NATreeLeafConstructor      leafconstructor,
  NATreeLeafDestructor       leafdestructor);

NA_IAPI void naSetTreeConfigurationNodeCallbacks(
  NATreeConfiguration*       config,
  NATreeNodeConstructor      nodeconstructor,
  NATreeNodeDestructor       nodedestructor,
  NATreeNodeChildUpdater     nodeChildUpdater);

NA_IAPI void naSetTreeConfigurationTokenCallbacks(
  NATreeConfiguration*       config,
  NATreeNodeTokenSearcher    nodeSearcher,
  NATreeLeafTokenSearcher    leafSearcher);



// ////////////////////
// NATree

// Creates, Empties and Clears a tree.
NA_IAPI NATree* naInitTree(NATree* tree, NATreeConfiguration* config);
NA_IAPI void naEmptyTree(NATree* tree);
NA_IAPI void naClearTree();

NA_IAPI NABool naIsTreeEmpty(const NATree* tree);

NA_IAPI NABool naAddTreeFirstConst  (NATree* tree, const void* content);
NA_IAPI NABool naAddTreeFirstMutable(NATree* tree,       void* content);
NA_IAPI NABool naAddTreeLastConst   (NATree* tree, const void* content);
NA_IAPI NABool naAddTreeLastMutable (NATree* tree,       void* content);

NA_IAPI const void* naGetTreeFirstConst  (const NATree* tree);
NA_IAPI void*       naGetTreeFirstMutable(const NATree* tree);
NA_IAPI const void* naGetTreeLastConst   (const NATree* tree);
NA_IAPI void*       naGetTreeLastMutable (const NATree* tree);

NA_IAPI void naUpdateTree(NATree* tree);

// ////////////////////
// NATreeIterator

// ///////////////////////////////
// Creating and positioning an iterator:
//
// The naMakeTreeXXX functions will create a new iterator which is
// positioned at the initial position of the tree. From there on, a tree
// can be searched forward or backward. You define, if the iterator can
// only access the contents, mutate them or even modify the whole tree.
// Add or remove functions for example will emit a warning if the iterator
// is not a modifier. Therefore, you can even savely transfer iterators
// to third-party programmers.
//
// You can reset any iterator to the initial state with the reset function.
//
// After you are done using the iterator, you should clear it with a call to
// naClearTreeIterator. NALib keeps track of where the iterators are when
// NDEBUG is undefined. Therefore, you will get lots of warnings if the
// iterators are not properly cleared. In the release code, no checks are
// performed.
NA_IAPI NATreeIterator naMakeTreeAccessor(const NATree* tree);
NA_IAPI NATreeIterator naMakeTreeMutator(NATree* tree);
NA_IAPI NATreeIterator naMakeTreeModifier(NATree* tree);
NA_IAPI void naResetTreeIterator(NATreeIterator* iter);
NA_IAPI void naClearTreeIterator(NATreeIterator* iter);

// Moves the iterator to the leaf containing the given key. If such a leaf is
// not found in the tree, NA_FALSE ist returned and iter returns to the initial
// state again.
// The assumeclose parameter indicates that the desired key is expected to be
// in the neighborhood of the current location. Do not set this flag if you
// access elements more randomly. The wrong value can make quite a difference
// in performance. Test it out!
NA_IAPI NABool naLocateTreeKey(     NATreeIterator* iter,
                                        const void* key,
                                             NABool assumeclose);
NA_IAPI NABool naLocateTreeFirst(   NATreeIterator* iter);
NA_IAPI NABool naLocateTreeLast(    NATreeIterator* iter);
NA_IAPI NABool naLocateTreeIterator(NATreeIterator* iter,
                                    NATreeIterator* srciter);
NA_IAPI NABool naLocateTreeToken   (NATreeIterator* iter,
                                              void* token);

// /////////////////////////////////
// Iterating
// Moves the iterator in order through the tree. The Back-variant moves the
// iterator backwards. Returns NA_FALSE when iteration is over.
NA_IAPI NABool naIterateTree        (NATreeIterator* iter);
NA_IAPI NABool naIterateTreeBack    (NATreeIterator* iter);

// /////////////////////////////////
// Returns the content of the current element without moving the iterator.
NA_IAPI const void* naGetTreeCurKey    (NATreeIterator* iter);
NA_IAPI const void* naGetTreeCurConst  (NATreeIterator* iter);
NA_IAPI void*       naGetTreeCurMutable(NATreeIterator* iter);

// Adds the given content to the tree under the given key.
// If there already exists an item with the given key, the given content will
// either replace the existing content or the given content will be discarded.
// When replacing, the existing node will be destructed completely and a new
// node will beconstructed with the new content.
// The functions return NA_FALSE if the existing element was kept. In any other
// case, NA_TRUE is returned. The iterator will afterwards always point at the
// tree leaf with the given key, no matter if it is the replaced content or
// the existing content.
NA_IAPI NABool naAddTreeKeyConst(   NATreeIterator* iter,
                                        const void* key,
                                        const void* content,
                                             NABool replace);
NA_IAPI NABool naAddTreeKeyMutable( NATreeIterator* iter,
                                        const void* key,
                                              void* content,
                                             NABool replace);

// Adds the given content at the specified position. Note that if the iterator
// is at initial position, "Next" behaves like "First" and "Prev" behaves like
// "Last".
NA_IAPI NABool naAddTreePrevConst(  NATreeIterator* iter,
                                        const void* content,
                                             NABool movetonew);
NA_IAPI NABool naAddTreePrevMutable(NATreeIterator* iter,
                                              void* content,
                                             NABool movetonew);
NA_IAPI NABool naAddTreeNextConst(  NATreeIterator* iter,
                                        const void* content,
                                             NABool movetonew);
NA_IAPI NABool naAddTreeNextMutable(NATreeIterator* iter,
                                              void* content,
                                             NABool movetonew);

// /////////////////////////////////
// Removes elements.
//
// The advance parameter denotes how the iterator shall move after the
// element has been removed. If advance is NA_FALSE, the iterator will be
// moved to the previous element. If advance is NA_TRUE, the iterator will
// be moved to the next element.
NA_IAPI void naRemoveTreeCur(NATreeIterator* iter, NABool advance);

// Returns true if the iterator is at its initial position.
NA_IAPI NABool naIsTreeAtInitial(NATreeIterator* iter);

// Debugging function: Returns true if there are other iterators visiting
// the same tree element as the given iterator.
#ifndef NDEBUG
  NA_IAPI NABool naIsTreeIteratorAlone(NATreeIterator* iter);
#endif

// Inline implementations are in a separate file:
#include "NAStruct/NATree/NATreeII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_TREE_INCLUDED


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
