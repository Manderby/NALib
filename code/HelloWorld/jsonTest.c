#include "NAJSON.h"

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
  void* member5;
  NAUTF8Char* member6;
  TinyObject object1;
  TinyObject* object2;
};

typedef struct ArrayValues ArrayValues;
struct ArrayValues {
  int32 values1[5];
  size_t count2;
  int32* values2;
  NAUTF8Char* values3[5];
  size_t count4;
  NAUTF8Char** values4;
};

typedef struct Test Test;
struct Test {
  SimpleValues simpleValues;
  ArrayValues arrayValues;
  
};

NAJSONParser* allocateSimpleParser(void){
  NAJSONParser* parser = naAllocateJSONParser();

  NAJSONRuleSet* tinyRules1 = naRegisterJSONRuleSet(parser);
  naAddJSONRule(tinyRules1, "message", naNewJSONRuleString(offsetof(Test, simpleValues.object1.message)));

  NAJSONRuleSet* tinyRules2 = naRegisterJSONRuleSet(parser);
  naAddJSONRule(tinyRules2, "message", naNewJSONRuleString(offsetof(TinyObject, message)));

  NAJSONRuleSet* simpleRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(simpleRules, "value1", naNewJSONRuleInt64(offsetof(Test, simpleValues.member1)));
  naAddJSONRule(simpleRules, "value2", naNewJSONRuleFloat(offsetof(Test, simpleValues.member2)));
  naAddJSONRule(simpleRules, "value3", naNewJSONRuleDouble(offsetof(Test, simpleValues.member3)));
  naAddJSONRule(simpleRules, "value4", naNewJSONRuleInt32(offsetof(Test, simpleValues.member4)));
  naAddJSONRule(simpleRules, "value5", naNewJSONRulePointerObject(
    offsetof(Test, simpleValues.member5),
    0,
    NA_NULL));
  naAddJSONRule(simpleRules, "value6", naNewJSONRuleString(offsetof(Test, simpleValues.member6)));
  naAddJSONRule(simpleRules, "value7", naNewJSONRuleObject(tinyRules1));
  naAddJSONRule(simpleRules, "value8", naNewJSONRulePointerObject(
    offsetof(Test, simpleValues.object2),
    sizeof(TinyObject),
    tinyRules2));

  NAJSONRuleSet* arrayRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(arrayRules, "values1", naNewJSONRuleFixedArrayInt32(
    offsetof(Test, arrayValues.values1),
    5));
  naAddJSONRule(arrayRules, "values2", naNewJSONRuleArrayInt32(
    offsetof(Test, arrayValues.values2),
    offsetof(Test, arrayValues.count2)));
  naAddJSONRule(arrayRules, "values3", naNewJSONRuleFixedArrayString(
    offsetof(Test, arrayValues.values3),
    5));
  naAddJSONRule(arrayRules, "values4", naNewJSONRuleArrayString(
    offsetof(Test, arrayValues.values4),
    offsetof(Test, arrayValues.count4)));

  NAJSONRuleSet* baseRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(baseRules, "SimpleObject", naNewJSONRuleObject(simpleRules));
  naAddJSONRule(baseRules, "ArraysObject", naNewJSONRuleObject(arrayRules));

  // The whole file contains one (unnamed) object.
  NAJSONRuleSet* fileRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(fileRules, "", naNewJSONRuleObject(baseRules));

  naSetJSONParserInitialRules(parser, fileRules);
  return parser;
}

int jsonTest(void){

  naStartRuntime();
  
  NAString* wd = naNewStringWithCurWorkingDirectory();
  printf("Working Directory: %s", naGetStringUTF8Pointer(wd));
  naDelete(wd);
  
  NAFile* file = naCreateFileReadingPath("res/JSONinput.txt");
  NAFileSize fileSize = naComputeFileByteSize(file);
  NAByte* buf = naMalloc(fileSize + 2);
  naReadFileBytes(file, buf, fileSize);
  buf[fileSize] = '\0';

  NADateTime now1 = naMakeDateTimeNow();

  Test test;
  NAJSONParser* simpleParser = allocateSimpleParser();

  #define TESTCOUNT 100
  for(int i = 0; i < TESTCOUNT; ++i){
    naParseJSONBuffer(simpleParser, &test, buf, fileSize);
  }

  NADateTime now2 = naMakeDateTimeNow();
  printf("Time: %f milliseconds per buffer\n", 1000. * naGetDateTimeDifference(&now2, &now1) / (double)TESTCOUNT);
  printf("Variable \'test\' now contains all desired values.\n");

  naDeallocateJSONParser(simpleParser);

  naFree(buf);

  naStopRuntime();

  return 0;
}
