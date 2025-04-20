
#ifndef NA_CONFIGURATION_INCLUDED
#define NA_CONFIGURATION_INCLUDED

// NAConfiguration.h is included at the beginning of the NABase.h file.
// You may include it preemptively to safe some compile time.



// This file contains configurations of your installation of NALib. There are
// some few options you can compile NALib with. These options are implemented
// as preprocessor macros such that you can define them with prefix headers
// when you need them in your special project.
//
// Note that preprocessor flags with values usually follow this scheme:
// PREPROCESSOR_MACRO=value
// Without spaces at the equal sign. Here are some examples on how to do this:
//
// CMake:
// add_definitions(-DNA_COMPILE_GUI=1)
//
// XCode:
// Add the preprocessor macros to your project or target settings.
// 
// Visual Studio:
// Add the preprocessor macros to your project settings.
// Or:
// Provide a property sheet named NAConfiguration.props right next to the
// solution. Add the following lines at an appropriate place in the props file:
//   <ItemDefinitionGroup>
//     <ClCompile>
//       <PreprocessorDefinitions>NA_COMPILE_GUI=1;NA_COMPILE_OPENGL=1;%(PreprocessorDefinitions)</PreprocessorDefinitions>
//     </ClCompile>
//     <Link>
//       <AdditionalDependencies>ComCtl32.lib;opengl32.lib;MSImg32.lib%(AdditionalDependencies)</AdditionalDependencies>
//     </Link>
//   </ItemDefinitionGroup>
// Add the following line to your all jour projects (not the solution!):
// <Import Project="$(SolutionDir)\NAConfiguration.props" Condition="Exists('$(SolutionDir)\NAConfiguration.props')" />
// Use a text editor for that. It is proposed to add this line right after the
// line which imports the property sheet "Microsoft.Cpp.Default.props".
//
// The use of a cross compilation tool line CMake is encouraged!




// ////////////////////////////////
// Debugging
// ////////////////////////////////

// Depending on the definition of the NA_DEBUG macro, the naError and naCrash
// functions will either be defined or not. If defined, debugging checks like
// the following can be written anywhere in the code:
// 
// #if NA_DEBUG
//   if(somethingIsWrong)
//     naError("Uppsie");
//   if(somethingIsVeryWrong)
//     naCrash("Gonna crash!");
// #endif
// 
// For more information, see the comments in NADebugging.h
// 
// Be default, NA_DEBUG is set to 1 when NDEBUG is undefined.

#ifndef NA_DEBUG
  #ifndef NDEBUG
    #define NA_DEBUG 1
  #else
    #define NA_DEBUG 0
  #endif
#endif



// ////////////////////////////////
// OpenGL and Metal support
// ////////////////////////////////

// If the following macros are set to 1, NALib compiles with support for the
// specific graphics engine.
//
// If so, you MUST link your program to the appropriate library:
//
// OpenGL:
// Windows: WINAPI (always available) and opengl32.lib
// Macintosh: Cocoa and /System/Library/Frameworks/Metal.framework/OpenGL
//
// Metal:
// Windows: unavailable
// Macintosh: Cocoa and /System/Library/Frameworks/Metal.framework/Metal
//
// The #include directives can be found on top of the NAApp.h file
//
// Default is 0

#ifndef NA_COMPILE_OPENGL
  #define NA_COMPILE_OPENGL 0
#endif

#ifndef NA_COMPILE_METAL
  #define NA_COMPILE_METAL 0
#endif



// ////////////////////////////////
// GUI application support
// ////////////////////////////////

// If the following macro is set to 1, NALib compiles with GUI support.
//
// If so, you MUST link your program to the appropriate library:
// Windows: WINAPI (always available) and ComCtl32.lib, see next macro.
// Macintosh: Cocoa
//
// Default is 0

#ifndef NA_COMPILE_GUI
  #define NA_COMPILE_GUI 0
#endif



