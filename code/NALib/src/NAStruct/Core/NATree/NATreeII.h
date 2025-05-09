
// This file contains inline implementations of the file NATree.h
// Do not include this file directly! It will automatically be included when
// including "NATree.h"

// A Tree consists of Items which can either be a Node or a Leaf.
NA_PROTOTYPE(NATreeItem);
NA_PROTOTYPE(NATreeNode);
NA_PROTOTYPE(NATreeLeaf);
NA_PROTOTYPE(NATreeIterationInfo);

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
// See NA_TREE_KEY_i32 for example.
#define NA_TREE_CONFIG_KEY_TYPE_MASK  0xff

#define NA_TREE_FLAG_ROOT_IS_LEAF 0x01
#define NA_TREE_FLAG_TMP_KEY_TAKEN 0x02

#define NA_TREE_UNSPECIFIED_INDEX     0x0fffffff
#define NA_TREE_CONFIG_INVALID_OFFSET 0x0fffffff

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
//                           on a node with the given baseKey. The data can
//                           be any data required by the datastructure
// NAKeyEqualComparer        Returns true if the two keys map to the same equal key.
// NAKeyAssigner             Assigns the src key to dst.
// NAKeyTester               Tests whether the key is within the given limits.
//
// NATreeLeafAdder           Sets the given child with the given type to the
//                           parent at the given index.
// NATreeLeafRemover         Removes the given leaf from the tree.
// NATreeLeafInserter        Expects the child ad childIndex of grandparent to
//                           be a leaf which will be split to a node containing
//                           both that leaf and the new sibling.
//
// NANodeConteinTester       Tests if the given key is in the given leaf.
// NALeafConteinTester       Tests if the given key is in the given node.
// NANodeOverlapTester       Tests if the given range consisting of lower and
//                           upper key overlaps the given node.
// NALeafOverlapTester       Tests if the given range consisting of lower and
//                           upper key overlaps the given leaf.
typedef size_t          (*NAKeyIndexGetter)(const void* baseKey, const void* testKey, const void* data);
typedef size_t          (*NAChildIndexGetter)(NATreeNode* parentNode, const void* childKey);
typedef NABool          (*NAKeyEqualComparer)(const void* key1, const void* key2);
typedef NABool          (*NAKeySmallerComparer)(const void* key1, const void* key2);
typedef NABool          (*NAKeySmallerEqualComparer)(const void* key1, const void* key2);
typedef void            (*NAKeyAssigner)(void* dst, const void* src);
typedef void            (*NAKeyAdder)(void* dst, const void* src1, const void* src2);
typedef NABool          (*NAKeyTester)(const void* lowerLimit, const void* upperLimit, const void* key);
typedef NABool          (*NAKeyNodeContainTester)(NATreeNode* parentNode, const void* key);
typedef NABool          (*NAKeyLeafContainTester)(NATreeLeaf* leaf, const void* key);
typedef NABool          (*NAKeyNodeOverlapTester)(NATreeNode* parentNode, const void* lowerKey, const void* upperKey);
typedef NABool          (*NAKeyLeafOverlapTester)(NATreeLeaf* leaf, const void* lowerKey, const void* upperKey);

typedef void            (*NA_TreeNodeDestructor)(NATreeNode* node);
typedef void            (*NA_TreeLeafDestructor)(NATreeLeaf* leaf);

// This function shall return the uppermost node which contains the given key.
// It must start searching for the key at the given item and bubble upwards.
// Should return nullptr, if the key does not fit anywhere in the tree. The
// given item is never nullptr and never the root of the tree which itself is
// never empty.
typedef NATreeNode*     (*NATreeBubbleLocator)(const NATree* tree, NATreeItem* item, const void* key);
// This function must insert the given newleaf into the tree, positioned
// relative to the existingleaf as defined by insertOrder.
typedef NATreeLeaf*     (*NATreeLeafInserter)(NATree* tree, NATreeItem* existingitem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);

