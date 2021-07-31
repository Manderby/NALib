
#include "NATesting.h"
#include "NAMemory.h"
#include "NAString.h"
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

int main(int argc, const char** argv){

  //printf("NALib Version: %d (", NA_VERSION);
  //#if NA_DEBUG
  //  printf("Debug");
  //#else
  //  printf("Release");
  //#endif
  //printf(", %d Bits Addresses, %d Bits Integers)" NA_NL NA_NL, NA_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  naStartRuntime();


//  for(int i = 1; i < argc; i++)
//  {
//    printf("ArgumentFound: %s" NA_NL, argv[i]);
//  }


  // Print macro information
  #if NA_PRINTOUT_ENABLED == 1
    //printNABase();
    //printNACore();
    //printNAStruct();
  #endif

  // Start testing
  NABool testStartSuccessful = naStartTesting("NALib", .01, NA_FALSE, argc, argv);
  if(testStartSuccessful)
  {
    naTestGroupFunction(NABase);
    naTestGroupFunction(NACore);
    naTestGroupFunction(NAStruct);

    //printf(NA_NL);
    //naPrintUntested();

    //printf(NA_NL "Benchmarking:" NA_NL);
    //benchmarkNABase();
    //benchmarkNAStruct();
    
    printf(NA_NL);
  }else{
    printf("Could not start Testing." NA_NL);
  }
  naStopTesting();



  naStopRuntime();

  return testStartSuccessful ? EXIT_SUCCESS : EXIT_FAILURE;
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
