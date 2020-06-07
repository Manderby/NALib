
#ifndef NA_TREE_INCLUDED
#define NA_TREE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// ///////////////////////////////////////////
// Trees
//
// This is the implementation of a general tree structure for various kinds.
// You use a NATreeConfiguration to define how the tree shall behave. Please
// read carefully about the callback methods below.
//
// Note that there is a basic distinction between trees with and without keys.
// Some APIs are only allowed for one of the kind. You will get an error when
// using NDEBUG.
//
// With this structure, it is possible to...
// - Store keys with inner nodes and leafes to automatically sort the tree.
// - Automatically balance a bin tree using AVL.
// - Store data for each inner node and/or leaf.
// - Search according to keys or, if no keys are available, a token of any
//   other kind.
// - Use iterators on the leafes.

#include "NAMemory.h"



typedef struct NATree NATree;
typedef struct NATreeIterator NATreeIterator;
typedef struct NATreeConfiguration NATreeConfiguration;



// ///////////////////////////////////////////
// CALLBACKS
//
// When creating an NATree, you can define various callbacks which are stored
// in an NATreeConfiguration structure. Here are the signatures for these
// callbacks:
// ///////////////////////////////////////////


// NATreeContructorCallback and NATreeDestructorCallback
// Will be called whenever a tree is created or destroyed. Use this callback
// to handle potential registration of whatever you need.
//
// The userdata parameter corresponds to the data provided in the
// naSetTreeConfigurationUserData function.
//
// Note that a new tree can be for example be implicitely created when another
// tree gets duplicated.
//
// The constructor will be called BEFORE the tree is initialized and the
// destructor is called AFTER the tree has been cleared.
typedef void (*NATreeContructorCallback)(NAPtr userdata);
typedef void (*NATreeDestructorCallback)(NAPtr userdata);

// NATreeLeafDataConstructor and NATreeLeafDataDestructor
// The constructor will be called when creating a new leaf. The destructor is
// called, when a leaf ultimately gets deleted.
//
// The constructor allows you to process the given content before storing. In
// the end, you have to return an NAPtr which then will be stored in the tree.
// Use this to either just process the given content and return it in the end
// or create a completely new content out of the given content. If a tree does
// not define a leaf constructor, a leaf will simply store the given content
// without any processing.
//
// In the destructor, the leafdata denotes the data which was returned from the
// constructor.
//
// The key parameter denotes the key of the node. If the tree does not use a
// key, that parameter is Null.
//
// Use these callbacks for example, if you want to store a reference counted
// pointer which shall be incremented before storing it in the tree. Or you
// can use the data stored in content to allocate a memory block from
// a user defined pool-structure and store the leafdata within, ultimately
// returning the memory block. Or maybe content contains just an integer which
// counts the number of elements in the tree and you just return naMakePtrNull.
// Many more examples exist.
//
// Use naGetPtrConst and naGetPtrMutable to read content and leafdata.
// Use naMakePtrWithDataConst and naMakePtrWithDataMutable to create the
// return value.
typedef NAPtr (*NATreeLeafDataConstructor)(const void* key, NAPtr content);
typedef void  (*NATreeLeafDataDestructor) (NAPtr leafdata);

// NATreeNodeDataConstructor and NATreeNodeDataDestructor
// The constructor is called when a tree creates an internal tree node other
// than a leaf. The destructor is called before a tree ultimately deletes an
// internal node.
//
// Internal nodes can NOT be manipulated directly but you are allowed to store
// any data with every node if desired. You can return an NAPtr to any data.
// This data pointer will then be available to other callback functions.
//
// The key parameter denotes the key of the node. If the tree does not use a
// key, that parameter is Null. Note that the content parameter is here
// just for information. You may or may not use it.
typedef NAPtr (*NATreeNodeDataConstructor)(const void* key);
typedef void  (*NATreeNodeDataDestructor) (NAPtr nodedata);

// NATreeNodeUpdater 
// Gets called whenever childs of a parent node change. The parent data is the
// data stored in the parent node. The childdatas parameter contains an
// array of the datas stored in all children. The childIndex parameter denotes
// which child caused the call or is -1 if no particular child caused the
// update. The childmask parameter is a bitfield denoting 1 for the child
// being a leaf and 0 for being an inner node.
//
// When returning NA_TRUE, the update will propagate to the next parent. When
// returning NA_FALSE, propagation stops.
typedef NABool (*NATreeNodeUpdater)       (NAPtr parentdata,
                                          NAPtr* childdatas,
                                           NAInt childIndex,
                                           NAInt childmask);



// ////////////////////
// NATreeConfiguration
// ////////////////////

