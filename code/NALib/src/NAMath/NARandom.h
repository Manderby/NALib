
#ifndef NA_RANDOM_INCLUDED
#define NA_RANDOM_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "NABase.h"


// This file contains functions to generate pseudo random numbers.
//
// There exists a default random generator in the standard C libraries. This
// random generator is widely known to perform quite poor. Therefore, on some
// (but not all) systems, there exists a better random generator. But as this
// better generator does not exists on all systems, we use rand() until the
// author decides to implement a better random generator for all systems. That
// one will be called Random instead of Rand.
//
// Note that due to this fact, this file only contains the most necessary
// functions. There will be more to come. And the currently implemented random-
// functions will become obsolete when this happends. All functions depending
// on random variables will change to the better random generator as well.
// A notice will be made in the chagelog.


// Mapping of the standard rand() and srand() functions. The rand function
// returns a pseudo random number in [0, RAND_MAX]. The srand function sets
// a new seed for the pseudo random generator.
NA_IAPI NAInt naRand(void);
NA_IAPI void  naSRand(uint32 seed);

// Seeds the random generator with the provided seed. If seed is 0, a seed will
// be chosen dependent on the system time. Not suitable for security business
// but good enough for pretty much all other uses.
NA_IAPI uint32 naSeedRand(uint32 seed);

// Returns a uniformly disributed random value within interval [0,1)
NA_IAPI double naUniformRandZE(void);
NA_IAPI float  naUniformRandZEf(void);

// Returns a uniformly disributed random value within interval [0,1]
NA_IAPI double naUniformRandZI(void);
NA_IAPI float  naUniformRandZIf(void);



// todo: More to come.



// Inline implementations are in a separate file:
#include "Core/NARandomII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_RANDOM_INCLUDED



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
