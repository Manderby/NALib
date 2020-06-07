
// /////////////////////////////////////
// Item
// /////////////////////////////////////

NA_HIDEF void naInitTreeItem(NATreeItem* item){
  // We do not initialize the parent. Code must use naSetTreeItemParent.
  #ifndef NDEBUG
    item->iterCount = 0;
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF void naClearTreeItem(NATreeItem* item){
  #ifndef NDEBUG
    if(item->iterCount)
      naError("There are still iterators running on this item. Did you forget a call to naClearTreeIterator?");
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF NATreeNode* naGetTreeItemParent(const NATreeItem* item){
  return item->parent;
}



NA_HIDEF void naSetTreeItemParent(NATreeItem* item, NATreeNode* parent){
  item->parent = parent;
}



NA_HIDEF NABool naIsTreeItemRoot(const NATreeItem* item){
  return (naGetTreeItemParent(item) == NA_NULL);
}



// /////////////////////////////////////
// Node
// /////////////////////////////////////

NA_HIDEF NATreeItem* naGetTreeNodeItem(NATreeNode* node){
  return &(node->item);
}



NA_HIDEF void naInitTreeNode(const NATreeConfiguration* config, NATreeNode* node, const void* key){
  NAInt i;
  naInitTreeItem(naGetTreeNodeItem(node));
  node->flags = 0;

  if(config->keyAssigner){
    config->keyAssigner(naGetTreeNodeKey(config, node), key);
  }

  for(i = 0; i < config->childpernode; i++){
    naSetTreeNodeChildEmpty(node, i);
  }

  if(config->nodeDataConstructor){
    naSetTreeNodeData(config, node, config->nodeDataConstructor(&key));
  }else{
    naSetTreeNodeData(config, node, naMakePtrNull());
  }
}



NA_HIDEF void naClearTreeNode(NATreeNode* node){
  naClearTreeItem(naGetTreeNodeItem(node));
}



NA_HIDEF void naDestructNodeData(const NATreeConfiguration* config, NAPtr data){
  if(config->nodeDataDestructor){
    config->nodeDataDestructor(data);
  }
}



NA_HIDEF void naDestructTreeNode(const NATreeConfiguration* config, NATreeNode* node, NABool recursive){
  #ifndef NDEBUG
    if(!node)
      naCrash("node shall not be Null");
  #endif
  
  if(recursive){
    NAInt i;
    for(i = 0; i < config->childpernode; i++){
      NATreeItem* child = naGetTreeNodeChild(config, node, i);
      if(child){
        if(naIsNodeChildLeaf(node, i)){
          naDestructTreeLeaf(config, (NATreeLeaf*)child);
        }else{
          naDestructTreeNode(config, (NATreeNode*)child, NA_TRUE);
        }
      }
    }
  }

  naDestructNodeData(config, naGetTreeNodeData(config, node));
  naClearTreeNode(node);
  config->nodeDestructor(node);
}



NA_HIDEF NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childIndex){
  #ifndef NDEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  return (NABool)((node->flags >> childIndex) & 0x01);
}



NA_HIDEF void naMarkNodeChildLeaf(NATreeNode* node, NAInt childIndex, NABool isleaf){
  #ifndef NDEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  node->flags &= ~(1 << childIndex);
  node->flags |= (NAInt)isleaf << childIndex;
}



NA_HIDEF void* naGetTreeNodeKey(const NATreeConfiguration* config, NATreeNode* node){
  #ifndef NDEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  return ((char*)node) + config->nodeKeyOffset; 
}



NA_HIDEF NAPtr naGetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node){
  #ifndef NDEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((char*)node) + config->nodeUserDataOffset); 
}



NA_HIDEF void naSetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node, NAPtr newdata){
  #ifndef NDEBUG
    if(!node)
      naCrash("Node is Null Pointer.");
  #endif
  // We thank the power of pointer arithmetic!
  *(NAPtr*)(((char*)node) + config->nodeUserDataOffset) = newdata; 
}



