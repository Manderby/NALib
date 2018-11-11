
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"

//NA_RUNTIME_TYPE(NATreeNode, NA_NULL, NA_FALSE);



NA_HDEF void naInitTreeNode(NATreeNode* node, NATreeNode* parent){
  node->flags = 0;
  node->basenode.parent = parent;
    
  #ifndef NDEBUG
    node->basenode.itercount = 0;
  #endif
}



NA_HDEF void naClearTreeNode(NATreeNode* node){
  #ifndef NDEBUG
    if(node->basenode.itercount)
      naError("naClearTreeNode", "There are still iterators running on this node. Did you forget a call to naClearTreeIterator?");
  #endif  
}



NA_HDEF void naInitTreeLeaf(NATreeLeaf* leaf, NATreeNode* parent){
  leaf->basenode.parent = parent;
    
  #ifndef NDEBUG
    leaf->basenode.itercount = 0;
  #endif
}



NA_HDEF void naClearTreeLeaf(NATreeLeaf* leaf){
  #ifndef NDEBUG
    if(leaf->basenode.itercount)
      naError("naClearTreeLeaf", "There are still iterators running on this leaf. Did you forget a call to naClearTreeIterator?");
  #endif  
}




// Phase 1: Rise towards the root
//NA_HDEF NABool naLocateTreeNodeBubble(NATreeIterator* iter){
//}
//
//
//
//// Phase 2: Descent towards the leafes
//NA_HDEF NABool naLocateTreeNodeCapture(NATreeIterator* iter){
////  NABool retvalue = NA_FALSE;
////  #ifndef NDEBUG
////    if(!iter->node)
////      naError("naLocateTreeNode", "Iterator must be at an inner node");
////  #endif
////  if(key < iter->node->key){
////    // We look in the left subtree
////    if(naHasNodeChild(iter->node, 0)){
////      if(naGetNodeChildType(iter->node, 0)){
////        iter->childindx = 0;
////        retvalue = NA_TRUE;
////      }else{
////        naSetTreeIteratorCurNode(iter, iter->node->childs[0]);
////        retvalue = naLocateTreeNode(iter, key);
////      }
////    }
////  }else{
////    // we look in the right subtree
////    if(naHasNodeChild(iter->node, 1)){
////      if(naGetNodeChildType(iter->node, 1)){
////        iter->childindx = 1;
////        retvalue = NA_TRUE;
////      }else{
////        naSetTreeIteratorCurNode(iter, iter->node->childs[1]);
////        retvalue = naLocateTreeNode(iter, key);
////      }
////    }
////  }
////  return retvalue;
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
