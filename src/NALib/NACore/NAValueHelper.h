
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_HELPER_INCLUDED
#define NA_HELPER_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// This file contains many helper functions concerning values which are used
// all over NALib.
//
// They have such a general meaning that it is hard to find a distinct place
// where they belong.
//
// They are very small functions and all are inlined. You may use the
// functions for your own code if you like but they are mostly written here
// for completeness and are marked as helper functions. They are quite specific
// to NALib and may not be of great use elsewhere.
//
// This file is located at the very bottom and does not require any other
// file to be included other than the NASystem file containing the base type
// definitions.


// Converts an integer denoting an end or max to its counterpart. Also does
// some checks if the values over- or underflow.
NA_HIAPI int32  naEndToMaxi32 (int32 end);
NA_HIAPI int64  naEndToMaxi64 (int64 end);
NA_HIAPI NAInt  naEndToMaxi   (NAInt end);
NA_HIAPI int32  naMaxToEndi32 (int32 max);
NA_HIAPI int64  naMaxToEndi64 (int64 max);
NA_HIAPI NAInt  naMaxToEndi   (NAInt max);
NA_HIAPI double naOriginAndLengthToEnd  (double  origin,  double length);
NA_HIAPI float  naOriginAndLengthToEndf (float   origin,  float  length);
NA_HIAPI NAInt  naOriginAndLengthToEndi (NAInt   origin,  NAInt  length);
NA_HIAPI NAInt  naOriginAndLengthToMaxi (NAInt   origin,  NAInt  length);
NA_HIAPI double naStartAndEndToLength   (double  start,   double end);
NA_HIAPI float  naStartAndEndToLengthf  (float   start,   float  end);
NA_HIAPI NAInt  naStartAndEndToLengthi  (NAInt   start,   NAInt  end);
NA_HIAPI NAInt  naMinAndMaxToLengthi    (NAInt   min,     NAInt  max);
    

// The following functions are mostly used in other datastructures such as
// NARect and NARange. They define the default semantics against which values
// are checked in NALib when debugging. For example, a size is defined to be
// not useful when negative.

// VALID means: Anything but NaN. Integer values therefore are always valid.
NA_HIAPI NABool naIsPosValueValid(double a);
NA_HIAPI NABool naIsPosValueValidf(float a);
NA_HIAPI NABool naIsPosValueValidi(NAInt a);
NA_HIAPI NABool naIsSizeValueValid(double a);
NA_HIAPI NABool naIsSizeValueValidf(float a);
NA_HIAPI NABool naIsSizeValueValidi(NAInt a);

// EMPTY means: Precisely Zero.
NA_HIAPI NABool naIsSizeValueEmpty(double a);
NA_HIAPI NABool naIsSizeValueEmptyf(float a);
NA_HIAPI NABool naIsSizeValueEmptyi(NAInt a);

// NEGATIVE means: Smaller than Zero.
//
// Note from the author: These functions test for Negativity, not Positivity.
// Using a negative word in a function name is usually not good practice as it
// tends to lead to double negatives when reading the source code. There are
// situations though when using a negative word is acceptable or even better
// than its positive counterpart. Like here for example. Or the NDEBUG macro.
// Or the "End of file" marker EOF for example. But such situations are rare.
// Try to use positive words whenever possible! For example, test for "Inside"
// rather than "Outside".
NA_HIAPI NABool naIsSizeValueNegative(double a);
NA_HIAPI NABool naIsSizeValueNegativef(float a);
NA_HIAPI NABool naIsSizeValueNegativei(NAInt a);

// USEFUL means: Positions must be valid. Sizes must be valid, not empty and
// not negative.
NA_HIAPI NABool naIsPosValueUseful(double a);
NA_HIAPI NABool naIsPosValueUsefulf(float a);
NA_HIAPI NABool naIsPosValueUsefuli(NAInt a);
NA_HIAPI NABool naIsSizeValueUseful(double a);
NA_HIAPI NABool naIsSizeValueUsefulf(float a);
NA_HIAPI NABool naIsSizeValueUsefuli(NAInt a);


