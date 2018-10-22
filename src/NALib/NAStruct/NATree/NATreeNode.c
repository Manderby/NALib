
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


NA_RUNTIME_TYPE(NATreeNode, NA_NULL, NA_FALSE);




NA_HDEF NATreeNode* naAllocTreeNode(NATree* tree, NATreeNode* parent, NAInt indxinparent, double key, void* leaf){
  NATreeNode* node = naNew(NATreeNode);
  node->parent = parent;
  node->indxinparent = indxinparent;
  node->childs[0] = NA_NULL;
  node->childs[1] = leaf;
  node->key = key;
  node->flags = 0;
  naSetNodeChildType(node, 1, NA_TREE_NODE_CHILD_LEAF);
  #ifndef NDEBUG
    node->itercount = 0;
  #endif
  if(tree->config->nodeconstructor){
    node->userdata = tree->config->nodeconstructor(&key);
  }else{
    node->userdata = naMakePtrNull(); 
  }
  return node;
}



NA_HDEF void naDeallocTreeNode(NATree* tree, NATreeNode* node){
  #ifndef NDEBUG
    if(node->itercount)
      naError("naDeallocTreeNode", "There are still iterators running on this node. Did you forget a call to naClearTreeIterator?");
  #endif
  if(node->childs[0]){naDeallocTreeNode(tree, node->childs[0]);}
  if(node->childs[1]){naDeallocTreeNode(tree, node->childs[1]);}
  
  if(tree->config->nodedestructor){tree->config->nodedestructor(node->userdata);}
  
  naDelete(node);
}



NA_HDEF NABool naLocateTreeNode(NATreeIterator* iter, double key){
  NABool retvalue;
  if(key < iter->node->key){
    if(naIsNodeChildLeaf(iter->node, 0)){
      iter->childindx = 0;
      retvalue = (iter->node->childs[0] != NA_NULL);
    }else{
      iter->node = iter->node->childs[0];
      retvalue = naLocateTreeNode(iter, key);
    }
  }else{
    if(naIsNodeChildLeaf(iter->node, 1)){
      iter->childindx = 1;
      retvalue = (iter->node->childs[1] != NA_NULL);
    }else{
      iter->node = iter->node->childs[1];
      retvalue = naLocateTreeNode(iter, key);
    }
  }
  return retvalue;
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
