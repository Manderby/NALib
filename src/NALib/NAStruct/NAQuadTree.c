
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "../NAQuadTree.h"
#include "../NAMemory.h"



struct NAQuadTreeNode{
  NAInt childlength;
  NAInt segmentinparent;
  NAQuadTreeNode* parentnode;
  void* child[4];
  NAPosi childorigin[4];
  void* nodedata;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};




// ////////////////////////////
// Node
// ////////////////////////////


NA_HIDEF NARecti naGetQuadTreeNodeRect(NAQuadTreeNode* node){
  return naMakeRecti(node->childorigin[0], naMakeSizei(node->childlength * 2, node->childlength * 2));
}



// Creates an inner node.
NA_HDEF NAQuadTreeNode* naAllocQuadTreeNode(NAInt childlength, NAInt segmentinparent, NAQuadTreeNode* parentnode, NAPosi origin, NAQuadTreeNodeAllocator allocator){
  NAQuadTreeNode* node = naAlloc(NAQuadTreeNode);
  node->childlength = childlength;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->child[0] = NA_NULL;
  node->child[1] = NA_NULL;
  node->child[2] = NA_NULL;
  node->child[3] = NA_NULL;
  node->childorigin[0] = naMakePosi(origin.x                    , origin.y);
  node->childorigin[1] = naMakePosi(origin.x + node->childlength, origin.y);
  node->childorigin[2] = naMakePosi(origin.x                    , origin.y + node->childlength);
  node->childorigin[3] = naMakePosi(origin.x + node->childlength, origin.y + node->childlength);
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
NA_HDEF void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  #ifndef NDEBUG
    if(node->itercount)
      naError("naDeallocQuadTreeNode", "Iterators still running on this node. Did you forget a call to NAClearQuadTreeIterator?");
  #endif

  if(node->childlength == tree->leaflength){
    if(node->child[0]){tree->callbacks.leafdeallocator(node->child[0], tree->callbacks.userdata);}
    if(node->child[1]){tree->callbacks.leafdeallocator(node->child[1], tree->callbacks.userdata);}
    if(node->child[2]){tree->callbacks.leafdeallocator(node->child[2], tree->callbacks.userdata);}
    if(node->child[3]){tree->callbacks.leafdeallocator(node->child[3], tree->callbacks.userdata);}
  }else{
    if(node->child[0]){naDeallocQuadTreeNode(tree, node->child[0]);}
    if(node->child[1]){naDeallocQuadTreeNode(tree, node->child[1]);}
    if(node->child[2]){naDeallocQuadTreeNode(tree, node->child[2]);}
    if(node->child[3]){naDeallocQuadTreeNode(tree, node->child[3]);}
  }

  if(node->nodedata && tree->callbacks.nodedeallocator){
    tree->callbacks.nodedeallocator(node->nodedata);
  }
  free(node);
}



// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAQuadTreeNode* naAddQuadTreeNodeChild(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segment){
  #ifndef NDEBUG
    if(parentnode->child[segment])
      naError("naAddQuadTreeNodeChild", "A child already exists at that segment of the parent node.");
  #endif

  NAQuadTreeNode* node = naAllocQuadTreeNode(
          parentnode->childlength / 2,
          segment,
          parentnode,
          parentnode->childorigin[segment],
          tree->callbacks.nodeallocator);
  
  parentnode->child[segment] = node;
  return node;
}



// Returns the segment index [0-3] in which the given pos can be found.
// Warning: The position is expected to be inside this node.
NA_HIDEF NAInt naGetQuadTreeNodeSegment(NAQuadTreeNode* node, NAPosi pos){
  NAInt segment = 0;
  if(pos.x >= node->childorigin[3].x){segment |= 1;}
  if(pos.y >= node->childorigin[3].y){segment |= 2;}
  #ifndef NDEBUG
    if(!naContainsRectiPos(naMakeRecti(node->childorigin[segment], naMakeSizei(node->childlength, node->childlength)), pos))
      naError("naGetQuadTreeNodeSegment", "node does not actually contains pos");
  #endif
  return segment;
}



