
#include "../../testHelper.h"
#include "NABase.h"
#include "NAString.h"
#include <stdio.h>


//typedef struct NANumericsTestValues NANumericsTestValues;
//struct NANumericsTestValues{
//  int8   v8;
//  int16  v16;
//  int32  v32;
//  int64  v64;
//  int128 v128;
//  int256 v256;
//};
//
//NANumericsTestValues naMakePositiveNumericsTestValues(){
//  NANumericsTestValues p;
//  p.v8 = 42;
//  p.v16 = (int16)p.v8;
//  p.v32 = (int32)p.v16;
//  p.v64 = naMakei64(p.v32, p.v32);
//  p.v128 = naMakei128(p.v64, naCasti64Tou64(p.v64));
//  p.v256 = naMakei256(p.v128, naCasti128Tou128(p.v128));
//  return p;
//}
//
//NANumericsTestValues naMakeNegativeNumericsTestValues(const NANumericsTestValues* positiveValues){
//  NANumericsTestValues n;
//  n.v8 = -positiveValues->v8;
//  n.v16 = -positiveValues->v16;
//  n.v32 = -positiveValues->v32;
//  n.v64 = naNegi64(positiveValues->v64);
//  n.v128 = naNegi128(positiveValues->v128);
//  n.v256 = naNegi256(positiveValues->v256);
//  return n;
//}
//
//NANumericsTestValues naMakeSignFlippedNumericsTestValues(const NANumericsTestValues* positiveValues){
//  NANumericsTestValues s;
//  s.v8 = positiveValues->v8 | NA_SIGN_MASK_8;
//  s.v16 = positiveValues->v16 | NA_SIGN_MASK_16;
//  s.v32 = positiveValues->v32 | NA_SIGN_MASK_32;
//  s.v64 = naOri64(positiveValues->v64, NA_SIGN_MASK_64);
//  s.v128 = naOri128(positiveValues->v128, NA_SIGN_MASK_128);
//  s.v256 = naOri256(positiveValues->v256, NA_SIGN_MASK_256);
//  return s;
//}


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
  }

  naG("naMakeFloat invalid cases") {
    //naTError(naMakeFloat(0x800000, 0));
    //naTError(naMakeFloat(0x0, 1000));
    //naTError(naMakeFloat(0x0, 128));
    //naTError(naMakeFloat(0x0, -127));
    //naTError(naMakeFloat(0x0, -1000));
  }
}



void printNAFloatingPoint(){
  printf("NAFloatingPoint.h:" NA_NL);

  printMacroInt(NA_IEEE754_SINGLE_BITS);
  printMacroInt(NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
  printMacroux32(NA_IEEE754_SINGLE_SIGNIFICAND_NORM, NA_FALSE);
  printMacroux32(NA_IEEE754_SINGLE_SIGNIFICAND_MASK, NA_FALSE);
  printMacroInt(NA_IEEE754_SINGLE_EXPONENT_BITS);
  printMacroInt(NA_IEEE754_SINGLE_EXPONENT_BIAS);
  printMacroInt(NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL);
  printMacroInt(NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
  printMacroux32(NA_IEEE754_SINGLE_EXPONENT_MASK, NA_FALSE);
  printMacroux32(NA_IEEE754_SINGLE_SIGN_MASK, NA_FALSE);

  printMacroInt(NA_IEEE754_DOUBLE_BITS);
  printMacroInt(NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  printMacroux64(NA_IEEE754_DOUBLE_SIGNIFICAND_NORM, NA_FALSE);
  printMacroux64(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK, NA_FALSE);
  printMacroInt(NA_IEEE754_DOUBLE_EXPONENT_BITS);
  printMacroInt(NA_IEEE754_DOUBLE_EXPONENT_BIAS);
  printMacroInt(NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL);
  printMacroInt(NA_IEEE754_DOUBLE_EXPONENT_SPECIAL);
  printMacroux64(NA_IEEE754_DOUBLE_EXPONENT_MASK, NA_FALSE);
  printMacroux64(NA_IEEE754_DOUBLE_SIGN_MASK, NA_FALSE);

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
