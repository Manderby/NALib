
#include "NATesting.h"
#include "NAUtility/NAMemory.h"
#include "NAUtility/NAString.h"
#include <stdio.h>



// Prototypes
void printNABase(void);
void printNACore(void);
void printNAStruct(void);

void testNABase(void);
void testNACore(void);
void testNAStruct(void);

void benchmarkNABase(void);
void benchmarkNAStruct(void);



void printNALib(void){
  #if NA_PRINTOUT_ENABLED == 1
    printNABase();
    printNACore();
    printNAStruct();
  #endif
}

void testNALib(void){
  naTestFunction(testNABase);
  naTestFunction(testNACore);
  naTestFunction(testNAStruct);
}

void benchmarkNALib(void){
  printf(NA_NL "Benchmarking:" NA_NL);
  benchmarkNABase();
  benchmarkNAStruct();
}



int main(int argc, const char** argv){
  printf("Testing NALib Version: %d ", NA_VERSION);
  #if NA_DEBUG
    printf("(Debug)" NA_NL);
  #else
    printf("(Release)" NA_NL);
  #endif

  naStartRuntime();

  // Print macro information
//  printNALib();

  // Start testing
  NABool testStartSuccessful = naStartTesting("NALib", .01, NA_FALSE, argc, argv);
  if(testStartSuccessful)
  {
    testNALib();
//    naPrintUntested();
//    benchmarkNALib();
  }else{
    printf("Could not start Testing.");
  }
  
  printf(NA_NL);
  naStopTesting();

  naStopRuntime();

  return EXIT_SUCCESS;
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


