
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../NAQuadTree.h"
#include "../NAMemory.h"



struct NAQuadTreeNode{
  int16 childexponent;
  int16 segmentinparent;
  NAQuadTreeNode* parentnode;
  uint16 leafmask;
  void* child[4];
  NAPos origin;
  void* nodedata;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};



NA_HAPI void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node);
NA_HAPI void naEnsureQuadTreeInvariant(NAQuadTreeIterator* iter);


#
NA_HIDEF int16 naGetQuadTreeBaseLeafExponent(const NAQuadTree* tree){
  return tree->configuration.baseleafexponent;
}


#
NA_HIDEF NASize naGetQuadTreeSizeWithExponent(int16 exponent){
  double length = naMakeDoubleWithExponent((int32)exponent);
  return naMakeSize(length, length);
}


#
NA_HDEF NAPos naGetQuadTreeNodeChildOrigin(NAQuadTreeNode* parentnode, int16 childsegment){
  NAPos childorigin = parentnode->origin;
  NASize childsize = naGetQuadTreeSizeWithExponent(parentnode->childexponent);
  if(childsegment & 1){childorigin.x += childsize.width;}
  if(childsegment & 2){childorigin.y += childsize.height;}
  return childorigin;
}


#
NA_DEF NARect naGetQuadTreeAlignedRect(int16 leafexponent, NAPos coord){
  NASize leafsize = naGetQuadTreeSizeWithExponent(leafexponent);
  NARect leafalign = naMakeRect(naMakePos(0, 0), leafsize);
  NARect retrect = naMakeRect(naMakePosWithAlignment(coord, leafalign), leafsize);
  return retrect;
}



// ////////////////////////////
// Node
// ////////////////////////////



#
NA_HIDEF NARect naGetQuadTreeNodeRect(NAQuadTreeNode* node){
  return naMakeRect(node->origin, naGetQuadTreeSizeWithExponent(node->childexponent + 1));
}


#
NA_HDEF void naFillQuadTreeNodeChildData(const void* childdata[4], NAQuadTreeNode* node){
  childdata[0] = (node->leafmask & 0x01) ? node->child[0] : (((NAQuadTreeNode*)(node->child[0]))->nodedata);
  childdata[1] = (node->leafmask & 0x02) ? node->child[1] : (((NAQuadTreeNode*)(node->child[1]))->nodedata);
  childdata[2] = (node->leafmask & 0x04) ? node->child[2] : (((NAQuadTreeNode*)(node->child[2]))->nodedata);
  childdata[3] = (node->leafmask & 0x08) ? node->child[3] : (((NAQuadTreeNode*)(node->child[3]))->nodedata);
}


#
// Creates an inner node.
NA_HDEF NAQuadTreeNode* naAllocQuadTreeNode(int16 childexponent, int16 segmentinparent, NAQuadTreeNode* parentnode, NAPos origin, NAQuadTree* tree){
  NAQuadTreeNode* node = naAlloc(NAQuadTreeNode);
  node->childexponent = childexponent;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->leafmask = 0x0f;
  node->child[0] = NA_NULL;
  node->child[1] = NA_NULL;
  node->child[2] = NA_NULL;
  node->child[3] = NA_NULL;
  node->origin = origin;
  if(tree->configuration.nodeallocator){
    NASize childsize = naGetQuadTreeSizeWithExponent(node->childexponent);
    node->nodedata = tree->configuration.nodeallocator(origin, childsize);
  }else{
    node->nodedata = NA_NULL;
  }
  #ifndef NDEBUG
    node->itercount = 0;
  #endif
  return node;
}


#
NA_HIDEF void naDeallocQuadTreeNodeChild(NAQuadTree* tree, NAQuadTreeNode* node, int16 segment){
  if(node->leafmask & (1 << segment)){
    if(node->child[segment]){
      tree->configuration.leafdeallocator(node->child[segment], tree->configuration.userdata);
    }
  }else{
    naDeallocQuadTreeNode(tree, node->child[segment]);
  }
}


