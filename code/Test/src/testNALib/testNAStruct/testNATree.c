
#include "NATest.h"
#include <stdio.h>

#include "NAStruct/NATree.h"


void testTreeConfiguration(void){
  naTestGroup("Create and release"){
    NATreeConfiguration* config = NA_NULL;
    naTestVoid(config = naCreateTreeConfiguration(0));
    naTestVoid(naReleaseTreeConfiguration(config));
    // Invalid flag combinations:
    naTestError(config = naCreateTreeConfiguration(NA_TREE_QUADTREE | NA_TREE_KEY_NOKEY));
    naTestError(config = naCreateTreeConfiguration(NA_TREE_QUADTREE | NA_TREE_BALANCE_AVL));
    naTestError(config = naCreateTreeConfiguration(NA_TREE_OCTTREE | NA_TREE_KEY_NOKEY));
    naTestError(config = naCreateTreeConfiguration(NA_TREE_OCTTREE | NA_TREE_BALANCE_AVL));
    naTestError(config = naCreateTreeConfiguration(3)); // invalid key type
    naTestCrash(naReleaseTreeConfiguration(NA_NULL));
  }
}



void printNATree(void){
  printf("NATree.h:" NA_NL);

  naPrintMacro(NA_TREE_KEY_NOKEY);
  naPrintMacro(NA_TREE_KEY_DOUBLE);
  naPrintMacro(NA_TREE_KEY_NAINT);
  naPrintMacro(NA_TREE_BALANCE_AVL);
  naPrintMacro(NA_TREE_QUADTREE);
  naPrintMacro(NA_TREE_OCTTREE);
  naPrintMacro(NA_TREE_ROOT_NO_LEAF);

  naPrintMacroDefined(naBeginTreeAccessorIteration(typedElem, tree, lowerLimit, upperLimit, iter));
  naPrintMacroDefined(naBeginTreeMutatorIteration(typedElem, tree, lowerLimit, upperLimit, iter));
  naPrintMacroDefined(naBeginTreeModifierIteration(typedElem, tree, lowerLimit, upperLimit, iter));
  naPrintMacroDefined(naEndTreeIteration(iter));

  naPrintMacro(NA_TREE_SEARCH_PARENT);
  naPrintMacro(NA_TREE_SEARCH_FOUND);
  naPrintMacro(NA_TREE_SEARCH_ABORT);

  printf(NA_NL "NATreeII.h:" NA_NL);

  naPrintMacro(NA_TREE_NODE_MAX_CHILDS);
  naPrintMacroux32(NA_TREE_NODE_CHILDS_MASK);

  naPrintMacro(NA_TREE_NODE_AVL_BITSHIFT);
  naPrintMacroux32(NA_TREE_NODE_AVL_LEFT);
  naPrintMacroux32(NA_TREE_NODE_AVL_EQUAL);
  naPrintMacroux32(NA_TREE_NODE_AVL_RIGHT);
  naPrintMacroux32(NA_TREE_NODE_AVL_MASK);

  naPrintMacro(NA_TREE_CONFIG_KEY_TYPE_MASK);

  naPrintMacro(NA_TREE_FLAG_ROOT_IS_LEAF);
  naPrintMacro(NA_TREE_FLAG_TMP_KEY_TAKEN);

  naPrintMacroInt(NA_TREE_NODE_CHILDS_OFFSET);

  printf(NA_NL "NATreeConfigurationII.h:" NA_NL);

  naPrintMacroux16(NA_TREE_CONFIG_DEBUG_FLAG_IMMUTABLE);

  printf(NA_NL "NATreeIteratorII.h:" NA_NL);

  naPrintMacroux8(NA_TREE_ITERATOR_MODIFIER);
  naPrintMacroux8(NA_TREE_ITERATOR_CLEARED);

  printf(NA_NL);
}



void testNATree(void){
  naTestFunction(testTreeConfiguration);  
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
