
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

uint64 randu64();
double getTime();
double printAndSwapTime(const char* title, double starttime);
void printi64(int64 i);
void printu64(uint64 ui);
void testEquali64(int64 i, uint32 goalhi, uint32 goallo);
void testEqualu64(uint64 ui, uint32 goalhi, uint32 goallo);
void testEqualBool(NABool b, NABool goalb);



#define TESTSIZE 1000000





void testMaking(){
  int64 i1 = naMakei64WithLo(-0x12345678);
  int64 i2 = naMakei64WithDouble(-123456789012345.);
  uint64 ui1 = naMakeu64WithLo(0x12345678);
  uint64 ui2 = naMakeu64WithLo(0x12345678);
  uint64 ui3 = naMakeu64WithDouble(123456789012345.);
  uint64 ui4 = naMakeu64(0x12345678, 0x56473829);
  testEquali64(i1, 0xffffffff, 0xedcba988);
  testEquali64(i2, 0xffff8fb7, 0x79f22087);
  testEqualu64(ui1, 0x00000000, 0x12345678);
  testEqualu64(ui2, 0x00000000, 0x12345678);
  testEqualu64(ui3, 0x00007048 , 0x860ddf79);
  testEqualu64(ui4, 0x12345678 , 0x56473829);
}


void timeMaking(){
  double t;
  int i;
  int64 i64 = NA_ZERO_i64;
  uint64 u64 = NA_ZERO_u64;


  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i64 = naAddi64(naMakei64WithLo(-rand()), i64);}
  t = printAndSwapTime("naMakei64WithLo", t);
  for(i = 0; i < TESTSIZE; i++){i64 = naAddi64(naMakei64WithDouble(-(double)rand() * (double)rand()), i64);}
  t = printAndSwapTime("naMakei64WithDouble", t);

  for(i = 0; i < TESTSIZE; i++){u64 = naAddu64(naMakeu64WithLo((uint32)rand()), u64);}
  t = printAndSwapTime("naMakeu64WithLo", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naAddu64(naMakeu64WithDouble((double)rand() * (double)rand()), u64);}
  t = printAndSwapTime("naMakeu64WithDouble", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naAddu64(naMakeu64((uint32)rand(), (uint32)rand()), u64);}
  t = printAndSwapTime("naMakeu64", t);

  NA_UNUSED(i64);
  NA_UNUSED(u64);
  NA_UNUSED(t);
}



void testBinary(){
  int64 i = naMakei64WithDouble(-123456789012345.); 
  int64 term = naCastu64Toi64(naMakeu64(0x55555555, 0x55555555));
  uint64 ui = naMakeu64(0x12345678, 0x56473829);
  uint64 uterm = naMakeu64(0x55555555, 0x55555555);

  int64 i1 = naNoti64(i);
  int64 i2 = naOri64(i, term);
  int64 i3 = naAndi64(i, term);
  int64 i4 = naXori64(i, term);
  int64 i5 = naShli64(i, 5);
  int64 i6 = naShri64(i, 5);

  uint64 ui1 = naNotu64(ui);
  uint64 ui2 = naOru64(ui, uterm);
  uint64 ui3 = naAndu64(ui, uterm);
  uint64 ui4 = naXoru64(ui, uterm);
  uint64 ui5 = naShlu64(ui, 5);
  uint64 ui6 = naShru64(ui, 5);

  testEquali64(i1, 0x00007048, 0x860ddf78);
  testEquali64(i2, 0xffffdff7, 0x7df775d7);
  testEquali64(i3, 0x55550515, 0x51500005);
  testEquali64(i4, 0xaaaadae2, 0x2ca775d2);
  testEquali64(i5, 0xfff1f6ef, 0x3e4410e0);
  testEquali64(i6, 0xfffffc7d, 0xbbcf9104);

  testEqualu64(ui1, 0xedcba987, 0xa9b8c7d6);
  testEqualu64(ui2, 0x5775577d, 0x57577d7d);
  testEqualu64(ui3, 0x10145450, 0x54451001);
  testEqualu64(ui4, 0x4761032d, 0x03126d7c);
  testEqualu64(ui5, 0x468acf0a, 0xc8e70520);
  testEqualu64(ui6, 0x0091a2b3, 0xc2b239c1);
}



