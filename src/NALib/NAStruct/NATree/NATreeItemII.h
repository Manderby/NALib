
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// /////////////////////////////////////
// Item
// /////////////////////////////////////

NA_HIDEF void naInitTreeItem(NATreeItem* item){
  // We do not initialize the parent. Code must use naSetTreeItemParent.
  #ifndef NDEBUG
    item->itercount = 0;
  #else
    NA_UNUSED(item);
  #endif
}



NA_HIDEF void naClearTreeItem(NATreeItem* item){
  #ifndef NDEBUG
    if(item->itercount)
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



NA_HIDEF NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childindx){
  return (NABool)((node->flags >> childindx) & 0x01);
}



NA_HIDEF void naMarkNodeChildLeaf(NATreeNode* node, NAInt childindx, NABool isleaf){
  node->flags &= ~(1 << childindx);
  node->flags |= (NAInt)isleaf << childindx;
}



NA_HIDEF void* naGetTreeNodeKey(const NATreeConfiguration* config, NATreeNode* node){
  // We thank the power of pointer arithmetic!
  return ((char*)node) + config->nodeKeyOffset; 
}



NA_HIDEF NAPtr naGetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node){
  // We thank the power of pointer arithmetic!
  return *(NAPtr*)(((char*)node) + config->nodeUserDataOffset); 
}



NA_HIDEF void naSetTreeNodeData(const NATreeConfiguration* config, NATreeNode* node, NAPtr newdata){
  // We thank the power of pointer arithmetic!
  *(NAPtr*)(((char*)node) + config->nodeUserDataOffset) = newdata; 
}



NA_HIDEF NATreeItem** naGetTreeNodeChildStorage(NATreeNode* parent){
  // We thank the power of pointer arithmetic!
  return (NATreeItem**)(((char*)parent) + NA_TREE_NOTE_CHILDS_OFFSET);
  // Note that this computation has originally been done using an integer
  // stored in config. But as this is used so often and needs to be
  // available at all times, it simply had been defined that the storage for
  // the children must come immediately after the storage of the node.
}



NA_HIDEF NATreeItem* naGetTreeNodeChild(const NATreeConfiguration* config, NATreeNode* parent, NAInt childindx){
  #ifndef NDEBUG
    if(childindx < 0)
      naError("childindx must be >= 0");
    if(childindx >= config->childpernode)
      naError("childindx out of bounds");
  #else
    NA_UNUSED(config);
  #endif
  // We thank the power of pointer arithmetic!
  return naGetTreeNodeChildStorage(parent)[childindx]; 
}



NA_HIDEF void naSetTreeNodeChildEmpty(NATreeNode* parent, NAInt childindx){
  // We thank the power of pointer arithmetic!
//  #ifndef NDEBUG
//    if(!(naGetTreeNodeChildStorage(parent)[childindx]))
//      naError("Child was not existent before.");
//  #endif
  naGetTreeNodeChildStorage(parent)[childindx] = NA_NULL;
}



NA_HIDEF void naSetTreeNodeChild(NATreeNode* parent, NATreeItem* child, NAInt childindx, NABool isChildLeaf){
  // We thank the power of pointer arithmetic!
//  #ifndef NDEBUG
//    if((naGetTreeNodeChildStorage(parent)[childindx]))
//      naError("Duplicate child assign.");
//  #endif
  naSetTreeItemParent(child, parent);
  naMarkNodeChildLeaf(parent, childindx, isChildLeaf);
  naGetTreeNodeChildStorage(parent)[childindx] = child;
}



NA_HDEF NAInt naGetTreeNodeChildIndex(const NATreeConfiguration* config, NATreeNode* parent, NATreeItem* child){
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
