
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains configurations of your installation of NALib. There are
// some few options you can compile NALib with. These options are implemented
// as preprocessor macros such that you can define them with prefix headers
// when you need them in your special project.
//
// Note that preprocessor flags with values usually follow this scheme:
// PREPROCESSOR_MACRO=value
// Without spaces at the equal sign.
//
// NAConfiguration.h is included at the beginning of the NABase.h file.
// Do not include it anywhere else.
//
// Note that there is no possibility to platform-independently provide a clean
// configuration structure without the need to configure either the NALib
// installation or your compiler in one or another way. The solution with this
// file allows you to share NALib with a default implementation but which can
// be altered project-wise by as simple compiler prefix macro definition.



// ////////////////////////////////
// Support for 64 bit integers
// ////////////////////////////////

// C dialects before C99 and before C++11 do NOT have any standardized notion
// of the the long long data type and the LL suffix for integers. But often
// times, it can be accessed through compiler extensions, even for standards
// before C99 for example in the form of GNU extensions.
//
// NALib by default assumes, that the long long type and the integer suffix LL
// is only available with the standards C99 or C++11 or newer. But you can
// set the following macro to 1 which causes NALib to try to compile itself
// with long long datatypes and LL suffixes.
//
// Beware, although this might work, you maybe will get a lot of warnings.
// Consider turning them off using -Wno-long-long.
 
#ifndef NA_TYPE_ASSUME_NATIVE_LONG_LONG
  #define NA_TYPE_ASSUME_NATIVE_LONG_LONG 0
#endif

// It may be that you try to compile with a standard before C99 and do not
// want to have long long types. If you still run on a 64 bit system, there
// will hence be no native int type which is big enough to hold an address.
// NALib will do its best to fulfill its word nontheless, but there are some
// core elements which will not work properly. Therefore, a warning is emitted.
// If you know what you are doing, you can silence that warning by defining
// the following macro to be 0.

#ifndef NA_TYPE_WARN_IF_NO_NATIVE_ADDRESS_TYPE
  #define NA_TYPE_WARN_IF_NO_NATIVE_ADDRESS_TYPE 1
#endif




// ////////////////////////////////
// NAInt bit width
// ////////////////////////////////

// Usually, an NAInt has the same bit width as an address in the current
// configuration, meaning 32 bits on a 32 bit system and 64 bits on a 64 bit
// system. But there may be instances, where an NAInt shall have a different
// size.
//
// Choose from the following values:
// 0   Automatic setting. See below.
// 1   NAInt has the same size as an int.
// 2   NAInt has the same size as a long int.
// 3   NAInt has the same size as NS_SYSTEM_ADDRESS_BITS.
// 32  NAInt has a 32 bit width.
// 64  NAInt has a 64 bit width.
//
// The automatic setting goes as follows:
// - The assumed bit width shall be the same size as NS_SYSTEM_ADDRESS_BITS
// - But in case of 64, when no 64 bit integer type is available, it falls back
//   to 32 bits. Please refer to NA_TYPE_ASSUME_NATIVE_LONG_LONG for more
//   information.
//
// By default, the automatic setting (0) is used. But you can change this
// setting for example, when using standards before C99 which are considered
// to be not aware of the long long integer type. If you use GNU extensions,
// that type is perfectly available. Hence you can simply set this setting
// to 64 or even better to NS_SYSTEM_ADDRESS_BITS (setting 2)
//
// Note that whenever the final bit width turns out to be 64, long long support
// is expected from NALib. Therefore, when using 64 bit types on older
// standards, you may want to turn off the warnings using -Wno-long-long.
//

#ifndef NA_PREFERRED_NAINT_BITS
  #define NA_PREFERRED_NAINT_BITS 0
#endif


// ////////////////////////////////
// OpenGL support
// ////////////////////////////////

// By default, the following macro is defined as 0 and therefore, NALib does
// not compile with OpenGL support. But if you want to, simply set the this
// macro to 1.
//
// If done so, you MUST link your program to the OpenGL library. The #include
// directives can be found on top of the NAUI.h file

#ifndef NA_CONFIG_COMPILE_OPENGL
  #define NA_CONFIG_COMPILE_OPENGL 0
#endif



// ////////////////////////////////
// GUI support
// ////////////////////////////////

// By default, the following macro is defined as 0 and therefore, NALib does
// not compile with GUI support. But if you want to, simply set the this
// macro to 1.
//
// If done so, you MUST link your program to the appropriate library:
// Windows: WinAPI
// Macintosh: Cocoa

#ifndef NA_CONFIG_COMPILE_GUI
  #define NA_CONFIG_COMPILE_GUI 0
#endif



// ////////////////////////////////
// Aligned memory allocation
// ////////////////////////////////

// Usually, aligned memory can be created in unix like systems using several
// methods. Unfortunately, none of them work reliably on Mac OS X. See the
// NAMemory.h file for more information. Default is USE_CUSTOM which is safe
// on all systems.
// - NA_MEMORY_ALIGNED_MEM_MAC_OS_X_USE_CUSTOM 
// - NA_MEMORY_ALIGNED_MEM_MAC_OS_X_USE_ALIGNED_ALLOC (needs full C11 support)
// - NA_MEMORY_ALIGNED_MEM_MAC_OS_X_USE_POSIX_MEMALIGN


