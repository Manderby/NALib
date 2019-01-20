
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeOct.h"



NA_RUNTIME_TYPE(NATreeOctNode, NA_NULL, NA_FALSE);
NA_RUNTIME_TYPE(NATreeOctLeaf, NA_NULL, NA_FALSE);



NA_HIDEF void naDestructTreeChildOct(NATree* tree, NATreeOctNode* octnode, NAInt childindx){
  NATreeBaseNode* child = octnode->childs[childindx];
  if(naIsNodeChildLeaf((NATreeNode*)octnode, childindx)){
    naDestructTreeLeafOct(tree, (NATreeLeaf*)child);
  }else{
    naDestructTreeNodeOct(tree, (NATreeNode*)child, NA_TRUE);
  }
}



// ////////////////////////////
// Callback functions
// ////////////////////////////


NA_HDEF NAInt naGetKeyIndexOctDouble(const void* baseorigin, const void* testorigin){
  return !(*(const double*)testorigin < *(const double*)baseorigin); // results in 0 or 1
}
NA_HDEF NABool naEqualKeyOctDouble(const void* origin1, const void* origin2){
  return (*(const double*)origin1 == *(const double*)origin2);
}
NA_HDEF void naAssignKeyOctDouble(void* dst, const void* src){
  *(double*)dst = *(const double*)src;
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
  naInitTreeNode((NATreeNode*)octnode);

  // Node-specific initialization
  tree->config->keyAssigner(&(octnode->origin), origin);
  octnode->childs[0] = (NATreeBaseNode*)leftleaf;
  octnode->childs[1] = (NATreeBaseNode*)rightleaf;
  naMarkNodeChildLeaf((NATreeNode*)octnode, 0, NA_TRUE);
  naMarkNodeChildLeaf((NATreeNode*)octnode, 1, NA_TRUE);
  ((NATreeBaseNode*)leftleaf)->parent = (NATreeNode*)octnode;
  ((NATreeBaseNode*)rightleaf)->parent = (NATreeNode*)octnode;

  if(tree->config->nodeConstructor){
    octnode->data = tree->config->nodeConstructor(&origin, tree->config->data);
  }else{
    octnode->data = naMakePtrNull();
  }

  return (NATreeNode*)octnode;
}



NA_HDEF void naDestructTreeNodeOct(NATree* tree, NATreeNode* node, NABool recursive){
  NATreeOctNode* octnode;
  #ifndef NDEBUG
    if(!node)
      naCrash("naDestructTreeNodeOct", "node shall not be Null");
  #endif
  octnode = (NATreeOctNode*)node;
  if(recursive){
    naDestructTreeChildOct(tree, octnode, 0);
    naDestructTreeChildOct(tree, octnode, 1);
  }

  if(tree->config->nodeDestructor){tree->config->nodeDestructor(octnode->data, tree->config->data);}
  naClearTreeNode((NATreeNode*)octnode);
  naDelete(node);
}



NA_HDEF NATreeLeaf* naConstructTreeLeafOct(NATree* tree, const void* origin, NAPtr data){
  NATreeOctLeaf* octleaf = naNew(NATreeOctLeaf);
  naInitTreeLeaf(&(octleaf->leaf));

  // Node-specific initialization
  if(origin){tree->config->keyAssigner(&(octleaf->origin), origin);}
  octleaf->data = naConstructLeafData(tree, &(octleaf->origin), data);

  return (NATreeLeaf*)octleaf;
}



// DONE
NA_HDEF void naDestructTreeLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeOctLeaf* octleaf;
  #ifndef NDEBUG
    if(!leaf)
      naCrash("naDestructTreeLeafOct", "leaf shall not be Null");
  #endif
  octleaf = (NATreeOctLeaf*)leaf;

  naDestructLeafData(tree, octleaf->data);
  naClearTreeLeaf(&(octleaf->leaf));
  naDelete(leaf);
}



NA_HDEF NATreeNode* naLocateBubbleOctWithLimits(const NATree* tree, NATreeNode* node, const void* origin, const void* leftlimit, const void* rightlimit, NATreeBaseNode* prevnode){
  NATreeOctNode* octnode;
  #ifndef NDEBUG
    if(node == NA_NULL)
      naError("naLocateBubbleOctWithLimits", "node should not be null");
    if(prevnode == NA_NULL)
      naError("naLocateBubbleOctWithLimits", "prevnode should not be null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naLocateBubbleOctWithLimits", "tree is configured with no key");
  #endif
  octnode = (NATreeOctNode*)(node);
  // If we are at a node which stores the key itself, return this node.
  if(tree->config->keyEqualer(origin, &(octnode->origin))){return node;}
  // Otherwise, we set the limits dependent on the previous node.
  if(naGetChildIndexOct(node, prevnode) == 1){
    leftlimit = &(octnode->origin);
  }else{
    rightlimit = &(octnode->origin);
  }
  // If we know both limits and the key is contained within, return.
  if(leftlimit && rightlimit && tree->config->keyTester(leftlimit, rightlimit, origin)){
    return node;
  }
  // Otherwise, go up if possible.
  if(((NATreeBaseNode*)node)->parent){
    return naLocateBubbleOctWithLimits(tree, ((NATreeBaseNode*)node)->parent, origin, leftlimit, rightlimit, (NATreeBaseNode*)octnode);
  }else{
    // We reached the root. No need to break a sweat. Simply return null.
    return NA_NULL;
  }
}



