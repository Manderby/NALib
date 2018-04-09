
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAQuadTree.h
// Do not include this file directly! It will automatically be included when
// including "NAQuadTree.h"




#define REMOVEPosiToPos(p) naMakePos(p.x, p.y)
#define REMOVEPosToPosi(p) naMakePosi((NAInt)naRound(p.x), (NAInt)naRound(p.y))
#define REMOVESizeiToSize(s) naMakeSize(s.width, s.height)
#define REMOVERectiToRect(r) naMakeRectS(r.pos.x, r.pos.y, r.size.width, r.size.height)
#define REMOVERectToRecti(r) naMakeRectiS((NAInt)naRound(r.pos.x), (NAInt)naRound(r.pos.y), (NAInt)naRound(r.size.width), (NAInt)naRound(r.size.height))



typedef struct NAQuadTreeNode NAQuadTreeNode;

struct NAQuadTree{
  NAQuadTreeConfiguration configuration;
  NAQuadTreeNode* root;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NAQuadTreeIterator{
  NAPtr tree;
  NAQuadTreeNode* curnode;
  NAInt cursegment;
  NAPos leaforigin;
  NAInt flags;
};




NA_IDEF NABool naIsQuadTreeEmpty(const NAQuadTree* tree){
  return (tree->root == NA_NULL);
}



NA_IDEF NAInt naGetQuadTreeMinLeafExponent(const NAQuadTree* tree){
  return tree->configuration.minleafexponent;
}



NA_IDEF NAPos naGetQuadTreeAlignedCoord(NAInt leafexponent, NAPos coord){
  NARecti leafalign = naMakeRecti(naMakePosi(0, 0), naMakeSizei(leafexponent, leafexponent));
  NAPosi alignposint = naMakePosiWithAlignment(naMakePosiWithIntegerPos(coord), leafalign);
  return REMOVEPosiToPos(alignposint);
}



NA_IDEF NAQuadTreeConfiguration naGetQuadTreeConfiguration(const NAQuadTree* tree){
  return tree->configuration;
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
