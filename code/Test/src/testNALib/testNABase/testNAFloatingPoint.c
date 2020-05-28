
#include "NABase.h"
#include "NAString.h"
#include <stdio.h>



void testMakeFloat(){
  // Never seen this kind of literal yet? Go look for hex float literals!
  // It is actually quite useful for testing. One only has to remember that
  // single precision only use 23 bits for the significant, which is why the
  // hex digits after the point of the compared value appear to be doubled.
  naG("naMakeFloat valid cases"){
    int32 number = 0x712345;
    int32 maxNumber = 0x7fffff;
    int32 negNumber = -number;
    int32 negMaxNumber = -maxNumber;
    naT(naMakeFloat(0, 0) == 0x1.000000p0f);
    naT(naMakeFloat(1, 0) == 0x1.000002p0f);
    naT(naMakeFloat(1, 42) == 0x1.000002p42f);
    naT(naMakeFloat(-1, 42) == -0x1.000002p42f);
    naT(naMakeFloat(1, -42) == 0x1.000002p-42f);
    naT(naMakeFloat(-1, -42) == -0x1.000002p-42f);
    naT(naMakeFloat(number, 42) == 0x1.e2468ap42f);
    naT(naMakeFloat(negNumber, 42) == -0x1.e2468ap42f);
    naT(naMakeFloat(maxNumber, 42) == 0x1.fffffep42f);
    naT(naMakeFloat(negMaxNumber, 42) == -0x1.fffffep42f);
    naT(naMakeFloat(maxNumber, 127) == 0x1.fffffep127f);
    naT(naMakeFloat(0, -126) == 0x1.000000p-126f);
    naT(naMakeFloat(0, -127) == 0.f);
  }

  naG("naMakeFloat invalid cases"){
    naE(naMakeFloat(0x800000, 0));
    naE(naMakeFloat(0x1, 1000));
    naE(naMakeFloat(0x1, 128));
    naE(naMakeFloat(0x1, -127));
    naE(naMakeFloat(0x1, -1000));
  }

  naG("naMakeFloatWithExponent valid cases"){
    naT(naMakeFloatWithExponent(0) == 1.f);
    naT(naMakeFloatWithExponent(1) == 2.f);
    naT(naMakeFloatWithExponent(8) == 256.f);
    naT(naMakeFloatWithExponent(42) == 0x1.000000p42f);
    naT(naMakeFloatWithExponent(127) == 0x1.000000p127f);
    naT(naMakeFloatWithExponent(-1) == 0.5f);
    naT(naMakeFloatWithExponent(-42) == 0x1.000000p-42f);
    naT(naMakeFloatWithExponent(-126) == 0x1.000000p-126f);
  }

  naG("naMakeFloatWithExponent invalid cases"){
    naE(naMakeFloatWithExponent(128));
    naE(naMakeFloatWithExponent(1000));
    naE(naMakeFloatWithExponent(-127));
    naE(naMakeFloatWithExponent(-1000));
  }

  naG("naMakeFloatSubnormal valid cases"){
    int32 number = 0x712345;
    int32 maxNumber = 0x7fffff;
    int32 negNumber = -number;
    int32 negMaxNumber = -maxNumber;
    naT(naMakeFloatSubnormal(1) > 0.f);
    naT(naMakeFloatSubnormal(number) > 0.f);
    naT(naMakeFloatSubnormal(maxNumber) > 0.f);
    naT(naMakeFloatSubnormal(negNumber) < 0.f);
    naT(naMakeFloatSubnormal(negMaxNumber) < 0.f);
    naT(naMakeFloatSubnormal(1) < 0x1.000000p-126f);
    naT(naMakeFloatSubnormal(number) < 0x1.000000p-126f);
    naT(naMakeFloatSubnormal(maxNumber) < 0x1.000000p-126f);
    naT(naMakeFloatSubnormal(negNumber) > -0x1.000000p-126f);
    naT(naMakeFloatSubnormal(negMaxNumber) > -0x1.000000p-126f);
  }

  naG("naMakeFloatSubnormal invalid cases"){
    naE(naMakeFloatSubnormal(0x800000));
    naE(naMakeFloatSubnormal(-0x800000));
  }
}



