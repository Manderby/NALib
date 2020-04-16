
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

uint64 randUInt64();
uint128 randUInt128();
double getTime();
double printAndSwapTime(const char* title, double starttime);
void printInt128(int128 i);
void printUInt128(uint128 ui);
void testEqualInt128(int128 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3);
void testEqualUInt128(uint128 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3);
void testEqualBool(NABool b, NABool goalb);

#define MAKEi128(a,b,c,d) naMakeInt128(naMakeInt64((int32)a, b), naMakeUInt64(c, d));
#define MAKEu128(a,b,c,d) naMakeUInt128(naMakeUInt64(a, b), naMakeUInt64(c, d));


#define TESTSIZE 100000





void testMaking(){
  int128 i1 = naMakeInt128WithLo(naNegInt64(naMakeInt64(0x12345678, 0x12345678)));
  int128 i2 = naMakeInt128WithDouble(-123456789012345.);
  uint128 ui1 = naMakeUInt128WithLo(naMakeUInt64WithLiteralLo(0x12345678));
  uint128 ui2 = naMakeUInt128WithLo(naMakeUInt64(0x12345678, 0x12345678));
  uint128 ui3 = naMakeUInt128WithDouble(123456789012345.);
  uint128 ui4 = MAKEu128(0x12345678, 0x56473829, 0xfabcedae, 0x33333333);
  testEqualInt128(i1, 0xffffffff, 0xffffffff, 0xedcba987, 0xedcba988);
  testEqualInt128(i2, 0xffffffff, 0xffffffff, 0xffff8fb7, 0x79f22087);
  testEqualUInt128(ui1, 0x00000000, 0x00000000, 0x00000000, 0x12345678);
  testEqualUInt128(ui2, 0x00000000, 0x00000000, 0x12345678, 0x12345678);
  testEqualUInt128(ui3, 0x00000000, 0x00000000, 0x00007048 , 0x860ddf79);
  testEqualUInt128(ui4, 0x12345678 , 0x56473829, 0xfabcedae, 0x33333333);
}



void timeMaking(){
  double t;
  int i;
  int128 i128 = NA_ZERO_128;
  uint128 ui128 = NA_ZERO_128u;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i128 = naAddInt128(naMakeInt128WithLo(naNegInt64(naCastUInt64ToInt64(randUInt64()))), i128);}
  t = printAndSwapTime("naMakeInt128WithLo", t);
  for(i = 0; i < TESTSIZE; i++){i128 = naAddInt128(naMakeInt128WithDouble(-(double)rand() * (double)rand()), i128);}
  t = printAndSwapTime("naMakeInt128WithDouble", t);

  for(i = 0; i < TESTSIZE; i++){ui128 = naAddUInt128(naMakeUInt128WithLo(randUInt64()), ui128);}
  t = printAndSwapTime("naMakeUInt128WithLo", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naAddUInt128(naMakeUInt128WithDouble((double)rand() * (double)rand()), ui128);}
  t = printAndSwapTime("naMakeUInt128WithDouble", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naAddUInt128(naMakeUInt128(randUInt64(), randUInt64()), ui128);}
  t = printAndSwapTime("naMakeUInt128", t);

  NA_UNUSED(i128);
  NA_UNUSED(ui128);
  NA_UNUSED(t);
}



void testBinary(){
  int128 i =      MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  int128 term =   MAKEi128(0x55555555, 0x55555555, 0x55555555, 0x55555555);
  uint128 ui =    MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  uint128 uterm = MAKEu128(0x55555555, 0x55555555, 0x55555555, 0x55555555);

  int128 i1 = naNotInt128(i);
  int128 i2 = naOrInt128(i, term);
  int128 i3 = naAndInt128(i, term);
  int128 i4 = naXorInt128(i, term);
  int128 i5 = naShlInt128(i, 5);
  int128 i6 = naShrInt128(i, 5);

  uint128 ui1 = naNotUInt128(ui);
  uint128 ui2 = naOrUInt128(ui, uterm);
  uint128 ui3 = naAndUInt128(ui, uterm);
  uint128 ui4 = naXorUInt128(ui, uterm);
  uint128 ui5 = naShlUInt128(ui, 5);
  uint128 ui6 = naShrUInt128(ui, 5);

  testEqualInt128(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53e);
  testEqualInt128(i2, 0xfd7775dd, 0xf575f7fd, 0x7ddf5575, 0x7ffd5fd5);
  testEqualInt128(i3, 0x51105554, 0x50151404, 0x54105510, 0x15140041);
  testEqualInt128(i4, 0xac672089, 0xa560e3f9, 0x29cf0065, 0x6ae95f94);
  testEqualInt128(i5, 0x264ebb9e, 0x06b6d58f, 0x934aa607, 0xf7815820);
  testEqualInt128(i6, 0xffc993ae, 0xe781adb5, 0x63e4d2a9, 0x81fde056);

  testEqualUInt128(ui1, 0x00a045b1, 0xf965d0db, 0xc0054761, 0xa755d642);
  testEqualUInt128(ui2, 0xff5fff5f, 0x57df7f75, 0x7ffffddf, 0x5dff7dfd);
  testEqualUInt128(ui3, 0x55551044, 0x04100504, 0x15501014, 0x50000115);
  testEqualUInt128(ui4, 0xaa0aef1b, 0x53cf7a71, 0x6aafedcb, 0x0dff7ce8);
  testEqualUInt128(ui5, 0xebf749c0, 0xd345e487, 0xff5713cb, 0x154537a0);
  testEqualUInt128(ui6, 0x07fafdd2, 0x7034d179, 0x21ffd5c4, 0xf2c5514d);
}