#
// Deallocates one node. Deallocates all subnodes and leafes in the process.
NA_HDEF void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  #ifndef NDEBUG
    if(!node)
      naError("naDeallocQuadTreeNode", "Iterator is null");
    if(node->itercount)
      naError("naDeallocQuadTreeNode", "Iterators still running on this node. Did you forget a call to NAClearQuadTreeIterator?");
  #endif

  naDeallocQuadTreeNodeChild(tree, node, 0);
  naDeallocQuadTreeNodeChild(tree, node, 1);
  naDeallocQuadTreeNodeChild(tree, node, 2);
  naDeallocQuadTreeNodeChild(tree, node, 3);
  
  if(node->nodedata && tree->configuration.nodedeallocator){
    tree->configuration.nodedeallocator(node->nodedata);
  }
  naFree(node);
}


#
// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAQuadTreeNode* naAddQuadTreeNodeChild(NAQuadTree* tree, NAQuadTreeNode* parentnode, int16 segmentinparent){
  NAQuadTreeNode* node;
  #ifndef NDEBUG
    if(parentnode->child[segmentinparent])
      naError("naAddQuadTreeNodeChild", "A child already exists at that segment of the parent node.");
  #endif

  NAPos origin = naGetQuadTreeNodeChildOrigin(parentnode, segmentinparent);
  node = naAllocQuadTreeNode(parentnode->childexponent - 1, segmentinparent, parentnode, origin, tree);
  parentnode->child[segmentinparent] = node;
  parentnode->leafmask &= ~(1 << segmentinparent);
  return node;
}


#
// Returns the segment index [0-3] in which the given pos can be found.
// Warning: The position is expected to be inside this node.
NA_HIDEF int16 naGetQuadTreeNodeSegment(NAQuadTreeNode* parentnode, NAPos pos){
  int16 segment = 0;
  NASize childsize = naGetQuadTreeSizeWithExponent(parentnode->childexponent);
  if(pos.x >= parentnode->origin.x + childsize.width) {segment |= 1;}
  if(pos.y >= parentnode->origin.y + childsize.height){segment |= 2;}
  #ifndef NDEBUG
    if(!naContainsRectPosE(naMakeRect(parentnode->origin, naGetQuadTreeSizeWithExponent(parentnode->childexponent + 1)), pos))
      naError("naGetQuadTreeNodeSegment", "parentnode does not actually contain pos");
  #endif
  return segment;
}


#
NA_HDEF NAPos naGetQuadTreeNodeParentOrigin(int16 childexponent, NAPos childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  
  NAPos parentorigin = childorigin;
  NASize childsize = naGetQuadTreeSizeWithExponent(childexponent);
  int16 cycle = ((childexponent % 4) + 4 ) % 4;
  if(cycle & 1){parentorigin.x -= childsize.width;}
  if(cycle & 2){parentorigin.y -= childsize.height;}
  #ifndef NDEBUG
    if(!naContainsRectPosE(naMakeRect(parentorigin, naGetQuadTreeSizeWithExponent(childexponent + 1)), childorigin))
      naError("naGetQuadTreeNodeParentOrigin", "New parent rect does not contain pos");
  #endif
  return parentorigin;
}


#
// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular
// node.
NA_HDEF void naUpdateQuadTreeNodeBubbling(NAQuadTree* tree, NAQuadTreeNode* parentnode, int16 segment){
  NABool bubble = NA_TRUE;
  #ifndef NDEBUG
    if(parentnode == NA_NULL)
      naError("naUpdateQuadTreeNodeBubbling", "Parent is null.");
  #endif
  
  NASize childsize = naGetQuadTreeSizeWithExponent(parentnode->childexponent);
  
  // We call the update callback.
  if(tree->configuration.childchanged){
    const void* childdata[4];
    naFillQuadTreeNodeChildData(childdata, parentnode);
    bubble = tree->configuration.childchanged(parentnode->nodedata, segment, childdata, parentnode->leafmask, childsize);
  }
  
  // Then we propagate the message towards the root if requested.
  if(bubble && parentnode->parentnode){
    #ifndef NDEBUG
      if(parentnode->segmentinparent == -1)
        naError("naUpdateQuadTreeNode", "Internal inconsistency detected");
    #endif
    naUpdateQuadTreeNodeBubbling(tree, parentnode->parentnode, parentnode->segmentinparent);
  }
}



