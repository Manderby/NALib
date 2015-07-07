
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



#include "../NALib/NAString.h"
#include "../NALib/NAByteArray.h"
#include "../NALib/NADateTime.h"
#define TESTSIZE 10000000

int main(void){
  printSystemTest();
  
  NADateTime time1;
  NADateTime time2;
  
  naStartRuntime();
  
  NAString* newstring;
    newstring = malloc(sizeof(NAByteArray));
    free(newstring);
    newstring = naMalloc(sizeof(NAByteArray));
    naFree(newstring);
    newstring = naNew(NAString);
    naDelete(newstring);
  
  time1 = naMakeDateTimeNow();
  for(int i=0; i<TESTSIZE; i++){
    newstring = malloc(sizeof(NAByteArray));
    naInitByteArray(newstring);
    naClearByteArray(newstring);
    free(newstring);
  }
  time2 = naMakeDateTimeNow();
  printf("Time diff: %f\n", naGetDateTimeDiff(&time2, &time1));

  time1 = naMakeDateTimeNow();
  for(int i=0; i<TESTSIZE; i++){
    newstring = naMalloc(sizeof(NAByteArray));
    naInitByteArray(newstring);
    naClearByteArray(newstring);
    naFree(newstring);
  }
  time2 = naMakeDateTimeNow();
  printf("Time diff: %f\n", naGetDateTimeDiff(&time2, &time1));

  time1 = naMakeDateTimeNow();
  for(int i=0; i<TESTSIZE; i++){
    newstring = naNewString();
    naDelete(newstring);
  }
  time2 = naMakeDateTimeNow();
  printf("Time diff: %f\n", naGetDateTimeDiff(&time2, &time1));
  
  
  printf("\nPress enter to quit.\n");
  fgetc(stdin);
  return 0;
}

