//
//// This file is part of NALib, a collection of C source code.
//// Full license notice at the bottom.
//
//
//// This file contains inline implementations of the file NAQuadTree.h
//// Do not include this file directly! It will automatically be included when
//// including "NAQuadTree.h"
//
//
//
//
//typedef struct NAQuadTreeNode NAQuadTreeNode;
//
//struct NAQuadTree{
//  NAQuadTreeConfiguration configuration;
//  NAQuadTreeNode* root;
//  #ifndef NDEBUG
//    NAInt itercount;
//  #endif
//};
//
//struct NAQuadTreeIterator{
//  NAPtr tree;
//  NAQuadTreeNode* node; // Denotes the node currently visiting
//  int16 childsegment;   // Denotes the child segment currently visiting.
//  NAPos pos;            // Denotes the exact position which was requested.
//  #ifndef NDEBUG
//    NAInt flags;
//  #endif
//};
//
//
//
//
//NA_IDEF NABool naIsQuadTreeEmpty(const NAQuadTree* tree){
//  return (tree->root == NA_NULL);
//}
//
//
//
//#undef naBeginQuadTreeAccessorIteration
//#define naBeginQuadTreeAccessorIteration(typedelem, quadtree, limit, iter)\
//  iter = naMakeQuadTreeAccessor(quadtree);\
//  while(naIterateQuadTree(&iter, limit)){\
//    typedelem = naGetQuadTreeCurConst(&iter)
//
//#undef naBeginQuadTreeMutatorIteration
//#define naBeginQuadTreeMutatorIteration(typedelem, quadtree, limit, iter)\
//  iter = naMakeQuadTreeMutator(quadtree);\
//  while(naIterateQuadTree(&iter, limit)){\
//    typedelem = naGetQuadTreeCurMutable(&iter, NA_FALSE)
//
//#undef naBeginQuadTreeModifierIteration
//#define naBeginQuadTreeModifierIteration(typedelem, quadtree, limit, iter)\
//  iter = naMakeQuadTreeModifier(quadtree);\
//  while(naIterateQuadTree(&iter, limit)){\
//    typedelem = naGetQuadTreeCurMutable(&iter, NA_FALSE)
//
//#undef naEndQuadTreeIteration
//#define naEndQuadTreeIteration(iter)\
//  }\
//  naClearQuadTreeIterator(&iter)
//
//
//
//
//
//
//// Copyright (c) NALib, Tobias Stamm
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
