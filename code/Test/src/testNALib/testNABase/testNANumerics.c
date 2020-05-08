
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../testHelper.h"
#include NA_TEST_NALIB_PATH(../../, NABase.h)
#include <stdio.h>


typedef struct NANumericsTestValues NANumericsTestValues;
struct NANumericsTestValues{
  int8   pos8;
  int16  pos16;
  int32  pos32;
  int64  pos64;
  int128 pos128;
  int256 pos256;
  int8   neg8;
  int16  neg16;
  int32  neg32;
  int64  neg64;
  int128 neg128;
  int256 neg256;
};

NANumericsTestValues naMakeNumericsTestValues(){
  NANumericsTestValues t;
  t.pos8 = 42;
  t.pos16 = (int16)t.pos8;
  t.pos32 = (int32)t.pos16;
  t.pos64 = naMakeInt64(t.pos32, t.pos32);
  t.pos128 = naMakeInt128(t.pos64, naCastInt64ToUInt64(t.pos64));
  t.pos256 = naMakeInt256(t.pos128, naCastInt128ToUInt128(t.pos128));
  t.neg8 = -t.pos8;
  t.neg16 = -t.pos16;
  t.neg32 = -t.pos32;
  t.neg64 = naNegInt64(t.pos64);
  t.neg128 = naNegInt128(t.pos128);
  t.neg256 = naNegInt256(t.pos256);
  return t;
}

void testGetSignum(){
  NANumericsTestValues t = naMakeNumericsTestValues();

  naG("naGetSignum8") {
    naT(naGetSignum8(t.pos8) == NA_ZERO_u8);
    naT(naGetSignum8(NA_ZERO_i8) == NA_ZERO_u8);
    naT(naGetSignum8(t.neg8) == NA_MAX_u8);
    naT(naGetSignum8(NA_MAX_i8) == NA_ZERO_u8);
    naT(naGetSignum8(NA_MIN_i8) == NA_MAX_u8);
  }

  naG("naGetSignum16") {
    naT(naGetSignum16(t.pos16) == NA_ZERO_u16);
    naT(naGetSignum16(NA_ZERO_i16) == NA_ZERO_u16);
    naT(naGetSignum16(t.neg16) == NA_MAX_u16);
    naT(naGetSignum16(NA_MAX_i16) == NA_ZERO_u16);
    naT(naGetSignum16(NA_MIN_i16) == NA_MAX_u16);
  }

  naG("naGetSignum32") {
    naT(naGetSignum32(t.pos32) == NA_ZERO_u32);
    naT(naGetSignum32(NA_ZERO_i32) == NA_ZERO_u32);
    naT(naGetSignum32(t.neg32) == NA_MAX_u32);
    naT(naGetSignum32(NA_MAX_i32) == NA_ZERO_u32);
    naT(naGetSignum32(NA_MIN_i32) == NA_MAX_u32);
  }

  naG("naGetSignum64") {
    naT(naEqualUInt64(naGetSignum64(t.pos64), NA_ZERO_u64));
    naT(naEqualUInt64(naGetSignum64(NA_ZERO_i64), NA_ZERO_u64));
    naT(naEqualUInt64(naGetSignum64(t.neg64), NA_MAX_u64));
    naT(naEqualUInt64(naGetSignum64(NA_MAX_i64), NA_ZERO_u64));
    naT(naEqualUInt64(naGetSignum64(NA_MIN_i64), NA_MAX_u64));
  }

  naG("naGetSignum128") {
    naT(naEqualUInt128(naGetSignum128(t.pos128), NA_ZERO_u128));
    naT(naEqualUInt128(naGetSignum128(NA_ZERO_i128), NA_ZERO_u128));
    naT(naEqualUInt128(naGetSignum128(t.neg128), NA_MAX_u128));
    naT(naEqualUInt128(naGetSignum128(NA_MAX_i128), NA_ZERO_u128));
    naT(naEqualUInt128(naGetSignum128(NA_MIN_i128), NA_MAX_u128));
  }

  naG("naGetSignum256") {
    naT(naEqualUInt256(naGetSignum256(t.pos256), NA_ZERO_u256));
    naT(naEqualUInt256(naGetSignum256(NA_ZERO_i256), NA_ZERO_u256));
    naT(naEqualUInt256(naGetSignum256(t.neg256), NA_MAX_u256));
    naT(naEqualUInt256(naGetSignum256(NA_MAX_i256), NA_ZERO_u256));
    naT(naEqualUInt256(naGetSignum256(NA_MIN_i256), NA_MAX_u256));
  }
}



