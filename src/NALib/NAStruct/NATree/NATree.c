
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"




// Every Add resulting in a change in the tree must go through this function.
NA_HDEF NATreeLeaf* naAddTreeContentInPlace(NATree* tree, NATreeItem* item, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  // We need to create a node holding both the old leaf and the new one.
  NATreeLeaf* contentleaf = tree->config->leafInserter(tree, item, key, content, insertOrder);
  NATreeNode* parent = naGetTreeItemParent(&(contentleaf->item));
  if(parent){
    naUpdateTreeNodeBubbling(tree, parent, naGetTreeNodeChildIndex(tree, parent, &(contentleaf->item)));
  }
  return contentleaf;
}



NA_HDEF void naFillTreeNodeChildData(NATree* tree, NAPtr childdata[NA_TREE_NODE_MAX_CHILDS], NATreeNode* node){
  NAInt i;
  for(i=0; i<tree->config->childpernode; i++){
    NATreeItem* child = naGetTreeNodeChild(tree, node, i);
    if(naIsNodeChildLeaf(node, i)){
      childdata[i] = naGetTreeLeafData(tree, (NATreeLeaf*)child);
    }else{
      childdata[i] = naGetTreeNodeData(tree, (NATreeNode*)child);
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
    bubble = tree->config->nodeUpdater(naGetTreeNodeData(tree, parent), childdata, childindx, parent->flags & NA_TREE_CHILDS_MASK);
  }

  // Then we propagate the message towards the root if requested.
  if(bubble && !naIsTreeItemRoot(tree, &(parent->item))){
    NATreeNode* grandparent = naGetTreeItemParent(&(parent->item));
    naUpdateTreeNodeBubbling(tree, grandparent, naGetTreeNodeChildIndex(tree, grandparent, &(parent->item)));
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
      bubble |= naUpdateTreeNodeCapturing(tree, (NATreeNode*)naGetTreeNodeChild(tree, node, i));
    }
  }
  
  // Only if at least one of the childs requested further bubbling, we update
  // this very node.
  if(bubble){
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    naFillTreeNodeChildData(tree, childdata, node);
    bubble = tree->config->nodeUpdater(naGetTreeNodeData(tree, node), childdata, -1, node->flags & NA_TREE_CHILDS_MASK);
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
