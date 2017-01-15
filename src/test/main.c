

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include <stdio.h>
#include "../NALib/NASystem.h"


void testSystem(void);





//#include "../NALib/NAString.h"
//#include "../NALib/NAPNG.h"
//#include "../NALib/NAMemory.h"
//#include "../NALib/NADateTime.h"
//
//#define TESTCOUNT 1000000

int main(void){
  testSystem();
  
//  naStartRuntime();
//  naStopRuntime();
  
  printf("\nPress enter to quit.\n");
  fgetc(stdin);
  return 0;
}

