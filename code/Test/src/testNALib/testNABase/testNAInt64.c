
#include "NATest.h"
#include "NAUtility/NAString.h"
#include <stdio.h>


NABool equalu64(NAu64 u, uint32 goal1, uint32 goal0) {
  return (naGetu64Hi(u) == goal1
       && naGetu64Lo(u) == goal0);
}

NABool equali64(NAi64 i, uint32 goal1, uint32 goal0) {
  return equalu64(naCasti64Tou64(i), goal1, goal0);
}



void testNAInt64Make(void) {
  NAi64 i1 = naMakei64(-0x12345678, 0x98765432);
  NAi64 i2 = naCasti32Toi64(-0x12345678);
  NAi64 i3 = naCastDoubleToi64(-123456789012345.);
  NAi64 i4 = naMakei64Withu32(0x12345678, 0x98765432);
  NAu64 u1 = naMakeu64(0x98765432, 0x12345678);
  NAu64 u2 = naMakeu64WithLo(0x12345678);
  NAu64 u3 = naMakeu64WithDouble(123456789012345.);
  NAu64 u4 = naMakeu64WithLiteralLo(0x12345678);
  NAu64 u5 = naMakeu64Withu32(0x98765432, 0x12345678);

  naTest(equali64(i1, 0xedcba988, 0x98765432));
  naTest(equali64(i2, 0xffffffff, 0xedcba988));
  naTest(equali64(i3, 0xffff8fb7, 0x79f22087));
  naTest(equali64(i4, 0x12345678, 0x98765432));
  naTest(equalu64(u1, 0x98765432, 0x12345678));
  naTest(equalu64(u2, 0x00000000, 0x12345678));
  naTest(equalu64(u3, 0x00007048, 0x860ddf79));
  naTest(equalu64(u4, 0x00000000, 0x12345678));
  naTest(equalu64(u5, 0x98765432, 0x12345678));
}



void testNAInt64Cast(void) {
  naTest(naCasti64ToDouble(naMakei64(0x00200000, 0x00000000)) == 0x1.p53);
  naTest(naCasti64ToDouble(naNegi64(naMakei64(0x00200000, 0x00000000))) == -0x1.p53);
}



void benchmarkNAInt64Make(void) {
  naBenchmark(naMakei64(-(int32)naTestIn, naTestIn));
  naBenchmark(naCasti32Toi64(-(int32)naTestIn));
  naBenchmark(naCastDoubleToi64(-((int32)naTestIn / NA_MAX_i32)));
  naBenchmark(naMakei64Withu32(naTestIn, naTestIn));
  naBenchmark(naMakeu64(naTestIn, naTestIn));
  naBenchmark(naMakeu64WithLo(naTestIn));
  naBenchmark(naMakeu64WithDouble((naTestIn / NA_MAX_u32)));
  naBenchmark(naMakeu64Withu32(naTestIn, naTestIn));
}



void testNAInt64Binary(void) {
  NAi64 i = naCastDoubleToi64(-123456789012345.); 
  NAi64 term = naMakei64Withu32(0x55555555, 0x55555555);
  NAu64 u = naMakeu64Withu32(0x12345678, 0x56473829);
  NAu64 uterm = naMakeu64Withu32(0x55555555, 0x55555555);

  NAi64 i1 = naNoti64(i);
  NAi64 i2 = naOri64(i, term);
  NAi64 i3 = naAndi64(i, term);
  NAi64 i4 = naXori64(i, term);
  NAi64 i5 = naShli64(i, 5);
  NAi64 i6 = naShri64(i, 5);

  NAu64 u1 = naNotu64(u);
  NAu64 u2 = naOru64(u, uterm);
  NAu64 u3 = naAndu64(u, uterm);
  NAu64 u4 = naXoru64(u, uterm);
  NAu64 u5 = naShlu64(u, 5);
  NAu64 u6 = naShru64(u, 5);

  naTest(equali64(i1, 0x00007048, 0x860ddf78));
  naTest(equali64(i2, 0xffffdff7, 0x7df775d7));
  naTest(equali64(i3, 0x55550515, 0x51500005));
  naTest(equali64(i4, 0xaaaadae2, 0x2ca775d2));
  naTest(equali64(i5, 0xfff1f6ef, 0x3e4410e0));
  naTest(equali64(i6, 0xfffffc7d, 0xbbcf9104));

  naTest(equalu64(u1, 0xedcba987, 0xa9b8c7d6));
  naTest(equalu64(u2, 0x5775577d, 0x57577d7d));
  naTest(equalu64(u3, 0x10145450, 0x54451001));
  naTest(equalu64(u4, 0x4761032d, 0x03126d7c));
  naTest(equalu64(u5, 0x468acf0a, 0xc8e70520));
  naTest(equalu64(u6, 0x0091a2b3, 0xc2b239c1));
}



