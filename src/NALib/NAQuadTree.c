
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAQuadTree.h"
#include "NAMemory.h"



struct NAQuadTreeNode{
  NAUInt childsize;
  NAInt segmentinparent;
  NAQuadTreeNode* parentnode;
  void* child[4];
  NAPosi childorigin[4];
  void* nodedata;
};



NA_HDEF NAQuadTreeNode* naAllocQuadTreeNode(NAUInt childsize, NAInt segmentinparent, NAQuadTreeNode* parentnode, NAPosi origin, NAQuadTreeNodeAllocator allocator){
  NAQuadTreeNode* node = naAlloc(NAQuadTreeNode);
  node->childsize = childsize;
  node->segmentinparent = segmentinparent;
  node->parentnode = parentnode;
  node->child[0] = NA_NULL;
  node->child[1] = NA_NULL;
  node->child[2] = NA_NULL;
  node->child[3] = NA_NULL;
  node->childorigin[0] = naMakePosi(origin.x, origin.y);
  node->childorigin[1] = naMakePosi(origin.x + node->childsize, origin.y);
  node->childorigin[2] = naMakePosi(origin.x, origin.y + node->childsize);
  node->childorigin[3] = naMakePosi(origin.x + node->childsize, origin.y + node->childsize);
  if(allocator){
    node->nodedata = allocator(origin, childsize * 2);
  }else{
    node->nodedata = NA_NULL;
  }
  return node;
}



NA_HDEF void naComputeQuadTreeAlignment(NAUInt childsize, NAPosi childorigin, NAPosi* parentorigin, NAInt* childsegment){
  // In order to achieve a full coverage of the whole space
  // (negative and positive in all dimensions), we align parent nodes
  // in a cyclic way.
  NARecti alignrect;
  NAPosi alignorigin = childorigin;
  NAInt cycle = naLog2i(childsize) % 4;
  if(cycle & 1){alignorigin.x -= childsize;}
  if(cycle & 2){alignorigin.y -= childsize;}
  alignrect = naMakeRecti(alignorigin, naMakeSizei(childsize * 2, childsize * 2));
  *parentorigin = naMakePosiWithAlignment(childorigin, alignrect);
  *childsegment = 0;
  if(parentorigin->x != childorigin.x){*childsegment |= 1;}
  if(parentorigin->y != childorigin.y){*childsegment |= 2;}
}




NA_HDEF void naCreateQuadTreeNodeChildData(const void* childdata[4], NAQuadTreeNode* node){
  childdata[0] = (node->child[0]) ? (((NAQuadTreeNode*)(node->child[0]))->nodedata) : NA_NULL;
  childdata[1] = (node->child[1]) ? (((NAQuadTreeNode*)(node->child[1]))->nodedata) : NA_NULL;
  childdata[2] = (node->child[2]) ? (((NAQuadTreeNode*)(node->child[2]))->nodedata) : NA_NULL;
  childdata[3] = (node->child[3]) ? (((NAQuadTreeNode*)(node->child[3]))->nodedata) : NA_NULL;
}




