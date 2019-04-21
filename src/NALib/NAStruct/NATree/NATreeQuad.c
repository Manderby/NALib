
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeQuad.h"



NA_RUNTIME_TYPE(NATreeQuadNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeQuadLeaf, NA_NULL, NA_FALSE);



NA_HIDEF NATreeNode* naGetQuadNodeNode(NATreeQuadNode* quadnode){
  return &(quadnode->node);
}
NA_HIDEF NATreeLeaf* naGetQuadLeafLeaf(NATreeQuadLeaf* quadleaf){
  return &(quadleaf->leaf);
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



NA_HIDEF NABool naContainsTreeNodeChildQuad(NAPos* basepos, NAPos* testpos, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  return (testpos->x >= basepos->x)
      && (testpos->y >= basepos->y)
      && (testpos->x < (basepos->x + 2 * childwidth))
      && (testpos->y < (basepos->y + 2 * childwidth));
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



NA_HDEF NATreeLeaf* naConstructTreeLeafQuad(const NATreeConfiguration* config, const void* key, NAPtr content){
  NATreeQuadLeaf* quadLeaf = naNew(NATreeQuadLeaf);
  naInitTreeLeaf(config, naGetQuadLeafLeaf(quadLeaf), key, content);
  quadLeaf->leafexponent = naGetTreeConfigurationBaseLeafExponent(config);
  return naGetQuadLeafLeaf(quadLeaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt naGetChildIndexQuadDouble(NATreeNode* parentnode, const void* childkey){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentnode);
  return naGetKeyIndexQuadDouble(naGetQuadNodeKey(quadNode), childkey, &(quadNode->childexponent));
}
// The data parameter contains the leaf exponent of the children.
NA_HDEF NAInt naGetKeyIndexQuadDouble(const void* baseorigin, const void* testorigin, const void* data){
  NAInt childexponent = *((NAInt*)data);
  NAPos* basepos = (NAPos*)baseorigin;
  NAPos* testpos = (NAPos*)testorigin;
  #ifndef NDEBUG
    if(!naContainsTreeNodeChildQuad(basepos, testpos, childexponent))
      naError("Pos lies outside");
  #endif
  NAInt indx = 0;
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  if(testpos->x >= basepos->x + childwidth){indx |= 1;}
  if(testpos->y >= basepos->y + childwidth){indx |= 2;}
  return indx;
}
NA_HDEF NABool naEqualKeyQuadDouble(const void* origin1, const void* origin2){
  NAPos* pos1 = (NAPos*)origin1;
  NAPos* pos2 = (NAPos*)origin2;
  return naEqualPos(*pos1, *pos2);
}
NA_HDEF NABool naLowerKeyQuadDouble(const void* origin1, const void* origin2){
  NAPos* pos1 = (NAPos*)origin1;
  NAPos* pos2 = (NAPos*)origin2;
  return naLowerPos(*pos1, *pos2);
}
NA_HDEF NABool naLowerEqualKeyQuadDouble(const void* origin1, const void* origin2){
  NAPos* pos1 = (NAPos*)origin1;
  NAPos* pos2 = (NAPos*)origin2;
  return naLowerEqualPos(*pos1, *pos2);
}
NA_HDEF void naAssignKeyQuadDouble(void* dst, const void* src){
  naCopyPos(dst, src);
}
NA_HDEF void naAddKeyQuadDouble(void* dst, const void* src1, const void* src2){
  NAPos* dstVertex = (NAPos*)dst; 
  NAPos* src1Vertex = (NAPos*)src1; 
  NAPos* src2Vertex = (NAPos*)src2; 
  dstVertex->x = src1Vertex->x + src2Vertex->x;
  dstVertex->y = src1Vertex->y + src2Vertex->y;
}
NA_HDEF NABool naTestKeyQuadDouble(const void* lowerlimit, const void* upperlimit, const void* key){
  return naLowerEqualKeyQuadDouble(lowerlimit, key) && naLowerKeyQuadDouble(key, upperlimit);
}
NA_HDEF NABool naTestKeyContainQuadDouble(NATreeNode* parentnode, const void* key){
  NATreeQuadNode* quadNode = (NATreeQuadNode*)(parentnode);
  double childwidth = naMakeDoubleWithExponent((int32)quadNode->childexponent);
  NAPos upperlimit = naMakePos(quadNode->origin.x + 2 * childwidth, quadNode->origin.y + 2 * childwidth);
  return naLowerEqualKeyQuadDouble(&(quadNode->origin), key) && naLowerKeyQuadDouble(key, &upperlimit);
}



// Callback. Do not call directly.
NA_HDEF void naDestructTreeNodeQuad(NATreeNode* node){
  naDelete(node);
}



// Callback. Do not call directly.
NA_HDEF void naDestructTreeLeafQuad(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleQuadWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* lowerlimit, const void* upperlimit, NATreeItem* previtem){
  NATreeQuadNode* quadNode;
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
  if(tree->config->keyEqualComparer(origin, naGetQuadNodeKey(quadNode))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetTreeNodeChildIndex(tree->config, node, previtem) == 1){ // for quad trees, this is of course wrong.
    lowerlimit = naGetQuadNodeKey(quadNode);
  }else{
    upperlimit = naGetQuadNodeKey(quadNode);
  }
  // If we know both limits and the key is contained within, return.
  if(lowerlimit && upperlimit && tree->config->keyTester(lowerlimit, upperlimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  NATreeItem* item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(item)){
    return naLocateBubbleQuadWithLimits(tree, naGetTreeItemParent(item), origin, lowerlimit, upperlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* naLocateBubbleQuad(const NATree* tree, NATreeItem* item, const void* origin){
  return naLocateBubbleQuadWithLimits(tree, naGetTreeItemParent(item), origin, NA_NULL, NA_NULL, item);
}



NA_HDEF void naSetTreeNodeChildQuad(NATreeQuadNode* parent, NATreeItem* child, NAInt childIndex, NABool isChildLeaf){
  naSetTreeItemParent(child, naGetQuadNodeNode(parent));
  naMarkNodeChildLeaf(naGetQuadNodeNode(parent), childIndex, isChildLeaf);
  naSetTreeNodeChild(naGetQuadNodeNode(parent), childIndex, child);
}



NA_HDEF NATreeNode* naRemoveLeafQuad(NATree* tree, NATreeLeaf* leaf){
  NATreeItem* leafItem = naGetTreeLeafItem(leaf);
  NATreeNode* parent = naGetTreeItemParent(leafItem);
  if(naIsTreeItemRoot(leafItem)){
    tree->root = NA_NULL;
  }else{
    NAInt leafindx = naGetTreeNodeChildIndex(tree->config, parent, leafItem);
    #ifndef NDEBUG
      if(!naIsNodeChildLeaf(parent, leafindx))
        naError("Child is no leaf");
      if(((NATreeQuadNode*)parent)->childs[leafindx] == NA_NULL)
        naError("No child to erase");
      if(!parent)
        naError("That is strange. parent should not be Null");
    #endif
    ((NATreeQuadNode*)parent)->childs[leafindx] = NA_NULL;

    while(parent)
    {
      // Search for a sibling and count if there is more than one.
      NATreeItem* sibling = NA_NULL;
      NAInt siblingcount = 0;
      NAInt siblingindex;
      if(((NATreeQuadNode*)parent)->childs[0]){siblingindex = 0; sibling = ((NATreeQuadNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeQuadNode*)parent)->childs[1]){siblingindex = 1; sibling = ((NATreeQuadNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeQuadNode*)parent)->childs[2]){siblingindex = 2; sibling = ((NATreeQuadNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeQuadNode*)parent)->childs[3]){siblingindex = 3; sibling = ((NATreeQuadNode*)parent)->childs[siblingindex]; siblingcount++;}
      #ifndef NDEBUG
        if(siblingcount == 0)
          naCrash("This should not happen");
      #endif
      if(siblingcount != 1){break;}
      NATreeQuadNode* grandparent = (NATreeQuadNode*)naGetTreeItemParent(naGetTreeNodeItem(parent));
      NABool isSiblingLeaf = naIsTreeItemLeaf(tree, sibling);
      if(grandparent){
        // There is a grandparent. Simply add the sibling at the place where
        // the parent was and delete the parent.
        NAInt parentindex = naGetTreeNodeChildIndex(tree->config, naGetQuadNodeNode(grandparent), naGetTreeNodeItem(parent));
        naSetTreeNodeChildQuad(grandparent, sibling, parentindex, isSiblingLeaf);
        naDestructTreeNode(tree->config, parent, NA_FALSE);
      }else{
        // This was the last parent before being a root. Attach the sibling as
        // the new root.
        naSetTreeRoot(tree, sibling, isSiblingLeaf);
      }
      parent = naGetQuadNodeNode(grandparent);
    }
  }
  naDestructTreeLeaf(tree->config, leaf);
  return parent;
}



// Oomph. That code is mighty confusing!
NA_HDEF NATreeLeaf* naInsertLeafQuad(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NA_UNUSED(insertOrder);
  #ifndef NDEBUG
    if(insertOrder != NA_TREE_LEAF_INSERT_ORDER_KEY)
      naError("Invalid insertOrder");
    if((tree->config->flags & NA_TREE_CONFIG_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  
  // Create the new leaf and initialize it.
  NATreeLeaf* newLeaf = naConstructTreeLeafQuad(tree->config, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    naSetTreeRoot(tree, naGetTreeLeafItem(newLeaf), NA_TRUE);
    
  }else{
  
    NAPos* existingChildOrigin;
    if(naIsTreeItemLeaf(tree, existingItem)){
       existingChildOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)existingItem);
    }else{
       existingChildOrigin = naGetQuadNodeKey((NATreeQuadNode*)existingItem);
    }
    NAPos* newLeafOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)newLeaf); 
    
    // Search for an already existing parent in the tree which contains both
    // existingChild and newLeaf. Note that existingChild can be a node and
    // hence can itself be the desired parent.
    NATreeQuadNode* existingParent = NA_NULL;
    NAInt existingParentChildExponent = -1;
    NAPos* existingParentOrigin = NA_NULL;
    
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
      NAPos* prevRootOrigin;
      NAPos newRootOrigin;
      NAInt newRootChildExponent;
      if(naIsTreeRootLeaf(tree)){
        newRootChildExponent = ((NATreeQuadLeaf*)(tree->root))->leafexponent - 1;
        prevRootOrigin = naGetQuadLeafKey((NATreeQuadLeaf*)(tree->root));
      }else{
        NATreeQuadNode* rootQuadNode = (NATreeQuadNode*)tree->root;
        newRootChildExponent = rootQuadNode->childexponent;
        prevRootOrigin = naGetQuadNodeKey(rootQuadNode);
      }
      tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
      
      while(1){
        newRootChildExponent++;
        #ifndef NDEBUG
          if(newRootChildExponent >= NA_SYSTEM_ADDRESS_BITS)
            naCrash("childexponent grown too big.");
          NAPos testRootOrigin = newRootOrigin;
        #endif
        newRootOrigin = naGetTreeNewRootOriginQuad(newRootChildExponent, newRootOrigin);
        #ifndef NDEBUG
          if(!naContainsTreeNodeChildQuad(&newRootOrigin, &testRootOrigin, newRootChildExponent))
            naError("Expanding root fails to cover the original root");
        #endif
        if(naContainsTreeNodeChildQuad(&newRootOrigin, newLeafOrigin, newRootChildExponent)){break;}
      }
      // Reaching here, newRootOrigin and newRootChildExponent
      // denote a new parent containing both the existing child and the new leaf.
      // We create a new node which will become the root.
      NATreeQuadNode* newRoot = naConstructTreeNodeQuad(tree->config, newRootOrigin, newRootChildExponent);
      
      // Now, we attach the previous root to the new root at the appropriate
      // child index.
      NAInt prevRootIndx = tree->config->keyIndexGetter(&newRootOrigin, prevRootOrigin, &newRootChildExponent);
      naSetTreeNodeChildQuad(newRoot, tree->root, prevRootIndx, naIsTreeRootLeaf(tree));

      // Finally, we set the newRoot to be the root of the tree and mark
      // it to be a node.
      naSetTreeRoot(tree, naGetQuadNodeItem(newRoot), NA_FALSE);

      // Now, the newRoot becomes our existing parent.
      existingParent = newRoot;
      existingParentOrigin = naGetQuadNodeKey(newRoot);
      existingParentChildExponent = newRoot->childexponent;
    }
    // Now, we are sure, we have an existing parent.
    
    // We test if the desired index is free. If so, we have found the final
    // place for tne new child.
    NAInt desiredChildIndex = tree->config->keyIndexGetter(existingParentOrigin, newLeafOrigin, &existingParentChildExponent);
    NATreeItem* desiredChild =  existingParent->childs[desiredChildIndex];

    if(!desiredChild){
      // If the space for the new child if free, take it.
      naSetTreeNodeChildQuad(existingParent, naGetTreeLeafItem(newLeaf), desiredChildIndex, NA_TRUE);
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
      NAInt smallestExistingChildIndex = -1;
      NAInt smallestNewLeafIndex = -1;
      while(1){
        smallestExistingChildIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
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
        NATreeQuadNode* smallestParent = naConstructTreeNodeQuad(tree->config, smallestParentOrigin, smallestParentChildExponent);
        
        // First, attach the previous item to the new parent.
        NABool isPrevExistingChildLeaf = naIsTreeItemLeaf(tree, prevExistingChild);
        NAInt smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        naSetTreeNodeChildQuad(smallestParent, prevExistingChild, smallestExistingIndex, isPrevExistingChildLeaf);
              
        #ifndef NDEBUG
          if(smallestParentChildExponent >= existingParent->childexponent)
            naError("exponent must be smaller");
        #endif
        
        // Then, attach the new parent to the existing parent.
        #ifndef NDEBUG
          NAInt testExistingIndex = tree->config->keyIndexGetter(naGetQuadNodeKey(existingParent), naGetQuadNodeKey(smallestParent), &(existingParent->childexponent));
          if(testExistingIndex != prevExistingChildIndex)
            naError("Newly computed index differs from previously computed index");
        #endif
        naSetTreeNodeChildQuad(existingParent, naGetQuadNodeItem(smallestParent), prevExistingChildIndex, NA_FALSE);
        
        existingParent = smallestParent;
      }
      
      // Now existingParent contains both items and is minimal. Attach the newLeaf
      // at its appropriate place.
      #ifndef NDEBUG
        if(existingParent->childs[smallestNewLeafIndex])
          naError("Child is already occupied");
      #endif
      naSetTreeNodeChildQuad(existingParent, naGetTreeLeafItem(newLeaf), smallestNewLeafIndex, NA_TRUE);
    }
  }

  return newLeaf;
}



// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