#
// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the childchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateQuadTreeNodeCapturing(NAQuadTree* tree, NAQuadTreeNode* node){
  NABool bubble = NA_FALSE;
  
  NASize childsize = naGetQuadTreeSizeWithExponent(node->childexponent);

  // this node stores subnodes
  if(!(node->leafmask & 0x01) && node->child[0]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[0]);}
  if(!(node->leafmask & 0x02) && node->child[1]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[1]);}
  if(!(node->leafmask & 0x04) && node->child[2]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[2]);}
  if(!(node->leafmask & 0x08) && node->child[3]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[3]);}
  if(tree->configuration.childchanged){
    const void* childdata[4];
    naFillQuadTreeNodeChildData(childdata, node);
    bubble |= tree->configuration.childchanged(node->nodedata, -1, childdata, node->leafmask, childsize);
  }
  
  return bubble;
}


#
NA_HIDEF void naSetQuadTreeIteratorCurNode(NAQuadTreeIterator* iter, NAQuadTreeNode* newnode){
  #ifndef NDEBUG
    if(iter->node){iter->node->itercount--;}
  #endif
  iter->node = newnode;
  #ifndef NDEBUG
    if(iter->node){iter->node->itercount++;}
  #endif
}



#
// Expects the node to be the node to remove.
NA_HDEF void naRemoveQuadTreeNode(NAQuadTreeIterator* iter, NAQuadTreeNode* replacenode){
  NAQuadTreeNode* node = iter->node;
  NAQuadTree* tree = naGetPtrMutable(&(iter->tree));
  
  // As the current node will be erased, we adjust the iterator to point
  // at the parent.
  naSetQuadTreeIteratorCurNode(iter, node->parentnode);
  iter->childsegment = node->segmentinparent;

  if(node->parentnode){
    node->parentnode->child[node->segmentinparent] = replacenode;
    if(replacenode){
      replacenode->parentnode = node->parentnode;
      replacenode->segmentinparent = node->segmentinparent;
    }else{
      // There is no replace node. It is Null and we set it as a leaf.
      node->parentnode->leafmask |= (1 << node->segmentinparent);
    }
    // Ensure the invariant for the parent.
    naEnsureQuadTreeInvariant(iter);
  }else{
    // This was the root.
    if(replacenode){
      replacenode->parentnode = NA_NULL;
      replacenode->segmentinparent = -1;
    }
    tree->root = replacenode;
  }
  
  // Finally, we deallocate the node.
  naDeallocQuadTreeNode(tree, node);
}







// ////////////////////////////
// Iterator
// ////////////////////////////


// Debugging flags
#define NA_QUADTREE_ITERATOR_MUTATOR         0x01
#define NA_QUADTREE_ITERATOR_MODIFIER        0x02



#
// Checks whether the iterator is positioned at a leaf and it exists.
NA_HIDEF NABool naIsQuadTreeIteratorAtLeaf(NAQuadTreeIterator* iter){
  return (iter->node) && (iter->childsegment != -1) && (iter->node->child[iter->childsegment]) && (iter->node->leafmask & (1 << iter->childsegment));
}



#
// Creates a node which contains both the existing child as well as the newpos.
// Attaches the node into the tree.
NA_HIDEF NAQuadTreeNode* naCreateQuadTreeParentNode(NAQuadTreeNode* existingchildnode, NAPos newpos, NAQuadTree* tree){
  int16 parentchildexponent = existingchildnode->childexponent;
  NAPos parentorigin = existingchildnode->origin;
  while(1){
    parentchildexponent++;
    parentorigin = naGetQuadTreeNodeParentOrigin(parentchildexponent, parentorigin);
    NASize parentsize = naGetQuadTreeSizeWithExponent(parentchildexponent + 1);
    if(naContainsRectPosE(naMakeRect(parentorigin, parentsize), newpos)){break;}
  }
  NAQuadTreeNode* newparent = naAllocQuadTreeNode(parentchildexponent, existingchildnode->segmentinparent, existingchildnode->parentnode, parentorigin, tree);
  existingchildnode->segmentinparent = naGetQuadTreeNodeSegment(newparent, existingchildnode->origin);
  existingchildnode->parentnode = newparent;
  newparent->child[existingchildnode->segmentinparent] = existingchildnode;
  newparent->leafmask &= ~(1 << existingchildnode->segmentinparent);
  return newparent;
}


