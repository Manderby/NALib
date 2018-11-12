
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


typedef struct NATreeIterationInfo NATreeIterationInfo;
struct NATreeIterationInfo{
  NAInt step;
  NAInt startindx;
  NAInt breakindx;
};

// Prototypes
NA_HIAPI NABool naIterateTreeChilds(NATreeIterator* iter, NATreeNode* curnode, NAInt previndx, NABool* finished, const NATreeIterationInfo* info);
NA_HAPI  NABool naIterateTreeCapture(NATreeIterator* iter, NATreeNode* curnode, const NATreeIterationInfo* info);
NA_HAPI  NABool naIterateTreeBubble(NATreeIterator* iter, NATreeBaseNode* curnode, const NATreeIterationInfo* info);

NA_HIDEF NABool naIterateTreeChilds(NATreeIterator* iter, NATreeNode* parent, NAInt previndx, NABool* finished, const NATreeIterationInfo* info){
  *finished = NA_TRUE;
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NAInt indx = previndx + info->step;
  while(indx != info->breakindx){
    NANodeChildType childtype = naGetNodeChildType(parent, indx);
    if(naIsNodeChildTypeValid(childtype)){
      // There is another child available
      NATreeBaseNode* child = tree->config->nodechildgetter(parent, indx);
      if(childtype == NA_TREE_NODE_CHILD_LEAF){
        // We found the next leaf. Good ending
        naSetTreeIteratorCurNode(iter, child);
        return NA_TRUE;
      }else{
        // We have to go deeper.
        return naIterateTreeCapture(iter, (NATreeNode*)child, info);
      }
    }
    indx += info->step;
  }
  *finished = NA_FALSE;
  return NA_FALSE;  // This return value is obsolete and must be ignored in the caller.
}



NA_HDEF NABool naIterateTreeCapture(NATreeIterator* iter, NATreeNode* curnode, const NATreeIterationInfo* info){
  // When capturing during in-order-iteration, we simply go down the tree and
  // search for the "first"-most leaf.
  NABool finished;
  NABool result = naIterateTreeChilds(iter, curnode, info->startindx, &finished, info);
  if(finished){
    return result;
  }else{
    // No more childs available. Iteration is over.
    naSetTreeIteratorCurNode(iter, NA_NULL);
    return NA_FALSE;
  }
}



NA_HDEF NABool naIterateTreeBubble(NATreeIterator* iter, NATreeBaseNode* curnode, const NATreeIterationInfo* info){
  if(!curnode->parent){
    // We reached the root with no further options. Iteration is over.
    naSetTreeIteratorCurNode(iter, NA_NULL);
    return NA_FALSE;
  }
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NAInt indx = tree->config->nodechildindexgetter(curnode->parent, curnode);

  NABool finished;
  NABool result = naIterateTreeChilds(iter, curnode->parent, indx, &finished, info);
  if(finished){
    return result;
  }else{
    // If non more childs are available, bubble further.
    return naIterateTreeBubble(iter, &(curnode->parent->basenode), info);
  }
}



NA_DEF NABool naIterateTree(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  
  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(iter->basenode)
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }
  
  NATreeIterationInfo info = {1, -1, tree->config->childpernode};
  
  if(!iter->basenode){
    // If the iterator is at initial position, we use the root and capture. 
    return naIterateTreeCapture(iter, tree->root, &info);
  }else{
    // Otherwise, we use the current leaf and bubble
    return naIterateTreeBubble(iter, iter->basenode, &info);
  }
}



NA_DEF NABool naIterateTreeBack(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  
  NATreeIterationInfo info = {-1, tree->config->childpernode, -1};

  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(iter->basenode)
      naCrash("naIterateTreeBack", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }
  
  if(!iter->basenode){
    // If the iterator is at initial position, we use the root and capture. 
    return naIterateTreeCapture(iter, tree->root, &info);
  }else{
    // Otherwise, we use the current leaf and bubble
    return naIterateTreeBubble(iter, iter->basenode, &info);
  }
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
