
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeOct.h"



NA_RUNTIME_TYPE(NATreeOctNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeOctLeaf, NA_NULL, NA_FALSE);



NA_HIDEF NATreeNode* naGetOctNodeNode(NATreeOctNode* octnode){
  return &(octnode->node);
}
NA_HIDEF NATreeLeaf* naGetOctLeafLeaf(NATreeOctLeaf* octleaf){
  return &(octleaf->leaf);
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
#ifndef NDEBUG
  NA_HIDEF NAInt* naGetOctNodeChildExponent(NATreeOctNode* octNode){
    return &(octNode->childexponent);
  }
#endif



NA_HIDEF NABool naContainsTreeNodeChildOct(NAVertex* basevertex, NAVertex* testvertex, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  return (testvertex->x >= basevertex->x)
      && (testvertex->y >= basevertex->y)
      && (testvertex->z >= basevertex->z)
      && (testvertex->x < (basevertex->x + 2 * childwidth))
      && (testvertex->y < (basevertex->y + 2 * childwidth))
      && (testvertex->z < (basevertex->z + 2 * childwidth));
}



// Note that this function is not entirely deterministic. Depending on the
// order the leafes are created, the resulting root of the whole tree might
// be placed at a different origin. To make this completely deterministic,
// one would force the origin to align to a predefined pattern which is
// - due to the cyclic manner of the parent - a little complicated and
// frankly should be not important in any case. Therefore... fuckit.
NA_HDEF NAVertex naGetTreeNewRootOriginOct(NAInt childexponent, NAVertex childorigin){
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



NA_HDEF NAVertex naGetChildOriginOct(NAVertex parentorigin, NAInt childindx, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  NAVertex childorigin = parentorigin;
  if(childindx & 1){childorigin.x += childwidth;}
  if(childindx & 2){childorigin.y += childwidth;}
  if(childindx & 4){childorigin.z += childwidth;}
  return childorigin;
}



NA_HDEF NATreeOctNode* naConstructTreeNodeOct(NATree* tree, NAVertex origin, NAInt childexponent){
  NATreeOctNode* octNode = naNew(NATreeOctNode);
  naInitTreeNode(tree, naGetOctNodeNode(octNode), &origin);

  // Node-specific initialization
  octNode->childexponent = childexponent;
  
  return octNode;
}



NA_HDEF NATreeLeaf* naConstructTreeLeafOct(NATree* tree, const void* key, NAPtr content){
  NATreeOctLeaf* octLeaf = naNew(NATreeOctLeaf);
  naInitTreeLeaf(tree, naGetOctLeafLeaf(octLeaf), key, content);
  octLeaf->leafexponent = naGetTreeConfigurationOcttreeBaseLeafExponent(tree->config);
  return naGetOctLeafLeaf(octLeaf);
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt naGetChildIndexOctDouble(NATreeNode* parentnode, const void* childkey){
  NATreeOctNode* octNode = (NATreeOctNode*)(parentnode);
  return naGetKeyIndexOctDouble(naGetOctNodeKey(octNode), childkey, &(octNode->childexponent));
}
// The data parameter contains the leaf exponent of the children.
NA_HDEF NAInt naGetKeyIndexOctDouble(const void* baseorigin, const void* testorigin, const void* data){
  NAInt childexponent = *((NAInt*)data);
  NAVertex* basevertex = (NAVertex*)baseorigin;
  NAVertex* testvertex = (NAVertex*)testorigin;
  #ifndef NDEBUG
    if(!naContainsTreeNodeChildOct(basevertex, testvertex, childexponent))
      naError("Vertex lies outside");
  #endif
  NAInt indx = 0;
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  if(testvertex->x >= basevertex->x + childwidth){indx |= 1;}
  if(testvertex->y >= basevertex->y + childwidth){indx |= 2;}
  if(testvertex->z >= basevertex->z + childwidth){indx |= 4;}
  return indx;
}
NA_HDEF NABool naEqualKeyOctDouble(const void* origin1, const void* origin2){
  NAVertex* vertex1 = (NAVertex*)origin1;
  NAVertex* vertex2 = (NAVertex*)origin2;
  return naEqualVertex(*vertex1, *vertex2);
}
NA_HDEF void naAssignKeyOctDouble(void* dst, const void* src){
  naCopyVertex(dst, src);
}
NA_HDEF NABool naTestKeyOctDouble(const void* leftlimit, const void* rightlimit, const void* origin){
  // todo
  return ((*(const double*)leftlimit <= *(const double*)origin) && (*(const double*)rightlimit >= *(const double*)origin));
}
NA_HDEF NABool naTestKeyContainOctDouble(NATreeNode* parentnode, const void* key){
  NATreeOctNode* octNode = (NATreeOctNode*)(parentnode);
  NAVertex* testvertex = (NAVertex*)key;
  double childwidth = naMakeDoubleWithExponent((int32)octNode->childexponent);
  return (testvertex->x >= octNode->origin.x)
      && (testvertex->y >= octNode->origin.y)
      && (testvertex->z >= octNode->origin.z)
      && (testvertex->x < octNode->origin.x + 2 * childwidth)
      && (testvertex->y < octNode->origin.y + 2 * childwidth)
      && (testvertex->z < octNode->origin.z + 2 * childwidth);
}



// Callback. Do not call directly.
NA_HDEF void naDestructTreeNodeOct(NATreeNode* node){
  naDelete(node);
}



// Callback. Do not call directly.
NA_HDEF void naDestructTreeLeafOct(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleOctWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* leftlimit, const void* rightlimit, NATreeItem* previtem){
  NATreeOctNode* octNode;
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("node should not be null");
    if(previtem == NA_NULL)
      naError("prevnode should not be null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  octNode = (NATreeOctNode*)node;
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(origin, naGetOctNodeKey(octNode))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetTreeNodeChildIndex(tree, node, previtem) == 1){
    leftlimit = naGetOctNodeKey(octNode);
  }else{
    rightlimit = naGetOctNodeKey(octNode);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  NATreeItem* item = naGetTreeNodeItem(node);
  if(!naIsTreeItemRoot(tree, item)){
    return naLocateBubbleOctWithLimits(tree, naGetTreeItemParent(item), origin, leftlimit, rightlimit, item);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



NA_HDEF NATreeNode* naLocateBubbleOct(const NATree* tree, NATreeItem* item, const void* origin){
  return naLocateBubbleOctWithLimits(tree, naGetTreeItemParent(item), origin, NA_NULL, NA_NULL, item);
}



NA_HDEF void naSetTreeRootOct(NATree* tree, NATreeItem* newroot, NABool isLeaf){
  tree->root = newroot;
  naMarkTreeRootLeaf(tree, isLeaf);
  newroot->parent = NA_NULL;
}



NA_HDEF void naSetTreeNodeChildOct(NATreeOctNode* parent, NATreeItem* child, NAInt childIndex, NABool isChildLeaf){
  naSetTreeItemParent(child, naGetOctNodeNode(parent));
  naMarkNodeChildLeaf(naGetOctNodeNode(parent), childIndex, isChildLeaf);
  parent->childs[childIndex] = child;
}



NA_HDEF NATreeNode* naRemoveLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeItem* leafItem = naGetTreeLeafItem(leaf);
  NATreeNode* parent = naGetTreeItemParent(leafItem);
  if(naIsTreeItemRoot(tree, leafItem)){
    tree->root = NA_NULL;
  }else{
    NAInt leafindx = naGetTreeNodeChildIndex(tree, parent, leafItem);
    #ifndef NDEBUG
      if(!naIsNodeChildLeaf(parent, leafindx))
        naError("Child is no leaf");
      if(((NATreeOctNode*)parent)->childs[leafindx] == NA_NULL)
        naError("No child to erase");
      if(!parent)
        naError("That is strange. parent should not be Null");
    #endif
    ((NATreeOctNode*)parent)->childs[leafindx] = NA_NULL;

    while(parent)
    {
      // Search for a sibling and count if there is more than one.
      NATreeItem* sibling = NA_NULL;
      NAInt siblingcount = 0;
      NAInt siblingindex;
      if(((NATreeOctNode*)parent)->childs[0]){siblingindex = 0; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[1]){siblingindex = 1; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[2]){siblingindex = 2; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[3]){siblingindex = 3; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[4]){siblingindex = 4; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[5]){siblingindex = 5; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[6]){siblingindex = 6; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      if(((NATreeOctNode*)parent)->childs[7]){siblingindex = 7; sibling = ((NATreeOctNode*)parent)->childs[siblingindex]; siblingcount++;}
      #ifndef NDEBUG
        if(siblingcount == 0)
          naCrash("This should not happen");
      #endif
      if(siblingcount != 1){break;}
      NATreeOctNode* grandparent = (NATreeOctNode*)naGetTreeItemParent(naGetTreeNodeItem(parent));
      NABool isSiblingLeaf = naIsTreeItemLeaf(tree, sibling);
      if(grandparent){
        // There is a grandparent. Simply add the sibling at the place where
        // the parent was and delete the parent.
        NAInt parentindex = naGetTreeNodeChildIndex(tree, naGetOctNodeNode(grandparent), naGetTreeNodeItem(parent));
        naSetTreeNodeChildOct(grandparent, sibling, parentindex, isSiblingLeaf);
        naDestructTreeNode(tree, parent, NA_FALSE);
      }else{
        // This was the last parent before being a root. Attach the sibling as
        // the new root.
        naSetTreeRootOct(tree, sibling, isSiblingLeaf);
      }
      parent = naGetOctNodeNode(grandparent);
    }
  }
  naDestructTreeLeaf(tree, leaf);
  return parent;
}



// Oomph. That code is mighty confusing!
NA_HDEF NATreeLeaf* naInsertLeafOct(NATree* tree, NATreeItem* existingItem, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NA_UNUSED(insertOrder);
  #ifndef NDEBUG
    if(insertOrder != NA_TREE_LEAF_INSERT_ORDER_KEY)
      naError("Invalid insertOrder");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  
  // Create the new leaf and initialize it.
  NATreeLeaf* newLeaf = naConstructTreeLeafOct(tree, key, content);

  if(!existingItem){
    // There is no leaf to add to, meaning there was no root. Therefore, we
    // create a first leaf.
    naSetTreeRootOct(tree, naGetTreeLeafItem(newLeaf), NA_TRUE);
    
  }else{
  
    NAVertex* existingChildOrigin;
    if(naIsTreeItemLeaf(tree, existingItem)){
       existingChildOrigin = naGetOctLeafKey((NATreeOctLeaf*)existingItem);
    }else{
       existingChildOrigin = naGetOctNodeKey((NATreeOctNode*)existingItem);
    }
    NAVertex* newLeafOrigin = naGetOctLeafKey((NATreeOctLeaf*)newLeaf); 
    
    // Search for an already existing parent in the tree which contains both
    // existingChild and newLeaf. Note that existingChild can be a node and
    // hence can itself be the desired parent.
    NATreeOctNode* existingParent = NA_NULL;
    NAInt existingParentChildExponent = -1;
    NAVertex* existingParentOrigin = NA_NULL;
    
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
      NAVertex* prevRootOrigin;
      NAVertex newRootOrigin;
      NAInt newRootChildExponent;
      if(naIsTreeRootLeaf(tree)){
        newRootChildExponent = ((NATreeOctLeaf*)(tree->root))->leafexponent - 1;
        prevRootOrigin = naGetOctLeafKey((NATreeOctLeaf*)(tree->root));
      }else{
        NATreeOctNode* rootOctNode = (NATreeOctNode*)tree->root;
        newRootChildExponent = rootOctNode->childexponent;
        prevRootOrigin = naGetOctNodeKey(rootOctNode);
      }
      tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
      
      while(1){
        newRootChildExponent++;
        #ifndef NDEBUG
          if(newRootChildExponent >= NA_SYSTEM_ADDRESS_BITS)
            naCrash("childexponent grown too big.");
          NAVertex testRootOrigin = newRootOrigin;
        #endif
        newRootOrigin = naGetTreeNewRootOriginOct(newRootChildExponent, newRootOrigin);
        #ifndef NDEBUG
          if(!naContainsTreeNodeChildOct(&newRootOrigin, &testRootOrigin, newRootChildExponent))
            naError("Expanding root fails to cover the original root");
        #endif
        if(naContainsTreeNodeChildOct(&newRootOrigin, newLeafOrigin, newRootChildExponent)){break;}
      }
      // Reaching here, newRootOrigin and newRootChildExponent
      // denote a new parent containing both the existing child and the new leaf.
      // We create a new node which will become the root.
      NATreeOctNode* newRoot = naConstructTreeNodeOct(tree, newRootOrigin, newRootChildExponent);
      
      // Now, we attach the previous root to the new root at the appropriate
      // child index.
      NAInt prevRootIndx = tree->config->keyIndexGetter(&newRootOrigin, prevRootOrigin, &newRootChildExponent);
      naSetTreeNodeChildOct(newRoot, tree->root, prevRootIndx, naIsTreeRootLeaf(tree));

      // Finally, we set the newRoot to be the root of the tree and mark
      // it to be a node.
      naSetTreeRootOct(tree, naGetOctNodeItem(newRoot), NA_FALSE);

      // Now, the newRoot becomes our existing parent.
      existingParent = newRoot;
      existingParentOrigin = naGetOctNodeKey(newRoot);
      existingParentChildExponent = newRoot->childexponent;
    }
    // Now, we are sure, we have an existing parent.
    
    // We test if the desired index is free. If so, we have found the final
    // place for tne new child.
    NAInt desiredChildIndex = tree->config->keyIndexGetter(existingParentOrigin, newLeafOrigin, &existingParentChildExponent);
    NATreeItem* desiredChild =  existingParent->childs[desiredChildIndex];

    if(!desiredChild){
      // If the space for the new child if free, take it.
      naSetTreeNodeChildOct(existingParent, naGetTreeLeafItem(newLeaf), desiredChildIndex, NA_TRUE);
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
      NAInt smallestExistingChildIndex = -1;
      NAInt smallestNewLeafIndex = -1;
      while(1){
        smallestExistingChildIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        smallestNewLeafIndex       = tree->config->keyIndexGetter(&smallestParentOrigin, newLeafOrigin,       &smallestParentChildExponent);
        if(smallestExistingChildIndex != smallestNewLeafIndex){break;}
        // The two items share the same child. Go further down.
        smallestParentOrigin = naGetChildOriginOct(smallestParentOrigin, smallestNewLeafIndex, smallestParentChildExponent);
        smallestParentChildExponent--;
        #ifndef NDEBUG
          if(smallestParentChildExponent < naGetTreeConfigurationOcttreeBaseLeafExponent(tree->config))
            naError("child exponent shrank too deep");
        #endif
      }
      
      // Now, smallestParentOrigin and smallestParentChildExponent denote the
      // values for the parent which first differentiates between existingChild
      // and newLeaf.
      // If these exponents differ, we have to create a node between the
      // existingParent and existingChild.
      
      if(smallestParentChildExponent != existingParentChildExponent){
        NATreeOctNode* smallestParent = naConstructTreeNodeOct(tree, smallestParentOrigin, smallestParentChildExponent);
        
        // First, attach the previous item to the new parent.
        NABool isPrevExistingChildLeaf = naIsTreeItemLeaf(tree, prevExistingChild);
        NAInt smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingChildOrigin, &smallestParentChildExponent);
        naSetTreeNodeChildOct(smallestParent, prevExistingChild, smallestExistingIndex, isPrevExistingChildLeaf);
              
        #ifndef NDEBUG
          if(smallestParentChildExponent >= existingParent->childexponent)
            naError("exponent must be smaller");
        #endif
        
        // Then, attach the new parent to the existing parent.
        #ifndef NDEBUG
          NAInt testExistingIndex = tree->config->keyIndexGetter(naGetOctNodeKey(existingParent), naGetOctNodeKey(smallestParent), &(existingParent->childexponent));
          if(testExistingIndex != prevExistingChildIndex)
            naError("Newly computed index differs from previously computed index");
        #endif
        naSetTreeNodeChildOct(existingParent, naGetOctNodeItem(smallestParent), prevExistingChildIndex, NA_FALSE);
        
        existingParent = smallestParent;
      }
      
      // Now existingParent contains both items and is minimal. Attach the newLeaf
      // at its appropriate place.
      #ifndef NDEBUG
        if(existingParent->childs[smallestNewLeafIndex])
          naError("Child is already occupied");
      #endif
      naSetTreeNodeChildOct(existingParent, naGetTreeLeafItem(newLeaf), smallestNewLeafIndex, NA_TRUE);
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
