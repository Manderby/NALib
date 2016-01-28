
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_QUADTREE_INCLUDED
#define NA_QUADTREE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NACoord.h"

// An NAQuadTree is a container struct capable of storing a two-dimensional
// object of any size. The content of the object is divided into equally sized
// quadratic chunks with a given minimal size. These chunks are the leafs of
// the tree. Aside from leafes, an NAQuadTree stores parent nodes. Every leaf
// has a parent node and every node may have another parent node.
//
// The child nodes are sometimes referred to as segments which are stored
// internally in the following order:
// (lower x, lower y)
// (upper x, lower y)
// (lower x, upper y)
// (upper x, upper y)
//
// There are two ways of accessing and manipulating the tree: Direct access of
// the leafes upon coordinate location or iterating over all leaves. The tree
// uses some internal variables to make the search and iteration fast.
//
// Manipulating an NAQuadTree is done in the leafes. When a leaf has been
// altered, a message is sent to the parent nodes. This is called bubbling.
// You can intercept these bubbling messages implementing the appropriate
// callback functions.
//
// Regarding Const and Mutable: This datastructure can not be manipulated like
// other datastructures from NALib. You do not add single elements but instead,
// the tree asks you to provide chunks when necessary. Therefore, the data
// stored is always mutable. You can nontheless access the tree const or
// mutable. This is important when for example, if you just have a const tree
// but require a chunk of it for reading.


// ///////////////////////////////////////////
// Various callback functions which you can give to a tree.
// ///////////////////////////////////////////

// Allocation function which allocate your leaf-chunks. This callback always
// must be present. Returning Null is a bad thing. This callback will be called
// when creating new chunks (whenever a NAQuadTree gets expanded) or when
// duplicating an NAQuadTree.
// The parameters given are the origin of the data as well as the (always
// square) leafsize of the data rectangle. When copydata is non-Null, it points
// to already existing chunk data which shall be copied to the new chunk. This
// will be the case when duplicating an NAQuadTree.
// The userdata parameter contains whatever you defined upon creation of the
// tree within the NAQuadTreeCallbacks struct. With this, it is for example
// possible to allocate data from a user defined pool-structure.
// Note that both origin and leafsize are just here for information. You may or
// may not use them. And you will probably not need to store them. But you may
// if you really want to.
// The leafsize given will always be the leafsize declared at creation of an
// NAQuadTree.
// Also note that the coordinates of origin are guaranteed to be a multiple of
// leafsize.
typedef void* (*NAQuadTreeLeafAllocator)(   NAPosi origin,
                                            NAUInt leafsize,
                                             void* userdata,
                                       const void* copydata);

// The destruction function of your leaf-chunks. This callback always must be
// present. The pointer given is a pointer created when NAQuadTreeLeafAllocator
// was called. The userdata is the same as desribed in the allocator function.
typedef void  (*NAQuadTreeLeafDeallocator)(   void* leafdata,
                                              void* userdata);

// This callback is required when shifting an NAQuadTree. You are given two
// chunk data pointers, two origins and a size.
// Your task is to copy the data in copydata starting at copyorigin to dstdata
// starting at dstorigin. The size denotes the size of the rectangle which
// shall be copied. The origins are given relative to the chunk origin, meaning
// no matter where the actual origin of the chunk is, the given origins will
// always be greaterequal zero. Additionally, all coordinates of the two
// rectangles are guaranteed to be inside the corresopnding chunk. Note that
// the size does not necessarily denotes a square. It can denote any rectangle.
// The leafsize is given to you as an additional information. It corresponds
// to the leafsize used when the NAQuadTree was created.
typedef void  (*NAQuadTreeDataCopier)( const void* dstdata,
                                            NAPosi dstorigin,
                                       const void* copydata,
                                            NAPosi copyorigin,
                                           NASizei size,
                                            NAUInt leafsize);

