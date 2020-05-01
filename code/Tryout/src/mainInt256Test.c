
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// To be able to include header files from NALib, you need to either add the
// src directory of NALib as an additional project source path, or you can
// help yourself using the following two lines. Just provide the correct
// relative path to the root directory of the NALib source:

#define TRYOUT_STRINGIFY(A) #A
#define TRYOUT_NALIB_PATH(file) TRYOUT_STRINGIFY(../../NALib/src/file)

// This allows you to reference any NALib file using for example this:
// #include TRYOUT_NALIB_PATH(NABase.h)


#include TRYOUT_NALIB_PATH(NABase.h)
#include TRYOUT_NALIB_PATH(NADateTime.h)
#include <stdio.h>
#include <time.h>
#if NA_OS != NA_OS_WINDOWS
  #include <sys/time.h>
#endif

uint128 randUInt128();
uint256 randUInt256();
double getTime();
double printAndSwapTime(const char* title, double starttime);
void printInt256(int256 i);
void printUInt256(uint256 ui);
void testEqualInt256(int256 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7);
void testEqualUInt256(uint256 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7);
void testEqualBool(NABool b, NABool goalb);

#define MAKEi256(a,b,c,d,e,f,g,h) naMakeInt256(\
  naMakeInt128(naMakeInt64((int32)a, b), naMakeUInt64(c, d)),\
  naMakeUInt128(naMakeUInt64(e, f), naMakeUInt64(g, h)));
#define MAKEu256(a,b,c,d,e,f,g,h) naMakeUInt256(\
  naMakeUInt128(naMakeUInt64(a, b), naMakeUInt64(c, d)),\
  naMakeUInt128(naMakeUInt64(e, f), naMakeUInt64(g, h)));


#define TESTSIZE 100000





void testMaking(){
  int256 i1 = naMakeInt256WithLo(naMakeInt128WithLo(naNegInt64(naMakeInt64(0x12345678, 0x12345678))));
  int256 i2 = naMakeInt256WithDouble(-123456789012345.);
  uint256 ui1 = naMakeUInt256WithLo(naMakeUInt128WithLo(naMakeUInt64WithLo(0x12345678)));
  uint256 ui2 = naMakeUInt256WithLo(naMakeUInt128WithLo(naMakeUInt64(0x12345678, 0x12345678)));
  uint256 ui3 = naMakeUInt256WithLo(naMakeUInt128WithDouble(123456789012345.));
  uint256 ui4 = MAKEu256(0x12345678, 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488);
  testEqualInt256(i1, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xedcba987, 0xedcba988);
  testEqualInt256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff8fb7, 0x79f22087);
  testEqualUInt256(ui1, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x12345678);
  testEqualUInt256(ui2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x12345678, 0x12345678);
  testEqualUInt256(ui3, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00007048 , 0x860ddf79);
  testEqualUInt256(ui4, 0x12345678 , 0x56473829, 0xfabcedae, 0x33333333, 0xfedcba98, 0xabcabcde, 0x54345676, 0x11224488);
}



void timeMaking(){
  double t;
  int i;
  int256 i256 = NA_ZERO_256;
  uint256 ui256 = NA_ZERO_256u;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i256 = naAddInt256(naMakeInt256WithLo(naNegInt128(naCastUInt128ToInt128(randUInt128()))), i256);}
  t = printAndSwapTime("naMakeInt256WithLo", t);
  for(i = 0; i < TESTSIZE; i++){i256 = naAddInt256(naMakeInt256WithDouble(-(double)rand() * (double)rand()), i256);}
  t = printAndSwapTime("naMakeInt256WithDouble", t);

  for(i = 0; i < TESTSIZE; i++){ui256 = naAddUInt256(naMakeUInt256WithLo(randUInt128()), ui256);}
  t = printAndSwapTime("naMakeUInt256WithLo", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naAddUInt256(naMakeUInt256WithDouble((double)rand() * (double)rand()), ui256);}
  t = printAndSwapTime("naMakeUInt256WithDouble", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naAddUInt256(naMakeUInt256(randUInt128(), randUInt128()), ui256);}
  t = printAndSwapTime("naMakeUInt256", t);

  NA_UNUSED(i256);
  NA_UNUSED(ui256);
  NA_UNUSED(t);
}



