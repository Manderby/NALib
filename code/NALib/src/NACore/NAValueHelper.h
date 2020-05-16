
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
// file to be included other than the NABase.h file containing the base type
// definitions.


// Tests a bit mask for the specified flag and returns NA_TRUE if it is set.
NA_IAPI NABool naGetFlag(int flags, int flag);
NA_IAPI NABool naGetFlagi(NAInt flags, NAInt flag);
NA_IAPI NABool naGetFlagu(NAUInt flags, NAUInt flag);
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
NA_HIAPI int32   naMakeMaxWithEndi32          (int32 end);
NA_HIAPI NAi64 naMakeMaxWithEndi64          (NAi64 end);
NA_HIAPI NAInt   naMakeMaxWithEndi            (NAInt end);
NA_HIAPI int32   naMakeEndWithMaxi32          (int32 max);
NA_HIAPI NAi64 naMakeEndWithMaxi64          (NAi64 max);
NA_HIAPI NAInt   naMakeEndWithMaxi            (NAInt max);
NA_HIAPI double  naMakeEndWithStartAndLength  (double start, double length);
NA_HIAPI float   naMakeEndWithStartAndLengthf (float  start, float  length);
NA_HIAPI NAInt   naMakeEndWithStartAndLengthi (NAInt  start, NAInt  length);
NA_HIAPI NAInt   naMakeMaxWithMinAndLengthi   (NAInt  min,   NAInt  length);
NA_HIAPI double  naMakeLengthWithStartAndEnd  (double start, double end);
NA_HIAPI float   naMakeLengthWithStartAndEndf (float  start, float  end);
NA_HIAPI NAInt   naMakeLengthWithStartAndEndi (NAInt  start, NAInt  end);
NA_HIAPI NAInt   naMakeLengthWithMinAndMaxi   (NAInt  min,   NAInt  max);
                 
NA_HIAPI NAInt   naMakeIntWithIntegerFloat    (float x);
NA_HIAPI NAInt   naMakeIntWithIntegerDouble   (double x);


// The following functions are mostly used in other datastructures such as
// NARect and NARange. They define the default semantics against which values
// are checked in NALib when debugging. For example, a length is defined to be
// not useful when negative.

// VALID means: Anything but NaN. Integer values therefore are always valid.
NA_HIAPI NABool naIsOffsetValueValid  (double a);
NA_HIAPI NABool naIsOffsetValueValidf (float  a);
NA_HIAPI NABool naIsOffsetValueValidi (NAInt  a);
NA_HIAPI NABool naIsOffsetValueValidu (NAUInt a);
NA_HIAPI NABool naIsLengthValueValid  (double a);
NA_HIAPI NABool naIsLengthValueValidf (float  a);
NA_HIAPI NABool naIsLengthValueValidi (NAInt  a);
NA_HIAPI NABool naIsLengthValueValidu (NAUInt a);

// EMPTY means: Precisely Zero.
NA_HIAPI NABool naIsLengthValueEmpty  (double a);
NA_HIAPI NABool naIsLengthValueEmptyf (float  a);
NA_HIAPI NABool naIsLengthValueEmptyi (NAInt  a);
NA_HIAPI NABool naIsLengthValueEmptyu (NAUInt a);

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
NA_HIAPI NABool naIsLengthValueNegativeu (NAUInt a);

// USEFUL means: Positions must be valid. Lengths must be valid, not empty and
// not negative.
NA_HIAPI NABool naIsOffsetValueUseful  (double a);
NA_HIAPI NABool naIsOffsetValueUsefulf (float  a);
NA_HIAPI NABool naIsOffsetValueUsefuli (NAInt  a);
NA_HIAPI NABool naIsOffsetValueUsefulu (NAUInt a);
NA_HIAPI NABool naIsLengthValueUseful  (double a);
NA_HIAPI NABool naIsLengthValueUsefulf (float  a);
NA_HIAPI NABool naIsLengthValueUsefuli (NAInt  a);
NA_HIAPI NABool naIsLengthValueUsefulu (NAUInt a);


// Returns a value which is the given x floored to a multiple of
// alignlength with an offset. Also works for negative values.
NA_HIAPI NAInt naAlignValuei(NAInt x, NAInt offset, NAInt alignlength);
NA_HIAPI double naAlignValued(double x, double offset, double alignlength);









NA_IDEF NABool naGetFlag(int flags, int flag){
  return ((flags & flag) == flag);
}
NA_IDEF NABool naGetFlagi(NAInt flags, NAInt flag){
  return ((flags & flag) == flag);
}
NA_IDEF NABool naGetFlagu(NAUInt flags, NAUInt flag){
  return ((flags & flag) == flag);
}
NA_IDEF void naSetFlag(int* flags, int flag, NABool set){
  #ifndef NDEBUG
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | (set * flag);
}
NA_IDEF void naSetFlagi(NAInt* flags, NAInt flag, NABool set){
  #ifndef NDEBUG
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | (set * flag);
}
NA_IDEF void naSetFlagu(NAUInt* flags, NAUInt flag, NABool set){
  #ifndef NDEBUG
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | ((NAUInt)set * flag);
}
NA_IDEF NABool naToggleFlag(int* flags, int flag){
  (*flags) ^= flag;
  return naGetFlag(*flags, flag);
}
NA_IDEF NABool naToggleFlagi(NAInt* flags, NAInt flag){
  (*flags) ^= flag;
  return naGetFlagi(*flags, flag);
}
NA_IDEF NABool naToggleFlagu(NAUInt* flags, NAUInt flag){
  (*flags) ^= flag;
  return naGetFlagu(*flags, flag);
}





