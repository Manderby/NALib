
#if defined NA_MACROS_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_MACROS_INCLUDED
#define NA_MACROS_INCLUDED



// Stringifies any macro no matter if they are undefined, empty or defined.
#define NA_STRINGIFY(...)

// Concatenates the given parameters.
#define NA_CONCAT2(a1, a2)
#define NA_CONCAT3(a1, a2, a3)
#define NA_CONCAT4(a1, a2, a3, a4)

// Concatenates the given parameters by first evaluating them.
#define NA_CONCAT_EVAL2(a1, a2)
#define NA_CONCAT_EVAL3(a1, a2, a3)
#define NA_CONCAT_EVAL4(a1, a2, a3, a4)

// Template function generator. Use this macro to create a generic name of
// the function with the specified template arguments.
#define NA_T1(name, arg1)
#define NA_T2(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3)



// Inline implementations are in a separate file:
#include "Core/NAMacrosII.h"



#endif // NA_MACROS_INCLUDED



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