// On windows, GUI programming is done with WINAPI. This would by default use
// the version 5 of common controls which looks very outdated. As usually,
// a newer look is preferred, the default in NALib is to load version 6.
// Unfortunately, this requires you to link your binary to ComCtl32.lib.
// Set this macro to 0 if you want the old look or not link to the library.
//
// If your code does not compile for windows or NA_COMPILE_GUI is 0,
// this macro has no effect.
//
// Default is 1

#ifndef NA_USE_WINDOWS_COMMON_CONTROLS_6
  #define NA_USE_WINDOWS_COMMON_CONTROLS_6 1
#endif



// NALib uses macros like NA_IAPI and NA_IDEF for inlineable functions.
// According to the standard, such functions should be declared "inline"
// only in the definition, not the declaration. But compilers tend to
// be difficult and therefore, there are some compiler versions out there
// which may emit warnings and errors. Accordingly, all inline macros can
// be defined "static inline" in NALib. Saves a lot of trouble.
//
// This non-standard way may lead to slightly bigger binaries. But so far,
// the author never had problems with that.
//
// Default is 1

#ifndef NA_INLINE_DEFINITION_NON_STANDARD
  #define NA_INLINE_DEFINITION_NON_STANDARD 1
#endif



// ////////////////////////////////
// Aligned memory allocation
// ////////////////////////////////

// Usually, aligned memory can be created in unix like systems using several
// methods. Unfortunately, none of them did work reliably on the Mac OS X
// version Snow Leopard back around the year 2015. It looks though as since
// then, the error got corrected and now, the default method is Posix.
//
// These are some notes about the different methods:
// - Custom uses a rather large chunk of memory to ensure there is enough
//   space for aligned memory. It works but may lead to false positives
//   with memory-leak detection tools.
// - aligned_alloc under C11 is possibly unavailable in macOS. See
//   https://stackoverflow.com/questions/44841574/aligned-alloc-not-found-for-clang
// - posix_memalign as well as malloc_zone_memalign are non-C-standard and
//   did return misaligned pointers in Snow Leopard some years back.
// - __attribute ((aligned(#))) is unsuitable (because it is not usable at
//   runtine) and is non-standard
// - _Alignas is again unsuitable (because not available at runtime) and
//    only available since C11.
// Therefore, a custom implementation is provided which is costly but always
// works.

#define NA_MEMALIGN_USE_CUSTOM         0
#define NA_MEMALIGN_USE_ALIGNED_ALLOC  1
#define NA_MEMALIGN_USE_POSIX          2

// Default is POSIX

#ifndef NA_MEMALIGN
  #define NA_MEMALIGN NA_MEMALIGN_USE_POSIX
#endif



// ////////////////////////////////
// Integer emulation
// ////////////////////////////////

// Integers with 64, 128 or even 256 bits are not available on all systems as
// native types. NALib has emulations for these but they are not accessible
// using simple integral types but as structs and need special function calls
// to work properly. Therefore, they are only implemented where really
// needed internally in NALib.
//
// The need though might arise that applications should be written for systems
// which do not have the native types. Therefore, one can define with the
// preprocessor macro NA_NATIVE_INTEGER_UP_TO to what maximal bit number NALib
// shall use the native types. Everything above will be emulated.

// value | int32  | int64    | int128   | int256 
// ------+--------+----------+----------+----------
// 32    | navive | emulated | emulated | emulated
// 64    | native | native*  | emulated | emulated
// 128   | native | native*  | native*  | emulated
// 256   | native | native*  | native*  | native*
// * = if available on the system
// 32 bits integers are assumed to always be available.
//
// Default is 256

#define NA_NATIVE_INTEGER_UP_TO 256



// ////////////////////////////////
// Runtime memory pools
// ////////////////////////////////

// Define the size of a memory pool part. This size is used for the
// NALib runtime system, more precisely allocation with naNew, as well as
// for garbage collection.
//
// If you set this macro to 0, one memory page size will be used.
//
// Turns out, on most systems, one pageSize is far too small to result in
// good speed improvements for naNew. A large enough custom byteSize can
// result in up to 2 times faster allocation and deallocation.
//
// Default is (1 << 16)

#ifndef NA_POOLPART_BYTESIZE
  #define NA_POOLPART_BYTESIZE (1 << 16)
