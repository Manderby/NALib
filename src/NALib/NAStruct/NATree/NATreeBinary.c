
#include "NATree.h"
#include "NACoord.h"

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



NA_HDEF NATreeNode* naConstructTreeNodeBinary(NATree* tree, NATreeNode* parent, double key, NAPtr data){
  NATreeBinaryNode* binnode = naNew(NATreeBinaryNode);
  naInitTreeNode(&(binnode->node), parent);

  // Node-specific initialization
  binnode->key = key;
  binnode->childs[0] = NA_NULL;
  binnode->childs[1] = NA_NULL;
  naSetNodeChildType(&(binnode->node), 0, NA_TREE_NODE_CHILD_NULL);
  naSetNodeChildType(&(binnode->node), 1, NA_TREE_NODE_CHILD_NULL);

  if(tree->config->nodeConstructor){
    binnode->data = tree->config->nodeConstructor(&key, tree->config->data, data);
  }else{
    binnode->data = data; 
  }

  return &(binnode->node);
}



NA_HIDEF void naDestructTreeChildBinary(NATree* tree, NATreeBinaryNode* binnode, NAInt childindx){
  NATreeBaseNode* child = binnode->childs[childindx];
  NANodeChildType childtype = naGetNodeChildType(child->parent, childindx);
  if(childtype == NA_TREE_NODE_CHILD_LEAF){tree->config->leafCoreDestructor(tree, (NATreeLeaf*)child);}
  else if(childtype == NA_TREE_NODE_CHILD_NODE){tree->config->nodeCoreDestructor(tree, (NATreeNode*)child);}
}



NA_HDEF void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node){
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)node;
  naDestructTreeChildBinary(tree, binnode, 0);
  naDestructTreeChildBinary(tree, binnode, 1);
  
  if(tree->config->nodeDestructor){tree->config->nodeDestructor(binnode->data, tree->config->data);}
  naClearTreeNode(&(binnode->node));
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, NATreeNode* parent, double key, NAPtr data){
  NATreeBinaryLeaf* binleaf = naNew(NATreeBinaryLeaf);
  naInitTreeLeaf(&(binleaf->leaf), parent);
  
  // Node-specific initialization
  binleaf->key = key;
  if(tree->config->leafConstructor){
    binleaf->data = tree->config->leafConstructor(&key, tree->config->data, data);
  }else{
    binleaf->data = data; 
  }

  return &(binleaf->leaf);
}



NA_HDEF void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)leaf;
  
  if(tree->config->leafDestructor){tree->config->leafDestructor(binleaf->data, tree->config->data);}
  naClearTreeLeaf(&(binleaf->leaf));
  naDelete(leaf);
}



NA_HDEF void naLocateTreeNodeBubbleBinary(NATreeIterator* iter, double* leftlimit, double* rightlimit, NAInt previndxinparent){
  NATreeBinaryNode* node = (NATreeBinaryNode*)(iter->basenode);
  // If we reached the root, return.
  if(node == NA_NULL){return;}
  // If we are at a node which stores the key itself, return.
  if(iter->key.d == node->key){return;}
  // Otherwise, we set the limits dependent on the previous index in parent.
  if(previndxinparent == 0){
    rightlimit = &(node->key);
  }else{
    leftlimit = &(node->key);
  }
  // If we knowh both limits and the key is contained within, return.
  if(leftlimit && rightlimit && naContainsRangeOffset(naMakeRangeWithStartAndEnd(*leftlimit, *rightlimit), iter->key.d)){
    return;
  }
  // Otherwise, go up.
  NAInt indxinparent = naGetTreeNodeIndexInParentBinary(iter->basenode);
  naSetTreeIteratorCurNode(iter, (NATreeBaseNode*)iter->basenode->parent);
  naLocateTreeNodeBubbleBinary(iter, leftlimit, rightlimit, indxinparent);
}



NA_HDEF void naLocateTreeBubbleBinary(NATreeIterator* iter){
  naLocateTreeNodeBubbleBinary(iter, NA_NULL, NA_NULL, 1);
}



NA_HDEF NABool naLocateTreeCaptureBinary(NATreeIterator* iter){
  if(!iter->basenode){
    // If the iterator is at the initial state, try to find the root node.
    const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
    // if there is no root, return false.
    if(!tree->root){return NA_FALSE;}
    naSetTreeIteratorCurNode(iter, &(tree->root->basenode));
  }
  
  NATreeBinaryNode* node = (NATreeBinaryNode*)(iter->basenode);

  NAInt childindx = naGetTreeNodeChildIndxBinary((NATreeNode*)iter->basenode, &(iter->key));
  NANodeChildType childtype = naGetNodeChildType((NATreeNode*)iter->basenode, childindx);
  NATreeBaseNode* childnode = node->childs[childindx];
  
  if(childtype == NA_TREE_NODE_CHILD_NODE){
    // When the left subtree denotes a node, we follow it.
    naSetTreeIteratorCurNode(iter, childnode);
    return naLocateTreeCaptureBinary(iter);
  }else if(childtype == NA_TREE_NODE_CHILD_LEAF){
    // When the left subtree denotes a leaf, we place the iter and return.
    naSetTreeIteratorCurNode(iter, childnode);
    return (iter->key.d == ((NATreeBinaryLeaf*)childnode)->key);
  }

  // In any other case (subtree not available or denoting a link), we give up.
  return NA_FALSE;
}



NA_HDEF void naSetTreeIteratorKeyBinary(NATreeIterator* iter, const void* key){
  iter->key.d = *((const double*)key);
}



NA_HDEF NAInt naGetTreeNodeIndexInParentBinary(NATreeBaseNode* basenode){
  #ifndef NDEBUG
    if(!basenode->parent)
      naError("naGetTreeNodeIndexInParentBinary", "node has no parent");
  #endif
  return (basenode->parent && (((NATreeBinaryNode*)(basenode->parent))->childs[1] == basenode));
}



NA_HDEF NAInt naGetTreeNodeChildIndxBinary(NATreeNode* node, const void* key){
  NATreeBinaryNode* binnode = (NATreeBinaryNode*)(node);
  return !(*(const double*)key < binnode->key);
}



NA_HDEF NABool naTestNodeLimitBinary(NATreeIterator* iter, const void* limit){
  NA_UNUSED(iter);
  NA_UNUSED(limit);
  
  // todo.
  
  return NA_TRUE;
}



NA_HDEF NAPtr* naGetTreeLeafDataBinary(NATreeLeaf* leaf){
  NATreeBinaryLeaf* binleaf = (NATreeBinaryLeaf*)(leaf);
  return &(binleaf->data);
}
