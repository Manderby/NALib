

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//

#include "NABuffer.h"
#include "NAFile.h"

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

  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
   NA_UNUSED(getchar());
  #endif
  
  naStartRuntime();

  NABuffer* buf = naNewBufferWithInpuFile("test.txt");
  NABufferIterator iter = naMakeBufferAccessor(buf);
  naSeekBufferFromStart(&iter, 0);
  printf("%d\n", naParseBufferInt32(&iter, NA_FALSE));
  naClearBufferIterator(&iter);
  naRelease(buf);
  
  naStopRuntime();
  
  return 0;
}

