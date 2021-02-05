
#include "../../NATree.h"
#include "NATreeQuad.h"
#include "../../NAKey.h"



NA_RUNTIME_TYPE(NATreeQuadNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeQuadLeaf, NA_NULL, NA_FALSE);



NA_HIDEF NATreeNode* na_GetQuadNodeNode(NATreeQuadNode* quadNode){
  return &(quadNode->node);
}
NA_HIDEF NATreeLeaf* na_GetQuadLeafLeaf(NATreeQuadLeaf* quadLeaf){
  return &(quadLeaf->leaf);
}
NA_HIDEF NATreeItem* na_GetQuadNodeItem(NATreeQuadNode* quadnode){
  return na_GetTreeNodeItem(na_GetQuadNodeNode(quadnode));
}
NA_HIDEF void* na_GetQuadNodeKey(NATreeQuadNode* quadNode){
  return &(quadNode->origin);
}
NA_HIDEF void* na_GetQuadLeafKey(NATreeQuadLeaf* quadLeaf){
  return &(quadLeaf->origin);
}



NA_HIDEF NABool na_ContainsTreeNodeChildQuad(const NAPos* basePos, const NAPos* testPos, NAInt childExponent){
  double childwidth = naMakeDoubleWithExponent((int32)childExponent);
  return (testPos->x >= basePos->x)
      && (testPos->y >= basePos->y)
      && (testPos->x < (basePos->x + 2 * childwidth))
      && (testPos->y < (basePos->y + 2 * childwidth));
}



// Note that this function is not entirely deterministic. Depending on the
// order the leafes are created, the resulting root of the whole tree might
// be placed at a different origin. To make this completely deterministic,
// one would force the origin to align to a predefined pattern which is
// - due to the cyclic manner of the parent - a little complicated and
// frankly should be not important in any case. Therefore... fuckit.
NA_HDEF NAPos na_GetTreeNewRootOriginQuad(NAInt childExponent, NAPos childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.

  double childwidth = naMakeDoubleWithExponent((int32)childExponent);
  NAPos parentorigin = childorigin;
  int16 cycle = ((childExponent % 4) + 4 ) % 4;
  if(cycle & 1){parentorigin.x -= childwidth;}
  if(cycle & 2){parentorigin.y -= childwidth;}

  return parentorigin;
}



NA_HDEF NAPos na_GetChildOriginQuad(NAPos parentorigin, NAInt childIndex, NAInt childExponent){
  double childwidth = naMakeDoubleWithExponent((int32)childExponent);
  NAPos childorigin = parentorigin;
  if(childIndex & 1){childorigin.x += childwidth;}
  if(childIndex & 2){childorigin.y += childwidth;}
  return childorigin;
}



NA_HDEF NATreeQuadNode* na_ConstructTreeNodeQuad(const NATreeConfiguration* config, NAPos origin, NAInt childExponent){
  NATreeQuadNode* quadNode = naNew(NATreeQuadNode);
  na_InitTreeNode(config, na_GetQuadNodeNode(quadNode), &origin);

  // Node-specific initialization
  quadNode->childExponent = childExponent;
  
  return quadNode;
}



NA_HIDEF NAPos na_GetQuadTreeAlignedPos(NAInt leafExponent, const NAPos* pos){
  double leafwidth = naMakeDoubleWithExponent((int32)leafExponent);
  NARect leafalign = naMakeRect(naMakePos(0, 0), naMakeSize(leafwidth, leafwidth));
  return naMakePosWithAlignment(*pos, leafalign);
}



