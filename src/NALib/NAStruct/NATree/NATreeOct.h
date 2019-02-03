
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NACoord.h"

typedef struct NATreeOctNode NATreeOctNode;
struct NATreeOctNode{
  NATreeNode node;
  NAVertex origin;
  NAInt childexponent;
  NATreeItem* childs[8];
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeOctNode);

typedef struct NATreeOctLeaf NATreeOctLeaf;
struct NATreeOctLeaf{
  NATreeLeaf leaf;
  NAVertex origin;  // todo remove this maybe?
  NAInt leafexponent; // todo remove this maybe?
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeOctLeaf);

NA_HAPI NAInt naGetKeyIndexOctDouble(const void* basekey, const void* testkey, const void* data);
NA_HAPI NABool naEqualKeyOctDouble(const void* key1, const void* key2);
NA_HAPI void naAssignKeyOctDouble(void* dst, const void* src);
NA_HAPI NABool naTestKeyOctDouble(const void* leftlimit, const void* rightlimit, const void* key);

//NA_HAPI NAInt naGetKeyIndexOctNAInt(const void* basekey, const void* key);
//NA_HAPI NABool naEqualKeyOctNAInt(const void* key1, const void* key2);
//NA_HAPI void naAssignKeyOctNAInt(void* dst, const void* src);
//NA_HAPI NABool naTestKeyOctNAInt(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI void naDestructTreeNodeOct(NATree* tree, NATreeNode* node, NABool recursive);
NA_HAPI NATreeLeaf* naConstructTreeLeafOct(NATree* tree, const void* key, NAPtr data);
NA_HAPI void naDestructTreeLeafOct(NATree* tree, NATreeLeaf* leaf);

NA_HAPI NATreeNode* naLocateBubbleOct(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI NATreeLeaf* naLocateCaptureOct(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound);
NA_HAPI NAInt naGetChildIndexOct(NATreeNode* parent, NATreeItem* child);
NA_HAPI NAInt naGetChildKeyIndexOct(const NATree* tree, NATreeNode* parent, const void* key);
NA_HAPI NATreeItem* naGetChildOct(NATreeNode* parent, NAInt childindx);
NA_HAPI NATreeNode* naRemoveLeafOct(NATree* tree, NATreeLeaf* leaf);
NA_HAPI NATreeLeaf* naInsertLeafOct(NATree* tree, NATreeLeaf* existingleaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI const void* naGetLeafKeyOct(NATreeLeaf* leaf);
NA_HAPI NAPtr naGetLeafDataOct(NATreeLeaf* leaf);
NA_HAPI NAPtr naGetNodeDataOct(NATreeNode* node);

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