// Allocates and initializes a new and empty node which is a child of
// an existing node and denotes the given segment.
NA_HDEF NAQuadTreeNode* naAddQuadTreeChildNode(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segment){

  NAQuadTreeNode* node = naAllocQuadTreeNode(
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
NA_HDEF NAQuadTreeNode* naAddQuadTreeNodeParent(NAQuadTree* tree, NAQuadTreeNode* childnode){

  NAInt childsegment;
  NAPosi parentorigin;
  NAQuadTreeNode* node;
  naComputeQuadTreeAlignment(childnode->childsize * 2, childnode->childorigin[0], &parentorigin, &childsegment);
  
  node = naAllocQuadTreeNode(
          childnode->childsize * 2,
          -1,
          NA_NULL,
          parentorigin,
          tree->callbacks.nodeallocator);
  
  node->child[childsegment] = childnode;
  childnode->parentnode = node;
  childnode->segmentinparent = childsegment;
  if(tree->callbacks.childchanged){
    const void* childdata[4];
    naCreateQuadTreeNodeChildData(childdata, node);
    tree->callbacks.childchanged(node->nodedata, childsegment, childdata);
  }
  return node;
}



// Creates a duplicate of the given oldnode.
// perentnode is the parent in the newtree
NA_HDEF void naCopyQuadTreeNode(NAQuadTree* newtree, NAQuadTreeNode* parentnode, NAInt segment, const NAQuadTreeNode* oldnode, const NAQuadTree* contenttree){

  NAQuadTreeNode* node;

  if(parentnode){
    node = naAddQuadTreeChildNode(newtree, parentnode, segment);
  }else{
    node = naAllocQuadTreeNode(
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
      if(oldnode->child[0]){node->child[0] = newtree->callbacks.leafallocator(node->childorigin[0], newtree->leafsize, newtree->callbacks.userdata, naGetQuadTreeLeafConst(contenttree, node->childorigin[0], NA_NULL));}
      if(oldnode->child[1]){node->child[1] = newtree->callbacks.leafallocator(node->childorigin[1], newtree->leafsize, newtree->callbacks.userdata, naGetQuadTreeLeafConst(contenttree, node->childorigin[1], NA_NULL));}
      if(oldnode->child[2]){node->child[2] = newtree->callbacks.leafallocator(node->childorigin[2], newtree->leafsize, newtree->callbacks.userdata, naGetQuadTreeLeafConst(contenttree, node->childorigin[2], NA_NULL));}
      if(oldnode->child[3]){node->child[3] = newtree->callbacks.leafallocator(node->childorigin[3], newtree->leafsize, newtree->callbacks.userdata, naGetQuadTreeLeafConst(contenttree, node->childorigin[3], NA_NULL));}
    }else{
      if(oldnode->child[0]){node->child[0] = newtree->callbacks.leafallocator(node->childorigin[0], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[0]);}
      if(oldnode->child[1]){node->child[1] = newtree->callbacks.leafallocator(node->childorigin[1], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[1]);}
      if(oldnode->child[2]){node->child[2] = newtree->callbacks.leafallocator(node->childorigin[2], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[2]);}
      if(oldnode->child[3]){node->child[3] = newtree->callbacks.leafallocator(node->childorigin[3], newtree->leafsize, newtree->callbacks.userdata, oldnode->child[3]);}
    }
    if(newtree->callbacks.leafchanged){newtree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);}
  }else{
    const void* childdata[4];
    if(oldnode->child[0]){naCopyQuadTreeNode(newtree, node, 0, oldnode->child[0], contenttree);}
    if(oldnode->child[1]){naCopyQuadTreeNode(newtree, node, 1, oldnode->child[1], contenttree);}
    if(oldnode->child[2]){naCopyQuadTreeNode(newtree, node, 2, oldnode->child[2], contenttree);}
    if(oldnode->child[3]){naCopyQuadTreeNode(newtree, node, 3, oldnode->child[3], contenttree);}
    naCreateQuadTreeNodeChildData(childdata, node);
    if(newtree->callbacks.childchanged){newtree->callbacks.childchanged(node->nodedata, -1, childdata);}
  }

}



// Deallocates one node.
NA_HDEF void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->childsize == tree->leafsize){
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



// Returns the segment index [0-3] in which the given pos can be found.
// Warning: The position is expected to be inside this node.
NA_HDEF NAUInt naGetQuadTreeNodeSegment(NAQuadTreeNode* node, NAPosi coord){
  NAUInt segment = 0;
  if(coord.x >= node->childorigin[3].x){segment |= 1;}
  if(coord.y >= node->childorigin[3].y){segment |= 2;}
  #ifndef NDEBUG
    if(!naIsPosiInRecti(coord, naMakeRecti(node->childorigin[segment], naMakeSizei(node->childsize, node->childsize))))
      naError("naGetQuadTreeNodeSegment", "node does not actually contains pos");
  #endif
  return segment;
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF void naUpdateQuadTreeNodeBubbling(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAInt segment){
  NABool bubble = NA_TRUE;
  
  // If we have a leaf parent, we deal with it first.
  if(parentnode->childsize == tree->leafsize){
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(parentnode->nodedata, segment, (const void* const) parentnode->child);
    }
    if(parentnode->parentnode){
      #ifndef NDEBUG
        if(parentnode->segmentinparent == -1)
          naError("naUpdateQuadTreeNode", "Inernal inconsistency detected");
      #endif
      segment = parentnode->segmentinparent;
    }
    parentnode = parentnode->parentnode;
  }
  
  // Then we loop over the height of the tree as long as we have a parent node
  // and as long as we can bubble.
  while(parentnode && bubble){
    if(tree->callbacks.childchanged){
      const void* childdata[4];
      naCreateQuadTreeNodeChildData(childdata, parentnode);
      bubble = tree->callbacks.childchanged(parentnode->nodedata, segment, childdata);
    }
    if(parentnode->parentnode){
      #ifndef NDEBUG
        if(parentnode->segmentinparent == -1)
          naError("naUpdateQuadTreeNode", "Inernal inconsistency detected");
      #endif
      segment = parentnode->segmentinparent;
    }
    parentnode = parentnode->parentnode;
  }
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular node
// (for example because it had been removed).
NA_HDEF NABool naUpdateQuadTreeNodeCapturing(NAQuadTree* tree, NAQuadTreeNode* node){
  NABool bubble = NA_FALSE;
  
  if(node->childsize == tree->leafsize){
    // This node stores leafs
    if(tree->callbacks.leafchanged){
      bubble = tree->callbacks.leafchanged(node->nodedata, -1, (const void* const) node->child);
    }
  }else{
    // this node stores subnodes
    if(tree->callbacks.childchanged){
      if(node->child[0]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[0]);}
      if(node->child[1]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[1]);}
      if(node->child[2]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[2]);}
      if(node->child[3]){bubble |= naUpdateQuadTreeNodeCapturing(tree, node->child[3]);}
    }
    if(bubble){
      const void* childdata[4];
      naCreateQuadTreeNodeChildData(childdata, node);
      bubble = tree->callbacks.childchanged(node->nodedata, -1, childdata);
    }
  }
  
  return bubble;
}



