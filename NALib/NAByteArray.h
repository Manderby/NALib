
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
// size.
//
// Multiple NAByteArrays can point to the same storage. Meaning: Multiple
// NAByteArrays can point to different sections of the same byte stream by
// providing an offset and a size relative to another NAByteArray. This allows
// for very fast and convenient parsing of byte streams without copying
// the bytes over and over again. This struct is also used by NAString and
// NABitArray


#include "NASystem.h"

// This macro can be used to indicate an invalid index into an array.
#define NA_INVALID_INDEX -1

// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Creates or fills a new, EMPTY ByteArray and returns the pointer to the
// NAByteArray struct. Other than the struct itself, no additional memory will
// be allocated.
NA_IAPI NAByteArray* naCreateByteArray(NAByteArray* array);

// Creates or fills a new ByteArray with the desired size and returns the
// pointer. Allocates memory on the heap which will be freed automatically when
// the array storage is no longer used.
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
// The returned ByteArray itself will NOT contain any additional bytes but
// instead have the exact same size as given as argument.
//
// The negative size is used for example for zero-terminated strings or can
// be used for example for sentinels in pointer arrays. There, with negative
// size, it is safe to read past the end of an array as you can be sure that
// at least one pointer size after the array is filled with binary zero.
NA_IAPI NAByteArray* naCreateByteArrayWithSize(NAByteArray* array,
                                                      NAInt size);

// Creates or fills a new ByteArray which contains the data of the given
// buffer WITHOUT copying. The size denotes the size of the buffer in bytes.
// The programmer is responsible for that size does not overflows the buffer.
// When takeownership is set to NA_TRUE, the array will deallocate the given
// buffer with free() when it is no longer used. Note that size can not be
// negative. If size is null, an empty array is created and the buffer is left
// completely untouched and will never be deleted, even if takeownership is
// true.
// There are two creation functions, one for const data and one for non-const.
//
// Use these functions to encapsulate your own raw buffers into an NAByteArray!
NA_IAPI NAByteArray* naCreateByteArrayWithConstBuffer(
                                                NAByteArray* array,
                                                 const void* buffer,
                                                       NAInt size);
NA_IAPI NAByteArray* naCreateByteArrayWithMutableBuffer(
                                                NAByteArray* array,
                                                       void* buffer,
                                                       NAInt size,
                                                      NABool takeownership);

// Function naCreateByteArrayExtraction:
// Creates or fills a new ByteArray with the contents of srcarray. The content
// is NOT copied, but the dest array points to the same storage as srcarray
// with an offset and size relative to the src array.
//
// Use this function to perform all sorts of manipulations (examples below):
// - dstarray can be the same as srcarray
// - if offset is negative, it denotes the number of bytes from the end.
//   Note that the end has index [size], meaning -1 denotes the index [size-1]
//   which is the last element.
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
// - ( 2,  0)   ->   CDEF (truncate head from left)
// - (-2,  0)   ->     EF (truncate head from right)
// - ( 0,  2)   -> AB     (truncate tail from left)
// - ( 0, -3)   -> ABCD   (truncate tail from right)
// - (-3, -2)   ->    DE  (mix it as you desire)
// - ( 0, -1)   -> ABCDEF (exact duplicate)
// - ( 1,  0)   ->        (empty array because of desired size = 0)
// - ( 2, -5)   ->        (empty array because of resulting size = 0)
// - (-9,  9)   ->        (empty array with warning when debugging)
//
// If dest ist equal to src and if the resulting array is empty, the storage
// is automatically detached and freed if no longer used.
//
// Warning: srcarray may be const but if dstarray ist the same as srcarray,
// srcarray may nontheless be altered!!!
NA_IAPI NAByteArray* naCreateByteArrayExtraction( NAByteArray* dstarray,
                                            const NAByteArray* srcarray,
                                                         NAInt offset,
                                                         NAInt size);

// Clears or Deletes the given array. Any previously used storage is detached.
// When the storage is no longer used, it gets deleted automatically.
NA_IAPI void naClearByteArray  (NAByteArray* array);
NA_IAPI void naDestroyByteArray(NAByteArray* array);

