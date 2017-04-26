
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


// Tests a bit mask for the specified flag and returns NA_TRUE if it is set.
NA_IAPI NABool naTestFlag(int flags, int flag);
NA_IAPI NABool naTestFlagi(NAInt flags, NAInt flag);
NA_IAPI NABool naTestFlagu(NAUInt flags, NAUInt flag);
// Sets the given flag in the bit mask according to the boolean parameter set.
NA_IAPI void   naSetFlag(int* flags, int flag, NABool set);
NA_IAPI void   naSetFlagi(NAInt* flags, NAInt flag, NABool set);
NA_IAPI void   naSetFlagu(NAUInt* flags, NAUInt flag, NABool set);
// Toggles the specified flag in the bit mask and returns the resulting boolean
// value of that flag.
NA_IAPI NABool naToggleFlag(int* flags, int flag);
NA_IAPI NABool naToggleFlagi(NAInt* flags, NAInt flag);
NA_IAPI NABool naToggleFlagu(NAUInt* flags, NAUInt flag);


// Converts an integer denoting an end or max to its counterpart. Also does
// some checks if the values over- or underflow.
NA_HIAPI int32  naMakeMaxWithEndi32          (int32 end);
NA_HIAPI int64  naMakeMaxWithEndi64          (int64 end);
NA_HIAPI NAInt  naMakeMaxWithEndi            (NAInt end);
NA_HIAPI int32  naMakeEndWithMaxi32          (int32 max);
NA_HIAPI int64  naMakeEndWithMaxi64          (int64 max);
NA_HIAPI NAInt  naMakeEndWithMaxi            (NAInt max);
NA_HIAPI double naMakeEndWithStartAndLength  (double start, double length);
NA_HIAPI float  naMakeEndWithStartAndLengthf (float  start, float  length);
NA_HIAPI NAInt  naMakeEndWithStartAndLengthi (NAInt  start, NAInt  length);
NA_HIAPI NAInt  naMakeMaxWithMinAndLengthi   (NAInt  min,   NAInt  length);
NA_HIAPI double naMakeLengthWithStartAndEnd  (double start, double end);
NA_HIAPI float  naMakeLengthWithStartAndEndf (float  start, float  end);
NA_HIAPI NAInt  naMakeLengthWithStartAndEndi (NAInt  start, NAInt  end);
NA_HIAPI NAInt  naMakeLengthWithMinAndMaxi   (NAInt  min,   NAInt  max);
    

// The following functions are mostly used in other datastructures such as
// NARect and NARange. They define the default semantics against which values
// are checked in NALib when debugging. For example, a length is defined to be
// not useful when negative.

// VALID means: Anything but NaN. Integer values therefore are always valid.
NA_HIAPI NABool naIsOffsetValueValid  (double a);
NA_HIAPI NABool naIsOffsetValueValidf (float  a);
NA_HIAPI NABool naIsOffsetValueValidi (NAInt  a);
NA_HIAPI NABool naIsLengthValueValid  (double a);
NA_HIAPI NABool naIsLengthValueValidf (float  a);
NA_HIAPI NABool naIsLengthValueValidi (NAInt  a);

// EMPTY means: Precisely Zero.
NA_HIAPI NABool naIsLengthValueEmpty  (double a);
NA_HIAPI NABool naIsLengthValueEmptyf (float  a);
NA_HIAPI NABool naIsLengthValueEmptyi (NAInt  a);

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
NA_HIAPI NABool naIsLengthValueNegative  (double a);
NA_HIAPI NABool naIsLengthValueNegativef (float  a);
NA_HIAPI NABool naIsLengthValueNegativei (NAInt  a);

