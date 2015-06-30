
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAQuadTree.h"



// Allocates and initializes a new and empty QuadTree node. The rects of all
// childs are initialized but no leafs are set or allocated.
NA_HDEF NAQuadTreeNode* naAllocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAPosi origin, NAInt childsize){
  NAQuadTreeNode* node = naAlloc(NAQuadTreeNode);
  naNulln(node, sizeof(NAQuadTreeNode));

  node->parentnode = parentnode;  
  node->childrect[0] = naMakeRecti(naMakePosi(origin.x, origin.y), naMakeSizei(childsize, childsize));
  node->childrect[1] = naMakeRecti(naMakePosi(origin.x + childsize, origin.y), naMakeSizei(childsize, childsize));
  node->childrect[2] = naMakeRecti(naMakePosi(origin.x, origin.y + childsize), naMakeSizei(childsize, childsize));
  node->childrect[3] = naMakeRecti(naMakePosi(origin.x + childsize, origin.y + childsize), naMakeSizei(childsize, childsize));
  if(tree->callbacks.nodeallocator){
    node->nodedata = tree->callbacks.nodeallocator(naMakeRecti(naMakePosi(origin.x, origin.y), naMakeSizei(childsize*2, childsize*2)));
  }
  return node;
}


NA_HDEF void naDestructQuadTreeNodeSegment(NAQuadTree* tree, NAQuadTreeNode* node, NAUInt segment){
  #ifndef NDEBUG
    if(!node->child[segment])
      naError("naDestructQuadTreeNodeSegment", "Segment of node does not exists");
  #endif
  if(node->childflag[segment] & NA_QUADTREE_CHILD_IS_LEAF){
    tree->callbacks.datadestructor(node->child[segment]);
  }else{
    naDeallocQuadTreeNode(tree, node->child[segment]);
  }
}



NA_HDEF void naDeallocQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->child[0]){naDestructQuadTreeNodeSegment(tree, node, 0);}
  if(node->child[1]){naDestructQuadTreeNodeSegment(tree, node, 1);}
  if(node->child[2]){naDestructQuadTreeNodeSegment(tree, node, 2);}
  if(node->child[3]){naDestructQuadTreeNodeSegment(tree, node, 3);}
  if(node->nodedata && tree->callbacks.nodedestructor){tree->callbacks.nodedestructor(node->nodedata);}
  free(node);
}





NA_DEF NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAInt minchildsize, NAQuadTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naInitQuadTree", "tree is NULL"); return NA_NULL;}
    if(!callbacks.dataallocator)
      naError("naInitQuadTree", "Must have a data allocator");
    if(!callbacks.datadestructor)
      naError("naInitQuadTree", "Must have a data destructor");
    if(minchildsize < 1)
      naError("naInitQuadTree", "minchildsize can not be smaller than 1");
  #endif
  tree->minchildsize = minchildsize;
  tree->root = NA_NULL;
  tree->curnode = NA_NULL;
  tree->cursegment = 0;
  naCpyn(&(tree->callbacks), &callbacks, sizeof(NAQuadTreeCallbacks));
  return tree;
}




