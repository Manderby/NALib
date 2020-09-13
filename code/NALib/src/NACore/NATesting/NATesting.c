
#include "../../NATesting.h"

#if NA_TESTING_ENABLED == 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if NA_OS != NA_OS_WINDOWS
#include <sys/time.h>
#endif

#include "../../NAStack.h"
#include "../../NAString.h"



#define NA_TEST_INDEX_COUNT 0x10000
#define NA_TEST_INDEX_MASK (NA_TEST_INDEX_COUNT - 1)

typedef struct NATestData NATestData;
struct NATestData {
  const char* name;
  int lineNum;
  NABool success;
  NAStack childs;
  int childSuccessCount;
  int leafSuccessCount;
  int totalLeafCount;
  NATestData* parent;
};

typedef struct NATesting NATesting;
struct NATesting {
  NATestData* testData;
  NATestData* curTestData;
  double timePerBenchmark;
  int printAllTestGroups;
  NABool testCaseRunning;
  int errorCount;
  NAStack untestedStrings;
  int curInIndex;
  uint32 in[NA_TEST_INDEX_COUNT];
  char out[NA_TEST_INDEX_COUNT];
};

NATesting* na_Testing = NA_NULL;



NA_HIDEF void na_InitTestingData(NATestData* testData, const char* name, NATestData* parent, int lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NA_TRUE;
  naInitStack(&(testData->childs), naSizeof(NATestData), 2);
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->totalLeafCount = 0;
  testData->parent = parent;
}



NA_HIDEF void na_ClearTestingData(NATestData* testData){
  naForeachStackMutable(&(testData->childs), (NAMutator)na_ClearTestingData);
  naClearStack(&(testData->childs));
}



NA_HIDEF void na_PrintTestName(NATestData* testData){
  if(testData->parent){na_PrintTestName(testData->parent);}
  printf("%s ", testData->name);
}



NA_HIDEF void na_PrintRatio(int successCount, int totalCount){
  if(totalCount){
    double ratio = (double)successCount / (double)totalCount * 100.;
    printf (" (%.02f%%)", ratio);
  }
}


NA_HIDEF void na_PrintTestGroup(NATestData* testData){
  int leafSuccessCount = testData->leafSuccessCount;
  int leafTotalCount = testData->totalLeafCount;
  int childSuccessCount = testData->childSuccessCount;
  int childTotalCount = (int)naGetStackCount(&(testData->childs));

  printf("G ");
  if(testData->parent){na_PrintTestName(testData->parent);}
  if(leafTotalCount == childTotalCount){
    printf("%s: %d / %d Tests ok", testData->name, leafSuccessCount, leafTotalCount);
    na_PrintRatio(leafSuccessCount, leafTotalCount);
    printf(NA_NL);
  }else{
    printf("%s: %d / %d Groups ok", testData->name, childSuccessCount, childTotalCount);
    na_PrintRatio(childSuccessCount, childTotalCount);
    printf(", %d / %d Tests ok", leafSuccessCount, leafTotalCount);
    na_PrintRatio(leafSuccessCount, leafTotalCount);
    printf(NA_NL);
  }
}



NA_DEF void naStartTesting(const NAUTF8Char* rootName, double timePerBenchmark, NABool printAllGroups){
#ifndef NDEBUG
  if(na_Testing)
    naError("Testing already running.");
#endif

  na_Testing = naAlloc(NATesting);

  na_Testing->testData = naAlloc(NATestData);
  na_InitTestingData(na_Testing->testData, rootName, NA_NULL, 0);

  na_Testing->curTestData = na_Testing->testData;
  na_Testing->timePerBenchmark = timePerBenchmark;
  na_Testing->printAllTestGroups = printAllGroups;
  na_SetTestCaseRunning(NA_FALSE);
  na_ResetErrorCount();

  for(na_Testing->curInIndex = 0; na_Testing->curInIndex < NA_TEST_INDEX_COUNT; na_Testing->curInIndex++){
    na_Testing->in[na_Testing->curInIndex] = ((uint32)rand() << 20) ^ ((uint32)rand() << 10) ^ ((uint32)rand());
  }

  naInitStack(&(na_Testing->untestedStrings), naSizeof(NAString*), 2);

}



NA_DEF void naStopTesting(){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  na_StopTestGroup();

  if(na_Testing->testData->success){
    printf("All test successful." NA_NL);
    if(!na_Testing->printAllTestGroups){
      na_PrintTestGroup(na_Testing->testData);
    }
  }
  printf("Testing finished." NA_NL NA_NL);

  na_ClearTestingData(na_Testing->testData);
  naFree(na_Testing->testData);
  naForeachStackpMutable(&(na_Testing->untestedStrings), (NAMutator)naDelete);
  naClearStack(&(na_Testing->untestedStrings));

  naFree(na_Testing);
  na_Testing = NA_NULL;
}



NA_DEF void naPrintUntested(void){
  NAInt stackCount =  naGetStackCount(&(na_Testing->untestedStrings));
  if(!stackCount){
    printf("No untested functionality." NA_NL);
  }else{
    printf("Untested functionality (%d):" NA_NL, (int)stackCount);
    NAStackIterator iter = naMakeStackAccessor(&(na_Testing->untestedStrings));
    while(naIterateStack(&iter)){
      const NAString* string = naGetStackCurpConst(&iter);
      printf("U %s" NA_NL, naGetStringUTF8Pointer(string));
    }
    naClearStackIterator(&iter);
  }
}



