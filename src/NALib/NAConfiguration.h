
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains configurations of your installation of NALib. There are
// some few options you can compile NALib with. These options are implemented
// as preprocessor macros such that you can define them with prefix headers
// when you need them in your special project.
//
// NAConfiguration.h is included at the beginning of the NASystem.h file.
// Do not include it anywhere else.
//
// Note that there is no possibility to platform-independently provide a clean
// configuration structure without the need to configure either the NALib
// installation or your compiler in one or another way. The solution with this
// file allows you to share NALib with a default implementation but which can
// be altered project-wise by as simple compiler prefix macro definition.




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
// Runtime memory pools
// ////////////////////////////////

// By default, the following macro is defined as 1 and therefore, NALib will
// use its own implementation of memory pools instead of malloc and free for
// anything related to naNew and naDelete.
//
// The implementation with pools proved to be a lot faster with older compilers
// but with some more modern compilers, especially in conjunction with C++, the
// basic malloc and free can (though rarely) be faster.

#ifndef NA_RUNTIME_USES_MEMORY_POOLS
  #define NA_RUNTIME_USES_MEMORY_POOLS 1
#endif

// Define the size of a core memory pool:
//
// With the following macro, you can define, what the byte size of the memory
// pools shall be. The default value for the pool size is (1<<16). If you set
// this macro to 0, the memory page size will be used.
//
// Turns out, on most systems, the pagesize is far too small to result in good
// speed improvements. A large enough custom bytesize can result in up to
// 2 times faster allocation and deallocation.

#ifndef NA_COREPOOL_BYTESIZE
  #define NA_COREPOOL_BYTESIZE (1 << 16)
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


// When set to 1, the NAString structure always creates a cached version of
// its content. This will take considerably more time if you use lots of string
// manipulations but will help when debugging as the debugger can display the
// content as char* immediately.
//
// Default is 0: The NAString struct will only create a cached version when
// necessary.

#ifndef NA_STRING_ALWAYS_CACHE
  #define NA_STRING_ALWAYS_CACHE 1
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
