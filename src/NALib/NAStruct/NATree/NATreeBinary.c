
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"
#include "NACoord.h"

struct NATreeBinaryNode{
  NATreeNode node;
  double key;
  NATreeBaseNode* childs[2];
  NAPtr data;
};
NA_RUNTIME_TYPE(NATreeBinaryNode, NA_NULL, NA_FALSE);

struct NATreeBinaryLeaf{
  NATreeLeaf leaf;
  double key;
  NAPtr data;
};
NA_RUNTIME_TYPE(NATreeBinaryLeaf, NA_NULL, NA_FALSE);



NA_HDEF NATreeNode* naConstructTreeNodeBinary(NATree* tree, NATreeNode* parent, double key, NAPtr data){
  NATreeBinaryNode* binnode = naNew(NATreeBinaryNode);
  naInitTreeNode(&(binnode->node), parent);

  // Node-specific initialization
  binnode->key = key;
  binnode->childs[0] = NA_NULL;
  binnode->childs[1] = NA_NULL;
  naSetNodeChildType(&(binnode->node), 0, NA_TREE_NODE_CHILD_NULL);
  naSetNodeChildType(&(binnode->node), 1, NA_TREE_NODE_CHILD_NULL);

  if(tree->config->nodeConstructor){
    binnode->data = tree->config->nodeConstructor(&key, tree->config->data, data);
  }else{
    binnode->data = data; 
  }

  return &(binnode->node);
}



NA_HIDEF void naDestructTreeChildBinary(NATree* tree, NATreeBinaryNode* binnode, NAInt childindx){
  NATreeBaseNode* child = binnode->childs[childindx];
  NANodeChildType childtype = naGetNodeChildType(child->parent, childindx);
  if(childtype == NA_TREE_NODE_CHILD_LEAF){tree->config->leafCoreDestructor(tree, (NATreeLeaf*)child);}
  else if(childtype == NA_TREE_NODE_CHILD_NODE){tree->config->nodeCoreDestructor(tree, (NATreeNode*)child);}
}



NA_HDEF void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node){
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)node;
  naDestructTreeChildBinary(tree, binnode, 0);
  naDestructTreeChildBinary(tree, binnode, 1);
  
  if(tree->config->nodeDestructor){tree->config->nodeDestructor(binnode->data, tree->config->data);}
  naClearTreeNode(&(binnode->node));
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, NATreeNode* parent, double key, NAPtr data){
  NATreeBinaryLeaf* binleaf = naNew(NATreeBinaryLeaf);
  naInitTreeLeaf(&(binleaf->leaf), parent);
  
  // Node-specific initialization
  binleaf->key = key;
  if(tree->config->leafConstructor){
    binleaf->data = tree->config->leafConstructor(&key, tree->config->data, data);
  }else{
    binleaf->data = data; 
  }

  return &(binleaf->leaf);
}



NA_HDEF void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)leaf;
  
  if(tree->config->leafDestructor){tree->config->leafDestructor(binleaf->data, tree->config->data);}
  naClearTreeLeaf(&(binleaf->leaf));
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateNodeBubbleBinaryWithLimits(NATreeNode* node, const double* key, double* leftlimit, double* rightlimit, NATreeBaseNode* prevnode){
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("naLocateNodeBubbleBinaryWithLimits", "node should not be null");
  #endif
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(*key == binnode->key){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetChildIndexBinary(&(binnode->node), prevnode) == 0){
    rightlimit = &(binnode->key);
  }else{
    // Note that this else case will also happen if NA_NULL was sent as prevnode.
    leftlimit = &(binnode->key);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && naContainsRangeOffset(naMakeRangeWithStartAndEnd(*leftlimit, *rightlimit), *key)){
    return node;
  }
  // Otherwise, go up if possible.
  if(node->basenode.parent){
    return naLocateNodeBubbleBinaryWithLimits(node->basenode.parent, key, leftlimit, rightlimit, &(binnode->node.basenode));
  }else{
    // If this node is the root node, we return this node.
    return node;
  }
}



NA_HDEF NATreeNode* naLocateNodeBubbleBinary(NATreeNode* node, const void* key){
  const double* dkey = (const double*)key;
  return naLocateNodeBubbleBinaryWithLimits(node, dkey, NA_NULL, NA_NULL, NA_NULL);
}



NA_HDEF NATreeBaseNode* naLocateNodeCaptureBinary(NATreeNode* node, const void* key, NABool* keyleaffound){
  #ifndef NDEBUG
    if(!node)
      naError("naLocateNodeCaptureBinary", "node must not be Null");
  #endif
  
  *keyleaffound = NA_FALSE;
  const double* dkey = (const double*)key;
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(node);
  NAInt childindx = !(*(const double*)key < binnode->key);  // results in 0 or 1
  NANodeChildType childtype = naGetNodeChildType(&(binnode->node), childindx);
  NATreeBaseNode* childnode = binnode->childs[childindx];
  
  if(childtype == NA_TREE_NODE_CHILD_NODE){
    // When the subtree denotes a node, we follow it.
    return naLocateNodeCaptureBinary((NATreeNode*)childnode, dkey, keyleaffound);
  }else if((childtype == NA_TREE_NODE_CHILD_LEAF) && (*dkey == ((NATreeBinaryLeaf*)childnode)->key)){
    // When the subtree denotes a leaf and it has the desire key, success!
    *keyleaffound = NA_TRUE;
    return childnode; // Good ending
  }
  
  // In any other case (wrong leaf, subtree not available or subtree denoting
  // a link, we give up and return the current node.
  return &(binnode->node.basenode);
}



NA_HDEF NABool naTestNodeLimitBinary(NATreeIterator* iter, const void* limit){
  NA_UNUSED(iter);
  NA_UNUSED(limit);
  
  // todo.
  
  return NA_TRUE;
}



NA_HDEF NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child){
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(parent);
  if(!child){return -1;}
  if(child == binnode->childs[0]){return 0;}
  #ifndef NDEBUG
    if(child != binnode->childs[1])
      naError("naGetChildIndexBinary", "Child is no child of parent");
  #endif
  return 1;
}



NA_HDEF NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx){
  #ifndef NDEBUG
    if(childindx < 0 || childindx >= 2)
      naError("naGetChildBinary", "child index not valid");
  #endif
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(parent);
  return binnode->childs[childindx];
}



NA_HDEF NAPtr* naGetTreeLeafDataBinary(NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)(leaf);
  return &(binleaf->data);
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