// This function alters the given pos and size such that size will become
// positive while retaining the depicted range. For example with doubles:
// pos = 3. and size = -2. will become pos = 1. and size = 2. The resulting
// range starts at 1. and ends at 1. + 2. = 3.
// Note: When using integers, the end must be computed with -1. Therefore
// the result differs:
// pos = 3 and size = -2 will become pos = 2 and size = 2. The resulting
// range starts at 2 and ends at (2 + 2) - 1 = 3
// If size already was positive, nothing will be changed.
NA_HIAPI void naMakePositive      (double* NA_RESTRICT pos,
                                   double* NA_RESTRICT size);
NA_HIAPI void naMakePositivei     (NAInt*  NA_RESTRICT pos,
                                   NAInt*  NA_RESTRICT size);


// This function returns a pair of positive integers (positivepos,positivesize)
// out of a possibly negative pair (pos,size) such that the resulting range
// will be fully contained in a range given by [0, containingsize-1].
// Negative values are treated as follows and in the following order:
// - if pos is negative, it denotes the number of units from the end.
//   For integers, pos -1 therefore corresponds to size-1.
// - If the size is now 0, the function will return.
// - if size is negative, it denotes the size up and including to the given
//   number of units from the end, meaning -1 denotes the last unit.
// - if the pos and size combination somehow leads to a resulting size of
//   exactly 0, the resulting range will be empty without a warning emitted.
// - If the pos and size combination somehow leads to an over- or underflow,
//   a warning will be emitted if NDEBUG is defined. The resulting range will
//   be empty.
NA_HIAPI void naMakeIntegerRangePositiveInSize(
                                    NAUInt* NA_RESTRICT positivepos,
                                    NAUInt* NA_RESTRICT positivesize,
                                    NAInt               pos,
                                    NAInt               size,
                                    NAUInt              containingsize);
// This function is declared as a helper function. It is used by some core
// implementations of NAByteArray. It seems much more useful in this file
// though.











NA_HIDEF int32 naEndToMaxi32(int32 end){
  #ifndef NDEBUG
    if(end == NA_INT32_MIN)
      naError("naEndToMaxi32", "Integer underflow");
  #endif
  return end - 1;
}
NA_HIDEF int64 naEndToMaxi64(int64 end){
  #ifndef NDEBUG
    if(end == NA_INT64_MIN)
      naError("naEndToMaxi64", "Integer underflow");
  #endif
  return end - 1LL;
}
NA_HIDEF NAInt naEndToMaxi(NAInt end){
  #if NA_SYSTEM_ADDRESS_BITS == 32
    return naEndToMaxi32(end);
  #else
    return naEndToMaxi64(end);
  #endif
}
NA_HIDEF int32 naMaxToEndi32(int32 max){
  #ifndef NDEBUG
    if(max == NA_INT32_MAX)
      naError("naMaxToEndi32", "Integer overflow");
  #endif
  return max + 1;
}
NA_HIDEF int64 naMaxToEndi64(int64 max){
  #ifndef NDEBUG
    if(max == NA_INT64_MAX)
      naError("naMaxToEndi64", "Integer overflow");
  #endif
  return max + 1LL;
}
NA_HIDEF NAInt naMaxToEndi(NAInt max){
  #if NA_SYSTEM_ADDRESS_BITS == 32
    return naMaxToEndi32(max);
  #else
    return naMaxToEndi64(max);
  #endif
}



