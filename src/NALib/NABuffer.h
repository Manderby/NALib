
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_BUFFER_INCLUDED
#define NA_BUFFER_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// The typedef needs to be here to resolve cyclic include problems.
typedef struct NABuffer NABuffer;
typedef struct NABufferIterator NABufferIterator;



#include "NASystem.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAFile.h"


// This file contains the API for NABuffer and its underlying storage structure
// NABuffer.
//
// You need both structures as the NABuffer defines, where a buffer reads
// and writes bytes.
//
// A buffer source defines linear memory which is filled with the contents of
// a specified source. This source can be nothing, a file, an existing C-array,
// and more. The buffer source guarantees, that each byte of that source has
// its clearly defined position in memory.
//
// An NABuffer structure is capable of storing sparse memory meaning as
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
// An NABuffer structure uses an NABuffer structure as its source and
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





// Prototype for the fill function.
// The function has to perform the tast to fill range.length bytes at dst with
// content. The data will be the pointer given in the descriptor and the origin
// of the desired data is given by range.origin.
typedef void (*NABufferSourceFiller)(void* data, void* dst, NARangei range);

// Flags for the buffer source:
#define NA_BUFFER_SOURCE_RANGE_LIMITED    0x01

// Descriptor for a custom source.
//
// You can create a buffer from any linear source you want by filling out a
// descriptor and use it for naCreateBufferCustomSource.
typedef struct NABufferSourceDescriptor NABufferSourceDescriptor;
struct NABufferSourceDescriptor{
  void*                data;        // ptr being sent to filler and destructor.
  NAMutator            destructor;  // Destructor when source no longer needed.
  NABufferSourceFiller filler;      // Fill function filling memory.
  NAUInt               flags;       // Flags for the source
  NARangei             limit;       // Source limit (only used if flags set)
};




// /////////////////////////////////
// NABuffer
// /////////////////////////////////



// ////////////////////////////////////////
// Buffer creation, Retaining and Releasing
// ////////////////////////////////////////

// Allocates an empty buffer.
// When securememory is set to NA_TRUE, each byte will be set to binary zero
// before it can be used.
NA_API NABuffer* naCreateBuffer(NABool securememory);

// Creates a buffer referencing a subrange of another buffer. The origin of
// the new buffer will be at zero and its range is fixed. Does NOT copy any
// content, only references it.
NA_API NABuffer* naCreateBufferExtraction( NABuffer* srcbuffer,
                                            NARangei range);

// Creates a buffer having an exact copy of the bytes in srcbuffer within the
// given range. The new content will be COPIED to a new memory buffer which can
// be chosen to be either secure or not. The origin of the new buffer will be
// at zero.
// All content will be loaded in the new buffer. If there are sparse parts
// in the src buffer, they will be filled with the current source.
NA_API NABuffer* naCreateBufferCopy(const NABuffer* srcbuffer,
                                            NARangei range,
                                              NABool securememory);

// Creates a buffer with no source and no content. It can be used as a
// placeholder or to collect other buffers with naAppendBuffer or
// naWriteBufferBuffer.
NA_API NABuffer* naCreateBufferPlain();

// Creates a buffer sharing the same source as the given buffer.
NA_API NABuffer* naCreateBufferWithSameSource(  NABuffer* srcbuffer);
                    
// Creates a buffer inputting contents from a file. Its origin is always at
// zero and its range is fixed.
NA_API NABuffer* naCreateBufferWithInpuFile(const char* filename);

// Creates a buffer accessing already existing const or mutable data. If the
// data is mutable, you can give a cleanup method if you want to delete the
// memory of the data pointer when no longer needed.
NA_API NABuffer* naCreateBufferWithConstData( const void* data,
                                                    NAInt bytesize);
NA_API NABuffer* naCreateBufferWithMutableData(     void* data,
                                                    NAInt bytesize,
                                          NAMemoryCleanup cleanup);