#define randi64 naMakei64Withu32(naTestIn, naTestIn)
#define randu64 naMakeu64Withu32(naTestIn, naTestIn)

void benchmarkNAInt64Binary(void) {
  naBenchmark(naNoti64(randi64));
  naBenchmark(naOri64(randi64, randi64));
  naBenchmark(naAndi64(randi64, randi64));
  naBenchmark(naXori64(randi64, randi64));
  naBenchmark(naShli64(randi64, naTestIn % 63));
  naBenchmark(naShri64(randi64, naTestIn % 63));

  naBenchmark(naNotu64(randu64));
  naBenchmark(naOru64(randu64, randu64));
  naBenchmark(naAndu64(randu64, randu64));
  naBenchmark(naXoru64(randu64, randu64));
  naBenchmark(naShlu64(randu64, naTestIn % 63));
  naBenchmark(naShru64(randu64, naTestIn % 63));
}



void testNAInt64Comparison(void) {
  NAi64 i1 = naCastDoubleToi64(-123456789012345.);
  NAi64 i2 = naCastDoubleToi64(-123456.);
  NAu64 u1 = naMakeu64WithDouble(123456789012345.);
  NAu64 u2 = naMakeu64WithDouble(123456.);

  naTest(naEquali64(i1, i1));
  naTest(!naGreateri64(i1, i2));
  naTest(naGreateri64(i2, i1));
  naTest(!naGreateri64(i1, i1));
  naTest(!naGreaterEquali64(i1, i2));
  naTest(naGreaterEquali64(i2, i1));
  naTest(naGreaterEquali64(i1, i1));
  naTest(naSmalleri64(i1, i2));
  naTest(!naSmalleri64(i2, i1));
  naTest(!naSmalleri64(i1, i1));
  naTest(naSmallerEquali64(i1, i2));
  naTest(!naSmallerEquali64(i2, i1));
  naTest(naSmallerEquali64(i1, i1));

  naTest(naEqualu64(u1, u1));
  naTest(naGreateru64(u1, u2));
  naTest(!naGreateru64(u2, u1));
  naTest(!naGreateru64(u1, u1));
  naTest(naGreaterEqualu64(u1, u2));
  naTest(!naGreaterEqualu64(u2, u1));
  naTest(naGreaterEqualu64(u1, u1));
  naTest(!naSmalleru64(u1, u2));
  naTest(naSmalleru64(u2, u1));
  naTest(!naSmalleru64(u1, u1));
  naTest(!naSmallerEqualu64(u1, u2));
  naTest(naSmallerEqualu64(u2, u1));
  naTest(naSmallerEqualu64(u1, u1));
}



void benchmarkNAInt64Comparison(void) {
  naBenchmark(naEquali64(randi64, randi64));
  naBenchmark(naGreateri64(randi64, randi64));
  naBenchmark(naGreaterEquali64(randi64, randi64));
  naBenchmark(naSmalleri64(randi64, randi64));
  naBenchmark(naSmallerEquali64(randi64, randi64));

  naBenchmark(naEqualu64(randu64, randu64));
  naBenchmark(naGreateru64(randu64, randu64));
  naBenchmark(naGreaterEqualu64(randu64, randu64));
  naBenchmark(naSmalleru64(randu64, randu64));
  naBenchmark(naSmallerEqualu64(randu64, randu64));
}