void testMakeDouble(){
  // Never seen this kind of literal yet? Go look for hex float literals!
  // It is actually quite useful for testing..
  naG("naMakeDouble valid cases"){
    int64 number = 0xf123456789abc;
    int64 maxNumber = 0xfffffffffffff;
    int64 negNumber = -number;
    int64 negMaxNumber = -maxNumber;
    naT(naMakeDouble(0, 0) == 0x1.0000000000000p0);
    naT(naMakeDouble(1, 0) == 0x1.0000000000001p0);
    naT(naMakeDouble(1, 42) == 0x1.0000000000001p42);
    naT(naMakeDouble(-1, 42) == -0x1.0000000000001p42);
    naT(naMakeDouble(1, -42) == 0x1.0000000000001p-42);
    naT(naMakeDouble(-1, -42) == -0x1.0000000000001p-42);
    naT(naMakeDouble(number, 42) == 0x1.f123456789abcp42);
    naT(naMakeDouble(negNumber, 42) == -0x1.f123456789abcp42);
    naT(naMakeDouble(maxNumber, 42) == 0x1.fffffffffffffp42);
    naT(naMakeDouble(negMaxNumber, 42) == -0x1.fffffffffffffp42);
    naT(naMakeDouble(maxNumber, 1023) == 0x1.fffffffffffffp1023);
    naT(naMakeDouble(0, -1022) == 0x1.0000000000000p-1022);
    naT(naMakeDouble(0, -1023) == 0.f);
  }

  naG("naMakeDouble invalid cases"){
    naE(naMakeDouble(0x10000000000000, 0));
    naE(naMakeDouble(0x1, 10000));
    naE(naMakeDouble(0x1, 1024));
    naE(naMakeDouble(0x1, -1023));
    naE(naMakeDouble(0x1, -10000));
  }

  naG("naMakeDoubleWithExponent valid cases"){
    naT(naMakeDoubleWithExponent(0) == 1.f);
    naT(naMakeDoubleWithExponent(1) == 2.f);
    naT(naMakeDoubleWithExponent(8) == 256.f);
    naT(naMakeDoubleWithExponent(42) == 0x1.000000p42);
    naT(naMakeDoubleWithExponent(1023) == 0x1.000000p1023);
    naT(naMakeDoubleWithExponent(-1) == 0.5f);
    naT(naMakeDoubleWithExponent(-42) == 0x1.000000p-42);
    naT(naMakeDoubleWithExponent(-1022) == 0x1.000000p-1022);
  }

  naG("naMakeDoubleWithExponent invalid cases"){
    naE(naMakeDoubleWithExponent(1024));
    naE(naMakeDoubleWithExponent(10000));
    naE(naMakeDoubleWithExponent(-1023));
    naE(naMakeDoubleWithExponent(-10000));
  }

  naG("naMakeDoubleSubnormal valid cases"){
    int64 number = 0xf123456789abc;
    int64 maxNumber = 0xfffffffffffff;
    int64 negNumber = -number;
    int64 negMaxNumber = -maxNumber;
    naT(naMakeDoubleSubnormal(1) > 0.);
    naT(naMakeDoubleSubnormal(number) > 0.);
    naT(naMakeDoubleSubnormal(maxNumber) > 0.);
    naT(naMakeDoubleSubnormal(negNumber) < 0.);
    naT(naMakeDoubleSubnormal(negMaxNumber) < 0.);
    naT(naMakeDoubleSubnormal(1) < 0x1.000000p-1022);
    naT(naMakeDoubleSubnormal(number) < 0x1.000000p-1022);
    naT(naMakeDoubleSubnormal(maxNumber) < 0x1.000000p-1022);
    naT(naMakeDoubleSubnormal(negNumber) > -0x1.000000p-1022);
    naT(naMakeDoubleSubnormal(negMaxNumber) > -0x1.000000p-1022);
  }

  naG("naMakeDoubleSubnormal invalid cases"){
    naE(naMakeDoubleSubnormal(0x10000000000000));
    naE(naMakeDoubleSubnormal(-0x10000000000000));
  }
}



