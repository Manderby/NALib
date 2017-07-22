

#include "tests.h"

#include "../NALib/NAMemory.h"
#include "../NALib/NADateTime.h"


#define NA_TEST_MEMORY_M_COUNT 1
#define NA_TEST_MEMORY_COUNT (NA_TEST_MEMORY_M_COUNT * 1000000)



typedef struct AVeryUsefulStruct AVeryUsefulStruct;
struct AVeryUsefulStruct{
  int     a;
  float   b;
  double  c[10];
  NABool  printdestructmessage;
};

void destructAVeryUsefulStruct(AVeryUsefulStruct* avus){
  // Do things to destruct the struct.
  if(avus->printdestructmessage){
    printf("Destructor of AVeryUsefulStruct called.\n");
  }
}

NA_RUNTIME_TYPE(AVeryUsefulStruct, destructAVeryUsefulStruct);



void testMallocFree(void){
  void* ptr1;
  void* ptr2;
  void* ptr3;
  void* ptr4;

  printf("Allocating 0x1234 Bytes... ");
  ptr1 = naMalloc(0x1234);
  printf("at address 0x%" NA_PRIx "\n", (NAUInt)ptr1);

  printf("Allocating Bytes for a type... ");
  ptr2 = naAlloc(AVeryUsefulStruct);
  ((AVeryUsefulStruct*)ptr2)->printdestructmessage = NA_TRUE;
  printf("at address 0x%" NA_PRIx "\n", (NAUInt)ptr2);

  printf("Allocating 0x1234 Bytes aligned at 0x100000... ");
  ptr3 = naMallocAligned(0x1234, 0x100000);
  printf("at address 0x%" NA_PRIx "\n", (NAUInt)ptr3);

  printf("Allocating 0x1234 Bytes aligned at page... ");
  ptr4 = naMallocPageAligned(0x1234);
  printf("at address 0x%" NA_PRIx "\n", (NAUInt)ptr4);

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
  ((AVeryUsefulStruct*)ptr1)->printdestructmessage = NA_TRUE;
  printf("at address 0x%" NA_PRIx "\n", (NAUInt)ptr1);

  printf("Deleting that pointer.\n");
  naDelete(ptr1);

  printf("\nPerformance analysis:\n");
  structarray = (AVeryUsefulStruct**)naMalloc(sizeof(AVeryUsefulStruct*) * NA_TEST_MEMORY_COUNT);
  #if (NA_RUNTIME_USES_MEMORY_POOLS == 1)
    printf("Runtime uses memory pools (see NAConfiguration.h).\n");
    printf("Pool size: %" NA_PRIu "\n", naGetRuntimePoolSize());
  #else
    printf("Runtime uses malloc and free (see NAConfiguration.h).\n");
  #endif

  printf("Allocating %d M structs... ", NA_TEST_MEMORY_M_COUNT);
  time1 = naMakeDateTimeNow();
  structptr = structarray;
  for(i = 0; i < NA_TEST_MEMORY_COUNT; i++){
    *structptr = naNew(AVeryUsefulStruct);
    // Turning off the destruct message, otherwise it will be printed a million
    // times.
    (*((AVeryUsefulStruct**)structptr))->printdestructmessage = NA_FALSE;
    structptr++;
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
  
  printf("\nNAPtr Constants (not visible in API):\n");
  printf("Cleanup bits: %d\n", NA_MEMORY_CLEANUP_BITS);
  printf("Cleanup mask: 0x%x\n", NA_MEMORY_CLEANUP_MASK);
  printf("Data cleanup bits: %d - %d\n", NA_REFCOUNT_DATA_CLEANUP_BITSHIFT, NA_REFCOUNT_DATA_CLEANUP_BITSHIFT + NA_MEMORY_CLEANUP_BITS - 1);
//  printf("Struct cleanup bits: %d - %d\n", NA_REFCOUNT_STRUCT_CLEANUP_BITSHIFT, NA_REFCOUNT_STRUCT_CLEANUP_BITSHIFT + NA_MEMORY_CLEANUP_BITS - 1);

  printf("\nCreating Null-NAPtr.\n");
  ptrNull = naMakeNullPtr();
  
  printf("Creating NAPtr with bytesize.\n");
  ptr = naMakePtrWithBytesize(100);
  
  printf("Creating NAPtr with const buffer.\n");
  ptrconst = naMakePtrWithDataConst(string);
  
  printf("Creating NAPtr with mutable buffer.\n");
  array = (int*)naMalloc(sizeof(int) * 5);
  ptrmutable = naMakePtrWithDataMutable(array/*, NA_MEMORY_CLEANUP_NA_FREE*/);

  printf("Creating NAPtr with extraction.\n");
  ptrExtract = naMakePtrWithExtraction(&ptrmutable, 2 * sizeof(int), 3 * sizeof(int));
  
  printf("Accessing and mutating NAPtr. Writing 1234, reading: ");
  testptrmutable = (int*)naGetPtrMutable(&ptrExtract);
  *testptrmutable = 1234;
  testptrconst = (const int*)naGetPtrConst(&ptrExtract);
  printf("%d\n", *testptrconst);
  
  printf("Testing NAPtr: ");
  // Note that naIsPtrConst returns always NA_TRUE when compiling with NDEBUG.
  printf("ptrmutable is %s, ", (naIsPtrConst(&ptrmutable) ? "Const" : "Mutable"));
  printf("ptrconst is %s", (naIsPtrConst(&ptrconst) ? "Const" : "Mutable"));
  #ifdef NDEBUG
    printf(" (Compiled as Release!)");
  #endif
  printf("\n");
  
  printf("Clearing the NAPtr's.\n");
  naCleanupPtr(&ptrNull,    NA_NULL);
  naCleanupPtr(&ptr,        (NAMutator)naFree);
  naCleanupPtr(&ptrconst,   NA_NULL);
  naCleanupPtr(&ptrmutable, (NAMutator)naFree);
  naCleanupPtr(&ptrExtract, NA_NULL);
}


void testNASmartPtr(void){
  int mydata = 42;
  int* mydataptr;
  const int* mydataptrconst;
  const char* mydatacharptrconst;
  AVeryUsefulStruct* avusptr;
  NASmartPtr sptr1;
  NASmartPtr sptr2;
  NASmartPtr* sptr3;
  NASmartPtr* sptr4;
  

  printf("\nNASmartPtr Constants (not visible in API):\n");
//  printf("Flag bits count: %d\n", NA_REFCOUNT_FLAG_BITS);
//  printf("Flag bits: %d - %d\n", NA_REFCOUNT_FLAGS_BITSHIFT, NA_REFCOUNT_FLAGS_BITSHIFT + NA_REFCOUNT_FLAG_BITS - 1);
//  printf("Refcount mask: 0x%" NA_PRIx "\n", NA_REFCOUNT_MASK);
  printf("\nCreating NASmartPtr.\n");
  // Initializing some pointers
  naInitSmartPtrMutable(&sptr1, /*NA_MEMORY_CLEANUP_NONE, */&mydata/*, NA_MEMORY_CLEANUP_NONE*/);
  naInitSmartPtrMutable(&sptr2, /*NA_MEMORY_CLEANUP_NONE, */naAlloc(AVeryUsefulStruct)/*, NA_MEMORY_CLEANUP_NA_FREE*/);
  sptr3 = naAlloc(NASmartPtr);
  naInitSmartPtrMutable(sptr3, /*NA_MEMORY_CLEANUP_NA_FREE, */naAlloc(int)/*, NA_MEMORY_CLEANUP_NA_FREE*/);
  sptr4 = naAlloc(NASmartPtr);
  naInitSmartPtrConst(sptr4, /*NA_MEMORY_CLEANUP_NA_FREE, */"Constant String Literal");
  
  printf("Retaining the smart pointers...\n");
  naRetainSmartPtr(&sptr1);
  naRetainSmartPtr(&sptr1);
  naRetainSmartPtr(&sptr2);
  naRetainSmartPtr(&sptr2);
  naRetainSmartPtr(sptr3);
  naRetainSmartPtr(sptr3);
  naRetainSmartPtr(sptr4);
  naRetainSmartPtr(sptr4);
  
  printf("Accessing and mutating the smart pointers...\n");
  mydataptrconst = (const int*)naGetSmartPtrConst(&sptr1);
  printf("Data in sptr1: %d\n", *mydataptrconst);
  printf("Turning destruct message of sptr2 on.\n");
  avusptr = (AVeryUsefulStruct*)naGetSmartPtrMutable(&sptr2);
  avusptr->printdestructmessage = NA_TRUE;
  printf("Set data in sptr3 to 12345");
  mydataptr = (int*)naGetSmartPtrMutable(sptr3);
  *mydataptr = 12345;
  mydataptrconst = (const int*)naGetSmartPtrConst(sptr3);
  printf (", reading it again: %d\n", *mydataptrconst);
  mydatacharptrconst = (const char*)naGetSmartPtrConst(sptr4);
  printf("Data in sptr4: %s\n", mydatacharptrconst);
  
  
  printf("Releasing the smart pointers...\n");
  naReleaseSmartPtr(&sptr1, NA_NULL);
  naReleaseSmartPtr(&sptr1, NA_NULL);
  naReleaseSmartPtr(&sptr1, NA_NULL);
  naReleaseSmartPtr(&sptr2, (NAMutator)destructAVeryUsefulStruct);
  naReleaseSmartPtr(&sptr2, (NAMutator)destructAVeryUsefulStruct);
  naReleaseSmartPtr(&sptr2, (NAMutator)destructAVeryUsefulStruct);
  naReleaseSmartPtr(sptr3, NA_NULL);
  naReleaseSmartPtr(sptr3, NA_NULL);
  naReleaseSmartPtr(sptr3, NA_NULL);
  naReleaseSmartPtr(sptr4, NA_NULL);
  naReleaseSmartPtr(sptr4, NA_NULL);
  naReleaseSmartPtr(sptr4, NA_NULL);

  printf("\n");
  
}



void testNAPointer(void){
  int mydata = 42;
  const int* mydataptrconst;
  const char* mychardataptrconst;
  AVeryUsefulStruct* avusptr;
  NAPointer* ptr1;
  NAPointer* ptr2;
  NAPointer* ptr3;
  
  naStartRuntime();
  
  printf("\nCreating NAPointer.\n");
  ptr1 = naNewPointerMutable(&mydata, /*NA_MEMORY_CLEANUP_NONE, */NA_NULL);
  ptr2 = naNewPointerMutable(naAlloc(AVeryUsefulStruct), /*NA_MEMORY_CLEANUP_NA_FREE, */(NAMutator)destructAVeryUsefulStruct);
  ptr3 = naNewPointerConst("Constant string literal");

  printf("Retaining NAPointer.\n");
  naRetain(ptr1);
  naRetain(ptr1);
  naRetain(ptr2);
  naRetain(ptr2);
  naRetain(ptr3);
  naRetain(ptr3);

  printf("Accessing and mutating the pointers...\n");
  mydataptrconst = (const int*)naGetPointerConst(ptr1);
  printf("Data in ptr1: %d\n", *mydataptrconst);
  printf("Turning destruct message of ptr2 on.\n");
  avusptr = (AVeryUsefulStruct*)naGetPointerMutable(ptr2);
  avusptr->printdestructmessage = NA_TRUE;
  mychardataptrconst = (const char*)naGetPointerConst(ptr3);
  printf("Data in ptr3: %s\n", mychardataptrconst);

  printf("Releasing the pointers...\n");
  naRelease(ptr1);
  naRelease(ptr1);
  naRelease(ptr1);
  naRelease(ptr2);
  naRelease(ptr2);
  naRelease(ptr2);
  naRelease(ptr3);
  naRelease(ptr3);
  naRelease(ptr3);
  
  printf("\n");
  naStopRuntime();
}



void testMemory(void){
  
  printf("\n");
  printf("Memory Test\n");
  printf("==========\n");

  printf("Memory page size: %" NA_PRIu "\n", naGetSystemMemoryPagesize());
  printf("Memory page mask: 0x%" NA_PRIx "\n", naGetSystemMemoryPagesizeMask());
  
  testMallocFree();
  testRuntime();
  testNAPtr();
  testNASmartPtr();
  testNAPointer();

}


