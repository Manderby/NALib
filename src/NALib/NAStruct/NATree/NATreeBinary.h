
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains prototypes of the file NATreeBinary.c
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"


typedef struct NATreeBinaryNode NATreeBinaryNode;
struct NATreeBinaryNode{
  NATreeNode node;
  union{
    double d;
    NAInt i;
  } key;
  NATreeBaseNode* childs[2];
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryNode);

typedef struct NATreeBinaryLeaf NATreeBinaryLeaf;
struct NATreeBinaryLeaf{
  NATreeLeaf leaf;
  union{
    double d;
    NAInt i;
  } key;
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryLeaf);

NA_HAPI NAInt naGetKeyIndexBinaryDouble(const void* basekey, const void* testkey);
NA_HAPI NABool naEqualKeyBinaryDouble(const void* key1, const void* key2);
NA_HAPI void naAssignKeyBinaryDouble(void* dst, const void* src);
NA_HAPI NABool naTestKeyBinaryDouble(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI NAInt naGetKeyIndexBinaryNAInt(const void* basekey, const void* key);
NA_HAPI NABool naEqualKeyBinaryNAInt(const void* key1, const void* key2);
NA_HAPI void naAssignKeyBinaryNAInt(void* dst, const void* src);
NA_HAPI NABool naTestKeyBinaryNAInt(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI NATreeNode* naConstructTreeNodeBinary(NATree* tree, const void* key, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf);
NA_HAPI void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node, NABool recursive);
NA_HAPI NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, const void* key, NAPtr data);
NA_HAPI void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf);

NA_HAPI NATreeNode* naLocateBubbleBinary(const NATree* tree, NATreeLeaf* leaf, const void* key);
NA_HAPI NATreeLeaf* naLocateCaptureBinary(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound);
NA_HAPI NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child);
NA_HAPI NAInt naGetChildKeyIndexBinary(const NATree* tree, NATreeNode* parent, const void* key);
NA_HAPI NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx);
NA_HAPI NATreeNode* naRemoveLeafBinary(NATree* tree, NATreeLeaf* leaf);
NA_HAPI NATreeLeaf* naInsertLeafBinary(NATree* tree, NATreeLeaf* existingleaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI const void* naGetLeafKeyBinary(NATreeLeaf* leaf);
NA_HAPI NAPtr* naGetLeafDataBinary(NATreeLeaf* leaf);
NA_HDEF NAPtr* naGetNodeDataBinary(NATreeNode* node);

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
