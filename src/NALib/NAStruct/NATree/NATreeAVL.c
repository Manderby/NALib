
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeBin.h"



NA_HDEF void naInitNodeAVL(NATreeBinNode* binnode){
  ((NATreeNode*)binnode)->flags |= NA_TREE_NODE_AVL_EQUAL;
}



NA_HIDEF NAInt naGetNodeAVL(NATreeBinNode* binnode){
  return (NAInt)((((NATreeNode*)binnode)->flags & NA_TREE_NODE_AVL_MASK) >> NA_TREE_NODE_AVL_BITSHIFT) - 1;
}



NA_HIDEF void naSetNodeAVL(NATreeBinNode* binnode, NAInt balance){
  naSetFlagi(&(((NATreeNode*)binnode)->flags), NA_TREE_NODE_AVL_MASK, NA_FALSE);
  ((NATreeNode*)binnode)->flags |= (balance + 1) << NA_TREE_NODE_AVL_BITSHIFT;
}



NA_HIDEF void naRotateLeftBinary(NATree* tree, NATreeBinNode* parent, NATreeBinNode* rightchild){
  NATreeNode* grandparent;
  #ifndef NDEBUG
    if(naIsItemLeaf(tree, &(parent->node.item)))
      naError("naRotateLeftBinary", "given parent is not a node");
    if(naIsItemLeaf(tree, &(rightchild->node.item)))
      naError("naRotateLeftBinary", "given right child is not a node");
  #endif
  grandparent = parent->node.item.parent;
  if(!naIsTreeItemRoot(tree, &(parent->node.item))){
    NAInt parentindx = naGetChildIndexBinary(grandparent, &(parent->node.item));
    ((NATreeBinNode*)grandparent)->childs[parentindx] = &(rightchild->node.item);
  }else{
    tree->root = &(rightchild->node.item);
  }

  parent->childs[1] = rightchild->childs[0];
  parent->childs[1]->parent = (NATreeNode*)parent;
  naMarkNodeChildLeaf((NATreeNode*)parent, 1, naIsNodeChildLeaf((NATreeNode*)rightchild, 0));
  parent->node.item.parent = (NATreeNode*)rightchild;

  rightchild->childs[0] = &(parent->node.item);
  naMarkNodeChildLeaf((NATreeNode*)rightchild, 0, NA_FALSE);
  rightchild->node.item.parent = grandparent;

  naUpdateTreeNodeBubbling(tree, (NATreeNode*)parent, -1);
}



NA_HIDEF void naRotateRightBinary(NATree* tree, NATreeBinNode* leftchild, NATreeBinNode* parent){
  NATreeNode* grandparent;
  #ifndef NDEBUG
    if(naIsItemLeaf(tree, &(leftchild->node.item)))
      naError("naRotateRightBinary", "given left child is not a node");
    if(naIsItemLeaf(tree, &(parent->node.item)))
      naError("naRotateRightBinary", "given parent is not a node");
  #endif
  grandparent = parent->node.item.parent;
  if(!naIsTreeItemRoot(tree, &(parent->node.item))){
    NAInt parentindx = naGetChildIndexBinary(grandparent, &(parent->node.item));
    ((NATreeBinNode*)grandparent)->childs[parentindx] = &(leftchild->node.item);
  }else{
    tree->root = &(leftchild->node.item);
  }

  parent->childs[0] = leftchild->childs[1];
  parent->childs[0]->parent = (NATreeNode*)parent;
  naMarkNodeChildLeaf((NATreeNode*)parent, 0, naIsNodeChildLeaf((NATreeNode*)leftchild, 1));
  parent->node.item.parent = (NATreeNode*)leftchild;

  leftchild->childs[1] = &(parent->node.item);
  naMarkNodeChildLeaf((NATreeNode*)leftchild, 1, NA_FALSE);
  leftchild->node.item.parent = grandparent;

  naUpdateTreeNodeBubbling(tree, (NATreeNode*)parent, -1);
}



NA_HIDEF void naPropagateAVLGrow(NATree* tree, NATreeBinNode* binnode){
  if(!naIsTreeItemRoot(tree, &(binnode->node.item))){
    naGrowAVL(tree, (NATreeBinNode*)(binnode->node.item.parent), naGetChildIndexBinary(binnode->node.item.parent, &(binnode->node.item)));
  }
}



NA_HIDEF void naPropagateAVLShrink(NATree* tree, NATreeBinNode* binnode){
  if(!naIsTreeItemRoot(tree, &(binnode->node.item))){
    naShrinkAVL(tree, (NATreeBinNode*)(binnode->node.item.parent), naGetChildIndexBinary(binnode->node.item.parent, &(binnode->node.item)));
  }
}



