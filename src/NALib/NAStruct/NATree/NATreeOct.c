
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeOct.h"



NA_RUNTIME_TYPE(NATreeOctNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeOctLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void* naGetOctNodeKey(NATreeOctNode* octnode){
  return &(octnode->origin);
}
NA_HIDEF void* naGetOctLeafKey(NATreeOctLeaf* octleaf){
  return &(octleaf->origin);
}
NA_HIDEF NAInt* naGetOctNodeChildExponent(NATreeOctNode* octnode){
  return &(octnode->childexponent);
}



NA_HIDEF NABool naContainsTreeNodeChildOct(NAVertex* basevertex, NAVertex* testvertex, NAInt childexponent){
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  return (testvertex->x >= basevertex->x)
      && (testvertex->y >= basevertex->y)
      && (testvertex->z >= basevertex->z)
      && (testvertex->x < basevertex->x + 2 * childwidth)
      && (testvertex->y < basevertex->y + 2 * childwidth)
      && (testvertex->z < basevertex->z + 2 * childwidth);
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
  NATreeOctNode* octnode = naNew(NATreeOctNode);
  naInitTreeNode(naGetOctNodeNode(octnode));

  // Node-specific initialization
  octnode->childexponent = childexponent;
  tree->config->keyAssigner(naGetOctNodeKey(octnode), &origin);
  octnode->childs[0] = NA_NULL;
  octnode->childs[1] = NA_NULL;
  octnode->childs[2] = NA_NULL;
  octnode->childs[3] = NA_NULL;
  octnode->childs[4] = NA_NULL;
  octnode->childs[5] = NA_NULL;
  octnode->childs[6] = NA_NULL;
  octnode->childs[7] = NA_NULL;

  if(tree->config->nodeDataConstructor){
    octnode->data = tree->config->nodeDataConstructor(&origin, tree->config->data);
  }else{
    octnode->data = naMakePtrNull();
  }

  return octnode;
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


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
  return ((*(const double*)leftlimit <= *(const double*)origin) && (*(const double*)rightlimit >= *(const double*)origin));
}



NA_HDEF NATreeLeaf* naConstructTreeLeafOct(const NATree* tree){
  NATreeOctLeaf* octleaf = naNew(NATreeOctLeaf);
  octleaf->leafexponent = naGetTreeConfigurationOcttreeBaseLeafExponent(tree->config);
  return naGetOctLeafLeaf(octleaf);
}



NA_HDEF void naDestructTreeLeafOct(NATreeLeaf* leaf){
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleOctWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* leftlimit, const void* rightlimit, NATreeItem* previtem){
  NATreeOctNode* octnode;
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("node should not be null");
    if(previtem == NA_NULL)
      naError("prevnode should not be null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  octnode = (NATreeOctNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(origin, naGetOctNodeKey(octnode))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetTreeNodeChildIndex(tree, node, previtem) == 1){
    leftlimit = naGetOctNodeKey(octnode);
  }else{
    rightlimit = naGetOctNodeKey(octnode);
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



NA_HDEF NATreeLeaf* naLocateCaptureOct(const NATree* tree, NATreeNode* node, const void* origin, NABool* matchfound){
  NATreeOctNode* octnode;
  NAInt childindx;
  NATreeItem* child;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif

  *matchfound = NA_FALSE;

  if(!node){
    if(naIsTreeRootLeaf(tree)){
      *matchfound = tree->config->keyEqualer(origin, naGetOctLeafKey(((NATreeOctLeaf*)tree->root)));
      return (NATreeLeaf*)tree->root;
    }else{
      node = (NATreeNode*)tree->root;
    }
  }

  octnode = (NATreeOctNode*)(node);
  childindx = tree->config->keyIndexGetter(naGetOctNodeKey(octnode), origin, naGetOctNodeChildExponent(octnode));
  child = octnode->childs[childindx];

  if(!child){
    return NA_NULL;
  }else if(naIsNodeChildLeaf(node, childindx)){
    // When the subtree denotes a leaf, we test, if the key is equal and return
    // the result.
    *matchfound = tree->config->keyEqualer(origin, naGetOctLeafKey(((NATreeOctLeaf*)child)));
    return (NATreeLeaf*)child;
  }else{
    // When the subtree denotes a node, we follow it.
    return naLocateCaptureOct(tree, (NATreeNode*)child, origin, matchfound);
  }
}



NA_HDEF NATreeNode* naRemoveLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = naGetTreeItemParent(naGetTreeLeafItem(leaf));
  NATreeNode* grandparent = NA_NULL;
  if(!naIsTreeItemRoot(tree, naGetTreeLeafItem(leaf))){
    NAInt leafindx = naGetTreeNodeChildIndex(tree, parent, naGetTreeLeafItem(leaf));
    NATreeItem* sibling = ((NATreeOctNode*)parent)->childs[1 - leafindx];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafindx);

    grandparent = naGetTreeItemParent(naGetTreeNodeItem(parent));
    if(!naIsTreeItemRoot(tree, naGetTreeNodeItem(parent))){
      NAInt parentindx = naGetTreeNodeChildIndex(tree, grandparent, naGetTreeNodeItem(parent));
      ((NATreeOctNode*)grandparent)->childs[parentindx] = sibling;
      naMarkNodeChildLeaf(grandparent, parentindx, issiblingleaf);
    }else{
      tree->root = sibling;
      naMarkTreeRootLeaf(tree, issiblingleaf);
    }
    sibling->parent = grandparent;

    naDestructTreeNode(tree, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  naDestructTreeLeaf(tree, leaf);
  return grandparent;
}



// Oomph. That code is mighty confusing!
NA_HDEF void naInsertLeafOct(NATree* tree, NATreeLeaf* existingLeaf, NATreeLeaf* newLeaf, NATreeLeafInsertOrder insertOrder){
  #ifndef NDEBUG
    if(insertOrder != NA_TREE_LEAF_INSERT_ORDER_KEY)
      naError("Invalid insertOrder");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  
  NAVertex* existingLeafOrigin = naGetOctLeafKey((NATreeOctLeaf*)existingLeaf); 
  NAVertex* newLeafOrigin = naGetOctLeafKey((NATreeOctLeaf*)newLeaf); 
  
  // Search for an already existing parent in the tree which contains both
  // existingLeaf and newLeaf
  NATreeNode* existingParent = NA_NULL;
  NAInt existingParentChildExponent = -1;
  NAVertex* existingParentOrigin;
  
  if(!naIsTreeRootLeaf(tree)){
    // Bubble throught the tree, starting from the existing leaf.
    existingParent = naGetTreeItemParent(naGetTreeLeafItem(existingLeaf));
    existingParentOrigin = naGetOctNodeKey((NATreeOctNode*)existingParent);
    existingParentChildExponent = ((NATreeOctNode*)existingParent)->childexponent;
    while(existingParent && !naContainsTreeNodeChildOct(existingParentOrigin, newLeafOrigin, existingParentChildExponent)){
      existingParent = naGetTreeItemParent(naGetTreeNodeItem(existingParent));
      existingParentOrigin = naGetOctNodeKey((NATreeOctNode*)existingParent);
      existingParentChildExponent = ((NATreeOctNode*)existingParent)->childexponent;
    }
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
      newRootChildExponent = ((NATreeOctNode*)(tree->root))->childexponent;
      prevRootOrigin = naGetOctNodeKey((NATreeOctNode*)(tree->root));
    }
    tree->config->keyAssigner(&newRootOrigin, prevRootOrigin);
    
    while(1){
      newRootChildExponent++;
      #ifndef NDEBUG
        if(newRootChildExponent >= NA_SYSTEM_ADDRESS_BITS)
          naCrash("childexponent grown too big.");
      #endif
      newRootOrigin = naGetTreeNewRootOriginOct(newRootChildExponent, newRootOrigin);
      if(naContainsTreeNodeChildOct(&newRootOrigin, newLeafOrigin, newRootChildExponent)){break;}
    }
    // Reaching here, newRootOrigin and newRootChildExponent
    // denote a new parent containing both the existing leaf and the new leaf.
    // We create a new node which will become the root.
    NATreeOctNode* newRoot = naConstructTreeNodeOct(tree, newRootOrigin, newRootChildExponent);
    
    // Now, we attach the previous root to the new root at the appropriate
    // child index.
    NAInt prevRootIndx = tree->config->keyIndexGetter(&newRootOrigin, prevRootOrigin, &newRootChildExponent);
    naSetTreeItemParent(tree->root, naGetOctNodeNode(newRoot));
    naMarkNodeChildLeaf(naGetOctNodeNode(newRoot), prevRootIndx, naIsTreeRootLeaf(tree));
    newRoot->childs[prevRootIndx] = tree->root;
    
    // Finally, we set the newRoot to be the root of the tree and mark
    // it to be a node.
    tree->root = naGetOctNodeItem(newRoot);
    naMarkTreeRootLeaf(tree, NA_FALSE);

    // Now, the newRoot becomes our existing parent.
    existingParent = (NATreeNode*)newRoot;
    existingParentOrigin = naGetOctNodeKey(newRoot);
    existingParentChildExponent = newRoot->childexponent;
  }
  // Now, we are sure, we have an existing parent. We store the child and the
  // index of the subtree which contains the existing leaf for later.
  NAInt prevExistingChildIndex = tree->config->keyIndexGetter(existingParentOrigin, existingLeafOrigin, &existingParentChildExponent);
  NATreeItem* prevExistingChild =  ((NATreeOctNode*)(existingParent))->childs[prevExistingChildIndex];

  // Find out what the smallest possible parent would be which contains both
  // existingLeaf and newLeaf 
  NAVertex smallestParentOrigin;
  tree->config->keyAssigner(&smallestParentOrigin, existingParentOrigin);

  NAInt smallestParentChildExponent = existingParentChildExponent;
  NAInt smallestExistingLeafIndex;
  NAInt smallestNewLeafIndex;
  while(1){
    smallestExistingLeafIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingLeafOrigin, &smallestParentChildExponent);
    smallestNewLeafIndex      = tree->config->keyIndexGetter(&smallestParentOrigin, newLeafOrigin,      &smallestParentChildExponent);
    if(smallestExistingLeafIndex != smallestNewLeafIndex){break;}
    // The two leafes share the same child. Go further down.
    smallestParentOrigin = naGetChildOriginOct(smallestParentOrigin, smallestNewLeafIndex, smallestParentChildExponent);
    smallestParentChildExponent--;
  }
  
  // Now, smallestParentOrigin and smallestParentChildExponent denote the
  // values for the parent which first differentiates between existingLeaf
  // and newLeaf.
  // If these are the same values as the existingParent values, we do not need
  // to look any further. If they differ however, we have to create a node
  // between existingParent and existingLeaf.
  
  if(smallestParentChildExponent != existingParentChildExponent){
    NATreeOctNode* smallestParent = naConstructTreeNodeOct(tree, smallestParentOrigin, smallestParentChildExponent);
    
    // First, attach the previous item to the new parent.
    NABool isPrevExistingChildLeaf = naIsTreeItemLeaf(tree, prevExistingChild);
    naSetTreeItemParent(prevExistingChild, naGetOctNodeNode(smallestParent));
    NAInt smallestExistingIndex = tree->config->keyIndexGetter(&smallestParentOrigin, existingLeafOrigin, &smallestParentChildExponent);
    naMarkNodeChildLeaf(naGetOctNodeNode(smallestParent), smallestExistingIndex, isPrevExistingChildLeaf);
    ((NATreeOctNode*)smallestParent)->childs[smallestExistingIndex] = prevExistingChild;
    
    // Then, attach the new parent to the existing parent.
    naSetTreeItemParent(naGetOctNodeItem(smallestParent), (NATreeNode*)existingParent);
    naMarkNodeChildLeaf((NATreeNode*)existingParent, smallestExistingLeafIndex, NA_FALSE);
    ((NATreeOctNode*)existingParent)->childs[smallestExistingLeafIndex] = naGetOctNodeItem(smallestParent);
    
    existingParent = (NATreeNode*)smallestParent;
  }
  
  // Now existingParent contains both leafs and is minimal. Attach the newLeaf
  // at its appropriate place.
  naSetTreeItemParent(naGetTreeLeafItem(newLeaf), existingParent);
  naMarkNodeChildLeaf(existingParent, smallestNewLeafIndex, NA_TRUE);
  ((NATreeOctNode*)existingParent)->childs[smallestNewLeafIndex] = naGetTreeLeafItem(newLeaf);
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
