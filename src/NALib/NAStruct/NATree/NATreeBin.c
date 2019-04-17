
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

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
  NA_UNUSED(tree);
  parent->childs[childIndex] = child;
  NATreeNode* parentNode = naGetBinNodeNode(parent);
  naMarkNodeChildLeaf(parentNode, childIndex, isChildLeaf);
  naSetTreeItemParent(child, parentNode);
}



NA_HDEF NATreeNode* naConstructTreeNodeBin(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf){
  NATreeBinNode* binnode = naNew(NATreeBinNode);
  naInitTreeNode(tree, naGetBinNodeNode(binnode), key);

  // Node-specific initialization
  naAddTreeNodeChildBin(tree, binnode, naGetTreeLeafItem(leftleaf),  0, NA_TRUE);
  naAddTreeNodeChildBin(tree, binnode, naGetTreeLeafItem(rightleaf), 1, NA_TRUE);
  if(tree->config->flags & NA_TREE_BALANCE_AVL){naInitNodeAVL(binnode);}

  return naGetBinNodeNode(binnode);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBin(NATree* tree, const void* key, NAPtr content){
  NATreeBinLeaf* binleaf = naNew(NATreeBinLeaf);
  naInitTreeLeaf(tree, naGetBinLeafLeaf(binleaf), key, content);
  return naGetBinLeafLeaf(binleaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt naGetChildIndexBinDouble(NATreeNode* parentnode, const void* childkey){
  NATreeBinNode* binnode = (NATreeBinNode*)(parentnode);
  return naGetKeyIndexBinDouble(naGetBinNodeKey(binnode), childkey, NA_NULL);
}
NA_HDEF NAInt naGetKeyIndexBinDouble(const void* basekey, const void* testkey, const void* data){
  NA_UNUSED(data);
  return !(*(const double*)testkey < *(const double*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinDouble(const void* key1, const void* key2){
  return (*(const double*)key1 == *(const double*)key2);
}
NA_HDEF NABool naLowerKeyBinDouble(const void* key1, const void* key2){
  return (*(const double*)key1 < *(const double*)key2);
}
NA_HDEF NABool naLowerEqualKeyBinDouble(const void* key1, const void* key2){
  return (*(const double*)key1 <= *(const double*)key2);
}
NA_HDEF void naAssignKeyBinDouble(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
}
NA_HDEF void naAddKeyBinDouble(void* dst, const void* src1, const void* src2){
  *(double*)dst = *(const double*)src1 + *(const double*)src2;
}
NA_HDEF NABool naTestKeyBinDouble(const void* lowerlimit, const void* upperlimit, const void* key){
  return ((*(const double*)lowerlimit <= *(const double*)key) && (*(const double*)key) < *(const double*)upperlimit);
}



NA_HDEF NAInt naGetChildIndexBinNAInt(NATreeNode* parentnode, const void* childkey){
  NATreeBinNode* binnode = (NATreeBinNode*)(parentnode);
  return naGetKeyIndexBinNAInt(naGetBinNodeKey(binnode), childkey, NA_NULL);
}
NA_HDEF NAInt naGetKeyIndexBinNAInt(const void* basekey, const void* key, const void* data){
  NA_UNUSED(data);
  // if key is equal to basekey, the return value must be 1.
  return !(*(const NAInt*)key < *(const NAInt*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinNAInt(const void* key1, const void* key2){
  return (*(const NAInt*)key1 == *(const NAInt*)key2);
}
NA_HDEF NABool naLowerKeyBinNAInt(const void* key1, const void* key2){
  return (*(const NAInt*)key1 < *(const NAInt*)key2);
}
NA_HDEF NABool naLowerEqualKeyBinNAInt(const void* key1, const void* key2){
  return (*(const NAInt*)key1 <= *(const NAInt*)key2);
}
NA_HDEF void naAssignKeyBinNAInt(void* dst, const void* src){
  *(NAInt*)dst = *(const NAInt*)src;
}
NA_HDEF void naAddKeyBinNAInt(void* dst, const void* src1, const void* src2){
  *(NAInt*)dst = *(const NAInt*)src1 + *(const NAInt*)src2;
}
NA_HDEF NABool naTestKeyBinNAInt(const void* lowerlimit, const void* upperlimit, const void* key){
  return ((*(const NAInt*)lowerlimit <= *(const NAInt*)key) && (*(const NAInt*)upperlimit >= *(const NAInt*)key));
}



NA_HDEF void naDestructTreeNodeBin(NATreeNode* node){
  naDelete(node);
}



NA_HDEF void naDestructTreeLeafBin(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleBinWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* lowerlimit, const void* upperlimit, NATreeItem* previtem){
  NATreeBinNode* binnode;
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
  if(naGetTreeNodeChildIndex(tree, node, previtem) == 1){
    lowerlimit = naGetBinNodeKey(binnode);
  }else{
    upperlimit = naGetBinNodeKey(binnode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerlimit && upperlimit && tree->config->keyTester(lowerlimit, upperlimit, key)){
    return node;
  }
  // Otherwise, go up if possible.
  NATreeItem* item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(tree, item)){
    return naLocateBubbleBinWithLimits(tree, naGetTreeItemParent(item), key, lowerlimit, upperlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* naLocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key){
  return naLocateBubbleBinWithLimits(tree, naGetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeNode* naRemoveLeafBin(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = naGetTreeItemParent(naGetTreeLeafItem(leaf));
  NATreeNode* grandparent = NA_NULL;
  if(!naIsTreeItemRoot(tree, naGetTreeLeafItem(leaf))){
    NAInt leafindx = naGetTreeNodeChildIndex(tree, parent, naGetTreeLeafItem(leaf));
    NATreeItem* sibling = ((NATreeBinNode*)parent)->childs[1 - leafindx];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafindx);

    grandparent = naGetTreeItemParent(naGetTreeNodeItem(parent));
    if(!naIsTreeItemRoot(tree, naGetTreeNodeItem(parent))){
      NAInt parentindx = naGetTreeNodeChildIndex(tree, grandparent, naGetTreeNodeItem(parent));
      ((NATreeBinNode*)grandparent)->childs[parentindx] = sibling;
      naMarkNodeChildLeaf(grandparent, parentindx, issiblingleaf);

      if(tree->config->flags & NA_TREE_BALANCE_AVL){naShrinkAVL(tree, (NATreeBinNode*)grandparent, parentindx);}
    }else{
      tree->root = sibling;
      naMarkTreeRootLeaf(tree, issiblingleaf);
    }
    sibling->parent = grandparent;

    naDestructTreeNode(tree, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  naDestructTreeLeaf(tree, leaf);
  return grandparent;
}



NA_HDEF NATreeLeaf* naInsertLeafBin(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* left;
  NATreeLeaf* right;

  // Create the new leaf and initialize it.
  NATreeLeaf* newleaf = naConstructTreeLeafBin(tree, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    NATreeItem* leafItem = naGetTreeLeafItem(newleaf);
    tree->root = leafItem;
    naSetTreeItemParent(leafItem, NA_NULL);
    naMarkTreeRootLeaf(tree, NA_TRUE);
  
  }else{

    #ifndef NDEBUG
      if(!naIsTreeItemLeaf(tree, existingItem))
        naError("Item should be a leaf");
    #endif
    NATreeLeaf* existingLeaf = (NATreeLeaf*)existingItem;

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

    NATreeNode* existingParent = naGetTreeItemParent(existingItem);
    NABool wasTreeItemRoot = naIsTreeItemRoot(tree, existingItem);
    NATreeItem* newParent = naGetTreeNodeItem(naConstructTreeNodeBin(tree, naGetBinLeafKey(((NATreeBinLeaf*)right)), left, right));
    
    naSetTreeItemParent(newParent, existingParent);
    if(!wasTreeItemRoot){
      NAInt existingindx = naGetTreeNodeChildIndex(tree, existingParent, existingItem);
      naMarkNodeChildLeaf(existingParent, existingindx, NA_FALSE);
      ((NATreeBinNode*)existingParent)->childs[existingindx] = newParent;
      if(tree->config->flags & NA_TREE_BALANCE_AVL){naGrowAVL(tree, (NATreeBinNode*)existingParent, existingindx);}
    }else{
      // The leaf was the root of the tree.
      tree->root = newParent;
      naMarkTreeRootLeaf(tree, NA_FALSE);
    }
  }
  
  return newleaf;
}



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