void testGetSigni(){
  NANumericsTestValues t = naMakeNumericsTestValues();

  naG("naGetSigni8") {
    naT(naSigni8(t.pos8) == NA_ONE_i8);
    naT(naSigni8(NA_ZERO_i8) == NA_ONE_i8);
    naT(naSigni8(t.neg8) == NA_MINUS_ONE_i8);
    naT(naSigni8(NA_MAX_i8) == NA_ONE_i8);
    naT(naSigni8(NA_MIN_i8) == NA_MINUS_ONE_i8);
  }

  naG("naGetSigni16") {
    naT(naSigni16(t.pos16) == NA_ONE_i16);
    naT(naSigni16(NA_ZERO_i16) == NA_ONE_i16);
    naT(naSigni16(t.neg16) == NA_MINUS_ONE_i16);
    naT(naSigni16(NA_MAX_i16) == NA_ONE_i16);
    naT(naSigni16(NA_MIN_i16) == NA_MINUS_ONE_i16);
  }

  naG("naGetSigni32") {
    naT(naSigni32(t.pos32) == NA_ONE_i32);
    naT(naSigni32(NA_ZERO_i32) == NA_ONE_i32);
    naT(naSigni32(t.neg32) == NA_MINUS_ONE_i32);
    naT(naSigni32(NA_MAX_i32) == NA_ONE_i32);
    naT(naSigni32(NA_MIN_i32) == NA_MINUS_ONE_i32);
  }

  naG("naGetSigni64") {
    naT(naEqualInt64(naSigni64(t.pos64), NA_ONE_i64));
    naT(naEqualInt64(naSigni64(NA_ZERO_i64), NA_ONE_i64));
    naT(naEqualInt64(naSigni64(t.neg64), NA_MINUS_ONE_i64));
    naT(naEqualInt64(naSigni64(NA_MAX_i64), NA_ONE_i64));
    naT(naEqualInt64(naSigni64(NA_MIN_i64), NA_MINUS_ONE_i64));
  }

  naG("naGetSigni128") {
    naT(naEqualInt128(naSigni128(t.pos128), NA_ONE_i128));
    naT(naEqualInt128(naSigni128(NA_ZERO_i128), NA_ONE_i128));
    naT(naEqualInt128(naSigni128(t.neg128), NA_MINUS_ONE_i128));
    naT(naEqualInt128(naSigni128(NA_MAX_i128), NA_ONE_i128));
    naT(naEqualInt128(naSigni128(NA_MIN_i128), NA_MINUS_ONE_i128));
  }

  naG("naGetSigni256") {
    naT(naEqualInt256(naSigni256(t.pos256), NA_ONE_i256));
    naT(naEqualInt256(naSigni256(NA_ZERO_i256), NA_ONE_i256));
    naT(naEqualInt256(naSigni256(t.neg256), NA_MINUS_ONE_i256));
    naT(naEqualInt256(naSigni256(NA_MAX_i256), NA_ONE_i256));
    naT(naEqualInt256(naSigni256(NA_MIN_i256), NA_MINUS_ONE_i256));
  }
}



