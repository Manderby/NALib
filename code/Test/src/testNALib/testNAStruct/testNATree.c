
#include "NATest.h"
#include <stdio.h>

#include "NAStruct/NATree.h"



static NABool treeConstructorCalled;
static NABool treeDestructorCalled;
static NABool leafConstructorCalled;
static NABool leafDestructorCalled;
static NABool nodeConstructorCalled;
static NABool nodeDestructorCalled;
static NABool nodeUpdaterCalled;
static int32 debugData;
void treeCon(NAPtr userData){debugData = *(int32*)naGetPtrConst(userData); treeConstructorCalled = NA_TRUE;}
void treeDes(NAPtr userData){debugData = *(int32*)naGetPtrConst(userData); treeDestructorCalled = NA_TRUE;}
NAPtr leafCon(const void* key, NAPtr content){leafConstructorCalled = NA_TRUE; return naMakePtrNull();}
void leafDes(NAPtr leafData){leafDestructorCalled = NA_TRUE;}
NAPtr nodeCon(const void* key){nodeConstructorCalled = NA_TRUE; return naMakePtrNull();}
void nodeDes(NAPtr nodeData){nodeDestructorCalled = NA_TRUE;}
NABool nodeUp(NAPtr parentData, NAPtr* childDatas, NAInt childIndex, NAInt childMask){nodeUpdaterCalled = NA_TRUE; return NA_FALSE;}



void testTreeConfiguration(void){
  naTestGroup("Internal functions"){
    naTestCrash(na_GetTreeConfigurationConst(NA_NULL));
    naTestCrash(na_DeallocTreeConfiguration(NA_NULL));
    naTestCrash(na_RetainTreeConfiguration(NA_NULL));
    
    NATreeConfiguration* config = naCreateTreeConfiguration(0);
    naTest(!na_GetTreeConfigurationConst(config));
    
    naTestVoid(na_RetainTreeConfiguration(config));
    naTest(na_GetTreeConfigurationConst(config));
  
    naTestVoid(na_RetainTreeConfiguration(config));
    naTestVoid(naReleaseTreeConfiguration(config));
   
    naTestVoid(na_DeallocTreeConfiguration(config));
  }

  naTestGroup("Create and release"){
    NATreeConfiguration* config = NA_NULL;
    naTestVoid(config = naCreateTreeConfiguration(0));
    naTestVoid(naReleaseTreeConfiguration(config));
    // Invalid flag combinations:
    // QuadTree and OctTree only work with double keys.
    naTestError(config = naCreateTreeConfiguration(NA_TREE_QUADTREE | NA_TREE_KEY_NOKEY));
    naTestError(config = naCreateTreeConfiguration(NA_TREE_OCTTREE | NA_TREE_KEY_NOKEY));
    // QuadTree and OctTree do not work with AVL balancing.
    naTestError(config = naCreateTreeConfiguration(NA_TREE_QUADTREE | NA_TREE_BALANCE_AVL));
    naTestError(config = naCreateTreeConfiguration(NA_TREE_OCTTREE | NA_TREE_BALANCE_AVL));
    naTestError(config = naCreateTreeConfiguration(3)); // invalid key type
    naTestCrash(naReleaseTreeConfiguration(NA_NULL));
  }

  naTestGroup("Config Setters"){
    double dummyKey = 1.;
    double dummyKey2 = 2.;
    int32 dummyInt = 1234;
    NATree dummyTree;

    NATreeConfiguration* config = naCreateTreeConfiguration(NA_TREE_KEY_DOUBLE);

    naTestVoid(naSetTreeConfigurationUserData(config, naMakePtrWithDataConst(&dummyInt)));
    naTestError(naSetTreeConfigurationUserData(config, naMakePtrWithDataConst(&dummyInt)));
    
    naTestVoid(naSetTreeConfigurationTreeCallbacks(config, treeCon, treeDes));
    naTestVoid(naSetTreeConfigurationLeafCallbacks(config, leafCon, leafDes));
    naTestVoid(naSetTreeConfigurationNodeCallbacks(config, nodeCon, nodeDes, nodeUp));

    naTestGroup("Tree constructor"){
      debugData = 0;
      treeConstructorCalled = NA_FALSE;
      treeDestructorCalled = NA_FALSE;
      naInitTree(&dummyTree, config);
      naTest(treeConstructorCalled);
      naTest(debugData = dummyInt);
    }

    naTestGroup("Leaf callbacks"){
      leafConstructorCalled = NA_FALSE;
      leafDestructorCalled = NA_FALSE;
      NATreeIterator iter = naMakeTreeModifier(&dummyTree);
      naAddTreeKeyConst(&iter, &dummyKey, &dummyInt, NA_TRUE);
      naTest(leafConstructorCalled);
      naClearTreeIterator(&iter);
      naEmptyTree(&dummyTree);
      naTest(leafDestructorCalled);
    }

    naTestGroup("Node callbacks"){
      nodeConstructorCalled = NA_FALSE;
      nodeDestructorCalled = NA_FALSE;
      nodeUpdaterCalled = NA_FALSE;
      NATreeIterator iter2 = naMakeTreeModifier(&dummyTree);
      naAddTreeKeyConst(&iter2, &dummyKey, &dummyInt, NA_TRUE);
      naAddTreeKeyConst(&iter2, &dummyKey2, &dummyInt, NA_TRUE);
      naTest(nodeConstructorCalled);
      naTest(nodeUpdaterCalled);
      naClearTreeIterator(&iter2);
      naEmptyTree(&dummyTree);
      naTest(nodeDestructorCalled);
    }

    naTestGroup("Tree destructor"){
      debugData = 0;
      naClearTree(&dummyTree);
      naTest(treeDestructorCalled);
      naTest(debugData = dummyInt);
    }

    // Test for already set values.
    naTestError(naSetTreeConfigurationTreeCallbacks(config, treeCon, treeDes));
    naTestError(naSetTreeConfigurationLeafCallbacks(config, leafCon, leafDes));
    naTestError(naSetTreeConfigurationNodeCallbacks(config, nodeCon, nodeDes, nodeUp));

    naReleaseTreeConfiguration(config);
  }

  naTestGroup("Quad and Octtree Setters"){
    NATreeConfiguration* noneConfig = naCreateTreeConfiguration(0);
    NATreeConfiguration* quadConfig = naCreateTreeConfiguration(NA_TREE_QUADTREE | NA_TREE_KEY_DOUBLE);
    NATreeConfiguration* octConfig = naCreateTreeConfiguration(NA_TREE_OCTTREE | NA_TREE_KEY_DOUBLE);
    naTestError(naSetTreeConfigurationBaseLeafExponent(noneConfig, 5));
    naTestVoid(naSetTreeConfigurationBaseLeafExponent(quadConfig, 444));
    naTestVoid(naSetTreeConfigurationBaseLeafExponent(octConfig, 888));

    NATree quadTree;
    NATree octTree;
    naInitTree(&quadTree, quadConfig);
    naInitTree(&octTree, octConfig);
    naTestError(naGetTreeConfigurationBaseLeafExponent(noneConfig));
    naTest(naGetTreeConfigurationBaseLeafExponent(quadConfig) == 444);
    naTest(naGetTreeConfigurationBaseLeafExponent(octConfig) == 888);

    naClearTree(&octTree);
    naClearTree(&quadTree);
    naReleaseTreeConfiguration(octConfig);
    naReleaseTreeConfiguration(quadConfig);
    naReleaseTreeConfiguration(noneConfig);
  }
}



