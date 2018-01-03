
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#ifndef NA_RANDOM_INCLUDED
#define NA_RANDOM_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"


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
#include "NAMath/NARandomII.h"



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_RANDOM_INCLUDED

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
