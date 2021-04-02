
// This file contains inline implementations of the file NAValueHelper.h
// Do not include this file directly! It will automatically be included when
// including "NAValueHelper.h"



NA_IDEF NABool naGetFlagu32(uint32 flags, uint32 flag){
  return ((flags & flag) == flag);
}
NA_IDEF void naSetFlagu32(uint32* flags, uint32 flag, NABool set){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~flag) | (set * flag);
}
NA_IDEF NABool naToggleFlagu32(uint32* flags, uint32 flag){
  #ifndef NDEBUG
    if(flags == NA_NULL)
      naCrash("flags is Nullpointer");
  #endif
  (*flags) ^= flag;
  return naGetFlagu32(*flags, flag);
}



NA_IDEF int32 naMakeMaxWithEndi32(int32 end){
  #ifndef NDEBUG
    if(end == NA_MIN_i32)
      naError("Integer underflow");
  #endif
  return end - NA_ONE_i32;
}
NA_IDEF NAi64 naMakeMaxWithEndi64(NAi64 end){
  #ifndef NDEBUG
    if(naEquali64(end, NA_MIN_i64))
      naError("Integer underflow");
  #endif
  return naSubi64(end, NA_ONE_i64);
}
NA_IDEF NAInt naMakeMaxWithEndi(NAInt end){
  #if NA_TYPE_NAINT_BITS == 32
    return naMakeMaxWithEndi32(end);
  #elif NA_TYPE_NAINT_BITS == 64
    return naMakeMaxWithEndi64(end);
  #endif
}
NA_IDEF size_t naMakeMaxWithEnds(size_t end){
  #ifndef NDEBUG
    if(end == NA_ZERO_s)
      naError("Integer underflow");
  #endif
  return end - NA_ONE_s;
}



NA_IDEF int32 naMakeEndWithMaxi32(int32 max){
  #ifndef NDEBUG
    if(max == NA_MAX_i32)
      naError("Integer overflow");
  #endif
  return max + NA_ONE_i32;
}
NA_IDEF NAi64 naMakeEndWithMaxi64(NAi64 max){
  #ifndef NDEBUG
    if(naEquali64(max, NA_MAX_i64))
      naError("Integer overflow");
  #endif
  return naAddi64(max, NA_ONE_i64);
}
NA_IDEF NAInt naMakeEndWithMaxi(NAInt max){
  #if NA_TYPE_NAINT_BITS == 32
    return naMakeEndWithMaxi32(max);
  #elif NA_TYPE_NAINT_BITS == 64
    return naMakeEndWithMaxi64(max);
  #endif
}
NA_IDEF size_t naMakeEndWithMaxs(size_t max){
  #ifndef NDEBUG
    if(max == NA_MAX_s)
      naError("Integer overflow");
  #endif
  return naAddi64(max, 1);
}



