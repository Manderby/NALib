

#include "tests.h"

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



const char* na_signed_integer_encoding_strings[NA_ENDIANNESS_COUNT] = {
  "Unknown",
  "Sign & Magnitude",
  "One's complement",
  "Two's complement",
};



void testSystem(void){
  printf("\n");
  printf("System Test\n");
  printf("===========\n");
  
  printf("NALib Version:            %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug)\n");
  #else
    printf("(Release)\n");
  #endif
  
  printf("System:                   %s\n", na_system_strings[NA_SYSTEM]);
  #if defined __cplusplus
    #if defined NA_CPP14
      printf("Compiled for              C++14 (%d)\n", __cplusplus);
    #elif defined NA_CPP11
      printf("Compiled for              C++11 (%d)\n", __cplusplus);
    #elif defined NA_CPP98
      printf("Compiled for              C++98 (%d)\n", __cplusplus);
    #else
      printf("Compiled for              C++ with no standard\n");
    #endif
  #else
    #if defined NA_C11
      printf("Compiled for              C11 (%d)\n", __STDC_VERSION__);
    #elif defined NA_C99
      printf("Compiled for              C99 (%d)\n", __STDC_VERSION__);
    #elif defined NA_C94
      printf("Compiled for              C94 (%d)\n", __STDC_VERSION__);
    #elif defined NA_C90
      printf("Compiled for              C89 / C90\n");
    #else
      printf("Compiled for              C with no standard\n");
    #endif
  #endif
  
  printf("Endianness:               %s\n", na_endianness_strings[NA_SYSTEM_ENDIANNESS]);
  
  printf("Default \"void*\" size:     %d Bits (%d Bytes)\n", NA_SYSTEM_ADDRESS_BITS, NA_SYSTEM_ADDRESS_BYTES);
  printf("Default \"int\" size:       %d Bits\n", NA_SYSTEM_INT_BITS);
  
  printf("Signed integer encoding:  %s\n", na_signed_integer_encoding_strings[NA_SIGNED_INTEGER_ENCODING]);
}