NA_HDEF NAQuadTreeNode* naDuplicateQuadTreeNode(NAQuadTree* newtree, NAQuadTreeNode* parentnode, NAQuadTreeNode* oldnode, NAQuadTree* contenttree){
  NAInt s;
  NAQuadTreeNode* newnode = naAllocQuadTreeNode(newtree, parentnode, oldnode->childrect[0].pos, oldnode->childrect[0].size.width);
  newnode->parentnode = parentnode;
  for(s=0; s<4; s++){
    newnode->child[s] = NA_NULL;
    newnode->childrect[s] = oldnode->childrect[s];
    newnode->childflag[s] = oldnode->childflag[s];
    if(oldnode->child[s]){
      if(oldnode->childflag[s] & NA_QUADTREE_CHILD_IS_LEAF){
        if(contenttree){
          NABool hascontent = naLocateQuadTreePosi(contenttree, newnode->childrect[s].pos, NA_FALSE);
          if(hascontent){
            // create a copy of the content leaf
            newnode->child[s] = newtree->callbacks.dataallocator(newnode->childrect[s], naGetQuadTreeConst(contenttree, NA_NULL));
          }else{
            // Create an empty leaf
            newnode->child[s] = newtree->callbacks.dataallocator(newnode->childrect[s], NA_NULL);
          }
        }else{
          // Create a copy of the old nodes leaf
          newnode->child[s] = newtree->callbacks.dataallocator(newnode->childrect[s], oldnode->child[s]);
        }
//        naUpdateQuadTreeLeaf(newtree, newnode, s);
      }else{
        newnode->child[s] = naDuplicateQuadTreeNode(newtree, newnode, oldnode->child[s], contenttree);
//        naUpdateQuadTreeNode(newtree, newnode->child[s], newnode->childrect[s].pos);
      }
    }
  }
//  if(newtree->callbacks.nodeallocator){
//    NAInt size = newnode->childrect[0].size.width * 2;
//    newnode->nodedata = newtree->callbacks.nodeallocator(naMakeRecti(newnode->childrect[0].pos, naMakeSizei(size, size)));
//  }else{
//    newnode->nodedata = NA_NULL;
//  }
  return newnode;
}


// Returns the segment index [0-3] in which the given pos can be found.
// Returns -1 if the given pos is not inside this node. This is not an error
// as it might happen when a position far outside of an existing tree is
// checked.
NA_HDEF NAUInt naGetQuadTreeNodeSegment(NAQuadTreeNode* node, NAPosi pos){
  NAUInt segment = 0;
  if(pos.x >= node->childrect[3].pos.x){segment |= 1;}
  if(pos.y >= node->childrect[3].pos.y){segment |= 2;}
  #ifndef NDEBUG
    if(!naIsPosiInRecti(pos, node->childrect[segment]))
      naError("naGetQuadTreeNodeSegment", "node does not actually contains pos");
  #endif
  return segment;
}








NA_DEF NAQuadTree* naCopyQuadTree(NAQuadTree* newtree, const NAQuadTree* duptree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTree", "newtree is NULL"); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->minchildsize, duptree->callbacks);
  if(duptree->root){
    newtree->root = naDuplicateQuadTreeNode(newtree, NA_NULL, duptree->root, NA_NULL); // todo const safe
    
    NAGrowingSpace leafs;
    naInitGrowingSpaceWithQuadTreeChunks(&leafs, (NAQuadTree*)newtree);  // todo const save
    naFirstGrowingSpaceElement(&leafs);
    NAMapi* curleaf = naGetGrowingSpaceMutableContent(&leafs);
    while(curleaf){
      naLocateQuadTreePosi(newtree, curleaf->rect.pos, NA_FALSE);
      naUpdateQuadTreeLeaf(newtree, newtree->curnode, newtree->cursegment);
      naNextGrowingSpaceElement(&leafs);
      curleaf = naGetGrowingSpaceMutableContent(&leafs);
    }
    
    naClearGrowingSpace(&leafs, NA_NULL);
  }
  return newtree;
}



NA_DEF NAQuadTree* naCopyQuadTreeWithMaskTree(NAQuadTree* newtree, NAQuadTree* duptree, NAQuadTree* masktree){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithMaskTree", "newtree is NULL"); return NA_NULL;}
  #endif
  newtree = naInitQuadTree(newtree, duptree->minchildsize, duptree->callbacks);
  if(masktree->root){
    newtree->root = naDuplicateQuadTreeNode(newtree, NA_NULL, masktree->root, duptree);
  }
  return newtree;
}



