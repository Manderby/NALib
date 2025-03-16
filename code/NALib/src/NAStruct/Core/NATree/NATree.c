
#include "../../NATree.h"



#if NA_DEBUG
  NATreeNode na_invalidParentNode;
#endif


// Every Add resulting in a change in the tree must go through this function.
NA_HDEF NATreeLeaf* na_AddTreeContentInPlace(NATree* tree, NATreeItem* item, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder) {
  // We need to create a node holding both the old leaf and the new one.
  NATreeLeaf* contentLeaf = tree->config->leafInserter(tree, item, key, content, insertOrder);
  NATreeNode* parent = na_GetTreeItemParent(&(contentLeaf->item));
  if(parent) {
    na_UpdateTreeNodeBubbling(tree, parent, na_GetTreeNodeChildIndex(parent, &(contentLeaf->item), tree->config));
  }
  return contentLeaf;
}



NA_HDEF void na_FillTreeNodeChildData(const NATreeConfiguration* config, NAPtr childdata[NA_TREE_NODE_MAX_CHILDS], NATreeNode* node) {
  for(size_t i = 0; i < config->abi.childPerNode; ++i) {
    NATreeItem* child = na_GetTreeNodeChild(node, i, config);
    if(!child) {
      childdata[i] = naMakePtrNull();
    }else if(na_GetNodeChildIsLeaf(node, i, config)) {
      childdata[i] = na_GetTreeLeafData((NATreeLeaf*)child, config);
    }else{
      childdata[i] = na_GetTreeNodeData((NATreeNode*)child, config);
    }
  }
}



// Expects the parent node of a child which has changed. The index indicates
// which child of the parent node caused the trouble.
// If NA_TREE_UNSPECIFIED_INDEX is given, there is no particular child being
// responsible for the change.
NA_HDEF void na_UpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, size_t responsibleChildIndex) {
  NABool bubble = NA_TRUE;
  
  while(bubble && parent) {
    // We call the update callback.
    if(tree->config->nodeUpdater) {
      NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
      na_FillTreeNodeChildData(tree->config, childdata, parent);
      bubble = tree->config->nodeUpdater(na_GetTreeNodeData(parent, tree->config), childdata, responsibleChildIndex, parent->flags & NA_TREE_NODE_CHILDS_MASK);
    }

    // Then we propagate the message towards the root if requested.
    if(bubble && !na_GetTreeItemIsRoot(&(parent->item))) {
      NATreeNode* grandparent = na_GetTreeItemParent(&(parent->item));
      responsibleChildIndex = na_GetTreeNodeChildIndex(grandparent, &(parent->item), tree->config);
      parent = grandparent;
    }else{
      parent = NA_NULL;
    }
  }
}




// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the childchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool na_UpdateTreeNodeCapturing(NATree* tree, NATreeNode* node) {
  NABool bubble;
  #if NA_DEBUG
    if(!tree->config->nodeUpdater)
      naCrash("tree is configured without nodeUpdater callback");
  #endif
  bubble = NA_FALSE;

  // Go through all childs and call the capturing message recursively.
  for(size_t i = 0; i < tree->config->abi.childPerNode; ++i) {
    NATreeNode* subnode = (NATreeNode*)na_GetTreeNodeChild(node, i, tree->config);
    if(subnode) {
      if(na_GetNodeChildIsLeaf(node, i, tree->config)) {
        bubble |= NA_TRUE;
      }else{
        // this node stores subnodes
        bubble |= na_UpdateTreeNodeCapturing(tree, subnode);
      }
    }
  }
  
  // Only if at least one of the childs requested further bubbling, we update
  // this very node.
  if(bubble) {
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    na_FillTreeNodeChildData(tree->config, childdata, node);
    bubble = tree->config->nodeUpdater(na_GetTreeNodeData(node, tree->config), childdata, NA_TREE_UNSPECIFIED_INDEX, node->flags & NA_TREE_NODE_CHILDS_MASK);
  }

  return bubble;
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
