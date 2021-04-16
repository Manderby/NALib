
#if defined NA_DEBUGGING_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_DEBUGGING_INCLUDED
#define NA_DEBUGGING_INCLUDED



// /////////////////////////////////
// Debugging:
//
// In this file, the naError and naCrash functions are defined which can be
// used for checking code inline at runtime. The use of these functions is
// dependent on the NA_DEBUG macro defined in the NAConfiguration.h file.
// 
// By default, NA_DEBUG is 1 when the NDEBUG macro is undefined.
// 
// When it comes to debugging, the macro NDEBUG is the only one being
// somewhat standardized across all systems and compilers. It literally tells
// the compiler to NOT-DEBUG.
//
// Usually in IDEs (Integrated Development Environments), there are at least
// two configurations per application: Debug and Release, sometimes also
// called Development and Deployment. The Release-Configuration usually
// automatically has the NDEBUG macro defined as a preprocessor build option.
// Not every IDE does this by default though, so you might have to add it
// manually to the build options.
//
// NALib does a ton of debugging checks even for the smallest functions. This
// does reduce the performance while debugging considerably. Therefore, in
// release configurations, NA_DEBUG is usually set to 0.
//
// Note that the naError and naCrash functions will emit errors on stderr in
// the following format:
// Function_name: Errormessage NEWLINE
// Therefore, a programmer can simply set a breakpoint in the implemented
// function and start debugging.
//
// If due to some reason, setting a breakpoint does not work in the desired
// function, you can simply set a breakpoint in the naError function.
// This function is explicitely NOT inlined and is defined in a separate
// implementation file (and therefore a separate translation unit) due to
// that reason. See the NADebugging.c file.
//
// Also note that in NALib, code executed within NA_DEBUG will NOT alter the
// functionality of code. Except from speed differences, a few more bytes for
// certain structs and outputs to strerr, NALib will NOT behave differently
// depending on NA_DEBUG being 0 or 1. The only remark is that calls to
// naCrash will forcefully lead to an exit(EXIT_FAILURE) call before the real
// error is executed. If you use code sanity checking (for example clang
// analyzer) you should therefore always check both with and without NA_DEBUG
// defined. Otherwise, the analyzer might miss some errors.



#if NA_DEBUG

  // Evaluates to the current function name. Depending on the system, this
  // can be including a classname if this is compiled in C++.
  #define NA_DEBUG_FUNCTIONSYMBOL

  // Prints an error. When this function gets called, the ongoing of the
  // application is undefined. Sometimes, the error might affect everything
  // which comes after it, sometimes, the error will just result in a NaN or
  // even be corrected automatically. Nontheless, any error should be
  // considered a potential risk for the application to eventually crash.
  #define naError(text)

  // Prints a crash message.
  // This macro is used before the application experiences a critical error
  // like dereferencing an invalid pointer. The application would almost
  // certainly crash few steps after this function call. Therefore, it
  // is crashed deliberately with an exit call.
  //
  // Note that this is a macro, not a function. This had been changed in
  // NALib version 21 because different code analysis tools behaved
  // differently in different C standards and on different systems. Some did
  // not detected the deliberate crash (the exit call) and would assume, the
  // calling function would continue to run hence running into the same
  // issues as before and outputting warnings. This had been solved using
  // explicit return statements after the naCrash call. But other tools
  // detected that exit call and warned about the additional return statement
  // that it will not be executed and hence outputting some warnings as well.
  //
  // With a macro, the exit call is directly copied to the place the error
  // occurs and all code analysis tools are silented.
  #define naCrash(text)

#endif // NA_DEBUG



// Inline implementations are in a separate file:
#include "NADebuggingII.h"



#endif // NA_DEBUGGING_INCLUDED



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
