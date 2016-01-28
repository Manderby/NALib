
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAOctTree.h"
#include "NAMemory.h"



struct NAOctTreeNode{
  NAUInt childsize;
  NAInt segmentinparent;
  NAOctTreeNode* parentnode;
  void* child[8];
  NAVertexi childorigin[8];
  void* nodedata;
};



NA_HDEF NAOctTreeNode* naAllocOctTreeNode(NAUInt childsize, NAInt segmentinparent, NAOctTreeNode* parentnode, NAVertexi origin, NAOctTreeNodeAllocator allocator){
  NAOctTreeNode* node = naAlloc(NAOctTreeNode);
  node->childsize = childsize;
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
  node->childorigin[0] = naMakeVertexi(origin.x, origin.y, origin.z);
  node->childorigin[1] = naMakeVertexi(origin.x + node->childsize, origin.y, origin.z);
  node->childorigin[2] = naMakeVertexi(origin.x, origin.y + node->childsize, origin.z);
  node->childorigin[3] = naMakeVertexi(origin.x + node->childsize, origin.y + node->childsize, origin.z);
  node->childorigin[4] = naMakeVertexi(origin.x, origin.y, origin.z + node->childsize);
  node->childorigin[5] = naMakeVertexi(origin.x + node->childsize, origin.y, origin.z + node->childsize);
  node->childorigin[6] = naMakeVertexi(origin.x, origin.y + node->childsize, origin.z + node->childsize);
  node->childorigin[7] = naMakeVertexi(origin.x + node->childsize, origin.y + node->childsize, origin.z + node->childsize);
  if(allocator){
    node->nodedata = allocator(origin, childsize * 2);
  }else{
    node->nodedata = NA_NULL;
  }
  return node;
}



NA_HDEF void naComputeOctTreeAlignment(NAUInt childsize, NAVertexi childorigin, NAVertexi* parentorigin, NAInt* childsegment){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  NABoxi alignbox;
  NAVertexi alignorigin = childorigin;
  NAInt cycle = naLog2i(childsize) % 8;
  if(cycle & 1){alignorigin.x -= childsize;}
  if(cycle & 2){alignorigin.y -= childsize;}
  if(cycle & 4){alignorigin.z -= childsize;}
  alignbox = naMakeBoxi(alignorigin, naMakeVolumei(childsize * 2, childsize * 2, childsize * 2));
  *parentorigin = naMakeVertexiWithAlignment(childorigin, alignbox);
  *childsegment = 0;
  if(parentorigin->x != childorigin.x){*childsegment |= 1;}
  if(parentorigin->y != childorigin.y){*childsegment |= 2;}
  if(parentorigin->z != childorigin.z){*childsegment |= 4;}
}




NA_HDEF void naCreateOctTreeNodeChildData(const void* childdata[8], NAOctTreeNode* node){
  childdata[0] = (node->child[0]) ? (((NAOctTreeNode*)(node->child[0]))->nodedata) : NA_NULL;
  childdata[1] = (node->child[1]) ? (((NAOctTreeNode*)(node->child[1]))->nodedata) : NA_NULL;
  childdata[2] = (node->child[2]) ? (((NAOctTreeNode*)(node->child[2]))->nodedata) : NA_NULL;
  childdata[3] = (node->child[3]) ? (((NAOctTreeNode*)(node->child[3]))->nodedata) : NA_NULL;
  childdata[4] = (node->child[4]) ? (((NAOctTreeNode*)(node->child[4]))->nodedata) : NA_NULL;
  childdata[5] = (node->child[5]) ? (((NAOctTreeNode*)(node->child[5]))->nodedata) : NA_NULL;
  childdata[6] = (node->child[6]) ? (((NAOctTreeNode*)(node->child[6]))->nodedata) : NA_NULL;
  childdata[7] = (node->child[7]) ? (((NAOctTreeNode*)(node->child[7]))->nodedata) : NA_NULL;
}




// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAOctTreeNode* naAddOctTreeChildNode(NAOctTree* tree, NAOctTreeNode* parentnode, NAInt segment){

  NAOctTreeNode* node = naAllocOctTreeNode(
          parentnode->childsize / 2,
          segment,
          parentnode,
          parentnode->childorigin[segment],
          tree->callbacks.nodeallocator);
  
  parentnode->child[segment] = node;
  return node;
}



// Allocates and initializes a new node which becomes the parent of an existing
// node. This function is called very seldomly.
NA_HDEF NAOctTreeNode* naAddOctTreeNodeParent(NAOctTree* tree, NAOctTreeNode* childnode){

  NAInt childsegment;
  NAVertexi parentorigin;
  NAOctTreeNode* node;
  naComputeOctTreeAlignment(childnode->childsize * 2, childnode->childorigin[0], &parentorigin, &childsegment);
  
  node = naAllocOctTreeNode(
          childnode->childsize * 2,
          -1,
          NA_NULL,
          parentorigin,
          tree->callbacks.nodeallocator);
  
  node->child[childsegment] = childnode;
  childnode->parentnode = node;
  childnode->segmentinparent = childsegment;
  if(tree->callbacks.childchanged){
    const void* childdata[8];
    naCreateOctTreeNodeChildData(childdata, node);
    tree->callbacks.childchanged(node->nodedata, childsegment, childdata);
  }
  return node;
}