NA_DEF NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAUInt leafsize, NAQuadTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naInitQuadTree", "tree is Null-Pointer"); return NA_NULL;}
    if(!callbacks.leafallocator)
      naError("naInitQuadTree", "Must have a data allocator");
    if(!callbacks.leafdeallocator)
      naError("naInitQuadTree", "Must have a data destructor");
    if(leafsize < 1)
      naError("naInitQuadTree", "leafsize can not be smaller than 1");
  #endif
  tree->leafsize = leafsize;
  tree->callbacks = callbacks;
  tree->root = NA_NULL;
  tree->visitnode = NA_NULL;
  tree->curnode = NA_NULL;
  tree->cursegment = 0;
  return tree;
}




NA_DEF NAQuadTree* naCopyQuadTree(NAQuadTree* newtree, const NAQuadTree* duptree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTree", "newtree is Null-Pointer"); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->leafsize, duptree->callbacks);
  if(duptree->root){
    // Copy the root node
    // consequently, all other nodes and all leafs will be created.
    naCopyQuadTreeNode(newtree, NA_NULL, -1, duptree->root, NA_NULL);
  }
  return newtree;
}



NA_DEF NAQuadTree* naCopyQuadTreeWithMaskTree(NAQuadTree* newtree, const NAQuadTree* duptree, const NAQuadTree* masktree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithMaskTree", "newtree is Null-Pointer"); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->leafsize, duptree->callbacks);
  if(masktree->root){
    // Copy the root node
    // consequently, all other nodes and all leafs will be created.
    naCopyQuadTreeNode(newtree, NA_NULL, -1, masktree->root, duptree);
  }
  return newtree;
}