void testGetAbsi(){
  NANumericsTestValues t = naMakeNumericsTestValues();

  naG("naAbsi8") {
    naT(naAbsi8(t.pos8) == t.pos8);
    naT(naAbsi8(NA_ZERO_i8) == NA_ZERO_i8);
    naT(naAbsi8(t.neg8) == t.pos8);
    naT(naAbsi8(NA_MAX_i8) == NA_MAX_i8);
    naT(naAbsi8(NA_MIN_i8) == NA_MIN_i8);
  }

  naG("naAbsi16") {
    naT(naAbsi16(t.pos16) == t.pos16);
    naT(naAbsi16(NA_ZERO_i16) == NA_ZERO_i16);
    naT(naAbsi16(t.neg16) == t.pos16);
    naT(naAbsi16(NA_MAX_i16) == NA_MAX_i16);
    naT(naAbsi16(NA_MIN_i16) == NA_MIN_i16);
  }

  naG("naAbsi32") {
    naT(naAbsi32(t.pos32) == t.pos32);
    naT(naAbsi32(NA_ZERO_i32) == NA_ZERO_i32);
    naT(naAbsi32(t.neg32) == t.pos32);
    naT(naAbsi32(NA_MAX_i32) == NA_MAX_i32);
    naT(naAbsi32(NA_MIN_i32) == NA_MIN_i32);
  }

  naG("naAbsi64") {
    naT(naEqualInt64(naAbsi64(t.pos64), t.pos64));
    naT(naEqualInt64(naAbsi64(NA_ZERO_i64), NA_ZERO_i64));
    naT(naEqualInt64(naAbsi64(t.neg64), t.pos64));
    naT(naEqualInt64(naAbsi64(NA_MAX_i64), NA_MAX_i64));
    naT(naEqualInt64(naAbsi64(NA_MIN_i64), NA_MIN_i64));
  }

  naG("naAbsi128") {
    naT(naEqualInt128(naAbsi128(t.pos128), t.pos128));
    naT(naEqualInt128(naAbsi128(NA_ZERO_i128), NA_ZERO_i128));
    naT(naEqualInt128(naAbsi128(t.neg128), t.pos128));
    naT(naEqualInt128(naAbsi128(NA_MAX_i128), NA_MAX_i128));
    naT(naEqualInt128(naAbsi128(NA_MIN_i128), NA_MIN_i128));
  }

  naG("naAbsi256") {
    naT(naEqualInt256(naAbsi256(t.pos256), t.pos256));
    naT(naEqualInt256(naAbsi256(NA_ZERO_i256), NA_ZERO_i256));
    naT(naEqualInt256(naAbsi256(t.neg256), t.pos256));
    naT(naEqualInt256(naAbsi256(NA_MAX_i256), NA_MAX_i256));
    naT(naEqualInt256(naAbsi256(NA_MIN_i256), NA_MIN_i256));
  }
}


void testNANumerics(){
  naF(GetSignum);
  naF(GetSigni);
  naF(GetAbsi);
}



void printNANumerics(){
  printf("NANumerics.h:" NA_NL);

  printMacroPlain(NA_SIGN_MASK_8);
  printMacroPlain(NA_SIGN_MASK_16);
  printMacroPlain(NA_SIGN_MASK_32);
  printMacroPlain(NA_SIGN_MASK_64);
  printMacroPlain(NA_SIGN_MASK_128);
  printMacroPlain(NA_SIGN_MASK_256);

  printMacroPlain(NA_ZERO_u8);
  printMacroPlain(NA_ONE_u8);
  printMacroPlain(NA_ZERO_i8);
  printMacroPlain(NA_ONE_i8);
  printMacroPlain(NA_MINUS_ONE_i8);

  printMacroPlain(NA_ZERO_u16);
  printMacroPlain(NA_ONE_u16);
  printMacroPlain(NA_ZERO_i16);
  printMacroPlain(NA_ONE_i16);
  printMacroPlain(NA_MINUS_ONE_i16);

  printMacroPlain(NA_ZERO_u32);
  printMacroPlain(NA_ONE_u32);
  printMacroPlain(NA_ZERO_i32);
  printMacroPlain(NA_ONE_i32);
  printMacroPlain(NA_MINUS_ONE_i32);

  printMacroPlain(NA_ZERO_u64);
  printMacroPlain(NA_ONE_u64);
  printMacroPlain(NA_ZERO_i64);
  printMacroPlain(NA_ONE_i64);
  printMacroPlain(NA_MINUS_ONE_i64);

  printMacroPlain(NA_ZERO_u128);
  printMacroPlain(NA_ONE_u128);
  printMacroPlain(NA_ZERO_i128);
  printMacroPlain(NA_ONE_i128);
  printMacroPlain(NA_MINUS_ONE_i128);

  printMacroPlain(NA_ZERO_u256);
  printMacroPlain(NA_ONE_u256);
  printMacroPlain(NA_ZERO_i256);
  printMacroPlain(NA_ONE_i256);
  printMacroPlain(NA_MINUS_ONE_i256);

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