// Flags for NATreeConfiguration
//
// KEY_NOKEY        Set this flag for your nodes and leafes to have no key.
// KEY_DOUBLE       Set this flag for your keys to have the double type.
// KEY_NAINT        Set this flag for your keys to have the NAInt type.
// BALANCE_AVL      Makes the tree a self-balancing tree using the AVL method
// NA_TREE_QUADTREE Makes the tree a quadtree using 2-dimensional keys.
// NA_TREE_OCTTREE  Makes the tree an octtree using 3-dimensional keys.
// NA_TREE_ROOT_NO_LEAF Ensures that the root of the tree never is a leaf.
//                      (currently available only for quadtree and octtree)
#define NA_TREE_KEY_NOKEY     0x0000
#define NA_TREE_KEY_DOUBLE    0x0001
#define NA_TREE_KEY_NAINT     0x0002
#define NA_TREE_BALANCE_AVL   0x0010
#define NA_TREE_QUADTREE      0x0020
#define NA_TREE_OCTTREE       0x0040
#define NA_TREE_ROOT_NO_LEAF  0x0100

// This is the callback struct you can use to create an NATree. Please read the
// extensive comments at the appropriate callback signatures to understand how
// this configuration must be filled. The flags are a combination of the macros
// defined above.
//
// If you try to change a configuration after it has been used for at least
// one tree, you will get a warning if NDEBUG is undefined.

NA_API  NATreeConfiguration* naCreateTreeConfiguration(NAInt flags);
NA_IAPI void naReleaseTreeConfiguration(NATreeConfiguration* config);

NA_IAPI void naSetTreeConfigurationUserData(
  NATreeConfiguration*       config,
  NAPtr                      userdata);

NA_IAPI void naSetTreeConfigurationTreeCallbacks(
  NATreeConfiguration*       config,
  NATreeContructorCallback   treeconstructor,
  NATreeDestructorCallback   treedestructor);

NA_IAPI void naSetTreeConfigurationLeafCallbacks(
  NATreeConfiguration*       config,
  NATreeLeafDataConstructor  leafdataconstructor,
  NATreeLeafDataDestructor   leafdatadestructor);

NA_IAPI void naSetTreeConfigurationNodeCallbacks(
  NATreeConfiguration*       config,
  NATreeNodeDataConstructor  nodedataconstructor,
  NATreeNodeDataDestructor   nodedatadestructor,
  NATreeNodeUpdater          nodeUpdater);

NA_IAPI void naSetTreeConfigurationBaseLeafExponent(
  NATreeConfiguration*       config,
  NAInt                      baseleafexponent);

NA_IAPI NAInt naGetTreeConfigurationBaseLeafExponent(
  const NATreeConfiguration* config);


// ////////////////////
// NATree
// ////////////////////

// Creates, Empties and Clears a tree. The config gets retained.
NA_IAPI NATree* naInitTree(NATree* tree, NATreeConfiguration* config);
NA_IAPI void naEmptyTree(NATree* tree);
NA_IAPI void naClearTree(NATree* tree);

// Returns true if the tree is completely empty.
NA_IAPI NABool naIsTreeEmpty(const NATree* tree);

// Returns the data stored at the first or last leaf. Note that trying to get
// a mutable pointer of a data object which was stored as const will result
// in a warning when NDEBUG is undefined.
//
// Note that these are just convenience functions. They internally createn an
// iterator, search for the desired item and clear the iterator again. You can
// do this manually if you want. The results are exactly the same.
NA_IAPI const void* naGetTreeFirstConst  (const NATree* tree);
NA_IAPI void*       naGetTreeFirstMutable(const NATree* tree);
NA_IAPI const void* naGetTreeLastConst   (const NATree* tree);
NA_IAPI void*       naGetTreeLastMutable (const NATree* tree);

// Adds the given content at the beginning or end of a tree which is configured
// without keys. These functions will not work with keyed trees. All functions
// return always true to be consistent with the other add functions below which
// return true when a new item has been added to the tree.
//
// Note that these are just convenience functions. They internally create an
// iterator, add the element and clear the iterator again. You can do this
// manually if you want. The results are exactly the same.
NA_IAPI NABool naAddTreeFirstConst  (NATree* tree, const void* content);
NA_IAPI NABool naAddTreeFirstMutable(NATree* tree,       void* content);
NA_IAPI NABool naAddTreeLastConst   (NATree* tree, const void* content);
NA_IAPI NABool naAddTreeLastMutable (NATree* tree,       void* content);

// Traverses the whole tree and calls the nodeUpdater callback for every
// inner node from the leaves towards the root.
NA_IAPI void naUpdateTree(NATree* tree);

// Returns the content stored in the root node, if any.
NA_IAPI NAPtr naGetRootNodeContent(NATree* tree);

