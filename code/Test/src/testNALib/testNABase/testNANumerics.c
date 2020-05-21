
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

  naG("naGetSignum8") {
    naT(naGetSignum8(p.v8) == NA_ZERO_u8);
    naT(naGetSignum8(NA_ZERO_i8) == NA_ZERO_u8);
    naT(naGetSignum8(n.v8) == NA_MAX_u8);
    naT(naGetSignum8(NA_MAX_i8) == NA_ZERO_u8);
    naT(naGetSignum8(NA_MIN_i8) == NA_MAX_u8);
  }

  naG("naGetSignum16") {
    naT(naGetSignum16(p.v16) == NA_ZERO_u16);
    naT(naGetSignum16(NA_ZERO_i16) == NA_ZERO_u16);
    naT(naGetSignum16(n.v16) == NA_MAX_u16);
    naT(naGetSignum16(NA_MAX_i16) == NA_ZERO_u16);
    naT(naGetSignum16(NA_MIN_i16) == NA_MAX_u16);
  }

  naG("naGetSignum32") {
    naT(naGetSignum32(p.v32) == NA_ZERO_u32);
    naT(naGetSignum32(NA_ZERO_i32) == NA_ZERO_u32);
    naT(naGetSignum32(n.v32) == NA_MAX_u32);
    naT(naGetSignum32(NA_MAX_i32) == NA_ZERO_u32);
    naT(naGetSignum32(NA_MIN_i32) == NA_MAX_u32);
  }

  naG("naGetSignum64") {
    naT(naEqualu64(naGetSignum64(p.v64), NA_ZERO_u64));
    naT(naEqualu64(naGetSignum64(NA_ZERO_i64), NA_ZERO_u64));
    naT(naEqualu64(naGetSignum64(n.v64), NA_MAX_u64));
    naT(naEqualu64(naGetSignum64(NA_MAX_i64), NA_ZERO_u64));
    naT(naEqualu64(naGetSignum64(NA_MIN_i64), NA_MAX_u64));
  }

  naG("naGetSignum128") {
    naT(naEqualu128(naGetSignum128(p.v128), NA_ZERO_u128));
    naT(naEqualu128(naGetSignum128(NA_ZERO_i128), NA_ZERO_u128));
    naT(naEqualu128(naGetSignum128(n.v128), NA_MAX_u128));
    naT(naEqualu128(naGetSignum128(NA_MAX_i128), NA_ZERO_u128));
    naT(naEqualu128(naGetSignum128(NA_MIN_i128), NA_MAX_u128));
  }

  naG("naGetSignum256") {
    naT(naEqualu256(naGetSignum256(p.v256), NA_ZERO_u256));
    naT(naEqualu256(naGetSignum256(NA_ZERO_i256), NA_ZERO_u256));
    naT(naEqualu256(naGetSignum256(n.v256), NA_MAX_u256));
    naT(naEqualu256(naGetSignum256(NA_MAX_i256), NA_ZERO_u256));
    naT(naEqualu256(naGetSignum256(NA_MIN_i256), NA_MAX_u256));
  }
}



