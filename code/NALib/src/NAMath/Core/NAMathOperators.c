
// This file contains inline implementations of the file NAMathOperators.h
// Do not include this file directly! It will automatically be included when
// including "NAMathOperators.h"


#include "../NAMathOperators.h"



#define _MM_HINT_T0 1
#define _MM_HINT_T1 2
#define _MM_HINT_T2 3



NA_DEF float naSimpleSumf(size_t sampleCount, const float* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif
  float result = 0.f;
  for(size_t i = 0; i < sampleCount; ++i) {
    result += array[i];
  }
  return result;
}

NA_DEF double naSimpleSum(size_t sampleCount, const double* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif
  double result = 0.;
  for(size_t i = 0; i < sampleCount; ++i) {
    result += array[i];
  }
  return result;
}



// Copied and adapted from Wikipedia
NA_DEF float naKahanSumf(size_t sampleCount, const float* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  // Prepare the accumulator.
  float sum = 0.f;
  // A running compensation for lost low-order bits.
  float c = 0.f;
  // The array input has elements indexed input[1] to input[input.length].
  for(size_t iStep = 0; iStep < sampleCount; ++iStep) {
    // c is zero the first time around.
    float y = array[iStep] - c;
    // Alas, sum is big, y small, so low-order digits of y are lost.         
    float t = sum + y;
    // (t - sum) cancels the high-order part of y;
    // subtracting y recovers negative (low part of y)
    c = (t - sum) - y;
    // Algebraically, c should always be zero. Beware
    // overly-aggressive optimizing compilers!
    sum = t;
  }// Next time around, the lost low part will be added to y in a fresh attempt.

  return sum;
}

// Copied and adapted from Wikipedia
NA_DEF double naKahanSum(size_t sampleCount, const double* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  // Prepare the accumulator.
  double sum = 0.;
  // A running compensation for lost low-order bits.
  double c = 0.;
  // The array input has elements indexed input[1] to input[input.length].
  for(size_t iStep = 0; iStep < sampleCount; ++iStep) {
    // c is zero the first time around.
    double y = array[iStep] - c;
    // Alas, sum is big, y small, so low-order digits of y are lost.         
    double t = sum + y;
    // (t - sum) cancels the high-order part of y;
    // subtracting y recovers negative (low part of y)
    c = (t - sum) - y;
    // Algebraically, c should always be zero. Beware
    // overly-aggressive optimizing compilers!
    sum = t;
  }// Next time around, the lost low part will be added to y in a fresh attempt.

  return sum;
}



// Copied and adapted from Wikipedia
NA_DEF float naKahanBabushkaNeumaierSumf(size_t sampleCount, const float* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  float sum = 0.f;
  float c = 0.f;                       // A running compensation for lost low-order bits.

  for(size_t iStep = 0; iStep < sampleCount; ++iStep) {
    float t = sum + array[iStep];
    if(naAbsf(sum) >= naAbsf(array[iStep]))
      c += (sum - t) + array[iStep]; // If sum is bigger, low-order digits of input[iStep] are lost.
    else
      c += (array[iStep] - t) + sum; // Else low-order digits of sum are lost.
    sum = t;
  }

  return sum + c;                   // Correction only applied once in the very end.
}

// Copied and adapted from Wikipedia
NA_DEF double naKahanBabushkaNeumaierSum(size_t sampleCount, const double* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  double sum = 0.;
  double c = 0.;                       // A running compensation for lost low-order bits.

  for(size_t iStep = 0; iStep < sampleCount; ++iStep) {
    double t = sum + array[iStep];
    if(naAbs(sum) >= naAbs(array[iStep]))
      c += (sum - t) + array[iStep]; // If sum is bigger, low-order digits of input[iStep] are lost.
    else
      c += (array[iStep] - t) + sum; // Else low-order digits of sum are lost.
    sum = t;
  }

  return sum + c;                   // Correction only applied once in the very end.
}



// Copied and adapted from Wikipedia
NA_DEF float naKahanBabushkaKleinSumf(size_t sampleCount, const float* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  float sum = 0.f;
  float cs  = 0.f;
  float ccs = 0.f;

  for(size_t iStep = 0; iStep < sampleCount; ++iStep) {
    float c;
    float cc;
    float t = sum + array[iStep];
    if(naAbsf(sum) >= naAbsf(array[iStep]))
      c = (sum - t) + array[iStep];
    else
      c = (array[iStep] - t) + sum;

    sum = t;
    t = cs + c;
    if(naAbsf(cs) >= naAbsf(c))
      cc = (cs - t) + c;
    else
      cc = (c - t) + cs;

    cs = t;
    ccs = ccs + cc;
  }

  return sum + cs + ccs;
}

