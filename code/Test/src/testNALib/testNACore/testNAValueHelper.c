
#include "NATest.h"
#include <stdio.h>

#include "NABase/NABase.h"
#include "NAUtility/NAValueHelper.h"

#include "NAUtility/NAThreading.h"
#include "NAUtility/NAString.h"

void testFlags(void) {
  uint32 flagsu = 0xcccccccc;          // 0b11001100110011001100110011001100
  uint32 testFlagu1 = 0x00000001;      // not set
  uint32 testFlagu2 = 0x00000004;      // set
  uint32 testMultiFlagu1 = 0x00000104; // not all set
  uint32 testMultiFlagu2 = 0x00000404; // all set

  naTestGroup("Querying uint32 flags") {
    flagsu = 0xcccccccc;
    naTest(!naGetFlagu32  (flagsu, testFlagu1));
    naTest( naGetFlagu32  (flagsu, testFlagu2));
    naTest(!naGetFlagu32  (flagsu, testMultiFlagu1));
    naTest( naGetFlagu32Or(flagsu, testMultiFlagu1));
    naTest( naGetFlagu32  (flagsu, testMultiFlagu2));
  }

  naTestGroup("Setting uint32 flags") {
    flagsu = 0xcccccccc;
    naTest((naSetFlagu32(&flagsu, testFlagu1, NA_TRUE), flagsu == 0xcccccccd));
    naTest((naSetFlagu32(&flagsu, testFlagu1, NA_FALSE), flagsu == 0xcccccccc));
    naTest((naSetFlagu32(&flagsu, testFlagu2, NA_TRUE), flagsu == 0xcccccccc));
    naTest((naSetFlagu32(&flagsu, testMultiFlagu1, NA_TRUE), flagsu == 0xcccccdcc));
    naTest((naSetFlagu32(&flagsu, testMultiFlagu1, NA_FALSE), flagsu == 0xccccccc8));
    naTest((naSetFlagu32(&flagsu, testMultiFlagu2, NA_TRUE), flagsu == 0xcccccccc));
    naTestCrash(naSetFlagu32(NA_NULL, 1234, NA_TRUE));
    // Testing a value neither 0 nor 1 does not work on all compilers.
    //naTestError(naSetFlagu32(&flagsu, testFlagu1, 1234));
  }

  naTestGroup("Toggling uint32 flags") {
    flagsu = 0xcccccccc;
    naTest( naToggleFlagu32(&flagsu, testFlagu1));
    naTest(!naToggleFlagu32(&flagsu, testFlagu1));
    naTest(!naToggleFlagu32(&flagsu, testFlagu2));
    naTest( naToggleFlagu32(&flagsu, testFlagu2));
    naTest(!naToggleFlagu32(&flagsu, testMultiFlagu1));
    naTest(!naToggleFlagu32(&flagsu, testMultiFlagu1));
    naTest(!naToggleFlagu32(&flagsu, testMultiFlagu2));
    naTest( naToggleFlagu32(&flagsu, testMultiFlagu2));
    naTestCrash(naToggleFlagu32(NA_NULL, testFlagu1));
  }
}



