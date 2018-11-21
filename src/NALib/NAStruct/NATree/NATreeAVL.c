
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"
#include "NATreeBinary.h"



NA_HDEF void naInitNodeAVL(NATreeBinaryNode* node){
  ((NATreeNode*)node)->flags |= NA_TREE_NODE_AVL_EQUAL;
}



NA_HIDEF NAInt naGetNodeAVL(NATreeBinaryNode* node){
  return (NAInt)((((NATreeNode*)node)->flags & NA_TREE_NODE_AVL_MASK) >> NA_TREE_NODE_AVL_BITSHIFT) - 1;
}



NA_HIDEF void naSetNodeAVL(NATreeBinaryNode* node, NAInt balance){
  naSetFlagi(&(((NATreeNode*)node)->flags), NA_TREE_NODE_AVL_MASK, NA_FALSE);
  ((NATreeNode*)node)->flags |= (balance + 1) << NA_TREE_NODE_AVL_BITSHIFT;
}



NA_HIDEF void naRotateLeftBinary(NATree* tree, NATreeBinaryNode* parent, NATreeBinaryNode* rightchild){
  #ifndef NDEBUG
    if(naIsBaseNodeLeaf(tree, (NATreeBaseNode*)parent))
      naError("naRotateLeftBinary", "given parent is not a node");
    if(naIsBaseNodeLeaf(tree, (NATreeBaseNode*)rightchild))
      naError("naRotateLeftBinary", "given right child is not a node");
  #endif
  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent); 
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = (NATreeBaseNode*)rightchild;
  }else{
    tree->root = (NATreeBaseNode*)rightchild;
  }

  parent->childs[1] = rightchild->childs[0];
  parent->childs[1]->parent = (NATreeNode*)parent;
  naMarkNodeChildLeaf((NATreeNode*)parent, 1, naIsNodeChildLeaf((NATreeNode*)rightchild, 0));
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)rightchild;
  
  rightchild->childs[0] = (NATreeBaseNode*)parent;
  naMarkNodeChildLeaf((NATreeNode*)rightchild, 0, NA_FALSE);
  ((NATreeBaseNode*)rightchild)->parent = grandparent;
}



NA_HIDEF void naRotateRightBinary(NATree* tree, NATreeBinaryNode* leftchild, NATreeBinaryNode* parent){
  #ifndef NDEBUG
    if(naIsBaseNodeLeaf(tree, (NATreeBaseNode*)leftchild))
      naError("naRotateRightBinary", "given left child is not a node");
    if(naIsBaseNodeLeaf(tree, (NATreeBaseNode*)parent))
      naError("naRotateRightBinary", "given parent is not a node");
  #endif
  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent); 
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = (NATreeBaseNode*)leftchild;
  }else{
    tree->root = (NATreeBaseNode*)leftchild;
  }

  parent->childs[0] = leftchild->childs[1];
  parent->childs[0]->parent = (NATreeNode*)parent;
  naMarkNodeChildLeaf((NATreeNode*)parent, 0, naIsNodeChildLeaf((NATreeNode*)leftchild, 1));
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)leftchild;
  
  leftchild->childs[1] = (NATreeBaseNode*)parent;
  naMarkNodeChildLeaf((NATreeNode*)leftchild, 1, NA_FALSE);
  ((NATreeBaseNode*)leftchild)->parent = grandparent;
}



NA_HIDEF void naPropagateAVLGrow(NATree* tree, NATreeBinaryNode* node){
  if(((NATreeBaseNode*)node)->parent){
    naGrowAVL(tree, (NATreeBinaryNode*)((NATreeBaseNode*)node)->parent, naGetChildIndexBinary(((NATreeBaseNode*)node)->parent, (NATreeBaseNode*)node));
  }
}



NA_HIDEF void naPropagateAVLShrink(NATree* tree, NATreeBinaryNode* node){
  if(((NATreeBaseNode*)node)->parent){
    naShrinkAVL(tree, (NATreeBinaryNode*)((NATreeBaseNode*)node)->parent, naGetChildIndexBinary(((NATreeBaseNode*)node)->parent, (NATreeBaseNode*)node));
  }
}



