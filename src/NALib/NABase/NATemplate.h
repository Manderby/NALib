
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"

// This file contains helper macros for creating functions very similar to
// template functions in C++. To create a template-like behaviour, certain
// tricks with the preprocessor are used. It is not recommended to use this
// too extensively but for certain elements of NALib, they are very well fit.
//
// Implementation on this has started after the NATree implementation had
// been completed. The trees required key values to create ordered search
// trees. These keys had to exist in multiple types, like double, NAInt, NAPos,
// NAVertex, etc. To use these different types, the first implementation used
// function pointers which worked fine but had the drawback of requiring lots
// of function calls for very tiny functions.
//
// On the other hand, the NAHeap implementation had an implementation of
// multiple key types since the very beginning of NALib. It used multi-line
// macros which expanded to functions which then could be used to fill the
// interiour of a function body with very specific source code snipplets.
// The drawback of that method is that the actual implementation of the logic
// is hidden withing a complicated macro and is not debuggable.
//
// The advantage of macros is their speed. A direct comparison was made
// between an implementation of NAHeap with the traditional macros and a
// reprogrammed version using function pointers. The result was that even
// with a highly optimizing modern compiler, more than 10% of performance
// was lost due to the inability to inline the code and hence using function
// calls.
//
// Therefore, a new way was found to create a template-like behaviour which
// combines the advantages of both methods: Inlineable code which is
// high-performance and function bodies which are easy to read and debug.
//
// The method goes approximately like this: If a function requires template
// parameters, it is placed in a template file, marked with the T at the end,
// for example NAHeapT.h for the heap. That template file is then included
// in another file whereas all necessary template arguments are defined as
// macros in the outside file. For example:
//
// #define NA_T_KEY_TYPE double
//   #include "NAHeapT.h"
// #undef NA_T_KEY_TYPE
//
// With that, the template argument is available in the template file and
// can be used in whatever way the programmer wants. Important is that the
// template parameter must be un-defined after the include.
//
// To make a standardized way of dealing with the name mangling problem of
// re-declaring the same function with different template arguments, the
// NA_T1, NA_T2, NA_T3,... macros have been defined. They can be used in
// the whole code and will evaluate to the desired function. The function
// signature in the template file must use the same macros to name the
// functions.
//
// To create that name mangling, a little preprocessor trick needed to be
// applied:
//
// The double indirection macro concatenation is a trick implied by the
// standard. The problem is that the ## directive will not evaluate macros
// therefore, one has to create a forwarding macro which evaluates it and
// then the evaluated macros get concatenated in a second macro. 
#define NA_CONCAT2(a1, a2)              a1 ## a2
#define NA_CONCAT3(a1, a2, a3)          a1 ## a2 ## a3
#define NA_CONCAT4(a1, a2, a3, a4)      a1 ## a2 ## a3 ## a4
#define NA_TOKEN_EVAL2(a1, a2)          NA_CONCAT2(a1,a2)
#define NA_TOKEN_EVAL3(a1, a2, a3)      NA_CONCAT3(a1,a2,a3)
#define NA_TOKEN_EVAL4(a1, a2, a3, a4)  NA_CONCAT4(a1,a2,a3,a4)

// Template function generator. Use this macro to create a generic name of the
// function with the specified template arguments.
#define NA_T1(name, arg1)               NA_TOKEN_EVAL2(name, arg1)
#define NA_T2(name, arg1, arg2)         NA_TOKEN_EVAL3(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3)   NA_TOKEN_EVAL4(name, arg1, arg2, arg3)




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
