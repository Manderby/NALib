
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"
#include "NATreeBinary.h"



struct NATreeBinaryNode{
  NATreeNode node;
  double key;
  NATreeBaseNode* childs[2];
  NAPtr data;
};
NA_RUNTIME_TYPE(NATreeBinaryNode, NA_NULL, NA_FALSE);

struct NATreeBinaryLeaf{
  NATreeLeaf leaf;
  double key;
  NAPtr data;
};
NA_RUNTIME_TYPE(NATreeBinaryLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void naDestructTreeChildBinary(NATree* tree, NATreeBinaryNode* binnode, NAInt childindx){
  NATreeBaseNode* child = binnode->childs[childindx];
  NANodeChildType childtype = naGetNodeChildType((NATreeNode*)binnode, childindx);
  if(childtype == NA_TREE_NODE_CHILD_LEAF){naDestructTreeLeafBinary(tree, (NATreeLeaf*)child);}
  else if(childtype == NA_TREE_NODE_CHILD_NODE){naDestructTreeNodeBinary(tree, (NATreeNode*)child);}
}



NA_HIDEF NABool naTestNodeChildsBinary(NATreeNode* node){
  return naIsNodeChildTypeValid(naGetNodeChildType(node, 0)) || naIsNodeChildTypeValid(naGetNodeChildType(node, 1));
}



NA_HDEF void naRemoveNodeBinary(NATree* tree, NATreeNode* node){
  NATreeNode* parent = ((NATreeBaseNode*)node)->parent;
  if(!parent){
    tree->root = NA_NULL;
  }else{
    NAInt nodeindx = naGetChildIndexBinary(parent, (NATreeBaseNode*)node);
    naSetNodeChildType(parent, nodeindx, NA_TREE_NODE_CHILD_NULL);
    ((NATreeBinaryNode*)parent)->childs[nodeindx] = NA_NULL;

    if(tree->config->flags & NA_TREE_BALANCE_AVL){naShrinkAVL(tree, (NATreeBinaryNode*)parent, nodeindx);}

    if(!naTestNodeChildsBinary(parent)){
      naRemoveNodeBinary(tree, parent);
    }
  }

  naDestructTreeNodeBinary(tree, node);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////



NA_HDEF NAInt naGetKeyIndexBinaryDouble(const void* basekey, const void* key){
  return !(*(const double*)key < *(const double*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinaryDouble(const void* key1, const void* key2){
  return (*(const double*)key1 == *(const double*)key2);
}
NA_HDEF void naAssignKeyBinaryDouble(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
}
NA_HDEF NABool naTestKeyBinaryDouble(const void* leftlimit, const void* rightlimit, const void* key){
  return ((*(const double*)leftlimit <= *(const double*)key) && (*(const double*)rightlimit >= *(const double*)key));
}



NA_HDEF NAInt naGetKeyIndexBinaryNAInt(const void* basekey, const void* key){
  return !(*(const NAInt*)key < *(const NAInt*)basekey); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyBinaryNAInt(const void* key1, const void* key2){
  return (*(const NAInt*)key1 == *(const NAInt*)key2);
}
NA_HDEF void naAssignKeyBinaryNAInt(void* dst, const void* src){
  *(NAInt*)dst = *(const NAInt*)src;
}
NA_HDEF NABool naTestKeyBinaryNAInt(const void* leftlimit, const void* rightlimit, const void* key){
  return ((*(const NAInt*)leftlimit <= *(const NAInt*)key) && (*(const NAInt*)rightlimit >= *(const NAInt*)key));
}



NA_HDEF NATreeNode* naConstructTreeNodeBinary(NATree* tree, const void* key){
  NATreeBinaryNode* binnode = naNew(NATreeBinaryNode);
  naInitTreeNode((NATreeNode*)binnode);

  // Node-specific initialization
  tree->config->keyAssigner(&(binnode->key), key);
  binnode->childs[0] = NA_NULL;
  binnode->childs[1] = NA_NULL;
  naSetNodeChildType((NATreeNode*)binnode, 0, NA_TREE_NODE_CHILD_NULL);
  naSetNodeChildType((NATreeNode*)binnode, 1, NA_TREE_NODE_CHILD_NULL);
  if(tree->config->flags & NA_TREE_BALANCE_AVL){naInitNodeAVL((NATreeBinaryNode*)binnode);}

  if(tree->config->nodeConstructor){
    binnode->data = tree->config->nodeConstructor(&key, tree->config->data);
  }else{
    binnode->data = naMakePtrNull(); 
  }

  return (NATreeNode*)binnode;
}



NA_HDEF void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node){
  #ifndef NDEBUG
    if(!node)
      naCrash("naDestructTreeNodeBinary", "node shall not be Null");
  #endif
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)node;
  naDestructTreeChildBinary(tree, binnode, 0);
  naDestructTreeChildBinary(tree, binnode, 1);
  
  if(tree->config->nodeDestructor){tree->config->nodeDestructor(binnode->data, tree->config->data);}
  naClearTreeNode((NATreeNode*)binnode);
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, const void* key, NAPtr data){
  NATreeBinaryLeaf* binleaf = naNew(NATreeBinaryLeaf);
  naInitTreeLeaf(&(binleaf->leaf));
  
  // Node-specific initialization
  tree->config->keyAssigner(&(binleaf->key), key);
  binleaf->data = naConstructLeafData(tree, &(binleaf->key), data);

  return (NATreeLeaf*)binleaf;
}



NA_HDEF void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf){
  #ifndef NDEBUG
    if(!leaf)
      naCrash("naDestructTreeLeafBinary", "leaf shall not be Null");
  #endif
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)leaf;

  naDestructLeafData(tree, binleaf->data);
  naClearTreeLeaf(&(binleaf->leaf));
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleBinaryWithLimits(const NATree* tree, NATreeNode* node, const void* key, const void* leftlimit, const void* rightlimit, NATreeBaseNode* prevnode){
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("naLocateBubbleBinaryWithLimits", "node should not be null");
  #endif
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(key, &(binnode->key))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(!prevnode || naGetChildIndexBinary((NATreeNode*)binnode, prevnode) == 1){
    // Note that this case will also happen if NA_NULL was sent as prevnode.
    leftlimit = &(binnode->key);
  }else{
    rightlimit = &(binnode->key);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, key)){
    return node;
  }
  // Otherwise, go up if possible.
  if(((NATreeBaseNode*)node)->parent){
    return naLocateBubbleBinaryWithLimits(tree, ((NATreeBaseNode*)node)->parent, key, leftlimit, rightlimit, (NATreeBaseNode*)binnode);
  }else{
    // If this node is the root node, we return this node.
    return node;
  }
}



NA_HDEF NATreeNode* naLocateBubbleBinary(const NATree* tree, NATreeNode* node, const void* key){
  return naLocateBubbleBinaryWithLimits(tree, node, key, NA_NULL, NA_NULL, NA_NULL);
}



NA_HDEF NATreeNode* naLocateCaptureBinary(const NATree* tree, NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* childindx){
  #ifndef NDEBUG
    if(!node)
      naError("naLocateCaptureBinary", "node must not be Null");
  #endif
  
  *keyleaffound = NA_FALSE;
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(node);
  *childindx = naGetChildKeyIndexBinary(tree, node, key);
  NANodeChildType childtype = naGetNodeChildType(node, *childindx);
  NATreeBaseNode* childnode = binnode->childs[*childindx];
  
  if(childtype == NA_TREE_NODE_CHILD_NODE){
    // When the subtree denotes a node, we follow it.
    return naLocateCaptureBinary(tree, (NATreeNode*)childnode, key, keyleaffound, childindx);
  }else if(childtype == NA_TREE_NODE_CHILD_LEAF){
    if(tree->config->keyEqualer(key, &(((NATreeBinaryLeaf*)childnode)->key))){
      // When the subtree denotes a leaf and it has the desire key, success!
      *keyleaffound = NA_TRUE;
    }
  }
  
  // In any other case (wrong leaf, subtree not available or subtree denoting
  // a link, we give up and return the current node.
  return node;
}



NA_HDEF NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child){
  #ifndef NDEBUG
    if(!child)
      naError("naGetChildIndexBinary", "Child should not be Null");
  #endif
  NATreeBinaryNode* binparent = (NATreeBinaryNode*)(parent);
  NAInt retvalue = (child != binparent->childs[0]); // return 0 or 1
  #ifndef NDEBUG
    if(child != binparent->childs[retvalue])
      naError("naGetChildIndexBinary", "Child is no child of parent");
  #endif
  return retvalue;
}



NA_HDEF NAInt naGetChildKeyIndexBinary(const NATree* tree, NATreeNode* parent, const void* key){
  #ifndef NDEBUG
    if(!parent)
      naError("naGetChildKeyIndexBinary", "parent is Null");
  #endif
  NATreeBinaryNode* binparent = (NATreeBinaryNode*)(parent);
  return tree->config->keyIndexGetter(&(binparent->key), key);
}



NA_HDEF NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx){
  #ifndef NDEBUG
    if(childindx < 0 || childindx >= 2)
      naError("naGetChildBinary", "child index not valid");
  #endif
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(parent);
  return binnode->childs[childindx];
}



