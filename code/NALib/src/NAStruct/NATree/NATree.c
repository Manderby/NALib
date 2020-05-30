
#include "../../NATree.h"




// Every Add resulting in a change in the tree must go through this function.
NA_HDEF NATreeLeaf* naAddTreeContentInPlace(NATree* tree, NATreeItem* item, const void* key, NAPtr content, NATreeLeafInsertOrder insertOrder){
  // We need to create a node holding both the old leaf and the new one.
  NATreeLeaf* contentleaf = tree->config->leafInserter(tree, item, key, content, insertOrder);
  NATreeNode* parent = naGetTreeItemParent(&(contentleaf->item));
  if(parent){
    naUpdateTreeNodeBubbling(tree, parent, naGetTreeNodeChildIndex(tree->config, parent, &(contentleaf->item)));
  }
  return contentleaf;
}



NA_HDEF void naFillTreeNodeChildData(const NATreeConfiguration* config, NAPtr childdata[NA_TREE_NODE_MAX_CHILDS], NATreeNode* node){
  NAInt i;
  for(i = 0; i < config->childpernode; i++){
    NATreeItem* child = naGetTreeNodeChild(config, node, i);
    if(!child){
      childdata[i] = naMakePtrNull();
    }else if(naIsNodeChildLeaf(node, i)){
      childdata[i] = naGetTreeLeafData(config, (NATreeLeaf*)child);
    }else{
      childdata[i] = naGetTreeNodeData(config, (NATreeNode*)child);
    }
  }
}



// Expects the parent node of a child which has changed. The segment indicates
// which segment caused the trouble. If -1 is given, there is no particular
// node.
NA_HDEF void naUpdateTreeNodeBubbling(NATree* tree, NATreeNode* parent, NAInt childindx){
  NABool bubble = NA_TRUE;

  while(bubble && parent){
    // We call the update callback.
    if(tree->config->nodeUpdater){
      NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
      naFillTreeNodeChildData(tree->config, childdata, parent);
      bubble = tree->config->nodeUpdater(naGetTreeNodeData(tree->config, parent), childdata, childindx, parent->flags & NA_TREE_NODE_CHILDS_MASK);
    }

    // Then we propagate the message towards the root if requested.
    if(bubble && !naIsTreeItemRoot(&(parent->item))){
      NATreeNode* grandparent = naGetTreeItemParent(&(parent->item));
      childindx = naGetTreeNodeChildIndex(tree->config, grandparent, &(parent->item));
      parent = grandparent;
    }else{
      parent = NA_NULL;
    }
  }
}




// Propagates a capturing update event from the root to the leafes.
// All leaf nodes will be called with the childchanged callback with -1
// denoting all leafes shall be updated. These callbacks can send back a
// bubbling request which after capturing all leafes will be propagated
// upwards again so that every node in the path receives a childchanged
// message which again can define if the message shall be bubbled even
// further.
NA_HDEF NABool naUpdateTreeNodeCapturing(NATree* tree, NATreeNode* node){
  NABool bubble;
  NAInt i;
  #ifndef NDEBUG
    if(!tree->config->nodeUpdater)
      naCrash("tree is configured without nodeUpdater callback");
  #endif
  bubble = NA_FALSE;

  // Go through all childs and call the capturing message recursively.
  for(i = 0; i < tree->config->childpernode; i++){
    NATreeNode* subnode = (NATreeNode*)naGetTreeNodeChild(tree->config, node, i);
    if(subnode){
      if(naIsNodeChildLeaf(node, i)){
        bubble |= NA_TRUE;
      }else{
        // this node stores subnodes
        bubble |= naUpdateTreeNodeCapturing(tree, subnode);
      }
    }
  }
  
  // Only if at least one of the childs requested further bubbling, we update
  // this very node.
  if(bubble){
    NAPtr childdata[NA_TREE_NODE_MAX_CHILDS];
    naFillTreeNodeChildData(tree->config, childdata, node);
    bubble = tree->config->nodeUpdater(naGetTreeNodeData(tree->config, node), childdata, -1, node->flags & NA_TREE_NODE_CHILDS_MASK);
  }

  return bubble;
}



#ifndef NDEBUG

//  typedef struct NADebugTreeToken NADebugTreeToken;
//  struct NADebugTreeToken{
//    NATree* tree;
//    NAInt nodecount;
//    NAInt leafcount;
//    NAInt curdepth;
//    NAInt maxdepth;
//    NAInt previndx;
//  };
//  
//  NAInt debugNode(void* token, NAPtr data){
//    NADebugTreeToken* debugtoken;
//    NA_UNUSED(data);
//    debugtoken = (NADebugTreeToken*)token;
//    debugtoken->nodecount++;
//    debugtoken->curdepth++;
//    if(debugtoken->curdepth > debugtoken->maxdepth){debugtoken->maxdepth = debugtoken->curdepth;}
//
//    debugtoken->previndx++;
////    if(debugtoken->previndx == tree->config->childcound){
////      // last child just has been visited. Go up to the parent.
////      if()
////    }else{
////    
////    }
//    debugtoken->curdepth--;
//    return NA_TREE_SEARCH_PARENT;
//  }
//
//  NAInt debugLeaf(void* token, NAPtr data){
//    NADebugTreeToken* debugtoken;
//    NA_UNUSED(data);
//    debugtoken = (NADebugTreeToken*)token;
//    debugtoken->leafcount++;
//    return NA_TREE_SEARCH_PARENT;
//  }
//
//  void naDebugTree(NATree* tree){
//    printf("Tree Debug:" NA_NL);
//    printf(NA_TAB "Structure uses %d bytes." NA_NL, (int)naSizeof(NATree));
//    printf(NA_TAB "%d iterators running on the tree." NA_NL, (int)tree->iterCount);
//    if(tree->root){
//      NATreeIterator iter = naMakeTreeAccessor(tree);
//      NADebugTreeToken token;
//      naZeron(&token, naSizeof(NADebugTreeToken));
//      token.tree = tree;
//      token.previndx = -1;
//      naLocateTreeToken(&iter, &token, debugNode, debugLeaf);
//      
//      printf(NA_TAB "Inner node count: %d * %d Bytes = %d bytes" NA_NL, (int)token.nodecount, (int)tree->config->sizeofNode, (int)((size_t)token.nodecount * tree->config->sizeofNode));
//      printf(NA_TAB "Leaf count: %d * %d Bytes = %d bytes" NA_NL, (int)token.leafcount, (int)tree->config->sizeofLeaf, (int)((size_t)token.leafcount * tree->config->sizeofLeaf));
//      printf(NA_TAB "Max node depth: %d" NA_NL, (int)token.maxdepth);
//      naClearTreeIterator(&iter);
//    }else{
//      printf(NA_TAB "Tree has no root." NA_NL);
//    }
//  }
#endif



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