// Creates a duplicate of the given oldnode.
// perentnode is the parent in the newtree
NA_HDEF void naCopyOctTreeNode(NAOctTree* newtree, NAOctTreeNode* parentnode, NAInt segment, const NAOctTreeNode* oldnode, const NAOctTree* contenttree){

  NAOctTreeNode* node;

  if(parentnode){
    node = naAddOctTreeChildNode(newtree, parentnode, segment);
  }else{
    node = naAllocOctTreeNode(
          oldnode->childsize,
          -1,
          NA_NULL,
          oldnode->childorigin[0],
          newtree->callbacks.nodeallocator);
    newtree->root = node;
  }
  
  if(node->childsize == newtree->leafsize){
    if(contenttree){
      // Note that in contrast to the else case, we create an empty node if
      // there is no content available.
      if(oldnode->child[0]){node->child[0] = newtree->callbacks.leafallocator(node->childorigin[0], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[0], NA_NULL));}
      if(oldnode->child[1]){node->child[1] = newtree->callbacks.leafallocator(node->childorigin[1], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[1], NA_NULL));}
      if(oldnode->child[2]){node->child[2] = newtree->callbacks.leafallocator(node->childorigin[2], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[2], NA_NULL));}
      if(oldnode->child[3]){node->child[3] = newtree->callbacks.leafallocator(node->childorigin[3], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[3], NA_NULL));}
      if(oldnode->child[4]){node->child[4] = newtree->callbacks.leafallocator(node->childorigin[4], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[4], NA_NULL));}
      if(oldnode->child[5]){node->child[5] = newtree->callbacks.leafallocator(node->childorigin[5], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[5], NA_NULL));}
      if(oldnode->child[6]){node->child[6] = newtree->callbacks.leafallocator(node->childorigin[6], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[6], NA_NULL));}
      if(oldnode->child[7]){node->child[7] = newtree->callbacks.leafallocator(node->childorigin[7], newtree->leafsize, newtree->callbacks.userdata, naGetOctTreeLeafConst(contenttree, node->childorigin[7], NA_NULL));}
    }else{
      if(oldnode->child[0]){node->child[0] = newtree->callbacks.leafallocator(node->childorigin[0], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[0]);}
      if(oldnode->child[1]){node->child[1] = newtree->callbacks.leafallocator(node->childorigin[1], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[1]);}
      if(oldnode->child[2]){node->child[2] = newtree->callbacks.leafallocator(node->childorigin[2], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[2]);}
      if(oldnode->child[3]){node->child[3] = newtree->callbacks.leafallocator(node->childorigin[3], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[3]);}
      if(oldnode->child[4]){node->child[4] = newtree->callbacks.leafallocator(node->childorigin[4], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[4]);}
      if(oldnode->child[5]){node->child[5] = newtree->callbacks.leafallocator(node->childorigin[5], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[5]);}
      if(oldnode->child[6]){node->child[6] = newtree->callbacks.leafallocator(node->childorigin[6], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[6]);}
      if(oldnode->child[7]){node->child[7] = newtree->callbacks.leafallocator(node->childorigin[7], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[7]);}
    }
    if(newtree->callbacks.leafchanged){newtree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);}
  }else{
    const void* childdata[8];
    if(oldnode->child[0]){naCopyOctTreeNode(newtree, node, 0, oldnode->child[0], contenttree);}
    if(oldnode->child[1]){naCopyOctTreeNode(newtree, node, 1, oldnode->child[1], contenttree);}
    if(oldnode->child[2]){naCopyOctTreeNode(newtree, node, 2, oldnode->child[2], contenttree);}
    if(oldnode->child[3]){naCopyOctTreeNode(newtree, node, 3, oldnode->child[3], contenttree);}
    if(oldnode->child[4]){naCopyOctTreeNode(newtree, node, 4, oldnode->child[4], contenttree);}
    if(oldnode->child[5]){naCopyOctTreeNode(newtree, node, 5, oldnode->child[5], contenttree);}
    if(oldnode->child[6]){naCopyOctTreeNode(newtree, node, 6, oldnode->child[6], contenttree);}
    if(oldnode->child[7]){naCopyOctTreeNode(newtree, node, 7, oldnode->child[7], contenttree);}
    naCreateOctTreeNodeChildData(childdata, node);
    if(newtree->callbacks.childchanged){newtree->callbacks.childchanged(node->nodedata, -1, childdata);}
  }

}