#
// If the iterator is at no node, there either is no root or the root does not
// contain the desired coordinate.
NA_HDEF void naCreateQuadTreeLeaf(NAQuadTreeIterator* iter, const void* data){
  NAQuadTree* tree;

  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_MODIFIER))
      naError("naCreateQuadTreeLeaf", "iterator must be modifier");
  #endif

  tree = (NAQuadTree*)naGetPtrMutable(&(iter->tree));
  int16 baseleafexponent = naGetQuadTreeBaseLeafExponent(tree);

  if(!iter->node){
    // The tree root does not contain the coord or does not exist at all.
    if(tree->root){
      // We expand the tree at the root.
      tree->root = naCreateQuadTreeParentNode(tree->root, iter->pos, tree);
    }else{
      // We create the very first node of this tree.
      NAPos leaforigin = naGetQuadTreeAlignedRect(baseleafexponent, iter->pos).pos;
      NAPos rootorigin = naGetQuadTreeNodeParentOrigin(baseleafexponent, leaforigin);
      tree->root = naAllocQuadTreeNode(baseleafexponent, -1, NA_NULL, rootorigin, tree);
    }
    // We call this function recursively.
    naSetQuadTreeIteratorCurNode(iter, tree->root);
    naCreateQuadTreeLeaf(iter, data);
    
  }else{
    int16 segment;

    // We have a current node which is expected to contain the coord.
    #ifndef NDEBUG
      NARect noderect = naGetQuadTreeNodeRect(iter->node);
      if(!naContainsRectPosE(noderect, iter->pos))
        naError("naCreateQuadTreeLeaf", "Cur node does not contain the desired pos.");
    #endif
        
    segment = naGetQuadTreeNodeSegment(iter->node, iter->pos);
    if(iter->node->child[segment]){
      #ifndef NDEBUG
        if(iter->node->leafmask & (1 << segment))
          naError("naCreateQuadTreeLeaf", "Leaf already exists.");
      #endif
      // There already is a segment, but it holds a node which is too small to
      // contain the desired origin. Create a common parent of the two and
      // reattach them.
      NAQuadTreeNode* newparent = naCreateQuadTreeParentNode(iter->node->child[segment], iter->pos, tree);
      iter->node->child[segment] = newparent;
      // We call this function recursively.
      naSetQuadTreeIteratorCurNode(iter, newparent);
      naCreateQuadTreeLeaf(iter, data);
    }else{
      // There is no segment.
      if(iter->node->childexponent <= baseleafexponent){
        // The node childexponent is small enough to contain a leaf.
        NARect leafrect = naMakeRect(naGetQuadTreeNodeChildOrigin(iter->node, segment), naGetQuadTreeSizeWithExponent(iter->node->childexponent));
        iter->node->child[segment] = tree->configuration.leafallocator(leafrect, tree->configuration.userdata, data);
        iter->childsegment = segment;
        // Good ending. We created the leaf.
      }else{
        // The node has a too big childexponent. We need to create another
        // internal subnode and call this function recursively.
        NAPos leaforigin = naGetQuadTreeAlignedRect(baseleafexponent, iter->pos).pos;
        NAQuadTreeNode* newnode = naAllocQuadTreeNode(baseleafexponent, segment, iter->node, leaforigin, tree);
        iter->node->child[segment] = newnode;
        iter->node->leafmask &= ~(1 << segment);
        naSetQuadTreeIteratorCurNode(iter, newnode);
        naCreateQuadTreeLeaf(iter, data);
      }
    }
  }
}


