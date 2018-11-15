
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains prototypes of the file NATreeBinary.c
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"


typedef struct NATreeBinaryNode NATreeBinaryNode;
typedef struct NATreeBinaryLeaf NATreeBinaryLeaf;
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryNode);
NA_EXTERN_RUNTIME_TYPE(NATreeBinaryLeaf);

NA_HAPI NATreeNode* naConstructTreeNodeBinary(NATree* tree, const void* key);
NA_HAPI void naDestructTreeNodeBinary(NATree* tree, NATreeNode* node);
NA_HAPI NATreeLeaf* naConstructTreeLeafBinary(NATree* tree, const void* key, NAPtr data);
NA_HAPI void naDestructTreeLeafBinary(NATree* tree, NATreeLeaf* leaf);

NA_HAPI NATreeNode* naLocateBubbleBinary(NATreeNode* node, const void* key);
NA_HAPI NATreeNode* naLocateCaptureBinary(NATreeNode* node, const void* key, NABool* keyleaffound, NAInt* childindx);
NA_HAPI NAInt naGetChildIndexBinary(NATreeNode* parent, NATreeBaseNode* child);
NA_HAPI NAInt naGetChildKeyIndexBinary(NATreeNode* parent, const void* key);
NA_HAPI NATreeBaseNode* naGetChildBinary(NATreeNode* parent, NAInt childindx);
NA_HAPI void naAddLeafBinary(NATreeNode* parent, NATreeLeaf* leaf, NAInt leafindx);
NA_HAPI void naRemoveLeafBinary(NATree* tree, NATreeLeaf* leaf);
NA_HAPI void naReplaceLeafBinary(NATree* tree, NATreeLeaf* leaf, NAPtr data);
NA_HAPI void naSplitLeafBinary(NATree* tree, NATreeNode* grandparent, NAInt leafindx, NATreeLeaf* sibling);
NA_HAPI const void* naGetLeafKeyBinary(NATreeLeaf* leaf);
NA_HAPI NAPtr* naGetLeafDataBinary(NATreeLeaf* leaf);



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
