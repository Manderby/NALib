
#include "NATest.h"
#include "NABase/NABase.h"
#include <stdio.h>



void testNAInteger(void) {
  naTestGroup("NABool typedefs") {
    NABool boolean = NA_TRUE;
    naTest(boolean == NA_TRUE);
  }

  naTestGroup("native typedefs") {
    uint8   u8   = 42;
    int8    i8   = -42;
    uint16  u16  = 42;
    int16   i16  = -42;
    uint32  u32  = 42;
    int32   i32  = -42;
    uint64  u64  = naMakeu64WithLo(42);
    int64   i64  = naCasti32Toi64(-42);
    uint128 u128 = naMakeu128WithLo(u64);
    int128  i128 = naMakei128WithLo(i64);
    uint256 u256 = naMakeu256WithLo(u128);
    int256  i256 = naMakei256WithLo(i128);
    naTest(u8  == 42);
    naTest(i8  == -42);
    naTest(u16 == 42);
    naTest(i16 == -42);
    naTest(u32 == 42);
    naTest(i32 == -42);
    naTest(naEqualu64(u64,   naMakeu64WithLo(42)));
    naTest(naEquali64(i64,   naCasti32Toi64(-42)));
    naTest(naEqualu128(u128, naMakeu128WithLo(u64)));
    naTest(naEquali128(i128, naMakei128WithLo(i64)));
    naTest(naEqualu256(u256, naMakeu256WithLo(u128)));
    naTest(naEquali256(i256, naMakei256WithLo(i128)));
  }

  naTestGroup("emulated typedefs") {
    NAu64  u64  = naMakeu64WithLo(42);
    NAi64  i64  = naCasti32Toi64(-42);
    NAu128 u128 = naMakeu128WithLo(u64);
    NAi128 i128 = naMakei128WithLo(i64);
    NAu256 u256 = naMakeu256WithLo(u128);
    NAi256 i256 = naMakei256WithLo(i128);
    naTest(naEqualu64(u64,   naMakeu64WithLo(42)));
    naTest(naEquali64(i64,   naCasti32Toi64(-42)));
    naTest(naEqualu128(u128, naMakeu128WithLo(u64)));
    naTest(naEquali128(i128, naMakei128WithLo(i64)));
    naTest(naEqualu256(u256, naMakeu256WithLo(u128)));
    naTest(naEquali256(i256, naMakei256WithLo(i128)));
  }
}



void printNAInteger(void) {
  printf("NAInteger.h:" NA_NL);
  
  naPrintMacroInt(NA_FALSE);
  naPrintMacroInt(NA_TRUE);

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
