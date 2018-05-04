
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"




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
// building a library. This means that this function will not be listed in the
// .lib file on windows and will not be accessible anywhere when including the
// (.dll or .dylib) library. A function declared with NA_LINKER_EXPORT will
// explicitely be exported. This is a system-independent implementation which
// allows the programmer to define the exporting on a very low granularity.
// There exist other methods to define the exporting in supplementary files.
//
// As NALib is not intended to be compiled as a library, all functions are
// either declared static inline or with NA_LINKER_NO_EXPORT. NA_LINKER_EXPORT
// will never be used in NALib but you may use it in your own code.
//
// Usually, the NA_LINKER_NO_EXPORT macro is defined in some other, hidden file
// as it shall not be visible to the end user and will only be used on hidden
// parts. This is not necessary but makes the code clean and helps detect
// functions which should not be accessible to the user. In NALib, this does
// not matter and therefore the NA_LINKER_NO_EXPORT macro is defined here
// together with the NA_LINKER_EXPORT macro.

#if (defined NA_C99) || (defined NA_CPP98)
  #define NA_INLINE             inline
  #ifdef __cplusplus
    // c++ does not handles restrict consistently. So we just omit it.
    #define NA_RESTRICT
  #else
    #define NA_RESTRICT         restrict
  #endif
#else
  #define NA_INLINE
#endif

#if NA_OS == NA_OS_WINDOWS
  #if !defined NA_RESTRICT
    #define NA_RESTRICT         __restrict
  #endif
  #if !defined NA_INLINE
    #define NA_INLINE           _inline
  #endif
  #define NA_LINKER_NO_EXPORT
  #define NA_LINKER_EXPORT      __declspec(dllexport)
#elif NA_OS == NA_OS_MAC_OS_X
  #if !defined NA_RESTRICT
    #define NA_RESTRICT         __restrict__
  #endif
  #define NA_LINKER_NO_EXPORT   __attribute__ ((visibility("hidden")))
  #define NA_LINKER_EXPORT      __attribute__ ((visibility("default")))
#else
#endif


// The following macros mark the declarations and definitions of symbols
// in NALib:
//
// NA_API marks the declaration of a function which is intended to be used by
// the programmer. Its counterpart is NA_DEF which marks the implementation of
// the appropriate function. NA_API functions can only be found in header
// files. NA_DEF functions can only be found in implementation files.
//
// Note that API stands for "Application Programming Interface" and DEF stands
// for "Definition".
//
// NA_IAPI is short for NA_INLINE_API and NA_IDEF is short for NA_INLINE_DEF.
// These two prefixes mark inline functions in NALib. But note that inline
// functions do not need to be static. Even more so, the declaration of an
// inline function should not be declared with the inline keyword. But as
// compilers tend to be difficult and there are some versions out there which
// may emit warnings, both macros are defined "static inline" in NALib. Saves
// a lot of trouble. The static keyword may lead to slightly bigger binaries.
// But so far, the author never had problems with that.
//
// Also note that visibility attributes have no effect on inline functions as
// they MUST be visible to the compiler in order for it to inline the code.
// Hence in NALib, any visibility attribute is omitted with inline functions
// as otherwise compilers would constantly emit annoying warnings.
//
// Again, note that the following macros are just used for NALib, they may
// very well differ in your own implementation!
#define NA_API    NA_LINKER_NO_EXPORT
#define NA_DEF    NA_LINKER_NO_EXPORT
#define NA_IAPI   static NA_INLINE
#define NA_IDEF   static NA_INLINE

// Additionally, there are definitions of so called HELPER functions.
// Helper functions should not be accessible or visible to the user. Helper
// functions are called from other functions and usually are declared in
// the same instance and defined once, and only once in an implementation file,
// not a header file. Again, in NALib, things are a little more transparent so
// these macros are a mere hint for the programmer, not more.
#define NA_HAPI   NA_LINKER_NO_EXPORT
#define NA_HDEF   NA_LINKER_NO_EXPORT
#define NA_HIAPI  static NA_INLINE
#define NA_HIDEF  static NA_INLINE
// Authors comment: Note that all symbols are declared and defined very
// restrictive. Meaning: No function or variable of NALib will show up in a
// library or software which is built with NALib.



// ////////////////////////
// The definition of NA_NULL is usually set to a zero castet as a pointer.
// The new C++11 standard however has a new keyword. Let's use it if it is
// available!
//
// The NA_NORETURN macro expands to _Noreturn only in C11 or CPP11

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
// We always include stdarg as otherwise there might be a problem with va_copy.
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



// ////////////////////////
// The following macro is used to suppress compiler warnings in functions
// which do not make any use of a given parameter. This happens when function
// signatures are chosen to match predefined function pointer types or shall
// in general have a specific look. By using the given parameter as an
// expression and casting the result to void makes the compiler think that
// the parameter is evaluated but has the explicit order to not use the value.
//
// The compiler still may and in certain cases still WILL evaluate the value.
// Be careful with this when using the volatile keyword!

#define NA_UNUSED(x) (void)(x)



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