// This callback is called when a quad tree creates a node other than a leaf.
// Nodes can not be directly manipulated but you are allowed to store any data
// with every node if desired. The origin parameter denotes the origin of all
// leafes this node is coprised of and the nodesize denotes the (always square)
// rectangle all leafes are contained in.
// Note that both origin and nodesize are just here for information. You may or
// may not use them. And you will probably not need to store them. But you may
// if you really want to.
// You can return a pointer to any data, even a Null pointer. This data pointer
// will be available to NAQuadTreeLeafChanged, NAQuadTreeChildChanged and
// NAQuadTreeNodeDestructor.
typedef void* (*NAQuadTreeNodeAllocator)(   NAPosi origin,
                                            NAUInt nodesize);

// The node destructor is called before a quad tree ultimately deletes a node.
// The pointer created with NAQuadTreeNodeAllocator will be sent to this
// function such that you can deallocate the memory if necessary. Note that
// this function is NOT called if a node stores a NULL pointer.
typedef void  (*NAQuadTreeNodeDestructor)(   void* nodedata);

// Whenever you altered a leaf and want to propagate the change over the whole
// tree, you call naUpdateQuadTreeCurrent which in turn will call the following
// two callback functions. Both will be called with the parental node data you
// may have stored with NAQuadTreeNodeAllocator. Additionally, you are given
// all four child data pointers as an array as well as a segment index denoting
// which of the four childs has caused the calling.
//
// The two functions differ on whether the childs are leafs or nodes. The data
// given are either pointers to leaf chunk data or pointers to node data you
// stored with NAQuadTreeNodeAllocator. All individual pointers can be Null
// meaning there is no leaf stored or Null has explicitely been stored as a
// node.
//
// Segment is a number in [0 - 3] denoting the child which has been changed.
// The value -1 is given if there was no particular child causing the trouble.
// This happens for example, when a tree gets copied as a whole.
//
// This function can return NA_TRUE if the message shall be propagated towards
// the next parent (bubbling). If NA_FALSE is returned, the tree update stops
// immediately. This return value though is ignored when copying a whole tree,
// where these callbacks are called whenever a node has been fully constructed.
typedef NABool(*NAQuadTreeLeafChanged)(      void* nodedata,
                                             NAInt segment,
                                 const void* const leafdata[4]);
typedef NABool(*NAQuadTreeChildChanged)(     void* nodedata,
                                             NAInt segment,
                                 const void* const childdata[4]);

// This callback is called when the data of a leaf chunk shall be serialized.
// You must always return the number of bytes required to store the chunk data.
// You may use the leafsize given for the calculation of that size. The
// leafsize always is the leafsize declared upon creation of the NAQuadTree.
//
// When buffer is a Null pointer, just return the require size. When buffer is
// NOT a Null-Pointer, this buffer is guaranteed to have the required size
// and you can copy the data to the buffer.
//
// Therefore, when you call naSerializeQuadTree, this callback will be called
// for every chunk twice: First to akquire the required size and second to
// actually store the data.
typedef uint64(*NAQuadTreeSerializer)        (void* buffer,
                                        const void* leafdata,
                                             NAUInt leafsize);

// This callback is called when a buffer shall be deserialized which
// previously had been serialized with NAQuadTreeSerializer. Create a leaf
// chunk and return a pointer to the chunk data. The origin and (always
// quadratic) leafsize are given to you as an additional information.
//
// Note that this callback is very similar to NAQuadTreeLeafAllocator but
// here, buffer denotes not a chunk which shall be copied but rather more a
// buffer which need to be deserialized. These can be two different things.
typedef void* (*NAQuadTreeDeserializer)(     NAPosi origin,
                                             NAUInt leafsize,
                                        const void* buffer);




