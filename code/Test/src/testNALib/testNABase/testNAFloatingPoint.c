
#include "NATesting.h"
#include "NAString.h"
#include <stdio.h>



void testMakeFloat(void){
  // Never seen this kind of literal yet? Go look for hex float literals!
  // It is actually quite useful for testing. One only has to remember that
  // single precision only use 23 bits for the significant, which is why the
  // hex digits after the point of the compared value appear to be doubled.
  naTestGroup("naMakeFloat valid cases"){
    int32 number = 0x712345;
    int32 maxNumber = 0x7fffff;
    int32 negNumber = -number;
    int32 negMaxNumber = -maxNumber;
    naTest(naMakeFloat(0, 0) == 0x1.000000p0f);
    naTest(naMakeFloat(1, 0) == 0x1.000002p0f);
    naTest(naMakeFloat(1, 42) == 0x1.000002p42f);
    naTest(naMakeFloat(-1, 42) == -0x1.000002p42f);
    naTest(naMakeFloat(1, -42) == 0x1.000002p-42f);
    naTest(naMakeFloat(-1, -42) == -0x1.000002p-42f);
    naTest(naMakeFloat(number, 42) == 0x1.e2468ap42f);
    naTest(naMakeFloat(negNumber, 42) == -0x1.e2468ap42f);
    naTest(naMakeFloat(maxNumber, 42) == 0x1.fffffep42f);
    naTest(naMakeFloat(negMaxNumber, 42) == -0x1.fffffep42f);
    naTest(naMakeFloat(maxNumber, 127) == 0x1.fffffep127f);
    naTest(naMakeFloat(0, -126) == 0x1.000000p-126f);
    naTest(naMakeFloat(0, -127) == 0.f);
  }

  naTestGroup("naMakeFloat invalid cases"){
    naTestError(naMakeFloat(0x800000, 0));
    naTestError(naMakeFloat(0x1, 1000));
    naTestError(naMakeFloat(0x1, 128));
    naTestError(naMakeFloat(0x1, -127));
    naTestError(naMakeFloat(0x1, -1000));
  }

  naTestGroup("naMakeFloatWithExponent valid cases"){
    naTest(naMakeFloatWithExponent(0) == 1.f);
    naTest(naMakeFloatWithExponent(1) == 2.f);
    naTest(naMakeFloatWithExponent(8) == 256.f);
    naTest(naMakeFloatWithExponent(42) == 0x1.000000p42f);
    naTest(naMakeFloatWithExponent(127) == 0x1.000000p127f);
    naTest(naMakeFloatWithExponent(-1) == 0.5f);
    naTest(naMakeFloatWithExponent(-42) == 0x1.000000p-42f);
    naTest(naMakeFloatWithExponent(-126) == 0x1.000000p-126f);
  }

  naTestGroup("naMakeFloatWithExponent invalid cases"){
    naTestError(naMakeFloatWithExponent(128));
    naTestError(naMakeFloatWithExponent(1000));
    naTestError(naMakeFloatWithExponent(-127));
    naTestError(naMakeFloatWithExponent(-1000));
  }

  naTestGroup("naMakeFloatSubnormal valid cases"){
    int32 number = 0x712345;
    int32 maxNumber = 0x7fffff;
    int32 negNumber = -number;
    int32 negMaxNumber = -maxNumber;
    naTest(naMakeFloatSubnormal(1) > 0.f);
    naTest(naMakeFloatSubnormal(number) > 0.f);
    naTest(naMakeFloatSubnormal(maxNumber) > 0.f);
    naTest(naMakeFloatSubnormal(negNumber) < 0.f);
    naTest(naMakeFloatSubnormal(negMaxNumber) < 0.f);
    naTest(naMakeFloatSubnormal(1) < 0x1.000000p-126f);
    naTest(naMakeFloatSubnormal(number) < 0x1.000000p-126f);
    naTest(naMakeFloatSubnormal(maxNumber) < 0x1.000000p-126f);
    naTest(naMakeFloatSubnormal(negNumber) > -0x1.000000p-126f);
    naTest(naMakeFloatSubnormal(negMaxNumber) > -0x1.000000p-126f);
  }

  naTestGroup("naMakeFloatSubnormal invalid cases"){
    naTestError(naMakeFloatSubnormal(0x800000));
    naTestError(naMakeFloatSubnormal(-0x800000));
  }
}



