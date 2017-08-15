

//
// This is just a small file to get you started using NALib.
//
// This file should compile and run and print some version notes on the screen.
//


#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NAMemory.h"
#include "../NALib/NADateTime.h"




int main(void){

  char* testdata = "Diese Website möchte kompakte Informationen für die Programmierung mit C und C++ liefern. Aufgrund der zeitintensiven Arbeit für eine solche Referenz kann nicht auf sämtliche Eigenheiten der Sprachen eingegangen, und Vollständigkeit sowie Korrektheit nicht gewährleistet werden. Der Autor freut sich über Hinweise per Email.";

  naStartRuntime();
  NAString* instring = naNewStringWithFormat(testdata);
  NAString* base64 = naNewStringWithBufferBase64Encoded(naGetStringBufferConst(instring), NA_TRUE);
  printf("%.*s\n", naGetStringBytesize(base64), naGetStringUTF8Pointer(base64));
  NABuffer* outbuf = naCreateBufferWithStringBase64Decoded(base64);
  NAString* outstring = naNewStringWithBufferExtraction(outbuf, naGetBufferRange(outbuf));

  printf("%.*s", naGetStringBytesize(outstring), naGetStringUTF8Pointer(outstring));
  naReleaseBuffer(outbuf);
  naDelete(base64);
  naDelete(instring);
  naDelete(outstring);
  naStopRuntime();
  
//  NADateTime time1;
//  NADateTime time2;
//  
//  time1 = naMakeDateTimeNow();
//
//  testSystem();  
//  testMemory();
//
//  naStartRuntime();
//    testStack();
//    testBuffer();
//  naStopRuntime();
//  
//  printf("\nTest completed. ");
//  
//  time2 = naMakeDateTimeNow();
//  printf("Total time: %f seconds\n", naGetDateTimeDifference(&time2, &time1));

  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    printf("\nPress enter to quit.\n");
    fgetc(stdin);
  #endif
  printf("End.\n");
  return 0;
}

