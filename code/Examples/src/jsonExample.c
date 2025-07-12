#include "../../NALib/src/NALib.h"

// This is already included by including NALib.h but here you can find all
// relevant APIs:
#include "../../NALib/src/NAUtility/NAJSON.h"

typedef struct TinyObject TinyObject;
struct TinyObject {
  NAUTF8Char* message;
};

typedef struct SimpleValues SimpleValues;
struct SimpleValues {
  int64 member1;
  float member2;
  double member3;
  int32 member4;
  NAUTF8Char* member5;
  TinyObject* object6;
  TinyObject object7;
  void* object8;
};

typedef struct ArrayValues ArrayValues;
struct ArrayValues {
  int32 values1[5];
  size_t count2;
  int32* values2;
  NAUTF8Char* values3[5];
  size_t count4;
  NAUTF8Char** values4;

  TinyObject objects5[5];
  TinyObject* objects6[5];
  size_t count7;
  TinyObject* objects7;
  size_t count8;
  TinyObject** objects8;
};

typedef struct Test Test;
struct Test {
  SimpleValues simpleValues;
  ArrayValues arrayValues;
};



NAJSONWorker* allocateSimpleWorker(void){
  NAJSONWorker* worker = naAllocateJSONWorker();

  NAJSONRuleSet* simpleRules = naRegisterJSONRuleSet(worker);
  
  // Reading simple values
  
  naAddJSONRule(simpleRules, "value1", naNewJSONRuleInt64(offsetof(Test, simpleValues.member1)));
  naAddJSONRule(simpleRules, "value2", naNewJSONRuleFloat(offsetof(Test, simpleValues.member2)));
  naAddJSONRule(simpleRules, "value3", naNewJSONRuleDouble(offsetof(Test, simpleValues.member3)));
  naAddJSONRule(simpleRules, "value4", naNewJSONRuleBool(offsetof(Test, simpleValues.member4)));
  naAddJSONRule(simpleRules, "value5", naNewJSONRuleString(offsetof(Test, simpleValues.member5)));
  
  // Reading objects
  
  // Reading an object by creating it with malloc and storing a pointer.
  NAJSONRuleSet* tinyRules1 = naRegisterJSONRuleSet(worker);
  naAddJSONRule(tinyRules1, "message", naNewJSONRuleString(offsetof(TinyObject, message)));
  naAddJSONRule(simpleRules, "object6", naNewJSONRulePointerObject(
    offsetof(Test, simpleValues.object6),
    sizeof(TinyObject),
    tinyRules1));

  // Reading an object directly into an existing object.
  NAJSONRuleSet* tinyRules2 = naRegisterJSONRuleSet(worker);
  naAddJSONRule(tinyRules2, "message", naNewJSONRuleString(offsetof(TinyObject, message)));
  naAddJSONRule(simpleRules, "object7", naNewJSONRuleObject(tinyRules2, offsetof(Test, simpleValues.object7)));

  // Reading an object which turns out to be null.
  naAddJSONRule(simpleRules, "object8", naNewJSONRulePointerObject(
    offsetof(Test, simpleValues.object8),
    0,
    NA_NULL));
    
  // reading arrays of simple values

  NAJSONRuleSet* arrayRules = naRegisterJSONRuleSet(worker);
  naAddJSONRule(arrayRules, "values1", naNewJSONRuleFixedArrayInt32(
    offsetof(Test, arrayValues.values1),
    /*elementCount:*/ 5));
  naAddJSONRule(arrayRules, "values2", naNewJSONRuleDynamicArrayInt32(
    offsetof(Test, arrayValues.values2),
    offsetof(Test, arrayValues.count2)));
  naAddJSONRule(arrayRules, "values3", naNewJSONRuleFixedArrayString(
    offsetof(Test, arrayValues.values3),
    /*elementCount:*/ 5));
  naAddJSONRule(arrayRules, "values4", naNewJSONRuleDynamicArrayString(
    offsetof(Test, arrayValues.values4),
    offsetof(Test, arrayValues.count4)));
    
  // reading arrays of objects

  // Reading objects into a fixed array
  naAddJSONRule(arrayRules, "values5", naNewJSONRuleFixedArray(
    offsetof(Test, arrayValues.objects5),
    /*elementCount:*/ 5,
    sizeof(TinyObject),
    naNewJSONRuleObject(tinyRules1, 0)));

  // Reading objects into a fixed array
  naAddJSONRule(arrayRules, "values6", naNewJSONRuleFixedPointerArray(
    offsetof(Test, arrayValues.objects6),
    /*elementCount:*/ 5,
    sizeof(TinyObject),
    naNewJSONRuleObject(tinyRules1, 0)));

  // Reading objects into a dynamic array
  naAddJSONRule(arrayRules, "values7", naNewJSONRuleDynamicArray(
    offsetof(Test, arrayValues.objects7),
    offsetof(Test, arrayValues.count7),
    sizeof(TinyObject),
    naNewJSONRuleObject(tinyRules1, 0)));

  // Reading objects as poitners into a dynamic array
  naAddJSONRule(arrayRules, "values8", naNewJSONRuleDynamicPointerArray(
    offsetof(Test, arrayValues.objects8),
    offsetof(Test, arrayValues.count8),
    sizeof(TinyObject),
    naNewJSONRuleObject(tinyRules1, 0)));

  NAJSONRuleSet* baseRules = naRegisterJSONRuleSet(worker);
  naAddJSONRule(baseRules, "SimpleObject", naNewJSONRuleObject(simpleRules, 0));
  naAddJSONRule(baseRules, "ArraysObject", naNewJSONRuleObject(arrayRules, 0));

  // The whole file contains one (unnamed) object. As this is the last ruleSet
  // being registered, it will automatically serve as the initial ruleSet when
  // parsing.
  NAJSONRuleSet* fileRules = naRegisterJSONRuleSet(worker);
  naAddJSONRule(fileRules, "", naNewJSONRuleObject(baseRules, 0));

  return worker;
}


