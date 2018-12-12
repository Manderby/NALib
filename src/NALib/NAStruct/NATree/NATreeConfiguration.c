
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"
#include "NATreeBinary.h"



NA_DEF NATreeConfiguration* naCreateTreeConfiguration(NAInt flags){
  NATreeConfiguration* config = naAlloc(NATreeConfiguration);
  naZeron(config, sizeof(NATreeConfiguration));
  config->flags = flags;
  naInitRefCount(&(config->refcount));

  config->childpernode            = 2;
  switch(flags & NA_TREE_KEY_TYPE_MASK){
  case NA_TREE_KEY_NOKEY:
    config->keyIndexGetter        = NA_NULL;
    config->keyEqualer            = NA_NULL;
    config->keyAssigner           = NA_NULL;
    config->keyTester             = NA_NULL;
    break;
  case NA_TREE_KEY_DOUBLE:
    config->keyIndexGetter        = naGetKeyIndexBinaryDouble;
    config->keyEqualer            = naEqualKeyBinaryDouble;
    config->keyAssigner           = naAssignKeyBinaryDouble;
    config->keyTester             = naTestKeyBinaryDouble;
    break;
  case NA_TREE_KEY_NAINT:
    config->keyIndexGetter        = naGetKeyIndexBinaryNAInt;
    config->keyEqualer            = naEqualKeyBinaryNAInt;
    config->keyAssigner           = naAssignKeyBinaryNAInt;
    config->keyTester             = naTestKeyBinaryNAInt;
    break;
  default:
    #ifndef NDEBUG
      naError("naCreateTreeConfiguration", "Invalid key type in flags");
    #endif
    break;
  }

  config->nodeCoreDestructor      = naDestructTreeNodeBinary;
  config->leafCoreConstructor     = naConstructTreeLeafBinary;
  config->leafCoreDestructor      = naDestructTreeLeafBinary;

  config->bubbleLocator           = naLocateBubbleBinary;
  config->captureLocator          = naLocateCaptureBinary;
  config->childIndexGetter        = naGetChildIndexBinary;
  config->childKeyIndexGetter     = naGetChildKeyIndexBinary;
  config->childGetter             = naGetChildBinary;
  config->leafRemover             = naRemoveLeafBinary;
  config->leafInserter            = naInsertLeafBinary;
  config->leafKeyGetter           = naGetLeafKeyBinary;
  config->leafDataGetter          = naGetLeafDataBinary;
  config->nodeDataGetter          = naGetNodeDataBinary;

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