#ifndef NA_MEMORY_ALIGNED_MEM_MAC_OS_X
  #define NA_MEMORY_ALIGNED_MEM_MAC_OS_X NA_MEMORY_ALIGNED_MEM_MAC_OS_X_USE_CUSTOM
#endif



// ////////////////////////////////
// Runtime memory pools
// ////////////////////////////////

// Define the size of a core memory pool:
//
// With the following macro, you can define, what the byte size of the memory
// pools shall be. This size is used for allocation with naNew, as well as for
// garbage collection. The default value for the pool size is (1<<16). If you
// set this macro to 0, the memory page size will be used.
//
// Turns out, on most systems, the pagesize is far too small to result in good
// speed improvements for naNew. A large enough custom bytesize can result in
// up to 2 times faster allocation and deallocation.
//
// The corepool size must be smaller than NA_INT32_MAX

#ifndef NA_COREPOOL_BYTESIZE
  #define NA_COREPOOL_BYTESIZE (1 << 16)
#endif

// Define if memory pools shall be cleaned up aggressively.
//
// Memory pools do organize themselves by allocating and deallocating large
// chunks of memory. Upon startup, there is no memory allocated whatsoever.
// This should of course be the same in the end when the runtime system stops.
// Nontheless, it might be very helpful to always keep one chunk per type in
// memory to prevent allocating and deallocating the same (first) chunk over
// and over again.
//
// With this flag, the programmer defines, if that last chunk shall be freed
// aggressively, meaning as soon as no more memory is needed for that type. If
// not, the last chunk stays in memory up until the runtime system stops.
//
// Default is 0: The last pool chunk stays in memory.
//
// This flag is important for systems with very small memory as well as for
// performance testing: When clenaing up aggressively, the runtime can be
// considerably slower. Otherwise, one big chunk of memory always stays ready
// resulting in very high performance, especially for small testing cases. But
// it might not be what should have been measured by the performance test.

#ifndef NA_MEMORY_POOL_AGGRESSIVE_CLEANUP
  #define NA_MEMORY_POOL_AGGRESSIVE_CLEANUP 0
#endif

// Defines when the temp garbage collection starts collecting automatically.
//
// With this macro, you can define, if and when the garbage collection should
// perform an automatic collection of temporarily allocated memory. If you set
// this value to 0, no automatic collection will be performed ever. You need to
// call naCollectGarbage manually.
//
// If it is set to a non-zero value, it will fire during a call to naMallocTmp
// but only when the sum of all previously temporary allocated bytes had become
// higher than this number in the previous call to naMallocTmp. If so, the
// garbage will first be collected and then the new bytes will be allocated
// and returned.
//
// The default value is 1000000 (1 Million) when no GUI is in use and 0 when
// a GUI is in use.
//
// This macro is set to 0 by default when a GUI is in use, as the event loop
// of the NAGUI implementation calls naCollectGarbage automatically before
// executing each and every event.

#ifndef NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT
  #if NA_CONFIG_COMPILE_GUI == 0
    #define NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT (1000000)
  #else
    #define NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT (0)
  #endif
#endif



// ////////////////////////////////
// Buffers
// ////////////////////////////////

// Define the size of a buffer part used for NABuffer:
//
// With the following macro, you can define, what the default byte size of one
// buffer part shall be. The value 0 denotes that the memory page size will be
// used. Custom values typically are powers of 2, for example 4096.
//
// The default value is 0. 

#ifndef NA_BUFFER_PART_BYTESIZE
  #define NA_BUFFER_PART_BYTESIZE 0
#endif


// String caching
//
// When set to 1, the NAString structure always tries to create a cached
// version of its content. This is really only useful when debugging as the
// cached version will be stored within the NAString structure. With NDEBUG
// defined though, nothing will be stored and it is just a waste of time.
// Nontheless as caching will take a considerable amount of time even in
// debugging, you may choose to set NA_STRING_ALWAYS_CACHE to 0 even for
// debugging right here.
//
// The default is that strings will always be cached when running in debug mode
// but will not be cached if NDEBUG is defined.

#ifndef NA_STRING_ALWAYS_CACHE
  #ifndef NDEBUG
    #define NA_STRING_ALWAYS_CACHE 1
  #else
    #define NA_STRING_ALWAYS_CACHE 0
  #endif
#endif



// ////////////////////////////////
// Mutexes on Windows
// ////////////////////////////////

// There are two ways to implement mutexes on windows: "Critical section" or
// "Mutex". Both implementations work but some performence issues have been
// observed with the "Mutex" implementation. 
//
// Set this macro to 1 if you want to use the "critical section" or set it to
// 0 if you want to use "Mutex". Default is 1

#ifndef NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION
  #define NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION 1
#endif



// ////////////////////////////////
// Mathematical, chemical and physical constants
// ////////////////////////////////

// The constants in NAMathConstants.h are constants from physics, chemistry,
// mathematics, all mixed together. These constants are directly from NIST:
// http://physics.nist.gov/constants
//
// With the following macro, you define, what year of the CODATA you would like
// to use. Possible values are 2006, 2010 and 2014.
//
// By default, NALib uses the latest setting.

#ifndef NA_NIST_CODATA_YEAR
  #define NA_NIST_CODATA_YEAR 2014
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