NA_HDEF NAPosi naGetQuadTreeNodeParentOrigin(NAInt childsize, NAPosi childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  //
  // Implementation note: To be sure that the childorigin stays aligned, we
  // simply take that origin als the alignment origin and work from there.

  NARecti alignrect = naMakeRecti(childorigin, naMakeSizei(childsize * 2, childsize * 2));
  NAInt cycle = naLog2i(childsize) % 4;
  if(cycle & 1){alignrect.pos.x -= childsize;}
  if(cycle & 2){alignrect.pos.y -= childsize;}

  return naMakePosiWithAlignment(childorigin, alignrect);
}



NA_HDEF void naFillQuadTreeNodeChildData(const void* childdata[4], NAQuadTreeNode* node){
  childdata[0] = (node->child[0]) ? (((NAQuadTreeNode*)(node->child[0]))->nodedata) : NA_NULL;
  childdata[1] = (node->child[1]) ? (((NAQuadTreeNode*)(node->child[1]))->nodedata) : NA_NULL;
  childdata[2] = (node->child[2]) ? (((NAQuadTreeNode*)(node->child[2]))->nodedata) : NA_NULL;
  childdata[3] = (node->child[3]) ? (((NAQuadTreeNode*)(node->child[3]))->nodedata) : NA_NULL;
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF void naUpdateQuadTreeNodeBubbling(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segment){
  NABool bubble = NA_TRUE;
  
  // We call the update callbacks depending whether this is a leaf node or
  // an inner node.
  if(parentnode->childlength == tree->leaflength){
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(parentnode->nodedata, segment, (const void* const) parentnode->child);
    }
  }else{
    if(tree->callbacks.childchanged){
      const void* childdata[4];
      naFillQuadTreeNodeChildData(childdata, parentnode);
      bubble = tree->callbacks.childchanged(parentnode->nodedata, segment, childdata);
    }
  }
  
  // Then we propagate the message towards the root.
  if(bubble && parentnode->parentnode){
    #ifndef NDEBUG
      if(parentnode->segmentinparent == -1)
        naError("naUpdateQuadTreeNode", "Inernal inconsistency detected");
    #endif
    naUpdateQuadTreeNodeBubbling(tree, parentnode->parentnode, parentnode->segmentinparent);
  }
}



// Propagates a capturing update event the from the root to the leafes.
// All leaf nodes will be called with the leafchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that everly node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateQuadTreeNodeCapturing(NAQuadTree* tree, NAQuadTreeNode* node){
  NABool bubble = NA_FALSE;
  
  if(node->childlength == tree->leaflength){
    // This node stores leafs
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);
    }
  }else{
    // this node stores subnodes
    if(node->child[0]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[0]);}
    if(node->child[1]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[1]);}
    if(node->child[2]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[2]);}
    if(node->child[3]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[3]);}
    if(bubble && tree->callbacks.childchanged){
      const void* childdata[4];
      naFillQuadTreeNodeChildData(childdata, node);
      bubble = tree->callbacks.childchanged(node->nodedata, -1, childdata);
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
        {naCrash("naRemoveQuadTreeNode", "Inernal inconsistency detected"); return;}
    #endif
    node->parentnode->child[node->segmentinparent] = NA_NULL;
    
    // Test if there are other childs...
    if(node->parentnode->child[0] || node->parentnode->child[1] || node->parentnode->child[2] || node->parentnode->child[3]){
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






// ////////////////////////////
// Tree
// ////////////////////////////


NA_DEF void* naGetQuadTreeRootNodeData(NAQuadTree* tree){
  if(tree->root){return tree->root->nodedata;}
  return NA_NULL;
}



NA_DEF void naSetQuadTreeInRect(NAQuadTree* tree, NARecti rect, NAQuadTreeDataSetter datasetter, const void* userdata){
  NARecti chunkrect;
  chunkrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->leaflength, tree->leaflength));

  NAQuadTreeIterator iter = naMakeQuadTreeModifier(tree);
  while(naIterateQuadTree(&iter, &rect, NA_TRUE)){
    void* curchunk = naGetQuadTreeCurrentMutable(&iter, NA_TRUE);
    chunkrect.pos = naGetQuadTreeCurrentOrigin(&iter);
    NARecti clamprect = naClampRectiToRect(rect, chunkrect);
    clamprect.pos.x -= chunkrect.pos.x;
    clamprect.pos.y -= chunkrect.pos.y;
    datasetter(curchunk, clamprect, tree->leaflength, userdata);
  }
  naClearQuadTreeIterator(&iter);

}



