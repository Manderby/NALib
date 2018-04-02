
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../NAQuadTree.h"
#include "../NAMemory.h"



struct NAQuadTreeNode{
  NAInt childexponent;
  NAInt segmentinparent;
  NAQuadTreeNode* parentnode;
  void* child[4];
  NAPos childorigin[4];
  void* nodedata;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};




// ////////////////////////////
// Node
// ////////////////////////////


NA_HIDEF NARect naGetQuadTreeNodeRect(NAQuadTreeNode* node){
  return naMakeRect(node->childorigin[0], naMakeSize(node->childexponent * 2., node->childexponent * 2.));
}



NA_HDEF void naFillQuadTreeNodeChildData(const void* childdata[4], NAQuadTreeNode* node){
  childdata[0] = (node->child[0]) ? (((NAQuadTreeNode*)(node->child[0]))->nodedata) : NA_NULL;
  childdata[1] = (node->child[1]) ? (((NAQuadTreeNode*)(node->child[1]))->nodedata) : NA_NULL;
  childdata[2] = (node->child[2]) ? (((NAQuadTreeNode*)(node->child[2]))->nodedata) : NA_NULL;
  childdata[3] = (node->child[3]) ? (((NAQuadTreeNode*)(node->child[3]))->nodedata) : NA_NULL;
}



// Creates an inner node.
NA_HDEF NAQuadTreeNode* naAllocQuadTreeNode(NAInt childexponent, NAInt segmentinparent, NAQuadTreeNode* parentnode, NAPos origin, NAQuadTreeNode* dummynode, NAQuadTreeNodeAllocator allocator){
  NAQuadTreeNode* node = naAlloc(NAQuadTreeNode);
  node->childexponent = childexponent;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->child[0] = dummynode;
  node->child[1] = dummynode;
  node->child[2] = dummynode;
  node->child[3] = dummynode;
  node->childorigin[0] = naMakePos(origin.x                      , origin.y);
  node->childorigin[1] = naMakePos(origin.x + node->childexponent, origin.y);
  node->childorigin[2] = naMakePos(origin.x                      , origin.y + node->childexponent);
  node->childorigin[3] = naMakePos(origin.x + node->childexponent, origin.y + node->childexponent);
  if(allocator){
    node->nodedata = allocator(origin, childexponent * 2);
  }else{
    node->nodedata = NA_NULL;
  }
  #ifndef NDEBUG
    node->itercount = 0;
  #endif
  return node;
}



// Deallocates one node. Deallocates all subnodes and leafes in the process.
NA_HDEF void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  #ifndef NDEBUG
    if(node->itercount)
      naError("naDeallocQuadTreeNode", "Iterators still running on this node. Did you forget a call to NAClearQuadTreeIterator?");
  #endif

  if(node->childexponent == tree->configuration.minleafexponent){
    if(node->child[0]){tree->configuration.leafdeallocator(node->child[0], tree->configuration.userdata);}
    if(node->child[1]){tree->configuration.leafdeallocator(node->child[1], tree->configuration.userdata);}
    if(node->child[2]){tree->configuration.leafdeallocator(node->child[2], tree->configuration.userdata);}
    if(node->child[3]){tree->configuration.leafdeallocator(node->child[3], tree->configuration.userdata);}
  }else{
    if(node->child[0]){naDeallocQuadTreeNode(tree, node->child[0]);}
    if(node->child[1]){naDeallocQuadTreeNode(tree, node->child[1]);}
    if(node->child[2]){naDeallocQuadTreeNode(tree, node->child[2]);}
    if(node->child[3]){naDeallocQuadTreeNode(tree, node->child[3]);}
  }

  if(node->nodedata && tree->configuration.nodedeallocator){
    tree->configuration.nodedeallocator(node->nodedata);
  }
  naFree(node);
}



// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAQuadTreeNode* naAddQuadTreeNodeChild(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segmentinparent){
  NAQuadTreeNode* node;
  #ifndef NDEBUG
    if(parentnode->child[segmentinparent])
      naError("naAddQuadTreeNodeChild", "A child already exists at that segment of the parent node.");
  #endif

  node = naAllocQuadTreeNode(
          parentnode->childexponent / 2,
          segmentinparent,
          parentnode,
          parentnode->childorigin[segmentinparent],
          NA_NULL,
          tree->configuration.nodeallocator);
  
  parentnode->child[segmentinparent] = node;
  return node;
}



