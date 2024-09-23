
#include "NATest.h"
#include "NAUtility/NAString.h"
#include <stdio.h>


NABool equalu256(NAu256 u, uint32 goal7, uint32 goal6, uint32 goal5, uint32 goal4, uint32 goal3, uint32 goal2, uint32 goal1, uint32 goal0) {
  return (naGetu64Hi(naGetu128Hi(naGetu256Hi(u))) == goal7
       && naGetu64Lo(naGetu128Hi(naGetu256Hi(u))) == goal6
       && naGetu64Hi(naGetu128Lo(naGetu256Hi(u))) == goal5
       && naGetu64Lo(naGetu128Lo(naGetu256Hi(u))) == goal4
       && naGetu64Hi(naGetu128Hi(naGetu256Lo(u))) == goal3
       && naGetu64Lo(naGetu128Hi(naGetu256Lo(u))) == goal2
       && naGetu64Hi(naGetu128Lo(naGetu256Lo(u))) == goal1
       && naGetu64Lo(naGetu128Lo(naGetu256Lo(u))) == goal0);
}

NABool equali256(NAi256 i, uint32 goal7, uint32 goal6, uint32 goal5, uint32 goal4, uint32 goal3, uint32 goal2, uint32 goal1, uint32 goal0) {
  return equalu256(naCasti256Tou256(i), goal7, goal6, goal5, goal4, goal3, goal2, goal1, goal0);
}



void testNAInt256Make(void) {
  NAi256 i1 = naMakei256(naMakei128(naMakei64(-0x11223344, 0xffeeddcc), naMakeu64(0x98765432, 0x12345678)), naMakeu128(naMakei64(0xfedcba98, 0xabcabcde), naMakeu64(0x54345676, 0x11224488)));
  NAi256 i2 = naMakei256WithLo(naMakei128WithLo(naNegi64(naMakei64(0x12345678, 0x12345678))));
  NAi256 i3 = naMakei256WithDouble(-123456789012345.);
  NAi256 i4 = naMakei256WithBinary(0x12345678, 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488);
  NAu256 u1 = naMakeu256(naMakeu128(naMakeu64(0x11223344, 0xffeeddcc), naMakeu64(0x98765432, 0x12345678)), naMakeu128(naMakei64(0xfedcba98, 0xabcabcde), naMakeu64(0x54345676, 0x11224488)));
  NAu256 u2 = naMakeu256WithLo(naMakeu128WithLo(naMakeu64(0x12345678, 0x12345678)));
  NAu256 u3 = naMakeu256WithDouble(123456789012345.);
  NAu256 u5 = naMakeu256WithBinary(0x12345678, 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488);

  naTest(equali256(i1, 0xeeddccbc, 0xffeeddcc, 0x98765432, 0x12345678, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488));
  naTest(equali256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xedcba987, 0xedcba988));
  naTest(equali256(i3, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff8fb7, 0x79f22087));
  naTest(equali256(i4, 0x12345678, 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488));
  naTest(equalu256(u1, 0x11223344, 0xffeeddcc, 0x98765432, 0x12345678, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488));
  naTest(equalu256(u2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x12345678, 0x12345678));
  naTest(equalu256(u3, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00007048 , 0x860ddf79));
  naTest(equalu256(u5, 0x12345678 , 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488));
}



#define randi128 naMakei128WithBinary(naTestIn, naTestIn, naTestIn, naTestIn)
#define randu128 naMakeu128WithBinary(naTestIn, naTestIn, naTestIn, naTestIn)
#define randi256 naMakei256WithBinary(naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn)
#define randu256 naMakeu256WithBinary(naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn)



void benchmarkNAInt256Make(void) {
  naBenchmark(naMakei256(naNegi128(randi128), randu128));
  naBenchmark(naMakei256WithLo(naNegi128(randi128)));
  naBenchmark(naMakei256WithDouble(-((int32)naTestIn / NA_MAX_i32)));
  naBenchmark(naMakei256WithBinary(naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn));
  naBenchmark(naMakeu256(randu128, randu128));
  naBenchmark(naMakeu256WithLo(randu128));
  naBenchmark(naMakeu256WithDouble((naTestIn / NA_MAX_u32)));
  naBenchmark(naMakeu256WithBinary(naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn, naTestIn));
}



