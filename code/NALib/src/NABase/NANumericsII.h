
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#ifndef NA_NUMERICS_II_INCLUDED
#define NA_NUMERICS_II_INCLUDED



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
NA_IDEF NAu64 naGetSignum64(NAi64 i){
  return naCasti64Tou64(naShri64(i, (NA_TYPE64_BITS - 1)));
}
NA_IDEF NAu128 naGetSignum128(NAi128 i){
  return naCasti128Tou128(naShri128(i, (NA_TYPE128_BITS - 1)));
}
NA_IDEF NAu256 naGetSignum256(NAi256 i){
  return naCasti256Tou256(naShri256(i, (NA_TYPE256_BITS - 1)));
}



NA_IDEF int8 naSigni8(int8 i){
  return (i >> (NA_TYPE8_BITS - 2)) | NA_ONE_i8;
}
NA_IDEF int16 naSigni16(int16 i){
  return (i >> (NA_TYPE16_BITS - 2)) | NA_ONE_i16;
}
NA_IDEF int32 naSigni32(int32 i){
  return (i >> (NA_TYPE32_BITS - 2)) | NA_ONE_i32;
}
NA_IDEF NAi64 naSigni64(NAi64 i){
  return naOri64(naShri64(i, NA_TYPE64_BITS - 2), NA_ONE_i64);
}
NA_IDEF NAi128 naSigni128(NAi128 i){
  return naOri128(naShri128(i, NA_TYPE128_BITS - 2), NA_ONE_i128);
}
NA_IDEF NAi256 naSigni256(NAi256 i){
  return naOri256(naShri256(i, NA_TYPE256_BITS - 2), NA_ONE_i256);
}



NA_IDEF void naSetSignBit8(void* i){
  *((int8*)i) |= NA_SIGN_MASK_8;
}
NA_IDEF void naSetSignBit16(void* i){
  *((int16*)i) |= NA_SIGN_MASK_16;
}
NA_IDEF void naSetSignBit32(void* i){
  *((int32*)i) |= NA_SIGN_MASK_32;
}
NA_IDEF void naSetSignBit64(void* i){
  *((NAi64*)i) = naOri64(*((NAi64*)i), NA_SIGN_MASK_64);
}
NA_IDEF void naSetSignBit128(void* i){
  *((NAi128*)i) = naOri128(*((NAi128*)i), NA_SIGN_MASK_128);
}
NA_IDEF void naSetSignBit256(void* i){
  *((NAi256*)i) = naOri256(*((NAi256*)i), NA_SIGN_MASK_256);
}

NA_IDEF void naUnsetSignBit8(void* i){
  *((int8*)i) &= ~NA_SIGN_MASK_8;
}
NA_IDEF void naUnsetSignBit16(void* i){
  *((int16*)i) &= ~NA_SIGN_MASK_16;
}
NA_IDEF void naUnsetSignBit32(void* i){
  *((int32*)i) &= ~NA_SIGN_MASK_32;
}
NA_IDEF void naUnsetSignBit64(void* i){
  *((NAi64*)i) = naAndi64(*((NAi64*)i), naNoti64(NA_SIGN_MASK_64));
}
NA_IDEF void naUnsetSignBit128(void* i){
  *((NAi128*)i) = naAndi128(*((NAi128*)i), naNoti128(NA_SIGN_MASK_128));
}
NA_IDEF void naUnsetSignBit256(void* i){
  *((NAi256*)i) = naAndi256(*((NAi256*)i), naNoti256(NA_SIGN_MASK_256));
}



NA_IDEF int8 naAbsi8(int8 i){
  int8 signum = (int8)naGetSignum8(i);
  return (signum ^ i) - signum;
}
NA_IDEF int16 naAbsi16(int16 i){
  int16 signum = (int16)naGetSignum16(i);
  return (signum ^ i) - signum;
}
NA_IDEF int32 naAbsi32(int32 i){
  int32 signum = (int32)naGetSignum32(i);
  return (signum ^ i) - signum;
}
NA_IDEF NAi64 naAbsi64(NAi64 i){
  NAi64 signum = naCastu64Toi64(naGetSignum64(i));
  return naSubi64(naXori64(signum, i), signum);
}
NA_IDEF NAi128 naAbsi128(NAi128 i){
  NAi128 signum = naCastu128Toi128(naGetSignum128(i));
  return naSubi128(naXori128(signum, i), signum);
}
NA_IDEF NAi256 naAbsi256(NAi256 i){
  NAi256 signum = naCastu256Toi256(naGetSignum256(i));
  return naSubi256(naXori256(signum, i), signum);
}



#endif // NA_NUMERICS_II_INCLUDED

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
