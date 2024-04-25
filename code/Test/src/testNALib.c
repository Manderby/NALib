
// You need the NATest package for this project!
// It is located in the lib folder and can be optained as a git submodule. Run
// the following commands in the base folder of NALib:
//   git submodule init
//   git submodule update
// Or download the whole NALib package with the following command directly:
//   git clone --recurse-submodules https://github.com/Manderby/NALib.git
#include "NATest.h"



#include "NAUtility/NAMemory.h"
#include "NAUtility/NAString.h"
#include <stdio.h>



// Prototypes
void printNABase(void);
void printNACore(void);
void printNAUtility(void);
void printNAStruct(void);

void testNABase(void);
void testNAMath(void);
void testNACore(void);
void testNAUtility(void);
void testNAStruct(void);

void benchmarkNABase(void);
void benchmarkNAMath(void);
void benchmarkNAStruct(void);



void printNALib(void){
  printNABase();
  printNACore();
  printNAUtility();
  printNAStruct();
}

void testNALib(void){
  //naTestFunction(testNABase);
  naTestFunction(testNAMath);
  //naTestFunction(testNACore);
  //naTestFunction(testNAUtility);
  //naTestFunction(testNAStruct);
}

void benchmarkNALib(void){
  printf(NA_NL "Benchmarking:" NA_NL);
  //benchmarkNABase();
  benchmarkNAMath();
  //benchmarkNAStruct();
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
  //printNALib();

  // Start testing
  NABool testStartSuccessful = naStartTesting(
    "NALib",
    argc,
    argv);

  // Only output tests which fail.
  naSetTestPrintsAllTests(NA_FALSE);

  #if NA_DEBUG == 0
    naExecuteErrorTests(NA_FALSE);
    naExecuteCrashTests(NA_FALSE);
  #endif

  if(testStartSuccessful){
    testNALib();
    //naPrintUntested();
    naSetTimePerBenchmark(0.1);
    benchmarkNALib();
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