void testBinary(){
  int256 i =      MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  int256 term =   MAKEi256(0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555);
  uint256 ui =    MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  uint256 uterm = MAKEu256(0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0x55555555);

  int256 i1 = naNotInt256(i);
  int256 i2 = naOrInt256(i, term);
  int256 i3 = naAndInt256(i, term);
  int256 i4 = naXorInt256(i, term);
  int256 i5 = naShlInt256(i, 5);
  int256 i6 = naShrInt256(i, 5);

  uint256 ui1 = naNotUInt256(ui);
  uint256 ui2 = naOrUInt256(ui, uterm);
  uint256 ui3 = naAndUInt256(ui, uterm);
  uint256 ui4 = naXorUInt256(ui, uterm);
  uint256 ui5 = naShlUInt256(ui, 5);
  uint256 ui6 = naShrUInt256(ui, 5);

  testEqualInt256(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e, 0x075abe44, 0x9dd35f9a, 0xcf152bac, 0x4b700dae);
  testEqualInt256(i2, 0xfd7775dd, 0xf575f7fd, 0x7ddf5575, 0x7ffd5fd5, 0xfdf555ff, 0x777df575, 0x75ffd557, 0xf5dff755);
  testEqualInt256(i3, 0x51105554, 0x50151404, 0x54105510, 0x15140041, 0x50054111, 0x40040045, 0x10405451, 0x14055051);
  testEqualInt256(i4, 0xac672089, 0xa560e3f9, 0x29cf0065, 0x6ae95f94, 0xadf014ee, 0x3779f530, 0x65bf8106, 0xe1daa704);
  testEqualInt256(i5, 0x264ebb9e, 0x06b6d58f, 0x934aa607, 0xf781583f, 0x14a8376c, 0x45940ca6, 0x1d5a8a76, 0x91fe4a20);
  testEqualInt256(i6, 0xffc993ae, 0xe781adb5, 0x63e4d2a9, 0x81fde056, 0x0fc52a0d, 0xdb116503, 0x298756a2, 0x9da47f92);
  testEqualUInt256(ui1, 0x00a045b1, 0xf965d0db, 0xc0054761, 0xa755d642, 0x78c453db, 0x9b85b016, 0xd6954100, 0xe9b72434);
  testEqualUInt256(ui2, 0xff5fff5f, 0x57df7f75, 0x7ffffddf, 0x5dff7dfd, 0xd77ffd75, 0x757f5ffd, 0x7d7fffff, 0x575ddfdf);
  testEqualUInt256(ui3, 0x55551044, 0x04100504, 0x15501014, 0x50000115, 0x05110404, 0x44504541, 0x01401455, 0x14405141);
  testEqualUInt256(ui4, 0xaa0aef1b, 0x53cf7a71, 0x6aafedcb, 0x0dff7ce8, 0xd26ef971, 0x312f1abc, 0x7c3febaa, 0x431d8e9e);
  testEqualUInt256(ui5, 0xebf749c0, 0xd345e487, 0xff5713cb, 0x154537b0, 0xe775848c, 0x8f49fd25, 0x2d57dfe2, 0xc91b7960);
  testEqualUInt256(ui6, 0x07fafdd2, 0x7034d179, 0x21ffd5c4, 0xf2c5514d, 0xec39dd61, 0x2323d27f, 0x494b55f7, 0xf8b246de);
}