// USEFUL means: Positions must be valid. Lengths must be valid, not empty and
// not negative.
NA_HIAPI NABool naIsOffsetValueUseful  (double a);
NA_HIAPI NABool naIsOffsetValueUsefulf (float  a);
NA_HIAPI NABool naIsOffsetValueUsefuli (NAInt  a);
NA_HIAPI NABool naIsLengthValueUseful  (double a);
NA_HIAPI NABool naIsLengthValueUsefulf (float  a);
NA_HIAPI NABool naIsLengthValueUsefuli (NAInt  a);


// This function alters the given origin and length such that length will become
// positive while retaining the depicted range. For example with doubles:
// origin = 3. and length = -2. will become origin = 1. and length = 2. The resulting
// range starts at 1. and ends at 1. + 2. = 3.
// Note: When using integers, the end must be computed with -1. Therefore
// the result differs:
// origin = 3 and length = -2 will become origin = 2 and length = 2. The resulting
// range starts at 2 and ends at (2 + 2) - 1 = 3
// If length already was positive, nothing will be changed.
NA_HIAPI void naMakePositive      (double* NA_RESTRICT origin,
                                   double* NA_RESTRICT length);
NA_HIAPI void naMakePositivei     (NAInt*  NA_RESTRICT origin,
                                   NAInt*  NA_RESTRICT length);


NA_HIAPI NAInt naMakeIndexPositive(NAInt indx, NAInt length);


// This function returns a pair of positive integers (positiveorigin,positivesize)
// out of a possibly negative pair (pos,length) such that the resulting range
// will be fully contained in a range given by [0, containingsize-1].
// Negative values are treated as follows and in the following order:
// - if origin is negative, it denotes the number of units from the end.
//   For integers, origin -1 therefore corresponds to length-1.
// - If the length is now 0, the function will return.
// - if length is negative, it denotes the length up and including to the given
//   number of units from the end, meaning -1 denotes the last unit.
// - if the origin and length combination somehow leads to a resulting length of
//   exactly 0, the resulting range will be empty without a warning emitted.
// - If the origin and length combination somehow leads to an over- or underflow,
//   a warning will be emitted if NDEBUG is defined. The resulting range will
//   be empty.
NA_HIAPI void naMakeIntegerRangePositiveInLength(
                                    NAInt* NA_RESTRICT  positiveorigin,
                                    NAInt* NA_RESTRICT  positivesize,
                                    NAInt               origin,
                                    NAInt               length,
                                    NAInt               containinglength);
// This function is declared as a helper function. It is used by some core
// implementations of NAByteArray. It seems much more useful in this file
// though.


// Returns an integer which is the given x floored to a multiple of
// alignlength with an offset. Also works for negative values.
NA_HIAPI NAInt naAlignValue(NAInt x, NAInt offset, NAInt alignlength);









NA_IDEF NABool naTestFlag(int flags, int flag){
  return ((flags & flag) == flag);
}
NA_IDEF NABool naTestFlagi(NAInt flags, NAInt flag){
  return ((flags & flag) == flag);
}
NA_IDEF NABool naTestFlagu(NAUInt flags, NAUInt flag){
  return ((flags & flag) == flag);
}
NA_IDEF void naSetFlag(int* flags, int flag, NABool set){
  if(set){(*flags) |= flag;}else{(*flags) &= ~flag;}
}
NA_IDEF void naSetFlagi(NAInt* flags, NAInt flag, NABool set){
  if(set){(*flags) |= flag;}else{(*flags) &= ~flag;}
}
NA_IDEF void naSetFlagu(NAUInt* flags, NAUInt flag, NABool set){
  if(set){(*flags) |= flag;}else{(*flags) &= ~flag;}
}
NA_IDEF NABool naToggleFlag(int* flags, int flag){
  (*flags) ^= flag;
  return naTestFlag(*flags, flag);
}
NA_IDEF NABool naToggleFlagi(NAInt* flags, NAInt flag){
  (*flags) ^= flag;
  return naTestFlagi(*flags, flag);
}
NA_IDEF NABool naToggleFlagu(NAUInt* flags, NAUInt flag){
  (*flags) ^= flag;
  return naTestFlagu(*flags, flag);
}






