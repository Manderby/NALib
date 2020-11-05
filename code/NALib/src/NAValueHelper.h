
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
NA_IAPI int32  naMakeMaxWithEndi32          (int32 end);
NA_IAPI NAi64  naMakeMaxWithEndi64          (NAi64 end);
NA_IAPI NAInt  naMakeMaxWithEndi            (NAInt end);
NA_IAPI int32  naMakeEndWithMaxi32          (int32 max);
NA_IAPI NAi64  naMakeEndWithMaxi64          (NAi64 max);
NA_IAPI NAInt  naMakeEndWithMaxi            (NAInt max);
NA_IAPI double naMakeEndWithStartAndLength  (double start, double length);
NA_IAPI float  naMakeEndWithStartAndLengthf (float  start, float  length);
NA_IAPI NAInt  naMakeEndWithStartAndLengthi (NAInt  start, NAInt  length);
NA_IAPI NAInt  naMakeMaxWithMinAndLengthi   (NAInt  min,   NAInt  length);
NA_IAPI double naMakeLengthWithStartAndEnd  (double start, double end);
NA_IAPI float  naMakeLengthWithStartAndEndf (float  start, float  end);
NA_IAPI NAInt  naMakeLengthWithStartAndEndi (NAInt  start, NAInt  end);
NA_IAPI NAInt  naMakeLengthWithMinAndMaxi   (NAInt  min,   NAInt  max);



// Returns a value which is the given x floored to a multiple of
// alignlength with an offset. Also works for negative values.
NA_IAPI NAInt naAlignValuei(NAInt x, NAInt offset, NAInt alignlength);
NA_IAPI double naAlignValued(double x, double offset, double alignlength);



// Takes a floating point value and converts it to an NAInt. Emits an error
// when the original value was not integral.
NA_IAPI NAInt  naMakeIntWithIntegerFloat    (float x);
NA_IAPI NAInt  naMakeIntWithIntegerDouble   (double x);



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
NA_IAPI NABool naIsOffsetValueValid  (double a);
NA_IAPI NABool naIsOffsetValueValidf (float  a);
NA_IAPI NABool naIsOffsetValueValidi (NAInt  a);
NA_IAPI NABool naIsOffsetValueValidu (NAUInt a);
NA_IAPI NABool naIsLengthValueValid  (double a);
NA_IAPI NABool naIsLengthValueValidf (float  a);
NA_IAPI NABool naIsLengthValueValidi (NAInt  a);
NA_IAPI NABool naIsLengthValueValidu (NAUInt a);

// EMPTY means: Precisely Zero.
NA_IAPI NABool naIsLengthValueEmpty  (double a);
NA_IAPI NABool naIsLengthValueEmptyf (float  a);
NA_IAPI NABool naIsLengthValueEmptyi (NAInt  a);
NA_IAPI NABool naIsLengthValueEmptyu (NAUInt a);

// NEGATIVE means: Smaller than Zero.
NA_IAPI NABool naIsLengthValueNegative  (double a);
NA_IAPI NABool naIsLengthValueNegativef (float  a);
NA_IAPI NABool naIsLengthValueNegativei (NAInt  a);
NA_IAPI NABool naIsLengthValueNegativeu (NAUInt a);

// USEFUL means: Positions must be valid. Lengths must be valid, not empty and
// not negative.
NA_IAPI NABool naIsOffsetValueUseful  (double a);
NA_IAPI NABool naIsOffsetValueUsefulf (float  a);
NA_IAPI NABool naIsOffsetValueUsefuli (NAInt  a);
NA_IAPI NABool naIsOffsetValueUsefulu (NAUInt a);
NA_IAPI NABool naIsLengthValueUseful  (double a);
NA_IAPI NABool naIsLengthValueUsefulf (float  a);
NA_IAPI NABool naIsLengthValueUsefuli (NAInt  a);
NA_IAPI NABool naIsLengthValueUsefulu (NAUInt a);



// Inline implementations are in a separate file:
#include "NACore/NAValueHelperII.h"



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