void testRange(void) {
  naTestGroup("Make Max with End") {
    naTest(naMakeMaxWithEndi32(1234) == 1233);
    naTest(naMakeMaxWithEndi64(1234) == 1233);
    naTest(naMakeMaxWithEnds(1234) == 1233);
    naTestError(naMakeMaxWithEndi32(NA_MIN_i32));
    naTestError(naMakeMaxWithEndi64(NA_MIN_i64));
    naTestError(naMakeMaxWithEnds(NA_ZERO_s));

    naTest(naMakeEndWithMaxi32(1234) == 1235);
    naTest(naMakeEndWithMaxi64(1234) == 1235);
    naTest(naMakeEndWithMaxs(1234) == 1235);
    naTestError(naMakeEndWithMaxi32(NA_MAX_i32));
    naTestError(naMakeEndWithMaxi64(NA_MAX_i64));
    naTestError(naMakeEndWithMaxs(NA_MAX_s));
  }

  naTestGroup("Make End with Start and Length") {
    naTest(naMakeEndWithStartAndLength(5., 10.) == 15.);
    naTestError(naMakeEndWithStartAndLength(0., -1.));
    naTestError(naMakeEndWithStartAndLength(0., NA_NAN));
    naTest(naMakeEndWithStartAndLengthf(5.f, 10.f) == 15.f);
    naTestError(naMakeEndWithStartAndLengthf(0.f, -1.f));
    naTestError(naMakeEndWithStartAndLengthf(0.f, NA_NANf));
    naTest(naMakeEndWithStartAndLengthi32(5, 10) == 15);
    naTestError(naMakeEndWithStartAndLengthi32(0, -1));
    naTestError(naMakeEndWithStartAndLengthi32(NA_MAX_i32, 1));
    naTest(naMakeEndWithStartAndLengthi64(5, 10) == 15);
    naTestError(naMakeEndWithStartAndLengthi64(0, -1));
    naTestError(naMakeEndWithStartAndLengthi64(NA_MAX_i64, 1));
    naTest(naMakeEndWithStartAndLengths(5, 10) == 15);
    naTestError(naMakeEndWithStartAndLengths(1, NA_MAX_s));
  }

  naTestGroup("Make Max with Min and Length") {
    naTest(naMakeMaxWithMinAndLengthi32(5, 10) == 14);
    naTestError(naMakeMaxWithMinAndLengthi32(0, -1));
    naTestError(naMakeMaxWithMinAndLengthi32(NA_MAX_i32, 1));
    naTestError(naMakeMaxWithMinAndLengthi32(NA_MAX_i32, 2));
    naTest(naMakeMaxWithMinAndLengthi64(5, 10) == 14);
    naTestError(naMakeMaxWithMinAndLengthi64(0, -1));
    naTestError(naMakeMaxWithMinAndLengthi64(NA_MAX_i64, 1));
    naTestError(naMakeMaxWithMinAndLengthi64(NA_MAX_i64, 2));
    naTest(naMakeMaxWithMinAndLengths(5, 10) == 14);
    naTest(naMakeMaxWithMinAndLengths(0, NA_MAX_s) == NA_MAX_s - 1);
    naTestError(naMakeMaxWithMinAndLengths(1, NA_MAX_s));
  }

  naTestGroup("Make Length with Start and End") {
    naTest(naMakeLengthWithStartAndEnd(5., 10.) == 5.);
    naTest(naMakeLengthWithStartAndEnd(5., 5.) == 0.);
    naTest(naMakeLengthWithStartAndEndf(5.f, 10.f) == 5.f);
    naTest(naMakeLengthWithStartAndEndf(5.f, 5.f) == 0.f);
    naTest(naMakeLengthWithStartAndEndi32(5, 10) == 5);
    naTest(naMakeLengthWithStartAndEndi32(5, 5) == 0);
    naTest(naMakeLengthWithStartAndEndi64(5, 10) == 5);
    naTest(naMakeLengthWithStartAndEndi64(5, 5) == 0);
    naTest(naMakeLengthWithStartAndEnds(5, 10) == 5);
    naTest(naMakeLengthWithStartAndEnds(5, 5) == 0);
    naTestError(naMakeLengthWithStartAndEnd(10., 5.));
    naTestError(naMakeLengthWithStartAndEnd(5., NA_NAN));
    naTestError(naMakeLengthWithStartAndEndf(10.f, 5.f));
    naTestError(naMakeLengthWithStartAndEndf(5.f, NA_NANf));
    naTestError(naMakeLengthWithStartAndEndi32(50, 10));
    naTestError(naMakeLengthWithStartAndEndi32(-1, NA_MAX_i32));
    naTestError(naMakeLengthWithStartAndEndi64(50, 10));
    naTestError(naMakeLengthWithStartAndEndi64(-1, NA_MAX_i64));
    naTestError(naMakeLengthWithStartAndEnds(50, 10));
  }

  naTestGroup("Make Length with Start and End E variants") {
    naTest(naMakeLengthWithStartAndEndE(5., 10.) == 5.);
    naTest(naMakeLengthWithStartAndEndE(5., 5.) == 0.);
    naTest(naMakeLengthWithStartAndEndfE(5.f, 10.f) == 5.f);
    naTest(naMakeLengthWithStartAndEndfE(5.f, 5.f) == 0.f);
    naTest(naMakeLengthWithStartAndEndi32E(5, 10) == 5);
    naTest(naMakeLengthWithStartAndEndi32E(5, 5) == 0);
    naTest(naMakeLengthWithStartAndEndi64E(5, 10) == 5);
    naTest(naMakeLengthWithStartAndEndi64E(5, 5) == 0);
    naTest(naMakeLengthWithStartAndEndsE(5, 10) == 5);
    naTest(naMakeLengthWithStartAndEndsE(5, 5) == 0);
    naTestVoid(naMakeLengthWithStartAndEndE(10., 5.));
    naTestError(naMakeLengthWithStartAndEndE(5., NA_NAN));
    naTestVoid(naMakeLengthWithStartAndEndfE(10.f, 5.f));
    naTestError(naMakeLengthWithStartAndEndfE(5.f, NA_NANf));
    naTestVoid(naMakeLengthWithStartAndEndi32E(50, 10));
    naTestError(naMakeLengthWithStartAndEndi32E(-1, NA_MAX_i32));
    naTestVoid(naMakeLengthWithStartAndEndi64E(50, 10));
    naTestError(naMakeLengthWithStartAndEndi64E(-1, NA_MAX_i64));
    naTestVoid(naMakeLengthWithStartAndEndsE(50, 10));
  }

  naTestGroup("Make Length with Min and Max") {
    naTest(naMakeLengthWithMinAndMaxi32(5, 10) == 6);
    naTest(naMakeLengthWithMinAndMaxi32(5, 5) == 1);
    naTest(naMakeLengthWithMinAndMaxi64(5, 10) == 6);
    naTest(naMakeLengthWithMinAndMaxi64(5, 5) == 1);
    naTestError(naMakeLengthWithMinAndMaxi32(0, NA_MAX_i32));
    naTestError(naMakeLengthWithMinAndMaxi32(10, 5));
    naTestError(naMakeLengthWithMinAndMaxi32(-1, NA_MAX_i32 - 1));
    naTestError(naMakeLengthWithMinAndMaxi64(0, NA_MAX_i64));
    naTestError(naMakeLengthWithMinAndMaxi64(10, 5));
    naTestError(naMakeLengthWithMinAndMaxi64(-1, NA_MAX_i64 - 1));
    naTest(naMakeLengthWithMinAndMaxs(5, 10) == 6);
    naTest(naMakeLengthWithMinAndMaxs(5, 5) == 1);
    naTestError(naMakeLengthWithMinAndMaxs(0, NA_MAX_s));
    naTestError(naMakeLengthWithMinAndMaxs(10, 5));
  }
}