NA_HIDEF int32 naMakeMaxWithEndi32(int32 end){
  #ifndef NDEBUG
    if(end == NA_INT32_MIN)
      naError("naMakeMaxWithEndi32", "Integer underflow");
  #endif
  return end - 1;
}
NA_HIDEF int64 naMakeMaxWithEndi64(int64 end){
  #ifndef NDEBUG
    if(end == NA_INT64_MIN)
      naError("naMakeMaxWithEndi64", "Integer underflow");
  #endif
  return end - 1LL;
}
NA_HIDEF NAInt naMakeMaxWithEndi(NAInt end){
  #if NA_SYSTEM_ADDRESS_BITS == 32
    return naMakeMaxWithEndi32(end);
  #else
    return naMakeMaxWithEndi64(end);
  #endif
}
NA_HIDEF int32 naMakeEndWithMaxi32(int32 max){
  #ifndef NDEBUG
    if(max == NA_INT32_MAX)
      naError("naMakeEndWithMaxi32", "Integer overflow");
  #endif
  return max + 1;
}
NA_HIDEF int64 naMakeEndWithMaxi64(int64 max){
  #ifndef NDEBUG
    if(max == NA_INT64_MAX)
      naError("naMakeEndWithMaxi64", "Integer overflow");
  #endif
  return max + 1LL;
}
NA_HIDEF NAInt naMakeEndWithMaxi(NAInt max){
  #if NA_SYSTEM_ADDRESS_BITS == 32
    return naMakeEndWithMaxi32(max);
  #else
    return naMakeEndWithMaxi64(max);
  #endif
}



