
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_QUADTREE_INCLUDED
#define NA_QUADTREE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAByteMap2D.h"
#include "NAGrowingSpace.h"

typedef struct NAQuadTree NAQuadTree;


// Various callback functions which you can give to a tree.
// Allocation and destruction function pointers which allocate your leaf-
// objects for the given rect. Returning Null is a bad thing.
typedef void* (*NAQuadTreeDataAllocator)(NARecti rect, const void* copydata);
typedef void  (*NAQuadTreeDataDestructor)(void* ptr);
typedef void* (*NAQuadTreeDataCopier)(const void* dstdata, NARecti dstrect, const void* copydata, NARecti copyrect);
typedef void* (*NAQuadTreeNodeAllocator)(NARecti rect);
typedef void  (*NAQuadTreeNodeDestructor)(void* ptr);
// Segment is either a number in [0 - 3] denoting the child which has been
// changed or -1 if there is no particular child.
typedef NABool(*NAQuadTreeLeafChanged)(void* nodedata, NAInt segment, void* const leafdata[4]);
typedef NABool(*NAQuadTreeChildChanged)(void* nodedata, NAInt segment, const void* const childdata[4]);
typedef uint64(*NAQuadTreeSerializer)(void* buffer, const void* childdata);
typedef void* (*NAQuadTreeDeserializer)(NARecti rect, const void* buffer);

typedef struct NAQuadTreeCallbacks_struct{
  NAQuadTreeDataAllocator dataallocator;    // must be present.
  NAQuadTreeDataDestructor datadestructor;  // must be present.
  NAQuadTreeDataCopier datacopier;
  NAQuadTreeNodeAllocator nodeallocator;
  NAQuadTreeNodeDestructor nodedestructor;
  NAQuadTreeLeafChanged leafchanged;
  NAQuadTreeChildChanged childchanged;
  NAQuadTreeSerializer serialize;
  NAQuadTreeDeserializer deserialize;
} NAQuadTreeCallbacks;

// minchildsize denotes the minimal size of an element.
NA_IAPI NAQuadTree* naCreateQuadTree(NAQuadTree* tree, uint64 minchildsize, NAQuadTreeCallbacks callbacks);
NA_IAPI NAQuadTree* naCreateQuadTreeDuplicate(NAQuadTree* newtree, const NAQuadTree* duptree);
NA_IAPI NAQuadTree* naCreateQuadTreeDuplicateWithShift(NAQuadTree* newtree, NAQuadTree* duptree, NASizei shift);
NA_IDEF NAQuadTree* naCreateQuadTreeWithDeserialization(NAQuadTree* tree, const void* buf, NAQuadTreeCallbacks callbacks);
NA_IAPI void naEmptyQuadTree(NAQuadTree* tree);
NA_IAPI void naClearQuadTree(NAQuadTree* tree);
NA_IAPI void naDestroyQuadTree(NAQuadTree* tree);
NA_IAPI void naSerializeQuadTree(const NAQuadTree* tree, void* buf, uint64* bytesize);

NA_IAPI NABool naIsQuadTreeEmpty(NAQuadTree* tree);
NA_IAPI uint64 naGetQuadTreeMinChildSize(const NAQuadTree* tree);
NA_IDEF NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree);

NA_IAPI NABool naLocateQuadTreePosi(NAQuadTree* tree, NAPosi pos, NABool create);

// Returns the object currently looked at in the quadtree. If rect is not Null,
// it will be filled with the rect information of the object returned.
NA_IAPI const void* naGetQuadTreeConst(const NAQuadTree* tree, NARecti* rect);
NA_IAPI       void* naGetQuadTreeMutable(NAQuadTree* tree, NARecti* rect);

NA_IAPI        void naRemoveQuadTreeLeaf(NAQuadTree* tree);

// Creates a growing space containing all chunks which overlap with the given
// rect. The chunks are returned as NAMapi structures whereas the data pointer
// corresponds to the user data pointer of the leaf objects.
// The rect is considered to be filled. When create is set to NA_TRUE, any
// chunk overlapping with the given rect which was not created will be created
// after this function.
NA_IDEF NAGrowingSpace* naCreateGrowingSpaceWithQuadTreeChunks(NAGrowingSpace* space, NAQuadTree* tree);
NA_IDEF NAGrowingSpace* naCreateGrowingSpaceWithQuadTreeChunksInRecti(NAGrowingSpace* space, NAQuadTree* tree, NARecti rect, NABool create);

