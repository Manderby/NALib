
#include "NATest.h"
#include "NAMath/NARandom.h"
#include "NAUtility/NAString.h"
#include <stdio.h>


#define INTEGRATION_ARRAY_SIZE 1000000

float simpleSum(const float* array){
  float result = 0.f;
  for(size_t i = 0; i < INTEGRATION_ARRAY_SIZE; ++i){
    result += array[i];
  }
  return result;
}

float arrayAccess(const void* arrayPtr, float x){
  const float* array = (const float*) arrayPtr;
  return array[(size_t)naRoundf(x * (INTEGRATION_ARRAY_SIZE - 1))];
}

void testNAMathOperators(void){
  float* array = naMalloc(INTEGRATION_ARRAY_SIZE * sizeof(float));
  
  for(size_t i = 0; i < INTEGRATION_ARRAY_SIZE; ++i){
//    array[i] = i + 1;
    array[i] = INTEGRATION_ARRAY_SIZE - i;
//    array[i] = naUniformRandZEf();
  }
//  for(size_t i = 0; i < INTEGRATION_ARRAY_SIZE; ++i){
//    float tmp = array[i];
//    size_t randIndex = (size_t)(naUniformRandZE() * INTEGRATION_ARRAY_SIZE);
//    array[i] = array[randIndex];
//    array[randIndex] = tmp;
//  }
  
  volatile float simpleResult = simpleSum(array);
  printf("Simple: %f\n", simpleResult);

  volatile float kahanSum = naKahanSum(INTEGRATION_ARRAY_SIZE, array);
  printf("Kaham Sum: %f\n", kahanSum);

  volatile float kahanBabushkaNeumaierSum = naKahanBabushkaNeumaierSum(INTEGRATION_ARRAY_SIZE, array);
  printf("Kaham Babushka Neumaier Sum: %f\n", kahanBabushkaNeumaierSum);

  volatile float kahanBabushkaKleinSum = naKahanBabushkaKleinSum(INTEGRATION_ARRAY_SIZE, array);
  printf("Kaham Babushka Klein Sum: %f\n", kahanBabushkaKleinSum);
  
//  volatile float binaryPairsResult = naIntegratef(
//    INTEGRATION_ARRAY_SIZE,
//    arrayAccess,
//    array,
//    0.,
//    1.);
//  printf("Binary Pairs: %f\n", binaryPairsResult);
  
  volatile float binaryPairsResult2 = naSumf(INTEGRATION_ARRAY_SIZE, array);
  printf("Binary Pairs2: %f\n", binaryPairsResult2);

  naBenchmark((simpleResult += simpleSum(array)));
  naBenchmark((kahanSum += naKahanSum(INTEGRATION_ARRAY_SIZE, array)));
  naBenchmark((kahanBabushkaNeumaierSum += naKahanBabushkaNeumaierSum(INTEGRATION_ARRAY_SIZE, array)));
  naBenchmark((kahanBabushkaKleinSum += naKahanBabushkaKleinSum(INTEGRATION_ARRAY_SIZE, array)));
//  naBenchmark((binaryPairsResult += naIntegratef(INTEGRATION_ARRAY_SIZE, arrayAccess, array, 0., 1.)));
  naBenchmark((binaryPairsResult2 += naSumf(INTEGRATION_ARRAY_SIZE, array)));

//  printf("Simple: %f\n", simpleResult);
//  printf("Binary Pairs: %f\n", binaryPairsResult);
//  printf("Binary Pairs2: %f\n", binaryPairsResult2);

  naFree(array);
}

void testNAMath(void){
  naTestFunction(testNAMathOperators);
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
