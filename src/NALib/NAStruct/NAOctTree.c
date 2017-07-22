
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "../NAOctTree.h"
#include "../NAMemory.h"



struct NAOctTreeNode{
  NAInt childlength;
  NAInt segmentinparent;
  NAOctTreeNode* parentnode;
  void* child[8];
  NAVertexi childorigin[8];
  void* nodedata;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};




// ////////////////////////////
// Node
// ////////////////////////////


NA_HIDEF NABoxi naGetOctTreeNodeBox(NAOctTreeNode* node){
  return naMakeBoxi(node->childorigin[0], naMakeVolumei(node->childlength * 2, node->childlength * 2, node->childlength * 2));
}



NA_HDEF void naFillOctTreeNodeChildData(const void* childdata[8], NAOctTreeNode* node){
  childdata[0] = (node->child[0]) ? (((NAOctTreeNode*)(node->child[0]))->nodedata) : NA_NULL;
  childdata[1] = (node->child[1]) ? (((NAOctTreeNode*)(node->child[1]))->nodedata) : NA_NULL;
  childdata[2] = (node->child[2]) ? (((NAOctTreeNode*)(node->child[2]))->nodedata) : NA_NULL;
  childdata[3] = (node->child[3]) ? (((NAOctTreeNode*)(node->child[3]))->nodedata) : NA_NULL;
  childdata[4] = (node->child[4]) ? (((NAOctTreeNode*)(node->child[4]))->nodedata) : NA_NULL;
  childdata[5] = (node->child[5]) ? (((NAOctTreeNode*)(node->child[5]))->nodedata) : NA_NULL;
  childdata[6] = (node->child[6]) ? (((NAOctTreeNode*)(node->child[6]))->nodedata) : NA_NULL;
  childdata[7] = (node->child[7]) ? (((NAOctTreeNode*)(node->child[7]))->nodedata) : NA_NULL;
}



// Creates an inner node.
NA_HDEF NAOctTreeNode* naAllocOctTreeNode(NAInt childlength, NAInt segmentinparent, NAOctTreeNode* parentnode, NAVertexi origin, NAOctTreeNodeAllocator allocator){
  NAOctTreeNode* node = naAlloc(NAOctTreeNode);
  node->childlength = childlength;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->child[0] = NA_NULL;
  node->child[1] = NA_NULL;
  node->child[2] = NA_NULL;
  node->child[3] = NA_NULL;
  node->child[4] = NA_NULL;
  node->child[5] = NA_NULL;
  node->child[6] = NA_NULL;
  node->child[7] = NA_NULL;
  node->childorigin[0] = naMakeVertexi(origin.x                    , origin.y                    , origin.z);
  node->childorigin[1] = naMakeVertexi(origin.x + node->childlength, origin.y                    , origin.z);
  node->childorigin[2] = naMakeVertexi(origin.x                    , origin.y + node->childlength, origin.z);
  node->childorigin[3] = naMakeVertexi(origin.x + node->childlength, origin.y + node->childlength, origin.z);
  node->childorigin[4] = naMakeVertexi(origin.x                    , origin.y                    , origin.z + node->childlength);
  node->childorigin[5] = naMakeVertexi(origin.x + node->childlength, origin.y                    , origin.z + node->childlength);
  node->childorigin[6] = naMakeVertexi(origin.x                    , origin.y + node->childlength, origin.z + node->childlength);
  node->childorigin[7] = naMakeVertexi(origin.x + node->childlength, origin.y + node->childlength, origin.z + node->childlength);
  if(allocator){
    node->nodedata = allocator(origin, childlength * 2);
  }else{
    node->nodedata = NA_NULL;
  }
  #ifndef NDEBUG
    node->itercount = 0;
  #endif
  return node;
}