void testMakeDouble(void){
  // Never seen this kind of literal yet? Go look for hex float literals!
  // It is actually quite useful for testing..
  naTestGroup("naMakeDouble valid cases"){
    int64 number = 0xf123456789abc;
    int64 maxNumber = 0xfffffffffffff;
    int64 negNumber = -number;
    int64 negMaxNumber = -maxNumber;
    naTest(naMakeDouble(0, 0) == 0x1.0000000000000p0);
    naTest(naMakeDouble(1, 0) == 0x1.0000000000001p0);
    naTest(naMakeDouble(1, 42) == 0x1.0000000000001p42);
    naTest(naMakeDouble(-1, 42) == -0x1.0000000000001p42);
    naTest(naMakeDouble(1, -42) == 0x1.0000000000001p-42);
    naTest(naMakeDouble(-1, -42) == -0x1.0000000000001p-42);
    naTest(naMakeDouble(number, 42) == 0x1.f123456789abcp42);
    naTest(naMakeDouble(negNumber, 42) == -0x1.f123456789abcp42);
    naTest(naMakeDouble(maxNumber, 42) == 0x1.fffffffffffffp42);
    naTest(naMakeDouble(negMaxNumber, 42) == -0x1.fffffffffffffp42);
    naTest(naMakeDouble(maxNumber, 1023) == 0x1.fffffffffffffp1023);
    naTest(naMakeDouble(0, -1022) == 0x1.0000000000000p-1022);
    naTest(naMakeDouble(0, -1023) == 0.f);
  }

  naTestGroup("naMakeDouble invalid cases"){
    naTestError(naMakeDouble(0x10000000000000, 0));
    naTestError(naMakeDouble(0x1, 10000));
    naTestError(naMakeDouble(0x1, 1024));
    naTestError(naMakeDouble(0x1, -1023));
    naTestError(naMakeDouble(0x1, -10000));
  }

  naTestGroup("naMakeDoubleWithExponent valid cases"){
    naTest(naMakeDoubleWithExponent(0) == 1.f);
    naTest(naMakeDoubleWithExponent(1) == 2.f);
    naTest(naMakeDoubleWithExponent(8) == 256.f);
    naTest(naMakeDoubleWithExponent(42) == 0x1.000000p42);
    naTest(naMakeDoubleWithExponent(1023) == 0x1.000000p1023);
    naTest(naMakeDoubleWithExponent(-1) == 0.5f);
    naTest(naMakeDoubleWithExponent(-42) == 0x1.000000p-42);
    naTest(naMakeDoubleWithExponent(-1022) == 0x1.000000p-1022);
  }

  naTestGroup("naMakeDoubleWithExponent invalid cases"){
    naTestError(naMakeDoubleWithExponent(1024));
    naTestError(naMakeDoubleWithExponent(10000));
    naTestError(naMakeDoubleWithExponent(-1023));
    naTestError(naMakeDoubleWithExponent(-10000));
  }

  naTestGroup("naMakeDoubleSubnormal valid cases"){
    int64 number = 0xf123456789abc;
    int64 maxNumber = 0xfffffffffffff;
    int64 negNumber = -number;
    int64 negMaxNumber = -maxNumber;
    naTest(naMakeDoubleSubnormal(1) > 0.);
    naTest(naMakeDoubleSubnormal(number) > 0.);
    naTest(naMakeDoubleSubnormal(maxNumber) > 0.);
    naTest(naMakeDoubleSubnormal(negNumber) < 0.);
    naTest(naMakeDoubleSubnormal(negMaxNumber) < 0.);
    naTest(naMakeDoubleSubnormal(1) < 0x1.000000p-1022);
    naTest(naMakeDoubleSubnormal(number) < 0x1.000000p-1022);
    naTest(naMakeDoubleSubnormal(maxNumber) < 0x1.000000p-1022);
    naTest(naMakeDoubleSubnormal(negNumber) > -0x1.000000p-1022);
    naTest(naMakeDoubleSubnormal(negMaxNumber) > -0x1.000000p-1022);
  }

  naTestGroup("naMakeDoubleSubnormal invalid cases"){
    naTestError(naMakeDoubleSubnormal(0x10000000000000));
    naTestError(naMakeDoubleSubnormal(-0x10000000000000));
  }
}



