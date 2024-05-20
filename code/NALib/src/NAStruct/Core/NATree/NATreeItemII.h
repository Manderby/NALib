
// /////////////////////////////////////
// Item
// /////////////////////////////////////

NA_HIDEF void na_InitTreeItem(NATreeItem* item){
  // We do not initialize the parent. Code must use na_SetTreeItemParent.
  #if NA_DEBUG
    item->iterCount = 0;
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF void na_ClearTreeItem(NATreeItem* item){
  #if NA_DEBUG
    if(item->iterCount)
      naError("There are still iterators running on this item. Did you forget a call to naClearTreeIterator?");
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF NATreeNode* na_GetTreeItemParent(const NATreeItem* item){
  return item->parent;
}



NA_HIDEF void na_SetTreeItemParent(NATreeItem* item, NATreeNode* parent){
  item->parent = parent;
}



NA_HIDEF NABool na_IsTreeItemRoot(const NATreeItem* item){
  return (na_GetTreeItemParent(item) == NA_NULL);
}



// /////////////////////////////////////
// Node
// /////////////////////////////////////

NA_HIDEF NATreeItem* na_GetTreeNodeItem(NATreeNode* node){
  return &(node->item);
}



NA_HIDEF void na_InitTreeNode(const NATreeConfiguration* config, NATreeNode* node, const void* key){
  NAInt i;
  na_InitTreeItem(na_GetTreeNodeItem(node));
  node->flags = 0;

  if(config->keyAssigner){
    config->keyAssigner(na_GetTreeNodeKey(config, node), key);
  }

  for(i = 0; i < config->childpernode; ++i){
    na_SetTreeNodeChildEmpty(node, i);
  }

  if(config->nodeDataConstructor){
    na_SetTreeNodeData(config, node, config->nodeDataConstructor(&key));
  }else{
    na_SetTreeNodeData(config, node, naMakePtrNull());
  }
}



NA_HIDEF void na_ClearTreeNode(NATreeNode* node){
  na_ClearTreeItem(na_GetTreeNodeItem(node));
}



NA_HIDEF void na_DestructNodeData(const NATreeConfiguration* config, NAPtr data){
  if(config->nodeDataDestructor){
    config->nodeDataDestructor(data);
  }
}



NA_HIDEF void na_DestructTreeNode(const NATreeConfiguration* config, NATreeNode* node, NABool recursive){
  #if NA_DEBUG
    if(!node)
      naCrash("node shall not be Null");
  #endif
  
  if(recursive){
    NAInt i;
    for(i = 0; i < config->childpernode; ++i){
      NATreeItem* child = na_GetTreeNodeChild(config, node, i);
      if(child){
        if(na_IsNodeChildLeaf(node, i)){
          na_DestructTreeLeaf(config, (NATreeLeaf*)child);
        }else{
          na_DestructTreeNode(config, (NATreeNode*)child, NA_TRUE);
        }
      }
    }
  }

  na_DestructNodeData(config, na_GetTreeNodeData(config, node));
  na_ClearTreeNode(node);
  config->nodeDestructor(node);
}



NA_HIDEF NABool na_IsNodeChildLeaf(NATreeNode* node, NAInt childIndex){
  #if NA_DEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  return (NABool)((node->flags >> childIndex) & 0x01);
}



NA_HIDEF void na_MarkNodeChildLeaf(NATreeNode* node, NAInt childIndex, NABool isleaf){
  #if NA_DEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  node->flags &= ~(1 << childIndex);
  node->flags |= (NAInt)isleaf << childIndex;
}



NA_HIDEF void* na_GetTreeNodeKey(const NATreeConfiguration* config, NATreeNode* node){
  #if NA_DEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  return ((char*)node) + config->nodeKeyOffset; 
}



NA_HIDEF NAPtr na_GetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node){
  #if NA_DEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((char*)node) + config->nodeUserDataOffset); 
}



NA_HIDEF void na_SetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node, NAPtr newData){
  #if NA_DEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  *(NAPtr*)(((char*)node) + config->nodeUserDataOffset) = newData; 
}



