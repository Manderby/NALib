
#include "NABase.h"
#include "NAString.h"
#include <stdio.h>



void testNAFloatingPoint(){
  // Never seen this kind of literal yet? Go look for hex float literals!
  // It is actually quite useful for testing. One only has to remember that
  // single precision only use 23 bits for the significant, which is why the
  // hex digits after the point of the compared value appear to be doubled.
  naG("naMakeFloat valid cases") {
    int32 number = 0x123456;
    int32 maxNumber = 0x7fffff;
    int32 negNumber = -number;
    int32 negMaxNumber = -maxNumber;
    naT(naMakeFloat(0, 0) == 0x1.000000p0f);
    naT(naMakeFloat(1, 0) == 0x1.000002p0f);
    naT(naMakeFloat(1, 42) == 0x1.000002p42f);
    naT(naMakeFloat(-1, 42) == -0x1.000002p42f);
    naT(naMakeFloat(1, -42) == 0x1.000002p-42f);
    naT(naMakeFloat(-1, -42) == -0x1.000002p-42f);
    naT(naMakeFloat(number, 42) == 0x1.2468acp42f);
    naT(naMakeFloat(negNumber, 42) == -0x1.2468acp42f);
    naT(naMakeFloat(maxNumber, 42) == 0x1.fffffep42f);
    naT(naMakeFloat(negMaxNumber, 42) == -0x1.fffffep42f);
    naT(naMakeFloat(maxNumber, 127) == 0x1.fffffep127f);
    naT(naMakeFloat(0, -126) == 0x1.000000p-126f);
    naT(naMakeFloat(0x0, -127) == 0.f);
  }

  naG("naMakeFloat invalid cases") {
    naE(naMakeFloat(0x800000, 0));
    naE(naMakeFloat(0x1, 1000));
    naE(naMakeFloat(0x1, 128));
    naE(naMakeFloat(0x1, -127));
    naE(naMakeFloat(0x1, -1000));
  }
}



void printNAFloatingPoint(){
  printf("NAFloatingPoint.h:" NA_NL);

  naPrintMacroInt(NA_IEEE754_SINGLE_BITS);
  naPrintMacroInt(NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
  naPrintMacroux32(NA_IEEE754_SINGLE_SIGNIFICAND_NORM, NA_FALSE);
  naPrintMacroux32(NA_IEEE754_SINGLE_SIGNIFICAND_MASK, NA_FALSE);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_BITS);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_BIAS);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
  naPrintMacroux32(NA_IEEE754_SINGLE_EXPONENT_MASK, NA_FALSE);
  naPrintMacroux32(NA_IEEE754_SINGLE_SIGN_MASK, NA_FALSE);

  naPrintMacroInt(NA_IEEE754_DOUBLE_BITS);
  naPrintMacroInt(NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  naPrintMacroux64(NA_IEEE754_DOUBLE_SIGNIFICAND_NORM, NA_FALSE);
  naPrintMacroux64(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK, NA_FALSE);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_BITS);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_BIAS);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_SPECIAL);
  naPrintMacroux64(NA_IEEE754_DOUBLE_EXPONENT_MASK, NA_FALSE);
  naPrintMacroux64(NA_IEEE754_DOUBLE_SIGN_MASK, NA_FALSE);

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
