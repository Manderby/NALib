
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


#define NA_TREE_ITERATOR_MODIFIER 0x01


NA_DEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataConst(tree);
  iter.basenode = NA_NULL;
  iter.childindx = -1;
  naNulln(&(iter.key), sizeof(iter.key));
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_DEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.basenode = NA_NULL;
  iter.childindx = -1;
  naNulln(&(iter.key), sizeof(iter.key));
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_DEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.basenode = NA_NULL;
  iter.childindx = -1;
  naNulln(&(iter.key), sizeof(iter.key));
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



NA_DEF void naResetTreeIterator(NATreeIterator* iter){
  naSetTreeIteratorCurNode(iter, NA_NULL);
  iter->childindx = -1;
  naNulln(&(iter->key), sizeof(iter->key));
}



NA_DEF NABool naLocateTree(NATreeIterator* iter, const void* key){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  tree->config->iteratorKeySetter(iter, key);
  tree->config->treeBubbleLocator(iter);
  return tree->config->treeCaptureLocator(iter);
}



NA_DEF const void* naGetTreeConst(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!iter->basenode)
      naError("naGetTreeConst", "Iterator is at initial state. Dangerous garbage memory read incoming...");
    if(!iter->basenode->parent)
      naError("naGetTreeConst", "Iterator is at the root element, which can not be a leaf. Dangerous garbage memory read incoming...");
    if(naGetNodeChildType((NATreeNode*)(iter->basenode->parent), tree->config->nodeIndexInParentGetter(iter->basenode)) != NA_TREE_NODE_CHILD_LEAF)
      naError("naGetTreeConst", "Iterator is not at a leaf. Dangerous garbage memory read incoming...");
  #endif
  return naGetPtrConst(tree->config->leafDataGetter((NATreeLeaf*)iter->basenode));
}


//NA_DEF NABool naIterateTree(NATreeIterator* iter, const void* limit){
//  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
//  
//  // If the tree has no root, we can not iterate.
//  if(!tree->root){
//    #ifndef NDEBUG
//      if(iter->node)
//      naCrash("naIterateTree", "Current iterator node is set although no root available");
//    #endif
//    return NA_FALSE;
//  }
//  
//  // If the iterator has no current node being visited, we use the root. 
//  if(!iter->node){
//    naSetTreeIteratorCurNode(iter, tree->root);
//    #ifndef NDEBUG
//      if(!iter->node)
//      naCrash("naIterateTree", "No current node after setting the current node to the root");
//    #endif
//  }
//
//  // We go to the next child index. If we came to this function with index -1,
//  // we therefore end up with index 0.
//  iter->childindx++;
//    
//  // Search for a child index which is available and within limit.
//  while(iter->childindx < tree->config->childpernode){
//    if(naHasNodeChild(iter->node, iter->childindx)){
//      if(!limit){
//        // If there is no limit, we found a valid child.
//        break;
//      }else if(tree->config->limittester(iter, limit)){
//        // We have a child which is present and overlaps with the limit if
//        // available.
//        break;
//      }
//    }
//    // No valid child here. Go to the next one.
//    iter->childindx++;
//  }
//  
//  if(iter->childindx < tree->config->childpernode){
//    // There is a child available, either use the given leaf or go
//    // downwards if it is an inner node.
//    if(naGetNodeChildType(iter->node, iter->childindx)){
//      // Good ending. We found the next leaf.
//      return NA_TRUE;
//    }else{
//      // This is an inner node. We move downwards.
//      naSetTreeIteratorCurNode(iter, iter->node->childs[iter->childindx]);
//      // We start looking in the subnode from the start.
//      iter->childindx = -1;
//      return naIterateTree(iter, limit);
//    }
//    
//  }else{
//    // There is no more child available in this node. Go upwards.
//    if(iter->node->parent){
//      #ifndef NDEBUG
//        if(naGetNodeIndexInParent(iter->node) == NA_TREE_NODE_INDEX_IN_PARENT_MASK)
//          naError("naIterateTreeNode", "Inernal inconsistency detected: indx in parent should not be -1");
//      #endif
//      // There is a parent, set the iterator to that node and iterate anew.
//      iter->childindx = naGetNodeIndexInParent(iter->node);
//      naSetTreeIteratorCurNode(iter, iter->node->parent);
//      return naIterateTree(iter, limit);
//    }else{
//      // Bad ending. There is no parent node. This is the root and there are
//      // no more elements to be iterated.
//      naResetTreeIterator(iter);
//      return NA_FALSE;
//    }
//  }
//}



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
