
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_BYTE_ARRAY_INCLUDED
#define NA_BYTE_ARRAY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// NAByteArray
//
// An NAByteArray stores an array of raw bytes by dereferencing a storage.
// Every ByteArray has a pointer to an internal storage, a data pointer and a
// size. Upon creation of an NAByteArray, the last bytes of the storage can
// automatically filled with binary zero.
//
// Multiple NAByteArrays can point to the same storage. Meaning: Multiple
// NAByteArrays can point to different sections of the same byte stream by
// providing an offset and a size relative to another NAByteArray. This allows
// for very fast and convenient parsing of byte streams without copying
// the bytes over and over again.
//
// This struct is used by NAArray and NAString.


#include "NAMemory.h"


// The full type definition is in the file "NAByteArrayII.h"
typedef struct NAByteArray NAByteArray;


// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Initializes a new EMPTY NAByteArray and returns the given pointer.
// As NAByteArrays can not be changed in size, this function seems rather
// unnecessary as the only way of using an empty array is to find out that it
// is empty. But there are many occasions in programming where having such a
// function simplifies coding.
//
// This function is speedy. No additional memory will be allocated.
NA_IAPI NAByteArray* naInitByteArray(NAByteArray* array);

// Creates or fills a new NAByteArray with the desired size and returns the
// given pointer. Allocates memory on the heap which will be freed
// automatically when the array storage is no longer used.
//
// If size is zero, an empty array is returned.
//
// When size is positive, the precise amount of bytes is allocated and the
// whole memory block will be uninitialized.
//
// If size is negative, the absolute value of size is used but it will
// automatically be expanded by a certain number of bytes at the end of the
// array. It is guaranteed that there are at least 4 Bytes appended on 32 Bit
// systems and 8 Bytes on 64 Bit systems and it is guaranteed that these bytes
// are initialized with binary zero. See the implementation for more details.
// The returned NAByteArray itself will NOT contain any additional bytes but
// instead have the exact same size as given as argument.
//
// The negative size is used for example for zero-terminated strings or can
// be used for example for sentinels in pointer arrays. There, with negative
// size, it is safe to read past the end of an array as you can be sure that
// at least one pointer size after the array is filled with binary zero.
NA_API NAByteArray* naInitByteArrayWithSize(NAByteArray* array, NAInt size);

// Use the following functions to encapsulate your own raw buffers into an
// NAByteArray!
//
// There are two creation functions, one for const data and one for non-const.
//
// Creates or fills a new NAByteArray which contains the data of the given
// buffer WITHOUT copying. The size denotes the size of the buffer in bytes.
// The programmer is responsible for that size does not overflows the buffer.
// When takeownership is set to NA_TRUE, the array will deallocate the given
// buffer with free() when it is no longer used. If size is null, an empty
// array is created. But note that if takeownership is true, the buffer will
// still be referenced and deleted only upon clearing the NAByteArray.
//
// You can not take ownership of const buffers.
//
// If size is negative, the buffer is EXPECTED to be oversized by an unknown
// amount of bytes and that these bytes are filled with binary zeros. The
// oversized bytes will NEVER be accessed by this NAByteArray but they are
// assumed to be there. For example the buffer for the C-string literal "Hello"
// can be safely packed into an NAByteArray with size -5.
NA_API NAByteArray* naInitByteArrayWithConstBuffer(
                                                NAByteArray* array,
                                                 const void* buffer,
                                                       NAInt bytesize);
NA_API NAByteArray* naInitByteArrayWithMutableBuffer(
                                                NAByteArray* array,
                                                       void* buffer,
                                                       NAInt bytesize,
                                            NAPointerCleanup cleanuphint);



