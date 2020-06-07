
#include "../../NATesting.h"

#if NA_TESTING_ENABLED == 1

#include <stdio.h>
#include "../../NAStack.h"
#include "../../NAString.h"



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
  int printAllTestGroups;
  NABool testCaseRunning;
  int errorCount;
  NAStack untestedStrings;
};

NATesting* na_Testing = NA_NULL;



NA_HIDEF void naInitTestingData(NATestData* testData, const char* name, NATestData* parent, int lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NA_TRUE;
  naInitStack(&(testData->childs), naSizeof(NATestData), 2);
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->totalLeafCount = 0;
  testData->parent = parent;
}



NA_HIDEF void naClearTestingData(NATestData* testData){
  naForeachStackMutable(&(testData->childs), (NAMutator)naClearTestingData);
  naClearStack(&(testData->childs));
}



NA_HIDEF void naPrintTestName(NATestData* testData){
  if(testData->parent){naPrintTestName(testData->parent);}
  printf("%s ", testData->name);
}



NA_HIDEF void naPrintRatio(int successCount, int totalCount){
  if(totalCount){
    double ratio = (double)successCount / (double)totalCount * 100.;
    printf (" (%.02f%%)", ratio);
  }
}


NA_HIDEF void naPrintTestGroup(NATestData* testData){
  int leafSuccessCount = testData->leafSuccessCount;
  int leafTotalCount = testData->totalLeafCount;
  int childSuccessCount = testData->childSuccessCount;
  int childTotalCount = (int)naGetStackCount(&(testData->childs));

  printf("G ");
  if(testData->parent){naPrintTestName(testData->parent);}
  if(leafTotalCount == childTotalCount){
    printf("%s: %d / %d Tests ok", testData->name, leafSuccessCount, leafTotalCount);
    naPrintRatio(leafSuccessCount, leafTotalCount);
    printf(NA_NL);
  }else{
    printf("%s: %d / %d Groups ok", testData->name, childSuccessCount, childTotalCount);
    naPrintRatio(childSuccessCount, childTotalCount);
    printf(", %d / %d Tests ok", leafSuccessCount, leafTotalCount);
    naPrintRatio(leafSuccessCount, leafTotalCount);
    printf(NA_NL);
  }
}



NA_DEF void naStartTesting(const NAUTF8Char* rootName, NABool printAllGroups){
#ifndef NDEBUG
  if(na_Testing)
    naError("Testing already running.");
#endif

  na_Testing = naAlloc(NATesting);

  na_Testing->testData = naAlloc(NATestData);
  naInitTestingData(na_Testing->testData, rootName, NA_NULL, 0);

  na_Testing->curTestData = na_Testing->testData;
  na_Testing->printAllTestGroups = printAllGroups;
  na_SetTestCaseRunning(NA_FALSE);
  na_ResetErrorCount();

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
      naPrintTestGroup(na_Testing->testData);
    }
  }
  printf("Testing finished." NA_NL NA_NL);

  naClearTestingData(na_Testing->testData);
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



NA_HHDEF void na_UpdateTestParentLeaf(NATestData* testData, NABool leafSuccess){
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



NA_HHDEF void na_AddTest(const char* expr, int success, int lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  naInitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  if(na_GetErrorCount() > 0){
    testData->success = NA_FALSE;
    na_UpdateTestParentLeaf(na_Testing->curTestData, NA_FALSE);
    printf("  ");
    if(testData->parent){naPrintTestName(testData->parent);}
    printf("Line %d: %d errors occured in %s" NA_NL, lineNum, na_GetErrorCount(), expr);\
  }else{
    testData->success = (NABool)success;
    na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)success);
    if(!success){
      printf("  ");
      if(testData->parent){naPrintTestName(testData->parent);}
      printf("Line %d: %s" NA_NL, lineNum, expr);\
    }
  }
}



NA_HHDEF void na_AddTestError(const char* expr, int lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  naInitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  testData->success = na_GetErrorCount() != 0;
  na_UpdateTestParentLeaf(na_Testing->curTestData, (NABool)testData->success);
  if(!testData->success){
    printf("  ");
    if(testData->parent){naPrintTestName(testData->parent);}
    printf("Line %d: No Error raised in %s" NA_NL, lineNum, expr);\
  }
}



NA_HHDEF void na_RegisterUntested(const char* text){
  NAString** string = naPushStack(&(na_Testing->untestedStrings));
  *string = naNewStringWithUTF8CStringLiteral(text);
}



NA_HHDEF NABool na_GetTestCaseRunning(){
  return na_Testing->testCaseRunning;
}



NA_HHDEF void na_SetTestCaseRunning(NABool running){
  na_Testing->testCaseRunning = running;
}



NA_HHDEF void na_IncErrorCount(void){
  na_Testing->errorCount++;
}



NA_HHDEF void na_ResetErrorCount(void){
  na_Testing->errorCount = 0;
}



NA_HHDEF int na_GetErrorCount(void){
  return na_Testing->errorCount;
}



NA_HHDEF void na_StartTestGroup(const char* name, int lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  naInitTestingData(testData, name, na_Testing->curTestData, lineNum);
  na_Testing->curTestData->childSuccessCount++;
  na_Testing->curTestData = testData;
}



NA_HHDEF void na_StopTestGroup(){
  #ifndef NDEBUG
  if(!na_Testing)
    naError("Testing not running. Use naStartTesting.");
  #endif

  if(na_Testing->printAllTestGroups || !na_Testing->curTestData->success){
    naPrintTestGroup(na_Testing->curTestData);
  }
  na_Testing->curTestData = na_Testing->curTestData->parent;
}


#else

NA_DEF void naStartTesting(const NAUTF8Char* rootName, NABool printAllGroups){
  #ifndef NDEBUG
    NA_UNUSED(rootName);
    NA_UNUSED(printAllGroups);
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
