
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_OCTTREE_INCLUDED
#define NA_OCTTREE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NACoord.h"
#include "NAMemory.h"

// An NAOctTree is a container struct capable of storing a three-dimensional
// object of any bytesize. The content of the object is divided into equally
// sized cubic chunks with a given minimal length in one dimension. These
// chunks are the leafs of the tree. Aside from leafes, an NAOctTree stores
// parent nodes. Every leaf has a parent node and every node may have another
// parent node.
//
// The child nodes are sometimes referred to as segments which are stored
// internally in the following order:
// segment 0: (lower x, lower y, lower z)
// segment 1: (upper x, lower y, lower z)
// segment 2: (lower x, upper y, lower z)
// segment 3: (upper x, upper y, lower z)
// segment 4: (lower x, lower y, upper z)
// segment 5: (upper x, lower y, upper z)
// segment 6: (lower x, upper y, upper z)
// segment 7: (upper x, upper y, upper z)
//
// Access and manipulation of the tree is performed using iterators.
// Creating chunks and modifying the tree happends using callback functions.
//
// This datastructure can not be manipulated like other datastructures from
// NALib. The programmer does not add single elements but instead, the tree
// calls a callback function to provide chunks when necessary. Therefore, the
// data stored is always mutable. You can nontheless access the tree const or
// mutable. This is important when for example, if you just have a const tree
// but require a chunk of it for reading.
//
// Manipulating an NAOctTree is done in the leafes. When a leaf has been
// altered, a message is sent to the parent nodes. This is called bubbling.
// You can intercept these bubbling messages implementing the appropriate
// callback functions.



// ///////////////////////////////////////////
// CALLBACKS
// ///////////////////////////////////////////

// When creating an NAOctTree, you can create various callbacks which are
// stored in an NAOctTreeCallbacks structure. Here are the signatures for 
// these callbacks:

// NAOctTreeLeafAllocator
// Allocation function which allocate your leaf-chunks. This callback always
// must be present and shall always return something. Returning Null is a bad
// thing. This callback will be called when creating new chunks (whenever a
// NAOctTree gets expanded) or when duplicating an NAOctTree.
// The parameters given are the origin of the data as well as the (always
// cubic) leaflength of the data box. When copydata is non-Null, it
// points to already existing chunk data which shall be copied to the new
// chunk. This will be the case when duplicating an NAOctTree.
// The userdata parameter contains whatever you defined upon creation of the
// tree within the NAOctTreeCallbacks struct. With this, it is for example
// possible to allocate data from a user defined pool-structure.
// Note that both origin and leaflength are just here for information. You may
// or may not use them. And you will probably not need to store them. But you
// may if you really want to.
// The leaflength given will always be the leaflength declared at creation of
// an NAOctTree.
// Also note that the coordinates of origin are guaranteed to be a multiple of
// leaflength.
typedef void* (*NAOctTreeLeafAllocator)( NAVertexi origin,
                                             NAInt leaflength,
                                             void* userdata,
                                       const void* copydata);

// NAOctTreeLeafDeallocator
// The deallocation function of your leaf-chunks. This callback always must be
// present. The pointer given is a pointer created when NAOctTreeLeafAllocator
// was called. The userdata is the same as desribed in the allocator function.
typedef void  (*NAOctTreeLeafDeallocator)(  void* leafdata,
                                            void* userdata);

// NAOctTreeDataCopier
// This callback is required when shifting an NAOctTree. You are given two
// chunk data pointers, two origins and a volume.
// Your task is to copy the data in srcdata starting at srcorigin to dstdata
// starting at dstorigin. The volume denotes the volume of the box which
// shall be copied. The origins are given relative to the chunk origin, meaning
// no matter where the actual origin of the chunk is, the given origins will
// always be greaterequal zero. Additionally, all coordinates of the two
// boxes are guaranteed to be inside the corresopnding chunk. Note that
// the volume does not necessarily denotes a cube. It can denote any cuboid.
// The totalshift and the leaflength is given to you as an additional
// information. The totalsize corresponds to the shift given as a parameter to
// naInitOctTreeCopyShifted and the leaflength corresponds to the leaflength
// used when the NAOctTree was created.
// Example in 2D (numbers are approximate):
//          dstdata           srcdata
//        O---------+       O---------+       
//        |         |       |s----+   |       srcorigin s: (1,1,2)
//        |    d----+       ||    |   |       dstorigin d: (3,2,5)
//        |    |    | <---- |+----+   |       size: (4,2)
//        |    +----+       |         |       totalshift: (10,9,19)
//        +---------+       +---------+       leaflength: 8