// Returns the segment index [0-3] in which the given pos can be found.
// Warning: The position is expected to be inside this node.
NA_HIDEF NAInt naGetQuadTreeNodeSegment(NAQuadTreeNode* node, NAPos pos){
  NAInt segment = 0;
  if(pos.x >= node->childorigin[3].x){segment |= 1;}
  if(pos.y >= node->childorigin[3].y){segment |= 2;}
  #ifndef NDEBUG
    if(!naContainsRectiPos(naMakeRecti(REMOVEPosToPosi(node->childorigin[segment]), naMakeSizei(node->childexponent, node->childexponent)), REMOVEPosToPosi(pos)))
      naError("naGetQuadTreeNodeSegment", "node does not actually contains pos");
  #endif
  return segment;
}



NA_HDEF NAPos naGetQuadTreeNodeParentOrigin(NAInt childexponent, NAPos childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  //
  // Implementation note: To be sure that the childorigin stays aligned, we
  // simply take that origin als the alignment origin and work from there.

  NARect alignrect = naMakeRect(childorigin, naMakeSize(childexponent * 2., childexponent * 2.));
  NAInt cycle = naLog2i(childexponent) % 4;
  if(cycle & 1){alignrect.pos.x -= childexponent;}
  if(cycle & 2){alignrect.pos.y -= childexponent;}

  return REMOVEPosiToPos(naMakePosiWithAlignment(REMOVEPosToPosi(childorigin), REMOVERectToRecti(alignrect)));
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF void naUpdateQuadTreeNodeBubbling(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segment){
  NABool bubble = NA_TRUE;
  #ifndef NDEBUG
    if(parentnode == NA_NULL)
      naError("naUpdateQuadTreeNodeBubbling", "Parent is null.");
  #endif
  
  // We call the update callbacks depending whether this is a leaf node or
  // an inner node.
  if(parentnode->childexponent == naGetQuadTreeMinLeafExponent(tree)){
    if(tree->configuration.leafchanged){
      bubble = tree->configuration.leafchanged(parentnode->nodedata, segment, (const void* const) parentnode->child, parentnode->childexponent);
    }
  }else{
    if(tree->configuration.childchanged){
      const void* childdata[4];
      naFillQuadTreeNodeChildData(childdata, parentnode);
      bubble = tree->configuration.childchanged(parentnode->nodedata, segment, childdata, parentnode->childexponent);
    }
  }
  
  // Then we propagate the message towards the root.
  if(bubble && parentnode->parentnode){
    #ifndef NDEBUG
      if(parentnode->segmentinparent == -1)
        naError("naUpdateQuadTreeNode", "Internal inconsistency detected");
    #endif
    naUpdateQuadTreeNodeBubbling(tree, parentnode->parentnode, parentnode->segmentinparent);
  }
}



// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the leafchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateQuadTreeNodeCapturing(NAQuadTree* tree, NAQuadTreeNode* node){
  NABool bubble = NA_FALSE;
  
  if(node->childexponent == naGetQuadTreeMinLeafExponent(tree)){
    // This node stores leafs
    if(tree->configuration.leafchanged){
      bubble = tree->configuration.leafchanged(node->nodedata, -1, (const void* const) node->child, node->childexponent);
    }
  }else{
    // this node stores subnodes
    if(node->child[0]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[0]);}
    if(node->child[1]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[1]);}
    if(node->child[2]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[2]);}
    if(node->child[3]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[3]);}
    if(bubble && tree->configuration.childchanged){
      const void* childdata[4];
      naFillQuadTreeNodeChildData(childdata, node);
      bubble = tree->configuration.childchanged(node->nodedata, -1, childdata, node->childexponent);
    }
  }
  
  return bubble;
}



