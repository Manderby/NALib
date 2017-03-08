

#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NABuffer.h"
#include "../NALib/NADateTime.h"

#define NA_BUFFER_TEST_REPETITIONS 100000
#define NA_BUFFER_TESTARRAY_SIZE   10000
NAByte testdataarray[NA_BUFFER_TESTARRAY_SIZE];


void testBufferFile(void){
  NAASDFBuffer* buffer;
  NAASDFBuffer* token;

  printf("\nCreating file buffer with ASCII file.\n");
  buffer = naCreateASDFBufferFile("asciitest.txt");  
  
  for(int i=0; i<2000; i++){
    naReadASDFBufferBytes(buffer, &testdataarray, 100);
    testdataarray[100] = '\0';
    printf("%s", testdataarray);
  }
  
  token = naCreateASDFBufferExtraction(buffer, naMakeRangei(2, 10));
  naReadASDFBufferBytes(token, &testdataarray, 10);
  testdataarray[10] = '\0';
  printf("\n%s\n", testdataarray);
}



void testBufferMemory(void){
  NAASDFBuffer* buffer;
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
  buffer = naCreateASDFBuffer();  

  // We can seek to an absolute poisition within the buffer
  naSeekASDFBufferAbsolute(buffer, (NA_BUFFER_TEST_REPETITIONS * NA_BUFFER_TESTARRAY_SIZE) / 4);
  
  printf("Writing lots of bytes to the buffer %d times... ", NA_BUFFER_TEST_REPETITIONS);
  time1 = naMakeDateTimeNow();
  for(int i=0; i<NA_BUFFER_TEST_REPETITIONS; i++){
    // Writing single variables
    naWriteASDFBufferBytes(buffer, &testdata, sizeof(int));
    // Writing whole arrays
    naWriteASDFBufferBytes(buffer, testdataarray, NA_BUFFER_TESTARRAY_SIZE);
    // We can seek relative to the current position, even towards negative
    // absolute addresses.
    naSeekASDFBufferRelative(buffer, -(3 * NA_BUFFER_TESTARRAY_SIZE / 2));
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  
  range = naGetASDFBufferRange(buffer);
  printf("Buffer now stores a byte range of [%" NA_PRIi ", %" NA_PRIi "] (%" NA_PRIi " MB)\n", range.origin, naGetRangeiEnd(range), range.length / 1000000);
  

  printf("Writing some data ... ", testdata);
  naWriteASDFBufferBytes(buffer, &testdata, sizeof(int));
  naSeekASDFBufferRelative(buffer, -sizeof(int));
  naReadASDFBufferBytes(buffer, &testdata, sizeof(int));
  printf("Reading that data again: %d\n", testdata);
  
  naReleaseASDFBuffer(buffer);
}



void testBuffer(void){

//  testBufferMemory();
  testBufferFile();
  
}