// Creates a buffer with a custom source. Create a descriptor like this:
//   NABufferSourceDescriptor desc;
//   naNulln(&desc, sizeof(desc));
// And fill in the desired values. See NABufferSourceDescriptor for more
// details.
NA_API NABuffer* naCreateBufferWithCustomSource(NABufferSourceDescriptor desc);

// Releases a buffer source
NA_API void      naReleaseBuffer(NABuffer* source);



// ////////////////////////////////////////
// Various buffer functions
// ////////////////////////////////////////


// Gets information about the range of the buffer. The IsAtEnd function should
// only be used on buffers with a fixed range.
NA_API NARangei naGetBufferRange            (const NABuffer* buffer);
NA_API NABool   naIsBufferEmpty             (const NABuffer* buffer);

// Determines the status whether the buffer range is fixed. If you call the fix
// function, the current range of the buffer will be used as the fixed range.
// A buffer with a fixed range can no longer grow.
NA_API void     naFixBufferRange            (NABuffer* buffer);
NA_API NABool   naHasBufferFixedRange       (const NABuffer* buffer);

// Extends the buffers range by the desired bytesize on both ends
NA_API void     naExtendBufferRange         (NABuffer* buffer,
                                                 NAInt bytesatstart,
                                                 NAInt bytesatend);

// Sets or gets the volatile flag of the buffer.
NA_API NABool   naHasBufferVolatileSource(const NABuffer* buffer);
NA_API void     naSetBufferVolatileSource(NABuffer* buffer, NABool volatil);

// Get or set the endianness setting of this buffer. If not stated otherwise,
// the endianness of a new buffer is NA_ENDIANNESS_NATIVE.
NA_API void   naSetBufferEndianness(NABuffer* buffer, NAInt endianness);
NA_API NAInt  naGetBufferEndianness(NABuffer* buffer);

// Returns the byte at the given index. Warning: This function is costly. You
// might want to use one of the Reading or Parsing functions instead.
NA_API NAByte naGetBufferByteAtIndex(const NABuffer* buffer, NAInt indx);
//NA_API NAByte naGetBufferCurrentByte(const NABuffer* buffer);

// Searches for the given byte starting at (and including) startoffset, either
// forward or backwards.
NA_API NAInt naSearchBufferByteOffset(  const NABuffer* buffer,
                                                 NAByte byte,
                                                  NAInt startoffset,
                                                 NABool forward);

// Cache:   Allocates all memory of the desired range and fills it
//          according to the current source.
// Dismiss: Dismisses the bytes of the given range. Use this if you are
//          sure the denoted bytes are no longer in use. This gives
//          NALib the possibility to deallocate memory.
NA_API void naCacheBufferRange(   NABuffer* buffer,
                                   NARangei range,
                                     NABool forcevolatile);
NA_API void naDismissBufferRange( NABuffer* buffer,
                                   NARangei range);

// ////////////////////////////////
// WHOLE BUFFER FUNCTIONS
// ////////////////////////////////

// Compares two buffers. If casesensitive is NA_TRUE, an exact match is tested.
NA_API NABool naEqualBufferToBuffer(  const NABuffer* buffer1,
                                      const NABuffer* buffer2,
                                               NABool casesensitive);

// Assumes data to contain an equal amount of bytes as buffer and compares all
// bytes for equality. If casesensitive is NA_TRUE, an exact match is tested.
NA_API NABool naEqualBufferToData(    const NABuffer* buffer,
                                          const void* data,
                                                NAInt databytesize,
                                               NABool casesensitive);

// Appends the whole content of srcbuffer to the end of dstbuffer.
NA_API void naAppendBufferToBuffer(NABuffer* dstbuffer, const NABuffer* srcbuffer);