typedef void  (*NAOctTreeDataCopier)(  const void* dstdata,
                                         NAVertexi dstorigin,
                                       const void* srcdata,
                                         NAVertexi srcorigin,
                                         NAVolumei volume,
                                         NAVertexi totalshift,
                                             NAInt leaflength);

// NAOctTreeNodeAllocator
// This callback is called when a oct tree creates an internal tree node other
// than a leaf.
// Internal nodes can NOT be manipulated directly but you are allowed to store
// any data with every node if desired. You can return a pointer to any data,
// even a Null pointer. This data pointer will be available to
// NAOctTreeLeafChanged, NAOctTreeChildChanged and NAOctTreeNodeDeallocator.
// An internal node internally stores  pointers to childnodes or leafes. The
// origin parameter denotes the origin of the (lowerx, lowery)-node and the
// childnodesize denotes the (always cubic) box the childnodes have.
// Note that both origin and nodesize are just here for information. You may or
// may not use them. And you will probably not need to store them. But you may
// if you really want to.
typedef void* (*NAOctTreeNodeAllocator)( NAVertexi origin,
                                             NAInt childnodesize);

// NAOctTreeNodeDeallocator
// The node destructor is called before a oct tree ultimately deletes an
// internal node node.
// The pointer created with NAOctTreeNodeAllocator will be sent to this
// function such that you can deallocate the memory if necessary. Note that
// this function is NOT called if a node stores a NULL pointer.
typedef void  (*NAOctTreeNodeDeallocator)(void* nodedata);

// NAOctTreeLeafChanged and NAOctTreeChildChanged
// Whenever you altered a leaf and want to propagate the change over the whole
// tree, you call naUpdateOctTreeCur which in turn will call the following
// two callback functions. Both will be called with the PARENTAL (internal) node
// data you may have stored with NAOctTreeNodeAllocator. Additionally, you are
// given all eight child data pointers as an array as well as a segment index
// denoting which of the eight childs has caused the calling. See segment index
// description above. Plus the leaflength of a child. 
//
// The two functions differ on whether the childs are leafs or nodes. The data
// given are either pointers to leaf chunk data or pointers to node data you
// stored with NAOctTreeNodeAllocator. All individual pointers can be Null
// meaning there is no leaf stored or Null has explicitely been stored as a
// node. This means, these functions will be called EVEN IF Null was stored.
//
// Segment is a number in [0 - 7] denoting the child which caused the call.
// The value -1 is given if there was no particular child causing the call.
// This happens for example, when a tree gets copied as a whole or if the tree
// gets updated as a whole with naUpdateOctTree. In that case, probably, all
// leaves or childs should be considered important for whatever you do in the
// callback.
//
// This function can return NA_TRUE if the message shall be propagated towards
// the next parent (bubbling). If NA_FALSE is returned, the tree update stops.
// This return value though is ignored when copying a whole tree, where these
// callbacks are called whenever a node has been fully constructed.
//
// Note that these callbacks will only be called in the bubbling phase. Also
// note that if one of these functions is not implemented, bubbling is set to
// NA_TRUE by default.
typedef NABool(*NAOctTreeLeafChanged)(      void* nodedata,
                                            NAInt segment,
                                const void* const leafdata[8],
                                            NAInt leaflength);
typedef NABool(*NAOctTreeChildChanged)(     void* nodedata,
                                            NAInt segment,
                                const void* const childdata[8],
                                            NAInt leaflength);