#endif

// Define if memory pools shall be cleaned up aggressively.
//
// Memory pools do organize themselves by allocating and deallocating large
// chunks of memory. Upon startup, there is no memory allocated whatsoever.
// This should of course be the same in the end when the runtime system
// stops. Nontheless, it might be very helpful to always keep one chunk per
// type in memory to prevent allocating and deallocating the same (first)
// chunk over and over again.
//
// With this flag, the programmer defines, if that last chunk shall be freed
// aggressively, meaning as soon as no more memory is needed for that type,
// all related pools get deallocated. If set to 0, the last chunk stays in
// memory up until the runtime system stops.
//
// This flag is important for systems with very small memory as well as for
// performance testing: When clenaing up aggressively, the runtime can be
// considerably slower. Otherwise, one big chunk of memory always stays
// ready resulting in very high performance, especially for small testing
// cases. But this might not reveal potential performance losses when using
// the tested functionality in real life.
//
// Default is 0

#ifndef NA_MEMORY_POOL_AGGRESSIVE_CLEANUP
  #define NA_MEMORY_POOL_AGGRESSIVE_CLEANUP 0
#endif

// Defines when the temp garbage collection starts collecting automatically.
//
// With this macro, you can define, if and when the garbage collection should
// perform an automatic collection of temporarily allocated memory. If you
// set this value to 0, no automatic collection will be performed ever. You
// need to call naCollectGarbage manually.
//
// If this macro is set to a non-zero value, it will fire during a call to
// naMallocTmp but only when the sum of all previously temporary allocated
// bytes had become higher than this number in the previous call to
// naMallocTmp. If so, the garbage will first be collected and then the new
// bytes will be allocated and returned.
//
// The default value is 1000000 (1 Million) when no GUI is in use and 0 when
// a GUI is in use. When a GUI is in use, the event loop of NAApplication
// calls naCollectGarbage automatically before executing any event.

#ifndef NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT
  #if NA_COMPILE_GUI == 0
    #define NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT 1000000
  #else
    #define NA_GARBAGE_TMP_AUTOCOLLECT_LIMIT 0
  #endif
#endif



// ////////////////////////////////
// Buffers
// ////////////////////////////////

// Define the size of a buffer part used for NABuffer:
//
// With the following macro, you can define, what the default byte size of
// one buffer part shall be. The value 0 denotes that the memory page size
// will be used. Custom values typically are powers of 2, for example 4096.
//
// The default is 0

#ifndef NA_BUFFER_PART_BYTESIZE
  #define NA_BUFFER_PART_BYTESIZE 0
#endif



// String caching
//
// When set to 1, the NAString structure always tries to create a cached
// version of its content. This is really only useful when debugging as the
// cached version will be stored within the NAString structure. With NA_DEBUG
// being 0 though, nothing will be stored as it is in most cases just a waste
// of time. Nontheless as caching will take a considerable amount of time even
// in debugging, you may choose to set NA_STRING_ALWAYS_CACHE to 0 even for
// debugging right here.
//
// The default is that strings will always be cached when running in debug
// mode but will not be cached if NA_DEBUG is 0.

#ifndef NA_STRING_ALWAYS_CACHE
  #if NA_DEBUG
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
// 0 if you want to use "Mutex".
//
// Default is 1

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
// With the following macro, you define, what year of the CODATA you would
// like to use. Possible values are 2006, 2010, 2014, 2018 and 2022.
//
// By default, NALib uses the latest setting.

#ifndef NA_NIST_CODATA_YEAR
  #define NA_NIST_CODATA_YEAR 2022
#endif



// ////////////////////////////////
// Testing
// ////////////////////////////////

// NALib uses NATest as a testing framework. Set this macro to 1 if you want
// to use this functionality. In that case, you need to link/compile with the
// NATest library. In the distribution, it is located in the lib folder and
// you can get it as a git submodule.
//
// The default is 0.

#ifndef NA_TESTING_ENABLED
  #define NA_TESTING_ENABLED 0
#endif



#endif // NA_CONFIGURATION_INCLUDED



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