void timeBinary(){
  int64 i64_1 = naMakei64(0x693275dc, 0xf035bac);
  int64 i64_2 = naMakei64(0x95fba4e, 0x069a2f4);
  uint64 u64_1 = naMakeu64(0x693275dc, 0xf035bac);
  uint64 u64_2 = naMakeu64(0x95fba4e, 0x069a2f4);
  double t;
  int i;
  int64 i64;
  uint64 u64;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i64 = naNoti64(i64_1);}
  t = printAndSwapTime("naNoti64", t);
  for(i = 0; i < TESTSIZE; i++){i64 = naOri64(i64_1, i64_2);}
  t = printAndSwapTime("naOri64", t);
  for(i = 0; i < TESTSIZE; i++){i64 = naAndi64(i64_1, i64_2);}
  t = printAndSwapTime("naAndi64", t);
  for(i = 0; i < TESTSIZE; i++){i64 = naXori64(i64_1, i64_2);}
  t = printAndSwapTime("naXori64", t);
  for(i = 0; i < TESTSIZE; i++){i64 = naShli64(i64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShli64", t);
  for(i = 0; i < TESTSIZE; i++){i64 = naShri64(i64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShri64", t);

  for(i = 0; i < TESTSIZE; i++){u64 = naNotu64(u64_1);}
  t = printAndSwapTime("naNotu64", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naOru64(u64_1, u64_2);}
  t = printAndSwapTime("naOru64", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naAndu64(u64_1, u64_2);}
  t = printAndSwapTime("naAndu64", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naXoru64(u64_1, u64_2);}
  t = printAndSwapTime("naXoru64", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naShlu64(u64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShlu64", t);
  for(i = 0; i < TESTSIZE; i++){u64 = naShru64(u64_1, (int)(32. * (double)rand() / (double)RAND_MAX));}
  t = printAndSwapTime("naShru64", t);

  NA_UNUSED(i64);
  NA_UNUSED(u64);
  NA_UNUSED(t);
}



void testComparison(){
  int64 i1 = naMakei64WithDouble(-123456789012345.);
  int64 i2 = naMakei64WithDouble(-123456.);
  uint64 ui1 = naMakeu64WithDouble(123456789012345.);
  uint64 ui2 = naMakeu64WithDouble(123456.);

  testEqualBool(naEquali64(i1, i1), NA_TRUE);
  testEqualBool(naGreateri64(i1, i2), NA_FALSE);
  testEqualBool(naGreateri64(i2, i1), NA_TRUE);
  testEqualBool(naGreateri64(i1, i1), NA_FALSE);
  testEqualBool(naGreaterEquali64(i1, i2), NA_FALSE);
  testEqualBool(naGreaterEquali64(i2, i1), NA_TRUE);
  testEqualBool(naGreaterEquali64(i1, i1), NA_TRUE);
  testEqualBool(naSmalleri64(i1, i2), NA_TRUE);
  testEqualBool(naSmalleri64(i2, i1), NA_FALSE);
  testEqualBool(naSmalleri64(i1, i1), NA_FALSE);
  testEqualBool(naSmallerEquali64(i1, i2), NA_TRUE);
  testEqualBool(naSmallerEquali64(i2, i1), NA_FALSE);
  testEqualBool(naSmallerEquali64(i1, i1), NA_TRUE);

  testEqualBool(naEqualu64(ui1, ui1), NA_TRUE);
  testEqualBool(naGreateru64(ui1, ui2), NA_TRUE);
  testEqualBool(naGreateru64(ui2, ui1), NA_FALSE);
  testEqualBool(naGreateru64(ui1, ui1), NA_FALSE);
  testEqualBool(naGreaterEqualu64(ui1, ui2), NA_TRUE);
  testEqualBool(naGreaterEqualu64(ui2, ui1), NA_FALSE);
  testEqualBool(naGreaterEqualu64(ui1, ui1), NA_TRUE);
  testEqualBool(naSmalleru64(ui1, ui2), NA_FALSE);
  testEqualBool(naSmalleru64(ui2, ui1), NA_TRUE);
  testEqualBool(naSmalleru64(ui1, ui1), NA_FALSE);
  testEqualBool(naSmallerEqualu64(ui1, ui2), NA_FALSE);
  testEqualBool(naSmallerEqualu64(ui2, ui1), NA_TRUE);
  testEqualBool(naSmallerEqualu64(ui1, ui1), NA_TRUE);
}



void timeComparison(){
  int64 i01 = naMakei64WithDouble(-123456789012345.);
  int64 i02 = naMakei64WithDouble(987654321987.);
  uint64 ui01 = naMakeu64WithDouble(123456789012345.);
  uint64 ui02 = naMakeu64WithDouble(987654321987.);
  double t;
  int i;
  NABool testbool = NA_FALSE;

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){testbool ^= naEquali64(i01, i02); naInci64(i01); naDeci64(i02);}
  t = printAndSwapTime("naEquali64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateri64(i01, i02); naInci64(i01); naDeci64(i02);}
  t = printAndSwapTime("naGreateri64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEquali64(i01, i02); naInci64(i01); naDeci64(i02);}
  t = printAndSwapTime("naGreaterEquali64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleri64(i01, i02); naInci64(i01); naDeci64(i02);}
  t = printAndSwapTime("naSmalleri64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEquali64(i01, i02); naInci64(i01); naDeci64(i02);}
  t = printAndSwapTime("naSmallerEquali64", t);

  for(i = 0; i < TESTSIZE; i++){testbool ^= naEqualu64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
  t = printAndSwapTime("naEqualu64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreateru64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
  t = printAndSwapTime("naGreateru64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naGreaterEqualu64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
  t = printAndSwapTime("naGreaterEqualu64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmalleru64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
  t = printAndSwapTime("naSmalleru64", t);
  for(i = 0; i < TESTSIZE; i++){testbool ^= naSmallerEqualu64(ui01, ui02); naIncu64(ui01); naDecu64(ui02);}
  t = printAndSwapTime("naSmallerEqualu64", t);

  NA_UNUSED(i01);
  NA_UNUSED(i02);
  NA_UNUSED(ui01);
  NA_UNUSED(ui02);
  NA_UNUSED(t);
}



void testArithmetic(){
  int64 i01 = naMakei64WithDouble(-123456789012345.);
  int64 i02 = naMakei64WithDouble(987654321987.);
  int64 i03 = NA_ZERO_i64;
  int64 i04 = naMakei64WithDouble(-123.);
  int64 i1, i2;
  uint64 ui01 = naMakeu64WithDouble(123456789012345.);
  uint64 ui02 = naMakeu64WithDouble(987654321987.);
  uint64 ui03 = NA_ZERO_u64;
  uint64 ui2;

  i1 = naNegi64(i01);
  testEquali64(i1, 0x00007048, 0x860ddf79);
  i1 = naNegi64(naNegi64(i01));
  testEquali64(i1, 0xffff8fb7, 0x79f22087);
  naInci64(i01);
  testEquali64(i01, 0xffff8fb7, 0x79f22088);
  naDeci64(i01);
  testEquali64(i01, 0xffff8fb7, 0x79f22087);
  naInci64(i03);
  testEquali64(i03, 0x00000000, 0x00000001);
  naDeci64(i03);
  testEquali64(i03, 0x00000000, 0x00000000);
  naDeci64(i03);
  testEquali64(i03, 0xffffffff, 0xffffffff);
  naInci64(i03);
  testEquali64(i03, 0x00000000, 0x00000000);

  i2 = naAddi64(i01, i02);
  testEquali64(i2, 0xffff909d, 0x6ebb17ca);
  i2 = naSubi64(i01, i02);
  testEquali64(i2, 0xffff8ed1, 0x85292944);
  i2 = naMuli64(i01, i04);
  testEquali64(i2, 0x0035f2d8, 0x68aa5f23);
  i2 = naMuli64(i02, i04);
  testEquali64(i2, 0xffff9183, 0x637132cf);
  i2 = naDivi64(i01, i02);
  testEquali64(i2, 0xffffffff, 0xffffff84);
  i2 = naDivi64(i01, i04);
  testEquali64(i2, 0x000000e9, 0xb2001cdf);
  i2 = naDivi64(i04, i01);
  testEquali64(i2, 0x00000000, 0x00000000);
  i2 = naModi64(i01, i02);
  testEquali64(i2, 0xffffff1a, 0x0b49e4fb);
  i2 = naModi64(i01, i04);
  testEquali64(i2, 0xffffffff, 0xffffffac);

  naIncu64(ui01);
  testEqualu64(ui01, 0x00007048, 0x860ddf7a);
  naDecu64(ui01);
  testEqualu64(ui01, 0x00007048, 0x860ddf79);
  naIncu64(ui03);
  testEqualu64(ui03, 0x00000000, 0x00000001);
  naDecu64(ui03);
  testEqualu64(ui03, 0x00000000, 0x00000000);
  naDecu64(ui03);
  testEqualu64(ui03, 0xffffffff, 0xffffffff);
  naIncu64(ui03);
  testEqualu64(ui03, 0x00000000, 0x00000000);

  ui2 = naAddu64(ui01, ui02);
  testEqualu64(ui2, 0x0000712e, 0x7ad6d6bc);
  ui2 = naSubu64(ui01, ui02);
  testEqualu64(ui2, 0x00006f62, 0x9144e836);
  ui2 = naDivu64(ui01, ui02);
  testEqualu64(ui2, 0x00000000, 0x0000007c);
  ui2 = naModu64(ui01, ui02);
  testEqualu64(ui2, 0x000000e5, 0xf4b61b05);

}



void timeArithmetic(){
  double t;
  int i;
  int64 i64_1 = naCastu64Toi64(naMakeu64WithLo(27253242));
  int64 i64_2 = naCastu64Toi64(naMakeu64WithLo(345275662));
  uint64 u64_1 = naMakeu64WithLo(123876413);
  uint64 u64_2 = naMakeu64WithLo(98372154);

  t = getTime();
  for(i = 0; i < TESTSIZE; i++){i64_1 = naNegi64(naCastu64Toi64(randu64()));}
  t = printAndSwapTime("naNegi64", t);
  for(i = 0; i < TESTSIZE; i++){naInci64(i64_1);}
  t = printAndSwapTime("naInci64", t);
  for(i = 0; i < TESTSIZE; i++){naDeci64(i64_1);}
  t = printAndSwapTime("naDeci64", t);
  for(i = 0; i < TESTSIZE; i++){i64_1 = naAddi64(i64_1, i64_2);}
  t = printAndSwapTime("naAddi64", t);
  for(i = 0; i < TESTSIZE; i++){i64_2 = naSubi64(i64_1, i64_2);}
  t = printAndSwapTime("naSubi64", t);
  for(i = 0; i < TESTSIZE; i++){i64_1 = naMuli64(i64_1, i64_2);}
  t = printAndSwapTime("naMuli64", t);
  for(i = 0; i < TESTSIZE; i++){i64_1 = naDivi64(i64_1, i64_2);}
  t = printAndSwapTime("naDivi64", t);
  for(i = 0; i < TESTSIZE; i++){i64_1 = naModi64(i64_1, i64_2);}
  t = printAndSwapTime("naModi64", t);

  for(i = 0; i < TESTSIZE; i++){naIncu64(u64_1);}
  t = printAndSwapTime("naIncu64", t);
  for(i = 0; i < TESTSIZE; i++){naDecu64(u64_1);}
  t = printAndSwapTime("naDecu64", t);
  for(i = 0; i < TESTSIZE; i++){u64_1 = naAddu64(u64_1, u64_2);}
  t = printAndSwapTime("naAddu64", t);
  for(i = 0; i < TESTSIZE; i++){u64_1 = naSubu64(u64_1, u64_2);}
  t = printAndSwapTime("naSubu64", t);
  for(i = 0; i < TESTSIZE; i++){u64_2 = naMulu64(u64_1, u64_2);}
  t = printAndSwapTime("naMulu64", t);
  for(i = 0; i < TESTSIZE; i++){u64_1 = naDivu64(u64_1, u64_2);}
  t = printAndSwapTime("naDivu64", t);
  for(i = 0; i < TESTSIZE; i++){u64_1 = naModu64(u64_1, u64_2);}
  t = printAndSwapTime("naModu64", t);

  NA_UNUSED(i64_1);
  NA_UNUSED(i64_2);
  NA_UNUSED(u64_1);
  NA_UNUSED(u64_2);
  NA_UNUSED(t);
}



uint32 randu32(){
  return (uint32)rand();
}
NAu64 randu64(){
  return naMakeu64(randu32(), randu32());
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
    return naCasti64Toi32(dt.sisec) + dt.nsec / 1000000000.;
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
void printi64(int64 i){
  printu64(naCasti64Tou64(i));
}
void printu64(uint64 ui){
  #if !defined NA_TYPE_INT64
    printf("%08x %08x", ui.hi, ui.lo);
  #else
    printf("%016llx", ui);
  #endif
}
void testEquali64(int64 i, uint32 goalhi, uint32 goallo){
  testEqualu64(naCasti64Tou64(i), goalhi, goallo);
}
void testEqualu64(uint64 ui, uint32 goalhi, uint32 goallo){
  #if !defined NA_TYPE_INT64
    if(ui.lo == goallo && ui.hi == goalhi){
      printf("Pass ");
    }else{
      printf("FAIL ");
    }
  #else
    if((ui & NA_MAX_u32) == goallo && (ui >> 32) == goalhi){
      printf("Pass ");
    }else{
      printf("FAIL ");
    }
  #endif
  printu64(ui);
  printf(NA_NL);
}
void testEqualBool(NABool b, NABool goalb){
  if(b == goalb){
    printf("Pass" NA_NL);
  }else{
    printf("FAIL" NA_NL);
  }
}



int main(void){
  #if !defined NA_TYPE_INT64
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
