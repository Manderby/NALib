
#ifndef NA_VALUE_HELPER_INCLUDED
#define NA_VALUE_HELPER_INCLUDED
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
// functions for your own code if you like. Some are quite specific
// to NALib though and may not be of great use elsewhere.



// Tests a bit mask for the specified flag and returns NA_TRUE if it is set.
NA_IAPI NABool naGetFlagu32(uint32 flags, uint32 flag);

// Sets the given flag in the bit mask according to the boolean parameter set.
NA_IAPI void naSetFlagu32(uint32* flags, uint32 flag, NABool set);

// Toggles the specified flag in the bit mask and returns the resulting boolean
// value of that flag.
NA_IAPI NABool naToggleFlagu32(uint32* flags, uint32 flag);



// Converts an integer denoting an end or max to its counterpart. Also does
// some checks if the values over- or underflow.
NA_IAPI int32  naMakeMaxWithEndi32          (int32  end);
NA_IAPI NAi64  naMakeMaxWithEndi64          (int64  end);
NA_IAPI NAInt  naMakeMaxWithEndi            (NAInt  end);
NA_IAPI size_t naMakeMaxWithEnds            (size_t end);

NA_IAPI int32  naMakeEndWithMaxi32          (int32  max);
NA_IAPI NAi64  naMakeEndWithMaxi64          (int64  max);
NA_IAPI NAInt  naMakeEndWithMaxi            (NAInt  max);
NA_IAPI size_t naMakeEndWithMaxs            (size_t max);

NA_IAPI double naMakeEndWithStartAndLength   (double start, double length);
NA_IAPI float  naMakeEndWithStartAndLengthf  (float  start, float  length);
NA_IAPI NAInt  naMakeEndWithStartAndLengthi  (NAInt  start, NAInt  length);
NA_IAPI int32  naMakeEndWithStartAndLengthi32(int32  start, int32  length);
NA_IAPI int64  naMakeEndWithStartAndLengthi64(int64  start, int64  length);
NA_IAPI size_t naMakeEndWithStartAndLengths  (size_t start, size_t length);

NA_IAPI NAInt  naMakeMaxWithMinAndLengthi   (NAInt  min,   NAInt  length);
NA_IAPI int32  naMakeMaxWithMinAndLengthi32 (int32  min,   int32  length);
NA_IAPI int64  naMakeMaxWithMinAndLengthi64 (int64  min,   int64  length);
NA_IAPI size_t naMakeMaxWithMinAndLengths   (size_t min,   size_t length);

// The Variants with E also allow start to be greater than end,
// resulting in a negative size.
NA_IAPI double naMakeLengthWithStartAndEnd    (double start, double end);
NA_IAPI double naMakeLengthWithStartAndEndE   (double start, double end);
NA_IAPI float  naMakeLengthWithStartAndEndf   (float  start, float  end);
NA_IAPI float  naMakeLengthWithStartAndEndfE  (float  start, float  end);
NA_IAPI NAInt  naMakeLengthWithStartAndEndi   (NAInt  start, NAInt  end);
NA_IAPI NAInt  naMakeLengthWithStartAndEndiE  (NAInt  start, NAInt  end);
NA_IAPI int32  naMakeLengthWithStartAndEndi32 (int32  start, int32  end);
NA_IAPI int32  naMakeLengthWithStartAndEndi32E(int32  start, int32  end);
NA_IAPI int64  naMakeLengthWithStartAndEndi64 (int64  start, int64  end);
NA_IAPI int64  naMakeLengthWithStartAndEndi64E(int64  start, int64  end);
NA_IAPI size_t naMakeLengthWithStartAndEnds   (size_t start, size_t end);
NA_IAPI size_t naMakeLengthWithStartAndEndsE  (size_t start, size_t end);

NA_IAPI NAInt  naMakeLengthWithMinAndMaxi   (NAInt  min,   NAInt  max);
NA_IAPI int32  naMakeLengthWithMinAndMaxi32 (int32  min,   int32  max);
NA_IAPI int64  naMakeLengthWithMinAndMaxi64 (int64  min,   int64  max);
NA_IAPI size_t naMakeLengthWithMinAndMaxs   (size_t min,   size_t max);



