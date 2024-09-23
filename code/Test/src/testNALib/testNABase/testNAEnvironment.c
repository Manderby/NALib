
#include "NATest.h"
#include "NAUtility/NAString.h"
#include <stdio.h>



const char* na_OsEnumStrings[] = {
  "Unknown",
  "macOS",
  "Windows"
};

const char* na_EndiannessEnumStrings[] = {
  "Unknown",
  "Little",
  "Big",
  "Network"
};

void printNAEnvironment(void) {
  printf("NAEnvironment.h fixed values:" NA_NL);
  naPrintMacroInt          (NA_BITS_PER_BYTE);
  naPrintMacroInt          (NA_TYPE8_BITS);
  naPrintMacroInt          (NA_TYPE16_BITS);
  naPrintMacroInt          (NA_TYPE32_BITS);
  naPrintMacroInt          (NA_TYPE64_BITS);
  naPrintMacroInt          (NA_TYPE128_BITS);
  naPrintMacroInt          (NA_TYPE256_BITS);
  naPrintMacroInt          (NA_TYPE8_BYTES);
  naPrintMacroInt          (NA_TYPE16_BYTES);
  naPrintMacroInt          (NA_TYPE32_BYTES);
  naPrintMacroInt          (NA_TYPE64_BYTES);
  naPrintMacroInt          (NA_TYPE128_BYTES);
  naPrintMacroInt          (NA_TYPE256_BYTES);
  naPrintMacroInt          (NA_ENDIANNESS_UNKNOWN);
  naPrintMacroInt          (NA_ENDIANNESS_LITTLE);
  naPrintMacroInt          (NA_ENDIANNESS_BIG);
  naPrintMacroInt          (NA_ENDIANNESS_NETWORK);
  naPrintMacroInt          (NA_OS_UNKNOWN);
  naPrintMacroInt          (NA_OS_MAC_OS_X);
  naPrintMacroInt          (NA_OS_WINDOWS);

  printf(NA_NL "NAEnvironment.h computed values:" NA_NL);
  naPrintMacroEnum         (NA_OS, na_OsEnumStrings, 3);
  naPrintMacroEnum         (NA_ENDIANNESS_HOST, na_EndiannessEnumStrings, 4);
  naPrintMacroInt          (NA_ADDRESS_BITS);
  naPrintMacroInt          (NA_ADDRESS_BYTES);
  naPrintMacroInt          (NA_SIZE_T_BITS);
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
