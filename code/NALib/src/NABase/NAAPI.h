
#ifndef NA_API_INCLUDED
#define NA_API_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// /////////////////////////////////
// This file is the bare minimum required for header files of NALib.
//
// It loads the configuration and defines some basic macros
// /////////////////////////////////



// Include the configuration of NALib.
// Have a look at this file to find out how you can configure NALib by adding
// preprocessor macros to your compiler.
#include "NAConfiguration.h"



// A macro marking a given type as a struct type without the need of including
// its definition.
#define NA_PROTOTYPE(type) typedef struct type type



// ////////////////////////////////////////////////////////////
// Detection of the os
// ////////////////////////////////////////////////////////////

// The macro NA_OS will be filled with one of the following macros which can
// later in the code be used to distinguish between different operating
// systems.

#define NA_OS_UNKNOWN   0
#define NA_OS_MAC_OS_X  1
#define NA_OS_WINDOWS   2
#define NA_OS_FREEBSD   3

#if defined _WIN32
  #define NA_OS NA_OS_WINDOWS
#elif defined __APPLE__ && __MACH__
  #define NA_OS NA_OS_MAC_OS_X
#elif defined __FreeBSD__
  #define NA_OS NA_OS_FREEBSD
#else
  #define NA_OS NA_OS_UNKNOWN
#endif

// Interesting read: http://sourceforge.net/p/predef/wiki/OperatingSystems/



// ////////////////////////////////////////////////////////////
// System dependant mapping of keywords and prefixes
// ////////////////////////////////////////////////////////////

// The following macros will be defined:
//
// NA_RESTRICT
// NA_INLINE
// NA_LINKER_NO_EXPORT
// NA_LINKER_EXPORT
//
// The definition of NA_RESTRICT and NA_INLINE are just mappings of built-in
// keywords on different systems.
//
// A function declared with NA_LINKER_NO_EXPORT will not be exported when
// building a binary. Therefore this function will not be listed in the .lib
// file on windows and will not be accessible anywhere when including the
// (.dll or .dylib) library. A function declared with NA_LINKER_EXPORT will
// explicitely be exported. This is a system-independent implementation which
// allows the programmer to define the exporting on a very low granularity.
//
// As NALib is intended to be a reusable code collection, all functions are
// either declared static inline or with NA_LINKER_NO_EXPORT. The attribute
// NA_LINKER_EXPORT will never be used in NALib but you may use it in your
// own code.

#if NA_OS == NA_OS_WINDOWS
  #define NA_RESTRICT           __restrict
  #define NA_INLINE             _inline
  #define NA_LINKER_NO_EXPORT
  #define NA_LINKER_EXPORT      __declspec(dllexport)
#elif NA_OS == NA_OS_MAC_OS_X
  #define NA_INLINE             inline
  #ifdef __cplusplus
    // c++ does not handles restrict consistently. So we just omit it.
    #define NA_RESTRICT
  #else
    #define NA_RESTRICT         restrict
  #endif
  #define NA_LINKER_NO_EXPORT   __attribute__ ((visibility("hidden")))
  #define NA_LINKER_EXPORT      __attribute__ ((visibility("default")))
#else
  #define NA_INLINE             inline
  #ifdef __cplusplus
    // c++ does not handles restrict consistently. So we just omit it.
    #define NA_RESTRICT
  #else
    #define NA_RESTRICT         __restrict__
  #endif
  #define NA_LINKER_NO_EXPORT
  #define NA_LINKER_EXPORT
#endif


// The following macros mark the declarations and definitions of symbols
// in NALib:
//
// NA_API marks the declaration of a function which is intended to be used by
// the programmer. Its counterpart is NA_DEF which marks the implementation
// of the appropriate function. NA_API functions can only be found in header
// files. NA_DEF functions can only be found in implementation files.
//
// Note that API stands for "Application Programming Interface" and DEF
// stands for "Definition".
// 
// If the letter I is added, a function is marked as INLINE.
//
// Note that visibility attributes have no effect on inline functions as
// they MUST be visible to the compiler in order for it to inline the code.
// Hence in NALib, any visibility attribute is omitted with inline functions
// as otherwise compilers would constantly emit annoying warnings.
//
// Note that the following macros are just used for NALib, you can very well
// choose differently in your own implementation!

#define NA_API NA_LINKER_NO_EXPORT
#define NA_DEF NA_LINKER_NO_EXPORT
#if NA_INLINE_DEFINITION_NON_STANDARD
  #define NA_IAPI static NA_INLINE
  #define NA_IDEF static NA_INLINE
#else
  #define NA_IAPI   
  #define NA_IDEF   NA_INLINE
#endif

// Additionally, marks with the letter H are definitions of HELPER functions.
// Helper functions should not be accessible or visible to the user. Helper
// functions are called from other functions and usually are declared in
// the same instance and defined once, and only once in an implementation
// file, not a header file.

#define NA_HAPI    NA_LINKER_NO_EXPORT
#define NA_HDEF    NA_LINKER_NO_EXPORT
#if NA_INLINE_DEFINITION_NON_STANDARD
  #define NA_HIAPI static NA_INLINE
  #define NA_HIDEF static NA_INLINE
#else
  #define NA_HIAPI   
  #define NA_HIDEF   NA_INLINE
#endif

// Authors comment: Note that all symbols are declared and defined very
// restrictive. Meaning: No function or variable of NALib will show up in a
// library or software which is built with NALib.



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_API_INCLUDED



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
