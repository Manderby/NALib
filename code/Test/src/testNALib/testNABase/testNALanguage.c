
#include "NABase.h"
#include <stdio.h>



void na_accessor_func(const void* param){
  NA_UNUSED(param);
}
void na_mutator_func(void* param){
  NA_UNUSED(param);
}



void testNALanguage(){
  naTestGroup("typedefs"){
    NAAccessor testAccessor = na_accessor_func;
    NAMutator testMutator = na_mutator_func;
    naTest(testAccessor == na_accessor_func);
    naTest(testMutator == na_mutator_func);
  }

  #ifndef NDEBUG
    naTestGroup("ElementOverlap"){
      int32 test[3] = {42, 42, 42};
      naTest(naElementOverlap(&test[0], &test[0], 0, 0) == NA_FALSE);
      naTest(naElementOverlap(&test[0], &test[1], 0, 0) == NA_FALSE);
      naTest(naElementOverlap(&test[0], &test[1], 0, 1) == NA_FALSE);
      naTest(naElementOverlap(&test[1], &test[0], 0, 0) == NA_FALSE);
      naTest(naElementOverlap(&test[0], &test[2], 2, 0) == NA_FALSE);
      naTest(naElementOverlap(&test[0], &test[1], 2, 0) == NA_TRUE);
      naTest(naElementOverlap(&test[1], &test[0], 0, 2) == NA_TRUE);
      naTest(naElementOverlap(&test[2], &test[0], 0, 2) == NA_FALSE);
    }
  #endif
}



void printNALanguage(){
  printf("NALanguage.h:" NA_NL);
  
  naPrintMacroPlain(NA_RESTRICT);
  naPrintMacroPlain(NA_INLINE);
  naPrintMacroPlain(NA_LINKER_NO_EXPORT);
  naPrintMacroPlain(NA_LINKER_EXPORT);

  naPrintMacroPlain(NA_API);
  naPrintMacroPlain(NA_DEF);
  naPrintMacroPlain(NA_IAPI);
  naPrintMacroPlain(NA_IDEF);
  naPrintMacroPlain(NA_HAPI);
  naPrintMacroPlain(NA_HDEF);
  naPrintMacroPlain(NA_HIAPI);
  naPrintMacroPlain(NA_HIDEF);

  naPrintMacroPlain(NA_NULL);
  naPrintMacroPlain(NA_NORETURN);
  naPrintMacroPlain(NA_UNUSED(parameter));

  printf(NA_NL);
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