NA_HDEF NATreeLeaf* na_ConstructTreeLeafQuad(const NATreeConfiguration* config, const void* key, NAPtr content){
  NAInt leafExponent = naGetTreeConfigurationBaseLeafExponent(config);
  NATreeQuadLeaf* quadLeaf = naNew(NATreeQuadLeaf);
  NAPos alignedPos = na_GetQuadTreeAlignedPos(leafExponent, key);
  na_InitTreeLeaf(config, na_GetQuadLeafLeaf(quadLeaf), &alignedPos, content);
  quadLeaf->leafExponent = leafExponent;
  return na_GetQuadLeafLeaf(quadLeaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt na_GetChildIndexQuadDouble(NATreeNode* parentNode, const void* childKey){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentNode);
  return na_GetKeyIndexQuadDouble(na_GetQuadNodeKey(quadNode), childKey, &(quadNode->childExponent));
}
// The data parameter contains the leaf exponent of the children.
NA_HDEF NAInt na_GetKeyIndexQuadDouble(const void* baseKey, const void* testKey, const void* data){
  NAInt childExponent = *((NAInt*)data);
  NAPos* basePos = (NAPos*)baseKey;
  NAPos* testPos = (NAPos*)testKey;
  NAInt index = 0;
  double childwidth;

  #ifndef NDEBUG
    if(!na_ContainsTreeNodeChildQuad(basePos, testPos, childExponent))
      naError("Pos lies outside");
  #endif

  childwidth = naMakeDoubleWithExponent((int32)childExponent);
  if(testPos->x >= basePos->x + childwidth){index |= 1;}
  if(testPos->y >= basePos->y + childwidth){index |= 2;}
  return index;
}
NA_HDEF NABool na_TestKeyQuadDouble(const void* lowerLimit, const void* upperLimit, const void* key){
  return NA_KEY_OP(LowerEqual, NAPos)(lowerLimit, key) && NA_KEY_OP(Lower, NAPos)(key, upperLimit);
}
NA_HDEF NABool na_TestKeyNodeContainQuadDouble(NATreeNode* parentNode, const void* key){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentNode);
  double childwidth = naMakeDoubleWithExponent((int32)quadNode->childExponent);
  NAPos upperLimit = naMakePos(quadNode->origin.x + 2 * childwidth, quadNode->origin.y + 2 * childwidth);
  return
    NA_KEY_OP(LowerEqual, NAPos)(&(quadNode->origin), key) &&
    NA_KEY_OP(Lower, NAPos)(key, &upperLimit);
}
NA_HDEF NABool na_TestKeyLeafContainQuadDouble(NATreeLeaf* leaf, const void* key){
  NATreeQuadLeaf* quadLeaf = (NATreeQuadLeaf*)(leaf);
  double leafwidth = naMakeDoubleWithExponent((int32)quadLeaf->leafExponent);
  NAPos upperLimit = naMakePos(quadLeaf->origin.x + leafwidth, quadLeaf->origin.y + leafwidth);
  return
    NA_KEY_OP(LowerEqual, NAPos)(&(quadLeaf->origin), key) &&
    NA_KEY_OP(Lower, NAPos)(key, &upperLimit);
}
NA_HDEF NABool na_TestKeyNodeOverlapQuadDouble(NATreeNode* parentNode, const void* lowerKey, const void* upperKey){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentNode);
  double childwidth = naMakeDoubleWithExponent((int32)quadNode->childExponent);
  NAPos upperLimit = naMakePos(quadNode->origin.x + 2 * childwidth, quadNode->origin.y + 2 * childwidth);
  NARect nodeRect = naMakeRectWithPosAndPos(quadNode->origin, upperLimit);
  NARect keyRect = naMakeRectWithPosAndPos(*((NAPos*)lowerKey), *((NAPos*)upperKey));
  NARect intersection = naMakeRectWithRectIntersection(nodeRect, keyRect);
  return !naIsRectEmpty(intersection);
}
NA_HDEF NABool na_TestKeyLeafOverlapQuadDouble(NATreeLeaf* leaf, const void* lowerKey, const void* upperKey){
  NATreeQuadLeaf* quadLeaf = (NATreeQuadLeaf*)(leaf);
  double leafwidth = naMakeDoubleWithExponent((int32)quadLeaf->leafExponent);
  NAPos upperLimit = naMakePos(quadLeaf->origin.x + leafwidth, quadLeaf->origin.y + leafwidth);
  NARect leafRect = naMakeRectWithPosAndPos(quadLeaf->origin, upperLimit);
  NARect keyRect = naMakeRectWithPosAndPos(*((NAPos*)lowerKey), *((NAPos*)upperKey));
  NARect intersection = naMakeRectWithRectIntersection(leafRect, keyRect);
  return !naIsRectEmpty(intersection);
}



