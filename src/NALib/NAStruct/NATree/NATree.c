
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"
#include "NAMemory.h"



NA_DEF NATree* naInitTree(NATree* tree, NATreeConfiguration* config){
  tree->config = naRetainTreeConfiguration(config);
  // If the config defines a callback for constructing a tree, call it.
  if(tree->config->treeconstructor){
    tree->config->treeconstructor(tree->config->userdata);
  }
  // Init the tree root.
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}



//NA_DEF NATree* naInitTreeCopy(NATree* dsttree, NATree* srctree){
//  naInitTree(dsttree, srctree->config);
//  
////  dstiter = naMakeTreeModifier(dsttree);
////  srciter = naMakeTreeAccessor(srctree);
////  while(naIterateTree(&srciter, NA_NULL)){
////    const void* srckey;
////    const void* dupdata = naGetQuadTreeCurConst(&dupiter);
////    duporigin = naGetQuadTreeCurRect(&dupiter).pos;
////    naLocateQuadTreeCoord(&newiter, duporigin);
////    naCreateQuadTreeLeaf(&newiter, dupdata);
////  }
////  
////  naClearTreeIterator(&srciter);
////  naClearTreeIterator(&dstiter);
//
//  return dsttree;
//}



NA_DEF void naEmptyTree(NATree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("naEmptyTree", "There are still iterators running on this tree. Did you miss a naClearTreeIterator call?");
  #endif
  if(tree->root){naDeallocTreeNode(tree, tree->root);}
  tree->root = NA_NULL;
}



NA_DEF void naClearTree(NATree* tree){
  naEmptyTree(tree);
  naReleaseTreeConfiguration(tree->config);
  // If the config sets a callback function for deleting a tree, call it.
  if(tree->config->treedestructor){
    tree->config->treedestructor(tree->config->userdata);
  }
}



//NA_API NABool naInsertTree(NATreeIterator* iter, double key, NAPtr leafdata){
//  NA_UNUSED(tree);
//  NA_UNUSED(key);
//  NA_UNUSED(leaf);
////  if(tree->root){
////    NATreeIterator iter = naMakeTreeModifier(tree);
////    NABool found = naLocateTree(&iter, key);
////    NATreeNode* node = naAllocTreeNode(tree, iter.node, key, leaf);
////    #ifndef NDEBUG
////      if(found)
////        naError("naAddTreeLeaf", "An element with the given key already exists.");
////      if(iter.node->child[iter.childindx])
////        naError("naAddTreeLeaf", "child should be null");
////    #else
////      NA_UNUSED(found);
////    #endif
////    iter.node->child[iter.childindx] = node;
////    naClearTreeIterator(&iter);
////  }else{
////    tree->root = naAllocTreeNode(tree, NA_NULL, key, leaf);
////  }
//}




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