// COPIES the contents of the array to a separate storage and decouples it
// from the existing storage. When the existing storage is no longer used, it
// gets deleted automatically.
// If appendzerobytes is true, a specific number of bytes are added at the end
// and initialized with binary zero. The specific number of bytes is defined by
// the naCreateByteArrayWithSize function. See there for more information. If
// appendzerobytes is false, no bytes are appended. The returned ByteArray
// itself will NOT contain any additional zero bytes but instead have the size
// of the original array.
//
// COPIES ALWAYS!
NA_IAPI void naDecoupleByteArray(NAByteArray* array,
                                       NABool appendzerobytes);

// Returns a pointer to the very first byte of the raw data array. Warning:
// result is garbage if the array is empty. Notice: This function is speedy.
NA_IAPI const NAByte* naGetByteArrayConstPointer  (const NAByteArray* array);
NA_IAPI       NAByte* naGetByteArrayMutablePointer(      NAByteArray* array);

// Returns a POINTER to the byte at the given index. If index is negative, it
// is searched from the end of the array. For example -1 denotes the last byte.
// Warning: result is garbage if the array is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetByteArrayPointer or this
// function and use pointer arithmetic afterwards.
NA_IAPI const NAByte* naGetByteArrayConstByte  (const NAByteArray* array,
                                                             NAInt indx);
NA_IAPI       NAByte* naGetByteArrayMutableByte(      NAByteArray* array,
                                                             NAInt indx);

// Returns the number of bytes in this array.
NA_IAPI NAInt naGetByteArraySize(const NAByteArray* array);

// Returns true if the array is empty.
NA_IAPI NABool naIsByteArrayEmpty(const NAByteArray* array);


















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAPointer.h"
#include "NABinaryData.h"
#include "NARange.h"


struct NAByteArray{
  NAPointer* storage;       // pointer to the storage of all bytes
  union{                    // pointer to the first byte of this NAByteArray...
    const NAByte* constp;   // ... which is either const ...
          NAByte* p;        // ... or non-const.
  } ptr;
  NAInt size;               // size of this NAByteArray in bytes
};
// Note that ptr may not point to the same address as the data pointer stored
// in the storage!
//
// If size is zero, the ByteArray is considered empty. In that case, the
// other fields contain no useful information.



NA_IDEF NAByteArray* naCreateByteArray(NAByteArray* array){
  array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
  naNulli(&(array->size));
  return array;
}


NA_IDEF NAByteArray* naCreateByteArrayWithSize(NAByteArray* array,
                                                      NAInt size){
  NAInt addsize;
  if(!size){  // if size is zero
    array = naCreateByteArray(array);
  }else{
    array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
    if(size>0){
      array->storage = naCreatePointerWithSize(NA_NULL, size);
    }else{
      // When size is negative, a number of bytes is appended to the array which
      // will be initialized with binary zero but will not be visible to the
      // programmer. The following holds true:
      // - The additional bytes are all guaranteed to be initialized with binary
      //   zero.
      // - There are at least as many bytes appended as a address requires.
      //   Or more precise: 4 Bytes on 32 Bit systems, 8 Bytes on 64 Bit systems
      // - There are as many bytes allocated such that the total size is a
      //   multiple of an address size, meaning 4 or 8 Bytes depending on the
      //   system (32 Bit or 64 Bit).
      // - The total size (including the desired space) is at minimum 2 times
      //   the number of bytes needed for an address.
      // - The part without the additional bytes might partially become
      //   initialized with binary zero.
      size = -size;
      addsize = 2 * NA_SYSTEM_ADDRESS_BYTES - (size % NA_SYSTEM_ADDRESS_BYTES);
      array->storage = naCreatePointerWithSize(NA_NULL, size + addsize);
      
      array->ptr.p = (NAByte*)naGetPointerMutableData(array->storage);
      naNulli(&(array->ptr.p[size + addsize - 2 * NA_SYSTEM_ADDRESS_BYTES]));
      naNulli(&(array->ptr.p[size + addsize - NA_SYSTEM_ADDRESS_BYTES]));
    }
    array->ptr.p = (NAByte*)naGetPointerMutableData(array->storage);
    array->size = size;
  }
  
  return array;
}



