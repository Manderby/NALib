
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"



NA_HDEF NATreeLeaf* naIterateTreeCapture(const NATree* tree, NATreeNode* parent, NAInt previndx, NATreeIterationInfo* info){
  NAInt indx = previndx + info->step;
  if(indx != info->breakindx){
    NATreeBaseNode* child = tree->config->childGetter(parent, indx);
    if(naIsNodeChildLeaf(parent, indx)){
      // We found the next leaf. Good ending
      return (NATreeLeaf*)child;
    }else{
      // We have to go deeper.
      return naIterateTreeCapture(tree, (NATreeNode*)child, info->startindx, info);
    }
  }
  return NA_NULL;
}



// This function takes a given basenode and bubbles up to its parent. This
// function works recursively until either a parent offers a next leaf or
// there are no more parents.
NA_HDEF NATreeLeaf* naIterateTreeBubble(const NATree* tree, NATreeBaseNode* curnode, NATreeIterationInfo* info){
  NAInt indx;
  NATreeLeaf* leaf;
  if(!curnode->parent){
    // We reached the root with no further options. Iteration is over.
    return NA_NULL;
  }
  indx = tree->config->childIndexGetter(curnode->parent, curnode);

  leaf = naIterateTreeCapture(tree, curnode->parent, indx, info);
  if(leaf){
    return leaf;
  }else{
    // If non more childs are available, bubble further.
    return naIterateTreeBubble(tree, (NATreeBaseNode*)(curnode->parent), info);
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  NATreeLeaf* leaf;
  const NATree* tree = (const NATree*)naGetPtrConst(iter->tree);

  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("naIterateTree", "Current iterator node is set although no root available");
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
    // Otherwise, we use the current leaf and bubble
    leaf = naIterateTreeBubble(tree, (NATreeBaseNode*)(iter->leaf), info);
  }
  #ifndef NDEBUG
    if(leaf && !naIsBaseNodeLeaf(tree, (NATreeBaseNode*)leaf))
      naError("naIterateTreeWithInfo", "Result should be a leaf");
  #endif
  naSetTreeIteratorCurLeaf(iter, leaf);
  return (leaf != NA_NULL);
}



NA_HDEF NATreeLeaf* naLocateTreeLeaf(NATreeIterator* iter, const void* key, NABool* matchfound, NABool usebubble){
  const NATree* tree;
  NATreeNode* topnode;
  NATreeLeaf* retnode;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
  #endif

  tree = (const NATree*)naGetPtrConst(iter->tree);

  if(!tree->root){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    *matchfound = NA_FALSE;
    return NA_FALSE;
  }

  topnode = NA_NULL;

  // Move the iterator to the topmost node which contains the given key.
  if(usebubble && !naIsTreeAtInitial(iter)){
    topnode = tree->config->bubbleLocator(tree, iter->leaf, key);
  }

  // Search for the leaf containing key.
  retnode = tree->config->captureLocator(tree, topnode, key, matchfound);
  #ifndef NDEBUG
    if(!naIsBaseNodeLeaf(tree, (NATreeBaseNode*)retnode))
    {
      naError("naLocateTreeLeaf", "Result should be a leaf");
      }
  #endif
  return retnode;
}



NA_HDEF NATreeLeaf* naLocateTreeTokenLeaf(NATreeIterator* iter, void* token, NATreeNodeTokenSearcher nodeSearcher, NATreeLeafTokenSearcher leafSearcher, NABool* matchfound){
  NATreeBaseNode* basenode;
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

  basenode = (NATreeBaseNode*)iter->leaf;
  if(!basenode){basenode = tree->root;}
  while(basenode){
    NAInt nextindx;
    if(naIsBaseNodeLeaf(tree, basenode)){
      NAPtr data = tree->config->leafDataGetter((NATreeLeaf*)basenode);
      leafSearcher(token, data, matchfound);
      return (NATreeLeaf*)basenode;
    }else{
      NAPtr data = tree->config->nodeDataGetter((NATreeNode*)basenode);
      NABool continuesearch = nodeSearcher(token, data, &nextindx);
      if(!continuesearch){return NA_NULL;}
    }
    if(nextindx == -1){
      basenode = (NATreeBaseNode*)(basenode->parent);
    }else{
      basenode = tree->config->childGetter((NATreeNode*)basenode, nextindx);
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
    parent = ((NATreeBaseNode*)contentleaf)->parent;
    naUpdateTreeNodeBubbling(tree, parent, tree->config->childIndexGetter(parent, (NATreeBaseNode*)contentleaf));
  }else{
    #ifndef NDEBUG
      if(tree->root)
        naError("naAddTreeContentAtLeaf", "leaf is null but there is a root");
    #endif
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    contentleaf = tree->config->leafCoreConstructor(tree, key, content);
    tree->root = (NATreeBaseNode*)contentleaf;
    ((NATreeBaseNode*)contentleaf)->parent = NA_NULL;
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
    NATreeBaseNode* child = tree->config->childGetter(node, i);
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
  if(bubble && ((NATreeBaseNode*)parent)->parent){
    naUpdateTreeNodeBubbling(tree, ((NATreeBaseNode*)parent)->parent, tree->config->childIndexGetter(((NATreeBaseNode*)parent)->parent, (NATreeBaseNode*)parent));
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
