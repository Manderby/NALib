
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


NA_HIAPI NABool naIterateTreeChilds(NATreeIterator* iter, NATreeNode* curnode, NAInt previndx, NABool* finished);
NA_HAPI  NABool naIterateTreeCapture(NATreeIterator* iter, NATreeNode* curnode);
NA_HAPI  NABool naIterateTreeBubble(NATreeIterator* iter, NATreeBaseNode* curnode);



NA_HIDEF NABool naIterateTreeChilds(NATreeIterator* iter, NATreeNode* curnode, NAInt previndx, NABool* finished){
  *finished = NA_TRUE;
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NAInt indx = previndx + 1;
  while(indx < tree->config->childpernode){
    NANodeChildType childtype = naGetNodeChildType(curnode, indx);
    if(naIsNodeChildTypeValid(childtype)){
      // There is another child available
      NATreeBaseNode* child = tree->config->nodechildgetter(curnode, indx);
      if(childtype == NA_TREE_NODE_CHILD_LEAF){
        // We found the next child.
        naSetTreeIteratorCurNode(iter, child);
        return NA_TRUE;
      }else{
        // We have to go deeper.
        return naIterateTreeCapture(iter, (NATreeNode*)child);
      }
    }
    indx++;
  }
  *finished = NA_FALSE;
  return NA_FALSE;
}



NA_HDEF NABool naIterateTreeCapture(NATreeIterator* iter, NATreeNode* curnode){
  // When capturing during in-order-iteration, we simply go down the tree and
  // search for the "first"-most leaf.
  NABool finished;
  NABool result = naIterateTreeChilds(iter, curnode, -1, &finished);
  if(finished){
    return result;
  }else{
    // No more childs available. Iteration is over.
    naSetTreeIteratorCurNode(iter, NA_NULL);
    return NA_FALSE;
  }
}



NA_HDEF NABool naIterateTreeBubble(NATreeIterator* iter, NATreeBaseNode* curnode){
  if(!curnode->parent){
    // We reached the root with no further options. Iteration is over.
    naSetTreeIteratorCurNode(iter, NA_NULL);
    return NA_FALSE;
  }
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NAInt indx = tree->config->nodechildindexgetter(curnode->parent, curnode);

  NABool finished;
  NABool result = naIterateTreeChilds(iter, curnode->parent, indx, &finished);
  if(finished){
    return result;
  }else{
    // If non more childs are available, bubble further.
    return naIterateTreeBubble(iter, &(curnode->parent->basenode));
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
  
  if(!iter->basenode){
    // If the iterator is at initial position, we use the root and capture. 
    return naIterateTreeCapture(iter, tree->root);
  }else{
    // Otherwise, we use the current leaf and bubble
    return naIterateTreeBubble(iter, iter->basenode);
  }
  
  

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
//      // There is a parent, set the iterator to that node and iterate anew.
//      iter->childindx = naGetNodeIndexInParent(iter->node);
//      naSetTreeIteratorCurNode(iter, iter->node->parent);
//      return naIterateTree(iter, limit);
//    }else{
//      // Bad ending. There is no parent node. This is the root and there are
//      // no more elements to be iterated.
//      naSetTreeIteratorCurNode(iter, NA_NULL);
//      return NA_FALSE;
//    }
//  }
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