// Function naInitByteArrayExtraction:
// Creates or fills a new ByteArray with the contents of srcarray. The content
// is NOT copied, but the dest array points to the same storage as srcarray
// with an offset and size relative to the src array.
//
// dstarray can be the same as srcarray!
//
// Use this function to perform all sorts of manipulations (examples below):
// - if offset is negative, it denotes the number of bytes from the end.
//   Note that the end has index [size], meaning -1 denotes the index [size-1]
//   which is the last byte.
// - If the size is 0, the resulting ByteArray is empty.
// - if size is negative, it denotes the size up and including to the given
//   number of bytes from the end, meaning -1 denotes the last byte.
// - if the offset and size combination somehow leads to a size of exactly 0,
//   the resulting ByteArray will be empty without a warning emitted.
// - If the offset and size combination somehow leads to an over- or underflow,
//   a warning will be emitted if NDEBUG is defined. The resulting array will
//   be empty.
//
// Example: Array ABCDEF with the pair (offset, size):
// - ( 2,  2)   ->   CD   (extraction)
// - ( 2, -1)   ->   CDEF (truncate head from left)
// - (-2, -1)   ->     EF (truncate head from right)
// - ( 0,  2)   -> AB     (truncate tail from left)
// - ( 0, -3)   -> ABCD   (truncate tail from right)
// - (-3, -2)   ->    DE  (mix it as you desire)
// - ( 0, -1)   -> ABCDEF (exact duplicate)
// - ( 1,  0)   ->        (empty array because of desired size == 0)
// - ( 2, -5)   ->        (empty array because of resulting size == 0)
// - (-9,  9)   ->        (empty array with warning when debugging)
//
// If dest is equal to src and if the resulting array is empty, the storage
// is automatically detached and freed if no longer used.
//
// Warning: srcarray may be const but if dstarray is the same as srcarray,
// srcarray may nontheless be altered!!!
NA_API NAByteArray* naInitByteArrayExtraction( NAByteArray* dstarray,
                                            const NAByteArray* srcarray,
                                                         NAInt offset,
                                                         NAInt size);

// Clears the given array. Any previously used storage is detached.
// When the storage is no longer used, it gets deleted automatically.
// The given array is invalid after this function.
NA_IAPI void naClearByteArray  (NAByteArray* array);

// Empties the array. Does almost the same as naClearByteArray but as opposed
// to naClearByteArray, the array can still be valid after this function and
// represents an empty array.
NA_IAPI void naEmptyByteArray  (NAByteArray* array);

// COPIES the contents of the array to a separate storage and decouples it
// from the existing storage. When the existing storage is no longer used, it
// gets deleted automatically.
// If appendnulltermination is true, a certain number of bytes are added at the
// end of the newly created byte array and are initialized with binary zero.
// The specific number of bytes is defined by the naInitByteArrayWithSize
// function. See there for more information. If appendnulltermination is false,
// no bytes are appended. The returned ByteArray itself will NOT contain any
// additional zero bytes but instead have the size of the original array.
//
// COPIES ALWAYS!
//
// This function is necessary as naInitByteArrayExtraction will not copy any
// content but instead, reference everything on one common storage. This is
// useful when parsing a large string for example. But ultimatively, the user
// may want to access or alter the sub-arrays independently of the original
// storage. Therefore, naDecoupleByteArray needs to be called to detach
// the sub-array from the existing storage.
//
// The internal iteration index will be reset.
NA_API void naDecoupleByteArray(NAByteArray* array,
                                      NABool appendnulltermination);


// ///////////////////////////////
// Getter functions
//

// Returns a pointer to the very first byte of the raw data array. Warning:
// result is garbage if the array is empty. Notice: This function is speedy.
NA_IAPI const NAByte* naGetByteArrayConstPointer  (const NAByteArray* array);
NA_IAPI       NAByte* naGetByteArrayMutablePointer(      NAByteArray* array);

// Returns a POINTER to the byte at the given index. If index is negative, it
// is searched from the end of the array. For example -1 denotes the last byte.
// Warning: result is garbage if the array is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetByteArrayPointer or this
// function and use pointer arithmetics afterwards.
NA_IAPI const NAByte* naGetByteArrayConstByte  (const NAByteArray* array,
                                                             NAInt indx);
NA_IAPI       NAByte* naGetByteArrayMutableByte(      NAByteArray* array,
                                                             NAInt indx);

// Returns the number of bytes in this array. Note that the function always
// returns a positive number even if the array was created with a negative one.
// This function is speedy!
// The MaxIndex function returns size-1 but will emit an error when NDEBUG is
// undefined and size is 0.
NA_IAPI NAUInt naGetByteArraySize(const NAByteArray* array);
NA_IDEF NAUInt naGetByteArrayMaxIndex(const NAByteArray* array);

// Returns true if the array is empty. This is a convenience function which
// does the same thing as calling naGetByteArraySize and testing for size == 0.
// Sometimes, it helps to have english plain text in the API.
NA_IAPI NABool naIsByteArrayEmpty(const NAByteArray* array);