NA_DEF NAQuadTree* naCopyQuadTreeWithShift(NAQuadTree* newtree, NAQuadTree* duptree, NASizei shift){
  #ifndef NDEBUG
    if(!newtree)
      {naCrash("naCopyQuadTreeWithShift", "newtree is NULL"); return NA_NULL;}
    if(!duptree->callbacks.datacopier)
      naError("naCopyQuadTreeWithShift", "Data copier callback required.");
  #endif
  newtree = naInitQuadTree(newtree, duptree->minchildsize, duptree->callbacks);
  if(!duptree->root){return newtree;}

  NAUInt x1bound = ((shift.width % duptree->minchildsize) + duptree->minchildsize ) % duptree->minchildsize;
  NAUInt y1bound = ((shift.height % duptree->minchildsize) + duptree->minchildsize ) % duptree->minchildsize;
  NAUInt x2bound = duptree->minchildsize - x1bound;
  NAUInt y2bound = duptree->minchildsize - y1bound;
  NARecti rect0 = naMakeRectiE(naMakePosi(shift.width, shift.height), naMakeSizeiE(x2bound, y2bound));
  NARecti rect1 = naMakeRectiE(naMakePosi(shift.width + x2bound, shift.height), naMakeSizeiE(x1bound, y2bound));
  NARecti rect2 = naMakeRectiE(naMakePosi(shift.width, shift.height + y2bound), naMakeSizeiE(x2bound, y1bound));
  NARecti rect3 = naMakeRectiE(naMakePosi(shift.width + x2bound, shift.height + y2bound), naMakeSizeiE(x1bound, y1bound));
  
  NARecti alignrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(duptree->minchildsize, duptree->minchildsize));
  NAPosi alignpos1 = naMakePosiWithAlignment(duptree->root->childrect[0].pos, alignrect);
  NAPosi alignpos2 = naMakePosiWithAlignment(naGetRectiMaxPos(duptree->root->childrect[3]), alignrect);
  NAPosi origin;
  for(origin.y = alignpos1.y; origin.y <= alignpos2.y; origin.y += duptree->minchildsize){
    for(origin.x = alignpos1.x; origin.x <= alignpos2.x; origin.x += duptree->minchildsize){
      NABool found = naLocateQuadTreePosi(duptree, origin, NA_FALSE);
      const NAByte* dupdata = naGetQuadTreeMutable(duptree, NA_NULL);
      if(found){
        // We have a leaf with data. Now, we create all leafes in the new tree
        // containing the shifted leaf. There are max 4 new leaves.
        if(!naIsRectiEmpty(rect0)){
          NAPosi neworigin = naMakePosi(origin.x + rect0.pos.x, origin.y + rect0.pos.y);
          naLocateQuadTreePosi(newtree, neworigin, NA_TRUE);
          NAByte* newdata = naGetQuadTreeMutable(newtree, NA_NULL);
          NARecti duprect = naMakeRecti(naMakePosi(0, 0), rect0.size);
          NARecti newrect = naMakeRecti(naMakePosi(x1bound, y1bound), rect0.size);
          duptree->callbacks.datacopier(newdata, newrect, dupdata, duprect);
        }
        if(!naIsRectiEmpty(rect1)){
          NAPosi neworigin = naMakePosi(origin.x + rect1.pos.x, origin.y + rect1.pos.y);
          naLocateQuadTreePosi(newtree, neworigin, NA_TRUE);
          NAByte* newdata = naGetQuadTreeMutable(newtree, NA_NULL);
          NARecti duprect = naMakeRecti(naMakePosi(x2bound, 0), rect1.size);
          NARecti newrect = naMakeRecti(naMakePosi(0, y1bound), rect1.size);
          duptree->callbacks.datacopier(newdata, newrect, dupdata, duprect);
        }
        if(!naIsRectiEmpty(rect2)){
          NAPosi neworigin = naMakePosi(origin.x + rect2.pos.x, origin.y + rect2.pos.y);
          naLocateQuadTreePosi(newtree, neworigin, NA_TRUE);
          NAByte* newdata = naGetQuadTreeMutable(newtree, NA_NULL);
          NARecti duprect = naMakeRecti(naMakePosi(0, y2bound), rect2.size);
          NARecti newrect = naMakeRecti(naMakePosi(x1bound, 0), rect2.size);
          duptree->callbacks.datacopier(newdata, newrect, dupdata, duprect);
        }
        if(!naIsRectiEmpty(rect3)){
          NAPosi neworigin = naMakePosi(origin.x + rect3.pos.x, origin.y + rect3.pos.y);
          naLocateQuadTreePosi(newtree, neworigin, NA_TRUE);
          NAByte* newdata = naGetQuadTreeMutable(newtree, NA_NULL);
          NARecti duprect = naMakeRecti(naMakePosi(x2bound, y2bound), rect3.size);
          NARecti newrect = naMakeRecti(naMakePosi(0, 0), rect3.size);
          duptree->callbacks.datacopier(newdata, newrect, dupdata, duprect);
        }
      }
    }
  }
  return newtree;
  // todo: call update
}


