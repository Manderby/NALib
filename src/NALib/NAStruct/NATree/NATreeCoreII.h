
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations.
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"



#define NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE 0x8000

#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80


// Iterates through the tree by moving the iterator to the "next" leaf.
// Returns NA_FALSE, if iteration is over.
NA_HAPI  NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);

// Positions iter at the leaf containing key or which is closest to the key if
// no match was found.
NA_HAPI  NABool naLocateTreeLeaf(NATreeIterator* iter, const void* key, NABool usebubble);

// Prototypes
NA_HAPI  NATreeLeaf* naAddTreeContentAtLeaf(NATree* tree, NATreeLeaf* leaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI  NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace);
NA_HAPI  void naUpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, NAInt childindx);
NA_HAPI  NABool naUpdateTreeNodeCapturing(NATree* tree, NATreeNode* node);
NA_HIAPI NABool naIsTreeRootLeaf(const NATree* tree);

NA_HIAPI const NATree* naGetTreeIteratorTreeConst(const NATreeIterator* iter);
NA_HIAPI NATree* naGetTreeIteratorTreeMutable(NATreeIterator* iter);

#include "../../NACore/NAValueHelper.h"


// /////////////////////////////////////
// Configuration
// /////////////////////////////////////



NA_HIDEF void naDeallocConfiguration(NATreeConfiguration* config){
  if(config->internaldata){naFree(config->internaldata);}
  naFree(config);
}


// X
NA_IDEF void naReleaseTreeConfiguration(NATreeConfiguration* config){
  naReleaseRefCount(&(config->refcount), &(config->refcount), (NAMutator)naDeallocConfiguration);
}



NA_HIDEF NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config){
  #ifndef NDEBUG
    config->flags |= NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE;
  #endif
  return (NATreeConfiguration*)naRetainRefCount(&(config->refcount));
}


// X
NA_IDEF void naSetTreeConfigurationData(NATreeConfiguration* config, NAPtr data){
  #ifndef NDEBUG
    if(naGetPtrConst(config->data))
      naError("Configuration already has data");
  #endif
  config->data = data;
}


// X
NA_IDEF void naSetTreeConfigurationTreeCallbacks(NATreeConfiguration* config, NATreeContructorCallback treeConstructor, NATreeDestructorCallback  treeDestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->treeConstructor = treeConstructor;
  config->treeDestructor = treeDestructor;
}


// X
NA_IDEF void naSetTreeConfigurationLeafCallbacks(NATreeConfiguration* config, NATreeLeafDataConstructor leafDataConstructor, NATreeLeafDataDestructor leafDataDestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->leafDataConstructor = leafDataConstructor;
  config->leafDataDestructor = leafDataDestructor;
}


// X
NA_IDEF void naSetTreeConfigurationNodeCallbacks(NATreeConfiguration* config, NATreeNodeDataConstructor nodedataconstructor, NATreeNodeDataDestructor nodedatadestructor, NATreeNodeUpdater nodeUpdater){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->nodeDataConstructor = nodedataconstructor;
  config->nodeDataDestructor = nodedatadestructor;
  config->nodeUpdater = nodeUpdater;
}


// X
NA_IDEF void naSetTreeConfigurationOcttreeBaseLeafExponent(NATreeConfiguration* config, NAInt baseleafexponent){
  #ifndef NDEBUG
    if(!(config->flags & NA_TREE_OCTTREE))
      naError("This configuration is not for an octtree");
  #endif
  if(config->internaldata){naFree(config->internaldata);}
  NAInt* octtreedata = naMalloc(naSizeof(NAInt));
  *octtreedata = baseleafexponent;
  config->internaldata = octtreedata;
}


// X
NA_IDEF NAInt naGetTreeConfigurationOcttreeBaseLeafExponent(const NATreeConfiguration* config){
  return *((NAInt*)(config->internaldata));
}



// /////////////////////////////////////
// Item
// /////////////////////////////////////

