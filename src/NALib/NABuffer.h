
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_LINEAR_MEMORY_INCLUDED
#define NA_LINEAR_MEMORY_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAFile.h"


// This file contains the API for NABuffer and its underlying storage structure
// NAASDFBuffer.
//
// You need both structures as the NAASDFBuffer defines, where a buffer reads
// and writes bytes.
//
// A buffer source defines linear memory which is filled with the contents of
// a specified source. This source can be nothing, a file, an existing C-array,
// and more. The buffer source guarantees, that each byte of that source has
// its clearly defined position in memory.
//
// An NAASDFBuffer structure is capable of storing sparse memory meaning as
// long as a part of the memory is not explicitely needed, no memory at that
// position is allocated. This means for example that you can open a file which
// has a total filesize of 1GB but you only access a few bytes resulting in
// only needing a few bytes in memory. Even if these bytes are scattered around
// multiple positions in the file, only those parts of the file are read which
// are actually needed.
//
// Those parts which are allocated have a default size. That size can be
// defined in NAConfiguration.h with the NA_BUFFER_PART_BYTESIZE macro.
//
// An NABuffer structure uses an NAASDFBuffer structure as its source and
// "borrows" the contents of the linear memory, arbitrarily positioned. For
// example: The source of a file looks like this:
//
// +-0---------------------------+
// | This is the text of a file. |
// +-------------^---------------+
//               |
// A buffer may choose to use the bytes 12 - 15 of that source but placing it
// at its own absolute position 0.
//               |
//   +-----------+
// +-0----+   
// | text |  The content of the buffer is "text"
// +------+
//
// The important thing is, that memory will not be copied but only referenced
// (hence being very fast) and stays in memory as long as some structure needs
// it. This means, you can read a whole file, pick the contents you need to
// to keep and close the file source again, resulting in all memory deallocated
// which is not needed anymore and only leaving the memory active which is
// actually in use. 
//
// In version 19, buffers will be able to use multiple sources and stitch
// together pieces arbitrarily. Like for example:
//
// +-0---------------------------+  +-0------------------------------------+
// | This is the text of a file. |  | I am a const char* string in srccode |
// +-------------^---------------+  +--------------------^-----------------+
//               |                                       |
//   +-----------+     +---------------------------------+
// +-0----+          +-4------+
// | text |----------| string |   The content of the buffer is "textstring"
// +------+          +--------+
//






// /////////////////////////////////
// NABuffer
// /////////////////////////////////


typedef struct NAASDFBuffer NAASDFBuffer;


// Allocates a buffer with a specific source.
NA_API NAASDFBuffer* naCreateASDFBuffer();
NA_DEF NAASDFBuffer* naCreateASDFBufferExtraction(NAASDFBuffer* srcbuf, NARangei range);
NA_API NAASDFBuffer* naCreateASDFBufferFile(     const char* filename);
NA_API NAASDFBuffer* naCreateASDFBufferConstData(const void* data,
                                                           NAInt bytesize);
NA_API NAASDFBuffer* naCreateASDFBufferMutableData(    void* data,
                                                           NAInt bytesize,
                                                 NAMemoryCleanup cleanup);

// Retains and releases a buffer source
NA_API NAASDFBuffer*  naRetainASDFBuffer (NAASDFBuffer* source);
NA_API void           naReleaseASDFBuffer(NAASDFBuffer* source);


NA_API NARangei naDetermineBufferRange      (NAASDFBuffer* buffer);
NA_API NABool   naHasBufferDeterminedRange  (const NAASDFBuffer* buffer);
NA_API NARangei naGetASDFBufferRange        (const NAASDFBuffer* buffer);
NA_API NABool   naIsASDFBufferEmpty         (const NAASDFBuffer* buffer);
NA_API NAInt    naTellASDFBuffer            (const NAASDFBuffer* buffer);
NA_API NABool   naIsBufferAtEnd             (const NAASDFBuffer* buffer);

NA_API void naSetBufferEndianness(NAASDFBuffer* buffer, NAInt endianness);
NA_API NAInt naGetBufferEndianness(NAASDFBuffer* buffer);

NA_API void naCacheASDFBuffer(NAASDFBuffer* buffer, NARangei range);


// Seeks the given position within the buffer.
// Absolute searches relative to the zero-point of this buffer.
// Relative searches relative to the current position of this buffer.
// Source   searches relative to the zero-point of the source. (which currently
//          does the same as Absolute, waiting for version 19 to be useful).
NA_API void naSeekASDFBufferAbsolute(NAASDFBuffer* buffer, NAInt offset);
NA_API void naSeekASDFBufferRelative(NAASDFBuffer* buffer, NAInt offset);
NA_API void naSeekASDFASDFBuffer(NAASDFBuffer* buffer, NAInt offset);



// ////////////////////////////////
// BUFFER WRITING
//
// The writing functions each write data to the buffer. The data is written at
// the current position of the buffer and the current position is automatically
// moved forward the specified number of bytes.
//
// Usually, you do not need to worry about memory, which is managed
// automatically by the buffer structure.

