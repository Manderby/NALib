
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../NAOctTree.h"
#include "../NAMemory.h"



struct NAOctTreeNode{
  int16 childexponent;
  int16 segmentinparent;
  NAOctTreeNode* parentnode;
  uint16 leafmask;
  void* childs[8];
  NAVertex origin;
  void* nodedata;
  #ifndef NDEBUG
    NAInt itercount;
  #endif
};



NA_HAPI void naDeallocOctTreeNode(NAOctTree* tree, NAOctTreeNode* node);
NA_HAPI void naEnsureOctTreeInvariant(NAOctTreeIterator* iter);



NA_HIDEF int16 naGetOctTreeBaseLeafExponent(const NAOctTree* tree){
  return tree->configuration.baseleafexponent;
}



NA_HIDEF NAVolume naGetOctTreeVolumeWithExponent(int16 exponent){
  double length = naMakeDoubleWithExponent((int32)exponent);
  return naMakeVolume(length, length, length);
}



NA_HDEF NAVertex naGetOctTreeSegmentOrigin(NAVertex parentorigin, NAVolume childvolume, int16 childsegment){
  NAVertex childorigin = parentorigin;
  if(childsegment & 1){childorigin.x += childvolume.width;}
  if(childsegment & 2){childorigin.y += childvolume.height;}
  if(childsegment & 4){childorigin.z += childvolume.depth;}
  return childorigin;
}



NA_HDEF NABox naGetOctTreeNodeSegmentBox(NAOctTreeNode* parentnode, int16 childsegment){
  #ifndef NDEBUG
    if(!parentnode)
      naCrash("naGetOctTreeNodeSegmentBox", "parentnode is null");
  #endif
  NABox childbox;
  childbox.volume = naGetOctTreeVolumeWithExponent(parentnode->childexponent);
  childbox.vertex = naGetOctTreeSegmentOrigin(parentnode->origin, childbox.volume, childsegment);
  return childbox;
}



NA_DEF NABox naGetOctTreeAlignedBox(int16 leafexponent, NAVertex vertex){
  NAVolume leafvolume = naGetOctTreeVolumeWithExponent(leafexponent);
  NABox leafalign = naMakeBox(naMakeVertex(0, 0, 0), leafvolume);
  NABox retbox = naMakeBox(naMakeVertexWithAlignment(vertex, leafalign), leafvolume);
  return retbox;
}



// ////////////////////////////
// Node
// ////////////////////////////




NA_HIDEF NABool naIsOctTreeNodeSegmentLeaf(NAOctTreeNode* node, int16 segment){
  return node->leafmask & (1 << segment);
}



NA_HIDEF void naSetOctTreeNodeSegment(NAOctTreeNode* parentnode, int16 segment, NAOctTreeNode* childnode){
  parentnode->childs[segment] = childnode;
  if(childnode){
    parentnode->leafmask &= ~(1 << segment);
  }else{
    parentnode->leafmask |= (1 << segment);
  }
}



NA_HIDEF NABox naGetOctTreeNodeBox(NAOctTreeNode* node){
  return naMakeBox(node->origin, naGetOctTreeVolumeWithExponent(node->childexponent + 1));
}



NA_HDEF void naFillOctTreeNodeChildData(const void* childdata[8], NAOctTreeNode* node){
  #ifndef NDEBUG
    if(!(node->leafmask & 0x01) && !node->childs[0])
      naCrash("naFillOctTreeNodeChildData", "Node 0 is null");
    if(!(node->leafmask & 0x02) && !node->childs[1])
      naCrash("naFillOctTreeNodeChildData", "Node 1 is null");
    if(!(node->leafmask & 0x04) && !node->childs[2])
      naCrash("naFillOctTreeNodeChildData", "Node 2 is null");
    if(!(node->leafmask & 0x08) && !node->childs[3])
      naCrash("naFillOctTreeNodeChildData", "Node 3 is null");
    if(!(node->leafmask & 0x10) && !node->childs[4])
      naCrash("naFillOctTreeNodeChildData", "Node 4 is null");
    if(!(node->leafmask & 0x20) && !node->childs[5])
      naCrash("naFillOctTreeNodeChildData", "Node 5 is null");
    if(!(node->leafmask & 0x40) && !node->childs[6])
      naCrash("naFillOctTreeNodeChildData", "Node 6 is null");
    if(!(node->leafmask & 0x80) && !node->childs[7])
      naCrash("naFillOctTreeNodeChildData", "Node 7 is null");
  #endif
  childdata[0] = (node->leafmask & 0x01) ? node->childs[0] : (((NAOctTreeNode*)(node->childs[0]))->nodedata);
  childdata[1] = (node->leafmask & 0x02) ? node->childs[1] : (((NAOctTreeNode*)(node->childs[1]))->nodedata);
  childdata[2] = (node->leafmask & 0x04) ? node->childs[2] : (((NAOctTreeNode*)(node->childs[2]))->nodedata);
  childdata[3] = (node->leafmask & 0x08) ? node->childs[3] : (((NAOctTreeNode*)(node->childs[3]))->nodedata);
  childdata[4] = (node->leafmask & 0x10) ? node->childs[4] : (((NAOctTreeNode*)(node->childs[4]))->nodedata);
  childdata[5] = (node->leafmask & 0x20) ? node->childs[5] : (((NAOctTreeNode*)(node->childs[5]))->nodedata);
  childdata[6] = (node->leafmask & 0x40) ? node->childs[6] : (((NAOctTreeNode*)(node->childs[6]))->nodedata);
  childdata[7] = (node->leafmask & 0x80) ? node->childs[7] : (((NAOctTreeNode*)(node->childs[7]))->nodedata);
}



