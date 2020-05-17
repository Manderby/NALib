
// This is just a small file to get you started using NALib.
// It should compile and run and print some version notes on the screen.



#include "../../NALib/src/NABase.h"
#include "../../NALib/src/NAString.h"
#include <stdio.h>

int main(void){
  printf("NALib Version: %d (", NA_VERSION);
  #if defined NA_C11
    printf("C11");
  #elif defined NA_C99
    printf("C99");
  #elif defined NA_C90
    printf("C90");
  #else
    printf("nonSTDC");
  #endif
  #ifndef NDEBUG
    printf(" Debug");
  #else
    printf(" Release");
  #endif
  printf(" %d Bits Addresses, %d Bits Integers)" NA_NL, NA_ADDRESS_BITS, NA_TYPE_NAINT_BITS);

  #if NA_OS == NA_OS_WINDOWS
    printf("Finished." NA_NL);
    NA_UNUSED(getchar());
  #endif

  
  
  naStartRuntime();
    NAString* testString = naNewStringWithFormat("This is %s running." NA_NL, "Hello World");
    printf("%s", naGetStringUTF8Pointer(testString));
    naDelete(testString);
  naStopRuntime();
  


  return 0;
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
