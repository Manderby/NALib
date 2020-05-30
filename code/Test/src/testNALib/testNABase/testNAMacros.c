
#include "NABase.h"
#include <stdio.h>




void printNAMacros(){
  printf("NAMacros.h:" NA_NL);

  #define M1 A
  #define M2 B
  #define M3 C
  #define M4 D
  #define MNAME Name

  naPrintMacroPlain(NA_STRINGIFY(macro));
  naPrintMacroPlain(NA_CONCAT2(M1, M2));
  naPrintMacroPlain(NA_CONCAT3(M1, M2, M3));
  naPrintMacroPlain(NA_CONCAT4(M1, M2, M3, M4));
  naPrintMacroPlain(NA_CONCAT_EVAL2(M1, M2));
  naPrintMacroPlain(NA_CONCAT_EVAL3(M1, M2, M3));
  naPrintMacroPlain(NA_CONCAT_EVAL4(M1, M2, M3, M4));
  naPrintMacroPlain(NA_T1(MNAME, M1));
  naPrintMacroPlain(NA_T2(MNAME, M1, M2));
  naPrintMacroPlain(NA_T3(MNAME, M1, M2, M3));

  #undef M1
  #undef M2
  #undef M
  #undef M4
  #undef MNAME

  naPrintMacroPlain(naPrintMacroPlain(macro));
  naPrintMacroPlain(naPrintMacroInt(macro));
  naPrintMacroPlain(naPrintMacroIntSpecial(macro, v, s));
  naPrintMacroPlain(naPrintMacroIntSpecialHex(macro, v, s));
  naPrintMacroPlain(naPrintMacroIntYesNo(macro));
  naPrintMacroPlain(naPrintMacroEnum(macro, s, m));

  naPrintMacroPlain(naPrintMacroux8(macro));
  naPrintMacroPlain(naPrintMacroix8(macro));
  naPrintMacroPlain(naPrintMacroux16(macro));
  naPrintMacroPlain(naPrintMacroix16(macro));
  naPrintMacroPlain(naPrintMacroux32(macro));
  naPrintMacroPlain(naPrintMacroix32(macro));
  naPrintMacroPlain(naPrintMacroux64(macro));
  naPrintMacroPlain(naPrintMacroix64(macro));
  naPrintMacroPlain(naPrintMacroux128(macro));
  naPrintMacroPlain(naPrintMacroix128(macro));
  naPrintMacroPlain(naPrintMacroux256(macro));
  naPrintMacroPlain(naPrintMacroix256(macro));

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