// Deallocates one node. Deallocates all subnodes and leafes in the process.
NA_HDEF void naDeallocOctTreeNode(NAOctTree* tree, NAOctTreeNode* node){
  #ifndef NDEBUG
    if(node->itercount)
      naError("naDeallocOctTreeNode", "Iterators still running on this node. Did you forget a call to NAClearOctTreeIterator?");
  #endif

  if(node->childlength == tree->leaflength){
    if(node->child[0]){tree->callbacks.leafdeallocator(node->child[0], tree->callbacks.userdata);}
    if(node->child[1]){tree->callbacks.leafdeallocator(node->child[1], tree->callbacks.userdata);}
    if(node->child[2]){tree->callbacks.leafdeallocator(node->child[2], tree->callbacks.userdata);}
    if(node->child[3]){tree->callbacks.leafdeallocator(node->child[3], tree->callbacks.userdata);}
    if(node->child[4]){tree->callbacks.leafdeallocator(node->child[4], tree->callbacks.userdata);}
    if(node->child[5]){tree->callbacks.leafdeallocator(node->child[5], tree->callbacks.userdata);}
    if(node->child[6]){tree->callbacks.leafdeallocator(node->child[6], tree->callbacks.userdata);}
    if(node->child[7]){tree->callbacks.leafdeallocator(node->child[7], tree->callbacks.userdata);}
  }else{
    if(node->child[0]){naDeallocOctTreeNode(tree, node->child[0]);}
    if(node->child[1]){naDeallocOctTreeNode(tree, node->child[1]);}
    if(node->child[2]){naDeallocOctTreeNode(tree, node->child[2]);}
    if(node->child[3]){naDeallocOctTreeNode(tree, node->child[3]);}
    if(node->child[4]){naDeallocOctTreeNode(tree, node->child[4]);}
    if(node->child[5]){naDeallocOctTreeNode(tree, node->child[5]);}
    if(node->child[6]){naDeallocOctTreeNode(tree, node->child[6]);}
    if(node->child[7]){naDeallocOctTreeNode(tree, node->child[7]);}
  }

  if(node->nodedata && tree->callbacks.nodedeallocator){
    tree->callbacks.nodedeallocator(node->nodedata);
  }
  free(node);
}



// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAOctTreeNode* naAddOctTreeNodeChild(NAOctTree* tree, NAOctTreeNode* parentnode, NAInt segment){
  NAOctTreeNode* node;
  #ifndef NDEBUG
    if(parentnode->child[segment])
      naError("naAddOctTreeNodeChild", "A child already exists at that segment of the parent node.");
  #endif

  node = naAllocOctTreeNode(
          parentnode->childlength / 2,
          segment,
          parentnode,
          parentnode->childorigin[segment],
          tree->callbacks.nodeallocator);
  
  parentnode->child[segment] = node;
  return node;
}



// Returns the segment index [0-7] in which the given pos can be found.
// Warning: The position is expected to be inside this node.
NA_HIDEF NAInt naGetOctTreeNodeSegment(NAOctTreeNode* node, NAVertexi vertex){
  NAInt segment = 0;
  if(vertex.x >= node->childorigin[7].x){segment |= 1;}
  if(vertex.y >= node->childorigin[7].y){segment |= 2;}
  if(vertex.z >= node->childorigin[7].z){segment |= 4;}
  #ifndef NDEBUG
    if(!naContainsBoxiVertex(naMakeBoxi(node->childorigin[segment], naMakeVolumei(node->childlength, node->childlength, node->childlength)), vertex))
      naError("naGetOctTreeNodeSegment", "node does not actually contains vertex");
  #endif
  return segment;
}



NA_HDEF NAVertexi naGetOctTreeNodeParentOrigin(NAInt childsize, NAVertexi childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  //
  // Implementation note: To be sure that the childorigin stays aligned, we
  // simply take that origin als the alignment origin and work from there.

  NABoxi alignbox = naMakeBoxi(childorigin, naMakeVolumei(childsize * 2, childsize * 2, childsize * 2));
  NAInt cycle = naLog2i(childsize) % 8;
  if(cycle & 1){alignbox.vertex.x -= childsize;}
  if(cycle & 2){alignbox.vertex.y -= childsize;}
  if(cycle & 4){alignbox.vertex.z -= childsize;}

  return naMakeVertexiWithAlignment(childorigin, alignbox);
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF void naUpdateOctTreeNodeBubbling(NAOctTree* tree, NAOctTreeNode* parentnode, NAInt segment){
  NABool bubble = NA_TRUE;
  
  // We call the update callbacks depending whether this is a leaf node or
  // an inner node.
  if(parentnode->childlength == tree->leaflength){
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(parentnode->nodedata, segment, (const void* const) parentnode->child);
    }
  }else{
    if(tree->callbacks.childchanged){
      const void* childdata[8];
      naFillOctTreeNodeChildData(childdata, parentnode);
      bubble = tree->callbacks.childchanged(parentnode->nodedata, segment, childdata);
    }
  }
  
  // Then we propagate the message towards the root.
  if(bubble && parentnode->parentnode){
    #ifndef NDEBUG
      if(parentnode->segmentinparent == -1)
        naError("naUpdateOctTreeNode", "Inernal inconsistency detected");
    #endif
    naUpdateOctTreeNodeBubbling(tree, parentnode->parentnode, parentnode->segmentinparent);
  }
}



