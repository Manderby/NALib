
// This flag must never overlap with the public flags in NATree.h
#define NA_TREE_CONFIG_DEBUG_FLAG_CONST 0x8000



NA_HIDEF NABool na_GetTreeConfigurationConst(NATreeConfiguration* config) {
  #if NA_DEBUG
    if(!config)
      naCrash("config is nullptr");
  #endif
  return naGetFlagu32(config->flags, NA_TREE_CONFIG_DEBUG_FLAG_CONST);
}

NA_HIDEF void na_SetTreeConfigurationConst(NATreeConfiguration* config) {
  #if NA_DEBUG
    if(!config)
      naCrash("config nullptr");
  #endif
  naSetFlagu32(&config->flags, NA_TREE_CONFIG_DEBUG_FLAG_CONST, NA_TRUE);
}



NA_IDEF void naSetTreeConfigurationUserData(NATreeConfiguration* config, NAPtr userData) {
  #if NA_DEBUG
    if(naGetPtrConst(config->userData))
      naError("Configuration already has userData");
  #endif
  config->userData = userData;
}



NA_IDEF void naSetTreeConfigurationTreeCallbacks(NATreeConfiguration* config, NATreeContructorCallback treeConstructor, NATreeDestructorCallback  treeDestructor) {
  #if NA_DEBUG
    if(na_GetTreeConfigurationConst(config))
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->treeConstructor = treeConstructor;
  config->treeDestructor = treeDestructor;
}



NA_IDEF void naSetTreeConfigurationLeafCallbacks(NATreeConfiguration* config, NATreeLeafDataConstructor leafDataConstructor, NATreeLeafDataDestructor leafDataDestructor) {
  #if NA_DEBUG
    if(na_GetTreeConfigurationConst(config))
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->leafDataConstructor = leafDataConstructor;
  config->leafDataDestructor = leafDataDestructor;
}



NA_IDEF void naSetTreeConfigurationNodeCallbacks(NATreeConfiguration* config, NATreeNodeDataConstructor nodeDataConstructor, NATreeNodeDataDestructor nodeDataDestructor, NATreeNodeUpdater nodeUpdater) {
  #if NA_DEBUG
    if(na_GetTreeConfigurationConst(config))
      naError("Configuration already used in a tree. Mayor problems may occur in the future");
  #endif
  config->nodeDataConstructor = nodeDataConstructor;
  config->nodeDataDestructor = nodeDataDestructor;
  config->nodeUpdater = nodeUpdater;
}



NA_IDEF void naSetTreeConfigurationBaseLeafExponent(NATreeConfiguration* config, int32 baseLeafExponent) {
  int32* configData;
  #if NA_DEBUG
    if(!(config->flags & NA_TREE_QUADTREE) && !(config->flags & NA_TREE_OCTTREE))
      naError("This configuration is not for a quadtree and not for an octtree");
  #endif
  if(config->configData) {naFree(config->configData);}
  configData = (int32*)naMalloc(sizeof(int32));
  *configData = baseLeafExponent;
  config->configData = configData;
}



NA_IDEF int32 naGetTreeConfigurationBaseLeafExponent(const NATreeConfiguration* config) {
  #if NA_DEBUG
    if(!(config->flags & NA_TREE_QUADTREE) && !(config->flags & NA_TREE_OCTTREE))
      naError("This configuration is not for a quadtree and not for an octtree");
  #endif
  return *((int32*)(config->configData));
}



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
