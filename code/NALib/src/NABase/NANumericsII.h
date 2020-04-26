
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"


// In case you consider moving the cast inside: No. Don't. Nononono.
NA_IDEF uint8 naGetSignum8(int8 i){
  return (uint8)(i >> (NA_TYPE8_BITS  - 1));
}
NA_IDEF uint16 naGetSignum16(int16 i){
  return (uint16)(i >> (NA_TYPE16_BITS - 1));
}
NA_IDEF uint32 naGetSignum32(int32 i){
  return (uint32)(i >> (NA_TYPE32_BITS - 1));
}
NA_IDEF NAUInt64 naGetSignum64(NAInt64 i){
  return naCastInt64ToUInt64(naShrInt64(i, (NA_TYPE64_BITS - 1)));
}
NA_IDEF NAUInt128 naGetSignum128(NAInt128 i){
  return naCastInt128ToUInt128(naShrInt128(i, (NA_TYPE128_BITS - 1)));
}
NA_IDEF NAUInt256 naGetSignum256(NAInt256 i){
  return naCastInt256ToUInt256(naShrInt256(i, (NA_TYPE256_BITS - 1)));
}



NA_IDEF int8 naSigni8(int8 i){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return (int8)((naGetSignum8(i) << 1) + 1);
  #else
    return (i < 0) ? -1 : 1;
  #endif
}
NA_IDEF int16 naSigni16(int16 i){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return (int16)((naGetSignum16(i) << 1) + 1);
  #else
    return (i < 0) ? -1 : 1;
  #endif
}
NA_IDEF int32 naSigni32(int32 i){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return (int32)((naGetSignum32(i) << 1) + NA_ONE_32);
  #else
    return (i < 0) ? -NA_ONE_32 : NA_ONE_32;
  #endif
}
NA_IDEF NAInt64 naSigni64(NAInt64 i){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    NAUInt64 signum = naGetSignum64(i);
    return naCastUInt64ToInt64(naAddUInt64(naShlUInt64(signum, 1), NA_ONE_64u));
  #else
    return (i < 0) ? -NA_ONE_64 : NA_ONE_64;
  #endif
}
NA_IDEF NAInt128 naSigni128(NAInt128 i){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    NAUInt128 signum = naGetSignum128(i);
    return naCastUInt128ToInt128(naAddUInt128(naShlUInt128(signum, 1), NA_ONE_128u));
  #else
    return (i < 0) ? -NA_ONE_128 : NA_ONE_128;
  #endif
}
NA_IDEF NAInt256 naSigni256(NAInt256 i){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    NAUInt256 signum = naGetSignum256(i);
    return naCastUInt256ToInt256(naAddUInt256(naShlUInt256(signum, 1), NA_ONE_256u));
  #else
    return (i < 0) ? -NA_ONE_256 : NA_ONE_256;
  #endif
}



NA_IDEF void naSetSignBit8(void* i){
  *((uint8*)i) |= NA_VALUE8_SIGN_MASK;
}
NA_IDEF void naSetSignBit16(void* i){
  *((uint16*)i) |= NA_VALUE16_SIGN_MASK;
}
NA_IDEF void naSetSignBit32(void* i){
  *((uint32*)i) |= NA_VALUE32_SIGN_MASK;
}
NA_IDEF void naSetSignBit64(void* i){
  *((NAUInt64*)i) = naOrUInt64(*((NAUInt64*)i), NA_VALUE64_SIGN_MASK);
}
NA_IDEF void naSetSignBit128(void* i){
  *((NAUInt128*)i) = naOrUInt128(*((NAUInt128*)i), NA_VALUE128_SIGN_MASK);
}
NA_IDEF void naSetSignBit256(void* i){
  *((NAUInt256*)i) = naOrUInt256(*((NAUInt256*)i), NA_VALUE256_SIGN_MASK);
}

NA_IDEF void naUnsetSignBit8(void* i){
  *((uint8*)i) &= ~NA_VALUE8_SIGN_MASK;
}
NA_IDEF void naUnsetSignBit16(void* i){
  *((uint16*)i) &= ~NA_VALUE16_SIGN_MASK;
}
NA_IDEF void naUnsetSignBit32(void* i){
  *((uint32*)i) &= ~NA_VALUE32_SIGN_MASK;
}
NA_IDEF void naUnsetSignBit64(void* i){
  *((NAUInt64*)i) = naAndUInt64(*((NAUInt64*)i), naNotUInt64(NA_VALUE64_SIGN_MASK));
}
NA_IDEF void naUnsetSignBit128(void* i){
  *((NAUInt128*)i) = naAndUInt128(*((NAUInt128*)i), naNotUInt128(NA_VALUE128_SIGN_MASK));
}
NA_IDEF void naUnsetSignBit256(void* i){
  *((NAUInt256*)i) = naAndUInt256(*((NAUInt256*)i), naNotUInt256(NA_VALUE256_SIGN_MASK));
}



NA_IDEF int8 naAbsi8(int8 i){
  return naSigni8(i) * i;
}
NA_IDEF int16 naAbsi16(int16 i){
  return naSigni16(i) * i;
}
NA_IDEF int32 naAbsi32(int32 i){
  return naSigni32(i) * i;
}
NA_IDEF NAInt64 naAbsi64(NAInt64 i){
  return naMulInt64(naSigni64(i), i);
}
NA_IDEF NAInt128 naAbsi128(NAInt128 i){
  return naMulInt128(naSigni128(i), i);
}
NA_IDEF NAInt256 naAbsi256(NAInt256 i){
  return naMulInt256(naSigni256(i), i);
}


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
