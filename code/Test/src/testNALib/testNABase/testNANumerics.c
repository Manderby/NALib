
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../testHelper.h"
#include NA_TEST_NALIB_PATH(../../, NABase.h)
#include <stdio.h>



void testNANumerics(){
  int8   pos8   = 42;
  int16  pos16  = (int16)pos8;
  int32  pos32  = (int32)pos16;
  int64  pos64  = naMakeInt64WithLo(pos32);
  int128 pos128 = naMakeInt128WithLo(pos64);
  int256 pos256 = naMakeInt256WithLo(pos128);
  int8   neg8   = -pos8;
  int16  neg16  = -pos16;
  int32  neg32  = -pos32;
  int64  neg64  = naNegInt64(pos64);
  int128 neg128 = naNegInt128(pos128);
  int256 neg256 = naNegInt256(pos256);

  printf("Testing naGetSignum8..." NA_NL);
  testSuccess(naGetSignum8(pos8) == NA_ZERO_u8);
  testSuccess(naGetSignum8(NA_ZERO_i8) == NA_ZERO_u8);
  testSuccess(naGetSignum8(neg8) == NA_MAX_u8);
  testSuccess(naGetSignum8(NA_MAX_i8) == NA_ZERO_u8);
  testSuccess(naGetSignum8(NA_MIN_i8) == NA_MAX_u8);

  printf("Testing naGetSignum16..." NA_NL);
  testSuccess(naGetSignum16(pos16) == NA_ZERO_u16);
  testSuccess(naGetSignum16(NA_ZERO_i16) == NA_ZERO_u16);
  testSuccess(naGetSignum16(neg16) == NA_MAX_u16);
  testSuccess(naGetSignum16(NA_MAX_i16) == NA_ZERO_u16);
  testSuccess(naGetSignum16(NA_MIN_i16) == NA_MAX_u16);

  printf("Testing naGetSignum32..." NA_NL);
  testSuccess(naGetSignum32(pos32) == NA_ZERO_u32);
  testSuccess(naGetSignum32(NA_ZERO_i32) == NA_ZERO_u32);
  testSuccess(naGetSignum32(neg32) == NA_MAX_u32);
  testSuccess(naGetSignum32(NA_MAX_i32) == NA_ZERO_u32);
  testSuccess(naGetSignum32(NA_MIN_i32) == NA_MAX_u32);

  printf("Testing naGetSignum64..." NA_NL);
  testSuccess(naEqualUInt64(naGetSignum64(pos64), NA_ZERO_u64));
  testSuccess(naEqualUInt64(naGetSignum64(NA_ZERO_i64), NA_ZERO_u64));
  testSuccess(naEqualUInt64(naGetSignum64(neg64), NA_MAX_u64));
  testSuccess(naEqualUInt64(naGetSignum64(NA_MAX_i64), NA_ZERO_u64));
  testSuccess(naEqualUInt64(naGetSignum64(NA_MIN_i64), NA_MAX_u64));

  printf("Testing naGetSignum128..." NA_NL);
  testSuccess(naEqualUInt128(naGetSignum128(pos128), NA_ZERO_u128));
  testSuccess(naEqualUInt128(naGetSignum128(NA_ZERO_i128), NA_ZERO_u128));
  testSuccess(naEqualUInt128(naGetSignum128(neg128), NA_MAX_u128));
  testSuccess(naEqualUInt128(naGetSignum128(NA_MAX_i128), NA_ZERO_u128));
  testSuccess(naEqualUInt128(naGetSignum128(NA_MIN_i128), NA_MAX_u128));

  printf("Testing naGetSignum256..." NA_NL);
  testSuccess(naEqualUInt256(naGetSignum256(pos256), NA_ZERO_u256));
  testSuccess(naEqualUInt256(naGetSignum256(NA_ZERO_i256), NA_ZERO_u256));
  testSuccess(naEqualUInt256(naGetSignum256(neg256), NA_MAX_u256));
  testSuccess(naEqualUInt256(naGetSignum256(NA_MAX_i256), NA_ZERO_u256));
  testSuccess(naEqualUInt256(naGetSignum256(NA_MIN_i256), NA_MAX_u256));
}



void printNANumerics(){
  printf("NANumerics.h:" NA_NL);

  printMacroIntHex(NA_VALUE8_SIGN_MASK, NA_FALSE);
  printMacroIntHex(NA_VALUE16_SIGN_MASK, NA_FALSE);
  printMacroIntHex(NA_VALUE32_SIGN_MASK, NA_FALSE);
  printMacroPlain(NA_VALUE64_SIGN_MASK);
  printMacroPlain(NA_VALUE128_SIGN_MASK);
  printMacroPlain(NA_VALUE256_SIGN_MASK);

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