void testAlign(void){
  naTestGroup("naAlignValuei32"){
    naTest(naAlignValuei32(0, 0, 20) == 0);
    naTest(naAlignValuei32(5, 0, 20) == 0);
    naTest(naAlignValuei32(65, 0, 20) == 60);
    naTest(naAlignValuei32(-75, 0, 20) == -80);
    naTest(naAlignValuei32(10, 10, 20) == 10);
    naTest(naAlignValuei32(55, 10, 20) == 50);
    naTest(naAlignValuei32(5, 10, 20) == -10);
    naTestError(naAlignValuei32(5, 0, -20));
    naTestError(naAlignValuei32(5, NA_MIN_i32, 20));
    naTestError(naAlignValuei32(-5, NA_MAX_i32, 20));
  }

  naTestGroup("naAlignValuei64"){
    naTest(naAlignValuei64(0, 0, 20) == 0);
    naTest(naAlignValuei64(5, 0, 20) == 0);
    naTest(naAlignValuei64(65, 0, 20) == 60);
    naTest(naAlignValuei64(-75, 0, 20) == -80);
    naTest(naAlignValuei64(10, 10, 20) == 10);
    naTest(naAlignValuei64(55, 10, 20) == 50);
    naTest(naAlignValuei64(5, 10, 20) == -10);
    naTestError(naAlignValuei64(5, 0, -20));
    naTestError(naAlignValuei64(5, NA_MIN_i64, 20));
    naTestError(naAlignValuei64(-5, NA_MAX_i64, 20));
  }

  naTestGroup("naAlignValuef") {
    naTest(naAlignValuef(0.f, 0.f, 20.f) == 0.f);
    naTest(naAlignValuef(5.f, 0.f, 20.f) == 0.f);
    naTest(naAlignValuef(65.f, 0.f, 20.f) == 60.f);
    naTest(naAlignValuef(-75.f, 0.f, 20.f) == -80.f);
    naTest(naAlignValuef(10.f, 10.f, 20.f) == 10.f);
    naTest(naAlignValuef(55.f, 10.f, 20.f) == 50.f);
    naTest(naAlignValuef(5.f, 10.f, 20.f) == -10.f);
    naTestError(naAlignValuef(5.f, 0.f, -20.f));
    naTestError(naAlignValuef(5.f, -NA_INFINITYf, 20.f));
    naTestError(naAlignValuef(-5.f, NA_INFINITYf, 20.f));
  }

  naTestGroup("naAlignValuei64") {
    naTest(naAlignValuei64(0, 0, 20) == 0);
    naTest(naAlignValuei64(5, 0, 20) == 0);
    naTest(naAlignValuei64(65, 0, 20) == 60);
    naTest(naAlignValuei64(-75, 0, 20) == -80);
    naTest(naAlignValuei64(10, 10, 20) == 10);
    naTest(naAlignValuei64(55, 10, 20) == 50);
    naTest(naAlignValuei64(5, 10, 20) == -10);
    naTestError(naAlignValuei64(5, 0, -20));
    naTestError(naAlignValuei64(5, NA_MIN_i64, 20));
    naTestError(naAlignValuei64(-5, NA_MAX_i64, 20));
  }

  naTestGroup("naAlignValuei32") {
    naTest(naAlignValuei32(0, 0, 20) == 0);
    naTest(naAlignValuei32(5, 0, 20) == 0);
    naTest(naAlignValuei32(65, 0, 20) == 60);
    naTest(naAlignValuei32(-75, 0, 20) == -80);
    naTest(naAlignValuei32(10, 10, 20) == 10);
    naTest(naAlignValuei32(55, 10, 20) == 50);
    naTest(naAlignValuei32(5, 10, 20) == -10);
    naTestError(naAlignValuei32(5, 0, -20));
    naTestError(naAlignValuei32(5, NA_MIN_i32, 20));
    naTestError(naAlignValuei32(-5, NA_MAX_i32, 20));
  }

  naTestGroup("naAlignValues") {
    // The commented out tests will not work as size_t is unsigned.
    naTest(naAlignValues(0, 0, 20) == 0);
    naTest(naAlignValues(5, 0, 20) == 0);
    naTest(naAlignValues(65, 0, 20) == 60);
    //naTest(naAlignValues(-75, 0, 20) == -80);
    naTest(naAlignValues(10, 10, 20) == 10);
    naTest(naAlignValues(55, 10, 20) == 50);
    //naTest(naAlignValues(5, 10, 20) == -10);
    naTestError(naAlignValues(5, 0, -20));
    naTestError(naAlignValues(-5, NA_MAX_s, 20));
  }
}



