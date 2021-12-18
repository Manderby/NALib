
#include "NATesting.h"
#include "NAString.h"
#include <stdio.h>


NABool equalu128(NAu128 u, uint32 goal3, uint32 goal2, uint32 goal1, uint32 goal0){
  return (naGetu64Hi(naGetu128Hi(u)) == goal3
       && naGetu64Lo(naGetu128Hi(u)) == goal2
       && naGetu64Hi(naGetu128Lo(u)) == goal1
       && naGetu64Lo(naGetu128Lo(u)) == goal0);
}

NABool equali128(NAi128 i, uint32 goal3, uint32 goal2, uint32 goal1, uint32 goal0){
  return equalu128(naCasti128Tou128(i), goal3, goal2, goal1, goal0);
}



void testNAInt128Make(void){
  NAi128 i1 = naMakei128(naMakei64(-0x11223344, 0xffeeddcc), naMakeu64(0x98765432, 0x12345678));
  NAi128 i2 = naMakei128WithLo(naNegi64(naMakei64(0x12345678, 0x12345678)));
  NAi128 i3 = naMakei128WithDouble(-123456789012345.);
  NAi128 i4 = naMakei128WithBinary(0x11223344, 0xffeeddcc, 0x98765432, 0x12345678);
  NAu128 u1 = naMakeu128(naMakeu64(0x11223344, 0xffeeddcc), naMakeu64(0x98765432, 0x12345678));
  NAu128 u2 = naMakeu128WithLo(naMakeu64(0x98765432, 0x12345678));
  NAu128 u3 = naMakeu128WithDouble(123456789012345.);
  NAu128 u5 = naMakeu128WithBinary(0x11223344, 0xffeeddcc, 0x98765432, 0x12345678);

  naTest(equali128(i1, 0xeeddccbc, 0xffeeddcc, 0x98765432, 0x12345678));
  naTest(equali128(i2, 0xffffffff, 0xffffffff, 0xedcba987, 0xedcba988));
  naTest(equali128(i3, 0xffffffff, 0xffffffff, 0xffff8fb7, 0x79f22087));
  naTest(equali128(i4, 0x11223344, 0xffeeddcc, 0x98765432, 0x12345678));
  naTest(equalu128(u1, 0x11223344, 0xffeeddcc, 0x98765432, 0x12345678));
  naTest(equalu128(u2, 0x00000000, 0x00000000, 0x98765432, 0x12345678));
  naTest(equalu128(u3, 0x00000000, 0x00000000, 0x00007048 , 0x860ddf79));
  naTest(equalu128(u5, 0x11223344, 0xffeeddcc, 0x98765432, 0x12345678));
}



#define randi64 naMakei64WithBinary(naTestIn, naTestIn)
#define randu64 naMakeu64WithBinary(naTestIn, naTestIn)
#define randi128 naMakei128WithBinary(naTestIn, naTestIn, naTestIn, naTestIn)
#define randu128 naMakeu128WithBinary(naTestIn, naTestIn, naTestIn, naTestIn)

void benchmarkNAInt128Make(void){
  naBenchmark(naMakei128(-randi64, randu64));
  naBenchmark(naMakei128WithLo(-randi64));
  naBenchmark(naMakei128WithDouble(-((int32)naTestIn / NA_MAX_i32)));
  naBenchmark(naMakei128WithBinary(naTestIn, naTestIn, naTestIn, naTestIn));
  naBenchmark(naMakeu128(randu64, randu64));
  naBenchmark(naMakeu128WithLo(randu64));
  naBenchmark(naMakeu128WithDouble((naTestIn / NA_MAX_u32)));
  naBenchmark(naMakeu128WithBinary(naTestIn, naTestIn, naTestIn, naTestIn));
}