// Starts at the current object and traverses upwards the tree by calling
// the appropriate callback functions.
NA_IAPI void naUpdateQuadTree(NAQuadTree* tree);
NA_IAPI void* naGetQuadTreeRootNodeData(NAQuadTree* tree);

// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


typedef struct NAQuadTreeNode NAQuadTreeNode;
// Prototype
NA_HIAPI void naDestroyQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node);
NA_HIAPI void naUpdateQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* curnode, NAPosi curpos);
NA_HIAPI void naUpdateQuadTreeLeaf(NAQuadTree* tree, NAQuadTreeNode* leafparent, NAInt segment);


#define NA_QUADTREE_CHILD_IS_LEAF 0x01

struct NAQuadTreeNode{
  NAQuadTreeNode* parentnode;
  void* child[4];
  void* nodedata;
  NARecti childrect[4];
  int childflag[4];
};

struct NAQuadTree{
  uint64 minchildsize;
  NAQuadTreeCallbacks callbacks;
  NAQuadTreeNode* root;
  NAQuadTreeNode* curnode;
  NAUInt cursegment;
};



// Allocates and initializes a new and empty QuadTree node. The rects of all
// childs are initialized but no objects are allocated.
NA_HIDEF NAQuadTreeNode* naAllocateQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* parentnode, NAPosi origin, uint64 childsize){
  NAQuadTreeNode* node = naAllocNALibStruct(NA_NULL, NAQuadTreeNode);
  node->parentnode = parentnode;
  node->childrect[0] = naMakeRecti(naMakePosi(origin.x, origin.y), naMakeSizei(childsize, childsize));
  node->child[0] = NA_NULL;
  node->childflag[0] = 0;
  node->childrect[1] = naMakeRecti(naMakePosi(origin.x + childsize, origin.y), naMakeSizei(childsize, childsize));
  node->child[1] = NA_NULL;
  node->childflag[1] = 0;
  node->childrect[2] = naMakeRecti(naMakePosi(origin.x, origin.y + childsize), naMakeSizei(childsize, childsize));
  node->child[2] = NA_NULL;
  node->childflag[2] = 0;
  node->childrect[3] = naMakeRecti(naMakePosi(origin.x + childsize, origin.y + childsize), naMakeSizei(childsize, childsize));
  node->child[3] = NA_NULL;
  node->childflag[3] = 0;
  if(tree->callbacks.nodeallocator){
    node->nodedata = tree->callbacks.nodeallocator(naMakeRecti(naMakePosi(origin.x, origin.y), naMakeSizei(childsize*2, childsize*2)));
  }else{
    node->nodedata = NA_NULL;
  }
  return node;
}


NA_HIDEF NAQuadTreeNode* naDuplicateQuadTreeNode(NAQuadTree* newtree, NAQuadTreeNode* parentnode, NAQuadTreeNode* oldnode, NAQuadTree* contenttree){
  NAQuadTreeNode* newnode = naAllocateQuadTreeNode(newtree, parentnode, oldnode->childrect[0].pos, oldnode->childrect[0].size.width);
  newnode->parentnode = parentnode;
  for(NAInt s=0; s<4; s++){
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
NA_HIDEF NAUInt naGetQuadTreeNodeSegment(NAQuadTreeNode* node, NAPosi pos){
  NAUInt segment = 0;
  if(pos.x >= node->childrect[3].pos.x){segment |= 1;}
  if(pos.y >= node->childrect[3].pos.y){segment |= 2;}
  #ifndef NDEBUG
    if(!naIsPosiInRecti(pos, node->childrect[segment]))
      naError("naGetQuadTreeNodeSegment", "node does not actually contains pos");
  #endif
  return segment;
}



NA_HIDEF void naDestroyQuadTreeNodeSegment(NAQuadTree* tree, NAQuadTreeNode* node, NAUInt segment){
  #ifndef NDEBUG
    if(!node->child[segment])
      naError("naDestroyQuadTreeNodeSegment", "Segment of node does not exists");
  #endif
  if(node->childflag[segment] & NA_QUADTREE_CHILD_IS_LEAF){
    tree->callbacks.datadestructor(node->child[segment]);
  }else{
    naDestroyQuadTreeNode(tree, node->child[segment]);
  }
}



NA_HIDEF void naDestroyQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
  if(node->child[0]){naDestroyQuadTreeNodeSegment(tree, node, 0);}
  if(node->child[1]){naDestroyQuadTreeNodeSegment(tree, node, 1);}
  if(node->child[2]){naDestroyQuadTreeNodeSegment(tree, node, 2);}
  if(node->child[3]){naDestroyQuadTreeNodeSegment(tree, node, 3);}
  if(tree->callbacks.nodedestructor){tree->callbacks.nodedestructor(node->nodedata);}
  free(node);
}



NA_IDEF NAQuadTree* naCreateQuadTree(NAQuadTree* tree, uint64 minchildsize, NAQuadTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!callbacks.dataallocator)
      naError("naCreateQuadTree", "Must have a data allocator");
    if(!callbacks.datadestructor)
      naError("naCreateQuadTree", "Must have a data destructor");
  #endif
  tree = naAllocNALibStruct(tree, NAQuadTree);
  tree->minchildsize = minchildsize;
  tree->root = NA_NULL;
  tree->curnode = NA_NULL;
  tree->cursegment = 0;
  naCpyn(&(tree->callbacks), &callbacks, sizeof(NAQuadTreeCallbacks));
  return tree;
}