// Allocates and initializes a new node which becomes the parent of the existing
// treeroot. This function replaces the current root with a higher-level root.
NA_HDEF void naGrowQuadTreeRoot(NAQuadTree* tree){

  NAInt childsegment;
  NAQuadTreeNode* parentnode;
  NAPosi parentorigin = naGetQuadTreeNodeParentOrigin(tree->root->childlength * 2, tree->root->childorigin[0]);
  
  parentnode = naAllocQuadTreeNode(
          tree->root->childlength * 2,
          -1,
          NA_NULL,
          parentorigin,
          tree->callbacks.nodeallocator);
  
  childsegment = naGetQuadTreeNodeSegment(parentnode, tree->root->childorigin[0]);
  parentnode->child[childsegment] = tree->root;
  tree->root->parentnode = parentnode;
  tree->root->segmentinparent = childsegment;
  if(tree->callbacks.childchanged){
    const void* childdata[4];
    naFillQuadTreeNodeChildData(childdata, parentnode);
    tree->callbacks.childchanged(parentnode->nodedata, childsegment, childdata);
  }
  tree->root = parentnode;
}



// This methods checks if the root of the tree has only one child. As long
// as there is only one child in the root, the root is transferred to that
// very child to save memory and allow garbage collectors to collect.
NA_HDEF void naShrinkQuadTreeRoot(NAQuadTree* tree){
  while(1){
    if(tree->root->childlength == tree->leaflength){
      // The root node has only leafes as childs.
      return;
    }else{
      // We count how many children there are.
      NAUInt childcount = 0;
      NAUInt lastchildindex = 0;
      NAQuadTreeNode* lastchild;
      if(tree->root->child[0]){childcount++; lastchildindex = 0;}
      if(tree->root->child[1]){childcount++; lastchildindex = 1;}
      if(tree->root->child[2]){childcount++; lastchildindex = 2;}
      if(tree->root->child[3]){childcount++; lastchildindex = 3;}
      #ifndef NDEBUG
        if(!childcount)
          naError("naShrinkQuadTreeRoot", "Root should not be empty.");
      #endif
      // If there are more than 1 child, return.
      if(childcount > 1){return;}
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
  return (iter->curnode) && (iter->cursegment != -1) && (iter->curnode->child[iter->cursegment]) && (iter->curnode->childlength == tree->leaflength);
}



NA_HIDEF void naSetQuadTreeIteratorLeafOrigin(NAQuadTreeIterator* iter, NAPosi origin){
  iter->leaforigin = origin;
  iter->flags |= NA_QUADTREE_ITERATOR_HAS_ORIGIN;
}



NA_HIDEF void naSetQuadTreeIteratorCurrentNode(NAQuadTreeIterator* iter, NAQuadTreeNode* newnode){
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount--;}
  #endif
  iter->curnode = newnode;
  #ifndef NDEBUG
    if(iter->curnode){iter->curnode->itercount++;}
  #endif
}



NA_HIDEF void naCreateQuadTreeLeaf(NAQuadTreeIterator* iter, const void* serialdata){
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
      // We expand the tree at the root and call this function recursively.
      naGrowQuadTreeRoot(tree);
      NARecti noderect = naGetQuadTreeNodeRect(tree->root);
      if(naContainsRectiPos(noderect, iter->leaforigin)){naSetQuadTreeIteratorCurrentNode(iter, tree->root);}
    }else{
      // We create the very first node of this tree.
      NAPosi rootorigin = naGetQuadTreeNodeParentOrigin(tree->leaflength, iter->leaforigin);
      tree->root = naAllocQuadTreeNode(
                                        tree->leaflength,
                                        -1,
                                        NA_NULL,
                                        rootorigin,
                                        tree->callbacks.nodeallocator);
      naSetQuadTreeIteratorCurrentNode(iter, tree->root);
    }
    // We call this function recursively.
    naCreateQuadTreeLeaf(iter, serialdata);
    
  }else{
    // We have a current node which is expected to contain the coord.
    #ifndef NDEBUG
      NARecti noderect = naGetQuadTreeNodeRect(iter->curnode);
      if(!naContainsRectiPos(noderect, iter->leaforigin))
        naError("naCreateQuadTreeLeaf", "Current node does not contain the desired origin.");
    #endif
        
    // There is a current node but no subnode has been identified.
    NAInt segment = naGetQuadTreeNodeSegment(iter->curnode, iter->leaforigin);
    #ifndef NDEBUG
      if(iter->curnode->child[segment])
        naError("naCreateQuadTreeLeaf", "Tree has been modified between positioning this iterator and creating a leaf. Don't do that. Memory leaks and major pointer confusion expected.");
    #endif

    if(iter->curnode->childlength == tree->leaflength){
      // The node can not be divided anymore. Create a leaf.
      if(serialdata){
        iter->curnode->child[segment] = tree->callbacks.deserialize(iter->curnode->childorigin[segment], tree->leaflength, serialdata);
      }else{
        iter->curnode->child[segment] = tree->callbacks.leafallocator(iter->curnode->childorigin[segment], tree->leaflength, tree->callbacks.userdata, NA_NULL);
      }
      iter->cursegment = segment;
      // Good ending. We created the leaf.
    }else{
      // We create an internal subnode and call this function recursively.
      NAQuadTreeNode* newnode = naAddQuadTreeNodeChild(tree, iter->curnode, segment);
      naSetQuadTreeIteratorCurrentNode(iter, newnode);
      naCreateQuadTreeLeaf(iter, serialdata);
    }    
  }
}



