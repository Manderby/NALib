
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_QUADTREE_INCLUDED
#define NA_QUADTREE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NACoord.h"

// An NAOctTree is a container struct capable of storing a two-dimensional
// object of any size. The content of the object is divided into equally sized
// cubic chunks with a given minimal size. These chunks are the leafs of
// the tree. Aside from leafes, an NAOctTree stores parent nodes. Every leaf
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
// Manipulating an NAOctTree is done in the leafes. When a leaf has been
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
// when creating new chunks (whenever a NAOctTree gets expanded) or when
// duplicating an NAOctTree.
// The parameters given are the origin of the data as well as the (always
// square) leafsize of the data box. When copydata is non-Null, it points
// to already existing chunk data which shall be copied to the new chunk. This
// will be the case when duplicating an NAOctTree.
// Note that both origin and leafsize are just here for information. You may or
// may not use them. And you will probably not need to store them. But you may
// if you really want to.
// The leafsize given will always be the leafsize declared at creation of an
// NAOctTree.
// Also note that the coordinates of origin are guaranteed to be a multiple of
// leafsize.
typedef void* (*NAOctTreeLeafAllocator)(   NAVertexi origin,
                                            NAUInt leafsize,
                                       const void* copydata);

// The destruction function of your leaf-chunks. This callback always must be
// present. The pointer given is a pointer created when NAOctTreeLeafAllocator
// was called.
typedef void  (*NAOctTreeLeafDeallocator)(   void* leafdata);

// This callback is required when shifting an NAOctTree. You are given two
// chunk data pointers, two origins and a volume.
// Your task is to copy the data in copydata starting at copyorigin to dstdata
// starting at dstorigin. The volume denotes the volume of the box which
// shall be copied. The origins are given relative to the chunk origin, meaning
// no matter where the actual origin of the chunk is, the given origins will
// always be greaterequal zero. Additionally, all coordinates of the two
// boxes are guaranteed to be inside the corresopnding chunk. Note that
// the volume does not necessarily denotes a cube. It can denote any cuboid.
// The leafsize is given to you as an additional information. It corresponds
// to the leafsize used when the NAOctTree was created.
typedef void  (*NAOctTreeDataCopier)( const void* dstdata,
                                            NAVertexi dstorigin,
                                       const void* copydata,
                                            NAVertexi copyorigin,
                                           NAVolumei volume,
                                            NAUInt leafsize);

// This callback is called when a oct tree creates a node other than a leaf.
// Nodes can not be directly manipulated but you are allowed to store any data
// with every node if desired. The origin parameter denotes the origin of all
// leafes this node is coprised of and the nodesize denotes the (always cubic)
// box all leafes are contained in.
// Note that both origin and nodesize are just here for information. You may or
// may not use them. And you will probably not need to store them. But you may
// if you really want to.
// You can return a pointer to any data, even a Null pointer. This data pointer
// will be available to NAOctTreeLeafChanged, NAOctTreeChildChanged and
// NAOctTreeNodeDestructor.
typedef void* (*NAOctTreeNodeAllocator)(   NAVertexi origin,
                                            NAUInt nodesize);

// The node destructor is called before a oct tree ultimately deletes a node.
// The pointer created with NAOctTreeNodeAllocator will be sent to this
// function such that you can deallocate the memory if necessary. Note that
// this function is NOT called if a node stores a NULL pointer.
typedef void  (*NAOctTreeNodeDestructor)(   void* nodedata);

// Whenever you altered a leaf and want to propagate the change over the whole
// tree, you call naUpdateOctTreeCurrent which in turn will call the following
// two callback functions. Both will be called with the parental node data you
// may have stored with NAOctTreeNodeAllocator. Additionally, you are given
// all four child data pointers as an array as well as a segment index denoting
// which of the four childs has caused the calling.
//
// The two functions differ on whether the childs are leafs or nodes. The data
// given are either pointers to leaf chunk data or pointers to node data you
// stored with NAOctTreeNodeAllocator. All individual pointers can be Null
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
typedef NABool(*NAOctTreeLeafChanged)(      void* nodedata,
                                             NAInt segment,
                                 const void* const leafdata[4]);