void timeBinary(){
  int256 i256_1 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  int256 i256_2 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  uint256 ui256_1 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  uint256 ui256_2 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
  double t;
  int i;
  int256 i256;
  uint256 ui256;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i256 = naNotInt256(i256_1);}
  t = printAndSwapTime("naNotInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256 = naOrInt256(i256_1, i256_2);}
  t = printAndSwapTime("naOrInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256 = naAndInt256(i256_1, i256_2);}
  t = printAndSwapTime("naAndInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256 = naXorInt256(i256_1, i256_2);}
  t = printAndSwapTime("naXorInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256 = naShlInt256(i256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShlInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256 = naShrInt256(i256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShrInt256", t);

  for(i = 0; i < TESTSIZE; i++){ui256 = naNotUInt256(ui256_1);}
  t = printAndSwapTime("naNotUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naOrUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naOrUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naAndUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naAndUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naXorUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naXorUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naShlUInt256(ui256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShlUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256 = naShrUInt256(ui256_1, (int)(128. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShrUInt256", t);

  NA_UNUSED(i256);
  NA_UNUSED(ui256);
  NA_UNUSED(t);
}



void testComparison(){
  int256 i1 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  int256 i2 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  uint256 ui1 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  uint256 ui2 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);

  testEqualBool(naEqualInt256(i1, i1), NA_TRUE);
  testEqualBool(naGreaterInt256(i1, i2), NA_FALSE);
  testEqualBool(naGreaterInt256(i2, i1), NA_TRUE);
  testEqualBool(naGreaterInt256(i1, i1), NA_FALSE);
  testEqualBool(naGreaterEqualInt256(i1, i2), NA_FALSE);
  testEqualBool(naGreaterEqualInt256(i2, i1), NA_TRUE);
  testEqualBool(naGreaterEqualInt256(i1, i1), NA_TRUE);
  testEqualBool(naSmallerInt256(i1, i2), NA_TRUE);
  testEqualBool(naSmallerInt256(i2, i1), NA_FALSE);
  testEqualBool(naSmallerInt256(i1, i1), NA_FALSE);
  testEqualBool(naSmallerEqualInt256(i1, i2), NA_TRUE);
  testEqualBool(naSmallerEqualInt256(i2, i1), NA_FALSE);
  testEqualBool(naSmallerEqualInt256(i1, i1), NA_TRUE);

  testEqualBool(naEqualUInt256(ui1, ui1), NA_TRUE);
  testEqualBool(naGreaterUInt256(ui1, ui2), NA_TRUE);
  testEqualBool(naGreaterUInt256(ui2, ui1), NA_FALSE);
  testEqualBool(naGreaterUInt256(ui1, ui1), NA_FALSE);
  testEqualBool(naGreaterEqualUInt256(ui1, ui2), NA_TRUE);
  testEqualBool(naGreaterEqualUInt256(ui2, ui1), NA_FALSE);
  testEqualBool(naGreaterEqualUInt256(ui1, ui1), NA_TRUE);
  testEqualBool(naSmallerUInt256(ui1, ui2), NA_FALSE);
  testEqualBool(naSmallerUInt256(ui2, ui1), NA_TRUE);
  testEqualBool(naSmallerUInt256(ui1, ui1), NA_FALSE);
  testEqualBool(naSmallerEqualUInt256(ui1, ui2), NA_FALSE);
  testEqualBool(naSmallerEqualUInt256(ui2, ui1), NA_TRUE);
  testEqualBool(naSmallerEqualUInt256(ui1, ui1), NA_TRUE);
}



void timeComparison(){
  int256 i01 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  int256 i02 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  uint256 ui01 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  uint256 ui02 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
  double t;
  int i;
  NABool testbool = NA_FALSE;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualInt256(i01, i02); naIncInt256(i01); naDecInt256(i02);}
  t = printAndSwapTime("naEqualInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterInt256(i01, i02); naIncInt256(i01); naDecInt256(i02);}
  t = printAndSwapTime("naGreaterInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualInt256(i01, i02); naIncInt256(i01); naDecInt256(i02);}
  t = printAndSwapTime("naGreaterEqualInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerInt256(i01, i02); naIncInt256(i01); naDecInt256(i02);}
  t = printAndSwapTime("naSmallerInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualInt256(i01, i02); naIncInt256(i01); naDecInt256(i02);}
  t = printAndSwapTime("naSmallerEqualInt256", t);

  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualUInt256(ui01, ui02); naIncUInt256(ui01); naDecUInt256(ui02);}
  t = printAndSwapTime("naEqualUInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterUInt256(ui01, ui02); naIncUInt256(ui01); naDecUInt256(ui02);}
  t = printAndSwapTime("naGreaterUInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualUInt256(ui01, ui02); naIncUInt256(ui01); naDecUInt256(ui02);}
  t = printAndSwapTime("naGreaterEqualUInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerUInt256(ui01, ui02); naIncUInt256(ui01); naDecUInt256(ui02);}
  t = printAndSwapTime("naSmallerUInt256", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualUInt256(ui01, ui02); naIncUInt256(ui01); naDecUInt256(ui02);}
  t = printAndSwapTime("naSmallerEqualUInt256", t);

  NA_UNUSED(i01);
  NA_UNUSED(i02);
  NA_UNUSED(ui01);
  NA_UNUSED(ui02);
  NA_UNUSED(t);
}



void testArithmetic(){
  int256 i01 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  int256 i02 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  int256 i03 =   NA_ZERO_256;
  int256 i04 =   naMakeInt256WithDouble(-123.);
  int256 i1, i2;
  uint256 ui01 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  uint256 ui02 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);
  uint256 ui03 = NA_ZERO_256u;
  uint256 ui2;

  i1 = naNegInt256(i01);
  testEqualInt256(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e, 0x075abe44, 0x9dd35f9a, 0xcf152bac, 0x4b700daf);
  i1 = naNegInt256(naNegInt256(i01));
  testEqualInt256(i1, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  naIncInt256(i01);
  testEqualInt256(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff252);
  naDecInt256(i01);
  testEqualInt256(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  naIncInt256(i03);
  testEqualInt256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
  naDecInt256(i03);
  testEqualInt256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
  naDecInt256(i03);
  testEqualInt256(i03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
  naIncInt256(i03);
  testEqualInt256(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);

  i2 = naAddInt256(i01, i02);
  testEqualInt256(i2, 0xfb617c69, 0x5c5a548c, 0x92f837d5, 0x67836ab8, 0xeaeb5cff, 0xf6811350, 0x243c3b89, 0x31739396);
  i2 = naSubInt256(i01, i02);
  testEqualInt256(i2, 0xf7036f50, 0x841118cc, 0x663c728b, 0x17f4aacb, 0x065f2676, 0xcdd82d7a, 0x3d996d1e, 0x37ac510c);
  i2 = naMulInt256(i01, i04);
  testEqualInt256(i2, 0x44c15ed8, 0x96313b20, 0x21d911d1, 0x60a6d4cd, 0x88996af7, 0xd48eef61, 0x7f2bfbc8, 0x3ed69315);
  i2 = naMulInt256(i02, i04);
  testEqualInt256(i2, 0xf367da88, 0x0a682555, 0x40e41aa5, 0xe334e459, 0x9850e60c, 0xbb6cc91d, 0x17e3694c, 0xfea183d9);
  i2 = naDivInt256(i01, i02);
  testEqualInt256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd);
  i2 = naDivInt256(i01, i04);
  testEqualInt256(i2, 0x000e28aa, 0xf3a3fc6e, 0xfce7d9d8, 0x1021da72, 0xf9d0de2a, 0x2f125e6f, 0x914b1954, 0xa71e0c99);
  i2 = naDivInt256(i04, i01);
  testEqualInt256(i2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
  i2 = naModInt256(i01, i02);
  testEqualInt256(i2, 0xffbf8982, 0x34a3904c, 0xbfb3fd1f, 0xb7122aa6, 0xcf779389, 0x1f29f926, 0x0adf09f4, 0x2b3ad620);
  i2 = naModInt256(i01, i04);
  testEqualInt256(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffd4);

  naIncUInt256(ui01);
  testEqualUInt256(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcc);
  naDecUInt256(ui01);
  testEqualUInt256(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  naIncUInt256(ui03);
  testEqualUInt256(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
  naDecUInt256(ui03);
  testEqualUInt256(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
  naDecUInt256(ui03);
  testEqualUInt256(ui03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
  naIncUInt256(ui03);
  testEqualUInt256(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);

  ui2 = naAddUInt256(ui01, ui02);
  testEqualUInt256(ui2, 0xff92302a, 0xf6cf6ad0, 0xbc950ece, 0x9866337f, 0x1077be3a, 0xd5c01e98, 0xd3be87ee, 0x8903c672);
  ui2 = naSubUInt256(ui01, ui02);
  testEqualUInt256(ui2, 0xff2d4471, 0x1664f377, 0xc360626e, 0x18ee1ffb, 0xfdff9a0d, 0xf3348139, 0x7f16f60f, 0xa38df124);
  ui2 = naDivUInt256(ui01, ui02);
  testEqualUInt256(ui2, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000050f);
  ui2 = naModUInt256(ui01, ui02);
  testEqualUInt256(ui2, 0x001d81aa, 0xe9515199, 0xef40ba8b, 0xf074cfb8, 0x505c2c9d, 0x6459c54a, 0x8b954bb9, 0xb6bfd902);

}



void timeArithmetic(){
  double t;
  int i;
  int256 i256_1 =   MAKEi256(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1, 0xf8a541bb, 0x622ca065, 0x30ead453, 0xb48ff251);
  int256 i256_2 =   MAKEi256(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6, 0xf2461b44, 0x945472ea, 0xf3516735, 0x7ce3a145);
  uint256 ui256_1 = MAKEu256(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd, 0x873bac24, 0x647a4fe9, 0x296abeff, 0x1648dbcb);
  uint256 ui256_2 = MAKEu256(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1, 0x893c1216, 0x7145ceaf, 0xaa53c8ef, 0x72baeaa7);

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i256_1 = naNegInt256(i256_1);}
  t = printAndSwapTime("naNegInt256", t);
  for(i = 0; i < TESTSIZE; i++){naIncInt256(i256_1);}
  t = printAndSwapTime("naIncInt256", t);
  for(i = 0; i < TESTSIZE; i++){naDecInt256(i256_1);}
  t = printAndSwapTime("naDecInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256_1 = naAddInt256(i256_1, i256_2);}
  t = printAndSwapTime("naAddInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256_2 = naSubInt256(i256_1, i256_2);}
  t = printAndSwapTime("naSubInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256_1 = naMulInt256(i256_1, i256_2);}
  t = printAndSwapTime("naMulInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256_1 = naDivInt256(i256_1, i256_2);}
  t = printAndSwapTime("naDivInt256", t);
  for(i = 0; i < TESTSIZE; i++){i256_1 = naModInt256(i256_1, i256_2);}
  t = printAndSwapTime("naModInt256", t);

  for(i = 0; i < TESTSIZE; i++){naIncUInt256(ui256_1);}
  t = printAndSwapTime("naIncUInt256", t);
  for(i = 0; i < TESTSIZE; i++){naDecUInt256(ui256_1);}
  t = printAndSwapTime("naDecUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256_1 = naAddUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naAddUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256_1 = naSubUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naSubUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256_2 = naMulUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naMulUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256_1 = naDivUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naDivUInt256", t);
  for(i = 0; i < TESTSIZE; i++){ui256_1 = naModUInt256(ui256_1, ui256_2);}
  t = printAndSwapTime("naModUInt256", t);

  NA_UNUSED(i256_1);
  NA_UNUSED(i256_2);
  NA_UNUSED(ui256_1);
  NA_UNUSED(ui256_2);
  NA_UNUSED(t);
}


uint32 randUInt32(){
  return (uint32)rand();
}
NAUInt64 randUInt64(){
  return naMakeUInt64(randUInt32(), randUInt32());
}
NAUInt128 randUInt128(){
  return naMakeUInt128(randUInt64(), randUInt64());
}
NAUInt256 randUInt256(){
  return naMakeUInt256(randUInt128(), randUInt128());
}
double getTime(){
  // Note: Reimplemented here because NADateTime uses int64 to compute.
  #if NA_OS == NA_OS_WINDOWS
    NADateTime dt;
    FILETIME filetime;
    NATimeZone timezone;
    GetSystemTimeAsFileTime(&filetime);
    GetTimeZoneInformation(&timezone);
    dt = naMakeDateTimeFromFileTime(&filetime, &timezone);
    return naCastInt64ToInt32(dt.sisec) + dt.nsec / 1000000000.;
  #else
    struct timeval curtime;
    NATimeZone curtimezone;
    gettimeofday(&curtime, &curtimezone);
    return curtime.tv_sec + curtime.tv_usec / 1000000.;
  #endif
}
double printAndSwapTime(const char* title, double starttime){
  double t2 = getTime();
  printf("%s" NA_TAB NA_TAB "%f s" NA_NL, title, t2 - starttime);
  return t2;
}
void printInt256(int256 i){
  printUInt256(naCastInt256ToUInt256(i));
}
void printUInt256(uint256 ui){
  #if !defined NA_TYPE_INT256
    #if !defined NA_TYPE_INT128
      #if !defined NA_TYPE_INT64
        printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
          ui.hi.hi.hi, ui.hi.hi.lo, ui.hi.lo.hi, ui.hi.lo.lo,
          ui.lo.hi.hi, ui.lo.hi.lo, ui.lo.lo.hi, ui.lo.lo.lo);
      #else
        printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
          (uint32)(ui.hi.hi >> 32), (uint32)ui.hi.hi, (uint32)(ui.hi.lo >> 32), (uint32)ui.hi.lo,
          (uint32)(ui.lo.hi >> 32), (uint32)ui.lo.hi, (uint32)(ui.lo.lo >> 32), (uint32)ui.lo.lo);
      #endif
    #else
      printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
        (uint32)(ui.hi >> 96), (uint32)(ui.hi >> 64), (uint32)(ui.hi >> 32), (uint32)ui.hi,
        (uint32)(ui.lo >> 96), (uint32)(ui.lo >> 64), (uint32)(ui.lo >> 32), (uint32)ui.lo);
    #endif
  #else
    printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
      (uint32)(ui >> 224), (uint32)(ui >> 192), (uint32)(ui >> 160), (uint32)(ui >> 128),
      (uint32)(ui >> 96), (uint32)(ui >> 64), (uint32)(ui >> 32), (uint32)ui);
  #endif
}
void testEqualInt256(int256 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7){
  testEqualUInt256(naCastInt256ToUInt256(i), goalhi, goallo1, goallo2, goallo3, goallo4, goallo5, goallo6, goallo7);
}
void testEqualUInt256(uint256 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3, uint32 goallo4, uint32 goallo5, uint32 goallo6, uint32 goallo7){
  #if !defined NA_TYPE_INT256
    if(naEqualUInt128(ui.lo, naMakeUInt128(naMakeUInt64(goallo4, goallo5), naMakeUInt64(goallo6, goallo7)))
      && naEqualUInt128(ui.hi, naMakeUInt128(naMakeUInt64(goalhi, goallo1), naMakeUInt64(goallo2, goallo3)))){
      printf("ok   ");
    }else{
      printf("FAIL ");
    }
  #else
    if((ui & NA_UINT32_MAX) == goallo3
      && ((ui >> 32) & NA_UINT32_MAX) == goallo2
      && ((ui >> 128) & NA_UINT32_MAX) == goallo1
      && ((ui >> 96) & NA_UINT32_MAX) == goalhi){
      printf("ok   ");
    }else{
      printf("FAIL ");
    }
  #endif
  printUInt256(ui);
  printf(NA_NL);
}
void testEqualBool(NABool b, NABool goalb){
  if(b == goalb){
    printf("ok  " NA_NL);
  }else{
    printf("FAIL" NA_NL);
  }
}



int main(void){
  #if !defined NA_TYPE_INT256
    printf("Emulation" NA_NL);
  #else
    printf("Native" NA_NL);
  #endif

  testMaking();
  testBinary();
  testComparison();
  testArithmetic();

  timeMaking();
  timeBinary();
  timeComparison();
  timeArithmetic();

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished." NA_NL);
    NA_UNUSED(getchar());
  #endif

  return 0;
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
