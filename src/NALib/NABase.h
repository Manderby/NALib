
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_BASE_INCLUDED
#define NA_BASE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This is the base of the whole NALib implementation. Pretty much all files
// need this file. It contains the most basic type definitions, functions and
// preprocessor macros.



// The version number of this NALib distribution.
#define NA_VERSION 22



// Include the configuration of NALib.
// Have a look at this file to find out how you can configure NALib by adding
// preprocessor macros to your compiler.
#include "NAConfiguration.h"

// Gather information about where, how and what to compile.
#include "NABase/NAEnvironment.h"
#include "NABase/NACompiler.h"
#include "NABase/NALanguage.h"

// declaring numeric base types and related stuff
#include "NABase/NAInteger.h"
#include "NABase/NAFloatingPoint.h"
#include "NABase/NANumerics.h"

// Enabling the code to use debugger runtime testing (DRT)
#include "NABase/NADebug.h"

// Now, including implementations of the most basic kind...
#include "NABase/NAInt64II.h"
#include "NABase/NAFloatingPointII.h"
#include "NABase/NANumericsII.h"

// Additional definitions
#include "NABase/NAChar.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BASE_INCLUDED

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
