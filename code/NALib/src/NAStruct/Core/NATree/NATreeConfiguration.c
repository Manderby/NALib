
#include "../../NATree.h"
#include "../../../NAUtility/NAKey.h"
#include "../../../NAUtility/NAMemory.h"
#include "NATreeBin.h"
#include "NATreeQuad.h"
#include "NATreeOct.h"



NA_HAPI void na_DestroyTreeConfiguration(NATreeConfiguration* config);

NA_RUNTIME_TYPE(NATreeConfiguration, na_DestroyTreeConfiguration, NA_TRUE);



NA_DEF NATreeConfiguration* naCreateTreeConfiguration(uint32 flags){
  // This is just for testing if the implemented nodes "inheriting" from the
  // NATreeNode structure have their childs storage at the correct position.
  #if NA_DEBUG
    int nodeChildsOffset;
  #endif

  // Create the configuration and set everything to zero.
  NATreeConfiguration* config = naCreate(NATreeConfiguration);
  naZeron(config, sizeof(NATreeConfiguration));
  
  config->flags = flags;

  #if NA_DEBUG
    // Just some security measures in case the programmer sees no purpose in
    // setting it.
    config->leafKeyOffset        = NA_TREE_CONFIG_INVALID_OFFSET;
    config->nodeKeyOffset        = NA_TREE_CONFIG_INVALID_OFFSET;
    config->leafUserDataOffset   = NA_TREE_CONFIG_INVALID_OFFSET;
    config->nodeUserDataOffset   = NA_TREE_CONFIG_INVALID_OFFSET;
  #endif
  
  if(flags & NA_TREE_QUADTREE) {
  
    #if NA_DEBUG
      config->sizeofNode = sizeof(NATreeQuadNode);
      config->sizeofLeaf = sizeof(NATreeQuadLeaf);
    #endif
    
    config->childPerNode            = 4;
    switch(flags & NA_TREE_CONFIG_KEY_TYPE_MASK){
    case NA_TREE_KEY_DOUBLE:
      config->childIndexGetter      = na_GetChildIndexQuadDouble;
      config->keyIndexGetter        = na_GetKeyIndexQuadDouble;
      config->keyEqualComparer      = NA_KEY_OP(Equal, NAPos);
      config->keyLessComparer       = NA_KEY_OP(Less, NAPos);
      config->keyLessEqualComparer  = NA_KEY_OP(LessEqual, NAPos);
      config->keyAssigner           = NA_KEY_OP(Assign, NAPos);
      config->keyTester             = na_TestKeyQuadDouble;
      config->keyNodeContainTester  = na_TestKeyNodeContainQuadDouble;
      config->keyLeafContainTester  = na_TestKeyLeafContainQuadDouble;
      config->keyNodeOverlapTester  = na_TestKeyNodeOverlapQuadDouble;
      config->keyLeafOverlapTester  = na_TestKeyLeafOverlapQuadDouble;
      break;
    default:
      #if NA_DEBUG
        naError("Quadtree must have type double. Other options are not implemented yet.");
      #endif
      break;
    }
    if(flags & NA_TREE_BALANCE_AVL) {
      #if NA_DEBUG
        naError("Quadtree can not have AVL balance.");
      #endif
    }
    config->nodeDestructor          = na_DestructTreeNodeQuad;
    config->leafDestructor          = na_DestructTreeLeafQuad;

    config->bubbleLocator           = na_LocateBubbleQuad;
    config->leafRemover             = na_RemoveLeafQuad;
    config->leafInserter            = na_InsertLeafQuad;

    #if NA_DEBUG
      nodeChildsOffset                = NODE_CHILDS_OFFSET_QUAD;
    #endif
    config->leafKeyOffset           = LEAF_KEY_OFFSET_QUAD;
    config->nodeKeyOffset           = NODE_KEY_OFFSET_QUAD;
    config->leafUserDataOffset      = LEAF_USERDATA_OFFSET_QUAD;
    config->nodeUserDataOffset      = NODE_USERDATA_OFFSET_QUAD;

  }else if(flags & NA_TREE_OCTTREE) {
  
    #if NA_DEBUG
      config->sizeofNode = sizeof(NATreeOctNode);
      config->sizeofLeaf = sizeof(NATreeOctLeaf);
    #endif

    config->childPerNode            = 8;
    switch(flags & NA_TREE_CONFIG_KEY_TYPE_MASK){
    case NA_TREE_KEY_DOUBLE:
      config->childIndexGetter      = na_GetChildIndexOctDouble;
      config->keyIndexGetter        = na_GetKeyIndexOctDouble;
      config->keyEqualComparer      = NA_KEY_OP(Equal, NAVertex);
      config->keyLessComparer       = NA_KEY_OP(Less, NAVertex);
      config->keyLessEqualComparer  = NA_KEY_OP(LessEqual, NAVertex);
      config->keyAssigner           = NA_KEY_OP(Assign, NAVertex);
      config->keyTester             = na_TestKeyOctDouble;
      config->keyNodeContainTester  = na_TestKeyNodeContainOctDouble;
      config->keyLeafContainTester  = na_TestKeyLeafContainOctDouble;
      config->keyNodeOverlapTester  = na_TestKeyNodeOverlapOctDouble;
      config->keyLeafOverlapTester  = na_TestKeyLeafOverlapOctDouble;
      break;
    default:
      #if NA_DEBUG
        naError("Octtree must have type double. Other options are not implemented yet.");
      #endif
      break;
    }
    if(flags & NA_TREE_BALANCE_AVL) {
      #if NA_DEBUG
        naError("Octtree can not have AVL balance.");
      #endif
    }
    config->nodeDestructor          = na_DestructTreeNodeOct;
    config->leafDestructor          = na_DestructTreeLeafOct;
    
    config->bubbleLocator           = na_LocateBubbleOct;
    config->leafRemover             = na_RemoveLeafOct;
    config->leafInserter            = na_InsertLeafOct;

    #if NA_DEBUG
      nodeChildsOffset                = NODE_CHILDS_OFFSET_OCT;
    #endif
    config->leafKeyOffset           = LEAF_KEY_OFFSET_OCT;
    config->nodeKeyOffset           = NODE_KEY_OFFSET_OCT;
    config->leafUserDataOffset      = LEAF_USERDATA_OFFSET_OCT;
    config->nodeUserDataOffset      = NODE_USERDATA_OFFSET_OCT;

  }else{

    #if NA_DEBUG
      config->sizeofNode = sizeof(NATreeBinNode);
      config->sizeofLeaf = sizeof(NATreeBinLeaf);
    #endif

    config->childPerNode            = 2;
    switch(flags & NA_TREE_CONFIG_KEY_TYPE_MASK){
    case NA_TREE_KEY_NOKEY:
      config->keyIndexGetter        = NA_NULL;
      config->keyEqualComparer      = NA_NULL;
      config->keyLessComparer       = NA_NULL;
      config->keyLessEqualComparer  = NA_NULL;
      config->keyAssigner           = NA_NULL;
      config->keyTester             = NA_NULL;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = NA_NULL;
      config->keyNodeOverlapTester  = NA_NULL;
      config->keyLeafOverlapTester  = NA_NULL;
      break;
    case NA_TREE_KEY_DOUBLE:
      config->childIndexGetter      = na_GetChildIndexBinDouble;
      config->keyIndexGetter        = na_GetKeyIndexBinDouble;
      config->keyEqualComparer      = NA_KEY_OP(Equal, double);
      config->keyLessComparer       = NA_KEY_OP(Less, double);
      config->keyLessEqualComparer  = NA_KEY_OP(LessEqual, double);
      config->keyAssigner           = NA_KEY_OP(Assign, double);
      config->keyTester             = na_TestKeyBinDouble;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = na_TestKeyLeafContainBinDouble;
      config->keyNodeOverlapTester  = NA_NULL;
      config->keyLeafOverlapTester  = NA_NULL;
      break;
    case NA_TREE_KEY_NAINT:
      config->childIndexGetter      = na_GetChildIndexBinNAInt;
      config->keyIndexGetter        = na_GetKeyIndexBinNAInt;
      config->keyEqualComparer      = NA_KEY_OP(Equal, NAInt);
      config->keyLessComparer       = NA_KEY_OP(Less, NAInt);
      config->keyLessEqualComparer  = NA_KEY_OP(LessEqual, NAInt);
      config->keyAssigner           = NA_KEY_OP(Assign, NAInt);
      config->keyTester             = na_TestKeyBinNAInt;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = na_TestKeyLeafContainBinNAInt;
      config->keyNodeOverlapTester  = NA_NULL;
      config->keyLeafOverlapTester  = NA_NULL;
      break;
    case NA_TREE_KEY_i32:
      config->childIndexGetter      = na_GetChildIndexBini32;
      config->keyIndexGetter        = na_GetKeyIndexBini32;
      config->keyEqualComparer      = NA_KEY_OP(Equal, i32);
      config->keyLessComparer       = NA_KEY_OP(Less, i32);
      config->keyLessEqualComparer  = NA_KEY_OP(LessEqual, i32);
      config->keyAssigner           = NA_KEY_OP(Assign, i32);
      config->keyTester             = na_TestKeyBini32;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = na_TestKeyLeafContainBini32;
      config->keyNodeOverlapTester  = NA_NULL;
      config->keyLeafOverlapTester  = NA_NULL;
      break;
    case NA_TREE_KEY_u32:
      config->childIndexGetter      = na_GetChildIndexBinu32;
      config->keyIndexGetter        = na_GetKeyIndexBinu32;
      config->keyEqualComparer      = NA_KEY_OP(Equal, u32);
      config->keyLessComparer       = NA_KEY_OP(Less, u32);
      config->keyLessEqualComparer  = NA_KEY_OP(LessEqual, u32);
      config->keyAssigner           = NA_KEY_OP(Assign, u32);
      config->keyTester             = na_TestKeyBinu32;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = na_TestKeyLeafContainBinu32;
      config->keyNodeOverlapTester  = NA_NULL;
      config->keyLeafOverlapTester  = NA_NULL;
      break;
    default:
      #if NA_DEBUG
        naError("Invalid key type in flags");
      #endif
      break;
    }

    config->nodeDestructor          = na_DestructTreeNodeBin;
    config->leafDestructor          = na_DestructTreeLeafBin;

    config->bubbleLocator           = na_LocateBubbleBin;
    config->leafRemover             = na_RemoveLeafBin;
    config->leafInserter            = na_InsertLeafBin;
    
    #if NA_DEBUG
      nodeChildsOffset                = NODE_CHILDS_OFFSET_BIN;
    #endif
    config->leafKeyOffset           = LEAF_KEY_OFFSET_BIN;
    config->nodeKeyOffset           = NODE_KEY_OFFSET_BIN;
    config->leafUserDataOffset      = LEAF_USERDATA_OFFSET_BIN;
    config->nodeUserDataOffset      = NODE_USERDATA_OFFSET_BIN;
  }
  
  #if NA_DEBUG
    if(nodeChildsOffset != NA_TREE_NODE_CHILDS_OFFSET)
      naError("The childs storage must come right after the node storage.");
    if(config->childPerNode == 0)
      naError("config has uninitialized childPerNode");
    if(config->childPerNode > NA_TREE_NODE_MAX_CHILDS)
      naError("childPerNode is too high");
  #endif
  
  return config;
}



NA_HDEF void na_DestroyTreeConfiguration(NATreeConfiguration* config) {
  if(config->configData)
    naFree(config->configData);
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
