
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#ifndef NA_BIT_ARRAY_INCLUDED
#define NA_BIT_ARRAY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAByteArray.h"


// A Bit is stored as an NAByte. Sounds strange but that's how NALib does it.
typedef NAByte NABit;
typedef uint32 NANibble;  // A nibble consists of 4 Bits. Here in NALib, they
                          // are stored little-endiane'd in an uint32

#define NA_BIT0 0x00
#define NA_BIT1 0x01

#if NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG
  #define NA_NIBBLE_0 0x00000000
  #define NA_NIBBLE_1 0x01000000
  #define NA_NIBBLE_2 0x00010000
  #define NA_NIBBLE_3 0x01010000
  #define NA_NIBBLE_4 0x00000100
  #define NA_NIBBLE_5 0x01000100
  #define NA_NIBBLE_6 0x00010100
  #define NA_NIBBLE_7 0x01010100
  #define NA_NIBBLE_8 0x00000001
  #define NA_NIBBLE_9 0x01000001
  #define NA_NIBBLE_A 0x00010001
  #define NA_NIBBLE_B 0x01010001
  #define NA_NIBBLE_C 0x00000101
  #define NA_NIBBLE_D 0x01000101
  #define NA_NIBBLE_E 0x00010101
  #define NA_NIBBLE_F 0x01010101
#elif NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_LITTLE
  #define NA_NIBBLE_0 0x00000000
  #define NA_NIBBLE_1 0x00000001
  #define NA_NIBBLE_2 0x00000100
  #define NA_NIBBLE_3 0x00000101
  #define NA_NIBBLE_4 0x00010000
  #define NA_NIBBLE_5 0x00010001
  #define NA_NIBBLE_6 0x00010100
  #define NA_NIBBLE_7 0x00010101
  #define NA_NIBBLE_8 0x01000000
  #define NA_NIBBLE_9 0x01000001
  #define NA_NIBBLE_A 0x01000100
  #define NA_NIBBLE_B 0x01000101
  #define NA_NIBBLE_C 0x01010000
  #define NA_NIBBLE_D 0x01010001
  #define NA_NIBBLE_E 0x01010100
  #define NA_NIBBLE_F 0x01010101
#endif


// An NABitArray stores two arrays: A full bits storage and a subarray
// thereof referencing the full storage.
//
// The bits field is always a subarray of fullstorage. While the bitarray
// visible to the programmer is denoted by the bits field, fullstorage may
// have allocated more bits ready to be used when an operation needs to be
// performed.
//
// The constructor naCreateBitArrayWithCount allows you to create both a bit
// array where no space is wasted (fullstorage is the same as bits) as well as
// to create an empty bits array but with an arbitrary number of bits pre-
// allocated.


// Opaque type. See explanation in readme.txt
typedef struct NABitArray NABitArray;
struct NABitArray{
  struct NAByteArray fullstorage;
  struct NAByteArray bits;
};


// Creates an empty bit array.
NA_API NABitArray* naCreateBitArray(NABitArray* bitarray);


// Creates a bit array with the specified count.
// - If count is positive, the bit storage has precisely the desired size.
// - If count is negative, enough space will be allocated to hold the absolute
//   number of count as bits. But the returned array will be empty to begin
//   with. If you later perform manipulations on this bitarray which need more
//   space, the previously allocated space will be used.
//   Choose the negative count appropriate to the expected needs. If for
//   example, you expect an add-operation upon the bit array, provide 1 Bit
//   more than needed.
// - If count is zero, an empty bit array is returned.
//
// The returned bit array is always unititialized.
NA_API NABitArray* naCreateBitArrayWithCount( NABitArray* bitarray,
                                                    NAInt count);


// Creates a new array which is extended by a certain number of bits on both
// ends. todo
NA_API NABitArray * naCreateBitArrayExtension(NABitArray* dstarray,
                                              NABitArray* srcarray,
                                                    NAInt offset,
                                                    NAInt size);

// Will create an extraction of the given srcarray by referencing the values.
// This function will not copy any storage. Use naDecoupleBitArray for that.
// The resulting bit array will dereference the fullstorage in the given
// range and set the bits array to that very same range.
NA_API NABitArray* naCreateBitArrayExtraction(NABitArray* dstarray,
                                              NABitArray* srcarray,
                                                    NAInt offset,
                                                    NAInt size);