// This is the callback struct you can use to create an NAOctTree. After
// creation, these functions can no longer be changed.
typedef struct NAOctTreeCallbacks_struct{
  NAOctTreeLeafAllocator   leafallocator;    // must be present.
  NAOctTreeLeafDeallocator leafdeallocator;  // must be present.
  NAOctTreeDataCopier      datacopier;
  NAOctTreeNodeAllocator   nodeallocator;
  NAOctTreeNodeDeallocator nodedeallocator;
  NAOctTreeLeafChanged     leafchanged;
  NAOctTreeChildChanged    childchanged;
  void*                    userdata;
} NAOctTreeCallbacks;



// Typedefs of opaque types which should not be visible to the programmer.
typedef struct NAOctTree NAOctTree;
typedef struct NAOctTreeIterator NAOctTreeIterator;




// //////////////////////////////
// FUNCTIONS
// //////////////////////////////

// Initializes an empty NAOctTree with the given leaflength and callbacks.
NA_API NAOctTree* naInitOctTree(               NAOctTree* tree,
                                                    NAInt leaflength,
                                       NAOctTreeCallbacks callbacks);

// Initializes newtree by duplicating the given copytree.
NA_API NAOctTree* naInitOctTreeCopy(            NAOctTree* newtree,
                                          const NAOctTree* copytree);

// Duplicates the given copytree, but only containing the chunks which are
// present in masktree.
NA_DEF NAOctTree* naInitOctTreeCopyMasked(      NAOctTree* newtree,
                                          const NAOctTree* copytree,
                                          const NAOctTree* masktree);

// Duplicates the given copytree but shifts all contents by the given shift.
NA_API NAOctTree* naInitOctTreeCopyShifted(     NAOctTree* newtree,
                                          const NAOctTree* copytree,
                                                 NAVertexi shift);

// Clears the tree
NA_API void naClearOctTree(                     NAOctTree* tree);



// //////////////////////////
// Various functions:

// Empties the tree
NA_API void naEmptyOctTree(                   NAOctTree* tree);

// Returns true if the tree is empty
NA_IAPI NABool naIsOctTreeEmpty(        const NAOctTree* tree);

// Returns the leaf length in one dimension
NA_IAPI NAInt naGetOctTreeLeafLength(   const NAOctTree* tree);

// Returns the adjusted coord which is aligned on a chunk coordinate.
NA_IAPI NAVertexi naGetOctTreeAlignedCoord(NAInt leaflength, NAVertexi coord);

// Returns the callbacks (by copying the function pointers)
NA_IAPI NAOctTreeCallbacks naGetOctTreeCallbacks(
                                         const NAOctTree* tree);

// Returns the root node data, if available. If no root or no root data are
// available, NA_NULL is returned.
NA_API void* naGetOctTreeRootNodeData(       NAOctTree* tree);

// Same thing as naUpdateOctTreeCur but is called for every leaf in the
// whole tree. All messages will bubble as long as there is at least one child
// per node requesting a bubble. Use naUpdateOctTreeCur if you want to
// update only one element.
NA_API void naUpdateOctTree(                  NAOctTree* tree);



// ////////////////////////
// Calling one and the same function on a large portion of the tree.

// Function pointer used for the set iteration naSetOctTreeInRect. This
// callback is called for multiple chunks of a tree.
// When called, dstdata is the chunk data, box is the box within the data
// shall be handeled and leaflength is the leaf length in one dimension given
// upon creation of the tree.
// The userdata corresponds to whatever has been given in naSetOctTreeInRect.
// The origin of the box is given relative to the origin of the chunk.
// Therefore, all positions are greaterequal zero. All coordinates of box are
// guaranteed to be inside the leaf.
typedef void  (*NAOctTreeDataSetter)(  const void* dstdata,
                                            NABoxi box,
                                             NAInt leaflength,
                                       const void* userdata);

// Calls the given NAOctTreeDataSetter function for every chunk in the tree
// which is partially or fully overlapped with the given box. Will create
// the leafes if necessary.
NA_API void naSetOctTreeInRect(           NAOctTree* tree,
                                              NABoxi box,
                                 NAOctTreeDataSetter datasetter,
                                         const void* userdata);