// Propagates a capturing update event the from the root to the leafes.
// All leaf nodes will be called with the leafchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that everly node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateOctTreeNodeCapturing(NAOctTree* tree, NAOctTreeNode* node){
  NABool bubble = NA_FALSE;
  
  if(node->childlength == tree->leaflength){
    // This node stores leafs
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);
    }
  }else{
    // this node stores subnodes
    if(node->child[0]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[0]);}
    if(node->child[1]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[1]);}
    if(node->child[2]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[2]);}
    if(node->child[3]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[3]);}
    if(node->child[4]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[4]);}
    if(node->child[5]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[5]);}
    if(node->child[6]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[6]);}
    if(node->child[7]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[7]);}
    if(bubble && tree->callbacks.childchanged){
      const void* childdata[8];
      naFillOctTreeNodeChildData(childdata, node);
      bubble = tree->callbacks.childchanged(node->nodedata, -1, childdata);
    }
  }
  
  return bubble;
}



// Expects the node to be the node to remove.
NA_HDEF void naRemoveOctTreeNode(NAOctTree* tree, NAOctTreeNode* node){
  if(node->parentnode){
  
    // Remove the segment from the tree
    #ifndef NDEBUG
      if(node->segmentinparent == -1)
        {naCrash("naRemoveOctTreeNode", "Inernal inconsistency detected: Segment number is -1"); return;}
    #endif
    node->parentnode->child[node->segmentinparent] = NA_NULL;
    
    // Test if there are other childs...
    if(node->parentnode->child[0] || node->parentnode->child[1] || node->parentnode->child[2] || node->parentnode->child[3] || node->parentnode->child[4] || node->parentnode->child[5] || node->parentnode->child[6] || node->parentnode->child[7]){
      // If there is at least one more child, we update the parent node.
      naUpdateOctTreeNodeBubbling(tree, node->parentnode, -1);
    }else{
      // If there are no more childs, we remove the parent.
      naRemoveOctTreeNode(tree, node->parentnode);
    }
  }else{
    // If this node has no parent, it was the root
    tree->root = NA_NULL;
  }
  // Finally, we deallocate the node.
  naDeallocOctTreeNode(tree, node);
}



// Allocates and initializes a new node which becomes the parent of the existing
// treeroot. This function replaces the current root with a higher-level root.
NA_HDEF void naGrowOctTreeNodeRoot(NAOctTree* tree){

  NAInt childsegment;
  NAOctTreeNode* parentnode;
  NAVertexi parentorigin = naGetOctTreeNodeParentOrigin(tree->root->childlength * 2, tree->root->childorigin[0]);
  
  parentnode = naAllocOctTreeNode(
          tree->root->childlength * 2,
          -1,
          NA_NULL,
          parentorigin,
          tree->callbacks.nodeallocator);
  
  childsegment = naGetOctTreeNodeSegment(parentnode, tree->root->childorigin[0]);
  parentnode->child[childsegment] = tree->root;
  tree->root->parentnode = parentnode;
  tree->root->segmentinparent = childsegment;
  if(tree->callbacks.childchanged){
    const void* childdata[8];
    naFillOctTreeNodeChildData(childdata, parentnode);
    tree->callbacks.childchanged(parentnode->nodedata, childsegment, childdata);
  }
  tree->root = parentnode;
}



// This methods checks if the root of the tree has only one child. As long
// as there is only one child in the root, the root is transferred to that
// very child to save memory and allow garbage collectors to collect.
NA_HDEF void naShrinkOctTreeNodeRoot(NAOctTree* tree){
  while(1){
    if(tree->root->childlength == tree->leaflength){
      // The root node has only leafes as childs.
      return;
    }else{
      // We count how many children there are.
      NAUInt childcount = 0;
      NAInt lastchildindex = -1;
      NAOctTreeNode* lastchild;
      if(tree->root->child[0]){childcount++; lastchildindex = 0;}
      if(tree->root->child[1]){childcount++; lastchildindex = 1;}
      if(tree->root->child[2]){childcount++; lastchildindex = 2;}
      if(tree->root->child[3]){childcount++; lastchildindex = 3;}
      if(tree->root->child[4]){childcount++; lastchildindex = 4;}
      if(tree->root->child[5]){childcount++; lastchildindex = 5;}
      if(tree->root->child[6]){childcount++; lastchildindex = 6;}
      if(tree->root->child[7]){childcount++; lastchildindex = 7;}
      #ifndef NDEBUG
        if(!childcount)
          naError("naShrinkOctTreeNodeRoot", "Root should not be empty.");
      #endif
      // If there are more than 1 child, return.
      if(childcount != 1){return;}  // note that it should be >1 but code sanity checks think 0 can happen as well.
      lastchild = tree->root->child[lastchildindex];
      // Now, the root has only one child node which now is stored in lastchild.
      // We adjust the child to become the new root.
      lastchild->parentnode = NA_NULL;
      lastchild->segmentinparent = -1;
      // We remove the child from the current root
      tree->root->child[lastchildindex] = NA_NULL;
      // ... deallocate the current root
      naDeallocOctTreeNode(tree, tree->root);
      // ...and set the new root.
      tree->root = lastchild;
    }
  }
}