// DONE
NA_HDEF NATreeNode* naLocateBubbleOct(const NATree* tree, NATreeLeaf* leaf, const void* origin){
  #ifndef NDEBUG
    if(leaf == NA_NULL)
      naCrash("naLocateBubbleOct", "leaf should not be null");
  #endif
  if(((NATreeBaseNode*)leaf)->parent){
    return naLocateBubbleOctWithLimits(tree, ((NATreeBaseNode*)leaf)->parent, origin, NA_NULL, NA_NULL, (NATreeBaseNode*)leaf);
  }else{
    return NA_NULL;
  }
}



NA_HDEF NATreeLeaf* naLocateCaptureOct(const NATree* tree, NATreeNode* node, const void* origin, NABool* matchfound){
  NATreeOctNode* octnode;
  NAInt childindx;
  NATreeBaseNode* child;
  #ifndef NDEBUG
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naLocateCaptureOct", "tree is configured with no key");
  #endif

  *matchfound = NA_FALSE;

  if(!node){
    if(tree->root){
      if(naIsTreeRootLeaf(tree)){
        *matchfound = tree->config->keyEqualer(origin, &(((NATreeOctLeaf*)tree->root)->origin));
        return (NATreeLeaf*)tree->root;
      }else{
        node = (NATreeNode*)tree->root;
      }
    }else{
      return NA_NULL;
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



// DONE
NA_HDEF NAInt naGetChildIndexOct(NATreeNode* parent, NATreeBaseNode* child){
  NATreeOctNode* octparent;
  NAInt retvalue = -1;
  #ifndef NDEBUG
    if(!child)
      naError("naGetChildIndexOct", "Child should not be Null");
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
      naError("naGetChildIndexOct", "Child is no child of parent");
  #endif
  return retvalue;
}



NA_HDEF NAInt naGetChildKeyIndexOct(const NATree* tree, NATreeNode* parent, const void* origin){
  NATreeOctNode* octparent;
  #ifndef NDEBUG
    if(!parent)
      naError("naGetChildKeyIndexOct", "parent is Null");
    if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) == NA_TREE_KEY_NOKEY)
      naError("naGetChildKeyIndexOct", "tree is configured with no key");
  #endif
  octparent = (NATreeOctNode*)(parent);
  return tree->config->keyIndexGetter(&(octparent->origin), origin);
}



// DONE
NA_HDEF NATreeBaseNode* naGetChildOct(NATreeNode* parent, NAInt childindx){
  NATreeOctNode* octnode;
  #ifndef NDEBUG
    if(childindx < 0 || childindx >= 8)
      naCrash("naGetChildOct", "child index not valid");
  #endif
  octnode = (NATreeOctNode*)(parent);
  return octnode->childs[childindx];
}



NA_HDEF NATreeNode* naRemoveLeafOct(NATree* tree, NATreeLeaf* leaf){
  NATreeNode* parent = ((NATreeBaseNode*)leaf)->parent;
  NATreeNode* grandparent = NA_NULL;
  if(parent){
    NAInt leafindx = naGetChildIndexOct(parent, (NATreeBaseNode*)leaf);
    NATreeBaseNode* sibling = ((NATreeOctNode*)parent)->childs[1 - leafindx];
    NABool issiblingleaf = naIsNodeChildLeaf(parent, 1 - leafindx);

    grandparent = ((NATreeBaseNode*)parent)->parent;
    if(grandparent){
      NAInt parentindx = naGetChildIndexOct(grandparent, (NATreeBaseNode*)parent);
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
        naError("naInsertLeafOct", "tree is configured with no key");
    #endif
    if(tree->config->keyIndexGetter(&(((NATreeOctLeaf*)existingleaf)->origin), &(((NATreeOctLeaf*)newleaf)->origin)) == 1){
      left = existingleaf;
      right = newleaf;
    }else{
      left = newleaf;
      right = existingleaf;
    }
    break;
  case NA_TREE_LEAF_INSERT_ORDER_PREV:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
        naError("naInsertLeafOct", "tree is configured with key");
    #endif
    left = newleaf;
    right = existingleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_NEXT:
    #ifndef NDEBUG
      if((tree->config->flags & NA_TREE_KEY_TYPE_MASK) != NA_TREE_KEY_NOKEY)
        naError("naInsertLeafOct", "tree is configured with key");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  case NA_TREE_LEAF_INSERT_ORDER_REPLACE:
    #ifndef NDEBUG
      naError("naInsertLeafOct", "This case should not happen");
    #endif
    left = existingleaf;
    right = newleaf;
    break;
  default:
    #ifndef NDEBUG
	  naError("naInsertLeafOct", "Invalid insertOrder");
    #endif
	left = existingleaf;
	right = newleaf;
	break;
  }

  existingparent = ((NATreeBaseNode*)existingleaf)->parent;
  newparent = (NATreeOctNode*)naConstructTreeNodeOct(tree, &(((NATreeOctLeaf*)right)->origin), left, right);
  ((NATreeBaseNode*)newparent)->parent = existingparent;
  if(existingparent){
    NAInt existingindx = naGetChildIndexOct(existingparent, (NATreeBaseNode*)existingleaf);
    naMarkNodeChildLeaf(existingparent, existingindx, NA_FALSE);
    ((NATreeOctNode*)existingparent)->childs[existingindx] = (NATreeBaseNode*)newparent;
  }else{
    // The leaf was the root of the tree.
    tree->root = (NATreeBaseNode*)newparent;
    naMarkTreeRootLeaf(tree, NA_FALSE);
  }

  return newleaf;
}



NA_HDEF const void* naGetLeafKeyOct(NATreeLeaf* leaf){
  NATreeOctLeaf* octleaf = (NATreeOctLeaf*)(leaf);
  return &(octleaf->origin);
}



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