void testTreeItems(){
  naTestGroup("Item"){
    naTestCrash(na_InitTreeItem(NA_NULL));
    naTestCrash(na_ClearTreeItem(NA_NULL));
    naTestCrash(na_GetTreeItemParent(NA_NULL));
    naTestCrash(na_SetTreeItemParent(NA_NULL, NA_NULL));
    naTestCrash(na_GetTreeItemIsRoot(NA_NULL));
    naTestCrash(na_GetTreeItemIterCount(NA_NULL));
    naTestCrash(na_IncTreeItemIterCount(NA_NULL));
    naTestCrash(na_DecTreeItemIterCount(NA_NULL));
    
    NATreeItem item;
    naTestVoid(na_InitTreeItem(&item));
    naTestVoid(na_ClearTreeItem(&item));
    
    naTestError(na_GetTreeItemParent(&item));
    naTestError(na_GetTreeItemIsRoot(&item));

    NATreeNode node;
    naTestVoid(na_SetTreeItemParent(&item, &node));
    naTest(na_GetTreeItemParent(&item) == &node);
    
    naTest(na_GetTreeItemIterCount(&item) == 0);
    naTestVoid(na_IncTreeItemIterCount(&item));
    naTest(na_GetTreeItemIterCount(&item) == 1);
    naTestVoid(na_DecTreeItemIterCount(&item));
    naTest(na_GetTreeItemIterCount(&item) == 0);
    naTestError(na_DecTreeItemIterCount(&item));
  }
  
  naTestGroup("Node"){
    NATreeConfiguration* config = naCreateTreeConfiguration(0);
    NATreeNode node;
    NATreeItem item;

    naTestCrash(na_GetTreeNodeItem(NA_NULL));
    naTestCrash(na_GetTreeNodeKey(NA_NULL, NA_NULL));
    naTestCrash(na_GetTreeNodeKey(&node, NA_NULL));
    naTestCrash(na_GetTreeNodeChildStorage(NA_NULL));
    naTestCrash(na_GetTreeNodeData(NA_NULL, NA_NULL));
    naTestCrash(na_SetTreeNodeData(NA_NULL, naMakePtrNull(), NA_NULL));
    naTestCrash(na_SetTreeNodeData(&node, naMakePtrNull(), NA_NULL));
    naTestCrash(na_GetNodeChildIsLeaf(NA_NULL, 0, NA_NULL));
    naTestCrash(na_GetNodeChildIsLeaf(&node, 0, NA_NULL));
    naTestCrash(na_SetNodeChildIsLeaf(NA_NULL, 0, NA_FALSE, NA_NULL));
    naTestCrash(na_SetNodeChildIsLeaf(&node, 0, NA_FALSE, NA_NULL));
    naTestCrash(na_SetNodeChildIsLeaf(&node, 5555, NA_FALSE, config));
    naTestCrash(na_GetTreeNodeChild(NA_NULL, 0, NA_NULL));
    naTestCrash(na_GetTreeNodeChild(&node, 0, NA_NULL));
    naTestCrash(na_GetTreeNodeChild(&node, 5555, config));
    naTestCrash(na_SetTreeNodeChild(NA_NULL, NA_NULL, 0, NA_FALSE, NA_FALSE));
    naTestCrash(na_SetTreeNodeChild(&node, NA_NULL, 0, NA_FALSE, NA_FALSE));
    naTestCrash(na_SetTreeNodeChild(&node, &item, 5555, NA_FALSE, NA_FALSE));
    naTestCrash(na_SetTreeNodeChild(&node, &item, 5555, NA_FALSE, config));
    naTestCrash(na_InitTreeNode(NA_NULL, NA_NULL, NA_NULL));
    naTestCrash(na_InitTreeNode(&node, NA_NULL, NA_NULL));
    naTestCrash(na_ClearTreeNode(NA_NULL));
    naTestCrash(na_DestructTreeNode(NA_NULL, NA_FALSE, NA_NULL));
    naTestCrash(na_DestructTreeNode(&node, NA_FALSE, NA_NULL));
    naTestCrash(na_GetTreeNodeChildIndex(NA_NULL, NA_NULL, NA_NULL));
    naTestCrash(na_GetTreeNodeChildIndex(&node, NA_NULL, NA_NULL));

    // Searching for nullptr as a child is not desired.
    naTestError(na_GetTreeNodeChildIndex(&node, NA_NULL, config));
    // item is not a child of node.
    naTestError(na_GetTreeNodeChildIndex(&node, &item, config));
  }
}




