
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations.
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"



#define NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE 0x8000

#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80

// Prototypes
NA_HAPI  NATreeLeaf* naLocateTreeLeaf(NATreeIterator* iter, const void* key, NABool* matchfound, NABool usebubble);
NA_HAPI  NATreeLeaf* naLocateTreeTokenLeaf(NATreeIterator* iter, void* token, NATreeNodeTokenSearcher nodeSearcher, NATreeLeafTokenSearcher leafSearcher, NABool* matchfound);
NA_HAPI  NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NATreeLeaf* naAddTreeContentAtLeaf(NATree* tree, NATreeLeaf* leaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI  NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace);
NA_HAPI  void naUpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, NAInt childindx);
NA_HAPI  NABool naUpdateTreeNodeCapturing(NATree* tree, NATreeNode* node);
NA_HIAPI NABool naIsTreeRootLeaf(const NATree* tree);

#include "../../NACore/NAValueHelper.h"


// /////////////////////////////////////
// Configuration
// /////////////////////////////////////



NA_HIDEF void naDeallocConfiguration(NATreeConfiguration* config){
  if(config->internaldata){naFree(config->internaldata);}
  naFree(config);
}



NA_IDEF void naReleaseTreeConfiguration(NATreeConfiguration* config){
  naReleaseRefCount(&(config->refcount), &(config->refcount), (NAMutator)naDeallocConfiguration);
}



NA_HIDEF NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config){
  #ifndef NDEBUG
    config->flags |= NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE;
  #endif
  return (NATreeConfiguration*)naRetainRefCount(&(config->refcount));
}



NA_IAPI void naSetTreeConfigurationData(NATreeConfiguration* config, NAPtr data){
  #ifndef NDEBUG
    if(naGetPtrConst(config->data))
      naError("naSetTreeConfigurationData", "Configuration already has data");
  #endif
  config->data = data;
}