void testNAInt128Binary(void){
  NAi128 i =      naMakei128WithBinary(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  NAi128 term =   naMakei128WithBinary(0x55555555, 0x55555555, 0x55555555, 0x55555555);
  NAu128 u =    naMakeu128WithBinary(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  NAu128 uterm = naMakeu128WithBinary(0x55555555, 0x55555555, 0x55555555, 0x55555555);
  
  NAi128 i1 = naNoti128(i);
  NAi128 i2 = naOri128(i, term);
  NAi128 i3 = naAndi128(i, term);
  NAi128 i4 = naXori128(i, term);
  NAi128 i5 = naShli128(i, 5);
  NAi128 i6 = naShri128(i, 5);
  
  NAu128 u1 = naNotu128(u);
  NAu128 u2 = naOru128(u, uterm);
  NAu128 u3 = naAndu128(u, uterm);
  NAu128 u4 = naXoru128(u, uterm);
  NAu128 u5 = naShlu128(u, 5);
  NAu128 u6 = naShru128(u, 5);
  
  naTest(equali128(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e));
  naTest(equali128(i2, 0xfd7775dd, 0xf575f7fd, 0x7ddf5575, 0x7ffd5fd5));
  naTest(equali128(i3, 0x51105554, 0x50151404, 0x54105510, 0x15140041));
  naTest(equali128(i4, 0xac672089, 0xa560e3f9, 0x29cf0065, 0x6ae95f94));
  naTest(equali128(i5, 0x264ebb9e, 0x06b6d58f, 0x934aa607, 0xf7815820));
  naTest(equali128(i6, 0xffc993ae, 0xe781adb5, 0x63e4d2a9, 0x81fde056));
  
  naTest(equalu128(u1, 0x00a045b1, 0xf965d0db, 0xc0054761, 0xa755d642));
  naTest(equalu128(u2, 0xff5fff5f, 0x57df7f75, 0x7ffffddf, 0x5dff7dfd));
  naTest(equalu128(u3, 0x55551044, 0x04100504, 0x15501014, 0x50000115));
  naTest(equalu128(u4, 0xaa0aef1b, 0x53cf7a71, 0x6aafedcb, 0x0dff7ce8));
  naTest(equalu128(u5, 0xebf749c0, 0xd345e487, 0xff5713cb, 0x154537a0));
  naTest(equalu128(u6, 0x07fafdd2, 0x7034d179, 0x21ffd5c4, 0xf2c5514d));
}



void benchmarkNAInt128Binary(void){
  naBenchmark(naNoti128(randi128));
  naBenchmark(naOri128(randi128, randi128));
  naBenchmark(naAndi128(randi128, randi128));
  naBenchmark(naXori128(randi128, randi128));
  naBenchmark(naShli128(randi128, naTestIn % 127));
  naBenchmark(naShri128(randi128, naTestIn % 127));

  naBenchmark(naNotu128(randu128));
  naBenchmark(naOru128(randu128, randu128));
  naBenchmark(naAndu128(randu128, randu128));
  naBenchmark(naXoru128(randu128, randu128));
  naBenchmark(naShlu128(randu128, naTestIn % 127));
  naBenchmark(naShru128(randu128, naTestIn % 127));
}



void testNAInt128Comparison(void){
  NAi128 i1 =   naMakei128WithBinary(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  NAi128 i2 =   naMakei128WithBinary(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  NAu128 u1 = naMakeu128WithBinary(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  NAu128 u2 = naMakeu128WithBinary(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
  
  naTest(naEquali128(i1, i1));
  naTest(!naGreateri128(i1, i2));
  naTest(naGreateri128(i2, i1));
  naTest(!naGreateri128(i1, i1));
  naTest(!naGreaterEquali128(i1, i2));
  naTest(naGreaterEquali128(i2, i1));
  naTest(naGreaterEquali128(i1, i1));
  naTest(naSmalleri128(i1, i2));
  naTest(!naSmalleri128(i2, i1));
  naTest(!naSmalleri128(i1, i1));
  naTest(naSmallerEquali128(i1, i2));
  naTest(!naSmallerEquali128(i2, i1));
  naTest(naSmallerEquali128(i1, i1));
  
  naTest(naEqualu128(u1, u1));
  naTest(naGreateru128(u1, u2));
  naTest(!naGreateru128(u2, u1));
  naTest(!naGreateru128(u1, u1));
  naTest(naGreaterEqualu128(u1, u2));
  naTest(!naGreaterEqualu128(u2, u1));
  naTest(naGreaterEqualu128(u1, u1));
  naTest(!naSmalleru128(u1, u2));
  naTest(naSmalleru128(u2, u1));
  naTest(!naSmalleru128(u1, u1));
  naTest(!naSmallerEqualu128(u1, u2));
  naTest(naSmallerEqualu128(u2, u1));
  naTest(naSmallerEqualu128(u1, u1));
}



void benchmarkNAInt128Comparison(void){
  naBenchmark(naEquali128(randi128, randi128));
  naBenchmark(naGreateri128(randi128, randi128));
  naBenchmark(naGreaterEquali128(randi128, randi128));
  naBenchmark(naSmalleri128(randi128, randi128));
  naBenchmark(naSmallerEquali128(randi128, randi128));

  naBenchmark(naEqualu128(randu128, randu128));
  naBenchmark(naGreateru128(randu128, randu128));
  naBenchmark(naGreaterEqualu128(randu128, randu128));
  naBenchmark(naSmalleru128(randu128, randu128));
  naBenchmark(naSmallerEqualu128(randu128, randu128));
}



void testNAInt128Arithmetic(void){
  NAi128 i01 =   naMakei128WithBinary(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  NAi128 i02 =   naMakei128WithBinary(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  NAi128 i03 =   NA_ZERO_i128;
  NAi128 i04 =   naMakei128WithDouble(-123.);
  NAi128 i1, i2;
  NAu128 u01 = naMakeu128WithBinary(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  NAu128 u02 = naMakeu128WithBinary(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
  NAu128 u03 = NA_ZERO_u128;
  NAu128 u2;
  
  i1 = naNegi128(i01);
  naTest(equali128(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53f));
  i1 = naNegi128(naNegi128(i01));
  naTest(equali128(i1, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1));
  naInci128(i01);
  naTest(equali128(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac2));
  naDeci128(i01);
  naTest(equali128(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1));
  naInci128(i03);
  naTest(equali128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000001));
  naDeci128(i03);
  naTest(equali128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  naDeci128(i03);
  naTest(equali128(i03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff));
  naInci128(i03);
  naTest(equali128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  
  i2 = naAddi128(i01, i02);
  naTest(equali128(i2, 0xfb617c69, 0x5c5a548c, 0x92f837d5, 0x67836ab7));
  i2 = naSubi128(i01, i02);
  naTest(equali128(i2, 0xf7036f50, 0x841118cc, 0x663c728b, 0x17f4aacb));
  i2 = naMuli128(i01, i04);
  naTest(equali128(i2, 0x44c15ed8, 0x96313b20, 0x21d911d1, 0x60a6d545));
  i2 = naMuli128(i02, i04);
  naTest(equali128(i2, 0xf367da88, 0x0a682555, 0x40e41aa5, 0xe334e4ce));
  i2 = naDivi128(i01, i02);
  naTest(equali128(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd));
  i2 = naDivi128(i01, i04);
  naTest(equali128(i2, 0x000e28aa, 0xf3a3fc6e, 0xfce7d9d8, 0x1021da72));
  i2 = naDivi128(i04, i01);
  naTest(equali128(i2, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  i2 = naModi128(i01, i02);
  naTest(equali128(i2, 0xffbf8982, 0x34a3904c, 0xbfb3fd1f, 0xb7122aa3));
  i2 = naModi128(i01, i04);
  naTest(equali128(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff87));
  
  naIncu128(u01);
  naTest(equalu128(u01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29be));
  naDecu128(u01);
  naTest(equalu128(u01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd));
  naIncu128(u03);
  naTest(equalu128(u03, 0x00000000, 0x00000000, 0x00000000, 0x00000001));
  naDecu128(u03);
  naTest(equalu128(u03, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  naDecu128(u03);
  naTest(equalu128(u03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff));
  naIncu128(u03);
  naTest(equalu128(u03, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  
  u2 = naAddu128(u01, u02);
  naTest(equalu128(u2, 0xff92302a, 0xf6cf6ad0, 0xbc950ece, 0x9866337e));
  u2 = naSubu128(u01, u02);
  naTest(equalu128(u2, 0xff2d4471, 0x1664f377, 0xc360626e, 0x18ee1ffc));
  u2 = naDivu128(u01, u02);
  naTest(equalu128(u2, 0x00000000, 0x00000000, 0x00000000, 0x0000050f));
  u2 = naModu128(u01, u02);
  naTest(equalu128(u2, 0x001d81aa, 0xe9515199, 0xef40ba8b, 0xf074d26e));
}



void benchmarkNAInt128Arithmetic(void){
  NAi128 i;
  NAu128 u;

  naBenchmark(naNegi128(randi128));
  naBenchmark((i = randi128, naInci128(i), i));
  naBenchmark((i = randi128, naDeci128(i), i));

  naBenchmark(naAddi128(randi128, randi128));
  naBenchmark(naSubi128(randi128, randi128));
  naBenchmark(naMuli128(randi128, randi128));
  naBenchmark(naDivi128(randi128, randi128));
  naBenchmark(naModi128(randi128, randi128));

  naBenchmark((u = randu128, naIncu128(u), u));
  naBenchmark((u = randu128, naDecu128(u), u));

  naBenchmark(naAddu128(randu128, randu128));
  naBenchmark(naSubu128(randu128, randu128));
  naBenchmark(naMulu128(randu128, randu128));
  naBenchmark(naDivu128(randu128, randu128));
  naBenchmark(naModu128(randu128, randu128));
}



void testNAInt128(void){
  naTestFunction(testNAInt128Make);
  naTestFunction(testNAInt128Binary);
  naTestFunction(testNAInt128Comparison);
  naTestFunction(testNAInt128Arithmetic);
}



void benchmarkNAInt128(void){
  printf(NA_NL "NAInt128:" NA_NL);
  benchmarkNAInt128Make();
  benchmarkNAInt128Binary();
  benchmarkNAInt128Comparison();
  benchmarkNAInt128Arithmetic();
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
