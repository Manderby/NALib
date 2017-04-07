
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_SYSTEM_INCLUDED
#define NA_SYSTEM_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This is the base of the whole NALib implementation. Almost all files
// include this file. It contains the most basic type definitions and
// preprocessor macros.

// After including this file, the following standard libraries are included:
// <stdint.h>
// <limits.h>
// <stdlib.h>
// <stdarg.h>
// <sys/types.h> (only on Mac OS X)



// The version number of this NALib distribution.
#define NA_VERSION 19

// Include the configuration of NALib.
// Have a look at this file to find out how you can configure NALib by adding
// preprocessor macros to your compiler.
#include "NAConfiguration.h"

// The various Systems:
#define NA_SYSTEM_UNKNOWN   0
#define NA_SYSTEM_MAC_OS_X  1
#define NA_SYSTEM_WINDOWS   2
#define NA_SYSTEM_COUNT     3


// The various Endianness-Settings:
#define NA_ENDIANNESS_UNKNOWN 0 // Behaves the same as native.
#define NA_ENDIANNESS_LITTLE  1
#define NA_ENDIANNESS_BIG     2
#define NA_ENDIANNESS_NETWORK 2 // Same as BIG
#define NA_ENDIANNESS_NATIVE  3 //
#define NA_ENDIANNESS_HOST    3 // Same as NATIVE
#define NA_ENDIANNESS_COUNT   4
// Note that when expecting the endianness as an argument, it will have the
// type NAInt. Unfortunately, these macros can not be defined as enums.

// The various signed integer encodings:
#define NA_SIGNED_INTEGER_ENCODING_UNKNOWN          0
#define NA_SIGNED_INTEGER_ENCODING_SIGN_MAGNITUDE   1
#define NA_SIGNED_INTEGER_ENCODING_ONES_COMPLEMENT  2
#define NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT  3

// Figuring out what system this is. The following macros will be defined:
//
// NA_SYSTEM                One of the system macros above
// NA_SYSTEM_ENDIANNESS     One of the endianness macros above
// NA_SYSTEM_ADDRESS_BITS   32 or 64. Denoting the number of bits per address.
//
// Currently, there are two system configurations assumed:
// - Mac OS X with GCC or Clang
// - Windows with Microsoft Visual Studio compiler
//
// Note that the author is completely aware that the system is not bound to
// a specific compiler. But these combinations are the ones having been used
// during years by the author both for hobby and job.
//
// Interesting read: http://sourceforge.net/p/predef/wiki/OperatingSystems/
//
// In the future, there might be more or different macros
#if defined _WIN32
  #define NA_SYSTEM NA_SYSTEM_WINDOWS
  #define NA_SYSTEM_ENDIANNESS NA_ENDIANNESS_LITTLE
  #if defined _WIN64
    #define NA_SYSTEM_ADDRESS_BITS 64
  #else
    #define NA_SYSTEM_ADDRESS_BITS 32
  #endif

#elif defined __APPLE__ && __MACH__
  #define NA_SYSTEM NA_SYSTEM_MAC_OS_X
  #if defined __LITTLE_ENDIAN__
    #define NA_SYSTEM_ENDIANNESS NA_ENDIANNESS_LITTLE
  #elif defined __BIG_ENDIAN__
    #define NA_SYSTEM_ENDIANNESS NA_ENDIANNESS_BIG
  #endif
  #if defined __LP64__
    #define NA_SYSTEM_ADDRESS_BITS 64
  #else
    #define NA_SYSTEM_ADDRESS_BITS 32
  #endif
  
#else
  #warning "System not detected"
  #define NA_SYSTEM NA_SYSTEM_UNKNOWN
#endif

#ifndef NA_SYSTEM_ADDRESS_BITS
  #warning "Address bits not detected, trying 32."
  #define NA_SYSTEM_ADDRESS_BITS 32
#endif

#ifndef NA_SYSTEM_ENDIANNESS
  #warning "Endian format not detected."
  #define NA_SYSTEM_ENDIANNESS NA_ENDIANNESS_UNKNOWN
#endif



// The number of bytes per address
#define NA_SYSTEM_ADDRESS_BYTES (NA_SYSTEM_ADDRESS_BITS >> 3)



// NALib does not provide macros for the kind of compiler is used (like for
// example C, C++ or Objective-C). Please check for definition of the default
// macros:
//
// __cplusplus    Compiler is a C++ compiler
// __OBJC__       Compiler is an Objective-C Compiler
//
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



