
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



#define NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE 0x8000



NA_HIDEF void naDeallocConfiguration(NATreeConfiguration* config){
  if(config->configdata){naFree(config->configdata);}
  naFree(config);
}



NA_IDEF void naReleaseTreeConfiguration(NATreeConfiguration* config){
  naReleaseRefCount(&(config->refcount), &(config->refcount), (NAMutator)naDeallocConfiguration);
}



NA_HIDEF NATreeConfiguration* naRetainTreeConfiguration(NATreeConfiguration* config){
  #ifndef NDEBUG
    config->flags |= NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE;
  #endif
  return (NATreeConfiguration*)naRetainRefCount(&(config->refcount));
}



NA_IDEF void naSetTreeConfigurationUserData(NATreeConfiguration* config, NAPtr userdata){
  #ifndef NDEBUG
    if(naGetPtrConst(config->userdata))
      naError("Configuration already has userdata");
  #endif
  config->userdata = userdata;
}



NA_IDEF void naSetTreeConfigurationTreeCallbacks(NATreeConfiguration* config, NATreeContructorCallback treeConstructor, NATreeDestructorCallback  treeDestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->treeConstructor = treeConstructor;
  config->treeDestructor = treeDestructor;
}



NA_IDEF void naSetTreeConfigurationLeafCallbacks(NATreeConfiguration* config, NATreeLeafDataConstructor leafDataConstructor, NATreeLeafDataDestructor leafDataDestructor){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->leafDataConstructor = leafDataConstructor;
  config->leafDataDestructor = leafDataDestructor;
}



NA_IDEF void naSetTreeConfigurationNodeCallbacks(NATreeConfiguration* config, NATreeNodeDataConstructor nodedataconstructor, NATreeNodeDataDestructor nodedatadestructor, NATreeNodeUpdater nodeUpdater){
  #ifndef NDEBUG
    if(config->flags & NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE)
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->nodeDataConstructor = nodedataconstructor;
  config->nodeDataDestructor = nodedatadestructor;
  config->nodeUpdater = nodeUpdater;
}



NA_IDEF void naSetTreeConfigurationBaseLeafExponent(NATreeConfiguration* config, NAInt baseleafexponent){
  NAInt* configdata;
  #ifndef NDEBUG
    if(!(config->flags & NA_TREE_QUADTREE) && !(config->flags & NA_TREE_OCTTREE))
      naError("This configuration is not for a quadtree and not for an octtree");
  #endif
  if(config->configdata){naFree(config->configdata);}
  configdata = naMalloc(naSizeof(NAInt));
  *configdata = baseleafexponent;
  config->configdata = configdata;
}



NA_IDEF NAInt naGetTreeConfigurationBaseLeafExponent(const NATreeConfiguration* config){
  #ifndef NDEBUG
    if(!(config->flags & NA_TREE_QUADTREE) && !(config->flags & NA_TREE_OCTTREE))
      naError("This configuration is not for a quadtree and not for an octtree");
  #endif
  return *((NAInt*)(config->configdata));
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
