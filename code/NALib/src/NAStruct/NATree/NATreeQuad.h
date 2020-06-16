
#include "../../NACoord.h"

typedef struct NATreeQuadNode NATreeQuadNode;
struct NATreeQuadNode{
  NATreeNode node;
  NATreeItem* childs[4];
  NAPos origin;
  NAPtr userData;
  NAInt childexponent;
};
NA_EXTERN_RUNTIME_TYPE(NATreeQuadNode);

typedef struct NATreeQuadLeaf NATreeQuadLeaf;
struct NATreeQuadLeaf{
  NATreeLeaf leaf;
  NAPos origin;  // todo remove this maybe?
  NAPtr userData;
  NAInt leafexponent; // todo remove this maybe?
};
NA_EXTERN_RUNTIME_TYPE(NATreeQuadLeaf);

#include <stddef.h>
#define NODE_CHILDS_OFFSET_QUAD     offsetof(NATreeQuadNode, childs)
#define LEAF_KEY_OFFSET_QUAD        offsetof(NATreeQuadLeaf, origin)
#define NODE_KEY_OFFSET_QUAD        offsetof(NATreeQuadNode, origin)
#define LEAF_USERDATA_OFFSET_QUAD   offsetof(NATreeQuadLeaf, userData)
#define NODE_USERDATA_OFFSET_QUAD   offsetof(NATreeQuadNode, userData)

NA_HAPI  NAInt na_GetChildIndexQuadDouble(NATreeNode* parentnode, const void* childkey);
NA_HAPI  NAInt na_GetKeyIndexQuadDouble(const void* basekey, const void* testkey, const void* data);
NA_HAPI  NABool na_TestKeyQuadDouble(const void* lowerLimit, const void* upperLimit, const void* key);
NA_HAPI  NABool na_TestKeyNodeContainQuadDouble(NATreeNode* parentnode, const void* key);
NA_HAPI  NABool na_TestKeyLeafContainQuadDouble(NATreeLeaf* leaf, const void* key);

NA_HAPI  void na_DestructTreeNodeQuad(NATreeNode* node);
NA_HAPI  void na_DestructTreeLeafQuad(NATreeLeaf* leaf);

NA_HAPI  NATreeNode* na_LocateBubbleQuad(const NATree* tree, NATreeItem* item, const void* key);
NA_HAPI  NATreeNode* na_RemoveLeafQuad(NATree* tree, NATreeLeaf* leaf);
NA_HAPI  NATreeLeaf* na_InsertLeafQuad(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder);



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
