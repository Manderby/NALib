
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// /////////////////////////////////
// Debugging:
//
// The following definitions will only be defined if NDEBUG is undefined.
// Many functions of NALib will perform many tests which will slow down
// the performance more or less considerably. If NDEBUG is defined however,
// no tests are performed whatsoever.
//
// When it comes to debugging, the macro NDEBUG is the only one being somewhat
// standardized across all systems and compilers. It literally tells the
// compiler to NOT-DEBUG, which means: When the macro is undefined, code
// encapsulated in #ifndef NDEBUG will be compiled, otherwise not.
//
// Usually in IDEs (Integrated Development Environments), there are at least
// two configurations per application: Debug and Release, sometimes also called
// Development and Deployment. The Release-Configuration usually automatically
// has the NDEBUG macro defined as a preprocessor build option. Not every IDE
// does this by default though, so you might have to add it manually to the
// build options.
//
// Note that in NALib, all tests performed by code encapsulated by
// #ifndef NDEBUG will emit errors on sterr in the following format:
// Function_name: Errormessage NEWLINE
// Therefore, a programmer can simply set a breakpoint in the denoted function
// and start debugging.
//
// If due to some reason, setting a breakpoint does not work in the desired
// function, you can simply set a breakpoint in the naError function.
// This function is explicitely NOT inlined and is defined in a separate
// implementation file (and therefore a separate translation unit) due to that
// reason:
//
// NADebug.c
//
// Also note that in NALib, code executed within NDEBUG will not alter the
// runtime execution. In other words: Except from speed differences and outputs
// to strerr, NALib will NOT behave differently depending on NDEBUG being
// defined or not. The only remark is that calls to naCrash will eventually
// lead to an exit(EXIT_FAILURE) call before the real error is executed.
// If you use code sanity checking (for example clang analyzer) you should
// therefore always check both with and without NDEBUG defined. Otherwise,
// the analyzer might miss some errors.

#ifndef NDEBUG

  #include <stdio.h>    // for the fprintf function
  #include <stdlib.h>   // for the exit function 

  // Prints an error. When this function gets called, the ongoing of the
  // application is undefined. Sometimes, the error might affect everything
  // which comes after it, sometimes, the error will just result in a NaN or
  // even be corrected automatically. Nontheless, any error should be
  // considered a potential risk for the application to eventually crash.
  void naError(const char* functionsymbol, const char* text);


  // Prints a crash message.
  // This macro is used before the application experiences a critical error
  // like dereferencing an invalid pointer. The application would almost
  // certainly crash few steps after this function call. Therefore, it
  // is crashed deliberately with an exit call.
  //
  // Note that this is a macro, not a function. This had been changed in
  // NALib version 21 because different code analysis tools behaved differently
  // in different C standards and on different systems. Some did not detected
  // the deliberate crash (the exit call) and would assume, the calling
  // function would continue to run hence running into the same issues
  // as before and outputting warnings. This had been solved using explicit
  // return statements after the naCrash call. But other tools detected that
  // exit call and warned about the additional return statement that it will
  // not be executed and hence outputting some warnings as well.
  //
  // With a macro, the exit call is directly copied to the place the error
  // occurs and all code analysis tools are silented.
    
  #define naCrash(functionsymbol, text)\
    {\
    naError(functionsymbol, text);\
    fprintf(stderr, NA_NL "Crashing the application deliberately..." NA_NL);\
    exit(EXIT_FAILURE);\
    }

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
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.