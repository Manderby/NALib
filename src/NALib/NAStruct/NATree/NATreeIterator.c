
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


// Prototypes
NA_HIAPI NATreeLeaf* naIterateTreeCapture(const NATree* tree, NATreeNode* curnode, NAInt previndx, NATreeIterationInfo* info);
NA_HAPI  NATreeLeaf* naIterateTreeBubble(const NATree* tree, NATreeBaseNode* curnode, NATreeIterationInfo* info);



// This function expects a parent node and searches for the next available
// child node after the given previndx. Works recursively until either no
// valid child is found or a leaf is returned
NA_HIDEF NATreeLeaf* naIterateTreeCapture(const NATree* tree, NATreeNode* parent, NAInt previndx, NATreeIterationInfo* info){
  NAInt indx = previndx + info->step;
  while(indx != info->breakindx){
    NANodeChildType childtype = naGetNodeChildType(parent, indx);
    if(naIsNodeChildTypeValid(childtype)){
      // There is another child available
      NATreeBaseNode* child = tree->config->nodechildgetter(parent, indx);
      if(childtype == NA_TREE_NODE_CHILD_LEAF){
        // We found the next leaf. Good ending
        return (NATreeLeaf*)child;
      }else{
        // We have to go deeper.
        return naIterateTreeCapture(tree, (NATreeNode*)child, info->startindx, info);
      }
    }
    indx += info->step;
  }
  return NA_NULL;
}



// This function takes a given basenode and bubbles up to its parent. This
// function works recursively until either a parent offers a next leaf or
// there are no more parents.
NA_HDEF NATreeLeaf* naIterateTreeBubble(const NATree* tree, NATreeBaseNode* curnode, NATreeIterationInfo* info){
  if(!curnode->parent){
    // We reached the root with no further options. Iteration is over.
    return NA_NULL;
  }
  NAInt indx = tree->config->nodechildindexgetter(curnode->parent, curnode);

  NATreeLeaf* leaf = naIterateTreeCapture(tree, curnode->parent, indx, info);
  if(leaf){
    return leaf;
  }else{
    // If non more childs are available, bubble further.
    return naIterateTreeBubble(tree, &(curnode->parent->basenode), info);
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  
  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(iter->leaf)
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }
  
  NATreeLeaf* leaf;
  if(!iter->leaf){
    // If the iterator is at initial position, we use the root and capture. 
    leaf = naIterateTreeCapture(tree, tree->root, info->startindx, info);
  }else{
    // Otherwise, we use the current leaf and bubble
    leaf = naIterateTreeBubble(tree, &(iter->leaf->basenode), info);
  }
  naSetTreeIteratorCurLeaf(iter, leaf);
  return (leaf != NA_NULL);
}



NA_DEF NABool naAddTreeConst(NATreeIterator* iter, const void* key, const void* content, NABool replace){
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