// ////////////////////////////
// Iterator
// ////////////////////////////


#define NA_OCTTREE_ITERATOR_MODIFIER     0x01
#define NA_OCTTREE_ITERATOR_HAS_ORIGIN   0x02


// Checks whether the iterator is positioned at a leaf and it exists.
NA_HIDEF NABool naIsOctTreeIteratorAtLeaf(NAOctTreeIterator* iter){
  const NAOctTree* tree = naGetPtrConst(&(iter->tree));
  return (iter->curnode) && (iter->cursegment != -1) && (iter->curnode->child[iter->cursegment]) && (iter->curnode->childlength == tree->leaflength);
}



NA_HIDEF void naSetOctTreeIteratorLeafOrigin(NAOctTreeIterator* iter, NAVertexi origin){
  iter->leaforigin = origin;
  iter->flags |= NA_OCTTREE_ITERATOR_HAS_ORIGIN;
}



NA_HIDEF void naSetOctTreeIteratorCurNode(NAOctTreeIterator* iter, NAOctTreeNode* newnode){
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  iter->curnode = newnode;
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount++;}
  #endif
}



NA_HIDEF void naCreateOctTreeLeaf(NAOctTreeIterator* iter, const void* data){
  NAOctTree* tree;
  #ifndef NDEBUG
    if(!(iter->flags & NA_OCTTREE_ITERATOR_MODIFIER))
      naError("naCreateOctTreeLeaf", "iterator must be modifier");
    if(!(iter->flags & NA_OCTTREE_ITERATOR_HAS_ORIGIN))
      naError("naCreateOctTreeLeaf", "origin required for leaf creation");
  #endif

  tree = (NAOctTree*)naGetPtrMutable(&(iter->tree));

  if(!iter->curnode){
    // The tree root does not contain the coord or does not exist at all.
    if(tree->root){
      NABoxi nodebox;
      // We expand the tree at the root and call this function recursively.
      naGrowOctTreeNodeRoot(tree);
      nodebox = naGetOctTreeNodeBox(tree->root);
      if(naContainsBoxiVertex(nodebox, iter->leaforigin)){naSetOctTreeIteratorCurNode(iter, tree->root);}
    }else{
      // We create the very first node of this tree.
      NAVertexi rootorigin = naGetOctTreeNodeParentOrigin(tree->leaflength, iter->leaforigin);
      tree->root = naAllocOctTreeNode(
                                        tree->leaflength,
                                        -1,
                                        NA_NULL,
                                        rootorigin,
                                        tree->callbacks.nodeallocator);
      naSetOctTreeIteratorCurNode(iter, tree->root);
    }
    // We call this function recursively.
    naCreateOctTreeLeaf(iter, data);
    
  }else{
    NAInt segment;
    // We have a current node which is expected to contain the coord.
    #ifndef NDEBUG
      NABoxi nodebox = naGetOctTreeNodeBox(iter->curnode);
      if(!naContainsBoxiVertex(nodebox, iter->leaforigin))
        naError("naCreateOctTreeLeaf", "Cur node does not contain the desired origin.");
    #endif
        
    // There is a current node but no subnode has been identified.
    segment = naGetOctTreeNodeSegment(iter->curnode, iter->leaforigin);
    #ifndef NDEBUG
      if(iter->curnode->child[segment])
        naError("naCreateOctTreeLeaf", "Tree has been modified between positioning this iterator and creating a leaf. Don't do that. Memory leaks and major pointer confusion expected.");
    #endif

    if(iter->curnode->childlength == tree->leaflength){
      // The node can not be divided anymore. Create a leaf.
      iter->curnode->child[segment] = tree->callbacks.leafallocator(iter->curnode->childorigin[segment], tree->leaflength, tree->callbacks.userdata, data);
      iter->cursegment = segment;
      // Good ending. We created the leaf.
    }else{
      // We create an internal subnode and call this function recursively.
      NAOctTreeNode* newnode = naAddOctTreeNodeChild(tree, iter->curnode, segment);
      naSetOctTreeIteratorCurNode(iter, newnode);
      naCreateOctTreeLeaf(iter, data);
    }    
  }
}



NA_HDEF void* naGetOctTreeCurMutableWithData(NAOctTreeIterator* iter, NABool create, const void* data){
  #ifndef NDEBUG
    if(naIsPtrConst(&(iter->tree)))
      naError("naGetOctTreeCurMutable", "Mutating iterator which is not mutable");
  #endif
  if(naIsOctTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    if(create){
      naCreateOctTreeLeaf(iter, data);
      return iter->curnode->child[iter->cursegment];
    }else{
      return NA_NULL;
    }
  }
}



NA_DEF NAOctTreeIterator naMakeOctTreeAccessor(const NAOctTree* tree){
  NAOctTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitOctTreeIterator(&iter);
  return iter;
}



