
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_BASE_INCLUDED
#define NA_BASE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This is the base of the whole NALib implementation. Almost all files
// include this file. It contains the most basic type definitions and
// preprocessor macros.

// After including this file, the following standard libraries are included:
// <limits.h>
// <stdlib.h>
// <stdarg.h>
// <sys/types.h> (only on Mac OS X)



// The version number of this NALib distribution.
#define NA_VERSION 21

// Include the configuration of NALib.
// Have a look at this file to find out how you can configure NALib by adding
// preprocessor macros to your compiler.
#include "NAConfiguration.h"


#include "NABase/NAEnvironment.h"
#include "NABase/NACompiler.h"
#include "NABase/NALanguage.h"
#include "NABase/NAInteger.h"




// ////////////////////////
// Characters and system dependent strings
// The tabs and newlines used on different systems. The native newline argument
// NA_NL denotes the native newline of the current system.
#define NA_NL_UNIX  "\n"
#define NA_NL_MAC9  "\r"
#define NA_NL_WIN   "\r\n"
#if NA_OS == NA_OS_WINDOWS
  #define NA_NL NA_NL_WIN
#else
  #define NA_NL NA_NL_UNIX
#endif

#define NA_TAB      "\t"
#define NA_PATH_DELIMITER_UNIX '/'
#define NA_PATH_DELIMITER_WIN  '\\'
#define NA_SUFFIX_DELIMITER    '.'

// Note that in NALib, a String is internally always encoded in UTF-8.
// The encoding of strings given by system calls though may be of
// different kind. Therefore, there exist two typedefs: NAUTF8Char
// and SystemChar.
//
// Note that NAUTF8Char is defined as char to simplify debugging.
// Types like NAByte, int8 or uint8 can not be used as it these are
// defined to be incompatible with char. But at least we can be sure that
// a char consists of 8 Bits. See definition of NAByte for that.
typedef char NAUTF8Char;

#if NA_OS == NA_OS_WINDOWS
  #include <windows.h>
  // The SystemChar is a character type which denotes the one used in the
  // project preferences. It is CHAR for "Multi Byte Character Set" and
  // WCHAR for "Unicode character set".
  #ifdef UNICODE
    typedef WCHAR SystemChar;
  #else
    typedef CHAR SystemChar;
  #endif
#elif NA_OS == NA_OS_MAC_OS_X
  // typedef short SystemChar;  // unused at the moment
#else
#endif



// ////////////////////////
// Note that in NALib, there is no base typedef for a void* or a const void*
// pointer. Some libraries or frameworks define them with a name like NAVoid.
// The author believes that void* pointers should only be typedef'd if they
// describe a distinct purpose, like for example if a returned void* points
// to a group of structs sharing some common properties.
//
// If void* is used plainly as a pointer to an untyped address, the author
// thinks that it is best to just write void*, not some abscure type name.
// Therefore, you won't find any type definition of NAVoid here.



// ////////////////////////
// The following macro is used to suppress compiler warnings in functions
// which do not make any use of a given parameter. This happens when function
// signatures are chosen to match predefined function pointer types or shall
// in general have a specific look. By using the given parameter as an
// expression and casting the result to void makes the compiler think that
// the parameter is used but has the explicit order to not use any
// evaluated value.
//
// Be careful with this when using the volatile keyword.

#define NA_UNUSED(x) (void)(x)


// ////////////////////////
// The definition of NA_NULL is usually set to the NULL found in stdlib. The
// new C++11 standard however has a new keyword. Let's use it if it is
// available! Note that stdlib is needed anyway for malloc, free and exit
// which is most probably needed anyway.
//
// Also note that in the comments of NALib as well as in the error messages,
// it will oftem times be called "Null-Pointer". In the implementation
// however, NA_NULL is used.
//
// The NA_NORETURN macro expands to _Noreturn only in C11 or CPP11

#include <stdlib.h>
#if (defined NA_C11) || (defined NA_CPP11)
  #ifdef __cplusplus
    #define NA_NULL nullptr
  #else
    #define NA_NULL ((void*)0)
  #endif
  #define NA_NORETURN _Noreturn
#else
  #define NA_NULL ((void*)0)
  #define NA_NORETURN
#endif


// ////////////////////////
// Some container structs like NAArray or NAList provide naForeachXXX functions
// with an additional parameter: An accessor or mutator callback with the
// following signatures:

typedef void (*NAAccessor) (const void*);
typedef void (*NAMutator)  (      void*);

// This parameter allows you to provide a function pointer to an accessing or
// mutation function which will be called for every element in the container.
//
// You can use accessors or mutators to simply address all elements in a
// container struct. But you can also use them to initialize all elements with
// a call to naInitXXX and to desctruct all elements with a call to naClearXXX.
//
// IMPORTANT:
// Beware that your accessor or mutator will always be called with a POINTER to
// the content. If for example, you have an array of integers, your callback
// will get values of type "int *". But if your array stores pointers to int,
// your callback will get values of type "int **".
//
// Note that you can also use any of the functions of NALib as callback
// functions as long as they accept only one data pointer parameter and return
// void.
//
// You may have to cast callback functions to "NAAccessor" or NAMutator".
//
// Apart from callback functions, many container structs have separate
// iterators.
// Have a look at the API.
//
// Callback functions are used for:
// - Accessor or Mutator functions for naForeachXXX functions.
// - Application start functions
// - Threading start functions
// - Timed functions
// - General callback functions



// ////////////////////////
// We always include stdarg as otherwise there might be a problem with va_copy.
// Additionally, variadic arguments are used in naError since NALib version 15.
#include <stdarg.h>
// va_copy is defined since C99. But before, you had to use something like
// the following if not available.
#if (!defined NA_C99) && (!defined NA_CPP98)
  #ifndef va_copy
    #define va_copy(d,s) (memcpy (&d, &s, sizeof (va_list)))
    // This definition is proposed to be the most secure fallback solution. But
    // on many systems, the following definition works as well:
    // #define va_copy(d,s) ((d) = (s))
  #endif
#endif




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

  #include <stdio.h>

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