NA_HIDEF void naInitTreeItem(NATreeItem* item){
  // We don not initialize the parent. Code must use naSetTreeItemParent.
  #ifndef NDEBUG
    item->itercount = 0;
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF void naClearTreeItem(NATreeItem* item){
  #ifndef NDEBUG
    if(item->itercount)
      naError("There are still iterators running on this item. Did you forget a call to naClearTreeIterator?");
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF NATreeNode* naGetTreeItemParent(const NATreeItem* item){
  return item->parent;
}



NA_HIDEF void naSetTreeItemParent(NATreeItem* item, NATreeNode* parent){
  item->parent = parent;
}



NA_HIDEF NABool naIsTreeItemRoot(const NATree* tree, const NATreeItem* item){
  NA_UNUSED(tree);
  return (naGetTreeItemParent(item) == NA_NULL);
}



// /////////////////////////////////////
// Node
// /////////////////////////////////////

NA_HIDEF void naInitTreeNode(NATreeNode* node){
  naInitTreeItem(&(node->item));
  node->flags = 0;
}



NA_HIDEF void naClearTreeNode(NATreeNode* node){
  naClearTreeItem(&(node->item));
}



NA_HIDEF NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childindx){
  return (NABool)((node->flags >> childindx) & 0x01);
}



NA_HIDEF NABool naIsTreeItemLeaf(const NATree* tree, NATreeItem* item){
  NAInt childindx;
  NATreeNode* parent;
  if(naIsTreeItemRoot(tree, item)){return naIsTreeRootLeaf(tree);}
  parent = naGetTreeItemParent(item);
  childindx = tree->config->childIndexGetter(parent, item);
  return naIsNodeChildLeaf(parent, childindx);
}



NA_HIDEF void naMarkNodeChildLeaf(NATreeNode* node, NAInt childindx, NABool isleaf){
  node->flags &= ~(1 << childindx);
  node->flags |= (NAInt)isleaf << childindx;
}



NA_HIDEF NABool naIsTreeRootLeaf(const NATree* tree){
  return (NABool)((tree->flags & NA_TREE_FLAG_ROOT_IS_LEAF) == NA_TREE_FLAG_ROOT_IS_LEAF);
}



NA_HIDEF void naMarkTreeRootLeaf(NATree* tree, NABool isleaf){
  tree->flags &= ~NA_TREE_FLAG_ROOT_IS_LEAF;
  tree->flags |= (isleaf * NA_TREE_FLAG_ROOT_IS_LEAF);
}



// /////////////////////////////////////
// Leaf
// /////////////////////////////////////



NA_HIDEF void naInitTreeLeaf(NATreeLeaf* leaf){
  naInitTreeItem(&(leaf->item));
}



NA_HIDEF NAPtr naConstructLeafData(NATree* tree, const void* key, NAPtr data){
  if(tree->config->leafDataConstructor){
    return tree->config->leafDataConstructor(key, tree->config->data, data);
  }else{
    return data;
  }
}



NA_HIDEF void naDestructLeafData(NATree* tree, NAPtr data){
  if(tree->config->leafDataDestructor){
    tree->config->leafDataDestructor(data, tree->config->data);
  }
}



NA_HIDEF void naClearTreeLeaf(NATreeLeaf* leaf){
  naClearTreeItem(&(leaf->item));
}



// /////////////////////////////////////
// Tree
// /////////////////////////////////////

NA_IDEF NATree* naInitTree(NATree* tree, NATreeConfiguration* config){
  tree->config = naRetainTreeConfiguration(config);

  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_OCTTREE) && !tree->config->internaldata)
      naError("Octtree configuration needs more information. Use naSetTreeConfigurationOcttreeBaseLeafExponent");
  #endif

  // If the config defines a callback for constructing a tree, call it.
  if(tree->config->treeConstructor){
    tree->config->treeConstructor(tree->config->data);
  }

  // Init the tree root.
  tree->root = NA_NULL;
  tree->flags = 0;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif

  return tree;
}



NA_IDEF void naEmptyTree(NATree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("There are still iterators running on this tree. Did you miss a naClearTreeIterator call?");
  #endif
  if(tree->root){
    if(naIsTreeRootLeaf(tree)){
      tree->config->leafCoreDestructor(tree, (NATreeLeaf*)tree->root);
    }else{
      tree->config->nodeCoreDestructor(tree, (NATreeNode*)tree->root, NA_TRUE);
    }
  }
  tree->root = NA_NULL;
}



NA_IDEF void naClearTree(NATree* tree){
  naEmptyTree(tree);
  // If the config sets a callback function for deleting a tree, call it.
  if(tree->config->treeDestructor){
    tree->config->treeDestructor(tree->config->data);
  }
  naReleaseTreeConfiguration(tree->config);
}



NA_IDEF NABool naIsTreeEmpty(const NATree* tree){
  return (tree->root == NA_NULL);
}