void testGetFloat(){
  naG("naGetFloatExponent valid cases"){
    naT(naGetFloatExponent(1.f) == 0);
    naT(naGetFloatExponent(0x1.0p42f) == 42);
    naT(naGetFloatExponent(0x1.0p-42f) == -42);
    naT(naGetFloatExponent(0x1.0p127f) == 127);
    naT(naGetFloatExponent(0x1.0p-126f) == -126);
  }

  naG("naGetFloatExponent invalid cases"){
    naE(naGetFloatExponent(0x1.0p-127)); // subnormal
    naE(naGetFloatExponent(0x1.0p-149)); // subnormal -126-23
    naE(naGetFloatExponent(0.f));
    naE(naGetFloatExponent(NA_INFINITYf));
  }

  naG("naGetFloatInteger valid cases"){
    naT(naGetFloatInteger(0.f) == 0);
    naT(naGetFloatInteger(0.001234f) == 0);
    naT(naGetFloatInteger(0x0.fffffep0f) == 0);
    naT(naGetFloatInteger(0.999999999f) == 1);
    naT(naGetFloatInteger(42.f) == 42);
    naT(naGetFloatInteger(-42.f) == -42);
    naT(naGetFloatInteger(0x1.fffffep23f) == 0xffffff);
    naT(naGetFloatInteger(-0x1.fffffep23f) == -0xffffff);
  }

  naG("naGetFloatInteger invalid cases"){
    naE(naGetFloatInteger(NA_INFINITYf));
    naE(naGetFloatInteger(0x1.fffffep24f));
  }

  naG("naGetFloatFraction valid cases"){
    naT(naGetFloatFraction(0.0f) == 0);
    naT(naGetFloatFraction(0.5f) == 500000);
    naT(naGetFloatFraction(0.001234f) == 1234);
    naT(naGetFloatFraction(0.123456f) == 123456);
    naT(naGetFloatFraction(0.000001f) == 000001);
    naT(naGetFloatFraction(0.999999f) == 999999);
  }

  naG("naGetFloatFraction invalid cases"){
    naE(naGetFloatFraction(1.0f));
    naE(naGetFloatFraction(1111.0f));
    naE(naGetFloatFraction(1.56f));
    naE(naGetFloatFraction(123.56f));
    naE(naGetFloatFraction(1234.12f));
    naE(naGetFloatFraction(0.999999999f)); // compiler rounding up
    naE(naGetFloatFraction(0x0.fffffep0f));  // function rounding up
  }

  naG("naGetFloatFractionE"){
    naT(naGetFloatFractionE(1.0f) == 0);
    naT(naGetFloatFractionE(1111.0f) == 0);
    naT(naGetFloatFractionE(1.56f) >= 550000);
    naT(naGetFloatFractionE(1.56f) <= 570000);
    naT(naGetFloatFractionE(123.56f) >= 550000);
    naT(naGetFloatFractionE(123.56f) <= 570000);
    naT(naGetFloatFractionE(1234.12f) >= 110000);
    naT(naGetFloatFractionE(1234.12f) <= 130000);
    naT(naGetFloatFractionE(0.999999999f) == 0); // compiler rounding up
    naT(naGetFloatFractionE(0x0.fffffep0f) == 1000000);  // function rounding up
  }

  naG("naGetFloatFractionSlow valid cases"){
    naT(naGetFloatFractionSlow(0.0f) == 0);
    naT(naGetFloatFractionSlow(1.0f) == 0);
    naT(naGetFloatFractionSlow(1111.0f) == 0);
    naT(naGetFloatFractionSlow(0.5f) == 500000);
    naT(naGetFloatFractionSlow(0.001234f) == 1234);
    naT(naGetFloatFractionSlow(0.123456f) == 123456);
    naT(naGetFloatFractionSlow(1.56f) == 560000);
    naT(naGetFloatFractionSlow(123.56f) == 560000);
    naT(naGetFloatFractionSlow(1234.12f) == 120000);
    naT(naGetFloatFractionSlow(0.000001f) == 000001);
    naT(naGetFloatFractionSlow(0.999999f) == 999999);
    naT(naGetFloatFractionSlow(0.999999999f) == 0); // compiler rounds up to 1
  }

  naG("naGetFloatFractionSlow invalid cases"){
    naE(naGetFloatFractionSlow(0x0.fffffep0f));  // rounding up
  }

  naG("naGetFloatFractionSlowE valid cases"){
    naT(naGetFloatFractionSlowE(0x0.fffffep0f) == 1000000);  // rounding up
  }
}



