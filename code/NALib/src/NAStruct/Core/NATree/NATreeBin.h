
NA_PROTOTYPE(NATreeBinNode);
struct NATreeBinNode{
  NATreeNode node;
  NATreeItem* childs[2];  // must come right after the node.
  union{
    double d;
    int64 i;
  } key;
  NAPtr userData;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinNode);

NA_PROTOTYPE(NATreeBinLeaf);
struct NATreeBinLeaf{
  NATreeLeaf leaf;
  union{
    double d;
    int64 i;
  } key;
  NAPtr userData;
};
NA_EXTERN_RUNTIME_TYPE(NATreeBinLeaf);

#define NODE_CHILDS_OFFSET_BIN     offsetof(NATreeBinNode, childs)
#define LEAF_KEY_OFFSET_BIN        offsetof(NATreeBinLeaf, key)
#define NODE_KEY_OFFSET_BIN        offsetof(NATreeBinNode, key)
#define LEAF_USERDATA_OFFSET_BIN   offsetof(NATreeBinLeaf, userData)
#define NODE_USERDATA_OFFSET_BIN   offsetof(NATreeBinNode, userData)

NA_HAPI size_t na_GetChildIndexBinDouble(NATreeNode* parentNode, const void* childKey);
NA_HAPI size_t na_GetKeyIndexBinDouble(const void* baseKey, const void* testKey, const void* data);
NA_HAPI NABool na_TestKeyBinDouble(const void* lowerLimit, const void* upperLimit, const void* key);
NA_HAPI NABool na_TestKeyLeafContainBinDouble(NATreeLeaf* leaf, const void* key);

NA_HAPI size_t na_GetChildIndexBini32(NATreeNode* parentNode, const void* childKey);
NA_HAPI size_t na_GetKeyIndexBini32(const void* baseKey, const void* key, const void* data);
NA_HAPI NABool na_TestKeyBini32(const void* lowerLimit, const void* upperLimit, const void* key);
NA_HAPI NABool na_TestKeyLeafContainBini32(NATreeLeaf* leaf, const void* key);

NA_HAPI size_t na_GetChildIndexBinu32(NATreeNode* parentNode, const void* childKey);
NA_HAPI size_t na_GetKeyIndexBinu32(const void* baseKey, const void* key, const void* data);
NA_HAPI NABool na_TestKeyBinu32(const void* lowerLimit, const void* upperLimit, const void* key);
NA_HAPI NABool na_TestKeyLeafContainBinu32(NATreeLeaf* leaf, const void* key);

NA_HAPI  void na_DestructTreeNodeBin(NATreeNode* node);
NA_HAPI  void na_DestructTreeLeafBin(NATreeLeaf* leaf);

NA_HAPI  NATreeNode* na_LocateBubbleBin(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI  NATreeNode* na_RemoveLeafBin(NATree* tree, NATreeLeaf* leaf);
NA_HAPI  NATreeLeaf* na_InsertLeafBin(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);

NA_HAPI  void na_InitNodeAVL(NATreeBinNode* binNode);
NA_HAPI  void na_GrowAVL(NATree* tree, NATreeBinNode* binNode, size_t childIndex);
NA_HAPI  void na_ShrinkAVL(NATree* tree, NATreeBinNode* binNode, size_t childIndex);

NA_HIAPI NATreeItem* na_GetBinNodeItem(NATreeBinNode* binNode);






NA_HIDEF NATreeNode* na_GetBinNodeNode(NATreeBinNode* binNode) {
  return &binNode->node;
}
NA_HIDEF NATreeLeaf* na_GetBinLeafLeaf(NATreeBinLeaf* binleaf) {
  return &binleaf->leaf;
}



NA_HIDEF NATreeItem* na_GetBinNodeItem(NATreeBinNode* binNode) {
  return na_GetTreeNodeItem(na_GetBinNodeNode(binNode));
}
NA_HIDEF NATreeItem* na_GetBinLeafItem(NATreeBinLeaf* binleaf) {
  return na_GetTreeLeafItem(na_GetBinLeafLeaf(binleaf));
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