void testIsValueValid(void) {
  naTestGroup("naIsOffsetValueValid") {
      naTest(naIsOffsetValueValid(0.));
      naTest(naIsOffsetValueValid(NA_INFINITY));
      naTest(naIsOffsetValueValid(-NA_INFINITY));
      naTest(!naIsOffsetValueValid(NA_NAN));
  }
  naTestGroup("naIsOffsetValueValidf") {
      naTest(naIsOffsetValueValidf(0.f));
      naTest(naIsOffsetValueValidf(NA_INFINITYf));
      naTest(naIsOffsetValueValidf(-NA_INFINITYf));
      naTest(!naIsOffsetValueValidf(NA_NANf));
  }
  naTestGroup("naIsOffsetValueValidi32") {
    naTest(naIsOffsetValueValidi32(0));
    naTest(naIsOffsetValueValidi32(NA_MAX_i32));
    naTest(naIsOffsetValueValidi32(NA_MIN_i32));
  }
  naTestGroup("naIsOffsetValueValidi64") {
    naTest(naIsOffsetValueValidi64(0));
    naTest(naIsOffsetValueValidi64(NA_MAX_i64));
    naTest(naIsOffsetValueValidi64(NA_MIN_i64));
  }
  naTestGroup("naIsOffsetValueValids") {
      naTest(naIsOffsetValueValids(0));
      naTestError(naIsOffsetValueValids(NA_MAX_s));
  }

  naTestGroup("naIsLengthValueValid") {
      naTest(naIsLengthValueValid(0.));
      naTest(naIsLengthValueValid(NA_INFINITY));
      naTest(naIsLengthValueValid(-NA_INFINITY));
      naTest(!naIsLengthValueValid(NA_NAN));
  }
  naTestGroup("naIsLengthValueValidf") {
      naTest(naIsLengthValueValidf(0.f));
      naTest(naIsLengthValueValidf(NA_INFINITYf));
      naTest(naIsLengthValueValidf(-NA_INFINITYf));
      naTest(!naIsLengthValueValidf(NA_NANf));
  }
  naTestGroup("naIsLengthValueValidi32") {
    naTest(naIsLengthValueValidi32(0));
    naTest(naIsLengthValueValidi32(NA_MAX_i32));
    naTest(naIsLengthValueValidi32(NA_MIN_i32));
  }
  naTestGroup("naIsLengthValueValidi64") {
    naTest(naIsLengthValueValidi64(0));
    naTest(naIsLengthValueValidi64(NA_MAX_i64));
    naTest(naIsLengthValueValidi64(NA_MIN_i64));
  }
  naTestGroup("naIsLengthValueValids") {
      naTest(naIsLengthValueValids(0));
      naTestError(naIsLengthValueValids(NA_MAX_s));
  }
}