void testGetFloat(void){
  naTestGroup("naGetFloatExponent valid cases"){
    naTest(naGetFloatExponent(1.f) == 0);
    naTest(naGetFloatExponent(0x1.0p42f) == 42);
    naTest(naGetFloatExponent(0x1.0p-42f) == -42);
    naTest(naGetFloatExponent(0x1.0p127f) == 127);
    naTest(naGetFloatExponent(0x1.0p-126f) == -126);
  }

  naTestGroup("naGetFloatExponent invalid cases"){
    naTestError(naGetFloatExponent(0x1.0p-127)); // subnormal
    naTestError(naGetFloatExponent(0x1.0p-149)); // subnormal -126-23
    naTestError(naGetFloatExponent(0.f));
    naTestError(naGetFloatExponent(NA_INFINITYf));
  }

  naTestGroup("naGetFloatInteger valid cases"){
    naTest(naGetFloatInteger(0.f) == 0);
    naTest(naGetFloatInteger(-0.f) == 0);
    naTest(naGetFloatInteger(0.001234f) == 0);
    naTest(naGetFloatInteger(0x0.fffffep0f) == 0);
    naTest(naGetFloatInteger(0.999999999f) == 1);
    naTest(naGetFloatInteger(42.f) == 42);
    naTest(naGetFloatInteger(-42.f) == -42);
    naTest(naGetFloatInteger(0x1.fffffep23f) == 0xffffff);
    naTest(naGetFloatInteger(-0x1.fffffep23f) == -0xffffff);
    naTest(naGetFloatInteger(0x00ffffff) == 0x00ffffff);
    naTest(naGetFloatInteger(-0x00ffffff) == -0x00ffffff);
}

  naTestGroup("naGetFloatInteger invalid cases"){
    naTestError(naGetFloatInteger(NA_INFINITYf));
    naTestError(naGetFloatInteger(0x1.fffffep24f));
    naTestError(naGetFloatInteger(0x01000000));
    naTestError(naGetFloatInteger(-0x01000000));
  }

  naTestGroup("naGetFloatFraction valid cases"){
    naTest(naGetFloatFraction(0.0f) == 0);
    naTest(naGetFloatFraction(0.5f) == 500000);
    naTest(naGetFloatFraction(0.001234f) == 1234);
    naTest(naGetFloatFraction(0.123456f) == 123456);
    naTest(naGetFloatFraction(0.000001f) == 000001);
    naTest(naGetFloatFraction(0.999999f) == 999999);
  }

  naTestGroup("naGetFloatFraction invalid cases"){
    naTestError(naGetFloatFraction(1.0f));
    naTestError(naGetFloatFraction(1111.0f));
    naTestError(naGetFloatFraction(1.56f));
    naTestError(naGetFloatFraction(123.56f));
    naTestError(naGetFloatFraction(1234.12f));
    naTestError(naGetFloatFraction(0.999999999f)); // compiler rounding up
    naTestError(naGetFloatFraction(0x0.fffffep0f));  // function rounding up
  }

  naTestGroup("naGetFloatFractionE"){
    naTest(naGetFloatFractionE(1.0f) == 0);
    naTest(naGetFloatFractionE(1111.0f) == 0);
    naTest(naGetFloatFractionE(1.56f) >= 550000);
    naTest(naGetFloatFractionE(1.56f) <= 570000);
    naTest(naGetFloatFractionE(123.56f) >= 550000);
    naTest(naGetFloatFractionE(123.56f) <= 570000);
    naTest(naGetFloatFractionE(1234.12f) >= 110000);
    naTest(naGetFloatFractionE(1234.12f) <= 130000);
    naTest(naGetFloatFractionE(0.999999999f) == 0); // compiler rounding up
    naTest(naGetFloatFractionE(0x0.fffffep0f) == 1000000);  // function rounding up
  }

  naTestGroup("naGetFloatFractionSlow valid cases"){
    naTest(naGetFloatFractionSlow(0.0f) == 0);
    naTest(naGetFloatFractionSlow(1.0f) == 0);
    naTest(naGetFloatFractionSlow(1111.0f) == 0);
    naTest(naGetFloatFractionSlow(0.5f) == 500000);
    naTest(naGetFloatFractionSlow(0.001234f) == 1234);
    naTest(naGetFloatFractionSlow(0.123456f) == 123456);
    naTest(naGetFloatFractionSlow(1.56f) == 560000);
    naTest(naGetFloatFractionSlow(123.56f) == 560000);
    naTest(naGetFloatFractionSlow(1234.12f) == 120000);
    naTest(naGetFloatFractionSlow(0.000001f) == 000001);
    naTest(naGetFloatFractionSlow(0.999999f) == 999999);
    naTest(naGetFloatFractionSlow(0.999999999f) == 0); // compiler rounds up to 1
  }

  naTestGroup("naGetFloatFractionSlow invalid cases"){
    naTestError(naGetFloatFractionSlow(0x0.fffffep0f));  // rounding up
  }

  naTestGroup("naGetFloatFractionSlowE valid cases"){
    naTest(naGetFloatFractionSlowE(0x0.fffffep0f) == 1000000);  // rounding up
  }
}