NA_HDEF void* naGetQuadTreeCurrentMutableWithSerialData(NAQuadTreeIterator* iter, NABool create, const void* serialdata){
  #ifndef NDEBUG
    if(naIsPtrConst(&(iter->tree)))
      naError("naGetQuadTreeCurrentMutable", "Mutating iterator which is not mutable");
  #endif
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    if(create){
      naCreateQuadTreeLeaf(iter, serialdata);
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
  iter.tree = naMakePtrWithDataMutable(tree, NA_MEMORY_CLEANUP_NONE);
  naInitQuadTreeIterator(&iter);
  return iter;
}



NA_DEF NAQuadTreeIterator naMakeQuadTreeModifier(NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree, NA_MEMORY_CLEANUP_NONE);
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
  naClearPtr(&(iter->tree));
}



NA_DEF void naResetQuadTreeIterator(NAQuadTreeIterator* iter){
  naSetQuadTreeIteratorCurrentNode(iter, NA_NULL);
  iter->cursegment = -1;
  iter->flags &= ~NA_QUADTREE_ITERATOR_HAS_ORIGIN;
}



NA_DEF const void* naGetQuadTreeCurrentConst(NAQuadTreeIterator* iter){
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->curnode->child[iter->cursegment];
  }else{
    return NA_NULL;
  }
}



NA_DEF void* naGetQuadTreeCurrentMutable(NAQuadTreeIterator* iter, NABool create){
  return naGetQuadTreeCurrentMutableWithSerialData(iter, create, NA_NULL);
}



NA_DEF NAPosi naGetQuadTreeCurrentOrigin(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naGetQuadTreeCurrentOrigin", "Iterator has no origin");
  #endif
  return iter->leaforigin;
}



NA_DEF void naRemoveQuadTreeCurrent(NAQuadTreeIterator* iter){
  if(!iter->curnode){return;}
  NAQuadTree* tree = naGetPtrMutable(&(iter->tree));
    
  // First, we destroy the data of the leaf chunk
  tree->callbacks.leafdeallocator(iter->curnode->child[iter->cursegment], tree->callbacks.userdata);
  iter->curnode->child[iter->cursegment] = NA_NULL;

  // Test if there are other childs in the current parent.
  if(iter->curnode->child[0] || iter->curnode->child[1] || iter->curnode->child[2] || iter->curnode->child[3]){
    // If there is at least one more child, we update the node.
    naUpdateQuadTreeNodeBubbling(tree, iter->curnode, -1);
  }else{
    // If there are no more childs, we remove the node.
    NAQuadTreeNode* delnode = iter->curnode;
    naSetQuadTreeIteratorCurrentNode(iter, NA_NULL);
    naRemoveQuadTreeNode(tree, delnode);
  }
  if(tree->root){
    // Now, the root may have only one child left, so we check if we can adjust
    // the tree accordingly.
    naShrinkQuadTreeRoot(tree);
  }
}



