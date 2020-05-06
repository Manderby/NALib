
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
  // Testing if types can be assigned.
  printf("Testing typedef NAAccessor" NA_NL);
  NAAccessor testAccessor = na_accessor_func;

  printf("Testing typedef NAMutator" NA_NL);
  NAMutator testMutator = na_mutator_func;
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