NA_DEF NAQuadTree* naCopyQuadTreeWithShift(NAQuadTree* newtree, const NAQuadTree* duptree, NASizei shift){
  NAUInt x1bound;
  NAUInt y1bound;
  NAUInt x2bound;
  NAUInt y2bound;
  NARecti rect0;
  NARecti rect1;
  NARecti rect2;
  NARecti rect3;
  const void* dupchunk;
  NAPosi origin;

  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithShift", "newtree is Null-Pointer"); return NA_NULL;}
    if(!duptree->callbacks.datacopier)
      naError("naCopyQuadTreeWithShift", "Data copier callback required.");
  #endif
  newtree = naInitQuadTree(newtree, duptree->leafsize, duptree->callbacks);
  if(!duptree->root){return newtree;}

  x1bound = ((shift.width % duptree->leafsize) + duptree->leafsize ) % duptree->leafsize;
  y1bound = ((shift.height % duptree->leafsize) + duptree->leafsize ) % duptree->leafsize;
  x2bound = duptree->leafsize - x1bound;
  y2bound = duptree->leafsize - y1bound;
  rect0 = naMakeRectiE(naMakePosi(shift.width, shift.height), naMakeSizeiE(x2bound, y2bound));
  rect1 = naMakeRectiE(naMakePosi(shift.width + x2bound, shift.height), naMakeSizeiE(x1bound, y2bound));
  rect2 = naMakeRectiE(naMakePosi(shift.width, shift.height + y2bound), naMakeSizeiE(x2bound, y1bound));
  rect3 = naMakeRectiE(naMakePosi(shift.width + x2bound, shift.height + y2bound), naMakeSizeiE(x1bound, y1bound));
  
  naFirstQuadTree(duptree);
  while((dupchunk = naIterateQuadTreeConst(duptree, &origin))){
    NAPosi neworigin;
    void* newdata;
    // We have a leaf with data. Now, we create all leafes in the new tree
    // containing the shifted leaf. There are max 4 new leaves.
    if(!naIsRectiEmpty(rect0)){
      neworigin = naMakePosi(origin.x + rect0.pos.x, origin.y + rect0.pos.y);
      newdata = naGetQuadTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakePosi(x1bound, y1bound), dupchunk, naMakePosi(0, 0), rect0.size, duptree->leafsize);
    }
    if(!naIsRectiEmpty(rect1)){
      neworigin = naMakePosi(origin.x + rect1.pos.x, origin.y + rect1.pos.y);
      newdata = naGetQuadTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakePosi(0, y1bound), dupchunk, naMakePosi(x2bound, 0), rect1.size, duptree->leafsize);
    }
    if(!naIsRectiEmpty(rect2)){
      neworigin = naMakePosi(origin.x + rect2.pos.x, origin.y + rect2.pos.y);
      newdata = naGetQuadTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakePosi(x1bound, 0), dupchunk, naMakePosi(0, y2bound), rect2.size, duptree->leafsize);
    }
    if(!naIsRectiEmpty(rect3)){
      neworigin = naMakePosi(origin.x + rect3.pos.x, origin.y + rect3.pos.y);
      newdata = naGetQuadTreeLeafMutable(newtree, neworigin, NA_TRUE, NA_NULL, NA_NULL);
      duptree->callbacks.datacopier(newdata, naMakePosi(0, 0), dupchunk, naMakePosi(x2bound, y2bound), rect3.size, duptree->leafsize);
    }
  }
  
  // Now, all the data has been copied. Update the whole tree.
  naUpdateQuadTree(newtree);
  return newtree;
}