void testNAInt256Binary(void) {
  NAi256 i =      naMakei256WithBinary(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  NAi256 term =   naMakei256WithBinary(0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555);
  NAu256 u =    naMakeu256WithBinary(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  NAu256 uterm = naMakeu256WithBinary(0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555);
  
  NAi256 i1 = naNoti256(i);
  NAi256 i2 = naOri256(i, term);
  NAi256 i3 = naAndi256(i, term);
  NAi256 i4 = naXori256(i, term);
  NAi256 i5 = naShli256(i, 5);
  NAi256 i6 = naShri256(i, 5);
  
  NAu256 u1 = naNotu256(u);
  NAu256 u2 = naOru256(u, uterm);
  NAu256 u3 = naAndu256(u, uterm);
  NAu256 u4 = naXoru256(u, uterm);
  NAu256 u5 = naShlu256(u, 5);
  NAu256 u6 = naShru256(u, 5);
  
  naTest(equali256(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e, 0x075abe44, 0x9dd35f9a, 0xcf152bac, 0x4b700dae));
  naTest(equali256(i2, 0xfd7775dd, 0xf575f7fd, 0x7ddf5575, 0x7ffd5fd5, 0xfdf555ff, 0x777df575, 0x75ffd557, 0xf5dff755));
  naTest(equali256(i3, 0x51105554, 0x50151404, 0x54105510, 0x15140041, 0x50054111, 0x40040045, 0x10405451, 0x14055051));
  naTest(equali256(i4, 0xac672089, 0xa560e3f9, 0x29cf0065, 0x6ae95f94, 0xadf014ee, 0x3779f530, 0x65bf8106, 0xe1daa704));
  naTest(equali256(i5, 0x264ebb9e, 0x06b6d58f, 0x934aa607, 0xf781583f, 0x14a8376c, 0x45940ca6, 0x1d5a8a76, 0x91fe4a20));
  naTest(equali256(i6, 0xffc993ae, 0xe781adb5, 0x63e4d2a9, 0x81fde056, 0x0fc52a0d, 0xdb116503, 0x298756a2, 0x9da47f92));
  naTest(equalu256(u1, 0x00a045b1, 0xf965d0db, 0xc0054761, 0xa755d642, 0x78c453db, 0x9b85b016, 0xd6954100, 0xe9b72434));
  naTest(equalu256(u2, 0xff5fff5f, 0x57df7f75, 0x7ffffddf, 0x5dff7dfd, 0xd77ffd75, 0x757f5ffd, 0x7d7fffff, 0x575ddfdf));
  naTest(equalu256(u3, 0x55551044, 0x04100504, 0x15501014, 0x50000115, 0x05110404, 0x44504541, 0x01401455, 0x14405141));
  naTest(equalu256(u4, 0xaa0aef1b, 0x53cf7a71, 0x6aafedcb, 0x0dff7ce8, 0xd26ef971, 0x312f1abc, 0x7c3febaa, 0x431d8e9e));
  naTest(equalu256(u5, 0xebf749c0, 0xd345e487, 0xff5713cb, 0x154537b0, 0xe775848c, 0x8f49fd25, 0x2d57dfe2, 0xc91b7960));
  naTest(equalu256(u6, 0x07fafdd2, 0x7034d179, 0x21ffd5c4, 0xf2c5514d, 0xec39dd61, 0x2323d27f, 0x494b55f7, 0xf8b246de));
}



void benchmarkNAInt256Binary(void) {
  naBenchmark(naNoti256(randi256));
  naBenchmark(naOri256(randi256, randi256));
  naBenchmark(naAndi256(randi256, randi256));
  naBenchmark(naXori256(randi256, randi256));
  naBenchmark(naShli256(randi256, naTestIn % 255));
  naBenchmark(naShri256(randi256, naTestIn % 255));

  naBenchmark(naNotu256(randu256));
  naBenchmark(naOru256(randu256, randu256));
  naBenchmark(naAndu256(randu256, randu256));
  naBenchmark(naXoru256(randu256, randu256));
  naBenchmark(naShlu256(randu256, naTestIn % 255));
  naBenchmark(naShru256(randu256, naTestIn % 255));
}



void testNAInt256Comparison(void) {
  NAi256 i1 =   naMakei256WithBinary(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  NAi256 i2 =   naMakei256WithBinary(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  NAu256 u1 = naMakeu256WithBinary(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  NAu256 u2 = naMakeu256WithBinary(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
  
  naTest(naEquali256(i1, i1));
  naTest(!naGreateri256(i1, i2));
  naTest(naGreateri256(i2, i1));
  naTest(!naGreateri256(i1, i1));
  naTest(!naGreaterEquali256(i1, i2));
  naTest(naGreaterEquali256(i2, i1));
  naTest(naGreaterEquali256(i1, i1));
  naTest(naSmalleri256(i1, i2));
  naTest(!naSmalleri256(i2, i1));
  naTest(!naSmalleri256(i1, i1));
  naTest(naSmallerEquali256(i1, i2));
  naTest(!naSmallerEquali256(i2, i1));
  naTest(naSmallerEquali256(i1, i1));
  
  naTest(naEqualu256(u1, u1));
  naTest(naGreateru256(u1, u2));
  naTest(!naGreateru256(u2, u1));
  naTest(!naGreateru256(u1, u1));
  naTest(naGreaterEqualu256(u1, u2));
  naTest(!naGreaterEqualu256(u2, u1));
  naTest(naGreaterEqualu256(u1, u1));
  naTest(!naSmalleru256(u1, u2));
  naTest(naSmalleru256(u2, u1));
  naTest(!naSmalleru256(u1, u1));
  naTest(!naSmallerEqualu256(u1, u2));
  naTest(naSmallerEqualu256(u2, u1));
  naTest(naSmallerEqualu256(u1, u1));
}



void benchmarkNAInt256Comparison(void) {
  naBenchmark(naEquali256(randi256, randi256));
  naBenchmark(naGreateri256(randi256, randi256));
  naBenchmark(naGreaterEquali256(randi256, randi256));
  naBenchmark(naSmalleri256(randi256, randi256));
  naBenchmark(naSmallerEquali256(randi256, randi256));

  naBenchmark(naEqualu256(randu256, randu256));
  naBenchmark(naGreateru256(randu256, randu256));
  naBenchmark(naGreaterEqualu256(randu256, randu256));
  naBenchmark(naSmalleru256(randu256, randu256));
  naBenchmark(naSmallerEqualu256(randu256, randu256));
}



void testNAInt256Arithmetic(void) {
  NAi256 i01 =   naMakei256WithBinary(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  NAi256 i02 =   naMakei256WithBinary(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  NAi256 i03 =   NA_ZERO_i256;
  NAi256 i04 =   naMakei256WithDouble(-123.);
  NAi256 i1, i2;
  NAu256 u01 = naMakeu256WithBinary(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  NAu256 u02 = naMakeu256WithBinary(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
  NAu256 u03 = NA_ZERO_u256;
  NAu256 u2;
  
  i1 = naNegi256(i01);
  naTest(equali256(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e, 0x075abe44, 0x9dd35f9a, 0xcf152bac, 0x4b700daf));
  i1 = naNegi256(naNegi256(i01));
  naTest(equali256(i1, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251));
  naInci256(i01);
  naTest(equali256(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff252));
  naDeci256(i01);
  naTest(equali256(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251));
  naInci256(i03);
  naTest(equali256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001));
  naDeci256(i03);
  naTest(equali256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  naDeci256(i03);
  naTest(equali256(i03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff));
  naInci256(i03);
  naTest(equali256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  
  i2 = naAddi256(i01, i02);
  naTest(equali256(i2, 0xfb617c69, 0x5c5a548c, 0x92f837d5, 0x67836ab8, 0xeaeb5cff, 0xf6811350, 0x243c3b89, 0x31739396));
  i2 = naSubi256(i01, i02);
  naTest(equali256(i2, 0xf7036f50, 0x841118cc, 0x663c728b, 0x17f4aacb, 0x065f2676, 0xcdd82d7a, 0x3d996d1e, 0x37ac510c));
  i2 = naMuli256(i01, i04);
  naTest(equali256(i2, 0x44c15ed8, 0x96313b20, 0x21d911d1, 0x60a6d4cd, 0x88996af7, 0xd48eef61, 0x7f2bfbc8, 0x3ed69315));
  i2 = naMuli256(i02, i04);
  naTest(equali256(i2, 0xf367da88, 0x0a682555, 0x40e41aa5, 0xe334e459, 0x9850e60c, 0xbb6cc91d, 0x17e3694c, 0xfea183d9));
  i2 = naDivi256(i01, i02);
  naTest(equali256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd));
  i2 = naDivi256(i01, i04);
  naTest(equali256(i2, 0x000e28aa, 0xf3a3fc6e, 0xfce7d9d8, 0x1021da72, 0xf9d0de2a, 0x2f125e6f, 0x914b1954, 0xa71e0c99));
  i2 = naDivi256(i04, i01);
  naTest(equali256(i2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  i2 = naModi256(i01, i02);
  naTest(equali256(i2, 0xffbf8982, 0x34a3904c, 0xbfb3fd1f, 0xb7122aa6, 0xcf779389, 0x1f29f926, 0x0adf09f4, 0x2b3ad620));
  i2 = naModi256(i01, i04);
  naTest(equali256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffd4));
  
  naIncu256(u01);
  naTest(equalu256(u01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcc));
  naDecu256(u01);
  naTest(equalu256(u01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb));
  naIncu256(u03);
  naTest(equalu256(u03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001));
  naDecu256(u03);
  naTest(equalu256(u03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  naDecu256(u03);
  naTest(equalu256(u03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff));
  naIncu256(u03);
  naTest(equalu256(u03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000));
  
  u2 = naAddu256(u01, u02);
  naTest(equalu256(u2, 0xff92302a, 0xf6cf6ad0, 0xbc950ece, 0x9866337f, 0x1077be3a, 0xd5c01e98, 0xd3be87ee, 0x8903c672));
  u2 = naSubu256(u01, u02);
  naTest(equalu256(u2, 0xff2d4471, 0x1664f377, 0xc360626e, 0x18ee1ffb, 0xfdff9a0d, 0xf3348139, 0x7f16f60f, 0xa38df124));
  u2 = naDivu256(u01, u02);
  naTest(equalu256(u2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000050f));
  u2 = naModu256(u01, u02);
  naTest(equalu256(u2, 0x001d81aa, 0xe9515199, 0xef40ba8b, 0xf074cfb8, 0x505c2c9d, 0x6459c54a, 0x8b954bb9, 0xb6bfd902));
}



void benchmarkNAInt256Arithmetic(void) {
  NAi256 i;
  NAu256 u;

  naBenchmark(naNegi256(randi256));
  naBenchmark((i = randi256, (void)naInci256(i), i));
  naBenchmark((i = randi256, (void)naDeci256(i), i));

  naBenchmark(naAddi256(randi256, randi256));
  naBenchmark(naSubi256(randi256, randi256));
  naBenchmark(naMuli256(randi256, randi256));
  naBenchmark(naDivi256(randi256, randi256));
  naBenchmark(naModi256(randi256, randi256));

  naBenchmark((u = randu256, (void)naIncu256(u), u));
  naBenchmark((u = randu256, (void)naDecu256(u), u));

  naBenchmark(naAddu256(randu256, randu256));
  naBenchmark(naSubu256(randu256, randu256));
  naBenchmark(naMulu256(randu256, randu256));
  naBenchmark(naDivu256(randu256, randu256));
  naBenchmark(naModu256(randu256, randu256));
}



void testNAInt256(void) {
  naTestFunction(testNAInt256Make);
  naTestFunction(testNAInt256Binary);
  naTestFunction(testNAInt256Comparison);
  naTestFunction(testNAInt256Arithmetic);
}



void benchmarkNAInt256(void) {
  printf(NA_NL "NAInt256:" NA_NL);
  benchmarkNAInt256Make();
  benchmarkNAInt256Binary();
  benchmarkNAInt256Comparison();
  benchmarkNAInt256Arithmetic();
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