// ////////////////////////////////////
// Iterators
//
// You can iterate over every leaf of a whole tree like this:
//
// NATreeIterator iter = naMakeTreeIteratorMutable(tree);
// while(naIterateTree(&iter)){
//   MyLeafData* leafdata = naGetTreeCurLeafMutable(&iter);
//   double* myKey = naGetTreeCurLeafKey(&iter);
//   // do stuff with the leafdata.
// }
// naClearTreeIterator(&iter);
//
// You can choose to have an Accessor, a Mutator or a Modifier as Iterator.
//
// Beware to always use naClearTreeIterator. Otherwise NALib will emit
// warnings that there are still iterators running on the struct if NDEBUG is
// undefined.
//
// You can also use the predefined Begin and End Iterator macros. Beware,
// these are macros. They perform a simple one-by-one traversal of the tree.
// Use them as follows:
//
// NATreeIterator iteratorname;
// naBeginTreeMutatorIteration(MyLeaf* leaf, mytree, lowerlimit, upperlimit, iteratorname);
//   doStuffWithLeaf(leaf);
// naEndListIteration(iteratorname);

#define naBeginTreeAccessorIteration(typedelem, tree, lowerlimit, upperlimit, iter)
#define naBeginTreeMutatorIteration (typedelem, tree, lowerlimit, upperlimit, iter)
#define naBeginTreeModifierIteration(typedelem, tree, lowerlimit, upperlimit, iter)
#define naEndTreeIteration(iter)

// ///////////////////////////////
// Creating and positioning an iterator:
//
// The naMakeTreeXXX functions will create a new iterator which is
// positioned at the initial position of the tree. From there on, a tree
// can be searched forward or backward. You define, if the iterator can
// only access the contents, mutate them or even modify the whole tree.
// Add or remove functions for example will emit a warning if the iterator
// is not a modifier. Therefore, you can even savely transfer iterators
// to third-party programmers.
//
// You can reset any iterator to the initial state with the reset function.
//
// After you are done using the iterator, you should clear it with a call to
// naClearTreeIterator. NALib keeps track of where the iterators are when
// NDEBUG is undefined. Therefore, you will get lots of warnings if the
// iterators are not properly cleared. In the release code, no checks are
// performed.
NA_IAPI NATreeIterator naMakeTreeAccessor(const NATree* tree);
NA_IAPI NATreeIterator naMakeTreeMutator(NATree* tree);
NA_IAPI NATreeIterator naMakeTreeModifier(NATree* tree);
NA_IAPI void naResetTreeIterator(NATreeIterator* iter);
NA_IAPI void naClearTreeIterator(NATreeIterator* iter);

// Function callback types for the naLocateTreeToken function. See below.
//
// You can search a tree using an arbitrary token using naLocateTreeToken.
// The following two callbacks will be called either for a node or a leaf
// given its stored data.
//
// The return value defines, how the search shall continue.
//
// For the node callback, you can return the index of the child where to
// search next. If the parent node shall be searched, you can use the PARENT
// macro below. If the search shall stop, either use the FOUND or ABORT macro
// below.
//
// For the child callback, return one of the macros below. Any other value is
// not allowed.
//
// When using the FOUND macro, the search stops and the return value of the
// LocateTreeToken function will be NA_TRUE. When using the ABORT macro,
// search stops as well but the return value will be NA_FALSE.
//
// Beware, you can very vell create an endless loop!
#define NA_TREE_SEARCH_PARENT  -1
#define NA_TREE_SEARCH_FOUND   -2
#define NA_TREE_SEARCH_ABORT   -3
typedef NAInt (*NATreeNodeTokenSearcher)(   void* token,
                                            NAPtr data);
typedef NAInt (*NATreeLeafTokenSearcher)(   void* token,
                                            NAPtr data);

// Moves the iterator to the specified position. If the desired position is
// not found or the iterator points at the initial position, NA_FALSE is
// returned.
//
// Key:      Searches for the given absolute key. When assumeclose is true, the 
//           desired key is assumed to be in the neighborhood of the current
//           location. Beware, only set it to true if you are sure it is close.
//           This can make quite a difference in performance. Test it out!
// First:    Locates the first element of the whole tree.
// Last:     Locates the last element of the whole tree.
// Iterator: Positions the given iterator at the exact same position as the
//           given source iterator.
// Token:    Searches a desired location in the tree using the given token.
//           You must provide the appropriate token callbacks. See above.
//           This is the only function which allows searching for leafes in
//           a tree which is configured to have no keys. The iterator will
//           point to the last leaf for which matchfound was true.
NA_IAPI NABool naLocateTreeKey(     NATreeIterator* iter,
                                        const void* key,
                                             NABool assumeclose);
NA_IAPI NABool naLocateTreeFirst(   NATreeIterator* iter);
NA_IAPI NABool naLocateTreeLast(    NATreeIterator* iter);
NA_IAPI NABool naLocateTreeIterator(NATreeIterator* iter,
                                    NATreeIterator* srciter);