// Deallocates one node.
NA_HDEF void naDeallocOctTreeNode(NAOctTree* tree, NAOctTreeNode* node){
  if(node->childsize == tree->leafsize){
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

  if(node->nodedata && tree->callbacks.nodedestructor){
    tree->callbacks.nodedestructor(node->nodedata);
  }
  free(node);
}



// Returns the segment index [0-7] in which the given vertex can be found.
// Warning: The vertex is expected to be inside this node.
NA_HDEF NAUInt naGetOctTreeNodeSegment(NAOctTreeNode* node, NAVertexi coord){
  NAUInt segment = 0;
  if(coord.x >= node->childorigin[7].x){segment |= 1;}
  if(coord.y >= node->childorigin[7].y){segment |= 2;}
  if(coord.z >= node->childorigin[7].z){segment |= 4;}
  #ifndef NDEBUG
    if(!naIsVertexiInBoxi(coord, naMakeBoxi(node->childorigin[segment], naMakeVolumei(node->childsize, node->childsize, node->childsize))))
      naError("naGetOctTreeNodeSegment", "node does not actually contains vertex");
  #endif
  return segment;
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF void naUpdateOctTreeNodeBubbling(NAOctTree* tree, NAOctTreeNode* parentnode, NAInt segment){
  NABool bubble = NA_TRUE;
  
  // If we have a leaf parent, we deal with it first.
  if(parentnode->childsize == tree->leafsize){
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(parentnode->nodedata, segment, (const void* const) parentnode->child);
    }
    if(parentnode->parentnode){
      #ifndef NDEBUG
        if(parentnode->segmentinparent == -1)
          naError("naUpdateOctTreeNode", "Inernal inconsistency detected");
      #endif
      segment = parentnode->segmentinparent;
    }
    parentnode = parentnode->parentnode;
  }
  
  // Then we loop over the height of the tree as long as we have a parent node
  // and as long as we can bubble.
  while(parentnode && bubble){
    if(tree->callbacks.childchanged){
      const void* childdata[8];
      naCreateOctTreeNodeChildData(childdata, parentnode);
      bubble = tree->callbacks.childchanged(parentnode->nodedata, segment, childdata);
    }
    if(parentnode->parentnode){
      #ifndef NDEBUG
        if(parentnode->segmentinparent == -1)
          naError("naUpdateOctTreeNode", "Inernal inconsistency detected");
      #endif
      segment = parentnode->segmentinparent;
    }
    parentnode = parentnode->parentnode;
  }
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF NABool naUpdateOctTreeNodeCapturing(NAOctTree* tree, NAOctTreeNode* node){
  NABool bubble = NA_FALSE;
  
  if(node->childsize == tree->leafsize){
    // This node stores leafs
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);
    }
  }else{
    // this node stores subnodes
    if(tree->callbacks.childchanged){
      if(node->child[0]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[0]);}
      if(node->child[1]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[1]);}
      if(node->child[2]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[2]);}
      if(node->child[3]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[3]);}
      if(node->child[4]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[4]);}
      if(node->child[5]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[5]);}
      if(node->child[6]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[6]);}
      if(node->child[7]){bubble |= naUpdateOctTreeNodeCapturing(tree, node->child[7]);}
    }
    if(bubble){
      const void* childdata[8];
      naCreateOctTreeNodeChildData(childdata, node);
      bubble = tree->callbacks.childchanged(node->nodedata, -1, childdata);
    }
  }
  
  return bubble;
}



NA_DEF NAOctTree* naInitOctTree(NAOctTree* tree, NAUInt leafsize, NAOctTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naInitOctTree", "tree is NULL"); return NA_NULL;}
    if(!callbacks.leafallocator)
      naError("naInitOctTree", "Must have a data allocator");
    if(!callbacks.leafdeallocator)
      naError("naInitOctTree", "Must have a data destructor");
    if(leafsize < 1)
      naError("naInitOctTree", "leafsize can not be smaller than 1");
  #endif
  tree->leafsize = leafsize;
  tree->callbacks = callbacks;
  tree->root = NA_NULL;
  tree->visitnode = NA_NULL;
  tree->curnode = NA_NULL;
  tree->cursegment = 0;
  return tree;
}




NA_DEF NAOctTree* naCopyOctTree(NAOctTree* newtree, const NAOctTree* duptree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyOctTree", "newtree is NULL"); return NA_NULL;}
  #endif
  newtree = naInitOctTree(newtree, duptree->leafsize, duptree->callbacks);
  if(duptree->root){
    // Copy the root node
    // consequently, all other nodes and all leafs will be created.
    naCopyOctTreeNode(newtree, NA_NULL, -1, duptree->root, NA_NULL);
  }
  return newtree;
}



NA_DEF NAOctTree* naCopyOctTreeWithMaskTree(NAOctTree* newtree, const NAOctTree* duptree, const NAOctTree* masktree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyOctTreeWithMaskTree", "newtree is NULL"); return NA_NULL;}
  #endif
  newtree = naInitOctTree(newtree, duptree->leafsize, duptree->callbacks);
  if(masktree->root){
    // Copy the root node
    // consequently, all other nodes and all leafs will be created.
    naCopyOctTreeNode(newtree, NA_NULL, -1, masktree->root, duptree);
  }
  return newtree;
}



