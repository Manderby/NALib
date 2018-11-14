
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


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



// Flags for NATreeConfiguration
#define NA_TREE_KEY_DOUBLE 0


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

NA_API NATreeConfiguration* naCreateTreeConfiguration(NAInt flags);
NA_API void naReleaseTreeConfiguration(NATreeConfiguration* config);

NA_API void naSetTreeConfigurationTreeCallbacks(
  NATreeConfiguration*       config,
  NATreeContructorCallback   treeconstructor,
  NATreeDestructorCallback   treedestructor);

NA_API void naSetTreeConfigurationLeafCallbacks(
  NATreeConfiguration*       config,
  NATreeLeafConstructor      leafconstructor,
  NATreeLeafDestructor       leafdestructor);

NA_API void naSetTreeConfigurationNodeCallbacks(
  NATreeConfiguration*       config,
  NATreeNodeConstructor      nodeconstructor,
  NATreeNodeDestructor       nodedestructor);



// ////////////////////
// NATree

NA_API NATree* naInitTree(NATree* tree, NATreeConfiguration* config);
//NA_API NATree* naInitTreeCopy(NATree* dsttree, NATree* srctree);
NA_API void naEmptyTree(NATree* tree);
NA_API void naClearTree();




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
NA_IAPI NABool naLocateTree(NATreeIterator* iter, const void* key);

// /////////////////////////////////
// Iterating
// Moves the iterator in order through the tree. The Back-variant moves the
// iterator backwards. Returns NA_FALSE when iteration is over.
NA_IAPI NABool naIterateTree        (NATreeIterator* iter);
NA_IAPI NABool naIterateTreeBack    (NATreeIterator* iter);

// /////////////////////////////////
// Returns the content of the current element without moving the iterator.
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
NA_API NABool naAddTreeConst(    NATreeIterator* iter,
                                     const void* key,
                                     const void* content,
                                          NABool replace);
NA_API NABool naAddTreeMutable(  NATreeIterator* iter,
                                     const void* key,
                                           void* content,
                                          NABool replace);


// Inline implementations are in a separate file:
#include "NAStruct/NATree/NATreeII.h"




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
