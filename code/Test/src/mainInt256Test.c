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