// Expects the node to be the node to remove.
NA_HDEF void naRemoveQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->parentnode){
  
    // Remove the segment from the tree
    #ifndef NDEBUG
      if(node->segmentinparent == -1)
        {naCrash("naRemoveQuadTreeNode", "Inernal inconsistency detected: Segment number is -1"); return;}
    #endif
    node->parentnode->child[node->segmentinparent] = NA_NULL;
    
    // Test if there are other childs...
    if((node->parentnode->child[0]) || (node->parentnode->child[1]) || (node->parentnode->child[2]) || (node->parentnode->child[3])){
      // If there is at least one more child, we update the parent node.
      naUpdateQuadTreeNodeBubbling(tree, node->parentnode, -1);
    }else{
      // If there are no more childs, we remove the parent.
      naRemoveQuadTreeNode(tree, node->parentnode);
    }
  }else{
    // If this node has no parent, it was the root
    tree->root = NA_NULL;
  }
  // Finally, we deallocate the node.
  naDeallocQuadTreeNode(tree, node);
}



// Allocates and initializes a new node which becomes the parent of the existing
// treeroot. This function replaces the current root with a higher-level root.
NA_HDEF void naGrowQuadTreeNodeRoot(NAQuadTree* tree){

  NAInt childsegment;
  NAQuadTreeNode* parentnode;
  NAPos parentorigin = naGetQuadTreeNodeParentOrigin(tree->root->childexponent * 2, tree->root->childorigin[0]);
  
  parentnode = naAllocQuadTreeNode(
          tree->root->childexponent * 2,
          -1,
          NA_NULL,
          parentorigin,
          NA_NULL,
          tree->configuration.nodeallocator);
  
  childsegment = naGetQuadTreeNodeSegment(parentnode, tree->root->childorigin[0]);
  parentnode->child[childsegment] = tree->root;
  tree->root->parentnode = parentnode;
  tree->root->segmentinparent = childsegment;
  if(tree->configuration.childchanged){
    const void* childdata[4];
    naFillQuadTreeNodeChildData(childdata, parentnode);
    tree->configuration.childchanged(parentnode->nodedata, childsegment, childdata, parentnode->childexponent);
  }
  tree->root = parentnode;
}



// This methods checks if the root of the tree has only one child. As long
// as there is only one child in the root, the root is transferred to that
// very child to save memory and allow garbage collectors to collect.
NA_HDEF void naShrinkQuadTreeNodeRoot(NAQuadTree* tree){
  while(1){
    if(tree->root->childexponent == naGetQuadTreeMinLeafExponent(tree)){
      // The root node has only leafes as childs.
      return;
    }else{
      // We count how many children there are.
      NAUInt childcount = 0;
      NAInt lastchildindex = -1;
      NAQuadTreeNode* lastchild;
      if(tree->root->child[0]){childcount++; lastchildindex = 0;}
      if(tree->root->child[1]){childcount++; lastchildindex = 1;}
      if(tree->root->child[2]){childcount++; lastchildindex = 2;}
      if(tree->root->child[3]){childcount++; lastchildindex = 3;}
      #ifndef NDEBUG
        if(!childcount)
          naError("naShrinkQuadTreeNodeRoot", "Root should not be empty.");
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
      naDeallocQuadTreeNode(tree, tree->root);
      // ...and set the new root.
      tree->root = lastchild;
    }
  }
}








// ////////////////////////////
// Iterator
// ////////////////////////////


#define NA_QUADTREE_ITERATOR_MODIFIER     0x01
#define NA_QUADTREE_ITERATOR_HAS_ORIGIN   0x02


// Checks whether the iterator is positioned at a leaf and it exists.
NA_HIDEF NABool naIsQuadTreeIteratorAtLeaf(NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  return (iter->curnode) && (iter->cursegment != -1) && (iter->curnode->child[iter->cursegment]) && (iter->curnode->childexponent == naGetQuadTreeMinLeafExponent(tree));
}



NA_HIDEF void naSetQuadTreeIteratorLeafOrigin(NAQuadTreeIterator* iter, NAPos origin){
  iter->leaforigin = origin;
  iter->flags |= NA_QUADTREE_ITERATOR_HAS_ORIGIN;
}



NA_HIDEF void naSetQuadTreeIteratorCurNode(NAQuadTreeIterator* iter, NAQuadTreeNode* newnode){
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  iter->curnode = newnode;
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount++;}
  #endif
}



