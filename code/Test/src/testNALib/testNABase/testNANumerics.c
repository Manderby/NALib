
#include "NABase.h"
#include "NAString.h"
#include <stdio.h>


typedef struct NANumericsTestValues NANumericsTestValues;
struct NANumericsTestValues{
  int8   v8;
  int16  v16;
  int32  v32;
  int64  v64;
  int128 v128;
  int256 v256;
};

NANumericsTestValues naMakePositiveNumericsTestValues(){
  NANumericsTestValues p;
  p.v8 = 42;
  p.v16 = (int16)p.v8;
  p.v32 = (int32)p.v16;
  p.v64 = naMakei64(p.v32, p.v32);
  p.v128 = naMakei128(p.v64, naCasti64Tou64(p.v64));
  p.v256 = naMakei256(p.v128, naCasti128Tou128(p.v128));
  return p;
}

NANumericsTestValues naMakeNegativeNumericsTestValues(const NANumericsTestValues* positiveValues){
  NANumericsTestValues n;
  n.v8 = -positiveValues->v8;
  n.v16 = -positiveValues->v16;
  n.v32 = -positiveValues->v32;
  n.v64 = naNegi64(positiveValues->v64);
  n.v128 = naNegi128(positiveValues->v128);
  n.v256 = naNegi256(positiveValues->v256);
  return n;
}

NANumericsTestValues naMakeSignFlippedNumericsTestValues(const NANumericsTestValues* positiveValues){
  NANumericsTestValues s;
  s.v8 = positiveValues->v8 | NA_SIGN_MASK_8;
  s.v16 = positiveValues->v16 | NA_SIGN_MASK_16;
  s.v32 = positiveValues->v32 | NA_SIGN_MASK_32;
  s.v64 = naOri64(positiveValues->v64, NA_SIGN_MASK_64);
  s.v128 = naOri128(positiveValues->v128, NA_SIGN_MASK_128);
  s.v256 = naOri256(positiveValues->v256, NA_SIGN_MASK_256);
  return s;
}

void testGetSignum(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues n = naMakeNegativeNumericsTestValues(&p);

  naTestGroup("naGetSignum8") {
    naTest(naGetSignum8(p.v8) == NA_ZERO_u8);
    naTest(naGetSignum8(NA_ZERO_i8) == NA_ZERO_u8);
    naTest(naGetSignum8(n.v8) == NA_MAX_u8);
    naTest(naGetSignum8(NA_MAX_i8) == NA_ZERO_u8);
    naTest(naGetSignum8(NA_MIN_i8) == NA_MAX_u8);
  }

  naTestGroup("naGetSignum16") {
    naTest(naGetSignum16(p.v16) == NA_ZERO_u16);
    naTest(naGetSignum16(NA_ZERO_i16) == NA_ZERO_u16);
    naTest(naGetSignum16(n.v16) == NA_MAX_u16);
    naTest(naGetSignum16(NA_MAX_i16) == NA_ZERO_u16);
    naTest(naGetSignum16(NA_MIN_i16) == NA_MAX_u16);
  }

  naTestGroup("naGetSignum32") {
    naTest(naGetSignum32(p.v32) == NA_ZERO_u32);
    naTest(naGetSignum32(NA_ZERO_i32) == NA_ZERO_u32);
    naTest(naGetSignum32(n.v32) == NA_MAX_u32);
    naTest(naGetSignum32(NA_MAX_i32) == NA_ZERO_u32);
    naTest(naGetSignum32(NA_MIN_i32) == NA_MAX_u32);
  }

  naTestGroup("naGetSignum64") {
    naTest(naEqualu64(naGetSignum64(p.v64), NA_ZERO_u64));
    naTest(naEqualu64(naGetSignum64(NA_ZERO_i64), NA_ZERO_u64));
    naTest(naEqualu64(naGetSignum64(n.v64), NA_MAX_u64));
    naTest(naEqualu64(naGetSignum64(NA_MAX_i64), NA_ZERO_u64));
    naTest(naEqualu64(naGetSignum64(NA_MIN_i64), NA_MAX_u64));
  }

  naTestGroup("naGetSignum128") {
    naTest(naEqualu128(naGetSignum128(p.v128), NA_ZERO_u128));
    naTest(naEqualu128(naGetSignum128(NA_ZERO_i128), NA_ZERO_u128));
    naTest(naEqualu128(naGetSignum128(n.v128), NA_MAX_u128));
    naTest(naEqualu128(naGetSignum128(NA_MAX_i128), NA_ZERO_u128));
    naTest(naEqualu128(naGetSignum128(NA_MIN_i128), NA_MAX_u128));
  }

  naTestGroup("naGetSignum256") {
    naTest(naEqualu256(naGetSignum256(p.v256), NA_ZERO_u256));
    naTest(naEqualu256(naGetSignum256(NA_ZERO_i256), NA_ZERO_u256));
    naTest(naEqualu256(naGetSignum256(n.v256), NA_MAX_u256));
    naTest(naEqualu256(naGetSignum256(NA_MAX_i256), NA_ZERO_u256));
    naTest(naEqualu256(naGetSignum256(NA_MIN_i256), NA_MAX_u256));
  }
}



