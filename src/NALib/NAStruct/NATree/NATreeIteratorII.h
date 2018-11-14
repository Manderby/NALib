
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations for the tree iterator.
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"



#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80

typedef struct NATreeIterationInfo NATreeIterationInfo;
struct NATreeIterationInfo{
  NAInt step;
  NAInt startindx;
  NAInt breakindx;
};

NA_HAPI NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataConst(tree);
  iter.leaf = NA_NULL;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.leaf = NA_NULL;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.leaf = NA_NULL;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_IDEF void naResetTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naResetTreeIterator", "This iterator has been cleared. You need to make it anew.");
  #endif
  naSetTreeIteratorCurLeaf(iter, NA_NULL);
}



NA_IDEF void naClearTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #endif
}



NA_HIDEF void naSetTreeIteratorCurLeaf(NATreeIterator* iter, NATreeLeaf* newleaf){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naSetTreeIteratorCurLeaf", "This iterator has been cleared. You need to make it anew.");
    if(iter->leaf){iter->leaf->itercount--;}
  #endif
  iter->leaf = newleaf;
  #ifndef NDEBUG
    if(iter->leaf){iter->leaf->itercount++;}
  #endif
}



NA_IDEF NABool naLocateTree(NATreeIterator* iter, const void* key){
  NABool keyleaffound;
  NAInt childindx;  // unused.
  NATreeBaseNode* node = naLocateTreeNode(iter, key, &keyleaffound, &childindx);
  
  if(keyleaffound){
    naSetTreeIteratorCurLeaf(iter, (NATreeLeaf*)node);
  }else{
    naSetTreeIteratorCurLeaf(iter, NA_NULL);
  }
  return keyleaffound;
}



NA_IDEF const void* naGetTreeCurConst(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naGetTreeConst", "This iterator has been cleared. You need to make it anew.");
    if(!iter->leaf)
      naError("naGetTreeConst", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return naGetPtrConst(tree->config->leafDataGetter(iter->leaf));
}



NA_IDEF void* naGetTreeCurMutable(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
  if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("naGetTreeMutable", "This iterator has been cleared. You need to make it anew.");
  if(!iter->leaf)
    naError("naGetTreeMutable", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return naGetPtrMutable(tree->config->leafDataGetter(iter->leaf));
}



NA_IDEF NABool naIterateTree(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NATreeIterationInfo info = {1, -1, tree->config->childpernode};
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naIterateTreeBack(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NATreeIterationInfo info = {-1, tree->config->childpernode, -1};
  return naIterateTreeWithInfo(iter, &info);
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
