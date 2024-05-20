
#include "../../NATree.h"



// Locates the iterator on the "next" leaf from the current node given an
// index of the child to first look at. If if no more child is available, the
// iterator is set to the initial position.
NA_HDEF void na_IterateTreeCapture(NATreeIterator* iter, NAInt index, NATreeIterationInfo* info) {
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  NATreeNode* parentNode;

  #if NA_DEBUG
    if(na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a node");
    if(index == info->breakIndex)
      naError("Index is aready at breakindex. Function will not work properly.");
  #endif
  
  parentNode = (NATreeNode*)iter->item;

  // Note: It is safe to assume that this loop is executed at least once.
  while(index != info->breakIndex) {
    // We set the iterator to whatever is stored in the desired child.
    na_SetTreeIteratorCurItem(iter, na_GetTreeNodeChild(tree->config, parentNode, index));

    if(iter->item) {
      if(na_IsNodeChildLeaf(parentNode, index)) {
        // The child is a leaf.
        if(
          (!info->lowerLimit && !info->upperLimit) ||
          tree->config->keyLeafOverlapTester((NATreeLeaf*)iter->item, info->lowerLimit, info->upperLimit))
        {
          // We found our goal. Good ending.
          break;
        }
      }else{
        // The child is a node.
        if(
          (!info->lowerLimit && !info->upperLimit) ||
          tree->config->keyNodeOverlapTester((NATreeNode*)iter->item, info->lowerLimit, info->upperLimit))
        {
          // we have to go deeper.
          parentNode = (NATreeNode*)iter->item;
          index = info->startIndex;
          continue;
        }
      }
      na_SetTreeIteratorCurItem(iter, NA_NULL);
    }
    // In any other case, just try the next child.
    index += info->step;
  }
}



// Expects the current item pointed to by the iterator to be exhausted and
// searches for the the "next" item by bubbling to the parent.
NA_HDEF void na_IterateTreeBubble(NATreeIterator* iter, NATreeIterationInfo* info) {
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  NATreeItem* item;
  
  #if NA_DEBUG
    if(!na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf");
  #endif

  item = iter->item;
  na_SetTreeIteratorCurItem(iter, NA_NULL);

  while(!na_IsTreeItemRoot(item)) {
    NATreeNode* parent = na_GetTreeItemParent(item);
    NAInt nextIndex = na_GetTreeNodeChildIndex(tree->config, parent, item) + info->step;

    // Capture the next sibling, if any.
    if(nextIndex != info->breakIndex) {
      // Yes, it would be more beautiful to check for breakIndex in the
      // capture function. But with this if structure, we save one
      // unnecessary function call.
      na_SetTreeIteratorCurItem(iter, &(parent->item));
      na_IterateTreeCapture(iter, nextIndex, info);
      
      // If we found a leaf, break.
      if(!naIsTreeAtInitial(iter) && na_IsTreeItemLeaf(tree, iter->item))
        break;
      
      na_SetTreeIteratorCurItem(iter, NA_NULL);
    }
    item = &(parent->item);
  }
}



NA_HDEF NABool na_IterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info) {
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeAtInitial(iter) && !na_IsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf");
  #endif

  // If the tree has no root, we do not iterate.
  if(naIsTreeEmpty(tree)) {
    #if NA_DEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("Current iterator item is set although no root available");
    #endif
    return NA_FALSE;
  }

  if(naIsTreeAtInitial(iter)) {
    // If the iterator is at initial position, we use the root and capture.
    na_SetTreeIteratorCurItem(iter, tree->root);
    if(!naIsTreeRootLeaf(tree)) {
      na_IterateTreeCapture(iter, info->startIndex, info);
    }
  }else{
    // Otherwise, we use the current leaf and bubble to the next one.
    // Note that if iter is not at a leaf, this might lead to overjumping a
    // few leafes.
    na_IterateTreeBubble(iter, info);
  }
  
  // Return false, if no more leaf is available.
  return !naIsTreeAtInitial(iter);
}



NA_HDEF NATreeItem* na_LocateTreeKeyCapture(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound) {
  NAInt childIndex;
  NATreeItem* child;
  #if NA_DEBUG
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif

  *matchfound = NA_FALSE;

  if(!node) {
    if(naIsTreeRootLeaf(tree)) {
      *matchfound = tree->config->keyLeafContainTester((NATreeLeaf*)tree->root, key);
      return tree->root;
    }else{
      node = (NATreeNode*)tree->root;
    }
  }

  // Test if the node still contains the desired key. If not, we arrived at
  // the node closest to the desired key.
  if(tree->config->keyNodeContainTester && !tree->config->keyNodeContainTester(node, key)) {
    return na_GetTreeNodeItem(node);
  }

  childIndex = tree->config->childIndexGetter(node, key);
  child = na_GetTreeNodeChild(tree->config, node, childIndex);

  if(!child) {
    // No child at the desired position. Return the closest parent.
    return na_GetTreeNodeItem(node);
  }else if(na_IsNodeChildLeaf(node, childIndex)) {
    // When the subtree denotes a leaf, we test, if the key is equal and return
    // the result.
    *matchfound = tree->config->keyLeafContainTester((NATreeLeaf*)child, key);
    return child;
  }else{
    // When the subtree denotes a node, we follow it.
    return na_LocateTreeKeyCapture(tree, (NATreeNode*)child, key, matchfound);
  }
}



NA_HDEF NABool na_LocateTreeKey(NATreeIterator* iter, const void* key, NABool usebubble) {
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  NATreeNode* node;
  NATreeItem* founditem;
  NABool matchfound;
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif

  // When there is no root, nothing can be found.
  if(naIsTreeEmpty(tree)) {
    #if NA_DEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("Current iterator item is set although no root available");
    #endif
    return NA_FALSE;
  }

  node = NA_NULL;

  // If bubbling is requested, search for the topmost node which potentially
  // contains the given key. But make sure, the iterator is at a leaf and
  // not at the root.
  if(usebubble && !naIsTreeAtInitial(iter) && !na_IsTreeItemRoot(iter->item)) {
    node = tree->config->bubbleLocator(tree, iter->item, key);
  }

  // Search for the leaf containing key, starting from the uppermost node.
  founditem = na_LocateTreeKeyCapture(tree, node, key, &matchfound);
  na_SetTreeIteratorCurItem(iter, founditem);
  #if NA_DEBUG
    if(!founditem)
      naError("Result of capture locator was Null");
  #endif
  return matchfound;
}



NA_DEF NABool naLocateTreeToken(NATreeIterator* iter, void* token, NATreeNodeTokenSearcher nodeSearcher, NATreeLeafTokenSearcher leafSearcher) {
  NAInt nextIndex = NA_TREE_SEARCH_ABORT;
  const NATree* tree = na_GetTreeIteratorTreeConst(iter);
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!nodeSearcher)
      naCrash("node token searcher is Null.");
    if(!leafSearcher)
      naCrash("leaf token searcher is Null.");
  #endif
  
  // If the tree is empty, we do nothing.
  if(!naIsTreeEmpty(tree)) {
    if(!iter->item) {
      na_SetTreeIteratorCurItem(iter, tree->root);
    }
    
    while(iter->item) {
      if(na_IsTreeItemLeaf(tree, iter->item)) {
        // If the current item is a leaf, call the leaf searcher callback.
        NAPtr data = na_GetTreeLeafData(tree->config, (NATreeLeaf*)iter->item);
        nextIndex = leafSearcher(token, data);
      }else{
        // If the current item is a node, call the node searcher callback.
        NAPtr data = na_GetTreeNodeData(tree->config, (NATreeNode*)iter->item);
        nextIndex = nodeSearcher(token, data);
      }
      
      #if NA_DEBUG
        if(nextIndex < NA_TREE_SEARCH_ABORT)
          naError("Invalid return value given in callback");
        if(na_IsTreeItemLeaf(tree, iter->item) && nextIndex >= 0)
          naError("Leaf callback must one of the predefined macros");
        if(!na_IsTreeItemLeaf(tree, iter->item) && nextIndex >= tree->config->childpernode)
          naError("child index returned in callback is too high");
      #endif
      
      if(nextIndex < NA_TREE_SEARCH_PARENT) {
        break;
      }else if(nextIndex == NA_TREE_SEARCH_PARENT) {
        NATreeNode* parentNode = na_GetTreeItemParent(iter->item);
        if(parentNode) {
          na_SetTreeIteratorCurItem(iter, &(parentNode->item));
        }else{
          naResetTreeIterator(iter);
          nextIndex = NA_TREE_SEARCH_ABORT;
        }
      }else{
        na_SetTreeIteratorCurItem(iter, na_GetTreeNodeChild(tree->config, (NATreeNode*)iter->item, nextIndex));
      }
    }
  }

  // Return NA_FALSE in any case which does not correspond to the FOUND macro.
  return (nextIndex == NA_TREE_SEARCH_FOUND);
}



NA_HDEF NABool na_AddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace) {
  NABool found;
  NATree* tree = na_GetTreeIteratorTreeMutable(iter);;
  #if NA_DEBUG
    if(naGetFlagu32(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  // We do not use bubbling when inserting as there is almost never a benefit
  // from it. Even more so, it performs mostly worse.

  found = na_LocateTreeKey(iter, key, NA_FALSE);

  if(!found || replace) {
    if(found) {
      // Destruct the leaf and recreate it again.
      na_DestructLeafData(tree->config, na_GetTreeLeafData(tree->config, (NATreeLeaf*)(iter->item)));
      na_SetTreeLeafData(tree->config, (NATreeLeaf*)(iter->item), na_ConstructLeafData(tree->config, key, content));
    }else{
      // Add the new data and set the iterator to that newly created position.
      NATreeLeaf* contentleaf = na_AddTreeContentInPlace(tree, iter->item, key, content, NA_TREE_LEAF_INSERT_ORDER_KEY);
      na_SetTreeIteratorCurItem(iter, &(contentleaf->item));
    }
  }
  return found;
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