NA_API  NABool naLocateTreeToken   (NATreeIterator* iter,
                                              void* token,
                            NATreeNodeTokenSearcher nodeSearcher,
                            NATreeLeafTokenSearcher leafSearcher);

// Executes the nodeUpdater callback starting from the parent node of the
// current leaf of iter. Bubbles towards the root if the callback returns
// NA_TRUE.
NA_IAPI void naUpdateTreeLeaf(NATreeIterator* iter);

// The function naBubbleTreeToken executes the given callback for every inner
// node starting with the parent node of the current leaf of iter. Bubbles
// towards the root as long as the callback returns NA_TRUE.
//
// The callback will get the token, the data of the current node as well as
// the index of the child from which the bubbling comes from.
typedef NABool (*NATreeNodeTokenCallback)(     void* token,
                                               NAPtr data,
                                               NAInt nextIndex);
NA_IAPI void naBubbleTreeToken(const NATreeIterator* iter,
                                               void* token,
                             NATreeNodeTokenCallback nodeTokenCallback);



// /////////////////////////////////
// Iterating
// /////////////////////////////////

// Moves the iterator in order through the tree. The Back-variant moves the
// iterator backwards. Returns NA_FALSE when iteration is over. For trees
// which store a key, you can even send a limit in which range the tree shall
// be traversed. You can send Null if you want the whole tree to be traversed.
NA_IAPI NABool naIterateTree        (NATreeIterator* iter,
                                         const void* lowerlimit,
                                         const void* upperlimit);
NA_IAPI NABool naIterateTreeBack    (NATreeIterator* iter,
                                         const void* lowerlimit,
                                         const void* upperlimit);

// /////////////////////////////////
// Returns the content of the current element without moving the iterator.
// Beware: You must know whether the iterator is at a leaf or a node.
// Of course this could be detected automatically by the iterator but that
// would allow for very sloppy designs.
NA_IAPI const void* naGetTreeCurLeafKey    (NATreeIterator* iter);
NA_IAPI const void* naGetTreeCurLeafConst  (NATreeIterator* iter);
NA_IAPI void*       naGetTreeCurLeafMutable(NATreeIterator* iter);

NA_IAPI const void* naGetTreeCurNodeKey    (NATreeIterator* iter);
NA_IAPI const void* naGetTreeCurNodeConst  (NATreeIterator* iter);
NA_IAPI void*       naGetTreeCurNodeMutable(NATreeIterator* iter);

// Adds the given content to the tree under the given key. These functions
// only work on trees which are configured to have keys.
//
// If there already exists an item with the given key, the given content will
// either replace the existing content when the replace parameter is true or
// the existing content stays as it was when the replace parameter is false.
// When replacing, the data stored in the existing item will be destructed
// and a new data will be constructed with the new content, if any.
//
// The functions return NA_TRUE if the given key was found.
//
// The iterator will afterwards always point at the tree leaf with the given
// key, no matter if it is the replaced content or the existing content.
NA_IAPI NABool naAddTreeKeyConst(   NATreeIterator* iter,
                                        const void* key,
                                        const void* content,
                                             NABool replace);
NA_IAPI NABool naAddTreeKeyMutable( NATreeIterator* iter,
                                        const void* key,
                                              void* content,
                                             NABool replace);

// Adds the given content at the specified position. Note that if the iterator
// is at initial position, "Next" behaves like "First" and "Prev" behaves like
// "Last".
//
// These functions only work on trees which are configured to have no keys.
NA_IAPI NABool naAddTreePrevConst(  NATreeIterator* iter,
                                        const void* content,
                                             NABool movetonew);
NA_IAPI NABool naAddTreePrevMutable(NATreeIterator* iter,
                                              void* content,
                                             NABool movetonew);
NA_IAPI NABool naAddTreeNextConst(  NATreeIterator* iter,
                                        const void* content,
                                             NABool movetonew);
NA_IAPI NABool naAddTreeNextMutable(NATreeIterator* iter,
                                              void* content,
                                             NABool movetonew);

// /////////////////////////////////
// Removes elements.
//
// The advance parameter denotes how the iterator shall move after the
// element has been removed. If advance is NA_FALSE, the iterator will be
// moved to the previous element. If advance is NA_TRUE, the iterator will
// be moved to the next element.
NA_IAPI void naRemoveTreeCurLeaf(NATreeIterator* iter);

// Returns true if the iterator is at its initial position.
NA_IAPI NABool naIsTreeAtInitial(const NATreeIterator* iter);


#ifndef NDEBUG
  void naDebugTree(NATree* tree);
#endif 

// Inline implementations are in a separate file:
#include "NAStruct/NATree/NATreeII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_TREE_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
