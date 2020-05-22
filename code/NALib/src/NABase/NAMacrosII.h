
#if defined NA_MACROS_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_MACROS_II_INCLUDED
#define NA_MACROS_II_INCLUDED



// First, undefine the macros from the .h file.
#undef NA_STRINGIFY
#undef NA_CONCAT1
#undef NA_CONCAT2
#undef NA_CONCAT3
#undef NA_CONCAT4
#undef NA_TOKEN_EVAL1
#undef NA_TOKEN_EVAL2
#undef NA_TOKEN_EVAL3
#undef NA_TOKEN_EVAL4
#undef NA_T1
#undef NA_T2
#undef NA_T3

#undef naPrintMacroPlain
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
#define NA_STRINGIFY(...)                #__VA_ARGS__ ""

#define NA_CONCAT1(a1 )                 a1
#define NA_CONCAT2(a1, a2)              a1 ## a2
#define NA_CONCAT3(a1, a2, a3)          a1 ## a2 ## a3
#define NA_CONCAT4(a1, a2, a3, a4)      a1 ## a2 ## a3 ## a4
#define NA_TOKEN_EVAL1(a1)              NA_CONCAT1(a1)
#define NA_TOKEN_EVAL2(a1, a2)          NA_CONCAT2(a1, a2)
#define NA_TOKEN_EVAL3(a1, a2, a3)      NA_CONCAT3(a1, a2, a3)
#define NA_TOKEN_EVAL4(a1, a2, a3, a4)  NA_CONCAT4(a1, a2, a3, a4)

#define NA_T1(name, arg1)               NA_TOKEN_EVAL2(name, arg1)
#define NA_T2(name, arg1, arg2)         NA_TOKEN_EVAL3(name, arg1, arg2)
#define NA_T3(name, arg1, arg2, arg3)   NA_TOKEN_EVAL4(name, arg1, arg2, arg3)



// /////////////////////////////////
// Printouts
// /////////////////////////////////

#if NA_PRINTOUT_ENABLED == 1

void naPrintMacroPlainCore(const NAUTF8Char* macroString, const NAUTF8Char* macroStringified);
void naPrintMacroIntCore(const NAUTF8Char* macroString, int value);
void naPrintMacroIntSpecialCore(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString);
void naPrintMacroIntSpecialHexCore(const NAUTF8Char* macroString, int value, int specialValue, const NAUTF8Char* specialString);
void naPrintMacroIntYesNoCore(const NAUTF8Char* macroString, int value);
void naPrintMacroEnumCore(const NAUTF8Char* macroString, int value, const NAUTF8Char** strings, int enumCount);

void naPrintMacroux8Core(const NAUTF8Char* macroString, uint8 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroix8Core(const NAUTF8Char* macroString, int8 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroux16Core(const NAUTF8Char* macroString, uint16 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroix16Core(const NAUTF8Char* macroString, int16 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroux32Core(const NAUTF8Char* macroString, uint32 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroix32Core(const NAUTF8Char* macroString, int32 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroux64Core(const NAUTF8Char* macroString, uint64 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroix64Core(const NAUTF8Char* macroString, int64 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroux128Core(const NAUTF8Char* macroString, uint128 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroix128Core(const NAUTF8Char* macroString, int128 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroux256Core(const NAUTF8Char* macroString, uint256 value, const NAUTF8Char* macroStringify, NABool showMacro);
void naPrintMacroix256Core(const NAUTF8Char* macroString, int256 value, const NAUTF8Char* macroStringify, NABool showMacro);

#define naPrintMacroPlain(macro)\
  naPrintMacroPlainCore(#macro, NA_STRINGIFY(macro))
#define naPrintMacroInt(macro)\
  naPrintMacroIntCore(#macro, (int)macro)
#define naPrintMacroIntSpecial(macro, specialValue, specialString)\
  naPrintMacroIntSpecialCore(#macro, (int)macro, (int)specialValue, specialString)
#define naPrintMacroIntSpecialHex(macro, specialValue, specialString)\
  naPrintMacroIntSpecialHexCore(#macro, (int)macro, (int)specialValue, specialString)
#define naPrintMacroIntYesNo(macro)\
  naPrintMacroIntYesNoCore(#macro, (int)macro)
#define naPrintMacroEnum(macro, strings, maxValue)\
  naPrintMacroEnumCore(#macro, (int)macro, strings, maxValue)

#define naPrintMacroux8(macro, showMacro)\
  naPrintMacroux8Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroix8(macro, showMacro)\
  naPrintMacroix8Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroux16(macro, showMacro)\
  naPrintMacroux16Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroix16(macro, showMacro)\
  naPrintMacroix16Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroux32(macro, showMacro)\
  naPrintMacroux32Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroix32(macro, showMacro)\
  naPrintMacroix32Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroux64(macro, showMacro)\
  naPrintMacroux64Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroix64(macro, showMacro)\
  naPrintMacroix64Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroux128(macro, showMacro)\
  naPrintMacroux128Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroix128(macro, showMacro)\
  naPrintMacroix128Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroux256(macro, showMacro)\
  naPrintMacroux256Core(#macro, macro, NA_STRINGIFY(macro), showMacro)
#define naPrintMacroix256(macro, showMacro)\
  naPrintMacroix256Core(#macro, macro, NA_STRINGIFY(macro), showMacro)

#else // NA_PRINTOUT_ENABLED == 1

#define naPrintMacroPlain(macro)
#define naPrintMacroInt(macro)
#define naPrintMacroIntSpecial(macro, specialValue, specialString)
#define naPrintMacroIntSpecialHex(macro, specialValue, specialString)
#define naPrintMacroIntYesNo(macro)
#define naPrintMacroEnum(macro, strings, maxValue)

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
