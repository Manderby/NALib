
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeOct.h"



NA_RUNTIME_TYPE(NATreeOctNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeOctLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void naDestructTreeChildOct(NATree* tree, NATreeOctNode* octnode, NAInt childindx){
  NATreeItem* child = octnode->childs[childindx];
  if(naIsNodeChildLeaf(&(octnode->node), childindx)){
    naDestructTreeLeafOct(tree, (NATreeLeaf*)child);
  }else{
    naDestructTreeNodeOct(tree, (NATreeNode*)child, NA_TRUE);
  }
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


// The data parameter contains the leaf exponent of the children.
NA_HDEF NAInt naGetKeyIndexOctDouble(const void* baseorigin, const void* testorigin, const void* data){
  NAInt childexponent = *((NAInt*)data);
  NAVertex* basevertex = (NAVertex*)baseorigin;
  NAVertex* testvertex = (NAVertex*)testorigin;
  NAInt indx = 0;
  double childwidth = naMakeDoubleWithExponent((int32)childexponent);
  #ifndef NDEBUG
    if((testvertex->x < basevertex->x) || (testvertex->y < basevertex->y) || (testvertex->z < basevertex->z))
      naError("Vertex lies below test origin");
    if((testvertex->x >= basevertex->x + 2 * childwidth) || (testvertex->y >= basevertex->y + 2 * childwidth) || (testvertex->z >= basevertex->z + 2 * childwidth))
      naError("Vertex lies above test origin");
  #endif
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



//NA_HDEF NAInt naGetKeyIndexOctNAInt(const void* baseorigin, const void* origin){
//  return !(*(const NAInt*)origin < *(const NAInt*)baseorigin); // results in 0 or 1
//}
//NA_HDEF NABool naEqualKeyOctNAInt(const void* origin1, const void* origin2){
//  return (*(const NAInt*)origin1 == *(const NAInt*)origin2);
//}
//NA_HDEF void naAssignKeyOctNAInt(void* dst, const void* src){
//  *(NAInt*)dst = *(const NAInt*)src;
//}
//NA_HDEF NABool naTestKeyOctNAInt(const void* leftlimit, const void* rightlimit, const void* origin){
//  return ((*(const NAInt*)leftlimit <= *(const NAInt*)origin) && (*(const NAInt*)rightlimit >= *(const NAInt*)origin));
//}



NA_HDEF NATreeNode* naConstructTreeNodeOct(NATree* tree, const void* origin, NATreeLeaf* leftleaf, NATreeLeaf* rightleaf){
  NATreeOctNode* octnode = naNew(NATreeOctNode);
  naInitTreeNode(&(octnode->node));

  // Node-specific initialization
  tree->config->keyAssigner(&(octnode->origin), origin);
  octnode->childs[0] = &(leftleaf->item);
  octnode->childs[1] = &(rightleaf->item);
  octnode->childexponent = 0; /*todo childexponent*/
  naMarkNodeChildLeaf(&(octnode->node), 0, NA_TRUE);
  naMarkNodeChildLeaf(&(octnode->node), 1, NA_TRUE);
  naSetTreeItemParent(&(leftleaf->item), &(octnode->node));
  naSetTreeItemParent(&(rightleaf->item), &(octnode->node));

  if(tree->config->nodeConstructor){
    octnode->data = tree->config->nodeConstructor(&origin, tree->config->data);
  }else{
    octnode->data = naMakePtrNull();
  }

  return &(octnode->node);
}



NA_HDEF void naDestructTreeNodeOct(NATree* tree, NATreeNode* node, NABool recursive){
  NATreeOctNode* octnode;
  #ifndef NDEBUG
    if(!node)
      naCrash("node shall not be Null");
  #endif
  octnode = (NATreeOctNode*)node;
  if(recursive){
    naDestructTreeChildOct(tree, octnode, 0);
    naDestructTreeChildOct(tree, octnode, 1);
  }

  if(tree->config->nodeDestructor){tree->config->nodeDestructor(octnode->data, tree->config->data);}
  naClearTreeNode(&(octnode->node));
  naDelete(node);
}



// DONE
NA_HDEF NATreeLeaf* naConstructTreeLeafOct(NATree* tree, const void* origin, NAPtr data){
  NATreeOctLeaf* octleaf = naNew(NATreeOctLeaf);
  naInitTreeLeaf(&(octleaf->leaf));

  // Node-specific initialization
  if(origin){tree->config->keyAssigner(&(octleaf->origin), origin);}
  octleaf->data = naConstructLeafData(tree, &(octleaf->origin), data);
  octleaf->leafexponent = naGetTreeConfigurationOcttreeBaseLeafExponent(tree->config);

  return &(octleaf->leaf);
}



NA_HDEF void naDestructTreeLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeOctLeaf* octleaf;
  #ifndef NDEBUG
    if(!leaf)
      naCrash("leaf shall not be Null");
  #endif
  octleaf = (NATreeOctLeaf*)leaf;

  naDestructLeafData(tree, octleaf->data);
  naClearTreeLeaf(&(octleaf->leaf));
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
  if(tree->config->keyEqualer(origin, &(octnode->origin))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetChildIndexOct(node, previtem) == 1){
    leftlimit = &(octnode->origin);
  }else{
    rightlimit = &(octnode->origin);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  NATreeItem* item = &(node->item);
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
      *matchfound = tree->config->keyEqualer(origin, &(((NATreeOctLeaf*)tree->root)->origin));
      return (NATreeLeaf*)tree->root;
    }else{
      node = (NATreeNode*)tree->root;
    }
  }

  octnode = (NATreeOctNode*)(node);
  childindx = naGetChildKeyIndexOct(tree, node, origin);
  child = octnode->childs[childindx];

  if(naIsNodeChildLeaf(node, childindx)){
    // When the subtree denotes a leaf, we test, if the key is equal and return
    // the result.
    *matchfound = tree->config->keyEqualer(origin, &(((NATreeOctLeaf*)child)->origin));
    return (NATreeLeaf*)child;
  }else{
    // When the subtree denotes a node, we follow it.
    return naLocateCaptureOct(tree, (NATreeNode*)child, origin, matchfound);
  }
}



NA_HDEF NAInt naGetChildIndexOct(NATreeNode* parent, NATreeItem* child){
  NATreeOctNode* octparent;
  NAInt retvalue = -1;
  #ifndef NDEBUG
    if(!child)
      naError("Child should not be Null");
  #endif
  octparent = (NATreeOctNode*)(parent);
  if     (octparent->childs[0] == child){retvalue = 0;}
  else if(octparent->childs[1] == child){retvalue = 1;}
  else if(octparent->childs[2] == child){retvalue = 2;}
  else if(octparent->childs[3] == child){retvalue = 3;}
  else if(octparent->childs[4] == child){retvalue = 4;}
  else if(octparent->childs[5] == child){retvalue = 5;}
  else if(octparent->childs[6] == child){retvalue = 6;}
  else if(octparent->childs[7] == child){retvalue = 7;}
  #ifndef NDEBUG
    if(retvalue == -1)
      naError("Child is no child of parent");
  #endif
  return retvalue;
}



NA_HDEF NAInt naGetChildKeyIndexOct(const NATree* tree, NATreeNode* parent, const void* origin){
  NATreeOctNode* octparent;
  #ifndef NDEBUG
    if(!parent)
      naError("parent is Null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("tree is configured with no key");
  #endif
  octparent = (NATreeOctNode*)(parent);
  return tree->config->keyIndexGetter(&(octparent->origin), origin, 0 /*todo childexponent*/);
}



NA_HDEF NATreeItem* naGetChildOct(NATreeNode* parent, NAInt childindx){
  NATreeOctNode* octnode;
  #ifndef NDEBUG
    if(childindx < 0 || childindx >= 8)
      naCrash("child index not valid");
  #endif
  octnode = (NATreeOctNode*)(parent);
  return octnode->childs[childindx];
}



NA_HDEF NATreeNode* naRemoveLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = naGetTreeItemParent(&(leaf->item));
  NATreeNode* grandparent = NA_NULL;
  if(!naIsTreeItemRoot(tree, &(leaf->item))){
    NAInt leafindx = naGetChildIndexOct(parent, &(leaf->item));
    NATreeItem* sibling = ((NATreeOctNode*)parent)->childs[1 - leafindx];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafindx);

    grandparent = naGetTreeItemParent(&(parent->item));
    if(!naIsTreeItemRoot(tree, &(parent->item))){
      NAInt parentindx = naGetChildIndexOct(grandparent, &(parent->item));
      ((NATreeOctNode*)grandparent)->childs[parentindx] = sibling;
      naMarkNodeChildLeaf(grandparent, parentindx, issiblingleaf);
    }else{
      tree->root = sibling;
      naMarkTreeRootLeaf(tree, issiblingleaf);
    }
    sibling->parent = grandparent;

    naDestructTreeNodeOct(tree, parent, NA_FALSE);
  }else{
    tree->root = NA_NULL;
  }
  naDestructTreeLeafOct(tree, leaf);
  return grandparent;
}



