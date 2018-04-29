
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"



// NALib does not provide macros for the kind of compiler is used (like for
// example C, C++ or Objective-C). Please check for definition of the default
// macros:
//
// __cplusplus    Compiler is a C++ compiler
// __OBJC__       Compiler is an Objective-C Compiler




// On the other hand, NALib detects, what c or c++ standard is used. It might
// be one or multiple of the following:
//
// NA_C89
// NA_C90
// NA_C94
// NA_C99
// NA_C11
//
// NA_CPP98
// NA_CPP11
// NA_CPP14
//
// If a later version is defined, all earlier versions are defined as well.
#ifndef __cplusplus
  // This is a C compilation
  #if defined __STDC__

    #define NA_C89  // C89 and C90 are virtually the same thing.
    #define NA_C90

    #if defined __STDC_VERSION__
      #if __STDC_VERSION__ >= 199409L
        #define NA_C94
      #endif
      #if __STDC_VERSION__ >= 199901L
        #define NA_C99
      #endif
      #if __STDC_VERSION__ >= 201112L
        #define NA_C11
      #endif
    #endif

  #endif

#else
  // This is a C++ compilation
  #if __cplusplus >= 199711L
    #define NA_CPP98  // The ISO ratified the November '97 version in '98
  #endif
  #if __cplusplus >= 201103L
    #define NA_CPP11
  #endif
  #if __cplusplus >= 201402L
    #define NA_CPP14
  #endif
#endif



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
