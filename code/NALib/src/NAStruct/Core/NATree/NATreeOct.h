
#ifndef NA_TREE_OCT_INCLUDED
#define NA_TREE_OCT_INCLUDED



NA_PROTOTYPE(NATreeOctNode);
NA_PROTOTYPE(NATreeOctLeaf);

NA_HAPI void na_fillTreeNodeOctABI(NATreeNodeABI* abi);

NA_HAPI  size_t na_GetChildIndexOctDouble(NATreeNode* parentNode, const void* childKey);
NA_HAPI  size_t na_GetKeyIndexOctDouble(const void* baseKey, const void* testKey, const void* data);
NA_HAPI  NABool na_TestKeyOctDouble(const void* lowerLimit, const void* upperLimit, const void* key);
NA_HAPI  NABool na_TestKeyNodeContainOctDouble(NATreeNode* parentNode, const void* key);
NA_HAPI  NABool na_TestKeyLeafContainOctDouble(NATreeLeaf* leaf, const void* key);
NA_HAPI  NABool na_TestKeyNodeOverlapOctDouble(NATreeNode* parentNode, const void* lowerKey, const void* upperKey);
NA_HAPI  NABool na_TestKeyLeafOverlapOctDouble(NATreeLeaf* parentNode, const void* lowerKey, const void* upperKey);

NA_HAPI  void na_DestructTreeNodeOct(NATreeNode* node);
NA_HAPI  void na_DestructTreeLeafOct(NATreeLeaf* leaf);

NA_HAPI  NATreeNode* na_LocateBubbleOct(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI  NATreeNode* na_RemoveLeafOct(NATree* tree, NATreeLeaf* leaf);
NA_HAPI  NATreeLeaf* na_InsertLeafOct(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);



#endif // NA_TREE_OCT_INCLUDED

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