// Creates an inner node.
NA_HDEF NAOctTreeNode* naAllocOctTreeNode(int16 childexponent, int16 segmentinparent, NAOctTreeNode* parentnode, NAVertex origin, NAOctTree* tree){
  NAOctTreeNode* node = naAlloc(NAOctTreeNode);
  node->childexponent = childexponent;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->leafmask = 0xff;
  node->childs[0] = NA_NULL;
  node->childs[1] = NA_NULL;
  node->childs[2] = NA_NULL;
  node->childs[3] = NA_NULL;
  node->childs[4] = NA_NULL;
  node->childs[5] = NA_NULL;
  node->childs[6] = NA_NULL;
  node->childs[7] = NA_NULL;
  node->origin = origin;
  if(tree->configuration.nodeallocator){
    NAVolume childvolume = naGetOctTreeVolumeWithExponent(node->childexponent);
    #ifndef NDEBUG
      if(parentnode){
        NABox testbox = naGetOctTreeNodeSegmentBox(parentnode, segmentinparent);
        NABox testchildbox = naMakeBox(origin, naGetOctTreeVolumeWithExponent(node->childexponent + 1));
        if(!naContainsBoxBox(testbox, testchildbox))
          naError("naAllocOctTreeNode", "Child is not within segment box");
      }
    #endif
    node->nodedata = tree->configuration.nodeallocator(origin, childvolume);
  }else{
    node->nodedata = NA_NULL;
  }
  #ifndef NDEBUG
    node->itercount = 0;
  #endif
  return node;
}



NA_HIDEF void naDeallocOctTreeNodeChild(NAOctTree* tree, NAOctTreeNode* node, int16 segment){
  if(naIsOctTreeNodeSegmentLeaf(node, segment)){
    if(node->childs[segment]){
      tree->configuration.leafdeallocator(node->childs[segment], tree->configuration.userdata);
    }
  }else{
    naDeallocOctTreeNode(tree, node->childs[segment]);
  }
}



// Deallocates one node. Deallocates all subnodes and leafes in the process.
NA_HDEF void naDeallocOctTreeNode(NAOctTree* tree, NAOctTreeNode* node){
  #ifndef NDEBUG
    if(!node)
      naCrash("naDeallocOctTreeNode", "Iterator is null");
    if(node->itercount)
      naError("naDeallocOctTreeNode", "Iterators still running on this node. Did you forget a call to NAClearOctTreeIterator?");
  #endif

  naDeallocOctTreeNodeChild(tree, node, 0);
  naDeallocOctTreeNodeChild(tree, node, 1);
  naDeallocOctTreeNodeChild(tree, node, 2);
  naDeallocOctTreeNodeChild(tree, node, 3);
  naDeallocOctTreeNodeChild(tree, node, 4);
  naDeallocOctTreeNodeChild(tree, node, 5);
  naDeallocOctTreeNodeChild(tree, node, 6);
  naDeallocOctTreeNodeChild(tree, node, 7);
  
  if(node->nodedata && tree->configuration.nodedeallocator){
    tree->configuration.nodedeallocator(node->nodedata);
  }
  naFree(node);
}



// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAOctTreeNode* naAddOctTreeNodeChild(NAOctTree* tree, NAOctTreeNode* parentnode, int16 segmentinparent){
  NAOctTreeNode* node;
  #ifndef NDEBUG
    if(parentnode->childs[segmentinparent])
      naError("naAddOctTreeNodeChild", "A child already exists at that segment of the parent node.");
  #endif

  NABox segmentbox = naGetOctTreeNodeSegmentBox(parentnode, segmentinparent);
  node = naAllocOctTreeNode(parentnode->childexponent - 1, segmentinparent, parentnode, segmentbox.vertex, tree);
  naSetOctTreeNodeSegment(parentnode, segmentinparent, node);
  return node;
}



// Returns the segment index [0-7] in which the given vertex can be found.
// Warning: The vertex is expected to be inside this node.
NA_HIDEF int16 naGetOctTreeSegment(NAVertex parentorigin, int16 childexponent, NAVertex vertex){
  int16 segment = 0;
  NAVolume childvolume = naGetOctTreeVolumeWithExponent(childexponent);
  if(vertex.x >= parentorigin.x + childvolume.width) {segment |= 1;}
  if(vertex.y >= parentorigin.y + childvolume.height){segment |= 2;}
  if(vertex.z >= parentorigin.z + childvolume.depth) {segment |= 4;}
  #ifndef NDEBUG
    if(!naContainsBoxVertexE(naMakeBox(parentorigin, naGetOctTreeVolumeWithExponent(childexponent + 1)), vertex))
      naError("naGetOctTreeSegment", "box of parent does not actually contain vertex");
  #endif
  return segment;
}