void testGetSigni(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues n = naMakeNegativeNumericsTestValues(&p);

  naTestGroup("naGetSigni8") {
    naTest(naSigni8(p.v8) == NA_ONE_i8);
    naTest(naSigni8(NA_ZERO_i8) == NA_ONE_i8);
    naTest(naSigni8(n.v8) == NA_MINUS_ONE_i8);
    naTest(naSigni8(NA_MAX_i8) == NA_ONE_i8);
    naTest(naSigni8(NA_MIN_i8) == NA_MINUS_ONE_i8);
  }

  naTestGroup("naGetSigni16") {
    naTest(naSigni16(p.v16) == NA_ONE_i16);
    naTest(naSigni16(NA_ZERO_i16) == NA_ONE_i16);
    naTest(naSigni16(n.v16) == NA_MINUS_ONE_i16);
    naTest(naSigni16(NA_MAX_i16) == NA_ONE_i16);
    naTest(naSigni16(NA_MIN_i16) == NA_MINUS_ONE_i16);
  }

  naTestGroup("naGetSigni32") {
    naTest(naSigni32(p.v32) == NA_ONE_i32);
    naTest(naSigni32(NA_ZERO_i32) == NA_ONE_i32);
    naTest(naSigni32(n.v32) == NA_MINUS_ONE_i32);
    naTest(naSigni32(NA_MAX_i32) == NA_ONE_i32);
    naTest(naSigni32(NA_MIN_i32) == NA_MINUS_ONE_i32);
  }

  naTestGroup("naGetSigni64") {
    naTest(naEquali64(naSigni64(p.v64), NA_ONE_i64));
    naTest(naEquali64(naSigni64(NA_ZERO_i64), NA_ONE_i64));
    naTest(naEquali64(naSigni64(n.v64), NA_MINUS_ONE_i64));
    naTest(naEquali64(naSigni64(NA_MAX_i64), NA_ONE_i64));
    naTest(naEquali64(naSigni64(NA_MIN_i64), NA_MINUS_ONE_i64));
  }

  naTestGroup("naGetSigni128") {
    naTest(naEquali128(naSigni128(p.v128), NA_ONE_i128));
    naTest(naEquali128(naSigni128(NA_ZERO_i128), NA_ONE_i128));
    naTest(naEquali128(naSigni128(n.v128), NA_MINUS_ONE_i128));
    naTest(naEquali128(naSigni128(NA_MAX_i128), NA_ONE_i128));
    naTest(naEquali128(naSigni128(NA_MIN_i128), NA_MINUS_ONE_i128));
  }

  naTestGroup("naGetSigni256") {
    naTest(naEquali256(naSigni256(p.v256), NA_ONE_i256));
    naTest(naEquali256(naSigni256(NA_ZERO_i256), NA_ONE_i256));
    naTest(naEquali256(naSigni256(n.v256), NA_MINUS_ONE_i256));
    naTest(naEquali256(naSigni256(NA_MAX_i256), NA_ONE_i256));
    naTest(naEquali256(naSigni256(NA_MIN_i256), NA_MINUS_ONE_i256));
  }
}