void testIsValueEmpty(void) {
  naTestGroup("naIsLengthValueEmpty") {
      naTest(naIsLengthValueEmpty(0.));
      naTest(!naIsLengthValueEmpty(1.));
      naTest(!naIsLengthValueEmpty(-1.));
      naTest(!naIsLengthValueEmpty(NA_NAN));
  }
  naTestGroup("naIsLengthValueEmptyf") {
      naTest(naIsLengthValueEmptyf(0.f));
      naTest(!naIsLengthValueEmptyf(1.f));
      naTest(!naIsLengthValueEmptyf(-1.f));
      naTest(!naIsLengthValueEmptyf(NA_NANf));
  }
  naTestGroup("naIsLengthValueEmptyi32") {
    naTest(naIsLengthValueEmptyi32(0));
    naTest(!naIsLengthValueEmptyi32(1));
    naTest(!naIsLengthValueEmptyi32(-1));
  }
  naTestGroup("naIsLengthValueEmptyi64") {
    naTest(naIsLengthValueEmptyi64(0));
    naTest(!naIsLengthValueEmptyi64(1));
    naTest(!naIsLengthValueEmptyi64(-1));
  }
  naTestGroup("naIsLengthValueEmptys") {
      naTest(naIsLengthValueEmptys(0));
      naTest(!naIsLengthValueEmptys(1));
      naTestError(naIsLengthValueEmptys(NA_MAX_s));
  }
}



void testIsValueNegative(void) {
  naTestGroup("naIsLengthValueNegative") {
      naTest(!naIsLengthValueNegative(0.));
      naTest(!naIsLengthValueNegative(1.));
      naTest(naIsLengthValueNegative(-1.));
      naTest(!naIsLengthValueNegative(NA_INFINITY));
      naTest(naIsLengthValueNegative(-NA_INFINITY));
      naTest(!naIsLengthValueNegative(NA_NAN));
  }
  naTestGroup("naIsLengthValueNegativef") {
      naTest(!naIsLengthValueNegativef(0.f));
      naTest(!naIsLengthValueNegativef(1.f));
      naTest(naIsLengthValueNegativef(-1.f));
      naTest(!naIsLengthValueNegativef(NA_INFINITYf));
      naTest(naIsLengthValueNegativef(-NA_INFINITYf));
      naTest(!naIsLengthValueNegativef(NA_NANf));
  }
  naTestGroup("naIsLengthValueNegativei32") {
    naTest(!naIsLengthValueNegativei32(0));
    naTest(!naIsLengthValueNegativei32(1));
    naTest(naIsLengthValueNegativei32(-1));
    naTest(!naIsLengthValueNegativei32(NA_MAX_i32));
    naTest(naIsLengthValueNegativei32(NA_MIN_i32));
  }
  naTestGroup("naIsLengthValueNegativei64") {
    naTest(!naIsLengthValueNegativei64(0));
    naTest(!naIsLengthValueNegativei64(1));
    naTest(naIsLengthValueNegativei64(-1));
    naTest(!naIsLengthValueNegativei64(NA_MAX_i64));
    naTest(naIsLengthValueNegativei64(NA_MIN_i64));
  }
  naTestGroup("naIsLengthValueNegatives") {
      naTest(!naIsLengthValueNegatives(0));
      naTest(!naIsLengthValueNegatives(1));
      naTestError(naIsLengthValueNegatives(NA_MAX_s));
  }
}



