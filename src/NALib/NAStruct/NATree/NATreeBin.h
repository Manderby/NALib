
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


typedef struct NATreeBinNode NATreeBinNode;
struct NATreeBinNode{
  NATreeNode node;
  union{
    double d;
    NAInt i;
  } key;
  NATreeItem* childs[2];
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinNode);

typedef struct NATreeBinLeaf NATreeBinLeaf;
struct NATreeBinLeaf{
  NATreeLeaf leaf;
  union{
    double d;
    NAInt i;
  } key;
  NAPtr data;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinLeaf);

NA_HAPI NAInt naGetKeyIndexBinDouble(const void* basekey, const void* testkey, const void* data);
NA_HAPI NABool naEqualKeyBinDouble(const void* key1, const void* key2);
NA_HAPI void naAssignKeyBinDouble(void* dst, const void* src);
NA_HAPI NABool naTestKeyBinDouble(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI NAInt naGetKeyIndexBinNAInt(const void* basekey, const void* key, const void* data);
NA_HAPI NABool naEqualKeyBinNAInt(const void* key1, const void* key2);
NA_HAPI void naAssignKeyBinNAInt(void* dst, const void* src);
NA_HAPI NABool naTestKeyBinNAInt(const void* leftlimit, const void* rightlimit, const void* key);

NA_HAPI void naDestructTreeNodeBin(NATree* tree, NATreeNode* node, NABool recursive);
NA_HAPI NATreeLeaf* naConstructTreeLeafBin(NATree* tree, const void* key, NAPtr data);
NA_HAPI void naDestructTreeLeafBin(NATree* tree, NATreeLeaf* leaf);

NA_HAPI NATreeNode* naLocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI NATreeLeaf* naLocateCaptureBin(const NATree* tree, NATreeNode* node, const void* key, NABool* matchfound);
NA_HAPI NAInt naGetChildIndexBin(NATreeNode* parent, NATreeItem* child);
NA_HAPI NAInt naGetChildKeyIndexBin(const NATree* tree, NATreeNode* parent, const void* key);
NA_HAPI NATreeItem* naGetChildBin(NATreeNode* parent, NAInt childindx);
NA_HAPI NATreeNode* naRemoveLeafBin(NATree* tree, NATreeLeaf* leaf);
NA_HAPI NATreeLeaf* naInsertLeafBin(NATree* tree, NATreeLeaf* existingleaf, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);
NA_HAPI const void* naGetLeafKeyBin(NATreeLeaf* leaf);
NA_HAPI NAPtr naGetLeafDataBin(NATreeLeaf* leaf);
NA_HAPI NAPtr naGetNodeDataBin(NATreeNode* node);

NA_HAPI void naInitNodeAVL(NATreeBinNode* binnode);
NA_HAPI void naGrowAVL(NATree* tree, NATreeBinNode* binnode, NAInt childindx);
NA_HAPI void naShrinkAVL(NATree* tree, NATreeBinNode* binnode, NAInt childindx);

NA_HIAPI NATreeItem* naGetBinNodeItem(NATreeBinNode* binnode);




NA_HIDEF NATreeItem* naGetBinNodeItem(NATreeBinNode* binnode){
  return &(binnode->node.item);
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
