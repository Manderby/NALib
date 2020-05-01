
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_BASE_MACROS_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_BASE_MACROS_INCLUDED
#define NA_BASE_MACROS_INCLUDED



// Concatenation and evaluation macros. See Template comments below.
#define NA_CONCAT2(a1, a2)              a1 ## a2
#define NA_CONCAT3(a1, a2, a3)          a1 ## a2 ## a3
#define NA_CONCAT4(a1, a2, a3, a4)      a1 ## a2 ## a3 ## a4
#define NA_TOKEN_EVAL2(a1, a2)          NA_CONCAT2(a1,a2)
#define NA_TOKEN_EVAL3(a1, a2, a3)      NA_CONCAT3(a1,a2,a3)
#define NA_TOKEN_EVAL4(a1, a2, a3, a4)  NA_CONCAT4(a1,a2,a3,a4)

// Template function generator. Use this macro to create a generic name of
// the function with the specified template arguments.
//
// To create such name mangling, a little preprocessor trick needs to be
// applied: The double indirection macro concatenation is a trick implied by
// the standard. The problem is that the ## directive will not evaluate
// macros therefore, one has to create a forwarding macro which evaluates it,
// then the evaluated macros get concatenated in a second macro. 
#define NA_T1(name, arg1)               NA_TOKEN_EVAL2(name, arg1)
#define NA_T2(name, arg1, arg2)         NA_TOKEN_EVAL3(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3)   NA_TOKEN_EVAL4(name, arg1, arg2, arg3)



#endif // NA_BASE_MACROS_INCLUDED

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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
