
// /////////////////////////////////////
// Item
// /////////////////////////////////////



#if NA_DEBUG
  NATreeNode na_invalidParentNode;
#endif



NA_HIDEF void na_InitTreeItem(NATreeItem* item){
  // We do not initialize the parent. Code must use na_SetTreeItemParent.
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
    item->iterCount = 0;
    item->parent = &na_invalidParentNode;
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF void na_ClearTreeItem(NATreeItem* item) {
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
    if(item->iterCount)
      naError("There are still iterators running on this item. Did you forget a call to naClearTreeIterator?");
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF NATreeNode* na_GetTreeItemParent(const NATreeItem* item){
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
    if(item->parent == &na_invalidParentNode)
      naError("Item has an invalid parent.");
  #endif
  return item->parent;
}



NA_HIDEF void na_SetTreeItemParent(NATreeItem* item, NATreeNode* parent){
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
  #endif
  item->parent = parent;
}



NA_HIDEF NABool na_GetTreeItemIsRoot(const NATreeItem* item){
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
  #endif
  return (na_GetTreeItemParent(item) == NA_NULL);
}



NA_HIDEF size_t na_GetTreeItemIterCount(const NATreeItem* item){
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
  #endif
  return item->iterCount;
}
NA_HIDEF void na_IncTreeItemIterCount(NATreeItem* item){
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
  #endif
  item->iterCount++;
}
NA_HIDEF void na_DecTreeItemIterCount(NATreeItem* item){
  #if NA_DEBUG
    if(!item)
      naCrash("item is nullptr");
    if(item->iterCount == 0)
      naError("Too many iterator decrements");
  #endif
  item->iterCount--;
}



// /////////////////////////////////////
// Node
// /////////////////////////////////////

NA_HIDEF NATreeItem* na_GetTreeNodeItem(NATreeNode* node){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
  #endif
  return &(node->item);
}



NA_HIDEF void* na_GetTreeNodeKey(NATreeNode* node, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(config->nodeKeyOffset == NA_TREE_CONFIG_INVALID_OFFSET)
      naError("config has an invalid node offset stored");
  #endif
  // We thank the power of pointer arithmetic!
  return ((NAByte*)node) + config->nodeKeyOffset; 
}



NA_HIDEF NATreeItem** na_GetTreeNodeChildStorage(NATreeNode* node){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
  #endif
  // We thank the power of pointer arithmetic!
  return (NATreeItem**)(((NAByte*)node) + NA_TREE_NODE_CHILDS_OFFSET);
  // Note that this computation has originally been done using an integer
  // stored in config. But as this is used so often and needs to be
  // available at all times, it simply had been defined that the storage for
  // the children must come immediately after the storage of the node.
}



NA_HIDEF NAPtr na_GetTreeNodeData(NATreeNode* node, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(config->nodeUserDataOffset == NA_TREE_CONFIG_INVALID_OFFSET)
      naError("config has an invalid node offset stored");
  #endif
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((NAByte*)node) + config->nodeUserDataOffset); 
}



NA_HIDEF void na_SetTreeNodeData(NATreeNode* node, NAPtr data, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(config->nodeUserDataOffset == NA_TREE_CONFIG_INVALID_OFFSET)
      naError("config has an invalid node offset stored");
  #endif
  // We thank the power of pointer arithmetic!
  *(NAPtr*)(((NAByte*)node) + config->nodeUserDataOffset) = data; 
}



NA_HIDEF NABool na_GetNodeChildIsLeaf(NATreeNode* node, NAInt childIndex, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(childIndex >= config->childPerNode)
      naCrash("childIndex out of bounds");
  #endif
  return naGetFlagu32(node->flags, 1 << childIndex);
}



NA_HIDEF void na_SetNodeChildIsLeaf(NATreeNode* node, NAInt childIndex, NABool isleaf, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(childIndex >= config->childPerNode)
      naCrash("childIndex out of bounds");
  #endif
  naSetFlagu32(&node->flags, 1 << childIndex, isleaf);
}



NA_HIDEF NATreeItem* na_GetTreeNodeChild(NATreeNode* node, size_t childIndex, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(childIndex >= config->childPerNode)
      naCrash("childIndex out of bounds");
  #else
    NA_UNUSED(config);
  #endif
  return na_GetTreeNodeChildStorage(node)[childIndex]; 
}



NA_HIDEF void na_SetTreeNodeChild(NATreeNode* node, NATreeItem* child, NAInt childIndex, NABool isChildLeaf, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!child)
      naCrash("child is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(childIndex >= config->childPerNode)
      naCrash("childIndex out of bounds");
  #endif
  na_SetTreeItemParent(child, node);
  na_SetNodeChildIsLeaf(node, childIndex, isChildLeaf, config);
  na_GetTreeNodeChildStorage(node)[childIndex] = child;
}



NA_HIDEF void na_InitTreeNode(NATreeNode* node, const void* key, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
  #endif
  
  na_InitTreeItem(na_GetTreeNodeItem(node));
  node->flags = 0;

  if(config->keyAssigner){
    config->keyAssigner(na_GetTreeNodeKey(node, config), key);
  }

  for(size_t i = 0; i < config->childPerNode; ++i){
    na_GetTreeNodeChildStorage(node)[i] = NA_NULL;
  }

  if(config->nodeDataConstructor){
    na_SetTreeNodeData(node, config->nodeDataConstructor(&key), config);
  }else{
    na_SetTreeNodeData(node, naMakePtrNull(), config);
  }
}



NA_HIDEF void na_ClearTreeNode(NATreeNode* node){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
  #endif
  na_ClearTreeItem(na_GetTreeNodeItem(node));
}



NA_HIDEF void na_DestructTreeNode(NATreeNode* node, NABool recursive, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
  #endif
  
  if(recursive){
    for(size_t i = 0; i < config->childPerNode; ++i){
      NATreeItem* child = na_GetTreeNodeChild(node, i, config);
      if(child){
        if(na_GetNodeChildIsLeaf(node, i, config)){
          na_DestructTreeLeaf(config, (NATreeLeaf*)child);
        }else{
          na_DestructTreeNode((NATreeNode*)child, NA_TRUE, config);
        }
      }
    }
  }

  if(config->nodeDataDestructor){
    config->nodeDataDestructor(na_GetTreeNodeData(node, config));
  }

  config->nodeDestructor(node);
}



NA_HDEF size_t na_GetTreeNodeChildIndex(NATreeNode* node, NATreeItem* child, const NATreeConfiguration* config){
  #if NA_DEBUG
    if(!node)
      naCrash("node is nullptr");
    if(!config)
      naCrash("config is nullptr");
    if(!child)
      naError("child should not be Null");
  #endif

  NATreeItem** childs = na_GetTreeNodeChildStorage(node);
  
  // We assume, the child MUST be a child of this parent. Therefore we only
  // search until childPerNode - 1 and just return the last possibility in
  // good belief.
  size_t retValue;
  for(retValue = 0; retValue < config->childPerNode - 1; retValue++){
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

NA_HIDEF NATreeItem* na_GetTreeLeafItem(NATreeLeaf* leaf) {
  return &leaf->item;
}



NA_HIDEF void na_InitTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf, const void* key, NAPtr content) {
  na_InitTreeItem(na_GetTreeLeafItem(leaf));
  
  if(config->keyAssigner) {
    config->keyAssigner(na_GetTreeLeafKey(config, leaf), key);
  }
  na_SetTreeLeafData(config, leaf, na_ConstructLeafData(config, key, content));
}



NA_HIDEF void na_ClearTreeLeaf(NATreeLeaf* leaf) {
  na_ClearTreeItem(na_GetTreeLeafItem(leaf));
}



NA_HIDEF void na_DestructLeafData(const NATreeConfiguration* config, NAPtr data) {
  if(config->leafDataDestructor) {
    config->leafDataDestructor(data);
  }
}



NA_HIDEF void na_DestructTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf) {
  #if NA_DEBUG
    if(!leaf)
      naCrash("leaf shall not be Null");
  #endif
  na_DestructLeafData(config, na_GetTreeLeafData(config, leaf));
  na_ClearTreeLeaf(leaf);
  config->leafDestructor(leaf);
}



NA_HIDEF NAPtr na_ConstructLeafData(const NATreeConfiguration* config, const void* key, NAPtr content) {
  if(config->leafDataConstructor) {
    return config->leafDataConstructor(key, content);
  }else{
    // If there is no constructor, we plainly store the given content.
    return content;
  }
}



NA_HIDEF void* na_GetTreeLeafKey(const NATreeConfiguration* config, NATreeLeaf* leaf){
  #if NA_DEBUG
    if(!config)
      naCrash("config is nullptr");
    if(!leaf)
      naCrash("leaf is nullptr");
    if(config->leafKeyOffset == NA_TREE_CONFIG_INVALID_OFFSET)
      naError("config has an invalid leaf offset stored");
  #endif
  // We thank the power of pointer arithmetic!
  return ((NAByte*)leaf) + config->leafKeyOffset; 
}



NA_HIDEF NAPtr na_GetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf){
  #if NA_DEBUG
    if(!config)
      naCrash("config is nullptr");
    if(!leaf)
      naCrash("leaf is nullptr");
    if(config->leafUserDataOffset == NA_TREE_CONFIG_INVALID_OFFSET)
      naError("config has an invalid leaf offset stored");
  #endif
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((NAByte*)leaf) + config->leafUserDataOffset); 
}



NA_HIDEF void na_SetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf, NAPtr newcontent){
  #if NA_DEBUG
    if(!config)
      naCrash("config is nullptr");
    if(!leaf)
      naCrash("leaf is nullptr");
    if(config->leafUserDataOffset == NA_TREE_CONFIG_INVALID_OFFSET)
      naError("config has an invalid leaf offset stored");
  #endif
  // We thank the power of pointer arithmetic!
  *(NAPtr*)(((NAByte*)leaf) + config->leafUserDataOffset) = newcontent; 
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