NA_DEF NAQuadTree* naInitQuadTreeWithDeserialization(NAQuadTree* tree, const void* buf, NAQuadTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!tree)
      {naCrash("naCopyQuadTreeWithShift", "tree is NULL"); return NA_NULL;}
    if(!(callbacks.deserialize)){
      naCrash("naInitQuadTreeWithDeserialization", "Callbacks required for deserialization");
      return NA_NULL;
    }
  #endif
  const NAByte* dataptr = buf;
  uint64 datasize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  uint64 minchildsize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  tree = naInitQuadTree(tree, (NAInt)minchildsize, callbacks);
  datasize -= 2 * sizeof(uint64);
  while(datasize){
    int64 posx = *((const int64*)dataptr); dataptr += sizeof(int64);
    int64 posy = *((const int64*)dataptr); dataptr += sizeof(int64);
    datasize -= 2 * sizeof(int64);
    // Create the leaf temporarily but delete it immediately and recreate it
    // as a deserialization.
    naLocateQuadTreePosi(tree, naMakePosi((NAInt)posx, (NAInt)posy), NA_TRUE);
    callbacks.datadestructor(tree->curnode->child[tree->cursegment]);
    tree->curnode->child[tree->cursegment] = callbacks.deserialize(naMakeRecti(naMakePosi((NAInt)posx, (NAInt)posy), naMakeSizei((NAInt)minchildsize, (NAInt)minchildsize)), dataptr);
    naUpdateQuadTreeLeaf(tree, tree->curnode, tree->cursegment);
    uint64 bytesread = callbacks.serialize(NA_NULL, tree->curnode->child[tree->cursegment]);
    dataptr += bytesread;
    datasize -= bytesread;
  }
  return tree;
}


NA_DEF void naEmptyQuadTree(NAQuadTree* tree){
  naClearQuadTree(tree);
  tree->root = NA_NULL;
  tree->curnode = NA_NULL;
  tree->cursegment = 0;
}



NA_DEF void naClearQuadTree(NAQuadTree* tree){
  if(tree->root){naDeallocQuadTreeNode(tree, tree->root);}
}



NA_DEF void naSerializeQuadTree(const NAQuadTree* tree, void* buf, uint64* bytesize){
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
  NAGrowingSpace leafs;
  naInitGrowingSpaceWithQuadTreeChunks(&leafs, (NAQuadTree*)tree);  // todo const save
  const NAMapi* curmap;

  if(!buf){
    *bytesize = sizeof(uint64) * 2; // datasize and minchildsize
    naFirstGrowingSpaceElement(&leafs);
    curmap = naGetGrowingSpaceConstContent(&leafs);
    while(curmap){
      *bytesize += tree->callbacks.serialize(NA_NULL, curmap->data);
      *bytesize += sizeof(int64) * 2; // the origin.x and origin.y
      naNextGrowingSpaceElement(&leafs);
      curmap = naGetGrowingSpaceConstContent(&leafs);
    }
  }else{
    NAByte* dataptr = buf;
    *((uint64*)dataptr) = *bytesize; dataptr += sizeof(uint64);
    *((uint64*)dataptr) = tree->minchildsize; dataptr += sizeof(uint64);

    naFirstGrowingSpaceElement(&leafs);
    curmap = naGetGrowingSpaceConstContent(&leafs);
    while(curmap){
      *((int64*)dataptr) = curmap->rect.pos.x; dataptr += sizeof(int64);
      *((int64*)dataptr) = curmap->rect.pos.y; dataptr += sizeof(int64);
      dataptr += tree->callbacks.serialize(dataptr, curmap->data);
      naNextGrowingSpaceElement(&leafs);
      curmap = naGetGrowingSpaceConstContent(&leafs);
    }
  }

  naClearGrowingSpace(&leafs, NA_NULL);
}



