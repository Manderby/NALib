
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


typedef struct NATreeBaseNode NATreeBaseNode;
typedef struct NATreeNode NATreeNode;
typedef struct NATreeLeaf NATreeLeaf;


// Returns true if the iterator is located at a position which overlaps the
// range/rect/box given in limit.
typedef NATreeNode* (*NATreeNodeCoreConstructor)(NATree* tree, NATreeNode* parent, double key, NAPtr data);
typedef void        (*NATreeNodeCoreDestructor)(NATree* tree, NATreeNode* node);
typedef NATreeLeaf* (*NATreeLeafCoreConstructor)(NATree* tree, NATreeNode* parent, double key, NAPtr data);
typedef void        (*NATreeLeafCoreDestructor)(NATree* tree, NATreeLeaf* leaf);
typedef void        (*NATreeIteratorKeySetter)(NATreeIterator* iter, const void* key);
typedef NAPtr*      (*NATreeLeafDataGetter)(NATreeLeaf* leaf);
typedef NAInt       (*NATreeNodeIndexInParentGetter)(NATreeBaseNode* basenode);
typedef NAInt       (*NATreeNodeChildIndexGetter)(NATreeNode* node, const void* key);
typedef NABool      (*NATreeNodeLimitTester)(NATreeIterator* iter, const void* limit);
// The core node constructor and destructor
typedef void        (*NATreeBubbleLocator)(NATreeIterator* iter);
typedef NABool      (*NATreeCaptureLocator)(NATreeIterator* iter);

typedef enum {
  NA_TREE_NODE_CHILD_NULL = 0x00,
  NA_TREE_NODE_CHILD_LINK = 0x01,
  NA_TREE_NODE_CHILD_NODE = 0x02,
  NA_TREE_NODE_CHILD_LEAF = 0x03
} NANodeChildType;

#define NA_TREE_NODE_CHILD_MASK 0x03
#define NA_TREE_NODE_CHILD_AVAILABLE_MASK 0x02

#define NA_TREE_NODE_MAX_CHILD_COUNT 8

#define NA_TREE_NODE_INDEX_IN_PARENT_BITSHIFT (NA_TREE_NODE_MAX_CHILD_COUNT * 2)
#define NA_TREE_NODE_INDEX_IN_PARENT_MASK 0x0f

struct NATreeConfiguration{
  NARefCount                  refcount;
  NATreeContructorCallback    treeConstructor;
  NATreeDestructorCallback    treeDestructor;
  NATreeNodeConstructor       nodeConstructor;
  NATreeNodeDestructor        nodeDestructor;
  NATreeLeafConstructor       leafConstructor;
  NATreeLeafDestructor        leafDestructor;
//  NAQuadTreeDataCopier      datacopier;
//  NAQuadTreeChildChanged    childchanged;
  NAPtr                       data; // the data provided by the user
//  int16                     baseleafexponent;
  NAInt                       flags;
  
  // Hidden settings:
  NAInt                         childpernode;
  NATreeNodeCoreConstructor     nodeCoreConstructor;
  NATreeNodeCoreDestructor      nodeCoreDestructor;
  NATreeLeafCoreConstructor     leafCoreConstructor;
  NATreeLeafCoreDestructor      leafCoreDestructor;
  
  NATreeBubbleLocator           treeBubbleLocator;
  NATreeCaptureLocator          treeCaptureLocator;
  NATreeIteratorKeySetter       iteratorKeySetter;
  NATreeNodeIndexInParentGetter nodeIndexInParentGetter;
  NATreeNodeChildIndexGetter    nodeChildIndexGetter;
  NATreeNodeLimitTester         nodelimittester;
  NATreeLeafDataGetter          leafDataGetter;

  #ifndef NDEBUG
    NAInt                 debugflags;
  #endif
};

struct NATreeBaseNode{
  NATreeNode* parent;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeNode{
  NATreeBaseNode basenode;
  NAInt flags;
};

struct NATreeLeaf{
  NATreeBaseNode basenode;
};

struct NATreeIterator{
  NAInt flags;
  NAPtr tree;
  NATreeBaseNode* basenode;
  NAInt childindx;
  union{
    double d;
  } key;
};

struct NATree{
  NATreeConfiguration* config;
  NATreeNode* root;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};



// NATreeNode
NA_HAPI void naInitTreeNode(NATreeNode* node, NATreeNode* parent);
NA_HAPI void naClearTreeNode(NATreeNode* node);
NA_HAPI void naInitTreeLeaf(NATreeLeaf* leaf, NATreeNode* parent);
NA_HAPI void naClearTreeLeaf(NATreeLeaf* leaf);
NA_HIAPI NABool naHasNodeChild(NATreeNode* node, NAInt childindx);
NA_HIAPI NANodeChildType naGetNodeChildType(NATreeNode* node, NAInt childindx);
NA_HIAPI void naSetNodeChildType(NATreeNode* node, NAInt childindx, NANodeChildType childtype);
NA_HAPI NABool naLocateTreeNode(NATreeIterator* iter);

// NATreeConfiguration
NA_HAPI NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config);

// NATreeIterator
NA_HIAPI void naSetTreeIteratorCurNode(NATreeIterator* iter, NATreeBaseNode* newnode);




NA_HIDEF NABool naHasNodeChild(NATreeNode* node, NAInt childindx){
  return naTestFlagi(((node->flags >> (childindx * 2)) & NA_TREE_NODE_CHILD_MASK), 0x02); // 0x02 means eigther node or leaf
}
NA_HIDEF NANodeChildType naGetNodeChildType(NATreeNode* node, NAInt childindx){
  return (NANodeChildType)((node->flags >> (childindx * 2)) & NA_TREE_NODE_CHILD_MASK);
}
NA_HIDEF void naSetNodeChildType(NATreeNode* node, NAInt childindx, NANodeChildType childtype){
  // Clear the old type
  naSetFlagi(&(node->flags), NA_TREE_NODE_CHILD_MASK << (childindx * 2), NA_FALSE);
  // Set the new type
  naSetFlagi(&(node->flags), childtype << (childindx * 2), NA_TRUE);
}



NA_HIDEF void naSetTreeIteratorCurNode(NATreeIterator* iter, NATreeBaseNode* newnode){
  #ifndef NDEBUG
    if(iter->basenode){iter->basenode->itercount--;}
  #endif
  iter->basenode = newnode;
  #ifndef NDEBUG
    if(iter->basenode){iter->basenode->itercount++;}
  #endif
}



// Binary Tree
typedef struct NATreeBinaryNode NATreeBinaryNode;
typedef struct NATreeBinaryLeaf NATreeBinaryLeaf;
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryNode);
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryLeaf);

NA_HAPI NATreeNode* naConstructTreeNodeBinary(NATree* tree, NATreeNode* parent, double key, NAPtr data);
NA_HAPI void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node);
NA_HAPI NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, NATreeNode* parent, double key, NAPtr data);
NA_HAPI void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf);

NA_HAPI void naLocateTreeBubbleBinary(NATreeIterator* iter);
NA_HAPI NABool naLocateTreeCaptureBinary(NATreeIterator* iter);
NA_HAPI NAInt naGetTreeNodeIndexInParentBinary(NATreeBaseNode* basenode);
NA_HAPI NAInt naGetTreeNodeChildIndxBinary(NATreeNode* node, const void* key);
NA_HAPI NABool naTestNodeLimitBinary(NATreeIterator* iter, const void* limit);
NA_HAPI void naSetTreeIteratorKeyBinary(NATreeIterator* iter, const void* key);
NA_HAPI NAPtr* naGetTreeLeafDataBinary(NATreeLeaf* leaf);



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