#
NA_HIDEF void naInitQuadTreeIterator(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    NAQuadTree* mutabletree = (NAQuadTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount++;
  #endif
  iter->node = NA_NULL;
  iter->childsegment = -1;
  iter->pos = naMakePos(0., 0.);
  #ifndef NDEBUG
    iter->flags = 0;
  #endif
}


#
NA_DEF NAQuadTreeIterator naMakeQuadTreeAccessor(const NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitQuadTreeIterator(&iter);
  return iter;
}


#
NA_DEF NAQuadTreeIterator naMakeQuadTreeMutator(const NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitQuadTreeIterator(&iter);
  #ifndef NDEBUG
    iter.flags |= NA_QUADTREE_ITERATOR_MUTATOR;
  #endif
  return iter;
}


#
NA_DEF NAQuadTreeIterator naMakeQuadTreeModifier(NAQuadTree* tree){
  NAQuadTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitQuadTreeIterator(&iter);
  #ifndef NDEBUG
    iter.flags |= NA_QUADTREE_ITERATOR_MUTATOR;
    iter.flags |= NA_QUADTREE_ITERATOR_MODIFIER;
  #endif
  return iter;
}


#
NA_DEF void naClearQuadTreeIterator(NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    NAQuadTree* mutabletree = (NAQuadTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    if(iter->node){iter->node->itercount--;}
  #endif
  naCleanupPtr(&(iter->tree), NA_NULL);
}


#
NA_DEF void naResetQuadTreeIterator(NAQuadTreeIterator* iter){
  naSetQuadTreeIteratorCurNode(iter, NA_NULL);
  iter->childsegment = -1;
  iter->pos = naMakePos(0., 0.);
}


#
NA_DEF const void* naGetQuadTreeCurConst(NAQuadTreeIterator* iter){
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->node->child[iter->childsegment];
  }else{
    return NA_NULL;
  }
}


#
NA_DEF void* naGetQuadTreeCurMutable(NAQuadTreeIterator* iter, NABool create){
  #ifndef NDEBUG
    if(!(iter->flags & NA_QUADTREE_ITERATOR_MUTATOR))
      naError("naGetQuadTreeCurMutable", "Trying to mutate element with iterator which was created non-mutable");
    if(create && !(iter->flags & NA_QUADTREE_ITERATOR_MODIFIER))
      naError("naGetQuadTreeCurMutable", "iterator must be modifier when create is true.");
  #endif
  if(naIsQuadTreeIteratorAtLeaf(iter)){
    return iter->node->child[iter->childsegment];
  }else{
    if(create){
      naCreateQuadTreeLeaf(iter, NA_NULL);
      return iter->node->child[iter->childsegment];
    }else{
      return NA_NULL;
    }
  }
}


#
NA_DEF NAPos naGetQuadTreeCurOrigin(const NAQuadTreeIterator* iter){
  #ifndef NDEBUG
    if(!iter->node)
      naError("naGetQuadTreeCurOrigin", "Iterator has no valid position");
    if(iter->childsegment == -1)
      naError("naGetQuadTreeCurOrigin", "Iterator is not positioned on a leaf");
  #endif
  return naGetQuadTreeNodeChildOrigin(iter->node, iter->childsegment);
}


#
NA_DEF NARect naGetQuadTreeCurRect(const NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!iter->node)
      naError("naGetQuadTreeCurOrigin", "Iterator has no valid position");
    if(iter->childsegment == -1)
      naError("naGetQuadTreeCurOrigin", "Iterator is not positioned on a leaf");
  #endif
  return naMakeRect(naGetQuadTreeNodeChildOrigin(iter->node, iter->childsegment), naGetQuadTreeSizeWithExponent(naGetQuadTreeBaseLeafExponent(tree)));
}


#
NA_DEF NARecti naGetQuadTreeCurRecti(const NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  #ifndef NDEBUG
    if(!iter->node)
      naError("naGetQuadTreeCurOrigin", "Iterator has no valid position");
    if(iter->childsegment == -1)
      naError("naGetQuadTreeCurOrigin", "Iterator is not positioned on a leaf");
    if(naGetQuadTreeBaseLeafExponent(tree) < 0)
      naError("naGetQuadTreeCurRecti", "Can not return valid integer rect for trees with exponents < 0");
  #endif
  // todo
  NAPos pos = naGetQuadTreeNodeChildOrigin(iter->node, iter->childsegment);
  NASize size = naGetQuadTreeSizeWithExponent(naGetQuadTreeBaseLeafExponent(tree));
  return naMakeRectiSE((NAInt)naRound(pos.x), (NAInt)naRound(pos.y), (NAInt)naRound(size.width), (NAInt)naRound(size.height));
}