void cleanTest(Test* test) {
  naFree(test->simpleValues.member5);
  naFree(test->simpleValues.object8);
  naFree(test->arrayValues.values2);
  for(size_t i = 0; i < test->arrayValues.count4; ++i)
    naFree(test->arrayValues.values4[i]);
  naFree(test->arrayValues.values4);
  naFree(test->arrayValues.objects7);
  for(size_t i = 0; i < test->arrayValues.count8; ++i)
    naFree(test->arrayValues.objects8[i]);
  naFree(test->arrayValues.objects8);
}


int jsonExample(void){

  naStartRuntime();
  
  printf("Reading JSON file...\n");

  // Loading the buffer from a file.
  NADateTime now1 = naMakeDateTimeNow();
  NAFile* file = naCreateFileReadingPath("JSONinput.txt");
  if(!naIsFileOpen(file)){
    printf("File not found. You must adjust the path on your machine for this example to work.\n");
    return 0;
  }
  fsize_t bufferSize = naComputeFileByteSize(file);
  NAByte* buf = naMalloc(bufferSize + 1);
  naReadFileBytes(file, buf, bufferSize);
  // Buffer needs to be closed with a \0 byte.
  buf[bufferSize] = '\0';
  naRelease(file);

  NADateTime now2 = naMakeDateTimeNow();

  printf("Time: %f milliseconds to read file\n", 1000. * naGetDateTimeDifference(&now2, &now1));

  // Creating a worker which reads into the Test structure.
  Test test;
  NAJSONWorker* simpleWorker = allocateSimpleWorker();

  // Running a benchmark
  #define TESTCOUNT 1000
  NADateTime now3 = naMakeDateTimeNow();
  for(int i = 0; i < TESTCOUNT; ++i){
  
    // Parse the buffer into the test variable.
    naParseJSONBuffer(simpleWorker, &test, buf, bufferSize + 1);
    
    // Deleting the objects from the memory again.
    cleanTest(&test);
  }
  NADateTime now4 = naMakeDateTimeNow();

  printf("Time: %f milliseconds to parse file\n", 1000. * naGetDateTimeDifference(&now4, &now3) / (double)TESTCOUNT);

  printf("Writing JSON file...\n");

  // Prepare the data for writing.
  naParseJSONBuffer(simpleWorker, &test, buf, bufferSize + 1);
  
  NABuffer* jsonBuffer = NA_NULL;
//  for(int i = 0; i < TESTCOUNT; ++i){
//    if(jsonBuffer) { naRelease(jsonBuffer); }
    jsonBuffer = naCreateBufferWithJSON(simpleWorker, &test);
//  }
  NADateTime now5 = naMakeDateTimeNow();
  printf("Time: %f milliseconds to create file contents\n", 1000. * naGetDateTimeDifference(&now5, &now4) / (double)TESTCOUNT);
  
  naFixBufferRange(jsonBuffer);
  NAFile* outFile = naCreateFileWritingPath("JSONOutput.txt", NA_FILEMODE_DEFAULT);
  naWriteBufferToFile(jsonBuffer, outFile);

  NADateTime now6 = naMakeDateTimeNow();
  printf("Time: %f milliseconds to write file\n", 1000. * naGetDateTimeDifference(&now6, &now5));

  cleanTest(&test);

  naRelease(outFile);
  naRelease(jsonBuffer);

  // Deallocating the worker also deallocates all rules and ruleSets.
  naDeallocateJSONWorker(simpleWorker);

  naFree(buf);

  naStopRuntime();

  return 0;
}
