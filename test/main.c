

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include <stdio.h>
#include "../NALib/NASystem.h"



const char* na_system_strings[NA_SYSTEM_COUNT] = {
  "Mac OS X",
  "Windows",
  "Unknown"
};



const char* na_endianness_strings[NA_ENDIANNESS_COUNT] = {
  "Native Endianness",
  "Little Endian",
  "Big Endian",
  "Unknown Endianness",
};



void printSystemTest(){
  printf("NALib Version %d (%s, %d Bits, %s)\n", NA_VERSION, na_system_strings[NA_SYSTEM], NA_SYSTEM_ADDRESS_BITS, na_endianness_strings[NA_SYSTEM_ENDIANNESS]);
}



//#include "../NALib/NAMemory.h"
//#include "../NALib/NAUI.h"
//#include "../NALib/NARuntime.h"

int main(void){
  printSystemTest();
  
//  naStartRuntime();
//  naStartDefaultApplication();
//  naInitUI();
//  
//  NAWindow* window = naNewWindow("Brob", 0, 0, 500, 800, NA_TRUE);
//  naShowWindow(window);
//  
//  naRunUI();
//  
//  naClearUI();
//  naStopRuntime();
  
  printf("\nPress enter to quit.\n");
  fgetc(stdin);
  return 0;
}