NA_HDEF void naGrowAVL(NATree* tree, NATreeBinaryNode* node, NAInt childindx){
  NAInt nodebalance = naGetNodeAVL(node);
  nodebalance += (childindx * 2) - 1;
  if(nodebalance == 0){
    naSetNodeAVL(node, nodebalance);
  }else if(nodebalance == 1 || nodebalance == -1){
    naSetNodeAVL(node, nodebalance);
    naPropagateAVLGrow(tree, node);
  }else if(nodebalance == -2){
    NATreeBinaryNode* child = (NATreeBinaryNode*)node->childs[0];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == -1){
      naRotateRightBinary(tree, child, node);
      naSetNodeAVL(child, 0);
      naSetNodeAVL(node, 0);
    }else{
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("naGrowAVL", "child balance invalid with node balance -2");
      #endif
      NATreeBinaryNode* grandchild = (NATreeBinaryNode*)child->childs[1];
      NAInt grandchildbalance = naGetNodeAVL(grandchild);
      naRotateLeftBinary(tree, child, grandchild);
      naRotateRightBinary(tree, grandchild, node);
      if(grandchildbalance == -1){
        naSetNodeAVL(node, 1);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, 0);
      }else if(grandchildbalance == 0){ // Note that in classical AVL, there is
        naSetNodeAVL(node, 0);          // no case 0 but it has been added here
        naSetNodeAVL(grandchild, 0);    // because of the split operation which
        naSetNodeAVL(child, 0);         // adds a balanced node to a subtree.
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != 1)
            naError("naGrowAVL", "grandchild balance invalid with node balance -2");
        #endif
        naSetNodeAVL(node, 0);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, -1);
      }
    }
  }else if(nodebalance == 2){
    NATreeBinaryNode* child = (NATreeBinaryNode*)node->childs[1];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 1){
      naRotateLeftBinary(tree, node, child);
      naSetNodeAVL(node, 0);
      naSetNodeAVL(child, 0);
    }else{
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("naGrowAVL", "child balance invalid with node balance 2");
      #endif
      NATreeBinaryNode* grandchild = (NATreeBinaryNode*)child->childs[0];
      NAInt grandchildbalance = naGetNodeAVL(grandchild);
      naRotateRightBinary(tree, grandchild, child);
      naRotateLeftBinary(tree, node, grandchild);
      if(grandchildbalance == 1){
        naSetNodeAVL(node, -1);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, 0);
      }else if(grandchildbalance == 0){ // Note that in classical AVL, there is
        naSetNodeAVL(node, 0);          // no case 0 but it has been added here
        naSetNodeAVL(grandchild, 0);    // because of the split operation which
        naSetNodeAVL(child, 0);         // adds a balanced node to a subtree.
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != -1)
            naError("naGrowAVL", "grandchild balance invalid with node balance 2");
        #endif
        naSetNodeAVL(node, 0);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, 1);
      }
    }
  }else{
    #ifndef NDEBUG
      naError("naGrowAVL", "AVL balance out of balance");
    #endif
  }
}



NA_HDEF void naShrinkAVL(NATree* tree, NATreeBinaryNode* node, NAInt childindx){
  NAInt nodebalance = naGetNodeAVL(node);
  nodebalance -= (childindx * 2) - 1;
  if(nodebalance == 0){
    naSetNodeAVL(node, nodebalance);
    naPropagateAVLShrink(tree, node);
  }else if(nodebalance == 1 || nodebalance == -1){
    naSetNodeAVL(node, nodebalance);
  }else if(nodebalance == 2){
    NATreeBinaryNode* child = (NATreeBinaryNode*)node->childs[1];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 0){
      naRotateLeftBinary(tree, node, child);
      naSetNodeAVL(node, 1);
      naSetNodeAVL(child, -1);
    }else if(childbalance == 1){
      naRotateLeftBinary(tree, node, child);
      naSetNodeAVL(node, 0);
      naSetNodeAVL(child, 0);
      naPropagateAVLShrink(tree, child);
    }else{
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("naShrinkAVL", "child balance invalid with node balance 2");
      #endif
      NATreeBinaryNode* grandchild = (NATreeBinaryNode*)child->childs[0];
      NAInt grandchildbalance = naGetNodeAVL(grandchild);
      naRotateRightBinary(tree, grandchild, child);
      naRotateLeftBinary(tree, node, grandchild);
      naSetNodeAVL(grandchild, 0);
      if(grandchildbalance == -1){
        naSetNodeAVL(node, 0);
        naSetNodeAVL(child, 1);
      }else if(grandchildbalance == 0){
        naSetNodeAVL(node, 0);
        naSetNodeAVL(child, 0);
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != 1)
            naError("naShrinkAVL", "grandchild balance invalid with node balance 2");
        #endif
        naSetNodeAVL(node, -1);
        naSetNodeAVL(child, 0);
      }
      naPropagateAVLShrink(tree, grandchild);
    }
  }else if(nodebalance == -2){
    NATreeBinaryNode* child = (NATreeBinaryNode*)node->childs[0];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 0){
      naRotateRightBinary(tree, child, node);
      naSetNodeAVL(child, 1);
      naSetNodeAVL(node, -1);
    }else if(childbalance == -1){
      naRotateRightBinary(tree, child, node);
      naSetNodeAVL(child, 0);
      naSetNodeAVL(node, 0);
      naPropagateAVLShrink(tree, child);
    }else{
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("naShrinkAVL", "child balance invalid with node balance -2");
      #endif
      NATreeBinaryNode* grandchild = (NATreeBinaryNode*)child->childs[1];
      NAInt grandchildbalance = naGetNodeAVL(grandchild);
      naRotateLeftBinary(tree, child, grandchild);
      naRotateRightBinary(tree, grandchild, node);
      naSetNodeAVL(grandchild, 0);
      if(grandchildbalance == 1){
        naSetNodeAVL(child, -1);
        naSetNodeAVL(node, 0);
      }else if(grandchildbalance == 0){
        naSetNodeAVL(child, 0);
        naSetNodeAVL(node, 0);
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != -1)
            naError("naShrinkAVL", "grandchild balance invalid with node balance -2");
        #endif
        naSetNodeAVL(child, 0);
        naSetNodeAVL(node, 1);
      }
      naPropagateAVLShrink(tree, grandchild);
    }
  }else{
    #ifndef NDEBUG
      naError("naShrinkAVL", "AVL balance out of balance");
    #endif
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
