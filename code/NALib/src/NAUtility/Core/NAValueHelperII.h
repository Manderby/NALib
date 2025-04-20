
// This file contains inline implementations of the file NAValueHelper.h
// Do not include this file directly! It will automatically be included when
// including "NAValueHelper.h"



NA_IDEF NABool naGetFlagu32(uint32 flags, uint32 mask) {
  return ((flags & mask) == mask);
}
NA_IDEF NABool naGetFlagu32Or(uint32 flags, uint32 mask) {
  return ((flags & mask) != 0);
}
NA_IDEF void naSetFlagu32(uint32* flags, uint32 mask, NABool set) {
  #if NA_DEBUG
    if(flags == NA_NULL)
      naCrash("flags is nullptr");
    if(set != NA_FALSE && set != NA_TRUE)
      naError("Boolean value invalid");
  #endif
  *flags = (*flags & ~mask) | ((uint32)set * mask);
}
NA_IDEF NABool naToggleFlagu32(uint32* flags, uint32 mask) {
  #if NA_DEBUG
    if(flags == NA_NULL)
      naCrash("flags is nullptr");
  #endif
  (*flags) ^= mask;
  return naGetFlagu32(*flags, mask);
}



NA_IDEF int32 naMakeMaxWithEndi32(int32 end) {
  #if NA_DEBUG
    if(end == NA_MIN_i32)
      naError("Integer underflow");
  #endif
  return end - NA_ONE_i32;
}
NA_IDEF int64 naMakeMaxWithEndi64(int64 end) {
  #if NA_DEBUG
    if(naEquali64(end, NA_MIN_i64))
      naError("Integer underflow");
  #endif
  return naSubi64(end, NA_ONE_i64);
}
NA_IDEF size_t naMakeMaxWithEnds(size_t end) {
  #if NA_DEBUG
    if(end == NA_ZERO_s)
      naError("Integer underflow");
  #endif
  return end - NA_ONE_s;
}



NA_IDEF int32 naMakeEndWithMaxi32(int32 max) {
  #if NA_DEBUG
    if(max == NA_MAX_i32)
      naError("Integer overflow");
  #endif
  return max + NA_ONE_i32;
}
NA_IDEF int64 naMakeEndWithMaxi64(int64 max) {
  #if NA_DEBUG
    if(naEquali64(max, NA_MAX_i64))
      naError("Integer overflow");
  #endif
  return naAddi64(max, NA_ONE_i64);
}
NA_IDEF size_t naMakeEndWithMaxs(size_t max) {
  #if NA_DEBUG
    if(max == NA_MAX_s)
      naError("Integer overflow");
  #endif
  return max + 1;
}