// This is the callback struct you can use to create an NAQuadTree. After
// creation, these functions can no longer be changed.
typedef struct NAQuadTreeCallbacks_struct{
  NAQuadTreeLeafAllocator   leafallocator;    // must be present.
  NAQuadTreeLeafDeallocator leafdeallocator;   // must be present.
  NAQuadTreeDataCopier      datacopier;
  NAQuadTreeNodeAllocator   nodeallocator;
  NAQuadTreeNodeDestructor  nodedestructor;
  NAQuadTreeLeafChanged     leafchanged;
  NAQuadTreeChildChanged    childchanged;
  NAQuadTreeSerializer      serialize;
  NAQuadTreeDeserializer    deserialize;
  void*                     userdata;         // custom pointer sent to
                                              // various callback functions.
} NAQuadTreeCallbacks;

// A typedef of an opaque type not accessible to the programmer.
typedef struct NAQuadTreeNode NAQuadTreeNode;

// The typedef of the NAQuadTree.
typedef struct NAQuadTree NAQuadTree;
struct NAQuadTree{
  NAUInt leafsize;
  NAQuadTreeCallbacks callbacks;
  NAQuadTreeNode* root;
  NAQuadTreeNode* visitnode;    // visitnode stores tha last node visited. This
  NAQuadTreeNode* curnode;      // Makes searching in the tree very fast.
  NAUInt cursegment;            // curnode and cursegment are for the iteration
};




// Initializes an empty NAQuadTree with the given leafsize and callbacks.
NA_API NAQuadTree* naInitQuadTree(              NAQuadTree* tree,
                                                     NAUInt leafsize,
                                        NAQuadTreeCallbacks callbacks);

// Duplicates the given duptree.
NA_API NAQuadTree* naCopyQuadTree(              NAQuadTree* newtree,
                                          const NAQuadTree* duptree);

// Duplicates the given duptree, but only containing the chunks which are
// present in masktree.
NA_DEF NAQuadTree* naCopyQuadTreeWithMaskTree(  NAQuadTree* newtree,
                                          const NAQuadTree* duptree,
                                          const NAQuadTree* masktree);

// Duplicates the given duptree but shifts all contents by the given shift.
NA_API NAQuadTree* naCopyQuadTreeWithShift(     NAQuadTree* newtree,
                                          const NAQuadTree* duptree,
                                                    NASizei shift);

// Serializes a full NAQuadTree.
// When buf is Null, the total amount of bytes required for the whole tree
// including all structural data is returned in bytesize.
// When buf is not Null, the tree serializes into the given buffer. You must
// send the bytesize you received from the first call again!
NA_API void naSerializeQuadTree(            const NAQuadTree* tree,
                                                        void* buf,
                                                      uint64* bytesize);
// Deserializes a full NAQuadTree
NA_API NAQuadTree* naInitQuadTreeWithDeserialization(NAQuadTree* tree,
                                                const void* buf,
                                        NAQuadTreeCallbacks callbacks);

// Empties the tree
NA_API void naEmptyQuadTree(                    NAQuadTree* tree);

// Clears the tree
NA_API void naClearQuadTree(                    NAQuadTree* tree);

// Returns true if the tree is empty
NA_API NABool naIsQuadTreeEmpty(                NAQuadTree* tree);

// Returns the leaf size
NA_API uint64 naGetQuadTreeLeafSize(      const NAQuadTree* tree);

// Returns the callbacks (by copying the function pointers)
NA_API NAQuadTreeCallbacks naGetQuadTreeCallbacks(const NAQuadTree* tree);

// Returns the chunk containing the given coord. If no chunk with the desired
// coord exists, NA_NULL is returned. When create is true, the chunk will be
// created and returned. If the origin pointer is not Null it will be filled
// with the origin information of the chunk returned. When create is true,
// didcreate returns whether the leaf has indeed been created or not.
NA_API const void* naGetQuadTreeLeafConst(  const NAQuadTree* tree,
                                                      NAPosi  coord,
                                                      NAPosi* origin);
NA_API       void* naGetQuadTreeLeafMutable(      NAQuadTree* tree,
                                                      NAPosi  coord,
                                                      NABool  create,
                                                     NABool*  didcreate,
                                                      NAPosi* origin);