NA_IAPI NAQuadTree* naCreateQuadTreeDuplicate(NAQuadTree* newtree, const NAQuadTree* duptree){
  newtree = naCreateQuadTree(newtree, duptree->minchildsize, duptree->callbacks);
  if(duptree->root){
    newtree->root = naDuplicateQuadTreeNode(newtree, NA_NULL, duptree->root, NA_NULL); // todo const safe
    
    NAGrowingSpace leafs;
    naCreateGrowingSpaceWithQuadTreeChunks(&leafs, (NAQuadTree*)newtree);  // todo const save
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



NA_IAPI NAQuadTree* naCreateQuadTreeDuplicateWithMaskTree(NAQuadTree* newtree, NAQuadTree* duptree, NAQuadTree* masktree){
  newtree = naCreateQuadTree(newtree, duptree->minchildsize, duptree->callbacks);
  if(masktree->root){
    newtree->root = naDuplicateQuadTreeNode(newtree, NA_NULL, masktree->root, duptree);
  }
  return newtree;
}



NA_IAPI NAQuadTree* naCreateQuadTreeDuplicateWithShift(NAQuadTree* newtree, NAQuadTree* duptree, NASizei shift){
  #ifndef NDEBUG
    if(!duptree->callbacks.datacopier)
      naError("naCreateQuadTreeDuplicateWithShift", "Data copier callback required.");
  #endif
  newtree = naCreateQuadTree(newtree, duptree->minchildsize, duptree->callbacks);
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


NA_IDEF NAQuadTree* naCreateQuadTreeWithDeserialization(NAQuadTree* tree, const void* buf, NAQuadTreeCallbacks callbacks){
  #ifndef NDEBUG
    if(!(callbacks.deserialize))
      naError("naCreateQuadTreeWithDeserialization", "Callbacks required for deserialization");
  #endif
  const NAByte* dataptr = buf;
  uint64 datasize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  uint64 minchildsize = *((const uint64*)dataptr); dataptr += sizeof(uint64);
  tree = naCreateQuadTree(tree, minchildsize, callbacks);
  datasize -= 2 * sizeof(uint64);
  while(datasize){
    int64 posx = *((const int64*)dataptr); dataptr += sizeof(int64);
    int64 posy = *((const int64*)dataptr); dataptr += sizeof(int64);
    datasize -= 2 * sizeof(int64);
    // Create the leaf temporarily but delete it immediately and recreate it
    // as a deserialization.
    naLocateQuadTreePosi(tree, naMakePosi(posx, posy), NA_TRUE);
    callbacks.datadestructor(tree->curnode->child[tree->cursegment]);
    tree->curnode->child[tree->cursegment] = callbacks.deserialize(naMakeRecti(naMakePosi(posx, posy), naMakeSizei(minchildsize, minchildsize)), dataptr);
    naUpdateQuadTreeLeaf(tree, tree->curnode, tree->cursegment);
    uint64 bytesread = callbacks.serialize(NA_NULL, tree->curnode->child[tree->cursegment]);
    dataptr += bytesread;
    datasize -= bytesread;
  }
  return tree;
}


NA_IDEF void naEmptyQuadTree(NAQuadTree* tree){
  naClearQuadTree(tree);
  tree->root = NA_NULL;
  tree->curnode = NA_NULL;
  tree->cursegment = 0;
}



NA_IDEF void naClearQuadTree(NAQuadTree* tree){
  if(tree->root){naDestroyQuadTreeNode(tree, tree->root);}
}



NA_IDEF void naDestroyQuadTree(NAQuadTree* tree){
  naClearQuadTree(tree);
  free(tree);
}



NA_IAPI void naSerializeQuadTree(const NAQuadTree* tree, void* buf, uint64* bytesize){
  #ifndef NDEBUG
    if(!(tree->callbacks.serialize))
      naError("naCreateQuadTreeWithDeserialization", "Callbacks required for serialization");
    if(!bytesize)
      naError("naCreateQuadTreeWithDeserialization", "bytesize required to read/store byte size");
    if(buf && (*bytesize == 0))
      naError("naCreateQuadTreeWithDeserialization", "bytesize is zero");
  #endif
  NAGrowingSpace leafs;
  naCreateGrowingSpaceWithQuadTreeChunks(&leafs, (NAQuadTree*)tree);  // todo const save
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
NA_HIDEF NABool naLocateQuadTreeNodePosi(NAQuadTree* tree, NAQuadTreeNode* node, NAPosi pos, NABool create){
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
          node->child[segment] = naAllocateQuadTreeNode(tree, node, node->childrect[segment].pos, node->childrect[segment].size.width / 2);
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
        node->parentnode = naAllocateQuadTreeNode(tree, NA_NULL, alignrect.pos, noderect.size.width);
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



NA_IDEF NABool naIsQuadTreeEmpty(NAQuadTree* tree){
  return (tree->root == NA_NULL);
}



NA_IDEF uint64 naGetQuadTreeMinChildSize(const NAQuadTree* tree){
  return tree->minchildsize;
}


NA_IDEF NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree){
  return tree->callbacks;
}



// Returns true if there already was a content.
NA_IDEF NABool naLocateQuadTreePosi(NAQuadTree* tree, NAPosi pos, NABool create){
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
      tree->root = naAllocateQuadTreeNode(tree, NA_NULL, origin, tree->minchildsize);
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




NA_HIDEF const void* naGetQuadTreeConst(const NAQuadTree* tree, NARecti* rect){
  if(tree->curnode){
    if(rect){*rect = tree->curnode->childrect[tree->cursegment];}
    return tree->curnode->child[tree->cursegment];
  }else{
    return NA_NULL;
  }
}


NA_HIDEF void* naGetQuadTreeMutable(NAQuadTree* tree, NARecti* rect){
  if(tree->curnode){
    if(rect){*rect = tree->curnode->childrect[tree->cursegment];}
    return tree->curnode->child[tree->cursegment];
  }else{
    return NA_NULL;
  }
}


NA_HIDEF void naRemoveQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* node){
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
  naDestroyQuadTreeNode(tree, node);
}



NA_IDEF void naRemoveQuadTreeLeaf(NAQuadTree* tree){
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



NA_IDEF NAGrowingSpace* naCreateGrowingSpaceWithQuadTreeChunks(NAGrowingSpace* space, NAQuadTree* tree){
  naCreateGrowingSpace(space, sizeof(NAMapi), NA_NULL);
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



NA_IDEF NAGrowingSpace* naCreateGrowingSpaceWithQuadTreeChunksInRecti(NAGrowingSpace* space, NAQuadTree* tree, NARecti rect, NABool create){
  NARecti alignrect = naMakeRecti(naMakePosi(0, 0), naMakeSizei(tree->minchildsize, tree->minchildsize));
  NAPosi alignpos1 = naMakePosiWithAlignment(rect.pos, alignrect);
  NAPosi alignpos2 = naMakePosiWithAlignment(naGetRectiMaxPos(rect), alignrect);
  naCreateGrowingSpace(space, sizeof(NAMapi), NA_NULL);
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



NA_HIDEF void naUpdateQuadTreeNode(NAQuadTree* tree, NAQuadTreeNode* curnode, NAPosi curpos){
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


NA_HIDEF void naUpdateQuadTreeLeaf(NAQuadTree* tree, NAQuadTreeNode* leafparent, NAInt segment){
  if(tree->callbacks.leafchanged){
    NABool bubble = tree->callbacks.leafchanged(leafparent->nodedata, segment, leafparent->child);
    if(bubble){naUpdateQuadTreeNode(tree, leafparent, leafparent->childrect[segment].pos);}
  }
}


NA_IDEF void naUpdateQuadTree(NAQuadTree* tree){
  if(tree->curnode){naUpdateQuadTreeLeaf(tree, tree->curnode, tree->cursegment);}
}



NA_IDEF void* naGetQuadTreeRootNodeData(NAQuadTree* tree){
  if(tree->root){return tree->root->nodedata;}
  return NA_NULL;
}



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_QUADTREE_INCLUDED

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