NA_HDEF void naAddLeafBinary(NATree* tree, NATreeNode* parent, NATreeLeaf* leaf, NAInt leafindx){
  NATreeBinaryNode* binparent = (NATreeBinaryNode*)parent; 
  naSetNodeChildType(parent, leafindx, NA_TREE_NODE_CHILD_LEAF);
  binparent->childs[leafindx] = (NATreeBaseNode*)leaf;
  ((NATreeBaseNode*)leaf)->parent = parent;

//  if(tree->config->flags & NA_TREE_BALANCE_AVL){naGrowAVL(tree, (NATreeBinaryNode*)parent, leafindx);}
}



NA_HDEF void naRemoveLeafBinary(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = ((NATreeBaseNode*)leaf)->parent;
  NAInt leafindx = naGetChildIndexBinary(parent, (NATreeBaseNode*)leaf);

  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent);
    NATreeBaseNode* sibling = ((NATreeBinaryNode*)parent)->childs[1 - leafindx];
    if(sibling){sibling->parent = grandparent;}
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = sibling;
    naSetNodeChildType(grandparent, parentindx, naGetNodeChildType(parent, 1 - leafindx));
    
    ((NATreeBinaryNode*)parent)->childs[1 - leafindx] = NA_NULL;
    naSetNodeChildType(parent, 1 - leafindx, NA_TREE_NODE_CHILD_NULL);
    naDestructTreeNodeBinary(tree, parent);

    if(tree->config->flags & NA_TREE_BALANCE_AVL){naShrinkAVL(tree, (NATreeBinaryNode*)grandparent, parentindx);}
  }