// Returns true if there already was a content.
NA_HDEF NABool naLocateQuadTreeNodePosi(NAQuadTree* tree, NAQuadTreeNode* node, NAPosi pos, NABool create){
  NARecti noderect = naMakeRecti(node->childrect[0].pos, naMakeSizei(node->childrect[0].size.width * 2, node->childrect[0].size.width * 2));
  if(naIsPosiInRecti(pos, noderect)){
  
    // The position is stored somewhere inside the rectangle of this node
    NAUInt segment = naGetQuadTreeNodeSegment(node, pos);
    if(node->child[segment]){
      if(node->childflag[segment] & NA_QUADTREE_CHILD_IS_LEAF){
        // We found it!
        tree->curnode = node;
        tree->cursegment = segment;
        return NA_TRUE;
      }else{
        // Go on searching in the sub-node
        return naLocateQuadTreeNodePosi(tree, node->child[segment], pos, create);
      }
    }else{
      if(create){
        if(noderect.size.width / 2 == tree->minchildsize){
          // The node can not be divided anymore. Create a leaf.
          node->child[segment] = tree->callbacks.dataallocator(node->childrect[segment], NA_NULL);
          #ifndef NDEBUG
            if(!node->child[segment])
              naError("naLocateQuadTreeNodePosi", "Allocator returned null.");
          #endif
          node->childflag[segment] |= NA_QUADTREE_CHILD_IS_LEAF;
          tree->curnode = node;
          tree->cursegment = segment;
          naUpdateQuadTreeLeaf(tree, node, segment);
        }else{
          // The node can be further subdivided.
          node->child[segment] = naAllocQuadTreeNode(tree, node, node->childrect[segment].pos, node->childrect[segment].size.width / 2);
          // Go on searching in the sub node. Note that when the sub just
          // had been created, NA_FALSE must be returned. Therefore we ignore
          // the return value of this function and return NA_FALSE later.
          naLocateQuadTreeNodePosi(tree, node->child[segment], pos, create);
        }
      }
      return NA_FALSE;
    }

  }else{
  
    // The position is not stored within the rect of this node.
    if(node->parentnode){
      // We have a parent. Search there.
      return naLocateQuadTreeNodePosi(tree, node->parentnode, pos, create);
    }else{
      // There is no parent
      #ifndef NDEBUG
        if(tree->root != node)
          naError("naLocateQuadTreeNodePosi", "node seems not to be part of the tree.");
      #endif
      // If there is no parent, we create a new super-node which becomes the
      // parent of this node as well as the new root for the whole tree.
      if(create){
        // Note that in order to achieve a full coverage of the whole space
        // (negative and positive in x and y), we align the super node such
        // that more space is allocated in the direction of where to store the
        // new data.
        NARecti alignrect;
        alignrect.pos.x = noderect.pos.x;
        alignrect.pos.y = noderect.pos.y;
        alignrect.size.width = noderect.size.width * 2;
        alignrect.size.height = noderect.size.height * 2;
        NAUInt segment = 0;
        if(pos.x < noderect.pos.x){
          segment |= 1; alignrect.pos.x -= noderect.size.width;
        }
        if(pos.y < noderect.pos.y){segment |= 2; alignrect.pos.y -= noderect.size.height;}
        node->parentnode = naAllocQuadTreeNode(tree, NA_NULL, alignrect.pos, noderect.size.width);
        tree->root = node->parentnode;
        // Attach this node to the new parent
        node->parentnode->child[segment] = node;
        #ifndef NDEBUG
          if(!naEqualRecti(node->parentnode->childrect[segment], noderect))
            naError("naLocateQuadTreeNodePosi", "computed node rects do not match.");
        #endif
        naUpdateQuadTreeNode(tree, node, pos);
        // Go on searching in the parent node. Note that when the parent just
        // had been created, NA_FALSE must be returned. Therefore, we ignore
        // the function result and return NA_FALSE later
        naLocateQuadTreeNodePosi(tree, node->parentnode, pos, create);
      }
      return NA_FALSE;
    }
  }
}



