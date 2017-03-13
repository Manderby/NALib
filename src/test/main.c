

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NAMemory.h"




int main(void){
//  testSystem();  
  testMemory();

  naStartRuntime();
//    testStack();
//    testBuffer();
  naStopRuntime();
  
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    printf("\nPress enter to quit.\n");
    fgetc(stdin);
  #endif
  return 0;
}