// ////////////////////////////////////
// Iteration functions
//
// Every tree has an internal pointer denoting the current leaf. The
// programmer can control and access this leaf with iteration functions.
// If no current leaf is set, NA_NULL is returned as a pointer. The easiest
// way to implement an iteration is using a while loop:
//
// NAQuadTree* mytree;
// void* curleaf;
// naFirstQuadTree(mytree);
// while((curleaf = naIterateQuadTreeMutable(mytree))){
//   Do stuff with curleaf.
// }
//
// You should enclose the while-condition in additional parantheses such that
// a compiler knows that the returned pointer must be evaluated as a condition.
//
// Note: You can safely use the remove function on curleaf while iterating.
//
// When being inside the while scope, the list itself already points to the
// leaf AFTER iteration.
//
// Do NOT use a for-loop for iteration! The internal pointer may, depending on
// how you write the for-loop, not point to the leaf AFTER iteration which
// can be devastating when removing leafs. Also, it is very hard to read.
// ////////////////////////////////////

// Moves the internal pointer to the first most leaf.
NA_API        void naFirstQuadTree(         const NAQuadTree* tree);
// Returns the current leaf and moves the internal pointer to the next leaf.
// If there is no leaf available, the function will return NA_NULL.
// The origin of the current leaf will be stored in origin when the argument
// is not Null.
NA_API const void* naIterateQuadTreeConst(       const NAQuadTree* tree,
                                                      NAPosi* origin);
NA_API void* naIterateQuadTreeMutable(           NAQuadTree* tree,
                                                      NAPosi* origin);

// Same as above but only iterates upon leafs which overlap with the given rect.
NA_API void naFirstQuadTreeInRect(          const NAQuadTree* tree,
                                                      NARecti rect);
NA_API const void* naIterateQuadTreeInRectConst( const NAQuadTree* tree,
                                                      NAPosi* origin,
                                                      NARecti rect);
NA_API void* naIterateQuadTreeInRectMutable(     NAQuadTree* tree,
                                                      NAPosi* origin,
                                                      NARecti rect);

// Function pointer used for the set iteration naSetQuadTreeInRect. This
// callback is called for multiple chunks of a tree.
// When called, dstdata is the chunk data, rect is the rect within the data
// shall be set and leafsize is the leaf size given upon creation of the tree.
// The userdata corresponds to whatever has been given in naSetQuadTreeInRect.
// The origin of the rect is given relative to the origin of the chunk.
// Therefore, all positions are greaterequal zero. All coordinates of rect are
// guaranteed to be inside the leaf.
typedef void  (*NAQuadTreeDataSetter)( const void* dstdata,
                                           NARecti rect,
                                            NAUInt leafsize,
                                       const void* userdata);

// Calls the given NAQuadTreeDataSetter function for every chunk in the tree
// which is partially or fully overlapped with the given rect. Will create
// the chunks if necessary.
NA_API void naSetQuadTreeInRect(          NAQuadTree* tree,
                                              NARecti rect,
                                 NAQuadTreeDataSetter datasetter,
                                          const void* userdata);

// Removes the leaf currently looked at.
// Beware, after this function, the tree forgets its last visit and must
// search anew.
NA_API        void naRemoveQuadTreeLeaf(NAQuadTree* tree, NAPosi coord);


// Starts at the leaf containing the given coord and traverses upwards
// (bubbling) the tree by calling the appropriate callback functions of all
// nodes. When the callback functions return NA_FALSE, bubbling stops.
NA_API void naUpdateQuadTreeAtCoord(NAQuadTree* tree, NAPosi coord);
// Same thing but updates the whole tree. All messages will bubble as long
// as there is at least one child per node requesting a bubble.
NA_API void naUpdateQuadTree(NAQuadTree* tree);

// Returns the root node data, if available. If no root or no root data are
// available, NA_NULL is returned.
NA_API void* naGetQuadTreeRootNodeData(NAQuadTree* tree);







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