NA_HIDEF void naCreateQuadTreeLeaf(NAQuadTreeIterator* iter, const void* data){
  NAQuadTree* tree;

  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_MODIFIER))
      naError("naCreateQuadTreeLeaf", "iterator must be modifier");
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naCreateQuadTreeLeaf", "origin required for leaf creation");
  #endif

  tree = (NAQuadTree*)naGetPtrMutable(&(iter->tree));

  if(!iter->curnode){
    // The tree root does not contain the coord or does not exist at all.
    if(tree->root){
      NARect noderect;
      // We expand the tree at the root and call this function recursively.
      naGrowQuadTreeNodeRoot(tree);
      noderect = naGetQuadTreeNodeRect(tree->root);
      if(naContainsRectiPos(REMOVERectToRecti(noderect), REMOVEPosToPosi(iter->leaforigin))){naSetQuadTreeIteratorCurNode(iter, tree->root);}
    }else{
      // We create the very first node of this tree.
      NAInt minleafexponent = naGetQuadTreeMinLeafExponent(tree);
      NAPos rootorigin = naGetQuadTreeNodeParentOrigin(minleafexponent, iter->leaforigin);
      tree->root = naAllocQuadTreeNode(
                                        minleafexponent,
                                        -1,
                                        NA_NULL,
                                        rootorigin,
                                        NA_NULL,
                                        tree->configuration.nodeallocator);
      naSetQuadTreeIteratorCurNode(iter, tree->root);
    }
    // We call this function recursively.
    naCreateQuadTreeLeaf(iter, data);
    
  }else{
    NAInt segment;

    // We have a current node which is expected to contain the coord.
    #ifndef NDEBUG
      NARect noderect = naGetQuadTreeNodeRect(iter->curnode);
      if(!naContainsRectiPos(REMOVERectToRecti(noderect), REMOVEPosToPosi(iter->leaforigin)))
        naError("naCreateQuadTreeLeaf", "Cur node does not contain the desired origin.");
    #endif
        
    // There is a current node but no subnode has been identified.
    segment = naGetQuadTreeNodeSegment(iter->curnode, iter->leaforigin);
    #ifndef NDEBUG
      if(iter->curnode->child[segment])
        naError("naCreateQuadTreeLeaf", "Tree has been modified between positioning this iterator and creating a leaf. Don't do that. Memory leaks and major pointer confusion expected.");
    #endif

    NAInt minleafexponent = naGetQuadTreeMinLeafExponent(tree);
    if(iter->curnode->childexponent == minleafexponent){
      // The node can not be divided anymore. Create a leaf.
      iter->curnode->child[segment] = tree->configuration.leafallocator(iter->curnode->childorigin[segment], minleafexponent, tree->configuration.userdata, data);
      iter->cursegment = segment;
      // Good ending. We created the leaf.
    }else{
      // We create an internal subnode and call this function recursively.
      NAQuadTreeNode* newnode = naAddQuadTreeNodeChild(tree, iter->curnode, segment);
      naSetQuadTreeIteratorCurNode(iter, newnode);
      naCreateQuadTreeLeaf(iter, data);
    }    
  }
}