NA_DEF NAOctTreeIterator naMakeOctTreeMutator(NAOctTree* tree){
  NAOctTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitOctTreeIterator(&iter);
  return iter;
}



NA_DEF NAOctTreeIterator naMakeOctTreeModifier(NAOctTree* tree){
  NAOctTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitOctTreeIterator(&iter);
  iter.flags |= NA_OCTTREE_ITERATOR_MODIFIER;
  return iter;
}



NA_DEF void naClearOctTreeIterator(NAOctTreeIterator* iter){
  #ifndef NDEBUG
    NAOctTree* mutabletree = (NAOctTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  naCleanupPtr(&(iter->tree), NA_NULL);
}



NA_DEF void naResetOctTreeIterator(NAOctTreeIterator* iter){
  naSetOctTreeIteratorCurNode(iter, NA_NULL);
  iter->cursegment = -1;
  iter->flags &= ~NA_OCTTREE_ITERATOR_HAS_ORIGIN;
}



NA_DEF const void* naGetOctTreeCurConst(NAOctTreeIterator* iter){
  if(naIsOctTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    return NA_NULL;
  }
}



NA_DEF void* naGetOctTreeCurMutable(NAOctTreeIterator* iter, NABool create){
  return naGetOctTreeCurMutableWithData(iter, create, NA_NULL);
}



NA_DEF NAVertexi naGetOctTreeCurOrigin(NAOctTreeIterator* iter){
  #ifndef NDEBUG
    if(!(iter->flags & NA_OCTTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetOctTreeCurOrigin", "Iterator has no origin");
  #endif
  return iter->leaforigin;
}



NA_DEF void naRemoveOctTreeCur(NAOctTreeIterator* iter){
  NAOctTree* tree;
  if(!iter->curnode){return;}
  tree = naGetPtrMutable(&(iter->tree));
    
  // First, we destroy the data of the leaf chunk
  tree->callbacks.leafdeallocator(iter->curnode->child[iter->cursegment], tree->callbacks.userdata);
  iter->curnode->child[iter->cursegment] = NA_NULL;

  // Test if there are other childs in the current parent.
  if(iter->curnode->child[0] || iter->curnode->child[1] || iter->curnode->child[2] || iter->curnode->child[3] || iter->curnode->child[4] || iter->curnode->child[5] || iter->curnode->child[6] || iter->curnode->child[7]){
    // If there is at least one more child, we update the node.
    naUpdateOctTreeNodeBubbling(tree, iter->curnode, -1);
  }else{
    // If there are no more childs, we remove the node.
    NAOctTreeNode* delnode = iter->curnode;
    naSetOctTreeIteratorCurNode(iter, NA_NULL);
    naRemoveOctTreeNode(tree, delnode);
  }
  if(tree->root){
    // Now, the root may have only one child left, so we check if we can adjust
    // the tree accordingly.
    naShrinkOctTreeNodeRoot(tree);
  }
}



NA_DEF NABool naIterateOctTree(NAOctTreeIterator* iter, const NABoxi* limit, NABool create){
  const NAOctTree* tree;
  #ifndef NDEBUG
    if(create && !limit)
      naError("naIterateOctTree", "create should not be true if there is no limit");
  #endif

  tree = (const NAOctTree*)naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetOctTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->curnode)
      {naCrash("naIterateOctTree", "No current node after setting the node to the root"); return NA_FALSE;}
    #endif
    // todo: code sanity warning
  }

  // We go to the next segment. When we came to this function with segment -1,
  // we therefore start with the first segment.
  iter->cursegment++;
  
  // Search for a segment which is available.
  while(iter->cursegment < 8){
    NABool considerchild = NA_TRUE;
    if(limit){
      NABoxi childbox = naMakeBoxi(iter->curnode->childorigin[iter->cursegment], naMakeVolumei(iter->curnode->childlength, iter->curnode->childlength, iter->curnode->childlength));
      considerchild = naIsBoxiUseful(naMakeBoxiWithBoxIntersection(childbox, *limit));
    }
    if(considerchild){
      if(iter->curnode->child[iter->cursegment]){
        // We have a child which is present and overlaps with the limit if
        // available.
        break;
      }else if(create){
        // We trick the iterator to create a leaf.
        iter->leaforigin = iter->curnode->childorigin[iter->cursegment];
        iter->cursegment = -1;
        naCreateOctTreeLeaf(iter, NA_NULL);
        #ifndef NDEBUG
          if(iter->cursegment == -1)
          {naCrash("naIterateOctTree", "Segment of newly created Leaf is -1"); return NA_FALSE;}
        #endif  
        break;
      }
    }
    iter->cursegment++;
  }
  
  if(iter->cursegment < 8){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(iter->curnode->childlength == tree->leaflength){
      naSetOctTreeIteratorLeafOrigin(iter, iter->curnode->childorigin[iter->cursegment]);
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetOctTreeIteratorCurNode(iter, iter->curnode->child[iter->cursegment]);
      // We start looking in the subnode from the start.
      iter->cursegment = -1;
      return naIterateOctTree(iter, limit, create);
    }
    
  }else{
    // There is no more segment available in this node. Go upwards.
    if(iter->curnode->parentnode){
      #ifndef NDEBUG
        if(iter->curnode->segmentinparent == -1)
          naError("naIterateOctTreeNode", "Inernal inconsistency detected: Segment in parent should not be -1");
      #endif
      iter->cursegment = iter->curnode->segmentinparent;
      naSetOctTreeIteratorCurNode(iter, iter->curnode->parentnode);
      return naIterateOctTree(iter, limit, create);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naSetOctTreeIteratorCurNode(iter, NA_NULL);
      iter->cursegment = -1;
      iter->flags &= ~NA_OCTTREE_ITERATOR_HAS_ORIGIN;
      return NA_FALSE;
    }
  }
}



// Moves the iterator to the closest parent node containing coord.
NA_HDEF NABool naLocateOctTreeNode(NAOctTreeIterator* iter, NAVertexi origin){
  NABoxi nodebox;
  const NAOctTree* tree = naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetOctTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->curnode)
      {naCrash("naLocateOctTreeNode", "No current node after setting the current node to the root"); return NA_FALSE;}
    #endif
    // todo: code sanity warning
  }

  nodebox = naGetOctTreeNodeBox(iter->curnode);
  if(naContainsBoxiVertex(nodebox, origin)){
  
    // The coord is stored somewhere inside the box of this node
    NAInt segment = naGetOctTreeNodeSegment(iter->curnode, origin);
    if(iter->curnode->child[segment]){
      // There is a segment available which contains the coord.
      if(iter->curnode->childlength == tree->leaflength){
        // We are at leaf-level. We found it!
        iter->cursegment = segment;
        // Good ending.
        return NA_TRUE;
      }else{
        // Go on searching in the sub-node
        naSetOctTreeIteratorCurNode(iter, iter->curnode->child[segment]);
        return naLocateOctTreeNode(iter, origin);
      }
      
    }else{
      // There is no segment where the coord should be. No leaf found.
      iter->cursegment = -1;
      // Bad ending. We are stuck with the closest parent node.
      return NA_FALSE;
    }

  }else{
  
    // The coord is not stored within the box of this node.
    if(iter->curnode->parentnode){
      // We have a parent. Search there.
      naSetOctTreeIteratorCurNode(iter, iter->curnode->parentnode);
      return naLocateOctTreeNode(iter, origin);
    }else{
      // There is no parent
      naSetOctTreeIteratorCurNode(iter, NA_NULL);
      iter->cursegment = -1;
      // Bad ending. The desired coord lies outside of the whole tree.
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naLocateOctTreeCoord(NAOctTreeIterator* iter, NAVertexi coord){
  const NAOctTree* tree = naGetPtrConst(&(iter->tree));

  NABoxi leafalign = naMakeBoxi(naMakeVertexi(0, 0, 0), naMakeVolumei(tree->leaflength, tree->leaflength, tree->leaflength));
  NAVertexi neworigin = naMakeVertexiWithAlignment(coord, leafalign);

  naSetOctTreeIteratorLeafOrigin(iter, neworigin);  
  return naLocateOctTreeNode(iter, neworigin);
}



NA_DEF NABool naLocateOctTreeSteps(NAOctTreeIterator* iter, NAInt stepx, NAInt stepy, NAInt stepz){
  NAVertexi neworigin;
  const NAOctTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_OCTTREE_ITERATOR_HAS_ORIGIN))
      naError("naLocateOctTreeSteps", "Iterator must be positioned first before moving relative.");
  #endif

  neworigin = iter->leaforigin;
  neworigin.x += stepx * tree->leaflength;
  neworigin.y += stepy * tree->leaflength;
  neworigin.z += stepz * tree->leaflength;

  naSetOctTreeIteratorLeafOrigin(iter, neworigin);
  return naLocateOctTreeNode(iter, neworigin);
}



