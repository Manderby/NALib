
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#define NA_TEST_STRINGIFY(A) #A
#define NA_TEST_NALIB_PATH(file) NA_TEST_STRINGIFY(../../../../NALib/src/file)

#include "../../testHelper.h"
#include NA_TEST_NALIB_PATH(NABase.h)
#include NA_TEST_NALIB_PATH(NAMemory.h)
#include <stdio.h>



const char* na_memalign_enum_strings[] = {
  "Custom",
  "aligned_alloc",
  "Posix"
};

void printNAConfiguration(){
  printf("NAConfiguration.h:" NA_NL);
  printMacroIntSpecial   (NA_PREFERRED_NAINT_BITS, 0, "Automatic");
  printMacroIntYesNo     (NA_COMPILE_OPENGL);
  printMacroIntYesNo     (NA_COMPILE_GUI);
  printMacroIntYesNo     (NA_USE_WINDOWS_COMMON_CONTROLS_6);
  printMacroIntYesNo     (NA_INLINE_DEFINITION_NON_STANDARD);
  printMacroEnum         (NA_MEMALIGN, na_memalign_enum_strings, 3);
  printMacroIntSpecialHex(NA_POOLPART_BYTESIZE, 0, "Memory Page Size");
  printMacroIntYesNo     (NA_MEMORY_POOL_AGGRESSIVE_CLEANUP);
  printMacroIntSpecial   (NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT, 0, "No autocollect");
  printMacroIntSpecial   (NA_BUFFER_PART_BYTESIZE, 0, "Memory Page Size");
  printMacroIntYesNo     (NA_STRING_ALWAYS_CACHE);
  printMacroIntYesNo     (NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION);
  printMacroInt          (NA_NIST_CODATA_YEAR);
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

