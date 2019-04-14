
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations of the file NATree.h
// Do not include this file directly! It will automatically be included when
// including "NATree.h"

// A Tree consists of Items which can either be a Node or a Leaf.
typedef struct NATreeItem NATreeItem;
typedef struct NATreeNode NATreeNode;
typedef struct NATreeLeaf NATreeLeaf;
typedef struct NATreeIterationInfo NATreeIterationInfo;

// Currently, an NATree is supposed to store a max of 16 childs, meaning any
// other flag must be non-overlapping with 0x0000ffff or having a bitshift
// of 16 respectively.

#define NA_TREE_NODE_MAX_CHILDS 16
#define NA_TREE_NODE_CHILDS_MASK ((1 << NA_TREE_NODE_MAX_CHILDS) - 1)

#define NA_TREE_NODE_AVL_BITSHIFT NA_TREE_NODE_MAX_CHILDS
#define NA_TREE_NODE_AVL_LEFT  (0x00 << NA_TREE_NODE_AVL_BITSHIFT)
#define NA_TREE_NODE_AVL_EQUAL (0x01 << NA_TREE_NODE_AVL_BITSHIFT)
#define NA_TREE_NODE_AVL_RIGHT (0x02 << NA_TREE_NODE_AVL_BITSHIFT)
#define NA_TREE_NODE_AVL_MASK  (0x03 << NA_TREE_NODE_AVL_BITSHIFT)

#define NA_TREE_CONFIG_KEY_TYPE_MASK  0x07

#define NA_TREE_FLAG_ROOT_IS_LEAF 0x01
#define NA_TREE_FLAG_TMP_KEY_TAKEN 0x02

typedef enum{
  NA_TREE_LEAF_INSERT_ORDER_KEY,
  NA_TREE_LEAF_INSERT_ORDER_PREV,
  NA_TREE_LEAF_INSERT_ORDER_NEXT
} NATreeLeafInsertOrder;

// Callback function types for the different kind of trees. If you want to
// implement your own tree kind, you need to provide the following functions
// and set them in naCreateTreeConfiguration.
//
// NAKeyIndexGetter          Returns the childindex where key would be stored
//                           on a node with the given basekey. The data can
//                           be any data required by the datastructure
// NAKeyEqualComparer              Returns true if the two keys are equal.
// NAKeyAssigner             Assigns the src key to dst.
// NAKeyTester               Tests whether the key is within the given limits.
//
// NATreeLeafAdder           Sets the given child with the given type to the
//                           parent at the given index.
// NATreeLeafRemover         Removes the given leaf from the tree.
// NATreeLeafInserter        Expects the child ad childindx of grandparent to
//                           be a leaf which will be split to a node containing
//                           both that leaf and the new sibling.
typedef NAInt           (*NAKeyIndexGetter)(const void* basekey, const void* testkey, const void* data);
typedef NAInt           (*NAChildIndexGetter)(NATreeNode* parentnode, const void* childkey);
typedef NABool          (*NAKeyEqualComparer)(const void* key1, const void* key2);
typedef NABool          (*NAKeyLowerComparer)(const void* key1, const void* key2);
typedef NABool          (*NAKeyLowerEqualComparer)(const void* key1, const void* key2);
typedef void            (*NAKeyAssigner)(void* dst, const void* src);
typedef void            (*NAKeyAdder)(void* dst, const void* src1, const void* src2);
typedef NABool          (*NAKeyTester)(const void* lowerlimit, const void* upperlimit, const void* key);
typedef NABool          (*NAKeyContainTester)(NATreeNode* parentnode, const void* key);

typedef void            (*NATreeNodeCoreDestructor)(NATreeNode* node);
typedef void            (*NATreeLeafCoreDestructor)(NATreeLeaf* leaf);

