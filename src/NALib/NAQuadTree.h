
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_QUADTREE_INCLUDED
#define NA_QUADTREE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NACoord.h"
#include "NAMemory.h"

// An NAQuadTree is a container struct capable of storing a two-dimensional
// object of any size. The content of the object is divided into equally
// sized quadratic chunks with a given minimal length in one dimension. These
// chunks are the leafs of the tree. Aside from leafes, an NAQuadTree stores
// parent nodes. Every leaf has a parent node and every node may have another
// parent node.
//
// The child nodes are sometimes referred to as segments which are stored
// internally in the following order:
// segment 0: (lower x, lower y)
// segment 1: (upper x, lower y)
// segment 2: (lower x, upper y)
// segment 3: (upper x, upper y)
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
// Manipulating an NAQuadTree is done in the leafes. When a leaf has been
// altered, a message is sent to the parent nodes. This is called bubbling.
// You can intercept these bubbling messages implementing the appropriate
// callback functions.



// ///////////////////////////////////////////
// CALLBACKS
// ///////////////////////////////////////////

// When creating an NAQuadTree, you can create various callbacks which are
// stored in a NAQuadTreeConfiguration structure. Here are the signatures for 
// these callbacks:

// NAQuadTreeLeafAllocator
// Allocation function which allocate your leaf-chunks. This callback always
// must be present and shall always return something. Returning Null is a bad
// thing. This callback will be called when creating new chunks (whenever a
// NAQuadTree gets expanded) or when duplicating an NAQuadTree.
// The parameters given are the (always square) rect of the chunk as well as
// an userdata pointer which contains whatever you defined upon creation of the
// tree within the NAQuadTreeConfiguration struct. With this, it is for example
// possible to allocate data from a user defined pool-structure. That parameter
// can be NA_NULL.
// When copydata is non-Null, it points to already existing chunk data which
// shall be copied to the new chunk. This will be the case when duplicating
// an NAQuadTree.
// Note that the rect is just here for information. You may or may not use
// it. And you will probably not need to store it as all the important
// callback functions will provide the chunkrect again. But you may if you
// really want to.
// The size of the leaf rect will always correspond to the leaflength declared
// at creation of an NAQuadTree.
// Also note that the coordinates of origin are guaranteed to be a aligned to
// the chosen leaflength.
typedef void* (*NAQuadTreeLeafAllocator)(   NARect rect,
                                             void* userdata,
                                       const void* copydata);

// NAQuadTreeLeafDeallocator
// The deallocation function of your leaf-chunks. This callback always must be
// present. The pointer given is a pointer created when NAQuadTreeLeafAllocator
// was called. The userdata is the same as desribed in the allocator function.
typedef void  (*NAQuadTreeLeafDeallocator)(  void* leafdata,
                                             void* userdata);

// NAQuadTreeDataCopier
// This callback is required when shifting an NAQuadTree. You are given two
// chunk data pointers, two origins and a size.
// Your task is to copy the data in srcdata starting at srcorigin to dstdata
// starting at dstorigin. The size denotes the size of the rectangle which
// shall be copied. The origins are given relative to the chunk origin, meaning
// no matter where the actual origin of the chunk is, the given origins will
// always be greaterequal zero. Additionally, all coordinates of the two
// rectangles are guaranteed to be inside the corresopnding chunk. Note that
// the size does not necessarily denotes a square. It can denote any rectangle.
// The totalshift and the leaflength is given to you as an additional
// information. The totalshift corresponds to the shift given as a parameter
// to naInitQuadTreeCopyShifted and the leafsize corresponds to the size
// of a leaf defined when the NAQuadTree was created.
// Example (numbers are approximate):
//          dstdata           srcdata
//        O---------+       O---------+       
//        |         |       |s----+   |       srcorigin s: (1,1)
//        |    d----+       ||    |   |       dstorigin d: (3,2)
//        |    |    | <---- |+----+   |       size: (4,2)
//        |    +----+       |         |       totalshift: (10, 9)
//        +---------+       +---------+       leaflength: 8

typedef void  (*NAQuadTreeDataCopier)( const void* dstdata,
                                             NAPos dstorigin,
                                       const void* srcdata,
                                             NAPos srcorigin,
                                            NASize size,
                                             NAPos totalshift,
                                            NASize leafsize);

// NAQuadTreeNodeAllocator
// This callback is called when a quad tree creates an internal tree node other
// than a leaf.
// Internal nodes can NOT be manipulated directly but you are allowed to store
// any data with every node if desired. You can return a pointer to any data,
// even a Null pointer. This data pointer will be available to
// NAQuadTreeChildChanged and NAQuadTreeNodeDeallocator.
// An internal node internally stores pointers to childnodes or leafes. The
// origin parameter denotes the origin of the (lowerx, lowery)-node and the
// childsize denotes the (always square) size the childnodes have.
// Note that both origin and childsize are just here for information. You may
// or may not use them. And you will probably not need to store them. But you
// may if you really want to.
typedef void* (*NAQuadTreeNodeAllocator)( NAPos origin,
                                         NASize childsize);

