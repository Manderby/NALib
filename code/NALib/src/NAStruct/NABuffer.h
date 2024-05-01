
#ifndef NA_BUFFER_INCLUDED
#define NA_BUFFER_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



typedef struct NABuffer NABuffer;
typedef struct NABufferSource NABufferSource;
typedef struct NABufferIterator NABufferIterator;



#include "../NABase/NABase.h"
#include "../NAMath/NACoord.h"
#include "../NAUtility/NAFile.h"
#include "../NAUtility/NAString.h"


// This file contains the API for NABuffer.
//
// An NABuffer is capable of storing any kind of linear data, potentially
// broken into multiple parts. It uses sparse memory, meaning as long as a
// part is not explicitely needed, no memory at that position is allocated.
// This means for example that you can open a file which has a total file
// size of 1GB but you only access a few bytes resulting in only needing a
// few bytes in memory. Even if these bytes are scattered around multiple
// positions in the file, only those parts of the file are read which
// are actually needed.
//
// The parts of NABuffer have a default size. That size can be defined in
// NAConfiguration.h with the NA_BUFFER_PART_BYTESIZE macro.
//
// An NABuffer structure always requires a source to fill itself with content.
// A source defines a linear storage containing content. This source can be
// allocated memory, a file, an existing C-array, another NABuffer and more.
// If the content has to be cached data, the source guarantees, that each byte
// of that source has its clearly defined position in memory.
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
// it. This means that a programmer can read a whole file, pick the contents
// which are needed and close the file source again, resulting in all memory
// deallocated which is not needed anymore and only leaving the memory active
// which is actually in use.
//
// NABuffers are able to use multiple sources and stitch together pieces
// arbitrarily. Like for example:
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
// For a more detailed explanation, see the NABufferII.h file.




// ////////////////////////////////////////
// Buffer creation
// You can use naRetain and naRelease after the creation.
// ////////////////////////////////////////

// Allocates an empty buffer. Can for example be extended using naWriteBuffer
// calls. When secureMemory is set to NA_TRUE, each byte will be set to binary
// zero before it can be used.
NA_API NABuffer* naCreateBuffer(NABool secureMemory);

// Creates a buffer referencing a subrange of another buffer. The origin of
// the new buffer will be at zero and its range is fixed. Does NOT copy any
// content, only references it.
NA_API NABuffer* naCreateBufferExtraction( NABuffer* srcBuffer,
                                            NAInt offset,
                                            NAInt length);

// Creates a buffer having an exact copy of the bytes in srcBuffer within the
// given range. The new content will be COPIED to a new memory buffer which can
// be chosen to be either secure or not. The origin of the new buffer will be
// at zero.
// All content will be loaded in the new buffer. If there are sparse parts
// in the src buffer, they will be filled with the current source.
NA_API NABuffer* naCreateBufferCopy(const NABuffer* srcBuffer,
                                        NARangei range,
                                          NABool secureMemory);

// Creates a buffer sharing the same source as the given buffer. Also uses
// the same settings like endianness or newlines.
NA_API NABuffer* naCreateBufferWithSameSource(  NABuffer* srcBuffer);

// Creates a buffer inputting contents from a file. Its origin is always at
// zero and its range is fixed to the fileSize.
NA_API NABuffer* naCreateBufferWithInputPath(const char* filePath);

// Creates a buffer accessing already existing const or mutable data. If the
// data is mutable, you can give a destructor if you want to delete the
// memory of the data pointer when no longer needed.
NA_API NABuffer* naCreateBufferWithConstData( const void* data,
                                                 size_t byteSize);
NA_API NABuffer* naCreateBufferWithMutableData(     void* data,
                                                 size_t byteSize,
                                             NAMutator destructor);

