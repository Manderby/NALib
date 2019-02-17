
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NATree.h"
#include "NATreeBin.h"
#include "NATreeOct.h"



NA_DEF NATreeConfiguration* naCreateTreeConfiguration(NAInt flags){
  NATreeConfiguration* config = naAlloc(NATreeConfiguration);
  naZeron(config, sizeof(NATreeConfiguration));
  config->flags = flags;
  naInitRefCount(&(config->refcount));

  if(flags & NA_TREE_OCTTREE){
  
    config->childpernode            = 8;
    switch(flags & NA_TREE_KEY_TYPE_MASK){
    case NA_TREE_KEY_DOUBLE:
      config->keyIndexGetter        = naGetKeyIndexOctDouble;
      config->keyEqualer            = naEqualKeyOctDouble;
      config->keyAssigner           = naAssignKeyOctDouble;
      config->keyTester             = naTestKeyOctDouble;
      break;
    default:
      #ifndef NDEBUG
        naError("Octtree must have type double. Other options are not implemented yet.");
      #endif
      break;
    }
    if(flags & NA_TREE_BALANCE_AVL){
      #ifndef NDEBUG
        naError("Octtree can not have AVL balance.");
      #endif
    }
    config->nodeCoreDestructor      = naDestructTreeNodeOct;
    config->leafCoreConstructor     = naConstructTreeLeafOct;
    config->leafCoreDestructor      = naDestructTreeLeafOct;

    config->bubbleLocator           = naLocateBubbleOct;
    config->captureLocator          = naLocateCaptureOct;
    config->childIndexGetter        = naGetChildIndexOct;
    config->childKeyIndexGetter     = naGetChildKeyIndexOct;
    config->childGetter             = naGetChildOct;
    config->leafRemover             = naRemoveLeafOct;
    config->leafInserter            = naInsertLeafOct;
    config->leafKeyGetter           = naGetLeafKeyOct;
    config->nodeKeyGetter           = naGetNodeKeyOct;
    config->leafDataGetter          = naGetLeafDataOct;
    config->nodeDataGetter          = naGetNodeDataOct;

  }else{

    config->childpernode            = 2;
    switch(flags & NA_TREE_KEY_TYPE_MASK){
    case NA_TREE_KEY_NOKEY:
      config->keyIndexGetter        = NA_NULL;
      config->keyEqualer            = NA_NULL;
      config->keyAssigner           = NA_NULL;
      config->keyTester             = NA_NULL;
      break;
    case NA_TREE_KEY_DOUBLE:
      config->keyIndexGetter        = naGetKeyIndexBinDouble;
      config->keyEqualer            = naEqualKeyBinDouble;
      config->keyAssigner           = naAssignKeyBinDouble;
      config->keyTester             = naTestKeyBinDouble;
      break;
    case NA_TREE_KEY_NAINT:
      config->keyIndexGetter        = naGetKeyIndexBinNAInt;
      config->keyEqualer            = naEqualKeyBinNAInt;
      config->keyAssigner           = naAssignKeyBinNAInt;
      config->keyTester             = naTestKeyBinNAInt;
      break;
    default:
      #ifndef NDEBUG
        naError("Invalid key type in flags");
      #endif
      break;
    }

    config->nodeCoreDestructor      = naDestructTreeNodeBin;
    config->leafCoreConstructor     = naConstructTreeLeafBin;
    config->leafCoreDestructor      = naDestructTreeLeafBin;

    config->bubbleLocator           = naLocateBubbleBin;
    config->captureLocator          = naLocateCaptureBin;
    config->childIndexGetter        = naGetChildIndexBin;
    config->childKeyIndexGetter     = naGetChildKeyIndexBin;
    config->childGetter             = naGetChildBin;
    config->leafRemover             = naRemoveLeafBin;
    config->leafInserter            = naInsertLeafBin;
    config->leafKeyGetter           = naGetLeafKeyBin;
    config->nodeKeyGetter           = naGetNodeKeyBin;
    config->leafDataGetter          = naGetLeafDataBin;
    config->nodeDataGetter          = naGetNodeDataBin;
  }
  
  return config;
}



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