#
NA_HDEF void naEnsureQuadTreeInvariant(NAQuadTreeIterator* iter){
  NAInt childcount = 0;
  NAInt lastsegment = 0;  // Use 0 so that child[lastsegment] is valid further
                          // below and stores a NULL pointer.
  if((iter->node->child[0])){childcount++; lastsegment = 0;}
  if((iter->node->child[1])){childcount++; lastsegment = 1;}
  if((iter->node->child[2])){childcount++; lastsegment = 2;}
  if((iter->node->child[3])){childcount++; lastsegment = 3;}

  // We remove the node, if
  // - the node has no childs
  // - the node has only 1 child which is not a leaf
  // In all other cases, this node fulfills the invariant.
  if((childcount == 0) || ((childcount == 1) && !(iter->node->leafmask & (1 << lastsegment)))){
    naRemoveQuadTreeNode(iter, iter->node->child[lastsegment]);
  }
}


#
NA_DEF void naRemoveQuadTreeCur(NAQuadTreeIterator* iter){
  NAQuadTree* tree;
  #ifndef NDEBUG
    if(!iter->node)
      naError("naRemoveQuadTreeCur", "iterator is at no specific node in the tree");
    if(iter->childsegment == -1)
      naError("naRemoveQuadTreeCur", "iterator is at no specific leaf in the tree");
    if(!(iter->node->leafmask & (1 << iter->childsegment)))
      naError("naRemoveQuadTreeCur", "cur is not a leaf");
  #endif
  tree = naGetPtrMutable(&(iter->tree));
    
  // First, we destroy the data of the leaf chunk
  tree->configuration.leafdeallocator(iter->node->child[iter->childsegment], tree->configuration.userdata);
  iter->node->child[iter->childsegment] = NA_NULL;
  iter->node->leafmask |= (1 << iter->childsegment);

  naEnsureQuadTreeInvariant(iter);
}


#
// Moves the iterator to the closest parent node containing coord.
// Phase 2: Capture.
NA_HDEF NABool naLocateQuadTreeNodeCapture(NAQuadTreeIterator* iter){
  NABool found = NA_FALSE;
  while(iter->node){
    // The coord is stored somewhere inside the rectangle of this node
    int16 segment = naGetQuadTreeNodeSegment(iter->node, iter->pos);
    if((iter->node->child[segment])){
      // There is a segment available which contains the coord.
      if(iter->node->leafmask & (1 << segment)){
        // We found a leaf!
        iter->childsegment = segment;
        // Good ending.
        found = NA_TRUE;
        break;
      }else{
        // We test the child node, if it contains the desired origin.
        NARect noderect = naGetQuadTreeNodeRect(iter->node->child[segment]);
        if(naContainsRectPosE(noderect, iter->pos)){
          // Go on searching in the sub-node
          naSetQuadTreeIteratorCurNode(iter, iter->node->child[segment]);
        }else{
          // The child node does not contain the desired coordinate. There
          // is a node missing in between.
          break;
        }
      }
      
    }else{
      // There is no segment where the coord should be. No leaf found.
      // Bad ending. We are stuck with the closest parent node.
      break;
    }
  }
  return found;
}


#
// Moves the iterator to the closest parent node containing coord.
// Phase 1: Bubbling.
NA_HDEF NABool naLocateQuadTreeNodeBubble(NAQuadTreeIterator* iter){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));
  if(!iter->node){
    naSetQuadTreeIteratorCurNode(iter, tree->root);
  }

  iter->childsegment = -1;
  while(iter->node){
    NARect noderect = naGetQuadTreeNodeRect(iter->node);
    if(naContainsRectPosE(noderect, iter->pos)){break;}
    naSetQuadTreeIteratorCurNode(iter, iter->node->parentnode);
  }
  return naLocateQuadTreeNodeCapture(iter);
}


