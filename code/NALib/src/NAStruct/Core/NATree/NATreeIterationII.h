
#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80



#undef naBeginTreeAccessorIteration
#define naBeginTreeAccessorIteration(typedElem, tree, lowerLimit, upperLimit, iter)\
  iter = naMakeTreeAccessor(tree);\
  while(naIterateTree(&iter, lowerLimit, upperLimit)){\
    typedElem = naGetTreeCurLeafConst(&iter)

#undef naBeginTreeMutatorIteration
#define naBeginTreeMutatorIteration(typedElem, tree, lowerLimit, upperLimit, iter)\
  iter = naMakeTreeMutator(tree);\
  while(naIterateTree(&iter, lowerLimit, upperLimit)){\
    typedElem = naGetTreeCurLeafMutable(&iter)

#undef naBeginTreeModifierIteration
#define naBeginTreeModifierIteration(typedElem, tree, lowerLimit, upperLimit, iter)\
  iter = naMakeTreeModifier(tree);\
  while(naIterateTree(&iter, lowerLimit, upperLimit)){\
    typedElem = naGetTreeCurLeafMutable(&iter)

#undef naEndTreeIteration
#define naEndTreeIteration(iter)\
  }\
  naClearTreeIterator(&iter)



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  #if NA_DEBUG
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
  #if NA_DEBUG
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
  #if NA_DEBUG
    tree->iterCount++;
    iter.flags = NA_TREE_ITERATOR_MODIFIER;
  #endif
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.item = NA_NULL;
  return iter;
}



NA_HIDEF void na_SetTreeIteratorCurItem(NATreeIterator* iter, NATreeItem* newitem){
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeAtInitial(iter)){
      if(!iter->item)
        naCrash("No item stored in the current iterator");
      if(na_GetTreeItemIterCount(iter->item) == 0)
        naError("The current item has zero iterators already");
      na_DecTreeItemIterCount(iter->item);
    }
  #endif
  iter->item = newitem;
  #if NA_DEBUG
    if(!naIsTreeAtInitial(iter)){na_IncTreeItemIterCount(iter->item);}
  #endif
}



NA_IDEF void naResetTreeIterator(NATreeIterator* iter){
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  na_SetTreeIteratorCurItem(iter, NA_NULL);
}



NA_HIDEF const NATree* na_GetTreeIteratorTreeConst(const NATreeIterator* iter){
  return (const NATree*)naGetPtrConst(iter->tree);
}



NA_HIDEF NATree* na_GetTreeIteratorTreeMutable(NATreeIterator* iter){
  return (NATree*)naGetPtrMutable(iter->tree);
}



