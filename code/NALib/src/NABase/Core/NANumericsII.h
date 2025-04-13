
#if defined NA_NUMERICS_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_NUMERICS_II_INCLUDED
#define NA_NUMERICS_II_INCLUDED



// In case you consider moving the cast inside: No. Don't. Nononono.
NA_IDEF uint8 naGetSignum8(int8 i) {
  return (uint8)(i >> (NA_TYPE8_BITS  - 1));
}
NA_IDEF uint16 naGetSignum16(int16 i) {
  return (uint16)(i >> (NA_TYPE16_BITS - 1));
}
NA_IDEF uint32 naGetSignum32(int32 i) {
  return (uint32)(i >> (NA_TYPE32_BITS - 1));
}
NA_IDEF NAu64 naGetSignum64(NAi64 i) {
  return naCasti64Tou64(naShri64(i, (NA_TYPE64_BITS - 1)));
}
NA_IDEF NAu128 naGetSignum128(NAi128 i) {
  return naCasti128Tou128(naShri128(i, (NA_TYPE128_BITS - 1)));
}
NA_IDEF NAu256 naGetSignum256(NAi256 i) {
  return naCasti256Tou256(naShri256(i, (NA_TYPE256_BITS - 1)));
}



NA_IDEF int8 naSigni8(int8 i) {
  return (i >> (NA_TYPE8_BITS - 2)) | NA_ONE_i8;
}
NA_IDEF int16 naSigni16(int16 i) {
  return (i >> (NA_TYPE16_BITS - 2)) | NA_ONE_i16;
}
NA_IDEF int32 naSigni32(int32 i) {
  return (i >> (NA_TYPE32_BITS - 2)) | NA_ONE_i32;
}
NA_IDEF NAi64 naSigni64(NAi64 i) {
  return naOri64(naShri64(i, NA_TYPE64_BITS - 2), NA_ONE_i64);
}
NA_IDEF NAi128 naSigni128(NAi128 i) {
  return naOri128(naShri128(i, NA_TYPE128_BITS - 2), NA_ONE_i128);
}
NA_IDEF NAi256 naSigni256(NAi256 i) {
  return naOri256(naShri256(i, NA_TYPE256_BITS - 2), NA_ONE_i256);
}



NA_IDEF void naSetSignBit8(void* i) {
  *((int8*)i) |= NA_SIGN_MASK_8;
}
NA_IDEF void naSetSignBit16(void* i) {
  *((int16*)i) |= NA_SIGN_MASK_16;
}
NA_IDEF void naSetSignBit32(void* i) {
  *((int32*)i) |= NA_SIGN_MASK_32;
}
NA_IDEF void naSetSignBit64(void* i) {
  *((NAi64*)i) = naOri64(*((NAi64*)i), NA_SIGN_MASK_64);
}
NA_IDEF void naSetSignBit128(void* i) {
  *((NAi128*)i) = naOri128(*((NAi128*)i), NA_SIGN_MASK_128);
}
NA_IDEF void naSetSignBit256(void* i) {
  *((NAi256*)i) = naOri256(*((NAi256*)i), NA_SIGN_MASK_256);
}

NA_IDEF void naUnsetSignBit8(void* i) {
  *((int8*)i) &= ~NA_SIGN_MASK_8;
}
NA_IDEF void naUnsetSignBit16(void* i) {
  *((int16*)i) &= ~NA_SIGN_MASK_16;
}
NA_IDEF void naUnsetSignBit32(void* i) {
  *((int32*)i) &= ~NA_SIGN_MASK_32;
}
NA_IDEF void naUnsetSignBit64(void* i) {
  *((NAi64*)i) = naAndi64(*((NAi64*)i), naNoti64(NA_SIGN_MASK_64));
}
NA_IDEF void naUnsetSignBit128(void* i) {
  *((NAi128*)i) = naAndi128(*((NAi128*)i), naNoti128(NA_SIGN_MASK_128));
}
NA_IDEF void naUnsetSignBit256(void* i) {
  *((NAi256*)i) = naAndi256(*((NAi256*)i), naNoti256(NA_SIGN_MASK_256));
}



NA_IDEF int8 naAbsi8(int8 i) {
  // See comments of NAAbsi32
  uint8 signum = naGetSignum8(i);
  return (int8)(signum ^ (uint8)i) - signum;
}
NA_IDEF int16 naAbsi16(int16 i) {
  // See comments of NAAbsi32
  uint16 signum = naGetSignum16(i);
  return (int16)((signum ^ (uint16)i) - signum);
}
NA_IDEF int32 naAbsi32(int32 i) {
  // Note that the casts to unsigned int are necessary as otherwise, some
  // the computation NA_MAX_i32 - -1 leads to undefined results.
  uint32 signum = naGetSignum32(i);
  return (int32)((signum ^ (uint32)i) - signum);
}
NA_IDEF NAi64 naAbsi64(NAi64 i) {
  // See comments of NAAbsi32
  NAu64 signum = naGetSignum64(i);
  return naCastu64Toi64(naSubu64(naXoru64(signum, naCasti64Tou64(i)), signum));
}
NA_IDEF NAi128 naAbsi128(NAi128 i) {
  // See comments of NAAbsi32
  NAu128 signum = naGetSignum128(i);
  return naCastu128Toi128(naSubu128(naXoru128(signum, naCasti128Tou128(i)), signum));
}
NA_IDEF NAi256 naAbsi256(NAi256 i) {
  // See comments of NAAbsi32
  NAu256 signum = naGetSignum256(i);
  return naCastu256Toi256(naSubu256(naXoru256(signum, naCasti256Tou256(i)), signum));
}



#endif // NA_NUMERICS_II_INCLUDED



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