NA_DEF NAOctTree* naCopyOctTreeWithShift(NAOctTree* newtree, const NAOctTree* duptree, NAVolumei shift){
  NAUInt x1bound;
  NAUInt y1bound;
  NAUInt z1bound;
  NAUInt x2bound;
  NAUInt y2bound;
  NAUInt z2bound;
  NABoxi box0;
  NABoxi box1;
  NABoxi box2;
  NABoxi box3;
  NABoxi box4;
  NABoxi box5;
  NABoxi box6;
  NABoxi box7;
  const void* dupchunk;
  NAVertexi origin;

  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyOctTreeWithShift", "newtree is NULL"); return NA_NULL;}
    if(!duptree->callbacks.datacopier)
      naError("naCopyOctTreeWithShift", "Data copier callback required.");
  #endif
  newtree = naInitOctTree(newtree, duptree->leafsize, duptree->callbacks);
  if(!duptree->root){return newtree;}

  x1bound = ((shift.width % duptree->leafsize) + duptree->leafsize ) % duptree->leafsize;
  y1bound = ((shift.height % duptree->leafsize) + duptree->leafsize ) % duptree->leafsize;
  z1bound = ((shift.depth % duptree->leafsize) + duptree->leafsize ) % duptree->leafsize;
  x2bound = duptree->leafsize - x1bound;
  y2bound = duptree->leafsize - y1bound;
  z2bound = duptree->leafsize - z1bound;
  box0 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height, shift.depth), naMakeVolumeiE(x2bound, y2bound, z2bound));
  box1 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height, shift.depth), naMakeVolumeiE(x1bound, y2bound, z2bound));
  box2 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height + y2bound, shift.depth), naMakeVolumeiE(x2bound, y1bound, z2bound));
  box3 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height + y2bound, shift.depth), naMakeVolumeiE(x1bound, y1bound, z2bound));
  box4 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height, shift.depth + z2bound), naMakeVolumeiE(x2bound, y2bound, z1bound));
  box5 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height, shift.depth + z2bound), naMakeVolumeiE(x1bound, y2bound, z1bound));
  box6 = naMakeBoxiE(naMakeVertexi(shift.width, shift.height + y2bound, shift.depth + z2bound), naMakeVolumeiE(x2bound, y1bound, z1bound));
  box7 = naMakeBoxiE(naMakeVertexi(shift.width + x2bound, shift.height + y2bound, shift.depth + z2bound), naMakeVolumeiE(x1bound, y1bound, z1bound));
  
  naFirstOctTree(duptree);
  while((dupchunk = naIterateOctTreeConst(duptree, &origin))){
    NAVertexi neworigin;
    void* newdata;
    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 8 new leaves.
    if(!naIsBoxiEmpty(box0)){
      neworigin = naMakeVertexi(origin.x + box0.vertex.x, origin.y + box0.vertex.y, origin.z + box0.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, y1bound, z1bound), dupchunk, naMakeVertexi(0, 0, 0), box0.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box1)){
      neworigin = naMakeVertexi(origin.x + box1.vertex.x, origin.y + box1.vertex.y, origin.z + box1.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, y1bound, z1bound), dupchunk, naMakeVertexi(x2bound, 0, 0), box1.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box2)){
      neworigin = naMakeVertexi(origin.x + box2.vertex.x, origin.y + box2.vertex.y, origin.z + box2.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, 0, z1bound), dupchunk, naMakeVertexi(0, y2bound, 0), box2.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box3)){
      neworigin = naMakeVertexi(origin.x + box3.vertex.x, origin.y + box3.vertex.y, origin.z + box3.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, 0, z1bound), dupchunk, naMakeVertexi(x2bound, y2bound, 0), box3.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box4)){
      neworigin = naMakeVertexi(origin.x + box4.vertex.x, origin.y + box4.vertex.y, origin.z + box4.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, y1bound, 0), dupchunk, naMakeVertexi(0, 0, z2bound), box4.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box5)){
      neworigin = naMakeVertexi(origin.x + box5.vertex.x, origin.y + box5.vertex.y, origin.z + box5.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, y1bound, 0), dupchunk, naMakeVertexi(x2bound, 0, z2bound), box5.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box6)){
      neworigin = naMakeVertexi(origin.x + box6.vertex.x, origin.y + box6.vertex.y, origin.z + box6.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(x1bound, 0, 0), dupchunk, naMakeVertexi(0, y2bound, z2bound), box6.volume, duptree->leafsize);
    }
    if(!naIsBoxiEmpty(box7)){
      neworigin = naMakeVertexi(origin.x + box7.vertex.x, origin.y + box7.vertex.y, origin.z + box7.vertex.z);
      newdata = naGetOctTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakeVertexi(0, 0, 0), dupchunk, naMakeVertexi(x2bound, y2bound, z2bound), box7.volume, duptree->leafsize);
    }
  }
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateOctTree(newtree);
  return newtree;
}




