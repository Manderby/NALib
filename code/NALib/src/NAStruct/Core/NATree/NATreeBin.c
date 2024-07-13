
#include "../../NATree.h"
#include "NATreeBin.h"



NA_RUNTIME_TYPE(NATreeBinNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeBinLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void* na_GetBinNodeKey(NATreeBinNode* binnode) {
  return &binnode->key;
}
NA_HIDEF void* na_GetBinLeafKey(NATreeBinLeaf* binleaf) {
  return &binleaf->key;
}



NA_HIDEF void na_AddTreeNodeChildBin(NATree* tree, NATreeBinNode* parent, NATreeItem* child, NAInt childIndex, NABool isChildLeaf) {
  NATreeNode* parentNode;
  NA_UNUSED(tree);
  
  parent->childs[childIndex] = child;
  parentNode = na_GetBinNodeNode(parent);
  na_MarkNodeChildLeaf(parentNode, childIndex, isChildLeaf);
  na_SetTreeItemParent(child, parentNode);
}



NA_HDEF NATreeNode* na_NewTreeNodeBin(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf) {
  NATreeBinNode* binnode = naNew(NATreeBinNode);
  na_InitTreeNode(tree->config, na_GetBinNodeNode(binnode), key);

  // Node-specific initialization
  na_AddTreeNodeChildBin(tree, binnode, na_GetTreeLeafItem(leftleaf),  0, NA_TRUE);
  na_AddTreeNodeChildBin(tree, binnode, na_GetTreeLeafItem(rightleaf), 1, NA_TRUE);
  if(tree->config->flags & NA_TREE_BALANCE_AVL) {
    na_InitNodeAVL(binnode);
  }

  return na_GetBinNodeNode(binnode);
}



NA_HDEF NATreeLeaf* na_NewTreeLeafBin(NATree* tree, const void* key, NAPtr content) {
  NATreeBinLeaf* binleaf = naNew(NATreeBinLeaf);
  na_InitTreeLeaf(tree->config, na_GetBinLeafLeaf(binleaf), key, content);
  return na_GetBinLeafLeaf(binleaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt na_GetChildIndexBinDouble(NATreeNode* parentNode, const void* childKey) {
  NATreeBinNode* binnode = (NATreeBinNode*)(parentNode);
  return na_GetKeyIndexBinDouble(na_GetBinNodeKey(binnode), childKey, NA_NULL);
}
NA_HDEF NAInt na_GetKeyIndexBinDouble(const void* baseKey, const void* testKey, const void* data) {
  NA_UNUSED(data);
  return !(*(const double*)testKey < *(const double*)baseKey); // results in 0 or 1
}
NA_HDEF NABool na_TestKeyBinDouble(const void* lowerLimit, const void* upperLimit, const void* key) {
  return ((*(const double*)lowerLimit <= *(const double*)key) && (*(const double*)key) < *(const double*)upperLimit);
}
NA_HDEF NABool na_TestKeyLeafContainBinDouble(NATreeLeaf* leaf, const void* key) {
  double* key1 = (double*)na_GetBinLeafKey((NATreeBinLeaf*)leaf);
  double* key2 = (double*)key;
  return *key1 == *key2;
}


NA_HDEF NAInt na_GetChildIndexBinNAInt(NATreeNode* parentNode, const void* childKey) {
  NATreeBinNode* binnode = (NATreeBinNode*)(parentNode);
  return na_GetKeyIndexBinNAInt(na_GetBinNodeKey(binnode), childKey, NA_NULL);
}
NA_HDEF NAInt na_GetKeyIndexBinNAInt(const void* baseKey, const void* key, const void* data) {
  NA_UNUSED(data);
  // if key is equal to baseKey, the return value must be 1.
  return !(*(const NAInt*)key < *(const NAInt*)baseKey); // results in 0 or 1
}
NA_HDEF NABool na_TestKeyBinNAInt(const void* lowerLimit, const void* upperLimit, const void* key) {
  return ((*(const NAInt*)lowerLimit <= *(const NAInt*)key) && (*(const NAInt*)key <= *(const NAInt*)upperLimit));
}
NA_HDEF NABool na_TestKeyLeafContainBinNAInt(NATreeLeaf* leaf, const void* key) {
  NAInt* key1 = (NAInt*)na_GetBinLeafKey((NATreeBinLeaf*)leaf);
  NAInt* key2 = (NAInt*)key;
  return *key1 == *key2;
}



NA_HDEF void na_DestructTreeNodeBin(NATreeNode* node) {
  naDelete(node);
}



NA_HDEF void na_DestructTreeLeafBin(NATreeLeaf* leaf) {
  naDelete(leaf);
}



NA_HDEF NATreeNode* na_LocateBubbleBinWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* lowerLimit, const void* upperLimit, NATreeItem* previtem) {
  NATreeBinNode* binnode;
  NATreeItem* item;
  
  #if NA_DEBUG
    if(node == NA_NULL)
      naError("node should not be Null");
    if(previtem == NA_NULL)
      naError("prevnode should not be Null");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  binnode = (NATreeBinNode*)(node);
  
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualComparer(key, na_GetBinNodeKey(binnode)))
    return node;
  
  // Otherwise, we set the limits dependent on the previous node.
  if(na_GetTreeNodeChildIndex(tree->config, node, previtem) == 1) {
    lowerLimit = na_GetBinNodeKey(binnode);
  }else{
    upperLimit = na_GetBinNodeKey(binnode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerLimit && upperLimit && tree->config->keyTester(lowerLimit, upperLimit, key)) {
    return node;
  }
  // Otherwise, go up if possible.
  item = na_GetTreeNodeItem(node);
  if(!na_IsTreeItemRoot(item)) {
    return na_LocateBubbleBinWithLimits(tree, na_GetTreeItemParent(item), key, lowerLimit, upperLimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* na_LocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key) {
  return na_LocateBubbleBinWithLimits(tree, na_GetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeNode* na_RemoveLeafBin(NATree* tree, NATreeLeaf* leaf) {
  NATreeNode* parent = na_GetTreeItemParent(na_GetTreeLeafItem(leaf));
  NATreeNode* grandparent = NA_NULL;
  if(!na_IsTreeItemRoot(na_GetTreeLeafItem(leaf))) {
    NAInt leafIndex = na_GetTreeNodeChildIndex(tree->config, parent, na_GetTreeLeafItem(leaf));
    NATreeItem* sibling = ((NATreeBinNode*)parent)->childs[1 - leafIndex];
    NABool issiblingleaf = na_IsNodeChildLeaf(parent, 1 - leafIndex);

    grandparent = na_GetTreeItemParent(na_GetTreeNodeItem(parent));
    if(!na_IsTreeItemRoot(na_GetTreeNodeItem(parent))) {
      NAInt parentIndex = na_GetTreeNodeChildIndex(tree->config, grandparent, na_GetTreeNodeItem(parent));
      ((NATreeBinNode*)grandparent)->childs[parentIndex] = sibling;
      na_MarkNodeChildLeaf(grandparent, parentIndex, issiblingleaf);

      if(tree->config->flags & NA_TREE_BALANCE_AVL) {
        na_ShrinkAVL(tree, (NATreeBinNode*)grandparent, parentIndex);
      }
    }else{
      tree->root = sibling;
      na_MarkTreeRootLeaf(tree, issiblingleaf);
    }
    sibling->parent = grandparent;

    na_DestructTreeNode(tree->config, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  na_DestructTreeLeaf(tree->config, leaf);
  return grandparent;
}



NA_HDEF NATreeLeaf* na_InsertLeafBin(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder) {
  // Create the new leaf and initialize it.
  NATreeLeaf* newleaf = na_NewTreeLeafBin(tree, key, content);

  if(!existingItem) {
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    NATreeItem* leafItem = na_GetTreeLeafItem(newleaf);
    tree->root = leafItem;
    na_SetTreeItemParent(leafItem, NA_NULL);
    na_MarkTreeRootLeaf(tree, NA_TRUE);
  
  }else{
    NATreeLeaf* existingLeaf;
    NATreeNode* existingParent;
    NABool wasTreeItemRoot;
    NATreeItem* newParent;
    NATreeLeaf* left;
    NATreeLeaf* right;

    #if NA_DEBUG
      if(!na_IsTreeItemLeaf(tree, existingItem))
        naError("Item should be a leaf");
    #endif

    existingLeaf = (NATreeLeaf*)existingItem;
  
    switch(insertOrder) {
    case NA_TREE_LEAF_INSERT_ORDER_KEY:
      #if NA_DEBUG
        if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
          naError("tree is configured with no key");
      #endif
      if(tree->config->keyIndexGetter(na_GetBinLeafKey(((NATreeBinLeaf*)existingItem)), na_GetBinLeafKey(((NATreeBinLeaf*)newleaf)), NA_NULL) == 1) {
        left = existingLeaf;
        right = newleaf;
      }else{
        left = newleaf;
        right = existingLeaf;
      }
      break;
    case NA_TREE_LEAF_INSERT_ORDER_PREV:
      #if NA_DEBUG
        if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
          naError("tree is configured with key");
      #endif
      left = newleaf;
      right = existingLeaf;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_NEXT:
      #if NA_DEBUG
        if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
          naError("tree is configured with key");
      #endif
      left = existingLeaf;
      right = newleaf;
      break;
    default:
      #if NA_DEBUG
      naError("Invalid insertOrder");
      #endif
    left = existingLeaf;
    right = newleaf;
    break;
    }

    existingParent = na_GetTreeItemParent(existingItem);
    wasTreeItemRoot = na_IsTreeItemRoot(existingItem);
    newParent = na_GetTreeNodeItem(na_NewTreeNodeBin(tree, na_GetBinLeafKey(((NATreeBinLeaf*)right)), left, right));
    
    na_SetTreeItemParent(newParent, existingParent);
    if(!wasTreeItemRoot) {
      NAInt existingIndex = na_GetTreeNodeChildIndex(tree->config, existingParent, existingItem);
      na_MarkNodeChildLeaf(existingParent, existingIndex, NA_FALSE);
      ((NATreeBinNode*)existingParent)->childs[existingIndex] = newParent;
      if(tree->config->flags & NA_TREE_BALANCE_AVL) {
        na_GrowAVL(tree, (NATreeBinNode*)existingParent, existingIndex);
      }
    }else{
      // The leaf was the root of the tree.
      tree->root = newParent;
      na_MarkTreeRootLeaf(tree, NA_FALSE);
    }
  }
  
  return newleaf;
}



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