// Writes the given number of bytes in data to the current position of buffer.
NA_API void naWriteASDFBufferBytes(NAASDFBuffer* buffer, const void* data, NAInt bytesize);

NA_API void naWriteBufferi8   (NAASDFBuffer* buffer, int8   value);
NA_API void naWriteBufferi16  (NAASDFBuffer* buffer, int16  value);
NA_API void naWriteBufferi32  (NAASDFBuffer* buffer, int32  value);
NA_API void naWriteBufferi64  (NAASDFBuffer* buffer, int64  value);
NA_API void naWriteBufferu8   (NAASDFBuffer* buffer, uint8  value);
NA_API void naWriteBufferu16  (NAASDFBuffer* buffer, uint16 value);
NA_API void naWriteBufferu32  (NAASDFBuffer* buffer, uint32 value);
NA_API void naWriteBufferu64  (NAASDFBuffer* buffer, uint64 value);
NA_API void naWriteBufferf    (NAASDFBuffer* buffer, float  value);
NA_API void naWriteBufferd    (NAASDFBuffer* buffer, double value);

NA_API void naWriteBufferi8v  (NAASDFBuffer* buffer, const int8*   src, NAInt count);
NA_API void naWriteBufferi16v (NAASDFBuffer* buffer, const int16*  src, NAInt count);
NA_API void naWriteBufferi32v (NAASDFBuffer* buffer, const int32*  src, NAInt count);
NA_API void naWriteBufferi64v (NAASDFBuffer* buffer, const int64*  src, NAInt count);
NA_API void naWriteBufferu8v  (NAASDFBuffer* buffer, const uint8*  src, NAInt count);
NA_API void naWriteBufferu16v (NAASDFBuffer* buffer, const uint16* src, NAInt count);
NA_API void naWriteBufferu32v (NAASDFBuffer* buffer, const uint32* src, NAInt count);
NA_API void naWriteBufferu64v (NAASDFBuffer* buffer, const uint64* src, NAInt count);
NA_API void naWriteBufferfv   (NAASDFBuffer* buffer, const float*  src, NAInt count);
NA_API void naWriteBufferdv   (NAASDFBuffer* buffer, const double* src, NAInt count);

NA_API void naWriteBufferBuffer(NAASDFBuffer* dstbuffer, NAASDFBuffer* srcbuffer, NARangei srcrange);
NA_API void naRepeatBufferBytes(NAASDFBuffer* buffer, NAInt distance, NAInt bytesize);
NA_API void naWriteBufferToFile(NAASDFBuffer* buffer, NAFile* file);

// ////////////////////////////////
// BUFFER READING
//

// Reads the given number of bytes from the current position of buffer to data.
NA_API void naReadASDFBufferBytes(NAASDFBuffer* buffer, void* data, NAInt bytesize);

NA_API int8   naReadBufferi8  (NAASDFBuffer* buffer);
NA_API int16  naReadBufferi16 (NAASDFBuffer* buffer);
NA_API int32  naReadBufferi32 (NAASDFBuffer* buffer);
NA_API int64  naReadBufferi64 (NAASDFBuffer* buffer);
NA_API uint8  naReadBufferu8  (NAASDFBuffer* buffer);
NA_API uint16 naReadBufferu16 (NAASDFBuffer* buffer);
NA_API uint32 naReadBufferu32 (NAASDFBuffer* buffer);
NA_API uint64 naReadBufferu64 (NAASDFBuffer* buffer);
NA_API float  naReadBufferf   (NAASDFBuffer* buffer);
NA_API double naReadBufferd   (NAASDFBuffer* buffer);

NA_API void naReadBufferi8v (NAASDFBuffer* buffer, int8*   dst, NAInt count);
NA_API void naReadBufferi16v(NAASDFBuffer* buffer, int16*  dst, NAInt count);
NA_API void naReadBufferi32v(NAASDFBuffer* buffer, int32*  dst, NAInt count);
NA_API void naReadBufferi64v(NAASDFBuffer* buffer, int64*  dst, NAInt count);
NA_API void naReadBufferu8v (NAASDFBuffer* buffer, uint8*  dst, NAInt count);
NA_API void naReadBufferu16v(NAASDFBuffer* buffer, uint16* dst, NAInt count);
NA_API void naReadBufferu32v(NAASDFBuffer* buffer, uint32* dst, NAInt count);
NA_API void naReadBufferu64v(NAASDFBuffer* buffer, uint64* dst, NAInt count);
NA_API void naReadBufferfv  (NAASDFBuffer* buffer, float*  dst, NAInt count);
NA_API void naReadBufferdv  (NAASDFBuffer* buffer, double* dst, NAInt count);


NA_API NABool naReadBufferBit (NAASDFBuffer* buffer);
NA_API NAUInt naReadBufferBits(NAASDFBuffer* buffer, uint8 count);
NA_API void   naPadBufferBits (NAASDFBuffer* buffer);



NA_API void naAccumulateBufferToChecksum(NAASDFBuffer* buffer, NAChecksum* checksum);



// Inline implementations are in a separate file:
#include "NAStruct/NABufferII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_LINEAR_MEMORY_INCLUDED

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