// ////////////////////////////////////////////////////////////
// System dependant mapping of global functions and macros
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

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #if !defined NA_RESTRICT
    #define NA_RESTRICT         __restrict
  #endif
  #if !defined NA_INLINE
    #define NA_INLINE           _inline
  #endif
  #define NA_LINKER_NO_EXPORT
  #define NA_LINKER_EXPORT      __declspec(dllexport)
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
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
// library which is built with NALib.




// /////////////////////////
// The integer types
// /////////////////////////

// We test if the current system has a (positive) integer encoding suitable for
// NALib. Some obscure compilers might handle this differently.
#if (0x01000000 >> 24) != 0x01
  #warning "Unknown integer number encoding. NALib might not compile or run."
#endif

// We test what encoding is used for negative integers. With this knowledge,
// certain tasks might be speeded up a bit.
#if   (-1 & 3) == 3
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
#elif (-1 & 3) == 2
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_ONES_COMPLEMENT
#elif (-1 & 3) == 1
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_SIGN_MAGNITUDE
#else
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_UNKNOWN
  #warning "Invalid signed integer encoding. NALib might not work properly."
#endif

// Definition of basic integer types. Depending on the C-standard and depending
// on the system the typedefs are provided differently in NALib.
//
// Note that these types do not have the NA prefix. The author decided that
// these types are too fundamental such that they do not need to be marked with
// the prefix. They should not collide with other typedefs found in other
// sourcecodes. If the do collide, there would be a serious problem and the
// implementation is likely incompatible. Please send the author an email if
// that happends.

#if (defined NA_C99) || (defined NA_CPP98)
  #include <stdint.h>
  typedef uint64_t          uint64;
  typedef uint32_t          uint32;
  typedef uint16_t          uint16;
  typedef uint8_t           uint8;
  typedef int64_t           int64;
  typedef int32_t           int32;
  typedef int16_t           int16;
  typedef int8_t            int8;
#elif NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef unsigned __int64  uint64;
  typedef unsigned __int32  uint32;
  typedef unsigned __int16  uint16;
  typedef unsigned __int8   uint8;
  typedef signed __int64    int64;
  typedef signed __int32    int32;
  typedef signed __int16    int16;
  typedef signed __int8     int8;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <sys/types.h>
  typedef u_int64_t         uint64;
  typedef u_int32_t         uint32;
  typedef u_int16_t         uint16;
  typedef u_int8_t          uint8;
  typedef int64_t           int64;
  typedef int32_t           int32;
  typedef int16_t           int16;
  typedef int8_t            int8;
#else
  #warning "Don't know how to define the basic integer types."
#endif


// Note that the following macros might also be defined using the values
// provided in the <limits.h> or <climits> header file. In NALib, the are
// provided manually to not be dependent on the various definitions of
// char, short, int, long and long long.

#if (defined NA_C99) || (defined NA_CPP98)
  // In C99, the values are defined in stdint.h (included above)
  #define NA_UINT8_MAX  UINT8_MAX
  #define NA_UINT16_MAX UINT16_MAX
  #define NA_UINT32_MAX UINT32_MAX
  #define NA_UINT64_MAX UINT64_MAX
  #define NA_INT8_MAX   INT8_MAX
  #define NA_INT8_MIN   INT8_MIN
  #define NA_INT16_MAX  INT16_MAX
  #define NA_INT16_MIN  INT16_MIN
  #define NA_INT32_MAX  INT32_MAX
  #define NA_INT32_MIN  INT32_MIN
  #define NA_INT64_MAX  INT64_MAX
  #define NA_INT64_MIN  INT64_MIN
#else
  #define NA_UINT8_MAX  (0xffu)
  #define NA_UINT16_MAX (0xffffu)
  #define NA_UINT32_MAX (0xffffffffu)
  #define NA_UINT64_MAX (0xffffffffffffffffuLL)
  // note that the - 1 is needed to avoid warnings on some compilers
  #define NA_INT8_MAX   (0x7f)
  #define NA_INT8_MIN   (0x81 - 1)
  #define NA_INT16_MAX  (0x7fff)
  #define NA_INT16_MIN  (0x8001 - 1)
  #define NA_INT32_MAX  (0x7fffffff)
  #define NA_INT32_MIN  (0x80000001 - 1)
  #define NA_INT64_MAX  (0x7fffffffffffffffLL)
  #define NA_INT64_MIN  (0x8000000000000001LL - 1LL)