NA_HIDEF double naMakeEndWithStartAndLength(double start, double length){
  double result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(result)){
      naError("naMakeEndWithStartAndLength", "result invalid");
    }
  #endif
  return result;
}
NA_HIDEF float naMakeEndWithStartAndLengthf(float start, float length){
  float result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidf(result)){
      naError("naMakeEndWithStartAndLengthf", "result invalid");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naMakeEndWithStartAndLengthi(NAInt start, NAInt length){
  NAInt result = start + length;
  #ifndef NDEBUG
    if(length > 0){
      if(result < start)
        naError("naMakeEndWithStartAndLengthi", "Integer overflow");
    }else{
      if(result > start)
        naError("naMakeEndWithStartAndLengthi", "Integer underflow");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naMakeMaxWithMinAndLengthi(NAInt min, NAInt length){
  NAInt result = naMakeMaxWithEndi(min + length);
  #ifndef NDEBUG
    if(length > 0){
      if(result < min)
        naError("naMakeMaxWithMinAndLengthi", "Integer overflow");
    }else{
      if(result > min)
        naError("naMakeMaxWithMinAndLengthi", "Integer underflow");
    }
  #endif
  return result;
}


NA_HIDEF double naMakeLengthWithStartAndEnd(double min, double end){
  return end - min;
}
NA_HIDEF float naMakeLengthWithStartAndEndf(float min, float end){
  return end - min;
}
NA_HIDEF NAInt naMakeLengthWithStartAndEndi(NAInt min, NAInt end){
  return end - min;
}
NA_HIDEF NAInt naMakeLengthWithMinAndMaxi(NAInt min, NAInt max){
  return naMakeEndWithMaxi(max) - min;
}



NA_HIDEF NABool naIsOffsetValueValid(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsOffsetValueValidf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsOffsetValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_HIDEF NABool naIsLengthValueValid(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsLengthValueValidf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsLengthValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_HIDEF NABool naIsLengthValueEmpty(double a){
  return (a == 0.);
}
NA_HIDEF NABool naIsLengthValueEmptyf(float a){
  return (a == 0.f);
}
NA_HIDEF NABool naIsLengthValueEmptyi(NAInt a){
  return (a == 0);
}


NA_HIDEF NABool naIsLengthValueNegative(double a){
  return (a < 0.);
}
NA_HIDEF NABool naIsLengthValueNegativef(float a){
  return (a < 0.f);
}
NA_HIDEF NABool naIsLengthValueNegativei(NAInt a){
  return (a < 0);
}


NA_HIDEF NABool naIsOffsetValueUseful(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsOffsetValueUsefulf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsOffsetValueUsefuli(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}


NA_HIDEF NABool naIsLengthValueUseful(double a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_HIDEF NABool naIsLengthValueUsefulf(float a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_HIDEF NABool naIsLengthValueUsefuli(NAInt a){
  return (a > 0);
}



NA_HIDEF void naMakePositive(double* NA_RESTRICT origin, double* NA_RESTRICT length){
  if(*length < 0.){
    *origin = *origin + *length;
    *length = -*length;
  }
}


NA_HIDEF void naMakePositivei(NAInt* NA_RESTRICT origin, NAInt* NA_RESTRICT length){
  if(*length < 0){
    *origin = *origin + *length + 1; // important + 1 !
    *length = -*length;
  }
}



NA_HIDEF NAInt naMakeIndexPositive(NAInt indx, NAInt length){
  #ifndef NDEBUG
    if(length < 0)
      naError("naMakeIndexPositive", "length should not be negative");
  #endif
  if(indx < 0){
    indx += length;
    #ifndef NDEBUG
      if(indx < 0)
        naError("naMakeIndexPositive", "positive index is not positive");
      if(indx >= length)
        naError("naMakeIndexPositive", "positive index overflows length");
    #endif
  }
  return indx;
}



NA_HIDEF void naMakeIntegerRangePositiveInLength(NAInt* NA_RESTRICT positiveorigin, NAInt* NA_RESTRICT positivesize, NAInt origin, NAInt length, NAInt containinglength){
  // First, we ensure that pos is withing the containing range. After that
  // we will look at the length parameter.
  NAInt remainingsize = containinglength - origin;
  if(origin < 0){
    origin += containinglength;
    remainingsize -= containinglength;
  }
  if(remainingsize < 0){
    #ifndef NDEBUG
      naError("naMakeIntegerRangePositiveInLength", "Invalid origin leads to range overflow. Correcting to empty range.");
    #endif
    *positiveorigin = 0;
    *positivesize = 0;
  }else if(remainingsize > containinglength){
    #ifndef NDEBUG
      naError("naMakeIntegerRangePositiveInLength", "Invalid pos leads to range underflow. Correcting to empty range.");
    #endif
    *positiveorigin = 0;
    *positivesize = 0;
  }else{
    *positiveorigin = origin;
    // The pos is positive. Now, adjust the size.
    if(length < 0){ // negative size parameter
      length = remainingsize + length + 1;  // Important + 1 !
      if(length < 0){
        // When the resulting size is smaller than 0, underflow.
        #ifndef NDEBUG
          naError("naMakeIntegerRangePositiveInLength", "Invalid size leads to range underflow. Correcting to empty range.");
        #endif
        *positiveorigin = 0;
        *positivesize = 0;
      }else{
        *positivesize = length;
      }
    }else{ // positive or 0 size parameter
      if(length > remainingsize){
        // When the desired size is bigger than the size available, overflow.
        #ifndef NDEBUG
          naError("naMakeIntegerRangePositiveInLength", "Invalid size leads to range overflow. Correcting to empty range.");
        #endif
        *positiveorigin = 0;
        *positivesize = 0;
      }else{
        *positivesize = length;
      }
    }
  }
}




NA_HIDEF NAInt naAlignValue(NAInt x, NAInt offset, NAInt alignlength){
  NAInt shiftx = x - offset;
  if(shiftx < 0){
    return ((((shiftx + 1) / alignlength) - 1) * alignlength) + offset;
  }else{
    return (((shiftx) / alignlength) * alignlength) + offset;
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
