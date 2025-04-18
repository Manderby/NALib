
#include "../../NATree.h"
#include "NATreeBin.h"



NA_HDEF void na_InitNodeAVL(NATreeBinNode* binnode) {
  na_GetBinNodeNode(binnode)->flags |= NA_TREE_NODE_AVL_EQUAL;
}



NA_HIDEF int64 na_GetNodeAVL(NATreeBinNode* binnode) {
  return (int64)((na_GetBinNodeNode(binnode)->flags & NA_TREE_NODE_AVL_MASK) >> NA_TREE_NODE_AVL_BITSHIFT) - 1;
}



NA_HIDEF void na_SetNodeAVL(NATreeBinNode* binnode, int64 balance) {
  naSetFlagu32(&na_GetBinNodeNode(binnode)->flags, NA_TREE_NODE_AVL_MASK, NA_FALSE);
  na_GetBinNodeNode(binnode)->flags |= (balance + 1) << NA_TREE_NODE_AVL_BITSHIFT;
}



NA_HIDEF void na_RotateLeftBin(NATree* tree, NATreeBinNode* parent, NATreeBinNode* rightchild) {
  NATreeNode* grandparent;
  #if NA_DEBUG
    if(na_IsTreeItemLeaf(tree, na_GetBinNodeItem(parent)))
      naError("given parent is not a node");
    if(na_IsTreeItemLeaf(tree, na_GetBinNodeItem(rightchild)))
      naError("given right child is not a node");
  #endif
  grandparent = na_GetTreeItemParent(na_GetBinNodeItem(parent));
  if(!na_GetTreeItemIsRoot(na_GetBinNodeItem(parent))) {
    size_t parentIndex = na_GetTreeNodeChildIndex(grandparent, na_GetBinNodeItem(parent), tree->config);
    ((NATreeBinNode*)grandparent)->childs[parentIndex] = na_GetBinNodeItem(rightchild);
  }else{
    tree->root = na_GetBinNodeItem(rightchild);
  }

  parent->childs[1] = rightchild->childs[0];
  parent->childs[1]->parent = na_GetBinNodeNode(parent);
  na_SetNodeChildIsLeaf(na_GetBinNodeNode(parent), 1, na_GetNodeChildIsLeaf(na_GetBinNodeNode(rightchild), 0, tree->config), tree->config);
  na_SetTreeItemParent(na_GetBinNodeItem(parent), na_GetBinNodeNode(rightchild));

  rightchild->childs[0] = na_GetBinNodeItem(parent);
  na_SetNodeChildIsLeaf(na_GetBinNodeNode(rightchild), 0, NA_FALSE, tree->config);
  na_SetTreeItemParent(na_GetBinNodeItem(rightchild), grandparent);

  na_UpdateTreeNodeBubbling(tree, na_GetBinNodeNode(parent), NA_TREE_UNSPECIFIED_INDEX);
}



NA_HIDEF void na_RotateRightBin(NATree* tree, NATreeBinNode* leftchild, NATreeBinNode* parent) {
  NATreeNode* grandparent;
  #if NA_DEBUG
    if(na_IsTreeItemLeaf(tree, na_GetBinNodeItem(leftchild)))
      naError("given left child is not a node");
    if(na_IsTreeItemLeaf(tree, na_GetBinNodeItem(parent)))
      naError("given parent is not a node");
  #endif
  grandparent = na_GetTreeItemParent(na_GetBinNodeItem(parent));
  if(!na_GetTreeItemIsRoot(na_GetBinNodeItem(parent))) {
    size_t parentIndex = na_GetTreeNodeChildIndex(grandparent, na_GetBinNodeItem(parent), tree->config);
    ((NATreeBinNode*)grandparent)->childs[parentIndex] = na_GetBinNodeItem(leftchild);
  }else{
    tree->root = na_GetBinNodeItem(leftchild);
  }

  parent->childs[0] = leftchild->childs[1];
  parent->childs[0]->parent = na_GetBinNodeNode(parent);
  na_SetNodeChildIsLeaf(na_GetBinNodeNode(parent), 0, na_GetNodeChildIsLeaf(na_GetBinNodeNode(leftchild), 1, tree->config), tree->config);
  na_SetTreeItemParent(na_GetBinNodeItem(parent), na_GetBinNodeNode(leftchild));

  leftchild->childs[1] = na_GetBinNodeItem(parent);
  na_SetNodeChildIsLeaf(na_GetBinNodeNode(leftchild), 1, NA_FALSE, tree->config);
  na_SetTreeItemParent(na_GetBinNodeItem(leftchild), grandparent);

  na_UpdateTreeNodeBubbling(tree, na_GetBinNodeNode(parent), NA_TREE_UNSPECIFIED_INDEX);
}



