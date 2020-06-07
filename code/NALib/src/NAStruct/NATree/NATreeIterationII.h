
#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80



#undef naBeginTreeAccessorIteration
#define naBeginTreeAccessorIteration(typedelem, tree, lowerlimit, upperlimit, iter)\
  iter = naMakeTreeAccessor(tree);\
  while(naIterateTree(&iter, lowerlimit, upperlimit)){\
    typedelem = naGetTreeCurLeafConst(&iter)

#undef naBeginTreeMutatorIteration
#define naBeginTreeMutatorIteration(typedelem, tree, lowerlimit, upperlimit, iter)\
  iter = naMakeTreeMutator(tree);\
  while(naIterateTree(&iter, lowerlimit, upperlimit)){\
    typedelem = naGetTreeCurLeafMutable(&iter)

#undef naBeginTreeModifierIteration
#define naBeginTreeModifierIteration(typedelem, tree, lowerlimit, upperlimit, iter)\
  iter = naMakeTreeModifier(tree);\
  while(naIterateTree(&iter, lowerlimit, upperlimit)){\
    typedelem = naGetTreeCurLeafMutable(&iter)

#undef naEndTreeIteration
#define naEndTreeIteration(iter)\
  }\
  naClearTreeIterator(&iter)



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->iterCount++;
    iter.flags = 0;
  #endif
  iter.tree = naMakePtrWithDataConst(tree);
  iter.item = NA_NULL;
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    tree->iterCount++;
    iter.flags = 0;
  #endif
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.item = NA_NULL;
  return iter;
}



// todo check modifier
NA_IDEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    tree->iterCount++;
    iter.flags = NA_TREE_ITERATOR_MODIFIER;
  #endif
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.item = NA_NULL;
  return iter;
}



NA_HIDEF void na_SetTreeIteratorCurItem(NATreeIterator* iter, NATreeItem* newitem){
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeAtInitial(iter)){
      if(!iter->item)
        naCrash("No item stored in the current iterator");
      if(iter->item->iterCount == 0)
        naError("The current item has zero iterators already");
      iter->item->iterCount--;
    }
  #endif
  iter->item = newitem;
  #ifndef NDEBUG
    if(!naIsTreeAtInitial(iter)){iter->item->iterCount++;}
  #endif
}



NA_IDEF void naResetTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  na_SetTreeIteratorCurItem(iter, NA_NULL);
}



NA_HIDEF const NATree* na_GetTreeIteratorTreeConst(const NATreeIterator* iter){
  return naGetPtrConst(iter->tree);
}



NA_HIDEF NATree* na_GetTreeIteratorTreeMutable(NATreeIterator* iter){
  return naGetPtrMutable(iter->tree);
}



NA_IDEF void naClearTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    NATree* mutabletree = na_GetTreeIteratorTreeMutable(iter);
    mutabletree->iterCount--;
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has already been cleared.");

    // Yes, we only execute the following line in debug code.
    // Release should not do anything at all.
    na_SetTreeIteratorCurItem(iter, NA_NULL);

    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #else
    NA_UNUSED(iter);
  #endif
}



NA_IDEF NABool naLocateTreeKey(NATreeIterator* iter, const void* key, NABool assumeclose){
  #ifndef NDEBUG
    const NATree* tree = na_GetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  return na_LocateTreeKeyCore(iter, key, assumeclose);
}



NA_IDEF NABool naLocateTreeFirst(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTree(iter, NA_NULL, NA_NULL);
}



NA_IDEF NABool naLocateTreeLast(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTreeBack(iter, NA_NULL, NA_NULL);
}



NA_IDEF NABool naLocateTreeIterator(NATreeIterator* iter, NATreeIterator* srciter){
  #ifndef NDEBUG
    if(na_GetTreeIteratorTreeConst(iter) != naGetPtrConst(srciter->tree))
      naError("The two iterators do not belong to the same tree");
  #endif
  na_SetTreeIteratorCurItem(iter, srciter->item);
  return !naIsTreeAtInitial(iter);
}



NA_IDEF void naBubbleTreeToken(const NATreeIterator* iter, void* token, NATreeNodeTokenCallback nodeTokenCallback){
  const NATree* tree;
  NATreeItem* item;
  NABool continueBubbling;
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is not at a leaf.");
  #endif
  tree = na_GetTreeIteratorTreeConst(iter);
  item = iter->item;
  continueBubbling = NA_TRUE;
  while(continueBubbling && !na_IsTreeItemRoot(item)){
    NATreeNode* parent = na_GetTreeItemParent(item);
    NAInt childIndex = na_GetTreeNodeChildIndex(tree->config, parent, item);
    continueBubbling = nodeTokenCallback(token, na_GetTreeNodeData(tree->config, parent), childIndex);
    item = &(parent->item);
  }
}



