
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"



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
  if(!curnode->parent){
    // We reached the root with no further options. Iteration is over.
    return NA_NULL;
  }
  NAInt indx = tree->config->childIndexGetter(curnode->parent, curnode);

  NATreeLeaf* leaf = naIterateTreeCapture(tree, curnode->parent, indx, info);
  if(leaf){
    return leaf;
  }else{
    // If non more childs are available, bubble further.
    return naIterateTreeBubble(tree, (NATreeBaseNode*)(curnode->parent), info);
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));

  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }

  NATreeLeaf* leaf;
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
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
  #endif

  tree = (const NATree*)naGetPtrConst(&(iter->tree));

  if(!tree->root){
    #ifndef NDEBUG
      if(!naIsTreeAtInitial(iter))
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    *matchfound = NA_FALSE;
    return NA_FALSE;
  }

  NATreeNode* topnode = NA_NULL;

  // Move the iterator to the topmost node which contains the given key.
  if(usebubble && !naIsTreeAtInitial(iter)){
    topnode = tree->config->bubbleLocator(tree, iter->leaf, key);
  }

  // Search for the leaf containing key.
  NATreeLeaf* retnode = tree->config->captureLocator(tree, topnode, key, matchfound);
  #ifndef NDEBUG
    if(!naIsBaseNodeLeaf(tree, (NATreeBaseNode*)retnode))
    {
      naError("naLocateTreeLeaf", "Result should be a leaf");
      }
  #endif
  return retnode;
}



NA_HDEF NATreeLeaf* naLocateTreeTokenLeaf(NATreeIterator* iter, void* token, NABool* matchfound){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTreeTokenLeaf", "This iterator has been cleared. You need to make it anew.");
    if(!tree->config->nodeTokenSearcher)
      naError("naLocateTreeTokenLeaf", "Tree configuration is missing a node token searcher.");
    if(!tree->config->leafTokenSearcher)
      naError("naLocateTreeTokenLeaf", "Tree configuration is missing a leaf token searcher.");
  #endif
  if(naIsTreeEmpty(tree)){
    naSetTreeIteratorCurLeaf(iter, NA_NULL);
    *matchfound = NA_FALSE;
    return NA_FALSE;
  }

  NATreeBaseNode* basenode = (NATreeBaseNode*)iter->leaf;
  while(NA_TRUE){
    NAInt nextindx;
    if(!basenode){basenode = tree->root;}
    if(naIsBaseNodeLeaf(tree, basenode)){
      NAPtr* data = tree->config->leafDataGetter((NATreeLeaf*)basenode);
      NABool continuesearch = tree->config->leafTokenSearcher(token, *data, matchfound);
      if(!continuesearch){return (NATreeLeaf*)basenode;}
      nextindx = -1;
    }else{
      NAPtr* data = tree->config->nodeDataGetter((NATreeNode*)basenode);
      NABool continuesearch = tree->config->nodeTokenSearcher(token, *data, &nextindx);
      if(!continuesearch){return NA_NULL;}
    }
    if(nextindx == -1){
      basenode = (NATreeBaseNode*)(basenode->parent);
    }else{
      basenode = tree->config->childGetter((NATreeNode*)basenode, nextindx);
    }
  }
}



// Every Add resulting in a change in the tree must go through this function.
NA_HDEF NATreeLeaf* naAddTreeContentAtLeaf(NATree* tree, NATreeLeaf* leaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* contentleaf;
  if(leaf){
    // We need to create a node holding both the old leaf and the new one.
    contentleaf = tree->config->leafInserter(tree, leaf, key, content, insertOrder);
    NATreeNode* parent = ((NATreeBaseNode*)contentleaf)->parent;
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
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naAddTreeLeaf", "This iterator has been cleared. You need to make it anew.");
  #endif
  // We do not use bubbling when inserting as there is almost never a benefit
  // from it. Even worse, it performs mostly worse.
  NATreeLeaf* leaf = naLocateTreeLeaf(iter, key, &matchfound, NA_FALSE);
  NATree* tree = (NATree*)naGetPtrMutable(&(iter->tree));

  if(matchfound && !replace){return NA_FALSE;}

  NATreeLeafInsertOrder insertOrder = matchfound ? NA_TREE_LEAF_INSERT_ORDER_REPLACE : NA_TREE_LEAF_INSERT_ORDER_KEY;
  NATreeLeaf* contentleaf = naAddTreeContentAtLeaf(tree, leaf, key, content, insertOrder);
  naSetTreeIteratorCurLeaf(iter, contentleaf);
  return NA_TRUE;
}



NA_HDEF void naFillTreeNodeChildData(NATree* tree, NAPtr childdata[NA_TREE_NODE_MAX_CHILDS], NATreeNode* node){
  for(NAInt i=0; i<tree->config->childpernode; i++){
    NATreeBaseNode* child = tree->config->childGetter(node, i);
    if(naIsNodeChildLeaf(node, i)){
      childdata[i] = *(tree->config->leafDataGetter((NATreeLeaf*)child));
    }else{
      childdata[i] = *(tree->config->nodeDataGetter((NATreeNode*)child));
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
  if(tree->config->nodeChildUpdater){
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    naFillTreeNodeChildData(tree, childdata, parent);
    bubble = tree->config->nodeChildUpdater(*(tree->config->nodeDataGetter(parent)), childdata, childindx, parent->flags & NA_TREE_CHILDS_MASK);
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
  NABool bubble = NA_FALSE;

  // this node stores subnodes
  for(NAInt i=0; i<tree->config->childpernode; i++){
    if(!naIsNodeChildLeaf(node, i)){
      bubble |= naUpdateTreeNodeCapturing(tree, (NATreeNode*)tree->config->childGetter(node, i));
    }
  }
  if(tree->config->nodeChildUpdater){
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    naFillTreeNodeChildData(tree, childdata, node);
    bubble |= tree->config->nodeChildUpdater(*(tree->config->nodeDataGetter(node)), childdata, -1, node->flags & NA_TREE_CHILDS_MASK);
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