NA_HIDEF double naOriginAndLengthToEnd(double origin, double length){
  double result = origin + length;
  #ifndef NDEBUG
    if(!naIsPosValueValid(result)){
      naError("naOriginAndLengthToEnd", "result invalid");
    }
  #endif
  return result;
}
NA_HIDEF float naOriginAndLengthToEndf(float origin, float length){
  float result = origin + length;
  #ifndef NDEBUG
    if(!naIsPosValueValidf(result)){
      naError("naOriginAndLengthToEndf", "result invalid");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naOriginAndLengthToEndi(NAInt origin, NAInt length){
  NAInt result = origin + length;
  #ifndef NDEBUG
    if(length > 0){
      if(result < origin)
        naError("naOriginAndLengthToEndi", "Integer overflow");
    }else{
      if(result > origin)
        naError("naOriginAndLengthToEndi", "Integer underflow");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naOriginAndLengthToMaxi(NAInt origin, NAInt length){
  NAInt result = naEndToMaxi(origin + length);
  #ifndef NDEBUG
    if(length > 0){
      if(result < origin)
        naError("naOriginAndLengthToMaxi", "Integer overflow");
    }else{
      if(result > origin)
        naError("naOriginAndLengthToMaxi", "Integer underflow");
    }
  #endif
  return result;
}


NA_HIDEF double naStartAndEndToLength(double min, double end){
  return end - min;
}
NA_HIDEF float naStartAndEndToLengthf(float min, float end){
  return end - min;
}
NA_HIDEF NAInt naStartAndEndToLengthi(NAInt min, NAInt end){
  return end - min;
}
NA_HIDEF NAInt naMinAndMaxToLengthi(NAInt min, NAInt max){
  return naMaxToEndi(max) - min;
}



NA_HIDEF NABool naIsPosValueValid(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsPosValueValidf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsPosValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_HIDEF NABool naIsSizeValueValid(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsSizeValueValidf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsSizeValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_HIDEF NABool naIsSizeValueEmpty(double a){
  return (a == 0.);
}
NA_HIDEF NABool naIsSizeValueEmptyf(float a){
  return (a == 0.f);
}
NA_HIDEF NABool naIsSizeValueEmptyi(NAInt a){
  return (a == 0);
}


NA_HIDEF NABool naIsSizeValueNegative(double a){
  return (a < 0.);
}
NA_HIDEF NABool naIsSizeValueNegativef(float a){
  return (a < 0.f);
}
NA_HIDEF NABool naIsSizeValueNegativei(NAInt a){
  return (a < 0);
}


NA_HIDEF NABool naIsPosValueUseful(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsPosValueUsefulf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsPosValueUsefuli(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}


NA_HIDEF NABool naIsSizeValueUseful(double a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_HIDEF NABool naIsSizeValueUsefulf(float a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_HIDEF NABool naIsSizeValueUsefuli(NAInt a){
  return (a > 0);
}



NA_HIDEF void naMakePositive(double* NA_RESTRICT pos, double* NA_RESTRICT size){
  if(*size < 0.){
    *pos = *pos + *size;
    *size = -*size;
  }
}


NA_HIDEF void naMakePositivei(NAInt* NA_RESTRICT pos, NAInt* NA_RESTRICT size){
  if(*size < 0){
    *pos = *pos + *size + 1; // important + 1 !
    *size = -*size;
  }
}




NA_HIDEF void naMakeIntegerRangePositiveInSize(NAUInt* NA_RESTRICT positivepos, NAUInt* NA_RESTRICT positivesize, NAInt pos, NAInt size, NAUInt containingsize){
  // First, we ensure that pos is withing the containing range. After that
  // we will look at the size parameter.
  NAInt remainingsize = containingsize - pos;
  if(pos < 0){
    pos += containingsize;
    remainingsize -= containingsize;
  }
  if(remainingsize < 0){
    #ifndef NDEBUG
      naError("naMakeIntegerRangePositiveInSize", "Invalid pos leads to range overflow. Correcting to empty range.");
    #endif
    *positivepos = 0;
    *positivesize = 0;
  }else if((NAUInt)remainingsize > containingsize){
    #ifndef NDEBUG
      naError("naMakeIntegerRangePositiveInSize", "Invalid pos leads to range underflow. Correcting to empty range.");
    #endif
    *positivepos = 0;
    *positivesize = 0;
  }else{
    *positivepos = pos;
    // The pos is positive. Now, adjust the size.
    if(size < 0){ // negative size parameter
      size = remainingsize + size + 1;  // Important + 1 !
      if(size < 0){
        // When the resulting size is smaller than 0, underflow.
        #ifndef NDEBUG
          naError("naMakeIntegerRangePositiveInSize", "Invalid size leads to range underflow. Correcting to empty range.");
        #endif
        *positivepos = 0;
        *positivesize = 0;
      }else{
        *positivesize = size;
      }
    }else{ // positive or 0 size parameter
      if(size > remainingsize){
        // When the desired size is bigger than the size available, overflow.
        #ifndef NDEBUG
          naError("naMakeIntegerRangePositiveInSize", "Invalid size leads to range overflow. Correcting to empty range.");
        #endif
        *positivepos = 0;
        *positivesize = 0;
      }else{
        *positivesize = size;
      }
    }
  }
}





#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_HELPER_INCLUDED

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