// Returns a value which is the given x floored to a multiple of
// alignLength with an offset. For example {0, 20, 40, 60, ...} for length 20.
// Also works for negative x and offset. The length must be > 0.
// Example:
// naAlignValuei(5, 0, 20) == 0
// naAlignValuei(65, 0, 20) == 60
// naAlignValuei(-75, 0, 20) == -80
// naAlignValuei(15, 10, 20) == 10
// naAlignValuei(35, 10, 20) == 30
NA_IAPI NAInt naAlignValuei(NAInt value, NAInt offset, NAInt alignLength);
NA_IAPI double naAlignValued(double value, double offset, double alignLength);



// The following functions are mostly used in other datastructures such as
// NARect or NABuffer. They define the default semantics against which values
// are checked in NALib when debugging. For example, a length is defined to be
// not useful when negative.
//
// Note from the author: The isNegative functions test for Negativity, not
// Positivity. Using a negative word in a function name is usually not good
// practice as it tends to lead to double negatives when reading the source
// code. There are situations though when using a negative word is acceptable
// or even better than its positive counterpart. Like here for example. Or
// the NDEBUG macro. Or the "End of file" marker EOF for example. But such
// situations are rare. Try to use positive words whenever possible!
// For example, test for "Inside" rather than "Outside".

// VALID means: Anything but NaN. Integer values therefore are always valid.
NA_IAPI NABool naIsOffsetValueValid   (double a);
NA_IAPI NABool naIsOffsetValueValidf  (float  a);
NA_IAPI NABool naIsOffsetValueValidi  (NAInt  a);
NA_IAPI NABool naIsOffsetValueValidi32(int32  a);
NA_IAPI NABool naIsOffsetValueValidi64(int64  a);
NA_IAPI NABool naIsOffsetValueValids  (size_t a);

NA_IAPI NABool naIsLengthValueValid   (double a);
NA_IAPI NABool naIsLengthValueValidf  (float  a);
NA_IAPI NABool naIsLengthValueValidi  (NAInt  a);
NA_IAPI NABool naIsLengthValueValidi32(int32  a);
NA_IAPI NABool naIsLengthValueValidi64(int64  a);
NA_IAPI NABool naIsLengthValueValids  (size_t a);

// EMPTY means: Precisely Zero.
NA_IAPI NABool naIsLengthValueEmpty   (double a);
NA_IAPI NABool naIsLengthValueEmptyf  (float  a);
NA_IAPI NABool naIsLengthValueEmptyi  (NAInt  a);
NA_IAPI NABool naIsLengthValueEmptyi32(int32  a);
NA_IAPI NABool naIsLengthValueEmptyi64(int64  a);
NA_IAPI NABool naIsLengthValueEmptys  (size_t a);

// NEGATIVE means: Smaller than Zero.
NA_IAPI NABool naIsLengthValueNegative   (double a);
NA_IAPI NABool naIsLengthValueNegativef  (float  a);
NA_IAPI NABool naIsLengthValueNegativei  (NAInt  a);
NA_IAPI NABool naIsLengthValueNegativei32(int32  a);
NA_IAPI NABool naIsLengthValueNegativei64(int64  a);
NA_IAPI NABool naIsLengthValueNegatives  (size_t a);

// USEFUL means: Positions must be deterministic (infinity is not).
// Lengths must be deterministic and greater than zero.
NA_IAPI NABool naIsOffsetValueUseful   (double a);
NA_IAPI NABool naIsOffsetValueUsefulf  (float  a);
NA_IAPI NABool naIsOffsetValueUsefuli  (NAInt  a);
NA_IAPI NABool naIsOffsetValueUsefuli32(int32  a);
NA_IAPI NABool naIsOffsetValueUsefuli64(int64  a);
NA_IAPI NABool naIsOffsetValueUsefuls  (size_t a);

NA_IAPI NABool naIsLengthValueUseful   (double a);
NA_IAPI NABool naIsLengthValueUsefulf  (float  a);
NA_IAPI NABool naIsLengthValueUsefuli  (NAInt  a);
NA_IAPI NABool naIsLengthValueUsefuli32(int32  a);
NA_IAPI NABool naIsLengthValueUsefuli64(int64  a);
NA_IAPI NABool naIsLengthValueUsefuls  (size_t a);



// Inline implementations are in a separate file:
#include "Core/NAValueHelperII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_VALUE_HELPER_INCLUDED



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
