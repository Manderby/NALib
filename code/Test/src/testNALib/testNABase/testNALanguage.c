
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../testHelper.h"
#include NA_TEST_NALIB_PATH(../../, NABase.h)
#include <stdio.h>



void na_accessor_func(const void* param){
  NA_UNUSED(param);
}
void na_mutator_func(void* param){
  NA_UNUSED(param);
}



void testNALanguage(){
  naG("typedefs"){
    NAAccessor testAccessor = na_accessor_func;
    NAMutator testMutator = na_mutator_func;
    naT(testAccessor == na_accessor_func);
    naT(testMutator == na_mutator_func);
  }

  #ifndef NDEBUG
    naG("ElementOverlap"){
      int32 test[3] = {42, 42, 42};
      naT(naElementOverlap(&test[0], &test[0], 0, 0) == NA_FALSE);
      naT(naElementOverlap(&test[0], &test[1], 0, 0) == NA_FALSE);
      naT(naElementOverlap(&test[0], &test[1], 0, 1) == NA_FALSE);
      naT(naElementOverlap(&test[1], &test[0], 0, 0) == NA_FALSE);
      naT(naElementOverlap(&test[0], &test[2], 2, 0) == NA_FALSE);
      naT(naElementOverlap(&test[0], &test[1], 2, 0) == NA_TRUE);
      naT(naElementOverlap(&test[1], &test[0], 0, 2) == NA_TRUE);
      naT(naElementOverlap(&test[2], &test[0], 0, 2) == NA_FALSE);
    }
  #endif
}



void printNALanguage(){
  printf("NALanguage.h:" NA_NL);
  
  printMacroPlain(NA_RESTRICT);
  printMacroPlain(NA_INLINE);
  printMacroPlain(NA_LINKER_NO_EXPORT);
  printMacroPlain(NA_LINKER_EXPORT);

  printMacroPlain(NA_API);
  printMacroPlain(NA_DEF);
  printMacroPlain(NA_IAPI);
  printMacroPlain(NA_IDEF);
  printMacroPlain(NA_HAPI);
  printMacroPlain(NA_HDEF);
  printMacroPlain(NA_HIAPI);
  printMacroPlain(NA_HIDEF);

  printMacroPlain(NA_NULL);
  printMacroPlain(NA_NORETURN);
  printMacroPlain(NA_UNUSED(parameter));

  printf(NA_NL);
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