NA_IDEF double naMakeEndWithStartAndLength(double start, double length) {
  #if NA_DEBUG
    if(length < 0.) {
      naError("length is negative");
    }
  #endif
  double result = start + length;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(result)) {
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF float naMakeEndWithStartAndLengthf(float start, float length) {
  #if NA_DEBUG
    if(length < 0.f) {
      naError("length is negative");
    }
  #endif
  float result = start + length;
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(result)) {
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF int32 naMakeEndWithStartAndLengthi32(int32 start, int32 length) {
  #if NA_DEBUG
    if(length < 0) {
      naError("length is negative");
    }
  #endif
  int32 result = start + length;
  #if NA_DEBUG
    if(length >= 0 && result < start)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF int64 naMakeEndWithStartAndLengthi64(int64 start, int64 length) {
  #if NA_DEBUG
    if(naSmalleri64(length, NA_ZERO_i64)) {
      naError("length is negative");
    }
  #endif
  int64 result = naAddi64(start, length);
  #if NA_DEBUG
    if(naGreaterEquali64(length, NA_ZERO_i64) && naSmalleri64(result, start))
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeEndWithStartAndLengths(size_t start, size_t length) {
  size_t result = start + length;
  #if NA_DEBUG
    if(result < start)
      naError("Integer overflow");
  #endif
  return result;
}



NA_IDEF int32 naMakeMaxWithMinAndLengthi32(int32 min, int32 length) {
  #if NA_DEBUG
    if(length < 0) {
      naError("length is negative");
    }
  #endif
  int32 result = naMakeMaxWithEndi32(min + length);
  #if NA_DEBUG
    if(length > 0 && result < min)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF int64 naMakeMaxWithMinAndLengthi64(int64 min, int64 length) {
  #if NA_DEBUG
    if(naSmalleri64(length, NA_ZERO_i64)) {
      naError("length is negative");
    }
  #endif
  int64 result = naMakeMaxWithEndi64(naAddi64(min, length));
  #if NA_DEBUG
    if(naGreateri64(length, NA_ZERO_i64) && naSmalleri64(result, min))
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeMaxWithMinAndLengths(size_t min, size_t length) {
  size_t result = naMakeMaxWithEnds(min + length);
  #if NA_DEBUG
    if(result < min)
      naError("Integer overflow");
  #endif
  return result;
}



NA_IDEF double naMakeLengthWithStartAndEnd(double start, double end) {
  #if NA_DEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  return naMakeLengthWithStartAndEndE(start, end);
}
NA_IDEF double naMakeLengthWithStartAndEndE(double start, double end) {
  double result = end - start;
  #if NA_DEBUG
    if(!naIsLengthValueValid(result)) {
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF float naMakeLengthWithStartAndEndf(float start, float end) {
  #if NA_DEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  return naMakeLengthWithStartAndEndfE(start, end);
}
NA_IDEF float naMakeLengthWithStartAndEndfE(float start, float end) {
  float result = end - start;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(result)) {
      naError("result invalid");
    }
  #endif
  return result;
}
NA_IDEF int32 naMakeLengthWithStartAndEndi32(int32 start, int32 end) {
  #if NA_DEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  return naMakeLengthWithStartAndEndi32E(start, end);
}
NA_IDEF int32 naMakeLengthWithStartAndEndi32E(int32 start, int32 end) {
  int32 result = end - start;
  #if NA_DEBUG
    if(end > start && result < 0)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF int64 naMakeLengthWithStartAndEndi64(int64 start, int64 end) {
  #if NA_DEBUG
    if(naGreateri64(start, end))
      naError("start is greater than end");
  #endif
  return naMakeLengthWithStartAndEndi64E(start, end);
}
NA_IDEF int64 naMakeLengthWithStartAndEndi64E(int64 start, int64 end) {
  int64 result = naSubi64(end, start);
  #if NA_DEBUG
    if(naGreateri64(end, start) && naSmalleri64(result, NA_ZERO_i64))
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeLengthWithStartAndEnds(size_t start, size_t end) {
  #if NA_DEBUG
    if(start > end)
      naError("start is greater than end");
  #endif
  return naMakeLengthWithStartAndEndsE(start, end);
}
NA_IDEF size_t naMakeLengthWithStartAndEndsE(size_t start, size_t end) {
  size_t result = end - start;
  return result;
}



NA_IDEF int32 naMakeLengthWithMinAndMaxi32(int32 min, int32 max) {
  #if NA_DEBUG
    if(max == NA_MAX_i32)
      naError("max being equal to the integer maximum will lead to an overflow");
    if(min > max + 1)
      naError("min is greater than max + 1");
  #endif
  int32 result = naMakeEndWithMaxi32(max) - min;
  #if NA_DEBUG
    if(max > min && result < 0)
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF int64 naMakeLengthWithMinAndMaxi64(int64 min, int64 max) {
  #if NA_DEBUG
    if(naEquali64(max, NA_MAX_i64))
      naError("max being equal to the integer maximum will lead to an overflow");
    if(naGreateri64(min, naAddi64(max, NA_ONE_i64)))
      naError("min is greater than max + 1");
  #endif
  int64 result = naSubi64(naMakeEndWithMaxi64(max), min);
  #if NA_DEBUG
    if(naGreateri64(max, min) && naSmalleri64(result, NA_ZERO_i64))
      naError("Integer overflow");
  #endif
  return result;
}
NA_IDEF size_t naMakeLengthWithMinAndMaxs(size_t min, size_t max) {
  #if NA_DEBUG
    if(max == NA_MAX_s)
      naError("max being equal to the integer maximum will lead to an overflow");
    if(min > max + 1)
      naError("min is greater than max + 1");
  #endif
  size_t result = naMakeEndWithMaxs(max) - min;
  return result;
}



#include "../../NAMath/NAMathOperators.h"

NA_IDEF double naAlignValue(double value, double offset, double alignLength) {
  double shiftValue = value - offset;
  #if NA_DEBUG
    if(!naIsLengthValueUseful(alignLength))
        naError("Length must be > 0.");
    if(!naIsOffsetValueUseful(shiftValue))
        naError("given offset results in a non-useful number.");
  #endif
  return ((naFloor((shiftValue) / alignLength)) * alignLength) + offset;
}
NA_IDEF float naAlignValuef(float value, float offset, float alignLength) {
  float shiftValue = value - offset;
  #if NA_DEBUG
    if(!naIsLengthValueUsefulf(alignLength))
        naError("Length must be > 0.");
    if(!naIsOffsetValueUsefulf(shiftValue))
        naError("given offset results in a non-useful number.");
  #endif
  return ((naFloorf((shiftValue) / alignLength)) * alignLength) + offset;
}
NA_IDEF int32 naAlignValuei32(int32 value, int32 offset, int32 alignLength) {
  int32 shiftValue = value - offset;
  #if NA_DEBUG
    if(!naIsLengthValueUsefuli32(alignLength))
      naError("Length must be > 0");
    if(offset > value && shiftValue > 0)
      naError("Underflow");
    if(value > offset && shiftValue < 0)
      naError("Overflow");
  #endif
  if(shiftValue < 0) {
    return (((int32)((shiftValue + 1) / alignLength) - 1) * alignLength) + offset;
  }else{
    return (((int32)((shiftValue + 0) / alignLength) - 0) * alignLength) + offset;
  }
}
NA_IDEF int64 naAlignValuei64(int64 value, int64 offset, int64 alignLength) {
  int64 shiftValue = naSubi64(value, offset);
  #if NA_DEBUG
    if(!naIsLengthValueUsefuli64(alignLength))
      naError("Length must be > 0");
    if(naGreateri64(offset, value) && naGreateri64(shiftValue, NA_ZERO_i64))
      naError("Underflow");
    if(naGreateri64(value, offset) && naSmalleri64(shiftValue, NA_ZERO_i64))
      naError("Overflow");
  #endif
  if(naSmalleri64(shiftValue, NA_ZERO_i64)) {
    return naAddi64(naMuli64(naSubi64(naDivi64(naAddi64(shiftValue, NA_ONE_i64), alignLength), NA_ONE_i64), alignLength), offset);
  }else{
    return naAddi64(naMuli64(naSubi64(naDivi64(naAddi64(shiftValue, NA_ZERO_i64), alignLength), NA_ZERO_i64), alignLength), offset);
  }
}
NA_IDEF size_t naAlignValues(size_t value, size_t offset, size_t alignLength) {
  size_t shiftValue = value - offset;
  #if NA_DEBUG
    if(!naIsLengthValueUsefuls(alignLength))
      naError("Length must be > 0");
    if(offset > value)
      naError("Underflow");
    if(value > offset && shiftValue < 0)
      naError("Overflow");
  #endif
  return (((size_t)((shiftValue + 0) / alignLength) - 0) * alignLength) + offset;
}



NA_IDEF NABool naIsOffsetValueValid(double a) {
  return !naIsNaN(a);
}
NA_IDEF NABool naIsOffsetValueValidf(float a) {
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsOffsetValueValidi32(int32 a) {
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueValidi64(int64 a) {
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_IDEF NABool naIsOffsetValueValids(size_t a) {
  #if NA_DEBUG
    if(a >> (NA_ADDRESS_BITS - 1) == 1)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}

NA_IDEF NABool naIsLengthValueValid(double a) {
  return !naIsNaN(a);
}
NA_IDEF NABool naIsLengthValueValidf(float a) {
  return !naIsNaNf(a);
}
NA_IDEF NABool naIsLengthValueValidi32(int32 a) {
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsLengthValueValidi64(int64 a) {
  NA_UNUSED(a);
  return NA_TRUE;
}
NA_IDEF NABool naIsLengthValueValids(size_t a) {
  #if NA_DEBUG
    if(a >> (NA_ADDRESS_BITS - 1) == 1)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}

NA_IDEF NABool naIsLengthValueEmpty(double a) {
  return (a == 0.);
}
NA_IDEF NABool naIsLengthValueEmptyf(float a) {
  return (a == 0.f);
}
NA_IDEF NABool naIsLengthValueEmptyi32(int32 a) {
  return (a == 0);
}
NA_IDEF NABool naIsLengthValueEmptyi64(int64 a) {
return naEquali64(a, NA_ZERO_i64);
}
NA_IDEF NABool naIsLengthValueEmptys(size_t a) {
  #if NA_DEBUG
    if(a >> (NA_ADDRESS_BITS - 1) == 1)
      naError("Unsigned integer looks like a negative number");
  #endif
  return (a == 0);
}


NA_IDEF NABool naIsLengthValueNegative(double a) {
  return (a < 0.);
}
NA_IDEF NABool naIsLengthValueNegativef(float a) {
  return (a < 0.f);
}
NA_IDEF NABool naIsLengthValueNegativei32(int32 a) {
  return (a < 0);
}
NA_IDEF NABool naIsLengthValueNegativei64(int64 a) {
  return naSmalleri64(a, NA_ZERO_i64);
}
NA_IDEF NABool naIsLengthValueNegatives(size_t a) {
  #if NA_DEBUG
    if(a >> (NA_ADDRESS_BITS - 1) == 1)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_FALSE;
}


NA_IDEF NABool naIsOffsetValueUseful(double a) {
  return !naIsNaN(a) && !naIsInfinite(a);
}
NA_IDEF NABool naIsOffsetValueUsefulf(float a) {
  return !naIsNaNf(a) && !naIsInfinitef(a);
}
NA_IDEF NABool naIsOffsetValueUsefuli32(int32 a) {
    NA_UNUSED(a);
    return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueUsefuli64(int64 a) {
    NA_UNUSED(a);
    return NA_TRUE;
}
NA_IDEF NABool naIsOffsetValueUsefuls(size_t a) {
  #if NA_DEBUG
    if(a >> (NA_ADDRESS_BITS - 1) == 1)
      naError("Unsigned integer looks like a negative number");
  #else
    NA_UNUSED(a);
  #endif
  return NA_TRUE;
}


NA_IDEF NABool naIsLengthValueUseful(double a) {
  // Note that this test will return NA_FALSE if a is NaN or infinite.
  return (a > 0.) && !naIsInfinite(a);
}
NA_IDEF NABool naIsLengthValueUsefulf(float a) {
  // Note that this test will return NA_FALSE if a is NaN or infinite.
  return (a > 0.) && !naIsInfinitef(a);
}
NA_IDEF NABool naIsLengthValueUsefuli32(int32 a) {
    return (a > 0);
}
NA_IDEF NABool naIsLengthValueUsefuli64(int64 a) {
    return naGreateri64(a, NA_ZERO_i64);
}
NA_IDEF NABool naIsLengthValueUsefuls(size_t a) {
  #if NA_DEBUG
    if(a >> (NA_ADDRESS_BITS - 1) == 1)
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
