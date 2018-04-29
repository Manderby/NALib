
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"



// The various Operating Systems:
#define NA_OS_UNKNOWN   0
#define NA_OS_MAC_OS_X  1
#define NA_OS_WINDOWS   2
#define NA_OS_COUNT     3


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
// NA_OS                One of the system macros above
// NA_SYSTEM_ENDIANNESS     One of the endianness macros above
// NA_SYSTEM_ADDRESS_BITS   32 or 64. Denoting the number of bits per address.
//
// Currently, there are the following system configurations assumed:
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
  #define NA_OS NA_OS_WINDOWS
  #define NA_SYSTEM_ENDIANNESS NA_ENDIANNESS_LITTLE
  #if defined _WIN64
    #define NA_SYSTEM_ADDRESS_BITS 64
  #else
    #define NA_SYSTEM_ADDRESS_BITS 32
  #endif

#elif defined __APPLE__ && __MACH__
  #define NA_OS NA_OS_MAC_OS_X
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
  #define NA_OS NA_OS_UNKNOWN
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