#
NA_DEF NABool naIterateQuadTree(NAQuadTreeIterator* iter, const NARect* limit){
  const NAQuadTree* tree = (const NAQuadTree*)naGetPtrConst(&(iter->tree));
  
  if(!tree->root){return NA_FALSE;}
  if(!iter->node){
    naSetQuadTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->node)
      naCrash("naIterateQuadTree", "No current node after setting the current node to the root");
    #endif
  }

  // We go to the next segment. When we came to this function with segment -1,
  // we therefore start with the first segment.
  iter->childsegment++;
  
  NASize childsize = naGetQuadTreeSizeWithExponent(iter->node->childexponent);
  
  // Search for a segment which is available.
  while(iter->childsegment < 4){
    if(iter->node->child[iter->childsegment]){
      if(!limit){
        // We have a child which is present and overlaps with the limit if
        // available.
        break;
      }
      NARect childrect = naMakeRect(naGetQuadTreeNodeChildOrigin(iter->node, iter->childsegment), childsize);
      NARect clamprect = naClampRectToRect(childrect, *limit);
      if(naIsRectUseful(clamprect)){
        // We have a child which is present and overlaps with the limit if
        // available.
        break;
      }
      break;
    }
    iter->childsegment++;
  }
  
  if(iter->childsegment < 4){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(iter->node->leafmask & (1 << iter->childsegment)){
      // Good ending. We found a next leaf.
      iter->pos = naGetQuadTreeNodeChildOrigin(iter->node, iter->childsegment);
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetQuadTreeIteratorCurNode(iter, iter->node->child[iter->childsegment]);
      // We start looking in the subnode from the start.
      iter->childsegment = -1;
      return naIterateQuadTree(iter, limit);
    }
    
  }else{
    // There is no more segment available in this node. Go upwards.
    if(iter->node->parentnode){
      #ifndef NDEBUG
        if(iter->node->segmentinparent == -1)
          naError("naIterateQuadTreeNode", "Inernal inconsistency detected: Segment in parent should not be -1");
      #endif
      iter->childsegment = iter->node->segmentinparent;
      naSetQuadTreeIteratorCurNode(iter, iter->node->parentnode);
      return naIterateQuadTree(iter, limit);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naResetQuadTreeIterator(iter);
      return NA_FALSE;
    }
  }
}


#
NA_DEF NABool naLocateQuadTreeCoord(NAQuadTreeIterator* iter, NAPos coord){
  iter->pos = coord;
  return naLocateQuadTreeNodeBubble(iter);
}


#
NA_DEF NABool naLocateQuadTreeIterator(NAQuadTreeIterator* dstiter, const NAQuadTreeIterator* srciter){
  dstiter->pos = naGetQuadTreeCurOrigin(srciter);
  return naLocateQuadTreeNodeBubble(dstiter);
}


#
NA_DEF NABool naLocateQuadTreeSteps(NAQuadTreeIterator* iter, NAInt stepx, NAInt stepy){
  const NAQuadTree* tree = naGetPtrConst(&(iter->tree));

  NASize baseleafsize = naGetQuadTreeSizeWithExponent(naGetQuadTreeBaseLeafExponent(tree));
  iter->pos.x += stepx * baseleafsize.width;
  iter->pos.y += stepy * baseleafsize.height;

  return naLocateQuadTreeNodeBubble(iter);
}


#
NA_DEF void naUpdateQuadTreeCur(NAQuadTreeIterator* iter){
  if(iter->node){
    NAQuadTree* tree = naGetPtrMutable(&(iter->tree));
    naUpdateQuadTreeNodeBubbling(tree, iter->node, iter->childsegment);
  }
}


#
NA_DEF void naUpdateQuadTree(NAQuadTree* tree){
  if(tree->root){
    naUpdateQuadTreeNodeCapturing(tree, tree->root);
  }
}








// ////////////////////////////
// Tree
// ////////////////////////////