void testNAInt64Arithmetic(void) {
  NAi64 i01 = naCastDoubleToi64(-123456789012345.);
  NAi64 i02 = naCastDoubleToi64(987654321987.);
  NAi64 i03 = NA_ZERO_i64;
  NAi64 i04 = naCastDoubleToi64(-123.);
  NAi64 i1, i2;
  NAu64 u01 = naMakeu64WithDouble(123456789012345.);
  NAu64 u02 = naMakeu64WithDouble(987654321987.);
  NAu64 u03 = NA_ZERO_u64;
  NAu64 u2;

  i1 = naNegi64(i01);
  naTest(equali64(i1, 0x00007048, 0x860ddf79));
  i1 = naNegi64(naNegi64(i01));
  naTest(equali64(i1, 0xffff8fb7, 0x79f22087));
  naInci64(i01);
  naTest(equali64(i01, 0xffff8fb7, 0x79f22088));
  naDeci64(i01);
  naTest(equali64(i01, 0xffff8fb7, 0x79f22087));
  naInci64(i03);
  naTest(equali64(i03, 0x00000000, 0x00000001));
  naDeci64(i03);
  naTest(equali64(i03, 0x00000000, 0x00000000));
  naDeci64(i03);
  naTest(equali64(i03, 0xffffffff, 0xffffffff));
  naInci64(i03);
  naTest(equali64(i03, 0x00000000, 0x00000000));

  i2 = naAddi64(i01, i02);
  naTest(equali64(i2, 0xffff909d, 0x6ebb17ca));
  i2 = naSubi64(i01, i02);
  naTest(equali64(i2, 0xffff8ed1, 0x85292944));
  i2 = naMuli64(i01, i04);
  naTest(equali64(i2, 0x0035f2d8, 0x68aa5f23));
  i2 = naMuli64(i02, i04);
  naTest(equali64(i2, 0xffff9183, 0x637132cf));
  i2 = naDivi64(i01, i02);
  naTest(equali64(i2, 0xffffffff, 0xffffff84));
  i2 = naDivi64(i01, i04);
  naTest(equali64(i2, 0x000000e9, 0xb2001cdf));
  i2 = naDivi64(i04, i01);
  naTest(equali64(i2, 0x00000000, 0x00000000));
  i2 = naModi64(i01, i02);
  naTest(equali64(i2, 0xffffff1a, 0x0b49e4fb));
  i2 = naModi64(i01, i04);
  naTest(equali64(i2, 0xffffffff, 0xffffffac));

  naIncu64(u01);
  naTest(equalu64(u01, 0x00007048, 0x860ddf7a));
  naDecu64(u01);
  naTest(equalu64(u01, 0x00007048, 0x860ddf79));
  naIncu64(u03);
  naTest(equalu64(u03, 0x00000000, 0x00000001));
  naDecu64(u03);
  naTest(equalu64(u03, 0x00000000, 0x00000000));
  naDecu64(u03);
  naTest(equalu64(u03, 0xffffffff, 0xffffffff));
  naIncu64(u03);
  naTest(equalu64(u03, 0x00000000, 0x00000000));

  u2 = naAddu64(u01, u02);
  naTest(equalu64(u2, 0x0000712e, 0x7ad6d6bc));
  u2 = naSubu64(u01, u02);
  naTest(equalu64(u2, 0x00006f62, 0x9144e836));
  u2 = naDivu64(u01, u02);
  naTest(equalu64(u2, 0x00000000, 0x0000007c));
  u2 = naModu64(u01, u02);
  naTest(equalu64(u2, 0x000000e5, 0xf4b61b05));
}



void benchmarkNAInt64Arithmetic(void) {
  NAi64 i;
  NAu64 u;

  naBenchmark(naNegi64(randi64));
  naBenchmark((i = randi64, naInci64(i)));
  naBenchmark((i = randi64, naDeci64(i)));

  naBenchmark(naAddi64(randi64, randi64));
  naBenchmark(naSubi64(randi64, randi64));
  naBenchmark(naMuli64(randi64, randi64));
  naBenchmark(naDivi64(randi64, randi64));
  naBenchmark(naModi64(randi64, randi64));

  naBenchmark((u = randu64, naIncu64(u)));
  naBenchmark((u = randu64, naDecu64(u)));

  naBenchmark(naAddu64(randu64, randu64));
  naBenchmark(naSubu64(randu64, randu64));
  naBenchmark(naMulu64(randu64, randu64));
  naBenchmark(naDivu64(randu64, randu64));
  naBenchmark(naModu64(randu64, randu64));
}



void testNAInt64(void) {
  naTestFunction(testNAInt64Make);
  naTestFunction(testNAInt64Cast);
  naTestFunction(testNAInt64Binary);
  naTestFunction(testNAInt64Comparison);
  naTestFunction(testNAInt64Arithmetic);
}



void benchmarkNAInt64(void) {
  printf(NA_NL "NAInt64:" NA_NL);
  benchmarkNAInt64Make();
  benchmarkNAInt64Binary();
  benchmarkNAInt64Comparison();
  benchmarkNAInt64Arithmetic();
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