NA_HDEF void na_UpdateTestParentLeaf(NATestData* testData, NABool leafSuccess){
  if(testData->parent){
    na_UpdateTestParentLeaf(testData->parent, leafSuccess);
  }

  testData->totalLeafCount++;
  if(leafSuccess){
    testData->leafSuccessCount++;
  }else{
    if(testData->success && testData->parent){
      testData->parent->childSuccessCount--;
    }
    testData->success = NA_FALSE;
  }
}



NA_HDEF void na_AddTest(const char* expr, int success, int lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  if(na_GetErrorCount() > 0){
    testData->success = NA_FALSE;
    na_UpdateTestParentLeaf(na_Testing->curTestData, NA_FALSE);
    printf("  ");
    if(testData->parent){na_PrintTestName(testData->parent);}
    printf("Line %d: %d errors occured in %s" NA_NL, lineNum, na_GetErrorCount(), expr);\
  }else{
    testData->success = (NABool)success;
    na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)success);
    if(!success){
      printf("  ");
      if(testData->parent){na_PrintTestName(testData->parent);}
      printf("Line %d: %s" NA_NL, lineNum, expr);\
    }
  }
}



NA_HDEF void na_AddTestError(const char* expr, int lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  testData->success = na_GetErrorCount() != 0;
  na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)testData->success);
  if(!testData->success){
    printf("  ");
    if(testData->parent){na_PrintTestName(testData->parent);}
    printf("Line %d: No Error raised in %s" NA_NL, lineNum, expr);\
  }
}



NA_HDEF void na_RegisterUntested(const char* text){
  NAString** string = naPushStack(&(na_Testing->untestedStrings));
  *string = naNewStringWithUTF8CStringLiteral(text);
}



NA_HDEF NABool na_GetTestCaseRunning(){
  return na_Testing->testCaseRunning;
}



NA_HDEF void na_SetTestCaseRunning(NABool running){
  na_Testing->testCaseRunning = running;
}



NA_HDEF void na_IncErrorCount(void){
  na_Testing->errorCount++;
}



NA_HDEF void na_ResetErrorCount(void){
  na_Testing->errorCount = 0;
}



NA_HDEF int na_GetErrorCount(void){
  return na_Testing->errorCount;
}



NA_HDEF void na_StartTestGroup(const char* name, int lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, name, na_Testing->curTestData, lineNum);
  na_Testing->curTestData->childSuccessCount++;
  na_Testing->curTestData = testData;
}



NA_HDEF void na_StopTestGroup(){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  if(na_Testing->printAllTestGroups || !na_Testing->curTestData->success){
    na_PrintTestGroup(na_Testing->curTestData);
  }
  na_Testing->curTestData = na_Testing->curTestData->parent;
}



NA_HDEF uint32 na_getBenchmarkIn(){
  na_Testing->curInIndex = (na_Testing->curInIndex + 1) & NA_TEST_INDEX_MASK;
  return na_Testing->in[na_Testing->curInIndex];
}



NA_HDEF double na_BenchmarkTime(){
  // Note: Reimplemented here because NADateTime uses int64 to compute.
  #if NA_OS == NA_OS_WINDOWS
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    fileTime.dwLowDateTime;
    return fileTime.dwLowDateTime / 10000000.;  // see definition of dwLowDateDime
  #else
    struct timeval curtime;
    struct timezone curtimezone;
    gettimeofday(&curtime, &curtimezone);
    return curtime.tv_sec + curtime.tv_usec / 1000000.;
  #endif
}



NA_HDEF double na_GetBenchmarkLimit(){
  return na_Testing->timePerBenchmark;
}



NA_HDEF int na_GetBenchmarkTestSizeLimit(){
  return 30;
}



NA_HDEF void na_PrintBenchmark(double timeDiff, int testSize, const char* exprString, int lineNum){
  if(timeDiff < na_GetBenchmarkLimit() || testSize >= 1 << na_GetBenchmarkTestSizeLimit()){
      printf("Line %d: Immeasurable   : %s" NA_NL, lineNum, exprString);
  }else{
    double execsPerSec = testSize / timeDiff;
    if(execsPerSec > 1000000000.)
      printf("Line %d: %6.2f G : %s" NA_NL, lineNum, execsPerSec * .000000001, exprString);
    else if(execsPerSec > 1000000.)
      printf("Line %d: %6.2f M : %s" NA_NL, lineNum, execsPerSec * .000001, exprString);
    else if(execsPerSec < 1000000.)
      printf("Line %d: %6.2f k : %s" NA_NL, lineNum, execsPerSec * .001, exprString);
    else
      printf("Line %d: %6.2f   : %s" NA_NL, lineNum, execsPerSec, exprString);
  }
}



NA_HDEF void na_StoreBenchmarkResult(char data){
  // yes, we are using the inIndex. It doesn't matter.
  na_Testing->out[na_Testing->curInIndex] ^= data;\
}



#else

NA_DEF void naStartTesting(const NAUTF8Char* rootName, double timePerBenchmark, NABool printAllGroups){
  NA_UNUSED(rootName);
  NA_UNUSED(timePerBenchmark);
  NA_UNUSED(printAllGroups);
  #ifndef NDEBUG
    naError("Testing is not enabled. Go look for NA_TESTING_ENABLED");
  #endif
}



NA_DEF void naStopTesting(){
  #ifndef NDEBUG
    naError("Testing is not enabled. Go look for NA_TESTING_ENABLED");
  #endif
}

#endif // NA_TESTING_ENABLED == 1



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