#
NA_DEF NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAQuadTreeConfiguration configuration){
  #ifndef NDEBUG
    if(!tree)
      naCrash("naInitQuadTree", "tree is Null-Pointer");
    if(!configuration.leafallocator)
      naError("naInitQuadTree", "Must have a data allocator");
    if(!configuration.leafdeallocator)
      naError("naInitQuadTree", "Must have a data deallocator");
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
  while(naIterateQuadTree(&dupiter, NA_NULL)){
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



NA_DEF NAQuadTree* naInitQuadTreeCopyShifted(NAQuadTree* newtree, const NAQuadTree* duptree, NAPos shift){
  double x1bound;
  double y1bound;
  double x2bound;
  double y2bound;
  NARect rect0;
  NARect rect1;
  NARect rect2;
  NARect rect3;
  NAQuadTreeIterator newiter;
  NAQuadTreeIterator iter;
  NASize baseleafsize;
  
  #ifndef NDEBUG
    if(!newtree)
      naCrash("naCopyQuadTreeWithShift", "newtree is Null-Pointer");
    if(!duptree->configuration.datacopier)
      naCrash("naCopyQuadTreeWithShift", "Data copier callback required for this function to work.");
  #endif
  newtree = naInitQuadTree(newtree, duptree->configuration);
  if(!duptree->root){return newtree;}

  // Create four rects which denote the rects in the new shifted tree which
  // are aligned to a leaflength.
  baseleafsize = naGetQuadTreeSizeWithExponent(naGetQuadTreeBaseLeafExponent(duptree));

  x1bound = naMod(shift.x, baseleafsize.width);
  y1bound = naMod(shift.y, baseleafsize.height);
  x2bound = baseleafsize.width - x1bound;
  y2bound = baseleafsize.height - y1bound;
  rect0 = naMakeRectE(naMakePos(shift.x, shift.y), naMakeSizeE(x2bound, y2bound));
  rect1 = naMakeRectE(naMakePos(shift.x + x2bound, shift.y), naMakeSizeE(x1bound, y2bound));
  rect2 = naMakeRectE(naMakePos(shift.x, shift.y + y2bound), naMakeSizeE(x2bound, y1bound));
  rect3 = naMakeRectE(naMakePos(shift.x + x2bound, shift.y + y2bound), naMakeSizeE(x1bound, y1bound));
  
  newiter = naMakeQuadTreeModifier(newtree);
  iter = naMakeQuadTreeAccessor(duptree);
  
  while(naIterateQuadTree(&iter, NA_NULL)){
    NAPos origin;
    NAPos neworigin;
    void* newdata;
    const void* dupchunk = naGetQuadTreeCurConst(&iter);
    origin = naGetQuadTreeCurOrigin(&iter);

    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 4 new leaves.
    if(!naIsRectEmpty(rect0)){
      neworigin = naMakePos(origin.x + rect0.pos.x, origin.y + rect0.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(x1bound, y1bound), dupchunk, naMakePos(0, 0), rect0.size, shift, baseleafsize);
    }
    if(!naIsRectEmpty(rect1)){
      neworigin = naMakePos(origin.x + rect1.pos.x, origin.y + rect1.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(0, y1bound), dupchunk, naMakePos(x2bound, 0), rect1.size, shift, baseleafsize);
    }
    if(!naIsRectEmpty(rect2)){
      neworigin = naMakePos(origin.x + rect2.pos.x, origin.y + rect2.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(x1bound, 0), dupchunk, naMakePos(0, y2bound), rect2.size, shift, baseleafsize);
    }
    if(!naIsRectEmpty(rect3)){
      neworigin = naMakePos(origin.x + rect3.pos.x, origin.y + rect3.pos.y);
      naLocateQuadTreeCoord(&newiter, neworigin);
      newdata = naGetQuadTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakePos(0, 0), dupchunk, naMakePos(x2bound, y2bound), rect3.size, shift, baseleafsize);
    }
  }
  naClearQuadTreeIterator(&iter);
  naClearQuadTreeIterator(&newiter);
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateQuadTree(newtree);
  return newtree;
}


#
NA_DEF void naClearQuadTree(NAQuadTree* tree){
  naEmptyQuadTree(tree);
}


#
NA_DEF void naEmptyQuadTree(NAQuadTree* tree){
  #ifndef NDEBUG
    if(tree->itercount != 0)
      naError("naEmptyQuadTree", "There are still iterators running on this tree. Did you miss a naClearQuadTreeIterator call?");
  #endif
  if(tree->root){naDeallocQuadTreeNode(tree, tree->root);}
  tree->root = NA_NULL;
}


#
NA_DEF void* naGetQuadTreeRootNodeData(NAQuadTree* tree){
  return (tree->root) ? (tree->root->nodedata) : NA_NULL;
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
