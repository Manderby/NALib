
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


#define NA_TREE_ITERATOR_MODIFIER 0x01


NA_DEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
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
  iter.tree = naMakePtrWithDataMutable(tree);
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
  iter.tree = naMakePtrWithDataMutable(tree);
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
    NATree* mutabletree = (NATree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
  #endif
}



NA_HIDEF void naSetTreeIteratorCurNode(NATreeIterator* iter, NATreeNode* newnode){
  #ifndef NDEBUG
    if(iter->node){iter->node->itercount--;}
  #endif
  iter->node = newnode;
  #ifndef NDEBUG
    if(iter->node){iter->node->itercount++;}
  #endif
}



NA_DEF void naResetTreeIterator(NATreeIterator* iter){
  naSetTreeIteratorCurNode(iter, NA_NULL);
  iter->childindx = -1;
}



NA_DEF NABool naLocateTree(NATreeIterator* iter, double key){
  if(!iter->node){
    const NATree* tree = naGetPtrConst(&(iter->tree));
    iter->node = tree->root;    
  }
  if(iter->node){
    return naLocateTreeNode(iter, key);
  }else{
    return NA_FALSE;
  }
}



NA_DEF NABool naIterateTree(NATreeIterator* iter, const void* limit){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  
  // If the tree has no root, we can not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(iter->node)
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }
  
  // If the iterator has no current node being visited, we use the root. 
  if(!iter->node){
    naSetTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->node)
      naCrash("naIterateTree", "No current node after setting the current node to the root");
    #endif
  }

  // We go to the next child index. If we came to this function with index -1,
  // we therefore end up with index 0.
  iter->childindx++;
    
  // Search for a child index which is available and within limit.
  while(iter->childindx < tree->config->childpernode){
    if(naHasNodeChild(iter->node, iter->childindx)){
      if(!limit){
        // If there is no limit, we found a new segment.
        break;
      }else if(tree->config->limittester(iter, limit)){
        // We have a child which is present and overlaps with the limit if
        // available.
        break;
      }
    }
    iter->childindx++;
  }
  
  if(iter->childindx < tree->config->childpernode){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(naIsNodeChildLeaf(iter->node, iter->childindx)){
      // Good ending. We found the next leaf.
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetTreeIteratorCurNode(iter, iter->node->childs[iter->childindx]);
      // We start looking in the subnode from the start.
      iter->childindx = -1;
      return naIterateTree(iter, limit);
    }
    
  }else{
    // There is no more segment available in this node. Go upwards.
    if(iter->node->parent){
      #ifndef NDEBUG
        if(iter->node->indxinparent == -1)
          naError("naIterateTreeNode", "Inernal inconsistency detected: Segment in parent should not be -1");
      #endif
      // There is a parent, set the iterator to that node and iterate anew.
      iter->childindx = iter->node->indxinparent;
      naSetTreeIteratorCurNode(iter, iter->node->parent);
      return naIterateTree(iter, limit);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naResetTreeIterator(iter);
      return NA_FALSE;
    }
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