void testGetDouble(void){
  naTestGroup("naGetDoubleExponent valid cases"){
    naTest(naGetDoubleExponent(1.) == 0);
    naTest(naGetDoubleExponent(0x1.0p42) == 42);
    naTest(naGetDoubleExponent(0x1.0p-42) == -42);
    naTest(naGetDoubleExponent(0x1.0p1023) == 1023);
    naTest(naGetDoubleExponent(0x1.0p-1022) == -1022);
  }

  naTestGroup("naGetDoubleExponent invalid cases"){
    naTestError(naGetDoubleExponent(0x1.0p-1023)); // subnormal
    naTestError(naGetDoubleExponent(0x1.0p-1074)); // subnormal -1022-52
    naTestError(naGetDoubleExponent(0.));
    naTestError(naGetDoubleExponent(NA_INFINITY));
  }
 
  naTestGroup("naGetDoubleInteger valid cases"){
    naTest(naGetDoubleInteger(0.) == 0);
    naTest(naGetDoubleInteger(-0.) == 0);
    naTest(naGetDoubleInteger(0.00001234) == 0);
    naTest(naGetDoubleInteger(0x0.fffffffffffffp0) == 0);
    naTest(naGetDoubleInteger(0.99999999999999999) == 1);
    naTest(naGetDoubleInteger(42.) == 42);
    naTest(naGetDoubleInteger(-42.) == -42);
    naTest(naGetDoubleInteger(0x1.fffffffffffffp52) == 0x1fffffffffffff);
    naTest(naGetDoubleInteger(-0x1.fffffffffffffp52) == -0x1fffffffffffff);
    naTest(naEquali64(naGetDoubleInteger(naCasti64ToDouble(naMakei64(0x001fffff, 0xffffffff))), naMakei64(0x001fffff, 0xffffffff)));
    naTest(naGetDoubleInteger(naCasti64ToDouble(naNegi64(naMakei64(0x001fffff, 0xffffffff)))) == naNegi64(naMakei64(0x001fffff, 0xffffffff)));
  }

  naTestGroup("naGetDoubleInteger invalid cases"){
    naTestError(naGetDoubleInteger(NA_INFINITY));
    naTestError(naGetDoubleInteger(0x1.fffffffffffffp53));
    naTestError(naGetDoubleInteger(naCasti64ToDouble(naMakei64(0x00200000, 0x00000000))));
    naTestError(naGetDoubleInteger(naCasti64ToDouble(naNegi64(naMakei64(0x00200000, 0x00000000)))));
  }

  naTestGroup("naGetDoubleFraction valid cases"){
    naTest(naGetDoubleFraction(0.0) == 0);
    naTest(naGetDoubleFraction(0.5) == 500000000000000);
    naTest(naGetDoubleFraction(0.00001234) == 12340000000);
    naTest(naGetDoubleFraction(0.123456) == 123456000000000);
    naTest(naGetDoubleFraction(0.000000000000001) == 000000000000001);
    naTest(naGetDoubleFraction(0.999999999999999) == 999999999999999);
  }

  naTestGroup("naGetDoubleFraction invalid cases"){
    naTestError(naGetDoubleFraction(1.0));
    naTestError(naGetDoubleFraction(1111.0));
    naTestError(naGetDoubleFraction(1234.5678));
    naTestError(naGetDoubleFraction(123456789.12345));
    naTestError(naGetDoubleFraction(123456789.12345678));
    naTestError(naGetDoubleFraction(0.99999999999999999)); // compiler rounding up
    naTestError(naGetDoubleFraction(0x0.fffffffffffffp0));  // function rounding up
  }

  naTestGroup("naGetDoubleFractionE"){
    naTest(naGetDoubleFractionE(1.0) == 0);
    naTest(naGetDoubleFractionE(1111.0) == 0);
    naTest(naGetDoubleFractionE(1234.5678) >= 567700000000000);
    naTest(naGetDoubleFractionE(1234.5678) <= 567900000000000);
    naTest(naGetDoubleFractionE(123456789.12345) >= 123440000000000);
    naTest(naGetDoubleFractionE(123456789.12345) <= 123460000000000);
    naTest(naGetDoubleFractionE(123456789.12345678) >= 123455000000000);
    naTest(naGetDoubleFractionE(123456789.12345678) <= 123457000000000);
    naTest(naGetDoubleFractionE(0.99999999999999999) == 0); // compiler rounding up
    naTest(naGetDoubleFractionE(0x0.fffffffffffffp0) == 1000000000000000);  // function rounding up
  }

  naTestGroup("naGetDoubleFractionSlow valid cases"){
    naTest(naGetDoubleFractionSlow(0.0) == 0);
    naTest(naGetDoubleFractionSlow(1.0) == 0);
    naTest(naGetDoubleFractionSlow(1111.0) == 0);
    naTest(naGetDoubleFractionSlow(0.5) == 500000000000000);
    naTest(naGetDoubleFractionSlow(0.00001234) == 12340000000);
    naTest(naGetDoubleFractionSlow(1234.5678) == 567800000000000);
    naTest(naGetDoubleFractionSlow(0.123456) == 123456000000000);
    naTest(naGetDoubleFractionSlow(123456789.12345) == 123450000000000);
    naTest(naGetDoubleFractionSlow(123456789.12345678) == 123457000000000);
    naTest(naGetDoubleFractionSlow(0.000000000000001) == 000000000000001);
    naTest(naGetDoubleFractionSlow(0.999999999999999) == 999999999999999);
    naTest(naGetDoubleFractionSlow(0.99999999999999999) == 0); // compiler rounds up to 1
  }

  naTestGroup("naGetDoubleFractionSlow invalid cases"){
    naTestError(naGetDoubleFractionSlow(0x0.fffffffffffffp0));  // rounding up
  }

  naTestGroup("naGetDoubleFractionSlowE valid cases"){
    naTest(naGetDoubleFractionSlowE(0x0.fffffffffffffp0) == 1000000000000000);  // rounding up
  }
}



