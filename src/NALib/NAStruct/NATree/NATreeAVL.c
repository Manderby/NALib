
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeBin.h"



NA_HDEF void naInitNodeAVL(NATreeBinNode* binnode){
  naGetBinNodeNode(binnode)->flags |= NA_TREE_NODE_AVL_EQUAL;
}



NA_HIDEF NAInt naGetNodeAVL(NATreeBinNode* binnode){
  return (NAInt)((naGetBinNodeNode(binnode)->flags & NA_TREE_NODE_AVL_MASK) >> NA_TREE_NODE_AVL_BITSHIFT) - 1;
}



NA_HIDEF void naSetNodeAVL(NATreeBinNode* binnode, NAInt balance){
  naSetFlagi(&(naGetBinNodeNode(binnode)->flags), NA_TREE_NODE_AVL_MASK, NA_FALSE);
  naGetBinNodeNode(binnode)->flags |= (balance + 1) << NA_TREE_NODE_AVL_BITSHIFT;
}



NA_HIDEF void naRotateLeftBin(NATree* tree, NATreeBinNode* parent, NATreeBinNode* rightchild){
  NATreeNode* grandparent;
  #ifndef NDEBUG
    if(naIsTreeItemLeaf(tree, naGetBinNodeItem(parent)))
      naError("given parent is not a node");
    if(naIsTreeItemLeaf(tree, naGetBinNodeItem(rightchild)))
      naError("given right child is not a node");
  #endif
  grandparent = naGetTreeItemParent(naGetBinNodeItem(parent));
  if(!naIsTreeItemRoot(naGetBinNodeItem(parent))){
    NAInt parentindx = naGetTreeNodeChildIndex(tree->config, grandparent, naGetBinNodeItem(parent));
    ((NATreeBinNode*)grandparent)->childs[parentindx] = naGetBinNodeItem(rightchild);
  }else{
    tree->root = naGetBinNodeItem(rightchild);
  }

  parent->childs[1] = rightchild->childs[0];
  parent->childs[1]->parent = naGetBinNodeNode(parent);
  naMarkNodeChildLeaf(naGetBinNodeNode(parent), 1, naIsNodeChildLeaf(naGetBinNodeNode(rightchild), 0));
  naSetTreeItemParent(naGetBinNodeItem(parent), naGetBinNodeNode(rightchild));

  rightchild->childs[0] = naGetBinNodeItem(parent);
  naMarkNodeChildLeaf(naGetBinNodeNode(rightchild), 0, NA_FALSE);
  naSetTreeItemParent(naGetBinNodeItem(rightchild), grandparent);

  naUpdateTreeNodeBubbling(tree, naGetBinNodeNode(parent), -1);
}



NA_HIDEF void naRotateRightBin(NATree* tree, NATreeBinNode* leftchild, NATreeBinNode* parent){
  NATreeNode* grandparent;
  #ifndef NDEBUG
    if(naIsTreeItemLeaf(tree, naGetBinNodeItem(leftchild)))
      naError("given left child is not a node");
    if(naIsTreeItemLeaf(tree, naGetBinNodeItem(parent)))
      naError("given parent is not a node");
  #endif
  grandparent = naGetTreeItemParent(naGetBinNodeItem(parent));
  if(!naIsTreeItemRoot(naGetBinNodeItem(parent))){
    NAInt parentindx = naGetTreeNodeChildIndex(tree->config, grandparent, naGetBinNodeItem(parent));
    ((NATreeBinNode*)grandparent)->childs[parentindx] = naGetBinNodeItem(leftchild);
  }else{
    tree->root = naGetBinNodeItem(leftchild);
  }

  parent->childs[0] = leftchild->childs[1];
  parent->childs[0]->parent = naGetBinNodeNode(parent);
  naMarkNodeChildLeaf(naGetBinNodeNode(parent), 0, naIsNodeChildLeaf(naGetBinNodeNode(leftchild), 1));
  naSetTreeItemParent(naGetBinNodeItem(parent), naGetBinNodeNode(leftchild));

  leftchild->childs[1] = naGetBinNodeItem(parent);
  naMarkNodeChildLeaf(naGetBinNodeNode(leftchild), 1, NA_FALSE);
  naSetTreeItemParent(naGetBinNodeItem(leftchild), grandparent);

  naUpdateTreeNodeBubbling(tree, naGetBinNodeNode(parent), -1);
}