NA_DEF void naUpdateOctTreeCur(NAOctTreeIterator* iter){
  if(iter->curnode){
    NAOctTree* tree = naGetPtrMutable(&(iter->tree));
    naUpdateOctTreeNodeBubbling(tree, iter->curnode, iter->cursegment);
  }
}



NA_DEF void naUpdateOctTree(NAOctTree* tree){
  if(tree->root){
    naUpdateOctTreeNodeCapturing(tree, tree->root);
  }
}








// ////////////////////////////
// Tree
// ////////////////////////////


NA_DEF NAOctTree* naInitOctTree(NAOctTree* tree, NAInt leaflength, NAOctTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naInitOctTree", "tree is Null-Pointer"); return NA_NULL;}
    if(!callbacks.leafallocator)
      naError("naInitOctTree", "Must have a data allocator");
    if(!callbacks.leafdeallocator)
      naError("naInitOctTree", "Must have a data destructor");
    if(leaflength < 1)
      naError("naInitOctTree", "leaflength can not be smaller than 1");
  #endif
  tree->leaflength = leaflength;
  tree->callbacks = callbacks;
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}



NA_DEF NAOctTree* naInitOctTreeCopy(NAOctTree* newtree, const NAOctTree* duptree){
  NAOctTreeIterator newiter;
  NAOctTreeIterator dupiter;
  newtree = naInitOctTree(newtree, duptree->leaflength, duptree->callbacks);

  newiter = naMakeOctTreeModifier(newtree);
  dupiter = naMakeOctTreeAccessor(duptree);
  while(naIterateOctTree(&dupiter, NA_NULL, NA_FALSE)){
    const void* dupdata = naGetOctTreeCurConst(&dupiter);
    NAVertexi duporigin = naGetOctTreeCurOrigin(&dupiter);
    naLocateOctTreeCoord(&newiter, duporigin);
    naCreateOctTreeLeaf(&newiter, dupdata);
  }
  
  naClearOctTreeIterator(&dupiter);
  naClearOctTreeIterator(&newiter);

  return newtree;
}



