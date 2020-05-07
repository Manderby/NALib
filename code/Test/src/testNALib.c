
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "testNALib.h"
#include "testHelper.h"
#include NA_TEST_NALIB_PATH(./, NABase.h)
#include NA_TEST_NALIB_PATH(./, NAMemory.h)
#include <stdio.h>


int main(void){

  printf("NALib Version: %d (", NA_VERSION);
  #ifndef NDEBUG
    printf("Debug");
  #else
    printf("Release");
  #endif
  printf(", %d Bits Addresses, %d Bits Integers)" NA_NL NA_NL, NA_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();
  naStartTesting("NALib");

  //naT(NA_TRUE);
  //naT(NA_TRUE);
  //naT(NA_FALSE);
  //naT(NA_TRUE);

  //naG("Groupie"){
  //  naT(NA_TRUE);
  //  naT(NA_FALSE);
  //  naT(NA_FALSE);
  //  naT(NA_TRUE);
  //}

  //naG("Blah"){
  //  naG("Knuss"){
  //    naT(NA_TRUE);
  //    naT(NA_FALSE);
  //    naT(NA_FALSE);
  //    naT(NA_TRUE);
  //  }
  //  naG("Fluss"){
  //    naT(NA_TRUE);
  //    naT(NA_FALSE);
  //    naT(NA_FALSE);
  //    naT(NA_TRUE);
  //  }
  //}
  
  //printNAConfiguration();
  //printNAEnvironment();
  //printNACompiler();
  //printNALanguage();
  //printNAEncoding();
  //printNAChar();
  //printNANumerics();

  naF(NALanguage);
  naF(NAChar);
  naF(NANumerics);

  naStopTesting();
  naStopRuntime();

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