void testGetSigni(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues n = naMakeNegativeNumericsTestValues(&p);

  naG("naGetSigni8") {
    naT(naSigni8(p.v8) == NA_ONE_i8);
    naT(naSigni8(NA_ZERO_i8) == NA_ONE_i8);
    naT(naSigni8(n.v8) == NA_MINUS_ONE_i8);
    naT(naSigni8(NA_MAX_i8) == NA_ONE_i8);
    naT(naSigni8(NA_MIN_i8) == NA_MINUS_ONE_i8);
  }

  naG("naGetSigni16") {
    naT(naSigni16(p.v16) == NA_ONE_i16);
    naT(naSigni16(NA_ZERO_i16) == NA_ONE_i16);
    naT(naSigni16(n.v16) == NA_MINUS_ONE_i16);
    naT(naSigni16(NA_MAX_i16) == NA_ONE_i16);
    naT(naSigni16(NA_MIN_i16) == NA_MINUS_ONE_i16);
  }

  naG("naGetSigni32") {
    naT(naSigni32(p.v32) == NA_ONE_i32);
    naT(naSigni32(NA_ZERO_i32) == NA_ONE_i32);
    naT(naSigni32(n.v32) == NA_MINUS_ONE_i32);
    naT(naSigni32(NA_MAX_i32) == NA_ONE_i32);
    naT(naSigni32(NA_MIN_i32) == NA_MINUS_ONE_i32);
  }

  naG("naGetSigni64") {
    naT(naEquali64(naSigni64(p.v64), NA_ONE_i64));
    naT(naEquali64(naSigni64(NA_ZERO_i64), NA_ONE_i64));
    naT(naEquali64(naSigni64(n.v64), NA_MINUS_ONE_i64));
    naT(naEquali64(naSigni64(NA_MAX_i64), NA_ONE_i64));
    naT(naEquali64(naSigni64(NA_MIN_i64), NA_MINUS_ONE_i64));
  }

  naG("naGetSigni128") {
    naT(naEquali128(naSigni128(p.v128), NA_ONE_i128));
    naT(naEquali128(naSigni128(NA_ZERO_i128), NA_ONE_i128));
    naT(naEquali128(naSigni128(n.v128), NA_MINUS_ONE_i128));
    naT(naEquali128(naSigni128(NA_MAX_i128), NA_ONE_i128));
    naT(naEquali128(naSigni128(NA_MIN_i128), NA_MINUS_ONE_i128));
  }

  naG("naGetSigni256") {
    naT(naEquali256(naSigni256(p.v256), NA_ONE_i256));
    naT(naEquali256(naSigni256(NA_ZERO_i256), NA_ONE_i256));
    naT(naEquali256(naSigni256(n.v256), NA_MINUS_ONE_i256));
    naT(naEquali256(naSigni256(NA_MAX_i256), NA_ONE_i256));
    naT(naEquali256(naSigni256(NA_MIN_i256), NA_MINUS_ONE_i256));
  }
}



void testSetUnsetSign(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues s = naMakeSignFlippedNumericsTestValues(&p);

  naG("naSetUnset8") {
    int8 tmp8 = p.v8;
    naSetSignBit8(&tmp8);
    naT(tmp8 == s.v8);
    naUnsetSignBit8(&tmp8);
    naT(tmp8 == p.v8);
    tmp8 = s.v8;
    naUnsetSignBit8(&tmp8);
    naT(tmp8 == p.v8);
    naSetSignBit8(&tmp8);
    naT(tmp8 == s.v8);
  }

  naG("naSetUnset16") {
    int16 tmp16 = p.v16;
    naSetSignBit16(&tmp16);
    naT(tmp16 == s.v16);
    naUnsetSignBit16(&tmp16);
    naT(tmp16 == p.v16);
    tmp16 = s.v16;
    naUnsetSignBit16(&tmp16);
    naT(tmp16 == p.v16);
    naSetSignBit16(&tmp16);
    naT(tmp16 == s.v16);
  }

  naG("naSetUnset32") {
    int32 tmp32 = p.v32;
    naSetSignBit32(&tmp32);
    naT(tmp32 == s.v32);
    naUnsetSignBit32(&tmp32);
    naT(tmp32 == p.v32);
    tmp32 = s.v32;
    naUnsetSignBit32(&tmp32);
    naT(tmp32 == p.v32);
    naSetSignBit32(&tmp32);
    naT(tmp32 == s.v32);
  }

  naG("naSetUnset64") {
    int64 tmp64 = p.v64;
    naSetSignBit64(&tmp64);
    naT(naEquali64(tmp64, s.v64));
    naUnsetSignBit64(&tmp64);
    naT(naEquali64(tmp64, p.v64));
    tmp64 = s.v64;
    naUnsetSignBit64(&tmp64);
    naT(naEquali64(tmp64, p.v64));
    naSetSignBit64(&tmp64);
    naT(naEquali64(tmp64, s.v64));
  }

  naG("naSetUnset128") {
    int128 tmp128 = p.v128;
    naSetSignBit128(&tmp128);
    naT(naEquali128(tmp128, s.v128));
    naUnsetSignBit128(&tmp128);
    naT(naEquali128(tmp128, p.v128));
    tmp128 = s.v128;
    naUnsetSignBit128(&tmp128);
    naT(naEquali128(tmp128, p.v128));
    naSetSignBit128(&tmp128);
    naT(naEquali128(tmp128, s.v128));
  }

  naG("naSetUnset256") {
    int256 tmp256 = p.v256;
    naSetSignBit256(&tmp256);
    naT(naEquali256(tmp256, s.v256));
    naUnsetSignBit256(&tmp256);
    naT(naEquali256(tmp256, p.v256));
    tmp256 = s.v256;
    naUnsetSignBit256(&tmp256);
    naT(naEquali256(tmp256, p.v256));
    naSetSignBit256(&tmp256);
    naT(naEquali256(tmp256, s.v256));
  }
}