NA_HIDEF void na_PropagateAVLGrow(NATree* tree, NATreeBinNode* binnode) {
  if(!na_GetTreeItemIsRoot(na_GetBinNodeItem(binnode))) {
    NATreeNode* parent = na_GetTreeItemParent(na_GetBinNodeItem(binnode));
    na_GrowAVL(tree, (NATreeBinNode*)parent, na_GetTreeNodeChildIndex(parent, na_GetBinNodeItem(binnode), tree->config));
  }
}



NA_HIDEF void na_PropagateAVLShrink(NATree* tree, NATreeBinNode* binnode) {
  if(!na_GetTreeItemIsRoot(na_GetBinNodeItem(binnode))) {
    NATreeNode* parent = na_GetTreeItemParent(na_GetBinNodeItem(binnode));
    na_ShrinkAVL(tree, (NATreeBinNode*)parent, na_GetTreeNodeChildIndex(parent, na_GetBinNodeItem(binnode), tree->config));
  }
}



NA_HDEF void na_GrowAVL(NATree* tree, NATreeBinNode* binnode, size_t childIndex) {
  int64 nodebalance = na_GetNodeAVL(binnode);
  nodebalance += ((int64)childIndex * 2) - 1;
  if(nodebalance == 0) {
    na_SetNodeAVL(binnode, nodebalance);
  }else if(nodebalance == 1 || nodebalance == -1) {
    na_SetNodeAVL(binnode, nodebalance);
    na_PropagateAVLGrow(tree, binnode);
  }else if(nodebalance == -2) {
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[0];
    int64 childbalance = na_GetNodeAVL(child);
    if(childbalance == -1) {
      na_RotateRightBin(tree, child, binnode);
      na_SetNodeAVL(child, 0);
      na_SetNodeAVL(binnode, 0);
    }else{
      NATreeBinNode* grandchild;
      int64 grandchildbalance;
      #if NA_DEBUG
        if(childbalance != 1)
          naError("child balance invalid with node balance -2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[1];
      grandchildbalance = na_GetNodeAVL(grandchild);
      na_RotateLeftBin(tree, child, grandchild);
      na_RotateRightBin(tree, grandchild, binnode);
      if(grandchildbalance == -1) {
        na_SetNodeAVL(binnode, 1);
        na_SetNodeAVL(grandchild, 0);
        na_SetNodeAVL(child, 0);
      }else if(grandchildbalance == 0) { // Note that in classical AVL, there is
        na_SetNodeAVL(binnode, 0);          // no case 0 but it has been added here
        na_SetNodeAVL(grandchild, 0);    // because of the split operation which
        na_SetNodeAVL(child, 0);         // adds a balanced node to a subtree.
      }else{
        #if NA_DEBUG
          if(grandchildbalance != 1)
            naError("grandchild balance invalid with node balance -2");
        #endif
        na_SetNodeAVL(binnode, 0);
        na_SetNodeAVL(grandchild, 0);
        na_SetNodeAVL(child, -1);
      }
    }
  }else if(nodebalance == 2) {
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[1];
    int64 childbalance = na_GetNodeAVL(child);
    if(childbalance == 1) {
      na_RotateLeftBin(tree, binnode, child);
      na_SetNodeAVL(binnode, 0);
      na_SetNodeAVL(child, 0);
    }else{
      NATreeBinNode* grandchild;
      int64 grandchildbalance;
      #if NA_DEBUG
        if(childbalance != -1)
          naError("child balance invalid with node balance 2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[0];
      grandchildbalance = na_GetNodeAVL(grandchild);
      na_RotateRightBin(tree, grandchild, child);
      na_RotateLeftBin(tree, binnode, grandchild);
      if(grandchildbalance == 1) {
        na_SetNodeAVL(binnode, -1);
        na_SetNodeAVL(grandchild, 0);
        na_SetNodeAVL(child, 0);
      }else if(grandchildbalance == 0) { // Note that in classical AVL, there is
        na_SetNodeAVL(binnode, 0);          // no case 0 but it has been added here
        na_SetNodeAVL(grandchild, 0);    // because of the split operation which
        na_SetNodeAVL(child, 0);         // adds a balanced node to a subtree.
      }else{
        #if NA_DEBUG
          if(grandchildbalance != -1)
            naError("grandchild balance invalid with node balance 2");
        #endif
        na_SetNodeAVL(binnode, 0);
        na_SetNodeAVL(grandchild, 0);
        na_SetNodeAVL(child, 1);
      }
    }
  }else{
    #if NA_DEBUG
      naError("AVL balance out of balance");
    #endif
  }
}



NA_HDEF void na_ShrinkAVL(NATree* tree, NATreeBinNode* binnode, size_t childIndex) {
  int64 nodebalance = na_GetNodeAVL(binnode);
  nodebalance -= ((int64)childIndex * 2) - 1;
  if(nodebalance == 0) {
    na_SetNodeAVL(binnode, nodebalance);
    na_PropagateAVLShrink(tree, binnode);
  }else if(nodebalance == 1 || nodebalance == -1) {
    na_SetNodeAVL(binnode, nodebalance);
  }else if(nodebalance == 2) {
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[1];
    int64 childbalance = na_GetNodeAVL(child);
    if(childbalance == 0) {
      na_RotateLeftBin(tree, binnode, child);
      na_SetNodeAVL(binnode, 1);
      na_SetNodeAVL(child, -1);
    }else if(childbalance == 1) {
      na_RotateLeftBin(tree, binnode, child);
      na_SetNodeAVL(binnode, 0);
      na_SetNodeAVL(child, 0);
      na_PropagateAVLShrink(tree, child);
    }else{
      NATreeBinNode* grandchild;
      int64 grandchildbalance;
      #if NA_DEBUG
        if(childbalance != -1)
          naError("child balance invalid with node balance 2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[0];
      grandchildbalance = na_GetNodeAVL(grandchild);
      na_RotateRightBin(tree, grandchild, child);
      na_RotateLeftBin(tree, binnode, grandchild);
      na_SetNodeAVL(grandchild, 0);
      if(grandchildbalance == -1) {
        na_SetNodeAVL(binnode, 0);
        na_SetNodeAVL(child, 1);
      }else if(grandchildbalance == 0) {
        na_SetNodeAVL(binnode, 0);
        na_SetNodeAVL(child, 0);
      }else{
        #if NA_DEBUG
          if(grandchildbalance != 1)
            naError("grandchild balance invalid with node balance 2");
        #endif
        na_SetNodeAVL(binnode, -1);
        na_SetNodeAVL(child, 0);
      }
      na_PropagateAVLShrink(tree, grandchild);
    }
  }else if(nodebalance == -2) {
    NATreeBinNode* child = (NATreeBinNode*)binnode->childs[0];
    int64 childbalance = na_GetNodeAVL(child);
    if(childbalance == 0) {
      na_RotateRightBin(tree, child, binnode);
      na_SetNodeAVL(child, 1);
      na_SetNodeAVL(binnode, -1);
    }else if(childbalance == -1) {
      na_RotateRightBin(tree, child, binnode);
      na_SetNodeAVL(child, 0);
      na_SetNodeAVL(binnode, 0);
      na_PropagateAVLShrink(tree, child);
    }else{
      NATreeBinNode* grandchild;
      int64 grandchildbalance;
      #if NA_DEBUG
        if(childbalance != 1)
          naError("child balance invalid with node balance -2");
      #endif
      grandchild = (NATreeBinNode*)child->childs[1];
      grandchildbalance = na_GetNodeAVL(grandchild);
      na_RotateLeftBin(tree, child, grandchild);
      na_RotateRightBin(tree, grandchild, binnode);
      na_SetNodeAVL(grandchild, 0);
      if(grandchildbalance == 1) {
        na_SetNodeAVL(child, -1);
        na_SetNodeAVL(binnode, 0);
      }else if(grandchildbalance == 0) {
        na_SetNodeAVL(child, 0);
        na_SetNodeAVL(binnode, 0);
      }else{
        #if NA_DEBUG
          if(grandchildbalance != -1)
            naError("grandchild balance invalid with node balance -2");
        #endif
        na_SetNodeAVL(child, 0);
        na_SetNodeAVL(binnode, 1);
      }
      na_PropagateAVLShrink(tree, grandchild);
    }
  }else{
    #if NA_DEBUG
      naError("AVL balance out of balance");
    #endif
  }
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
