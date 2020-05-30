
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

// This is directly linked to the types defined in NATree.h.
// See NA_TREE_KEY_NAINT for example.
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
// NAKeyEqualComparer        Returns true if the two keys map to the same equal key.
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
typedef NABool          (*NAKeyNodeContainTester)(NATreeNode* parentnode, const void* key);
typedef NABool          (*NAKeyLeafContainTester)(NATreeLeaf* leaf, const void* key);

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
typedef NATreeLeaf*     (*NATreeLeafInserter)(NATree* tree, NATreeItem* existingitem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);

typedef NATreeNode*     (*NATreeLeafRemover)(NATree* tree, NATreeLeaf* leaf);



struct NATreeConfiguration{
  
  // Core settings
  NARefCount                    refcount;
  NAInt                         flags;

  NAInt                         childpernode;
  int                           leafKeyOffset;
  int                           nodeKeyOffset;
  int                           leafUserDataOffset;
  int                           nodeUserDataOffset;
  void*                         configdata;

  #ifndef NDEBUG
    size_t                      sizeofNode;
    size_t                      sizeofLeaf;
  #endif

  NAKeyIndexGetter              keyIndexGetter;
  NAChildIndexGetter            childIndexGetter;
  NAKeyEqualComparer            keyEqualComparer;
  NAKeyLowerComparer            keyLowerComparer;
  NAKeyLowerEqualComparer       keyLowerEqualComparer;
  NAKeyAssigner                 keyAssigner;
  NAKeyTester                   keyTester;
  NAKeyNodeContainTester        keyNodeContainTester;
  NAKeyLeafContainTester        keyLeafContainTester;

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
    NAInt iterCount;
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
  NAInt flags;
  #ifndef NDEBUG
    NAInt iterCount;
  #endif
};

struct NATreeIterationInfo{
  NAInt step;
  NAInt startindx;
  NAInt breakindx;
  const void* lowerlimit;
  const void* upperlimit;
};


#define NA_TREE_NOTE_CHILDS_OFFSET sizeof(NATreeNode)


// Helper functions. Do not use as public API.

// Configuration
NA_HIAPI void naDeallocConfiguration(NATreeConfiguration* config);
NA_HIAPI NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config);

// Item
NA_HIAPI void naInitTreeItem(NATreeItem* item);
NA_HIAPI void naClearTreeItem(NATreeItem* item);
NA_HIAPI NATreeNode* naGetTreeItemParent(const NATreeItem* item);
NA_HIAPI void naSetTreeItemParent(NATreeItem* item, NATreeNode* parent);
NA_HIAPI NABool naIsTreeItemRoot(const NATreeItem* item);

// Node
NA_HIAPI NATreeItem* naGetTreeNodeItem(NATreeNode* node);
NA_HIAPI void naInitTreeNode(const NATreeConfiguration* config, NATreeNode* node, const void* key);
NA_HIAPI void naClearTreeNode(NATreeNode* node);
NA_HIAPI void naDestructNodeData(const NATreeConfiguration* config, NAPtr data);
NA_HIAPI void naDestructTreeNode(const NATreeConfiguration* config, NATreeNode* node, NABool recursive);
NA_HIAPI NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childindx);
NA_HIAPI void naMarkNodeChildLeaf(NATreeNode* node, NAInt childindx, NABool isleaf);
NA_HIAPI void* naGetTreeNodeKey(const NATreeConfiguration* config, NATreeNode* node);
NA_HIAPI NAPtr naGetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node);
NA_HIAPI void naSetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node, NAPtr newdata);
NA_HIAPI NATreeItem** naGetTreeNodeChildStorage(NATreeNode* parent);
NA_HIAPI NATreeItem* naGetTreeNodeChild(const NATreeConfiguration* config, NATreeNode* parent, NAInt childindx);
NA_HIAPI void naSetTreeNodeChildEmpty(NATreeNode* parent, NAInt childindx);
NA_HIAPI void naSetTreeNodeChild(NATreeNode* parent, NATreeItem* child, NAInt childindx, NABool isChildLeaf);
NA_HIAPI NAInt naGetTreeNodeChildIndex(const NATreeConfiguration* config, NATreeNode* parent, NATreeItem* child);

// Leaf
NA_HIAPI NATreeItem* naGetTreeLeafItem(NATreeLeaf* leaf);
NA_HIAPI void naInitTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf, const void* key, NAPtr content);
NA_HIAPI void naClearTreeLeaf(NATreeLeaf* leaf);
NA_HIAPI void naDestructLeafData(const NATreeConfiguration* config, NAPtr data);
NA_HIAPI void naDestructTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf);
NA_HIAPI NAPtr naConstructLeafData(const NATreeConfiguration* config, const void* key, NAPtr content);
NA_HIAPI void* naGetTreeLeafKey(const NATreeConfiguration* config, NATreeLeaf* leaf);
NA_HIAPI NAPtr naGetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf);
NA_HIAPI void naSetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf, NAPtr newcontent);

// Iterator
NA_HIAPI void naSetTreeIteratorCurItem(NATreeIterator* iter, NATreeItem* newitem);
NA_HIAPI const NATree* naGetTreeIteratorTreeConst(const NATreeIterator* iter);
NA_HIAPI NATree* naGetTreeIteratorTreeMutable(NATreeIterator* iter);
NA_HIAPI NABool naAddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool movetonew);
NA_HAPI void naIterateTreeCapture(NATreeIterator* iter, NAInt index, NATreeIterationInfo* info);
NA_HAPI void naIterateTreeBubble(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool naLocateTreeKeyCore(NATreeIterator* iter, const void* key, NABool usebubble);
NA_HAPI  NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace);

// Utilities
NA_HIAPI void naSetTreeRoot(NATree* tree, NATreeItem* newroot, NABool isLeaf);
NA_HIAPI void naClearTreeRoot(NATree* tree);
NA_HIAPI NABool naIsTreeRootLeaf(const NATree* tree);
NA_HIAPI void naMarkTreeRootLeaf(NATree* tree, NABool isleaf);
NA_HIAPI NABool naIsTreeItemLeaf(const NATree* tree, NATreeItem* item);
NA_HAPI  NATreeLeaf* naAddTreeContentInPlace(NATree* tree, NATreeItem* item, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI  void naUpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, NAInt childindx);
NA_HAPI  NABool naUpdateTreeNodeCapturing(NATree* tree, NATreeNode* node);

#include "../../NACore/NAValueHelper.h"
#include "NATreeConfigurationII.h"
#include "NATreeItemII.h"
#include "NATreeIterationII.h"
#include "NATreeUtilitiesII.h"



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
