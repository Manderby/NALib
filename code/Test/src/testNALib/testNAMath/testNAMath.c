
#include "NATest.h"
#include "NAMath/NARandom.h"
#include "NAUtility/NAString.h"
#include <stdio.h>



#define INTEGRATION_ARRAY_SIZE 100
#define INTEGRATION_RESULT ((INTEGRATION_ARRAY_SIZE + 1) * (INTEGRATION_ARRAY_SIZE / 2))



float arrayAccessf(const void* arrayPtr, float x){
  const float* array = (const float*) arrayPtr;
  return array[(size_t)naRoundf(x * (INTEGRATION_ARRAY_SIZE - 1))];
}

double arrayAccess(const void* arrayPtr, double x){
  const double* array = (const double*) arrayPtr;
  return array[(size_t)naRound(x * (INTEGRATION_ARRAY_SIZE - 1))];
}



float* createTestArrayf(size_t count){
  float* array = naMalloc(count * sizeof(float));
  
  // Fill the array with values from n to 1 (backwards)
  for(size_t i = 0; i < count; ++i){
    array[i] = (float)(count - i);
  }

  return array;
}

double* createTestArray(size_t count){
  double* array = naMalloc(count * sizeof(double));
  
  // Fill the array with values from n to 1 (backwards)
  for(size_t i = 0; i < count; ++i){
    array[i] = (double)(count - i);
  }

  return array;
}



