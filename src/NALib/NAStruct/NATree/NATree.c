
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"



// Returns the "next" leaf while capturing a given parent node and an index
// denoting the index of the child to first look at. If if no more leaf is
// available in the given parent node, NA_NULL is returned.
NA_HDEF NATreeLeaf* naIterateTreeCapture(const NATree* tree, NATreeNode* parent, NAInt indx, NATreeIterationInfo* info){
  while(indx != info->breakindx){
    NATreeItem* child = tree->config->childGetter(parent, indx);
    if(child){
      // If there is a child at the given index.
      if(naIsNodeChildLeaf(parent, indx)){
        // We found the next leaf. Good ending
        return (NATreeLeaf*)child;
      }else{
        // We found a node and have to go deeper.
        return naIterateTreeCapture(tree, (NATreeNode*)child, info->startindx, info);
      }
    }
    indx += info->step;
  }
  // No more leaf available.
  return NA_NULL;
}



// Returns the "next" leaf while bubbling a given "current" item. The "next"
// being defined by the step in the info. If if no more leaf is available
// in the given parent node, NA_NULL is returned.
NA_HDEF NATreeLeaf* naIterateTreeBubble(const NATree* tree, NATreeItem* item, NATreeIterationInfo* info){
  NAInt indx;
  NATreeLeaf* leaf;
  if(naIsTreeItemRoot(tree, item)){
    // We reached the root with no further options. Iteration is over.
    return NA_NULL;
  }
  indx = tree->config->childIndexGetter(item->parent, item);

  // Try to capture the next sibling.
  leaf = naIterateTreeCapture(tree, item->parent, indx + info->step, info);
  if(leaf){
    return leaf;
  }else{
    // No more childs are available in the current parent. Bubble.
    return naIterateTreeBubble(tree, &(item->parent->item), info);
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  NATreeLeaf* leaf;
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naIterateTreeWithInfo", "This iterator has been cleared. You need to make it anew.");
  #endif

  tree = (const NATree*)naGetPtrConst(iter->tree);

  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("naIterateTreeWithInfo", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }

  if(naIsTreeAtInitial(iter)){
    // If the iterator is at initial position, we use the root and capture.
    if(naIsTreeRootLeaf(tree)){
      leaf = (NATreeLeaf*)tree->root;
    }else{
      leaf = naIterateTreeCapture(tree, (NATreeNode*)tree->root, info->startindx, info);
    }
  }else{
    // Otherwise, we use the current leaf and bubble to the next one.
    leaf = naIterateTreeBubble(tree, &(iter->leaf->item), info);
  }
  
  #ifndef NDEBUG
    if(leaf && !naIsItemLeaf(tree, &(leaf->item)))
      naError("naIterateTreeWithInfo", "Result should be a leaf");
  #endif
  // We set the iterator to the found leaf.
  naSetTreeIteratorCurLeaf(iter, leaf);
  // Return false, if no more leaf is available.
  return (leaf != NA_NULL);
}



NA_HDEF NATreeLeaf* naLocateTreeLeaf(NATreeIterator* iter, const void* key, NABool* matchfound, NABool usebubble){
  const NATree* tree;
  NATreeNode* node;
  NATreeLeaf* leaf;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
  #endif

  tree = (const NATree*)naGetPtrConst(iter->tree);

  // When there is no root, nothing can be found.
  if(!tree->root){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    *matchfound = NA_FALSE;
    return NA_FALSE;
  }

  node = NA_NULL;

  // If bubbling is requested, search for the topmost node which potentially
  // contains the given key. But do not do this if the iter has no leaf set.
  if(usebubble && !naIsTreeAtInitial(iter)){
    node = tree->config->bubbleLocator(tree, &(iter->leaf->item), key);
  }

  // Search for the leaf containing key.
  leaf = tree->config->captureLocator(tree, node, key, matchfound);
  #ifndef NDEBUG
    if(!leaf)
      naError("naLocateTreeLeaf", "Result of captureLocator was Null");
    if(!naIsItemLeaf(tree, &(leaf->item)))
      naError("naLocateTreeLeaf", "Result should be a leaf");
  #endif
  return leaf;
}



NA_HDEF NATreeLeaf* naLocateTreeTokenLeaf(NATreeIterator* iter, void* token, NATreeNodeTokenSearcher nodeSearcher, NATreeLeafTokenSearcher leafSearcher, NABool* matchfound){
  NATreeItem* item;
  const NATree* tree = (const NATree*)naGetPtrConst(iter->tree);
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTreeTokenLeaf", "This iterator has been cleared. You need to make it anew.");
    if(!nodeSearcher)
      naCrash("naLocateTreeTokenLeaf", "node token searcher missing.");
    if(!leafSearcher)
      naCrash("naLocateTreeTokenLeaf", "leaf token searcher missing.");
  #endif
  if(naIsTreeEmpty(tree)){
    naSetTreeIteratorCurLeaf(iter, NA_NULL);
    *matchfound = NA_FALSE;
    return NA_FALSE;
  }

  item = &(iter->leaf->item);
  if(!item){item = tree->root;}
  while(item){
    NAInt nextindx;
    if(naIsItemLeaf(tree, item)){
      NAPtr data = tree->config->leafDataGetter((NATreeLeaf*)item);
      leafSearcher(token, data, matchfound);
      return (NATreeLeaf*)item;
    }else{
      NAPtr data = tree->config->nodeDataGetter((NATreeNode*)item);
      NABool continuesearch = nodeSearcher(token, data, &nextindx);
      if(!continuesearch){return NA_NULL;}
    }
    if(nextindx == -1){
      item = &(item->parent->item);
    }else{
      item = tree->config->childGetter((NATreeNode*)item, nextindx);
    }
  }
  return NA_NULL;
}



// Every Add resulting in a change in the tree must go through this function.
NA_HDEF NATreeLeaf* naAddTreeContentAtLeaf(NATree* tree, NATreeLeaf* leaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* contentleaf;
  NATreeNode* parent;
  if(leaf){
    // We need to create a node holding both the old leaf and the new one.
    contentleaf = tree->config->leafInserter(tree, leaf, key, content, insertOrder);
    parent = contentleaf->item.parent;
    naUpdateTreeNodeBubbling(tree, parent, tree->config->childIndexGetter(parent, &(contentleaf->item)));
  }else{
    #ifndef NDEBUG
      if(tree->root)
        naError("naAddTreeContentAtLeaf", "leaf is null but there is a root");
    #endif
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    contentleaf = tree->config->leafCoreConstructor(tree, key, content);
    tree->root = &(contentleaf->item);
    contentleaf->item.parent = NA_NULL;
    naMarkTreeRootLeaf(tree, NA_TRUE);
  }
  return contentleaf;
}



NA_HDEF NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace){
  NABool matchfound;
  NATreeLeaf* leaf;
  NATree* tree;
  NATreeLeafInsertOrder insertOrder;
  NATreeLeaf* contentleaf;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naAddTreeLeaf", "This iterator has been cleared. You need to make it anew.");
  #endif
  // We do not use bubbling when inserting as there is almost never a benefit
  // from it. Even worse, it performs mostly worse.
  leaf = naLocateTreeLeaf(iter, key, &matchfound, NA_FALSE);
  tree = (NATree*)naGetPtrMutable(iter->tree);

  if(matchfound && !replace){return NA_FALSE;}

  insertOrder = matchfound ? NA_TREE_LEAF_INSERT_ORDER_REPLACE : NA_TREE_LEAF_INSERT_ORDER_KEY;
  contentleaf = naAddTreeContentAtLeaf(tree, leaf, key, content, insertOrder);
  naSetTreeIteratorCurLeaf(iter, contentleaf);
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
    naUpdateTreeNodeBubbling(tree, parent->item.parent, tree->config->childIndexGetter(parent->item.parent, &(parent->item)));
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
      naError("naUpdateTreeNodeCapturing", "tree is configured without nodeUpdater callback");
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