void testGetDouble(){
  naG("naGetDoubleExponent valid cases"){
    naT(naGetDoubleExponent(1.) == 0);
    naT(naGetDoubleExponent(0x1.0p42) == 42);
    naT(naGetDoubleExponent(0x1.0p-42) == -42);
    naT(naGetDoubleExponent(0x1.0p1023) == 1023);
    naT(naGetDoubleExponent(0x1.0p-1022) == -1022);
  }

  naG("naGetDoubleExponent invalid cases"){
    naE(naGetDoubleExponent(0x1.0p-1023)); // subnormal
    naE(naGetDoubleExponent(0x1.0p-1074)); // subnormal -1022-52
    naE(naGetDoubleExponent(0.));
    naE(naGetDoubleExponent(NA_INFINITY));
  }
 
  naG("naGetDoubleInteger valid cases"){
    naT(naGetDoubleInteger(0.) == 0);
    naT(naGetDoubleInteger(0.00001234) == 0);
    naT(naGetDoubleInteger(0x0.fffffffffffffp0) == 0);
    naT(naGetDoubleInteger(0.99999999999999999) == 1);
    naT(naGetDoubleInteger(42.) == 42);
    naT(naGetDoubleInteger(-42.) == -42);
    naT(naGetDoubleInteger(0x1.fffffffffffffp52) == 0x1fffffffffffff);
    naT(naGetDoubleInteger(-0x1.fffffffffffffp52) == -0x1fffffffffffff);
  }

  naG("naGetDoubleInteger invalid cases"){
    naE(naGetDoubleInteger(NA_INFINITY));
    naE(naGetDoubleInteger(0x1.fffffffffffffp53));
  }

  naG("naGetDoubleFraction valid cases"){
    naT(naGetDoubleFraction(0.0) == 0);
    naT(naGetDoubleFraction(0.5) == 500000000000000);
    naT(naGetDoubleFraction(0.00001234) == 12340000000);
    naT(naGetDoubleFraction(0.123456) == 123456000000000);
    naT(naGetDoubleFraction(0.000000000000001) == 000000000000001);
    naT(naGetDoubleFraction(0.999999999999999) == 999999999999999);
  }

  naG("naGetDoubleFraction invalid cases"){
    naE(naGetDoubleFraction(1.0));
    naE(naGetDoubleFraction(1111.0));
    naE(naGetDoubleFraction(1234.5678));
    naE(naGetDoubleFraction(123456789.12345));
    naE(naGetDoubleFraction(123456789.12345678));
    naE(naGetDoubleFraction(0.99999999999999999)); // compiler rounding up
    naE(naGetDoubleFraction(0x0.fffffffffffffp0));  // function rounding up
  }

  naG("naGetDoubleFractionE"){
    naT(naGetDoubleFractionE(1.0) == 0);
    naT(naGetDoubleFractionE(1111.0) == 0);
    naT(naGetDoubleFractionE(1234.5678) >= 567700000000000);
    naT(naGetDoubleFractionE(1234.5678) <= 567900000000000);
    naT(naGetDoubleFractionE(123456789.12345) >= 123440000000000);
    naT(naGetDoubleFractionE(123456789.12345) <= 123460000000000);
    naT(naGetDoubleFractionE(123456789.12345678) >= 123455000000000);
    naT(naGetDoubleFractionE(123456789.12345678) <= 123457000000000);
    naT(naGetDoubleFractionE(0.99999999999999999) == 0); // compiler rounding up
    naT(naGetDoubleFractionE(0x0.fffffffffffffp0) == 1000000000000000);  // function rounding up
  }

  naG("naGetDoubleFractionSlow valid cases"){
    naT(naGetDoubleFractionSlow(0.0) == 0);
    naT(naGetDoubleFractionSlow(1.0) == 0);
    naT(naGetDoubleFractionSlow(1111.0) == 0);
    naT(naGetDoubleFractionSlow(0.5) == 500000000000000);
    naT(naGetDoubleFractionSlow(0.00001234) == 12340000000);
    naT(naGetDoubleFractionSlow(1234.5678) == 567800000000000);
    naT(naGetDoubleFractionSlow(0.123456) == 123456000000000);
    naT(naGetDoubleFractionSlow(123456789.12345) == 123450000000000);
    naT(naGetDoubleFractionSlow(123456789.12345678) == 123457000000000);
    naT(naGetDoubleFractionSlow(0.000000000000001) == 000000000000001);
    naT(naGetDoubleFractionSlow(0.999999999999999) == 999999999999999);
    naT(naGetDoubleFractionSlow(0.99999999999999999) == 0); // compiler rounds up to 1
  }

  naG("naGetDoubleFractionSlow invalid cases"){
    naE(naGetDoubleFractionSlow(0x0.fffffffffffffp0));  // rounding up
  }

  naG("naGetDoubleFractionSlowE valid cases"){
    naT(naGetDoubleFractionSlowE(0x0.fffffffffffffp0) == 1000000000000000);  // rounding up
  }
}



void testNAFloatingPoint(){
  naF(MakeFloat);
  naF(MakeDouble);
  naF(GetFloat);
  naF(GetDouble);
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
