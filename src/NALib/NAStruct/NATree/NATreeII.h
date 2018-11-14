
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

typedef NABool          (*NATreeNodeLimitTester)(NATreeIterator* iter, const void* limit);
typedef NATreeNode*     (*NATreeNodeBubbleLocator)(NATreeNode* node, const void* key);
typedef NATreeBaseNode* (*NATreeNodeCaptureLocator)(NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* leafindx);
typedef NAInt           (*NATreeNodeChildIndexGetter)(NATreeNode* parent, NATreeBaseNode* child);
typedef NATreeBaseNode* (*NATreeNodeChildGetter)(NATreeNode* parent, NAInt childindx);
typedef void            (*NATreeNodeChildAdder)(NATreeNode* parent, NATreeBaseNode* basenode, NAInt childindx, NANodeChildType childtype);
typedef void            (*NATreeLeafSplitter)(NATree* tree, NATreeLeaf* leaf, NATreeLeaf* sibling, NAInt siblingindx);
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
  
  NATreeNodeBubbleLocator       nodeBubbleLocator;
  NATreeNodeCaptureLocator      nodeCaptureLocator;
  NATreeNodeLimitTester         nodeLimitTester;
  NATreeNodeChildIndexGetter    nodeChildIndexGetter;
  NATreeNodeChildGetter         nodeChildGetter;
  NATreeNodeChildAdder          nodeChildAdder;
  NATreeLeafSplitter            leafSplitter;
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
#include "NATreeNodeII.h"

// NATreeConfiguration
NA_HAPI NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config);

// Iterator APIs and inline implementation thereof
NA_HDEF NATreeBaseNode* naLocateTreeNode(NATreeIterator* iter, const void* key, NABool* keyleaffound, NAInt* leafindx);
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

NA_HAPI NATreeNode* naLocateNodeBubbleBinary(NATreeNode* node, const void* key);
NA_HAPI NATreeBaseNode* naLocateNodeCaptureBinary(NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* childindx);
NA_HAPI NABool naTestNodeLimitBinary(NATreeIterator* iter, const void* limit);
NA_HAPI NAInt naGetNodeChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child);
NA_HAPI NATreeBaseNode* naGetNodeChildBinary(NATreeNode* parent, NAInt childindx);
NA_HAPI void naAddNodeChildBinary(NATreeNode* parent, NATreeBaseNode* basenode, NAInt leafindx, NANodeChildType childtype);
NA_HAPI void naSplitLeafBinary(NATree* tree, NATreeLeaf* leaf, NATreeLeaf* sibling, NAInt siblingindx);
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