void testGetAbsi(){
  NANumericsTestValues p = naMakePositiveNumericsTestValues();
  NANumericsTestValues n = naMakeNegativeNumericsTestValues(&p);

  naG("naAbsi8") {
    naT(naAbsi8(p.v8) == p.v8);
    naT(naAbsi8(NA_ZERO_i8) == NA_ZERO_i8);
    naT(naAbsi8(n.v8) == p.v8);
    naT(naAbsi8(NA_MAX_i8) == NA_MAX_i8);
    naT(naAbsi8(NA_MIN_i8) == NA_MIN_i8);
  }

  naG("naAbsi16") {
    naT(naAbsi16(p.v16) == p.v16);
    naT(naAbsi16(NA_ZERO_i16) == NA_ZERO_i16);
    naT(naAbsi16(n.v16) == p.v16);
    naT(naAbsi16(NA_MAX_i16) == NA_MAX_i16);
    naT(naAbsi16(NA_MIN_i16) == NA_MIN_i16);
  }

  naG("naAbsi32") {
    naT(naAbsi32(p.v32) == p.v32);
    naT(naAbsi32(NA_ZERO_i32) == NA_ZERO_i32);
    naT(naAbsi32(n.v32) == p.v32);
    naT(naAbsi32(NA_MAX_i32) == NA_MAX_i32);
    naT(naAbsi32(NA_MIN_i32) == NA_MIN_i32);
  }

  naG("naAbsi64") {
    naT(naEquali64(naAbsi64(p.v64), p.v64));
    naT(naEquali64(naAbsi64(NA_ZERO_i64), NA_ZERO_i64));
    naT(naEquali64(naAbsi64(n.v64), p.v64));
    naT(naEquali64(naAbsi64(NA_MAX_i64), NA_MAX_i64));
    naT(naEquali64(naAbsi64(NA_MIN_i64), NA_MIN_i64));
  }

  naG("naAbsi128") {
    naT(naEquali128(naAbsi128(p.v128), p.v128));
    naT(naEquali128(naAbsi128(NA_ZERO_i128), NA_ZERO_i128));
    naT(naEquali128(naAbsi128(n.v128), p.v128));
    naT(naEquali128(naAbsi128(NA_MAX_i128), NA_MAX_i128));
    naT(naEquali128(naAbsi128(NA_MIN_i128), NA_MIN_i128));
  }

  naG("naAbsi256") {
    naT(naEquali256(naAbsi256(p.v256), p.v256));
    naT(naEquali256(naAbsi256(NA_ZERO_i256), NA_ZERO_i256));
    naT(naEquali256(naAbsi256(n.v256), p.v256));
    naT(naEquali256(naAbsi256(NA_MAX_i256), NA_MAX_i256));
    naT(naEquali256(naAbsi256(NA_MIN_i256), NA_MIN_i256));
  }
}


void testNANumerics(){
  naF(GetSignum);
  naF(GetSigni);
  naF(SetUnsetSign);
  naF(GetAbsi);
}



