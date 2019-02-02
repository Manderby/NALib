
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeBinary.h"



NA_RUNTIME_TYPE(NATreeBinaryNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeBinaryLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void naDestructTreeChildBinary(NATree* tree, NATreeBinaryNode* binnode, NAInt childindx){
  NATreeBaseNode* child = binnode->childs[childindx];
  if(naIsNodeChildLeaf((NATreeNode*)binnode, childindx)){
    naDestructTreeLeafBinary(tree, (NATreeLeaf*)child);
  }else{
    naDestructTreeNodeBinary(tree, (NATreeNode*)child, NA_TRUE);
  }
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt naGetKeyIndexBinaryDouble(const void* basekey, const void* testkey, const void* data){
  NA_UNUSED(data);
  return !(*(const double*)testkey < *(const double*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinaryDouble(const void* key1, const void* key2){
  return (*(const double*)key1 == *(const double*)key2);
}
NA_HDEF void naAssignKeyBinaryDouble(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
}
NA_HDEF NABool naTestKeyBinaryDouble(const void* leftlimit, const void* rightlimit, const void* key){
  return ((*(const double*)leftlimit <= *(const double*)key) && (*(const double*)rightlimit >= *(const double*)key));
}



NA_HDEF NAInt naGetKeyIndexBinaryNAInt(const void* basekey, const void* key, const void* data){
  NA_UNUSED(data);
  return !(*(const NAInt*)key < *(const NAInt*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinaryNAInt(const void* key1, const void* key2){
  return (*(const NAInt*)key1 == *(const NAInt*)key2);
}
NA_HDEF void naAssignKeyBinaryNAInt(void* dst, const void* src){
  *(NAInt*)dst = *(const NAInt*)src;
}
NA_HDEF NABool naTestKeyBinaryNAInt(const void* leftlimit, const void* rightlimit, const void* key){
  return ((*(const NAInt*)leftlimit <= *(const NAInt*)key) && (*(const NAInt*)rightlimit >= *(const NAInt*)key));
}



NA_HDEF NATreeNode* naConstructTreeNodeBinary(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf){
  NATreeBinaryNode* binnode = naNew(NATreeBinaryNode);
  naInitTreeNode((NATreeNode*)binnode);

  #ifndef NDEBUG
    // This check has to be removed until there is a better solution for the
    // different key types than using a union.
//    if(((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY) && key)
//      naError("naConstructTreeNodeBinary", "tree is configured with no key but key is given which is not NULL");
  #endif

  // Node-specific initialization
  if(tree->config->keyAssigner){tree->config->keyAssigner(&(binnode->key), key);}
  binnode->childs[0] = (NATreeBaseNode*)leftleaf;
  binnode->childs[1] = (NATreeBaseNode*)rightleaf;
  naMarkNodeChildLeaf((NATreeNode*)binnode, 0, NA_TRUE);
  naMarkNodeChildLeaf((NATreeNode*)binnode, 1, NA_TRUE);
  ((NATreeBaseNode*)leftleaf)->parent = (NATreeNode*)binnode;
  ((NATreeBaseNode*)rightleaf)->parent = (NATreeNode*)binnode;
  if(tree->config->flags & NA_TREE_BALANCE_AVL){naInitNodeAVL((NATreeBinaryNode*)binnode);}

  if(tree->config->nodeConstructor){
    binnode->data = tree->config->nodeConstructor(&key, tree->config->data);
  }else{
    binnode->data = naMakePtrNull();
  }

  return (NATreeNode*)binnode;
}



NA_HDEF void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node, NABool recursive){
  NATreeBinaryNode* binnode;
  #ifndef NDEBUG
    if(!node)
      naCrash("naDestructTreeNodeBinary", "node shall not be Null");
  #endif
  binnode = (NATreeBinaryNode*)node;
  if(recursive){
    naDestructTreeChildBinary(tree, binnode, 0);
    naDestructTreeChildBinary(tree, binnode, 1);
  }

  if(tree->config->nodeDestructor){tree->config->nodeDestructor(binnode->data, tree->config->data);}
  naClearTreeNode((NATreeNode*)binnode);
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, const void* key, NAPtr data){
  NATreeBinaryLeaf* binleaf = naNew(NATreeBinaryLeaf);
  naInitTreeLeaf(&(binleaf->leaf));

  // Node-specific initialization
  if(key){tree->config->keyAssigner(&(binleaf->key), key);}
  binleaf->data = naConstructLeafData(tree, &(binleaf->key), data);

  return (NATreeLeaf*)binleaf;
}



NA_HDEF void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf;
  #ifndef NDEBUG
    if(!leaf)
      naCrash("naDestructTreeLeafBinary", "leaf shall not be Null");
  #endif
  binleaf = (NATreeBinaryLeaf*)leaf;

  naDestructLeafData(tree, binleaf->data);
  naClearTreeLeaf(&(binleaf->leaf));
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleBinaryWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* leftlimit, const void* rightlimit, NATreeBaseNode* prevnode){
  NATreeBinaryNode* binnode;
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("naLocateBubbleBinaryWithLimits", "node should not be null");
    if(prevnode == NA_NULL)
      naError("naLocateBubbleBinaryWithLimits", "prevnode should not be null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naLocateBubbleBinaryWithLimits", "tree is configured with no key");
  #endif
  binnode = (NATreeBinaryNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(key, &(binnode->key))){return node;}  // todo what about key being <= or >= ?
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetChildIndexBinary(node, prevnode) == 1){
    leftlimit = &(binnode->key);
  }else{
    rightlimit = &(binnode->key);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, key)){
    return node;
  }
  // Otherwise, go up if possible.
  if(((NATreeBaseNode*)node)->parent){
    return naLocateBubbleBinaryWithLimits(tree, ((NATreeBaseNode*)node)->parent, key, leftlimit, rightlimit, (NATreeBaseNode*)binnode);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* naLocateBubbleBinary(const NATree* tree, NATreeLeaf* leaf, const void* key){
  #ifndef NDEBUG
    if(leaf == NA_NULL)
      naCrash("naLocateBubbleBinary", "leaf should not be null");
  #endif
  if(((NATreeBaseNode*)leaf)->parent){
    return naLocateBubbleBinaryWithLimits(tree, ((NATreeBaseNode*)leaf)->parent, key, NA_NULL, NA_NULL, (NATreeBaseNode*)leaf);
  }else{
    return NA_NULL;
  }
}



NA_HDEF NATreeLeaf* naLocateCaptureBinary(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound){
  NATreeBinaryNode* binnode;
  NAInt childindx;
  NATreeBaseNode* child;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naLocateCaptureBinary", "tree is configured with no key");
  #endif

  *matchfound = NA_FALSE;

  if(!node){
    if(tree->root){
      if(naIsTreeRootLeaf(tree)){
        *matchfound = tree->config->keyEqualer(key, &(((NATreeBinaryLeaf*)tree->root)->key));
        return (NATreeLeaf*)tree->root;
      }else{
        node = (NATreeNode*)tree->root;
      }
    }else{
      return NA_NULL;
    }
  }

  binnode = (NATreeBinaryNode*)(node);
  childindx = naGetChildKeyIndexBinary(tree, node, key);
  child = binnode->childs[childindx];

  if(naIsNodeChildLeaf(node, childindx)){
    // When the subtree denotes a leaf, we test, if the key is equal and return
    // the result.
    *matchfound = tree->config->keyEqualer(key, &(((NATreeBinaryLeaf*)child)->key));
    return (NATreeLeaf*)child;
  }else{
    // When the subtree denotes a node, we follow it.
    return naLocateCaptureBinary(tree, (NATreeNode*)child, key, matchfound);
  }
}



NA_HDEF NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child){
  NATreeBinaryNode* binparent;
  NAInt retvalue;
  #ifndef NDEBUG
    if(!child)
      naError("naGetChildIndexBinary", "Child should not be Null");
  #endif
  binparent = (NATreeBinaryNode*)(parent);
  retvalue = (child != binparent->childs[0]); // return 0 or 1
  #ifndef NDEBUG
    if(child != binparent->childs[retvalue])
      naError("naGetChildIndexBinary", "Child is no child of parent");
  #endif
  return retvalue;
}



NA_HDEF NAInt naGetChildKeyIndexBinary(const NATree* tree, NATreeNode* parent, const void* key){
  NATreeBinaryNode* binparent;
  #ifndef NDEBUG
    if(!parent)
      naError("naGetChildKeyIndexBinary", "parent is Null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naGetChildKeyIndexBinary", "tree is configured with no key");
  #endif
  binparent = (NATreeBinaryNode*)(parent);
  return tree->config->keyIndexGetter(&(binparent->key), key, NA_NULL);
}



NA_HDEF NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx){
  NATreeBinaryNode* binnode;
  #ifndef NDEBUG
    if(childindx < 0 || childindx >= 2)
      naCrash("naGetChildBinary", "child index not valid");
  #endif
  binnode = (NATreeBinaryNode*)(parent);
  return binnode->childs[childindx];
}



NA_HDEF NATreeNode* naRemoveLeafBinary(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = ((NATreeBaseNode*)leaf)->parent;
  NATreeNode* grandparent = NA_NULL;
  if(parent){
    NAInt leafindx = naGetChildIndexBinary(parent, (NATreeBaseNode*)leaf);
    NATreeBaseNode* sibling = ((NATreeBinaryNode*)parent)->childs[1 - leafindx];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafindx);

    grandparent = ((NATreeBaseNode*)parent)->parent;
    if(grandparent){
      NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent);
      ((NATreeBinaryNode*)grandparent)->childs[parentindx] = sibling;
      naMarkNodeChildLeaf(grandparent, parentindx, issiblingleaf);

      if(tree->config->flags & NA_TREE_BALANCE_AVL){naShrinkAVL(tree, (NATreeBinaryNode*)grandparent, parentindx);}
    }else{
      tree->root = sibling;
      naMarkTreeRootLeaf(tree, issiblingleaf);
    }
    sibling->parent = grandparent;

    naDestructTreeNodeBinary(tree, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  naDestructTreeLeafBinary(tree, leaf);
  return grandparent;
}



NA_HDEF NATreeLeaf* naInsertLeafBinary(NATree* tree, NATreeLeaf* existingleaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* left;
  NATreeLeaf* right;
  NATreeLeaf* newleaf;
  NATreeNode* existingparent;
  NATreeBinaryNode* newparent;

  if(insertOrder == NA_TREE_LEAF_INSERT_ORDER_REPLACE){
    NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)existingleaf;
    naDestructLeafData(tree, binleaf->data);
    binleaf->data = naConstructLeafData(tree, &(binleaf->key), content);
    return existingleaf;
  }

  newleaf = tree->config->leafCoreConstructor(tree, key, content);

  switch(insertOrder){
  case NA_TREE_LEAF_INSERT_ORDER_KEY:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
        naError("naInsertLeafBinary", "tree is configured with no key");
    #endif
    if(tree->config->keyIndexGetter(&(((NATreeBinaryLeaf*)existingleaf)->key), &(((NATreeBinaryLeaf*)newleaf)->key), NA_NULL) == 1){
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
        naError("naInsertLeafBinary", "tree is configured with key");
    #endif
    left = newleaf;
    right = existingleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_NEXT:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
        naError("naInsertLeafBinary", "tree is configured with key");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_REPLACE:
    #ifndef NDEBUG
      naError("naInsertLeafBinary", "This case should not happen");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  default:
    #ifndef NDEBUG
	  naError("naInsertLeafBinary", "Invalid insertOrder");
    #endif
	left = existingleaf;
	right = newleaf;
	break;
  }

  existingparent = ((NATreeBaseNode*)existingleaf)->parent;
  newparent = (NATreeBinaryNode*)naConstructTreeNodeBinary(tree, &(((NATreeBinaryLeaf*)right)->key), left, right);
  ((NATreeBaseNode*)newparent)->parent = existingparent;
  if(existingparent){
    NAInt existingindx = naGetChildIndexBinary(existingparent, (NATreeBaseNode*)existingleaf);
    naMarkNodeChildLeaf(existingparent, existingindx, NA_FALSE);
    ((NATreeBinaryNode*)existingparent)->childs[existingindx] = (NATreeBaseNode*)newparent;
    if(tree->config->flags & NA_TREE_BALANCE_AVL){naGrowAVL(tree, (NATreeBinaryNode*)existingparent, existingindx);}
  }else{
    // The leaf was the root of the tree.
    tree->root = (NATreeBaseNode*)newparent;
    naMarkTreeRootLeaf(tree, NA_FALSE);
  }

  return newleaf;
}



NA_HDEF const void* naGetLeafKeyBinary(NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)(leaf);
  return &(binleaf->key);
}



NA_HDEF NAPtr naGetLeafDataBinary(NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)(leaf);
  return binleaf->data;
}



NA_HDEF NAPtr naGetNodeDataBinary(NATreeNode* node){
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(node);
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