// ////////////////////////////////////
// Iterators
//
// You can iterate over every leaf of a whole tree like this:
//
// NAOctTreeIterator iter = naMakeOctTreeIteratorMutable(tree);
// while(naIterateOctTree(&iter)){
//   MyLeafData* data = naGetOctTreeCurMutable(&iter);
//   NAVertexi origin = naGetOctTreeCurOrigin(&iter);
//   // do stuff with the leaf data.
// }
// naClearOctTreeIterator(&iter);
//
// You can choose to have an Accessor, a Mutator or a Modifier as Iterator.
//
// Beware to always use naClearOctTreeIterator. Otherwise NALib will emit
// warnings that there are still iterators running on the struct if NDEBUG is
// undefined.

// Makes the iterators but does not locates any specific leaf.
NA_API NAOctTreeIterator naMakeOctTreeAccessor(const NAOctTree* tree);
NA_API NAOctTreeIterator naMakeOctTreeMutator (      NAOctTree* tree);
NA_API NAOctTreeIterator naMakeOctTreeModifier(      NAOctTree* tree);

// Clears the iterator struct. Always use this after done iterating!
NA_API void naClearOctTreeIterator(NAOctTreeIterator* iter);

// Resets an iterator to point to no specific leaf.
NA_API void naResetOctTreeIterator(NAOctTreeIterator* iter);

// Iterates to the next leaf and returns NA_TRUE if there is one, NA_FALSE if
// the iteration is over. The leafes will be visited like they are stored
// within the tree. If you need axis ordered traversal, maybe have a look at
// the naIterateOctTreeSteps function.
//
// The limit denotes the box the iteration takes place in. Only leafes
// which partially or completely overlap with the limit box will be visited.
// If limit is NA_NULL, all leafes will be visited.
//
// If create is NA_TRUE, all leafes will be created which are not yet existing
// within the limit box. Limit must be non-null. You must have a modifier
// iterator when using create.
NA_API NABool naIterateOctTree(   NAOctTreeIterator* iter,
                                       const NABoxi* limit,
                                              NABool create);

// Moves the iterator to the leaf containing the given coord. If such a leaf is
// not found in the tree, NA_FALSE ist returned. The iterator though stores the
// coord it is supposed to point at. This allows you to locate a position an
// then use naGetOctTreeCurMutable to create a leaf at that position.
NA_API NABool naLocateOctTreeCoord(NAOctTreeIterator* iter, NAVertexi coord);

// Moves the iterator relative to the current position. Each step can be
// positive or negative. The iterator must be at a specific position before
// using this function. Probably you want to use naLocateOctTreeCoord before.
NA_API NABool naLocateOctTreeSteps(    NAOctTreeIterator* iter,
                                                    NAInt stepx,
                                                    NAInt stepy,
                                                    NAInt stepz);

// You can retrieve the current leaf with these functions. When create is set
// to NA_TRUE, a new leaf will be created if it does not exist. You need a
// modifier iterator for this.
NA_API const void* naGetOctTreeCurConst(  NAOctTreeIterator* iter);
NA_API void*       naGetOctTreeCurMutable(NAOctTreeIterator* iter,
                                                          NABool create);

// Returns the origin of the leaf the iterator is pointing at.
NA_API NAVertexi naGetOctTreeCurOrigin(NAOctTreeIterator* iter);

// Returns the box of the leaf the iterator is pointing at.
NA_API NABoxi naGetOctTreeCurBox(NAOctTreeIterator* iter);

// Starts at the leaf containing the given coord and traverses upwards
// (bubbling) the tree by calling the appropriate callback functions of all
// nodes. When the callback functions return NA_FALSE, bubbling stops.
NA_API void naUpdateOctTreeCur(  NAOctTreeIterator* iter);

// Removes the leaf the iterator currently points at.
NA_API void naRemoveOctTreeCur(NAOctTreeIterator* iter);





// Inline implementations are in a separate file:
#include "NAStruct/NAOctTreeII.h"





#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_OCTTREE_INCLUDED

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