// Returns the node containing the leaf containing coord. Stores the segment
// number in childsegment;
NA_HDEF NAQuadTreeNode* naLocateQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node, NAPosi coord, NABool create, const void* serialdata, NABool* didcreate, NAInt* leafsegment){
  NARecti noderect = naMakeRecti(node->childorigin[0], naMakeSizei(node->childsize * 2, node->childsize * 2));
  if(naIsPosiInRecti(coord, noderect)){
  
    // The coord is stored somewhere inside the rectangle of this node
    NAUInt segment = naGetQuadTreeNodeSegment(node, coord);
    if(node->child[segment]){
      // There is a segment available which contains the coord.
      if(node->childsize == tree->leafsize){
        // We are at leaf-level. We found it!
        #ifndef NDEBUG
          if(serialdata)
            naError("naLocateQuadTreeNode", "Leaf already exists when trying to deserialize");
        #endif
        tree->visitnode = node;
        *leafsegment = segment;
        if(didcreate){*didcreate = NA_FALSE;}
        return node;
      }else{
        // Go on searching in the sub-node
        return naLocateQuadTreeNode(tree, node->child[segment], coord, create, serialdata, didcreate, leafsegment);
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
              naError("naLocateQuadTreeNode", "Allocator returned null.");
          #endif
          *leafsegment = segment;
          if(didcreate){*didcreate = NA_TRUE;}
          return node;
        }else{
          // The node can be further subdivided.
          NAQuadTreeNode* subnode = naAddQuadTreeChildNode(tree, node, segment);
          // Go on searching in the sub node. Note that when the sub just
          // had been created, NA_FALSE must be returned. Therefore we ignore
          // the return value of this function and return NA_FALSE later.
          return naLocateQuadTreeNode(tree, subnode, coord, create, serialdata, didcreate, leafsegment);
        }
      }else{
        if(didcreate){*didcreate = NA_FALSE;}
        return NA_NULL;
      }
    }

  }else{
  
    // The coord is not stored within the rect of this node.
    if(node->parentnode){
      // We have a parent. Search there.
      return naLocateQuadTreeNode(tree, node->parentnode, coord, create, serialdata, didcreate, leafsegment);
    }else{
      // There is no parent
      #ifndef NDEBUG
        if(tree->root != node)
          naError("naLocateQuadTreeNode", "node seems not to be part of the tree.");
      #endif
      // If there is no parent, we create a new super-node which becomes the
      // parent of this node as well as the new root for the whole tree.
      if(create){
        tree->root = naAddQuadTreeNodeParent(tree, node);
        // Go on searching in the parent node. Note that when the parent just
        // had been created, NA_FALSE must be returned. Therefore, we ignore
        // the function result and return NA_FALSE later
        return naLocateQuadTreeNode(tree, node->parentnode, coord, create, serialdata, didcreate, leafsegment);
      }else{
        if(didcreate){*didcreate = NA_FALSE;}
        return NA_NULL;
      }
    }
  }
}