NA_IDEF void naSetTreeConfigurationTreeCallbacks(NATreeConfiguration* config, NATreeContructorCallback treeConstructor, NATreeDestructorCallback  treeDestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("naSetTreeConfigurationTreeCallbacks", "Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->treeConstructor = treeConstructor;
  config->treeDestructor = treeDestructor;
}



NA_IDEF void naSetTreeConfigurationLeafCallbacks(NATreeConfiguration* config, NATreeLeafConstructor leafConstructor, NATreeLeafDestructor leafDestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("naSetTreeConfigurationLeafCallbacks", "Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->leafConstructor = leafConstructor;
  config->leafDestructor = leafDestructor;
}



NA_IDEF void naSetTreeConfigurationNodeCallbacks(NATreeConfiguration* config, NATreeNodeConstructor nodeconstructor, NATreeNodeDestructor nodedestructor, NATreeNodeUpdater nodeUpdater){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("naSetTreeConfigurationNodeCallbacks", "Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->nodeConstructor = nodeconstructor;
  config->nodeDestructor = nodedestructor;
  config->nodeUpdater = nodeUpdater;
}



NA_IAPI void naSetTreeConfigurationOcttreeBaseLeafExponent(NATreeConfiguration* config, NAInt baseleafexponent){
  #ifndef NDEBUG
    if(!(config->flags & NA_TREE_OCTTREE))
      naError("naSetTreeConfigurationOcttreeBaseLeafExponent", "This configuration is not for an octtree");
  #endif
  if(config->internaldata){naFree(config->internaldata);}
  NAInt* octtreedata = naMalloc(naSizeof(NAInt));
  *octtreedata = baseleafexponent;
  config->internaldata = octtreedata;
}



NA_IAPI NAInt naGetTreeConfigurationOcttreeBaseLeafExponent(const NATreeConfiguration* config){
  return *((NAInt*)(config->internaldata));
}



NA_IAPI NAPtr naGetTreeConfigurationData(const NATreeConfiguration* config){
  return config->data;
}



// /////////////////////////////////////
// Tree
// /////////////////////////////////////

NA_IDEF NATree* naInitTree(NATree* tree, NATreeConfiguration* config){
  tree->config = naRetainTreeConfiguration(config);

  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_OCTTREE) && !tree->config->internaldata)
      naError("naInitTree", "Octtree configuration needs more information. Use naSetTreeConfigurationOcttreeBaseLeafExponent");
  #endif

  // If the config defines a callback for constructing a tree, call it.
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
      naError("naEmptyTree", "There are still iterators running on this tree. Did you miss a naClearTreeIterator call?");
  #endif
  if(tree->root){
    if(naIsTreeRootLeaf(tree)){
      tree->config->leafCoreDestructor(tree, (NATreeLeaf*)tree->root);
    }else{
      tree->config->nodeCoreDestructor(tree, (NATreeNode*)tree->root, NA_TRUE);
    }
  }
  tree->root = NA_NULL;
}



NA_IDEF void naClearTree(NATree* tree){
  naEmptyTree(tree);
  // If the config sets a callback function for deleting a tree, call it.
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
      naError("naAddTreeFirstConst", "This function should not be called on trees with keys");
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
      naError("naAddTreeFirstMutable", "This function should not be called on trees with keys");
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
      naError("naAddTreeLastConst", "This function should not be called on trees with keys");
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
      naError("naAddTreeLastMutable", "This function should not be called on trees with keys");
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
  retvalue = naGetTreeCurConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeFirstMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeFirst(&iter);
  retvalue = naGetTreeCurMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF const void* naGetTreeLastConst(const NATree* tree){
  const void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurConst(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void* naGetTreeLastMutable(const NATree* tree){
  void* retvalue;
  NATreeIterator iter = naMakeTreeAccessor(tree);
  naLocateTreeLast(&iter);
  retvalue = naGetTreeCurMutable(&iter);
  naClearTreeIterator(&iter);
  return retvalue;
}



NA_IDEF void naUpdateTree(NATree* tree){
  #ifndef NDEBUG
    if(!tree->config->nodeUpdater)
      naError("naUpdateTree", "tree is configured without nodeUpdater callback");
  #endif
  if(tree->root && !naIsTreeRootLeaf(tree)){
    naUpdateTreeNodeCapturing(tree, (NATreeNode*)tree->root);
  }
}



NA_IDEF void naUpdateTreeLeaf(NATreeIterator* iter){
  NATree* tree;
  NATreeNode* parent;
  #ifndef NDEBUG
    if(naIsTreeAtInitial(iter))
      naError("naUpdateTreeLeaf", "Iterator is not at a leaf");
  #endif
  tree = (NATree*)naGetPtrConst(iter->tree);
  parent = ((NATreeBaseNode*)iter->leaf)->parent;
  if(parent){
    naUpdateTreeNodeBubbling(tree, parent, tree->config->childIndexGetter(parent, ((NATreeBaseNode*)iter->leaf)));
  }
}

// /////////////////////////////////////
// Iterator
// /////////////////////////////////////



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  iter.flags = 0;
  iter.tree = naMakePtrWithDataConst(tree);
  iter.leaf = NA_NULL;
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    tree->itercount++;
  #endif
  iter.flags = 0;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.leaf = NA_NULL;
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  #ifndef NDEBUG
    tree->itercount++;
  #endif
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.leaf = NA_NULL;
  return iter;
}



NA_HIDEF void naSetTreeIteratorCurLeaf(NATreeIterator* iter, NATreeLeaf* newleaf){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naSetTreeIteratorCurLeaf", "This iterator has been cleared. You need to make it anew.");
    if(!naIsTreeAtInitial(iter)){
      if(iter->leaf->itercount == 0)
        naError("naSetTreeIteratorCurLeaf", "This leaf has zero iterators already");
      iter->leaf->itercount--;
    }
  #endif
  iter->leaf = newleaf;
  #ifndef NDEBUG
    if(!naIsTreeAtInitial(iter)){iter->leaf->itercount++;}
  #endif
}



NA_IDEF void naResetTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naResetTreeIterator", "This iterator has been cleared. You need to make it anew.");
  #endif
  naSetTreeIteratorCurLeaf(iter, NA_NULL);
}



NA_IDEF void naClearTreeIterator(NATreeIterator* iter){
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)naGetPtrConst(iter->tree);
    mutabletree->itercount--;
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naClearTreeIterator", "This iterator has already been cleared.");
  #endif
  naSetTreeIteratorCurLeaf(iter, NA_NULL);
  #ifndef NDEBUG
    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #endif
}



NA_IDEF NABool naLocateTreeKey(NATreeIterator* iter, const void* key, NABool assumeclose){
  NABool matchfound;
  NATreeLeaf* leaf = naLocateTreeLeaf(iter, key, &matchfound, assumeclose);

  if(leaf && matchfound){
    naSetTreeIteratorCurLeaf(iter, leaf);
  }else{
    naSetTreeIteratorCurLeaf(iter, NA_NULL);
  }
  return matchfound;
}



NA_IDEF NABool naLocateTreeFirst(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTree(iter);
}



NA_IDEF NABool naLocateTreeLast(NATreeIterator* iter){
  naResetTreeIterator(iter);
  return naIterateTreeBack(iter);
}



NA_IDEF NABool naLocateTreeIterator(NATreeIterator* iter, NATreeIterator* srciter){
  #ifndef NDEBUG
    if(naGetPtrConst(iter->tree) != naGetPtrConst(srciter->tree))
      naError("nalocateTreeIterator", "The two iterators do not belong to the same tree");
  #endif
  naSetTreeIteratorCurLeaf(iter, srciter->leaf);
  return (iter->leaf != NA_NULL);
}



NA_IAPI NABool naLocateTreeToken(NATreeIterator* iter, void* token, NATreeNodeTokenSearcher nodeSearcher, NATreeLeafTokenSearcher leafSearcher){
  NABool matchfound;
  NATreeLeaf* leaf = naLocateTreeTokenLeaf(iter, token, nodeSearcher, leafSearcher, &matchfound);

  if(leaf && matchfound){
    naSetTreeIteratorCurLeaf(iter, leaf);
  }else{
    naSetTreeIteratorCurLeaf(iter, NA_NULL);
  }
  return matchfound;
}



NA_IDEF void naBubbleTreeToken(const NATreeIterator* iter, void* token, NATreeNodeTokenCallback nodeTokenCallback){
  const NATree* tree;
  NATreeBaseNode* basenode;
  NABool continueBubbling;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naBubbleTreeToken", "This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("naBubbleTreeToken", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(iter->tree);
  basenode = (NATreeBaseNode*)iter->leaf;
  continueBubbling = NA_TRUE;
  while(continueBubbling && basenode->parent){
    NAInt childindx = tree->config->childIndexGetter(basenode->parent, basenode);
    continueBubbling = nodeTokenCallback(token, tree->config->nodeDataGetter(basenode->parent), childindx);
    basenode = (NATreeBaseNode*)(basenode->parent);
  }
}



NA_IDEF const void* naGetTreeCurKey(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naGetTreeConst", "This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("naGetTreeConst", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(iter->tree);
  return tree->config->leafKeyGetter(iter->leaf);
}



NA_IDEF const void* naGetTreeCurConst(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naGetTreeConst", "This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("naGetTreeConst", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(iter->tree);
  return naGetPtrConst(tree->config->leafDataGetter(iter->leaf));
}



NA_IDEF void* naGetTreeCurMutable(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
  if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("naGetTreeMutable", "This iterator has been cleared. You need to make it anew.");
  if(naIsTreeAtInitial(iter))
    naError("naGetTreeMutable", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(iter->tree);
  return naGetPtrMutable(tree->config->leafDataGetter(iter->leaf));
}



NA_IDEF NABool naIterateTree(NATreeIterator* iter){
  NATreeIterationInfo info;
  const NATree* tree;
  tree = (const NATree*)naGetPtrConst(iter->tree);
  info.step = 1;
  info.startindx = -1;
  info.breakindx = tree->config->childpernode;
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naIterateTreeBack(NATreeIterator* iter){
  NATreeIterationInfo info;
  const NATree* tree;
  tree = (const NATree*)naGetPtrConst(iter->tree);
  info.step = -1;
  info.startindx = tree->config->childpernode;
  info.breakindx = -1;
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naAddTreeKeyConst(NATreeIterator* iter, const void* key, const void* content, NABool replace){
  #ifndef NDEBUG
    const NATree* tree = (NATree*)naGetPtrConst(iter->tree);
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naAddTreeKeyConst", "This function should not be called on trees without keys");
  #endif
  return naAddTreeLeaf(iter, key, naMakePtrWithDataConst(content), replace);
}



NA_IDEF NABool naAddTreeKeyMutable(NATreeIterator* iter, const void* key, void* content, NABool replace){
  #ifndef NDEBUG
    const NATree* tree = (NATree*)naGetPtrConst(iter->tree);
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naAddTreeKeyMutable", "This function should not be called on trees without keys");
  #endif
  return naAddTreeLeaf(iter, key, naMakePtrWithDataMutable(content), replace);
}



NA_IDEF NABool naIsTreeAtInitial(const NATreeIterator* iter){
  return (iter->leaf == NA_NULL);
}



NA_IDEF NABool naAddTreeContent(NATreeIterator* iter, NAPtr content, NATreeLeafInsertOrder insertOrder, NABool movetonew){
  NATreeLeaf* contentleaf;
  NATree* tree = (NATree*)naGetPtrMutable(iter->tree);
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
      naError("naAddTreeContent", "This function should not be called on trees with keys");
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naAddTreeContent", "This iterator has been cleared. You need to make it anew.");
  #endif
  if(!iter->leaf && tree->root){
    // There is a root but the iterator is not at a leaf. We need to find the
    // correct leaf.
    switch(insertOrder){
    case NA_TREE_LEAF_INSERT_ORDER_KEY:
      #ifndef NDEBUG
        naError("naAddTreeContent", "This should not happen.");
      #endif
      break;
    case NA_TREE_LEAF_INSERT_ORDER_NEXT:
      naIterateTree(iter);
      insertOrder = NA_TREE_LEAF_INSERT_ORDER_PREV;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_PREV:
      naIterateTreeBack(iter);
      insertOrder = NA_TREE_LEAF_INSERT_ORDER_NEXT;
      break;
    case NA_TREE_LEAF_INSERT_ORDER_REPLACE:
      #ifndef NDEBUG
        naError("naAddTreeContent", "This case should not occur");
      #endif
      break;
    }
  }
  contentleaf = naAddTreeContentAtLeaf(tree, iter->leaf, NA_NULL, content, insertOrder);
  if(movetonew){naSetTreeIteratorCurLeaf(iter, contentleaf);}
  return NA_TRUE;
}
NA_IDEF NABool naAddTreePrevConst(NATreeIterator* iter, const void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_PREV, movetonew);
}
NA_IDEF NABool naAddTreePrevMutable(NATreeIterator* iter, void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_PREV, movetonew);
}
NA_IDEF NABool naAddTreeNextConst(NATreeIterator* iter, const void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataConst(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, movetonew);
}
NA_IDEF NABool naAddTreeNextMutable(NATreeIterator* iter, void* content, NABool movetonew){
  return naAddTreeContent(iter, naMakePtrWithDataMutable(content), NA_TREE_LEAF_INSERT_ORDER_NEXT, movetonew);
}



NA_IDEF void naRemoveTreeCur(NATreeIterator* iter, NABool advance){
  NATree* tree;
  NATreeLeaf* curleaf;
  NATreeNode* newparent;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
    if(naIsTreeAtInitial(iter))
      naError("naLocateTree", "Iterator is not at a leaf.");
  #endif
  tree = (NATree*)naGetPtrMutable(iter->tree);
  curleaf = iter->leaf;
  if(advance){naIterateTree(iter);}else{naIterateTreeBack(iter);}
  newparent = tree->config->leafRemover(tree, curleaf);
  naUpdateTreeNodeBubbling(tree, newparent, -1);
}



// /////////////////////////////////////
// Node
// /////////////////////////////////////

NA_HIDEF void naInitTreeNode(NATreeNode* node){
  // We don not initialize the parent. That may seem dangerous but there is
  // ony one function naAddTreeLeaf where leafes are created and there, the
  // parent is always set afterwards with a call to leafAdder.
  node->flags = 0;
}



NA_HIDEF void naClearTreeNode(NATreeNode* node){
  NA_UNUSED(node);
}



NA_HIDEF NABool naIsNodeChildLeaf(NATreeNode* node, NAInt childindx){
  return (NABool)((node->flags >> childindx) & 0x01);
}



NA_HIDEF NABool naIsBaseNodeLeaf(const NATree* tree, NATreeBaseNode* basenode){
  NAInt childindx;
  if(!basenode->parent){return naIsTreeRootLeaf(tree);}
  childindx = tree->config->childIndexGetter(basenode->parent, basenode);
  return naIsNodeChildLeaf(basenode->parent, childindx);
}



NA_HIDEF void naMarkNodeChildLeaf(NATreeNode* node, NAInt childindx, NABool isleaf){
  node->flags &= ~(1 << childindx);
  node->flags |= (NAInt)isleaf << childindx;
}



NA_HIDEF NABool naIsTreeRootLeaf(const NATree* tree){
  return (NABool)((tree->flags & NA_TREE_FLAG_ROOT_IS_LEAF) == NA_TREE_FLAG_ROOT_IS_LEAF);
}



NA_HIDEF void naMarkTreeRootLeaf(NATree* tree, NABool isleaf){
  tree->flags &= ~NA_TREE_FLAG_ROOT_IS_LEAF;
  tree->flags |= (isleaf * NA_TREE_FLAG_ROOT_IS_LEAF);
}



// /////////////////////////////////////
// Leaf
// /////////////////////////////////////



NA_HIDEF void naInitTreeLeaf(NATreeLeaf* leaf){
  // We don not initialize the parent. That may seem dangerous but there is
  // ony one function naAddTreeLeaf where leafes are created and there, the
  // parent is always set afterwards with a call to leafAdder.
  #ifndef NDEBUG
    leaf->itercount = 0;
  #else
    NA_UNUSED(leaf);
  #endif
}



NA_HIDEF NAPtr naConstructLeafData(NATree* tree, const void* key, NAPtr data){
  if(tree->config->leafConstructor){
    return tree->config->leafConstructor(key, tree->config->data, data);
  }else{
    return data;
  }
}



NA_HIDEF void naDestructLeafData(NATree* tree, NAPtr data){
  if(tree->config->leafDestructor){
    tree->config->leafDestructor(data, tree->config->data);
  }
}



NA_HIDEF void naClearTreeLeaf(NATreeLeaf* leaf){
  #ifndef NDEBUG
    if(leaf->itercount)
      naError("naClearTreeLeaf", "There are still iterators running on this leaf. Did you forget a call to naClearTreeIterator?");
  #else
    NA_UNUSED(leaf);
  #endif
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