NA_DEF NABool naIsQuadTreeEmpty(NAQuadTree* tree){
  return (tree->root == NA_NULL);
}



NA_DEF uint64 naGetQuadTreeMinChildSize(const NAQuadTree* tree){
  return tree->minchildsize;
}


NA_DEF NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree){
  return tree->callbacks;
}



// Returns true if there already was a content.
NA_DEF NABool naLocateQuadTreePosi(NAQuadTree* tree, NAPosi pos, NABool create){
  // We start looking at the current node.
  if(tree->curnode){
    return naLocateQuadTreeNodePosi(tree, tree->curnode, pos, create);
  }else if(tree->root){
    // If the current node is not set, we start at the root.
    return naLocateQuadTreeNodePosi(tree, tree->root, pos, create);
  }else{
    // There is no curnode and no root. We create the first node for this tree
    // if desired.
    if(create){
      NARecti alignrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->minchildsize * 2, tree->minchildsize * 2));
      NAPosi origin = naMakePosiWithAlignment(pos, alignrect);
      tree->root = naAllocQuadTreeNode(tree, NA_NULL, origin, tree->minchildsize);
      tree->curnode = tree->root;
      tree->cursegment = naGetQuadTreeNodeSegment(tree->root, pos);
      tree->curnode->child[tree->cursegment] = tree->callbacks.dataallocator(tree->curnode->childrect[tree->cursegment], NA_NULL);
      tree->curnode->childflag[tree->cursegment] |= NA_QUADTREE_CHILD_IS_LEAF;
      #ifndef NDEBUG
        if(!tree->curnode->child[tree->cursegment])
          naError("naLocateQuadTreePosi", "Allocator returned null.");
      #endif
    }
    return NA_FALSE;
  }
}




NA_HDEF const void* naGetQuadTreeConst(const NAQuadTree* tree, NARecti* rect){
  if(tree->curnode){
    if(rect){*rect = tree->curnode->childrect[tree->cursegment];}
    return tree->curnode->child[tree->cursegment];
  }else{
    return NA_NULL;
  }
}


NA_HDEF void* naGetQuadTreeMutable(NAQuadTree* tree, NARecti* rect){
  if(tree->curnode){
    if(rect){*rect = tree->curnode->childrect[tree->cursegment];}
    return tree->curnode->child[tree->cursegment];
  }else{
    return NA_NULL;
  }
}


NA_HDEF void naRemoveQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->parentnode){
    NAUInt segment = naGetQuadTreeNodeSegment(node->parentnode, node->childrect[0].pos);
    node->parentnode->child[segment] = NA_NULL;
    if(node->parentnode->child[0] || node->parentnode->child[1] || node->parentnode->child[2] || node->parentnode->child[3]){
      naUpdateQuadTreeNode(tree, node, node->childrect[0].pos);
    }else{
      naRemoveQuadTreeNode(tree, node->parentnode);
    }
  }else{
    tree->root = NA_NULL;
  }
  naDeallocQuadTreeNode(tree, node);
}



NA_DEF void naRemoveQuadTreeLeaf(NAQuadTree* tree){
  if(tree->curnode){
    tree->callbacks.datadestructor(tree->curnode->child[tree->cursegment]);
    tree->curnode->child[tree->cursegment] = NA_NULL;
    if(tree->curnode->child[0] || tree->curnode->child[1] || tree->curnode->child[2] || tree->curnode->child[3]){
      naUpdateQuadTreeLeaf(tree, tree->curnode, tree->cursegment);
    }else{
      naRemoveQuadTreeNode(tree, tree->curnode);
    }
    tree->curnode = NA_NULL;
  }
}