//  NATreeNode* newparent = ((NATreeBaseNode*)leaf)->parent;

//  naSetNodeChildType(parent, leafindx, NA_TREE_NODE_CHILD_NULL);
//  ((NATreeBinaryNode*)parent)->childs[leafindx] = NA_NULL;
//  naDestructTreeLeafBinary(tree, leaf);
//
//  if(tree->config->flags & NA_TREE_BALANCE_AVL){naShrinkAVL(tree, (NATreeBinaryNode*)parent, leafindx);}

//  if(!naTestNodeChildsBinary(parent)){
//    naRemoveNodeBinary(tree, parent);
//  }

}



NA_HDEF void naReplaceLeafBinary(NATree* tree, NATreeLeaf* leaf, NAPtr data){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)leaf;
  naDestructLeafData(tree, binleaf->data);
  binleaf->data = naConstructLeafData(tree, &(binleaf->key), data);
}



NA_HDEF void naSplitLeafBinary(NATree* tree, NATreeNode* grandparent, NAInt leafindx, NATreeLeaf* sibling){
  NATreeLeaf* left;
  NATreeLeaf* right;
  #ifndef NDEBUG
    if(naGetNodeChildType(grandparent, leafindx) != NA_TREE_NODE_CHILD_LEAF)
      naError("naSplitLeafBinary", "Given child should be a leaf");
  #endif
  NATreeBinaryLeaf* leaf = (NATreeBinaryLeaf*)naGetChildBinary(grandparent, leafindx);
  NAInt siblingindx = tree->config->keyIndexGetter(&(leaf->key), &(((NATreeBinaryLeaf*)sibling)->key));
  if(siblingindx == 0){
    left = sibling;
    right = (NATreeLeaf*)leaf;
  }else{
    left = (NATreeLeaf*)leaf;
    right = sibling;
  }
  
  NATreeBinaryNode* parent = (NATreeBinaryNode*)naConstructTreeNodeBinary(tree, &(((NATreeBinaryLeaf*)right)->key));
  naAddLeafBinary(tree, (NATreeNode*)parent, left, 0);
  naAddLeafBinary(tree, (NATreeNode*)parent, right, 1);

  naSetNodeChildType(grandparent, leafindx, NA_TREE_NODE_CHILD_NODE);
  ((NATreeBinaryNode*)grandparent)->childs[leafindx] = (NATreeBaseNode*)parent;
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)grandparent;

  if(tree->config->flags & NA_TREE_BALANCE_AVL){naGrowAVL(tree, (NATreeBinaryNode*)grandparent, leafindx);}
}



