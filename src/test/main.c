

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include <stdio.h>
#include "../NALib/NASystem.h"



const char* na_system_strings[NA_SYSTEM_COUNT] = {
  "Unknown",
  "Mac OS X",
  "Windows",
};



const char* na_endianness_strings[NA_ENDIANNESS_COUNT] = {
  "Unknown Endianness",
  "Little Endian",
  "Big Endian",
  "Native Endianness",
};



void printSystemTest(){
  printf("NALib Version %d (%s, %d Bits, %s)\n", NA_VERSION, na_system_strings[NA_SYSTEM], NA_SYSTEM_ADDRESS_BITS, na_endianness_strings[NA_SYSTEM_ENDIANNESS]);
}



#include "../NALib/NAString.h"
#include "../NALib/NAPNG.h"
#include "../NALib/NAMemory.h"
#include "../NALib/NADateTime.h"

  #define TESTCOUNT 1000000

int main(void){
  printSystemTest();
  
  naStartRuntime();
  naStopRuntime();
  
  printf("\nPress enter to quit.\n");
  fgetc(stdin);
  return 0;
}