NA_DEF NABool naIterateQuadTree(NAQuadTreeIterator* iter, const NARecti* limit, NABool create){
  #ifndef NDEBUG
    if(create && !limit)
      naError("naIterateQuadTree", "create should not be true if there is no limit");
  #endif

  const NAQuadTree* tree = (const NAQuadTree*)naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetQuadTreeIteratorCurrentNode(iter, tree->root);
  }

  // We go to the next segment. When we came to this function with segment -1,
  // we therefore start with the first segment.
  iter->cursegment++;
  
  // Search for a segment which is available.
  while(iter->cursegment < 4){
    NABool considerchild = NA_TRUE;
    if(limit){
      NARecti childrect = naMakeRecti(iter->curnode->childorigin[iter->cursegment], naMakeSizei(iter->curnode->childlength, iter->curnode->childlength));
      considerchild = naIsRectiUseful(naMakeRectiWithRectIntersection(childrect, *limit));
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
        break;
      }
    }
    iter->cursegment++;
  }
  
  if(iter->cursegment < 4){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(iter->curnode->childlength == tree->leaflength){
      naSetQuadTreeIteratorLeafOrigin(iter, iter->curnode->childorigin[iter->cursegment]);
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetQuadTreeIteratorCurrentNode(iter, iter->curnode->child[iter->cursegment]);
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
      naSetQuadTreeIteratorCurrentNode(iter, iter->curnode->parentnode);
      return naIterateQuadTree(iter, limit, create);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naSetQuadTreeIteratorCurrentNode(iter, NA_NULL);
      iter->cursegment = -1;
      iter->flags &= ~NA_QUADTREE_ITERATOR_HAS_ORIGIN;
      return NA_FALSE;
    }
  }
}



// Moves the iterator to the closest parent node containing coord.
NA_HDEF NABool naLocateQuadTreeNode(NAQuadTreeIterator* iter, NAPosi origin){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  if(!tree->root){return NA_FALSE;}
  if(!iter->curnode){
    naSetQuadTreeIteratorCurrentNode(iter, tree->root);
  }

  NARecti noderect = naGetQuadTreeNodeRect(iter->curnode);
  if(naContainsRectiPos(noderect, origin)){
  
    // The coord is stored somewhere inside the rectangle of this node
    NAInt segment = naGetQuadTreeNodeSegment(iter->curnode, origin);
    if(iter->curnode->child[segment]){
      // There is a segment available which contains the coord.
      if(iter->curnode->childlength == tree->leaflength){
        // We are at leaf-level. We found it!
        iter->cursegment = segment;
        // Good ending.
        return NA_TRUE;
      }else{
        // Go on searching in the sub-node
        naSetQuadTreeIteratorCurrentNode(iter, iter->curnode->child[segment]);
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
      naSetQuadTreeIteratorCurrentNode(iter, iter->curnode->parentnode);
      return naLocateQuadTreeNode(iter, origin);
    }else{
      // There is no parent
      naSetQuadTreeIteratorCurrentNode(iter, NA_NULL);
      iter->cursegment = -1;
      // Bad ending. The desired coord lies outside of the whole tree.
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naLocateQuadTreeCoord(NAQuadTreeIterator* iter, NAPosi coord){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));

  NARecti leafalign = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->leaflength, tree->leaflength));
  NAPosi neworigin = naMakePosiWithAlignment(coord, leafalign);
  naSetQuadTreeIteratorLeafOrigin(iter, neworigin);
  
  return naLocateQuadTreeNode(iter, neworigin);
}



NA_DEF NABool naLocateQuadTreeSteps(NAQuadTreeIterator* iter, NAInt stepx, NAInt stepy){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_HAS_ORIGIN))
      naError("naLocateQuadTreeSteps", "Iterator must be positioned first before moving relative.");
  #endif

  NAPosi neworigin = iter->leaforigin;
  neworigin.x += stepx * tree->leaflength;
  neworigin.y += stepy * tree->leaflength;

  return naLocateQuadTreeNode(iter, neworigin);
}



