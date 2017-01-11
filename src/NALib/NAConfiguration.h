
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
// configuration structure without the need to configure the NALib installation
// or your compiler one or the other way. That is unfortunate but a little
// macro preprocessor definition is quickly done.



// ////////////////////////////////
// OpenGL support
// ////////////////////////////////

#ifndef NA_CONFIG_COMPILE_OPENGL
  #define NA_CONFIG_COMPILE_OPENGL 0
#endif

// By default, this macro is defined as 0 and therefore, NALib does not compile
// with OpenGL support. But if you want to, simply set the this macro to 1.
//
// If done so, you MUST link your program to the OpenGL library.



// ////////////////////////////////
// Runtime memory pools
// ////////////////////////////////

#ifndef NA_RUNTIME_USES_MEMORY_POOLS
  #define NA_RUNTIME_USES_MEMORY_POOLS 1
#endif

// By default, this macro is defined as 1 and therefore, NALib will use its
// own implementation of memory pools instead of malloc and free for anything
// related to naNew and naDelete.
//
// The implementation with pools proved to be a lot faster with older compilers
// but with some more modern compilers, especially in conjunction with C++, the
// basic malloc and free can be faster.



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