// Callback. Do not call directly.
NA_HDEF void na_DestructTreeNodeQuad(NATreeNode* node){
  naDelete(node);
}



// Callback. Do not call directly.
NA_HDEF void na_DestructTreeLeafQuad(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HDEF NATreeNode* na_LocateBubbleQuadWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* lowerLimit, const void* upperLimit, NATreeItem* previtem){
  NATreeQuadNode* quadNode;
  NATreeItem* item;
  #ifndef NDEBUG
    naError("not implemented yet");
    if(node == NA_NULL)
      naError("node should not be null");
    if(previtem == NA_NULL)
      naError("prevnode should not be null");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  quadNode = (NATreeQuadNode*)node;
  // If we are at a node which stores the key itself, return this node.
//  if(tree->config->keyEqualComparer(origin, na_GetQuadNodeKey(quadNode))){return node;}  // Wrong! todo
  // Otherwise, we set the limits dependent on the previous node.
  if(na_GetTreeNodeChildIndex(tree->config, node, previtem) == 1){ // for quadtrees, that is of course wrong.
    lowerLimit = na_GetQuadNodeKey(quadNode);
  }else{
    upperLimit = na_GetQuadNodeKey(quadNode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerLimit && upperLimit && tree->config->keyTester(lowerLimit, upperLimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  item = na_GetTreeNodeItem(node);
  if(!na_IsTreeItemRoot(item)){
    return na_LocateBubbleQuadWithLimits(tree, na_GetTreeItemParent(item), origin, lowerLimit, upperLimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* na_LocateBubbleQuad(const NATree* tree, NATreeItem* item, const void* key){
  return na_LocateBubbleQuadWithLimits(tree, na_GetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeNode* na_RemoveLeafQuad(NATree* tree, NATreeLeaf* leaf){
  NATreeItem* leafItem = na_GetTreeLeafItem(leaf);
  NATreeNode* parent = na_GetTreeItemParent(leafItem);
  if(na_IsTreeItemRoot(leafItem)){
    #ifndef NDEBUG
      if(tree->config->flags & NA_TREE_ROOT_NO_LEAF)
        naError("Internal error: Tree root seems to be a leaf wheres there should be no leafes at the root");
    #endif
    na_ClearTreeRoot(tree);
  }else{
    NAInt leafIndex = na_GetTreeNodeChildIndex(tree->config, parent, leafItem);
    #ifndef NDEBUG
      if(!na_IsNodeChildLeaf(parent, leafIndex))
        naError("Child is not marked as a leaf");
      if(!parent)
        naCrash("That is strange. parent should not be Null");
      if(((NATreeQuadNode*)parent)->childs[leafIndex] == NA_NULL)
        naError("Child seems to be not linked to the tree");
    #endif
    
    // First, remove the child from the parent.
    ((NATreeQuadNode*)parent)->childs[leafIndex] = NA_NULL;

    // Then, find out if there are any parents which can be erased in the
    // process.
    while(parent)
    {
      // Search for a sibling and count if there is more than one.
      NATreeQuadNode* grandparent;
      NABool isSiblingLeaf;
      NAInt parentindex;
      NATreeItem* sibling = NA_NULL;
      NAInt siblingCount = 0;
      NAInt siblingIndex;
      if(((NATreeQuadNode*)parent)->childs[0]){siblingIndex = 0; sibling = ((NATreeQuadNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeQuadNode*)parent)->childs[1]){siblingIndex = 1; sibling = ((NATreeQuadNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeQuadNode*)parent)->childs[2]){siblingIndex = 2; sibling = ((NATreeQuadNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeQuadNode*)parent)->childs[3]){siblingIndex = 3; sibling = ((NATreeQuadNode*)parent)->childs[siblingIndex]; siblingCount++;}
      
      if(siblingCount == 0){
        // There are no more childs left in this parent.
        if(tree->config->flags & NA_TREE_ROOT_NO_LEAF){
          // The siblingCount being zero can only happen if this parent is the
          // root, having no more leafes. Completely delete it and break.
          #ifndef NDEBUG
            if(!na_IsTreeItemRoot(na_GetTreeNodeItem(parent)))
              naError("This should be the root");
          #endif
          na_ClearTreeRoot(tree);
          na_DestructTreeNode(tree->config, parent, NA_FALSE);
          parent = NA_NULL;
          break;
        }else{
          #ifndef NDEBUG
            naCrash("This should not happen");
          #endif
        }
        
      }else if(siblingCount == 1){
        // Only 1 sibling left.
        if(tree->config->flags & NA_TREE_ROOT_NO_LEAF){
          if(na_IsTreeItemRoot(na_GetTreeNodeItem(parent))){
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
      #ifndef NDEBUG
        if(siblingCount != 1)
          naError("Parent should have precisely one child");
      #endif
      
      if(siblingCount == 0){break;} // This is only here for code sanity checks.

      grandparent = (NATreeQuadNode*)na_GetTreeItemParent(na_GetTreeNodeItem(parent));
      isSiblingLeaf = na_IsTreeItemLeaf(tree, sibling);
      if(!grandparent){
        // This was the last parent before being a root. Attach the sibling as
        // the new root.
        na_SetTreeRoot(tree, sibling, isSiblingLeaf);
        break;
      }
      
      // There is a grandparent. Simply add the sibling at the place where
      // the parent was and delete the parent.
      parentindex = na_GetTreeNodeChildIndex(tree->config, na_GetQuadNodeNode(grandparent), na_GetTreeNodeItem(parent));
      na_SetTreeNodeChild(na_GetQuadNodeNode(grandparent), sibling, parentindex, isSiblingLeaf);
      na_DestructTreeNode(tree->config, parent, NA_FALSE);

      // Repeat for the next parent.
      parent = na_GetQuadNodeNode(grandparent);
    }
  }
  
  // The finally, destruct the leaf.
  na_DestructTreeLeaf(tree->config, leaf);
  return parent;
}



NA_HDEF NATreeQuadNode* naCreateTreeParentQuad(NATree* tree, NATreeItem* item, NABool isItemLeaf, const void* containedKey){
  // We haven't found any parent which contains both leafes, therefore we
  // need to add a new parent at the root.
  NAPos* prevRootOrigin;
  NAPos newRootOrigin;
  NAInt newRootChildExponent;
  if(isItemLeaf){
    newRootChildExponent = ((NATreeQuadLeaf*)item)->leafExponent - 1;
    prevRootOrigin = na_GetQuadLeafKey((NATreeQuadLeaf*)item);
  }else{
    newRootChildExponent = ((NATreeQuadNode*)item)->childExponent;
    prevRootOrigin = na_GetQuadNodeKey((NATreeQuadNode*)item);
  }
  tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
  
  while(1){
    #ifndef NDEBUG
      NAPos testRootOrigin;
    #endif
    newRootChildExponent++;
    #ifndef NDEBUG
      if(newRootChildExponent >= NA_ADDRESS_BITS)
        naCrash("childExponent grown too big.");
      testRootOrigin = newRootOrigin;
    #endif
    newRootOrigin = na_GetTreeNewRootOriginQuad(newRootChildExponent, newRootOrigin);
    #ifndef NDEBUG
      if(!na_ContainsTreeNodeChildQuad(&newRootOrigin, &testRootOrigin, newRootChildExponent))
        naError("Expanding root fails to cover the original root");
    #endif
    if(na_ContainsTreeNodeChildQuad(&newRootOrigin, (const NAPos*)containedKey, newRootChildExponent)){break;}
  }
  // Reaching here, newRootOrigin and newRootChildExponent
  // denote a new parent containing both the existing child and the new leaf.
  // We create a new node which will become the root.
  return na_ConstructTreeNodeQuad(tree->config, newRootOrigin, newRootChildExponent);
}


NA_HDEF void naEnlargeTreeRootQuad(NATree* tree, const void* containedKey){
  NAPos* prevRootOrigin;
  NATreeQuadNode* newRoot;
  NAPos* newRootOrigin;
  NAInt newRootChildExponent;
  NAInt prevRootIndex;
  
  if(naIsTreeRootLeaf(tree)){
    prevRootOrigin = na_GetQuadLeafKey((NATreeQuadLeaf*)tree->root);
  }else{
    prevRootOrigin = na_GetQuadNodeKey((NATreeQuadNode*)tree->root);
  }

  // First, we create a new root above the existing one
  newRoot = naCreateTreeParentQuad(tree, tree->root, naIsTreeRootLeaf(tree), containedKey);
  newRootOrigin = na_GetQuadNodeKey(newRoot);
  newRootChildExponent = newRoot->childExponent;

  // Now, we attach the previous root to the new root at the appropriate
  // child index.
  prevRootIndex = tree->config->keyIndexGetter(newRootOrigin, prevRootOrigin, &newRootChildExponent);
  na_SetTreeNodeChild(na_GetQuadNodeNode(newRoot), tree->root, prevRootIndex, naIsTreeRootLeaf(tree));

  // Finally, we set the newRoot to be the root of the tree and mark
  // it to be a node.
  na_SetTreeRoot(tree, na_GetQuadNodeItem(newRoot), NA_FALSE);
}


// Oomph. That code is mighty confusing!
NA_HDEF NATreeLeaf* na_InsertLeafQuad(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* newLeaf;
  NA_UNUSED(insertOrder);
  
  #ifndef NDEBUG
    if(insertOrder != NA_TREE_LEAF_INSERT_ORDER_KEY)
      naError("Invalid insertOrder");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  
  // Create the new leaf and initialize it.
  newLeaf = na_ConstructTreeLeafQuad(tree->config, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    na_SetTreeRoot(tree, na_GetTreeLeafItem(newLeaf), NA_TRUE);
    
    if(tree->config->flags & NA_TREE_ROOT_NO_LEAF){
      naEnlargeTreeRootQuad(tree, key);
    }

  }else{
  
    NAPos* existingChildOrigin;
    NAPos* newLeafOrigin;
    NATreeQuadNode* existingParent;
    NAInt existingParentChildExponent;
    NAPos* existingParentOrigin;
    NAInt desiredChildIndex;
    NATreeItem* desiredChild;
    
    if(na_IsTreeItemLeaf(tree, existingItem)){
       existingChildOrigin = na_GetQuadLeafKey((NATreeQuadLeaf*)existingItem);
    }else{
       existingChildOrigin = na_GetQuadNodeKey((NATreeQuadNode*)existingItem);
    }
    newLeafOrigin = na_GetQuadLeafKey((NATreeQuadLeaf*)newLeaf); 
    
    // Search for an already existing parent in the tree which contains both
    // existingChild and newLeaf. Note that existingChild can be a node and
    // hence can itself be the desired parent.
    existingParent = NA_NULL;
    existingParentChildExponent = -1;
    existingParentOrigin = NA_NULL;
    
    if(na_IsTreeItemLeaf(tree, existingItem)){
      existingParent = (NATreeQuadNode*)na_GetTreeItemParent(existingItem);
    }else{
      existingParent = (NATreeQuadNode*)existingItem;
    }
    // Bubble throught the tree, trying to search for a good parent.
    while(existingParent){
      existingParentOrigin = na_GetQuadNodeKey(existingParent);
      existingParentChildExponent = existingParent->childExponent;
      if(na_ContainsTreeNodeChildQuad(existingParentOrigin, newLeafOrigin, existingParentChildExponent)){break;}
      existingParent = (NATreeQuadNode*)na_GetTreeItemParent(na_GetQuadNodeItem(existingParent));
    }
    
    if(!existingParent){
      // We haven't found any parent which contains both leafes, therefore we
      // need to add a new parent at the root.
      naEnlargeTreeRootQuad(tree, newLeafOrigin);

      // Now, the newRoot becomes our existing parent.
      existingParent = (NATreeQuadNode*)tree->root;
      existingParentOrigin = na_GetQuadNodeKey(existingParent);
      existingParentChildExponent = existingParent->childExponent;
    }
    // Now, we are sure, we have an existing parent.
    
    // We test if the desired index is free. If so, we have found the final
    // place for tne new child.
    desiredChildIndex = tree->config->keyIndexGetter(existingParentOrigin, newLeafOrigin, &existingParentChildExponent);
    desiredChild =  existingParent->childs[desiredChildIndex];

    if(!desiredChild){
      // If the space for the new child if free, take it.
      na_SetTreeNodeChild(na_GetQuadNodeNode(existingParent), na_GetTreeLeafItem(newLeaf), desiredChildIndex, NA_TRUE);
    }else{
      // If there is a child at the desired index, we have to make some
      // adjustments to the tree.
      // We store the child and the
      // index of the subtree which contains the existing child for later.
      NAInt prevExistingChildIndex = tree->config->keyIndexGetter(existingParentOrigin, existingChildOrigin, &existingParentChildExponent);
      NATreeItem* prevExistingChild =  existingParent->childs[prevExistingChildIndex];
      // Starting from the current existingParent, find out what the smallest
      // possible parent would be which contains both existingChild and newLeaf
      // but with different childindex.
      NAPos smallestParentOrigin = *existingParentOrigin;
      NAInt smallestParentChildExponent = existingParentChildExponent;
      NAInt smallestNewLeafIndex = -1;
      while(1){
        NAInt smallestExistingChildIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        smallestNewLeafIndex       = tree->config->keyIndexGetter(&smallestParentOrigin, newLeafOrigin,       &smallestParentChildExponent);
        if(smallestExistingChildIndex != smallestNewLeafIndex){break;}
        // The two items share the same child. Go further down.
        smallestParentOrigin = na_GetChildOriginQuad(smallestParentOrigin, smallestNewLeafIndex, smallestParentChildExponent);
        smallestParentChildExponent--;
        #ifndef NDEBUG
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
        #ifndef NDEBUG
          NAInt testExistingIndex;
        #endif
        NATreeQuadNode* smallestParent = na_ConstructTreeNodeQuad(tree->config, smallestParentOrigin, smallestParentChildExponent);
        
        // First, attach the previous item to the new parent.
        NABool isPrevExistingChildLeaf = na_IsTreeItemLeaf(tree, prevExistingChild);
        NAInt smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        na_SetTreeNodeChild(na_GetQuadNodeNode(smallestParent), prevExistingChild, smallestExistingIndex, isPrevExistingChildLeaf);
              
        #ifndef NDEBUG
          if(smallestParentChildExponent >= existingParent->childExponent)
            naError("exponent must be smaller");
        #endif
        
        // Then, attach the new parent to the existing parent.
        #ifndef NDEBUG
          testExistingIndex = tree->config->keyIndexGetter(na_GetQuadNodeKey(existingParent), na_GetQuadNodeKey(smallestParent), &(existingParent->childExponent));
          if(testExistingIndex != prevExistingChildIndex)
            naError("Newly computed index differs from previously computed index");
        #endif
        na_SetTreeNodeChild(na_GetQuadNodeNode(existingParent), na_GetQuadNodeItem(smallestParent), prevExistingChildIndex, NA_FALSE);
        
        existingParent = smallestParent;
      }
      
      // Now existingParent contains both items and is minimal. Attach the newLeaf
      // at its appropriate place.
      #ifndef NDEBUG
        if(existingParent->childs[smallestNewLeafIndex])
          naError("Child is already occupied");
      #endif
      na_SetTreeNodeChild(na_GetQuadNodeNode(existingParent), na_GetTreeLeafItem(newLeaf), smallestNewLeafIndex, NA_TRUE);
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
