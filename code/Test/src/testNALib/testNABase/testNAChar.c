
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#define NA_TEST_STRINGIFY(A) #A
#define NA_TEST_NALIB_PATH(file) NA_TEST_STRINGIFY(../../../../NALib/src/file)

#include "../../testHelper.h"
#include NA_TEST_NALIB_PATH(NABase.h)
#include <stdio.h>



void testNAChar(){
  // Testing if types can be assigned.
  printf("Testing typedef NAUTF8Char" NA_NL);
  NAUTF8Char utf8Char = 'M';
}



void printNAChar(){
  printf("NAChar.h fixed values:" NA_NL);

  printMacroPlain(NA_NL_UNIX);
  printMacroPlain(NA_NL_MAC9);
  printMacroPlain(NA_NL_WIN);
  printMacroPlain(NA_TAB);
  printMacroPlain(NA_PATH_DELIMITER_UNIX);
  printMacroPlain(NA_PATH_DELIMITER_WIN);
  printMacroPlain(NA_SUFFIX_DELIMITER);

  printf(NA_NL "NAChar.h computed values:" NA_NL);
  printMacroPlain(NA_NL);
  printMacroString(NA_PRIi8);
  printMacroString(NA_PRIu8);
  printMacroString(NA_PRIx8);
  printMacroString(NA_SCNi8);
  printMacroString(NA_SCNu8);
  printMacroString(NA_PRIi16);
  printMacroString(NA_PRIu16);
  printMacroString(NA_PRIx16);
  printMacroString(NA_SCNi16);
  printMacroString(NA_SCNu16);
  printMacroString(NA_PRIi32);
  printMacroString(NA_PRIu32);
  printMacroString(NA_PRIx32);
  printMacroString(NA_SCNi32);
  printMacroString(NA_SCNu32);
  printMacroString(NA_PRIi64);
  printMacroString(NA_PRIu64);
  printMacroString(NA_PRIx64);
  printMacroString(NA_SCNi64);
  printMacroString(NA_SCNu64);

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