typedef NABool(*NAOctTreeChildChanged)(     void* nodedata,
                                             NAInt segment,
                                 const void* const childdata[4]);

// This callback is called when the data of a leaf chunk shall be serialized.
// You must always return the number of bytes required to store the chunk data.
// You may use the leafsize given for the calculation of that size. The
// leafsize always is the leafsize declared upon creation of the NAOctTree.
//
// When buffer is a Null pointer, just return the require size. When buffer is
// NOT a Null-Pointer, this buffer is guaranteed to have the required size
// and you can copy the data to the buffer.
//
// Therefore, when you call naSerializeOctTree, this callback will be called
// for every chunk twice: First to akquire the required size and second to
// actually store the data.
typedef uint64(*NAOctTreeSerializer)        (void* buffer,
                                        const void* leafdata,
                                             NAUInt leafsize);

// This callback is called when a buffer shall be deserialized which
// previously had been serialized with NAOctTreeSerializer. Create a leaf
// chunk and return a pointer to the chunk data. The origin and (always
// cubic) leafsize are given to you as an additional information.
//
// Note that this callback is very similar to NAOctTreeLeafAllocator but
// here, buffer denotes not a chunk which shall be copied but rather more a
// buffer which need to be deserialized. These can be two different things.
typedef void* (*NAOctTreeDeserializer)(     NAVertexi origin,
                                             NAUInt leafsize,
                                        const void* buffer);



// This is the callback struct you can use to create an NAOctTree. After
// creation, these functions can no longer be changed.
typedef struct NAOctTreeCallbacks_struct{
  NAOctTreeLeafAllocator    leafallocator;    // must be present.
  NAOctTreeLeafDeallocator  leafdeallocator;   // must be present.
  NAOctTreeDataCopier       datacopier;
  NAOctTreeNodeAllocator    nodeallocator;
  NAOctTreeNodeDestructor   nodedestructor;
  NAOctTreeLeafChanged      leafchanged;
  NAOctTreeChildChanged     childchanged;
  NAOctTreeSerializer       serialize;
  NAOctTreeDeserializer     deserialize;
} NAOctTreeCallbacks;

// A typedef of an opaque type not accessible to the programmer.
typedef struct NAOctTreeNode NAOctTreeNode;

// The typedef of the NAOctTree.
typedef struct NAOctTree NAOctTree;
struct NAOctTree{
  NAUInt leafsize;
  NAOctTreeCallbacks callbacks;
  NAOctTreeNode* root;
  NAOctTreeNode* visitnode;    // visitnode stores tha last node visited. This
  NAOctTreeNode* curnode;      // Makes searching in the tree very fast.
  NAUInt cursegment;           // curnode and cursegment are for the iteration
};




// Initializes an empty NAOctTree with the given leafsize and callbacks.
NA_API NAOctTree* naInitOctTree(              NAOctTree* tree,
                                                     NAUInt leafsize,
                                        NAOctTreeCallbacks callbacks);

// Duplicates the given duptree.
NA_API NAOctTree* naCopyOctTree(              NAOctTree* newtree,
                                          const NAOctTree* duptree);

// Duplicates the given duptree, but only containing the chunks which are
// present in masktree.
NA_DEF NAOctTree* naCopyOctTreeWithMaskTree(  NAOctTree* newtree,
                                          const NAOctTree* duptree,
                                          const NAOctTree* masktree);

// Duplicates the given duptree but shifts all contents by the given shift.
NA_API NAOctTree* naCopyOctTreeWithShift(     NAOctTree* newtree,
                                          const NAOctTree* duptree,
                                                    NAVolumei shift);

// Serializes a full NAOctTree.
// When buf is Null, the total amount of bytes required for the whole tree
// including all structural data is returned in bytesize.
// When buf is not Null, the tree serializes into the given buffer. You must
// send the bytesize you received from the first call again!
NA_API void naSerializeOctTree(            const NAOctTree* tree,
                                                        void* buf,
                                                      uint64* bytesize);
// Deserializes a full NAOctTree
NA_API NAOctTree* naInitOctTreeWithDeserialization(NAOctTree* tree,
                                                const void* buf,
                                        NAOctTreeCallbacks callbacks);

