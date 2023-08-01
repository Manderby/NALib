
#include "NATest.h"
#include "NAUtility/NAMemory.h"
#include <stdio.h>



const char* na_MemAlignEnumStrings[] = {
  "Custom",
  "aligned_alloc",
  "Posix"
};

void printNAConfiguration(void){
  printf("NAConfiguration.h:" NA_NL);
  naPrintMacroIntYesNo     (NA_DEBUG);
  naPrintMacroIntSpecial   (NA_PREFERRED_NAINT_BITS, 0, "Automatic");
  naPrintMacroIntYesNo     (NA_COMPILE_OPENGL);
  naPrintMacroIntYesNo     (NA_COMPILE_GUI);
  naPrintMacroIntYesNo     (NA_USE_WINDOWS_COMMON_CONTROLS_6);
  naPrintMacroIntYesNo     (NA_INLINE_DEFINITION_NON_STANDARD);
  naPrintMacroEnum         (NA_MEMALIGN, na_MemAlignEnumStrings, 3);
  naPrintMacroIntSpecialHex(NA_POOLPART_BYTESIZE, 0, "Memory Page Size");
  naPrintMacroIntYesNo     (NA_MEMORY_POOL_AGGRESSIVE_CLEANUP);
  naPrintMacroIntSpecial   (NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT, 0, "No autocollect");
  naPrintMacroIntSpecial   (NA_BUFFER_PART_BYTESIZE, 0, "Memory Page Size");
  naPrintMacroIntYesNo     (NA_STRING_ALWAYS_CACHE);
  naPrintMacroIntYesNo     (NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION);
  naPrintMacroInt          (NA_NIST_CODATA_YEAR);
  naPrintMacroIntYesNo     (NA_TESTING_ENABLED);
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