void printNANumerics(){
  printf("NANumerics.h:" NA_NL);
  NABool showMacro = NA_FALSE;

  naPrintMacroix8(NA_SIGN_MASK_8, NA_FALSE);
  naPrintMacroux8(NA_ZERO_u8, showMacro);
  naPrintMacroix8(NA_ZERO_i8, showMacro);
  naPrintMacroux8(NA_ONE_u8, showMacro);
  naPrintMacroix8(NA_ONE_i8, showMacro);
  naPrintMacroux8(NA_MAX_u8, showMacro);
  naPrintMacroix8(NA_MAX_i8, showMacro);
  naPrintMacroix8(NA_MIN_i8, showMacro);
  naPrintMacroix8(NA_MINUS_ONE_i8, showMacro);

  naPrintMacroix16(NA_SIGN_MASK_16, NA_FALSE);
  naPrintMacroux16(NA_ZERO_u16, showMacro);
  naPrintMacroix16(NA_ZERO_i16, showMacro);
  naPrintMacroux16(NA_ONE_u16, showMacro);
  naPrintMacroix16(NA_ONE_i16, showMacro);
  naPrintMacroux16(NA_MAX_u16, showMacro);
  naPrintMacroix16(NA_MAX_i16, showMacro);
  naPrintMacroix16(NA_MIN_i16, showMacro);
  naPrintMacroix16(NA_MINUS_ONE_i16, showMacro);

  naPrintMacroix32(NA_SIGN_MASK_32, NA_FALSE);
  naPrintMacroux32(NA_ZERO_u32, showMacro);
  naPrintMacroix32(NA_ZERO_i32, showMacro);
  naPrintMacroux32(NA_ONE_u32, showMacro);
  naPrintMacroix32(NA_ONE_i32, showMacro);
  naPrintMacroux32(NA_MAX_u32, showMacro);
  naPrintMacroix32(NA_MAX_i32, showMacro);
  naPrintMacroix32(NA_MIN_i32, showMacro);
  naPrintMacroix32(NA_MINUS_ONE_i32, showMacro);

  naPrintMacroix64(NA_SIGN_MASK_64, NA_FALSE);
  naPrintMacroux64(NA_ZERO_u64, showMacro);
  naPrintMacroix64(NA_ZERO_i64, showMacro);
  naPrintMacroux64(NA_ONE_u64, showMacro);
  naPrintMacroix64(NA_ONE_i64, showMacro);
  naPrintMacroux64(NA_MAX_u64, showMacro);
  naPrintMacroix64(NA_MAX_i64, showMacro);
  naPrintMacroix64(NA_MIN_i64, showMacro);
  naPrintMacroix64(NA_MINUS_ONE_i64, showMacro);

  naPrintMacroix128(NA_SIGN_MASK_128, NA_FALSE);
  naPrintMacroux128(NA_ZERO_u128, showMacro);
  naPrintMacroix128(NA_ZERO_i128, showMacro);
  naPrintMacroux128(NA_ONE_u128, showMacro);
  naPrintMacroix128(NA_ONE_i128, showMacro);
  naPrintMacroux128(NA_MAX_u128, showMacro);
  naPrintMacroix128(NA_MAX_i128, showMacro);
  naPrintMacroix128(NA_MIN_i128, showMacro);
  naPrintMacroix128(NA_MINUS_ONE_i128, showMacro);

  naPrintMacroix256(NA_SIGN_MASK_256, NA_FALSE);
  naPrintMacroux256(NA_ZERO_u256, showMacro);
  naPrintMacroix256(NA_ZERO_i256, showMacro);
  naPrintMacroux256(NA_ONE_u256, showMacro);
  naPrintMacroix256(NA_ONE_i256, showMacro);
  naPrintMacroux256(NA_MAX_u256, showMacro);
  naPrintMacroix256(NA_MAX_i256, showMacro);
  naPrintMacroix256(NA_MIN_i256, showMacro);
  naPrintMacroix256(NA_MINUS_ONE_i256, showMacro);

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