#endif


// ////////////////////////////////////
// Other Basic types used in NALib
// ////////////////////////////////////

#include <limits.h>

// Currently, NALib assumes a Byte to consist of precisely 8 bits. This is
// reflected in various enumerations and fundamental types like NAByte and
// NAUTF8Char. With this macro, we make sure, our code compiles just on the
// systems which have 8-Bit Bytes.
#if CHAR_BIT != 8
  #error "NALib can not work properly with chars unequal 8 bits."
#endif

// Also, we want to know what the default bytesize of an "int" is. It will be
// stored in the NA_SYSTEM_INT_BITS macro. This might be different to what the
// NA_SYSTEM_ADDRESS_BITS is.
#if UINT_MAX == NA_UINT32_MAX
  #define NA_SYSTEM_INT_BITS 32
#elif UINT_MAX == NA_UINT64_MAX
  #define NA_SYSTEM_INT_BITS 64
#else
  #warning "System uses default int bytesize other than 32 or 64."
  #define NA_SYSTEM_INT_BITS 32
#endif



// An NAByte is a type definition of a Byte.
//
// Defining an NAByte as an uint8 can be very handy. In NALib, the NAByte type
// is often used when a memory block needs to be accessed byte by byte. You
// could also use a void-pointer for that but void-pointers are sometimes just
// a little too cumbersome to work with and do not always have a bytesize defined
// depending on the standard used. Furthermore, a pointer to an uint8 can be
// displayed by a debugger while a pointer to void can not.
// Why not using the signed variant? Because there are many implementations
// using enumerations which go beyound 127 and do not want to use negative
// numbers.
typedef uint8 NAByte;


// NAInt and NAUInt
//
// The NAInt type is an integer of the bytesize which is needed for storing an
// address. This means that this type is dependent on the system NALib is
// compiled for. It can for example be 32 Bits on one and 64 Bits on another
// architecture.
//
// NAInt will be used as the default integer type in NALib. Many fundamental
// functions will return NAInt or expect it as an argument. Note that the NAInt
// type is signed!
//
// Starting with NALib Version 9, there also exists NAUInt, the unsigned
// variant. Many functions have been changed which explicitely require or
// return an unsigned integer. This helps detecting errors as a compiler can
// emit warnings on sign differences.
//
// The signed variant NAInt is also used by memory and array functions. This
// means that in 32-Bit systems, only about 2 billion entries can be accessed
// with arrays when using NALib. The author is perfectly aware of the fact
// that most standard library functions use an unsigned integer for size_t,
// and that NALib therefore will have some minor limitations regarding memory
// usage. But the author has been convinced by the simplicity and elegance of
// just using signed integers during the last couple of years. It makes
// programming and debugging so much easier. And in 64 Bit systems, the memory
// limitations will not be a problem for a couple of years coming.
//
// Note that integers with explicitely defined sizes are used. The definition
// of just "int" would be unreliable.
//
// In addition to the type, there is the definition of a printf-argument macro.
// Use the macro for example like this:
//
// printf("The point is at X coordinate %" NA_PRIi NA_NL, pos.x);
// printf("The array has %" NA_PRIu " entries." NA_NL, naGetArrayCount(array));
//
// One serious drawback of a system-dependent definition is that NALib might
// behave differently depending on the system it is compiled for. But it will
// only behave differently in cases where the behaviour would become difficult
// anyway. Meaning: You will most likely run into out-of-memory problems first.
//
// Note that a different solution would be to always go for int64 and convert
// between the different integer types. But compiling NALib on a non-64-Bit
// system then would definitely be a waste of space with lots of implicit
// arithmetic conversions.
//
#define NA_ZERO_32  0L
#define NA_ZERO_64  0LL
#define NA_ONE_32   1L
#define NA_ONE_64   1LL

#if NA_SYSTEM_ADDRESS_BITS == 32
  typedef int32 NAInt;
  typedef uint32 NAUInt;
  #define NA_PRIi "d"
  #define NA_PRIu "u"
  #define NA_PRIx "x"
  #define NA_SCNi "d"
  #define NA_SCNu "u"
  #define NA_INT_MAX NA_INT32_MAX
  #define NA_INT_MIN NA_INT32_MIN
  #define NA_UINT_MAX NA_UINT32_MAX
  #define NA_ZERO NA_ZERO_32
  #define NA_ONE  NA_ONE_32