NA_HIDEF NATreeItem** naGetTreeNodeChildStorage(NATreeNode* parent){
  #ifndef NDEBUG
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



NA_HIDEF NATreeItem* naGetTreeNodeChild(const NATreeConfiguration* config, NATreeNode* parent, NAInt childIndex){
  #ifndef NDEBUG
    if(childIndex < 0)
      naError("childIndex must be >= 0");
    if(childIndex >= config->childpernode)
      naError("childIndex out of bounds");
  #else
    NA_UNUSED(config);
  #endif
  // We thank the power of pointer arithmetic!
  return naGetTreeNodeChildStorage(parent)[childIndex]; 
}



NA_HIDEF void naSetTreeNodeChildEmpty(NATreeNode* parent, NAInt childIndex){
  // We thank the power of pointer arithmetic!
//  #ifndef NDEBUG
//    if(!(naGetTreeNodeChildStorage(parent)[childIndex]))
//      naError("Child was not existent before.");
//  #endif
  naGetTreeNodeChildStorage(parent)[childIndex] = NA_NULL;
}



NA_HIDEF void naSetTreeNodeChild(NATreeNode* parent, NATreeItem* child, NAInt childIndex, NABool isChildLeaf){
  // We thank the power of pointer arithmetic!
//  #ifndef NDEBUG
//    if((naGetTreeNodeChildStorage(parent)[childIndex]))
//      naError("Duplicate child assign.");
//  #endif
  naSetTreeItemParent(child, parent);
  naMarkNodeChildLeaf(parent, childIndex, isChildLeaf);
  naGetTreeNodeChildStorage(parent)[childIndex] = child;
}



NA_HHDEF NAInt na_GetTreeNodeChildIndex(const NATreeConfiguration* config, NATreeNode* parent, NATreeItem* child){
  NAInt retvalue;
  NATreeItem** childs;

  #ifndef NDEBUG
    if(!child)
      naError("Child should not be Null");
  #endif
  
  // We thank the power of pointer arithmetic!
  childs = naGetTreeNodeChildStorage(parent);
  
  // We assume, the child MUST be a child of this parent. Therefore we only
  // search until childpernode - 1 and just return the last possibility in
  // good belief.
  for(retvalue = 0; retvalue < config->childpernode - 1; retvalue++){
    if(childs[retvalue] == child){break;}
  }
  
  #ifndef NDEBUG
    if(childs[retvalue] != child)
      naError("returned childindex does not correspond to the desired child");
  #endif
  return retvalue;
}



// /////////////////////////////////////
// Leaf
// /////////////////////////////////////

NA_HIDEF NATreeItem* naGetTreeLeafItem(NATreeLeaf* leaf){
  return &(leaf->item);
}



NA_HIDEF void naInitTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf, const void* key, NAPtr content){
  naInitTreeItem(naGetTreeLeafItem(leaf));
  
  if(config->keyAssigner){
    config->keyAssigner(naGetTreeLeafKey(config, leaf), key);
  }
  naSetTreeLeafData(config, leaf, naConstructLeafData(config, key, content));
}



NA_HIDEF void naClearTreeLeaf(NATreeLeaf* leaf){
  naClearTreeItem(naGetTreeLeafItem(leaf));
}



NA_HIDEF void naDestructLeafData(const NATreeConfiguration* config, NAPtr data){
  if(config->leafDataDestructor){
    config->leafDataDestructor(data);
  }
}



NA_HIDEF void naDestructTreeLeaf(const NATreeConfiguration* config, NATreeLeaf* leaf){
  #ifndef NDEBUG
    if(!leaf)
      naCrash("leaf shall not be Null");
  #endif
  naDestructLeafData(config, naGetTreeLeafData(config, leaf));
  naClearTreeLeaf(leaf);
  config->leafDestructor(leaf);
}



NA_HIDEF NAPtr naConstructLeafData(const NATreeConfiguration* config, const void* key, NAPtr content){
  if(config->leafDataConstructor){
    return config->leafDataConstructor(key, content);
  }else{
    // If there is no constructor, we plainly store the given content.
    return content;
  }
}



NA_HIDEF void* naGetTreeLeafKey(const NATreeConfiguration* config, NATreeLeaf* leaf){
  return ((char*)leaf) + config->leafKeyOffset; 
}



NA_HIDEF NAPtr naGetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf){
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((char*)leaf) + config->leafUserDataOffset); 
}



NA_HIDEF void naSetTreeLeafData(const NATreeConfiguration* config, NATreeLeaf* leaf, NAPtr newcontent){
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