// Returns the node containing the leaf containing coord. Stores the segment
// number in childsegment;
NA_HDEF NAOctTreeNode* naLocateOctTreeNode(NAOctTree* tree, NAOctTreeNode* node, NAVertexi coord, NABool create, const void* serialdata, NABool* didcreate, NAInt* leafsegment){
  NABoxi nodebox = naMakeBoxi(node->childorigin[0], naMakeVolumei(node->childsize * 2, node->childsize * 2, node->childsize * 2));
  if(naIsVertexiInBoxi(coord, nodebox)){
  
    // The coord is stored somewhere inside the boxangle of this node
    NAUInt segment = naGetOctTreeNodeSegment(node, coord);
    if(node->child[segment]){
      // There is a segment available which contains the coord.
      if(node->childsize == tree->leafsize){
        // We are at leaf-level. We found it!
        #ifndef NDEBUG
          if(serialdata)
            naError("naLocateOctTreeNode", "Leaf already exists when trying to deserialize");
        #endif
        tree->visitnode = node;
        *leafsegment = segment;
        if(didcreate){*didcreate = NA_FALSE;}
        return node;
      }else{
        // Go on searching in the sub-node
        return naLocateOctTreeNode(tree, node->child[segment], coord, create, serialdata, didcreate, leafsegment);
      }
    }else{
      // There is no segment where the coord should be. Let's create one if
      // desired.
      if(create){
        if(node->childsize == tree->leafsize){
          // The node can not be divided anymore. Create a leaf.
          if(serialdata){
            node->child[segment] = tree->callbacks.deserialize(node->childorigin[segment], tree->leafsize, serialdata);
          }else{
            node->child[segment] = tree->callbacks.leafallocator(node->childorigin[segment], tree->leafsize, tree->callbacks.userdata, NA_NULL);
          }
          #ifndef NDEBUG
            if(!node->child[segment])
              naError("naLocateOctTreeNode", "Allocator returned null.");
          #endif
          *leafsegment = segment;
          if(didcreate){*didcreate = NA_TRUE;}
          return node;
        }else{
          // The node can be further subdivided.
          NAOctTreeNode* subnode = naAddOctTreeChildNode(tree, node, segment);
          // Go on searching in the sub node. Note that when the sub just
          // had been created, NA_FALSE must be returned. Therefore we ignore
          // the return value of this function and return NA_FALSE later.
          return naLocateOctTreeNode(tree, subnode, coord, create, serialdata, didcreate, leafsegment);
        }
      }else{
        if(didcreate){*didcreate = NA_FALSE;}
        return NA_NULL;
      }
    }

  }else{
  
    // The coord is not stored within the box of this node.
    if(node->parentnode){
      // We have a parent. Search there.
      return naLocateOctTreeNode(tree, node->parentnode, coord, create, serialdata, didcreate, leafsegment);
    }else{
      // There is no parent
      #ifndef NDEBUG
        if(tree->root != node)
          naError("naLocateOctTreeNode", "node seems not to be part of the tree.");
      #endif
      // If there is no parent, we create a new super-node which becomes the
      // parent of this node as well as the new root for the whole tree.
      if(create){
        tree->root = naAddOctTreeNodeParent(tree, node);
        // Go on searching in the parent node. Note that when the parent just
        // had been created, NA_FALSE must be returned. Therefore, we ignore
        // the function result and return NA_FALSE later
        return naLocateOctTreeNode(tree, node->parentnode, coord, create, serialdata, didcreate, leafsegment);
      }else{
        if(didcreate){*didcreate = NA_FALSE;}
        return NA_NULL;
      }
    }
  }
}




NA_HDEF NAOctTreeNode* naLocateOctTreeLeafParent(NAOctTree* tree, NAVertexi coord, NABool create, const void* serialdata, NABool* didcreate, NAInt* leafsegment){
  NAOctTreeNode* node;

  if(tree->visitnode){
    // We start looking at the visit node.
    return naLocateOctTreeNode(tree, tree->visitnode, coord, create, serialdata, didcreate, leafsegment);
  }else if(tree->curnode){
    // When no visit node is available, maybe the tree is currently iterating.
    return naLocateOctTreeNode(tree, tree->curnode, coord, create, serialdata, didcreate, leafsegment);
  }else if(tree->root){
    // If the current node is not set, we start at the root.
    return naLocateOctTreeNode(tree, tree->root, coord, create, serialdata, didcreate, leafsegment);
  }else if(create){
    // There is no visitnode, no curnode and no root. We create the first node
    // for this tree including the first leaf.
    NABoxi leafalign = naMakeBoxi(naMakeVertexi(0, 0, 0), naMakeVolumei(tree->leafsize, tree->leafsize, tree->leafsize));
    NAVertexi leaforigin = naMakeVertexiWithAlignment(coord, leafalign);
    NAVertexi rootorigin;
    naComputeOctTreeAlignment(tree->leafsize, leaforigin, &rootorigin, leafsegment);

    node = naAllocOctTreeNode(
            tree->leafsize,
            -1,
            NA_NULL,
            rootorigin,
            tree->callbacks.nodeallocator);
    
    if(serialdata){
      node->child[*leafsegment] = tree->callbacks.deserialize(leaforigin, tree->leafsize, serialdata);
    }else{
      node->child[*leafsegment] = tree->callbacks.leafallocator(leaforigin, tree->leafsize, tree->callbacks.userdata, NA_NULL);
    }
    tree->root = node;

    if(didcreate){*didcreate = NA_TRUE;}
    return tree->root;
  }else{
    if(didcreate){*didcreate = NA_FALSE;}
    return NA_FALSE;
  }
}