NA_IDEF const void* naGetTreeCurLeafKey(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(!na_IsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a leaf.");
  #endif
  return na_GetTreeLeafKey(tree->config, (NATreeLeaf*)iter->item);
}



NA_IDEF const void* naGetTreeCurNodeKey(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(na_IsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a node.");
  #endif
  return na_GetTreeNodeKey(tree->config, (NATreeNode*)iter->item);
}



NA_IDEF const void* naGetTreeCurLeafConst(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(!na_IsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a leaf.");
  #endif
  return naGetPtrConst(na_GetTreeLeafData(tree->config, (NATreeLeaf*)iter->item));
}



NA_IDEF const void* naGetTreeCurNodeConst(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(na_IsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a node.");
  #endif
  return naGetPtrConst(na_GetTreeNodeData(tree->config, (NATreeNode*)iter->item));
}



NA_IDEF void* naGetTreeCurLeafMutable(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
  if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("This iterator has been cleared. You need to make it anew.");
  if(naIsTreeAtInitial(iter))
    naError("This iterator is at initial position.");
  if(!na_IsTreeItemLeaf(tree, iter->item))
    naError("This iterator is not at a leaf.");
  #endif
  return naGetPtrMutable(na_GetTreeLeafData(tree->config, (NATreeLeaf*)iter->item));
}



NA_IDEF void* naGetTreeCurNodeMutable(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
  if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("This iterator has been cleared. You need to make it anew.");
  if(naIsTreeAtInitial(iter))
    naError("This iterator is at initial position.");
  if(na_IsTreeItemLeaf(tree, iter->item))
    naError("This iterator is not at a node.");
  #endif
  return naGetPtrMutable(na_GetTreeNodeData(tree->config, (NATreeNode*)iter->item));
}



NA_IDEF NABool naIterateTree(NATreeIterator* iter, const void* lowerlimit, const void* upperlimit){
  NATreeIterationInfo info;
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(iter->item && !na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf. Undefined behaviour. You should reset the iterator.");
    if(lowerlimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use a lower limit.");
    if(upperlimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use an upper limit.");
  #endif
  info.step = 1;
  info.startIndex = 0;
  info.breakIndex = tree->config->childpernode;
  info.lowerlimit = lowerlimit;
  info.upperlimit = upperlimit;
  return na_IterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naIterateTreeBack(NATreeIterator* iter, const void* lowerlimit, const void* upperlimit){
  NATreeIterationInfo info;
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(iter->item && !na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf. Undefined behaviour. You should reset the iterator.");
    if(lowerlimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use a lower limit.");
    if(upperlimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use an upper limit.");
  #endif
  info.step = -1;
  info.startIndex = tree->config->childpernode - 1;
  info.breakIndex = -1;
  info.lowerlimit = lowerlimit;
  info.upperlimit = upperlimit;
  return na_IterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naAddTreeKeyConst(NATreeIterator* iter, const void* key, const void* content, NABool replace){
  #ifndef NDEBUG
    const NATree* tree = na_GetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees without keys");
  #endif
  return na_AddTreeLeaf(iter, key, naMakePtrWithDataConst(content), replace);
}



NA_IDEF NABool naAddTreeKeyMutable(NATreeIterator* iter, const void* key, void* content, NABool replace){
  #ifndef NDEBUG
    const NATree* tree = na_GetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees without keys");
  #endif
  return na_AddTreeLeaf(iter, key, naMakePtrWithDataMutable(content), replace);
}



NA_IDEF NABool naIsTreeAtInitial(const NATreeIterator* iter){
  return (iter->item == NA_NULL);
}



NA_HIDEF NABool na_AddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool movetonew){
  NATreeLeaf* contentleaf;
  NATree* tree = na_GetTreeIteratorTreeMutable(iter);
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
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
      naIterateTree(iter, NA_NULL, NA_NULL);
      insertOrder = NA_TREE_LEAF_INSERT_ORDER_PREV;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_PREV:
      naIterateTreeBack(iter, NA_NULL, NA_NULL);
      insertOrder = NA_TREE_LEAF_INSERT_ORDER_NEXT;
      break;
    }
  }
  contentleaf = na_AddTreeContentInPlace(tree, iter->item, NA_NULL, content, insertOrder);
  if(movetonew){na_SetTreeIteratorCurItem(iter, &(contentleaf->item));}
  return NA_TRUE;
}
NA_IDEF NABool naAddTreePrevConst(NATreeIterator* iter, const void* content, NABool movetonew){
  return na_AddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_PREV, movetonew);
}
NA_IDEF NABool naAddTreePrevMutable(NATreeIterator* iter, void* content, NABool movetonew){
  return na_AddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_PREV, movetonew);
}
NA_IDEF NABool naAddTreeNextConst(NATreeIterator* iter, const void* content, NABool movetonew){
  return na_AddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, movetonew);
}
NA_IDEF NABool naAddTreeNextMutable(NATreeIterator* iter, void* content, NABool movetonew){
  return na_AddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, movetonew);
}



NA_IDEF void naRemoveTreeCurLeaf(NATreeIterator* iter){
  NATree* tree = na_GetTreeIteratorTreeMutable(iter);
  NATreeNode* newParent;
  NATreeItem* removeItem;

  #ifndef NDEBUG
    if(naGetFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(!na_IsTreeItemLeaf(tree, iter->item))
      naError("Iterator is not at a leaf.");
  #endif
  
  removeItem = iter->item;
  na_SetTreeIteratorCurItem(iter, NA_NULL);  // temporarily remove the iterator. // todo: make it advance, rev-advance or not.
  newParent = tree->config->leafRemover(tree, (NATreeLeaf*)(removeItem));
  if(newParent){na_SetTreeIteratorCurItem(iter, na_GetTreeNodeItem(newParent));}
  na_UpdateTreeNodeBubbling(tree, newParent, -1);
}



NA_IDEF void naUpdateTreeLeaf(NATreeIterator* iter){
  NATree* tree;
  NATreeNode* parent;
  if(naIsTreeAtInitial(iter)){return;}
  #ifndef NDEBUG
    if(naIsTreeAtInitial(iter))
      naError("Iterator is not at a leaf");
  #endif
  tree = na_GetTreeIteratorTreeMutable(iter);
  parent = na_GetTreeItemParent(iter->item);
  if(!na_IsTreeItemRoot(iter->item)){
    na_UpdateTreeNodeBubbling(tree, parent, na_GetTreeNodeChildIndex(tree->config, parent, iter->item));
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
