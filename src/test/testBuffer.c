

#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NABuffer.h"
#include "../NALib/NADateTime.h"

#define NA_BUFFER_TEST_REPETITIONS 100000
#define NA_BUFFER_TESTARRAY_SIZE   10000
NAByte testdataarray[NA_BUFFER_TESTARRAY_SIZE];


void testBufferFile(void){
  NABuffer* buffer;
  NABuffer* token;

  printf("\nCreating file buffer with ASCII file.\n");
  buffer = naCreateBufferFile("asciitest.txt");  
  
  for(int i=0; i<2000; i++){
    naReadBufferBytes(buffer, &testdataarray, 100);
    testdataarray[100] = '\0';
    printf("%s", testdataarray);
  }
  
  token = naCreateBufferExtraction(buffer, naMakeRangei(2, 10));
  naReadBufferBytes(token, &testdataarray, 10);
  testdataarray[10] = '\0';
  printf("\n%s\n", testdataarray);
}



void testBufferMemory(void){
  NABuffer* buffer;
  int testdata = 42;
  NADateTime time1;
  NADateTime time2;
  NARangei range;
  
  printf("\n");
  printf("Buffer Test\n");
  printf("===========\n");
  
  if(NA_BUFFER_PART_BYTESIZE){
    printf("Configuration.h: Custom buffer part size: %d\n", NA_BUFFER_PART_BYTESIZE);
  }else{
    printf("Configuration.h: buffer part size same as pagesize: %d\n", naGetSystemMemoryPagesize());
  }

  printf("\nCreating memory buffer.\n");
  buffer = naCreateBuffer();  

  // We can seek to an absolute poisition within the buffer
  naSeekBufferAbsolute(buffer, (NA_BUFFER_TEST_REPETITIONS * NA_BUFFER_TESTARRAY_SIZE) / 4);
  
  printf("Writing lots of bytes to the buffer %d times... ", NA_BUFFER_TEST_REPETITIONS);
  time1 = naMakeDateTimeNow();
  for(int i=0; i<NA_BUFFER_TEST_REPETITIONS; i++){
    // Writing single variables
    naWriteBufferBytes(buffer, &testdata, sizeof(int));
    // Writing whole arrays
    naWriteBufferBytes(buffer, testdataarray, NA_BUFFER_TESTARRAY_SIZE);
    // We can seek relative to the current position, even towards negative
    // absolute addresses.
    naSeekBufferRelative(buffer, -(3 * NA_BUFFER_TESTARRAY_SIZE / 2));
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  
  range = naGetBufferRange(buffer);
  printf("Buffer now stores a byte range of [%" NA_PRIi ", %" NA_PRIi "] (%" NA_PRIi " MB)\n", range.origin, naGetRangeiEnd(range), range.length / 1000000);
  

  printf("Writing some data ... ", testdata);
  naWriteBufferBytes(buffer, &testdata, sizeof(int));
  naSeekBufferRelative(buffer, -sizeof(int));
  naReadBufferBytes(buffer, &testdata, sizeof(int));
  printf("Reading that data again: %d\n", testdata);
  
  naReleaseBuffer(buffer);
}



void testBuffer(void){

//  testBufferMemory();
  testBufferFile();
  
}

