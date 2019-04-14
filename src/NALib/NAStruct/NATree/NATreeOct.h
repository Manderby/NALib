
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NACoord.h"

typedef struct NATreeOctNode NATreeOctNode;
struct NATreeOctNode{
  NATreeNode node;
  NAVertex origin;
  NATreeItem* childs[8];
  NAPtr userdata;
  NAInt childexponent;
};
NA_EXTERN_RUNTIME_TYPE(NATreeOctNode);

typedef struct NATreeOctLeaf NATreeOctLeaf;
struct NATreeOctLeaf{
  NATreeLeaf leaf;
  NAVertex origin;  // todo remove this maybe?
  NAPtr userdata;
  NAInt leafexponent; // todo remove this maybe?
};
NA_EXTERN_RUNTIME_TYPE(NATreeOctLeaf);

#include <stddef.h>
#define NODE_CHILDS_OFFSET_OCT     offsetof(NATreeOctNode, childs)
#define LEAF_KEY_OFFSET_OCT        offsetof(NATreeOctLeaf, origin)
#define NODE_KEY_OFFSET_OCT        offsetof(NATreeOctNode, origin)
#define LEAF_USERDATA_OFFSET_OCT   offsetof(NATreeOctLeaf, userdata)
#define NODE_USERDATA_OFFSET_OCT   offsetof(NATreeOctNode, userdata)

NA_HAPI  NAInt naGetChildIndexOctDouble(NATreeNode* parentnode, const void* childkey);
NA_HAPI  NAInt naGetKeyIndexOctDouble(const void* basekey, const void* testkey, const void* data);
NA_HAPI  NABool naEqualKeyOctDouble(const void* key1, const void* key2);
NA_HAPI  NABool naLowerKeyOctDouble(const void* origin1, const void* origin2);
NA_HAPI  NABool naLowerEqualKeyOctDouble(const void* origin1, const void* origin2);
NA_HAPI  void naAssignKeyOctDouble(void* dst, const void* src);
NA_HAPI  void naAddKeyOctDouble(void* dst, const void* src1, const void* src2);
NA_HAPI  NABool naTestKeyOctDouble(const void* lowerlimit, const void* upperlimit, const void* key);
NA_HAPI  NABool naTestKeyContainOctDouble(NATreeNode* parentnode, const void* key);

NA_HAPI  void naDestructTreeNodeOct(NATreeNode* node);
NA_HAPI  void naDestructTreeLeafOct(NATreeLeaf* leaf);

NA_HAPI  NATreeNode* naLocateBubbleOct(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI  NATreeNode* naRemoveLeafOct(NATree* tree, NATreeLeaf* leaf);
NA_HAPI  NATreeLeaf* naInsertLeafOct(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr constructordata, NATreeLeafInsertOrder insertOrder);

NA_HIAPI NATreeItem* naGetOctNodeItem(NATreeOctNode* octnode);





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
