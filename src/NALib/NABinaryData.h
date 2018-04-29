
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

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


#include "NABase.h"


// Copies the array s to array d byte-by-byte
NA_IAPI void naCopy8    (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy16   (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy32   (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy64   (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopy128  (void* NA_RESTRICT d, const void* NA_RESTRICT s);
NA_IAPI void naCopyn    (void* NA_RESTRICT d,
                   const void* NA_RESTRICT s,
                                     NAInt bytesize);


// Swaps the content of the two pointers with the given number of bits.
NA_IAPI void naSwap8   (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap16  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap32  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap64  (void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI void naSwap128 (void* NA_RESTRICT a, void* NA_RESTRICT b);
// Note: These functions are implemented with a well known swapping trick which
// only works if the two pointers do not point to the same location. Therefore
// restrict pointers make prefectly sense here.

// Swaps two Variables
NA_IAPI void naSwap    (double* NA_RESTRICT a, double* NA_RESTRICT b);
NA_IAPI void naSwapf   (float*  NA_RESTRICT a, float*  NA_RESTRICT b);
NA_IAPI void naSwapi   (NAInt*  NA_RESTRICT a, NAInt*  NA_RESTRICT b);
NA_IAPI void naSwapu   (NAUInt* NA_RESTRICT a, NAUInt* NA_RESTRICT b);


// Compares the array of the first argument with the array of the second
// argument and returns NA_TRUE only if they are all equal.
NA_IAPI NABool naEqual8(  void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual16( void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual32( void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual64( void* NA_RESTRICT a, void* NA_RESTRICT b);
NA_IAPI NABool naEqual128(void* NA_RESTRICT a, void* NA_RESTRICT b);


// Fills the given buffer with the value 0 for the given bytesize
NA_IAPI void naNulln32  (void* d, int32 bytesize);
NA_IAPI void naNulln64  (void* d, int64 bytesize);
NA_IAPI void naNulln    (void* d, NAInt bytesize);

// Fills the given buffer with the value 0 for the given bytesize
NA_IAPI void naSetn32  (void* d, int32 bytesize, NAByte value);
NA_IAPI void naSetn64  (void* d, int64 bytesize, NAByte value);
NA_IAPI void naSetn    (void* d, NAInt bytesize, NAByte value);



// Fills the array of the d argument with the given bytes in increasing
// address positions.
NA_IAPI void naFill8WithBytes(   void* d,
                      NAByte b0);
NA_IAPI void naFill16WithBytes(  void* d,
                      NAByte b0,  NAByte b1);
NA_IAPI void naFill32WithBytes(  void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3);
NA_IAPI void naFill64WithBytes(  void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                      NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7);
NA_IAPI void naFill128WithBytes( void* d,
                      NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                      NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                      NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                      NAByte b12, NAByte b13, NAByte b14, NAByte b15);


// Compares the array of the first argument with the given bytes in increasing
// address positions and returns NA_TRUE only if they are all equal.
NA_IAPI NABool naEqual8WithBytes(   const void* s,
                                NAByte b0);
NA_IAPI NABool naEqual16WithBytes(  const void* s,
                                NAByte b0,  NAByte b1);
NA_IAPI NABool naEqual32WithBytes(  const void* s,
                                NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3);
NA_IAPI NABool naEqual64WithBytes(  const void* s,
                                NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                                NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7);
NA_IAPI NABool naEqual128WithBytes( const void* s,
                                NAByte b0,  NAByte b1,  NAByte b2,  NAByte b3,
                                NAByte b4,  NAByte b5,  NAByte b6,  NAByte b7,
                                NAByte b8,  NAByte b9,  NAByte b10, NAByte b11,
                                NAByte b12, NAByte b13, NAByte b14, NAByte b15);




// ////////////////////
// Endianness
// ////////////////////


// Static Endianness conversions.
// Use those if you know exactly from which source endianness to which
// destination endianness you want to convert. If you don't know it at
// compile-time, use the NAEndiannessConverter below or NABuffer.
//
// Will convert the data directly within the buffer
// provided. All converters work in both ways, for example Little to Big
// will also convert Big to Little.
//
// The v-variants convert a whole array with count elements. The
// non-v-variants convert only one single value.

// Conversion between two same Endiannesses.
NA_IAPI  void naConvertNative8(          void* buffer);
NA_IAPI  void naConvertNative16(         void* buffer);
NA_IAPI  void naConvertNative32(         void* buffer);
NA_IAPI  void naConvertNative64(         void* buffer);
NA_IAPI  void naConvertNative128(        void* buffer);
NA_IAPI  void naConvertNative8v(         void* buffer, NAUInt count);
NA_IAPI  void naConvertNative16v(        void* buffer, NAUInt count);
NA_IAPI  void naConvertNative32v(        void* buffer, NAUInt count);
NA_IAPI  void naConvertNative64v(        void* buffer, NAUInt count);
NA_IAPI  void naConvertNative128v(       void* buffer, NAUInt count);

// Conversion between Little and Big Endianness
NA_IAPI  void naConvertLittleBig8(       void* buffer);
NA_IAPI  void naConvertLittleBig16(      void* buffer);
NA_IAPI  void naConvertLittleBig32(      void* buffer);
NA_IAPI  void naConvertLittleBig64(      void* buffer);
NA_IAPI  void naConvertLittleBig128(     void* buffer);
NA_IAPI  void naConvertLittleBig8v(      void* buffer, NAUInt count);
NA_IAPI  void naConvertLittleBig16v(     void* buffer, NAUInt count);
NA_IAPI  void naConvertLittleBig32v(     void* buffer, NAUInt count);
NA_IAPI  void naConvertLittleBig64v(     void* buffer, NAUInt count);
NA_IAPI  void naConvertLittleBig128v(    void* buffer, NAUInt count);

// Conversion between Native and Little Endianness
NA_IAPI  void naConvertNativeLittle8(    void* buffer);
NA_IAPI  void naConvertNativeLittle16(   void* buffer);
NA_IAPI  void naConvertNativeLittle32(   void* buffer);
NA_IAPI  void naConvertNativeLittle64(   void* buffer);
NA_IAPI  void naConvertNativeLittle128(  void* buffer);
NA_IAPI  void naConvertNativeLittle8v(   void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeLittle16v(  void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeLittle32v(  void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeLittle64v(  void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeLittle128v( void* buffer, NAUInt count);

// Conversion between Native and Big Endianness
NA_IAPI  void naConvertNativeBig8(       void* buffer);
NA_IAPI  void naConvertNativeBig16(      void* buffer);
NA_IAPI  void naConvertNativeBig32(      void* buffer);
NA_IAPI  void naConvertNativeBig64(      void* buffer);
NA_IAPI  void naConvertNativeBig128(     void* buffer);
NA_IAPI  void naConvertNativeBig8v(      void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeBig16v(     void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeBig32v(     void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeBig64v(     void* buffer, NAUInt count);
NA_IAPI  void naConvertNativeBig128v(    void* buffer, NAUInt count);



// If you have dynamically defined endiannesses, use an NAEndiannessConverter.
// Use the naMakeEndiannessConverter function to fill the structure.
// After creation, you can use the function-pointers stored in the
// NAEndiannessConverter struct directly.
//
// These are the structs accessible to the programmer.

typedef struct NAEndiannessConverter NAEndiannessConverter;

struct NAEndiannessConverter{
  void (*convert8)    (void* buffer);
  void (*convert16)   (void* buffer);
  void (*convert32)   (void* buffer);
  void (*convert64)   (void* buffer);
  void (*convert128)  (void* buffer);
  void (*convert8v)   (void* buffer, NAUInt count);
  void (*convert16v)  (void* buffer, NAUInt count);
  void (*convert32v)  (void* buffer, NAUInt count);
  void (*convert64v)  (void* buffer, NAUInt count);
  void (*convert128v) (void* buffer, NAUInt count);
};


// Returns an NAEndiannessConverter structure with all converters for all
// endiannesses available in NALib. Note that it does not really matter which
// is the endianness1 and which is endianness2 as all converters can be used
// bidirectional.
NA_IAPI NAEndiannessConverter naMakeEndiannessConverter(NAInt endianness1,
                                                        NAInt endianness2);


// naIsEndiannessNative returns NA_TRUE if the given endianness is the same as
// the endianness of this system. Also returns NA_TRUE when sending native or
// unknown endianness. This function is useful for dynamically checking for
// endiannesses. If you simply are interested in the (static) endianness of
// this system, check the NA_SYSTEM_ENDIANNESS macro.
NA_IAPI NABool naIsEndiannessNative(NAInt endianness);


// Use the following functions to write and read a 4-byte endianness
// marker. This is useful for files which state what endianness they
// are encoded with. Important notice: Read and write the marker as a
// raw 4-byte-array, not as a 4-byte-entity!
//
// The markers are
// - "BDdb" for Big Endian
// - "bdDB" for Little Endian
// Note that the marker will not likely change in the future.
NA_IAPI void  naFillEndiannessMarker (NAByte marker[4], NAInt endianness);
NA_IAPI NAInt naParseEndiannessMarker(const NAByte marker[4]);




// ////////////////////
// Checksums
// ////////////////////

// The full type definition is in the file "NABinaryDataII.h"
typedef struct NAChecksum NAChecksum;

typedef enum{
  NA_CHECKSUM_TYPE_CRC_PNG,
  NA_CHECKSUM_TYPE_ADLER_32   // Used in ZLIB
} NAChecksumType;


NA_API NAChecksum* naInitChecksum(NAChecksum* checksum, NAChecksumType type);
NA_API void naClearChecksum(NAChecksum* checksum);
NA_API void naResetChecksum(NAChecksum* checksum);
NA_API void naAccumulateChecksum(   NAChecksum* checksum,
                                  const NAByte* buf,
                                          NAInt bytesize);
NA_API uint32 naGetChecksumResult(NAChecksum* checksum);





// Inline implementations are in a separate file:
#include "NACore/NABinaryDataII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BINARY_DATA_INCLUDED


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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
