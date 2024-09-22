
#include "../../NATree.h"
#include "../../../NAUtility/NAKey.h"
#include "NATreeOct.h"



struct NATreeOctNode{
  NATreeNode node;
  NATreeItem* childs[8];  // must come right after the node.
  NAVertex origin;
  NAPtr userData;
  NAInt childExponent;
};
NA_RUNTIME_TYPE(NATreeOctNode, NA_NULL, NA_FALSE);

struct NATreeOctLeaf{
  NATreeLeaf leaf;
  NAVertex origin;  // todo remove this maybe?
  NAPtr userData;
  NAInt leafExponent; // todo remove this maybe?
};
NA_RUNTIME_TYPE(NATreeOctLeaf, NA_NULL, NA_FALSE);



#include <stddef.h>
#define NODE_CHILDS_OFFSET_OCT     offsetof(NATreeOctNode, childs)
#define LEAF_KEY_OFFSET_OCT        offsetof(NATreeOctLeaf, origin)
#define NODE_KEY_OFFSET_OCT        offsetof(NATreeOctNode, origin)
#define LEAF_USERDATA_OFFSET_OCT   offsetof(NATreeOctLeaf, userData)
#define NODE_USERDATA_OFFSET_OCT   offsetof(NATreeOctNode, userData)



NA_HDEF void na_fillTreeNodeOctABI(NATreeNodeABI* abi) {
  #if NA_DEBUG
    abi->sizeofNode = sizeof(NATreeOctNode);
    abi->sizeofLeaf = sizeof(NATreeOctLeaf);
    abi->nodeChildsOffset = NODE_CHILDS_OFFSET_OCT;
  #endif

  abi->leafKeyOffset           = LEAF_KEY_OFFSET_OCT;
  abi->nodeKeyOffset           = NODE_KEY_OFFSET_OCT;
  abi->leafUserDataOffset      = LEAF_USERDATA_OFFSET_OCT;
  abi->nodeUserDataOffset      = NODE_USERDATA_OFFSET_OCT;
}



NA_HIDEF NATreeNode* na_GetOctNodeNode(NATreeOctNode* octNode) {
  return &octNode->node;
}
NA_HIDEF NATreeLeaf* na_GetOctLeafLeaf(NATreeOctLeaf* octLeaf) {
  return &octLeaf->leaf;
}
NA_HIDEF NATreeItem* na_GetOctNodeItem(NATreeOctNode* octNode) {
  return na_GetTreeNodeItem(na_GetOctNodeNode(octNode));
}
NA_HIDEF void* na_GetOctNodeKey(NATreeOctNode* octNode) {
  return &octNode->origin;
}
NA_HIDEF void* na_GetOctLeafKey(NATreeOctLeaf* octLeaf) {
  return &octLeaf->origin;
}



NA_HIDEF NABool na_ContainsTreeNodeChildOct(const NAVertex* baseVertex, const NAVertex* testVertex, NAInt childExponent) {
  double childwidth = naMakeDoubleWithExponent((int32)childExponent);
  return (testVertex->x >= baseVertex->x)
      && (testVertex->y >= baseVertex->y)
      && (testVertex->z >= baseVertex->z)
      && (testVertex->x < (baseVertex->x + 2 * childwidth))
      && (testVertex->y < (baseVertex->y + 2 * childwidth))
      && (testVertex->z < (baseVertex->z + 2 * childwidth));
}



// Note that this function is not entirely deterministic. Depending on the
// order the leafes are created, the resulting root of the whole tree might
// be placed at a different origin. To make this completely deterministic,
// one would force the origin to align to a predefined pattern which is
// - due to the cyclic manner of the parent - a little complicated and
// frankly should be not important in any case. Therefore... fuckit.
NA_HDEF NAVertex na_GetTreeNewRootOriginOct(NAInt childExponent, NAVertex childorigin) {
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.

  double childwidth = naMakeDoubleWithExponent((int32)childExponent);
  NAVertex parentorigin = childorigin;
  int16 cycle = ((childExponent % 8) + 8 ) % 8;
  if(cycle & 1) { parentorigin.x -= childwidth; }
  if(cycle & 2) { parentorigin.y -= childwidth; }
  if(cycle & 4) { parentorigin.z -= childwidth; }

  return parentorigin;
}