// Creates a buffer with a custom source. You can create an NABuffer from
// any source you want by creating a custom source object, see below.
// Note that the NABuffer will retain the source, meaning you can safely
// release the source after this function.
// The sourceOffset denotes how many bytes the source is shifted away from the
// new buffer. For example if source denotes a string like "Hello World",
// ranging from 0 to 10 and you define an sourceOffset of -6, filling the new
// buffer at index 0 results in "World", containing the source indices 6 to 10
// of the source buffer.
// Note that usually, offset is 0 but you may choose otherwise. Beware that
// this can get rather confusing when the source itself has a range which
// contains negative indices. For example, if the string "Hello World" ranges
// from -2 to 8 instead and using a sourceOffset of -6, filling the new buffer
// at index 0 results in "rld", containing the source indices 6 to 8 of the
// source buffer.
NA_API NABuffer* naCreateBufferWithCustomSource( NABufferSource* source,
  NAInt sourceOffset);



// ////////////////////////////////////////
// Buffer Source
// ////////////////////////////////////////

//  Such a source has the purpose to fill memory blocks denoting
// a certain range of the source with specific contents.

// Prototype for the filler function.
// The function has to perform the task to fill dst with the content of the
// custom source. The range denotes both the source origin of the first byte
// of dst as well as the length in bytes the dst buffer holds. The sourceData
// parameter will be the data pointer given to naSetBufferSourceData.
typedef void (*NABufferFiller)(
  void* dst,
  NARangei sourceRange,
  void* sourceData);

// Creates a new custom source.
// You can provide a filler function as well as an underlying buffer. Both
// parameters can be NA_NULL. Their meaning is:
// filler: The given function gets called whenever a certain memory block needs
//         to be filled with the contents of the source. If NA_NULL is given,
//         the memory blocks will be left uninitialized.
// cache:  An optional NABuffer which holds all memory blocks sorted by their
//         linear address for later access. If NA_NULL is given, memory blocks
//         will always be created anew and on the go, completely unordered. For
//         example, a file buffer has a simple linear NABuffer which holds
//         all bytes which already have been read. But a custom source which
//         initializes all bytes to 42 does not need a buffer.
NA_API NABufferSource* naCreateBufferSource(
  NABufferFiller filler,
  NABuffer* cache);

// Add data to your source which will be available when the filler function
// is called. Add a dataDestructor to automatically clean up that data when
// the source is no longer needed. This function can only be called once per
// source!
NA_IAPI void naSetBufferSourceData(
  NABufferSource* source,
  void* data,
  NAMutator dataDestructor);

// Limits the source to a specific range of bytes. This is for example used
// when reading a file, where the source is limited to the file size. This
// function can only be called once per source!
NA_IAPI void naSetBufferSourceLimit(
  NABufferSource* source,
  NARangei limit);



// ////////////////////////////////////////
// Various buffer functions
// ////////////////////////////////////////

// Working with the range of the buffer.
// A buffer has a current range denoting what the first and last index of
// the content is. That range can either be fixed (for example for read-files)
// or non-fixed (for example for growing memory buffers). If you call the fix
// function, the current range of the buffer will be used as the fixed range.
// A buffer with a fixed range can no longer change.
// The extend function extends the buffers range by the desired byteSize on
// both ends.
// Note that extending does not automatically allocates memory. It just
// changes the range the buffer is allowed to address.
NA_IAPI NABool   naIsBufferEmpty            (const NABuffer* buffer);
NA_IAPI NARangei naGetBufferRange           (const NABuffer* buffer);
NA_IAPI NABool   naHasBufferFixedRange      (const NABuffer* buffer);
NA_IAPI void     naFixBufferRange           (NABuffer*       buffer);
NA_IAPI void     naExtendBufferRange        (NABuffer*       buffer,
                                                       NAInt bytesAtStart,
                                                       NAInt bytesAtEnd);

// Get or set the newline encoding of this buffer. The newline encoding of a
// new buffer is either NA_NEWLINE_NATIVE or the same encoding as the buffer
// it is created from, if any. It will be used when ASCII strings are written
// to the buffer.
NA_IAPI NANewlineEncoding naGetBufferNewlineEncoding(NABuffer* buffer);
NA_IAPI void naSetBufferNewlineEncoding(             NABuffer* buffer,
                                             NANewlineEncoding newlineEncoding);