// NAQuadTreeNodeDeallocator
// The node destructor is called before a quad tree ultimately deletes an
// internal node.
// The pointer created with NAQuadTreeNodeAllocator will be sent to this
// function such that you can deallocate the memory if necessary. Note that
// this function is NOT called if a node stores a NULL pointer. Therefore
// you should never return NA_NULL in the allocation callback!
typedef void  (*NAQuadTreeNodeDeallocator)(void* nodedata);

// NAQuadTreeChildChanged
// Whenever you altered a leaf and want to propagate the change over the whole
// tree, you call naUpdateQuadTreeCur which in turn will call the following
// two callback functions. Both will be called with the PARENTAL (internal)
// node data you may have stored with NAQuadTreeNodeAllocator. Additionally,
// you are given all four child data pointers as an array as well as a segment
// index denoting which of the four childs has caused the calling. See segment
// index description above. You also get the size of the childs.
//
// The two functions differ on whether the childs are leafs or nodes. The data
// given are either pointers to leaf chunk data or pointers to node data you
// stored with NAQuadTreeNodeAllocator. All individual pointers can be Null
// meaning there is no leaf stored or Null has explicitely been stored as a
// node. This means, these functions will be called EVEN IF Null was stored.
//
// Segment is a number in [0 - 3] denoting the child which caused the call.
// The value -1 is given if there was no particular child causing the call.
// This happens for example, when a tree gets copied as a whole or if the tree
// gets updated as a whole with naUpdateQuadTree. In that case, probably, all
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
typedef NABool(*NAQuadTreeChildChanged)(     void* nodedata,
                                             int16 segment,
                                 const void* const childdata[4],
                                            uint16 leafmask,
                                            NASize childsize);



// This is the callback struct you can use to create an NAQuadTree. After
// creation, these functions can no longer be changed.
typedef struct NAQuadTreeConfiguration_struct{
  NAQuadTreeLeafAllocator   leafallocator;    // must be present.
  NAQuadTreeLeafDeallocator leafdeallocator;  // must be present.
  NAQuadTreeDataCopier      datacopier;
  NAQuadTreeNodeAllocator   nodeallocator;
  NAQuadTreeNodeDeallocator nodedeallocator;
  NAQuadTreeChildChanged    childchanged;
  void*                     userdata;
  int16                     baseleafexponent;
} NAQuadTreeConfiguration;



// Typedefs of opaque types which should not be visible to the programmer.
typedef struct NAQuadTree NAQuadTree;
typedef struct NAQuadTreeIterator NAQuadTreeIterator;




// //////////////////////////////
// FUNCTIONS
// //////////////////////////////

// Initializes an empty NAQuadTree with the given leaflength and callbacks.
NA_API NAQuadTree* naInitQuadTree(              NAQuadTree* tree,
                                    NAQuadTreeConfiguration configuration);

// Initializes newtree by duplicating the given copytree.
NA_API NAQuadTree* naInitQuadTreeCopy(          NAQuadTree* newtree,
                                          const NAQuadTree* copytree);

// Duplicates the given copytree but shifts all contents by the given shift.
NA_API NAQuadTree* naInitQuadTreeCopyShifted(   NAQuadTree* newtree,
                                          const NAQuadTree* copytree,
                                                      NAPos shift);

// Clears the tree
NA_API void naClearQuadTree(                    NAQuadTree* tree);



// //////////////////////////
// Various functions:

// Empties the tree
NA_API void naEmptyQuadTree(                   NAQuadTree* tree);

// Returns true if the tree is empty
NA_IAPI NABool naIsQuadTreeEmpty(        const NAQuadTree* tree);

// Returns the adjusted pos which is aligned on a chunk coordinate.
NA_API NARect naGetQuadTreeAlignedRect(int16 leafexponent, NAPos pos);

// Returns the root node data, if available. If no root or no root data are
// available, NA_NULL is returned.
NA_API void* naGetQuadTreeRootNodeData(       NAQuadTree* tree);

// Same thing as naUpdateQuadTreeCur but is called for every leaf in the
// whole tree. All messages will bubble as long as there is at least one child
// per node requesting a bubble. Use naUpdateQuadTreeCur if you want to
// update only one element.
NA_API void naUpdateQuadTree(                  NAQuadTree* tree);