NA_HDEF NAQuadTreeNode* naLocateQuadTreeLeafParent(NAQuadTree* tree, NAPosi coord, NABool create, const void* serialdata, NABool* didcreate, NAInt* leafsegment){
  NAQuadTreeNode* node;
  
  if(tree->visitnode){
    // We start looking at the visit node.
    return naLocateQuadTreeNode(tree, tree->visitnode, coord, create, serialdata, didcreate, leafsegment);
  }else if(tree->curnode){
    // When no visit node is available, maybe the tree is currently iterating.
    return naLocateQuadTreeNode(tree, tree->curnode, coord, create, serialdata, didcreate, leafsegment);
  }else if(tree->root){
    // If the current node is not set, we start at the root.
    return naLocateQuadTreeNode(tree, tree->root, coord, create, serialdata, didcreate, leafsegment);
  }else if(create){
    // There is no visitnode, no curnode and no root. We create the first node
    // for this tree including the first leaf.
    NARecti leafalign = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->leafsize, tree->leafsize));
    NAPosi leaforigin = naMakePosiWithAlignment(coord, leafalign);
    NAPosi rootorigin;
    naComputeQuadTreeAlignment(tree->leafsize, leaforigin, &rootorigin, leafsegment);

    node = naAllocQuadTreeNode(
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



NA_DEF NAQuadTree* naInitQuadTreeWithDeserialization(NAQuadTree* tree, const void* buf, NAQuadTreeCallbacks callbacks){
  const NAByte* dataptr;
  uint64 datasize;
  uint64 leafsize;
  NAInt leafsegment;
  NAQuadTreeNode* node;
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naCopyQuadTreeWithShift", "tree is Null-Pointer"); return NA_NULL;}
    if(!(callbacks.deserialize)){
      naCrash("naInitQuadTreeWithDeserialization", "Callbacks required for deserialization");
      return NA_NULL;
    }
  #endif
  dataptr = buf;
  datasize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  leafsize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  tree = naInitQuadTree(tree, (NAInt)leafsize, callbacks);
  datasize -= 2 * sizeof(uint64);
  while(datasize){
    int64 posx, posy;
    uint64 bytesread;
    posx = *((const int64*)dataptr); dataptr += sizeof(int64);
    posy = *((const int64*)dataptr); dataptr += sizeof(int64);
    datasize -= 2 * sizeof(int64);
    // Create the leaf temporarily but delete it immediately and recreate it
    // as a deserialization.
    node = naLocateQuadTreeLeafParent(tree, naMakePosi((NAInt)posx, (NAInt)posy), NA_TRUE, dataptr, NA_NULL, &leafsegment);
    bytesread = callbacks.serialize(NA_NULL, node->child[leafsegment], tree->leafsize);
    dataptr += bytesread;
    datasize -= bytesread;
  }
  naUpdateQuadTree(tree);
  return tree;
}



NA_DEF void naEmptyQuadTree(NAQuadTree* tree){
  naClearQuadTree(tree);
  tree->root = NA_NULL;
  tree->curnode = NA_NULL;
  tree->visitnode = NA_NULL;
}



NA_DEF void naClearQuadTree(NAQuadTree* tree){
  if(tree->root){naDeallocQuadTreeNode(tree, tree->root);}
}



NA_DEF void naSerializeQuadTree(const NAQuadTree* tree, void* buf, uint64* bytesize){
  const void* curleaf;
  #ifndef NDEBUG
    if(!(tree->callbacks.serialize))
      naError("naSerializeQuadTree", "Callbacks required for serialization");
    if(!bytesize){
      naCrash("naSerializeQuadTree", "bytesize required to read/store byte size");
      return;
    }
    if(buf && (*bytesize == 0))
      naError("naSerializeQuadTree", "bytesize is zero");
  #endif

  naFirstQuadTree(tree);
  if(!buf){
    *bytesize = sizeof(uint64) * 2; // datasize and leafsize
    while((curleaf = naIterateQuadTreeConst(tree, NA_NULL))){
      *bytesize += sizeof(int64) * 2; // the origin.x and origin.y
      *bytesize += tree->callbacks.serialize(NA_NULL, curleaf, tree->leafsize);
    }
  }else{
    NAPosi origin;
    NAByte* dataptr = buf;
    *((uint64*)dataptr) = *bytesize; dataptr += sizeof(uint64);
    *((uint64*)dataptr) = tree->leafsize; dataptr += sizeof(uint64);

    while((curleaf = naIterateQuadTreeConst(tree, &origin))){
      *((int64*)dataptr) = origin.x; dataptr += sizeof(int64);
      *((int64*)dataptr) = origin.y; dataptr += sizeof(int64);
      dataptr += tree->callbacks.serialize(dataptr, curleaf, tree->leafsize);
    }
  }

}



NA_DEF NABool naIsQuadTreeEmpty(NAQuadTree* tree){
  return (tree->root == NA_NULL);
}



NA_DEF uint64 naGetQuadTreeLeafSize(const NAQuadTree* tree){
  return tree->leafsize;
}



NA_DEF NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree){
  return tree->callbacks;
}




NA_HDEF const void* naGetQuadTreeLeafConst(const NAQuadTree* tree, NAPosi coord, NAPosi* origin){
  NAQuadTreeNode* node;
  NAInt leafsegment;
  node = naLocateQuadTreeLeafParent((NAQuadTree*)tree, coord, NA_FALSE, NA_NULL, NA_NULL, &leafsegment);
  if(node){
    if(origin){*origin = node->childorigin[leafsegment];}
    return node->child[leafsegment];
  }else{
    return NA_NULL;
  }
}



NA_HDEF void* naGetQuadTreeLeafMutable(NAQuadTree* tree, NAPosi coord, NABool create, NABool* didcreate, NAPosi* origin){
  NAQuadTreeNode* node;
  NAInt leafsegment;
  node = naLocateQuadTreeLeafParent(tree, coord, create, NA_NULL, didcreate, &leafsegment);
  if(node){
    if(origin){*origin = node->childorigin[leafsegment];}
    return node->child[leafsegment];
  }else{
    return NA_NULL;
  }
}