typedef NATreeNode*     (*NATreeLeafRemover)(NATree* tree, NATreeLeaf* leaf);



NA_PROTOTYPE(NATreeNodeABI);
struct NATreeNodeABI{
  size_t                        childPerNode;
  size_t                        leafKeyOffset;
  size_t                        nodeKeyOffset;
  size_t                        leafUserDataOffset;
  size_t                        nodeUserDataOffset;

  #if NA_DEBUG
    size_t                      sizeofNode;
    size_t                      sizeofLeaf;
    size_t                      nodeChildsOffset;
  #endif
};


struct NATreeConfiguration{
  
  // Main settings
  uint32                        flags;

  NATreeNodeABI                 abi;
  void*                         configData; // currently only used for
                                            // exponation of quad and oct tree.

  NAKeyIndexGetter              keyIndexGetter;
  NAChildIndexGetter            childIndexGetter;
  NAKeyEqualComparer            keyEqualComparer;
  NAKeySmallerComparer          keySmallerComparer;
  NAKeySmallerEqualComparer     keySmallerEqualComparer;
  NAKeyAssigner                 keyAssigner;
  NAKeyTester                   keyTester;
  NAKeyNodeContainTester        keyNodeContainTester;
  NAKeyLeafContainTester        keyLeafContainTester;
  NAKeyNodeOverlapTester        keyNodeOverlapTester;
  NAKeyLeafOverlapTester        keyLeafOverlapTester;

  NA_TreeNodeDestructor         nodeDestructor;
  NA_TreeLeafDestructor         leafDestructor;
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
  NAPtr                         userData;
};

NA_EXTERN_RUNTIME_TYPE(NATreeConfiguration);



struct NATreeItem{
  NATreeNode* parent;
  #if NA_DEBUG
    size_t iterCount;
  #endif
};

struct NATreeNode{
  NATreeItem item;
  uint32 flags; // bitfield storing whether childs are nodes or leafs as well
                // as storing additional flags. For example AVL balance.
};

struct NATreeLeaf{
  NATreeItem item;
};

struct NATreeIterator{
  NAPtr tree;
  NATreeItem* item;
  #if NA_DEBUG
    uint32 flags;
  #endif
};

struct NATree{
  NATreeConfiguration* config;
  NATreeItem* root;
  int32 flags;
  #if NA_DEBUG
    size_t iterCount;
  #endif
};

struct NATreeIterationInfo{
  int32 step;
  int32 startIndex;
  int32 breakIndex;
  const void* lowerLimit;
  const void* upperLimit;
};


#define NA_TREE_NODE_CHILDS_OFFSET sizeof(NATreeNode)


// Helper functions. Do not use as public API.

// Configuration
NA_HIAPI NABool na_GetTreeConfigurationConst(NATreeConfiguration* config);

// Item
NA_HIAPI void na_InitTreeItem(NATreeItem* item);
NA_HIAPI void na_ClearTreeItem(NATreeItem* item);
NA_HIAPI NATreeNode* na_GetTreeItemParent(const NATreeItem* item);
NA_HIAPI void na_SetTreeItemParent(NATreeItem* item, NATreeNode* parent);
NA_HIAPI NABool na_GetTreeItemIsRoot(const NATreeItem* item);
NA_HIAPI size_t na_GetTreeItemIterCount(const NATreeItem* item);
NA_HIAPI void na_IncTreeItemIterCount(NATreeItem* item);
NA_HIAPI void na_DecTreeItemIterCount(NATreeItem* item);

