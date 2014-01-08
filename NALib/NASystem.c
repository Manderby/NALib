
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NASystem.h"


// This is an example of how extern variables should be defined. For example,
// the na_system_strings variable is defined as extern in the NASystems.h file.
// But it is defined and hence instanciated (as a global variable) here.
//
// The header file will be included by potentially many other files but this
// implementation file will be compiled once, and only once. Hence, the
// variable will also be instanciated only once.
//
// Note for C++: Member statics must be defined with the appropriate Namespace.
// For example: MyClass::mystaticvar = 52;




#ifndef NDEBUGY

  // The error printing method. Errors will be emitted to the stderr output.
  // When NDEBUG is defined, these functions are OBSOLETE!
  #ifdef __cplusplus
    #include <cstdio>
  #else
    #include <stdio.h>
  #endif


  void naError(const char* functionsymbol, const char* message){
    // Set a breakpoint here, if everything fails.
    fprintf(stderr, "Error in %s: %s\n", functionsymbol, message);
  }



  void naCrash(const char* functionsymbol, const char* message){
    // Set a breakpoint here, if everything fails.
    fprintf(stderr, "Critical Error in %s: %s\n", functionsymbol, message);
    fprintf(stderr, "The application will likely crash...\n");
    // Uncomment the following line, if you want to crash deliberately. By
    // default, this line is commented out as it would change the behaviour of
    // NALib depending on whether NDEBUG is defined or not.
    //
    // exit(1);
    //
    // Note that in the code, where naCrash is called, there always exists a
    // return instruction afterwards. This helps the compiler and code sanity
    // checks to determine that a function can not crash, except deliberately.
  }

#endif


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