NA_HIDEF void naPropagateAVLGrow(NATree* tree, NATreeBinNode* binnode){
  if(!naIsTreeItemRoot(naGetBinNodeItem(binnode))){
    NATreeNode* parent = naGetTreeItemParent(naGetBinNodeItem(binnode));
    naGrowAVL(tree, (NATreeBinNode*)parent, naGetTreeNodeChildIndex(tree->config, parent, naGetBinNodeItem(binnode)));
  }
}



NA_HIDEF void naPropagateAVLShrink(NATree* tree, NATreeBinNode* binnode){
  if(!naIsTreeItemRoot(naGetBinNodeItem(binnode))){
    NATreeNode* parent = naGetTreeItemParent(naGetBinNodeItem(binnode));
    naShrinkAVL(tree, (NATreeBinNode*)parent, naGetTreeNodeChildIndex(tree->config, parent, naGetBinNodeItem(binnode)));
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
      naRotateRightBin(tree, child, binnode);
      naSetNodeAVL(child, 0);
      naSetNodeAVL(binnode, 0);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("child balance invalid with node balance -2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[1];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateLeftBin(tree, child, grandchild);
      naRotateRightBin(tree, grandchild, binnode);
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
            naError("grandchild balance invalid with node balance -2");
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
      naRotateLeftBin(tree, binnode, child);
      naSetNodeAVL(binnode, 0);
      naSetNodeAVL(child, 0);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("child balance invalid with node balance 2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[0];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateRightBin(tree, grandchild, child);
      naRotateLeftBin(tree, binnode, grandchild);
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
            naError("grandchild balance invalid with node balance 2");
        #endif
        naSetNodeAVL(binnode, 0);
        naSetNodeAVL(grandchild, 0);
        naSetNodeAVL(child, 1);
      }
    }
  }else{
    #ifndef NDEBUG
      naError("AVL balance out of balance");
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
      naRotateLeftBin(tree, binnode, child);
      naSetNodeAVL(binnode, 1);
      naSetNodeAVL(child, -1);
    }else if(childbalance == 1){
      naRotateLeftBin(tree, binnode, child);
      naSetNodeAVL(binnode, 0);
      naSetNodeAVL(child, 0);
      naPropagateAVLShrink(tree, child);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("child balance invalid with node balance 2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[0];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateRightBin(tree, grandchild, child);
      naRotateLeftBin(tree, binnode, grandchild);
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
            naError("grandchild balance invalid with node balance 2");
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
      naRotateRightBin(tree, child, binnode);
      naSetNodeAVL(child, 1);
      naSetNodeAVL(binnode, -1);
    }else if(childbalance == -1){
      naRotateRightBin(tree, child, binnode);
      naSetNodeAVL(child, 0);
      naSetNodeAVL(binnode, 0);
      naPropagateAVLShrink(tree, child);
    }else{
      NATreeBinNode* grandchild;
      NAInt grandchildbalance;
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("child balance invalid with node balance -2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[1];
      grandchildbalance = naGetNodeAVL(grandchild);
      naRotateLeftBin(tree, child, grandchild);
      naRotateRightBin(tree, grandchild, binnode);
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
            naError("grandchild balance invalid with node balance -2");
        #endif
        naSetNodeAVL(child, 0);
        naSetNodeAVL(binnode, 1);
      }
      naPropagateAVLShrink(tree, grandchild);
    }
  }else{
    #ifndef NDEBUG
      naError("AVL balance out of balance");
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