void testIsValueUseful(void) {
  naTestGroup("naIsOffsetValueUseful") {
      naTest(naIsOffsetValueUseful(0.));
      naTest(naIsOffsetValueUseful(1.));
      naTest(naIsOffsetValueUseful(-1.));
      naTest(!naIsOffsetValueUseful(NA_INFINITY));
      naTest(!naIsOffsetValueUseful(-NA_INFINITY));
      naTest(!naIsOffsetValueUseful(NA_NAN));
  }
  naTestGroup("naIsOffsetValueUsefulf") {
      naTest(naIsOffsetValueUsefulf(0.f));
      naTest(naIsOffsetValueUsefulf(1.f));
      naTest(naIsOffsetValueUsefulf(-1.f));
      naTest(!naIsOffsetValueUsefulf(NA_INFINITYf));
      naTest(!naIsOffsetValueUsefulf(-NA_INFINITYf));
      naTest(!naIsOffsetValueUsefulf(NA_NANf));
  }
  naTestGroup("naIsOffsetValueUsefuli32") {
    naTest(naIsOffsetValueUsefuli32(0));
    naTest(naIsOffsetValueUsefuli32(1));
    naTest(naIsOffsetValueUsefuli32(-1));
    naTest(naIsOffsetValueUsefuli32(NA_MAX_i32));
    naTest(naIsOffsetValueUsefuli32(NA_MIN_i32));
  }
  naTestGroup("naIsOffsetValueUsefuli64") {
    naTest(naIsOffsetValueUsefuli64(0));
    naTest(naIsOffsetValueUsefuli64(1));
    naTest(naIsOffsetValueUsefuli64(-1));
    naTest(naIsOffsetValueUsefuli64(NA_MAX_i64));
    naTest(naIsOffsetValueUsefuli64(NA_MIN_i64));
  }
  naTestGroup("naIsOffsetValueUsefuls") {
      naTest(naIsOffsetValueUsefuls(0));
      naTest(naIsOffsetValueUsefuls(1));
      naTestError(naIsOffsetValueUsefuls(NA_MAX_s));
  }

  naTestGroup("naIsLengthValueUseful") {
      naTest(!naIsLengthValueUseful(0.));
      naTest(naIsLengthValueUseful(1.));
      naTest(!naIsLengthValueUseful(-1.));
      naTest(!naIsLengthValueUseful(NA_INFINITY));
      naTest(!naIsLengthValueUseful(-NA_INFINITY));
      naTest(!naIsLengthValueUseful(NA_NAN));
  }
  naTestGroup("naIsLengthValueUsefulf") {
      naTest(!naIsLengthValueUsefulf(0.f));
      naTest(naIsLengthValueUsefulf(1.f));
      naTest(!naIsLengthValueUsefulf(-1.f));
      naTest(!naIsLengthValueUsefulf(NA_INFINITYf));
      naTest(!naIsLengthValueUsefulf(-NA_INFINITYf));
      naTest(!naIsLengthValueUsefulf(NA_NANf));
  }
  naTestGroup("naIsLengthValueUsefuli32") {
    naTest(!naIsLengthValueUsefuli32(0));
    naTest(naIsLengthValueUsefuli32(1));
    naTest(!naIsLengthValueUsefuli32(-1));
    naTest(naIsLengthValueUsefuli32(NA_MAX_i32));
    naTest(!naIsLengthValueUsefuli32(NA_MIN_i32));
  }
  naTestGroup("naIsLengthValueUsefuli64") {
    naTest(!naIsLengthValueUsefuli64(0));
    naTest(naIsLengthValueUsefuli64(1));
    naTest(!naIsLengthValueUsefuli64(-1));
    naTest(naIsLengthValueUsefuli64(NA_MAX_i64));
    naTest(!naIsLengthValueUsefuli64(NA_MIN_i64));
  }
  naTestGroup("naIsLengthValueUsefuls") {
      naTest(!naIsLengthValueUsefuls(0));
      naTest(naIsLengthValueUsefuls(1));
      naTestError(naIsLengthValueUsefuls(NA_MAX_s));
  }
}



void testNAValueHelper(void) {
  naTestFunction(testFlags);
  naTestFunction(testRange);
  naTestFunction(testAlign);
  naTestFunction(testIsValueValid);
  naTestFunction(testIsValueEmpty);
  naTestFunction(testIsValueNegative);
  naTestFunction(testIsValueUseful);
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
