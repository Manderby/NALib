

#include "tests.h"

#include "../NALib/NASystem.h"
#include "../NALib/NALinearMemory.h"

NAByte testdataarray[10000];

void testBuffer(void){
  NAASDFBuffer* buffer;
  NAASDFBuffer* source1;
  NAASDFBuffer* source2;
  int testdata = 42;
  
  printf("\n");
  printf("Buffer Test\n");
  printf("===========\n");
  
  printf("Creating memory buffer.\n");
  source1 = naCreateASDFBuffer();
  source2 = naCreateASDFBuffer();
  buffer = naCreateASDFBuffer();
//  naSetASDFASDFBuffer(buffer, source1, 50);
  
//  naPutASDFBufferBytes(buffer, &testdata, sizeof(int));
//  naSetASDFASDFBuffer(buffer, source2, 50);
//  naPutASDFBufferBytes(buffer, &testdata, sizeof(int));


  printf("Writing bytes to the buffer.\n");
  for(int i=0; i<10000; i++){
    naWriteASDFBufferBytes(buffer, testdataarray, 10000);
    naWriteASDFBufferBytes(buffer, &testdata, sizeof(int));
  }
  
  naSeekASDFBufferAbsolute(buffer, 0);
  
//  naReadASDFBufferBytes(buffer, &testdata, sizeof(int));
//  printf("Data read: %d\n", testdata);
  
//  naSetASDFASDFBuffer(buffer, source2, 50);

//  for(int i=0; i<100000; i++){
//    naPutASDFBufferBytes(buffer, testdataarray, 10000);
//    //naPutASDFBufferBytes(buffer, &testdata, sizeof(int));
//  }
  
  naReleaseASDFBuffer(buffer);
  naReleaseASDFBuffer(source1);
  naReleaseASDFBuffer(source2);
}

