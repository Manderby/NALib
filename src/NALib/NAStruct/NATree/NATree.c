
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
    naSetTreeIteratorCurItem(iter, tree->config->childGetter(parentnode, indx));

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

  if(naIsTreeItemRoot(tree, iter->item)){
    naSetTreeIteratorCurItem(iter, NA_NULL);
  }else{
    NAInt nextindx;
    NATreeItem* item = iter->item;
    NATreeNode* parent = naGetTreeItemParent(item);
    nextindx = tree->config->childIndexGetter(parent, item) + info->step;

    // Capture the next sibling, if any.
    if(nextindx == info->breakindx){
      // Yes, it would be more beautiful to do this in the capture function.
      // But with this if structure, we save one unnecessary function call.
      naSetTreeIteratorCurItem(iter, NA_NULL);
    }else{
      naIterateTreeCapture(iter, nextindx, info);
    }
    
    if(naIsTreeAtInitial(iter)){
      // No more childs are available in the current node. Bubble to the parent.
      naSetTreeIteratorCurItem(iter, &(parent->item));
      naIterateTreeBubble(iter, info);
    }
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeItemLeaf(tree, iter->item))
      naError("Iter is not placed at a leaf");
  #endif

  tree = naGetTreeIteratorTreeConst(iter);

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
    naIterateTreeBubble(iter, info);
  }
  
  // Return false, if no more leaf is available.
  return !naIsTreeAtInitial(iter);
}



NA_HDEF NABool naLocateTreeLeaf(NATreeIterator* iter, const void* key, NABool usebubble){
  const NATree* tree;
  NATreeNode* node;
  NATreeLeaf* leaf;
  NABool matchfound;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif

  tree = naGetTreeIteratorTreeConst(iter);

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
        NAPtr data = tree->config->leafDataGetter((NATreeLeaf*)iter->item);
        nextindx = leafSearcher(token, data);
      }else{
        // If the current item is a node, call the node searcher callback.
        NAPtr data = tree->config->nodeDataGetter((NATreeNode*)iter->item);
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
        naSetTreeIteratorCurItem(iter, tree->config->childGetter((NATreeNode*)iter->item, nextindx));
      }
    }
  }

  // Return NA_FALSE in any case which does not correspond to the FOUND macro.
  return (nextindx == NA_TREE_SEARCH_FOUND);
}



// Every Add resulting in a change in the tree must go through this function.
NA_HDEF NATreeLeaf* naAddTreeContentAtLeaf(NATree* tree, NATreeLeaf* leaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* contentleaf;
  NATreeNode* parent;
  if(leaf){
    // We need to create a node holding both the old leaf and the new one.
    contentleaf = tree->config->leafInserter(tree, leaf, key, content, insertOrder);
    parent = naGetTreeItemParent(&(contentleaf->item));
    naUpdateTreeNodeBubbling(tree, parent, tree->config->childIndexGetter(parent, &(contentleaf->item)));
  }else{
    #ifndef NDEBUG
      if(tree->root)
        naError("leaf is null but there is a root");
    #endif
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    contentleaf = tree->config->leafCoreConstructor(tree, key, content);
    tree->root = &(contentleaf->item);
    naSetTreeItemParent(&(contentleaf->item), NA_NULL);
    naMarkTreeRootLeaf(tree, NA_TRUE);
  }
  return contentleaf;
}



NA_HDEF NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace){
  NABool found;
  NATree* tree;
  NATreeLeafInsertOrder insertOrder;
  NATreeLeaf* contentleaf;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("This iterator has been cleared. You need to make it anew.");
  #endif
  tree = naGetTreeIteratorTreeMutable(iter);
  // We do not use bubbling when inserting as there is almost never a benefit
  // from it. Even more so, it performs mostly worse.
  found = naLocateTreeLeaf(iter, key, NA_FALSE);

  if(found && !replace){return NA_FALSE;}

  insertOrder = found ? NA_TREE_LEAF_INSERT_ORDER_REPLACE : NA_TREE_LEAF_INSERT_ORDER_KEY;
  contentleaf = naAddTreeContentAtLeaf(tree, (NATreeLeaf*)iter->item, key, content, insertOrder);
  naSetTreeIteratorCurItem(iter, &(contentleaf->item));
  return NA_TRUE;
}



NA_HDEF void naFillTreeNodeChildData(NATree* tree, NAPtr childdata[NA_TREE_NODE_MAX_CHILDS], NATreeNode* node){
  NAInt i;
  for(i=0; i<tree->config->childpernode; i++){
    NATreeItem* child = tree->config->childGetter(node, i);
    if(naIsNodeChildLeaf(node, i)){
      childdata[i] = tree->config->leafDataGetter((NATreeLeaf*)child);
    }else{
      childdata[i] = tree->config->nodeDataGetter((NATreeNode*)child);
    }
  }
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular
// node.
NA_HDEF void naUpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, NAInt childindx){
  NABool bubble = NA_TRUE;
  if(parent == NA_NULL){return;}

  // We call the update callback.
  if(tree->config->nodeUpdater){
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    naFillTreeNodeChildData(tree, childdata, parent);
    bubble = tree->config->nodeUpdater(tree->config->nodeDataGetter(parent), childdata, childindx, parent->flags & NA_TREE_CHILDS_MASK);
  }

  // Then we propagate the message towards the root if requested.
  if(bubble && !naIsTreeItemRoot(tree, &(parent->item))){
    NATreeNode* grandparent = naGetTreeItemParent(&(parent->item));
    naUpdateTreeNodeBubbling(tree, grandparent, tree->config->childIndexGetter(grandparent, &(parent->item)));
  }
}




// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the childchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateTreeNodeCapturing(NATree* tree, NATreeNode* node){
  NABool bubble;
  NAInt i;
  #ifndef NDEBUG
    if(!tree->config->nodeUpdater)
      naError("tree is configured without nodeUpdater callback");
  #endif
  bubble = NA_FALSE;

  // Go through all childs and call the capturing message recursively.
  for(i=0; i<tree->config->childpernode; i++){
    if(naIsNodeChildLeaf(node, i)){
      bubble |= NA_TRUE;
    }else{
      // this node stores subnodes
      bubble |= naUpdateTreeNodeCapturing(tree, (NATreeNode*)tree->config->childGetter(node, i));
    }
  }
  
  // Only if at least one of the childs requested further bubbling, we update
  // this very node.
  if(bubble){
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    naFillTreeNodeChildData(tree, childdata, node);
    bubble = tree->config->nodeUpdater(tree->config->nodeDataGetter(node), childdata, -1, node->flags & NA_TREE_CHILDS_MASK);
  }

  return bubble;
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