NA_HDEF void* naGetQuadTreeCurMutableWithData(NAQuadTreeIterator* iter, NABool create, const void* data){
  #ifndef NDEBUG
    if(naIsPtrConst(&(iter->tree)))
      naError("naGetQuadTreeCurMutable", "Mutating iterator which is not mutable");
  #endif
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    if(create){
      naCreateQuadTreeLeaf(iter, data);
      return iter->curnode->child[iter->cursegment];
    }else{
      return NA_NULL;
    }
  }
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeAccessor(const NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitQuadTreeIterator(&iter);
  return iter;
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeMutator(NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitQuadTreeIterator(&iter);
  return iter;
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeModifier(NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitQuadTreeIterator(&iter);
  iter.flags |= NA_QUADTREE_ITERATOR_MODIFIER;
  return iter;
}



NA_DEF void naClearQuadTreeIterator(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    NAQuadTree* mutabletree = (NAQuadTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  naCleanupPtr(&(iter->tree), NA_NULL);
}



NA_DEF void naResetQuadTreeIterator(NAQuadTreeIterator* iter){
  naSetQuadTreeIteratorCurNode(iter, NA_NULL);
  iter->cursegment = -1;
  iter->flags &= ~NA_QUADTREE_ITERATOR_HAS_ORIGIN;
}



NA_DEF const void* naGetQuadTreeCurConst(NAQuadTreeIterator* iter){
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    return NA_NULL;
  }
}



NA_DEF void* naGetQuadTreeCurMutable(NAQuadTreeIterator* iter, NABool create){
  return naGetQuadTreeCurMutableWithData(iter, create, NA_NULL);
}



NA_DEF NAPos naGetQuadTreeCurOrigin(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetQuadTreeCurOrigin", "Iterator has no origin");
  #endif
  return iter->leaforigin;
}



NA_DEF NARect naGetQuadTreeCurRect(NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetQuadTreeCurRect", "Iterator has no origin");
  #endif
  return naMakeRect(iter->leaforigin, naMakeSize(naGetQuadTreeMinLeafExponent(tree), naGetQuadTreeMinLeafExponent(tree)));
}



NA_DEF void naRemoveQuadTreeCur(NAQuadTreeIterator* iter){
  NAQuadTree* tree;
  if(!iter->curnode){return;}
  tree = naGetPtrMutable(&(iter->tree));
    
  // First, we destroy the data of the leaf chunk
  tree->configuration.leafdeallocator(iter->curnode->child[iter->cursegment], tree->configuration.userdata);
  iter->curnode->child[iter->cursegment] = NA_NULL;

  // Test if there are other childs in the current parent.
  if((iter->curnode->child[0]) || (iter->curnode->child[1]) || (iter->curnode->child[2]) || (iter->curnode->child[3])){
    // If there is at least one more child, we update the node.
    naUpdateQuadTreeNodeBubbling(tree, iter->curnode, -1);
  }else{
    // If there are no more childs, we remove the node.
    NAQuadTreeNode* delnode = iter->curnode;
    naSetQuadTreeIteratorCurNode(iter, NA_NULL);
    naRemoveQuadTreeNode(tree, delnode);
  }
  if(tree->root){
    // Now, the root may have only one child left, so we check if we can adjust
    // the tree accordingly.
    naShrinkQuadTreeNodeRoot(tree);
  }
}



NA_DEF NABool naIterateQuadTree(NAQuadTreeIterator* iter, const NARect* limit, NABool create){
  const NAQuadTree* tree;
  #ifndef NDEBUG
    if(create && !limit)
      naError("naIterateQuadTree", "create should not be true if there is no limit");
  #endif

  tree = (const NAQuadTree*)naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetQuadTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->curnode)
      {naCrash("naIterateQuadTree", "No current node after setting the current node to the root"); return NA_FALSE;}
    #endif
    // todo: code sanity warning
  }

  // We go to the next segment. When we came to this function with segment -1,
  // we therefore start with the first segment.
  iter->cursegment++;
  
  // Search for a segment which is available.
  while(iter->cursegment < 4){
    NABool considerchild = NA_TRUE;
    if(limit){
      NARect childrect = naMakeRect(iter->curnode->childorigin[iter->cursegment], naMakeSize(iter->curnode->childexponent, iter->curnode->childexponent));
      considerchild = naIsRectUseful(naMakeRectWithRectIntersection(childrect, *limit));
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
        naCreateQuadTreeLeaf(iter, NA_NULL);
        #ifndef NDEBUG
          if(iter->cursegment == -1)
          {naCrash("naIterateQuadTree", "Segment of newly created Leaf is -1"); return NA_FALSE;}
        #endif  
        break;
      }
    }
    iter->cursegment++;
  }
  
  if(iter->cursegment < 4){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(iter->curnode->childexponent == naGetQuadTreeMinLeafExponent(tree)){
      naSetQuadTreeIteratorLeafOrigin(iter, iter->curnode->childorigin[iter->cursegment]);
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetQuadTreeIteratorCurNode(iter, iter->curnode->child[iter->cursegment]);
      // We start looking in the subnode from the start.
      iter->cursegment = -1;
      return naIterateQuadTree(iter, limit, create);
    }
    
  }else{
    // There is no more segment available in this node. Go upwards.
    if(iter->curnode->parentnode){
      #ifndef NDEBUG
        if(iter->curnode->segmentinparent == -1)
          naError("naIterateQuadTreeNode", "Inernal inconsistency detected: Segment in parent should not be -1");
      #endif
      iter->cursegment = iter->curnode->segmentinparent;
      naSetQuadTreeIteratorCurNode(iter, iter->curnode->parentnode);
      return naIterateQuadTree(iter, limit, create);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naSetQuadTreeIteratorCurNode(iter, NA_NULL);
      iter->cursegment = -1;
      iter->flags &= ~NA_QUADTREE_ITERATOR_HAS_ORIGIN;
      return NA_FALSE;
    }
  }
}



