
#if defined NA_ENVIRONMENT_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_ENVIRONMENT_INCLUDED
#define NA_ENVIRONMENT_INCLUDED


// The number of bits in a byte.
#define NA_BITS_PER_BYTE (8)

// The various native type sizes
#define NA_TYPE8_BITS   (8)
#define NA_TYPE16_BITS  (16)
#define NA_TYPE32_BITS  (32)
#define NA_TYPE64_BITS  (64)
#define NA_TYPE128_BITS (128)
#define NA_TYPE256_BITS (256)

// The various type sizes in number of bytes instead of bits.
#define NA_TYPE8_BYTES   (NA_TYPE8_BITS   / NA_BITS_PER_BYTE)
#define NA_TYPE16_BYTES  (NA_TYPE16_BITS  / NA_BITS_PER_BYTE)
#define NA_TYPE32_BYTES  (NA_TYPE32_BITS  / NA_BITS_PER_BYTE)
#define NA_TYPE64_BYTES  (NA_TYPE64_BITS  / NA_BITS_PER_BYTE)
#define NA_TYPE128_BYTES (NA_TYPE128_BITS / NA_BITS_PER_BYTE)
#define NA_TYPE256_BYTES (NA_TYPE256_BITS / NA_BITS_PER_BYTE)

// The various endianness types.
// NA_ENDIANNESS_NETWORK is defined to be the same as NA_ENDIANNESS_BIG.
// NA_ENDIANNESS_HOST will be set to whatever this host endianness is.
// Note that when expecting the endianness as an argument, it will have the
// type NAInt. Unfortunately, these macros can not be defined as enums.
#define NA_ENDIANNESS_UNKNOWN 0
#define NA_ENDIANNESS_LITTLE  1
#define NA_ENDIANNESS_BIG     2
#define NA_ENDIANNESS_NETWORK 2
// NA_ENDIANNESS_HOST will be defined below.

// The various Operating Systems:
#define NA_OS_UNKNOWN   0
#define NA_OS_MAC_OS_X  1
#define NA_OS_WINDOWS   2

// Figuring out what system this is. The following macros will be defined:
//
// NA_OS                   One of the system macros above
// NA_ENDIANNESS_HOST      Either big or little, see macros above
// NA_ADDRESS_BITS         32 or 64. Denoting the number of bits per address.
// NA_SIZE_T_BITS          32 or 64. Denoting the number of bits for size_t.
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
  #undef  NA_ENDIANNESS_HOST
  #define NA_ENDIANNESS_HOST NA_ENDIANNESS_LITTLE
  #if defined _M_ARM
    #define NA_ADDRESS_BITS NA_TYPE32_BITS
    #define NA_SIZE_T_BITS NA_TYPE32_BITS
  #elif defined _M_ARM64
    #define NA_ADDRESS_BITS NA_TYPE64_BITS
    #define NA_SIZE_T_BITS NA_TYPE64_BITS
  #elif defined _WIN64
    #define NA_ADDRESS_BITS NA_TYPE64_BITS
    #define NA_SIZE_T_BITS NA_TYPE64_BITS
  #else
    #define NA_ADDRESS_BITS NA_TYPE32_BITS
    #define NA_SIZE_T_BITS NA_TYPE32_BITS
  #endif

#elif defined __APPLE__ && __MACH__
  #define NA_OS NA_OS_MAC_OS_X
  #if defined __LITTLE_ENDIAN__
    #undef  NA_ENDIANNESS_HOST
    #define NA_ENDIANNESS_HOST NA_ENDIANNESS_LITTLE
  #elif defined __BIG_ENDIAN__
    #undef  NA_ENDIANNESS_HOST
    #define NA_ENDIANNESS_HOST NA_ENDIANNESS_BIG
  #endif
  #if defined __LP64__
    #define NA_ADDRESS_BITS NA_TYPE64_BITS
    #define NA_SIZE_T_BITS NA_TYPE64_BITS
  #else
    #define NA_ADDRESS_BITS NA_TYPE32_BITS
    #define NA_SIZE_T_BITS NA_TYPE32_BITS
  #endif

  #if defined __has_feature
    #define NA_MACOS_USES_ARC __has_feature(objc_arc)
  #else
    #define NA_MACOS_USES_ARC 0
  #endif
  #if NA_MACOS_USES_ARC
    // Note: Do not use a cast for release configurations as the compiler
    // not necessarily is an objective-C compiler and hence does not know
    // the id type.
    #define NA_COCOA_BRIDGE __bridge
    #define NA_COCOA_PTR_OBJC_TO_C(obj) (void*)CFBridgingRetain(obj)
    #define NA_COCOA_PTR_C_TO_OBJC(ptr) CFBridgingRelease(ptr)
    #define NA_COCOA_RETAIN(obj) obj
    #define NA_COCOA_RELEASE(obj) (void)obj
    #define NA_COCOA_AUTORELEASE(obj) obj
    #define NA_COCOA_SUPER_DEALLOC()
  #else
    #define NA_COCOA_BRIDGE
    #define NA_COCOA_PTR_OBJC_TO_C(obj) (void*)obj
    #define NA_COCOA_PTR_C_TO_OBJC(ptr) (id)ptr
    #define NA_COCOA_RETAIN(obj) [obj retain]
    #define NA_COCOA_RELEASE(obj) [obj release]
    #define NA_COCOA_AUTORELEASE(obj) [obj autorelease]
    #define NA_COCOA_SUPER_DEALLOC() [super dealloc]
  #endif

#else
  #warning "System unknown. Assuming 32bit Addr, 32bit Int, little endian"
  #define NA_OS NA_OS_UNKNOWN
  #define NA_ENDIANNESS_HOST NA_ENDIANNESS_LITTLE
  #define NA_ADDRESS_BITS NA_TYPE32_BITS
  #define NA_SIZE_T_BITS NA_TYPE32_BITS
  #undef NA_PREFERRED_NAINT_BITS
  #define NA_PREFERRED_NAINT_BITS 32
#endif

#ifndef NA_ADDRESS_BITS
  #warning "Address bits not detected, trying 32."
#define NA_ADDRESS_BITS NA_TYPE32_BITS
#endif

#ifndef NA_SIZE_T_BITS
  #warning "size_t bits not detected, trying 32."
#define NA_SIZE_T_BITS NA_TYPE32_BITS
#endif

#ifndef NA_ENDIANNESS_HOST
  #error "Endian format of host not detected."
#endif



// The number of bytes per address
#define NA_ADDRESS_BYTES (NA_ADDRESS_BITS >> 3)



#if (NA_SIZE_T_BITS != NA_ADDRESS_BITS)
  #warning "size_t and address size do not match. Might cause problems."
#endif

#endif // NA_ENVIRONMENT_INCLUDED



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
