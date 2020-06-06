
#if defined NA_MACROS_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_MACROS_II_INCLUDED
#define NA_MACROS_II_INCLUDED



// First, undefine the macros from the .h file.
#undef NA_STRINGIFY
#undef NA_CONCAT2
#undef NA_CONCAT3
#undef NA_CONCAT4
#undef NA_CONCAT_EVAL2
#undef NA_CONCAT_EVAL3
#undef NA_CONCAT_EVAL4
#undef NA_T1
#undef NA_T2
#undef NA_T3

#undef naPrintMacro
#undef naPrintMacroDefined
#undef naPrintMacroInt
#undef naPrintMacroIntSpecial
#undef naPrintMacroIntSpecialHex
#undef naPrintMacroIntYesNo
#undef naPrintMacroEnum

#undef naPrintMacroux8
#undef naPrintMacroix8
#undef naPrintMacroux16
#undef naPrintMacroix16
#undef naPrintMacroux32
#undef naPrintMacroix32
#undef naPrintMacroux64
#undef naPrintMacroix64
#undef naPrintMacroux128
#undef naPrintMacroix128
#undef naPrintMacroux256
#undef naPrintMacroix256



// My proudest achievement at 4 o'clock in the morning.
#define NA_STRINGIFY(...)               #__VA_ARGS__ ""

#define NA_CONCAT2(a1, a2)              a1 ## a2
#define NA_CONCAT3(a1, a2, a3)          a1 ## a2 ## a3
#define NA_CONCAT4(a1, a2, a3, a4)      a1 ## a2 ## a3 ## a4

#define NA_CONCAT_EVAL2(a1, a2)         NA_CONCAT2(a1, a2)
#define NA_CONCAT_EVAL3(a1, a2, a3)     NA_CONCAT3(a1, a2, a3)
#define NA_CONCAT_EVAL4(a1, a2, a3, a4) NA_CONCAT4(a1, a2, a3, a4)

// To create template name mangling, a little preprocessor trick needs to be
// applied: The double indirection macro concatenation is a trick implied by
// the standard. The problem is that the ## directive will not evaluate
// macros therefore, one has to create a forwarding macro which evaluates it,
// then the evaluated macros get concatenated in a second macro. 
#define NA_T1(name, arg1)               NA_CONCAT_EVAL2(name, arg1)
#define NA_T2(name, arg1, arg2)         NA_CONCAT_EVAL3(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3)   NA_CONCAT_EVAL4(name, arg1, arg2, arg3)



// /////////////////////////////////
// Printouts
// /////////////////////////////////