// Moves the iterator to the closest parent node containing coord.
NA_HDEF NABool naLocateQuadTreeNode(NAQuadTreeIterator* iter, NAPos origin){
  NARect noderect;
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetQuadTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->curnode)
      {naCrash("naLocateQuadTreeNode", "No current node after setting the current node to the root"); return NA_FALSE;}
    #endif
  }

  noderect = naGetQuadTreeNodeRect(iter->curnode);
  if(naContainsRectiPos(REMOVERectToRecti(noderect), REMOVEPosToPosi(origin))){
  
    // The coord is stored somewhere inside the rectangle of this node
    NAInt segment = naGetQuadTreeNodeSegment(iter->curnode, origin);
    if((iter->curnode->child[segment])){
      // There is a segment available which contains the coord.
      if(iter->curnode->childexponent == naGetQuadTreeMinLeafExponent(tree)){
        // We are at leaf-level. We found it!
        iter->cursegment = segment;
        // Good ending.
        return NA_TRUE;
      }else{
        // Go on searching in the sub-node
        naSetQuadTreeIteratorCurNode(iter, iter->curnode->child[segment]);
        return naLocateQuadTreeNode(iter, origin);
      }
      
    }else{
      // There is no segment where the coord should be. No leaf found.
      iter->cursegment = -1;
      // Bad ending. We are stuck with the closest parent node.
      return NA_FALSE;
    }

  }else{
  
    // The coord is not stored within the rect of this node.
    if(iter->curnode->parentnode){
      // We have a parent. Search there.
      naSetQuadTreeIteratorCurNode(iter, iter->curnode->parentnode);
      return naLocateQuadTreeNode(iter, origin);
    }else{
      // There is no parent
      naSetQuadTreeIteratorCurNode(iter, NA_NULL);
      iter->cursegment = -1;
      // Bad ending. The desired coord lies outside of the whole tree.
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naLocateQuadTreeCoord(NAQuadTreeIterator* iter, NAPos coord){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));

  NAPos neworigin = naGetQuadTreeAlignedCoord(naGetQuadTreeMinLeafExponent(tree), coord);

  naSetQuadTreeIteratorLeafOrigin(iter, neworigin);  
  return naLocateQuadTreeNode(iter, neworigin);
}



NA_DEF NABool naLocateQuadTreeSteps(NAQuadTreeIterator* iter, NAInt stepx, NAInt stepy){
  NAPos neworigin;
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naLocateQuadTreeSteps", "Iterator must be positioned first before moving relative.");
  #endif

  NAInt minleafexponent = naGetQuadTreeMinLeafExponent(tree);
  neworigin = iter->leaforigin;
  neworigin.x += stepx * minleafexponent;
  neworigin.y += stepy * minleafexponent;

  naSetQuadTreeIteratorLeafOrigin(iter, neworigin);
  return naLocateQuadTreeNode(iter, neworigin);
}



NA_DEF void naUpdateQuadTreeCur(NAQuadTreeIterator* iter){
  if(iter->curnode){
    NAQuadTree* tree = naGetPtrMutable(&(iter->tree));
    naUpdateQuadTreeNodeBubbling(tree, iter->curnode, iter->cursegment);
  }
}



NA_DEF void naUpdateQuadTree(NAQuadTree* tree){
  if(tree->root){
    naUpdateQuadTreeNodeCapturing(tree, tree->root);
  }
}








// ////////////////////////////
// Tree
// ////////////////////////////