// Note that this function is not entirely deterministic. Depending on the
// order the leafes are created, the resulting root of the whole tree might
// be placed at a different origin. To make this completely deterministic,
// one would force the origin to align to a predefined pattern which is
// - due to the cyclic manner of the parent - a little complicated and
// frankly should be not important in any case. Therefore... maybe later.
NA_HDEF NAVertex naGetOctTreeRootOrigin(int16 childexponent, NAVertex childorigin){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  
  NAVolume childvolume = naGetOctTreeVolumeWithExponent(childexponent);
  NAVertex parentorigin = childorigin;
  int16 cycle = ((childexponent % 8) + 8 ) % 8;
  if(cycle & 1){parentorigin.x -= childvolume.width;}
  if(cycle & 2){parentorigin.y -= childvolume.height;}
  if(cycle & 4){parentorigin.z -= childvolume.depth;}

  return parentorigin;
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular
// node.
NA_HDEF void naUpdateOctTreeNodeBubbling(NAOctTree* tree, NAOctTreeNode* parentnode, int16 segment){
  NABool bubble = NA_TRUE;
  #ifndef NDEBUG
    if(parentnode == NA_NULL)
      naError("naUpdateOctTreeNodeBubbling", "Parent is null.");
  #endif
  
  NAVolume childvolume = naGetOctTreeVolumeWithExponent(parentnode->childexponent);
  
  // We call the update callback.
  if(tree->configuration.childchanged){
    const void* childdata[8];
    naFillOctTreeNodeChildData(childdata, parentnode);
    bubble = tree->configuration.childchanged(parentnode->nodedata, segment, childdata, parentnode->leafmask, childvolume);
  }
  
  // Then we propagate the message towards the root if requested.
  if(bubble && parentnode->parentnode){
    #ifndef NDEBUG
      if(parentnode->segmentinparent == -1)
        naError("naUpdateOctTreeNode", "Internal inconsistency detected");
    #endif
    naUpdateOctTreeNodeBubbling(tree, parentnode->parentnode, parentnode->segmentinparent);
  }
}




// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the childchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateOctTreeNodeCapturing(NAOctTree* tree, NAOctTreeNode* node){
  NABool bubble = NA_FALSE;
  
  NAVolume childvolume = naGetOctTreeVolumeWithExponent(node->childexponent);

  #ifndef NDEBUG
    if(!(node->leafmask & 0x01) && !node->childs[0])
      naError("naUpdateOctTreeNodeCapturing", "Node 0 is null");
    if(!(node->leafmask & 0x02) && !node->childs[1])
      naError("naUpdateOctTreeNodeCapturing", "Node 1 is null");
    if(!(node->leafmask & 0x04) && !node->childs[2])
      naError("naUpdateOctTreeNodeCapturing", "Node 2 is null");
    if(!(node->leafmask & 0x08) && !node->childs[3])
      naError("naUpdateOctTreeNodeCapturing", "Node 3 is null");
    if(!(node->leafmask & 0x10) && !node->childs[4])
      naError("naUpdateOctTreeNodeCapturing", "Node 4 is null");
    if(!(node->leafmask & 0x20) && !node->childs[5])
      naError("naUpdateOctTreeNodeCapturing", "Node 5 is null");
    if(!(node->leafmask & 0x40) && !node->childs[6])
      naError("naUpdateOctTreeNodeCapturing", "Node 6 is null");
    if(!(node->leafmask & 0x80) && !node->childs[7])
      naError("naUpdateOctTreeNodeCapturing", "Node 7 is null");
  #endif

  // this node stores subnodes
  if(!(node->leafmask & 0x01) && node->childs[0]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[0]);}
  if(!(node->leafmask & 0x02) && node->childs[1]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[1]);}
  if(!(node->leafmask & 0x04) && node->childs[2]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[2]);}
  if(!(node->leafmask & 0x08) && node->childs[3]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[3]);}
  if(!(node->leafmask & 0x10) && node->childs[4]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[4]);}
  if(!(node->leafmask & 0x20) && node->childs[5]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[5]);}
  if(!(node->leafmask & 0x40) && node->childs[6]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[6]);}
  if(!(node->leafmask & 0x80) && node->childs[7]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->childs[7]);}
  if(tree->configuration.childchanged){
    const void* childdata[8];
    naFillOctTreeNodeChildData(childdata, node);
    bubble |= tree->configuration.childchanged(node->nodedata, -1, childdata, node->leafmask, childvolume);
  }
  
  return bubble;
}



NA_HIDEF void naSetOctTreeIteratorCurNode(NAOctTreeIterator* iter, NAOctTreeNode* newnode){
  #ifndef NDEBUG
    if(iter->node){iter->node->itercount--;}
  #endif
  iter->node = newnode;
  #ifndef NDEBUG
    if(iter->node){iter->node->itercount++;}
  #endif
}




// Expects the node to be the node to remove.
// The replacenode is a node which currently is stored as a child but which
// shall get in place of this node.
NA_HDEF void naRemoveOctTreeNode(NAOctTreeIterator* iter, NAOctTreeNode* replacenode){
  NAOctTreeNode* node = iter->node;
  NAOctTree* tree = naGetPtrMutable(&(iter->tree));
  
  // As the current node will be erased, we adjust the iterator to point
  // at the parent.
  naSetOctTreeIteratorCurNode(iter, node->parentnode);
  iter->childsegment = node->segmentinparent;

  if(node->parentnode){
    if(replacenode){
      node->parentnode->childs[node->segmentinparent] = replacenode;
      replacenode->parentnode = node->parentnode;
      replacenode->segmentinparent = node->segmentinparent;
    }else{
      // There is no replace node.
      naSetOctTreeNodeSegment(node->parentnode, node->segmentinparent, NA_NULL);
    }
    // Ensure the invariant for the parent.
    naEnsureOctTreeInvariant(iter);
  }else{
    // This was the root.
    if(replacenode){
      replacenode->parentnode = NA_NULL;
      replacenode->segmentinparent = -1;
    }
    tree->root = replacenode;
  }
  
  // Finally, we deallocate the node.
  naDeallocOctTreeNode(tree, node);
}







// ////////////////////////////
// Iterator
// ////////////////////////////


// Debugging flags
#define NA_OCTTREE_ITERATOR_MUTATOR         0x01
#define NA_OCTTREE_ITERATOR_MODIFIER        0x02




