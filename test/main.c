
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


#include "../NALib/NAPool.h"

int main(void){
  printSystemTest();
  
  int test1 = 5;
  int test2 = 6;
  int* ret;
  NAPool pool;
  naCreatePoolEmpty(&pool, 10);
  naSpitPool(&pool, &test1);
  naSpitPool(&pool, &test2);
  naSpitPool(&pool, &test1);
  ret = naSuckPool(&pool);
  printf("%d\n", *ret);
  ret = naSuckPool(&pool);
  printf("%d\n", *ret);
//  ret = naSuckPool(&pool);
//  printf("%d\n", *ret);
  naClearPool(&pool);
  
  printf("\nPress enter to quit.\n");
  fgetc(stdin);
  return 0;
}