NA_DEF void naUpdateQuadTreeCurrent(NAQuadTreeIterator* iter){
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













// Creates a duplicate at the position of the given oldnode filled with the
// content in contenttree.
// perentnode is the parent in the newtree
NA_HDEF void naCopyQuadTreeNode(NAQuadTree* newtree, NAQuadTreeNode* parentnode, NAInt segment, const NAQuadTreeNode* oldnode, const NAQuadTree* contenttree){

  NAQuadTreeNode* node;

  if(parentnode){
    node = naAddQuadTreeNodeChild(newtree, parentnode, segment);
  }else{
    node = naAllocQuadTreeNode(
          oldnode->childlength,
          -1,
          NA_NULL,
          oldnode->childorigin[0],
          newtree->callbacks.nodeallocator);
    newtree->root = node;
  }
  
  if(node->childlength == newtree->leaflength){
    if(contenttree){
      // Note that in contrast to the else case, we create an empty node if
      // there is no content available.
      NAQuadTreeIterator iter = naMakeQuadTreeAccessor(contenttree);
        naLocateQuadTreeCoord(&iter, node->childorigin[0]);
        if(oldnode->child[0]){node->child[0] = newtree->callbacks.leafallocator(node->childorigin[0], newtree->leaflength, newtree->callbacks.userdata, naGetQuadTreeCurrentConst(&iter));}
        naLocateQuadTreeCoord(&iter, node->childorigin[1]);
        if(oldnode->child[1]){node->child[1] = newtree->callbacks.leafallocator(node->childorigin[1], newtree->leaflength, newtree->callbacks.userdata, naGetQuadTreeCurrentConst(&iter));}
        naLocateQuadTreeCoord(&iter, node->childorigin[2]);
        if(oldnode->child[2]){node->child[2] = newtree->callbacks.leafallocator(node->childorigin[2], newtree->leaflength, newtree->callbacks.userdata, naGetQuadTreeCurrentConst(&iter));}
        naLocateQuadTreeCoord(&iter, node->childorigin[3]);
        if(oldnode->child[3]){node->child[3] = newtree->callbacks.leafallocator(node->childorigin[3], newtree->leaflength, newtree->callbacks.userdata, naGetQuadTreeCurrentConst(&iter));}
      naClearQuadTreeIterator(&iter);
    }else{
      if(oldnode->child[0]){node->child[0] = newtree->callbacks.leafallocator(node->childorigin[0], newtree->leaflength, newtree->callbacks.userdata, oldnode->child[0]);}
      if(oldnode->child[1]){node->child[1] = newtree->callbacks.leafallocator(node->childorigin[1], newtree->leaflength, newtree->callbacks.userdata, oldnode->child[1]);}
      if(oldnode->child[2]){node->child[2] = newtree->callbacks.leafallocator(node->childorigin[2], newtree->leaflength, newtree->callbacks.userdata, oldnode->child[2]);}
      if(oldnode->child[3]){node->child[3] = newtree->callbacks.leafallocator(node->childorigin[3], newtree->leaflength, newtree->callbacks.userdata, oldnode->child[3]);}
    }
    if(newtree->callbacks.leafchanged){newtree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);}
  }else{
    const void* childdata[4];
    if(oldnode->child[0]){naCopyQuadTreeNode(newtree, node, 0, oldnode->child[0], contenttree);}
    if(oldnode->child[1]){naCopyQuadTreeNode(newtree, node, 1, oldnode->child[1], contenttree);}
    if(oldnode->child[2]){naCopyQuadTreeNode(newtree, node, 2, oldnode->child[2], contenttree);}
    if(oldnode->child[3]){naCopyQuadTreeNode(newtree, node, 3, oldnode->child[3], contenttree);}
    naFillQuadTreeNodeChildData(childdata, node);
    if(newtree->callbacks.childchanged){newtree->callbacks.childchanged(node->nodedata, -1, childdata);}
  }

}







NA_DEF NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAInt leaflength, NAQuadTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naInitQuadTree", "tree is Null-Pointer"); return NA_NULL;}
    if(!callbacks.leafallocator)
      naError("naInitQuadTree", "Must have a data allocator");
    if(!callbacks.leafdeallocator)
      naError("naInitQuadTree", "Must have a data destructor");
    if(leaflength < 1)
      naError("naInitQuadTree", "leaflength can not be smaller than 1");
  #endif
  tree->leaflength = leaflength;
  tree->callbacks = callbacks;
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}