NA_DEF NAGrowingSpace* naInitGrowingSpaceWithQuadTreeChunks(NAGrowingSpace* space, NAQuadTree* tree){
  naInitGrowingSpace(space, sizeof(NAMapi), NA_NULL);
  if(!tree->root){return space;}
  
  NARecti alignrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->minchildsize, tree->minchildsize));
  NAPosi alignpos1 = naMakePosiWithAlignment(tree->root->childrect[0].pos, alignrect);
  NAPosi alignpos2 = naMakePosiWithAlignment(naGetRectiMaxPos(tree->root->childrect[3]), alignrect);
  NAPosi origin;
  for(origin.y = alignpos1.y; origin.y <= alignpos2.y; origin.y += tree->minchildsize){
    for(origin.x = alignpos1.x; origin.x <= alignpos2.x; origin.x += tree->minchildsize){
      NABool found = naLocateQuadTreePosi(tree, origin, NA_FALSE);
      if(found){
        NAMapi* curmap = naNewGrowingSpaceElement(space);
        curmap->data = naGetQuadTreeMutable(tree, &(curmap->rect));
      }
    }
  }
  return space;
}



NA_DEF NAGrowingSpace* naInitGrowingSpaceWithQuadTreeChunksInRecti(NAGrowingSpace* space, NAQuadTree* tree, NARecti rect, NABool create){
  NARecti alignrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->minchildsize, tree->minchildsize));
  NAPosi alignpos1 = naMakePosiWithAlignment(rect.pos, alignrect);
  NAPosi alignpos2 = naMakePosiWithAlignment(naGetRectiMaxPos(rect), alignrect);
  naInitGrowingSpace(space, sizeof(NAMapi), NA_NULL);
  NAPosi origin;
  for(origin.y = alignpos1.y; origin.y <= alignpos2.y; origin.y += tree->minchildsize){
    for(origin.x = alignpos1.x; origin.x <= alignpos2.x; origin.x += tree->minchildsize){
      NABool found = naLocateQuadTreePosi(tree, origin, create);
      if(create || found){
        NAMapi* curmap = naNewGrowingSpaceElement(space);
        curmap->data = naGetQuadTreeMutable(tree, &(curmap->rect));
      }
    }
  }
  return space;
}



NA_HDEF void naUpdateQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* curnode, NAPosi curpos){
  NABool bubble = NA_TRUE;
  while(bubble && curnode->parentnode && tree->callbacks.childchanged){
    NAUInt segment;
    if(naIsPosiInRecti(curpos, naMakeRecti(curnode->childrect[0].pos, naMakeSizei(curnode->childrect[0].size.width * 2, curnode->childrect[0].size.width * 2)))){
      segment = naGetQuadTreeNodeSegment(curnode->parentnode, curpos);
    }else{
      segment = -1;
    }
    curnode = curnode->parentnode;
    const void* childdata[4];
    childdata[0] = (curnode->child[0]) ? (((NAQuadTreeNode*)(curnode->child[0]))->nodedata) : NA_NULL;
    childdata[1] = (curnode->child[1]) ? (((NAQuadTreeNode*)(curnode->child[1]))->nodedata) : NA_NULL;
    childdata[2] = (curnode->child[2]) ? (((NAQuadTreeNode*)(curnode->child[2]))->nodedata) : NA_NULL;
    childdata[3] = (curnode->child[3]) ? (((NAQuadTreeNode*)(curnode->child[3]))->nodedata) : NA_NULL;
    bubble = tree->callbacks.childchanged(curnode->nodedata, segment, childdata);
  }
}


NA_HDEF void naUpdateQuadTreeLeaf(NAQuadTree* tree, NAQuadTreeNode* leafparent, NAInt segment){
  if(tree->callbacks.leafchanged){
    NABool bubble = tree->callbacks.leafchanged(leafparent->nodedata, segment, leafparent->child);
    if(bubble){naUpdateQuadTreeNode(tree, leafparent, leafparent->childrect[segment].pos);}
  }
}


NA_DEF void naUpdateQuadTree(NAQuadTree* tree){
  if(tree->curnode){naUpdateQuadTreeLeaf(tree, tree->curnode, tree->cursegment);}
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