// This function shall return the uppermost node which contains the given key.
// It must start searching for the key at the given item and bubble upwards.
// Should return Null, if the key does not fit anywhere in the tree. The
// given item is never Null and never the root of the tree which itself is
// never empty.
typedef NATreeNode*     (*NATreeBubbleLocator)(const NATree* tree, NATreeItem* item, const void* key);
// This function must insert the given newleaf into the tree, positioned
// relative to the existingleaf as defined by insertOrder.
typedef NATreeLeaf*     (*NATreeLeafInserter)(NATree* tree, NATreeItem* existingitem, const void* key, NAPtr constructordata, NATreeLeafInsertOrder insertOrder);

typedef NATreeNode*     (*NATreeLeafRemover)(NATree* tree, NATreeLeaf* leaf);



struct NATreeConfiguration{
  
  // Core settings
  NARefCount                    refcount;
  NAInt                         flags;

  NAInt                         childpernode;
  int                           keyByteSize;
  int                           leafKeyOffset;
  int                           nodeKeyOffset;
  int                           nodeChildsOffset;
  int                           leafUserDataOffset;
  int                           nodeUserDataOffset;
  void*                         configdata;

  NAKeyIndexGetter              keyIndexGetter;
  NAChildIndexGetter            childIndexGetter;
  NAKeyEqualComparer            keyEqualComparer;
  NAKeyLowerComparer            keyLowerComparer;
  NAKeyLowerEqualComparer       keyLowerEqualComparer;
  NAKeyAssigner                 keyAssigner;
  NAKeyAdder                    keyAdder;
  NAKeyTester                   keyTester;
  NAKeyContainTester            keyContainTester;

  NATreeNodeCoreDestructor      nodeDestructor;
  NATreeLeafCoreDestructor      leafDestructor;

  NATreeBubbleLocator           bubbleLocator;
  NATreeLeafRemover             leafRemover;
  NATreeLeafInserter            leafInserter;

  // User settings (callbacks and data defined in configuration)
  NATreeContructorCallback      treeConstructor;
  NATreeDestructorCallback      treeDestructor;
  NATreeLeafDataConstructor     leafDataConstructor;
  NATreeLeafDataDestructor      leafDataDestructor;
  NATreeNodeDataConstructor     nodeDataConstructor;
  NATreeNodeDataDestructor      nodeDataDestructor;
  NATreeNodeUpdater             nodeUpdater;
  NAPtr                         userdata;
};

struct NATreeItem{
  NATreeNode* parent;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeNode{
  NATreeItem item;
  NAInt flags;
};

struct NATreeLeaf{
  NATreeItem item;
};

struct NATreeIterator{
  NAPtr tree;
  NATreeItem* item;
  #ifndef NDEBUG
    NAInt flags;
  #endif
};

struct NATree{
  NATreeConfiguration* config;
  NATreeItem* root;
  void* tmpkey;
  NAInt flags;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeIterationInfo{
  NAInt step;
  NAInt startindx;
  NAInt breakindx;
  const void* lowerlimit;
  const void* upperlimit;
};



// Helper functions. Do not use as public API.

// Configuration
NA_HIAPI void naDeallocConfiguration(NATreeConfiguration* config);
NA_HIAPI NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config);

// Item
NA_HIAPI void naInitTreeItem(NATreeItem* item);
NA_HIAPI void naClearTreeItem(NATreeItem* item);
NA_HIAPI NATreeNode* naGetTreeItemParent(const NATreeItem* item);
NA_HIAPI void naSetTreeItemParent(NATreeItem* item, NATreeNode* parent);
NA_HIAPI NABool naIsTreeItemRoot(const NATree* tree, const NATreeItem* item);

// Node
NA_HIAPI NATreeItem* naGetTreeNodeItem(NATreeNode* node);
NA_HIAPI void naInitTreeNode(const NATree* tree, NATreeNode* node, const void* key);
NA_HIAPI void naClearTreeNode(NATreeNode* node);
NA_HIAPI void naDestructNodeData(const NATree* tree, NAPtr data);
NA_HIAPI void naDestructTreeNode(NATree* tree, NATreeNode* node, NABool recursive);
NA_HIAPI NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childindx);
NA_HIAPI void naMarkNodeChildLeaf(NATreeNode* node, NAInt childindx, NABool isleaf);
NA_HIAPI void* naGetTreeNodeKey(const NATree* tree, NATreeNode* node);
NA_HIAPI NAPtr naGetTreeNodeData(const NATree* tree, NATreeNode* node);
NA_HIAPI void naSetTreeNodeData(const NATree* tree, NATreeNode* node, NAPtr newdata);
NA_HIAPI NATreeItem* naGetTreeNodeChild(const NATree* tree, NATreeNode* parent, NAInt childindx);
NA_HIAPI void naSetTreeNodeChild(const NATree* tree, NATreeNode* parent, NAInt childindx, NATreeItem* newchild);
NA_HIAPI NAInt naGetTreeNodeChildIndex(const NATree* tree, NATreeNode* parent, NATreeItem* child);

