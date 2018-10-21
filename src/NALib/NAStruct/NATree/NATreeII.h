
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

struct NATreeConfiguration{
  NARefCount                  refcount;
  NAMutator                   destructor;
  NATreeContructorCallback    treeconstructor;
  NATreeDestructorCallback    treedestructor;
  NATreeLeafConstructor       leafconstructor;
  NATreeLeafDestructor        leafdestructor;
//  NAQuadTreeDataCopier      datacopier;
//  NAQuadTreeNodeAllocator   nodeallocator;
//  NAQuadTreeNodeDeallocator nodedeallocator;
//  NAQuadTreeChildChanged    childchanged;
  NAPtr                   userdata;
//  int16                     baseleafexponent;
  NAInt                   flags;
  #ifndef NDEBUG
    NAInt                 debugflags;
  #endif
};

typedef struct NATreeNode NATreeNode;
struct NATreeNode{
  double key;
  NATreeNode* parent;
  NATreeNode* child[2];
  NAInt flags;
};

struct NATree{
  NATreeConfiguration* config;
  NATreeNode* root;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};

struct NATreeIterator{
  NAPtr treeptr;
  NATreeNode* node;
  NAInt childindx;
  NAInt flags;
};


// NATreeNode
NA_HAPI NATreeNode* naAllocTreeNode(NATreeNode* parent, double key, void* leaf);
NA_HAPI void naDeallocTreeNode(NATreeNode* node);
NA_HAPI NABool naLocateTreeNode(NATreeIterator* iter, double key);

// NATreeConfiguration
NA_HAPI NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config);


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