void testNAFloatingPoint(void){
  naTestFunction(testMakeFloat);
  naTestFunction(testMakeDouble);
  naTestFunction(testGetFloat);
  naTestFunction(testGetDouble);
}



void printNAFloatingPoint(void){
  printf("NAFloatingPoint.h:" NA_NL);

  printf("Fundamental values:" NA_NL);
  naPrintMacro(NA_INFINITYf);
  naPrintMacro(NA_INFINITY);
  naPrintMacro(NA_INFINITYl);
  naPrintMacro(NA_NANf);
  naPrintMacro(NA_NAN);
  naPrintMacro(NA_NANl);
  naPrintMacro(NA_SINGULARITYf);
  naPrintMacro(NA_SUB_NORMf);
  naPrintMacro(NA_SUP_NORMf);
  naPrintMacro(NA_SINGULARITY);
  naPrintMacro(NA_SUB_NORM);
  naPrintMacro(NA_SUP_NORM);
  naPrintMacro(NA_SINGULARITYl);
  naPrintMacro(NA_SUB_NORMl);
  naPrintMacro(NA_SUP_NORMl);

  printf("Single precision:" NA_NL);
  naPrintMacroInt(NA_IEEE754_SINGLE_BITS);
  naPrintMacroInt(NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
  naPrintMacroux32(NA_IEEE754_SINGLE_SIGNIFICAND_NORM);
  naPrintMacroux32(NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_BITS);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_BIAS);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL);
  naPrintMacroInt(NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
  naPrintMacroux32(NA_IEEE754_SINGLE_EXPONENT_MASK);
  naPrintMacroux32(NA_IEEE754_SINGLE_SIGN_MASK);

  printf("Double precision:" NA_NL);
  naPrintMacroInt(NA_IEEE754_DOUBLE_BITS);
  naPrintMacroInt(NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  naPrintMacroux64(NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
  naPrintMacroux64(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_BITS);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_BIAS);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL);
  naPrintMacroInt(NA_IEEE754_DOUBLE_EXPONENT_SPECIAL);
  naPrintMacroux64(NA_IEEE754_DOUBLE_EXPONENT_MASK);
  naPrintMacroux64(NA_IEEE754_DOUBLE_SIGN_MASK);

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