// Leaf
NA_HIAPI NATreeItem* naGetTreeLeafItem(NATreeLeaf* leaf);
NA_HIAPI void naInitTreeLeaf(NATree* tree, NATreeLeaf* leaf, const void* key, NAPtr constructordata);
NA_HIAPI void naClearTreeLeaf(NATreeLeaf* leaf);
NA_HIAPI void naDestructLeafData(const NATree* tree, NAPtr data);
NA_HIAPI void naDestructTreeLeaf(NATree* tree, NATreeLeaf* leaf);
NA_HIAPI NAPtr naConstructLeafData(NATree* tree, const void* key, NAPtr constructordata);
NA_HIAPI void* naGetTreeLeafKey(const NATree* tree, NATreeLeaf* leaf);
NA_HIAPI NAPtr naGetTreeLeafData(const NATree* tree, NATreeLeaf* leaf);
NA_HIAPI void naSetTreeLeafData(const NATree* tree, NATreeLeaf* leaf, NAPtr newcontent);

// Iterator
NA_HIAPI void naSetTreeIteratorCurItem(NATreeIterator* iter, NATreeItem* newitem);
NA_HIAPI const NATree* naGetTreeIteratorTreeConst(const NATreeIterator* iter);
NA_HIAPI NATree* naGetTreeIteratorTreeMutable(NATreeIterator* iter);
NA_HIAPI NABool naAddTreeContent(NATreeIterator* iter, NAPtr constructordata, NATreeLeafInsertOrder insertOrder, NABool movetonew);
NA_HAPI void naIterateTreeCapture(NATreeIterator* iter, NAInt indx, NATreeIterationInfo* info);
NA_HAPI void naIterateTreeBubble(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool naLocateTreeKeyCore(NATreeIterator* iter, const void* key, NABool usebubble);
NA_HAPI  NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr constructordata, NABool replace);

// Utilities
NA_HIAPI NABool naIsTreeRootLeaf(const NATree* tree);
NA_HIAPI void naMarkTreeRootLeaf(NATree* tree, NABool isleaf);
NA_HIAPI NABool naIsTreeItemLeaf(const NATree* tree, NATreeItem* item);
NA_HIAPI void* naRequestTreeTmpKey(const NATree* tree);
NA_HIAPI void naResignTreeTmpKey(const NATree* tree);
NA_HAPI  NATreeLeaf* naAddTreeContentInPlace(NATree* tree, NATreeItem* item, const void* key, NAPtr constructordata, NATreeLeafInsertOrder insertOrder);
NA_HAPI  void naUpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, NAInt childindx);
NA_HAPI  NABool naUpdateTreeNodeCapturing(NATree* tree, NATreeNode* node);

#include "../../NACore/NAValueHelper.h"
#include "NATreeConfigurationII.h"
#include "NATreeItemII.h"
#include "NATreeIterationII.h"
#include "NATreeUtilitiesII.h"


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
