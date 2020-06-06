
#include "NATesting.h"
#include <stdio.h>



// Prototypes
void printNAConfiguration(void);
void printNAEnvironment(void);
void printNACompiler(void);
void printNALanguage(void);
void printNAEncoding(void);
void printNAChar(void);
void printNAInteger(void);
void printNAMacros(void);
void printNADebugging(void);
void printNANumerics(void);
void printNAFloatingPoint(void);

void testNALanguage(void);
void testNAChar(void);
void testNAInteger(void);
void testNAInt64(void);
void testNAInt128(void);
void testNAMacros(void);
void testNADebugging(void);
void testNANumerics(void);
void testNAFloatingPoint(void);



void printNABase(){
  printNAConfiguration();
  printNAEnvironment();
  printNACompiler();
  printNALanguage();
  printNAEncoding();
  printNAChar();
  printNAInteger();
  printNAMacros();
  printNADebugging();
  printNANumerics();
  printNAFloatingPoint();
}

void testNABase(){
  //naTestGroupFunction(NALanguage);
  //naTestGroupFunction(NAChar);
  //naTestGroupFunction(NAInteger);
  naTestGroupFunction(NAInt64);
  naTestGroupFunction(NAInt128);
  //naTestGroupFunction(NAMacros);
  //naTestGroupFunction(NADebugging);
  //naTestGroupFunction(NANumerics);
  //naTestGroupFunction(NAFloatingPoint);
}



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
