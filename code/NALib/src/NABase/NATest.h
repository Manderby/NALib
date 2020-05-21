


#include <string.h>



// Starts and stops the testing. Provide the top name of the code you
// want to test. A final printout will be made upon stopping.
NA_API void naStartTesting(const NAUTF8Char* rootName);
NA_API void naStopTesting(void);

// The testing calls.
// naT: Tests the given expression for true.
// naG: Groups together tests. Use it like naG("Multiple Tests"){ ... }
// naF: Calls a function with type void(*)(void) with the prefix "test" and
//      uses the given name as a group name.
//      For example naF(Algo) calls void testAlgo().
#define naT(expr)
#define naG(string)
#define naF(identifier)



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
#include "NATestII.h"



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