// ////////////////////////////////////
// Iterators
//
// You can iterate over every leaf of a whole tree like this:
//
// NAQuadTreeIterator iter = naMakeQuadTreeIteratorMutable(tree);
// while(naIterateQuadTree(&iter)){
//   MyLeafData* data = naGetQuadTreeCurMutable(&iter);
//   NARect rect = naGetQuadTreeCurRect(&iter);
//   // do stuff with the leaf data.
// }
// naClearQuadTreeIterator(&iter);
//
// You can choose to have an Accessor, a Mutator or a Modifier as Iterator.
//
// Beware to always use naClearQuadTreeIterator. Otherwise NALib will emit
// warnings that there are still iterators running on the struct if NDEBUG is
// undefined.
//
// You can also use the predefined Begin and End Iterator macros. Beware,
// these are macros. They perform a simple one-by-one traversal of the tree.
// Use them as follows:
//
// NAQuadTreeIterator iteratorname;
// naBeginQuadTreeMutatorIteration(MyLeaf* leaf, mytree, limit, iteratorname);
//   doStuffWithLeaf(leaf);
// naEndListIteration(iteratorname);

#define naBeginQuadTreeAccessorIteration(typedelem, quadtree, limit, iter)
#define naBeginQuadTreeMutatorIteration (typedelem, quadtree, limit, iter)
#define naBeginQuadTreeModifierIteration(typedelem, quadtree, limit, iter)
#define naEndQuadTreeIteration(iter)


// Makes the iterators but does not locates any specific leaf.
NA_API NAQuadTreeIterator naMakeQuadTreeAccessor(const NAQuadTree* tree);
NA_API NAQuadTreeIterator naMakeQuadTreeMutator (const NAQuadTree* tree);
NA_API NAQuadTreeIterator naMakeQuadTreeModifier(      NAQuadTree* tree);

// Clears the iterator struct. Always use this after done iterating!
NA_API void naClearQuadTreeIterator(NAQuadTreeIterator* iter);

// Resets an iterator to point completely outside of the tree.
NA_API void naResetQuadTreeIterator(NAQuadTreeIterator* iter);

// Iterates to the next leaf and returns NA_TRUE if there is one, NA_FALSE if
// the iteration is over.
//
// The limit denotes the rectangle the iteration takes place in. Only leafes
// which partially or completely overlap with the limit rect will be visited.
// If limit is NA_NULL, all leafes will be visited.
//
// The leafes will always be visited axis-ordered:
// First from lowest to highest x, then from lowest to highest y.
NA_API NABool naIterateQuadTree(  NAQuadTreeIterator* iter,
                                        const NARect* limit);

// Moves the iterator to the leaf containing the given pos. If such a leaf is
// not found in the tree, NA_FALSE ist returned. The iterator though stores the
// coord it is supposed to point at. This allows you to locate a position and
// then use naGetQuadTreeCurMutable to create a leaf at that position.
NA_API NABool naLocateQuadTreeCoord(NAQuadTreeIterator* iter, NAPos pos);

// Moves the stditer to the leaf having the same coord as srciter. If such a
// leaf is not found in the tree, NA_FALSE ist returned. The iterator though
// stores the coord it is supposed to point at. This allows you to locate a
// position and then use naGetQuadTreeCurMutable to create a leaf at that
// position.
NA_API NABool naLocateQuadTreeIterator(NAQuadTreeIterator* dstiter,
                                 const NAQuadTreeIterator* srciter);

// Moves the iterator relative to the current position by multiples of the
// size of a leaf defined by baseleafexponent upon tree creation. Each step
// can be positive or negative. The iterator must be at a specific position
// before using this function.
// Probably you want to use naLocateQuadTreeCoord before.
NA_API NABool naLocateQuadTreeSteps(  NAQuadTreeIterator* iter,
                                                    NAInt stepx,
                                                    NAInt stepy);

// You can retrieve the current leaf with these functions. When create is set
// to NA_TRUE, a new leaf will be created if it does not exist. You need a
// modifier iterator for this.
NA_API const void* naGetQuadTreeCurConst(  NAQuadTreeIterator* iter);
NA_API void*       naGetQuadTreeCurMutable(NAQuadTreeIterator* iter,
                                                        NABool create);

// Returns the rect of the leaf the iterator is pointing at.
NA_API NARect  naGetQuadTreeCurRect(const NAQuadTreeIterator* iter);
NA_API NARecti naGetQuadTreeCurRecti(const NAQuadTreeIterator* iter);

// Starts at the leaf containing the given coord and traverses upwards
// (bubbling) the tree by calling the appropriate callback functions of all
// nodes. When the callback functions return NA_FALSE, bubbling stops.
NA_API void naUpdateQuadTreeCur(  NAQuadTreeIterator* iter);

// Removes the leaf the iterator currently points at.
NA_API void naRemoveQuadTreeCur(NAQuadTreeIterator* iter);





// Inline implementations are in a separate file:
#include "NAStruct/NAQuadTreeII.h"





#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_QUADTREE_INCLUDED

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