NA_IDEF NAByteArray* naCreateByteArrayWithMutableBuffer(
                                                NAByteArray* array,
                                                       void* buffer,
                                                       NAInt size,
                                                      NABool takeownership){
  #ifndef NDEBUG
    if(size < 0)
      naError("naCreateByteArrayWithBuffer", "size is negative.");
    if(!buffer)
      naError("naCreateByteArrayWithBuffer", "buffer is Null-Pointer.");
  #endif
  if(!size){  // if size is zero
    #ifndef NDEBUG
      if(takeownership)
        naError("naCreateByteArrayWithBuffer", "Can not take ownership of buffer with size 0.");
    #endif
    array = naCreateByteArray(array);
  }else{
    array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
    array->storage = naCreatePointerWithMutableBuffer(NA_NULL, buffer, takeownership);
    array->ptr.p = (NAByte*)naGetPointerMutableData(array->storage);
    array->size = size;
  }
  return array;
}



NA_IDEF NAByteArray* naCreateByteArrayWithConstBuffer(
                                                NAByteArray* array,
                                                 const void* buffer,
                                                       NAInt size){
  #ifndef NDEBUG
    if(size < 0)
      naError("naCreateByteArrayWithConstBuffer", "size is negative.");
    if(!buffer)
      naError("naCreateByteArrayWithConstBuffer", "buffer is Null-Pointer.");
  #endif
  if(!size){  // if size is zero
    array = naCreateByteArray(array);
  }else{
    array = (NAByteArray*)naAllocateIfNull(array, sizeof(NAByteArray));
    array->storage = naCreatePointerWithConstBuffer(NA_NULL, buffer);
    array->ptr.constp = (const NAByte*)naGetPointerConstData(array->storage);
    array->size = size;
  }
  return array;
}





NA_IDEF NAByteArray* naCreateByteArrayExtraction(
                                          NAByteArray* dstarray,
                                    const NAByteArray* srcarray,
                                                 NAInt offset,
                                                 NAInt size){
  NAByte* newptr;
  NAInt newsize;

  #ifndef NDEBUG
    if(!srcarray)
      {naCrash("naCreateByteArrayExtraction", "Source is Null-Pointer."); return NA_NULL;}
  #endif

  dstarray = (NAByteArray*)naAllocateIfNull(dstarray, sizeof(NAByteArray));
  // Note that dstarray may be equal to srcarray.

  // First, set the offset and compute the size with the new offset.
  newptr = srcarray->ptr.p + offset;
  newsize = naGetByteArraySize(srcarray) - offset;

  if(offset < 0){
    // If offset is negative
    newptr += naGetByteArraySize(srcarray);
    newsize -= naGetByteArraySize(srcarray);
  }

  if(newsize < 0){
    #ifndef NDEBUG
      naError("naCreateByteArrayExtraction", "Invalid offset leads to array overflow. Correcting to empty array.");
    #endif
    newsize = 0;
  }else if(newsize > naGetByteArraySize(srcarray)){
    #ifndef NDEBUG
      naError("naCreateByteArrayExtraction", "Invalid offset leads to array underflow. Correcting to empty array.");
    #endif
    newsize = 0;
  }else{
    // The offset is valid. Now, adjust the size.
    if(size < 0){ // negative size parameter
      newsize += size + 1;
      if(newsize < 0){
        // When the resulting size is smaller than 0, underflow.
        #ifndef NDEBUG
          naError("naCreateByteArrayExtraction", "Invalid size leads to array underflow. Correcting to empty array.");
        #endif
        newsize = 0;
      }
    }else{ // positive size parameter
      if(size <= newsize){
        newsize = size;
      }else{
        // When the desired size is bigger than the size available, overflow.
        #ifndef NDEBUG
          naError("naCreateByteArrayExtraction", "Invalid size leads to array overflow. Correcting to empty array.");
        #endif
        newsize = 0;
      }
    }
  }

  if(!newsize){
    // If the extraction results in an empty array...
    if(dstarray == srcarray){
      naClearByteArray(dstarray); // clear the old array.
      naCreateByteArray(dstarray);
    }else{
      naCreateByteArray(dstarray);
    }
  }else{
    // The resulting array has content!
    if(dstarray != srcarray){
      dstarray = naCreateByteArray(dstarray);
      dstarray->storage = naRetainPointer(srcarray->storage);
    }
    dstarray->ptr.p = newptr;
    dstarray->size = newsize;
  }
  
  return dstarray;
}



