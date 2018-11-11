
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
                                              NAPtr configdata,
                                              NAPtr nodedata);

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

NA_API NATreeIterator naMakeTreeAccessor(const NATree* tree);
NA_API NATreeIterator naMakeTreeMutator(NATree* tree);
NA_API NATreeIterator naMakeTreeModifier(NATree* tree);
NA_API void naClearTreeIterator(NATreeIterator* iter);

NA_API void naResetTreeIterator(NATreeIterator* iter);

// Moves the iterator to the leaf containing the given key. If such a leaf is
// not found in the tree, NA_FALSE ist returned. The iterator though stores the
// coord it is supposed to point at. This allows you to locate a key and then
// use naGetTreeMutable to create a leaf with that key.
NA_API NABool naLocateTree(NATreeIterator* iter, const void* key);

NA_API const void* naGetTreeConst(NATreeIterator* iter);

// Locates or inserts a leaf with a specific key in the tree. You can access
// the leaf with a successing call to naGetTreeCur.
//
// Locate:  Just move the iterator to the leaf.
// Insert:  Move the iterator to the leaf. If non-existent, create it with the
//          leafdata given. If existent, does not change the contents but
//          emits a warning in debug mode.
// Replace: Move the iterator to the leaf. If non-existent, create it with the
//          leafdata given. If existent, replace it with a new leaf containing
//          the leafdata given.
//
// Note that when a new leaf is created and the configuration defines a leaf
// constructor, the leafdata given will be available in the constructor.
// If so, accessing the leaf afterwards with naGetTreeCur will get you the leaf
// you created in the construtor, not the leafdata.
//
// When replacing an existing leaf and having both a constructor and destructor
// for leafes defined, the new leaf is first constructed, then the old one is
// destructed.
//
// All functions return true, if the leaf did exist, otherwise false.

//NA_API NABool naInsertTreeConst   (NATreeIterator* iter,
//                                       const void* key,
//                                       const void* leafdata);
//NA_API NABool naInsertTreeMutable (NATreeIterator* iter,
//                                       const void* key,
//                                             void* leafdata);




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
