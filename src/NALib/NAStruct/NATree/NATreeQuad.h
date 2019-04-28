
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NACoord.h"

typedef struct NATreeQuadNode NATreeQuadNode;
struct NATreeQuadNode{
  NATreeNode node;
  NATreeItem* childs[4];
  NAPos origin;
  NAPtr userdata;
  NAInt childexponent;
};
NA_EXTERN_RUNTIME_TYPE(NATreeQuadNode);

typedef struct NATreeQuadLeaf NATreeQuadLeaf;
struct NATreeQuadLeaf{
  NATreeLeaf leaf;
  NAPos origin;  // todo remove this maybe?
  NAPtr userdata;
  NAInt leafexponent; // todo remove this maybe?
};
NA_EXTERN_RUNTIME_TYPE(NATreeQuadLeaf);

#include <stddef.h>
#define NODE_CHILDS_OFFSET_QUAD     offsetof(NATreeQuadNode, childs)
#define LEAF_KEY_OFFSET_QUAD        offsetof(NATreeQuadLeaf, origin)
#define NODE_KEY_OFFSET_QUAD        offsetof(NATreeQuadNode, origin)
#define LEAF_USERDATA_OFFSET_QUAD   offsetof(NATreeQuadLeaf, userdata)
#define NODE_USERDATA_OFFSET_QUAD   offsetof(NATreeQuadNode, userdata)

NA_HAPI  NAInt naGetChildIndexQuadDouble(NATreeNode* parentnode, const void* childkey);
NA_HAPI  NAInt naGetKeyIndexQuadDouble(const void* basekey, const void* testkey, const void* data);
NA_HAPI  NABool naEqualKeyQuadDouble(const void* key1, const void* key2);
NA_HAPI  NABool naLowerKeyQuadDouble(const void* origin1, const void* origin2);
NA_HAPI  NABool naLowerEqualKeyQuadDouble(const void* origin1, const void* origin2);
NA_HAPI  void naAssignKeyQuadDouble(void* dst, const void* src);
NA_HAPI  void naAddKeyQuadDouble(void* dst, const void* src1, const void* src2);
NA_HAPI  NABool naTestKeyQuadDouble(const void* lowerlimit, const void* upperlimit, const void* key);
NA_HAPI  NABool naTestKeyNodeContainQuadDouble(NATreeNode* parentnode, const void* key);
NA_HAPI  NABool naTestKeyLeafContainQuadDouble(NATreeLeaf* leaf, const void* key);

NA_HAPI  void naDestructTreeNodeQuad(NATreeNode* node);
NA_HAPI  void naDestructTreeLeafQuad(NATreeLeaf* leaf);

NA_HAPI  NATreeNode* naLocateBubbleQuad(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI  NATreeNode* naRemoveLeafQuad(NATree* tree, NATreeLeaf* leaf);
NA_HAPI  NATreeLeaf* naInsertLeafQuad(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);

NA_HIAPI NATreeItem* naGetQuadNodeItem(NATreeQuadNode* quadnode);





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
