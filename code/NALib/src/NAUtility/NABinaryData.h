
#ifndef NA_BINARY_DATA_INCLUDED
#define NA_BINARY_DATA_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// This file contains multiple functions dealing with binary data.
//
// Aside from copying, swapping or comparing bytes, there are also some
// encoding and decoding functions, Endianness conversions as well as
// Checksums.


#include "../NABase/NABase.h"
#include <stddef.h>


// Copies the array s to array d byte-by-byte
NA_IAPI void naCopy8  (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy16 (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy32 (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy64 (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy128(void* NA_RESTRICT d, const void* NA_RESTRICT s);

NA_IAPI void naCopyn(
  void* NA_RESTRICT d,
  const void* NA_RESTRICT s,
  size_t byteSize);


// Swaps the content of the two pointers with the given number of bits.
NA_IAPI void naSwap8  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap16 (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap32 (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap64 (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap128(void* NA_RESTRICT a, void* NA_RESTRICT b);
// Note: These functions are implemented with a well known swapping trick which
// only works if the two pointers do not point to the same location. Therefore
// restrict pointers make prefectly sense here.

// Swaps two Variables
NA_IAPI void naSwap (double* NA_RESTRICT a, double* NA_RESTRICT b);
NA_IAPI void naSwapf(float*  NA_RESTRICT a, float*  NA_RESTRICT b);
NA_IAPI void naSwapi(NAInt*  NA_RESTRICT a, NAInt*  NA_RESTRICT b);


// Compares the array of the first argument with the array of the second
// argument and returns NA_TRUE only if they are all equal.
NA_IAPI NABool naEqual8  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual16 (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual32 (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual64 (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b);


// Fills the given buffer with binary zero for the given byteSize
NA_IAPI void naZeron(void* d, size_t byteSize);

// Fills the given buffer with the value for the given byteSize
NA_IAPI void naSetn(void* d, size_t byteSize, NAByte value);



// Fills the array of the d argument with the given bytes in increasing
// address positions.
NA_IAPI void naFill8WithBytes(
  void* d,
  NAByte b0);
NA_IAPI void naFill16WithBytes(
  void* d,
  NAByte b0,  NAByte b1);
NA_IAPI void naFill32WithBytes(
  void* d,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3);
NA_IAPI void naFill64WithBytes(
  void* d,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7);
NA_IAPI void naFill128WithBytes(
  void* d,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15);
NA_IAPI void naFill256WithBytes(
  void* d,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15,
  NAByte b16, NAByte b17, NAByte b18, NAByte b19,
  NAByte b20, NAByte b21, NAByte b22, NAByte b23,
  NAByte b24, NAByte b25, NAByte b26, NAByte b27,
  NAByte b28, NAByte b29, NAByte b30, NAByte b31);


// Compares the array of the first argument with the given bytes in ascending
// address positions and returns NA_TRUE only if they are all equal.
NA_IAPI NABool naEqual8WithBytes(
  const void* s,
  NAByte b0);
NA_IAPI NABool naEqual16WithBytes(
  const void* s,
  NAByte b0,  NAByte b1);
NA_IAPI NABool naEqual32WithBytes(
  const void* s,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3);
NA_IAPI NABool naEqual64WithBytes(
  const void* s,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7);
NA_IAPI NABool naEqual128WithBytes(
  const void* s,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15);
NA_IAPI NABool naEqual256WithBytes(
  const void* s,
  NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
  NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
  NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
  NAByte b12, NAByte b13, NAByte b14, NAByte b15,
  NAByte b16, NAByte b17, NAByte b18, NAByte b19,
  NAByte b20, NAByte b21, NAByte b22, NAByte b23,
  NAByte b24, NAByte b25, NAByte b26, NAByte b27,
  NAByte b28, NAByte b29, NAByte b30, NAByte b31);




// ////////////////////
// Endianness
// ////////////////////

// Endianness conversions.
// Use those to convert from a source endianness to host endianness or vice
// versa. You can also explicitely convert from Little to Big or vice versa.
// Note that the NABuffer implementation offers more functionality.
//
// Will convert the data directly within the buffer
// provided. All converters work in both ways, for example Little to Big
// will also convert Big to Little.
//
// The v-variants convert a whole array with count elements. The
// non-v-variants convert only one single value.

// Conversion from host to the desired endianness. If endianness is the same
// as the host endianness, nothing happends.
NA_IAPI void naConvertEndianness8   (uint32 endianness, void* buffer);
NA_IAPI void naConvertEndianness16  (uint32 endianness, void* buffer);
NA_IAPI void naConvertEndianness32  (uint32 endianness, void* buffer);
NA_IAPI void naConvertEndianness64  (uint32 endianness, void* buffer);
NA_IAPI void naConvertEndianness128 (uint32 endianness, void* buffer);
NA_IAPI void naConvertEndianness8v  (uint32 endianness, void* buffer, size_t count);
NA_IAPI void naConvertEndianness16v (uint32 endianness, void* buffer, size_t count);
NA_IAPI void naConvertEndianness32v (uint32 endianness, void* buffer, size_t count);
NA_IAPI void naConvertEndianness64v (uint32 endianness, void* buffer, size_t count);
NA_IAPI void naConvertEndianness128v(uint32 endianness, void* buffer, size_t count);

// Conversion between Little and Big Endianness
NA_IAPI void naConvertLittleBig8   (void* buffer);
NA_IAPI void naConvertLittleBig16  (void* buffer);
NA_IAPI void naConvertLittleBig32  (void* buffer);
NA_IAPI void naConvertLittleBig64  (void* buffer);
NA_IAPI void naConvertLittleBig128 (void* buffer);
NA_IAPI void naConvertLittleBig8v  (void* buffer, size_t count);
NA_IAPI void naConvertLittleBig16v (void* buffer, size_t count);
NA_IAPI void naConvertLittleBig32v (void* buffer, size_t count);
NA_IAPI void naConvertLittleBig64v (void* buffer, size_t count);
NA_IAPI void naConvertLittleBig128v(void* buffer, size_t count);

// naIsEndiannessHost returns NA_TRUE if the given endianness is the same as
// the endianness of this system. Also returns NA_TRUE when sending native or
// unknown endianness. This function is useful for dynamically checking for
// endiannesses. If you simply are interested in the endianness of this host,
// check the NA_ENDIANNESS_HOST macro.
NA_IAPI NABool naIsEndiannessHost(uint32 endianness);

// Use the following functions to write and read a 4-byte endianness
// marker. This is useful for files which state what endianness they
// are encoded with. Important notice: Read and write the marker as a
// raw 4-byte-array, not as a 4-byte-entity!
//
// The markers are
// - "BDdb" for Big Endian
// - "bdDB" for Little Endian
// Note that the marker will not likely change in the future.
NA_IAPI void  naFillEndiannessMarker (NAByte marker[4], uint32 endianness);
NA_IAPI uint32 naParseEndiannessMarker(const NAByte marker[4]);




// ////////////////////
// Checksums
// ////////////////////

// The full type definition is in the file "NABinaryDataII.h"
typedef struct NAChecksum NAChecksum;

typedef enum{
  NA_CHECKSUM_TYPE_CRC_PNG,
  NA_CHECKSUM_TYPE_ADLER_32   // Used in Deflate
} NAChecksumType;


NA_API NAChecksum* naInitChecksum(NAChecksum* checksum, NAChecksumType type);
NA_API void naClearChecksum(NAChecksum* checksum);
NA_API void naResetChecksum(NAChecksum* checksum);
NA_API void naAccumulateChecksum(
  NAChecksum* checksum,
  const NAByte* buf,
  size_t byteSize);
NA_API uint32 naGetChecksumResult(NAChecksum* checksum);





// Inline implementations are in a separate file:
#include "NABinaryData/NABinaryDataII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BINARY_DATA_INCLUDED



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
