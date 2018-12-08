
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_BUFFER_INCLUDED
#define NA_BUFFER_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



typedef struct NABuffer NABuffer;
typedef struct NABufferSource NABufferSource;
typedef struct NABufferIterator NABufferIterator;



#include "NABase.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAFile.h"
#include "NAString.h"


// This file contains the API for NABuffer.
//
// An NABuffer is capable of storing any kind of linear data, potentially
// broken into multiple parts. It uses sparse memory, meaning as long as a
// part is not explicitely needed, no memory at that position is allocated.
// This means for example that you can open a file which has a total filesize
// of 1GB but you only access a few bytes resulting in only needing a few
// bytes in memory. Even if these bytes are scattered around multiple
// positions in the file, only those parts of the file are read which
// are actually needed.
//
// The parts of NABuffer have a default size. That size can be defined in
// NAConfiguration.h with the NA_BUFFER_PART_BYTESIZE macro.
//
// An NABuffer structure always requires a source to fill itself with content.
// A source defines a linear storage containing content. This source can be
// allocated memory, a file, an existing C-array, another NABuffer and more.
// The source guarantees, that each byte of that source has its clearly defined
// position in memory.
//
// The NABuffer uses that source and "borrows" the contents of the linear
// memory, arbitrarily positioned.
//
// For example: The source of a file looks like this:
//
// +-0---------------------------+
// | This is the text of a file. |   Stored on disk.
// +-------------^---------------+
//               |
// An NABuffer may choose to use the bytes 12 - 15 of that source but placing
// it at its own absolute position 0.
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
// In version 19, buffers became able to use multiple sources and stitch
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




// ////////////////////////////////////////
// Buffer creation, Retaining and Releasing
// ////////////////////////////////////////

// Allocates an empty buffer. Can be extended using naWriteBuffer calls.
// When securememory is set to NA_TRUE, each byte will be set to binary zero
// before it can be used.
NA_API NABuffer* naNewBuffer(NABool securememory);

// Creates a buffer referencing a subrange of another buffer. The origin of
// the new buffer will be at zero and its range is fixed. Does NOT copy any
// content, only references it.
NA_API NABuffer* naNewBufferExtraction( NABuffer* srcbuffer,
                                         NARangei range);

// Creates a buffer having an exact copy of the bytes in srcbuffer within the
// given range. The new content will be COPIED to a new memory buffer which can
// be chosen to be either secure or not. The origin of the new buffer will be
// at zero.
// All content will be loaded in the new buffer. If there are sparse parts
// in the src buffer, they will be filled with the current source.
//NA_API NABuffer* naNewBufferCopy(const NABuffer* srcbuffer,
//                                        NARangei range,
//                                          NABool securememory);

// Creates a buffer sharing the same source as the given buffer. Also uses
// the same settings like endianness or newlines.
NA_API NABuffer* naNewBufferWithSameSource(  NABuffer* srcbuffer);

// Creates a buffer inputting contents from a file. Its origin is always at
// zero and its range is fixed to the filesize.
NA_API NABuffer* naNewBufferWithInputFile(const char* filename);

// Creates a buffer accessing already existing const or mutable data. If the
// data is mutable, you can give a destructor if you want to delete the
// memory of the data pointer when no longer needed.
NA_API NABuffer* naNewBufferWithConstData( const void* data,
                                                 NAInt bytesize);
NA_API NABuffer* naNewBufferWithMutableData(     void* data,
                                                 NAInt bytesize,
                                             NAMutator destructor);



// ////////////////////////////////////////
// Buffer with custom source
// ////////////////////////////////////////

// You can create an NABuffer from any linear source you want by creating a
// custom source object. Such a source has the purpose to fill memory blocks
// denoting a certain range of the source with specific contents.

// Prototype for the filler function.
// The function has to perform the task to fill dst with the content of the
// custom source. The range denotes both the source origin of the first byte
// of dst as well as the length in bytes the dst buffer holds. The sourcedata
// parameter will be the data pointer given to naSetBufferSourceData.
typedef void (*NABufferFiller)(void* dst, NARangei sourcerange, void* sourcedata);