NA_DEF NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAQuadTreeConfiguration configuration){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naInitQuadTree", "tree is Null-Pointer"); return NA_NULL;}
    if(!configuration.leafallocator)
      naError("naInitQuadTree", "Must have a data allocator");
    if(!configuration.leafdeallocator)
      naError("naInitQuadTree", "Must have a data destructor");
    if(configuration.minleafexponent < 1)
      naError("naInitQuadTree", "leafexponent can not be smaller than 1");
  #endif
  tree->configuration = configuration;
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}



NA_DEF NAQuadTree* naInitQuadTreeCopy(NAQuadTree* newtree, const NAQuadTree* duptree){
  NAQuadTreeIterator newiter;
  NAQuadTreeIterator dupiter;
  newtree = naInitQuadTree(newtree, duptree->configuration);

  newiter = naMakeQuadTreeModifier(newtree);
  dupiter = naMakeQuadTreeAccessor(duptree);
  while(naIterateQuadTree(&dupiter, NA_NULL, NA_FALSE)){
    NAPos duporigin;
    const void* dupdata = naGetQuadTreeCurConst(&dupiter);
    duporigin = naGetQuadTreeCurOrigin(&dupiter);
    naLocateQuadTreeCoord(&newiter, duporigin);
    naCreateQuadTreeLeaf(&newiter, dupdata);
  }
  
  naClearQuadTreeIterator(&dupiter);
  naClearQuadTreeIterator(&newiter);

  return newtree;
}



NA_DEF NAQuadTree* naInitQuadTreeCopyMasked(NAQuadTree* newtree, const NAQuadTree* duptree, const NAQuadTree* masktree){
  NAQuadTreeIterator newiter;
  NAQuadTreeIterator dupiter;
  NAQuadTreeIterator maskiter;
  newtree = naInitQuadTree(newtree, duptree->configuration);

  newiter = naMakeQuadTreeModifier(newtree);
  dupiter = naMakeQuadTreeAccessor(duptree);
  maskiter = naMakeQuadTreeAccessor(masktree);
  while(naIterateQuadTree(&maskiter, NA_NULL, NA_FALSE)){
    const void* dupdata;
    NAPos duporigin = naGetQuadTreeCurOrigin(&maskiter);
    naLocateQuadTreeCoord(&dupiter, duporigin);
    dupdata = naGetQuadTreeCurConst(&dupiter);
    naLocateQuadTreeCoord(&newiter, duporigin);
    naCreateQuadTreeLeaf(&newiter, dupdata);
  }
  
  naClearQuadTreeIterator(&dupiter);
  naClearQuadTreeIterator(&newiter);
  naClearQuadTreeIterator(&maskiter);

  return newtree;
}