void testIntegrationFunctionsf(void){
  float* array = createTestArrayf(INTEGRATION_ARRAY_SIZE);
  
  float simpleResult;
  float kahanSum;
  float kahanBabushkaNeumaierSum;
  float kahanBabushkaKleinSum;
  float binaryPairsResult;
  float binaryPairsResult2;
  
  naTestGroup("Check correctness of result"){
    simpleResult = naSimpleSumf(INTEGRATION_ARRAY_SIZE, array);
    naTest(simpleResult == INTEGRATION_RESULT);

    kahanSum = naKahanSumf(INTEGRATION_ARRAY_SIZE, array);
    naTest(kahanSum == INTEGRATION_RESULT);

    kahanBabushkaNeumaierSum = naKahanBabushkaNeumaierSumf(INTEGRATION_ARRAY_SIZE, array);
    naTest(kahanBabushkaNeumaierSum == INTEGRATION_RESULT);

    kahanBabushkaKleinSum = naKahanBabushkaKleinSumf(INTEGRATION_ARRAY_SIZE, array);
    naTest(kahanBabushkaKleinSum == INTEGRATION_RESULT);
    
//    binaryPairsResult = naIntegratef(
//      INTEGRATION_ARRAY_SIZE,
//      arrayAccess,
//      array,
//      0.f,
//      1.f);
//    naTest(binaryPairsResult == INTEGRATION_RESULT);

    binaryPairsResult2 = naSumf(INTEGRATION_ARRAY_SIZE, array);
    naTest(binaryPairsResult2 == INTEGRATION_RESULT);
  }

  naTestGroup("Nullpointer array"){
    naTestCrash(naSimpleSumf(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naKahanSumf(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naKahanBabushkaNeumaierSumf(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naKahanBabushkaKleinSumf(INTEGRATION_ARRAY_SIZE, NA_NULL));
//    naTestCrash(naIntegratef(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naSumf(INTEGRATION_ARRAY_SIZE, NA_NULL));
  }

  naTestGroup("Zero size access"){
    naTest(naSimpleSumf(0, NA_NULL) == 0.f);
    naTest(naKahanSumf(0, NA_NULL) == 0.f);
    naTest(naKahanBabushkaNeumaierSumf(0, NA_NULL) == 0.f);
    naTest(naKahanBabushkaKleinSumf(0, NA_NULL) == 0.f);
//    naTestCrash(naIntegratef(0, NA_NULL) == 0.f);
    naTest(naSumf(0, NA_NULL) == 0.f);
  }

  naTestGroup("Size 1 access"){
    naTest(naSimpleSumf(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naKahanSumf(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naKahanBabushkaNeumaierSumf(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naKahanBabushkaKleinSumf(1, array) == INTEGRATION_ARRAY_SIZE);
//    naTestCrash(naIntegratef(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naSumf(1, array) == INTEGRATION_ARRAY_SIZE);
  }

  naFree(array);
}



void testIntegrationFunctions(void){
  double* array = createTestArray(INTEGRATION_ARRAY_SIZE);
  
  double simpleResult;
  double kahanSum;
  double kahanBabushkaNeumaierSum;
  double kahanBabushkaKleinSum;
  double binaryPairsResultFunc;
  double binaryPairsResult;
  double binaryPairsResult2;

  naTestGroup("Check correctness of result"){
    simpleResult = naSimpleSum(INTEGRATION_ARRAY_SIZE, array);
    naTest(simpleResult == INTEGRATION_RESULT);

    kahanSum = naKahanSum(INTEGRATION_ARRAY_SIZE, array);
    naTest(kahanSum == INTEGRATION_RESULT);

    kahanBabushkaNeumaierSum = naKahanBabushkaNeumaierSum(INTEGRATION_ARRAY_SIZE, array);
    naTest(kahanBabushkaNeumaierSum == INTEGRATION_RESULT);

    kahanBabushkaKleinSum = naKahanBabushkaKleinSum(INTEGRATION_ARRAY_SIZE, array);
    naTest(kahanBabushkaKleinSum == INTEGRATION_RESULT);
    
//    binaryPairsResultFunc = naIntegrate(
//      INTEGRATION_ARRAY_SIZE,
//      arrayAccess,
//      array,
//      0.,
//      1.);
//    naTest(binaryPairsResultFunc == INTEGRATION_RESULT);

    binaryPairsResult = naSum(INTEGRATION_ARRAY_SIZE, array);
    naTest(binaryPairsResult == INTEGRATION_RESULT);

    binaryPairsResult2 = naSum2(INTEGRATION_ARRAY_SIZE, array);
    naTest(binaryPairsResult2 == INTEGRATION_RESULT);
  }

  naTestGroup("Nullpointer array"){
    naTestCrash(naSimpleSum(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naKahanSum(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naKahanBabushkaNeumaierSum(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naKahanBabushkaKleinSum(INTEGRATION_ARRAY_SIZE, NA_NULL));
//    naTestCrash(naIntegrate(INTEGRATION_ARRAY_SIZE, NA_NULL));
    naTestCrash(naSum(INTEGRATION_ARRAY_SIZE, NA_NULL));
  }

  naTestGroup("Zero size access"){
    naTest(naSimpleSum(0, NA_NULL) == 0.f);
    naTest(naKahanSum(0, NA_NULL) == 0.f);
    naTest(naKahanBabushkaNeumaierSum(0, NA_NULL) == 0.f);
    naTest(naKahanBabushkaKleinSum(0, NA_NULL) == 0.f);
//    naTestCrash(naIntegrate(0, NA_NULL) == 0.f);
    naTest(naSum(0, NA_NULL) == 0.f);
  }

  naTestGroup("Size 1 access"){
    naTest(naSimpleSum(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naKahanSum(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naKahanBabushkaNeumaierSum(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naKahanBabushkaKleinSum(1, array) == INTEGRATION_ARRAY_SIZE);
//    naTestCrash(naIntegrate(1, array) == INTEGRATION_ARRAY_SIZE);
    naTest(naSum(1, array) == INTEGRATION_ARRAY_SIZE);
  }

  naFree(array);
}



void benchmarkIntegrationFunctionsf(void){
  size_t count = 1000000000;
  float* array = createTestArrayf(count);

  volatile float simpleResult = 0.f;
  volatile float kahanSum = 0.f;
  volatile float kahanBabushkaNeumaierSum = 0.f;
  volatile float kahanBabushkaKleinSum = 0.f;
  volatile float binaryPairsResult = 0.f;
  volatile float binaryPairsResult2 = 0.f;

  naBenchmark((simpleResult += naSimpleSumf(count, array)));
  naBenchmark((kahanSum += naKahanSumf(count, array)));
  naBenchmark((kahanBabushkaNeumaierSum += naKahanBabushkaNeumaierSumf(count, array)));
  naBenchmark((kahanBabushkaKleinSum += naKahanBabushkaKleinSumf(count, array)));
  naBenchmark((binaryPairsResult += naIntegratef(count, arrayAccessf, array, 0.f, 1.f)));
  naBenchmark((binaryPairsResult2 += naSumf(count, array)));

  naFree(array);
}

void benchmarkIntegrationFunctions(void){
  size_t count = 1000000;
  double* array = createTestArray(count);

  volatile double simpleResult = 0.;
  volatile double kahanSum = 0.;
  volatile double kahanBabushkaNeumaierSum = 0.;
  volatile double kahanBabushkaKleinSum = 0.;
  volatile double binaryPairsResult = 0.;
  volatile double binaryPairsResult2 = 0.;

  naBenchmark((simpleResult += naSimpleSum(count, array)));
  naBenchmark((kahanSum += naKahanSum(count, array)));
  naBenchmark((kahanBabushkaNeumaierSum += naKahanBabushkaNeumaierSum(count, array)));
  naBenchmark((kahanBabushkaKleinSum += naKahanBabushkaKleinSum(count, array)));
  naBenchmark((binaryPairsResult += naIntegrate(count, arrayAccess, array, 0., 1.)));
  naBenchmark((binaryPairsResult2 += naSum(count, array)));
  naBenchmark((binaryPairsResult2 += naSum2(count, array)));

  naFree(array);
}



void testNAMath(void){
  //naTestFunction(testIntegrationFunctionsf);
  naTestFunction(testIntegrationFunctions);
}



void benchmarkNAMath(void){
  //naTestFunction(benchmarkIntegrationFunctionsf);
  naTestFunction(benchmarkIntegrationFunctions);
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