// Create custom sources.
// You can provide a filler function as well as an underlying buffer. Both
// parameters can be NA_NULL. Their meaning is:
// filler: The given function gets called whenever a certain memory block needs
//         to be filled with the contents of the source. If NA_NULL is given,
//         the memory blocks will be returned uninitialized.
// buffer: An optional NABuffer which holds all memory blocks sorted by their
//         linear address for later access. If NA_NULL is given, memory blocks
//         will always be created anew and on the go, completely unordered. For
//         example, a file buffer has a simple underlying NABuffer which holds
//         all bytes which already have been read.
NA_API NABufferSource* naNewBufferSource(      NABufferFiller filler,
                                                    NABuffer* buffer);

// Add data to your source which will be available when the filler function
// is called. Add a datadestructor to automatically clean up that deta when
// the source is no longer needed.
NA_API void naSetBufferSourceData(            NABufferSource* source,
                                                        void* data,
                                                    NAMutator datadestructor);

// Limits the source to a specific range of bytes. This is for example used
// when reading a file, where the source is limited to the file size.
NA_API void naSetBufferSourceLimit(           NABufferSource* source,
                                                     NARangei limit);
// Sets the volatile flag of the buffer.
// A volatile buffer always reads its content from the source anew upon every
// call. Most of the time, you won't need that.
NA_API void naSetBufferSourceVolatile(        NABufferSource* source);

// Creates a buffer with a custom source. Note that the NABuffer will retain
// the source, meaning you can safely release the source after this function.
// The sourceoffset denotes how many bytes the source is shifted away from the
// bnew uffer. For example if source denotes a string like "Hello World",
// ranging from 0 to 10 and you define an sourceoffset of -6, filling the new
// buffer at index 0 results in "World", containing the source indices 6 to 10
// of the source buffer.
// Note that usually, offset is 0 but you may choose otherwise. Beware that
// this can get rather confusing when the source itself has a range which
// contains negative indices. For example, if the string "Hello World" ranges
// from -2 to 8 instead and using a sourceoffset of -6, filling the new buffer
// at index 0 results in "rld", containing the source indices 6 to 8 of the
// source buffer.
NA_API NABuffer* naNewBufferWithCustomSource( NABufferSource* source,
                                                        NAInt sourceoffset);



// ////////////////////////////////////////
// Various buffer functions
// ////////////////////////////////////////

// Working with the range of the buffer.
// A buffer has a current range denoting what the first and last index of
// the content is. That range can either be fixed (for example for read-files)
// or non-fixed (for example for growing memory buffers). If you call the fix
// function, the current range of the buffer will be used as the fixed range.
// A buffer with a fixed range can no longer change.
// The extend function extends the buffers range by the desired bytesize on
// both ends.
// Note that extending does not automatically allocates memory. It just
// changes the range the buffer is allowed to address.
NA_API NABool   naIsBufferEmpty             (const NABuffer* buffer);
NA_API NARangei naGetBufferRange            (const NABuffer* buffer);
NA_API NABool   naHasBufferFixedRange      (const NABuffer* buffer);
NA_API void     naFixBufferRange            (NABuffer*       buffer);
NA_API void     naExtendBufferRange         (NABuffer*       buffer,
                                                       NAInt bytesatstart,
                                                       NAInt bytesatend);

// Get or set the newline encoding of this buffer. The newline encoding of a
// new buffer is either NA_NEWLINE_NATIVE or the same encoding as the buffer
// it is created from, if any. It will be used when ASCII strings are written
// to the buffer.
NA_API NANewlineEncoding naGetBufferNewlineEncoding(NABuffer* buffer);
NA_API void naSetBufferNewlineEncoding(             NABuffer* buffer,
                                            NANewlineEncoding newlineencoding);

// Get or set the endianness setting of this buffer. If not stated otherwise,
// the endianness of a new buffer is NA_ENDIANNESS_NATIVE.
NA_API NAInt  naGetBufferEndianness(NABuffer* buffer);
NA_API void   naSetBufferEndianness(NABuffer* buffer, NAInt endianness);

// Returns the byte at the given index. Warning: This function is costly. You
// might want to use one of the Reading or Parsing functions instead.
NA_API NAByte naGetBufferByteAtIndex(const NABuffer* buffer, NAInt indx);

// Searches for the given byte starting at (and including) startoffset, either
// forward or backwards.
NA_API NAInt naSearchBufferByteOffset(  const NABuffer* buffer,
                                                 NAByte byte,
                                                  NAInt startoffset,
                                                 NABool forward);

