
#include "testHelper.h"
#include "NAString.h"
#include "NAStack.h"



const char* na_yesno_strings[] = {
  "No",
  "Yes",
};


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



NATestData* na_testData = NA_NULL;
NATestData* na_curTestData = NA_NULL;
int na_printAllTestGroups = 0;



void naInitTestingData(NATestData* testData, const char* name, NATestData* parent, int lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NA_TRUE;
  naInitStack(&(testData->childs), naSizeof(NATestData), 2);
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->totalLeafCount = 0;
  testData->parent = parent;
}



void naClearTestingData(NATestData* testData){
  naForeachStackMutable(&(testData->childs), (NAMutator)naClearTestingData);
  naClearStack(&(testData->childs));
}



void naPrintTestName(NATestData* testData){
  if(testData->parent){naPrintTestName(testData->parent);}
  printf("%s ", testData->name);
}



void naPrintRatio(int successCount, int totalCount){
  if(totalCount){
    double ratio = (double)successCount / (double)totalCount * 100.;
    printf (" (%.02f%%)", ratio);
  }
}


void naPrintTestGroup(NATestData* testData){
  int leafSuccessCount = testData->leafSuccessCount;
  int leafTotalCount = testData->totalLeafCount;
  int childSuccessCount = testData->childSuccessCount;
  int childTotalCount = naGetStackCount(&(testData->childs));

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



void naStartTesting(const char* rootName){
  #ifndef NDEBUG
    if(na_testData)
      naError("Test already running.");
  #endif
  na_testData = naAlloc(NATestData);

  naInitTestingData(na_testData, rootName, NA_NULL, 0);

  na_curTestData = na_testData;
}



void naStopTesting(void){
  naStopTestGroup();

  if(na_testData->success){
    printf("All test successful." NA_NL);
    if(!na_printAllTestGroups){
      naPrintTestGroup(na_testData);
    }
  }
  printf("Testing finished." NA_NL NA_NL);
  na_curTestData = NA_NULL;
  naClearTestingData(na_testData);
  naFree(na_testData);
  na_testData = NA_NULL;
}



void naUpdateTestParentLeaf(NATestData* testData, NABool leafSuccess){
  if(testData->parent){
    naUpdateTestParentLeaf(testData->parent, leafSuccess);
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



void naAddTest(const char* expr, int success, int lineNum){
  NATestData* testData = naPushStack(&(na_curTestData->childs));
  naInitTestingData(testData, expr, na_curTestData, lineNum);
  testData->success = (NABool)success;
  naUpdateTestParentLeaf(na_curTestData, (NABool)success);
  if(!success){
    printf("  ");
    if(testData->parent){naPrintTestName(testData->parent);}
    printf("Line %d: %s" NA_NL, lineNum, expr);\
  }
}



void naStartTestGroup(const char* name, int lineNum){
  NATestData* testData = naPushStack(&(na_curTestData->childs));
  naInitTestingData(testData, name, na_curTestData, lineNum);
  na_curTestData->childSuccessCount++;
  na_curTestData = testData;
}



void naStopTestGroup(){
  if(na_printAllTestGroups || !na_curTestData->success){
    naPrintTestGroup(na_curTestData);
  }
  na_curTestData = na_curTestData->parent;
}



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