// Checks whether the iterator is positioned at a leaf and it exists.
NA_HIDEF NABool naIsOctTreeIteratorAtLeaf(NAOctTreeIterator* iter){
  return (iter->node) && (iter->childsegment != -1) && (iter->node->childs[iter->childsegment]) && naIsOctTreeNodeSegmentLeaf(iter->node, iter->childsegment);
}



// This is a rather large function. It handles creation of a leaf no matter
// where the iterator is. The desired leaf shall contain the position stored
// in the iterator. If the iterator is positioned at a node, that node should
// contain the desired position. If a segment is selected (!= -1), then that
// would be the position where the new leaf will be created.
//
// In certain cases, this function is called recursively when helping nodes
// needed to be created.
NA_HDEF void naCreateOctTreeLeaf(NAOctTreeIterator* iter, const void* data){
  NAOctTree* tree;

  #ifndef NDEBUG
    if(!(iter->flags & NA_OCTTREE_ITERATOR_MODIFIER))
      naError("naCreateOctTreeLeaf", "iterator must be modifier");
    if((iter->childsegment != -1))
      naError("naCreateOctTreeLeaf", "iterator is already at a leaf");
  #endif

  tree = (NAOctTree*)naGetPtrMutable(&(iter->tree));
  int16 baseleafexponent = naGetOctTreeBaseLeafExponent(tree);

  if(!iter->node){
    // The tree root does not contain the vertex or does not exist at all.
    if(tree->root){
      // We have a root but the desired vertex is not contained within.
      // We expand the tree at the root.
      int16 parentchildexponent = tree->root->childexponent;
      NAVertex parentorigin = tree->root->origin;
      while(1){
        parentchildexponent++;
        parentorigin = naGetOctTreeRootOrigin(parentchildexponent, parentorigin);
        NAVolume parentvolume = naGetOctTreeVolumeWithExponent(parentchildexponent + 1);
        if(naContainsBoxVertexE(naMakeBox(parentorigin, parentvolume), iter->vertex)){break;}
      }
      NAOctTreeNode* newroot = naAllocOctTreeNode(parentchildexponent, tree->root->segmentinparent, tree->root->parentnode, parentorigin, tree);
      tree->root->segmentinparent = naGetOctTreeSegment(newroot->origin, newroot->childexponent, tree->root->origin);
      tree->root->parentnode = newroot;
      naSetOctTreeNodeSegment(newroot, tree->root->segmentinparent, tree->root);
      tree->root = newroot;
    }else{
      // We create the very first node of this tree.
      NAVertex leaforigin = naGetOctTreeAlignedBox(baseleafexponent, iter->vertex).vertex;
      NAVertex rootorigin = naGetOctTreeRootOrigin(baseleafexponent, leaforigin);
      tree->root = naAllocOctTreeNode(baseleafexponent, -1, NA_NULL, rootorigin, tree);
    }
    // We call this function recursively.
    naSetOctTreeIteratorCurNode(iter, tree->root);
    naCreateOctTreeLeaf(iter, data);
    
  }else{
    // ...else we have a current node which is expected to contain the vertex.
    int16 segment;

    #ifndef NDEBUG
      NABox nodebox = naGetOctTreeNodeBox(iter->node);
      if(!naContainsBoxVertexE(nodebox, iter->vertex))
        naError("naCreateOctTreeLeaf", "Cur node does not contain the desired vertex.");
    #endif
        
    segment = naGetOctTreeSegment(iter->node->origin, iter->node->childexponent, iter->vertex);
    if(iter->node->childs[segment]){
      #ifndef NDEBUG
        if(naIsOctTreeNodeSegmentLeaf(iter->node, segment))
          naError("naCreateOctTreeLeaf", "Leaf already exists.");
      #endif
      // There already is a segment, but it holds a node which is too small to
      // contain the desired origin. Create a common parent of the two and
      // reattach them.
      NAOctTreeNode* existingchildnode = iter->node->childs[segment];
      NAVertex innerparentorigin = naGetOctTreeSegmentOrigin(iter->node->origin, naGetOctTreeVolumeWithExponent(iter->node->childexponent), segment);
      int16 innerchildexponent = iter->node->childexponent - 1;
      NAVolume innerchildvolume = naGetOctTreeVolumeWithExponent(innerchildexponent);
      while(1){
        int16 innersegment = naGetOctTreeSegment(innerparentorigin, innerchildexponent, iter->vertex);
        if(innersegment == naGetOctTreeSegment(innerparentorigin, innerchildexponent, existingchildnode->origin)){
          innerparentorigin = naGetOctTreeSegmentOrigin(innerparentorigin, innerchildvolume, innersegment);
          innerchildexponent = innerchildexponent - 1;
          innerchildvolume = naGetOctTreeVolumeWithExponent(innerchildexponent);
        }else{
          break;
        }
      }
      #ifndef NDEBUG
        if(innerchildexponent <= existingchildnode->childexponent)
          naError("naCreateOctTreeLeaf", "Wrong exponent");
      #endif
      NAOctTreeNode* newparent = naAllocOctTreeNode(innerchildexponent, existingchildnode->segmentinparent, existingchildnode->parentnode, innerparentorigin, tree);
      naSetOctTreeNodeSegment(existingchildnode->parentnode, existingchildnode->segmentinparent, newparent);
      existingchildnode->segmentinparent = naGetOctTreeSegment(newparent->origin, newparent->childexponent, existingchildnode->origin);
      existingchildnode->parentnode = newparent;
      naSetOctTreeNodeSegment(newparent, existingchildnode->segmentinparent, existingchildnode);
      
      // We call this function recursively.
      naSetOctTreeIteratorCurNode(iter, newparent);
      naCreateOctTreeLeaf(iter, data);
    }else{
      // There is no segment.
      if(iter->node->childexponent <= baseleafexponent){
        // The node childexponent is small enough to contain a leaf.
        NABox leafbox = naGetOctTreeNodeSegmentBox(iter->node, segment);
        iter->node->childs[segment] = tree->configuration.leafallocator(leafbox, tree->configuration.userdata, data);
        iter->childsegment = segment;
        // Good ending. We created the leaf.
      }else{
        // The node has a too big childexponent. We need to create another
        // internal subnode and call this function recursively.
        NAVertex parentorigin = naMakeVertexWithAlignment(iter->vertex, naMakeBox(iter->node->origin, naGetOctTreeVolumeWithExponent(baseleafexponent + 1)));
        NAOctTreeNode* newnode = naAllocOctTreeNode(baseleafexponent, segment, iter->node, parentorigin, tree);
        naSetOctTreeNodeSegment(iter->node, segment, newnode);
        naSetOctTreeIteratorCurNode(iter, newnode);
        naCreateOctTreeLeaf(iter, data);
      }
    }
  }
}