#elif NA_SYSTEM_ADDRESS_BITS == 64
  typedef int64 NAInt;
  typedef uint64 NAUInt;
  #define NA_PRIi "lld"
  #define NA_PRIu "llu"
  #define NA_PRIx "llx"
  #define NA_SCNi "lld"
  #define NA_SCNu "llu"
  #define NA_INT_MAX NA_INT64_MAX
  #define NA_INT_MIN NA_INT64_MIN
  #define NA_UINT_MAX NA_UINT64_MAX
  #define NA_ZERO NA_ZERO_64
  #define NA_ONE  NA_ONE_64
#else
  #warning "NAInt undefined"
#endif



// NABool
// Note that in NALib, the definition of NABool is set to an unsigned integer
// and not char or unsigned char. This is unusual but most probably the easiest
// way to use whatever the current processor can use as its fastest integer.
// It is in the believe of the author, that in modern computers, speed is more
// important than space as opposed to earlier times where wasting space was a
// complete no-go. When using lots of Boolean values, there are better ways to
// store them than in NABools anyway: Use C-style-masks.
//
// Note that before NALib version 13, NABool had been defined with "int". This
// procudes several warnings though on certain machines, therefore, the newer
// typedefs had been applied.
//
// Also note that there is no distinction between C and C++. Therefore, NALib
// never uses the bool type or the true and false keywords. Also the macros
// available in C11 are unused.
//
#if NA_SYSTEM_INTEGER_BITS == 32
  typedef uint32 NABool;
  #define NA_TRUE    1L
  #define NA_FALSE   0L
#else
  typedef uint64 NABool;
  #define NA_TRUE    1LL
  #define NA_FALSE   0LL
#endif



// ////////////////////////
// Characters and system dependent strings
// The tabs and newlines used on different systems. The native newline argument
// NA_NL will be defined below.
#define NA_NL_UNIX  "\n"
#define NA_NL_MAC9  "\r"
#define NA_NL_WIN   "\r\n"
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

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
  // The SystemChar is a character type which denotes the one used in the
  // project preferences. It is CHAR for "Multi Byte Character Set" and
  // WCHAR for "Unicode character set".
  #ifdef UNICODE
    typedef WCHAR SystemChar;
  #else
    typedef CHAR SystemChar;
  #endif
  #define NA_NL NL_WIN
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  // typedef short SystemChar;  // unused at the moment
  #define NA_NL NL_UNIX
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
    #define NA_NULL NULL
  #endif
  #define NA_NORETURN _Noreturn
#else
  #define NA_NULL NULL
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
// You can use accessors or mutators to simply address all elements in a container
// struct. But you can also use them to initialize all elements with a call to
// naInitXXX and to desctruct all elements with a call to naClearXXX.
//
// IMPORTANT:
// Beware that your accessor or mutator will always be called with a POINTER to
// the content. If for example, you have an array of integers, your callback will
// get values of type "int *". But if your array stores pointers to int, your
// callback will get values of type "int **".
//
// Note that you can also use any of the functions of NALib as callback functions
// as long as they accept only one data pointer parameter and return void.
//
// You may have to cast callback functions to "NAAccessor" or NAMutator".
//
// Apart from callback functions, many container structs have separate iterators.
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
// Function_name: Errormessage\n
// Therefore, a programmer can simply set a breakpoint in the denoted function
// and start debugging.
//
// If due to some reason, setting a breakpoint does not work in the desired
// function, you can simply set a breakpoint in the naError or naCrash function.
// These two functions are explicitely NOT inlined and are defined in a separate
// implementation file (and therefore a separate translation unit) due to that
// reason.
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

  // Prints an error. When this function gets called, the ongoing of the
  // application is undefined. Sometimes, the error might affect everything
  // which comes after it, sometimes, the error will just result in a NaN or
  // even be corrected automatically. Nontheless, any error should be
  // considered a potential risk for the application to eventually crash.
  void naError(const char* functionsymbol, const char* text, ...);


  // Prints a crash message.
  // This function is used when the application experiences a critical error
  // like dereferencing an invalid pointer. The application will almost
  // certainly crash few steps after this function call.
  NA_NORETURN void naCrash(const char* functionsymbol, const char* text, ...);

#endif






#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_SYSTEM_INCLUDED

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
