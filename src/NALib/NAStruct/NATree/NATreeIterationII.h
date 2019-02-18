
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80



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



// todo check modifier
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

    // Yes, we only execute the following line in debug code.
    // Release should not do anything at all.
    naSetTreeIteratorCurItem(iter, NA_NULL);

    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #else
    NA_UNUSED(iter);
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



NA_HIDEF NABool naAddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool movetonew){
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