// Empties the tree
NA_API void naEmptyOctTree(                    NAOctTree* tree);

// Clears the tree
NA_API void naClearOctTree(                    NAOctTree* tree);

// Returns true if the tree is empty
NA_API NABool naIsOctTreeEmpty(                NAOctTree* tree);

// Returns the leaf size
NA_API uint64 naGetOctTreeLeafSize(      const NAOctTree* tree);

// Returns the callbacks (by copying the function pointers)
NA_API NAOctTreeCallbacks naGetOctTreeCallbacks(const NAOctTree* tree);

// Returns the chunk containing the given coord. If no chunk with the desired
// coord exists, NA_NULL is returned. When create is true, the chunk will be
// created and returned. If the origin pointer is not Null it will be filled
// with the origin information of the chunk returned. When create is true,
// didcreate returns whether the leaf has indeed been created or not.
NA_API const void* naGetOctTreeLeafConst(  const NAOctTree* tree,
                                                      NAVertexi  coord,
                                                      NAVertexi* origin);
NA_API       void* naGetOctTreeLeafMutable(      NAOctTree* tree,
                                                      NAVertexi  coord,
                                                      NABool  create,
                                                     NABool*  didcreate,
                                                      NAVertexi* origin);

// Moves the internal pointer to the first most leaf.
NA_API        void naFirstOctTree(         const NAOctTree* tree);
// Returns the current leaf but moves the internal pointer to the next leaf.
// The origin of the current leaf will be stored in origin when not Null.
NA_API  const void* naIterateOctTreeConst(   const NAOctTree* tree,
                                                        NAVertexi* origin);
NA_API        void* naIterateOctTreeMutable(       NAOctTree* tree,
                                                        NAVertexi* origin);

// Same as above but only iterates upon leafs which overlap with the given box.
NA_API        void naFirstOctTreeInBox(    const NAOctTree* tree,
                                                      NABoxi box);
NA_API  const void* naIterateOctTreeInBoxConst(   const NAOctTree* tree,
                                                              NAVertexi* origin,
                                                              NABoxi box);
NA_API        void* naIterateOctTreeInBoxMutable(       NAOctTree* tree,
                                                              NAVertexi* origin,
                                                              NABoxi box);

// Function pointer used for the set iteration naSetOctTreeInBox. This
// callback is called for multiple chunks of a tree.
// When called, dstdata is the chunk data, box is the box within the data
// shall be set and leafsize is the leaf size given upon creation of the tree.
// The userdata corresponds to whatever has been given in naSetOctTreeInBox.
// The origin of the box is given relative to the origin of the chunk.
// Therefore, all positions are greaterequal zero. All coordinates of box are
// guaranteed to be inside the leaf.
typedef void  (*NAOctTreeDataSetter)( const void* dstdata,
                                           NABoxi box,
                                            NAUInt leafsize,
                                       const void* userdata);

// Calls the given NAOctTreeDataSetter function for every chunk in the tree
// which is partially or fully overlapped with the given box. Will create
// the chunks if necessary.
NA_API void naSetOctTreeInBox(          NAOctTree* tree,
                                              NABoxi box,
                                 NAOctTreeDataSetter datasetter,
                                          const void* userdata);

// Removes the leaf currently looked at.
// Beware, after this function, the tree forgets its last visit and must
// search anew.
NA_API        void naRemoveOctTreeLeaf(NAOctTree* tree, NAVertexi coord);


// Starts at the leaf containing the given coord and traverses upwards
// (bubbling) the tree by calling the appropriate callback functions of all
// nodes. When the callback functions return NA_FALSE, bubbling stops.
NA_API void naUpdateOctTreeAtCoord(NAOctTree* tree, NAVertexi coord);
// Same thing but updates the whole tree. All messages will bubble as long
// as there is at least one child per node requesting a bubble.
NA_API void naUpdateOctTree(NAOctTree* tree);

// Returns the root node data, if available. If no root or no root data are
// available, NA_NULL is returned.
NA_API void* naGetOctTreeRootNodeData(NAOctTree* tree);







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
