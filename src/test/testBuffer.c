

#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NABuffer.h"
#include "../NALib/NADateTime.h"

#define NA_BUFFER_TEST_REPETITIONS 10000
#define NA_BUFFER_TESTARRAY_SIZE   10000
NAByte testdataarray[NA_BUFFER_TESTARRAY_SIZE];


void testBufferFile(void){
  NABuffer* buffer;
  NABuffer* token;
  NAString string;
  NAInt linesread;
  NABufferIterator iter;

  NAString cwd = naMakeStringWithCurrentWorkingDirectory();

  printf("\nCreating file input buffer with ASCII file.\n");
  printf("Working Directory: %s\n", naGetStringUTF8Pointer(&cwd));
  
  naClearString(&cwd);
  
  buffer = naCreateBufferWithInpuFile("res/asciitest.txt");  
  iter = naMakeBufferAccessor(buffer);
  
  // Creating a buffer extraction and then reading that whole buffer
  token = naCreateBufferExtraction(buffer, naMakeRangei(6, 8));
  naReadBufferBytes(&iter, &testdataarray, 8);
  testdataarray[8] = '\0';
  printf("Using buffer extraction [bytes 6-14]: %s\n", testdataarray);
  naReleaseBuffer(token);

  // We read some bytes from the file.
  naReadBufferBytes(&iter, &testdataarray, 14);
  printf("Reading 14 characters: %s\n", testdataarray);
  
  printf("Skipping buffer whitespaces.\n");
  naSkipBufferWhitespaces(&iter);
  
  string = naParseBufferLine(&iter, NA_TRUE, &linesread);
  printf("Reading the next filled line (%" NA_PRIi " lines skipped) : %s\n", linesread-1, naGetStringUTF8Pointer(&string));
  naClearString(&string);
  string = naParseBufferLine(&iter, NA_TRUE, &linesread);
  printf("Reading the next filled line (%" NA_PRIi " lines skipped) : %s\n", linesread-1, naGetStringUTF8Pointer(&string));
  naClearString(&string);
  string = naParseBufferLine(&iter, NA_TRUE, &linesread);
  printf("Reading the next filled line (%" NA_PRIi " lines skipped) : %s\n", linesread-1, naGetStringUTF8Pointer(&string));
  naClearString(&string);
  string = naParseBufferLine(&iter, NA_TRUE, &linesread);
  printf("Reading the next filled line (%" NA_PRIi " lines skipped) : %s\n", linesread-1, naGetStringUTF8Pointer(&string));
  naClearString(&string);
  string = naParseBufferLine(&iter, NA_TRUE, &linesread);
  printf("Reading the next filled line (%" NA_PRIi " lines skipped) : %s\n", linesread-1, naGetStringUTF8Pointer(&string));
  naClearString(&string);
  string = naParseBufferLine(&iter, NA_FALSE, &linesread);
  printf("Reading the next line without skipping (%" NA_PRIi " lines read) : %s\n", linesread, naGetStringUTF8Pointer(&string));
  naClearString(&string);
  
  // todo: more to come
  
  naClearBufferIterator(&iter);
  naReleaseBuffer(buffer);
}



void testBufferMemory(void){
  NABuffer* buffer;
  int testdata = 42;
  NADateTime time1;
  NADateTime time2;
  NARangei range;
  int i;
  NABufferIterator iter;
  
  printf("\n");
  printf("Buffer Test\n");
  printf("===========\n");
  
  if(NA_BUFFER_PART_BYTESIZE){
    printf("Configuration.h: Custom buffer part size: %d\n", NA_BUFFER_PART_BYTESIZE);
  }else{
    printf("Configuration.h: buffer part size same as pagesize: %" NA_PRIu "\n", naGetSystemMemoryPagesize());
  }

  printf("\nCreating memory buffer.\n");
  buffer = naCreateBuffer(NA_FALSE);  
  iter = naMakeBufferModifier(buffer);

  // We can seek to an absolute poisition within the buffer
  naSeekBufferAbsolute(&iter, (NA_BUFFER_TEST_REPETITIONS * NA_BUFFER_TESTARRAY_SIZE) / 4);
  
  printf("Writing lots of bytes to the buffer %d times... ", NA_BUFFER_TEST_REPETITIONS);
  time1 = naMakeDateTimeNow();
  for(i=0; i<NA_BUFFER_TEST_REPETITIONS; i++){
    // Writing single variables
    naWriteBufferBytes(&iter, &testdata, sizeof(int));
    // Writing whole arrays
    naWriteBufferBytes(&iter, testdataarray, NA_BUFFER_TESTARRAY_SIZE);
    // We can seek relative to the current position, even towards negative
    // absolute addresses.
    naSeekBufferRelative(&iter, -(3 * NA_BUFFER_TESTARRAY_SIZE / 2));
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  
  range = naGetBufferRange(buffer);
  printf("Buffer now stores a byte range of [%" NA_PRIi ", %" NA_PRIi "] (%" NA_PRIi " MB)\n", range.origin, naGetRangeiEnd(range), range.length / 1000000);
  
  naDismissBufferRange(buffer, naMakeRangeiWithStartAndEnd(1000, 2000));
  

  printf("Writing some data ... ");
  naWriteBufferBytes(&iter, &testdata, sizeof(int));
  naSeekBufferRelative(&iter, -naSizeof(int));
  naReadBufferBytes(&iter, &testdata, sizeof(int));
  printf("Reading that data again: %d\n", testdata);
  
  naClearBufferIterator(&iter);

  printf("Releasing the buffer ... ");
  time1 = naMakeDateTimeNow();
  naReleaseBuffer(buffer);
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  
}



void testBuffer(void){

  testBufferMemory();
  testBufferFile();
  
}

