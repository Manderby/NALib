
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
  int32 leafFailCount;
  NATestData* parent;
};



NATestData* na_testdata = NA_NULL;
NATestData* na_curtestdata = NA_NULL;

void naInitTestingData(NATestData* testData, const char* name, NATestData* parent, int32 lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NA_TRUE;
  naInitStack(&(testData->childs), naSizeof(NATestData), 2);
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->leafFailCount = 0;
  testData->parent = parent;
}




void naStartTesting(const char* rootName){
  #ifndef NDEBUG
    if(na_testdata)
      naError("Test already running.");
  #endif
  na_testdata = naAlloc(NATestData);

  naInitTestingData(na_testdata, rootName, NA_NULL, 0);

  na_curtestdata = na_testdata;
}



void naStopTesting(void){
  printf("Testing %s results:" NA_NL, na_testdata->name);

  naFree(na_testdata);
  na_testdata = NA_NULL;
  na_curtestdata = NA_NULL;
}



void naUpdateTestParent(NATestData* testData, NABool childSuccess, NABool leafSuccess){
  if(leafSuccess){
    testData->leafSuccessCount++;
  }else{
    testData->leafFailCount++;
  }
  if(childSuccess){
    testData->childSuccessCount++;
  }else{
    testData->success = NA_FALSE;
  }
  if (testData->parent) {
    naUpdateTestParent(testData->parent, testData->success, leafSuccess);
  }
}



void naAddSubTest(const char* name, int success, int lineNum){
  NATestData* testData = naPushStack(&(na_curtestdata->childs));
  naInitTestingData(testData, name, na_curtestdata, (int32)lineNum);
  testData->success = (NABool)success;
  naUpdateTestParent(na_curtestdata, (NABool)success, (NABool)success);
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

