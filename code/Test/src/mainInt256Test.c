//
//#include NA_TEST_NALIB_PATH(NABase.h)
//#include NA_TEST_NALIB_PATH(NADateTime.h)
//#include <stdio.h>
//#include <time.h>
//#if NA_OS != NA_OS_WINDOWS
//  #include <sys/time.h>
//#endif
//
//uint128 randu128(void);
//uint256 randu256(void);
//double getTime(void);
//double printAndSwapTime(const char* title, double starttime);
//void printi256(int256 i);
//void printu256(uint256 ui);
//void testEquali256(int256 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7);
//void testEqualu256(uint256 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7);
//void testEqualBool(NABool b, NABool goalb);
//
//#define MAKEi256(a,b,c,d,e,f,g,h) naMakei256(\
//  naMakei128(naMakei64((int32)a, b), naMakeu64(c, d)),\
//  naMakeu128(naMakeu64(e, f), naMakeu64(g, h)));
//#define MAKEu256(a,b,c,d,e,f,g,h) naMakeu256(\
//  naMakeu128(naMakeu64(a, b), naMakeu64(c, d)),\
//  naMakeu128(naMakeu64(e, f), naMakeu64(g, h)));
//
//
//#define TESTSIZE 100000
//
//
//
//
//
//void testMaking(){
//  int256 i1 = naMakei256WithLo(naMakei128WithLo(naNegi64(naMakei64(0x12345678, 0x12345678))));
//  int256 i2 = naMakei256WithDouble(-123456789012345.);
//  uint256 ui1 = naMakeu256WithLo(naMakeu128WithLo(naMakeu64WithLo(0x12345678)));
//  uint256 ui2 = naMakeu256WithLo(naMakeu128WithLo(naMakeu64(0x12345678, 0x12345678)));
//  uint256 ui3 = naMakeu256WithLo(naMakeu128WithDouble(123456789012345.));
//  uint256 ui4 = MAKEu256(0x12345678, 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488);
//  testEquali256(i1, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xedcba987, 0xedcba988);
//  testEquali256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff8fb7, 0x79f22087);
//  testEqualu256(ui1, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x12345678);
//  testEqualu256(ui2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x12345678, 0x12345678);
//  testEqualu256(ui3, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00007048 , 0x860ddf79);
//  testEqualu256(ui4, 0x12345678 , 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488);
//}
//
//
//
//void timeMaking(){
//  double t;
//  int i;
//  int256 i256 = NA_ZERO_i256;
//  uint256 u256 = NA_ZERO_u256;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i256 = naAddi256(naMakei256WithLo(naNegi128(naCastu128Toi128(randu128()))), i256);}
//  t = printAndSwapTime("naMakei256WithLo", t);
//  for(i = 0; i < TESTSIZE; i++){i256 = naAddi256(naMakei256WithDouble(-(double)rand() * (double)rand()), i256);}
//  t = printAndSwapTime("naMakei256WithDouble", t);
//
//  for(i = 0; i < TESTSIZE; i++){u256 = naAddu256(naMakeu256WithLo(randu128()), u256);}
//  t = printAndSwapTime("naMakeu256WithLo", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naAddu256(naMakeu256WithDouble((double)rand() * (double)rand()), u256);}
//  t = printAndSwapTime("naMakeu256WithDouble", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naAddu256(naMakeu256(randu128(), randu128()), u256);}
//  t = printAndSwapTime("naMakeu256", t);
//
//  NA_UNUSED(i256);
//  NA_UNUSED(u256);
//  NA_UNUSED(t);
//}
//
//
//
//void testBinary(){
//  int256 i =      MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  int256 term =   MAKEi256(0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555);
//  uint256 ui =    MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  uint256 uterm = MAKEu256(0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555);
//
//  int256 i1 = naNoti256(i);
//  int256 i2 = naOri256(i, term);
//  int256 i3 = naAndi256(i, term);
//  int256 i4 = naXori256(i, term);
//  int256 i5 = naShli256(i, 5);
//  int256 i6 = naShri256(i, 5);
//
//  uint256 ui1 = naNotu256(ui);
//  uint256 ui2 = naOru256(ui, uterm);
//  uint256 ui3 = naAndu256(ui, uterm);
//  uint256 ui4 = naXoru256(ui, uterm);
//  uint256 ui5 = naShlu256(ui, 5);
//  uint256 ui6 = naShru256(ui, 5);
//
//  testEquali256(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e, 0x075abe44, 0x9dd35f9a, 0xcf152bac, 0x4b700dae);
//  testEquali256(i2, 0xfd7775dd, 0xf575f7fd, 0x7ddf5575, 0x7ffd5fd5, 0xfdf555ff, 0x777df575, 0x75ffd557, 0xf5dff755);
//  testEquali256(i3, 0x51105554, 0x50151404, 0x54105510, 0x15140041, 0x50054111, 0x40040045, 0x10405451, 0x14055051);
//  testEquali256(i4, 0xac672089, 0xa560e3f9, 0x29cf0065, 0x6ae95f94, 0xadf014ee, 0x3779f530, 0x65bf8106, 0xe1daa704);
//  testEquali256(i5, 0x264ebb9e, 0x06b6d58f, 0x934aa607, 0xf781583f, 0x14a8376c, 0x45940ca6, 0x1d5a8a76, 0x91fe4a20);
//  testEquali256(i6, 0xffc993ae, 0xe781adb5, 0x63e4d2a9, 0x81fde056, 0x0fc52a0d, 0xdb116503, 0x298756a2, 0x9da47f92);
//  testEqualu256(ui1, 0x00a045b1, 0xf965d0db, 0xc0054761, 0xa755d642, 0x78c453db, 0x9b85b016, 0xd6954100, 0xe9b72434);
//  testEqualu256(ui2, 0xff5fff5f, 0x57df7f75, 0x7ffffddf, 0x5dff7dfd, 0xd77ffd75, 0x757f5ffd, 0x7d7fffff, 0x575ddfdf);
//  testEqualu256(ui3, 0x55551044, 0x04100504, 0x15501014, 0x50000115, 0x05110404, 0x44504541, 0x01401455, 0x14405141);
//  testEqualu256(ui4, 0xaa0aef1b, 0x53cf7a71, 0x6aafedcb, 0x0dff7ce8, 0xd26ef971, 0x312f1abc, 0x7c3febaa, 0x431d8e9e);
//  testEqualu256(ui5, 0xebf749c0, 0xd345e487, 0xff5713cb, 0x154537b0, 0xe775848c, 0x8f49fd25, 0x2d57dfe2, 0xc91b7960);
//  testEqualu256(ui6, 0x07fafdd2, 0x7034d179, 0x21ffd5c4, 0xf2c5514d, 0xec39dd61, 0x2323d27f, 0x494b55f7, 0xf8b246de);
//}
//
//
//
//void timeBinary(){
//  int256 i256_1 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  int256 i256_2 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
//  uint256 u256_1 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  uint256 u256_2 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
//  double t;
//  int i;
//  int256 i256;
//  uint256 u256;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i256 = naNoti256(i256_1);}
//  t = printAndSwapTime("naNoti256", t);
//  for(i = 0; i < TESTSIZE; i++){i256 = naOri256(i256_1, i256_2);}
//  t = printAndSwapTime("naOri256", t);
//  for(i = 0; i < TESTSIZE; i++){i256 = naAndi256(i256_1, i256_2);}
//  t = printAndSwapTime("naAndi256", t);
//  for(i = 0; i < TESTSIZE; i++){i256 = naXori256(i256_1, i256_2);}
//  t = printAndSwapTime("naXori256", t);
//  for(i = 0; i < TESTSIZE; i++){i256 = naShli256(i256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShli256", t);
//  for(i = 0; i < TESTSIZE; i++){i256 = naShri256(i256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShri256", t);
//
//  for(i = 0; i < TESTSIZE; i++){u256 = naNotu256(u256_1);}
//  t = printAndSwapTime("naNotu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naOru256(u256_1, u256_2);}
//  t = printAndSwapTime("naOru256", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naAndu256(u256_1, u256_2);}
//  t = printAndSwapTime("naAndu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naXoru256(u256_1, u256_2);}
//  t = printAndSwapTime("naXoru256", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naShlu256(u256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShlu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256 = naShru256(u256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShru256", t);
//
//  NA_UNUSED(i256);
//  NA_UNUSED(u256);
//  NA_UNUSED(t);
//}
//
//
//
//void testComparison(){
//  int256 i1 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  int256 i2 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
//  uint256 ui1 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  uint256 ui2 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
//
//  testEqualBool(naEquali256(i1, i1), NA_TRUE);
//  testEqualBool(naGreateri256(i1, i2), NA_FALSE);
//  testEqualBool(naGreateri256(i2, i1), NA_TRUE);
//  testEqualBool(naGreateri256(i1, i1), NA_FALSE);
//  testEqualBool(naGreaterEquali256(i1, i2), NA_FALSE);
//  testEqualBool(naGreaterEquali256(i2, i1), NA_TRUE);
//  testEqualBool(naGreaterEquali256(i1, i1), NA_TRUE);
//  testEqualBool(naSmalleri256(i1, i2), NA_TRUE);
//  testEqualBool(naSmalleri256(i2, i1), NA_FALSE);
//  testEqualBool(naSmalleri256(i1, i1), NA_FALSE);
//  testEqualBool(naSmallerEquali256(i1, i2), NA_TRUE);
//  testEqualBool(naSmallerEquali256(i2, i1), NA_FALSE);
//  testEqualBool(naSmallerEquali256(i1, i1), NA_TRUE);
//
//  testEqualBool(naEqualu256(ui1, ui1), NA_TRUE);
//  testEqualBool(naGreateru256(ui1, ui2), NA_TRUE);
//  testEqualBool(naGreateru256(ui2, ui1), NA_FALSE);
//  testEqualBool(naGreateru256(ui1, ui1), NA_FALSE);
//  testEqualBool(naGreaterEqualu256(ui1, ui2), NA_TRUE);
//  testEqualBool(naGreaterEqualu256(ui2, ui1), NA_FALSE);
//  testEqualBool(naGreaterEqualu256(ui1, ui1), NA_TRUE);
//  testEqualBool(naSmalleru256(ui1, ui2), NA_FALSE);
//  testEqualBool(naSmalleru256(ui2, ui1), NA_TRUE);
//  testEqualBool(naSmalleru256(ui1, ui1), NA_FALSE);
//  testEqualBool(naSmallerEqualu256(ui1, ui2), NA_FALSE);
//  testEqualBool(naSmallerEqualu256(ui2, ui1), NA_TRUE);
//  testEqualBool(naSmallerEqualu256(ui1, ui1), NA_TRUE);
//}
//
//
//
//void timeComparison(){
//  int256 i01 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  int256 i02 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
//  uint256 ui01 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  uint256 ui02 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
//  double t;
//  int i;
//  NABool testbool = NA_FALSE;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naEquali256(i01, i02); naInci256(i01); naDeci256(i02);}
//  t = printAndSwapTime("naEquali256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateri256(i01, i02); naInci256(i01); naDeci256(i02);}
//  t = printAndSwapTime("naGreateri256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEquali256(i01, i02); naInci256(i01); naDeci256(i02);}
//  t = printAndSwapTime("naGreaterEquali256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleri256(i01, i02); naInci256(i01); naDeci256(i02);}
//  t = printAndSwapTime("naSmalleri256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEquali256(i01, i02); naInci256(i01); naDeci256(i02);}
//  t = printAndSwapTime("naSmallerEquali256", t);
//
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualu256(ui01, ui02); naIncu256(ui01); naDecu256(ui02);}
//  t = printAndSwapTime("naEqualu256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateru256(ui01, ui02); naIncu256(ui01); naDecu256(ui02);}
//  t = printAndSwapTime("naGreateru256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualu256(ui01, ui02); naIncu256(ui01); naDecu256(ui02);}
//  t = printAndSwapTime("naGreaterEqualu256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleru256(ui01, ui02); naIncu256(ui01); naDecu256(ui02);}
//  t = printAndSwapTime("naSmalleru256", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualu256(ui01, ui02); naIncu256(ui01); naDecu256(ui02);}
//  t = printAndSwapTime("naSmallerEqualu256", t);
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
//  int256 i01 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  int256 i02 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
//  int256 i03 =   NA_ZERO_i256;
//  int256 i04 =   naMakei256WithDouble(-123.);
//  int256 i1, i2;
//  uint256 ui01 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  uint256 ui02 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
//  uint256 ui03 = NA_ZERO_u256;
//  uint256 ui2;
//
//  i1 = naNegi256(i01);
//  testEquali256(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e, 0x075abe44, 0x9dd35f9a, 0xcf152bac, 0x4b700daf);
//  i1 = naNegi256(naNegi256(i01));
//  testEquali256(i1, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  naInci256(i01);
//  testEquali256(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff252);
//  naDeci256(i01);
//  testEquali256(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  naInci256(i03);
//  testEquali256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
//  naDeci256(i03);
//  testEquali256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//  naDeci256(i03);
//  testEquali256(i03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
//  naInci256(i03);
//  testEquali256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//
//  i2 = naAddi256(i01, i02);
//  testEquali256(i2, 0xfb617c69, 0x5c5a548c, 0x92f837d5, 0x67836ab8, 0xeaeb5cff, 0xf6811350, 0x243c3b89, 0x31739396);
//  i2 = naSubi256(i01, i02);
//  testEquali256(i2, 0xf7036f50, 0x841118cc, 0x663c728b, 0x17f4aacb, 0x065f2676, 0xcdd82d7a, 0x3d996d1e, 0x37ac510c);
//  i2 = naMuli256(i01, i04);
//  testEquali256(i2, 0x44c15ed8, 0x96313b20, 0x21d911d1, 0x60a6d4cd, 0x88996af7, 0xd48eef61, 0x7f2bfbc8, 0x3ed69315);
//  i2 = naMuli256(i02, i04);
//  testEquali256(i2, 0xf367da88, 0x0a682555, 0x40e41aa5, 0xe334e459, 0x9850e60c, 0xbb6cc91d, 0x17e3694c, 0xfea183d9);
//  i2 = naDivi256(i01, i02);
//  testEquali256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd);
//  i2 = naDivi256(i01, i04);
//  testEquali256(i2, 0x000e28aa, 0xf3a3fc6e, 0xfce7d9d8, 0x1021da72, 0xf9d0de2a, 0x2f125e6f, 0x914b1954, 0xa71e0c99);
//  i2 = naDivi256(i04, i01);
//  testEquali256(i2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//  i2 = naModi256(i01, i02);
//  testEquali256(i2, 0xffbf8982, 0x34a3904c, 0xbfb3fd1f, 0xb7122aa6, 0xcf779389, 0x1f29f926, 0x0adf09f4, 0x2b3ad620);
//  i2 = naModi256(i01, i04);
//  testEquali256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffd4);
//
//  naIncu256(ui01);
//  testEqualu256(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcc);
//  naDecu256(ui01);
//  testEqualu256(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  naIncu256(ui03);
//  testEqualu256(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
//  naDecu256(ui03);
//  testEqualu256(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//  naDecu256(ui03);
//  testEqualu256(ui03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
//  naIncu256(ui03);
//  testEqualu256(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
//
//  ui2 = naAddu256(ui01, ui02);
//  testEqualu256(ui2, 0xff92302a, 0xf6cf6ad0, 0xbc950ece, 0x9866337f, 0x1077be3a, 0xd5c01e98, 0xd3be87ee, 0x8903c672);
//  ui2 = naSubu256(ui01, ui02);
//  testEqualu256(ui2, 0xff2d4471, 0x1664f377, 0xc360626e, 0x18ee1ffb, 0xfdff9a0d, 0xf3348139, 0x7f16f60f, 0xa38df124);
//  ui2 = naDivu256(ui01, ui02);
//  testEqualu256(ui2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000050f);
//  ui2 = naModu256(ui01, ui02);
//  testEqualu256(ui2, 0x001d81aa, 0xe9515199, 0xef40ba8b, 0xf074cfb8, 0x505c2c9d, 0x6459c54a, 0x8b954bb9, 0xb6bfd902);
//
//}
//
//
//
//void timeArithmetic(){
//  double t;
//  int i;
//  int256 i256_1 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
//  int256 i256_2 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
//  uint256 u256_1 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
//  uint256 u256_2 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i256_1 = naNegi256(i256_1);}
//  t = printAndSwapTime("naNegi256", t);
//  for(i = 0; i < TESTSIZE; i++){naInci256(i256_1);}
//  t = printAndSwapTime("naInci256", t);
//  for(i = 0; i < TESTSIZE; i++){naDeci256(i256_1);}
//  t = printAndSwapTime("naDeci256", t);
//  for(i = 0; i < TESTSIZE; i++){i256_1 = naAddi256(i256_1, i256_2);}
//  t = printAndSwapTime("naAddi256", t);
//  for(i = 0; i < TESTSIZE; i++){i256_2 = naSubi256(i256_1, i256_2);}
//  t = printAndSwapTime("naSubi256", t);
//  for(i = 0; i < TESTSIZE; i++){i256_1 = naMuli256(i256_1, i256_2);}
//  t = printAndSwapTime("naMuli256", t);
//  for(i = 0; i < TESTSIZE; i++){i256_1 = naDivi256(i256_1, i256_2);}
//  t = printAndSwapTime("naDivi256", t);
//  for(i = 0; i < TESTSIZE; i++){i256_1 = naModi256(i256_1, i256_2);}
//  t = printAndSwapTime("naModi256", t);
//
//  for(i = 0; i < TESTSIZE; i++){naIncu256(u256_1);}
//  t = printAndSwapTime("naIncu256", t);
//  for(i = 0; i < TESTSIZE; i++){naDecu256(u256_1);}
//  t = printAndSwapTime("naDecu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256_1 = naAddu256(u256_1, u256_2);}
//  t = printAndSwapTime("naAddu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256_1 = naSubu256(u256_1, u256_2);}
//  t = printAndSwapTime("naSubu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256_2 = naMulu256(u256_1, u256_2);}
//  t = printAndSwapTime("naMulu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256_1 = naDivu256(u256_1, u256_2);}
//  t = printAndSwapTime("naDivu256", t);
//  for(i = 0; i < TESTSIZE; i++){u256_1 = naModu256(u256_1, u256_2);}
//  t = printAndSwapTime("naModu256", t);
//
//  NA_UNUSED(i256_1);
//  NA_UNUSED(i256_2);
//  NA_UNUSED(u256_1);
//  NA_UNUSED(u256_2);
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
//NAu256 randu256(){
//  return naMakeu256(randu128(), randu128());
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
//void printi256(int256 i){
//  printu256(naCasti256Tou256(i));
//}
//void printu256(uint256 ui){
//  #if !defined NA_TYPE_INT256
//    #if !defined NA_TYPE_INT128
//      #if !defined NA_TYPE_INT64
//        printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
//          ui.hi.hi.hi, ui.hi.hi.lo, ui.hi.lo.hi, ui.hi.lo.lo,
//          ui.lo.hi.hi, ui.lo.hi.lo, ui.lo.lo.hi, ui.lo.lo.lo);
//      #else
//        printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
//          (uint32)(ui.hi.hi >> 32), (uint32)ui.hi.hi, (uint32)(ui.hi.lo >> 32), (uint32)ui.hi.lo,
//          (uint32)(ui.lo.hi >> 32), (uint32)ui.lo.hi, (uint32)(ui.lo.lo >> 32), (uint32)ui.lo.lo);
//      #endif
//    #else
//      printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
//        (uint32)(ui.hi >> 96), (uint32)(ui.hi >> 64), (uint32)(ui.hi >> 32), (uint32)ui.hi,
//        (uint32)(ui.lo >> 96), (uint32)(ui.lo >> 64), (uint32)(ui.lo >> 32), (uint32)ui.lo);
//    #endif
//  #else
//    printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
//      (uint32)(ui >> 224), (uint32)(ui >> 192), (uint32)(ui >> 160), (uint32)(ui >> 128),
//      (uint32)(ui >> 96), (uint32)(ui >> 64), (uint32)(ui >> 32), (uint32)ui);
//  #endif
//}
//void testEquali256(int256 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7){
//  testEqualu256(naCasti256Tou256(i), goalhi, goallo1, goallo2, goallo3, goallo4, goallo5, goallo6, goallo7);
//}
//void testEqualu256(uint256 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7){
//  #if !defined NA_TYPE_INT256
//    if(naEqualu128(ui.lo, naMakeu128(naMakeu64(goallo4, goallo5), naMakeu64(goallo6, goallo7)))
//      && naEqualu128(ui.hi, naMakeu128(naMakeu64(goalhi, goallo1), naMakeu64(goallo2, goallo3)))){
//      printf("ok   ");
//    }else{
//      printf("FAIL ");
//    }
//  #else
//    if((ui & NA_MAX_u32) == goallo3
//      && ((ui >> 32) & NA_MAX_u32) == goallo2
//      && ((ui >> 128) & NA_MAX_u32) == goallo1
//      && ((ui >> 96) & NA_MAX_u32) == goalhi){
//      printf("ok   ");
//    }else{
//      printf("FAIL ");
//    }
//  #endif
//  printu256(ui);
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
//  #if !defined NA_TYPE_INT256
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
