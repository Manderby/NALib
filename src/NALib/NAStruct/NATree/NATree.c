
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"

NAInt capturecount = 0; 
NAInt bubblecount = 0; 


// Prototypes
NA_HIAPI NANodeChildType naGetNodeType(const NATree* tree, NATreeBaseNode* node);



NA_HDEF NATreeLeaf* naIterateTreeCapture(const NATree* tree, NATreeNode* parent, NAInt previndx, NATreeIterationInfo* info){
  NAInt indx = previndx + info->step;
  while(indx != info->breakindx){
    NANodeChildType childtype = naGetNodeChildType(parent, indx);
    if(naIsNodeChildTypeValid(childtype)){
      // There is another child available
      NATreeBaseNode* child = tree->config->childGetter(parent, indx);
      if(childtype == NA_TREE_NODE_CHILD_LEAF){
        // We found the next leaf. Good ending
        return (NATreeLeaf*)child;
      }else{
        // We have to go deeper.
        return naIterateTreeCapture(tree, (NATreeNode*)child, info->startindx, info);
      }
    }
    indx += info->step;
  }
  return NA_NULL;
}



// This function takes a given basenode and bubbles up to its parent. This
// function works recursively until either a parent offers a next leaf or
// there are no more parents.
NA_HDEF NATreeLeaf* naIterateTreeBubble(const NATree* tree, NATreeBaseNode* curnode, NATreeIterationInfo* info){
  if(!curnode->parent){
    // We reached the root with no further options. Iteration is over.
    return NA_NULL;
  }
  NAInt indx = tree->config->childIndexGetter(curnode->parent, curnode);

  NATreeLeaf* leaf = naIterateTreeCapture(tree, curnode->parent, indx, info);
  if(leaf){
    return leaf;
  }else{
    // If non more childs are available, bubble further.
    return naIterateTreeBubble(tree, (NATreeBaseNode*)(curnode->parent), info);
  }
}



NA_HDEF NABool naIterateTreeWithInfo(NATreeIterator* iter, NATreeIterationInfo* info){
  const NATree* tree = (const NATree*)naGetPtrConst(&(iter->tree));
  
  // If the tree has no root, we do not iterate.
  if(!tree->root){
    #ifndef NDEBUG
      if(iter->leaf)
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    return NA_FALSE;
  }
  
  NATreeLeaf* leaf;
  if(!iter->leaf){
    // If the iterator is at initial position, we use the root and capture. 
    leaf = naIterateTreeCapture(tree, tree->root, info->startindx, info);
  }else{
    // Otherwise, we use the current leaf and bubble
    leaf = naIterateTreeBubble(tree, (NATreeBaseNode*)(iter->leaf), info);
  }
  #ifndef NDEBUG
    if(leaf && naGetNodeType(tree, (NATreeBaseNode*)leaf) != NA_TREE_NODE_CHILD_LEAF)
      naError("naIterateTreeWithInfo", "Result should be a leaf");
  #endif
  naSetTreeIteratorCurLeaf(iter, leaf);
  return (leaf != NA_NULL);
}



NA_HDEF NATreeNode* naLocateTreeNode(NATreeIterator* iter, const void* key, NABool* keyleaffound, NAInt* leafindx, NABool usebubble){
  const NATree* tree;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naLocateTree", "This iterator has been cleared. You need to make it anew.");
  #endif

  tree = (const NATree*)naGetPtrConst(&(iter->tree));

  if(!tree->root){
    #ifndef NDEBUG
      if(iter->leaf)
      naCrash("naIterateTree", "Current iterator node is set although no root available");
    #endif
    *keyleaffound = NA_FALSE;
    return NA_FALSE;
  }
  
  NATreeBaseNode* curnode = (NATreeBaseNode*)(iter->leaf);
  NATreeNode* topnode;
  
  // Move the iterator to the topmost inner node which contains the given key.
  if(usebubble && curnode){
    #ifndef NDEBUG
      if(naGetNodeType(tree, curnode) != NA_TREE_NODE_CHILD_LEAF)
        naError("naIterateTree", "current node is not a leaf");
    #endif
    topnode = tree->config->bubbleLocator(tree, curnode->parent, key);
  }else{
    topnode = tree->root;
  }
  
  // Search for the leaf containing key.
  NATreeNode* retnode = tree->config->captureLocator(tree, topnode, key, keyleaffound, leafindx);
  #ifndef NDEBUG
    if(naGetNodeType(tree, (NATreeBaseNode*)retnode) != NA_TREE_NODE_CHILD_NODE)
      naError("naLocateTreeNode", "Result should be a node");
  #endif
  return retnode;
}



NA_HDEF NABool naAddTreeLeaf(NATreeIterator* iter, const void* key, NAPtr content, NABool replace){
  NABool keyleaffound;
  NAInt childindx;
  #ifndef NDEBUG
    if(naTestFlagi(iter->flags, NA_TREE_ITERATOR_CLEARED))
      naError("naAddTreeLeaf", "This iterator has been cleared. You need to make it anew.");
  #endif
  // We do not use bubbling when inserting as there is almost never a benefit
  // from it. Even worse, it performs mostly worse.
  NATreeNode* node = naLocateTreeNode(iter, key, &keyleaffound, &childindx, NA_FALSE);
  NATree* tree = (NATree*)naGetPtrMutable(&(iter->tree));

  if(keyleaffound && !replace){
    naSetTreeIteratorCurLeaf(iter, (NATreeLeaf*)(tree->config->childGetter(node, childindx)));
    // Nothing else to do.
    return NA_FALSE;
  }

  if(keyleaffound){
    // We need to replace this node
    NATreeLeaf* oldleaf = (NATreeLeaf*)(tree->config->childGetter(node, childindx));
    tree->config->leafReplacer(tree, oldleaf, content);
    naSetTreeIteratorCurLeaf(iter, oldleaf);
  }else{
    NATreeLeaf* leaf = tree->config->leafCoreConstructor(tree, key, content);
    naSetTreeIteratorCurLeaf(iter, leaf);
    if(!node){
      // We need to create a root and attach the new leaf to it.
      NATreeNode* root = tree->config->nodeCoreConstructor(tree, key);
      ((NATreeBaseNode*)root)->parent = NA_NULL;
      tree->root = root;
      tree->config->leafAdder(root, leaf, tree->config->childKeyIndexGetter(tree, root, key));
    }else{
      NANodeChildType childtype = naGetNodeChildType(node, childindx);
      if(childtype == NA_TREE_NODE_CHILD_LEAF){
        // We need to create a node holding both the old leaf and the new one.
        tree->config->leafSplitter(tree, node, childindx, leaf);
      }else{
        #ifndef NDEBUG
          if(childtype == NA_TREE_NODE_CHILD_NODE)
            naError("naAddTreeConst", "Child should not be a node");
        #endif
        // We need to add the new leaf to this node
        tree->config->leafAdder((NATreeNode*)node, leaf, childindx);
      }
    }
  }
  return NA_TRUE;
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