void timeBinary(){
  int128 i128_1 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  int128 i128_2 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  uint128 ui128_1 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  uint128 ui128_2 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
  double t;
  int i;
  int128 i128;
  uint128 ui128;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i128 = naNotInt128(i128_1);}
  t = printAndSwapTime("naNotInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128 = naOrInt128(i128_1, i128_2);}
  t = printAndSwapTime("naOrInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128 = naAndInt128(i128_1, i128_2);}
  t = printAndSwapTime("naAndInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128 = naXorInt128(i128_1, i128_2);}
  t = printAndSwapTime("naXorInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128 = naShlInt128(i128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShlInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128 = naShrInt128(i128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShrInt128", t);

  for(i = 0; i < TESTSIZE; i++){ui128 = naNotUInt128(ui128_1);}
  t = printAndSwapTime("naNotUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naOrUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naOrUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naAndUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naAndUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naXorUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naXorUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naShlUInt128(ui128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShlUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128 = naShrUInt128(ui128_1, (int)(64. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShrUInt128", t);

  NA_UNUSED(i128);
  NA_UNUSED(ui128);
  NA_UNUSED(t);
}



void testComparison(){
  int128 i1 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  int128 i2 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  uint128 ui1 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  uint128 ui2 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);

  testEqualBool(naEqualInt128(i1, i1), NA_TRUE);
  testEqualBool(naGreaterInt128(i1, i2), NA_FALSE);
  testEqualBool(naGreaterInt128(i2, i1), NA_TRUE);
  testEqualBool(naGreaterInt128(i1, i1), NA_FALSE);
  testEqualBool(naGreaterEqualInt128(i1, i2), NA_FALSE);
  testEqualBool(naGreaterEqualInt128(i2, i1), NA_TRUE);
  testEqualBool(naGreaterEqualInt128(i1, i1), NA_TRUE);
  testEqualBool(naSmallerInt128(i1, i2), NA_TRUE);
  testEqualBool(naSmallerInt128(i2, i1), NA_FALSE);
  testEqualBool(naSmallerInt128(i1, i1), NA_FALSE);
  testEqualBool(naSmallerEqualInt128(i1, i2), NA_TRUE);
  testEqualBool(naSmallerEqualInt128(i2, i1), NA_FALSE);
  testEqualBool(naSmallerEqualInt128(i1, i1), NA_TRUE);

  testEqualBool(naEqualUInt128(ui1, ui1), NA_TRUE);
  testEqualBool(naGreaterUInt128(ui1, ui2), NA_TRUE);
  testEqualBool(naGreaterUInt128(ui2, ui1), NA_FALSE);
  testEqualBool(naGreaterUInt128(ui1, ui1), NA_FALSE);
  testEqualBool(naGreaterEqualUInt128(ui1, ui2), NA_TRUE);
  testEqualBool(naGreaterEqualUInt128(ui2, ui1), NA_FALSE);
  testEqualBool(naGreaterEqualUInt128(ui1, ui1), NA_TRUE);
  testEqualBool(naSmallerUInt128(ui1, ui2), NA_FALSE);
  testEqualBool(naSmallerUInt128(ui2, ui1), NA_TRUE);
  testEqualBool(naSmallerUInt128(ui1, ui1), NA_FALSE);
  testEqualBool(naSmallerEqualUInt128(ui1, ui2), NA_FALSE);
  testEqualBool(naSmallerEqualUInt128(ui2, ui1), NA_TRUE);
  testEqualBool(naSmallerEqualUInt128(ui1, ui1), NA_TRUE);
}



void timeComparison(){
  int128 i01 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  int128 i02 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  uint128 ui01 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  uint128 ui02 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
  double t;
  int i;
  NABool testbool = NA_FALSE;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualInt128(i01, i02); naIncInt128(i01); naDecInt128(i02);}
  t = printAndSwapTime("naEqualInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterInt128(i01, i02); naIncInt128(i01); naDecInt128(i02);}
  t = printAndSwapTime("naGreaterInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualInt128(i01, i02); naIncInt128(i01); naDecInt128(i02);}
  t = printAndSwapTime("naGreaterEqualInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerInt128(i01, i02); naIncInt128(i01); naDecInt128(i02);}
  t = printAndSwapTime("naSmallerInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualInt128(i01, i02); naIncInt128(i01); naDecInt128(i02);}
  t = printAndSwapTime("naSmallerEqualInt128", t);

  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualUInt128(ui01, ui02); naIncUInt128(ui01); naDecUInt128(ui02);}
  t = printAndSwapTime("naEqualUInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterUInt128(ui01, ui02); naIncUInt128(ui01); naDecUInt128(ui02);}
  t = printAndSwapTime("naGreaterUInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualUInt128(ui01, ui02); naIncUInt128(ui01); naDecUInt128(ui02);}
  t = printAndSwapTime("naGreaterEqualUInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerUInt128(ui01, ui02); naIncUInt128(ui01); naDecUInt128(ui02);}
  t = printAndSwapTime("naSmallerUInt128", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualUInt128(ui01, ui02); naIncUInt128(ui01); naDecUInt128(ui02);}
  t = printAndSwapTime("naSmallerEqualUInt128", t);

  NA_UNUSED(i01);
  NA_UNUSED(i02);
  NA_UNUSED(ui01);
  NA_UNUSED(ui02);
  NA_UNUSED(t);
}



void testArithmetic(){
  int128 i01 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  int128 i02 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  int128 i03 =   NA_ZERO_128;
  int128 i04 =   naMakeInt128WithDouble(-123.);
  int128 i1, i2;
  uint128 ui01 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  uint128 ui02 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);
  uint128 ui03 = NA_ZERO_128u;
  uint128 ui2;

  i1 = naNegInt128(i01);
  testEqualInt128(i1, 0x06cd8a23, 0x0fca4953, 0x8365aacf, 0xc043f53f);
  i1 = naNegInt128(naNegInt128(i01));
  testEqualInt128(i1, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  naIncInt128(i01);
  testEqualInt128(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac2);
  naDecInt128(i01);
  testEqualInt128(i01, 0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  naIncInt128(i03);
  testEqualInt128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
  naDecInt128(i03);
  testEqualInt128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
  naDecInt128(i03);
  testEqualInt128(i03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
  naIncInt128(i03);
  testEqualInt128(i03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);

  i2 = naAddInt128(i01, i02);
  testEqualInt128(i2, 0xfb617c69, 0x5c5a548c, 0x92f837d5, 0x67836ab7);
  i2 = naSubInt128(i01, i02);
  testEqualInt128(i2, 0xf7036f50, 0x841118cc, 0x663c728b, 0x17f4aacb);
  i2 = naMulInt128(i01, i04);
  testEqualInt128(i2, 0x44c15ed8, 0x96313b20, 0x21d911d1, 0x60a6d545);
  i2 = naMulInt128(i02, i04);
  testEqualInt128(i2, 0xf367da88, 0x0a682555, 0x40e41aa5, 0xe334e4ce);
  i2 = naDivInt128(i01, i02);
  testEqualInt128(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd);
  i2 = naDivInt128(i01, i04);
  testEqualInt128(i2, 0x000e28aa, 0xf3a3fc6e, 0xfce7d9d8, 0x1021da72);
  i2 = naModInt128(i01, i02);
  testEqualInt128(i2, 0xffbf8982, 0x34a3904c, 0xbfb3fd1f, 0xb7122aa3);
  i2 = naModInt128(i01, i04);
  testEqualInt128(i2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffff87);

  naIncUInt128(ui01);
  testEqualUInt128(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29be);
  naDecUInt128(ui01);
  testEqualUInt128(ui01, 0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  naIncUInt128(ui03);
  testEqualUInt128(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000001);
  naDecUInt128(ui03);
  testEqualUInt128(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
  naDecUInt128(ui03);
  testEqualUInt128(ui03, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
  naIncUInt128(ui03);
  testEqualUInt128(ui03, 0x00000000, 0x00000000, 0x00000000, 0x00000000);

  ui2 = naAddUInt128(ui01, ui02);
  testEqualUInt128(ui2, 0xff92302a, 0xf6cf6ad0, 0xbc950ece, 0x9866337e);
  ui2 = naSubUInt128(ui01, ui02);
  testEqualUInt128(ui2, 0xff2d4471, 0x1664f377, 0xc360626e, 0x18ee1ffc);
  ui2 = naDivUInt128(ui01, ui02);
  testEqualUInt128(ui2, 0x00000000, 0x00000000, 0x00000000, 0x0000050f);
  ui2 = naModUInt128(ui01, ui02);
  testEqualUInt128(ui2, 0x001d81aa, 0xe9515199, 0xef40ba8b, 0xf074d26e);

}



void timeArithmetic(){
  double t;
  int i;
  int128 i128_1 =   MAKEi128(0xf93275dc, 0xf035b6ac, 0x7c9a5530, 0x3fbc0ac1);
  int128 i128_2 =   MAKEi128(0x022f068c, 0x6c249de0, 0x165de2a5, 0x27c75ff6);
  uint128 ui128_1 = MAKEu128(0xff5fba4e, 0x069a2f24, 0x3ffab89e, 0x58aa29bd);
  uint128 ui128_2 = MAKEu128(0x003275dc, 0xf0353bac, 0x7c9a5630, 0x3fbc09c1);

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i128_1 = naNegInt128(i128_1);}
  t = printAndSwapTime("naNegInt128", t);
  for(i = 0; i < TESTSIZE; i++){naIncInt128(i128_1);}
  t = printAndSwapTime("naIncInt128", t);
  for(i = 0; i < TESTSIZE; i++){naDecInt128(i128_1);}
  t = printAndSwapTime("naDecInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128_1 = naAddInt128(i128_1, i128_2);}
  t = printAndSwapTime("naAddInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128_2 = naSubInt128(i128_1, i128_2);}
  t = printAndSwapTime("naSubInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128_1 = naMulInt128(i128_1, i128_2);}
  t = printAndSwapTime("naMulInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128_1 = naDivInt128(i128_1, i128_2);}
  t = printAndSwapTime("naDivInt128", t);
  for(i = 0; i < TESTSIZE; i++){i128_1 = naModInt128(i128_1, i128_2);}
  t = printAndSwapTime("naModInt128", t);

  for(i = 0; i < TESTSIZE; i++){naIncUInt128(ui128_1);}
  t = printAndSwapTime("naIncUInt128", t);
  for(i = 0; i < TESTSIZE; i++){naDecUInt128(ui128_1);}
  t = printAndSwapTime("naDecUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128_1 = naAddUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naAddUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128_1 = naSubUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naSubUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128_2 = naMulUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naMulUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128_1 = naDivUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naDivUInt128", t);
  for(i = 0; i < TESTSIZE; i++){ui128_1 = naModUInt128(ui128_1, ui128_2);}
  t = printAndSwapTime("naModUInt128", t);

  NA_UNUSED(i128_1);
  NA_UNUSED(i128_2);
  NA_UNUSED(ui128_1);
  NA_UNUSED(ui128_2);
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
  printf("%s\t\t%f s\n", title, t2 - starttime);
  return t2;
}
void printInt128(int128 i){
  printUInt128(naCastInt128ToUInt128(i));
}
void printUInt128(uint128 ui){
  #if !defined NA_TYPE_INT128
    #if !defined NA_TYPE_INT64
      printf("0x%08x 0x%08x 0x%08x 0x%08x", ui.hi.hi, ui.hi.lo, ui.lo.hi, ui.lo.lo);
    #else
      printf("0x%08x 0x%08x 0x%08x 0x%08x", (uint32)(ui.hi >> 32), (uint32)ui.hi, (uint32)(ui.lo >> 32), (uint32)ui.lo);
    #endif
  #else
    printf("0x%08x 0x%08x 0x%08x 0x%08x", (uint32)(ui >> 96), (uint32)(ui >> 64), (uint32)(ui >> 32), (uint32)ui);
  #endif
}
void testEqualInt128(int128 i, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3){
  testEqualUInt128(naCastInt128ToUInt128(i), goalhi, goallo1, goallo2, goallo3);
}
void testEqualUInt128(uint128 ui, uint32 goalhi, uint32 goallo1, uint32 goallo2, uint32 goallo3){
  #if !defined NA_TYPE_INT128
    if(naEqualUInt64(ui.lo, naMakeUInt64(goallo2, goallo3))
      && naEqualUInt64(ui.hi, naMakeUInt64(goalhi, goallo1))){
      printf("ok   ");
    }else{
      printf("FAIL ");
    }
  #else
    if((ui & NA_UINT32_MAX) == goallo3
      && ((ui >> 32) & NA_UINT32_MAX) == goallo2
      && ((ui >> 64) & NA_UINT32_MAX) == goallo1
      && ((ui >> 96) & NA_UINT32_MAX) == goalhi){
      printf("ok   ");
    }else{
      printf("FAIL ");
    }
  #endif
  printUInt128(ui);
  printf("\n");
}
void testEqualBool(NABool b, NABool goalb){
  if(b == goalb){
    printf("ok  \n");
  }else{
    printf("FAIL\n");
  }
}



int main(void){
  #if !defined NA_TYPE_INT128
    printf("Emulation\n");
  #else
    printf("Native\n");
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
    printf("Finished.\n");
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
