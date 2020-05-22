
#if defined NA_MACROS_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_MACROS_INCLUDED
#define NA_MACROS_INCLUDED



// Stringifies any macro no matter if they are undefined, empty or defined.
#define NA_STRINGIFY(...)

// Concatenation and evaluation macros. See Template comments below.
#define NA_CONCAT1(a1)
#define NA_CONCAT2(a1, a2)
#define NA_CONCAT3(a1, a2, a3)
#define NA_CONCAT4(a1, a2, a3, a4)
#define NA_TOKEN_EVAL1(a1)
#define NA_TOKEN_EVAL2(a1, a2)
#define NA_TOKEN_EVAL3(a1, a2, a3)
#define NA_TOKEN_EVAL4(a1, a2, a3, a4)

// Template function generator. Use this macro to create a generic name of
// the function with the specified template arguments.
//
// To create such name mangling, a little preprocessor trick needs to be
// applied: The double indirection macro concatenation is a trick implied by
// the standard. The problem is that the ## directive will not evaluate
// macros therefore, one has to create a forwarding macro which evaluates it,
// then the evaluated macros get concatenated in a second macro. 
#define NA_T1(name, arg1)
#define NA_T2(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3)



// //////////////////////////////////
// Printout macros.
// The following macros can be used to output some information to stdout.
// Note that they require NA_ENABLE_PRINTOUTS to be 1

// Prints information about the given macro.
// If undefined:            (Undefined)
// If defined but empty:    (Defined)
// If defined with content: The actual content
#define naPrintMacroPlain(macro)

// Prints the value of the macro as as simple decimal int.
#define naPrintMacroInt(macro)

// Prints the value of the macro as a decimal or hexadecimal int but outputs
// the given string if the given special Value occurs.
#define naPrintMacroIntSpecial(macro, specialValue, specialString)
#define naPrintMacroIntSpecialHex(macro, specialValue, specialString)

// Prints the value of the macro as Yes (1) or No (0)
#define naPrintMacroIntYesNo(macro)

// Prints the string indexed by the value of the macro. Provide an array
// of NAUTF8Char* and use maxValue to indicate what is the maximal possible
// value of that array.
#define naPrintMacroEnum(macro, strings, maxValue)

// Prints the given macro as a hex number, interpreted with the given type.
#define naPrintMacroux8(macro, showMacro)
#define naPrintMacroix8(macro, showMacro)
#define naPrintMacroux16(macro, showMacro)
#define naPrintMacroix16(macro, showMacro)
#define naPrintMacroux32(macro, showMacro)
#define naPrintMacroix32(macro, showMacro)
#define naPrintMacroux64(macro, showMacro)
#define naPrintMacroix64(macro, showMacro)
#define naPrintMacroux128(macro, showMacro)
#define naPrintMacroix128(macro, showMacro)
#define naPrintMacroux256(macro, showMacro)
#define naPrintMacroix256(macro, showMacro)



// Inline implementations are in a separate file:
#include "NAMacrosII.h"



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