NA_HIDEF void naInitOctTreeIterator(NAOctTreeIterator* iter){
  #ifndef NDEBUG
    NAOctTree* mutabletree = (NAOctTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount++;
  #endif
  iter->node = NA_NULL;
  iter->childsegment = -1;
  iter->vertex = naMakeVertex(0., 0., 0.);
  #ifndef NDEBUG
    iter->flags = 0;
  #endif
}



NA_DEF NAOctTreeIterator naMakeOctTreeAccessor(const NAOctTree* tree){
  NAOctTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitOctTreeIterator(&iter);
  return iter;
}



NA_DEF NAOctTreeIterator naMakeOctTreeMutator(const NAOctTree* tree){
  NAOctTreeIterator iter;
  iter.tree = naMakePtrWithDataConst(tree);
  naInitOctTreeIterator(&iter);
  #ifndef NDEBUG
    iter.flags |= NA_OCTTREE_ITERATOR_MUTATOR;
  #endif
  return iter;
}



NA_DEF NAOctTreeIterator naMakeOctTreeModifier(NAOctTree* tree){
  NAOctTreeIterator iter;
  iter.tree = naMakePtrWithDataMutable(tree);
  naInitOctTreeIterator(&iter);
  #ifndef NDEBUG
    iter.flags |= NA_OCTTREE_ITERATOR_MUTATOR;
    iter.flags |= NA_OCTTREE_ITERATOR_MODIFIER;
  #endif
  return iter;
}



NA_DEF void naClearOctTreeIterator(NAOctTreeIterator* iter){
  #ifndef NDEBUG
    NAOctTree* mutabletree = (NAOctTree*)naGetPtrConst(&(iter->tree));
    mutabletree->itercount--;
    if(iter->node){iter->node->itercount--;}
  #endif
  naCleanupPtr(&(iter->tree), NA_NULL);
}



NA_DEF void naResetOctTreeIterator(NAOctTreeIterator* iter){
  naSetOctTreeIteratorCurNode(iter, NA_NULL);
  iter->childsegment = -1;
  iter->vertex = naMakeVertex(0., 0., 0.);
}



NA_DEF const void* naGetOctTreeCurConst(NAOctTreeIterator* iter){
  if(naIsOctTreeIteratorAtLeaf(iter)){
    return iter->node->childs[iter->childsegment];
  }else{
    return NA_NULL;
  }
}



NA_DEF void* naGetOctTreeCurMutable(NAOctTreeIterator* iter, NABool create){
  #ifndef NDEBUG
    if(!(iter->flags & NA_OCTTREE_ITERATOR_MUTATOR))
      naError("naGetOctTreeCurMutable", "Trying to mutate element with iterator which was created non-mutable");
    if(create && !(iter->flags & NA_OCTTREE_ITERATOR_MODIFIER))
      naError("naGetOctTreeCurMutable", "iterator must be modifier when create is true.");
  #endif
  if(naIsOctTreeIteratorAtLeaf(iter)){
    return iter->node->childs[iter->childsegment];
  }else{
    if(create){
      naCreateOctTreeLeaf(iter, NA_NULL);
      return iter->node->childs[iter->childsegment];
    }else{
      return NA_NULL;
    }
  }
}



NA_DEF NABox naGetOctTreeCurBox(const NAOctTreeIterator* iter){
  #ifndef NDEBUG
    if(!iter->node)
      naError("naGetOctTreeCurBox", "Iterator has no valid position");
    if(iter->childsegment == -1)
      naError("naGetOctTreeCurBox", "Iterator is not positioned on a leaf");
  #endif
  return naGetOctTreeNodeSegmentBox(iter->node, iter->childsegment);
}



NA_DEF NABoxi naGetOctTreeCurBoxi(const NAOctTreeIterator* iter){
  #ifndef NDEBUG
    if(!iter->node)
      naError("naGetOctTreeCurBoxi", "Iterator has no valid position");
    if(iter->childsegment == -1)
      naError("naGetOctTreeCurBoxi", "Iterator is not positioned on a leaf");
    const NAOctTree* tree = naGetPtrConst(&(iter->tree));
    if(naGetOctTreeBaseLeafExponent(tree) < 0)
      naError("naGetOctTreeCurBoxi", "Can not return valid integer box for trees with exponents < 0");
  #endif
  // todo
  NABox box = naGetOctTreeNodeSegmentBox(iter->node, iter->childsegment);
  return naMakeBoxiSE((NAInt)naRound(box.vertex.x), (NAInt)naRound(box.vertex.y), (NAInt)naRound(box.vertex.z), (NAInt)naRound(box.volume.width), (NAInt)naRound(box.volume.height), (NAInt)naRound(box.volume.depth));
}



NA_HDEF void naEnsureOctTreeInvariant(NAOctTreeIterator* iter){
  NAInt childcount = 0;
  int16 lastsegment = -1;
  if((iter->node->childs[0])){childcount++; lastsegment = 0;}
  if((iter->node->childs[1])){childcount++; lastsegment = 1;}
  if((iter->node->childs[2])){childcount++; lastsegment = 2;}
  if((iter->node->childs[3])){childcount++; lastsegment = 3;}
  if((iter->node->childs[4])){childcount++; lastsegment = 4;}
  if((iter->node->childs[5])){childcount++; lastsegment = 5;}
  if((iter->node->childs[6])){childcount++; lastsegment = 6;}
  if((iter->node->childs[7])){childcount++; lastsegment = 7;}

  // We remove the node, if
  // - the node has no childs
  // - the node has only 1 child which is not a leaf
  // In all other cases, this node fulfills the invariant.
  if(childcount == 0){
    naRemoveOctTreeNode(iter, NA_NULL);
  }else if((childcount == 1) && !naIsOctTreeNodeSegmentLeaf(iter->node, lastsegment)){
    NAOctTreeNode* replacenode = iter->node->childs[lastsegment];
    naSetOctTreeNodeSegment(iter->node, lastsegment, NA_NULL);
    naRemoveOctTreeNode(iter, replacenode);
  }
}



NA_DEF void naRemoveOctTreeCur(NAOctTreeIterator* iter){
  NAOctTree* tree;
  #ifndef NDEBUG
    if(!iter->node)
      naError("naRemoveOctTreeCur", "iterator is at no specific node in the tree");
    if(iter->childsegment == -1)
      naError("naRemoveOctTreeCur", "iterator is at no specific leaf in the tree");
    if(!naIsOctTreeNodeSegmentLeaf(iter->node, iter->childsegment))
      naError("naRemoveOctTreeCur", "iterator is not at a leaf");
  #endif
  tree = naGetPtrMutable(&(iter->tree));
    
  // First, we destroy the data of the leaf chunk
  tree->configuration.leafdeallocator(iter->node->childs[iter->childsegment], tree->configuration.userdata);
  naSetOctTreeNodeSegment(iter->node, iter->childsegment, NA_NULL);

  // Then we make sure the parent node does not break the invariant.
  naEnsureOctTreeInvariant(iter);
}



// Moves the iterator to the closest parent node containing vertex.
// Phase 2: Capture.
NA_HDEF NABool naLocateOctTreeNodeCapture(NAOctTreeIterator* iter){
  NABool found = NA_FALSE;
  while(iter->node){
    #ifndef NDEBUG
      if(!naContainsBoxVertex(naGetOctTreeNodeBox(iter->node), iter->vertex))
        naError("naLocateOctTreeNodeCapture", "Inconsistent behaviour. Node should contain vertex");
    #endif
  
    // The vertex is stored somewhere inside the box of this node
    int16 segment = naGetOctTreeSegment(iter->node->origin, iter->node->childexponent, iter->vertex);
    if((iter->node->childs[segment])){
      // The segment which contains the vertex has a child stored.
      if(naIsOctTreeNodeSegmentLeaf(iter->node, segment)){
        // We found a leaf!
        iter->childsegment = segment;
        // Good ending.
        found = NA_TRUE;
        break;
      }else{
        // We test the child node, if it contains the desired origin.
        NABox nodebox = naGetOctTreeNodeBox(iter->node->childs[segment]);
        if(naContainsBoxVertexE(nodebox, iter->vertex)){  // The E means exclusive! This is important!
          // Go on searching in the sub-node
          naSetOctTreeIteratorCurNode(iter, iter->node->childs[segment]);
        }else{
          // The child node does not contain the desired vertex.
          // Bad ending. There is a node missing in between.
          break;
        }
      }
      
    }else{
      // There is no segment where the vertex should be. No leaf found.
      // Bad ending. We are stuck with the closest parent node.
      break;
    }
  }
  return found;
}



// Moves the iterator to the closest parent node containing vertex.
// Phase 1: Bubbling.
NA_HDEF NABool naLocateOctTreeNodeBubble(NAOctTreeIterator* iter){
  const NAOctTree* tree = naGetPtrConst(&(iter->tree));
  iter->childsegment = -1;
  
  // If the iterator has no node set, we start at the root
  if(!iter->node){
    naSetOctTreeIteratorCurNode(iter, tree->root);
  }
  
  // Then, we search towards the root until we find a node which contains the
  // desired vertex.
  while(iter->node){
    NABox nodebox = naGetOctTreeNodeBox(iter->node);
    if(naContainsBoxVertexE(nodebox, iter->vertex)){break;}
    naSetOctTreeIteratorCurNode(iter, iter->node->parentnode);
  }
  return naLocateOctTreeNodeCapture(iter);
}



NA_DEF NABool naIterateOctTree(NAOctTreeIterator* iter, const NABox* limit){
  const NAOctTree* tree = (const NAOctTree*)naGetPtrConst(&(iter->tree));
  
  if(!tree->root){return NA_FALSE;}
  
  // If the iterator has no current node being visited, we use the root. 
  if(!iter->node){
    naSetOctTreeIteratorCurNode(iter, tree->root);
    #ifndef NDEBUG
      if(!iter->node)
      naCrash("naIterateOctTree", "No current node after setting the current node to the root");
    #endif
  }

  // We go to the next segment. When we came to this function with segment -1,
  // we therefore start with the first segment.
  iter->childsegment++;
    
  // Search for a segment which is available.
  while(iter->childsegment < 8){
    if(iter->node->childs[iter->childsegment]){
      if(!limit){
        // If there is no limit, we found a new segment.
        break;
      }
      NABox childbox = naGetOctTreeNodeSegmentBox(iter->node, iter->childsegment);
      NABox clampbox = naClampBoxToBox(childbox, *limit);
      if(naIsBoxUseful(clampbox)){
        // We have a child which is present and overlaps with the limit if
        // available.
        break;
      }
    }
    iter->childsegment++;
  }
  
  if(iter->childsegment < 8){
    // There is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(naIsOctTreeNodeSegmentLeaf(iter->node, iter->childsegment)){
      // Good ending. We found a next leaf.
      iter->vertex = naGetOctTreeNodeSegmentBox(iter->node, iter->childsegment).vertex;
      return NA_TRUE;
    }else{
      // This is an inner node. We move downwards.
      naSetOctTreeIteratorCurNode(iter, iter->node->childs[iter->childsegment]);
      // We start looking in the subnode from the start.
      iter->childsegment = -1;
      return naIterateOctTree(iter, limit);
    }
    
  }else{
    // There is no more segment available in this node. Go upwards.
    if(iter->node->parentnode){
      #ifndef NDEBUG
        if(iter->node->segmentinparent == -1)
          naError("naIterateOctTreeNode", "Inernal inconsistency detected: Segment in parent should not be -1");
      #endif
      iter->childsegment = iter->node->segmentinparent;
      naSetOctTreeIteratorCurNode(iter, iter->node->parentnode);
      return naIterateOctTree(iter, limit);
    }else{
      // There is no parent node. This is the root and there are no more
      // elements to be iterated.
      naResetOctTreeIterator(iter);
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naLocateOctTreeCoord(NAOctTreeIterator* iter, NAVertex vertex){
  iter->vertex = vertex;
  return naLocateOctTreeNodeBubble(iter);
}



NA_DEF NABool naLocateOctTreeIterator(NAOctTreeIterator* dstiter, const NAOctTreeIterator* srciter){
  dstiter->vertex = naGetOctTreeCurBox(srciter).vertex;
  return naLocateOctTreeNodeBubble(dstiter);
}



NA_DEF NABool naLocateOctTreeSteps(NAOctTreeIterator* iter, NAInt stepx, NAInt stepy, NAInt stepz){
  const NAOctTree* tree = naGetPtrConst(&(iter->tree));

  NAVolume baseleafvolume = naGetOctTreeVolumeWithExponent(naGetOctTreeBaseLeafExponent(tree));
  iter->vertex.x += stepx * baseleafvolume.width;
  iter->vertex.y += stepy * baseleafvolume.height;
  iter->vertex.z += stepz * baseleafvolume.depth;

  return naLocateOctTreeNodeBubble(iter);
}



NA_DEF void naUpdateOctTreeCur(NAOctTreeIterator* iter){
  if(iter->node){
    NAOctTree* tree = naGetPtrMutable(&(iter->tree));
    naUpdateOctTreeNodeBubbling(tree, iter->node, iter->childsegment);
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


NA_DEF NAOctTree* naInitOctTree(NAOctTree* tree, NAOctTreeConfiguration configuration){
  #ifndef NDEBUG
    if(!tree)
      naCrash("naInitOctTree", "tree is Null-Pointer");
    if(!configuration.leafallocator)
      naError("naInitOctTree", "Must have a data allocator");
    if(!configuration.leafdeallocator)
      naError("naInitOctTree", "Must have a data deallocator");
  #endif
  tree->configuration = configuration;
  tree->root = NA_NULL;
  #ifndef NDEBUG
    tree->itercount = 0;
  #endif
  return tree;
}



NA_DEF NAOctTree* naInitOctTreeCopy(NAOctTree* newtree, const NAOctTree* duptree){
  NAOctTreeIterator newiter;
  NAOctTreeIterator dupiter;
  newtree = naInitOctTree(newtree, duptree->configuration);

  newiter = naMakeOctTreeModifier(newtree);
  dupiter = naMakeOctTreeAccessor(duptree);
  while(naIterateOctTree(&dupiter, NA_NULL)){
    NAVertex duporigin;
    const void* dupdata = naGetOctTreeCurConst(&dupiter);
    duporigin = naGetOctTreeCurBox(&dupiter).vertex;
    naLocateOctTreeCoord(&newiter, duporigin);
    naCreateOctTreeLeaf(&newiter, dupdata);
  }
  
  naClearOctTreeIterator(&dupiter);
  naClearOctTreeIterator(&newiter);

  return newtree;
}



NA_DEF NAOctTree* naInitOctTreeCopyShifted(NAOctTree* newtree, const NAOctTree* duptree, NAVertex shift){
  double x1bound;
  double y1bound;
  double z1bound;
  double x2bound;
  double y2bound;
  double z2bound;
  NABox box0;
  NABox box1;
  NABox box2;
  NABox box3;
  NABox box4;
  NABox box5;
  NABox box6;
  NABox box7;
  NAOctTreeIterator newiter;
  NAOctTreeIterator iter;
  NAVolume baseleafvolume;
  
  #ifndef NDEBUG
    if(!newtree)
      naCrash("naCopyOctTreeWithShift", "newtree is Null-Pointer");
    if(!duptree->configuration.datacopier)
      naCrash("naCopyOctTreeWithShift", "Data copier callback required for this function to work.");
  #endif
  newtree = naInitOctTree(newtree, duptree->configuration);
  if(!duptree->root){return newtree;}

  // Create eight boxs which denote the boxs in the new shifted tree which
  // are aligned to a leaflength.
  baseleafvolume = naGetOctTreeVolumeWithExponent(naGetOctTreeBaseLeafExponent(duptree));

  x1bound = naMod(shift.x, baseleafvolume.width);
  y1bound = naMod(shift.y, baseleafvolume.height);
  z1bound = naMod(shift.z, baseleafvolume.depth);
  x2bound = baseleafvolume.width - x1bound;
  y2bound = baseleafvolume.height - y1bound;
  z2bound = baseleafvolume.depth - z1bound;
  box0 = naMakeBoxE(naMakeVertex(shift.x, shift.y, shift.z), naMakeVolumeE(x2bound, y2bound, z2bound));
  box1 = naMakeBoxE(naMakeVertex(shift.x + x2bound, shift.y, shift.z), naMakeVolumeE(x1bound, y2bound, z2bound));
  box2 = naMakeBoxE(naMakeVertex(shift.x, shift.y + y2bound, shift.z), naMakeVolumeE(x2bound, y1bound, z2bound));
  box3 = naMakeBoxE(naMakeVertex(shift.x + x2bound, shift.y + y2bound, shift.z), naMakeVolumeE(x1bound, y1bound, z2bound));
  box4 = naMakeBoxE(naMakeVertex(shift.x, shift.y, shift.z + z2bound), naMakeVolumeE(x2bound, y2bound, z1bound));
  box5 = naMakeBoxE(naMakeVertex(shift.x + x2bound, shift.y, shift.z + z2bound), naMakeVolumeE(x1bound, y2bound, z1bound));
  box6 = naMakeBoxE(naMakeVertex(shift.x, shift.y + y2bound, shift.z + z2bound), naMakeVolumeE(x2bound, y1bound, z1bound));
  box7 = naMakeBoxE(naMakeVertex(shift.x + x2bound, shift.y + y2bound, shift.z + z2bound), naMakeVolumeE(x1bound, y1bound, z1bound));
  
  newiter = naMakeOctTreeModifier(newtree);
  iter = naMakeOctTreeAccessor(duptree);
  
  while(naIterateOctTree(&iter, NA_NULL)){
    NABox box;
    NAVertex neworigin;
    void* newdata;
    const void* dupchunk = naGetOctTreeCurConst(&iter);
    box = naGetOctTreeCurBox(&iter);
    #ifndef NDEBUG
      if(!naEqualVolume(box.volume, naGetOctTreeVolumeWithExponent(naGetOctTreeBaseLeafExponent(duptree))))
        naError("naInitOctTreeCopyShifted", "The implementation is not ready for shifting trees with different leaf sizes. sorry.");
    #endif

    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 8 new leaves.
    if(!naIsBoxEmpty(box0)){
      neworigin = naMakeVertex(box.vertex.x + box0.vertex.x, box.vertex.y + box0.vertex.y, box.vertex.z + box0.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(x1bound, y1bound, z1bound), dupchunk, naMakeVertex(0, 0, 0), box0.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box1)){
      neworigin = naMakeVertex(box.vertex.x + box1.vertex.x, box.vertex.y + box1.vertex.y, box.vertex.z + box1.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(0, y1bound, z1bound), dupchunk, naMakeVertex(x2bound, 0, 0), box1.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box2)){
      neworigin = naMakeVertex(box.vertex.x + box2.vertex.x, box.vertex.y + box2.vertex.y, box.vertex.z + box2.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(x1bound, 0, z1bound), dupchunk, naMakeVertex(0, y2bound, 0), box2.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box3)){
      neworigin = naMakeVertex(box.vertex.x + box3.vertex.x, box.vertex.y + box3.vertex.y, box.vertex.z + box3.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(0, 0, z1bound), dupchunk, naMakeVertex(x2bound, y2bound, 0), box3.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box4)){
      neworigin = naMakeVertex(box.vertex.x + box4.vertex.x, box.vertex.y + box4.vertex.y, box.vertex.z + box4.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(x1bound, y1bound, 0), dupchunk, naMakeVertex(0, 0, z2bound), box4.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box5)){
      neworigin = naMakeVertex(box.vertex.x + box5.vertex.x, box.vertex.y + box5.vertex.y, box.vertex.z + box5.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(0, y1bound, 0), dupchunk, naMakeVertex(x2bound, 0, z2bound), box5.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box6)){
      neworigin = naMakeVertex(box.vertex.x + box6.vertex.x, box.vertex.y + box6.vertex.y, box.vertex.z + box6.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(x1bound, 0, 0), dupchunk, naMakeVertex(0, y2bound, z2bound), box6.volume, shift, baseleafvolume);
    }
    if(!naIsBoxEmpty(box7)){
      neworigin = naMakeVertex(box.vertex.x + box7.vertex.x, box.vertex.y + box7.vertex.y, box.vertex.z + box7.vertex.z);
      naLocateOctTreeCoord(&newiter, neworigin);
      newdata = naGetOctTreeCurMutable(&newiter, NA_TRUE);
      duptree->configuration.datacopier(newdata, naMakeVertex(0, 0, 0), dupchunk, naMakeVertex(x2bound, y2bound, z2bound), box7.volume, shift, baseleafvolume);
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
