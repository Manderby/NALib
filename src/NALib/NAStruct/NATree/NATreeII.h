
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations of the file NATree.h
// Do not include this file directly! It will automatically be included when
// including "NATree.h"

typedef struct NATreeBaseNode NATreeBaseNode;
typedef struct NATreeNode NATreeNode;
typedef struct NATreeLeaf NATreeLeaf;

typedef enum {
  NA_TREE_NODE_CHILD_NULL = 0x00,
  NA_TREE_NODE_CHILD_LINK = 0x01,
  NA_TREE_NODE_CHILD_NODE = 0x02,
  NA_TREE_NODE_CHILD_LEAF = 0x03
} NANodeChildType;

typedef NATreeNode*     (*NATreeNodeCoreConstructor)(NATree* tree, const void* key);
typedef void            (*NATreeNodeCoreDestructor)(NATree* tree, NATreeNode* node);
typedef NATreeLeaf*     (*NATreeLeafCoreConstructor)(NATree* tree, const void* key, NAPtr data);
typedef void            (*NATreeLeafCoreDestructor)(NATree* tree, NATreeLeaf* leaf);

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
// NATreeChildSetter         Sets the given child with the given type to the
//                           parent at the given index.
// NATreeChildRemover        Removes the given child from the tree.
// NATreeLeafSplitter        Expects the child ad childindx of grandparent to
//                           be a leaf which will be split to a node containing
//                           both that leaf and the new sibling.
// NATreeLeafKeyGetter       Returns the key of the given leaf.
// NATreeLeafDataGetter      Returns the data of the given leaf.
typedef NATreeNode*     (*NATreeBubbleLocator)(NATreeNode* node, const void* key);
typedef NATreeNode*     (*NATreeCaptureLocator)(NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* childindx);
typedef NAInt           (*NATreeChildIndexGetter)(NATreeNode* parent, NATreeBaseNode* child);
typedef NAInt           (*NATreeChildKeyIndexGetter)(NATreeNode* parent, const void* key);
typedef NATreeBaseNode* (*NATreeChildGetter)(NATreeNode* parent, NAInt childindx);
typedef void            (*NATreeChildSetter)(NATreeNode* parent, NATreeBaseNode* child, NAInt childindx, NANodeChildType childtype);
typedef void            (*NATreeChildRemover)(NATree* tree, NATreeBaseNode* child);
typedef void            (*NATreeLeafSplitter)(NATree* tree, NATreeNode* grandparent, NAInt childindx, NATreeLeaf* sibling);
typedef const void*     (*NATreeLeafKeyGetter)(NATreeLeaf* leaf);
typedef NAPtr*          (*NATreeLeafDataGetter)(NATreeLeaf* leaf);

struct NATreeConfiguration{
  // User settings
  NARefCount                  refcount;
  NATreeContructorCallback    treeConstructor;
  NATreeDestructorCallback    treeDestructor;
  NATreeNodeConstructor       nodeConstructor;
  NATreeNodeDestructor        nodeDestructor;
  NATreeLeafConstructor       leafConstructor;
  NATreeLeafDestructor        leafDestructor;
  NAPtr                       data;
  NAInt                       flags;
  
  // Core settings:
  NAInt                         childpernode;
  NATreeNodeCoreConstructor     nodeCoreConstructor;
  NATreeNodeCoreDestructor      nodeCoreDestructor;
  NATreeLeafCoreConstructor     leafCoreConstructor;
  NATreeLeafCoreDestructor      leafCoreDestructor;
  
  NATreeBubbleLocator           bubbleLocator;
  NATreeCaptureLocator          captureLocator;
  NATreeChildIndexGetter        childIndexGetter;
  NATreeChildKeyIndexGetter     childKeyIndexGetter;
  NATreeChildGetter             childGetter;
  NATreeChildSetter             childSetter;
  NATreeChildRemover            childRemover;
  NATreeLeafSplitter            leafSplitter;
  NATreeLeafKeyGetter           leafKeyGetter;
  NATreeLeafDataGetter          leafDataGetter;

  #ifndef NDEBUG
    NAInt                 debugflags;
  #endif
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
  NATreeNode* root;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};



// NATreeNode APIs and inline implementation thereof
NA_HIAPI  void naInitTreeNode(NATreeNode* node);
NA_HIAPI  void naClearTreeNode(NATreeNode* node);
NA_HIAPI  void naInitTreeLeaf(NATreeLeaf* leaf);
NA_HIAPI  void naClearTreeLeaf(NATreeLeaf* leaf);
NA_HIAPI NABool naIsNodeChildTypeValid(NANodeChildType childtype);
NA_HIAPI NANodeChildType naGetNodeChildType(NATreeNode* node, NAInt childindx);
NA_HIAPI void naSetNodeChildType(NATreeNode* node, NAInt childindx, NANodeChildType childtype);
NA_HIAPI NANodeChildType naGetNodeType(const NATree* tree, NATreeBaseNode* node);
#include "NATreeNodeII.h"

// NATreeConfiguration
NA_HAPI NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config);

// Iterator APIs and inline implementation thereof
NA_HAPI NATreeNode* naLocateTreeNode(NATreeIterator* iter, const void* key, NABool* keyleaffound, NAInt* leafindx);
NA_HAPI NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace);
NA_HIAPI void naSetTreeIteratorCurLeaf(NATreeIterator* iter, NATreeLeaf* newleaf);
#include "NATreeIteratorII.h"

// Binary Tree
typedef struct NATreeBinaryNode NATreeBinaryNode;
typedef struct NATreeBinaryLeaf NATreeBinaryLeaf;
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryNode);
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryLeaf);

NA_HAPI NATreeNode* naConstructTreeNodeBinary(NATree* tree, const void* key);
NA_HAPI void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node);
NA_HAPI NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, const void* key, NAPtr data);
NA_HAPI void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf);

NA_HAPI NATreeNode* naLocateBubbleBinary(NATreeNode* node, const void* key);
NA_HAPI NATreeNode* naLocateCaptureBinary(NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* childindx);
NA_HAPI NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child);
NA_HAPI NAInt naGetChildKeyIndexBinary(NATreeNode* parent, const void* key);
NA_HAPI NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx);
NA_HAPI void naSetChildBinary(NATreeNode* parent, NATreeBaseNode* child, NAInt leafindx, NANodeChildType childtype);
NA_HAPI void naRemoveChildBinary(NATree* tree, NATreeBaseNode* child);
NA_HAPI void naSplitLeafBinary(NATree* tree, NATreeNode* grandparent, NAInt childindx, NATreeLeaf* sibling);
NA_HAPI const void* naGetLeafKeyBinary(NATreeLeaf* leaf);
NA_HAPI NAPtr* naGetLeafDataBinary(NATreeLeaf* leaf);



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
