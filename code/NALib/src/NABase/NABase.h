
#ifndef NA_BASE_INCLUDED
#define NA_BASE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// //////////////////////////
// All NALib functionality is based on this file. All NALib files include
// this file as their first action.
//
// Note that this file only includes the base definitions. If you need more
// functionality, for example NAArray, you have to explicitely include the
// appropriate header file yourself.
// //////////////////////////



// The version number of this NALib distribution.
#define NA_VERSION 26



// Include the configuration of NALib.
// Have a look at this file to find out how you can configure NALib by adding
// preprocessor macros to your compiler.
#include "NAConfiguration.h"

// Gather information about where, how and what to compile.
#include "NAEnvironment.h"
#include "NACompiler.h"
#include "NALanguage.h"

// declaring base types and related stuff
#include "NAEncoding.h"
#include "NAChar.h"
#include "NAInteger.h"

// declaring integer emulation
#include "NAInt64.h"
#include "NAInt128.h"
#include "NAInt256.h"

// Some helping macros and debugging
#include "NAMacros.h"
#include "NADebugging.h"

// Adding some base mathematical implementations
#include "NAPointerArithmetics.h"
#include "NANumerics.h"
#include "NAFloatingPoint.h"

// Adding inline implementations of numerical types
#include "Core/NAInt64II.h"
#include "Core/NAInt128II.h"
#include "Core/NAInt256II.h"
#include "Core/NANumericsII.h"
#include "Core/NAFloatingPointII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BASE_INCLUDED



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