void testSetUnsetSign(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues s = naMakeSignFlippedNumericsTestValues(&p);

  naTestGroup("naSetUnset8") {
    int8 tmp8 = p.v8;
    naSetSignBit8(&tmp8);
    naTest(tmp8 == s.v8);
    naUnsetSignBit8(&tmp8);
    naTest(tmp8 == p.v8);
    tmp8 = s.v8;
    naUnsetSignBit8(&tmp8);
    naTest(tmp8 == p.v8);
    naSetSignBit8(&tmp8);
    naTest(tmp8 == s.v8);
  }

  naTestGroup("naSetUnset16") {
    int16 tmp16 = p.v16;
    naSetSignBit16(&tmp16);
    naTest(tmp16 == s.v16);
    naUnsetSignBit16(&tmp16);
    naTest(tmp16 == p.v16);
    tmp16 = s.v16;
    naUnsetSignBit16(&tmp16);
    naTest(tmp16 == p.v16);
    naSetSignBit16(&tmp16);
    naTest(tmp16 == s.v16);
  }

  naTestGroup("naSetUnset32") {
    int32 tmp32 = p.v32;
    naSetSignBit32(&tmp32);
    naTest(tmp32 == s.v32);
    naUnsetSignBit32(&tmp32);
    naTest(tmp32 == p.v32);
    tmp32 = s.v32;
    naUnsetSignBit32(&tmp32);
    naTest(tmp32 == p.v32);
    naSetSignBit32(&tmp32);
    naTest(tmp32 == s.v32);
  }

  naTestGroup("naSetUnset64") {
    int64 tmp64 = p.v64;
    naSetSignBit64(&tmp64);
    naTest(naEquali64(tmp64, s.v64));
    naUnsetSignBit64(&tmp64);
    naTest(naEquali64(tmp64, p.v64));
    tmp64 = s.v64;
    naUnsetSignBit64(&tmp64);
    naTest(naEquali64(tmp64, p.v64));
    naSetSignBit64(&tmp64);
    naTest(naEquali64(tmp64, s.v64));
  }

  naTestGroup("naSetUnset128") {
    int128 tmp128 = p.v128;
    naSetSignBit128(&tmp128);
    naTest(naEquali128(tmp128, s.v128));
    naUnsetSignBit128(&tmp128);
    naTest(naEquali128(tmp128, p.v128));
    tmp128 = s.v128;
    naUnsetSignBit128(&tmp128);
    naTest(naEquali128(tmp128, p.v128));
    naSetSignBit128(&tmp128);
    naTest(naEquali128(tmp128, s.v128));
  }

  naTestGroup("naSetUnset256") {
    int256 tmp256 = p.v256;
    naSetSignBit256(&tmp256);
    naTest(naEquali256(tmp256, s.v256));
    naUnsetSignBit256(&tmp256);
    naTest(naEquali256(tmp256, p.v256));
    tmp256 = s.v256;
    naUnsetSignBit256(&tmp256);
    naTest(naEquali256(tmp256, p.v256));
    naSetSignBit256(&tmp256);
    naTest(naEquali256(tmp256, s.v256));
  }
}


void testGetAbsi(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues n = naMakeNegativeNumericsTestValues(&p);

  naTestGroup("naAbsi8") {
    naTest(naAbsi8(p.v8) == p.v8);
    naTest(naAbsi8(NA_ZERO_i8) == NA_ZERO_i8);
    naTest(naAbsi8(n.v8) == p.v8);
    naTest(naAbsi8(NA_MAX_i8) == NA_MAX_i8);
    naTest(naAbsi8(NA_MIN_i8) == NA_MIN_i8);
  }

  naTestGroup("naAbsi16") {
    naTest(naAbsi16(p.v16) == p.v16);
    naTest(naAbsi16(NA_ZERO_i16) == NA_ZERO_i16);
    naTest(naAbsi16(n.v16) == p.v16);
    naTest(naAbsi16(NA_MAX_i16) == NA_MAX_i16);
    naTest(naAbsi16(NA_MIN_i16) == NA_MIN_i16);
  }

  naTestGroup("naAbsi32") {
    naTest(naAbsi32(p.v32) == p.v32);
    naTest(naAbsi32(NA_ZERO_i32) == NA_ZERO_i32);
    naTest(naAbsi32(n.v32) == p.v32);
    naTest(naAbsi32(NA_MAX_i32) == NA_MAX_i32);
    naTest(naAbsi32(NA_MIN_i32) == NA_MIN_i32);
  }

  naTestGroup("naAbsi64") {
    naTest(naEquali64(naAbsi64(p.v64), p.v64));
    naTest(naEquali64(naAbsi64(NA_ZERO_i64), NA_ZERO_i64));
    naTest(naEquali64(naAbsi64(n.v64), p.v64));
    naTest(naEquali64(naAbsi64(NA_MAX_i64), NA_MAX_i64));
    naTest(naEquali64(naAbsi64(NA_MIN_i64), NA_MIN_i64));
  }

  naTestGroup("naAbsi128") {
    naTest(naEquali128(naAbsi128(p.v128), p.v128));
    naTest(naEquali128(naAbsi128(NA_ZERO_i128), NA_ZERO_i128));
    naTest(naEquali128(naAbsi128(n.v128), p.v128));
    naTest(naEquali128(naAbsi128(NA_MAX_i128), NA_MAX_i128));
    naTest(naEquali128(naAbsi128(NA_MIN_i128), NA_MIN_i128));
  }

  naTestGroup("naAbsi256") {
    naTest(naEquali256(naAbsi256(p.v256), p.v256));
    naTest(naEquali256(naAbsi256(NA_ZERO_i256), NA_ZERO_i256));
    naTest(naEquali256(naAbsi256(n.v256), p.v256));
    naTest(naEquali256(naAbsi256(NA_MAX_i256), NA_MAX_i256));
    naTest(naEquali256(naAbsi256(NA_MIN_i256), NA_MIN_i256));
  }
}