// Creates a bit array from the given string. The string can contain binary
// values, decimal values, hexadecimal values. The ASCII-function will create
// a bit representation of the ASCII characters.
// - If sizehint is positive, it denotes the precise number of bits the
//   resulting bit array shall contain.
// - If sizehint is zero, the size of the bit array is automatically set to
//   the number of bits needed to store all bits of the given string. May
//   result in an empty bit array.
// - If sizehint is negative, the full string is stored just as if sizehint
//   was zero. But the number of bits is dividable by the absolute value of
//   sizehint. Or to say it differently, the bit array is aligned.
//   For example, if sizehint is -8, the resulting bit array contains 8, 16,
//   24, 32, ... bits, but never something in between.
// Note that strings are always expected to be in natural reading order,
// meaning, the trailing of the string contains the least significant values.
NA_API NABitArray* naCreateBitArrayWithBinString(  NABitArray* bitarray,
                                                     NAString* string,
                                                         NAInt sizehint);
NA_API NABitArray* naCreateBitArrayWithDecString(  NABitArray* bitarray,
                                                     NAString* string,
                                                         NAInt sizehint);
NA_API NABitArray* naCreateBitArrayWithHexString(  NABitArray* bitarray,
                                                     NAString* string,
                                                         NAInt sizehint);
NA_API NABitArray* naCreateBitArrayWithASCIIString(NABitArray* bitarray, //todo make byte array.
                                                     NAString* string,
                                                         NAInt sizehint);

NA_API void naClearBitArray(NABitArray* array);
NA_API void naDestroyBitArray(NABitArray* array);

// Will decouple the given array from its current storage by creating its own
// storage with the precise size of the bits field. COPIES ALWAYS!
NA_API void naDecoupleBitArray(NABitArray* array);



// Returns the bit with the requested index. If index is negative, it
// is searched from the end of the array. For example -1 denotes the last bit.
// Warning: returns garbage if the array is empty.
NA_API NABit* naGetBitArrayBit(NABitArray* bitarray, NAInt indx);

// Returns the size of the bits array. Not the size of the full storage!
NA_API NAInt naGetBitArrayCount(NABitArray* bitarray);


// Creates string representations of the given bitarray as an integer number.
NA_API NAString* naCreateStringDecFromBitArray(NAString* string,
                                             NABitArray* bitarray);
NA_API NAString* naCreateStringHexFromBitArray(NAString* string,
                                             NABitArray* bitarray);
NA_API NAString* naCreateStringBinFromBitArray(NAString* string,
                                             NABitArray* bitarray);

// Creates a byte array out of the given bit array. Every 8 bits are combined
// into one single byte. The byte array is guaranteed to be null-terminated.
// Careful: Only works properly when bitcount can be divided by 8! Use
// naCreateBitArrayExtension first, if this is not the case.
NA_API NAByteArray* naCreateByteArrayFromBitArray(NAByteArray* bytearray,
                                                   NABitArray* bitarray);


// //////////////////////////////////
// Arithmetic functions
//
// The following functions take the bits of one or more source arrays and will
// perform arithmetic operations with them and finally store the result in
// a destination bit array.
//
// - These are not creation functions! All pointers, including the destination
//   array must point to an initialized NABitArray!
// - Some array pointers may be equal, but not all!
// - The number of bits considered for the computation is limited by the number
//   of bits allocated in the fullstorage of the dest array. The result may have
//   less bits, but never more.
// - All operations will be performed as UNSIGNED integers. Be careful: This can
//   lead to false computations when trying to add signed integers of different
//   bit sizes! Extend the smaller arrays using naCreateBitArrayExtension first.


// Adds two bit arrays and puts it into the storage of dstarray. All pointers
// can be equal.
NA_API NABit naComputeBitArrayAddBitArray( NABitArray* dstarray,
                                           NABitArray* srcarray1,
                                           NABitArray* srcarray2);

// Multiplies the integer value of srcarray by 10 (Ten). The src pointer MUST
// be different than the dst pointer!
NA_API void naComputeBitArrayMulTen(NABitArray* dstarray, NABitArray* srcarray);



// //////////////////////////////////
// Bit-Fiddling functions
// The following functions alter the bits on an existing array while leaving
// its size and memory position unchanged.

// Adds the integer value 1 to the bit array. A Wrap-around may occur.
// Returns the carry.
NA_API NABit naComputeBitArrayAddOne(NABitArray* array);

// Computes the one's and two's complement of the given array.
NA_API void naComputeBitArrayOnesComplement(NABitArray* array);
NA_API void naComputeBitArrayTwosComplement(NABitArray* array);

// Swaps the byte order. Careful: Only works properly when bitcount can be
// divided by 8! Use naCreateBitArrayExtension first, if this is not the case.
NA_API void naComputeBitArraySwapBytes(NABitArray* array);



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BIT_ARRAY_INCLUDED



// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