NA_DEF NAOctTree* naInitOctTreeCopyMasked(NAOctTree* newtree, const NAOctTree* duptree, const NAOctTree* masktree){
  NAOctTreeIterator newiter;
  NAOctTreeIterator dupiter;
  NAOctTreeIterator maskiter;
  newtree = naInitOctTree(newtree, duptree->leaflength, duptree->callbacks);

  newiter = naMakeOctTreeModifier(newtree);
  dupiter = naMakeOctTreeAccessor(duptree);
  maskiter = naMakeOctTreeAccessor(masktree);
  while(naIterateOctTree(&maskiter, NA_NULL, NA_FALSE)){
    const void* dupdata;
    NAVertexi duporigin = naGetOctTreeCurOrigin(&maskiter);
    naLocateOctTreeCoord(&dupiter, duporigin);
    dupdata = naGetOctTreeCurConst(&dupiter);
    naLocateOctTreeCoord(&newiter, duporigin);
    naCreateOctTreeLeaf(&newiter, dupdata);
  }
  
  naClearOctTreeIterator(&dupiter);
  naClearOctTreeIterator(&newiter);
  naClearOctTreeIterator(&maskiter);

  return newtree;
}





NA_DEF NAOctTree* naInitOctTreeCopyShifted(NAOctTree* newtree, const NAOctTree* duptree, NAVolumei shift){
  NAInt x1bound;
  NAInt y1bound;
  NAInt z1bound;
  NAInt x2bound;
  NAInt y2bound;
  NAInt z2bound;
  NABoxi box0;
  NABoxi box1;
  NABoxi box2;
  NABoxi box3;
  NABoxi box4;
  NABoxi box5;
  NABoxi box6;
  NABoxi box7;
  NAOctTreeIterator newiter;
  NAOctTreeIterator iter;

  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyOctTreeWithShift", "newtree is Null-Pointer"); return NA_NULL;}
    if(!duptree->callbacks.datacopier)
      {naCrash("naCopyOctTreeWithShift", "Data copier callback required for this function to work."); return NA_NULL;}
  #endif
  newtree = naInitOctTree(newtree, duptree->leaflength, duptree->callbacks);
  if(!duptree->root){return newtree;}

  // Create four boxs which denote the boxs in the new shifted tree which
  // are aligned to a leaflength.
  x1bound = ((shift.width % duptree->leaflength) + duptree->leaflength ) % duptree->leaflength;
  y1bound = ((shift.height % duptree->leaflength) + duptree->leaflength ) % duptree->leaflength;
  z1bound = ((shift.depth % duptree->leaflength) + duptree->leaflength ) % duptree->leaflength;
  x2bound = duptree->leaflength - x1bound;
  y2bound = duptree->leaflength - y1bound;
  z2bound = duptree->leaflength - z1bound;
  box0 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height, shift.depth), naMakeVolumeiE(x2bound, y2bound, z2bound));
  box1 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height, shift.depth), naMakeVolumeiE(x1bound, y2bound, z2bound));
  box2 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height + y2bound, shift.depth), naMakeVolumeiE(x2bound, y1bound, z2bound));
  box3 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height + y2bound, shift.depth), naMakeVolumeiE(x1bound, y1bound, z2bound));
  box4 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height, shift.depth + z2bound), naMakeVolumeiE(x2bound, y2bound, z1bound));
  box5 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height, shift.depth + z2bound), naMakeVolumeiE(x1bound, y2bound, z1bound));
  box6 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height + y2bound, shift.depth + z2bound), naMakeVolumeiE(x2bound, y1bound, z1bound));
  box7 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height + y2bound, shift.depth + z2bound), naMakeVolumeiE(x1bound, y1bound, z1bound));
  
  newiter = naMakeOctTreeModifier(newtree);
  iter = naMakeOctTreeAccessor(duptree);
  
  while(naIterateOctTree(&iter, NA_NULL, NA_FALSE)){
    const void* dupchunk = naGetOctTreeCurConst(&iter);
    NAVertexi origin = naGetOctTreeCurOrigin(&iter);

    NAVertexi neworigin;
    void* newdata;
    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 8 new leaves.
    if(!naIsBoxiEmpty(box0)){
      neworigin = naMakeVertexi(origin.x + box0.vertex.x, origin.y + box0.vertex.y, origin.z + box0.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, y1bound, z1bound), dupchunk, naMakeVertexi(0, 0, 0), box0.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box1)){
      neworigin = naMakeVertexi(origin.x + box1.vertex.x, origin.y + box1.vertex.y, origin.z + box1.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, y1bound, z1bound), dupchunk, naMakeVertexi(x2bound, 0, 0), box1.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box2)){
      neworigin = naMakeVertexi(origin.x + box2.vertex.x, origin.y + box2.vertex.y, origin.z + box2.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, 0, z1bound), dupchunk, naMakeVertexi(0, y2bound, 0), box2.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box3)){
      neworigin = naMakeVertexi(origin.x + box3.vertex.x, origin.y + box3.vertex.y, origin.z + box3.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, 0, z1bound), dupchunk, naMakeVertexi(x2bound, y2bound, 0), box3.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box4)){
      neworigin = naMakeVertexi(origin.x + box4.vertex.x, origin.y + box4.vertex.y, origin.z + box4.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, y1bound, 0), dupchunk, naMakeVertexi(0, 0, z2bound), box4.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box5)){
      neworigin = naMakeVertexi(origin.x + box5.vertex.x, origin.y + box5.vertex.y, origin.z + box5.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, y1bound, 0), dupchunk, naMakeVertexi(x2bound, 0, z2bound), box5.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box6)){
      neworigin = naMakeVertexi(origin.x + box6.vertex.x, origin.y + box6.vertex.y, origin.z + box6.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, 0, 0), dupchunk, naMakeVertexi(0, y2bound, z2bound), box6.volume, duptree->leaflength);
    }
    if(!naIsBoxiEmpty(box7)){
      neworigin = naMakeVertexi(origin.x + box7.vertex.x, origin.y + box7.vertex.y, origin.z + box7.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, 0, 0), dupchunk, naMakeVertexi(x2bound, y2bound, z2bound), box7.volume, duptree->leaflength);
    }
  }
  naClearOctTreeIterator(&iter);
  naClearOctTreeIterator(&newiter);
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateOctTree(newtree);
  return newtree;
}