// Uses all bytes of the buffer to write to output or use it in other structs.
// File:     Creates a new file and fills it with the content of the buffer.
// Data:     Assumes data to have enough space and fills all bytes inside.
// Checksum: Adds all bytes to the checksum.
NA_API void naWriteBufferToFile(NABuffer* buffer, NAFile* file);
NA_API void naWriteBufferToData(NABuffer* buffer, void* data);
NA_API void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum);



// ////////////////////////////////
// BUFFER ITERATION
// ////////////////////////////////

NA_API NABufferIterator naMakeBufferAccessor(const NABuffer* buffer);
NA_API NABufferIterator naMakeBufferMutator (const NABuffer* buffer);
NA_API NABufferIterator naMakeBufferModifier(      NABuffer* buffer);

NA_API void naClearBufferIterator(NABufferIterator* iter);

// Returns or changes the current position within the buffer.
// Absolute  searches relative to the zero-point of this buffer.
// Relative  searches relative to the current position of this buffer.
// Source    searches relative to the zero-point of the source.
// FromStart searches relative to the start of the buffers range.
// FromEnd   searches relative to the end of the buffers range.
//
// Important: The offset parameter ist always expected to be forward-oriented.
// If you want to seek to the last byte, you would need to write
// naSeekBufferFromEnd(buffer, -1).
NA_API NAInt  naTellBuffer          (const NABufferIterator* iter);
NA_API void   naSeekBufferAbsolute  (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferRelative  (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferSource    (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferFromStart (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferFromEnd   (NABufferIterator* iter, NAInt offset);

NA_API NABool naIterateBuffer(      NABufferIterator* iter, NAInt step);
NA_API NABool naIsBufferAtEnd(      const NABufferIterator* iter);




// ////////////////////////////////
// GET and SET
// ////////////////////////////////

// The following functions will get or set the given type without moving the
// iterator.

NA_API int8   naGetBufferi8  (NABufferIterator* iter);
NA_API int16  naGetBufferi16 (NABufferIterator* iter);
NA_API int32  naGetBufferi32 (NABufferIterator* iter);
NA_API int64  naGetBufferi64 (NABufferIterator* iter);
NA_API uint8  naGetBufferu8  (NABufferIterator* iter);
NA_API uint16 naGetBufferu16 (NABufferIterator* iter);
NA_API uint32 naGetBufferu32 (NABufferIterator* iter);
NA_API uint64 naGetBufferu64 (NABufferIterator* iter);
NA_API float  naGetBufferf   (NABufferIterator* iter);
NA_API double naGetBufferd   (NABufferIterator* iter);

NA_API void naSetBufferi8  (NABufferIterator* iter, int8   value);
NA_API void naSetBufferi16 (NABufferIterator* iter, int16  value);
NA_API void naSetBufferi32 (NABufferIterator* iter, int32  value);
NA_API void naSetBufferi64 (NABufferIterator* iter, int64  value);
NA_API void naSetBufferu8  (NABufferIterator* iter, uint8  value);
NA_API void naSetBufferu16 (NABufferIterator* iter, uint16 value);
NA_API void naSetBufferu32 (NABufferIterator* iter, uint32 value);
NA_API void naSetBufferu64 (NABufferIterator* iter, uint64 value);
NA_API void naSetBufferf   (NABufferIterator* iter, float  value);
NA_API void naSetBufferd   (NABufferIterator* iter, double value);


// ////////////////////////////////
// BUFFER READING
// ////////////////////////////////

// Reads the given number of bytes from the current position of buffer to data.
NA_API void naReadBufferBytes(  NABufferIterator* iter,
                                    void* data,
                                    NAInt bytesize);

NA_API int8   naReadBufferi8  (NABufferIterator* iter);
NA_API int16  naReadBufferi16 (NABufferIterator* iter);
NA_API int32  naReadBufferi32 (NABufferIterator* iter);
NA_API int64  naReadBufferi64 (NABufferIterator* iter);
NA_API uint8  naReadBufferu8  (NABufferIterator* iter);
NA_API uint16 naReadBufferu16 (NABufferIterator* iter);
NA_API uint32 naReadBufferu32 (NABufferIterator* iter);
NA_API uint64 naReadBufferu64 (NABufferIterator* iter);
NA_API float  naReadBufferf   (NABufferIterator* iter);
NA_API double naReadBufferd   (NABufferIterator* iter);

NA_API void naReadBufferi8v (NABufferIterator* iter, int8*   dst, NAInt count);
NA_API void naReadBufferi16v(NABufferIterator* iter, int16*  dst, NAInt count);
NA_API void naReadBufferi32v(NABufferIterator* iter, int32*  dst, NAInt count);
NA_API void naReadBufferi64v(NABufferIterator* iter, int64*  dst, NAInt count);
NA_API void naReadBufferu8v (NABufferIterator* iter, uint8*  dst, NAInt count);
NA_API void naReadBufferu16v(NABufferIterator* iter, uint16* dst, NAInt count);
NA_API void naReadBufferu32v(NABufferIterator* iter, uint32* dst, NAInt count);
NA_API void naReadBufferu64v(NABufferIterator* iter, uint64* dst, NAInt count);
NA_API void naReadBufferfv  (NABufferIterator* iter, float*  dst, NAInt count);
NA_API void naReadBufferdv  (NABufferIterator* iter, double* dst, NAInt count);

NA_API NABuffer* naReadBufferBuffer(  NABufferIterator* iter,
                                                  NAInt bytesize);


// ////////////////////////////////
// BINARY BUFFER WRITING
// ////////////////////////////////

// The writing functions each write data to the buffer. The data is written at
// the current position of the buffer and the current position is automatically
// moved forward the specified number of bytes.
//
// Usually, you do not need to worry about memory, which is managed
// automatically by the buffer structure.

// Writes the given number of bytes in data to the current position of buffer.
NA_API void naWriteBufferBytes( NABufferIterator* iter,
                              const void* data,
                                    NAInt bytesize);

NA_API void naWriteBufferi8  (NABufferIterator* iter, int8   value);
NA_API void naWriteBufferi16 (NABufferIterator* iter, int16  value);
NA_API void naWriteBufferi32 (NABufferIterator* iter, int32  value);
NA_API void naWriteBufferi64 (NABufferIterator* iter, int64  value);
NA_API void naWriteBufferu8  (NABufferIterator* iter, uint8  value);
NA_API void naWriteBufferu16 (NABufferIterator* iter, uint16 value);
NA_API void naWriteBufferu32 (NABufferIterator* iter, uint32 value);
NA_API void naWriteBufferu64 (NABufferIterator* iter, uint64 value);
NA_API void naWriteBufferf   (NABufferIterator* iter, float  value);
NA_API void naWriteBufferd   (NABufferIterator* iter, double value);

NA_API void naWriteBufferi8v (NABufferIterator* iter, const int8*   src, NAInt count);
NA_API void naWriteBufferi16v(NABufferIterator* iter, const int16*  src, NAInt count);
NA_API void naWriteBufferi32v(NABufferIterator* iter, const int32*  src, NAInt count);
NA_API void naWriteBufferi64v(NABufferIterator* iter, const int64*  src, NAInt count);
NA_API void naWriteBufferu8v (NABufferIterator* iter, const uint8*  src, NAInt count);
NA_API void naWriteBufferu16v(NABufferIterator* iter, const uint16* src, NAInt count);
NA_API void naWriteBufferu32v(NABufferIterator* iter, const uint32* src, NAInt count);
NA_API void naWriteBufferu64v(NABufferIterator* iter, const uint64* src, NAInt count);
NA_API void naWriteBufferfv  (NABufferIterator* iter, const float*  src, NAInt count);
NA_API void naWriteBufferdv  (NABufferIterator* iter, const double* src, NAInt count);

NA_API void naWriteBufferBuffer(  NABufferIterator* iter,
                                    const NABuffer* srcbuffer,
                                   NARangei srcrange);
NA_API void naRepeatBufferBytes(  NABufferIterator* iter,
                                      NAInt distance,
                                      NAInt bytesize);



// ////////////////////////////////
// ASCII BUFFER WRITING
// ////////////////////////////////


// Writes a tab or a newline to the file. The newline character is dependent on
// the buffer setting. You can change it using naSetBufferNewLine.
NA_API void naWriteBufferTab(NABufferIterator* iter);
NA_API void naWriteBufferNewLine(NABufferIterator* iter);

// Writes the given string to the buffer, whereas the Line variant appends an
// additional newline according to the settings of the buffer.
// No terminating null-character is written. Existing newline characters within
// the given string are NOT converted!
NA_API void naWriteBufferString(  NABufferIterator* iter,
                                    const NAString* string);
NA_API void naWriteBufferLine(    NABufferIterator* iter,
                                    const NAString* string);

// Writes a string to the buffer which can be written like a printf format. You
// can also use this function just to write a simple const char* string. The
// encoding is UTF-8. The Line variant appends an additional newline according
// to the settings of the buffer.
NA_API void naWriteBufferStringWithFormat( NABufferIterator* iter,
                                           const NAUTF8Char* format,
                                                             ...);
NA_API void naWriteBufferLineWithFormat(   NABufferIterator* iter,
                                           const NAUTF8Char* format,
                                                             ...);

// Same as WithFormat but with an existing va_list argument.
// The argumentlist argument will not be altered by this function.
NA_API void naWriteBufferStringWithArguments( NABufferIterator* iter,
                                              const NAUTF8Char* format,
                                                        va_list argumentlist);
NA_API void naWriteBufferLineWithArguments(   NABufferIterator* iter,
                                              const NAUTF8Char* format,
                                                        va_list argumentlist);





// ////////////////////////////////
// BITWISE MANIPULATION
// ////////////////////////////////

// Each buffer has an internal bit counter allowing you to use single bits
// of the buffer.
//
// Note that most read or write functions require the internal bit counter
// to be aligned to a byte boundary. Use naPadBufferBits to ensure that.

NA_API NABool naReadBufferBit (NABufferIterator* iter);
NA_API NAUInt naReadBufferBits(NABufferIterator* iter, uint8 count);

// Aligns the bit counter to the next byte boundary. If the bit counter is
// already at a boundary, nothing happends.
NA_API void   naPadBufferBits (NABufferIterator* iter);



// ////////////////////////////////
// ASCII BUFFER READING (PARSING)
// ////////////////////////////////

// Terminology:
//
// Note that in this implementation, a buffer has a beginning and an ending.
// The beginning contains the first byte and the ending the last. Words like
// "left" or "right" are misleading especially when using buffers for strings.
// There exist languages where the first character is not left but
// right (for example Arabic) or the characters are written from top to bottom
// (for example Chinese). Words like "leading / trailing" or "head / tail"  or
// "front / back" are too specific or confusing.
//
// We keep it simple here Start and End. Forward and Backward.

// Moves the internal pointer of the buffer forward till the first byte not
// denoting a white space (ord > 32) is found.
NA_API void naSkipBufferWhitespaces(NABufferIterator* iter);

// Moves the internal pointer of the buffer forward such that the delimiter at
// the current position is overjumped. If that delimiter is a whitespace,
// naSkipBufferWhitespaces will be called. In any other case just one byte is
// skipped.
NA_API void naSkipBufferDelimiter(NABufferIterator* iter);

// Returns the next line delimited by CR, LF or CR-LF. The returned line will
// not contain any line-ending characters but all other whitespaces (ord <= 32)
// are still there. When skipempty is NA_TRUE, lines containing nothing but
// whitespaces will be skipped.
// Returns the number of lines read if linesread is not NA_NULL. Will usually
// be 1 but will be greater than 1 when skipping empty lines. Will be 0 when
// the buffer had no lines to read.
NA_API NAString naParseBufferLine( NABufferIterator* iter,
                                      NABool skipempty,
                                      NAInt* linesread);

// Gathers the first token which is delimited by whitespaces. The buffer is
// expected to start at a non-whitespace! If it starts with a whitespace, an
// empty string is returned. In any case, the returned token will not have
// any whitespaces at the start or end.
// After this function, buffer will point to the next character after the
// token not being a whitespace.
NA_API NAString naParseBufferToken(NABufferIterator* iter);

// Gathers the first token within buffer which ends in the given delimiter.
// The delimiter will not be included in the returned string. After this
// function, buffer will point to the first character after the delimiter.
// Whitespaces at the start or end will NOT be stripped at all.
NA_API NAString naParseBufferTokenWithDelimiter( NABufferIterator* iter,
                                                    NAByte delimiter);

// Gathers the first token within buffer which ends in a path delimiter. Both
// path delimiters / and \ are detected. The delimiter will not be included.
// After this function, string will point to the first character after the
// delimiter. Whitespaces at the start or end will NOT be stripped at all.
NA_API NAString naParseBufferPathComponent(NABufferIterator* iter);

// Parses the given buffer for decimal digits and accumulates them into an
// unsigned integer. The function will start at the current byte and parse
// as long as there is a decimal digit. The number of bytes considered is
// returned. The resulting integer value is returned in retint. If retint is
// a Null-Pointer, the function just returns the number of bytes considered.
//
// The function will not parse more than maxdigitcount characters. If that
// value is 0 it can read any number of characters.
//
// If the parsed value exceeds max, retint will be max and a warning will be
// emitted when debugging. But note that the returned number of bytes contains
// all digits considered.
NA_DEF NAInt naParseBufferDecimalUnsignedInteger(  NABufferIterator* iter,
                                                     uint64* retint,
                                                       NAInt maxdigitcount,
                                                      uint64 max);

// Same as above but parses a signed integer. Note that there is an addidional
// min parameter.
NA_DEF NAInt naParseBufferDecimalSignedInteger(  NABufferIterator* iter,
                                                    int64* retint,
                                                     NAInt maxdigitcount,
                                                     int64 min,
                                                     int64 max);

// Parses the given buffer for an integer value and returns the value in the
// desired type.
//
// Whe skipdelimiter is true, the limiting character is skipped and the string
// points to the character right after the delimiter. If that delimiter is a
// whitespace, all whitespaces till the next non-whitespace are skipped.
//
// The unsigned variants only detect digits and assume a positive value. The
// signed variants detect when the first character is '+' or '-'. Multiple
// signes are not detected.
//
// If any of the strings exceeds the type range, the maximal / minimal value
// of that type is returned. When debugging, a warning is emitted, when the
// range of an uint64 is insufficient to hold the parsed value.
NA_API int8   naParseBufferInt8  (NABufferIterator* iter, NABool skipdelimiter);
NA_API int16  naParseBufferInt16 (NABufferIterator* iter, NABool skipdelimiter);
NA_API int32  naParseBufferInt32 (NABufferIterator* iter, NABool skipdelimiter);
NA_API int64  naParseBufferInt64 (NABufferIterator* iter, NABool skipdelimiter);
NA_API uint8  naParseBufferUInt8 (NABufferIterator* iter, NABool skipdelimiter);
NA_API uint16 naParseBufferUInt16(NABufferIterator* iter, NABool skipdelimiter);
NA_API uint32 naParseBufferUInt32(NABufferIterator* iter, NABool skipdelimiter);
NA_API uint64 naParseBufferUInt64(NABufferIterator* iter, NABool skipdelimiter);





// Inline implementations are in a separate file:
#include "NAStruct/NABufferII.h"



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BUFFER_INCLUDED

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
