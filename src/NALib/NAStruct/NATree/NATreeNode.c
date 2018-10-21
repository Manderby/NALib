
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"

#define NA_TREE_FLAG_CHILD_0_IS_LEAF  0x01
#define NA_TREE_FLAG_CHILD_1_IS_LEAF  0x02



NA_HDEF NATreeNode* naAllocTreeNode(NATreeNode* parent, double key, void* leaf){
  NATreeNode* node = naAlloc(NATreeNode);
  node->parent = parent;
  node->child[0] = NA_NULL;
  node->child[1] = leaf;
  node->key = key;
  node->flags = NA_TREE_FLAG_CHILD_0_IS_LEAF | NA_TREE_FLAG_CHILD_1_IS_LEAF;
  return node;
}



NA_HDEF void naDeallocTreeNode(NATreeNode* node){
  if(node->child[0]){naDeallocTreeNode(node->child[0]);}
  if(node->child[1]){naDeallocTreeNode(node->child[1]);}
  naFree(node);
}



NA_HDEF NABool naLocateTreeNode(NATreeIterator* iter, double key){
  NABool retvalue;
  if(key < iter->node->key){
    if(iter->node->flags & NA_TREE_FLAG_CHILD_0_IS_LEAF){
      iter->childindx = 0;
      retvalue = (iter->node->child[0] != NA_NULL);
    }else{
      iter->node = iter->node->child[0];
      retvalue = naLocateTreeNode(iter, key);
    }
  }else{
    if(iter->node->flags & NA_TREE_FLAG_CHILD_1_IS_LEAF){
      iter->childindx = 1;
      retvalue = (iter->node->child[1] != NA_NULL);
    }else{
      iter->node = iter->node->child[1];
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