NA_IDEF void naClearByteArray(NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naClearByteArray", "array is Null-Pointer."); return;}
  #endif
  if(!naIsByteArrayEmpty(array)){naReleasePointer(array->storage);}
}


NA_IDEF void naDestroyByteArray(NAByteArray* array){
  naClearByteArray(array);
  free(array);
}



NA_IDEF void naDecoupleByteArray(NAByteArray* array,
                                       NABool appendzerobytes){
  // Declaration before implementation. Needed for C90.
  NAInt arraysize;
  NAByteArray* newarray;
  #ifndef NDEBUG
    if(!array)
      {naCrash("naDecoupleByteArray", "array is Null-Pointer."); return;}
  #endif
  // Note: Do not use realloc as ptr may point to a subset of NAPointer.
  // Instead, create a new object and copy manually.
  arraysize = naGetByteArraySize(array);
  if(!arraysize){return;}
  if(appendzerobytes){
    newarray = naCreateByteArrayWithSize(NA_NULL, -arraysize);
  }else{
    newarray = naCreateByteArrayWithSize(NA_NULL, arraysize);
  }
  naCpyn(newarray->ptr.p, array->ptr.constp, arraysize);
  naClearByteArray(array);
  array->ptr = newarray->ptr;
  array->size = newarray->size;
  array->storage = newarray->storage;
}



NA_IDEF const NAByte* naGetByteArrayConstPointer(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetByteArrayConstPointer", "array is Null-Pointer."); return NA_NULL;}
    if(naIsByteArrayEmpty(array))
      naError("naGetByteArrayConstPointer", "array is empty");
  #endif
  return array->ptr.constp;
}

NA_IDEF NAByte* naGetByteArrayMutablePointer(NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetByteArrayMutablePointer", "array is Null-Pointer."); return NA_NULL;}
    if(naIsByteArrayEmpty(array))
      naError("naGetByteArrayMutablePointer", "array is empty");
  #endif
  return array->ptr.p;
}




NA_IDEF NAByte* naGetByteArrayMutableByte(NAByteArray* array, NAInt indx){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetByteArrayMutableByte", "array is Null-Pointer."); return NA_NULL;}
    if(naIsByteArrayEmpty(array))
      naError("naGetByteArrayMutableByte", "array is empty");
  #endif
  if(indx < 0){indx += naGetByteArraySize(array);}
  #ifndef NDEBUG
    if(!naInsidei(0, naGetByteArraySize(array), indx))
      naError("naGetByteArrayMutableByte", "indx out of bounds");
  #endif
  return &(array->ptr.p[indx]);
}

NA_IDEF const NAByte* naGetByteArrayConstByte(const NAByteArray* array,
                                                           NAInt indx){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetByteArrayConstByte", "array is Null-Pointer."); return NA_NULL;}
    if(naIsByteArrayEmpty(array))
      naError("naGetByteArrayConstByte", "array is empty");
  #endif
  if(indx < 0){indx += naGetByteArraySize(array);}
  #ifndef NDEBUG
    if(!naInsidei(0, naGetByteArraySize(array), indx))
      naError("naGetByteArrayConstByte", "indx out of bounds");
  #endif
  return &(array->ptr.constp[indx]);
}



NA_IDEF NAInt naGetByteArraySize(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naGetByteArraySize", "array is Null-Pointer."); return 0;}
    if(array->size < 0)
      naError("naGetByteArraySize", "returned size is negative. Uninitialized struct?");
  #endif
  return array->size;
}


NA_IDEF NABool naIsByteArrayEmpty(const NAByteArray* array){
  #ifndef NDEBUG
    if(!array)
      {naCrash("naIsByteArrayEmpty", "array is Null-Pointer."); return 0;}
    if(array->size < 0)
      naError("naIsByteArrayEmpty", "size is negative. Uninitialized struct?");
  #endif
  return (array->size == 0);
}


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
