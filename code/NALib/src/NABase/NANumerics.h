
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"



#define NA_VALUE8_SIGN_MASK       ((uint8) (1u << (NA_TYPE8_BITS   - 1u)))
#define NA_VALUE16_SIGN_MASK      ((uint16)(1u << (NA_TYPE16_BITS  - 1u)))
#define NA_VALUE32_SIGN_MASK      ((uint32)(1u << (NA_TYPE32_BITS  - 1u)))
#define NA_VALUE64_SIGN_MASK_HI   NA_VALUE32_SIGN_MASK
#define NA_VALUE64_SIGN_MASK_LO   NA_ZERO_32u
#define NA_VALUE64_SIGN_MASK      naMakeUInt64(NA_VALUE64_SIGN_MASK_HI, NA_VALUE64_SIGN_MASK_LO)
#define NA_VALUE128_SIGN_MASK_HI  NA_VALUE64_SIGN_MASK
#define NA_VALUE128_SIGN_MASK_LO  NA_ZERO_64u
#define NA_VALUE128_SIGN_MASK     naMakeUInt128(NA_VALUE128_SIGN_MASK_HI, NA_VALUE128_SIGN_MASK_LO)
#define NA_VALUE256_SIGN_MASK_HI  NA_VALUE128_SIGN_MASK
#define NA_VALUE256_SIGN_MASK_LO  NA_ZERO_128u
#define NA_VALUE256_SIGN_MASK     naMakeUInt256(NA_VALUE256_SIGN_MASK_HI, NA_VALUE256_SIGN_MASK_LO)



// Returns either 0 or -1 in two complement form but stored as an uint
// depending on whether the parameter is positive or negative.
NA_IAPI uint8     naGetSignum8  (int8  i);
NA_IAPI uint16    naGetSignum16 (int16 i);
NA_IAPI uint32    naGetSignum32 (int32 i);
NA_IAPI NAUInt64  naGetSignum64 (NAInt64 i);
NA_IAPI NAUInt128 naGetSignum128(NAInt128 i);
NA_IAPI NAUInt256 naGetSignum256(NAInt256 i);

// Returns either 1 or -1. The value 0 returns 1. Never returns 0.
NA_IAPI int8     naSigni8  (int8    x);
NA_IAPI int16    naSigni16 (int16   x);
NA_IAPI int32    naSigni32 (int32   x);
NA_IAPI NAInt64  naSigni64 (NAInt64 x);
NA_IAPI NAInt128 naSigni128(NAInt128 x);
NA_IAPI NAInt256 naSigni256(NAInt256 x);

// Sets or unsets the sign bit. This is pure bit logic, not performing
// any complement.
NA_IAPI void naSetSignBit8    (void* i);
NA_IAPI void naSetSignBit16   (void* i);
NA_IAPI void naSetSignBit32   (void* i);
NA_IAPI void naSetSignBit64   (void* i);
NA_IAPI void naSetSignBit128  (void* i);
NA_IAPI void naSetSignBit256  (void* i);
NA_IAPI void naUnsetSignBit8  (void* i);
NA_IAPI void naUnsetSignBit16 (void* i);
NA_IAPI void naUnsetSignBit32 (void* i);
NA_IAPI void naUnsetSignBit64 (void* i);
NA_IAPI void naUnsetSignBit128(void* i);
NA_IAPI void naUnsetSignBit256(void* i);

// Makes the value positive.
NA_IAPI int8     naAbsi8  (int8     x);
NA_IAPI int16    naAbsi16 (int16    x);
NA_IAPI int32    naAbsi32 (int32    x);
NA_IAPI NAInt64  naAbsi64 (NAInt64  x);
NA_IAPI NAInt128 naAbsi128(NAInt128 x);
NA_IAPI NAInt256 naAbsi256(NAInt256 x);



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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
