
#include "../../../NAMath/NACoord.h"

typedef struct NATreeOctNode NATreeOctNode;
struct NATreeOctNode{
  NATreeNode node;
  NATreeItem* childs[8];  // must come right after the node.
  NAVertex origin;
  NAPtr userData;
  NAInt childExponent;
};
NA_EXTERN_RUNTIME_TYPE(NATreeOctNode);

typedef struct NATreeOctLeaf NATreeOctLeaf;
struct NATreeOctLeaf{
  NATreeLeaf leaf;
  NAVertex origin;  // todo remove this maybe?
  NAPtr userData;
  NAInt leafExponent; // todo remove this maybe?
};
NA_EXTERN_RUNTIME_TYPE(NATreeOctLeaf);

#include <stddef.h>
#define NODE_CHILDS_OFFSET_OCT     offsetof(NATreeOctNode, childs)
#define LEAF_KEY_OFFSET_OCT        offsetof(NATreeOctLeaf, origin)
#define NODE_KEY_OFFSET_OCT        offsetof(NATreeOctNode, origin)
#define LEAF_USERDATA_OFFSET_OCT   offsetof(NATreeOctLeaf, userData)
#define NODE_USERDATA_OFFSET_OCT   offsetof(NATreeOctNode, userData)

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
