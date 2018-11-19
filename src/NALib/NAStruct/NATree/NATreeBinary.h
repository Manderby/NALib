
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains prototypes of the file NATreeBinary.c
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"


typedef struct NATreeBinaryNode NATreeBinaryNode;
struct NATreeBinaryNode{
  NATreeNode node;
  double key;
  NATreeBaseNode* childs[2];
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryNode);

typedef struct NATreeBinaryLeaf NATreeBinaryLeaf;
struct NATreeBinaryLeaf{
  NATreeLeaf leaf;
  double key;
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryLeaf);

NA_HAPI NAInt naGetKeyIndexBinaryDouble(const void* basekey, const void* key);
NA_HAPI NABool naEqualKeyBinaryDouble(const void* key1, const void* key2);
NA_HAPI void naAssignKeyBinaryDouble(void* dst, const void* src);
NA_HAPI NABool naTestKeyBinaryDouble(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI NAInt naGetKeyIndexBinaryNAInt(const void* basekey, const void* key);
NA_HAPI NABool naEqualKeyBinaryNAInt(const void* key1, const void* key2);
NA_HAPI void naAssignKeyBinaryNAInt(void* dst, const void* src);
NA_HAPI NABool naTestKeyBinaryNAInt(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI NATreeNode* naConstructTreeNodeBinary(NATree* tree, const void* key);
NA_HAPI void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node);
NA_HAPI NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, const void* key, NAPtr data);
NA_HAPI void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf);

NA_HAPI NATreeNode* naLocateBubbleBinary(const NATree* tree, NATreeNode* node, const void* key);
NA_HAPI NATreeNode* naLocateCaptureBinary(const NATree* tree, NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* childindx);
NA_HAPI NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child);
NA_HAPI NAInt naGetChildKeyIndexBinary(const NATree* tree, NATreeNode* parent, const void* key);
NA_HAPI NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx);
NA_HAPI void naAddLeafBinary(NATreeNode* parent, NATreeLeaf* leaf, NAInt leafindx);
NA_HAPI void naRemoveLeafBinary(NATree* tree, NATreeLeaf* leaf);
NA_HAPI void naReplaceLeafBinary(NATree* tree, NATreeLeaf* leaf, NAPtr data);
NA_HAPI void naSplitLeafBinary(NATree* tree, NATreeNode* grandparent, NAInt leafindx, NATreeLeaf* sibling);
NA_HAPI const void* naGetLeafKeyBinary(NATreeLeaf* leaf);
NA_HAPI NAPtr* naGetLeafDataBinary(NATreeLeaf* leaf);

NA_HAPI void naInitNodeAVL(NATreeBinaryNode* node);
NA_HAPI void naGrowAVL(NATree* tree, NATreeBinaryNode* node, NAInt childindx);
NA_HAPI void naShrinkAVL(NATree* tree, NATreeBinaryNode* node, NAInt childindx);

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