// Copied and adapted from Wikipedia
NA_DEF double naKahanBabushkaKleinSum(size_t sampleCount, const double* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  double sum = 0.;
  double cs  = 0.;
  double ccs = 0.;

  for(size_t iStep = 0; iStep < sampleCount; ++iStep) {
    double c;
    double cc;
    double t = sum + array[iStep];
    if(naAbs(sum) >= naAbs(array[iStep]))
      c = (sum - t) + array[iStep];
    else
      c = (array[iStep] - t) + sum;

    sum = t;
    t = cs + c;
    if(naAbs(cs) >= naAbs(c))
      cc = (cs - t) + c;
    else
      cc = (c - t) + cs;

    cs = t;
    ccs = ccs + cc;
  }

  return sum + cs + ccs;
}



NA_DEF float naSumf(size_t sampleCount, const float* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  float tmpSums[8 * sizeof(size_t)];  // 8 denotes bits per Byte
  memset(tmpSums, 0, sizeof(float) * (8 * sizeof(size_t))); // nullify.

  if(sampleCount > 1) {   
    for(size_t iStep = 0; iStep < sampleCount; iStep += 2) {
      // Add two neighboring values together with the temp sums up to the
      // position where the current iStep has its first binary 0.
      float walkingSum = array[iStep] + array[iStep + 1];
      char p = 1; // the current tmpSum as well as the current bit of iStep
      size_t step = 2; // = 2^p
      while(iStep & step) {
        walkingSum += tmpSums[p];
        p++;
        step <<= 1;
      }
      // Store the current sum at the position of the subsequent binary 1
      tmpSums[p] = walkingSum;
    }
  }
  
  // If the count is odd, store the remaining value at position 0.
  if(sampleCount & 1) {
    tmpSums[0] = array[sampleCount - 1];
  }
  
  float sum = 0.f;
  // Finally, go though all temp sums and add those to the final sum where
  // sampleCount has a binary 1
  for(size_t i = 0; i < 8 * sizeof(size_t); ++i) {
    if(sampleCount & ((size_t)1 << i)) {
      sum += tmpSums[i];
    }
  }
  return sum;
}

NA_DEF double naSum(size_t sampleCount, const double* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  double tmpSums[8 * sizeof(size_t)];  // 8 denotes bits per Byte
  memset(tmpSums, 0, sizeof(double) * (8 * sizeof(size_t))); // nullify.

  if(sampleCount > 1) {
    for(size_t iStep = 0; iStep < sampleCount; iStep += 2) {
      // Add two neighboring values together with the temp sums up to the
      // position where the current iStep has its first binary 0.
      double walkingSum = array[iStep] + array[iStep + 1];
      char p = 1; // the current tmpSum as well as the current bit of iStep
      size_t step = 2; // = 2^p
      while(iStep & step) {
        walkingSum += tmpSums[p];
        p++;
        step <<= 1;
      }
      // Store the current sum at the position of the subsequent binary 1
      tmpSums[p] = walkingSum;
    }
  }

  // If the count is odd, store the remaining value at position 0.
  if(sampleCount & 1) {
    tmpSums[0] = array[sampleCount - 1];
  }

  double sum = 0.;
  // Finally, go though all temp sums and add those to the final sum where
  // sampleCount has a binary 1
  for(size_t i = 0; i < 8 * sizeof(size_t); ++i) {
    if(sampleCount & ((size_t)1 << i)) {
      sum += tmpSums[i];
    }
  }
  return sum;
}

NA_DEF double naSum2(size_t sampleCount, const double* array) {
  #if NA_DEBUG
    if(!array && sampleCount)
      naCrash("array is null pointer");
  #endif

  double tmpSums[8 * sizeof(size_t)];  // 8 denotes bits per Byte
  memset(tmpSums, 0, sizeof(double) * (8 * sizeof(size_t))); // nullify.

  if(sampleCount > 1) {
    const double* arrayPtr = array;
    for(size_t iStep = 0; iStep < sampleCount; iStep += 2) {
      // Add two neighboring values together with the temp sums up to the
      // position where the current iStep has its first binary 0.
      double walkingSum = *arrayPtr++;
      walkingSum += *arrayPtr++;

      size_t step = 2;
      double* tmpSum = &tmpSums[1];
      while(iStep & step) {
        walkingSum += *tmpSum++;
        step <<= 1;
      }
      // Store the current sum at the position of the subsequent binary 1
      *tmpSum = walkingSum;

      //volatile register dummy = *arrayPtr;
      //_mm_prefetch(arrayPtr, _MM_HINT_T0);
    }
  }

  // If the count is odd, store the remaining value at position 0.
  if(sampleCount & 1) {
    tmpSums[0] = array[sampleCount - 1];
  }

  double sum = 0.;
  // Finally, go though all temp sums and add those to the final sum where
  // sampleCount has a binary 1
  for(size_t i = 0; i < 8 * sizeof(size_t); ++i) {
    if(sampleCount & ((size_t)1 << i)) {
      sum += tmpSums[i];
    }
  }
  return sum;
}