// Get or set the endianness setting of this buffer. If not stated otherwise,
// the endianness of a new buffer is NA_ENDIANNESS_HOST.
NA_IAPI NAInt  naGetBufferEndianness(NABuffer* buffer);
NA_IAPI void   naSetBufferEndianness(NABuffer* buffer, NAInt endianness);

// Returns the byte at the given index. Warning: This function is costly. You
// might want to use one of the Reading or Parsing functions instead.
NA_API NAByte naGetBufferByteAtIndex(NABuffer* buffer, size_t index);

// Searches for the given byte starting at (and including) startOffset, either
// forward or backwards.
NA_API NAInt naSearchBufferByteOffset(        NABuffer* buffer,
                                                 NAByte byte,
                                                  NAInt startOffset,
                                                 NABool forward);

// Cache:   Allocates all memory of the desired range and fills it according
//          to the current source.
// Dismiss: Dismisses the bytes of the given range. Use this if you want to
//          declare the denoted bytes to be no longer in use by this buffer.
//          This gives NALib the possibility to deallocate memory.
NA_API void naCacheBufferRange(   NABuffer* buffer,
                                   NARangei range);
NA_API void naDismissBufferRange( NABuffer* buffer,
                                   NARangei range);

// ////////////////////////////////
// WHOLE BUFFER FUNCTIONS
// ////////////////////////////////

// Compares two buffers. If caseSensitive is NA_TRUE, an exact match is tested.
NA_API NABool naEqualBufferToBuffer(  const NABuffer* buffer1,
                                      const NABuffer* buffer2,
                                               NABool caseSensitive);

// Assumes data to contain an equal amount of bytes as buffer and compares all
// bytes for equality. If caseSensitive is NA_TRUE, an exact match is tested.
NA_API NABool naEqualBufferToData(      NABuffer* buffer,
                                          const void* data,
                                                size_t dataByteSize,
                                               NABool caseSensitive);

// Appends the whole content of srcBuffer to the end of dstbuffer.
NA_API void naAppendBufferToBuffer(         NABuffer* dstbuffer,
                                      const NABuffer* srcBuffer);

// Converts the bytes of the given buffer to a string encoded in Base64.
// When appendEndSign is NA_TRUE, equal signs = will be appended if needed.
NA_API NAString* naNewStringWithBufferBase64Encoded(
                                            NABuffer* buffer,
                                               NABool appendEndSign);

// Extracts the bytes from string encoded in Base64.
NA_API NABuffer* naCreateBufferWithStringBase64Decoded(
                                       NAString* string);

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
NA_API NABufferIterator naMakeBufferMutator (NABuffer* buffer);
NA_API NABufferIterator naMakeBufferModifier(NABuffer* buffer);

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
// naLocateBufferFromEnd(buffer, -1).
NA_API  NAInt  naGetBufferLocation     (const NABufferIterator* iter);
NA_API  NABool naLocateBufferAbsolute  (NABufferIterator* iter, NAInt offset);
NA_IAPI NABool naLocateBufferRelative  (NABufferIterator* iter, NAInt offset);
NA_IAPI NABool naLocateBufferFromStart (NABufferIterator* iter, NAInt offset);
NA_IAPI NABool naLocateBufferFromEnd   (NABufferIterator* iter, NAInt offset);

NA_API  NABool naIterateBuffer(         NABufferIterator* iter, NAInt step);

NA_IAPI NABool naIsBufferAtInitial(     NABufferIterator* iter);
NA_IAPI NABool naIsBufferAtEnd(         NABufferIterator* iter);
NA_IAPI uint8 naGetBufferCurBit(        NABufferIterator* iter);



// ////////////////////////////////
// GETTER and SETTTER
// ////////////////////////////////

// The following functions will get or set single values WITHOUT moving the
// iterator.