NA_HDEF NATreeLeaf* naInsertLeafOct(NATree* tree, NATreeLeaf* existingleaf, const void* origin, NAPtr content, NATreeLeafInsertOrder insertOrder){
  NATreeLeaf* left;
  NATreeLeaf* right;
  NATreeLeaf* newleaf;
  NATreeNode* existingparent;
  NATreeOctNode* newparent;

  if(insertOrder == NA_TREE_LEAF_INSERT_ORDER_REPLACE){
    NATreeOctLeaf* octleaf = (NATreeOctLeaf*)existingleaf;
    naDestructLeafData(tree, octleaf->data);
    octleaf->data = naConstructLeafData(tree, &(octleaf->origin), content);
    return existingleaf;
  }

  newleaf = tree->config->leafCoreConstructor(tree, origin, content);

  switch(insertOrder){
  case NA_TREE_LEAF_INSERT_ORDER_KEY:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
        naError("tree is configured with no key");
    #endif
    NAInt childexponent = ((NATreeOctLeaf*)existingleaf)->leafexponent;
    // Find out if key inside box.
    if(tree->config->keyIndexGetter(&(((NATreeOctLeaf*)existingleaf)->origin), &(((NATreeOctLeaf*)newleaf)->origin), &childexponent) == 1){
      left = existingleaf;
      right = newleaf;
    }else{
      left = newleaf;
      right = existingleaf;
    }
    break;
  default:
    #ifndef NDEBUG
	  naError("Invalid insertOrder");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  }

  existingparent = naGetTreeItemParent(&(existingleaf->item));
  newparent = (NATreeOctNode*)naConstructTreeNodeOct(tree, &(((NATreeOctLeaf*)right)->origin), left, right);
  naSetTreeItemParent(&(newparent->node.item), existingparent);
  if(!naIsTreeItemRoot(tree, &(existingleaf->item))){
    NAInt existingindx = naGetChildIndexOct(existingparent, &(existingleaf->item));
    naMarkNodeChildLeaf(existingparent, existingindx, NA_FALSE);
    ((NATreeOctNode*)existingparent)->childs[existingindx] = naGetOctNodeItem(newparent);
  }else{
    // The leaf was the root of the tree.
    tree->root = naGetOctNodeItem(newparent);
    naMarkTreeRootLeaf(tree, NA_FALSE);
  }

  return newleaf;
}



NA_HDEF const void* naGetLeafKeyOct(NATreeLeaf* leaf){
  NATreeOctLeaf* octleaf = (NATreeOctLeaf*)(leaf);
  return &(octleaf->origin);
}



NA_HDEF const void* naGetNodeKeyOct(NATreeNode* node){
  NATreeOctNode* octnode = (NATreeOctNode*)(node);
  return &(octnode->origin);
}



// DONE
NA_HDEF NAPtr naGetLeafDataOct(NATreeLeaf* leaf){
  NATreeOctLeaf* octleaf = (NATreeOctLeaf*)(leaf);
  return octleaf->data;
}



NA_HDEF NAPtr naGetNodeDataOct(NATreeNode* node){
  NATreeOctNode* octnode = (NATreeOctNode*)(node);
  return octnode->data;
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