NA_HIDEF NATreeItem** na_GetTreeNodeChildStorage(NATreeNode* parent){
  #if NA_DEBUG
    if(!parent)
      naCrash("Parent is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  return (NATreeItem**)(((char*)parent) + NA_TREE_NOTE_CHILDS_OFFSET);
  // Note that this computation has originally been done using an integer
  // stored in config. But as this is used so often and needs to be
  // available at all times, it simply had been defined that the storage for
  // the children must come immediately after the storage of the node.
}



NA_HIDEF NATreeItem* na_GetTreeNodeChild(const NATreeConfiguration* config, NATreeNode* parent, NAInt childIndex){
  #if NA_DEBUG
    if(childIndex < 0)
      naError("childIndex must be >= 0");
    if(childIndex >= config->childpernode)
      naError("childIndex out of bounds");
  #else
    NA_UNUSED(config);
  #endif
  // We thank the power of pointer arithmetic!
  return na_GetTreeNodeChildStorage(parent)[childIndex]; 
}



NA_HIDEF void na_SetTreeNodeChildEmpty(NATreeNode* parent, NAInt childIndex){
  // We thank the power of pointer arithmetic!
//  #if NA_DEBUG
//    if(!(na_GetTreeNodeChildStorage(parent)[childIndex]))
//      naError("Child was not existent before.");
//  #endif
  na_GetTreeNodeChildStorage(parent)[childIndex] = NA_NULL;
}



NA_HIDEF void na_SetTreeNodeChild(NATreeNode* parent, NATreeItem* child, NAInt childIndex, NABool isChildLeaf){
  // We thank the power of pointer arithmetic!
//  #if NA_DEBUG
//    if((na_GetTreeNodeChildStorage(parent)[childIndex]))
//      naError("Duplicate child assign.");
//  #endif
  na_SetTreeItemParent(child, parent);
  na_MarkNodeChildLeaf(parent, childIndex, isChildLeaf);
  na_GetTreeNodeChildStorage(parent)[childIndex] = child;
}



NA_HDEF NAInt na_GetTreeNodeChildIndex(const NATreeConfiguration* config, NATreeNode* parent, NATreeItem* child){
  NAInt retValue;
  NATreeItem** childs;

  #if NA_DEBUG
    if(!child)
      naError("Child should not be Null");
  #endif
  
  // We thank the power of pointer arithmetic!
  childs = na_GetTreeNodeChildStorage(parent);
  
  // We assume, the child MUST be a child of this parent. Therefore we only
  // search until childpernode - 1 and just return the last possibility in
  // good belief.
  for(retValue = 0; retValue < config->childpernode - 1; retValue++){
    if(childs[retValue] == child)
      break;
  }
  
  #if NA_DEBUG
    if(childs[retValue] != child)
      naError("returned childindex does not correspond to the desired child");
  #endif
  return retValue;
}



// /////////////////////////////////////
// Leaf
// /////////////////////////////////////

NA_HIDEF NATreeItem* na_GetTreeLeafItem(NATreeLeaf* leaf){
  return &(leaf->item);
}



NA_HIDEF void na_InitTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf, const void* key, NAPtr content){
  na_InitTreeItem(na_GetTreeLeafItem(leaf));
  
  if(config->keyAssigner){
    config->keyAssigner(na_GetTreeLeafKey(config, leaf), key);
  }
  na_SetTreeLeafData(config, leaf, na_ConstructLeafData(config, key, content));
}



NA_HIDEF void na_ClearTreeLeaf(NATreeLeaf* leaf){
  na_ClearTreeItem(na_GetTreeLeafItem(leaf));
}



NA_HIDEF void na_DestructLeafData(const NATreeConfiguration* config, NAPtr data){
  if(config->leafDataDestructor){
    config->leafDataDestructor(data);
  }
}



NA_HIDEF void na_DestructTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf){
  #if NA_DEBUG
    if(!leaf)
      naCrash("leaf shall not be Null");
  #endif
  na_DestructLeafData(config, na_GetTreeLeafData(config, leaf));
  na_ClearTreeLeaf(leaf);
  config->leafDestructor(leaf);
}



NA_HIDEF NAPtr na_ConstructLeafData(const NATreeConfiguration* config, const void* key, NAPtr content){
  if(config->leafDataConstructor){
    return config->leafDataConstructor(key, content);
  }else{
    // If there is no constructor, we plainly store the given content.
    return content;
  }
}



NA_HIDEF void* na_GetTreeLeafKey(const NATreeConfiguration* config, NATreeLeaf* leaf){
  return ((char*)leaf) + config->leafKeyOffset; 
}



NA_HIDEF NAPtr na_GetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf){
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((char*)leaf) + config->leafUserDataOffset); 
}



NA_HIDEF void na_SetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf, NAPtr newcontent){
  // We thank the power of pointer arithmetic!
  *(NAPtr*)(((char*)leaf) + config->leafUserDataOffset) = newcontent; 
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
