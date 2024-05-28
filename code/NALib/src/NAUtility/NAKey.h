
#ifndef NA_KEY_INCLUDED
#define NA_KEY_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "../NABase/NABase.h"

// This file contains helper functions for creating functions very similar to
// template functions in C++. To create a template-like behaviour, certain
// tricks with the preprocessor are used. It is not recommended to use this
// too extensively but for certain elements of NALib, they are very well fit.
//
// Implementation on this has started after the NATree implementation had
// been completed. The trees required key values to create ordered search
// trees. These keys had to exist in multiple types, like double, NAInt,
// NAPos, NAVertex, etc. A first implementation used function pointers which
// worked fine but had the drawback of requiring lots of function calls for
// very tiny functions.
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
// NA_T1, NA_T2, NA_T3,... macros have been defined in NABaseMacros.h. They
// can be used in the whole code and will evaluate to the desired function.
// The function signature in the template file must use the same macros to
// name the functions.




// This macro helps you define the name of the key function in a starndardized
// way. For example:
//
// NA_KEY_OP(Greater, double)  ->  naGreaterKeydouble
//
// You are free to use the evaluated name directly bu beware if the API
// should change!
#define NA_KEY_OP(operator, type) NA_CONCAT4(na, operator, Key, type)



NA_IAPI void   NA_KEY_OP(Assign,       double)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Less,        double)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LessEqual,   double)(const void* a, const void* b);

NA_IAPI void   NA_KEY_OP(Assign,       float)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Less,        float)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LessEqual,   float)(const void* a, const void* b);

NA_IAPI void   NA_KEY_OP(Assign,       NAInt)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Less,        NAInt)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LessEqual,   NAInt)(const void* a, const void* b);

NA_IAPI void   NA_KEY_OP(Assign,       NAPos)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Less,        NAPos)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LessEqual,   NAPos)(const void* a, const void* b);

NA_IAPI void   NA_KEY_OP(Assign,       NAVertex)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Less,        NAVertex)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LessEqual,   NAVertex)(const void* a, const void* b);

NA_IAPI void   NA_KEY_OP(Assign,       NADateTime)(void* dst, const void* src);
NA_IAPI NABool NA_KEY_OP(Greater,      NADateTime)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(GreaterEqual, NADateTime)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Equal,        NADateTime)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(Less,        NADateTime)(const void* a, const void* b);
NA_IAPI NABool NA_KEY_OP(LessEqual,   NADateTime)(const void* a, const void* b);



#include "Core/NAKeyII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_KEY_INCLUDED



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