NA_IDEF void naClearTreeIterator(NATreeIterator* iter){
  #if NA_DEBUG
    NATree* mutabletree = na_GetTreeIteratorTreeMutable(iter);
    mutabletree->iterCount--;
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has already been cleared.");

    // Yes, we only execute the following line in debug code.
    // Release should not do anything at all.
    na_SetTreeIteratorCurItem(iter, NA_NULL);

    naSetFlagu32(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #else
    NA_UNUSED(iter);
  #endif
}



NA_IDEF NABool naLocateTreeKey(NATreeIterator* iter, const void* key, NABool assumeClose){
  #if NA_DEBUG
    const NATree* tree = na_GetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  return na_LocateTreeKey(iter, key, assumeClose);
}



NA_IDEF NABool naLocateTreeFirst(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTree(iter, NA_NULL, NA_NULL);
}



NA_IDEF NABool naLocateTreeLast(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTreeBack(iter, NA_NULL, NA_NULL);
}



NA_IDEF NABool naLocateTreeIterator(NATreeIterator* iter, NATreeIterator* srcIter){
  #if NA_DEBUG
    if(na_GetTreeIteratorTreeConst(iter) != naGetPtrConst(srcIter->tree))
      naError("The two iterators do not belong to the same tree");
  #endif
  na_SetTreeIteratorCurItem(iter, srcIter->item);
  return !naIsTreeAtInitial(iter);
}



NA_IDEF void naBubbleTreeToken(const NATreeIterator* iter, void* token, NATreeNodeTokenCallback nodeTokenCallback){
  const NATree* tree;
  NATreeItem* item;
  NABool continueBubbling;
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is not at a leaf.");
  #endif
  tree = na_GetTreeIteratorTreeConst(iter);
  item = iter->item;
  continueBubbling = NA_TRUE;
  while(continueBubbling && !na_GetTreeItemIsRoot(item)){
    NATreeNode* parent = na_GetTreeItemParent(item);
    NAInt childIndex = na_GetTreeNodeChildIndex(parent, item, tree->config);
    continueBubbling = nodeTokenCallback(token, na_GetTreeNodeData(parent, tree->config), childIndex);
    item = &(parent->item);
  }
}



NA_IDEF const void* naGetTreeCurLeafKey(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
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
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(na_IsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a node.");
  #endif
  return na_GetTreeNodeKey((NATreeNode*)iter->item, tree->config);
}



NA_IDEF const void* naGetTreeCurLeafConst(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
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
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("This iterator is at initial position.");
    if(na_IsTreeItemLeaf(tree, iter->item))
      naError("This iterator is not at a node.");
  #endif
  return naGetPtrConst(na_GetTreeNodeData((NATreeNode*)iter->item, tree->config));
}



NA_IDEF void* naGetTreeCurLeafMutable(NATreeIterator* iter){
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
  if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
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
  #if NA_DEBUG
  if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("This iterator has been cleared. You need to make it anew.");
  if(naIsTreeAtInitial(iter))
    naError("This iterator is at initial position.");
  if(na_IsTreeItemLeaf(tree, iter->item))
    naError("This iterator is not at a node.");
  #endif
  return naGetPtrMutable(na_GetTreeNodeData((NATreeNode*)iter->item, tree->config));
}



NA_IDEF NABool naIterateTree(NATreeIterator* iter, const void* lowerLimit, const void* upperLimit){
  NATreeIterationInfo info;
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
    if(iter->item && !na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf. Undefined behaviour. You should reset the iterator.");
    if(lowerLimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use a lower limit.");
    if(upperLimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use an upper limit.");
  #endif
  info.step = 1;
  info.startIndex = 0;
  info.breakIndex = tree->config->childPerNode;
  info.lowerLimit = lowerLimit;
  info.upperLimit = upperLimit;
  return na_IterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naIterateTreeBack(NATreeIterator* iter, const void* lowerLimit, const void* upperLimit){
  NATreeIterationInfo info;
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
    if(iter->item && !na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf. Undefined behaviour. You should reset the iterator.");
    if(lowerLimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use a lower limit.");
    if(upperLimit && ((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY))
      naError("Tree requires to have a key in order to use an upper limit.");
  #endif
  info.step = -1;
  info.startIndex = tree->config->childPerNode - 1;
  info.breakIndex = -1;
  info.lowerLimit = lowerLimit;
  info.upperLimit = upperLimit;
  return na_IterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naAddTreeKeyConst(NATreeIterator* iter, const void* key, const void* content, NABool replace){
  #if NA_DEBUG
    const NATree* tree = na_GetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees without keys");
  #endif
  return na_AddTreeLeaf(iter, key, naMakePtrWithDataConst(content), replace);
}



NA_IDEF NABool naAddTreeKeyMutable(NATreeIterator* iter, const void* key, void* content, NABool replace){
  #if NA_DEBUG
    const NATree* tree = na_GetTreeIteratorTreeConst(iter);
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees without keys");
  #endif
  return na_AddTreeLeaf(iter, key, naMakePtrWithDataMutable(content), replace);
}



NA_IDEF NABool naIsTreeAtInitial(const NATreeIterator* iter){
  return (iter->item == NA_NULL);
}



NA_HIDEF NABool na_AddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool moveToNew){
  NATreeLeaf* contentleaf;
  NATree* tree = na_GetTreeIteratorTreeMutable(iter);
  #if NA_DEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  if(!iter->item && tree->root){
    // There is a root but the iterator is not at a leaf. We need to find the
    // correct leaf.
    switch(insertOrder){
    case NA_TREE_LEAF_INSERT_ORDER_KEY:
      #if NA_DEBUG
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
  if(moveToNew){na_SetTreeIteratorCurItem(iter, &(contentleaf->item));}
  return NA_TRUE;
}
NA_IDEF NABool naAddTreePrevConst(NATreeIterator* iter, const void* content, NABool moveToNew){
  return na_AddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_PREV, moveToNew);
}
NA_IDEF NABool naAddTreePrevMutable(NATreeIterator* iter, void* content, NABool moveToNew){
  return na_AddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_PREV, moveToNew);
}
NA_IDEF NABool naAddTreeNextConst(NATreeIterator* iter, const void* content, NABool moveToNew){
  return na_AddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, moveToNew);
}
NA_IDEF NABool naAddTreeNextMutable(NATreeIterator* iter, void* content, NABool moveToNew){
  return na_AddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, moveToNew);
}



NA_IDEF void naRemoveTreeCurLeaf(NATreeIterator* iter){
  NATree* tree = na_GetTreeIteratorTreeMutable(iter);
  NATreeNode* newParent;
  NATreeItem* removeItem;

  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
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
  #if NA_DEBUG
    if(naIsTreeAtInitial(iter))
      naError("Iterator is not at a leaf");
  #endif
  tree = na_GetTreeIteratorTreeMutable(iter);
  parent = na_GetTreeItemParent(iter->item);
  if(!na_GetTreeItemIsRoot(iter->item)){
    na_UpdateTreeNodeBubbling(tree, parent, na_GetTreeNodeChildIndex(parent, iter->item, tree->config));
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
