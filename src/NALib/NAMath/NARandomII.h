
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NARandom.h
// Do not include this file directly! It will automatically be included when
// including "NARandom.h"



#include <stdlib.h>
#include "NADateTime.h"


NA_IDEF NAInt naRand(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return rand();
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return rand();
  #endif
}


NA_IDEF void naSRand(uint32 seed){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    srand(seed);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    srand(seed);
  #endif
}


NA_IDEF uint32 naSeedRand(uint32 seed){
  if(seed){
    seed = (uint32)seed;
  }else{
    NADateTime dt = naMakeDateTimeNow();
    seed = naCastInt64ToUInt32(dt.sisec) ^ (uint32)dt.nsec;
  }
  naSRand((uint32)seed);
  return seed;
}


#define NA_INV_RAND_MAX  (1.  / RAND_MAX)
#define NA_INV_RAND_MAXf (1.f / RAND_MAX)

NA_IDEF double naUniformRandZE(){
  double rnd;
  do{rnd = (double)naRand();} while(rnd == RAND_MAX);
  return rnd * NA_INV_RAND_MAX;
}
NA_IDEF float naUniformRandZEf(){
  float rnd;
  do{rnd = (float)naRand();} while(rnd == RAND_MAX);
  return rnd * NA_INV_RAND_MAXf;
}
    


NA_IDEF double naUniformRandZI(){
  return (double)naRand() * NA_INV_RAND_MAX;
}
NA_IDEF float naUniformRandZIf(){
  return (float)naRand() * NA_INV_RAND_MAXf;
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
