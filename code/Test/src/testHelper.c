
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



#include "testHelper.h"
#include NA_TEST_NALIB_PATH(./, NAString.h)
#include NA_TEST_NALIB_PATH(./, NAStack.h)



const char* na_yesno_strings[] = {
  "No",
  "Yes",
};


struct NATestData {
  const char* name;
  int32 lineNum;
  NABool success;
  NAStack childs;
  int32 childSuccessCount;
  int32 leafSuccessCount;
  int32 totalLeafCount;
  NATestData* parent;
};



NATestData* na_testData = NA_NULL;
NATestData* na_curTestData = NA_NULL;
int na_printAllTestGroups = 0;



void naInitTestingData(NATestData* testData, const char* name, NATestData* parent, int32 lineNum){
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
  naForeachStackMutable(&(testData->childs), naClearTestingData);
  naClearStack(&(testData->childs));
}



void naPrintTestName(NATestData* testData){
  if(testData->parent){naPrintTestName(testData->parent);}
  printf("%s ", testData->name);
}



void naPrintTestGroup(NATestData* testData){
  NAInt leafSuccessCount = (NAInt)testData->leafSuccessCount;
  NAInt leafTotalCount = (NAInt)testData->totalLeafCount;
  double leafRatio = (double)leafSuccessCount / (double)leafTotalCount * 100.;
  NAInt childSuccessCount = (NAInt)testData->childSuccessCount;
  NAInt childTotalCount = (NAInt)naGetStackCount(&(testData->childs));
  double childRatio = (double)childSuccessCount / (double)childTotalCount * 100.;

  printf("G ");
  if(testData->parent){naPrintTestName(testData->parent);}
  if(leafTotalCount == childTotalCount){
    printf("%s: %" NA_PRIi " / %" NA_PRIi " Tests ok (%.02f%%)" NA_NL, testData->name, leafSuccessCount, leafTotalCount, leafRatio);
  }else{
    printf("%s: %" NA_PRIi " / %" NA_PRIi " Groups ok (%.02f%%), %" NA_PRIi " / %" NA_PRIi " Tests ok (%.02f%%)" NA_NL, testData->name, childSuccessCount, childTotalCount, childRatio, leafSuccessCount, leafTotalCount, leafRatio);
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
  printf("Testing finished." NA_NL);
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
  naInitTestingData(testData, expr, na_curTestData, (int32)lineNum);
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
  naInitTestingData(testData, name, na_curTestData, (int32)lineNum);
  na_curTestData->childSuccessCount++;
  na_curTestData = testData;
}



void naStopTestGroup(){
  if(na_printAllTestGroups || !na_curTestData->success){
    naPrintTestGroup(na_curTestData);
  }
  na_curTestData = na_curTestData->parent;
}



// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

