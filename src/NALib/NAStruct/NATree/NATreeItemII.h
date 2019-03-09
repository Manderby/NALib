
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



NA_HIDEF NABool naIsTreeItemRoot(const NATree* tree, const NATreeItem* item){
  NA_UNUSED(tree);
  return (naGetTreeItemParent(item) == NA_NULL);
}



// /////////////////////////////////////
// Node
// /////////////////////////////////////

NA_HIDEF void naInitTreeNode(NATreeNode* node){
  naInitTreeItem(&(node->item));
  node->flags = 0;
}



NA_HIDEF void naClearTreeNode(NATreeNode* node){
  naClearTreeItem(&(node->item));
}



NA_HIDEF NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childindx){
  return (NABool)((node->flags >> childindx) & 0x01);
}



NA_HIDEF void naMarkNodeChildLeaf(NATreeNode* node, NAInt childindx, NABool isleaf){
  node->flags &= ~(1 << childindx);
  node->flags |= (NAInt)isleaf << childindx;
}



NA_HIDEF const void* naGetTreeNodeKey(const NATree* tree, NATreeNode* node){
  return ((const char*)node) + tree->config->nodeKeyOffset; 
}



NA_HIDEF NAPtr naGetTreeNodeData(const NATree* tree, NATreeNode* node){
  return *(NAPtr*)(((char*)node) + tree->config->nodeDataOffset); 
}


// /////////////////////////////////////
// Leaf
// /////////////////////////////////////

NA_HIDEF void naInitTreeLeaf(NATreeLeaf* leaf){
  naInitTreeItem(&(leaf->item));
}



NA_HIDEF NAPtr naConstructLeafData(NATree* tree, const void* key, NAPtr data){
  if(tree->config->leafDataConstructor){
    return tree->config->leafDataConstructor(key, tree->config->data, data);
  }else{
    return data;
  }
}



NA_HIDEF void naDestructLeafData(const NATree* tree, NAPtr data){
  if(tree->config->leafDataDestructor){
    tree->config->leafDataDestructor(data, tree->config->data);
  }
}



NA_HIDEF void naClearTreeLeaf(NATreeLeaf* leaf){
  naClearTreeItem(&(leaf->item));
}



NA_HIDEF const void* naGetTreeLeafKey(const NATree* tree, NATreeLeaf* leaf){
  return ((const char*)leaf) + tree->config->leafKeyOffset; 
}



NA_HIDEF NAPtr naGetTreeLeafData(const NATree* tree, NATreeLeaf* leaf){
  return *(NAPtr*)(((char*)leaf) + tree->config->leafDataOffset); 
}



NA_HIDEF void naSetTreeLeafData(const NATree* tree, NATreeLeaf* leaf, NAPtr newcontent){
  *(NAPtr*)(((char*)leaf) + tree->config->leafDataOffset) = newcontent; 
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
