
#include "../../NATree.h"
#include "NATreeQuad.h"
#include "../../NAKey.h"



NA_RUNTIME_TYPE(NATreeQuadNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeQuadLeaf, NA_NULL, NA_FALSE);



NA_HIDEF NATreeNode* naGetQuadNodeNode(NATreeQuadNode* quadNode){
  return &(quadNode->node);
}
NA_HIDEF NATreeLeaf* naGetQuadLeafLeaf(NATreeQuadLeaf* quadLeaf){
  return &(quadLeaf->leaf);
}
NA_HIDEF NATreeItem* naGetQuadNodeItem(NATreeQuadNode* quadnode){
  return naGetTreeNodeItem(naGetQuadNodeNode(quadnode));
}
NA_HIDEF void* naGetQuadNodeKey(NATreeQuadNode* quadNode){
  return &(quadNode->origin);
}
NA_HIDEF void* naGetQuadLeafKey(NATreeQuadLeaf* quadLeaf){
  return &(quadLeaf->origin);
}



NA_HIDEF NABool naContainsTreeNodeChildQuad(const NAPos* basePos, const NAPos* testPos, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
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
NA_HDEF NAPos naGetTreeNewRootOriginQuad(NAInt childexponent, NAPos childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.

  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  NAPos parentorigin = childorigin;
  int16 cycle = ((childexponent % 4) + 4 ) % 4;
  if(cycle & 1){parentorigin.x -= childwidth;}
  if(cycle & 2){parentorigin.y -= childwidth;}

  return parentorigin;
}



NA_HDEF NAPos naGetChildOriginQuad(NAPos parentorigin, NAInt childindx, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  NAPos childorigin = parentorigin;
  if(childindx & 1){childorigin.x += childwidth;}
  if(childindx & 2){childorigin.y += childwidth;}
  return childorigin;
}



NA_HDEF NATreeQuadNode* naConstructTreeNodeQuad(const NATreeConfiguration* config, NAPos origin, NAInt childexponent){
  NATreeQuadNode* quadNode = naNew(NATreeQuadNode);
  naInitTreeNode(config, naGetQuadNodeNode(quadNode), &origin);

  // Node-specific initialization
  quadNode->childexponent = childexponent;
  
  return quadNode;
}



NA_HIDEF NAPos naGetQuadTreeAlignedPos(NAInt leafexponent, const NAPos* pos){
  double leafwidth = naMakeDoubleWithExponent((int32)leafexponent);
  NARect leafalign = naMakeRect(naMakePos(0, 0), naMakeSize(leafwidth, leafwidth));
  return naMakePosWithAlignment(*pos, leafalign);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafQuad(const NATreeConfiguration* config, const void* key, NAPtr content){
  NAInt leafexponent = naGetTreeConfigurationBaseLeafExponent(config);
  NATreeQuadLeaf* quadLeaf = naNew(NATreeQuadLeaf);
  NAPos alignedPos = naGetQuadTreeAlignedPos(leafexponent, key);
  naInitTreeLeaf(config, naGetQuadLeafLeaf(quadLeaf), &alignedPos, content);
  quadLeaf->leafexponent = leafexponent;
  return naGetQuadLeafLeaf(quadLeaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt na_GetChildIndexQuadDouble(NATreeNode* parentnode, const void* childkey){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentnode);
  return na_GetKeyIndexQuadDouble(naGetQuadNodeKey(quadNode), childkey, &(quadNode->childexponent));
}
// The data parameter contains the leaf exponent of the children.
NA_HDEF NAInt na_GetKeyIndexQuadDouble(const void* basekey, const void* testkey, const void* data){
  NAInt childexponent = *((NAInt*)data);
  NAPos* basePos = (NAPos*)basekey;
  NAPos* testPos = (NAPos*)testkey;
  NAInt index = 0;
  double childwidth;

  #ifndef NDEBUG
    if(!naContainsTreeNodeChildQuad(basePos, testPos, childexponent))
      naError("Pos lies outside");
  #endif

  childwidth = naMakeDoubleWithExponent((int32)childexponent);
  if(testPos->x >= basePos->x + childwidth){index |= 1;}
  if(testPos->y >= basePos->y + childwidth){index |= 2;}
  return index;
}
NA_HDEF NABool na_TestKeyQuadDouble(const void* lowerlimit, const void* upperlimit, const void* key){
  return NA_KEY_OP(LowerEqual, NAPos)(lowerlimit, key) && NA_KEY_OP(Lower, NAPos)(key, upperlimit);
}
NA_HDEF NABool na_TestKeyNodeContainQuadDouble(NATreeNode* parentnode, const void* key){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentnode);
  double childwidth = naMakeDoubleWithExponent((int32)quadNode->childexponent);
  NAPos upperlimit = naMakePos(quadNode->origin.x + 2 * childwidth, quadNode->origin.y + 2 * childwidth);
  return NA_KEY_OP(LowerEqual, NAPos)(&(quadNode->origin), key) && NA_KEY_OP(Lower, NAPos)(key, &upperlimit);
}
NA_HDEF NABool na_TestKeyLeafContainQuadDouble(NATreeLeaf* leaf, const void* key){
  NATreeQuadLeaf* quadLeaf = (NATreeQuadLeaf*)(leaf);
  double leafwidth = naMakeDoubleWithExponent((int32)quadLeaf->leafexponent);
  NAPos upperlimit = naMakePos(quadLeaf->origin.x + leafwidth, quadLeaf->origin.y + leafwidth);
  return NA_KEY_OP(LowerEqual, NAPos)(&(quadLeaf->origin), key) && NA_KEY_OP(Lower, NAPos)(key, &upperlimit);
}



// Callback. Do not call directly.
NA_HDEF void na_DestructTreeNodeQuad(NATreeNode* node){
  naDelete(node);
}



// Callback. Do not call directly.
NA_HDEF void na_DestructTreeLeafQuad(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HDEF NATreeNode* na_LocateBubbleQuadWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* lowerlimit, const void* upperlimit, NATreeItem* previtem){
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
//  if(tree->config->keyEqualComparer(origin, naGetQuadNodeKey(quadNode))){return node;}  // Wrong! todo
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetTreeNodeChildIndex(tree->config, node, previtem) == 1){ // for quadtrees, that is of course wrong.
    lowerlimit = naGetQuadNodeKey(quadNode);
  }else{
    upperlimit = naGetQuadNodeKey(quadNode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerlimit && upperlimit && tree->config->keyTester(lowerlimit, upperlimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(item)){
    return na_LocateBubbleQuadWithLimits(tree, naGetTreeItemParent(item), origin, lowerlimit, upperlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* na_LocateBubbleQuad(const NATree* tree, NATreeItem* item, const void* key){
  return na_LocateBubbleQuadWithLimits(tree, naGetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HDEF NATreeNode* na_RemoveLeafQuad(NATree* tree, NATreeLeaf* leaf){
  NATreeItem* leafItem = naGetTreeLeafItem(leaf);
  NATreeNode* parent = naGetTreeItemParent(leafItem);
  if(naIsTreeItemRoot(leafItem)){
    #ifndef NDEBUG
      if(tree->config->flags & NA_TREE_ROOT_NO_LEAF)
        naError("Internal error: Tree root seems to be a leaf wheres there should be no leafes at the root");
    #endif
    naClearTreeRoot(tree);
  }else{
    NAInt leafindx = naGetTreeNodeChildIndex(tree->config, parent, leafItem);
    #ifndef NDEBUG
      if(!naIsNodeChildLeaf(parent, leafindx))
        naError("Child is not marked as a leaf");
      if(!parent)
        naCrash("That is strange. parent should not be Null");
      if(((NATreeQuadNode*)parent)->childs[leafindx] == NA_NULL)
        naError("Child seems to be not linked to the tree");
    #endif
    
    // First, remove the child from the parent.
    ((NATreeQuadNode*)parent)->childs[leafindx] = NA_NULL;

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
            if(!naIsTreeItemRoot(naGetTreeNodeItem(parent)))
              naError("This should be the root");
          #endif
          naClearTreeRoot(tree);
          naDestructTreeNode(tree->config, parent, NA_FALSE);
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
          if(naIsTreeItemRoot(naGetTreeNodeItem(parent))){
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

      grandparent = (NATreeQuadNode*)naGetTreeItemParent(naGetTreeNodeItem(parent));
      isSiblingLeaf = naIsTreeItemLeaf(tree, sibling);
      if(!grandparent){
        // This was the last parent before being a root. Attach the sibling as
        // the new root.
        naSetTreeRoot(tree, sibling, isSiblingLeaf);
        break;
      }
      
      // There is a grandparent. Simply add the sibling at the place where
      // the parent was and delete the parent.
      parentindex = naGetTreeNodeChildIndex(tree->config, naGetQuadNodeNode(grandparent), naGetTreeNodeItem(parent));
      naSetTreeNodeChild(naGetQuadNodeNode(grandparent), sibling, parentindex, isSiblingLeaf);
      naDestructTreeNode(tree->config, parent, NA_FALSE);

      // Repeat for the next parent.
      parent = naGetQuadNodeNode(grandparent);
    }
  }
  
  // The finally, destruct the leaf.
  naDestructTreeLeaf(tree->config, leaf);
  return parent;
}



NA_HDEF NATreeQuadNode* naCreateTreeParentQuad(NATree* tree, NATreeItem* item, NABool isItemLeaf, const void* containedKey){
  // We haven't found any parent which contains both leafes, therefore we
  // need to add a new parent at the root.
  NAPos* prevRootOrigin;
  NAPos newRootOrigin;
  NAInt newRootChildExponent;
  if(isItemLeaf){
    newRootChildExponent = ((NATreeQuadLeaf*)item)->leafexponent - 1;
    prevRootOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)item);
  }else{
    newRootChildExponent = ((NATreeQuadNode*)item)->childexponent;
    prevRootOrigin = naGetQuadNodeKey((NATreeQuadNode*)item);
  }
  tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
  
  while(1){
    #ifndef NDEBUG
      NAPos testRootOrigin;
    #endif
    newRootChildExponent++;
    #ifndef NDEBUG
      if(newRootChildExponent >= NA_ADDRESS_BITS)
        naCrash("childexponent grown too big.");
      testRootOrigin = newRootOrigin;
    #endif
    newRootOrigin = naGetTreeNewRootOriginQuad(newRootChildExponent, newRootOrigin);
    #ifndef NDEBUG
      if(!naContainsTreeNodeChildQuad(&newRootOrigin, &testRootOrigin, newRootChildExponent))
        naError("Expanding root fails to cover the original root");
    #endif
    if(naContainsTreeNodeChildQuad(&newRootOrigin, (const NAPos*)containedKey, newRootChildExponent)){break;}
  }
  // Reaching here, newRootOrigin and newRootChildExponent
  // denote a new parent containing both the existing child and the new leaf.
  // We create a new node which will become the root.
  return naConstructTreeNodeQuad(tree->config, newRootOrigin, newRootChildExponent);
}


NA_HDEF void naEnlargeTreeRootQuad(NATree* tree, const void* containedKey){
  NAPos* prevRootOrigin;
  NATreeQuadNode* newRoot;
  NAPos* newRootOrigin;
  NAInt newRootChildExponent;
  NAInt prevRootIndx;
  
  if(naIsTreeRootLeaf(tree)){
    prevRootOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)tree->root);
  }else{
    prevRootOrigin = naGetQuadNodeKey((NATreeQuadNode*)tree->root);
  }

  // First, we create a new root above the existing one
  newRoot = naCreateTreeParentQuad(tree, tree->root, naIsTreeRootLeaf(tree), containedKey);
  newRootOrigin = naGetQuadNodeKey(newRoot);
  newRootChildExponent = newRoot->childexponent;

  // Now, we attach the previous root to the new root at the appropriate
  // child index.
  prevRootIndx = tree->config->keyIndexGetter(newRootOrigin, prevRootOrigin, &newRootChildExponent);
  naSetTreeNodeChild(naGetQuadNodeNode(newRoot), tree->root, prevRootIndx, naIsTreeRootLeaf(tree));

  // Finally, we set the newRoot to be the root of the tree and mark
  // it to be a node.
  naSetTreeRoot(tree, naGetQuadNodeItem(newRoot), NA_FALSE);
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
  newLeaf = naConstructTreeLeafQuad(tree->config, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    naSetTreeRoot(tree, naGetTreeLeafItem(newLeaf), NA_TRUE);
    
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
    
    if(naIsTreeItemLeaf(tree, existingItem)){
       existingChildOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)existingItem);
    }else{
       existingChildOrigin = naGetQuadNodeKey((NATreeQuadNode*)existingItem);
    }
    newLeafOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)newLeaf); 
    
    // Search for an already existing parent in the tree which contains both
    // existingChild and newLeaf. Note that existingChild can be a node and
    // hence can itself be the desired parent.
    existingParent = NA_NULL;
    existingParentChildExponent = -1;
    existingParentOrigin = NA_NULL;
    
    if(naIsTreeItemLeaf(tree, existingItem)){
      existingParent = (NATreeQuadNode*)naGetTreeItemParent(existingItem);
    }else{
      existingParent = (NATreeQuadNode*)existingItem;
    }
    // Bubble throught the tree, trying to search for a good parent.
    while(existingParent){
      existingParentOrigin = naGetQuadNodeKey(existingParent);
      existingParentChildExponent = existingParent->childexponent;
      if(naContainsTreeNodeChildQuad(existingParentOrigin, newLeafOrigin, existingParentChildExponent)){break;}
      existingParent = (NATreeQuadNode*)naGetTreeItemParent(naGetQuadNodeItem(existingParent));
    }
    
    if(!existingParent){
      // We haven't found any parent which contains both leafes, therefore we
      // need to add a new parent at the root.
      naEnlargeTreeRootQuad(tree, newLeafOrigin);

      // Now, the newRoot becomes our existing parent.
      existingParent = (NATreeQuadNode*)tree->root;
      existingParentOrigin = naGetQuadNodeKey(existingParent);
      existingParentChildExponent = existingParent->childexponent;
    }
    // Now, we are sure, we have an existing parent.
    
    // We test if the desired index is free. If so, we have found the final
    // place for tne new child.
    desiredChildIndex = tree->config->keyIndexGetter(existingParentOrigin, newLeafOrigin, &existingParentChildExponent);
    desiredChild =  existingParent->childs[desiredChildIndex];

    if(!desiredChild){
      // If the space for the new child if free, take it.
      naSetTreeNodeChild(naGetQuadNodeNode(existingParent), naGetTreeLeafItem(newLeaf), desiredChildIndex, NA_TRUE);
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
        smallestParentOrigin = naGetChildOriginQuad(smallestParentOrigin, smallestNewLeafIndex, smallestParentChildExponent);
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
        NATreeQuadNode* smallestParent = naConstructTreeNodeQuad(tree->config, smallestParentOrigin, smallestParentChildExponent);
        
        // First, attach the previous item to the new parent.
        NABool isPrevExistingChildLeaf = naIsTreeItemLeaf(tree, prevExistingChild);
        NAInt smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        naSetTreeNodeChild(naGetQuadNodeNode(smallestParent), prevExistingChild, smallestExistingIndex, isPrevExistingChildLeaf);
              
        #ifndef NDEBUG
          if(smallestParentChildExponent >= existingParent->childexponent)
            naError("exponent must be smaller");
        #endif
        
        // Then, attach the new parent to the existing parent.
        #ifndef NDEBUG
          testExistingIndex = tree->config->keyIndexGetter(naGetQuadNodeKey(existingParent), naGetQuadNodeKey(smallestParent), &(existingParent->childexponent));
          if(testExistingIndex != prevExistingChildIndex)
            naError("Newly computed index differs from previously computed index");
        #endif
        naSetTreeNodeChild(naGetQuadNodeNode(existingParent), naGetQuadNodeItem(smallestParent), prevExistingChildIndex, NA_FALSE);
        
        existingParent = smallestParent;
      }
      
      // Now existingParent contains both items and is minimal. Attach the newLeaf
      // at its appropriate place.
      #ifndef NDEBUG
        if(existingParent->childs[smallestNewLeafIndex])
          naError("Child is already occupied");
      #endif
      naSetTreeNodeChild(naGetQuadNodeNode(existingParent), naGetTreeLeafItem(newLeaf), smallestNewLeafIndex, NA_TRUE);
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