NA_HDEF NAVertex na_GetChildOriginOct(NAVertex parentorigin, size_t childIndex, NAInt childExponent){
  double childwidth = naMakeDoubleWithExponent((int32)childExponent);
  NAVertex childorigin = parentorigin;
  if(childIndex & 1) { childorigin.x += childwidth; }
  if(childIndex & 2) { childorigin.y += childwidth; }
  if(childIndex & 4) { childorigin.z += childwidth; }
  return childorigin;
}



NA_HDEF NATreeOctNode* na_NewTreeNodeOct(const NATreeConfiguration* config, NAVertex origin, NAInt childExponent) {
  NATreeOctNode* octNode = naNew(NATreeOctNode);
  na_InitTreeNode(na_GetOctNodeNode(octNode), &origin, config);

  // Node-specific initialization
  octNode->childExponent = childExponent;
  
  return octNode;
}



NA_HIDEF NAVertex na_GetOctTreeAlignedVertex(NAInt leafExponent, const NAVertex* pos) {
  double leafwidth = naMakeDoubleWithExponent((int32)leafExponent);
  NABox leafalign = naMakeBox(naMakeVertex(0, 0, 0), naMakeVolume(leafwidth, leafwidth, leafwidth));
  return naMakeVertexWithAlignment(*pos, leafalign);
}



