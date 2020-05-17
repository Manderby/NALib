//
//#include NA_TEST_NALIB_PATH(NABase.h)
//#include NA_TEST_NALIB_PATH(NADateTime.h)
//#include <stdio.h>
//#include <time.h>
//#if NA_OS != NA_OS_WINDOWS
//  #include <sys/time.h>
//#endif
//
//uint64 randu64(void);
//uint128 randu128(void);
//double getTime(void);
//double printAndSwapTime(const char* title, double starttime);
//void printi128(int128 i);
//void printu128(uint128 ui);
//void testEquali128(int128 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3);
//void testEqualu128(uint128 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3);
//void testEqualBool(NABool b, NABool goalb);
//
//#define MAKEi128(a,b,c,d) naMakei128(naMakei64((int32)a, b), naMakeu64(c, d));
//#define MAKEu128(a,b,c,d) naMakeu128(naMakeu64(a, b), naMakeu64(c, d));
//
//
//#define TESTSIZE 100000
//
//
//
//
//
//void testMaking(){
//  int128 i1 = naMakei128WithLo(naNegi64(naMakei64(0x12345678, 0x12345678)));
//  int128 i2 = naMakei128WithDouble(-123456789012345.);
//  uint128 ui1 = naMakeu128WithLo(naMakeu64WithLo(0x12345678));
//  uint128 ui2 = naMakeu128WithLo(naMakeu64(0x12345678, 0x12345678));
//  uint128 ui3 = naMakeu128WithDouble(123456789012345.);
//  uint128 ui4 = MAKEu128(0x12345678, 0x56473829, 0xfabcedae, 0x33333333);
//  testEquali128(i1, 0xffffffff, 0xffffffff, 0xedcba987, 0xedcba988);
//  testEquali128(i2, 0xffffffff, 0xffffffff, 0xffff8fb7, 0x79f22087);
//  testEqualu128(ui1, 0x00000000, 0x00000000, 0x00000000, 0x12345678);
//  testEqualu128(ui2, 0x00000000, 0x00000000, 0x12345678, 0x12345678);
//  testEqualu128(ui3, 0x00000000, 0x00000000, 0x00007048 , 0x860ddf79);
//  testEqualu128(ui4, 0x12345678 , 0x56473829, 0xfabcedae, 0x33333333);
//}
//
//
//
//void timeMaking(){
//  double t;
//  int i;
//  int128 i128 = NA_ZERO_i128;
//  uint128 u128 = NA_ZERO_u128;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i128 = naAddi128(naMakei128WithLo(naNegi64(naCastu64Toi64(randu64()))), i128);}
//  t = printAndSwapTime("naMakei128WithLo", t);
//  for(i = 0; i < TESTSIZE; i++){i128 = naAddi128(naMakei128WithDouble(-(double)rand() * (double)rand()), i128);}
//  t = printAndSwapTime("naMakei128WithDouble", t);
//
//  for(i = 0; i < TESTSIZE; i++){u128 = naAddu128(naMakeu128WithLo(randu64()), u128);}
//  t = printAndSwapTime("naMakeu128WithLo", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naAddu128(naMakeu128WithDouble((double)rand() * (double)rand()), u128);}
//  t = printAndSwapTime("naMakeu128WithDouble", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naAddu128(naMakeu128(randu64(), randu64()), u128);}
//  t = printAndSwapTime("naMakeu128", t);
//
//  NA_UNUSED(i128);
//  NA_UNUSED(u128);
//  NA_UNUSED(t);
//}
//
//
//
//void testBinary(){
//  int128 i =      MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  int128 term =   MAKEi128(0x55555555, 0x55555555, 0x55555555, 0x55555555);
//  uint128 ui =    MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  uint128 uterm = MAKEu128(0x55555555, 0x55555555, 0x55555555, 0x55555555);
//
//  int128 i1 = naNoti128(i);
//  int128 i2 = naOri128(i, term);
//  int128 i3 = naAndi128(i, term);
//  int128 i4 = naXori128(i, term);
//  int128 i5 = naShli128(i, 5);
//  int128 i6 = naShri128(i, 5);
//
//  uint128 ui1 = naNotu128(ui);
//  uint128 ui2 = naOru128(ui, uterm);
//  uint128 ui3 = naAndu128(ui, uterm);
//  uint128 ui4 = naXoru128(ui, uterm);
//  uint128 ui5 = naShlu128(ui, 5);
//  uint128 ui6 = naShru128(ui, 5);
//
//  testEquali128(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e);
//  testEquali128(i2, 0xfd7775dd, 0xf575f7fd, 0x7ddf5575, 0x7ffd5fd5);
//  testEquali128(i3, 0x51105554, 0x50151404, 0x54105510, 0x15140041);
//  testEquali128(i4, 0xac672089, 0xa560e3f9, 0x29cf0065, 0x6ae95f94);
//  testEquali128(i5, 0x264ebb9e, 0x06b6d58f, 0x934aa607, 0xf7815820);
//  testEquali128(i6, 0xffc993ae, 0xe781adb5, 0x63e4d2a9, 0x81fde056);
//
//  testEqualu128(ui1, 0x00a045b1, 0xf965d0db, 0xc0054761, 0xa755d642);
//  testEqualu128(ui2, 0xff5fff5f, 0x57df7f75, 0x7ffffddf, 0x5dff7dfd);
//  testEqualu128(ui3, 0x55551044, 0x04100504, 0x15501014, 0x50000115);
//  testEqualu128(ui4, 0xaa0aef1b, 0x53cf7a71, 0x6aafedcb, 0x0dff7ce8);
//  testEqualu128(ui5, 0xebf749c0, 0xd345e487, 0xff5713cb, 0x154537a0);
//  testEqualu128(ui6, 0x07fafdd2, 0x7034d179, 0x21ffd5c4, 0xf2c5514d);
//}
//
//
//
//void timeBinary(){
//  int128 i128_1 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  int128 i128_2 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
//  uint128 u128_1 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  uint128 u128_2 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
//  double t;
//  int i;
//  int128 i128;
//  uint128 u128;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i128 = naNoti128(i128_1);}
//  t = printAndSwapTime("naNoti128", t);
//  for(i = 0; i < TESTSIZE; i++){i128 = naOri128(i128_1, i128_2);}
//  t = printAndSwapTime("naOri128", t);
//  for(i = 0; i < TESTSIZE; i++){i128 = naAndi128(i128_1, i128_2);}
//  t = printAndSwapTime("naAndi128", t);
//  for(i = 0; i < TESTSIZE; i++){i128 = naXori128(i128_1, i128_2);}
//  t = printAndSwapTime("naXori128", t);
//  for(i = 0; i < TESTSIZE; i++){i128 = naShli128(i128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShli128", t);
//  for(i = 0; i < TESTSIZE; i++){i128 = naShri128(i128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShri128", t);
//
//  for(i = 0; i < TESTSIZE; i++){u128 = naNotu128(u128_1);}
//  t = printAndSwapTime("naNotu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naOru128(u128_1, u128_2);}
//  t = printAndSwapTime("naOru128", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naAndu128(u128_1, u128_2);}
//  t = printAndSwapTime("naAndu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naXoru128(u128_1, u128_2);}
//  t = printAndSwapTime("naXoru128", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naShlu128(u128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShlu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128 = naShru128(u128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShru128", t);
//
//  NA_UNUSED(i128);
//  NA_UNUSED(u128);
//  NA_UNUSED(t);
//}
//
//
//
//void testComparison(){
//  int128 i1 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  int128 i2 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
//  uint128 ui1 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  uint128 ui2 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
//
//  testEqualBool(naEquali128(i1, i1), NA_TRUE);
//  testEqualBool(naGreateri128(i1, i2), NA_FALSE);
//  testEqualBool(naGreateri128(i2, i1), NA_TRUE);
//  testEqualBool(naGreateri128(i1, i1), NA_FALSE);
//  testEqualBool(naGreaterEquali128(i1, i2), NA_FALSE);
//  testEqualBool(naGreaterEquali128(i2, i1), NA_TRUE);
//  testEqualBool(naGreaterEquali128(i1, i1), NA_TRUE);
//  testEqualBool(naSmalleri128(i1, i2), NA_TRUE);
//  testEqualBool(naSmalleri128(i2, i1), NA_FALSE);
//  testEqualBool(naSmalleri128(i1, i1), NA_FALSE);
//  testEqualBool(naSmallerEquali128(i1, i2), NA_TRUE);
//  testEqualBool(naSmallerEquali128(i2, i1), NA_FALSE);
//  testEqualBool(naSmallerEquali128(i1, i1), NA_TRUE);
//
//  testEqualBool(naEqualu128(ui1, ui1), NA_TRUE);
//  testEqualBool(naGreateru128(ui1, ui2), NA_TRUE);
//  testEqualBool(naGreateru128(ui2, ui1), NA_FALSE);
//  testEqualBool(naGreateru128(ui1, ui1), NA_FALSE);
//  testEqualBool(naGreaterEqualu128(ui1, ui2), NA_TRUE);
//  testEqualBool(naGreaterEqualu128(ui2, ui1), NA_FALSE);
//  testEqualBool(naGreaterEqualu128(ui1, ui1), NA_TRUE);
//  testEqualBool(naSmalleru128(ui1, ui2), NA_FALSE);
//  testEqualBool(naSmalleru128(ui2, ui1), NA_TRUE);
//  testEqualBool(naSmalleru128(ui1, ui1), NA_FALSE);
//  testEqualBool(naSmallerEqualu128(ui1, ui2), NA_FALSE);
//  testEqualBool(naSmallerEqualu128(ui2, ui1), NA_TRUE);
//  testEqualBool(naSmallerEqualu128(ui1, ui1), NA_TRUE);
//}
//
//
//
//void timeComparison(){
//  int128 i01 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  int128 i02 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
//  uint128 ui01 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  uint128 ui02 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
//  double t;
//  int i;
//  NABool testbool = NA_FALSE;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naEquali128(i01, i02); naInci128(i01); naDeci128(i02);}
//  t = printAndSwapTime("naEquali128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateri128(i01, i02); naInci128(i01); naDeci128(i02);}
//  t = printAndSwapTime("naGreateri128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEquali128(i01, i02); naInci128(i01); naDeci128(i02);}
//  t = printAndSwapTime("naGreaterEquali128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleri128(i01, i02); naInci128(i01); naDeci128(i02);}
//  t = printAndSwapTime("naSmalleri128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEquali128(i01, i02); naInci128(i01); naDeci128(i02);}
//  t = printAndSwapTime("naSmallerEquali128", t);
//
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualu128(ui01, ui02); naIncu128(ui01); naDecu128(ui02);}
//  t = printAndSwapTime("naEqualu128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateru128(ui01, ui02); naIncu128(ui01); naDecu128(ui02);}
//  t = printAndSwapTime("naGreateru128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualu128(ui01, ui02); naIncu128(ui01); naDecu128(ui02);}
//  t = printAndSwapTime("naGreaterEqualu128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleru128(ui01, ui02); naIncu128(ui01); naDecu128(ui02);}
//  t = printAndSwapTime("naSmalleru128", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualu128(ui01, ui02); naIncu128(ui01); naDecu128(ui02);}
//  t = printAndSwapTime("naSmallerEqualu128", t);
//
//  NA_UNUSED(i01);
//  NA_UNUSED(i02);
//  NA_UNUSED(ui01);
//  NA_UNUSED(ui02);
//  NA_UNUSED(t);
//}
//
//
//
//void testArithmetic(){
//  int128 i01 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  int128 i02 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
//  int128 i03 =   NA_ZERO_i128;
//  int128 i04 =   naMakei128WithDouble(-123.);
//  int128 i1, i2;
//  uint128 ui01 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  uint128 ui02 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
//  uint128 ui03 = NA_ZERO_u128;
//  uint128 ui2;
//
//  i1 = naNegi128(i01);
//  testEquali128(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53f);
//  i1 = naNegi128(naNegi128(i01));
//  testEquali128(i1, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  naInci128(i01);
//  testEquali128(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac2);
//  naDeci128(i01);
//  testEquali128(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  naInci128(i03);
//  testEquali128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
//  naDeci128(i03);
//  testEquali128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//  naDeci128(i03);
//  testEquali128(i03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
//  naInci128(i03);
//  testEquali128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//
//  i2 = naAddi128(i01, i02);
//  testEquali128(i2, 0xfb617c69, 0x5c5a548c, 0x92f837d5, 0x67836ab7);
//  i2 = naSubi128(i01, i02);
//  testEquali128(i2, 0xf7036f50, 0x841118cc, 0x663c728b, 0x17f4aacb);
//  i2 = naMuli128(i01, i04);
//  testEquali128(i2, 0x44c15ed8, 0x96313b20, 0x21d911d1, 0x60a6d545);
//  i2 = naMuli128(i02, i04);
//  testEquali128(i2, 0xf367da88, 0x0a682555, 0x40e41aa5, 0xe334e4ce);
//  i2 = naDivi128(i01, i02);
//  testEquali128(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd);
//  i2 = naDivi128(i01, i04);
//  testEquali128(i2, 0x000e28aa, 0xf3a3fc6e, 0xfce7d9d8, 0x1021da72);
//  i2 = naDivi128(i04, i01);
//  testEquali128(i2, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//  i2 = naModi128(i01, i02);
//  testEquali128(i2, 0xffbf8982, 0x34a3904c, 0xbfb3fd1f, 0xb7122aa3);
//  i2 = naModi128(i01, i04);
//  testEquali128(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff87);
//
//  naIncu128(ui01);
//  testEqualu128(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29be);
//  naDecu128(ui01);
//  testEqualu128(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  naIncu128(ui03);
//  testEqualu128(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
//  naDecu128(ui03);
//  testEqualu128(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//  naDecu128(ui03);
//  testEqualu128(ui03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
//  naIncu128(ui03);
//  testEqualu128(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//
//  ui2 = naAddu128(ui01, ui02);
//  testEqualu128(ui2, 0xff92302a, 0xf6cf6ad0, 0xbc950ece, 0x9866337e);
//  ui2 = naSubu128(ui01, ui02);
//  testEqualu128(ui2, 0xff2d4471, 0x1664f377, 0xc360626e, 0x18ee1ffc);
//  ui2 = naDivu128(ui01, ui02);
//  testEqualu128(ui2, 0x00000000, 0x00000000, 0x00000000, 0x0000050f);
//  ui2 = naModu128(ui01, ui02);
//  testEqualu128(ui2, 0x001d81aa, 0xe9515199, 0xef40ba8b, 0xf074d26e);
//
//}
//
//
//
//void timeArithmetic(){
//  double t;
//  int i;
//  int128 i128_1 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
//  int128 i128_2 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
//  uint128 u128_1 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
//  uint128 u128_2 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i128_1 = naNegi128(i128_1);}
//  t = printAndSwapTime("naNegi128", t);
//  for(i = 0; i < TESTSIZE; i++){naInci128(i128_1);}
//  t = printAndSwapTime("naInci128", t);
//  for(i = 0; i < TESTSIZE; i++){naDeci128(i128_1);}
//  t = printAndSwapTime("naDeci128", t);
//  for(i = 0; i < TESTSIZE; i++){i128_1 = naAddi128(i128_1, i128_2);}
//  t = printAndSwapTime("naAddi128", t);
//  for(i = 0; i < TESTSIZE; i++){i128_2 = naSubi128(i128_1, i128_2);}
//  t = printAndSwapTime("naSubi128", t);
//  for(i = 0; i < TESTSIZE; i++){i128_1 = naMuli128(i128_1, i128_2);}
//  t = printAndSwapTime("naMuli128", t);
//  for(i = 0; i < TESTSIZE; i++){i128_1 = naDivi128(i128_1, i128_2);}
//  t = printAndSwapTime("naDivi128", t);
//  for(i = 0; i < TESTSIZE; i++){i128_1 = naModi128(i128_1, i128_2);}
//  t = printAndSwapTime("naModi128", t);
//
//  for(i = 0; i < TESTSIZE; i++){naIncu128(u128_1);}
//  t = printAndSwapTime("naIncu128", t);
//  for(i = 0; i < TESTSIZE; i++){naDecu128(u128_1);}
//  t = printAndSwapTime("naDecu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128_1 = naAddu128(u128_1, u128_2);}
//  t = printAndSwapTime("naAddu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128_1 = naSubu128(u128_1, u128_2);}
//  t = printAndSwapTime("naSubu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128_2 = naMulu128(u128_1, u128_2);}
//  t = printAndSwapTime("naMulu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128_1 = naDivu128(u128_1, u128_2);}
//  t = printAndSwapTime("naDivu128", t);
//  for(i = 0; i < TESTSIZE; i++){u128_1 = naModu128(u128_1, u128_2);}
//  t = printAndSwapTime("naModu128", t);
//
//  NA_UNUSED(i128_1);
//  NA_UNUSED(i128_2);
//  NA_UNUSED(u128_1);
//  NA_UNUSED(u128_2);
//  NA_UNUSED(t);
//}
//
//
//uint32 randu32(){
//  return (uint32)rand();
//}
//NAu64 randu64(){
//  return naMakeu64(randu32(), randu32());
//}
//NAu128 randu128(){
//  return naMakeu128(randu64(), randu64());
//}
//double getTime(){
//  // Note: Reimplemented here because NADateTime uses int64 to compute.
//  #if NA_OS == NA_OS_WINDOWS
//    NADateTime dt;
//    FILETIME filetime;
//    NATimeZone timezone;
//    GetSystemTimeAsFileTime(&filetime);
//    GetTimeZoneInformation(&timezone);
//    dt = naMakeDateTimeFromFileTime(&filetime, &timezone);
//    return naCasti64Toi32(dt.sisec) + dt.nsec / 1000000000.;
//  #else
//    struct timeval curtime;
//    NATimeZone curtimezone;
//    gettimeofday(&curtime, &curtimezone);
//    return curtime.tv_sec + curtime.tv_usec / 1000000.;
//  #endif
//}
//double printAndSwapTime(const char* title, double starttime){
//  double t2 = getTime();
//  printf("%s" NA_TAB NA_TAB "%f s" NA_NL, title, t2 - starttime);
//  return t2;
//}
//void printi128(int128 i){
//  printu128(naCasti128Tou128(i));
//}
//void printu128(uint128 ui){
//  #if !defined NA_TYPE_INT128
//    #if !defined NA_TYPE_INT64
//      printf("0x%08x 0x%08x 0x%08x 0x%08x", ui.hi.hi, ui.hi.lo, ui.lo.hi, ui.lo.lo);
//    #else
//      printf("0x%08x 0x%08x 0x%08x 0x%08x", (uint32)(ui.hi >> 32), (uint32)ui.hi, (uint32)(ui.lo >> 32), (uint32)ui.lo);
//    #endif
//  #else
//    printf("0x%08x 0x%08x 0x%08x 0x%08x", (uint32)(ui >> 96), (uint32)(ui >> 64), (uint32)(ui >> 32), (uint32)ui);
//  #endif
//}
//void testEquali128(int128 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3){
//  testEqualu128(naCasti128Tou128(i), goalhi, goallo1, goallo2, goallo3);
//}
//void testEqualu128(uint128 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3){
//  #if !defined NA_TYPE_INT128
//    if(naEqualu64(ui.lo, naMakeu64(goallo2, goallo3))
//      && naEqualu64(ui.hi, naMakeu64(goalhi, goallo1))){
//      printf("ok   ");
//    }else{
//      printf("FAIL ");
//    }
//  #else
//    if((ui & NA_MAX_u32) == goallo3
//      && ((ui >> 32) & NA_MAX_u32) == goallo2
//      && ((ui >> 64) & NA_MAX_u32) == goallo1
//      && ((ui >> 96) & NA_MAX_u32) == goalhi){
//      printf("ok   ");
//    }else{
//      printf("FAIL ");
//    }
//  #endif
//  printu128(ui);
//  printf(NA_NL);
//}
//void testEqualBool(NABool b, NABool goalb){
//  if(b == goalb){
//    printf("ok  " NA_NL);
//  }else{
//    printf("FAIL" NA_NL);
//  }
//}
//
//
//
//int main(void){
//  #if !defined NA_TYPE_INT128
//    printf("Emulation" NA_NL);
//  #else
//    printf("Native" NA_NL);
//  #endif
//
//  testMaking();
//  testBinary();
//  testComparison();
//  testArithmetic();
//
//  timeMaking();
//  timeBinary();
//  timeComparison();
//  timeArithmetic();
//
//  #if NA_OS == NA_OS_WINDOWS
//    printf("Finished." NA_NL);
//    NA_UNUSED(getchar());
//  #endif
//
//  return 0;
//}
//
//
//
//// This is free and unencumbered software released into the public domain.
//
//// Anyone is free to copy, modify, publish, use, compile, sell, or
//// distribute this software, either in source code form or as a compiled
//// binary, for any purpose, commercial or non-commercial, and by any
//// means.
//
//// In jurisdictions that recognize copyright laws, the author or authors
//// of this software dedicate any and all copyright interest in the
//// software to the public domain. We make this dedication for the benefit
//// of the public at large and to the detriment of our heirs and
//// successors. We intend this dedication to be an overt act of
//// relinquishment in perpetuity of all present and future rights to this
//// software under copyright law.
//
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//// OTHER DEALINGS IN THE SOFTWARE.
//
//// For more information, please refer to <http://unlicense.org/>