NA_DEF NAOctTree* naInitOctTreeWithDeserialization(NAOctTree* tree, const void* buf, NAOctTreeCallbacks callbacks){
  const NAByte* dataptr;
  uint64 datasize;
  uint64 leafsize;
  NAInt leafsegment;
  NAOctTreeNode* node;
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naCopyOctTreeWithShift", "tree is NULL"); return NA_NULL;}
    if(!(callbacks.deserialize)){
      naCrash("naInitOctTreeWithDeserialization", "Callbacks required for deserialization");
      return NA_NULL;
    }
  #endif
  dataptr = buf;
  datasize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  leafsize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  tree = naInitOctTree(tree, (NAInt)leafsize, callbacks);
  datasize -= 2 * sizeof(uint64);
  while(datasize){
    int64 posx, posy, posz;
    uint64 bytesread;
    posx = *((const int64*)dataptr); dataptr += sizeof(int64);
    posy = *((const int64*)dataptr); dataptr += sizeof(int64);
    posz = *((const int64*)dataptr); dataptr += sizeof(int64);
    datasize -= 3 * sizeof(int64);
    // Create the leaf temporarily but delete it immediately and recreate it
    // as a deserialization.
    node = naLocateOctTreeLeafParent(tree, naMakeVertexi((NAInt)posx, (NAInt)posy, (NAInt)posz), NA_TRUE, dataptr, NA_NULL, &leafsegment);
    bytesread = callbacks.serialize(NA_NULL, node->child[leafsegment], tree->leafsize);
    dataptr += bytesread;
    datasize -= bytesread;
  }
  naUpdateOctTree(tree);
  return tree;
}



NA_DEF void naEmptyOctTree(NAOctTree* tree){
  naClearOctTree(tree);
  tree->root = NA_NULL;
  tree->curnode = NA_NULL;
  tree->visitnode = NA_NULL;
}



NA_DEF void naClearOctTree(NAOctTree* tree){
  if(tree->root){naDeallocOctTreeNode(tree, tree->root);}
}



NA_DEF void naSerializeOctTree(const NAOctTree* tree, void* buf, uint64* bytesize){
  const void* curleaf;
  #ifndef NDEBUG
    if(!(tree->callbacks.serialize))
      naError("naSerializeOctTree", "Callbacks required for serialization");
    if(!bytesize){
      naCrash("naSerializeOctTree", "bytesize required to read/store byte size");
      return;
    }
    if(buf && (*bytesize == 0))
      naError("naSerializeOctTree", "bytesize is zero");
  #endif

  naFirstOctTree(tree);
  if(!buf){
    *bytesize = sizeof(uint64) * 2; // datasize and leafsize
    while((curleaf = naIterateOctTreeConst(tree, NA_NULL))){
      *bytesize += sizeof(int64) * 3; // the origin.x, origin.y and origin.z
      *bytesize += tree->callbacks.serialize(NA_NULL, curleaf, tree->leafsize);
    }
  }else{
    NAByte* dataptr = buf;
    NAVertexi origin;
    *((uint64*)dataptr) = *bytesize; dataptr += sizeof(uint64);
    *((uint64*)dataptr) = tree->leafsize; dataptr += sizeof(uint64);

    while((curleaf = naIterateOctTreeConst(tree, &origin))){
      *((int64*)dataptr) = origin.x; dataptr += sizeof(int64);
      *((int64*)dataptr) = origin.y; dataptr += sizeof(int64);
      *((int64*)dataptr) = origin.z; dataptr += sizeof(int64);
      dataptr += tree->callbacks.serialize(dataptr, curleaf, tree->leafsize);
    }
  }

}



NA_DEF NABool naIsOctTreeEmpty(NAOctTree* tree){
  return (tree->root == NA_NULL);
}



NA_DEF uint64 naGetOctTreeLeafSize(const NAOctTree* tree){
  return tree->leafsize;
}



NA_DEF NAOctTreeCallbacks naGetOctTreeCallbacks(const NAOctTree* tree){
  return tree->callbacks;
}




NA_HDEF const void* naGetOctTreeLeafConst(const NAOctTree* tree, NAVertexi coord, NAVertexi* origin){
  NAOctTreeNode* node;
  NAInt leafsegment;
  node = naLocateOctTreeLeafParent((NAOctTree*)tree, coord, NA_FALSE, NA_NULL, NA_NULL, &leafsegment);
  if(node){
    if(origin){*origin = node->childorigin[leafsegment];}
    return node->child[leafsegment];
  }else{
    return NA_NULL;
  }
}