// The segment number -1 means that the iteration goes downwards.
NA_HDEF void naIterateQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node, NAInt segment){
  // We go to the next segment. When we came to this function from above, we
  // start with the first segment.
  segment++;
  // Search for a segment which is available.
  while((segment < 4) && (node->child[segment] == NA_NULL)){segment++;}
  if(segment < 4){
    // If there is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(node->childsize == tree->leafsize){
      tree->curnode = node;
      tree->cursegment = segment;
    }else{
      naIterateQuadTreeNode(tree, node->child[segment], -1);
    }
  }else{
    // If there is no more segment available go upwards.
    if(node->parentnode){
      #ifndef NDEBUG
        if(node->segmentinparent == -1)
          naError("naIterateQuadTreeNode", "Inernal inconsistency detected");
      #endif
      naIterateQuadTreeNode(tree, node->parentnode, node->segmentinparent);
    }else{
      // no more searching. The iteration is over. We are at the end.
      tree->curnode = NA_NULL;
    }
  }
}



NA_DEF void naFirstQuadTree(const NAQuadTree* tree){
  if(!tree->root){return;}
  naIterateQuadTreeNode((NAQuadTree*)tree, tree->root, -1);
}



NA_DEF const void* naIterateQuadTreeConst(const NAQuadTree* tree, NAPosi* origin){
  const void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateQuadTreeNode((NAQuadTree*)tree, tree->curnode, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



NA_DEF void* naIterateQuadTreeMutable(NAQuadTree* tree, NAPosi* origin){
  void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateQuadTreeNode(tree, tree->curnode, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



// The segment number -1 means that the iteration goes downwards.
NA_HDEF void naIterateQuadTreeNodeInRect(NAQuadTree* tree, NAQuadTreeNode* node, NARecti* rect, NAInt segment){
  // We go to the next segment. When we came to this function from above, we
  // start with the first segment.
  segment++;
  // Search for a segment which is available.
  while(segment < 4){
    if(node->child[segment] != NA_NULL){
      NARecti childrect = naMakeRecti(node->childorigin[segment], naMakeSizei(node->childsize, node->childsize));
      if(naIsRectiUseful(naMakeRectiWithRectiIntersection(childrect, *rect))){break;}
    }
    segment++;
  }
  if(segment < 4){
    // If there is a segment available, either use the given leaf or go
    // downwards if it is an inner node.
    if(node->childsize == tree->leafsize){
      tree->curnode = node;
      tree->cursegment = segment;
    }else{
      naIterateQuadTreeNodeInRect(tree, node->child[segment], rect, -1);
    }
  }else{
    // If there is no more segment available go upwards.
    if(node->parentnode){
      #ifndef NDEBUG
        if(node->segmentinparent == -1)
          naError("naIterateQuadTreeNodeInRect", "Inernal inconsistency detected");
      #endif
      naIterateQuadTreeNodeInRect(tree, node->parentnode, rect, node->segmentinparent);
    }else{
      // no more searching. The iteration is over. We are at the end.
      tree->curnode = NA_NULL;
    }
  }
}



NA_DEF void naFirstQuadTreeInRect(const NAQuadTree* tree, NARecti rect){
  if(!tree->root){return;}
  naIterateQuadTreeNodeInRect((NAQuadTree*)tree, tree->root, &rect, -1);
}



NA_DEF const void* naIterateQuadTreeInRectConst(const NAQuadTree* tree, NAPosi* origin, NARecti rect){
  const void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateQuadTreeNodeInRect((NAQuadTree*)tree, tree->curnode, &rect, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



NA_DEF void* naIterateQuadTreeInRectMutable(NAQuadTree* tree, NAPosi* origin, NARecti rect){
  void* retptr;
  if(!tree->root){return NA_NULL;}
  if(tree->curnode){
    retptr = tree->curnode->child[tree->cursegment];
    if(origin){*origin = tree->curnode->childorigin[tree->cursegment];}
    naIterateQuadTreeNodeInRect(tree, tree->curnode, &rect, tree->cursegment);
  }else{
    retptr = NA_NULL;
  }
  return retptr;
}



NA_DEF void naSetQuadTreeInRect(NAQuadTree* tree, NARecti rect, NAQuadTreeDataSetter datasetter, const void* userdata){
  NARecti chunkrect;
  NAPosi start;
  NAPosi end;
  chunkrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->leafsize, tree->leafsize));

  start = naMakePosiWithAlignment(rect.pos, chunkrect);
  end = naGetRectiEnd(rect);
  
  for(chunkrect.pos.y = start.y; chunkrect.pos.y < end.y; chunkrect.pos.y += tree->leafsize){
    for(chunkrect.pos.x = start.x; chunkrect.pos.x < end.x; chunkrect.pos.x += tree->leafsize){
      void* curchunk = naGetQuadTreeLeafMutable(tree, chunkrect.pos, NA_TRUE, NA_NULL, NA_NULL);
      NARecti clamprect = naClampRectiToRecti(rect, chunkrect);
      clamprect.pos.x -= chunkrect.pos.x;
      clamprect.pos.y -= chunkrect.pos.y;
      datasetter(curchunk, clamprect, tree->leafsize, userdata);
    }
  }

}



// Expects the node to be the node to remove.
NA_HDEF void naRemoveQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->parentnode){
  
    // Remove the segment from the tree
    #ifndef NDEBUG
      if(node->segmentinparent == -1)
        naError("naRemoveQuadTreeNode", "Inernal inconsistency detected");
    #endif
    node->parentnode->child[node->segmentinparent] = NA_NULL;
    
    // Test if there are other childs...
    if(node->parentnode->child[0] || node->parentnode->child[1] || node->parentnode->child[2] || node->parentnode->child[3]){
      // If there is at least one more child, we update the parent node.
      naUpdateQuadTreeNodeBubbling(tree, node->parentnode, node->segmentinparent);
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




// This methods checks if the root of the tree has only one child. As long
// as there is only one child in the root, the root is transferred to that
// very child to save memory and allow garbage collectors to collect.
NA_HDEF void naAdjustQuadTreeRoot(NAQuadTree* tree){
  while(1){
    if(tree->root->childsize == tree->leafsize){
      // The root node has only leafes as childs.
      return;
    }else{
      // We count how many children there are.
      NAUInt i;
      NAUInt childcount = 0;
      NAUInt lastchildindex = 0;
      NAQuadTreeNode* lastchild;
      for(i=0; i<4; i++){if(tree->root->child[i]){childcount++; lastchildindex = i;}}
      #ifndef NDEBUG
        if(!childcount)
          naError("naAdjustQuadTreeRoot", "Root should not be empty.");
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
      naDeallocQuadTreeNode(tree, tree->root);
      // ...and set the new root.
      tree->root = lastchild;
    }
  }
}



NA_DEF void naRemoveQuadTreeLeaf(NAQuadTree* tree, NAPosi coord){
  NAQuadTreeNode* node;
  NAInt leafsegment;
  
  node = naLocateQuadTreeLeafParent(tree, coord, NA_FALSE, NA_NULL, NA_FALSE, &leafsegment);
  if(!node){return;}
  
  #ifndef NDEBUG
    if(node == tree->curnode)
      naError("naRemoveQuadTreeLeaf", "Removing the node currently iterating on");
  #endif

  // First, we destroy the data of the leaf chunk
  tree->callbacks.leafdeallocator(node->child[leafsegment], tree->callbacks.userdata);
  node->child[leafsegment] = NA_NULL;

  // Test if there are other childs in the current parent.
  if(node->child[0] || node->child[1] || node->child[2] || node->child[3]){
    // If there is at least one more child, we update the node.
    naUpdateQuadTreeNodeBubbling(tree, node, leafsegment);
  }else{
    // If there are no more childs, we remove the node.
    naRemoveQuadTreeNode(tree, node);
  }
  if(tree->root){
    // Now, if the root has only one child left, we adjust the tree accordingly.
    naAdjustQuadTreeRoot(tree);
  }
  // Removing always nullifies the last visit.
  tree->visitnode = NA_NULL;
}



NA_DEF void naUpdateQuadTreeAtCoord(NAQuadTree* tree, NAPosi coord){
  NAInt leafsegment;
  NAQuadTreeNode* node = naLocateQuadTreeLeafParent(tree, coord, NA_FALSE, NA_NULL, NA_FALSE, &leafsegment);
  if(node){
    naUpdateQuadTreeNodeBubbling(tree, node, leafsegment);
  }
}



NA_DEF void naUpdateQuadTree(NAQuadTree* tree){
  if(tree->root){
    naUpdateQuadTreeNodeCapturing(tree, tree->root);
  }
}



NA_DEF void* naGetQuadTreeRootNodeData(NAQuadTree* tree){
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