NA_IAPI int8   naGetBufferi8  (NABufferIterator* iter);
NA_IAPI int16  naGetBufferi16 (NABufferIterator* iter);
NA_IAPI int32  naGetBufferi32 (NABufferIterator* iter);
NA_IAPI NAi64  naGetBufferi64 (NABufferIterator* iter);
NA_IAPI uint8  naGetBufferu8  (NABufferIterator* iter);
NA_IAPI uint16 naGetBufferu16 (NABufferIterator* iter);
NA_IAPI uint32 naGetBufferu32 (NABufferIterator* iter);
NA_IAPI NAu64  naGetBufferu64 (NABufferIterator* iter);
NA_IAPI float  naGetBufferf   (NABufferIterator* iter);
NA_IAPI double naGetBufferd   (NABufferIterator* iter);

NA_IAPI void naSetBufferi8  (NABufferIterator* iter, int8   value);
NA_IAPI void naSetBufferi16 (NABufferIterator* iter, int16  value);
NA_IAPI void naSetBufferi32 (NABufferIterator* iter, int32  value);
NA_IAPI void naSetBufferi64 (NABufferIterator* iter, NAi64  value);
NA_IAPI void naSetBufferu8  (NABufferIterator* iter, uint8  value);
NA_IAPI void naSetBufferu16 (NABufferIterator* iter, uint16 value);
NA_IAPI void naSetBufferu32 (NABufferIterator* iter, uint32 value);
NA_IAPI void naSetBufferu64 (NABufferIterator* iter, NAu64  value);
NA_IAPI void naSetBufferf   (NABufferIterator* iter, float  value);
NA_IAPI void naSetBufferd   (NABufferIterator* iter, double value);



// ////////////////////////////////
// BUFFER READING
// ////////////////////////////////

// The following functions will read values. The iterator will move forward
// by the necessary number of bytes.

NA_IAPI int8   naReadBufferi8  (NABufferIterator* iter);
NA_IAPI int16  naReadBufferi16 (NABufferIterator* iter);
NA_IAPI int32  naReadBufferi32 (NABufferIterator* iter);
NA_IAPI NAi64  naReadBufferi64 (NABufferIterator* iter);
NA_IAPI uint8  naReadBufferu8  (NABufferIterator* iter);
NA_IAPI uint16 naReadBufferu16 (NABufferIterator* iter);
NA_IAPI uint32 naReadBufferu32 (NABufferIterator* iter);
NA_IAPI NAu64  naReadBufferu64 (NABufferIterator* iter);
NA_IAPI float  naReadBufferf   (NABufferIterator* iter);
NA_IAPI double naReadBufferd   (NABufferIterator* iter);

NA_IAPI void naReadBufferi8v (NABufferIterator* iter, int8*   dst, size_t count);
NA_IAPI void naReadBufferi16v(NABufferIterator* iter, int16*  dst, size_t count);
NA_IAPI void naReadBufferi32v(NABufferIterator* iter, int32*  dst, size_t count);
NA_IAPI void naReadBufferi64v(NABufferIterator* iter, NAi64*  dst, size_t count);
NA_IAPI void naReadBufferu8v (NABufferIterator* iter, uint8*  dst, size_t count);
NA_IAPI void naReadBufferu16v(NABufferIterator* iter, uint16* dst, size_t count);
NA_IAPI void naReadBufferu32v(NABufferIterator* iter, uint32* dst, size_t count);
NA_IAPI void naReadBufferu64v(NABufferIterator* iter, NAu64*  dst, size_t count);
NA_IAPI void naReadBufferfv  (NABufferIterator* iter, float*  dst, size_t count);
NA_IAPI void naReadBufferdv  (NABufferIterator* iter, double* dst, size_t count);

// Reads the given number of bytes from the current position of buffer to data.
NA_IAPI void naReadBufferBytes(
  NABufferIterator* iter,
  void* data,
  size_t byteSize);

// Reads the given number of bytes from the current position of buffer to data.
NA_IAPI NABuffer* naReadBufferBuffer( NABufferIterator* iter,
                                                  NAInt byteSize);


