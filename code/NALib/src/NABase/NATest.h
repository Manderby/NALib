
// Starts and stops the testing. Provide the top name of the code you
// want to test. A final printout will be made upon stopping.
NA_API void naStartTesting(const NAUTF8Char* rootName);
NA_API void naStopTesting(void);

// The testing calls.
// naT: Tests successfully if the given expression evaluates to true.
// naE: Tests successfully if one or more naError calls happen.
// naG: Groups together tests. Use it like naG("Multiple Tests"){ ... }
// naF: Calls a function with type void(*)(void) with the prefix "test" and
//      uses the given name as a group name.
//      For example naF(Algo) calls void testAlgo().
#define naT(expr)
#define naE(expr)
#define naG(string)
#define naF(identifier)



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
