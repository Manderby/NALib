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
//double getTime(void);
//double printAndSwapTime(const char* title, double starttime);
//void printi64(int64 i);
//void printu64(uint64 ui);
//void testEquali64(int64 i, uint32 goalhi, uint32 goallo);
//void testEqualu64(uint64 ui, uint32 goalhi, uint32 goallo);
//void testEqualBool(NABool b, NABool goalb);
//
//
//
//#define TESTSIZE 1000000
//
//
//
//void timeMaking(){
//  double t;
//  int i;
//  int64 i64 = NA_ZERO_i64;
//  uint64 u64 = NA_ZERO_u64;
//
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i64 = naAddi64(naMakei64WithLo(-rand()), i64);}
//  t = printAndSwapTime("naMakei64WithLo", t);
//  for(i = 0; i < TESTSIZE; i++){i64 = naAddi64(naMakei64WithDouble(-(double)rand() * (double)rand()), i64);}
//  t = printAndSwapTime("naMakei64WithDouble", t);
//
//  for(i = 0; i < TESTSIZE; i++){u64 = naAddu64(naMakeu64WithLo((uint32)rand()), u64);}
//  t = printAndSwapTime("naMakeu64WithLo", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naAddu64(naMakeu64WithDouble((double)rand() * (double)rand()), u64);}
//  t = printAndSwapTime("naMakeu64WithDouble", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naAddu64(naMakeu64((uint32)rand(), (uint32)rand()), u64);}
//  t = printAndSwapTime("naMakeu64", t);
//
//  NA_UNUSED(i64);
//  NA_UNUSED(u64);
//  NA_UNUSED(t);
//}
//
//
//
//void timeBinary(){
//  int64 i64_1 = naMakei64(0x693275dc, 0xf035bac);
//  int64 i64_2 = naMakei64(0x95fba4e, 0x069a2f4);
//  uint64 u64_1 = naMakeu64(0x693275dc, 0xf035bac);
//  uint64 u64_2 = naMakeu64(0x95fba4e, 0x069a2f4);
//  double t;
//  int i;
//  int64 i64;
//  uint64 u64;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i64 = naNoti64(i64_1);}
//  t = printAndSwapTime("naNoti64", t);
//  for(i = 0; i < TESTSIZE; i++){i64 = naOri64(i64_1, i64_2);}
//  t = printAndSwapTime("naOri64", t);
//  for(i = 0; i < TESTSIZE; i++){i64 = naAndi64(i64_1, i64_2);}
//  t = printAndSwapTime("naAndi64", t);
//  for(i = 0; i < TESTSIZE; i++){i64 = naXori64(i64_1, i64_2);}
//  t = printAndSwapTime("naXori64", t);
//  for(i = 0; i < TESTSIZE; i++){i64 = naShli64(i64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShli64", t);
//  for(i = 0; i < TESTSIZE; i++){i64 = naShri64(i64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShri64", t);
//
//  for(i = 0; i < TESTSIZE; i++){u64 = naNotu64(u64_1);}
//  t = printAndSwapTime("naNotu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naOru64(u64_1, u64_2);}
//  t = printAndSwapTime("naOru64", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naAndu64(u64_1, u64_2);}
//  t = printAndSwapTime("naAndu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naXoru64(u64_1, u64_2);}
//  t = printAndSwapTime("naXoru64", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naShlu64(u64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShlu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64 = naShru64(u64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
//  t = printAndSwapTime("naShru64", t);
//
//  NA_UNUSED(i64);
//  NA_UNUSED(u64);
//  NA_UNUSED(t);
//}
//
//
//
//void timeComparison(){
//  int64 i01 = naMakei64WithDouble(-123456789012345.);
//  int64 i02 = naMakei64WithDouble(987654321987.);
//  uint64 ui01 = naMakeu64WithDouble(123456789012345.);
//  uint64 ui02 = naMakeu64WithDouble(987654321987.);
//  double t;
//  int i;
//  NABool testbool = NA_FALSE;
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naEquali64(i01, i02); naInci64(i01); naDeci64(i02);}
//  t = printAndSwapTime("naEquali64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateri64(i01, i02); naInci64(i01); naDeci64(i02);}
//  t = printAndSwapTime("naGreateri64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEquali64(i01, i02); naInci64(i01); naDeci64(i02);}
//  t = printAndSwapTime("naGreaterEquali64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleri64(i01, i02); naInci64(i01); naDeci64(i02);}
//  t = printAndSwapTime("naSmalleri64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEquali64(i01, i02); naInci64(i01); naDeci64(i02);}
//  t = printAndSwapTime("naSmallerEquali64", t);
//
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualu64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
//  t = printAndSwapTime("naEqualu64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateru64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
//  t = printAndSwapTime("naGreateru64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualu64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
//  t = printAndSwapTime("naGreaterEqualu64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleru64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
//  t = printAndSwapTime("naSmalleru64", t);
//  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualu64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
//  t = printAndSwapTime("naSmallerEqualu64", t);
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
//  int64 i64_1 = naCastu64Toi64(naMakeu64WithLo(27253242));
//  int64 i64_2 = naCastu64Toi64(naMakeu64WithLo(345275662));
//  uint64 u64_1 = naMakeu64WithLo(123876413);
//  uint64 u64_2 = naMakeu64WithLo(98372154);
//
//  t = getTime();
//  for(i = 0; i < TESTSIZE; i++){i64_1 = naNegi64(naCastu64Toi64(randu64()));}
//  t = printAndSwapTime("naNegi64", t);
//  for(i = 0; i < TESTSIZE; i++){naInci64(i64_1);}
//  t = printAndSwapTime("naInci64", t);
//  for(i = 0; i < TESTSIZE; i++){naDeci64(i64_1);}
//  t = printAndSwapTime("naDeci64", t);
//  for(i = 0; i < TESTSIZE; i++){i64_1 = naAddi64(i64_1, i64_2);}
//  t = printAndSwapTime("naAddi64", t);
//  for(i = 0; i < TESTSIZE; i++){i64_2 = naSubi64(i64_1, i64_2);}
//  t = printAndSwapTime("naSubi64", t);
//  for(i = 0; i < TESTSIZE; i++){i64_1 = naMuli64(i64_1, i64_2);}
//  t = printAndSwapTime("naMuli64", t);
//  for(i = 0; i < TESTSIZE; i++){i64_1 = naDivi64(i64_1, i64_2);}
//  t = printAndSwapTime("naDivi64", t);
//  for(i = 0; i < TESTSIZE; i++){i64_1 = naModi64(i64_1, i64_2);}
//  t = printAndSwapTime("naModi64", t);
//
//  for(i = 0; i < TESTSIZE; i++){naIncu64(u64_1);}
//  t = printAndSwapTime("naIncu64", t);
//  for(i = 0; i < TESTSIZE; i++){naDecu64(u64_1);}
//  t = printAndSwapTime("naDecu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64_1 = naAddu64(u64_1, u64_2);}
//  t = printAndSwapTime("naAddu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64_1 = naSubu64(u64_1, u64_2);}
//  t = printAndSwapTime("naSubu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64_2 = naMulu64(u64_1, u64_2);}
//  t = printAndSwapTime("naMulu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64_1 = naDivu64(u64_1, u64_2);}
//  t = printAndSwapTime("naDivu64", t);
//  for(i = 0; i < TESTSIZE; i++){u64_1 = naModu64(u64_1, u64_2);}
//  t = printAndSwapTime("naModu64", t);
//
//  NA_UNUSED(i64_1);
//  NA_UNUSED(i64_2);
//  NA_UNUSED(u64_1);
//  NA_UNUSED(u64_2);
//  NA_UNUSED(t);
//}
//
//
//
//uint32 randu32(){
//  return (uint32)rand();
//}
//NAu64 randu64(){
//  return naMakeu64(randu32(), randu32());
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
//void printi64(int64 i){
//  printu64(naCasti64Tou64(i));
//}
//void printu64(uint64 ui){
//  #if !defined NA_TYPE_INT64
//    printf("%08x %08x", ui.hi, ui.lo);
//  #else
//    printf("%016llx", ui);
//  #endif
//}
//void testEqualBool(NABool b, NABool goalb){
//  if(b == goalb){
//    printf("Pass" NA_NL);
//  }else{
//    printf("FAIL" NA_NL);
//  }
//}
//
//
//
//int main(void){
//  #if !defined NA_TYPE_INT64
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