#if NA_PRINTOUT_ENABLED == 1

  // Prototypes of helper functions which should be invisible to the user.
  NA_HHAPI void na_PrintMacroPlain(const NAUTF8Char* macroString, const NAUTF8Char* macroStringified, NABool printContent);
  NA_HHAPI void na_PrintMacroInt(const NAUTF8Char* macroString, int value);
  NA_HHAPI void na_PrintMacroIntSpecial(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString);
  NA_HHAPI void na_PrintMacroIntSpecialHex(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString);
  NA_HHAPI void na_PrintMacroIntYesNo(const NAUTF8Char* macroString, int value);
  NA_HHAPI void na_PrintMacroEnum(const NAUTF8Char* macroString, int value, const NAUTF8Char** strings, int enumCount);

  NA_HHAPI void na_PrintMacroux8(const NAUTF8Char* macroString, uint8 value);
  NA_HHAPI void na_PrintMacroix8(const NAUTF8Char* macroString, int8 value);
  NA_HHAPI void na_PrintMacroux16(const NAUTF8Char* macroString, uint16 value);
  NA_HHAPI void na_PrintMacroix16(const NAUTF8Char* macroString, int16 value);
  NA_HHAPI void na_PrintMacroux32(const NAUTF8Char* macroString, uint32 value);
  NA_HHAPI void na_PrintMacroix32(const NAUTF8Char* macroString, int32 value);
  NA_HHAPI void na_PrintMacroux64(const NAUTF8Char* macroString, uint64 value);
  NA_HHAPI void na_PrintMacroix64(const NAUTF8Char* macroString, int64 value);
  NA_HHAPI void na_PrintMacroux128(const NAUTF8Char* macroString, uint128 value);
  NA_HHAPI void na_PrintMacroix128(const NAUTF8Char* macroString, int128 value);
  NA_HHAPI void na_PrintMacroux256(const NAUTF8Char* macroString, uint256 value);
  NA_HHAPI void na_PrintMacroix256(const NAUTF8Char* macroString, int256 value);

  #define naPrintMacro(macro)\
    na_PrintMacroPlain(#macro, NA_STRINGIFY(macro), NA_TRUE)
  #define naPrintMacroDefined(macro)\
    na_PrintMacroPlain(#macro, NA_STRINGIFY(macro), NA_FALSE)
  #define naPrintMacroInt(macro)\
    na_PrintMacroInt(#macro, (int)macro)
  #define naPrintMacroIntSpecial(macro, specialValue, specialString)\
    na_PrintMacroIntSpecial(#macro, (int)macro, (int)specialValue, specialString)
  #define naPrintMacroIntSpecialHex(macro, specialValue, specialString)\
    na_PrintMacroIntSpecialHex(#macro, (int)macro, (int)specialValue, specialString)
  #define naPrintMacroIntYesNo(macro)\
    na_PrintMacroIntYesNo(#macro, (int)macro)
  #define naPrintMacroEnum(macro, strings, maxValue)\
    na_PrintMacroEnum(#macro, (int)macro, strings, maxValue)

  #define naPrintMacroux8(macro)\
    na_PrintMacroux8(#macro, macro)
  #define naPrintMacroix8(macro)\
    na_PrintMacroix8(#macro, macro)
  #define naPrintMacroux16(macro)\
    na_PrintMacroux16(#macro, macro)
  #define naPrintMacroix16(macro)\
    na_PrintMacroix16(#macro, macro)
  #define naPrintMacroux32(macro)\
    na_PrintMacroux32(#macro, macro)
  #define naPrintMacroix32(macro)\
    na_PrintMacroix32(#macro, macro)
  #define naPrintMacroux64(macro)\
    na_PrintMacroux64(#macro, macro)
  #define naPrintMacroix64(macro)\
    na_PrintMacroix64(#macro, macro)
  #define naPrintMacroux128(macro)\
    na_PrintMacroux128(#macro, macro)
  #define naPrintMacroix128(macro)\
    na_PrintMacroix128(#macro, macro)
  #define naPrintMacroux256(macro)\
    na_PrintMacroux256(#macro, macro)
  #define naPrintMacroix256(macro)\
    na_PrintMacroix256(#macro, macro)

#else

  #define naPrintMacro(macro)
  #define naPrintMacroDefined(macro)
  #define naPrintMacroInt(macro)
  #define naPrintMacroIntSpecial(macro, specialValue, specialString)
  #define naPrintMacroIntSpecialHex(macro, specialValue, specialString)
  #define naPrintMacroIntYesNo(macro)
  #define naPrintMacroEnum(macro, strings, maxValue)

  #define naPrintMacroux8(macro)
  #define naPrintMacroix8(macro)
  #define naPrintMacroux16(macro)
  #define naPrintMacroix16(macro)
  #define naPrintMacroux32(macro)
  #define naPrintMacroix32(macro)
  #define naPrintMacroux64(macro)
  #define naPrintMacroix64(macro)
  #define naPrintMacroux128(macro)
  #define naPrintMacroix128(macro)
  #define naPrintMacroux256(macro)
  #define naPrintMacroix256(macro)

#endif // NA_PRINTOUT_ENABLED == 1


#endif // NA_MACROS_II_INCLUDED



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