NA_HDEF const void* naGetLeafKeyBinary(NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)(leaf);
  return &(binleaf->key);
}



NA_HDEF NAPtr* naGetLeafDataBinary(NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)(leaf);
  return &(binleaf->data);
}


// /////////////////////////////
// AVL
// /////////////////////////////

void naInitNodeAVL(NATreeBinaryNode* node){
  ((NATreeNode*)node)->flags |= NA_TREE_NODE_AVL_EQUAL;
}



NAInt naGetNodeAVL(NATreeBinaryNode* node){
  return (NAInt)((((NATreeNode*)node)->flags & NA_TREE_NODE_AVL_MASK) >> NA_TREE_NODE_AVL_BITSHIFT) - 1;
}



void naSetNodeAVL(NATreeBinaryNode* node, NAInt balance){
  naSetFlagi(&(((NATreeNode*)node)->flags), NA_TREE_NODE_AVL_MASK, NA_FALSE);
  ((NATreeNode*)node)->flags |= (balance + 1) << NA_TREE_NODE_AVL_BITSHIFT;
}



void naRotateLeftBinary(NATree* tree, NATreeBinaryNode* parent, NATreeBinaryNode* rightchild){
  #ifndef NDEBUG
    NANodeChildType parentchildtype = naGetNodeType(tree, (NATreeBaseNode*)parent);
    if(parentchildtype != NA_TREE_NODE_CHILD_NODE)
      naError("naRotateLeftBinary", "given parent is not a node");
    NANodeChildType rightchildtype = naGetNodeType(tree, (NATreeBaseNode*)rightchild);
    if(rightchildtype != NA_TREE_NODE_CHILD_NODE)
      naError("naRotateLeftBinary", "given right child is not a node");
  #endif
  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent); 
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = (NATreeBaseNode*)rightchild;
  }else{
    tree->root = (NATreeNode*)rightchild;
  }

  parent->childs[1] = rightchild->childs[0];
  if(parent->childs[1]){parent->childs[1]->parent = (NATreeNode*)parent;}
  naSetNodeChildType((NATreeNode*)parent, 1, naGetNodeChildType((NATreeNode*)rightchild, 0));
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)rightchild;
  
  rightchild->childs[0] = (NATreeBaseNode*)parent;
  naSetNodeChildType((NATreeNode*)rightchild, 0, NA_TREE_NODE_CHILD_NODE);
  ((NATreeBaseNode*)rightchild)->parent = grandparent;
}



