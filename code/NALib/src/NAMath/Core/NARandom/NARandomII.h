
// This file contains inline implementations of the file NARandom.h
// Do not include this file directly! It will automatically be included when
// including "NARandom.h"



#include <stdlib.h>
#include "../../../NAUtility/NADateTime.h"



NA_IDEF int32 naRand() {
  #if NA_OS == NA_OS_WINDOWS
    return (int32)rand();
  #elif NA_IS_POSIX
    return (int32)rand();
  #endif
}


NA_IDEF void naSRand(uint32 seed) {
  #if NA_OS == NA_OS_WINDOWS
    srand(seed);
  #elif NA_IS_POSIX
    srand(seed);
  #endif
}



#define NA_INV_RAND_MAX  (1.  / RAND_MAX)
#define NA_INV_RAND_MAXf (1.f / RAND_MAX)

NA_IDEF double naUniformRandZE() {
  double rnd;
  do {
    rnd = (double)naRand();
  } while(rnd == RAND_MAX);
  return rnd * NA_INV_RAND_MAX;
}
NA_IDEF float naUniformRandZEf() {
  float rnd;
  do {
    rnd = (float)naRand();
  } while(rnd == RAND_MAX);
  return rnd * NA_INV_RAND_MAXf;
}



NA_IDEF double naUniformRandZI() {
  return (double)naRand() * NA_INV_RAND_MAX;
}
NA_IDEF float naUniformRandZIf() {
  return (float)naRand() * NA_INV_RAND_MAXf;
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