NA_IDEF double naMakeEndWithStartAndLength(double start, double length){
  #ifndef NDEBUG
    if(length < 0.){
      naError("length is negative");
    }
  #endif
  double result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF float naMakeEndWithStartAndLengthf(float start, float length){
  #ifndef NDEBUG
    if(length < 0.f){
      naError("length is negative");
    }
  #endif
  float result = start + length;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidf(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF NAInt naMakeEndWithStartAndLengthi(NAInt start, NAInt length){
  #ifndef NDEBUG
    if(length < 0){
      naError("length is negative");
    }
  #endif
  NAInt result = start + length;
  #ifndef NDEBUG
    if(length >= 0 && result < start)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeEndWithStartAndLengths(size_t start, size_t length){
  size_t result = start + length;
  #ifndef NDEBUG
    if(result < start)
      naError("Integer overflow");
  #endif
  return result;
}



NA_IDEF NAInt naMakeMaxWithMinAndLengthi(NAInt min, NAInt length){
  #ifndef NDEBUG
    if(length < 0){
      naError("length is negative");
    }
  #endif
  NAInt result = naMakeMaxWithEndi(min + length);
  #ifndef NDEBUG
    if(length > 0 && result < min)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeMaxWithMinAndLengths(size_t min, size_t length){
  size_t result = naMakeMaxWithEnds(min + length);
  #ifndef NDEBUG
    if(result < min)
      naError("Integer overflow");
  #endif
  return result;
}



NA_IDEF double naMakeLengthWithStartAndEnd(double start, double end){
  #ifndef NDEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  double result = end - start;
  #ifndef NDEBUG
    if(!naIsLengthValueValid(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF float naMakeLengthWithStartAndEndf(float start, float end){
  #ifndef NDEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  float result = end - start;
  #ifndef NDEBUG
    if(!naIsLengthValueValidf(result)){
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF NAInt naMakeLengthWithStartAndEndi(NAInt start, NAInt end){
  #ifndef NDEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  NAInt result = end - start;
  #ifndef NDEBUG
    if(end > start && result < 0)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeLengthWithStartAndEnds(size_t start, size_t end){
  #ifndef NDEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  size_t result = end - start;
  return result;
}



NA_IDEF NAInt naMakeLengthWithMinAndMaxi(NAInt min, NAInt max){
  #ifndef NDEBUG
    if(max == NA_MAX_i32)
      naError("max being equal to the integer maximum will lead to an overflow");
    if(min > max + 1)
      naError("min is greater than max + 1");
  #endif
  NAInt result = naMakeEndWithMaxi(max) - min;
  #ifndef NDEBUG
    if(max > min && result < 0)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeLengthWithMinAndMaxs(size_t min, size_t max){
  #ifndef NDEBUG
    if(max == NA_MAX_s)
      naError("max being equal to the integer maximum will lead to an overflow");
    if(min > max + 1)
      naError("min is greater than max + 1");
  #endif
  size_t result = naMakeEndWithMaxs(max) - min;
  return result;
}



#include "../NAMathOperators.h"

NA_IDEF NAInt naAlignValuei(NAInt x, NAInt offset, NAInt alignlength){
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
NA_IDEF double naAlignValued(double x, double offset, double alignlength){
  double shiftx;
  #ifndef NDEBUG
  if(!naIsLengthValueUseful(alignlength))
    naError("Invalid size leads to range overflow. Correcting to empty range.");
  #endif
  shiftx = x - offset;
  return ((naFloor((shiftx) / alignlength)) * alignlength) + offset;
}



NA_IDEF NAInt naMakeIntWithIntegerFloat(float x){
  #ifndef NDEBUG
    if(naRoundf(x) != x)
      naError("Given float is not an integer number");
  #endif
  return (NAInt)x;
}
NA_IDEF NAInt naMakeIntWithIntegerDouble(double x){
  #ifndef NDEBUG
    if(naRound(x) != x)
      naError("Given double is not an integer number");
  #endif
  return (NAInt)x;
}



NA_IDEF NABool naIsOffsetValueValid(double a){
  return !naIsNaN(a);
}
NA_IDEF NABool naIsOffsetValueValidf(float a){
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsOffsetValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueValids(size_t a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}

NA_IDEF NABool naIsLengthValueValid(double a){
  return !naIsNaN(a);
}
NA_IDEF NABool naIsLengthValueValidf(float a){
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsLengthValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsLengthValueValids(size_t a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}

NA_IDEF NABool naIsLengthValueEmpty(double a){
  return (a == 0.);
}
NA_IDEF NABool naIsLengthValueEmptyf(float a){
  return (a == 0.f);
}
NA_IDEF NABool naIsLengthValueEmptyi(NAInt a){
  return (a == 0);
}
NA_IDEF NABool naIsLengthValueEmptys(size_t a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #endif
  return (a == 0);
}


NA_IDEF NABool naIsLengthValueNegative(double a){
  return (a < 0.);
}
NA_IDEF NABool naIsLengthValueNegativef(float a){
  return (a < 0.f);
}
NA_IDEF NABool naIsLengthValueNegativei(NAInt a){
  return (a < 0);
}
NA_IDEF NABool naIsLengthValueNegatives(size_t a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_FALSE;
}


NA_IDEF NABool naIsOffsetValueUseful(double a){
  return !naIsNaN(a);
}
NA_IDEF NABool naIsOffsetValueUsefulf(float a){
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsOffsetValueUsefuli(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueUsefuls(size_t a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}


NA_IDEF NABool naIsLengthValueUseful(double a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_IDEF NABool naIsLengthValueUsefulf(float a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_IDEF NABool naIsLengthValueUsefuli(NAInt a){
  return (a > 0);
}
NA_IDEF NABool naIsLengthValueUsefuls(size_t a){
  #ifndef NDEBUG
    if((NAInt)a < 0)
      naError("Unsigned integer looks like a negative number");
  #endif
  return (a > 0);
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