NA_DEF float naIntegratef(
  size_t sampleCount,
  IntegrateFuncf eval,
  const void* obj,
  float min,
  float max)
{
  // This is a highly efficient and accurate integration method developed by
  // Tobias Stamm. It might even outperform a simple for loop by using the
  // memory cache more efficiently but overall is much more accurate for
  // well-behaving value sequences as it combines neighboring values and hence
  // loses much fewer floating point accuracy when summing these up.
  
  float tmpSums[8 * sizeof(size_t)];  // 8 denotes bits per Byte
  memset(tmpSums, 0, sizeof(float) * (8 * sizeof(size_t))); // nullify.

  if(sampleCount > 1) {
    float stepSize = (max - min) / (sampleCount - 1);
    
    for(size_t iStep = 0; iStep < sampleCount; iStep += 2) {

      // Compute two neighboring values.
      float x1 = min + stepSize * iStep;
      float value1 = eval(obj, x1);
      float x2 = min + stepSize * (iStep + 1);
      float value2 = eval(obj, x2);
      
      // Add these values together with the temp sums up to the position where
      // the current iStep has its first binary 0.
      float walkingSum = value1 + value2;
      char p = 1; // the current tmpSum as well as the current bit of iStep
      size_t step = 2; // = 2^p
      while(iStep & step) {
        walkingSum += tmpSums[p];
        p++;
        step <<= 1;
      }
      // Store the current sum at the position of the subsequent binary 1
      tmpSums[p] = walkingSum;
    }
  }
  
  // If the count is odd, store the remaining value at position 0.
  if(sampleCount & 1) {
    tmpSums[0] = eval(obj, max);
  }
  
  float sum = 0.f;
  // Finally, go though all temp sums and add those to the final sum where
  // sampleCount has a binary 1
  for(size_t i = 0; i < 8 * sizeof(size_t); ++i) {
    if(sampleCount & ((size_t)1 << i)) {
      sum += tmpSums[i];
    }
  }
  return sum;
}



NA_DEF double naIntegrate(
  size_t sampleCount,
  IntegrateFuncd eval,
  const void* obj,
  double min,
  double max)
{
  // This is a highly efficient and accurate integration method developed by
  // Tobias Stamm. It might even outperform a simple for loop by using the
  // memory cache more efficiently but overall is much more accurate for
  // well-behaving value sequences as it combines neighboring values and hence
  // loses much fewer floating point accuracy when summing these up.
  
  double tmpSums[8 * sizeof(size_t)];  // 8 denotes bits per Byte
  memset(tmpSums, 0, sizeof(double) * (8 * sizeof(size_t))); // nullify.

  if(sampleCount > 1) {
    double stepSize = (max - min) / (sampleCount - 1);
    
    for(size_t iStep = 0; iStep < sampleCount; iStep += 2) {

      // Compute two neighboring values.
      double x1 = min + stepSize * iStep;
      double value1 = eval(obj, x1);
      double x2 = min + stepSize * (iStep + 1);
      double value2 = eval(obj, x2);
      
      // Add these values together with the temp sums up to the position where
      // the current iStep has its first binary 0.
      double walkingSum = value1 + value2;
      char p = 1; // the current tmpSum as well as the current bit of iStep
      size_t step = 2; // = 2^p
      while(iStep & step) {
        walkingSum += tmpSums[p];
        p++;
        step <<= 1;
      }
      // Store the current sum at the position of the subsequent binary 1
      tmpSums[p] = walkingSum;
    }
  }
  
  // If the count is odd, store the remaining value at position 0.
  if(sampleCount & 1) {
    tmpSums[0] = eval(obj, max);
  }
  
  double sum = 0.f;
  // Finally, go though all temp sums and add those to the final sum where
  // sampleCount has a binary 1
  for(size_t i = 0; i < 8 * sizeof(size_t); ++i) {
    if(sampleCount & ((size_t)1 << i)) {
      sum += tmpSums[i];
    }
  }
  return sum;
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
