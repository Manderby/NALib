
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



NA_HIDEF void naDestructTreeChildBin(NATree* tree, NATreeBinNode* binnode, NAInt childindx){
  NATreeItem* child = binnode->childs[childindx];
  if(naIsNodeChildLeaf(naGetBinNodeNode(binnode), childindx)){
    naDestructTreeLeaf(tree, (NATreeLeaf*)child);
  }else{
    naDestructTreeNodeBin(tree, (NATreeNode*)child, NA_TRUE);
  }
}



NA_HIDEF void naAddTreeNodeChildBin(NATree* tree, NATreeBinNode* parent, NATreeItem* child, NAInt childindx, NABool isChildLeaf){
  NA_UNUSED(tree);
  parent->childs[childindx] = child;
  naMarkNodeChildLeaf(naGetBinNodeNode(parent), childindx, isChildLeaf);
  naSetTreeItemParent(child, naGetBinNodeNode(parent));
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt naGetKeyIndexBinDouble(const void* basekey, const void* testkey, const void* data){
  NA_UNUSED(data);
  return !(*(const double*)testkey < *(const double*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinDouble(const void* key1, const void* key2){
  return (*(const double*)key1 == *(const double*)key2);
}
NA_HDEF void naAssignKeyBinDouble(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
}
NA_HDEF NABool naTestKeyBinDouble(const void* leftlimit, const void* rightlimit, const void* key){
  return ((*(const double*)leftlimit <= *(const double*)key) && (*(const double*)rightlimit >= *(const double*)key));
}



NA_HDEF NAInt naGetKeyIndexBinNAInt(const void* basekey, const void* key, const void* data){
  NA_UNUSED(data);
  return !(*(const NAInt*)key < *(const NAInt*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinNAInt(const void* key1, const void* key2){
  return (*(const NAInt*)key1 == *(const NAInt*)key2);
}
NA_HDEF void naAssignKeyBinNAInt(void* dst, const void* src){
  *(NAInt*)dst = *(const NAInt*)src;
}
NA_HDEF NABool naTestKeyBinNAInt(const void* leftlimit, const void* rightlimit, const void* key){
  return ((*(const NAInt*)leftlimit <= *(const NAInt*)key) && (*(const NAInt*)rightlimit >= *(const NAInt*)key));
}



NA_HDEF NATreeBinNode* naConstructTreeNodeBin(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf){
  NATreeBinNode* binnode = naNew(NATreeBinNode);
  naInitTreeNode(naGetBinNodeNode(binnode));

  #ifndef NDEBUG
    // This check has to be removed until there is a better solution for the
    // different key types than using a union.
//    if(((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY) && key)
//      naError("tree is configured with no key but key is given which is not NULL");
  #endif

  // Node-specific initialization
  if(tree->config->keyAssigner){tree->config->keyAssigner(naGetBinNodeKey(binnode), key);}

  naAddTreeNodeChildBin(tree, binnode, naGetTreeLeafItem(leftleaf),  0, NA_TRUE);
  naAddTreeNodeChildBin(tree, binnode, naGetTreeLeafItem(rightleaf), 1, NA_TRUE);

  if(tree->config->flags & NA_TREE_BALANCE_AVL){naInitNodeAVL((NATreeBinNode*)binnode);}

  if(tree->config->nodeDataConstructor){
    binnode->data = tree->config->nodeDataConstructor(&key, tree->config->data);
  }else{
    binnode->data = naMakePtrNull();
  }

  return binnode;
}



NA_HDEF void naDestructTreeNodeBin(NATree* tree, NATreeNode* node, NABool recursive){
  NATreeBinNode* binnode;
  #ifndef NDEBUG
    if(!node)
      naCrash("node shall not be Null");
  #endif
  binnode = (NATreeBinNode*)node;
  if(recursive){
    naDestructTreeChildBin(tree, binnode, 0);
    naDestructTreeChildBin(tree, binnode, 1);
  }

  if(tree->config->nodeDataDestructor){tree->config->nodeDataDestructor(binnode->data, tree->config->data);}
  naClearTreeNode(naGetBinNodeNode(binnode));
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBin(NATree* tree, const void* key, NAPtr data){
  NATreeBinLeaf* binleaf = naNew(NATreeBinLeaf);
  naInitTreeLeaf(naGetBinLeafLeaf(binleaf));

  // Node-specific initialization
  if(key){tree->config->keyAssigner(naGetBinLeafKey(binleaf), key);}
  binleaf->data = naConstructLeafData(tree, naGetBinLeafKey(binleaf ), data);

  return naGetBinLeafLeaf(binleaf);
}



NA_HDEF NATreeNode* naLocateBubbleBinWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* leftlimit, const void* rightlimit, NATreeItem* previtem){
  NATreeBinNode* binnode;
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("node should not be null");
    if(previtem == NA_NULL)
      naError("prevnode should not be null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  binnode = (NATreeBinNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(key, naGetBinNodeKey(binnode))){return node;}  // todo what about key being <= or >= ?
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetTreeNodeChildIndex(tree, node, previtem) == 1){
    leftlimit = naGetBinNodeKey(binnode);
  }else{
    rightlimit = naGetBinNodeKey(binnode);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, key)){
    return node;
  }
  // Otherwise, go up if possible.
  NATreeItem* item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(tree, item)){
    return naLocateBubbleBinWithLimits(tree, naGetTreeItemParent(item), key, leftlimit, rightlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* naLocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key){
  return naLocateBubbleBinWithLimits(tree, naGetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeLeaf* naLocateCaptureBin(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound){
  NATreeBinNode* binnode;
  NAInt childindx;
  NATreeItem* child;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif

  *matchfound = NA_FALSE;

  if(!node){
    if(naIsTreeRootLeaf(tree)){
      *matchfound = tree->config->keyEqualer(key, naGetBinLeafKey(((NATreeBinLeaf*)tree->root)));
      return (NATreeLeaf*)tree->root;
    }else{
      node = (NATreeNode*)tree->root;
    }
  }

  binnode = (NATreeBinNode*)(node);
  childindx = tree->config->keyIndexGetter(naGetBinNodeKey(binnode), key, NA_NULL);
  child = binnode->childs[childindx];

  if(naIsNodeChildLeaf(node, childindx)){
    // When the subtree denotes a leaf, we test, if the key is equal and return
    // the result.
    *matchfound = tree->config->keyEqualer(key, naGetBinLeafKey(((NATreeBinLeaf*)child)));
    return (NATreeLeaf*)child;
  }else{
    // When the subtree denotes a node, we follow it.
    return naLocateCaptureBin(tree, (NATreeNode*)child, key, matchfound);
  }
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

    naDestructTreeNodeBin(tree, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  naDestructTreeLeaf(tree, leaf);
  return grandparent;
}



NA_HDEF void naInsertLeafBin(NATree* tree, NATreeLeaf* existingleaf, NATreeLeaf* newleaf, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* left;
  NATreeLeaf* right;
  NATreeNode* existingparent;
  NATreeBinNode* newparent;

  switch(insertOrder){
  case NA_TREE_LEAF_INSERT_ORDER_KEY:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
        naError("tree is configured with no key");
    #endif
    if(tree->config->keyIndexGetter(naGetBinLeafKey(((NATreeBinLeaf*)existingleaf)), naGetBinLeafKey(((NATreeBinLeaf*)newleaf)), NA_NULL) == 1){
      left = existingleaf;
      right = newleaf;
    }else{
      left = newleaf;
      right = existingleaf;
    }
    break;
  case NA_TREE_LEAF_INSERT_ORDER_PREV:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
        naError("tree is configured with key");
    #endif
    left = newleaf;
    right = existingleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_NEXT:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
        naError("tree is configured with key");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  default:
    #ifndef NDEBUG
	  naError("Invalid insertOrder");
    #endif
	left = existingleaf;
	right = newleaf;
	break;
  }

  existingparent = naGetTreeItemParent(naGetTreeLeafItem(existingleaf));
  NABool wasTreeItemRoot = naIsTreeItemRoot(tree, naGetTreeLeafItem(existingleaf));
  newparent = naConstructTreeNodeBin(tree, naGetBinLeafKey(((NATreeBinLeaf*)right)), left, right);
  naSetTreeItemParent(naGetBinNodeItem(newparent), existingparent);
  if(!wasTreeItemRoot){
    NAInt existingindx = naGetTreeNodeChildIndex(tree, existingparent, naGetTreeLeafItem(existingleaf));
    naMarkNodeChildLeaf(existingparent, existingindx, NA_FALSE);
    ((NATreeBinNode*)existingparent)->childs[existingindx] = naGetBinNodeItem(newparent);
    if(tree->config->flags & NA_TREE_BALANCE_AVL){naGrowAVL(tree, (NATreeBinNode*)existingparent, existingindx);}
  }else{
    // The leaf was the root of the tree.
    tree->root = naGetBinNodeItem(newparent);
    naMarkTreeRootLeaf(tree, NA_FALSE);
  }
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