#ifndef NDEBUG
  // Returns true if the array is zero-terminated beyound the actual array.
  NA_IDEF NABool naIsByteArrayNullTerminated(const NAByteArray* array);
  // This function is only available when debugging. It mainly exists for the
  // sole purpose of detecting when an NAString tries to convert to a C-string
  // and is not null-terminated. When NDEBUG is defined, this function does no
  // longer exist and the test of NAString will simply not be performed.
#endif


// ////////////////////////////////////
// Iteration functions
//
// Every NAByteArray has an internal index denoting the current byte.
// The programmer can control and access this byte with iteration functions.
// If no current byte is set, NA_NULL is returned as a pointer. A typical
// example of iteration is the following:
//
// NAByteArray* myarray;
// NAByte* curbyte;
// naFirstByteArray(myarray);
// while((curbyte = naIterateByteArrayMutable(myarray, 1))){
//   Do stuff with curbyte.
// }
//
// You should enclose the while-condition in additional parantheses such that
// a compiler knows that the returned pointer must be evaluated as a condition.
//
// Note that all iteration functions are inline. They are somewhat fast. But
// walking through an array using pointer arithmetic is still faster as there
// are quite a few if conditions to check when iterating. An iterator has some
// advantages in certain situations though, as the position within the array
// is stored persistently.
//
// When being inside the while scope, the array itself already points to the
// byte AFTER iteration.
//
// Do NOT use a for-loop for iteration! Although it would not matter much with
// the NAByteArray structure, it nontheless should be kept consistent with
// other structures using iteration functions, see NAList for example.
// Also, it is very hard to read.

// With the following functions, you can initialize the internal pointer.
// Note that this function will set the internal pointer no matter if the
// array is empty or not. The resulting internal index may in a successing
// function call be bogus.
NA_IAPI void naFirstByteArray                  (const NAByteArray* array);
NA_IAPI void naLastByteArray                   (const NAByteArray* array);

// Returns a pointer to the current byte in retbyte and iterates the given
// bytes forward or backwards by using positive or negative numbers. If the
// returned byte is valid (inside the range of the array), the function returns
// NA_TRUE. If it is not valid, the function returns NA_NULL, but the returned
// pointer in retbyte will be computed just as if it was valid.
//
// If step is 0, you simply access the current byte. Note that for accessing
// the current byte, the use of naGetByteArrayCurrent would be preferable.
NA_IAPI const NAByte* naIterateByteArrayConst    (const NAByteArray* array,
                                                        NAInt step);
NA_IAPI NAByte* naIterateByteArrayMutable  (      NAByteArray* array,
                                                        NAInt step);

// Returns a pointer to the current byte without moving the internal index.
NA_IAPI const void* naGetByteArrayCurrentConst     (const NAByteArray* array);
NA_IAPI       void* naGetByteArrayCurrentMutable   (const NAByteArray* array);

// Returns the current index.
NA_IAPI       NAUInt naGetByteArrayCurrentIndex     (const NAByteArray* array);
// Returns the remaining number of bytes after the current index. Will emit
// a warning if the index is unset.
NA_IAPI       NAUInt naGetByteArrayRemainingSize    (const NAByteArray* array);

// The locate-function set the internal pointer to the index given. If the
// given index is negative, it denotes the byte from the end of the array,
// whereas -1 denotes the last byte.
//
// Note that this function will set the internal pointer no matter if the
// desired index is inside the array range or not. But the function will
// return NA_TRUE if the index was inside range and NA_FALSE if not.
NA_IAPI NABool naLocateByteArrayIndex          (const NAByteArray* array,
                                                             NAInt indx);

// Moves the internal pointer forward or backwards without accessing the
// content.
NA_IAPI void naIterateByteArray                (const NAByteArray* array,
                                                             NAInt step);

// Returns whether the array is at a certain position.
// This functions return garbage if the array is empty!
// Returns NA_FALSE when the internal index is not set.
NA_IAPI NABool naIsByteArrayAtFirst            (const NAByteArray* array);
NA_IAPI NABool naIsByteArrayAtLast             (const NAByteArray* array);




// Inline implementations are in a separate file:
#include "NAStruct/NAByteArrayII.h"



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BYTE_ARRAY_INCLUDED


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