NA_HDEF void* naGetOctTreeLeafMutable(NAOctTree* tree, NAVertexi coord, NABool create, NABool* didcreate, NAVertexi* origin){
  NAOctTreeNode* node;
  NAInt leafsegment;
  node = naLocateOctTreeLeafParent(tree, coord, create, NA_NULL, didcreate, &leafsegment);
  if(node){
    if(origin){*origin = node->childorigin[leafsegment];}
    return node->child[leafsegment];
  }else{
    return NA_NULL;
  }
}



// The segment number -1 means that the iteration goes downwards.
NA_HDEF void naIterateOctTreeNode(NAOctTree* tree, NAOctTreeNode* node, NAInt segment){
  // We go to the next segment. When we came to this function from above, we
  // start with the first segment.
  segment++;
  // Search for a segment which is available.
  while((segment < 8) && (node->child[segment] == NA_NULL)){segment++;}
  if(segment < 8){
    // If there is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(node->childsize == tree->leafsize){
      tree->curnode = node;
      tree->cursegment = segment;
    }else{
      naIterateOctTreeNode(tree, node->child[segment], -1);
    }
  }else{
    // If there is no more segment available go upwards.
    if(node->parentnode){
      #ifndef NDEBUG
        if(node->segmentinparent == -1)
          naError("naIterateOctTreeNode", "Inernal inconsistency detected");
      #endif
      naIterateOctTreeNode(tree, node->parentnode, node->segmentinparent);
    }else{
      // no more searching. The iteration is over. We are at the end.
      tree->curnode = NA_NULL;
    }
  }
}



NA_DEF void naFirstOctTree(const NAOctTree* tree){
  if(!tree->root){return;}
  naIterateOctTreeNode((NAOctTree*)tree, tree->root, -1);
}



