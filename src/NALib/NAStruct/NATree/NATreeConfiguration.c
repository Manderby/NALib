
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NATree.h"


#define NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE 0x01


NA_DEF NATreeConfiguration* naCreateTreeConfiguration(NAMutator destructor){
  NATreeConfiguration* config = naAlloc(NATreeConfiguration);
  naNulln(config, sizeof(NATreeConfiguration));
  naInitRefCount(&(config->refcount));
  config->destructor = destructor;
  return config;
}



NA_HDEF void naDeallocConfiguration(NATreeConfiguration* config){
  naFree(config);
}



NA_DEF void naReleaseTreeConfiguration(NATreeConfiguration* config){
  naReleaseRefCount(&(config->refcount), &(config->refcount), (NAMutator)naDeallocConfiguration);
}



NA_HDEF NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config){
  #ifndef NDEBUG
    config->debugflags |= NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE;
  #endif
  return (NATreeConfiguration*)naRetainRefCount(&(config->refcount));
}



NA_API void naSetTreeConfigurationTreeCallbacks(NATreeConfiguration* config, NATreeContructorCallback treeconstructor, NATreeDestructorCallback  treedestructor){
  #ifndef NDEBUG
    if(config->debugflags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("naSetTreeConfigurationTreeCallbacks", "Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->treeconstructor = treeconstructor;
  config->treedestructor = treedestructor;
}



NA_API void naSetTreeConfigurationLeafCallbacks(NATreeConfiguration* config, NATreeLeafConstructor leafconstructor, NATreeLeafDestructor leafdestructor){
  #ifndef NDEBUG
    if(config->debugflags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("naSetTreeConfigurationLeafCallbacks", "Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->leafconstructor = leafconstructor;
  config->leafdestructor = leafdestructor;
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