// ////////////////////////////////
// BINARY BUFFER WRITING
// ////////////////////////////////

// The writing functions each write data to the buffer. The data is written at
// the current position of the iterator and the current position is moved
// forward the necessary number of bytes.

NA_IAPI void naWriteBufferi8  (NABufferIterator* iter, int8   value);
NA_IAPI void naWriteBufferi16 (NABufferIterator* iter, int16  value);
NA_IAPI void naWriteBufferi32 (NABufferIterator* iter, int32  value);
NA_IAPI void naWriteBufferi64 (NABufferIterator* iter, NAi64  value);
NA_IAPI void naWriteBufferu8  (NABufferIterator* iter, uint8  value);
NA_IAPI void naWriteBufferu16 (NABufferIterator* iter, uint16 value);
NA_IAPI void naWriteBufferu32 (NABufferIterator* iter, uint32 value);
NA_IAPI void naWriteBufferu64 (NABufferIterator* iter, NAu64  value);
NA_IAPI void naWriteBufferf   (NABufferIterator* iter, float  value);
NA_IAPI void naWriteBufferd   (NABufferIterator* iter, double value);

NA_API  void naWriteBufferi8v (NABufferIterator* iter, const int8*   src, size_t count);
NA_API  void naWriteBufferi16v(NABufferIterator* iter, const int16*  src, size_t count);
NA_API  void naWriteBufferi32v(NABufferIterator* iter, const int32*  src, size_t count);
NA_API  void naWriteBufferi64v(NABufferIterator* iter, const NAi64*  src, size_t count);
NA_API  void naWriteBufferu8v (NABufferIterator* iter, const uint8*  src, size_t count);
NA_API  void naWriteBufferu16v(NABufferIterator* iter, const uint16* src, size_t count);
NA_API  void naWriteBufferu32v(NABufferIterator* iter, const uint32* src, size_t count);
NA_API  void naWriteBufferu64v(NABufferIterator* iter, const NAu64*  src, size_t count);
NA_API  void naWriteBufferfv  (NABufferIterator* iter, const float*  src, size_t count);
NA_API  void naWriteBufferdv  (NABufferIterator* iter, const double* src, size_t count);

// Writes the given number of bytes in data to the current position of buffer.
NA_IAPI void naWriteBufferBytes(
  NABufferIterator* iter,
  const void* data,
  size_t byteSize);

// Writes the content in the specified range of the source buffer into the
// buffer indicated by iter at the current position.
NA_API void naWriteBufferBuffer(
  NABufferIterator* iter,
  const NABuffer* srcBuffer,
  NARangei srcRange);

// Repeats all bytes at the current position which have the given distance
// to the current position (towards the start). Examples:
// Existing Buffer   distance   byteSize   Result
// ABCDEF            6          6          ABCDEFABCDEF
// ABCDEF            6          3          ABCDEFABC
// ABCDEF            5          2          ABCDEFBC
// ABCDEF            3          12         ABCDEFDEFDEFDEFDEF
// ABCDEF            1          10         ABCDEFFFFFFFFFFF
// Beware. Using this function to repeat small parts is very inefficient. It
// may be cleverer to create a temporary memory block and write the bytes
// there.
NA_API void naRepeatBufferBytes(
  NABufferIterator* iter,
  NAInt distance,
  size_t byteSize,
  NABool useCopy);



// ////////////////////////////////
// STRING BUFFER WRITING
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
// The argumentList argument will not be altered by this function.
NA_API void naWriteBufferStringWithArguments( NABufferIterator* iter,
                                               const NAUTF8Char* format,
                                                         va_list argumentList);
NA_API void naWriteBufferLineWithArguments(   NABufferIterator* iter,
                                               const NAUTF8Char* format,
                                                         va_list argumentList);



// ////////////////////////////////
// BITWISE MANIPULATION
// ////////////////////////////////

// Each buffer has an internal bit counter allowing you to use single bits
// of the buffer.
//
// Note that most read or write functions require the internal bit counter
// to be aligned to a byte boundary. Use naPadBufferBits to ensure that.