NA_IDEF NABool naAddTreeFirstConst(NATree* tree, const void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreeNextConst(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeFirstMutable(NATree* tree, void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreeNextMutable(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeLastConst(NATree* tree, const void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreePrevConst(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeLastMutable(NATree* tree, void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreePrevMutable(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF const void* naGetTreeFirstConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurLeafConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeFirstMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurLeafMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF const void* naGetTreeLastConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurLeafConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeLastMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurLeafMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void naUpdateTree(NATree* tree){
  #ifndef NDEBUG
    if(!tree->config->nodeUpdater)
      naError("tree is configured without nodeUpdater callback");
  #endif
  if(tree->root && !naIsTreeRootLeaf(tree)){
    naUpdateTreeNodeCapturing(tree, (NATreeNode*)tree->root);
  }
}



NA_IDEF void naUpdateTreeLeaf(NATreeIterator* iter){
  NATree* tree;
  NATreeNode* parent;
  #ifndef NDEBUG
    if(naIsTreeAtInitial(iter))
      naError("Iterator is not at a leaf");
  #endif
  tree = naGetTreeIteratorTreeMutable(iter);
  parent = naGetTreeItemParent(iter->item);
  if(!naIsTreeItemRoot(tree, iter->item)){
    naUpdateTreeNodeBubbling(tree, parent, tree->config->childIndexGetter(parent, iter->item));
  }
}

// /////////////////////////////////////
// Iterator
// /////////////////////////////////////



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  iter.flags = 0;
  iter.tree = naMakePtrWithDataConst(tree);
  iter.item = NA_NULL;
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    tree->itercount++;
  #endif
  iter.flags = 0;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.item = NA_NULL;
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    tree->itercount++;
  #endif
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.item = NA_NULL;
  return iter;
}



NA_HIDEF void naSetTreeIteratorCurItem(NATreeIterator* iter, NATreeItem* newitem){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeAtInitial(iter)){
      if(iter->item->itercount == 0)
        naError("The current item has zero iterators already");
      iter->item->itercount--;
    }
  #endif
  iter->item = newitem;
  #ifndef NDEBUG
    if(!naIsTreeAtInitial(iter)){iter->item->itercount++;}
  #endif
}



NA_IDEF void naResetTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  naSetTreeIteratorCurItem(iter, NA_NULL);
}



NA_HIDEF const NATree* naGetTreeIteratorTreeConst(const NATreeIterator* iter){
  return naGetPtrConst(iter->tree);
}



NA_HIDEF NATree* naGetTreeIteratorTreeMutable(NATreeIterator* iter){
  return naGetPtrMutable(iter->tree);
}



NA_IDEF void naClearTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    NATree* mutabletree = naGetTreeIteratorTreeMutable(iter);
    mutabletree->itercount--;
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has already been cleared.");
  #endif
  naSetTreeIteratorCurItem(iter, NA_NULL);
  #ifndef NDEBUG
    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #endif
}



NA_IDEF NABool naLocateTreeKey(NATreeIterator* iter, const void* key, NABool assumeclose){
  return naLocateTreeLeaf(iter, key, assumeclose);
}



NA_IDEF NABool naLocateTreeFirst(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTree(iter);
}



NA_IDEF NABool naLocateTreeLast(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTreeBack(iter);
}



NA_IDEF NABool naLocateTreeIterator(NATreeIterator* iter, NATreeIterator* srciter){
  #ifndef NDEBUG
    if(naGetTreeIteratorTreeConst(iter) != naGetPtrConst(srciter->tree))
      naError("The two iterators do not belong to the same tree");
  #endif
  naSetTreeIteratorCurItem(iter, srciter->item);
  return !naIsTreeAtInitial(iter);
}



NA_IDEF void naBubbleTreeToken(const NATreeIterator* iter, void* token, NATreeNodeTokenCallback nodeTokenCallback){
  const NATree* tree;
  NATreeItem* item;
  NABool continueBubbling;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is not at a leaf.");
  #endif
  tree = naGetTreeIteratorTreeConst(iter);
  item = iter->item;
  continueBubbling = NA_TRUE;
  while(continueBubbling && !naIsTreeItemRoot(tree, item)){
    NATreeNode* parent = naGetTreeItemParent(item);
    NAInt childindx = tree->config->childIndexGetter(parent, item);
    continueBubbling = nodeTokenCallback(token, tree->config->nodeDataGetter(parent), childindx);
    item = &(parent->item);
  }
}



NA_IDEF const void* naGetTreeCurLeafKey(NATreeIterator* iter){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(!naIsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a leaf.");
  #endif
  return tree->config->leafKeyGetter((NATreeLeaf*)&(iter->item));
}



NA_IDEF const void* naGetTreeCurNodeKey(NATreeIterator* iter){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(naIsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a node.");
  #endif
  return tree->config->nodeKeyGetter((NATreeNode*)&(iter->item));
}



NA_IDEF const void* naGetTreeCurLeafConst(NATreeIterator* iter){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(!naIsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a leaf.");
  #endif
  return naGetPtrConst(tree->config->leafDataGetter((NATreeLeaf*)&(iter->item)));
}



NA_IDEF const void* naGetTreeCurNodeConst(NATreeIterator* iter){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(naIsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a node.");
  #endif
  return naGetPtrConst(tree->config->nodeDataGetter((NATreeNode*)&(iter->item)));
}



NA_IDEF void* naGetTreeCurLeafMutable(NATreeIterator* iter){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
  if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("This iterator has been cleared. You need to make it anew.");
  if(naIsTreeAtInitial(iter))
    naError("This iterator is at initial position.");
  if(!naIsTreeItemLeaf(tree, iter->item))
    naError("This iterator is not at a leaf.");
  #endif
  return naGetPtrMutable(tree->config->leafDataGetter((NATreeLeaf*)&(iter->item)));
}



NA_IDEF void* naGetTreeCurNodeMutable(NATreeIterator* iter){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
  if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("This iterator has been cleared. You need to make it anew.");
  if(naIsTreeAtInitial(iter))
    naError("This iterator is at initial position.");
  if(naIsTreeItemLeaf(tree, iter->item))
    naError("This iterator is not at a node.");
  #endif
  return naGetPtrMutable(tree->config->nodeDataGetter((NATreeNode*)&(iter->item)));
}



NA_IDEF NABool naIterateTree(NATreeIterator* iter){
  NATreeIterationInfo info;
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(iter->item && !naIsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf. Undefined behaviour. You should reset the iterator.");
  #endif
  info.step = 1;
  info.startindx = 0;
  info.breakindx = tree->config->childpernode;
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naIterateTreeBack(NATreeIterator* iter){
  NATreeIterationInfo info;
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(iter->item && !naIsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf. Undefined behaviour. You should reset the iterator.");
  #endif
  info.step = -1;
  info.startindx = tree->config->childpernode - 1;
  info.breakindx = -1;
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naAddTreeKeyConst(NATreeIterator* iter, const void* key, const void* content, NABool replace){
  #ifndef NDEBUG
    const NATree* tree = naGetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees without keys");
  #endif
  return naAddTreeLeaf(iter, key, naMakePtrWithDataConst(content), replace);
}



NA_IDEF NABool naAddTreeKeyMutable(NATreeIterator* iter, const void* key, void* content, NABool replace){
  #ifndef NDEBUG
    const NATree* tree = naGetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees without keys");
  #endif
  return naAddTreeLeaf(iter, key, naMakePtrWithDataMutable(content), replace);
}



NA_IDEF NABool naIsTreeAtInitial(const NATreeIterator* iter){
  return (iter->item == NA_NULL);
}



NA_IDEF NABool naAddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool movetonew){
  NATreeLeaf* contentleaf;
  NATree* tree = naGetTreeIteratorTreeMutable(iter);
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  if(!iter->item && tree->root){
    // There is a root but the iterator is not at a leaf. We need to find the
    // correct leaf.
    switch(insertOrder){
    case NA_TREE_LEAF_INSERT_ORDER_KEY:
      #ifndef NDEBUG
        naError("This should not happen.");
      #endif
      break;
    case NA_TREE_LEAF_INSERT_ORDER_NEXT:
      naIterateTree(iter);
      insertOrder = NA_TREE_LEAF_INSERT_ORDER_PREV;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_PREV:
      naIterateTreeBack(iter);
      insertOrder = NA_TREE_LEAF_INSERT_ORDER_NEXT;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_REPLACE:
      #ifndef NDEBUG
        naError("This case should not occur");
      #endif
      break;
    }
  }
  contentleaf = naAddTreeContentAtLeaf(tree, (NATreeLeaf*)iter->item, NA_NULL, content, insertOrder);
  if(movetonew){naSetTreeIteratorCurItem(iter, &(contentleaf->item));}
  return NA_TRUE;
}
NA_IDEF NABool naAddTreePrevConst(NATreeIterator* iter, const void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_PREV, movetonew);
}
NA_IDEF NABool naAddTreePrevMutable(NATreeIterator* iter, void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_PREV, movetonew);
}
NA_IDEF NABool naAddTreeNextConst(NATreeIterator* iter, const void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, movetonew);
}
NA_IDEF NABool naAddTreeNextMutable(NATreeIterator* iter, void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, movetonew);
}



NA_IDEF void naRemoveTreeCurLeaf(NATreeIterator* iter, NABool advance){
  NATree* tree = naGetTreeIteratorTreeMutable(iter);
  NATreeNode* newparent;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(!naIsTreeItemLeaf(tree, iter->item))
      naError("Iterator is not at a leaf.");
  #endif
  if(advance){naIterateTree(iter);}else{naIterateTreeBack(iter);}
  newparent = tree->config->leafRemover(tree, (NATreeLeaf*)(iter->item));
  naUpdateTreeNodeBubbling(tree, newparent, -1);
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