void naRotateRightBinary(NATree* tree, NATreeBinaryNode* leftchild, NATreeBinaryNode* parent){
  #ifndef NDEBUG
    NANodeChildType leftchildtype = naGetNodeType(tree, (NATreeBaseNode*)leftchild);
    if(leftchildtype != NA_TREE_NODE_CHILD_NODE)
      naError("naRotateRightBinary", "given left child is not a node");
    NANodeChildType parentchildtype = naGetNodeType(tree, (NATreeBaseNode*)parent);
    if(parentchildtype != NA_TREE_NODE_CHILD_NODE)
      naError("naRotateRightBinary", "given parent is not a node");
  #endif
  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent); 
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = (NATreeBaseNode*)leftchild;
  }else{
    tree->root = (NATreeNode*)leftchild;
  }

  parent->childs[0] = leftchild->childs[1];
  if(parent->childs[0]){parent->childs[0]->parent = (NATreeNode*)parent;}
  naSetNodeChildType((NATreeNode*)parent, 0, naGetNodeChildType((NATreeNode*)leftchild, 1));
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)leftchild;
  
  leftchild->childs[1] = (NATreeBaseNode*)parent;
  naSetNodeChildType((NATreeNode*)leftchild, 1, NA_TREE_NODE_CHILD_NODE);
  ((NATreeBaseNode*)leftchild)->parent = grandparent;
}



void naExchangeLeftBinary(NATree* tree, NATreeBinaryNode* parent, NATreeBinaryNode* rightchild){
  #ifndef NDEBUG
    if(naGetNodeType(tree, (NATreeBaseNode*)parent) != NA_TREE_NODE_CHILD_NODE)
      naError("naExchangeLeftBinary", "given parent is not a node");
    if(naGetNodeType(tree, (NATreeBaseNode*)rightchild) != NA_TREE_NODE_CHILD_NODE)
      naError("naExchangeLeftBinary", "given right child is not a node");
    if(rightchild->childs[1])
      naError("naExchangeRightBinary", "right child has a right child");
  #endif
  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent); 
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = (NATreeBaseNode*)rightchild;
  }else{
    tree->root = (NATreeNode*)rightchild;
  }

  ((NATreeBaseNode*)rightchild)->parent = grandparent;

  rightchild->childs[1] = (NATreeBaseNode*)parent;
  naSetNodeChildType((NATreeNode*)rightchild, 1, NA_TREE_NODE_CHILD_NODE);
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)rightchild;

  parent->childs[1] = NA_NULL;
  naSetNodeChildType((NATreeNode*)parent, 1, NA_TREE_NODE_CHILD_NULL);
}



void naExchangeRightBinary(NATree* tree, NATreeBinaryNode* leftchild, NATreeBinaryNode* parent){
  #ifndef NDEBUG
    if(naGetNodeType(tree, (NATreeBaseNode*)leftchild) != NA_TREE_NODE_CHILD_NODE)
      naError("naExchangeRightBinary", "given left child is not a node");
    if(naGetNodeType(tree, (NATreeBaseNode*)parent) != NA_TREE_NODE_CHILD_NODE)
      naError("naExchangeRightBinary", "given parent is not a node");
    if(leftchild->childs[0])
      naError("naExchangeRightBinary", "left child has a left child");
  #endif
  NATreeNode* grandparent = ((NATreeBaseNode*)parent)->parent;
  if(grandparent){
    NAInt parentindx = naGetChildIndexBinary(grandparent, (NATreeBaseNode*)parent); 
    ((NATreeBinaryNode*)grandparent)->childs[parentindx] = (NATreeBaseNode*)leftchild;
  }else{
    tree->root = (NATreeNode*)leftchild;
  }

  ((NATreeBaseNode*)leftchild)->parent = grandparent;

  leftchild->childs[0] = (NATreeBaseNode*)parent;
  naSetNodeChildType((NATreeNode*)leftchild, 0, NA_TREE_NODE_CHILD_NODE);
  ((NATreeBaseNode*)parent)->parent = (NATreeNode*)leftchild;

  parent->childs[0] = NA_NULL;
  naSetNodeChildType((NATreeNode*)parent, 0, NA_TREE_NODE_CHILD_NULL);
}