NA_DEF NAQuadTree* naInitQuadTreeCopy(NAQuadTree* newtree, const NAQuadTree* duptree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTree", "newtree is Null-Pointer"); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->leaflength, duptree->callbacks);
  if(duptree->root){
    // Copy the root node
    // consequently, all other nodes and all leafs will be created.
    naCopyQuadTreeNode(newtree, NA_NULL, -1, duptree->root, NA_NULL);
  }
  return newtree;
}



NA_DEF NAQuadTree* naInitQuadTreeCopyMasked(NAQuadTree* newtree, const NAQuadTree* duptree, const NAQuadTree* masktree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithMaskTree", "newtree is Null-Pointer"); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->leaflength, duptree->callbacks);
  if(masktree->root){
    // Copy the root node
    // consequently, all other nodes and all leafs will be created.
    naCopyQuadTreeNode(newtree, NA_NULL, -1, masktree->root, duptree);
  }
  return newtree;
}



NA_DEF NAQuadTree* naInitQuadTreeCopyShifted(NAQuadTree* newtree, const NAQuadTree* duptree, NASizei shift){
  NAInt x1bound;
  NAInt y1bound;
  NAInt x2bound;
  NAInt y2bound;
  NARecti rect0;
  NARecti rect1;
  NARecti rect2;
  NARecti rect3;

  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithShift", "newtree is Null-Pointer"); return NA_NULL;}
    if(!duptree->callbacks.datacopier)
      {naCrash("naCopyQuadTreeWithShift", "Data copier callback required."); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->leaflength, duptree->callbacks);
  if(!duptree->root){return newtree;}

  x1bound = ((shift.width % duptree->leaflength) + duptree->leaflength ) % duptree->leaflength;
  y1bound = ((shift.height % duptree->leaflength) + duptree->leaflength ) % duptree->leaflength;
  x2bound = duptree->leaflength - x1bound;
  y2bound = duptree->leaflength - y1bound;
  rect0 = naMakeRectiE(naMakePosi(shift.width, shift.height), naMakeSizeiE(x2bound, y2bound));
  rect1 = naMakeRectiE(naMakePosi(shift.width + x2bound, shift.height), naMakeSizeiE(x1bound, y2bound));
  rect2 = naMakeRectiE(naMakePosi(shift.width, shift.height + y2bound), naMakeSizeiE(x2bound, y1bound));
  rect3 = naMakeRectiE(naMakePosi(shift.width + x2bound, shift.height + y2bound), naMakeSizeiE(x1bound, y1bound));
  
  NAQuadTreeIterator newiter = naMakeQuadTreeModifier(newtree);
  NAQuadTreeIterator iter = naMakeQuadTreeAccessor(duptree);
  
  while(naIterateQuadTree(&iter, NA_NULL, NA_FALSE)){
    const void* dupchunk = naGetQuadTreeCurrentConst(&iter);
    NAPosi origin = naGetQuadTreeCurrentOrigin(&iter);

    NAPosi neworigin;
    void* newdata;
    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 4 new leaves.
    if(!naIsRectiEmpty(rect0)){
      neworigin = naMakePosi(origin.x + rect0.pos.x, origin.y + rect0.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurrentMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakePosi(x1bound, y1bound), dupchunk, naMakePosi(0, 0), rect0.size, duptree->leaflength);
    }
    if(!naIsRectiEmpty(rect1)){
      neworigin = naMakePosi(origin.x + rect1.pos.x, origin.y + rect1.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurrentMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakePosi(0, y1bound), dupchunk, naMakePosi(x2bound, 0), rect1.size, duptree->leaflength);
    }
    if(!naIsRectiEmpty(rect2)){
      neworigin = naMakePosi(origin.x + rect2.pos.x, origin.y + rect2.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurrentMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakePosi(x1bound, 0), dupchunk, naMakePosi(0, y2bound), rect2.size, duptree->leaflength);
    }
    if(!naIsRectiEmpty(rect3)){
      neworigin = naMakePosi(origin.x + rect3.pos.x, origin.y + rect3.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurrentMutable(&newiter, NA_TRUE);
      duptree->callbacks.datacopier(newdata, naMakePosi(0, 0), dupchunk, naMakePosi(x2bound, y2bound), rect3.size, duptree->leaflength);
    }
  }
  naClearQuadTreeIterator(&iter);
  naClearQuadTreeIterator(&newiter);
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateQuadTree(newtree);
  return newtree;
}



NA_DEF NAQuadTree* naInitQuadTreeWithDeserialization(NAQuadTree* tree, const void* buf, NAQuadTreeCallbacks callbacks){
  const NAByte* dataptr;
  uint64 datasize;
  uint64 leaflength;
  NAQuadTreeNode* node;
  NAQuadTreeIterator iter;
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naCopyQuadTreeWithShift", "tree is Null-Pointer"); return NA_NULL;}
    if(!(callbacks.deserialize)){
      naCrash("naInitQuadTreeWithDeserialization", "Callbacks required for deserialization");
      return NA_NULL;
    }
  #endif
  iter = naMakeQuadTreeModifier(tree);
  dataptr = buf;
  datasize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  leaflength = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  tree = naInitQuadTree(tree, (NAInt)leaflength, callbacks);
  datasize -= 2 * sizeof(uint64);
  while(datasize){
    int64 posx, posy;
    uint64 bytesread;
    posx = *((const int64*)dataptr); dataptr += sizeof(int64);
    posy = *((const int64*)dataptr); dataptr += sizeof(int64);
    datasize -= 2 * sizeof(int64);
    // Create the leaf temporarily but delete it immediately and recreate it
    // as a deserialization.
    
    // todo.
    
    naLocateQuadTreeCoord(&iter, naMakePosi((NAInt)posx, (NAInt)posy));
    node = naGetQuadTreeCurrentMutableWithSerialData(&iter, NA_TRUE, dataptr);
//    node = naLocateQuadTreeLeafParent(tree, naMakePosi((NAInt)posx, (NAInt)posy), NA_TRUE, dataptr, NA_NULL, &leafsegment);
    bytesread = callbacks.serialize(NA_NULL, node->child[iter.cursegment], tree->leaflength);
    dataptr += bytesread;
    datasize -= bytesread;
  }
  naUpdateQuadTree(tree);
  naClearQuadTreeIterator(&iter);
  return tree;
}



NA_DEF void naClearQuadTree(NAQuadTree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("naClearQuadTree", "There are still iterators running on this tree. Did you miss a naClearQuadTreeIterator call?");
  #endif
  if(tree->root){naDeallocQuadTreeNode(tree, tree->root);}
}



NA_DEF void naSerializeQuadTree(const NAQuadTree* tree, void* buf, uint64* bytesize){
  #ifndef NDEBUG
    if(!(tree->callbacks.serialize))
      {naCrash("naSerializeQuadTree", "Callbacks required for serialization"); return;}
    if(!bytesize){
      naCrash("naSerializeQuadTree", "bytesize required to read/store byte size");
      return;
    }
    if(buf && (*bytesize == 0))
      naError("naSerializeQuadTree", "bytesize is zero");
  #endif

  if(!buf){
    *bytesize = sizeof(uint64) * 2; // datasize and leaflength
    NAQuadTreeIterator iter = naMakeQuadTreeAccessor(tree);
    while(naIterateQuadTree(&iter, NA_NULL, NA_FALSE)){
      const void* curleaf = naGetQuadTreeCurrentConst(&iter);
      *bytesize += sizeof(int64) * 2; // the origin.x and origin.y
      *bytesize += tree->callbacks.serialize(NA_NULL, curleaf, tree->leaflength);
    }
    naClearQuadTreeIterator(&iter);
  }else{
    NAByte* dataptr = buf;
    *((uint64*)dataptr) = *bytesize; dataptr += sizeof(uint64);
    *((uint64*)dataptr) = (uint64)tree->leaflength; dataptr += sizeof(uint64);

    NAQuadTreeIterator iter = naMakeQuadTreeAccessor(tree);
    while(naIterateQuadTree(&iter, NA_NULL, NA_FALSE)){
      const void* curleaf = naGetQuadTreeCurrentConst(&iter);
      NAPosi origin = naGetQuadTreeCurrentOrigin(&iter);
      *((int64*)dataptr) = origin.x; dataptr += sizeof(int64);
      *((int64*)dataptr) = origin.y; dataptr += sizeof(int64);
      dataptr += tree->callbacks.serialize(dataptr, curleaf, tree->leaflength);
    }
    naClearQuadTreeIterator(&iter);
  }

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