NA_HDEF NATreeLeaf* na_NewTreeLeafOct(const NATreeConfiguration* config, const void* key, NAPtr content) {
  NAInt leafExponent = naGetTreeConfigurationBaseLeafExponent(config);
  NATreeOctLeaf* octLeaf = naNew(NATreeOctLeaf);
  NAVertex alignedVertex = na_GetOctTreeAlignedVertex(leafExponent, key);
  na_InitTreeLeaf(na_GetOctLeafLeaf(octLeaf), &alignedVertex, content, config);
  octLeaf->leafExponent = leafExponent;
  return na_GetOctLeafLeaf(octLeaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF size_t na_GetChildIndexOctDouble(NATreeNode* parentNode, const void* childKey){
  NATreeOctNode* octNode = (NATreeOctNode*)(parentNode);
  return na_GetKeyIndexOctDouble(na_GetOctNodeKey(octNode), childKey, &octNode->childExponent);
}
// The data parameter contains the leaf exponent of the children.
NA_HDEF size_t na_GetKeyIndexOctDouble(const void* baseKey, const void* testKey, const void* data){
  NAInt childExponent = *((NAInt*)data);
  NAVertex* baseVertex = (NAVertex*)baseKey;
  NAVertex* testVertex = (NAVertex*)testKey;
  size_t index = 0;
  double childwidth;

  #if NA_DEBUG
    if(!na_ContainsTreeNodeChildOct(baseVertex, testVertex, childExponent))
      naError("Vertex lies outside");
  #endif

  childwidth = naMakeDoubleWithExponent((int32)childExponent);
  if(testVertex->x >= baseVertex->x + childwidth) { index |= 1; }
  if(testVertex->y >= baseVertex->y + childwidth) { index |= 2; }
  if(testVertex->z >= baseVertex->z + childwidth) { index |= 4; }
  return index;
}
NA_HDEF NABool na_TestKeyOctDouble(const void* lowerLimit, const void* upperLimit, const void* key) {
  return NA_KEY_OP(LessEqual, NAVertex)(lowerLimit, key) && NA_KEY_OP(Less, NAVertex)(key, upperLimit);
}
NA_HDEF NABool na_TestKeyNodeContainOctDouble(NATreeNode* parentNode, const void* key) {
  NATreeOctNode* octNode = (NATreeOctNode*)(parentNode);
  double childwidth = naMakeDoubleWithExponent((int32)octNode->childExponent);
  NAVertex upperLimit = naMakeVertex(octNode->origin.x + 2 * childwidth, octNode->origin.y + 2 * childwidth, octNode->origin.z + 2 * childwidth);
  return
    NA_KEY_OP(LessEqual, NAVertex)(&octNode->origin, key) &&
    NA_KEY_OP(Less, NAVertex)(key, &upperLimit);
}
NA_HDEF NABool na_TestKeyLeafContainOctDouble(NATreeLeaf* leaf, const void* key) {
  NATreeOctLeaf* octLeaf = (NATreeOctLeaf*)(leaf);
  double leafwidth = naMakeDoubleWithExponent((int32)octLeaf->leafExponent);
  NAVertex upperLimit = naMakeVertex(octLeaf->origin.x + leafwidth, octLeaf->origin.y + leafwidth, octLeaf->origin.z + leafwidth);
  return
    NA_KEY_OP(LessEqual, NAVertex)(&octLeaf->origin, key) &&
    NA_KEY_OP(Less, NAVertex)(key, &upperLimit);
}
NA_HDEF NABool na_TestKeyNodeOverlapOctDouble(NATreeNode* parentNode, const void* lowerKey, const void* upperKey) {
  NATreeOctNode* octNode = (NATreeOctNode*)(parentNode);
  double childwidth = naMakeDoubleWithExponent((int32)octNode->childExponent);
  NAVertex upperLimit = naMakeVertex(octNode->origin.x + 2 * childwidth, octNode->origin.y + 2 * childwidth, octNode->origin.z + 2 * childwidth);
  NABox nodeBox = naMakeBoxCombination(octNode->origin, upperLimit);
  NABox keyBox = naMakeBoxCombination(*((NAVertex*)lowerKey), *((NAVertex*)upperKey));
  NABox intersection = naMakeBoxIntersection(nodeBox, keyBox);
  return !naIsBoxEmpty(intersection);
}
NA_HDEF NABool na_TestKeyLeafOverlapOctDouble(NATreeLeaf* leaf, const void* lowerKey, const void* upperKey) {
  NATreeOctLeaf* octLeaf = (NATreeOctLeaf*)(leaf);
  double leafwidth = naMakeDoubleWithExponent((int32)octLeaf->leafExponent);
  NAVertex upperLimit = naMakeVertex(octLeaf->origin.x + 2 * leafwidth, octLeaf->origin.y + 2 * leafwidth, octLeaf->origin.z + 2 * leafwidth);
  NABox nodeBox = naMakeBoxCombination(octLeaf->origin, upperLimit);
  NABox keyBox = naMakeBoxCombination(*((NAVertex*)lowerKey), *((NAVertex*)upperKey));
  NABox intersection = naMakeBoxIntersection(nodeBox, keyBox);
  return !naIsBoxEmpty(intersection);
}



// Callback. Do not call directly.
NA_HDEF void na_DestructTreeNodeOct(NATreeNode* node){
  na_ClearTreeNode(node);
  naDelete(node);
}



// Callback. Do not call directly.
NA_HDEF void na_DestructTreeLeafOct(NATreeLeaf* leaf) {
  naDelete(leaf);
}



NA_HDEF NATreeNode* na_LocateBubbleOctWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* lowerLimit, const void* upperLimit, NATreeItem* previtem) {
  NATreeOctNode* octNode;
  NATreeItem* item;
  
  #if NA_DEBUG
    naError("not implemented yet");
    if(node == NA_NULL)
      naError("node should not be Null");
    if(previtem == NA_NULL)
      naError("prevnode should not be Null");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  octNode = (NATreeOctNode*)node;
  // If we are at a node which stores the key itself, return this node.
//  if(tree->config->keyEqualComparer(origin, na_GetOctNodeKey(octNode))) { return node; }  // Wrong! todo
  // Otherwise, we set the limits dependent on the previous node.
  if(na_GetTreeNodeChildIndex(node, previtem, tree->config) == 1){ // for octtrees, that is of course wrong.
    lowerLimit = na_GetOctNodeKey(octNode);
  }else{
    upperLimit = na_GetOctNodeKey(octNode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerLimit && upperLimit && tree->config->keyTester(lowerLimit, upperLimit, origin)) {
    return node;
  }
  // Otherwise, go up if possible.
  item = na_GetTreeNodeItem(node);
  if(!na_GetTreeItemIsRoot(item)){
    return na_LocateBubbleOctWithLimits(tree, na_GetTreeItemParent(item), origin, lowerLimit, upperLimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* na_LocateBubbleOct(const NATree* tree, NATreeItem* item, const void* key) {
  return na_LocateBubbleOctWithLimits(tree, na_GetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeNode* na_RemoveLeafOct(NATree* tree, NATreeLeaf* leaf) {
  NATreeItem* leafItem = na_GetTreeLeafItem(leaf);
  NATreeNode* parent = na_GetTreeItemParent(leafItem);
  if(na_GetTreeItemIsRoot(leafItem)){
    #if NA_DEBUG
      if(tree->config->flags & NA_TREE_ROOT_NO_LEAF)
        naError("Tree root seems to be a leaf wheres there should be no leafes at the root");
    #endif
    na_ClearTreeRoot(tree);
  }else{
    size_t leafIndex = na_GetTreeNodeChildIndex(parent, leafItem, tree->config);
    #if NA_DEBUG
      if(!na_GetNodeChildIsLeaf(parent, leafIndex, tree->config))
        naError("Child is not marked as a leaf");
      if(!parent)
        naCrash("That is strange. parent should not be Null");
      if(((NATreeOctNode*)parent)->childs[leafIndex] == NA_NULL)
        naError("Child seems to be not linked to the tree");
    #endif
    
    // First, remove the child from the parent.
    ((NATreeOctNode*)parent)->childs[leafIndex] = NA_NULL;

    // Then, find out if there are any parents which can be erased in the
    // process.
    while(parent)
    {
      // Search for a sibling and count if there is more than one.
      NATreeOctNode* grandParent;
      NABool isSiblingLeaf;
      NATreeItem* sibling = NA_NULL;
      NAInt siblingCount = 0;
      NAInt siblingIndex;
      if(((NATreeOctNode*)parent)->childs[0]) {
        siblingIndex = 0;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[1]) {
        siblingIndex = 1;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[2]) {
        siblingIndex = 2;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[3]) {
        siblingIndex = 3;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[4]) {
        siblingIndex = 4;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[5]) {
        siblingIndex = 5;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[6]) {
        siblingIndex = 6;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      if(((NATreeOctNode*)parent)->childs[7]) {
        siblingIndex = 7;
        sibling = ((NATreeOctNode*)parent)->childs[siblingIndex];
        siblingCount++;
      }
      
      if(siblingCount == 0) {
        // There are no more childs left in this parent.
        if(tree->config->flags & NA_TREE_ROOT_NO_LEAF) {
          // The siblingCount being zero can only happen if this parent is the
          // root, having no more leafes. Completely delete it and break.
          #if NA_DEBUG
            if(!na_GetTreeItemIsRoot(na_GetTreeNodeItem(parent)))
              naError("This should be the root");
          #endif
          na_ClearTreeRoot(tree);
          na_DestructTreeNode(parent, NA_FALSE, tree->config);
          break;
        }else{
          #if NA_DEBUG
            naCrash("This should not happen");
          #endif
        }
        
      }else if(siblingCount == 1) {
        // Only 1 sibling left.
        if(tree->config->flags & NA_TREE_ROOT_NO_LEAF){
          if(na_GetTreeItemIsRoot(na_GetTreeNodeItem(parent))){
            // If this is the root, we need to leave this node as it is, as it
            // is desired that the root always is a node.
            break;
          }
        }else{
          // Only one sibling means that the parent node is not useful.
          // Exit this if structure and try to optimize it.
        }

      }else{
        // We have more than 1 sibling left.
        // This node needs to be left as it is.
        break;
      }

      // Reaching here, the parent has only 1 child and shall be optimized away.
      #if NA_DEBUG
        if(siblingCount != 1)
          naError("Parent should have precisely one child");
      #endif
      
      if(siblingCount == 0)
        break; // This is only here for code sanity checks.
      
      grandParent = (NATreeOctNode*)na_GetTreeItemParent(na_GetTreeNodeItem(parent));
      isSiblingLeaf = na_IsTreeItemLeaf(tree, sibling);
      if(!grandParent) {
        // This was the last parent before being a root. Attach the sibling as
        // the new root.
        na_SetTreeRoot(tree, sibling, isSiblingLeaf);
        break;
      }
      
      // There is a grandParent. Simply add the sibling at the place where
      // the parent was and delete the parent.
      size_t parentIndex = na_GetTreeNodeChildIndex(na_GetOctNodeNode(grandParent), na_GetTreeNodeItem(parent), tree->config);
      na_SetTreeNodeChild(na_GetOctNodeNode(grandParent), sibling, parentIndex, isSiblingLeaf, tree->config);
      na_DestructTreeNode(parent, NA_FALSE, tree->config);

      // Repeat for the next parent.
      parent = na_GetOctNodeNode(grandParent);
    }
  }
  
  // The finally, destruct the leaf.
  na_DestructTreeLeaf(leaf, tree->config);
  return parent;
}



NA_HDEF NATreeOctNode* na_CreateTreeParentOct(NATree* tree, NATreeItem* item, NABool isItemLeaf, const void* containedKey) {
  // We haven't found any parent which contains both leafes, therefore we
  // need to add a new parent at the root.
  NAVertex* prevRootOrigin;
  NAVertex newRootOrigin;
  NAInt newRootChildExponent;
  if(isItemLeaf) {
    newRootChildExponent = ((NATreeOctLeaf*)item)->leafExponent - 1;
    prevRootOrigin = na_GetOctLeafKey((NATreeOctLeaf*)item);
  }else{
    NATreeOctNode* rootOctNode = (NATreeOctNode*)item;
    newRootChildExponent = rootOctNode->childExponent;
    prevRootOrigin = na_GetOctNodeKey(rootOctNode);
  }
  tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
  
  while(1) {
    #if NA_DEBUG
      NAVertex testRootOrigin;
    #endif
    newRootChildExponent++;
    #if NA_DEBUG
      if(newRootChildExponent >= NA_ADDRESS_BITS)
        naCrash("childExponent grown too big.");
      testRootOrigin = newRootOrigin;
    #endif
    newRootOrigin = na_GetTreeNewRootOriginOct(newRootChildExponent, newRootOrigin);
    #if NA_DEBUG
      if(!na_ContainsTreeNodeChildOct(&newRootOrigin, &testRootOrigin, newRootChildExponent))
        naError("Expanding root fails to cover the original root");
    #endif
    if(na_ContainsTreeNodeChildOct(&newRootOrigin, (const NAVertex*)containedKey, newRootChildExponent)) {
      break;
    }
  }
  // Reaching here, newRootOrigin and newRootChildExponent
  // denote a new parent containing both the existing child and the new leaf.
  // We create a new node which will become the root.
  return na_NewTreeNodeOct(tree->config, newRootOrigin, newRootChildExponent);
}


NA_HDEF void na_EnlargeTreeRootOct(NATree* tree, const void* containedKey) {
  NAVertex* prevRootOrigin;
  NATreeOctNode* newRoot;
  NAVertex* newRootOrigin;
  NAInt newRootChildExponent;
  
  if(naIsTreeRootLeaf(tree)) {
    prevRootOrigin = na_GetOctLeafKey((NATreeOctLeaf*)tree->root);
  }else{
    prevRootOrigin = na_GetOctNodeKey((NATreeOctNode*)tree->root);
  }

  // First, we create a new root above the existing one
  newRoot = na_CreateTreeParentOct(tree, tree->root, naIsTreeRootLeaf(tree), containedKey);
  newRootOrigin = na_GetOctNodeKey(newRoot);
  newRootChildExponent = newRoot->childExponent;

  // Now, we attach the previous root to the new root at the appropriate
  // child index.
  size_t prevRootIndex = tree->config->keyIndexGetter(newRootOrigin, prevRootOrigin, &newRootChildExponent);
  na_SetTreeNodeChild(na_GetOctNodeNode(newRoot), tree->root, prevRootIndex, naIsTreeRootLeaf(tree), tree->config);

  // Finally, we set the newRoot to be the root of the tree and mark
  // it to be a node.
  na_SetTreeRoot(tree, na_GetOctNodeItem(newRoot), NA_FALSE);
}


// Oomph. That code is mighty confusing!
NA_HDEF NATreeLeaf* na_InsertLeafOct(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder) {
  NATreeLeaf* newLeaf;
  NA_UNUSED(insertOrder);
  
  #if NA_DEBUG
    if(insertOrder != NA_TREE_LEAF_INSERT_ORDER_KEY)
      naError("Invalid insertOrder");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  
  // Create the new leaf and initialize it.
  newLeaf = na_NewTreeLeafOct(tree->config, key, content);

  if(!existingItem) {
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    na_SetTreeRoot(tree, na_GetTreeLeafItem(newLeaf), NA_TRUE);
    
    if(tree->config->flags & NA_TREE_ROOT_NO_LEAF) {
      na_EnlargeTreeRootOct(tree, key);
    }

  }else{
  
    NAVertex* existingChildOrigin;
    NAVertex* newLeafOrigin;
    NATreeOctNode* existingParent;
    NAInt existingParentChildExponent;
    NAVertex* existingParentOrigin;
    NATreeItem* desiredChild;
    
    if(na_IsTreeItemLeaf(tree, existingItem)) {
       existingChildOrigin = na_GetOctLeafKey((NATreeOctLeaf*)existingItem);
    }else{
       existingChildOrigin = na_GetOctNodeKey((NATreeOctNode*)existingItem);
    }
    newLeafOrigin = na_GetOctLeafKey((NATreeOctLeaf*)newLeaf); 
    
    // Search for an already existing parent in the tree which contains both
    // existingChild and newLeaf. Note that existingChild can be a node and
    // hence can itself be the desired parent.
    existingParent = NA_NULL;
    existingParentChildExponent = -1;
    existingParentOrigin = NA_NULL;
    
    if(na_IsTreeItemLeaf(tree, existingItem)) {
      existingParent = (NATreeOctNode*)na_GetTreeItemParent(existingItem);
    }else{
      existingParent = (NATreeOctNode*)existingItem;
    }
    // Bubble throught the tree, trying to search for a good parent.
    while(existingParent) {
      existingParentOrigin = na_GetOctNodeKey(existingParent);
      existingParentChildExponent = existingParent->childExponent;
      if(na_ContainsTreeNodeChildOct(existingParentOrigin, newLeafOrigin, existingParentChildExponent)) {
        break;
      }
      existingParent = (NATreeOctNode*)na_GetTreeItemParent(na_GetOctNodeItem(existingParent));
    }
    
    if(!existingParent) {
      // We haven't found any parent which contains both leafes, therefore we
      // need to add a new parent at the root.
      na_EnlargeTreeRootOct(tree, newLeafOrigin);

      // Now, the newRoot becomes our existing parent.
      existingParent = (NATreeOctNode*)tree->root;
      existingParentOrigin = na_GetOctNodeKey(existingParent);
      existingParentChildExponent = existingParent->childExponent;
    }
    // Now, we are sure, we have an existing parent.
    
    // We test if the desired index is free. If so, we have found the final
    // place for tne new child.
    size_t desiredChildIndex = tree->config->keyIndexGetter(existingParentOrigin, newLeafOrigin, &existingParentChildExponent);
    desiredChild =  existingParent->childs[desiredChildIndex];

    if(!desiredChild) {
      // If the space for the new child if free, take it.
      na_SetTreeNodeChild(na_GetOctNodeNode(existingParent), na_GetTreeLeafItem(newLeaf), desiredChildIndex, NA_TRUE, tree->config);
    }else{
      // If there is a child at the desired index, we have to make some
      // adjustments to the tree.
      // We store the child and the
      // index of the subtree which contains the existing child for later.
      size_t prevExistingChildIndex = tree->config->keyIndexGetter(existingParentOrigin, existingChildOrigin, &existingParentChildExponent);
      NATreeItem* prevExistingChild =  existingParent->childs[prevExistingChildIndex];
      // Starting from the current existingParent, find out what the smallest
      // possible parent would be which contains both existingChild and newLeaf
      // but with different childindex.
      NAVertex smallestParentOrigin = *existingParentOrigin;
      NAInt smallestParentChildExponent = existingParentChildExponent;
      size_t smallestNewLeafIndex;
      while(1){
        size_t smallestExistingChildIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        smallestNewLeafIndex       = tree->config->keyIndexGetter(&smallestParentOrigin, newLeafOrigin,       &smallestParentChildExponent);
        
        if(smallestExistingChildIndex != smallestNewLeafIndex)
          break;
          
        // The two items share the same child. Go further down.
        smallestParentOrigin = na_GetChildOriginOct(smallestParentOrigin, smallestNewLeafIndex, smallestParentChildExponent);
        smallestParentChildExponent--;
        #if NA_DEBUG
          if(smallestParentChildExponent < naGetTreeConfigurationBaseLeafExponent(tree->config))
            naError("child exponent shrank too deep");
        #endif
      }
      
      // Now, smallestParentOrigin and smallestParentChildExponent denote the
      // values for the parent which first differentiates between existingChild
      // and newLeaf.
      // If these exponents differ, we have to create a node between the
      // existingParent and existingChild.
      
      if(smallestParentChildExponent != existingParentChildExponent){
        NATreeOctNode* smallestParent = na_NewTreeNodeOct(tree->config, smallestParentOrigin, smallestParentChildExponent);
        
        // First, attach the previous item to the new parent.
        NABool isPrevExistingChildLeaf = na_IsTreeItemLeaf(tree, prevExistingChild);
        size_t smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        na_SetTreeNodeChild(na_GetOctNodeNode(smallestParent), prevExistingChild, smallestExistingIndex, isPrevExistingChildLeaf, tree->config);
              
        #if NA_DEBUG
          if(smallestParentChildExponent >= existingParent->childExponent)
            naError("exponent must be smaller");
        #endif
        
        // Then, attach the new parent to the existing parent.
        #if NA_DEBUG
          size_t testExistingIndex = tree->config->keyIndexGetter(na_GetOctNodeKey(existingParent), na_GetOctNodeKey(smallestParent), &(existingParent->childExponent));
          if(testExistingIndex != prevExistingChildIndex)
            naError("Newly computed index differs from previously computed index");
        #endif
        na_SetTreeNodeChild(na_GetOctNodeNode(existingParent), na_GetOctNodeItem(smallestParent), prevExistingChildIndex, NA_FALSE, tree->config);
        
        existingParent = smallestParent;
      }
      
      // Now existingParent contains both items and is minimal. Attach the newLeaf
      // at its appropriate place.
      #if NA_DEBUG
        if(existingParent->childs[smallestNewLeafIndex])
          naError("Child is already occupied");
      #endif
      na_SetTreeNodeChild(na_GetOctNodeNode(existingParent), na_GetTreeLeafItem(newLeaf), smallestNewLeafIndex, NA_TRUE, tree->config);
    }
  }

  return newLeaf;
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