void testNANumerics(){
  naTestGroupFunction(GetSignum);
  naTestGroupFunction(GetSigni);
  naTestGroupFunction(SetUnsetSign);
  naTestGroupFunction(GetAbsi);
}



void printNANumerics(){
  printf("NANumerics.h:" NA_NL);

  naPrintMacroix8(NA_SIGN_MASK_8);
  naPrintMacroux8(NA_ZERO_u8);
  naPrintMacroix8(NA_ZERO_i8);
  naPrintMacroux8(NA_ONE_u8);
  naPrintMacroix8(NA_ONE_i8);
  naPrintMacroux8(NA_MAX_u8);
  naPrintMacroix8(NA_MAX_i8);
  naPrintMacroix8(NA_MIN_i8);
  naPrintMacroix8(NA_MINUS_ONE_i8);

  naPrintMacroix16(NA_SIGN_MASK_16);
  naPrintMacroux16(NA_ZERO_u16);
  naPrintMacroix16(NA_ZERO_i16);
  naPrintMacroux16(NA_ONE_u16);
  naPrintMacroix16(NA_ONE_i16);
  naPrintMacroux16(NA_MAX_u16);
  naPrintMacroix16(NA_MAX_i16);
  naPrintMacroix16(NA_MIN_i16);
  naPrintMacroix16(NA_MINUS_ONE_i16);

  naPrintMacroix32(NA_SIGN_MASK_32);
  naPrintMacroux32(NA_ZERO_u32);
  naPrintMacroix32(NA_ZERO_i32);
  naPrintMacroux32(NA_ONE_u32);
  naPrintMacroix32(NA_ONE_i32);
  naPrintMacroux32(NA_MAX_u32);
  naPrintMacroix32(NA_MAX_i32);
  naPrintMacroix32(NA_MIN_i32);
  naPrintMacroix32(NA_MINUS_ONE_i32);

  naPrintMacroix64(NA_SIGN_MASK_64);
  naPrintMacroux64(NA_ZERO_u64);
  naPrintMacroix64(NA_ZERO_i64);
  naPrintMacroux64(NA_ONE_u64);
  naPrintMacroix64(NA_ONE_i64);
  naPrintMacroux64(NA_MAX_u64);
  naPrintMacroix64(NA_MAX_i64);
  naPrintMacroix64(NA_MIN_i64);
  naPrintMacroix64(NA_MINUS_ONE_i64);

  naPrintMacroix128(NA_SIGN_MASK_128);
  naPrintMacroux128(NA_ZERO_u128);
  naPrintMacroix128(NA_ZERO_i128);
  naPrintMacroux128(NA_ONE_u128);
  naPrintMacroix128(NA_ONE_i128);
  naPrintMacroux128(NA_MAX_u128);
  naPrintMacroix128(NA_MAX_i128);
  naPrintMacroix128(NA_MIN_i128);
  naPrintMacroix128(NA_MINUS_ONE_i128);

  naPrintMacroix256(NA_SIGN_MASK_256);
  naPrintMacroux256(NA_ZERO_u256);
  naPrintMacroix256(NA_ZERO_i256);
  naPrintMacroux256(NA_ONE_u256);
  naPrintMacroix256(NA_ONE_i256);
  naPrintMacroux256(NA_MAX_u256);
  naPrintMacroix256(NA_MAX_i256);
  naPrintMacroix256(NA_MIN_i256);
  naPrintMacroix256(NA_MINUS_ONE_i256);

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
