
#include "NATree.h"
#include "NAMemory.h"

typedef struct NATreeNode NATreeNode;
struct NATreeNode{
  double key;
  NATreeNode* parent;
  NATreeNode* child[2];
  NAInt flags;
};

struct NATree{
  NATreeNode* root;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeIterator{
  NAPtr treeptr;
  NATreeNode* node;
  NAInt childindx;
  NAInt flags;
};



// ////////////////////////////////////////
// Tree Node
// ////////////////////////////////////////


#define NA_TREE_FLAG_CHILD_0_IS_LEAF   0x01
#define NA_TREE_FLAG_CHILD_1_IS_LEAF  0x02



NATreeNode* naAllocTreeNode(NATreeNode* parent, double key, void* leaf){
  NATreeNode* node = naAlloc(NATreeNode);
  node->parent = parent;
  node->child[0] = NA_NULL;
  node->child[1] = leaf;
  node->key = key;
  node->flags = NA_TREE_FLAG_CHILD_0_IS_LEAF | NA_TREE_FLAG_CHILD_1_IS_LEAF;
  return node;
}



void naDeallocTreeNode(NATreeNode* node){
  if(node->child[0]){naDeallocTreeNode(node->child[0]);}
  if(node->child[1]){naDeallocTreeNode(node->child[1]);}
  naFree(node);
}



NABool naLocateTreeNode(NATreeIterator* iter, double key){
  NABool retvalue;
  if(key < iter->node->key){
    if(iter->node->flags & NA_TREE_FLAG_CHILD_0_IS_LEAF){
      iter->childindx = 0;
      retvalue = (iter->node->child[0] != NA_NULL);
    }else{
      iter->node = iter->node->child[0];
      retvalue = naLocateTreeNode(iter, key);
    }
  }else{
    if(iter->node->flags & NA_TREE_FLAG_CHILD_1_IS_LEAF){
      iter->childindx = 1;
      retvalue = (iter->node->child[1] != NA_NULL);
    }else{
      iter->node = iter->node->child[1];
      retvalue = naLocateTreeNode(iter, key);
    }
  }
  return retvalue;
}



// ////////////////////////////////////////
// Tree Iterator
// ////////////////////////////////////////


#define NA_TREE_ITERATOR_MODIFIER 0x01


NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.treeptr = naMakePtrWithDataConst(tree);
  iter.node = NA_NULL;
  iter.childindx = -1;
  iter.flags = 0;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  iter.treeptr = naMakePtrWithDataMutable(tree);
  iter.node = NA_NULL;
  iter.childindx = -1;
  iter.flags = 0;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  iter.treeptr = naMakePtrWithDataMutable(tree);
  iter.node = NA_NULL;
  iter.childindx = -1;
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



void naClearTreeIterator(NATreeIterator* iter){
  NA_UNUSED(iter);
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)naGetPtrConst(&(iter->treeptr));
    mutabletree->itercount--;
  #endif
}



NABool naLocateTree(NATreeIterator* iter, double key){
  if(!iter->node){
    const NATree* tree = naGetPtrConst(&(iter->treeptr));
    iter->node = tree->root;    
  }
  if(iter->node){
    return naLocateTreeNode(iter, key);
  }else{
    return NA_FALSE;
  }
}




// ////////////////////////////////////////
// Tree
// ////////////////////////////////////////



NATree* naInitTree(NATree* tree){
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}



void naClearTree(NATree* tree){
  if(tree->root){naDeallocTreeNode(tree->root);}
}



void naAddTreeLeaf(NATree* tree, double key, void* leaf){
  if(tree->root){
    NATreeIterator iter = naMakeTreeModifier(tree);
    NABool found = naLocateTree(&iter, key);
    NATreeNode* node = naAllocTreeNode(iter.node, key, leaf);
    #ifndef NDEBUG
      if(found)
        naError("naAddTreeLeaf", "An element with the given key already exists.");
      if(iter.node->child[iter.childindx])
        naError("naAddTreeLeaf", "child should be null");
    #else
      NA_UNUSED(found);
    #endif
    iter.node->child[iter.childindx] = node;
    naClearTreeIterator(&iter);
  }else{
    tree->root = naAllocTreeNode(NA_NULL, key, leaf);
  }
}