// Node
NA_HIAPI NATreeItem* na_GetTreeNodeItem(NATreeNode* node);
NA_HIAPI void* na_GetTreeNodeKey(NATreeNode* node, const NATreeConfiguration* config);
NA_HIAPI NATreeItem** na_GetTreeNodeChildStorage(NATreeNode* node);
NA_HIAPI NAPtr na_GetTreeNodeData(NATreeNode* node, const NATreeConfiguration* config);
NA_HIAPI void na_SetTreeNodeData(NATreeNode* node, NAPtr data, const NATreeConfiguration* config);
NA_HIAPI NABool na_GetNodeChildIsLeaf(NATreeNode* node, size_t childIndex, const NATreeConfiguration* config);
NA_HIAPI void na_SetNodeChildIsLeaf(NATreeNode* node, size_t childIndex, NABool isleaf, const NATreeConfiguration* config);
NA_HIAPI NATreeItem* na_GetTreeNodeChild(NATreeNode* node, size_t childIndex, const NATreeConfiguration* config);
NA_HIAPI void na_SetTreeNodeChild(NATreeNode* node, NATreeItem* child, size_t childIndex, NABool isChildLeaf, const NATreeConfiguration* config);
NA_HIAPI void na_InitTreeNode(NATreeNode* node, const void* key, const NATreeConfiguration* config);
NA_HIAPI void na_ClearTreeNode(NATreeNode* node);
NA_HIAPI void na_DestructTreeNode(NATreeNode* node, NABool recursive, const NATreeConfiguration* config);
NA_HIAPI size_t na_GetTreeNodeChildIndex(NATreeNode* node, NATreeItem* child, const NATreeConfiguration* config);

// Leaf
NA_HIAPI NATreeItem* na_GetTreeLeafItem(NATreeLeaf* leaf);
NA_HIAPI void na_InitTreeLeaf(NATreeLeaf* leaf, const void* key, NAPtr content, const NATreeConfiguration* config);
NA_HIAPI void na_ClearTreeLeaf(NATreeLeaf* leaf);
NA_HIAPI void na_DestructLeafData(NAPtr data, const NATreeConfiguration* config);
NA_HIAPI void na_DestructTreeLeaf(NATreeLeaf* leaf, const NATreeConfiguration* config);
NA_HIAPI NAPtr na_ConstructLeafData(const void* key, NAPtr content, const NATreeConfiguration* config);
NA_HIAPI void* na_GetTreeLeafKey(NATreeLeaf* leaf, const NATreeConfiguration* config);
NA_HIAPI NAPtr na_GetTreeLeafData(NATreeLeaf* leaf, const NATreeConfiguration* config);
NA_HIAPI void na_SetTreeLeafData(NATreeLeaf* leaf, NAPtr newcontent, const NATreeConfiguration* config);

// Iterator
NA_HIAPI void na_SetTreeIteratorCurItem(NATreeIterator* iter, NATreeItem* newitem);
NA_HIAPI const NATree* na_GetTreeIteratorTreeConst(const NATreeIterator* iter);
NA_HIAPI NATree* na_GetTreeIteratorTreeMutable(NATreeIterator* iter);
NA_HIAPI NABool na_AddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool moveToNew);
NA_HAPI void na_IterateTreeCapture(NATreeIterator* iter, int32 index, NATreeIterationInfo* info);
NA_HAPI void na_IterateTreeBubble(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool na_IterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool na_LocateTreeKey(NATreeIterator* iter, const void* key, NABool usebubble);
NA_HAPI  NABool na_AddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace);

// Utility
NA_HIAPI void na_SetTreeRoot(NATree* tree, NATreeItem* newroot, NABool isLeaf);
NA_HIAPI void na_ClearTreeRoot(NATree* tree);
NA_HIAPI void na_MarkTreeRootLeaf(NATree* tree, NABool isleaf);
NA_HIAPI NABool na_IsTreeItemLeaf(const NATree* tree, NATreeItem* item);
NA_HAPI  NATreeLeaf* na_AddTreeContentInPlace(NATree* tree, NATreeItem* item, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI  void na_UpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, size_t responsibleChildIndex);
NA_HAPI  NABool na_UpdateTreeNodeCapturing(NATree* tree, NATreeNode* node);

#include "../../../NAUtility/NAValueHelper.h"
#include "NATreeConfigurationII.h"
#include "NATreeItemII.h"
#include "NATreeIterationII.h"
#include "NATreeUtilityII.h"



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
