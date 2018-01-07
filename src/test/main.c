

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include "../NALib/NASystem.h"
#include <stdio.h>

int main(void){
  printf("NALib Version: %d ", NA_VERSION);
  #ifndef NDEBUG
    printf("(Debug ");
  #else
    printf("(Release ");
  #endif
  printf("%d Bits)\n", NA_SYSTEM_ADDRESS_BITS);

  return 0;
}

