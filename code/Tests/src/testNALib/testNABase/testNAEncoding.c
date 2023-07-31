
#include "NATesting.h"
#include "NAUtility/NAString.h"
#include <stdio.h>


const char* na_SignedIntegerEncodingEnumStrings[] = {
  "Unknown",
  "Sign & Magnitude",
  "Ones complement",
  "Twos complement"
};

const char* na_NativeTypesEnumStrings[] = {
  "No native type",
  "native char type",
  "native short type",
  "native int type",
  "native long type",
  "native long long type",
  "native int128 type",
  "native int256 type"
};

void printNAEncoding(void){
  printf("NAEncoding.h fixed values:" NA_NL);
  naPrintMacroInt          (NA_SIGN_ENCODING_UNKNOWN);
  naPrintMacroInt          (NA_SIGN_ENCODING_SIGN_MAGNITUDE);
  naPrintMacroInt          (NA_SIGN_ENCODING_ONES_COMPLEMENT);
  naPrintMacroInt          (NA_SIGN_ENCODING_TWOS_COMPLEMENT);

  naPrintMacroInt          (NA_TYPE_NATIVE_NONE);
  naPrintMacroInt          (NA_TYPE_NATIVE_CHAR);
  naPrintMacroInt          (NA_TYPE_NATIVE_SHORT);
  naPrintMacroInt          (NA_TYPE_NATIVE_INT);
  naPrintMacroInt          (NA_TYPE_NATIVE_LONG);
  naPrintMacroInt          (NA_TYPE_NATIVE_LONG_LONG);
  naPrintMacroInt          (NA_TYPE_NATIVE_INT_128);
  naPrintMacroInt          (NA_TYPE_NATIVE_INT_256);

  printf(NA_NL "NAEncoding.h computed values:" NA_NL);
  naPrintMacroEnum         (NA_SIGN_ENCODING, na_SignedIntegerEncodingEnumStrings, 4);
  naPrintMacroInt          (NA_TYPE_NATIVE_CHAR_BITS);
  naPrintMacroInt          (NA_TYPE_NATIVE_SHORT_BITS);
  naPrintMacroInt          (NA_TYPE_NATIVE_INT_BITS);
  naPrintMacroInt          (NA_TYPE_NATIVE_LONG_BITS);
  naPrintMacroInt          (NA_TYPE_NATIVE_LONG_LONG_BITS);
  naPrintMacroEnum         (NA_TYPE_INT8_REPRESENTATION, na_NativeTypesEnumStrings, 8);
  naPrintMacroEnum         (NA_TYPE_INT16_REPRESENTATION, na_NativeTypesEnumStrings, 8);
  naPrintMacroEnum         (NA_TYPE_INT32_REPRESENTATION, na_NativeTypesEnumStrings, 8);
  naPrintMacroEnum         (NA_TYPE_INT64_REPRESENTATION, na_NativeTypesEnumStrings, 8);
  naPrintMacroEnum         (NA_TYPE_INT128_REPRESENTATION, na_NativeTypesEnumStrings, 8);
  naPrintMacroEnum         (NA_TYPE_INT256_REPRESENTATION, na_NativeTypesEnumStrings, 8);
  naPrintMacro        (NA_TYPE_INT8);
  naPrintMacro        (NA_TYPE_INT16);
  naPrintMacro        (NA_TYPE_INT32);
  naPrintMacro        (NA_TYPE_INT64);
  naPrintMacro        (NA_TYPE_INT128);
  naPrintMacro        (NA_TYPE_INT256);

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
