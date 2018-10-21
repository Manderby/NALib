
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


#define NA_TREE_ITERATOR_MODIFIER 0x01


NA_DEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.treeptr = naMakePtrWithDataConst(tree);
  iter.node = NA_NULL;
  iter.childindx = -1;
  iter.flags = 0;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_DEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  iter.treeptr = naMakePtrWithDataMutable(tree);
  iter.node = NA_NULL;
  iter.childindx = -1;
  iter.flags = 0;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_DEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  iter.treeptr = naMakePtrWithDataMutable(tree);
  iter.node = NA_NULL;
  iter.childindx = -1;
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_DEF void naClearTreeIterator(NATreeIterator* iter){
  NA_UNUSED(iter);
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)naGetPtrConst(&(iter->treeptr));
    mutabletree->itercount--;
  #endif
}



NA_DEF NABool naLocateTree(NATreeIterator* iter, double key){
  if(!iter->node){
    const NATree* tree = naGetPtrConst(&(iter->treeptr));
    iter->node = tree->root;    
  }
  if(iter->node){
    return naLocateTreeNode(iter, key);
  }else{
    return NA_FALSE;
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
