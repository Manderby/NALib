
#include "NATesting.h"
#include <stdio.h>

#include "NAValueHelper.h"

void testFlags(){
  int flags = 0xcccccccc;          // 0b11001100110011001100110011001100
  int testFlag1 = 0x00000001;      // not set
  int testFlag2 = 0x00000004;      // set
  int testMultiFlag1 = 0x00000104; // not all set
  int testMultiFlag2 = 0x00000404; // all set
  NAInt flagsi = 0xcccccccc;
  NAInt testFlagi1 = 0x00000001;
  NAInt testFlagi2 = 0x00000004;
  NAInt testMultiFlagi1 = 0x00000104;
  NAInt testMultiFlagi2 = 0x00000404;
  NAUInt flagsu = 0xcccccccc;
  NAUInt testFlagu1 = 0x00000001;
  NAUInt testFlagu2 = 0x00000004;
  NAUInt testMultiFlagu1 = 0x00000104;
  NAUInt testMultiFlagu2 = 0x00000404;

  naTestGroup("Querying int flags"){
    naTest(!naGetFlag(flags, testFlag1));
    naTest( naGetFlag(flags, testFlag2));
    naTest(!naGetFlag(flags, testMultiFlag1));
    naTest( naGetFlag(flags, testMultiFlag2));
  }
  naTestGroup("Querying NAInt flags"){
    naTest(!naGetFlagi(flagsi, testFlagi1));
    naTest( naGetFlagi(flagsi, testFlagi2));
    naTest(!naGetFlagi(flagsi, testMultiFlagi1));
    naTest( naGetFlagi(flagsi, testMultiFlagi2));
  }
  naTestGroup("Querying NAUInt flags"){
    naTest(!naGetFlagu(flagsu, testFlagu1));
    naTest( naGetFlagu(flagsu, testFlagu2));
    naTest(!naGetFlagu(flagsu, testMultiFlagu1));
    naTest( naGetFlagu(flagsu, testMultiFlagu2));
  }

  naTestGroup("Setting int flags"){
    naTest((naSetFlag(&flags, testFlag1, NA_TRUE), flags == 0xcccccccd));
    naTest((naSetFlag(&flags, testFlag1, NA_FALSE), flags == 0xcccccccc));
    naTest((naSetFlag(&flags, testFlag2, NA_TRUE), flags == 0xcccccccc));
    naTest((naSetFlag(&flags, testMultiFlag1, NA_TRUE), flags == 0xcccccdcc));
    naTest((naSetFlag(&flags, testMultiFlag1, NA_FALSE), flags == 0xccccccc8));
    naTest((naSetFlag(&flags, testMultiFlag2, NA_TRUE), flags == 0xcccccccc));
    //naTestCrash(naSetFlag(NA_NULL, 1234, NA_TRUE));
    naTestError(naSetFlag(&flags, testFlag1, 1234));
  }
  naTestGroup("Setting NAInt flags"){
    naTest((naSetFlagi(&flagsi, testFlagi1, NA_TRUE), flagsi == 0xcccccccd));
    naTest((naSetFlagi(&flagsi, testFlagi1, NA_FALSE), flagsi == 0xcccccccc));
    naTest((naSetFlagi(&flagsi, testFlagi2, NA_TRUE), flagsi == 0xcccccccc));
    naTest((naSetFlagi(&flagsi, testMultiFlagi1, NA_TRUE), flagsi == 0xcccccdcc));
    naTest((naSetFlagi(&flagsi, testMultiFlagi1, NA_FALSE), flagsi == 0xccccccc8));
    naTest((naSetFlagi(&flagsi, testMultiFlagi2, NA_TRUE), flagsi == 0xcccccccc));
    //naTestCrash(naSetFlagi(NA_NULL, 1234, NA_TRUE));
    naTestError(naSetFlagi(&flagsi, testFlagi1, 1234));
  }
  naTestGroup("Setting NAUInt flags"){
    naTest((naSetFlagu(&flagsu, testFlagu1, NA_TRUE), flagsu == 0xcccccccd));
    naTest((naSetFlagu(&flagsu, testFlagu1, NA_FALSE), flagsu == 0xcccccccc));
    naTest((naSetFlagu(&flagsu, testFlagu2, NA_TRUE), flagsu == 0xcccccccc));
    naTest((naSetFlagu(&flagsu, testMultiFlagu1, NA_TRUE), flagsu == 0xcccccdcc));
    naTest((naSetFlagu(&flagsu, testMultiFlagu1, NA_FALSE), flagsu == 0xccccccc8));
    naTest((naSetFlagu(&flagsu, testMultiFlagu2, NA_TRUE), flagsu == 0xcccccccc));
    //naTestCrash(naSetFlagu(NA_NULL, 1234, NA_TRUE));
    naTestError(naSetFlagu(&flagsu, testFlagu1, 1234));
  }

  naTestGroup("Toggling int flags"){
    naTest( naToggleFlag(&flags, testFlag1));
    naTest(!naToggleFlag(&flags, testFlag1));
    naTest(!naToggleFlag(&flags, testFlag2));
    naTest( naToggleFlag(&flags, testFlag2));
    naTest(!naToggleFlag(&flags, testMultiFlag1));
    naTest(!naToggleFlag(&flags, testMultiFlag1));
    naTest(!naToggleFlag(&flags, testMultiFlag2));
    naTest( naToggleFlag(&flags, testMultiFlag2));
    //naTestCrash(naToggleFlag(NA_NULL, testFlag1, NA_TRUE));
  }
  naTestGroup("Toggling NAInt flags"){
    naTest( naToggleFlagi(&flagsi, testFlagi1));
    naTest(!naToggleFlagi(&flagsi, testFlagi1));
    naTest(!naToggleFlagi(&flagsi, testFlagi2));
    naTest( naToggleFlagi(&flagsi, testFlagi2));
    naTest(!naToggleFlagi(&flagsi, testMultiFlagi1));
    naTest(!naToggleFlagi(&flagsi, testMultiFlagi1));
    naTest(!naToggleFlagi(&flagsi, testMultiFlagi2));
    naTest( naToggleFlagi(&flagsi, testMultiFlagi2));
    //naTestCrash(naToggleFlagi(NA_NULL, testFlagi1, NA_TRUE));
  }
  naTestGroup("Toggling NAUInt flags"){
    naTest( naToggleFlagu(&flagsu, testFlagu1));
    naTest(!naToggleFlagu(&flagsu, testFlagu1));
    naTest(!naToggleFlagu(&flagsu, testFlagu2));
    naTest( naToggleFlagu(&flagsu, testFlagu2));
    naTest(!naToggleFlagu(&flagsu, testMultiFlagu1));
    naTest(!naToggleFlagu(&flagsu, testMultiFlagu1));
    naTest(!naToggleFlagu(&flagsu, testMultiFlagu2));
    naTest( naToggleFlagu(&flagsu, testMultiFlagu2));
    //naTestCrash(naToggleFlagu(NA_NULL, testFlagu1, NA_TRUE));
  }
}