NA_API NABool naReadBufferBit(NABufferIterator* iter);
NA_API uint32 naReadBufferBits32(NABufferIterator* iter, uint8 count);

// Aligns the bit counter to the next byte boundary. If the bit counter is
// already at a boundary, nothing happends.
NA_API void naPadBufferBits (NABufferIterator* iter);



// ////////////////////////////////
// STRING BUFFER READING (PARSING)
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
// are still there. When skipEmpty is NA_TRUE, lines containing nothing but
// whitespaces will be skipped.
// The line number of iter will increase according to the number of lines read.
// Use naGetBufferLineNumber to get that number. Warning: Experimental feature,
// not fully implemented!
NA_API NAString* naParseBufferLine( NABufferIterator* iter,
                                               NABool skipEmpty);

// Returns the current line number (starting with 1). This is an experimental
// feature which currently only works reliably if naParseBufferLine is used.
// If this function returns 0, naParseBufferLine has never been called.
NA_IAPI size_t naGetBufferLineNumber(NABufferIterator* iter);

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
// If skipWhitespace is NA_TRUE, any whitespaces after the delimiter will be
// skipped as well. Whitespaces at the start will NOT be skipped!
NA_API NAString* naParseBufferTokenWithDelimiter(
  NABufferIterator* iter,
  NAUTF8Char delimiter,
  NABool skipWhitespace);

// Gathers the first token within buffer which ends in a path delimiter. Both
// path delimiters / and \ are detected. The delimiter will not be included.
// After this function, string will point to the first character after the
// delimiter. Whitespaces at the start or end will NOT be stripped at all.
NA_API NAString* naParseBufferPathComponent(NABufferIterator* iter);

// Parses the given buffer for decimal digits and accumulates them into an
// unsigned integer. The function will start at the current byte and parse
// as long as there is a decimal digit. The number of bytes considered is
// returned. The resulting integer value is returned in retValuei. If retValuei is
// a Null-Pointer, the function just returns the number of bytes considered.
//
// The function will not parse more than maxDigitCount characters. If that
// value is 0 it can read any number of characters.
//
// If the parsed value exceeds max, retValuei will be max and a warning will be
// emitted when debugging. But note that the returned number of bytes contains
// all digits considered.
NA_DEF NAInt naParseBufferDecimalUnsignedInteger(  NABufferIterator* iter,
                                                     NAu64* retValuei,
                                                       NAInt maxDigitCount,
                                                      NAu64 max);

// Same as above but parses a signed integer. Note that there is an addidional
// min parameter.
NA_DEF NAInt naParseBufferDecimalSignedInteger(  NABufferIterator* iter,
                                                    NAi64* retValuei,
                                                     NAInt maxDigitCount,
                                                     NAi64 min,
                                                     NAi64 max);

// Parses the given buffer for an integer value and returns the value in the
// desired type.
//
// Whe skipDelimiter is true, the limiting character is skipped and the string
// points to the character right after the delimiter. If that delimiter is a
// whitespace, all whitespaces till the next non-whitespace are skipped.
//
// The unsigned variants only detect digits and assume a positive value. The
// signed variants detect when the first character is '+' or '-'. Multiple
// signes are not detected.
//
// If any of the strings exceeds the type range, the maximal / minimal value
// of that type is returned. When debugging, a warning is emitted, when the
// range of an NAu64 is insufficient to hold the parsed value.
NA_IAPI int8     naParseBufferi8  (NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI int16    naParseBufferi16 (NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI int32    naParseBufferi32 (NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI NAi64  naParseBufferi64 (NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI uint8    naParseBufferu8 (NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI uint16   naParseBufferu16(NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI uint32   naParseBufferu32(NABufferIterator* iter, NABool skipDelimiter);
NA_IAPI NAu64 naParseBufferu64(NABufferIterator* iter, NABool skipDelimiter);





// Inline implementations are in a separate file:
#include "Core/NABuffer/NABufferII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BUFFER_INCLUDED



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