NA_DEF const void* naIterateOctTreeConst(const NAOctTree* tree, NAVertexi* origin){
  const void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateOctTreeNode((NAOctTree*)tree, tree->curnode, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



NA_DEF void* naIterateOctTreeMutable(NAOctTree* tree, NAVertexi* origin){
  void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateOctTreeNode(tree, tree->curnode, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



// The segment number -1 means that the iteration goes downwards.
NA_HDEF void naIterateOctTreeNodeInBox(NAOctTree* tree, NAOctTreeNode* node, NABoxi* box, NAInt segment){
  // We go to the next segment. When we came to this function from above, we
  // start with the first segment.
  segment++;
  // Search for a segment which is available.
  while(segment < 8){
    if(node->child[segment] != NA_NULL){
      NABoxi childbox = naMakeBoxi(node->childorigin[segment], naMakeVolumei(node->childsize, node->childsize, node->childsize));
      if(naIsBoxiUseful(naMakeBoxiWithBoxiIntersection(childbox, *box))){break;}
    }
    segment++;
  }
  if(segment < 8){
    // If there is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(node->childsize == tree->leafsize){
      tree->curnode = node;
      tree->cursegment = segment;
    }else{
      naIterateOctTreeNodeInBox(tree, node->child[segment], box, -1);
    }
  }else{
    // If there is no more segment available go upwards.
    if(node->parentnode){
      #ifndef NDEBUG
        if(node->segmentinparent == -1)
          naError("naIterateOctTreeNodeInBox", "Inernal inconsistency detected");
      #endif
      naIterateOctTreeNodeInBox(tree, node->parentnode, box, node->segmentinparent);
    }else{
      // no more searching. The iteration is over. We are at the end.
      tree->curnode = NA_NULL;
    }
  }
}



NA_DEF void naFirstOctTreeInBox(const NAOctTree* tree, NABoxi box){
  if(!tree->root){return;}
  naIterateOctTreeNodeInBox((NAOctTree*)tree, tree->root, &box, -1);
}



NA_DEF const void* naIterateOctTreeInBoxConst(const NAOctTree* tree, NAVertexi* origin, NABoxi box){
  const void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateOctTreeNodeInBox((NAOctTree*)tree, tree->curnode, &box, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



NA_DEF void* naIterateOctTreeInBoxMutable(NAOctTree* tree, NAVertexi* origin, NABoxi box){
  void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateOctTreeNodeInBox(tree, tree->curnode, &box, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



NA_DEF void naSetOctTreeInBox(NAOctTree* tree, NABoxi box, NAOctTreeDataSetter datasetter, const void* userdata){
  NABoxi chunkbox;
  NAVertexi start;
  NAVertexi end;
  chunkbox = naMakeBoxi(naMakeVertexi(0, 0, 0), naMakeVolumei(tree->leafsize, tree->leafsize, tree->leafsize));

  start = naMakeVertexiWithAlignment(box.vertex, chunkbox);
  end = naGetBoxiEnd(box);
  
  for(chunkbox.vertex.z = start.z; chunkbox.vertex.z < end.z; chunkbox.vertex.z += tree->leafsize){
    for(chunkbox.vertex.y = start.y; chunkbox.vertex.y < end.y; chunkbox.vertex.y += tree->leafsize){
      for(chunkbox.vertex.x = start.x; chunkbox.vertex.x < end.x; chunkbox.vertex.x += tree->leafsize){
        void* curchunk = naGetOctTreeLeafMutable(tree, chunkbox.vertex, NA_TRUE, NA_NULL, NA_NULL);
        NABoxi clampbox = naClampBoxiToBoxi(box, chunkbox);
        clampbox.vertex.x -= chunkbox.vertex.x;
        clampbox.vertex.y -= chunkbox.vertex.y;
        clampbox.vertex.z -= chunkbox.vertex.z;
        datasetter(curchunk, clampbox, tree->leafsize, userdata);
      }
    }
  }

}



// Expects the node to be the node to remove.
NA_HDEF void naRemoveOctTreeNode(NAOctTree* tree, NAOctTreeNode* node){
  if(node->parentnode){
  
    // Remove the segment from the tree
    #ifndef NDEBUG
      if(node->segmentinparent == -1)
        naError("naRemoveOctTreeNode", "Inernal inconsistency detected");
    #endif
    node->parentnode->child[node->segmentinparent] = NA_NULL;
    
    // Test if there are other childs...
    if(node->parentnode->child[0] || node->parentnode->child[1] || node->parentnode->child[2] || node->parentnode->child[3] || node->parentnode->child[4] || node->parentnode->child[5] || node->parentnode->child[6] || node->parentnode->child[7]){
      // If there is at least one more child, we update the parent node.
      naUpdateOctTreeNodeBubbling(tree, node->parentnode, node->segmentinparent);
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



// This methods checks if the root of the tree has only one child. As long
// as there is only one child in the root, the root is transferred to that
// very child to save memory and allow garbage collectors to collect.
NA_HDEF void naAdjustOctTreeRoot(NAOctTree* tree){
  while(1){
    if(tree->root->childsize == tree->leafsize){
      // The root node has only leafes as childs.
      return;
    }else{
      // We count how many children there are.
      NAUInt i;
      NAUInt childcount = 0;
      NAUInt lastchildindex = 0;
      NAOctTreeNode* lastchild;
      for(i=0; i<8; i++){if(tree->root->child[i]){childcount++; lastchildindex = i;}}
      #ifndef NDEBUG
        if(!childcount)
          naError("naAdjustOctTreeRoot", "Root should not be empty.");
      #endif
      // If there are more than 1 child, return.
      if(childcount > 1){return;}
      // Now, the root has only one child node which now is stored in lastchild.
      // We adjust the child to become the new root.
      lastchild = tree->root->child[lastchildindex];
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



NA_DEF void naRemoveOctTreeLeaf(NAOctTree* tree, NAVertexi coord){
  NAOctTreeNode* node;
  NAInt leafsegment;
  
  node = naLocateOctTreeLeafParent(tree, coord, NA_FALSE, NA_NULL, NA_FALSE, &leafsegment);
  if(!node){return;}
  
  #ifndef NDEBUG
    if(node == tree->curnode)
      naError("naRemoveOctTreeLeaf", "Removing the node currently iterating on");
  #endif
  
  // First, we destroy the data of the leaf chunk
  tree->callbacks.leafdeallocator(node->child[leafsegment], tree->callbacks.userdata);
  node->child[leafsegment] = NA_NULL;

  // Test if there are other childs in the current parent.
  if(node->child[0] || node->child[1] || node->child[2] || node->child[3] || node->child[4] || node->child[5] || node->child[6] || node->child[7]){
    // If there is at least one more child, we update the node.
    naUpdateOctTreeNodeBubbling(tree, node, leafsegment);
  }else{
    // If there are no more childs, we remove the node.
    naRemoveOctTreeNode(tree, node);
  }
  if(tree->root){
    // Now, if the root has only one child left, we adjust the tree accordingly.
    naAdjustOctTreeRoot(tree);
  }
  // Removing always nullifies the last visit.
  tree->visitnode = NA_NULL;
}



NA_DEF void naUpdateOctTreeAtCoord(NAOctTree* tree, NAVertexi coord){
  NAInt leafsegment;
  NAOctTreeNode* node = naLocateOctTreeLeafParent(tree, coord, NA_FALSE, NA_NULL, NA_FALSE, &leafsegment);
  if(node){
    naUpdateOctTreeNodeBubbling(tree, node, leafsegment);
  }
}



NA_DEF void naUpdateOctTree(NAOctTree* tree){
  if(tree->root){
    naUpdateOctTreeNodeCapturing(tree, tree->root);
  }
}



NA_DEF void* naGetOctTreeRootNodeData(NAOctTree* tree){
  if(tree->root){return tree->root->nodedata;}
  return NA_NULL;
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
