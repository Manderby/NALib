
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations.
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"



#define NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE 0x8000

#define NA_TREE_ITERATOR_MODIFIER 0x01
#define NA_TREE_ITERATOR_CLEARED  0x80

// Prototypes
NA_HIAPI NABool naIsNodeChildTypeValid(NANodeChildType childtype);
NA_HIAPI NANodeChildType naGetNodeChildType(NATreeNode* node, NAInt childindx);
NA_HAPI  NATreeNode* naLocateTreeNode(NATreeIterator* iter, const void* key, NABool* keyleaffound, NAInt* leafindx, NABool usebubble);
NA_HAPI  NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info);
NA_HAPI  NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace);



// /////////////////////////////////////
// Configuration
// /////////////////////////////////////



NA_HIDEF void naDeallocConfiguration(NATreeConfiguration* config){
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



NA_IDEF void naSetTreeConfigurationNodeCallbacks(NATreeConfiguration* config, NATreeNodeConstructor nodeconstructor, NATreeNodeDestructor nodedestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("naSetTreeConfigurationNodeCallbacks", "Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->nodeConstructor = nodeconstructor;
  config->nodeDestructor = nodedestructor;
}



// /////////////////////////////////////
// Tree
// /////////////////////////////////////

NA_IDEF NATree* naInitTree(NATree* tree, NATreeConfiguration* config){
  tree->config = naRetainTreeConfiguration(config);
  
  // If the config defines a callback for constructing a tree, call it.
  if(tree->config->treeConstructor){
    tree->config->treeConstructor(tree->config->data);
  }
  
  // Init the tree root.
  tree->root = NA_NULL;
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
  if(tree->root){tree->config->nodeCoreDestructor(tree, tree->root);}
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



// /////////////////////////////////////
// Iterator
// /////////////////////////////////////



NA_IDEF NATreeIterator naMakeTreeAccessor(const NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataConst(tree);
  iter.leaf = NA_NULL;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeMutator(NATree* tree){
  NATreeIterator iter;
  iter.flags = 0;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.leaf = NA_NULL;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_IDEF NATreeIterator naMakeTreeModifier(NATree* tree){
  NATreeIterator iter;
  iter.flags = NA_TREE_ITERATOR_MODIFIER;
  iter.tree = naMakePtrWithDataMutable(tree);
  iter.leaf = NA_NULL;
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)tree;
    mutabletree->itercount++;
  #endif
  return iter;
}



NA_HIDEF void naSetTreeIteratorCurLeaf(NATreeIterator* iter, NATreeLeaf* newleaf){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naSetTreeIteratorCurLeaf", "This iterator has been cleared. You need to make it anew.");
    if(iter->leaf){iter->leaf->itercount--;}
  #endif
  iter->leaf = newleaf;
  #ifndef NDEBUG
    if(iter->leaf){iter->leaf->itercount++;}
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
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naClearTreeIterator", "This iterator has already been cleared.");
  #endif
  naSetTreeIteratorCurLeaf(iter, NA_NULL);
  #ifndef NDEBUG
    NATree* mutabletree = (NATree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    naSetFlagi(&(iter->flags), NA_TREE_ITERATOR_CLEARED, NA_TRUE);
  #endif
}



NA_IDEF NABool naLocateTree(NATreeIterator* iter, const void* key, NABool assumeclose){
  NABool keyleaffound;
  NAInt childindx;  // unused.
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NATreeNode* node = naLocateTreeNode(iter, key, &keyleaffound, &childindx, assumeclose);

  if(keyleaffound){
    naSetTreeIteratorCurLeaf(iter, (NATreeLeaf*)tree->config->childGetter(node, childindx));
  }else{
    naSetTreeIteratorCurLeaf(iter, NA_NULL);
  }
  return keyleaffound;
}



NA_IDEF const void* naGetTreeCurKey(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naGetTreeConst", "This iterator has been cleared. You need to make it anew.");
    if(!iter->leaf)
      naError("naGetTreeConst", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return tree->config->leafKeyGetter(iter->leaf);
}



NA_IDEF const void* naGetTreeCurConst(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naGetTreeConst", "This iterator has been cleared. You need to make it anew.");
    if(!iter->leaf)
      naError("naGetTreeConst", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return naGetPtrConst(tree->config->leafDataGetter(iter->leaf));
}



NA_IDEF void* naGetTreeCurMutable(NATreeIterator* iter){
  const NATree* tree;
  #ifndef NDEBUG
  if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
    naError("naGetTreeMutable", "This iterator has been cleared. You need to make it anew.");
  if(!iter->leaf)
    naError("naGetTreeMutable", "This iterator is not at a leaf.");
  #endif
  tree = (const NATree*)naGetPtrConst(&(iter->tree));
  return naGetPtrMutable(tree->config->leafDataGetter(iter->leaf));
}



NA_IDEF NABool naIterateTree(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NATreeIterationInfo info = {1, -1, tree->config->childpernode};
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naIterateTreeBack(NATreeIterator* iter){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  NATreeIterationInfo info = {-1, tree->config->childpernode, -1};
  return naIterateTreeWithInfo(iter, &info);
}



NA_IDEF NABool naAddTreeConst(NATreeIterator* iter, const void* key, const void* content, NABool replace){
  return naAddTreeLeaf(iter, key, naMakePtrWithDataConst(content), replace);
}



NA_IDEF NABool naAddTreeMutable(NATreeIterator* iter, const void* key, void* content, NABool replace){
  return naAddTreeLeaf(iter, key, naMakePtrWithDataMutable(content), replace);
}



NA_IDEF void naRemoveTreeCur(NATreeIterator* iter, NABool advance){
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
    if(!iter->leaf)
      naError("naLocateTree", "Iterator is not at a leaf.");
  #endif
  NATree* tree = (NATree*)naGetPtrMutable(&(iter->tree));
  NATreeLeaf* curleaf = iter->leaf;
  if(advance){naIterateTree(iter);}else{naIterateTreeBack(iter);}
  tree->config->leafRemover(tree, curleaf);
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



NA_HIDEF NANodeChildType naGetNodeChildType(NATreeNode* node, NAInt childindx){
  return (NANodeChildType)((node->flags >> (childindx * 2)) & NA_TREE_NODE_CHILD_MASK);
}



NA_HIDEF void naSetNodeChildType(NATreeNode* node, NAInt childindx, NANodeChildType childtype){
  // Clear the old type
  naSetFlagi(&(node->flags), NA_TREE_NODE_CHILD_MASK << (childindx * 2), NA_FALSE);
  // Set the new type
  naSetFlagi(&(node->flags), childtype << (childindx * 2), NA_TRUE);
}



NA_HIDEF NANodeChildType naGetNodeType(const NATree* tree, NATreeBaseNode* node){
  if(!node->parent){return NA_TREE_NODE_CHILD_NODE;}
  NAInt childindx = tree->config->childIndexGetter(node->parent, node);
  return naGetNodeChildType(node->parent, childindx);
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



// /////////////////////////////////////
// Varia
// /////////////////////////////////////

NA_HIDEF NABool naIsNodeChildTypeValid(NANodeChildType childtype){
  return naTestFlagi(childtype, NA_TREE_NODE_CHILD_AVAILABLE_MASK);
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
