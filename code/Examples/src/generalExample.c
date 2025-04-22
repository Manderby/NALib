#include "../../NALib/src/NALib.h"

int generalExample(void){

  naStartRuntime();
  
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
  printf(" %d Bits Addresses)\n\n", NA_ADDRESS_BITS);


  NAString* wd = naNewStringWithCurWorkingDirectory();
  printf("Working Directory: %s\n\n", naGetStringUTF8Pointer(wd));
  naDelete(wd);
  
  naSetGlobalTimeShiftToSystemSettings();
  NADateTime dateTime = naMakeDateTimeNow();
  NAString* timeString = naNewStringWithDateTime(
    &dateTime,
    NA_DATETIME_FORMAT_UTC_EXTENDED_WITH_SHIFT);
  printf("Current date and time: %s\n\n", naGetStringUTF8Pointer(timeString));
  naDelete(timeString);
    
  naStopRuntime();

  return 0;
}
