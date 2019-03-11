
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


NA_IDEF NATree* naInitTree(NATree* tree, NATreeConfiguration* config){
  tree->config = naRetainTreeConfiguration(config);

  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_OCTTREE) && !tree->config->coredata)
      naError("Octtree configuration needs more information. Use naSetTreeConfigurationOcttreeBaseLeafExponent");
  #endif

  // If the config has a callback for constructing a tree, call it.
  if(tree->config->treeConstructor){
    tree->config->treeConstructor(tree->config->data);
  }

  // Init the tree root.
  tree->root = NA_NULL;
  tree->flags = 0;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif

  return tree;
}



NA_IDEF void naEmptyTree(NATree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("There are still iterators running on this tree. Did you miss a naClearTreeIterator call?");
  #endif
  if(tree->root){
    if(naIsTreeRootLeaf(tree)){
      naDestructTreeLeaf(tree, (NATreeLeaf*)tree->root);
    }else{
      naDestructTreeNode(tree, (NATreeNode*)tree->root, NA_TRUE);
    }
  }
  tree->root = NA_NULL;
}



NA_IDEF void naClearTree(NATree* tree){
  naEmptyTree(tree);
  // If the config has a callback function for deleting a tree, call it.
  if(tree->config->treeDestructor){
    tree->config->treeDestructor(tree->config->data);
  }
  naReleaseTreeConfiguration(tree->config);
}



NA_IDEF NABool naIsTreeEmpty(const NATree* tree){
  return (tree->root == NA_NULL);
}



NA_IDEF NABool naAddTreeFirstConst(NATree* tree, const void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreeNextConst(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeFirstMutable(NATree* tree, void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreeNextMutable(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeLastConst(NATree* tree, const void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreePrevConst(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF NABool naAddTreeLastMutable(NATree* tree, void* content){
  NATreeIterator iter;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("This function should not be called on trees with keys");
  #endif
  iter = naMakeTreeModifier(tree);
  naAddTreePrevMutable(&iter, content, NA_FALSE);
  naClearTreeIterator(&iter);
  return NA_TRUE;
}



NA_IDEF const void* naGetTreeFirstConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurLeafConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeFirstMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurLeafMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF const void* naGetTreeLastConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurLeafConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeLastMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurLeafMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void naUpdateTree(NATree* tree){
  #ifndef NDEBUG
    if(!tree->config->nodeUpdater)
      naError("tree is configured without nodeUpdater callback");
  #endif
  if(tree->root && !naIsTreeRootLeaf(tree)){
    naUpdateTreeNodeCapturing(tree, (NATreeNode*)tree->root);
  }
}



NA_HIDEF NABool naIsTreeRootLeaf(const NATree* tree){
  return (NABool)((tree->flags & NA_TREE_FLAG_ROOT_IS_LEAF) == NA_TREE_FLAG_ROOT_IS_LEAF);
}



NA_HIDEF void naMarkTreeRootLeaf(NATree* tree, NABool isleaf){
  tree->flags &= ~NA_TREE_FLAG_ROOT_IS_LEAF;
  tree->flags |= (isleaf * NA_TREE_FLAG_ROOT_IS_LEAF);
}


// todo: If this shows up in performance, adding rootparent again? Or adding childindx as flag in every item?
NA_HIDEF NABool naIsTreeItemLeaf(const NATree* tree, NATreeItem* item){
  NABool retvalue;
  if(naIsTreeItemRoot(tree, item)){
    retvalue = naIsTreeRootLeaf(tree);
  }else{
    NATreeNode* parent = naGetTreeItemParent(item);
    NAInt childindx = naGetTreeNodeChildIndex(tree, parent, item);
    retvalue = naIsNodeChildLeaf(parent, childindx);
  }
  return retvalue;
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
