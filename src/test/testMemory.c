

#include "stdio.h"
#include "NAMemory.h"
#include "NADateTime.h"


#define NA_TEST_MEMORY_M_COUNT 1
#define NA_TEST_MEMORY_COUNT (NA_TEST_MEMORY_M_COUNT * 1000000)



typedef struct AVeryUsefulStruct AVeryUsefulStruct;
struct AVeryUsefulStruct{
  int     a;
  float   b;
  double  c[10];
};

void destructAVeryUsefulStruct(AVeryUsefulStruct* avus){
  // Do things to destruct the struct.
}

NA_RUNTIME_TYPE(AVeryUsefulStruct, destructAVeryUsefulStruct);



void testMallocFree(void){
  void* ptr1;
  void* ptr2;
  void* ptr3;
  void* ptr4;

  printf("Allocating 0x1234 Bytes... ");
  ptr1 = naMalloc(0x1234);
  printf("at address 0x%" NA_PRIx "\n", ptr1);

  printf("Allocating Bytes for a type... ");
  ptr2 = naAlloc(AVeryUsefulStruct);
  printf("at address 0x%" NA_PRIx "\n", ptr2);

  printf("Allocating 0x1234 Bytes aligned at 0x100000... ");
  ptr3 = naMallocAligned(0x1234, 0x100000);
  printf("at address 0x%" NA_PRIx "\n", ptr3);

  printf("Allocating 0x1234 Bytes aligned at page... ");
  ptr4 = naMallocPageAligned(0x1234);
  printf("at address 0x%" NA_PRIx "\n", ptr4);

  printf("Freeing all that memory again.\n");
  naFree(ptr1);
  naFree(ptr2);
  naFreeAligned(ptr3);
  naFreeAligned(ptr4);
}


void testRuntime(void){
  void* ptr1;
  NADateTime time1;
  NADateTime time2;
  int i;
  AVeryUsefulStruct** structarray;
  AVeryUsefulStruct** structptr;

  printf("\nStarting runtime...\n");
  naStartRuntime();
  
  printf("Allocating struct with new... ");
  ptr1 = naNew(AVeryUsefulStruct);
  printf("at address 0x%" NA_PRIx "\n", ptr1);

  printf("Deleting that pointer.\n");
  naDelete(ptr1);

  printf("\nPerformance analysis:\n");
  structarray = naMalloc(sizeof(AVeryUsefulStruct*) * NA_TEST_MEMORY_COUNT);
  #if NA_RUNTIME_USES_MEMORY_POOLS == 1
    printf("Runtime uses memory pools (see NAConfiguration.h).\n");
  #else
    printf("Runtime uses malloc and free (see NAConfiguration.h).\n");
  #endif

  printf("Allocating %d M structs... ", NA_TEST_MEMORY_M_COUNT);
  time1 = naMakeDateTimeNow();
  structptr = structarray;
  for(i = 0; i < NA_TEST_MEMORY_COUNT; i++){
    *structptr++ = naNew(AVeryUsefulStruct);
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));

  printf("Deleteing all structs again... ");
  time1 = naMakeDateTimeNow();
  structptr = structarray;
  for(i = 0; i < NA_TEST_MEMORY_COUNT; i++){
    naDelete(*structptr++);
  }
  time2 = naMakeDateTimeNow();
  printf("%f seconds\n", naGetDateTimeDifference(&time2, &time1));
  
  naFree(structarray);
 
  printf("Stopping runtime (just so that it has been done once).\n");
  naStopRuntime();
}


void testNAPtr(void){
  NAPtr ptr;
  NAPtr ptrconst;
  NAPtr ptrmutable;
  NAPtr ptrNull;
  NAPtr ptrExtract;
  char string[] = "Hello World";
  int* array;
  int* testptrmutable;
  const int* testptrconst;
  
  printf("\nCreating Null-NAPtr.\n");
  ptrNull = naMakeNullPtr();
  
  printf("Creating NAPtr with bytesize.\n");
  ptr = naMakePtrWithBytesize(100);
  
  printf("Creating NAPtr with const buffer.\n");
  ptrconst = naMakePtrWithConstData(string, strlen(string), 1);
  
  printf("Creating NAPtr with mutable buffer.\n");
  array = naMalloc(sizeof(int) * 5);
  ptrmutable = naMakePtrWithMutableData(array, 5 * sizeof(int), 0, NA_MEMORY_CLEANUP_FREE);

  printf("Creating NAPtr with extraction.\n");
  ptrExtract = naMakePtrWithExtraction(&ptrmutable, 2 * sizeof(int), 3 * sizeof(int));
  
  printf("Accessing and mutating NAPtr. Writing 1234, reading: ");
  testptrmutable = (int*)naGetPtrMutable(&ptrExtract);
  *testptrmutable = 1234;
  testptrconst = (const*)naGetPtrConst(&ptrExtract);
  printf("%d\n", *testptrconst);
  
  printf("Testing NAPtr: ");
  // Note that naIsPtrConst returns always NA_TRUE when compiling with NDEBUG.
  printf("ptrconst is %s, ", (naIsPtrConst(&ptrconst) ? "Const" : "Mutable"));
  printf("ptrmutable is %s\n", (naIsPtrConst(&ptrmutable) ? "Const" : "Mutable"));
  
  printf("Clearing the NAPtr's.\n");
  naClearPtr(&ptrNull);
  naFreePtr(&ptr);
  naClearPtr(&ptrconst);
  naFreePtr(&ptrmutable);
  naClearPtr(&ptrExtract);
}


void testMemory(void){
  
  printf("\n");
  printf("Memory Test\n");
  printf("==========\n");

  printf("Memory page size: %" NA_PRIu "\n", naGetSystemMemoryPagesize());
  printf("Memory page mask: %" NA_PRIx "\n", naGetSystemMemoryPagesizeMask());
  
  testMallocFree();
  testRuntime();
  testNAPtr();

  
}

