
#include "../../NATree.h"
#include "../../NAKey.h"
#include "NATreeOct.h"



NA_RUNTIME_TYPE(NATreeOctNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeOctLeaf, NA_NULL, NA_FALSE);



NA_HIDEF NATreeNode* naGetOctNodeNode(NATreeOctNode* octNode){
  return &(octNode->node);
}
NA_HIDEF NATreeLeaf* naGetOctLeafLeaf(NATreeOctLeaf* octLeaf){
  return &(octLeaf->leaf);
}
NA_HIDEF NATreeItem* naGetOctNodeItem(NATreeOctNode* octnode){
  return naGetTreeNodeItem(naGetOctNodeNode(octnode));
}
NA_HIDEF void* naGetOctNodeKey(NATreeOctNode* octNode){
  return &(octNode->origin);
}
NA_HIDEF void* naGetOctLeafKey(NATreeOctLeaf* octLeaf){
  return &(octLeaf->origin);
}



NA_HIDEF NABool naContainsTreeNodeChildOct(const NAVertex* baseVertex, const NAVertex* testVertex, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
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
NA_HHDEF NAVertex na_GetTreeNewRootOriginOct(NAInt childexponent, NAVertex childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.

  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  NAVertex parentorigin = childorigin;
  int16 cycle = ((childexponent % 8) + 8 ) % 8;
  if(cycle & 1){parentorigin.x -= childwidth;}
  if(cycle & 2){parentorigin.y -= childwidth;}
  if(cycle & 4){parentorigin.z -= childwidth;}

  return parentorigin;
}



NA_HHDEF NAVertex na_GetChildOriginOct(NAVertex parentorigin, NAInt childIndex, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  NAVertex childorigin = parentorigin;
  if(childIndex & 1){childorigin.x += childwidth;}
  if(childIndex & 2){childorigin.y += childwidth;}
  if(childIndex & 4){childorigin.z += childwidth;}
  return childorigin;
}



NA_HHDEF NATreeOctNode* na_ConstructTreeNodeOct(const NATreeConfiguration* config, NAVertex origin, NAInt childexponent){
  NATreeOctNode* octNode = naNew(NATreeOctNode);
  naInitTreeNode(config, naGetOctNodeNode(octNode), &origin);

  // Node-specific initialization
  octNode->childexponent = childexponent;
  
  return octNode;
}



NA_HIDEF NAVertex naGetOctTreeAlignedVertex(NAInt leafexponent, const NAVertex* pos){
  double leafwidth = naMakeDoubleWithExponent((int32)leafexponent);
  NABox leafalign = naMakeBox(naMakeVertex(0, 0, 0), naMakeVolume(leafwidth, leafwidth, leafwidth));
  return naMakeVertexWithAlignment(*pos, leafalign);
}



NA_HHDEF NATreeLeaf* na_ConstructTreeLeafOct(const NATreeConfiguration* config, const void* key, NAPtr content){
  NAInt leafexponent = naGetTreeConfigurationBaseLeafExponent(config);
  NATreeOctLeaf* octLeaf = naNew(NATreeOctLeaf);
  NAVertex alignedVertex = naGetOctTreeAlignedVertex(leafexponent, key);
  naInitTreeLeaf(config, naGetOctLeafLeaf(octLeaf), &alignedVertex, content);
  octLeaf->leafexponent = leafexponent;
  return naGetOctLeafLeaf(octLeaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HHDEF NAInt na_GetChildIndexOctDouble(NATreeNode* parentnode, const void* childkey){
  NATreeOctNode* octNode = (NATreeOctNode*)(parentnode);
  return na_GetKeyIndexOctDouble(naGetOctNodeKey(octNode), childkey, &(octNode->childexponent));
}
// The data parameter contains the leaf exponent of the children.
NA_HHDEF NAInt na_GetKeyIndexOctDouble(const void* basekey, const void* testkey, const void* data){
  NAInt childexponent = *((NAInt*)data);
  NAVertex* baseVertex = (NAVertex*)basekey;
  NAVertex* testVertex = (NAVertex*)testkey;
  NAInt index = 0;
  double childwidth;

  #ifndef NDEBUG
    if(!naContainsTreeNodeChildOct(baseVertex, testVertex, childexponent))
      naError("Vertex lies outside");
  #endif

  childwidth = naMakeDoubleWithExponent((int32)childexponent);
  if(testVertex->x >= baseVertex->x + childwidth){index |= 1;}
  if(testVertex->y >= baseVertex->y + childwidth){index |= 2;}
  if(testVertex->z >= baseVertex->z + childwidth){index |= 4;}
  return index;
}
NA_HHDEF NABool na_TestKeyOctDouble(const void* lowerlimit, const void* upperlimit, const void* key){
  return NA_KEY_OP(LowerEqual, NAVertex)(lowerlimit, key) && NA_KEY_OP(Lower, NAVertex)(key, upperlimit);
}
NA_HHDEF NABool na_TestKeyNodeContainOctDouble(NATreeNode* parentnode, const void* key){
  NATreeOctNode* octNode = (NATreeOctNode*)(parentnode);
  double childwidth = naMakeDoubleWithExponent((int32)octNode->childexponent);
  NAVertex upperlimit = naMakeVertex(octNode->origin.x + 2 * childwidth, octNode->origin.y + 2 * childwidth, octNode->origin.z + 2 * childwidth);
  return NA_KEY_OP(LowerEqual, NAVertex)(&(octNode->origin), key) && NA_KEY_OP(Lower, NAVertex)(key, &upperlimit);
}
NA_HHDEF NABool na_TestKeyLeafContainOctDouble(NATreeLeaf* leaf, const void* key){
  NATreeOctLeaf* octLeaf = (NATreeOctLeaf*)(leaf);
  double leafwidth = naMakeDoubleWithExponent((int32)octLeaf->leafexponent);
  NAVertex upperlimit = naMakeVertex(octLeaf->origin.x + leafwidth, octLeaf->origin.y + leafwidth, octLeaf->origin.z + leafwidth);
  return NA_KEY_OP(LowerEqual, NAVertex)(&(octLeaf->origin), key) && NA_KEY_OP(Lower, NAVertex)(key, &upperlimit);
}



// Callback. Do not call directly.
NA_HHDEF void na_DestructTreeNodeOct(NATreeNode* node){
  naDelete(node);
}



// Callback. Do not call directly.
NA_HHDEF void na_DestructTreeLeafOct(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HHDEF NATreeNode* na_LocateBubbleOctWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* lowerlimit, const void* upperlimit, NATreeItem* previtem){
  NATreeOctNode* octNode;
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
  octNode = (NATreeOctNode*)node;
  // If we are at a node which stores the key itself, return this node.
//  if(tree->config->keyEqualComparer(origin, naGetOctNodeKey(octNode))){return node;}  // Wrong! todo
  // Otherwise, we set the limits dependent on the previous node.
  if(na_GetTreeNodeChildIndex(tree->config, node, previtem) == 1){ // for octtrees, that is of course wrong.
    lowerlimit = naGetOctNodeKey(octNode);
  }else{
    upperlimit = naGetOctNodeKey(octNode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerlimit && upperlimit && tree->config->keyTester(lowerlimit, upperlimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(item)){
    return na_LocateBubbleOctWithLimits(tree, naGetTreeItemParent(item), origin, lowerlimit, upperlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HHDEF NATreeNode* na_LocateBubbleOct(const NATree* tree, NATreeItem* item, const void* key){
  return na_LocateBubbleOctWithLimits(tree, naGetTreeItemParent(item), key, NA_NULL, NA_NULL, item);
}



NA_HHDEF NATreeNode* na_RemoveLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeItem* leafItem = naGetTreeLeafItem(leaf);
  NATreeNode* parent = naGetTreeItemParent(leafItem);
  if(naIsTreeItemRoot(leafItem)){
    #ifndef NDEBUG
      if(tree->config->flags & NA_TREE_ROOT_NO_LEAF)
        naError("Internal error: Tree root seems to be a leaf wheres there should be no leafes at the root");
    #endif
    naClearTreeRoot(tree);
  }else{
    NAInt leafIndex = na_GetTreeNodeChildIndex(tree->config, parent, leafItem);
    #ifndef NDEBUG
      if(!naIsNodeChildLeaf(parent, leafIndex))
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
      NATreeOctNode* grandparent;
      NABool isSiblingLeaf;
      NAInt parentindex;
      NATreeItem* sibling = NA_NULL;
      NAInt siblingCount = 0;
      NAInt siblingIndex;
      if(((NATreeOctNode*)parent)->childs[0]){siblingIndex = 0; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[1]){siblingIndex = 1; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[2]){siblingIndex = 2; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[3]){siblingIndex = 3; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[4]){siblingIndex = 4; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[5]){siblingIndex = 5; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[6]){siblingIndex = 6; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      if(((NATreeOctNode*)parent)->childs[7]){siblingIndex = 7; sibling = ((NATreeOctNode*)parent)->childs[siblingIndex]; siblingCount++;}
      
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
      
      grandparent = (NATreeOctNode*)naGetTreeItemParent(naGetTreeNodeItem(parent));
      isSiblingLeaf = naIsTreeItemLeaf(tree, sibling);
      if(!grandparent){
        // This was the last parent before being a root. Attach the sibling as
        // the new root.
        naSetTreeRoot(tree, sibling, isSiblingLeaf);
        break;
      }
      
      // There is a grandparent. Simply add the sibling at the place where
      // the parent was and delete the parent.
      parentindex = na_GetTreeNodeChildIndex(tree->config, naGetOctNodeNode(grandparent), naGetTreeNodeItem(parent));
      naSetTreeNodeChild(naGetOctNodeNode(grandparent), sibling, parentindex, isSiblingLeaf);
      naDestructTreeNode(tree->config, parent, NA_FALSE);

      // Repeat for the next parent.
      parent = naGetOctNodeNode(grandparent);
    }
  }
  
  // The finally, destruct the leaf.
  naDestructTreeLeaf(tree->config, leaf);
  return parent;
}



NA_HHDEF NATreeOctNode* na_CreateTreeParentOct(NATree* tree, NATreeItem* item, NABool isItemLeaf, const void* containedKey){
  // We haven't found any parent which contains both leafes, therefore we
  // need to add a new parent at the root.
  NAVertex* prevRootOrigin;
  NAVertex newRootOrigin;
  NAInt newRootChildExponent;
  if(isItemLeaf){
    newRootChildExponent = ((NATreeOctLeaf*)item)->leafexponent - 1;
    prevRootOrigin = naGetOctLeafKey((NATreeOctLeaf*)item);
  }else{
    NATreeOctNode* rootOctNode = (NATreeOctNode*)item;
    newRootChildExponent = rootOctNode->childexponent;
    prevRootOrigin = naGetOctNodeKey(rootOctNode);
  }
  tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
  
  while(1){
    #ifndef NDEBUG
      NAVertex testRootOrigin;
    #endif
    newRootChildExponent++;
    #ifndef NDEBUG
      if(newRootChildExponent >= NA_ADDRESS_BITS)
        naCrash("childexponent grown too big.");
      testRootOrigin = newRootOrigin;
    #endif
    newRootOrigin = na_GetTreeNewRootOriginOct(newRootChildExponent, newRootOrigin);
    #ifndef NDEBUG
      if(!naContainsTreeNodeChildOct(&newRootOrigin, &testRootOrigin, newRootChildExponent))
        naError("Expanding root fails to cover the original root");
    #endif
    if(naContainsTreeNodeChildOct(&newRootOrigin, (const NAVertex*)containedKey, newRootChildExponent)){break;}
  }
  // Reaching here, newRootOrigin and newRootChildExponent
  // denote a new parent containing both the existing child and the new leaf.
  // We create a new node which will become the root.
  return na_ConstructTreeNodeOct(tree->config, newRootOrigin, newRootChildExponent);
}


NA_HHDEF void na_EnlargeTreeRootOct(NATree* tree, const void* containedKey){
  NAVertex* prevRootOrigin;
  NATreeOctNode* newRoot;
  NAVertex* newRootOrigin;
  NAInt newRootChildExponent;
  NAInt prevRootIndex;
  
  if(naIsTreeRootLeaf(tree)){
    prevRootOrigin = naGetOctLeafKey((NATreeOctLeaf*)tree->root);
  }else{
    prevRootOrigin = naGetOctNodeKey((NATreeOctNode*)tree->root);
  }

  // First, we create a new root above the existing one
  newRoot = na_CreateTreeParentOct(tree, tree->root, naIsTreeRootLeaf(tree), containedKey);
  newRootOrigin = naGetOctNodeKey(newRoot);
  newRootChildExponent = newRoot->childexponent;

  // Now, we attach the previous root to the new root at the appropriate
  // child index.
  prevRootIndex = tree->config->keyIndexGetter(newRootOrigin, prevRootOrigin, &newRootChildExponent);
  naSetTreeNodeChild(naGetOctNodeNode(newRoot), tree->root, prevRootIndex, naIsTreeRootLeaf(tree));

  // Finally, we set the newRoot to be the root of the tree and mark
  // it to be a node.
  naSetTreeRoot(tree, naGetOctNodeItem(newRoot), NA_FALSE);
}


// Oomph. That code is mighty confusing!
NA_HHDEF NATreeLeaf* na_InsertLeafOct(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* newLeaf;
  NA_UNUSED(insertOrder);
  
  #ifndef NDEBUG
    if(insertOrder != NA_TREE_LEAF_INSERT_ORDER_KEY)
      naError("Invalid insertOrder");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  
  // Create the new leaf and initialize it.
  newLeaf = na_ConstructTreeLeafOct(tree->config, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    naSetTreeRoot(tree, naGetTreeLeafItem(newLeaf), NA_TRUE);
    
    if(tree->config->flags & NA_TREE_ROOT_NO_LEAF){
      na_EnlargeTreeRootOct(tree, key);
    }

  }else{
  
    NAVertex* existingChildOrigin;
    NAVertex* newLeafOrigin;
    NATreeOctNode* existingParent;
    NAInt existingParentChildExponent;
    NAVertex* existingParentOrigin;
    NAInt desiredChildIndex;
    NATreeItem* desiredChild;
    
    if(naIsTreeItemLeaf(tree, existingItem)){
       existingChildOrigin = naGetOctLeafKey((NATreeOctLeaf*)existingItem);
    }else{
       existingChildOrigin = naGetOctNodeKey((NATreeOctNode*)existingItem);
    }
    newLeafOrigin = naGetOctLeafKey((NATreeOctLeaf*)newLeaf); 
    
    // Search for an already existing parent in the tree which contains both
    // existingChild and newLeaf. Note that existingChild can be a node and
    // hence can itself be the desired parent.
    existingParent = NA_NULL;
    existingParentChildExponent = -1;
    existingParentOrigin = NA_NULL;
    
    if(naIsTreeItemLeaf(tree, existingItem)){
      existingParent = (NATreeOctNode*)naGetTreeItemParent(existingItem);
    }else{
      existingParent = (NATreeOctNode*)existingItem;
    }
    // Bubble throught the tree, trying to search for a good parent.
    while(existingParent){
      existingParentOrigin = naGetOctNodeKey(existingParent);
      existingParentChildExponent = existingParent->childexponent;
      if(naContainsTreeNodeChildOct(existingParentOrigin, newLeafOrigin, existingParentChildExponent)){break;}
      existingParent = (NATreeOctNode*)naGetTreeItemParent(naGetOctNodeItem(existingParent));
    }
    
    if(!existingParent){
      // We haven't found any parent which contains both leafes, therefore we
      // need to add a new parent at the root.
      na_EnlargeTreeRootOct(tree, newLeafOrigin);

      // Now, the newRoot becomes our existing parent.
      existingParent = (NATreeOctNode*)tree->root;
      existingParentOrigin = naGetOctNodeKey(existingParent);
      existingParentChildExponent = existingParent->childexponent;
    }
    // Now, we are sure, we have an existing parent.
    
    // We test if the desired index is free. If so, we have found the final
    // place for tne new child.
    desiredChildIndex = tree->config->keyIndexGetter(existingParentOrigin, newLeafOrigin, &existingParentChildExponent);
    desiredChild =  existingParent->childs[desiredChildIndex];

    if(!desiredChild){
      // If the space for the new child if free, take it.
      naSetTreeNodeChild(naGetOctNodeNode(existingParent), naGetTreeLeafItem(newLeaf), desiredChildIndex, NA_TRUE);
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
      NAVertex smallestParentOrigin = *existingParentOrigin;
      NAInt smallestParentChildExponent = existingParentChildExponent;
      NAInt smallestNewLeafIndex = -1;
      while(1){
        NAInt smallestExistingChildIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        smallestNewLeafIndex       = tree->config->keyIndexGetter(&smallestParentOrigin, newLeafOrigin,       &smallestParentChildExponent);
        if(smallestExistingChildIndex != smallestNewLeafIndex){break;}
        // The two items share the same child. Go further down.
        smallestParentOrigin = na_GetChildOriginOct(smallestParentOrigin, smallestNewLeafIndex, smallestParentChildExponent);
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
        NATreeOctNode* smallestParent = na_ConstructTreeNodeOct(tree->config, smallestParentOrigin, smallestParentChildExponent);
        
        // First, attach the previous item to the new parent.
        NABool isPrevExistingChildLeaf = naIsTreeItemLeaf(tree, prevExistingChild);
        NAInt smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        naSetTreeNodeChild(naGetOctNodeNode(smallestParent), prevExistingChild, smallestExistingIndex, isPrevExistingChildLeaf);
              
        #ifndef NDEBUG
          if(smallestParentChildExponent >= existingParent->childexponent)
            naError("exponent must be smaller");
        #endif
        
        // Then, attach the new parent to the existing parent.
        #ifndef NDEBUG
          testExistingIndex = tree->config->keyIndexGetter(naGetOctNodeKey(existingParent), naGetOctNodeKey(smallestParent), &(existingParent->childexponent));
          if(testExistingIndex != prevExistingChildIndex)
            naError("Newly computed index differs from previously computed index");
        #endif
        naSetTreeNodeChild(naGetOctNodeNode(existingParent), naGetOctNodeItem(smallestParent), prevExistingChildIndex, NA_FALSE);
        
        existingParent = smallestParent;
      }
      
      // Now existingParent contains both items and is minimal. Attach the newLeaf
      // at its appropriate place.
      #ifndef NDEBUG
        if(existingParent->childs[smallestNewLeafIndex])
          naError("Child is already occupied");
      #endif
      naSetTreeNodeChild(naGetOctNodeNode(existingParent), naGetTreeLeafItem(newLeaf), smallestNewLeafIndex, NA_TRUE);
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
