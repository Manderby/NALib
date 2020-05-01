
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "testNALib.h"

#define NA_TEST_MAX_MACRO_LENGTH 40

#define printMacroHead(macro)\
{\
  const char* macroStr = macro;\
  printf(macroStr);\
  for(int c = 0; c < NA_TEST_MAX_MACRO_LENGTH - strlen(macroStr); c++){printf(" ");}\
}

#define printMacroInt(macro)\
  printMacroHead(#macro)\
  printf("%d", macro);\
  printf(NA_NL);

#define printMacroIntHex(macro)\
  printMacroHead(#macro)\
  printf("0x%x (%d)", macro, macro);\
  printf(NA_NL);

#define printMacroIntYesNo(macro)\
  printMacroHead(#macro)\
  if(macro){\
    printf("%d (Yes)", macro);\
  }else{\
    printf("0 (No)");\
  }\
  printf(NA_NL);

#define printMacroIntSpecial(macro, specialValue, specialString)\
  printMacroHead(#macro)\
  if(macro == specialValue){\
    printf(specialString);\
  }else{\
    printf("%d", macro);\
  }\
  printf(NA_NL);



int main(void){

  printf("Configuration.h:" NA_NL);
  printMacroIntSpecial(NA_PREFERRED_NAINT_BITS, 0, "Automatic");
  printMacroIntYesNo  (NA_CONFIG_COMPILE_OPENGL);
  printMacroIntYesNo  (NA_CONFIG_COMPILE_GUI);
  printMacroIntYesNo  (NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6);
  //printMacroDefinedInt(NA_MEMORY_ALIGNED_MEM_MAC_OS_X);
  printMacroIntHex    (NA_POOLPART_BYTESIZE);
  printMacroIntYesNo  (NA_MEMORY_POOL_AGGRESSIVE_CLEANUP);
  printMacroInt       (NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT);
  printMacroIntSpecial(NA_BUFFER_PART_BYTESIZE, 0, "page");
  printMacroIntYesNo  (NA_STRING_ALWAYS_CACHE);
  printMacroIntYesNo  (NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION);
  printMacroInt       (NA_NIST_CODATA_YEAR);

  #if NA_OS == NA_OS_WINDOWS
    printf(NA_NL "Finished." NA_NL);
    NA_UNUSED(getchar());
  #endif

  return 0;
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

