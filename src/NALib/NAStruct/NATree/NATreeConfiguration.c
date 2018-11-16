
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"
#include "NATreeBinary.h"



NA_DEF NATreeConfiguration* naCreateTreeConfiguration(NAInt flags){
  NATreeConfiguration* config = naAlloc(NATreeConfiguration);
  naNulln(config, sizeof(NATreeConfiguration));
  config->flags = flags;
  naInitRefCount(&(config->refcount));
//  if(flags == 0){
    config->childpernode = 2;
    config->keyIndexGetter          = naGetKeyIndexBinary;
    config->keyEqualer              = naEqualKeyBinary;
    config->keyAssigner             = naAssignKeyBinary;
    config->keyTester               = naTestKeyBinary;

    config->nodeCoreConstructor     = naConstructTreeNodeBinary;
    config->nodeCoreDestructor      = naDestructTreeNodeBinary;
    config->leafCoreConstructor     = naConstructTreeLeafBinary;
    config->leafCoreDestructor      = naDestructTreeLeafBinary;

    config->bubbleLocator           = naLocateBubbleBinary;
    config->captureLocator          = naLocateCaptureBinary;
    config->childIndexGetter        = naGetChildIndexBinary;
    config->childKeyIndexGetter     = naGetChildKeyIndexBinary;
    config->childGetter             = naGetChildBinary;
    config->leafAdder               = naAddLeafBinary;
    config->leafRemover             = naRemoveLeafBinary;
    config->leafReplacer            = naReplaceLeafBinary;
    config->leafSplitter            = naSplitLeafBinary;
    config->leafKeyGetter           = naGetLeafKeyBinary;
    config->leafDataGetter          = naGetLeafDataBinary;
//  }
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
