
#include "NATest.h"
#include "NAUtility/NAString.h"
#include <stdio.h>



void testNAMacros(void) {
  naTest(strcmp(NA_STRINGIFY(AsDf), "AsDf") == 0);

  int AB = 12;
  int ABC = 123;
  int ABCD = 1234;
  int NameA = 56;
  int NameAB = 567;
  int NameABC = 5678;

  #define M1 A
  #define M2 B
  #define M3 C
  #define M4 D
  #define MNAME Name

  naTest(NA_CONCAT2(A, B) == 12);
  naTest(NA_CONCAT3(A, B, C) == 123);
  naTest(NA_CONCAT4(A, B, C, D) == 1234);
  naTest(NA_CONCAT_EVAL2(M1, M2) == 12);
  naTest(NA_CONCAT_EVAL3(M1, M2, M3) == 123);
  naTest(NA_CONCAT_EVAL4(M1, M2, M3, M4) == 1234);
  naTest(NA_T1(MNAME, M1) == 56);
  naTest(NA_T2(MNAME, M1, M2) == 567);
  naTest(NA_T3(MNAME, M1, M2, M3) == 5678);

  #undef M1
  #undef M2
  #undef M
  #undef M4
  #undef MNAME
}



void printNAMacros(void) {
  printf("NAMacros.h:" NA_NL);

  #define M1 A
  #define M2 B
  #define M3 C
  #define M4 D
  #define MNAME Name

  naPrintMacro(NA_STRINGIFY(macro));
  naPrintMacro(NA_CONCAT2(M1, M2));
  naPrintMacro(NA_CONCAT3(M1, M2, M3));
  naPrintMacro(NA_CONCAT4(M1, M2, M3, M4));
  naPrintMacro(NA_CONCAT_EVAL2(M1, M2));
  naPrintMacro(NA_CONCAT_EVAL3(M1, M2, M3));
  naPrintMacro(NA_CONCAT_EVAL4(M1, M2, M3, M4));
  naPrintMacro(NA_T1(MNAME, M1));
  naPrintMacro(NA_T2(MNAME, M1, M2));
  naPrintMacro(NA_T3(MNAME, M1, M2, M3));

  #undef M1
  #undef M2
  #undef M
  #undef M4
  #undef MNAME

  naPrintMacro(naPrintMacro(macro));
  naPrintMacro(naPrintMacroInt(macro));
  naPrintMacro(naPrintMacroIntSpecial(macro, v, s));
  naPrintMacro(naPrintMacroIntSpecialHex(macro, v, s));
  naPrintMacro(naPrintMacroIntYesNo(macro));
  naPrintMacro(naPrintMacroEnum(macro, s, m));

  naPrintMacro(naPrintMacroux8(macro));
  naPrintMacro(naPrintMacroix8(macro));
  naPrintMacro(naPrintMacroux16(macro));
  naPrintMacro(naPrintMacroix16(macro));
  naPrintMacro(naPrintMacroux32(macro));
  naPrintMacro(naPrintMacroix32(macro));
  naPrintMacro(naPrintMacroux64(macro));
  naPrintMacro(naPrintMacroix64(macro));
  naPrintMacro(naPrintMacroux128(macro));
  naPrintMacro(naPrintMacroix128(macro));
  naPrintMacro(naPrintMacroux256(macro));
  naPrintMacro(naPrintMacroix256(macro));

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