NA_DEF NAQuadTree* naInitQuadTreeCopyShifted(NAQuadTree* newtree, const NAQuadTree* duptree, NAPos shift){
  NAInt x1bound;
  NAInt y1bound;
  NAInt x2bound;
  NAInt y2bound;
  NARecti rect0;
  NARecti rect1;
  NARecti rect2;
  NARecti rect3;
  NAQuadTreeIterator newiter;
  NAQuadTreeIterator iter;

  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithShift", "newtree is Null-Pointer"); return NA_NULL;}
    if(!duptree->configuration.datacopier)
      {naCrash("naCopyQuadTreeWithShift", "Data copier callback required for this function to work."); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->configuration);
  if(!duptree->root){return newtree;}

  // Create four rects which denote the rects in the new shifted tree which
  // are aligned to a leaflength.
  NAInt minleafexponent = naGetQuadTreeMinLeafExponent(duptree);
//  NAPosi shiftint = naMakePosiWithIntegerPos(shift);
  NAPosi shiftint = naMakePosiWithPos(shift);
  x1bound = ((shiftint.x % minleafexponent) + minleafexponent ) % minleafexponent;
  y1bound = ((shiftint.y % minleafexponent) + minleafexponent ) % minleafexponent;
  x2bound = naGetQuadTreeMinLeafExponent(duptree) - x1bound;
  y2bound = naGetQuadTreeMinLeafExponent(duptree) - y1bound;
  rect0 = naMakeRectiE(naMakePosi(shiftint.x, shiftint.y), naMakeSizeiE(x2bound, y2bound));
  rect1 = naMakeRectiE(naMakePosi(shiftint.x + x2bound, shiftint.y), naMakeSizeiE(x1bound, y2bound));
  rect2 = naMakeRectiE(naMakePosi(shiftint.x, shiftint.y + y2bound), naMakeSizeiE(x2bound, y1bound));
  rect3 = naMakeRectiE(naMakePosi(shiftint.x + x2bound, shiftint.y + y2bound), naMakeSizeiE(x1bound, y1bound));
  
  newiter = naMakeQuadTreeModifier(newtree);
  iter = naMakeQuadTreeAccessor(duptree);
  
  while(naIterateQuadTree(&iter, NA_NULL, NA_FALSE)){
    NAPos origin;
    NAPos neworigin;
    void* newdata;
    const void* dupchunk = naGetQuadTreeCurConst(&iter);
    origin = naGetQuadTreeCurOrigin(&iter);

    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 4 new leaves.
    if(!naIsRectiEmpty(rect0)){
      neworigin = naMakePos(origin.x + rect0.pos.x, origin.y + rect0.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(x1bound, y1bound), dupchunk, naMakePos(0, 0), REMOVESizeiToSize(rect0.size), shift, naGetQuadTreeMinLeafExponent(duptree));
    }
    if(!naIsRectiEmpty(rect1)){
      neworigin = naMakePos(origin.x + rect1.pos.x, origin.y + rect1.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(0, y1bound), dupchunk, naMakePos(x2bound, 0), REMOVESizeiToSize(rect1.size), shift, naGetQuadTreeMinLeafExponent(duptree));
    }
    if(!naIsRectiEmpty(rect2)){
      neworigin = naMakePos(origin.x + rect2.pos.x, origin.y + rect2.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(x1bound, 0), dupchunk, naMakePos(0, y2bound), REMOVESizeiToSize(rect2.size), shift, naGetQuadTreeMinLeafExponent(duptree));
    }
    if(!naIsRectiEmpty(rect3)){
      neworigin = naMakePos(origin.x + rect3.pos.x, origin.y + rect3.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(0, 0), dupchunk, naMakePos(x2bound, y2bound), REMOVESizeiToSize(rect3.size), shift, naGetQuadTreeMinLeafExponent(duptree));
    }
  }
  naClearQuadTreeIterator(&iter);
  naClearQuadTreeIterator(&newiter);
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateQuadTree(newtree);
  return newtree;
}



NA_DEF void naClearQuadTree(NAQuadTree* tree){
  naEmptyQuadTree(tree);
}




NA_DEF void naEmptyQuadTree(NAQuadTree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("naEmptyQuadTree", "There are still iterators running on this tree. Did you miss a naClearQuadTreeIterator call?");
  #endif
  if(tree->root){naDeallocQuadTreeNode(tree, tree->root);}
  tree->root = NA_NULL;
}



NA_DEF void* naGetQuadTreeRootNodeData(NAQuadTree* tree){
  if(tree->root){return tree->root->nodedata;}
  return NA_NULL;
}



NA_DEF void naSetQuadTreeInRect(NAQuadTree* tree, NARect rect, NAQuadTreeDataSetter datasetter, const void* userdata){
  NAQuadTreeIterator iter;
  NARect chunkrect;
  NAInt minleafexponent = naGetQuadTreeMinLeafExponent(tree);
  chunkrect = naMakeRect(naMakePos(0., 0.), naMakeSize(minleafexponent, minleafexponent));

  iter = naMakeQuadTreeModifier(tree);
  while(naIterateQuadTree(&iter, &rect, NA_TRUE)){
    NARect clamprect;
    void* curchunk = naGetQuadTreeCurMutable(&iter, NA_TRUE);
    chunkrect.pos = naGetQuadTreeCurOrigin(&iter);
    clamprect = naClampRectToRect(rect, chunkrect);
    clamprect.pos.x -= chunkrect.pos.x;
    clamprect.pos.y -= chunkrect.pos.y;
    datasetter(curchunk, clamprect, minleafexponent, userdata);
  }
  naClearQuadTreeIterator(&iter);

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