// Cache:   Allocates all memory of the desired range and fills it according
//          to the current source.
// Dismiss: Dismisses the bytes of the given range. Use this if you want to
//          declare the denoted bytes to be no longer in use by this buffer.
//          This gives NALib the possibility to deallocate memory.
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
NA_API void naAppendBufferToBuffer(         NABuffer* dstbuffer,
                                      const NABuffer* srcbuffer);

// Converts the bytes of the given buffer to a string encoded in Base64.
// When appendendsign is NA_TRUE, equal signs = will be appended if needed.
NA_API NAString* naNewStringWithBufferBase64Encoded(
                                      const NABuffer* buffer,
                                               NABool appendendsign);

// Extracts the bytes from string encoded in Base64.
NA_API NABuffer* naNewBufferWithStringBase64Decoded(
                                      const NAString* string);

// Uses all bytes of the buffer to write to output or use it in other structs.
// File:     Creates a new file and fills it with the content of the buffer.
// Data:     Assumes data to have enough space and fills all bytes inside.
// Checksum: Adds all bytes to the checksum.
NA_API void naWriteBufferToFile(NABuffer* buffer, NAFile* file);
NA_API void naWriteBufferToData(NABuffer* buffer, void* data);
NA_API void naAccumulateChecksumBuffer( NAChecksum* checksum,
                                          NABuffer* buffer);



// ////////////////////////////////
// BUFFER ITERATION
// ////////////////////////////////

NA_API NABufferIterator naMakeBufferAccessor(const NABuffer* buffer);
NA_API NABufferIterator naMakeBufferMutator (const NABuffer* buffer);
NA_API NABufferIterator naMakeBufferModifier(      NABuffer* buffer);

NA_API void naClearBufferIterator(NABufferIterator* iter);

NA_API NABool naLocateBuffer(NABufferIterator* iter, NAInt offset);

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
NA_API NAInt  naGetBufferLocation   (const NABufferIterator* iter);
NA_API void   naSeekBufferAbsolute  (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferRelative  (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferSource    (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferFromStart (NABufferIterator* iter, NAInt offset);
NA_API void   naSeekBufferFromEnd   (NABufferIterator* iter, NAInt offset);

NA_API NABool naIterateBuffer(      NABufferIterator* iter, NAInt step);

NA_API NABool naIsBufferAtInitial(  NABufferIterator* iter);
NA_API uint8 naGetBufferCurBit(NABufferIterator* iter);



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
// the buffer setting. You can change it using naSetBufferNewlineEncoding.
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
// We keep it simple here: Start and End. Forward and Backward.

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
// The line number of iter will increase according to the number of lines read.
// Use naGetBufferLineNumber to get that number. Warning: Experimental feature,
// not fully implemented!
NA_API NAString* naParseBufferLine( NABufferIterator* iter,
                                      NABool skipempty);

// Returns the current line number (starting with 1). This is an experimental
// feature which currently only works reliably if naParseBufferLine is used.
// If this function returns 0, naParseBufferLine has never been called.
NA_API NAUInt naGetBufferLineNumber(NABufferIterator* iter);

// Returns everything from the current position till the end of the buffer as
// a string.
NA_API NAString* naParseBufferRemainder(NABufferIterator* iter);

// Gathers the first token which is delimited by whitespaces. The buffer is
// expected to start at a non-whitespace! If it starts with a whitespace, an
// empty string is returned. In any case, the returned token will not have
// any whitespaces at the start or end.
// After this function, buffer will point to the next character after the
// token not being a whitespace.
NA_API NAString* naParseBufferToken(NABufferIterator* iter);

// Gathers the first token within buffer which ends in the given delimiter.
// The delimiter will not be included in the returned string. After this
// function, buffer will point to the first character after the delimiter.
// Whitespaces at the start or end will NOT be stripped at all.
NA_API NAString* naParseBufferTokenWithDelimiter( NABufferIterator* iter,
                                                    NAUTF8Char delimiter);

// Gathers the first token within buffer which ends in a path delimiter. Both
// path delimiters / and \ are detected. The delimiter will not be included.
// After this function, string will point to the first character after the
// delimiter. Whitespaces at the start or end will NOT be stripped at all.
NA_API NAString* naParseBufferPathComponent(NABufferIterator* iter);

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
#include "NAStruct/NABuffer/NABufferII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BUFFER_INCLUDED

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