NA_HDEF void naGrowAVL(NATree* tree, NATreeBinNode* binnode, NAInt childindx){
  NAInt nodebalance = naGetNodeAVL(binnode);
  nodebalance += (childindx * 2) - 1;
  if(nodebalance == 0){
    naSetNodeAVL(binnode, nodebalance);
  }else if(nodebalance == 1 || nodebalance == -1){
    naSetNodeAVL(binnode, nodebalance);
    naPropagateAVLGrow(tree, binnode);
  }else if(nodebalance == -2){
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[0];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == -1){
      naRotateRightBinary(tree, child, binnode);
      naSetNodeAVL(child, 0);
      naSetNodeAVL(binnode, 0);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("naGrowAVL", "child balance invalid with node balance -2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[1];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateLeftBinary(tree, child, grandchild);
      naRotateRightBinary(tree, grandchild, binnode);
      if(grandchildbalance == -1){
        naSetNodeAVL(binnode, 1);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, 0);
      }else if(grandchildbalance == 0){ // Note that in classical AVL, there is
        naSetNodeAVL(binnode, 0);          // no case 0 but it has been added here
        naSetNodeAVL(grandchild, 0);    // because of the split operation which
        naSetNodeAVL(child, 0);         // adds a balanced node to a subtree.
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != 1)
            naError("naGrowAVL", "grandchild balance invalid with node balance -2");
        #endif
        naSetNodeAVL(binnode, 0);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, -1);
      }
    }
  }else if(nodebalance == 2){
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[1];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 1){
      naRotateLeftBinary(tree, binnode, child);
      naSetNodeAVL(binnode, 0);
      naSetNodeAVL(child, 0);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("naGrowAVL", "child balance invalid with node balance 2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[0];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateRightBinary(tree, grandchild, child);
      naRotateLeftBinary(tree, binnode, grandchild);
      if(grandchildbalance == 1){
        naSetNodeAVL(binnode, -1);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, 0);
      }else if(grandchildbalance == 0){ // Note that in classical AVL, there is
        naSetNodeAVL(binnode, 0);          // no case 0 but it has been added here
        naSetNodeAVL(grandchild, 0);    // because of the split operation which
        naSetNodeAVL(child, 0);         // adds a balanced node to a subtree.
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != -1)
            naError("naGrowAVL", "grandchild balance invalid with node balance 2");
        #endif
        naSetNodeAVL(binnode, 0);
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



NA_HDEF void naShrinkAVL(NATree* tree, NATreeBinNode* binnode, NAInt childindx){
  NAInt nodebalance = naGetNodeAVL(binnode);
  nodebalance -= (childindx * 2) - 1;
  if(nodebalance == 0){
    naSetNodeAVL(binnode, nodebalance);
    naPropagateAVLShrink(tree, binnode);
  }else if(nodebalance == 1 || nodebalance == -1){
    naSetNodeAVL(binnode, nodebalance);
  }else if(nodebalance == 2){
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[1];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 0){
      naRotateLeftBinary(tree, binnode, child);
      naSetNodeAVL(binnode, 1);
      naSetNodeAVL(child, -1);
    }else if(childbalance == 1){
      naRotateLeftBinary(tree, binnode, child);
      naSetNodeAVL(binnode, 0);
      naSetNodeAVL(child, 0);
      naPropagateAVLShrink(tree, child);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("naShrinkAVL", "child balance invalid with node balance 2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[0];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateRightBinary(tree, grandchild, child);
      naRotateLeftBinary(tree, binnode, grandchild);
      naSetNodeAVL(grandchild, 0);
      if(grandchildbalance == -1){
        naSetNodeAVL(binnode, 0);
        naSetNodeAVL(child, 1);
      }else if(grandchildbalance == 0){
        naSetNodeAVL(binnode, 0);
        naSetNodeAVL(child, 0);
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != 1)
            naError("naShrinkAVL", "grandchild balance invalid with node balance 2");
        #endif
        naSetNodeAVL(binnode, -1);
        naSetNodeAVL(child, 0);
      }
      naPropagateAVLShrink(tree, grandchild);
    }
  }else if(nodebalance == -2){
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[0];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 0){
      naRotateRightBinary(tree, child, binnode);
      naSetNodeAVL(child, 1);
      naSetNodeAVL(binnode, -1);
    }else if(childbalance == -1){
      naRotateRightBinary(tree, child, binnode);
      naSetNodeAVL(child, 0);
      naSetNodeAVL(binnode, 0);
      naPropagateAVLShrink(tree, child);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("naShrinkAVL", "child balance invalid with node balance -2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[1];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateLeftBinary(tree, child, grandchild);
      naRotateRightBinary(tree, grandchild, binnode);
      naSetNodeAVL(grandchild, 0);
      if(grandchildbalance == 1){
        naSetNodeAVL(child, -1);
        naSetNodeAVL(binnode, 0);
      }else if(grandchildbalance == 0){
        naSetNodeAVL(child, 0);
        naSetNodeAVL(binnode, 0);
      }else{
        #ifndef NDEBUG
          if(grandchildbalance != -1)
            naError("naShrinkAVL", "grandchild balance invalid with node balance -2");
        #endif
        naSetNodeAVL(child, 0);
        naSetNodeAVL(binnode, 1);
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
