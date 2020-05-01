
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../testNALib.h"


const char* na_os_enum_strings[] = {
  "Unknown",
  "macOS",
  "Windows"
};

const char* na_endianness_enum_strings[] = {
  "Unknown",
  "Little",
  "Big",
  "Network"
};

void printNAEnvironment(){
  printf("NAEnvironment.h fixed values:" NA_NL);
  printMacroInt          (NA_BITS_PER_BYTE);
  printMacroInt          (NA_TYPE8_BITS);
  printMacroInt          (NA_TYPE16_BITS);
  printMacroInt          (NA_TYPE32_BITS);
  printMacroInt          (NA_TYPE64_BITS);
  printMacroInt          (NA_TYPE128_BITS);
  printMacroInt          (NA_TYPE256_BITS);
  printMacroInt          (NA_TYPE8_BYTES);
  printMacroInt          (NA_TYPE16_BYTES);
  printMacroInt          (NA_TYPE32_BYTES);
  printMacroInt          (NA_TYPE64_BYTES);
  printMacroInt          (NA_TYPE128_BYTES);
  printMacroInt          (NA_TYPE256_BYTES);
  printMacroInt          (NA_ENDIANNESS_UNKNOWN);
  printMacroInt          (NA_ENDIANNESS_LITTLE);
  printMacroInt          (NA_ENDIANNESS_BIG);
  printMacroInt          (NA_ENDIANNESS_NETWORK);
  printMacroInt          (NA_OS_UNKNOWN);
  printMacroInt          (NA_OS_MAC_OS_X);
  printMacroInt          (NA_OS_WINDOWS);

  printf(NA_NL "NAEnvironment.h computed values:" NA_NL);
  printMacroEnum         (NA_OS, na_os_enum_strings, 3);
  printMacroEnum         (NA_ENDIANNESS_HOST, na_endianness_enum_strings, 4);
  printMacroInt          (NA_ADDRESS_BITS);
  printMacroInt          (NA_ADDRESS_BYTES);
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

