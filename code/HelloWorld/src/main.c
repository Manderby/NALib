
// This is just a small file to get you started using NALib.
// It should compile and run and print some version notes on the screen.

// Prototypes
int jsonTest(void);


// Note that you could add an include path in your project to access the
// NAXXX.h files. But for the sake of simplicity, we use relative paths here.
#include "../../NALib/src/NABase.h"
#include "../../NALib/src/NAUtility/NAString.h"
#include "../../NALib/src/NAUtility/NADateTime.h"
#include "../../NALib/src/NAStruct/NAList.h"
#include "../../NALib/src/NAStruct/NAStack.h"
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



  printf("Hello World" NA_NL);



  naStartRuntime();

    naSetGlobalTimeShiftToSystemSettings();
    NADateTime dateTime = naMakeDateTimeNow();
    NAString* timeString = naNewStringWithDateTime(
      &dateTime,
      NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
    printf("Current date and time: %s" NA_NL, naGetStringUTF8Pointer(timeString));
    naDelete(timeString);



    int a = 12;
    int b = 34;
    int c = 56;

    NAList list;
    naInitList(&list);
    naAddListLastConst(&list, &a);
    naAddListLastConst(&list, &b);
    naAddListLastConst(&list, &c);
    printf("Listing elements: ");
    NAListIterator listIter = naMakeListAccessor(&list);
    while(naIterateList(&listIter)){
      printf("%d ", *(const int*)naGetListCurConst(&listIter));
    }
    naClearListIterator(&listIter);
    printf(NA_NL);
    naClearList(&list);



    NAStack stack;
    naInitStack(&stack, sizeof(int), 0, 0);
    *(int*)naPushStack(&stack) = 3;
    *(int*)naPushStack(&stack) = 2;
    *(int*)naPushStack(&stack) = 1;
    printf("Stacking elements: ");
    printf("%d ", *(int*)naPopStack(&stack));
    printf("%d ", *(int*)naPopStack(&stack));
    printf("%d ", *(int*)naPopStack(&stack));
    printf(NA_NL);
    naClearStack(&stack);

  naStopRuntime();
  


  jsonTest();


  printf("Finished." NA_NL);
  #if NA_OS == NA_OS_WINDOWS
    NA_UNUSED(getchar());
  #endif
  
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
