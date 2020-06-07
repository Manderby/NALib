
#include "../../NATree.h"
#include "NATreeBin.h"



NA_RUNTIME_TYPE(NATreeBinNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeBinLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void* naGetBinNodeKey(NATreeBinNode* binnode){
  return &(binnode->key);
}
NA_HIDEF void* naGetBinLeafKey(NATreeBinLeaf* binleaf){
  return &(binleaf->key);
}



NA_HIDEF void naAddTreeNodeChildBin(NATree* tree, NATreeBinNode* parent, NATreeItem* child, NAInt childIndex, NABool isChildLeaf){
  NATreeNode* parentNode;
  NA_UNUSED(tree);
  
  parent->childs[childIndex] = child;
  parentNode = naGetBinNodeNode(parent);
  naMarkNodeChildLeaf(parentNode, childIndex, isChildLeaf);
  naSetTreeItemParent(child, parentNode);
}



NA_HHDEF NATreeNode* na_ConstructTreeNodeBin(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf){
  NATreeBinNode* binnode = naNew(NATreeBinNode);
  naInitTreeNode(tree->config, naGetBinNodeNode(binnode), key);

  // Node-specific initialization
  naAddTreeNodeChildBin(tree, binnode, naGetTreeLeafItem(leftleaf),  0, NA_TRUE);
  naAddTreeNodeChildBin(tree, binnode, naGetTreeLeafItem(rightleaf), 1, NA_TRUE);
  if(tree->config->flags & NA_TREE_BALANCE_AVL){na_InitNodeAVL(binnode);}

  return naGetBinNodeNode(binnode);
}



NA_HHDEF NATreeLeaf* na_ConstructTreeLeafBin(NATree* tree, const void* key, NAPtr content){
  NATreeBinLeaf* binleaf = naNew(NATreeBinLeaf);
  naInitTreeLeaf(tree->config, naGetBinLeafLeaf(binleaf), key, content);
  return naGetBinLeafLeaf(binleaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HHDEF NAInt na_GetChildIndexBinDouble(NATreeNode* parentnode, const void* childkey){
  NATreeBinNode* binnode = (NATreeBinNode*)(parentnode);
  return na_GetKeyIndexBinDouble(naGetBinNodeKey(binnode), childkey, NA_NULL);
}
NA_HHDEF NAInt na_GetKeyIndexBinDouble(const void* basekey, const void* testkey, const void* data){
  NA_UNUSED(data);
  return !(*(const double*)testkey < *(const double*)basekey); // results in 0 or 1
}
NA_HHDEF NABool na_TestKeyBinDouble(const void* lowerlimit, const void* upperlimit, const void* key){
  return ((*(const double*)lowerlimit <= *(const double*)key) && (*(const double*)key) < *(const double*)upperlimit);
}
NA_HHDEF NABool na_TestKeyLeafContainBinDouble(NATreeLeaf* leaf, const void* key){
  double* key1 = (double*)naGetBinLeafKey((NATreeBinLeaf*)leaf);
  double* key2 = (double*)key;
  return *key1 == *key2;
}


NA_HHDEF NAInt na_GetChildIndexBinNAInt(NATreeNode* parentnode, const void* childkey){
  NATreeBinNode* binnode = (NATreeBinNode*)(parentnode);
  return na_GetKeyIndexBinNAInt(naGetBinNodeKey(binnode), childkey, NA_NULL);
}
NA_HHDEF NAInt na_GetKeyIndexBinNAInt(const void* basekey, const void* key, const void* data){
  NA_UNUSED(data);
  // if key is equal to basekey, the return value must be 1.
  return !(*(const NAInt*)key < *(const NAInt*)basekey); // results in 0 or 1
}
NA_HHDEF NABool na_TestKeyBinNAInt(const void* lowerlimit, const void* upperlimit, const void* key){
  return ((*(const NAInt*)lowerlimit <= *(const NAInt*)key) && (*(const NAInt*)key <= *(const NAInt*)upperlimit));
}
NA_HHDEF NABool na_TestKeyLeafContainBinNAInt(NATreeLeaf* leaf, const void* key){
  NAInt* key1 = (NAInt*)naGetBinLeafKey((NATreeBinLeaf*)leaf);
  NAInt* key2 = (NAInt*)key;
  return *key1 == *key2;
}



NA_HHDEF void na_DestructTreeNodeBin(NATreeNode* node){
  naDelete(node);
}



NA_HHDEF void na_DestructTreeLeafBin(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HHDEF NATreeNode* na_LocateBubbleBinWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* lowerlimit, const void* upperlimit, NATreeItem* previtem){
  NATreeBinNode* binnode;
  NATreeItem* item;
  
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("node should not be null");
    if(previtem == NA_NULL)
      naError("prevnode should not be null");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  binnode = (NATreeBinNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualComparer(key, naGetBinNodeKey(binnode))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(na_GetTreeNodeChildIndex(tree->config, node, previtem) == 1){
    lowerlimit = naGetBinNodeKey(binnode);
  }else{
    upperlimit = naGetBinNodeKey(binnode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerlimit && upperlimit && tree->config->keyTester(lowerlimit, upperlimit, key)){
    return node;
  }
  // Otherwise, go up if possible.
  item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(item)){
    return na_LocateBubbleBinWithLimits(tree, naGetTreeItemParent(item), key, lowerlimit, upperlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HHDEF NATreeNode* na_LocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key){
  return na_LocateBubbleBinWithLimits(tree, naGetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HHDEF NATreeNode* na_RemoveLeafBin(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = naGetTreeItemParent(naGetTreeLeafItem(leaf));
  NATreeNode* grandparent = NA_NULL;
  if(!naIsTreeItemRoot(naGetTreeLeafItem(leaf))){
    NAInt leafIndex = na_GetTreeNodeChildIndex(tree->config, parent, naGetTreeLeafItem(leaf));
    NATreeItem* sibling = ((NATreeBinNode*)parent)->childs[1 - leafIndex];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafIndex);

    grandparent = naGetTreeItemParent(naGetTreeNodeItem(parent));
    if(!naIsTreeItemRoot(naGetTreeNodeItem(parent))){
      NAInt parentIndex = na_GetTreeNodeChildIndex(tree->config, grandparent, naGetTreeNodeItem(parent));
      ((NATreeBinNode*)grandparent)->childs[parentIndex] = sibling;
      naMarkNodeChildLeaf(grandparent, parentIndex, issiblingleaf);

      if(tree->config->flags & NA_TREE_BALANCE_AVL){na_ShrinkAVL(tree, (NATreeBinNode*)grandparent, parentIndex);}
    }else{
      tree->root = sibling;
      naMarkTreeRootLeaf(tree, issiblingleaf);
    }
    sibling->parent = grandparent;

    naDestructTreeNode(tree->config, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  naDestructTreeLeaf(tree->config, leaf);
  return grandparent;
}



NA_HHDEF NATreeLeaf* na_InsertLeafBin(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  // Create the new leaf and initialize it.
  NATreeLeaf* newleaf = na_ConstructTreeLeafBin(tree, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    NATreeItem* leafItem = naGetTreeLeafItem(newleaf);
    tree->root = leafItem;
    naSetTreeItemParent(leafItem, NA_NULL);
    naMarkTreeRootLeaf(tree, NA_TRUE);
  
  }else{
    NATreeLeaf* existingLeaf;
    NATreeNode* existingParent;
    NABool wasTreeItemRoot;
    NATreeItem* newParent;
    NATreeLeaf* left;
    NATreeLeaf* right;

    #ifndef NDEBUG
      if(!naIsTreeItemLeaf(tree, existingItem))
        naError("Item should be a leaf");
    #endif

    existingLeaf = (NATreeLeaf*)existingItem;
  
    switch(insertOrder){
    case NA_TREE_LEAF_INSERT_ORDER_KEY:
      #ifndef NDEBUG
        if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
          naError("tree is configured with no key");
      #endif
      if(tree->config->keyIndexGetter(naGetBinLeafKey(((NATreeBinLeaf*)existingItem)), naGetBinLeafKey(((NATreeBinLeaf*)newleaf)), NA_NULL) == 1){
        left = existingLeaf;
        right = newleaf;
      }else{
        left = newleaf;
        right = existingLeaf;
      }
      break;
    case NA_TREE_LEAF_INSERT_ORDER_PREV:
      #ifndef NDEBUG
        if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
          naError("tree is configured with key");
      #endif
      left = newleaf;
      right = existingLeaf;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_NEXT:
      #ifndef NDEBUG
        if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
          naError("tree is configured with key");
      #endif
      left = existingLeaf;
      right = newleaf;
      break;
    default:
      #ifndef NDEBUG
      naError("Invalid insertOrder");
      #endif
    left = existingLeaf;
    right = newleaf;
    break;
    }

    existingParent = naGetTreeItemParent(existingItem);
    wasTreeItemRoot = naIsTreeItemRoot(existingItem);
    newParent = naGetTreeNodeItem(na_ConstructTreeNodeBin(tree, naGetBinLeafKey(((NATreeBinLeaf*)right)), left, right));
    
    naSetTreeItemParent(newParent, existingParent);
    if(!wasTreeItemRoot){
      NAInt existingIndex = na_GetTreeNodeChildIndex(tree->config, existingParent, existingItem);
      naMarkNodeChildLeaf(existingParent, existingIndex, NA_FALSE);
      ((NATreeBinNode*)existingParent)->childs[existingIndex] = newParent;
      if(tree->config->flags & NA_TREE_BALANCE_AVL){na_GrowAVL(tree, (NATreeBinNode*)existingParent, existingIndex);}
    }else{
      // The leaf was the root of the tree.
      tree->root = newParent;
      naMarkTreeRootLeaf(tree, NA_FALSE);
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
