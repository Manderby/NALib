
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations of the file NATree.h
// Do not include this file directly! It will automatically be included when
// including "NATree.h"

typedef struct NATreeBaseNode NATreeBaseNode;
typedef struct NATreeNode NATreeNode;
typedef struct NATreeLeaf NATreeLeaf;
typedef struct NATreeIterationInfo NATreeIterationInfo;

// Currently, an NATree is suppost to store a max of 16 childs, meaning any
// other flag must be non-overlapping with 0x0000ffff or having a bitshift
// of 16 respectively.

#define NA_TREE_NODE_MAX_CHILDS 16
#define NA_TREE_CHILDS_MASK ((1 << NA_TREE_NODE_MAX_CHILDS) - 1)

#define NA_TREE_NODE_AVL_BITSHIFT NA_TREE_NODE_MAX_CHILDS
#define NA_TREE_NODE_AVL_LEFT  (0x00 << NA_TREE_NODE_AVL_BITSHIFT)
#define NA_TREE_NODE_AVL_EQUAL (0x01 << NA_TREE_NODE_AVL_BITSHIFT)
#define NA_TREE_NODE_AVL_RIGHT (0x02 << NA_TREE_NODE_AVL_BITSHIFT)
#define NA_TREE_NODE_AVL_MASK  (0x03 << NA_TREE_NODE_AVL_BITSHIFT)

#define NA_TREE_KEY_TYPE_MASK  0x07

#define NA_TREE_FLAG_ROOT_IS_LEAF 0x01

typedef enum{
  NA_TREE_LEAF_INSERT_ORDER_KEY,
  NA_TREE_LEAF_INSERT_ORDER_PREV,
  NA_TREE_LEAF_INSERT_ORDER_NEXT,
  NA_TREE_LEAF_INSERT_ORDER_REPLACE,
} NATreeLeafInsertOrder;

// Callback function types for the different kind of trees. If you want to
// implement your own tree kind, you need to provide the following functions
// and set them in naCreateTreeConfiguration.
//
// NAKeyIndexGetter          Returns the childindex where key would be stored
//                           on a node with the given basekey.
// NAKeyEqualer              Returns true if the two keys are equal.
// NAKeyAssigner             Assigns the src key to dst.
// NAKeyTester               Tests whether the key is within the given limits.
//
// NATreeNodeCoreConstructor Creates a new node with the given key.
// NATreeNodeCoreDestructor  Destroys the given node
// NATreeLeafCoreConstructor Creates a new leaf with the given key and data
// NATreeLeafCoreDestructor  Destroys the given leaf.
//
// NATreeBubbleLocator       Searches from the given node upwards the tree and
//                           returns the node which is guaranteed to contain
//                           the given key or Null if there is no root.
// NATreeCaptureLocator      Searches from the given node downwards the tree
//                           and returns the parent node of the child which
//                           should contain key. Returns the childindx of that
//                           child in the childindx parameter.
// NATreeChildIndexGetter    Returns the index of the child in the parent.
// NATreeChildKeyIndexGetter Returns the index under which the given key should
//                           be stored in the given parent.
// NATreeChildGetter         Returns the Child of the given parent with the
//                           given index.
// NATreeLeafAdder           Sets the given child with the given type to the
//                           parent at the given index.
// NATreeLeafRemover         Removes the given leaf from the tree.
// NATreeLeafInserter        Expects the child ad childindx of grandparent to
//                           be a leaf which will be split to a node containing
//                           both that leaf and the new sibling.
// NATreeLeafKeyGetter       Returns the key of the given leaf.
// NATreeLeafDataGetter      Returns the data of the given leaf.
typedef NAInt           (*NAKeyIndexGetter)(const void* basekey, const void* testkey);
typedef NABool          (*NAKeyEqualer)(const void* key1, const void* key2);
typedef void            (*NAKeyAssigner)(void* dst, const void* src);
typedef NABool          (*NAKeyTester)(const void* leftlimit, const void* rightlimit, const void* key);

typedef void            (*NATreeNodeCoreDestructor)(NATree* tree, NATreeNode* node, NABool recursive);
typedef NATreeLeaf*     (*NATreeLeafCoreConstructor)(NATree* tree, const void* key, NAPtr data);
typedef void            (*NATreeLeafCoreDestructor)(NATree* tree, NATreeLeaf* leaf);

typedef NATreeNode*     (*NATreeBubbleLocator)(const NATree* tree, NATreeLeaf* leaf, const void* key);
typedef NATreeLeaf*     (*NATreeCaptureLocator)(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound);
typedef NAInt           (*NATreeChildIndexGetter)(NATreeNode* parent, NATreeBaseNode* child);
typedef NAInt           (*NATreeChildKeyIndexGetter)(const NATree* tree, NATreeNode* parent, const void* key);
typedef NATreeBaseNode* (*NATreeChildGetter)(NATreeNode* parent, NAInt childindx);
typedef NATreeNode*     (*NATreeLeafRemover)(NATree* tree, NATreeLeaf* leaf);
typedef NATreeLeaf*     (*NATreeLeafInserter)(NATree* tree, NATreeLeaf* existingleaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
typedef const void*     (*NATreeLeafKeyGetter)(NATreeLeaf* leaf);
typedef NAPtr*          (*NATreeLeafDataGetter)(NATreeLeaf* leaf);
typedef NAPtr*          (*NATreeNodeDataGetter)(NATreeNode* node);

struct NATreeConfiguration{

  // User settings
  NARefCount                    refcount;
  NATreeContructorCallback      treeConstructor;
  NATreeDestructorCallback      treeDestructor;
  NATreeNodeConstructor         nodeConstructor;
  NATreeNodeDestructor          nodeDestructor;
  NATreeNodeChildUpdater        nodeChildUpdater;
  NATreeLeafConstructor         leafConstructor;
  NATreeLeafDestructor          leafDestructor;
  NATreeNodeTokenSearcher       nodeTokenSearcher;
  NATreeLeafTokenSearcher       leafTokenSearcher;
  NAPtr                         data;
  NAInt                         flags;

  // Core settings:
  NAInt                         childpernode;
  NAKeyIndexGetter              keyIndexGetter;
  NAKeyEqualer                  keyEqualer;
  NAKeyAssigner                 keyAssigner;
  NAKeyTester                   keyTester;

  NATreeNodeCoreDestructor      nodeCoreDestructor;
  NATreeLeafCoreConstructor     leafCoreConstructor;
  NATreeLeafCoreDestructor      leafCoreDestructor;

  NATreeBubbleLocator           bubbleLocator;
  NATreeCaptureLocator          captureLocator;
  NATreeChildIndexGetter        childIndexGetter;
  NATreeChildKeyIndexGetter     childKeyIndexGetter;
  NATreeChildGetter             childGetter;
  NATreeLeafRemover             leafRemover;
  NATreeLeafInserter            leafInserter;
  NATreeLeafKeyGetter           leafKeyGetter;
  NATreeLeafDataGetter          leafDataGetter;
  NATreeNodeDataGetter          nodeDataGetter;
};

struct NATreeBaseNode{
  NATreeNode* parent;
};

struct NATreeNode{
  NATreeBaseNode basenode;
  NAInt flags;
};

struct NATreeLeaf{
  NATreeBaseNode basenode;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeIterator{
  NAInt flags;
  NAPtr tree;
  NATreeLeaf* leaf;
};

struct NATree{
  NATreeConfiguration* config;
  NATreeBaseNode* root;
  NAInt flags;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeIterationInfo{
  NAInt step;
  NAInt startindx;
  NAInt breakindx;
};

#include "NATreeCoreII.h"



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