void naPropagateAVLGrow(NATree* tree, NATreeBinaryNode* node){
  if(((NATreeBaseNode*)node)->parent){
    naGrowAVL(tree, (NATreeBinaryNode*)((NATreeBaseNode*)node)->parent, naGetChildIndexBinary(((NATreeBaseNode*)node)->parent, (NATreeBaseNode*)node));
  }
}



void naPropagateAVLShrink(NATree* tree, NATreeBinaryNode* node){
  if(((NATreeBaseNode*)node)->parent){
    naShrinkAVL(tree, (NATreeBinaryNode*)((NATreeBaseNode*)node)->parent, naGetChildIndexBinary(((NATreeBaseNode*)node)->parent, (NATreeBaseNode*)node));
  }
}



void naGrowAVL(NATree* tree, NATreeBinaryNode* node, NAInt childindx){
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
    }else if(childbalance == 0){
      // This case has been added.
      naRotateRightBinary(tree, child, node);
      naSetNodeAVL(child, 1);
      naSetNodeAVL(node, -1);
      naPropagateAVLGrow(tree, child);
    }else{
      #ifndef NDEBUG
        if(childbalance != 1)
          naError("naGrowAVL", "child balance invalid with node balance -2");
      #endif
      NATreeBinaryNode* grandchild = (NATreeBinaryNode*)child->childs[1];
      if(naGetNodeChildType((NATreeNode*)child, 1) == NA_TREE_NODE_CHILD_LEAF){
        // Special case
        naExchangeRightBinary(tree, child, node);
        naSetNodeAVL(child, 0);
        naSetNodeAVL(node, 0);
        naPropagateAVLShrink(tree, node);
      }else{
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
    }
  }else if(nodebalance == 2){
    NATreeBinaryNode* child = (NATreeBinaryNode*)node->childs[1];
    NAInt childbalance = naGetNodeAVL(child);
    if(childbalance == 1){
      naRotateLeftBinary(tree, node, child);
      naSetNodeAVL(node, 0);
      naSetNodeAVL(child, 0);
    }else if(childbalance == 0){
      // This case has been added.
      naRotateLeftBinary(tree, node, child);
      naSetNodeAVL(node, 1);
      naSetNodeAVL(child, -1);
      naPropagateAVLGrow(tree, child);
    }else{
      #ifndef NDEBUG
        if(childbalance != -1)
          naError("naGrowAVL", "child balance invalid with node balance 2");
      #endif
      NATreeBinaryNode* grandchild = (NATreeBinaryNode*)child->childs[0];
      if(naGetNodeChildType((NATreeNode*)child, 0) == NA_TREE_NODE_CHILD_LEAF){
        // Special case
        naExchangeLeftBinary(tree, node, child);
        naSetNodeAVL(child, 0);
        naSetNodeAVL(node, 0);
        naPropagateAVLShrink(tree, node);
      }else{
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
    }
  }else{
    #ifndef NDEBUG
      naError("naGrowAVL", "AVL balance out of balance");
    #endif
  }
}

void naShrinkAVL(NATree* tree, NATreeBinaryNode* node, NAInt childindx){
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
      if(naGetNodeChildType((NATreeNode*)child, 0) == NA_TREE_NODE_CHILD_LEAF){
        // Special case
        naExchangeLeftBinary(tree, node, child);
        naSetNodeAVL(child, 0);
        naSetNodeAVL(node, 0);
        naPropagateAVLShrink(tree, child);
      }else{
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
      if(naGetNodeChildType((NATreeNode*)child, 1) == NA_TREE_NODE_CHILD_LEAF){
        // Special case
        naExchangeRightBinary(tree, child, node);
        naSetNodeAVL(child, 0);
        naSetNodeAVL(node, 0);
        naPropagateAVLShrink(tree, child);
      }else{
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
