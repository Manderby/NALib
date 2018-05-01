
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//

#include "NABuffer.h"
#include "NAFile.h"
#include "NADateTime.h"

#include "../NALib/NABase.h"
#include <stdio.h>

//int main(void){
//  printf("NALib Version: %d ", NA_VERSION);
//  #ifndef NDEBUG
//    printf("(Debug ");
//  #else
//    printf("(Release ");
//  #endif
//  printf("%d Bits Addresses, %d Bits Integers)\n", NA_SYSTEM_ADDRESS_BITS, NA_TYPE_NAINT_BITS);
//
//  #if NA_OS == NA_OS_WINDOWS
//   NA_UNUSED(getchar());
//  #endif
//  
//  naStartRuntime();
//
////  int32 a = 2000000000;
////  int32 b = 4;
////  int64 res = a * b;
////  printf("%lld", res);
//
////  NAString* test = naNewString();
////  naDelete(test);
//
//  naMakeDateTimeNow();
//  printf("%d\n", NA_TYPE_NATIVE_CHAR_BITS);
//  printf("%d\n", NA_TYPE_NATIVE_SHORT_INT_BITS);
//  printf("%d\n", NA_TYPE_NATIVE_INT_BITS);
//  printf("%d\n", NA_TYPE_NATIVE_LONG_INT_BITS);
//  #if defined NA_TYPE_NATIVE_LONG_LONG_INT_BITS
//    printf("%d\n", NA_TYPE_NATIVE_LONG_LONG_INT_BITS);
//  #else
//    printf("No long long available\n");
//  #endif
//
//  printf("%d\n", NA_TYPE_INT8);
//  printf("%d\n", NA_TYPE_INT16);
//  printf("%d\n", NA_TYPE_INT32);
//  #if defined NA_TYPE_INT64
//    printf("%d\n", NA_TYPE_INT64);
//  #else
//    printf("No int64 available\n");
//  #endif
//
////  printf("%d\n", NA_IEEE754_SINGLE_BITS);
////  printf("%d\n", NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
////  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_SIGNIFICAND_NORM);
////  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
////  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_BITS);
////  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_BIAS);
////  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL);
////  printf("%d\n", NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
////  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_EXPONENT_MASK);
////  printf("%08" NA_PRIx32 "\n", NA_IEEE754_SINGLE_SIGN_MASK);
////
////printf("\n");
////
////  printf("%d\n", NA_IEEE754_DOUBLE_BITS);
////  printf("%d\n", NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
////  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
////  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
////  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_BITS);
////  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_BIAS);
////  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL);
////  printf("%d\n", NA_IEEE754_DOUBLE_EXPONENT_SPECIAL);
////  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_EXPONENT_MASK);
////  printf("%016" NA_PRIx64 "\n", NA_IEEE754_DOUBLE_SIGN_MASK);
////  
////  float testfloat = naMakeFloat(1, NA_IEEE754_SINGLE_EXPONENT_SPECIAL);
////  printf("%e\n", testfloat);
////  double testdouble = naMakeDoubleWithExponent(NA_IEEE754_DOUBLE_EXPONENT_SPECIAL-1);
////  printf("%e\n", testdouble);
////  double testdouble = naMakeDoubleSubnormal(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
////  printf("%e\n", testdouble);
////  double testdouble = naPowerOf2d(4);
////  printf("%f\n", testdouble);
//  
////  NABuffer* buf = naNewBufferWithInpuFile("test.txt");
////  NABufferIterator iter = naMakeBufferAccessor(buf);
////  naSeekBufferFromStart(&iter, 0);
////  printf("%d\n", naParseBufferInt32(&iter, NA_FALSE));
////  naClearBufferIterator(&iter);
////  naRelease(buf);
//  
//  naStopRuntime();
//  
//  return 0;
//}
//
//
//// Copyright (c) NALib, Tobias Stamm
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the source-code inherently
//// dependent on this software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
