
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

// NAQuadTreeNodeAllocator is called when a quad tree creates a node. Nodes
// can not be directly manipulated by the NAQuadTree API but you are allowed
// to store any data with every node. The rect parameter denots the rect this
// node and all of its child span. You can return a pointer to any data. This
// data pointer will be sent to NAQuadTreeLeafChanged, NAQuadTreeChildChanged
// and NAQuadTreeNodeDestructor.
typedef void* (*NAQuadTreeNodeAllocator)(NARecti rect);

// The node destructor is called before a quad tree deletes a node. The pointer
// created with NAQuadTreeNodeAllocator will be sent to this function such that
// you can deallocate the memory if necessary. Note that this function is NOT
// called if a node stores a NULL pointer.
typedef void  (*NAQuadTreeNodeDestructor)(void* ptr);


// Various callback functions which you can give to a tree.
// Allocation and destruction function pointers which allocate your leaf-
// objects for the given rect. Returning Null is a bad thing.
typedef void* (*NAQuadTreeDataAllocator)(NARecti rect, const void* copydata);
typedef void  (*NAQuadTreeDataDestructor)(void* ptr);
typedef void  (*NAQuadTreeDataCopier)(const void* dstdata, NARecti dstrect, const void* copydata, NARecti copyrect);
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
NA_API NAQuadTree* naInitQuadTree(NAQuadTree* tree, NAInt minchildsize, NAQuadTreeCallbacks callbacks);
NA_API NAQuadTree* naCopyQuadTree(NAQuadTree* newtree, const NAQuadTree* duptree);
NA_DEF NAQuadTree* naCopyQuadTreeWithMaskTree(NAQuadTree* newtree, NAQuadTree* duptree, NAQuadTree* masktree);
NA_API NAQuadTree* naCopyQuadTreeWithShift(NAQuadTree* newtree, NAQuadTree* duptree, NASizei shift);
NA_API NAQuadTree* naInitQuadTreeWithDeserialization(NAQuadTree* tree, const void* buf, NAQuadTreeCallbacks callbacks);
NA_API void naEmptyQuadTree(NAQuadTree* tree);
NA_API void naClearQuadTree(NAQuadTree* tree);
NA_API void naSerializeQuadTree(const NAQuadTree* tree, void* buf, uint64* bytesize);

NA_API NABool naIsQuadTreeEmpty(NAQuadTree* tree);
NA_API uint64 naGetQuadTreeMinChildSize(const NAQuadTree* tree);
NA_API NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree);

NA_API NABool naLocateQuadTreePosi(NAQuadTree* tree, NAPosi pos, NABool create);

// Returns the object currently looked at in the quadtree. If rect is not Null,
// it will be filled with the rect information of the object returned.
NA_API const void* naGetQuadTreeConst(const NAQuadTree* tree, NARecti* rect);
NA_API       void* naGetQuadTreeMutable(NAQuadTree* tree, NARecti* rect);

NA_API        void naRemoveQuadTreeLeaf(NAQuadTree* tree);

// Creates a growing space containing all chunks which overlap with the given
// rect. The chunks are returned as NAMapi structures whereas the data pointer
// corresponds to the user data pointer of the leaf objects.
// The rect is considered to be filled. When create is set to NA_TRUE, any
// chunk overlapping with the given rect which was not created will be created
// after this function.
NA_API NAGrowingSpace* naInitGrowingSpaceWithQuadTreeChunks(NAGrowingSpace* space, NAQuadTree* tree);
NA_API NAGrowingSpace* naInitGrowingSpaceWithQuadTreeChunksInRecti(NAGrowingSpace* space, NAQuadTree* tree, NARecti rect, NABool create);

// Starts at the current object and traverses upwards the tree by calling
// the appropriate callback functions.
NA_API void naUpdateQuadTree(NAQuadTree* tree);
NA_API void* naGetQuadTreeRootNodeData(NAQuadTree* tree);

// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


typedef struct NAQuadTreeNode NAQuadTreeNode;


#define NA_QUADTREE_CHILD_IS_LEAF 0x01

struct NAQuadTreeNode{
  NAQuadTreeNode* parentnode;
  void* child[4];
  int childflag[4];
  NARecti childrect[4];
  void* nodedata;
};

struct NAQuadTree{
  NAInt minchildsize;
  NAQuadTreeCallbacks callbacks;
  NAQuadTreeNode* root;
  NAQuadTreeNode* curnode;
  NAUInt cursegment;
};




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
