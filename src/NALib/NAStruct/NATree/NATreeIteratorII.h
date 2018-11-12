
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations for the tree iterator.
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"



#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataConst(tree);
  iter.basenode = NA_NULL;
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
  iter.basenode = NA_NULL;
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
  iter.basenode = NA_NULL;
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
  naSetTreeIteratorCurNode(iter, NA_NULL);
}



NA_IDEF void naClearTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #endif
}



NA_HIDEF void naSetTreeIteratorCurNode(NATreeIterator* iter, NATreeBaseNode* newnode){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naSetTreeIteratorCurNode", "This iterator has been cleared. You need to make it anew.");
    if(iter->basenode){iter->basenode->itercount--;}
  #endif
  iter->basenode = newnode;
  #ifndef NDEBUG
    if(iter->basenode){iter->basenode->itercount++;}
  #endif
}



NA_IDEF NABool naLocateTree(NATreeIterator* iter, const void* key){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
  #endif

  tree = (const NATree*)naGetPtrConst(&(iter->tree));

  if(!tree->root){
    #ifndef NDEBUG
      if(iter->basenode)
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }
  
  NATreeBaseNode* curnode = iter->basenode;
  NATreeNode* topnode;
  
  // Move the iterator to the topmost inner node which contains the given key.
  if(curnode){
    #ifndef NDEBUG
      NAInt childindx = tree->config->nodechildindexgetter(curnode->parent, curnode);
      if(naGetNodeChildType(curnode->parent, childindx) != NA_TREE_NODE_CHILD_LEAF)
        naError("naIterateTree", "current node is not a leaf");
    #endif
    topnode = tree->config->nodeBubbleLocator(curnode->parent, key);
  }else{
    topnode = tree->root;
  }
  
  // Search for the leaf containing key.
  NABool keyleaffound;
  NATreeBaseNode* node = tree->config->nodeCaptureLocator(topnode, key, &keyleaffound);
  
  if(keyleaffound){
    naSetTreeIteratorCurNode(iter, node);
    return NA_TRUE;
  }else{
    naSetTreeIteratorCurNode(iter, NA_NULL);
    return NA_FALSE;
  }
}



NA_IDEF const void* naGetTreeCurConst(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naGetTreeConst", "This iterator has been cleared. You need to make it anew.");
    if(!iter->basenode)
      naError("naGetTreeConst", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return naGetPtrConst(tree->config->leafDataGetter((NATreeLeaf*)iter->basenode));
}



NA_IAPI void* naGetTreeCurMutable(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
  if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("naGetTreeMutable", "This iterator has been cleared. You need to make it anew.");
  if(!iter->basenode)
    naError("naGetTreeMutable", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return naGetPtrMutable(tree->config->leafDataGetter((NATreeLeaf*)iter->basenode));
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
