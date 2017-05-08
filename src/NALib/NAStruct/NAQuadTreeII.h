
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAQuadTree.h
// Do not include this file directly! It will automatically be included when
// including "NAQuadTree.h"

typedef struct NAQuadTreeNode NAQuadTreeNode;

struct NAQuadTree{
  NAInt leaflength;
  NAQuadTreeCallbacks callbacks;
  NAQuadTreeNode* root;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NAQuadTreeIterator{
  NAPtr tree;
  NAQuadTreeNode* curnode;
  NAInt cursegment;
  NAPosi leaforigin;
  NAInt flags;
};





NA_IDEF void naEmptyQuadTree(NAQuadTree* tree){
  naClearQuadTree(tree);
  tree->root = NA_NULL;
}



NA_IDEF NABool naIsQuadTreeEmpty(const NAQuadTree* tree){
  return (tree->root == NA_NULL);
}



NA_IDEF NAInt naGetQuadTreeLeafLength(const NAQuadTree* tree){
  return tree->leaflength;
}



NA_IDEF NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree){
  return tree->callbacks;
}



NA_HIDEF void naInitQuadTreeIterator(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    NAQuadTree* mutabletree = (NAQuadTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount++;
  #endif
  iter->curnode = NA_NULL;
  iter->cursegment= -1;
  iter->flags = 0;
}






// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
