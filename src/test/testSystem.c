

#include "NASystem.h"


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



void testSystem(void){
  printf("NALib Version %d (%s, %d Bits, %s)\n", NA_VERSION, na_system_strings[NA_SYSTEM], NA_SYSTEM_ADDRESS_BITS, na_endianness_strings[NA_SYSTEM_ENDIANNESS]);
}

