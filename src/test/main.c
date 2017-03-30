

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NAMemory.h"
#include "../NALib/NADateTime.h"




int main(void){
  NADateTime time1;
  NADateTime time2;
  
  time1 = naMakeDateTimeNow();

  testSystem();  
  testMemory();

  naStartRuntime();
    testStack();
    testBuffer();
  naStopRuntime();
  
  printf("\nTest completed. ");
  
  time2 = naMakeDateTimeNow();
  printf("Total time: %f seconds\n", naGetDateTimeDifference(&time2, &time1));

  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    printf("\nPress enter to quit.\n");
    fgetc(stdin);
  #endif
  return 0;
}