void testRange(){
  naTestGroup("Make Max with End"){
    naTest(naMakeMaxWithEndi32(1234) == 1233);
    naTest(naMakeMaxWithEndi64(1234) == 1233);
    naTest(naMakeMaxWithEndi(1234) == 1233);
    naTestError(naMakeMaxWithEndi32(NA_MIN_i32));
    naTestError(naMakeMaxWithEndi64(NA_MIN_i64));
    naTest(naMakeEndWithMaxi32(1234) == 1235);
    naTest(naMakeEndWithMaxi64(1234) == 1235);
    naTest(naMakeEndWithMaxi(1234) == 1235);
    naTestError(naMakeEndWithMaxi32(NA_MAX_i32));
    naTestError(naMakeEndWithMaxi64(NA_MAX_i64));
  }

  naTestGroup("Make End with Start and Length"){
    naTest(naMakeEndWithStartAndLength(5., 10.) == 15.);
    naTestError(naMakeEndWithStartAndLength(0., NA_NAN));
    naTest(naMakeEndWithStartAndLengthf(5.f, 10.f) == 15.f);
    naTestError(naMakeEndWithStartAndLengthf(0.f, NA_NANf));
    naTest(naMakeEndWithStartAndLengthi(5, 10) == 15);
    naTestError(naMakeEndWithStartAndLengthi(NA_MIN_i, -1));
    naTestError(naMakeEndWithStartAndLengthi(NA_MAX_i, 1));
  }

  naTestGroup("Make Max with Min and Length"){
    naTest(naMakeMaxWithMinAndLengthi(5, 10) == 14);
    naTestError(naMakeMaxWithMinAndLengthi(NA_MIN_i, -1));
    naTestError(naMakeMaxWithMinAndLengthi(NA_MAX_i, 1));
  }

  naTestGroup("Make Length with Start and End"){
    naTest(naMakeLengthWithStartAndEnd(5., 10.) == 5.);
    naTest(naMakeLengthWithStartAndEndf(5.f, 10.f) == 5.f);
    naTest(naMakeLengthWithStartAndEndi(5, 10) == 5);
    naTestError(naMakeLengthWithStartAndEnd(5., NA_NAN));
    naTestError(naMakeLengthWithStartAndEndf(5.f, NA_NANf));
    naTestError(naMakeLengthWithStartAndEndi(NA_MIN_i, NA_MAX_i));
    naTestError(naMakeLengthWithStartAndEndi(NA_MAX_i, NA_MIN_i));
  }

  naTestGroup("Make Length with Start and End"){
    naTest(naMakeLengthWithMinAndMaxi(5, 10) == 9);
    //naTestError(naMakeLengthWithMinAndMaxi(5., 10.) == 9.);
  }
}



void testNAValueHelper(){
  naTestGroupFunction(Flags);  
  naTestGroupFunction(Range);  
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