NA_HIDEF int32 naMakeMaxWithEndi32(int32 end){
  #ifndef NDEBUG
    if(end == NA_MIN_i32)
      naError("Integer underflow");
  #endif
  return end - NA_ONE_i32;
}
NA_HIDEF NAi64 naMakeMaxWithEndi64(NAi64 end){
  #ifndef NDEBUG
    if(naEquali64(end, NA_MIN_i64))
      naError("Integer underflow");
  #endif
  return naSubi64(end, NA_ONE_i64);
}
NA_HIDEF NAInt naMakeMaxWithEndi(NAInt end){
  #if NA_TYPE_NAINT_BITS == 32
    return naMakeMaxWithEndi32(end);
  #elif NA_TYPE_NAINT_BITS == 64
    return naMakeMaxWithEndi64(end);
  #endif
}
NA_HIDEF int32 naMakeEndWithMaxi32(int32 max){
  #ifndef NDEBUG
    if(max == NA_MAX_i32)
      naError("Integer overflow");
  #endif
  return max + NA_ONE_i32;
}
NA_HIDEF NAi64 naMakeEndWithMaxi64(NAi64 max){
  #ifndef NDEBUG
    if(naEquali64(max, NA_MAX_i64))
      naError("Integer overflow");
  #endif
  return naAddi64(max, NA_ONE_i64);
}
NA_HIDEF NAInt naMakeEndWithMaxi(NAInt max){
  #if NA_TYPE_NAINT_BITS == 32
    return naMakeEndWithMaxi32(max);
  #elif NA_TYPE_NAINT_BITS == 64
    return naMakeEndWithMaxi64(max);
  #endif
}



NA_HIDEF double naMakeEndWithStartAndLength(double start, double length){
  double result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_HIDEF float naMakeEndWithStartAndLengthf(float start, float length){
  float result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidf(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naMakeEndWithStartAndLengthi(NAInt start, NAInt length){
  NAInt result = start + length;
  #ifndef NDEBUG
    if(length > 0){
      if(result < start)
        naError("Integer overflow");
    }else{
      if(result > start)
        naError("Integer underflow");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naMakeMaxWithMinAndLengthi(NAInt min, NAInt length){
  NAInt result = naMakeMaxWithEndi(min + length);
  #ifndef NDEBUG
    if(length > 0){
      if(result < min)
        naError("Integer overflow");
    }else{
      if(result > min)
        naError("Integer underflow");
    }
  #endif
  return result;
}


NA_HIDEF double naMakeLengthWithStartAndEnd(double start, double end){
  return end - start;
}
NA_HIDEF float naMakeLengthWithStartAndEndf(float start, float end){
  return end - start;
}
NA_HIDEF NAInt naMakeLengthWithStartAndEndi(NAInt start, NAInt end){
  return end - start;
}
NA_HIDEF NAInt naMakeLengthWithMinAndMaxi(NAInt min, NAInt max){
  return naMakeEndWithMaxi(max) - min;
}



#include "../NAMathOperators.h"

NA_HIDEF NAInt naMakeIntWithIntegerFloat(float x){
  #ifndef NDEBUG
    if(naRoundf(x) != x)
      naError("Given float is not an integer number");
  #endif
  return (NAInt)x;
}
NA_HIDEF NAInt naMakeIntWithIntegerDouble(double x){
  #ifndef NDEBUG
    if(naRound(x) != x)
      naError("Given double is not an integer number");
  #endif
  return (NAInt)x;
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
NA_HIDEF NABool naIsOffsetValueValidu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
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
NA_HIDEF NABool naIsLengthValueValidu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
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
NA_HIDEF NABool naIsLengthValueEmptyu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #endif
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
NA_HIDEF NABool naIsLengthValueNegativeu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_FALSE;
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
NA_HIDEF NABool naIsOffsetValueUsefulu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
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
NA_HIDEF NABool naIsLengthValueUsefulu(NAUInt a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #endif
  return (a > 0);
}



NA_HIDEF NAInt naAlignValuei(NAInt x, NAInt offset, NAInt alignlength){
  NAInt shiftx;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(alignlength))
      naError("Invalid size leads to range overflow. Correcting to empty range.");
  #endif
  shiftx = x - offset;
  if(shiftx < 0){
    return (((NAInt)((shiftx + 1) / alignlength) - 1) * alignlength) + offset;
  }else{
    return (((NAInt)((shiftx + 0) / alignlength) - 0) * alignlength) + offset;
  }
}
NA_HIDEF double naAlignValued(double x, double offset, double alignlength){
  double shiftx;
  #ifndef NDEBUG
    if(!naIsLengthValueUseful(alignlength))
      naError("Invalid size leads to range overflow. Correcting to empty range.");
  #endif
  shiftx = x - offset;
  return ((naFloor((shiftx) / alignlength)) * alignlength) + offset;
}



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_HELPER_INCLUDED



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