NA_DEF void naClearOctTree(NAOctTree* tree){
  naEmptyOctTree(tree);
}




NA_DEF void naEmptyOctTree(NAOctTree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("naEmptyOctTree", "There are still iterators running on this tree. Did you miss a naClearOctTreeIterator call?");
  #endif
  if(tree->root){naDeallocOctTreeNode(tree, tree->root);}
  tree->root = NA_NULL;
}



NA_DEF void* naGetOctTreeRootNodeData(NAOctTree* tree){
  if(tree->root){return tree->root->nodedata;}
  return NA_NULL;
}



NA_DEF void naSetOctTreeInBox(NAOctTree* tree, NABoxi box, NAOctTreeDataSetter datasetter, const void* userdata){
  NABoxi chunkbox;
  NAOctTreeIterator iter;
  chunkbox = naMakeBoxi(naMakeVertexi(0, 0, 0), naMakeVolumei(tree->leaflength, tree->leaflength, tree->leaflength));

  iter = naMakeOctTreeModifier(tree);
  while(naIterateOctTree(&iter, &box, NA_TRUE)){
    NABoxi clampbox;
    void* curchunk = naGetOctTreeCurMutable(&iter, NA_TRUE);
    chunkbox.vertex = naGetOctTreeCurOrigin(&iter);
    clampbox = naClampBoxiToBox(box, chunkbox);
    clampbox.vertex.x -= chunkbox.vertex.x;
    clampbox.vertex.y -= chunkbox.vertex.y;
    datasetter(curchunk, clampbox, tree->leaflength, userdata);
  }
  naClearOctTreeIterator(&iter);

}








// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
