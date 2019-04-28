
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


typedef struct NATreeBinNode NATreeBinNode;
struct NATreeBinNode{
  NATreeNode node;
  NATreeItem* childs[2];
  union{
    double d;
    NAInt i;
  } key;
  NAPtr userdata;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinNode);

typedef struct NATreeBinLeaf NATreeBinLeaf;
struct NATreeBinLeaf{
  NATreeLeaf leaf;
  union{
    double d;
    NAInt i;
  } key;
  NAPtr userdata;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinLeaf);

#include <stddef.h>
#define NODE_CHILDS_OFFSET_BIN     offsetof(NATreeBinNode, childs)
#define LEAF_KEY_OFFSET_BIN        offsetof(NATreeBinLeaf, key)
#define NODE_KEY_OFFSET_BIN        offsetof(NATreeBinNode, key)
#define LEAF_USERDATA_OFFSET_BIN   offsetof(NATreeBinLeaf, userdata)
#define NODE_USERDATA_OFFSET_BIN   offsetof(NATreeBinNode, userdata)

NA_HAPI  NAInt naGetChildIndexBinDouble(NATreeNode* parentnode, const void* childkey);
NA_HAPI  NAInt naGetKeyIndexBinDouble(const void* basekey, const void* testkey, const void* data);
NA_HAPI  NABool naEqualKeyBinDouble(const void* key1, const void* key2);
NA_HAPI  NABool naLowerKeyBinDouble(const void* key1, const void* key2);
NA_HAPI  NABool naLowerEqualKeyBinDouble(const void* key1, const void* key2);
NA_HAPI  void naAssignKeyBinDouble(void* dst, const void* src);
NA_HAPI  void naAddKeyBinDouble(void* dst, const void* src1, const void* src2);
NA_HAPI  NABool naTestKeyBinDouble(const void* lowerlimit, const void* upperlimit, const void* key);
NA_HAPI NABool naTestKeyLeafContainBinDouble(NATreeLeaf* leaf, const void* key);

NA_HAPI  NAInt naGetChildIndexBinNAInt(NATreeNode* parentnode, const void* childkey);
NA_HAPI  NAInt naGetKeyIndexBinNAInt(const void* basekey, const void* key, const void* data);
NA_HAPI  NABool naEqualKeyBinNAInt(const void* key1, const void* key2);
NA_HAPI  NABool naLowerKeyBinNAInt(const void* key1, const void* key2);
NA_HAPI  NABool naLowerEqualKeyBinNAInt(const void* key1, const void* key2);
NA_HAPI  void naAssignKeyBinNAInt(void* dst, const void* src);
NA_HAPI  void naAddKeyBinNAInt(void* dst, const void* src1, const void* src2);
NA_HAPI  NABool naTestKeyBinNAInt(const void* lowerlimit, const void* upperlimit, const void* key);
NA_HAPI NABool naTestKeyLeafContainBinNAInt(NATreeLeaf* leaf, const void* key);

NA_HAPI  void naDestructTreeNodeBin(NATreeNode* node);
NA_HAPI  void naDestructTreeLeafBin(NATreeLeaf* leaf);

NA_HAPI  NATreeNode* naLocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI  NATreeNode* naRemoveLeafBin(NATree* tree, NATreeLeaf* leaf);
NA_HAPI  NATreeLeaf* naInsertLeafBin(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);

NA_HAPI  void naInitNodeAVL(NATreeBinNode* binnode);
NA_HAPI  void naGrowAVL(NATree* tree, NATreeBinNode* binnode, NAInt childindx);
NA_HAPI  void naShrinkAVL(NATree* tree, NATreeBinNode* binnode, NAInt childindx);

NA_HIAPI NATreeItem* naGetBinNodeItem(NATreeBinNode* binnode);






NA_HIDEF NATreeNode* naGetBinNodeNode(NATreeBinNode* binnode){
  return &(binnode->node);
}
NA_HIDEF NATreeLeaf* naGetBinLeafLeaf(NATreeBinLeaf* binleaf){
  return &(binleaf->leaf);
}



NA_HIDEF NATreeItem* naGetBinNodeItem(NATreeBinNode* binnode){
  return naGetTreeNodeItem(naGetBinNodeNode(binnode));
}
NA_HIDEF NATreeItem* naGetBinLeafItem(NATreeBinLeaf* binleaf){
  return naGetTreeLeafItem(naGetBinLeafLeaf(binleaf));
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
