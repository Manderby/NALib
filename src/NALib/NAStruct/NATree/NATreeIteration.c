
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"



// Locates the iterator on the "next" leaf from the current node given an
// index of the child to first look at. If if no more child is available, the
// iterator is set to the initial position.
NA_HDEF void naIterateTreeCapture(NATreeIterator* iter, NAInt indx, NATreeIterationInfo* info){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naIsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a node");
    if(indx == info->breakindx)
      naError("Index is aready at breakindex. Function will not work properly.");
  #endif
  NATreeNode* parentnode = (NATreeNode*)iter->item;

  // Note: It is safe to assume that this loop is executed at least once.
  while(indx != info->breakindx){
    // We set the iterator to whatever is stored in the desired child.
    naSetTreeIteratorCurItem(iter, naGetTreeNodeChild(tree, parentnode, indx));

    if(iter->item){
      // If the child is a leaf, we found our goal. Good ending.
      if(naIsNodeChildLeaf(parentnode, indx)){break;}
      // The child is a node and we have to go deeper.
      parentnode = (NATreeNode*)iter->item;
      indx = info->startindx;
    }else{
      // If there is no child, just try the next one.
      indx += info->step;
    }
  }
}



// Expects the current item pointed to by the iterator to be exhausted and
// searches for the the "next" item by bubbling to the parent.
NA_HDEF void naIterateTreeBubble(NATreeIterator* iter, NATreeIterationInfo* info){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(!naIsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf");
  #endif

  NATreeItem* item = iter->item;
  naSetTreeIteratorCurItem(iter, NA_NULL);

  while(!naIsTreeItemRoot(tree, item)){
    NATreeNode* parent = naGetTreeItemParent(item);
    NAInt nextindx = naGetTreeNodeChildIndex(tree, parent, item) + info->step;

    // Capture the next sibling, if any.
    if(nextindx != info->breakindx){
      // Yes, it would be more beautiful to check for breakindx in the
      // capture function. But with this if structure, we save one
      // unnecessary function call.
      naSetTreeIteratorCurItem(iter, &(parent->item));
      naIterateTreeCapture(iter, nextindx, info);
      // If we found an item, break.
      if(!naIsTreeAtInitial(iter)){break;}
    }
    item = &(parent->item);
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeAtInitial(iter) && !naIsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf");
  #endif

  // If the tree has no root, we do not iterate.
  if(naIsTreeEmpty(tree)){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("Current iterator item is set although no root available");
    #endif
    return NA_FALSE;
  }

  if(naIsTreeAtInitial(iter)){
    // If the iterator is at initial position, we use the root and capture.
    naSetTreeIteratorCurItem(iter, tree->root);
    if(!naIsTreeRootLeaf(tree)){
      naIterateTreeCapture(iter, info->startindx, info);
    }
  }else{
    // Otherwise, we use the current leaf and bubble to the next one.
    // Note that if iter is not at a leaf, this might lead to overjumping a
    // few leafes.
    naIterateTreeBubble(iter, info);
  }
  
  // Return false, if no more leaf is available.
  return !naIsTreeAtInitial(iter);
}



NA_HDEF NABool naLocateTreeLeaf(NATreeIterator* iter, const void* key, NABool usebubble){
  const NATree* tree = naGetTreeIteratorTreeConst(iter);;
  NATreeNode* node;
  NATreeLeaf* leaf;
  NABool matchfound;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif

  // When there is no root, nothing can be found.
  if(naIsTreeEmpty(tree)){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("Current iterator item is set although no root available");
    #endif
    return NA_FALSE;
  }

  node = NA_NULL;

  // If bubbling is requested, search for the topmost node which potentially
  // contains the given key. But make sure, the iterator is at a leaf and
  // not at the root.
  if(usebubble && !naIsTreeAtInitial(iter) && !naIsTreeItemRoot(tree, iter->item)){
    node = tree->config->bubbleLocator(tree, iter->item, key);
  }
  // todo add test if key contained.

  // Search for the leaf containing key, starting from the uppermost node.
  leaf = tree->config->captureLocator(tree, node, key, &matchfound);
  naSetTreeIteratorCurItem(iter, &(leaf->item));
  #ifndef NDEBUG
    if(!leaf)
      naError("Result of captureLocator was Null");
    if(!naIsTreeItemLeaf(tree, &(leaf->item)))
      naError("Result should be a leaf");
  #endif
  return matchfound;
}



NA_HDEF NABool naLocateTreeToken(NATreeIterator* iter, void* token, NATreeNodeTokenSearcher nodeSearcher, NATreeLeafTokenSearcher leafSearcher){
  NAInt nextindx = NA_TREE_SEARCH_ABORT;
  const NATree* tree = naGetTreeIteratorTreeConst(iter);
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!nodeSearcher)
      naCrash("node token searcher is Null.");
    if(!leafSearcher)
      naCrash("leaf token searcher is Null.");
  #endif
  
  // If the tree is empty, we do nothing.
  if(!naIsTreeEmpty(tree)){
    if(!iter->item){naSetTreeIteratorCurItem(iter, tree->root);}
    
    while(iter->item){
      if(naIsTreeItemLeaf(tree, iter->item)){
        // If the current item is a leaf, call the leaf searcher callback.
        NAPtr data = naGetTreeLeafData(tree, (NATreeLeaf*)iter->item);
        nextindx = leafSearcher(token, data);
      }else{
        // If the current item is a node, call the node searcher callback.
        NAPtr data = naGetTreeNodeData(tree, (NATreeNode*)iter->item);
        nextindx = nodeSearcher(token, data);
      }
      
      #ifndef NDEBUG
        if(nextindx < NA_TREE_SEARCH_ABORT)
          naError("Invalid return value given in callback");
        if(naIsTreeItemLeaf(tree, iter->item) && nextindx >= 0)
          naError("Leaf callback must one of the predefined macros");
        if(!naIsTreeItemLeaf(tree, iter->item) && nextindx >= tree->config->childpernode)
          naError("child index returned in callback is too high");
      #endif
      
      if(nextindx < NA_TREE_SEARCH_PARENT){break;}
      if(nextindx == NA_TREE_SEARCH_PARENT){
        naSetTreeIteratorCurItem(iter, &(naGetTreeItemParent(iter->item)->item));
      // Note that when the iterator is now at the root, the while loop will
      // break automatically and this function returns NA_FALSE.
      }else{
        naSetTreeIteratorCurItem(iter, naGetTreeNodeChild(tree, (NATreeNode*)iter->item, nextindx));
      }
    }
  }

  // Return NA_FALSE in any case which does not correspond to the FOUND macro.
  return (nextindx == NA_TREE_SEARCH_FOUND);
}



NA_HDEF NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace){
  NABool found;
  NATree* tree = naGetTreeIteratorTreeMutable(iter);;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  // We do not use bubbling when inserting as there is almost never a benefit
  // from it. Even more so, it performs mostly worse.
  found = naLocateTreeLeaf(iter, key, NA_FALSE);

  if(found && !replace){return NA_FALSE;}

  if(found){
    // Replace the data
    naDestructLeafData(tree, naGetTreeLeafData(tree, (NATreeLeaf*)iter->item));
    naSetTreeLeafData(tree, (NATreeLeaf*)iter->item, content);
  }else{
    // Add the new data and set the iterator to that newly created position.
    NATreeLeaf* contentleaf = naAddTreeContentAtLeaf(tree, (NATreeLeaf*)iter->item, key, content, NA_TREE_LEAF_INSERT_ORDER_KEY);
    naSetTreeIteratorCurItem(iter, &(contentleaf->item));
  }
  return NA_TRUE;
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
