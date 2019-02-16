
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeBin.h"



NA_RUNTIME_TYPE(NATreeBinNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeBinLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void naDestructTreeChildBin(NATree* tree, NATreeBinNode* binnode, NAInt childindx){
  NATreeItem* child = binnode->childs[childindx];
  if(naIsNodeChildLeaf(&(binnode->node), childindx)){
    naDestructTreeLeafBin(tree, (NATreeLeaf*)child);
  }else{
    naDestructTreeNodeBin(tree, (NATreeNode*)child, NA_TRUE);
  }
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



NA_HDEF NATreeNode* naConstructTreeNodeBin(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf){
  NATreeBinNode* binnode = naNew(NATreeBinNode);
  naInitTreeNode(&(binnode->node));

  #ifndef NDEBUG
    // This check has to be removed until there is a better solution for the
    // different key types than using a union.
//    if(((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY) && key)
//      naError("naConstructTreeNodeBin", "tree is configured with no key but key is given which is not NULL");
  #endif

  // Node-specific initialization
  if(tree->config->keyAssigner){tree->config->keyAssigner(&(binnode->key), key);}
  binnode->childs[0] = &(leftleaf->item);
  binnode->childs[1] = &(rightleaf->item);
  naMarkNodeChildLeaf(&(binnode->node), 0, NA_TRUE);
  naMarkNodeChildLeaf(&(binnode->node), 1, NA_TRUE);
  leftleaf->item.parent = &(binnode->node);
  rightleaf->item.parent = &(binnode->node);
  if(tree->config->flags & NA_TREE_BALANCE_AVL){naInitNodeAVL((NATreeBinNode*)binnode);}

  if(tree->config->nodeConstructor){
    binnode->data = tree->config->nodeConstructor(&key, tree->config->data);
  }else{
    binnode->data = naMakePtrNull();
  }

  return &(binnode->node);
}



NA_HDEF void naDestructTreeNodeBin(NATree* tree, NATreeNode* node, NABool recursive){
  NATreeBinNode* binnode;
  #ifndef NDEBUG
    if(!node)
      naCrash("naDestructTreeNodeBin", "node shall not be Null");
  #endif
  binnode = (NATreeBinNode*)node;
  if(recursive){
    naDestructTreeChildBin(tree, binnode, 0);
    naDestructTreeChildBin(tree, binnode, 1);
  }

  if(tree->config->nodeDestructor){tree->config->nodeDestructor(binnode->data, tree->config->data);}
  naClearTreeNode(&(binnode->node));
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBin(NATree* tree, const void* key, NAPtr data){
  NATreeBinLeaf* binleaf = naNew(NATreeBinLeaf);
  naInitTreeLeaf(&(binleaf->leaf));

  // Node-specific initialization
  if(key){tree->config->keyAssigner(&(binleaf->key), key);}
  binleaf->data = naConstructLeafData(tree, &(binleaf->key), data);

  return &(binleaf->leaf);
}



NA_HDEF void naDestructTreeLeafBin(NATree* tree, NATreeLeaf* leaf){
  NATreeBinLeaf* binleaf;
  #ifndef NDEBUG
    if(!leaf)
      naCrash("naDestructTreeLeafBin", "leaf shall not be Null");
  #endif
  binleaf = (NATreeBinLeaf*)leaf;

  naDestructLeafData(tree, binleaf->data);
  naClearTreeLeaf(&(binleaf->leaf));
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleBinWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* leftlimit, const void* rightlimit, NATreeItem* previtem){
  NATreeBinNode* binnode;
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("naLocateBubbleBinWithLimits", "node should not be null");
    if(previtem == NA_NULL)
      naError("naLocateBubbleBinWithLimits", "prevnode should not be null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naLocateBubbleBinWithLimits", "tree is configured with no key");
  #endif
  binnode = (NATreeBinNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(key, &(binnode->key))){return node;}  // todo what about key being <= or >= ?
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetChildIndexBin(node, previtem) == 1){
    leftlimit = &(binnode->key);
  }else{
    rightlimit = &(binnode->key);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, key)){
    return node;
  }
  // Otherwise, go up if possible.
  NATreeItem* item = &(node->item);
  if(!naIsTreeItemRoot(tree, item)){
    return naLocateBubbleBinWithLimits(tree, item->parent, key, leftlimit, rightlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* naLocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key){
  return naLocateBubbleBinWithLimits(tree, item->parent, key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeLeaf* naLocateCaptureBin(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound){
  NATreeBinNode* binnode;
  NAInt childindx;
  NATreeItem* child;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naLocateCaptureBin", "tree is configured with no key");
  #endif

  *matchfound = NA_FALSE;

  if(!node){
    if(naIsTreeRootLeaf(tree)){
      *matchfound = tree->config->keyEqualer(key, &(((NATreeBinLeaf*)tree->root)->key));
      return (NATreeLeaf*)tree->root;
    }else{
      node = (NATreeNode*)tree->root;
    }
  }

  binnode = (NATreeBinNode*)(node);
  childindx = naGetChildKeyIndexBin(tree, node, key);
  child = binnode->childs[childindx];

  if(naIsNodeChildLeaf(node, childindx)){
    // When the subtree denotes a leaf, we test, if the key is equal and return
    // the result.
    *matchfound = tree->config->keyEqualer(key, &(((NATreeBinLeaf*)child)->key));
    return (NATreeLeaf*)child;
  }else{
    // When the subtree denotes a node, we follow it.
    return naLocateCaptureBin(tree, (NATreeNode*)child, key, matchfound);
  }
}



NA_HDEF NAInt naGetChildIndexBin(NATreeNode* parent, NATreeItem* child){
  NATreeBinNode* binparent;
  NAInt retvalue;
  #ifndef NDEBUG
    if(!child)
      naError("naGetChildIndexBin", "Child should not be Null");
  #endif
  binparent = (NATreeBinNode*)(parent);
  retvalue = (child != binparent->childs[0]); // return 0 or 1
  #ifndef NDEBUG
    if(child != binparent->childs[retvalue])
      naError("naGetChildIndexBin", "Child is no child of parent");
  #endif
  return retvalue;
}



NA_HDEF NAInt naGetChildKeyIndexBin(const NATree* tree, NATreeNode* parent, const void* key){
  NATreeBinNode* binparent;
  #ifndef NDEBUG
    if(!parent)
      naError("naGetChildKeyIndexBin", "parent is Null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naGetChildKeyIndexBin", "tree is configured with no key");
  #endif
  binparent = (NATreeBinNode*)(parent);
  return tree->config->keyIndexGetter(&(binparent->key), key, NA_NULL);
}



NA_HDEF NATreeItem* naGetChildBin(NATreeNode* parent, NAInt childindx){
  NATreeBinNode* binnode;
  #ifndef NDEBUG
    if(childindx < 0 || childindx >= 2)
      naCrash("naGetChildBin", "child index not valid");
  #endif
  binnode = (NATreeBinNode*)(parent);
  return binnode->childs[childindx];
}



NA_HDEF NATreeNode* naRemoveLeafBin(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = leaf->item.parent;
  NATreeNode* grandparent = NA_NULL;
  if(!naIsTreeItemRoot(tree, &(leaf->item))){
    NAInt leafindx = naGetChildIndexBin(parent, &(leaf->item));
    NATreeItem* sibling = ((NATreeBinNode*)parent)->childs[1 - leafindx];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafindx);

    grandparent = parent->item.parent;
    if(!naIsTreeItemRoot(tree, &(parent->item))){
      NAInt parentindx = naGetChildIndexBin(grandparent, &(parent->item));
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
  naDestructTreeLeafBin(tree, leaf);
  return grandparent;
}



NA_HDEF NATreeLeaf* naInsertLeafBin(NATree* tree, NATreeLeaf* existingleaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* left;
  NATreeLeaf* right;
  NATreeLeaf* newleaf;
  NATreeNode* existingparent;
  NATreeBinNode* newparent;

  if(insertOrder == NA_TREE_LEAF_INSERT_ORDER_REPLACE){
    NATreeBinLeaf* binleaf = (NATreeBinLeaf*)existingleaf;
    naDestructLeafData(tree, binleaf->data);
    binleaf->data = naConstructLeafData(tree, &(binleaf->key), content);
    return existingleaf;
  }

  newleaf = tree->config->leafCoreConstructor(tree, key, content);

  switch(insertOrder){
  case NA_TREE_LEAF_INSERT_ORDER_KEY:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
        naError("naInsertLeafBin", "tree is configured with no key");
    #endif
    if(tree->config->keyIndexGetter(&(((NATreeBinLeaf*)existingleaf)->key), &(((NATreeBinLeaf*)newleaf)->key), NA_NULL) == 1){
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
        naError("naInsertLeafBin", "tree is configured with key");
    #endif
    left = newleaf;
    right = existingleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_NEXT:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
        naError("naInsertLeafBin", "tree is configured with key");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_REPLACE:
    #ifndef NDEBUG
      naError("naInsertLeafBin", "This case should not happen");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  default:
    #ifndef NDEBUG
	  naError("naInsertLeafBin", "Invalid insertOrder");
    #endif
	left = existingleaf;
	right = newleaf;
	break;
  }

  existingparent = existingleaf->item.parent;
  NABool wasTreeItemRoot = naIsTreeItemRoot(tree, &(existingleaf->item));
  newparent = (NATreeBinNode*)naConstructTreeNodeBin(tree, &(((NATreeBinLeaf*)right)->key), left, right);
  newparent->node.item.parent = existingparent;
  if(!wasTreeItemRoot){
    NAInt existingindx = naGetChildIndexBin(existingparent, &(existingleaf->item));
    naMarkNodeChildLeaf(existingparent, existingindx, NA_FALSE);
    ((NATreeBinNode*)existingparent)->childs[existingindx] = naGetBinNodeItem(newparent);
    if(tree->config->flags & NA_TREE_BALANCE_AVL){naGrowAVL(tree, (NATreeBinNode*)existingparent, existingindx);}
  }else{
    // The leaf was the root of the tree.
    tree->root = naGetBinNodeItem(newparent);
    naMarkTreeRootLeaf(tree, NA_FALSE);
  }

  return newleaf;
}



NA_HDEF const void* naGetLeafKeyBin(NATreeLeaf* leaf){
  NATreeBinLeaf* binleaf = (NATreeBinLeaf*)(leaf);
  return &(binleaf->key);
}



NA_HDEF NAPtr naGetLeafDataBin(NATreeLeaf* leaf){
  NATreeBinLeaf* binleaf = (NATreeBinLeaf*)(leaf);
  return binleaf->data;
}



NA_HDEF NAPtr naGetNodeDataBin(NATreeNode* node){
  NATreeBinNode* binnode = (NATreeBinNode*)(node);
  return binnode->data;
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