void printNATree(void){
  printf("NATree.h:" NA_NL);

  naPrintMacro(NA_TREE_KEY_NOKEY);
  naPrintMacro(NA_TREE_KEY_DOUBLE);
  naPrintMacro(NA_TREE_KEY_NAINT);
  naPrintMacro(NA_TREE_BALANCE_AVL);
  naPrintMacro(NA_TREE_QUADTREE);
  naPrintMacro(NA_TREE_OCTTREE);
  naPrintMacro(NA_TREE_ROOT_NO_LEAF);

  naPrintMacroDefined(naBeginTreeAccessorIteration(typedElem, tree, lowerLimit, upperLimit, iter));
  naPrintMacroDefined(naBeginTreeMutatorIteration(typedElem, tree, lowerLimit, upperLimit, iter));
  naPrintMacroDefined(naBeginTreeModifierIteration(typedElem, tree, lowerLimit, upperLimit, iter));
  naPrintMacroDefined(naEndTreeIteration(iter));

  naPrintMacro(NA_TREE_SEARCH_PARENT);
  naPrintMacro(NA_TREE_SEARCH_FOUND);
  naPrintMacro(NA_TREE_SEARCH_ABORT);

  printf(NA_NL "NATreeII.h:" NA_NL);

  naPrintMacro(NA_TREE_NODE_MAX_CHILDS);
  naPrintMacroux32(NA_TREE_NODE_CHILDS_MASK);

  naPrintMacro(NA_TREE_NODE_AVL_BITSHIFT);
  naPrintMacroux32(NA_TREE_NODE_AVL_LEFT);
  naPrintMacroux32(NA_TREE_NODE_AVL_EQUAL);
  naPrintMacroux32(NA_TREE_NODE_AVL_RIGHT);
  naPrintMacroux32(NA_TREE_NODE_AVL_MASK);

  naPrintMacro(NA_TREE_CONFIG_KEY_TYPE_MASK);
  naPrintMacro(NA_TREE_CONFIG_STRUCTURE_MASK);

  naPrintMacro(NA_TREE_FLAG_ROOT_IS_LEAF);
  naPrintMacro(NA_TREE_FLAG_TMP_KEY_TAKEN);

  naPrintMacroInt(NA_TREE_NODE_CHILDS_OFFSET);

  printf(NA_NL "NATreeConfigurationII.h:" NA_NL);

  naPrintMacroux16(NA_TREE_CONFIG_DEBUG_FLAG_CONST);

  printf(NA_NL "NATreeIteratorII.h:" NA_NL);

  naPrintMacroux8(NA_TREE_ITERATOR_MODIFIER);
  naPrintMacroux8(NA_TREE_ITERATOR_CLEARED);

  printf(NA_NL);
}



void testNATree(void){
//  naTestFunction(testTreeConfiguration);  
  naTestFunction(testTreeItems);  
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
