
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_BUFFER_INCLUDED
#define NA_BUFFER_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif



#include "NAFile.h"
#include "NABinaryData.h"


// Important remark about text and binary files:
// File types can roughly be split into two categories: Text files and
// binary files. Text files are also known as ASCII files. Both file types
// have their Pros and Cons. Which one to use is up to the user. The NAFile
// structure allows to read and write both type of files interchangeably.
//
// Writing files is usually done sequentially for both text and binary files:
// You write some bytes or a string, then you write some other bytes or another
// string, and so on, until the whole file is written.
//
// Reading binary files is also done mostly sequentially: You maybe read an
// integer, then you read a float, and so on. Maybe you jump to a specific
// location in the file and read on sequetially from there.
//
// Reading text files on the other hand is usually done quite differently.
// Text files are usually subdivided into a hierarchical structure. An easy
// example is a logfile where each line is a sub-structure. XML-files for
// example have very complex sub-structures where each struture can span
// multiple lines or even the whole file. Parsing such files is anything but
// sequential.
//
// Therefore, when reading a text file, NALib encourages the programmer to
// do the following: Read the whole file into an NAString struct by using a
// single call to naNewStringWithFileContents. After that, use the parsing
// functions of the NAString struct.
//
// There are text-reading functions in NABuffer but they are slower and less
// powerful as the parsing functions of NAString.




// The full type definition is in the file "NABufferII.h"
typedef struct NABuffer NABuffer;


// The different flushing-behaviours. Only useful when writing to files.
typedef enum{
  NA_BUFFER_AUTOFLUSH_NONE,      // No flush till the end.
  NA_BUFFER_AUTOFLUSH_AUTO,      // Buffering is automated.
  NA_BUFFER_AUTOFLUSH_TEXT,      // Auto-flushes Strings, Lines and NewLines
  NA_BUFFER_AUTOFLUSH_MULTIBYTE, // Auto-flushes Strings, Lines and Byte arrays
  NA_BUFFER_AUTOFLUSH_ALL        // No buffer. Auto-Flushing all output
} NABufferAutoFlushing;





// //////////////////////
// OUTPUT BUFFERS
//
// Reading:   Opens an existing file and places the read pointer to the first
//            byte of the stream. The endianness converter is set to native.
//            Reading is buffered by default.
// Writing:   Creates a new file or overwrites and truncates an existing file
//            and places the write pointer to the first byte of the stream.
//            The endianness converter is set to native. Writing is buffered by
//            default.
// Appending: Creates a new file or opens an existing file for appending. The
//            write pointer is set to the end of the stream. Text is auto-
//            flushed by default. The endianness converter is uninitialized!
//            This has been designed as appending usually only is used for
//            textual files, not binary ones. And as the initialization of the
//            binary converters takes a certain amount of time but the handling
//            of append-files usually must be very speedy, the endianness
//            converter is just uninitialized. You can initialize it using
//            naSetFileEndianness.
// 
//
// Autoflush is set to auto by default if not stated otherwise.
//
// There is a special destination: Void. If you choose to set the destination
// of a buffer to Void, all writings will be lost. Beware: Changing the dest
// of a buffer to Void can not be undone. The buffer will forever be bound to
// Void.
//
// Creates a new output buffer which will accumulate everything you write. You
// can ask the buffer to flush its contents to a destination later.
NA_API NABuffer* naInitBuffer(NABuffer* buffer);

NA_API NABuffer* naInitBufferWithConstBytes(NABuffer* buffer, const NAByte* bytes, NAInt bytesize);

// Creates a new buffer which will automatically read from the given file.
NA_API NABuffer* naInitBufferWithFile(NABuffer* buffer,
                                            const char* filename);
// Creates a new buffer which will read the given ByteArray.
//NA_API NABuffer* naInitBufferInputtingFromByteArray(NABuffer* buffer,
//                                            const NAByteArray* array);

// Creates a new buffer which has the same settings (endianness, line ending)
// as srcbuffer but which only looks at the given number of bytes and can only
// input data, not write. srcbuffer will automatically advance the read pointer
// by the number of bytes.
NA_API NABuffer* naInitBufferWithBufferExtraction(
                                                     NABuffer* buffer,
                                                     NABuffer* srcbuffer,
                                                      NAInt bytesize);



// Creates a new buffer which will automatically write to the given file.
//NA_API NABuffer* naInitBufferOutputtingToFile(NABuffer* buffer,
//                                            const char* filename,
//                                              NAFileMode mode);
//
// Creates buffers outputting to the stdout and stderr. Autoflush ist set
// to text.
// Note that in contrast to other appending files (see naOpenFileForAppending),
// the binary converter is initialized and set to native for all standard
// channels.
NA_API NABuffer* naInitBufferOutputtingToStdout(NABuffer* buffer);
NA_API NABuffer* naInitBufferOutputtingToStderr(NABuffer* buffer);
// Creates a buffer whose contents will get lost in the Void. But beware! They
// will be buffered according to the autoflush setting.
NA_API NABuffer* naInitBufferOutputtingToVoid(NABuffer* buffer);


// Clears the buffer. Flushes the buffer if necessary and closes the file.
NA_API void naClearBuffer(NABuffer* buffer);

// Flushes the buffer to the current destination. Only useful for writing
// files. Undefined behaviour for other buffers.
NA_API void naFlushBuffer(NABuffer* buffer);

// Flushes all remaining contents to the currently set destination and then
// sets the destination to Void. Future wiritings to this buffer will be lost.
//
// If the previous dest was a file, it will be closed. If there was no previous
// destination, all buffers will be gone (They will not leak however, they is
// properly being deallocated).
NA_API void naVoidBuffer(NABuffer* buffer);



// //////////////////////
// BUFFERS SETTINGS
//
// Returns the buffer kind or whether the buffer is finite. Buffers kinds are
// either readable or writable. Only readable buffers can be finite. Finite
// buffers are bound (for example by the size of a file in bytes), whereas
// in-finite buffers have a (virtually) infinite amount of bytes available.
NA_API NABool naCanBufferSeek(const NABuffer* buffer);
NA_API NABool naCanBufferExtend(const NABuffer* buffer);
NA_API NABool naHasBufferDeterminedBytesize(const NABuffer* buffer);
//NA_API NABool naIsBufferSecure(const NABuffer* buffer);

// Returns current reading or writing position in absolute buffer bytesize.
// Will return 0 for both if the buffer has nothing in it yet.
NA_API NAInt     naGetBufferCurOffsetAbsolute(const NABuffer* buffer);

// Returns the size of the buffer in byte. Will compute the size if not known
// until now. Beware: After a call to this function, the buffer size is fixed!
// Determines the filesize if the source of the buffer is a file.
NA_API NAInt naDetermineBufferBytesize(NABuffer* buffer);

// Returns true if the buffer has no more bytes to read. Only useful for
// finite buffers. This is the function you should use to check if opening
// a file has been successful.
NA_API NABool naIsBufferReadAtEnd(const NABuffer* buffer);


NA_API void naWriteBufferToFile(NABuffer* buffer, NAFile* file);


// Sets the endianness of the buffer and prepares all converters.
// - If this buffer reads from a reading file, the endianness is the input
//   encoding for binary values and every binary multi-byte value will be
//   converted from input to native endianness automatically while reading.
// - If this buffer writes to a file, the endianness is the output encoding for
//   binary values and every binary multy-byte value will be converted from
//   native to output endianness automatically while writing.
// Use endianness constants like NA_ENDIANNESS_BIG for the endianness argument.
NA_API void naSetBufferEndianness(NABuffer* buffer, NAInt endianness);

NA_API NAInt naGetBufferEndianness(NABuffer* buffer);

// Only useful for write-files. Defines, how writings should be flushed. The
// NAFile struct by default uses a buffer to speed up writing.
NA_API void naSetBufferAutoFlushing( NABuffer* buffer,
                       NABufferAutoFlushing autoflushing);

// Only useful for Write-files. Defines, how a line ending shall be encoded.
// Note that this function only affects writeLine and writeNewLine calls. If
// you somehow write a different line-ending for example as part of a string,
// that line-ending will NOT be converted!
NA_API void naSetBufferNewLineEncoding( NABuffer* buffer,
                        NANewlineEncoding newlineencoding);

// Only useful for input buffers. Defines what will be returned if the buffer
// ends prematurely. When inputting, NABuffer always allocates enough bytes for
// the user to access no matter if the source has sufficient bytes available.
// If the source does not deliver certain bytes, they usually are left
// uninitialized. If the buffer is set to read securely, these bytes are filled
// with binary zero. It takes more time when inputting beyound the source
// boundary but may limit the number of follow-up errors in your application.
//
// Default is NA_FALSE.
NA_API void naSetBufferSecure(NABuffer* buffer, NABool secure);

// Jumps to a specific position within the buffer.
// Absolute: offset denotes the absolute address.
// Local:    offset denotes the address relative to the origin of this buffer.
// Relative: offset denotes the address relative to the current offset.
NA_API NABool naIsBufferEmpty(const NABuffer* buffer);

NA_API void naSeekBufferAbsolute(NABuffer* buffer, NAInt offset);
NA_API void naSeekBufferLocal(NABuffer* buffer, NAInt offset);
NA_API void naSeekBufferRelative(NABuffer* buffer, NAInt offset);

// Accumulates all bytes of the given buffer to the given checksum.
NA_API void naAccumulateBufferToChecksum( NABuffer* buffer,
                                        NAChecksum* checksum);




// //////////
// READING
// //////////

NA_API void naReadBuffer (NABuffer* buffer, NAFilesize bytecount);


// Reads the given number of bytes and stores them without further manipulation
// in buf. buf must be big enough, no overflow check is made.
//
// This function is NOT endianness- and NOT line-endings-aware!
NA_API void naReadBufferBytes (NABuffer* buffer, void* buf, NAInt bytesize);
//NA_API NAByteArray* naInitByteArrayFromBufferInput(NAByteArray* array,
//                                                      NABuffer* buffer,
//                                                       NAInt bytesize);

NA_API NABool naReadBufferBit (NABuffer* buffer);
NA_API NAUInt naReadBufferBits (NABuffer* buffer, uint8 count);

NA_API void naPadBufferReadBits(NABuffer*buffer);

// Read different basic datatypes as binary values.
// These functions ARE endianness-aware!
NA_API int8   naReadBufferi8 (NABuffer* buffer);
NA_API int16  naReadBufferi16(NABuffer* buffer);
NA_API int32  naReadBufferi32(NABuffer* buffer);
NA_API int64  naReadBufferi64(NABuffer* buffer);
NA_API uint8  naReadBufferu8 (NABuffer* buffer);
NA_API uint16 naReadBufferu16(NABuffer* buffer);
NA_API uint32 naReadBufferu32(NABuffer* buffer);
NA_API uint64 naReadBufferu64(NABuffer* buffer);
NA_API float  naReadBufferf  (NABuffer* buffer);
NA_API double naReadBufferd  (NABuffer* buffer);

// Read whole arrays of basic datatypes. The given dst buf argument must be
// big enough to hold count items. No overflow check will be performed.
// These functions ARE endianness-aware!
NA_API void naReadBufferi8v (NABuffer* buffer, int8*   dst, NAInt count);
NA_API void naReadBufferi16v(NABuffer* buffer, int16*  dst, NAInt count);
NA_API void naReadBufferi32v(NABuffer* buffer, int32*  dst, NAInt count);
NA_API void naReadBufferi64v(NABuffer* buffer, int64*  dst, NAInt count);
NA_API void naReadBufferu8v (NABuffer* buffer, uint8*  dst, NAInt count);
NA_API void naReadBufferu16v(NABuffer* buffer, uint16* dst, NAInt count);
NA_API void naReadBufferu32v(NABuffer* buffer, uint32* dst, NAInt count);
NA_API void naReadBufferu64v(NABuffer* buffer, uint64* dst, NAInt count);
NA_API void naReadBufferfv  (NABuffer* buffer, float*  dst, NAInt count);
NA_API void naReadBufferdv  (NABuffer* buffer, double* dst, NAInt count);


// //////////
// WRITING unaware of endianness or line endings
// //////////

// Writes the given number of bytes from buf to the buffer without further
// manipulation. buf must be big enough, no overflow check is made.
//
// This function is NOT endianness- and NOT line-endings-aware!
NA_DEF void naWriteBufferBytes(               NABuffer* buffer,
                                            const void* ptr,
                                               NAInt bytesize);
// Writes an NAByteArray to the file. This function is NOT endianness-aware.
NA_API void naWriteBufferByteArray(           NABuffer* buffer,
                                     const NAByteArray* bytearray);
// Writes the given string to the file. If the string is null-terminated, that
// NULL will NOT be written to the file. Newline characters are NOT converted!
// The string will automatically be converted to the text encoding of the file.
NA_API void naWriteBufferString(              NABuffer* buffer,
                                        const NAString* string);
// Writes a string to the file which can be written like a printf format. You
// can also use this function just to write a simple const char* string. The
// encoding is UTF-8.
NA_API void naWriteBufferStringWithFormat(    NABuffer* buffer,
                                      const NAUTF8Char* format,
                                                        ...);
// Same as naWriteFileStringWithFormat but with an existing va_list argument.
// The argumentlist argument will not be altered by this function. The encoding
// is UTF-8.
NA_API void naWriteBufferStringWithArguments( NABuffer* buffer,
                                      const NAUTF8Char* format,
                                                va_list argumentlist);
// Writes the given size in bytes from the scr buffer to the dst buffer.
// Reading and writing occurs at the current position of the buffers.
NA_API void naWriteBufferBuffer(              NABuffer* dstbuffer,
                                              NABuffer* srcbuffer,
                                               NAInt bytesize);
// Reads bytes from distance bytes backwards of the current position and
// stores it at the current position. Does it for bytesize successive bytes.
// Note: The sections may overlap!
NA_API void naRepeatBufferBytes(              NABuffer* buffer,
                                               NAInt distance,
                                               NAInt bytesize);




// //////////
// WRITING endianness-aware
// //////////


// Writes some standard data type. These functions ARE endianness-aware!
NA_API void naWriteBufferi8 (NABuffer* buffer, int8   value);
NA_API void naWriteBufferi16(NABuffer* buffer, int16  value);
NA_API void naWriteBufferi32(NABuffer* buffer, int32  value);
NA_API void naWriteBufferi64(NABuffer* buffer, int64  value);
NA_API void naWriteBufferu8 (NABuffer* buffer, uint8  value);
NA_API void naWriteBufferu16(NABuffer* buffer, uint16 value);
NA_API void naWriteBufferu32(NABuffer* buffer, uint32 value);
NA_API void naWriteBufferu64(NABuffer* buffer, uint64 value);
NA_API void naWriteBufferf  (NABuffer* buffer, float  value);
NA_API void naWriteBufferd  (NABuffer* buffer, double value);


// Writes whole arrays of basic datatypes. The src buffer is expected to hold
// count items. No overflow check will be performed.
// These functions ARE endianness-aware!
NA_API void naWriteBufferi8v (NABuffer* buffer, const int8*   src, NAInt count);
NA_API void naWriteBufferi16v(NABuffer* buffer, const int16*  src, NAInt count);
NA_API void naWriteBufferi32v(NABuffer* buffer, const int32*  src, NAInt count);
NA_API void naWriteBufferi64v(NABuffer* buffer, const int64*  src, NAInt count);
NA_API void naWriteBufferu8v (NABuffer* buffer, const uint8*  src, NAInt count);
NA_API void naWriteBufferu16v(NABuffer* buffer, const uint16* src, NAInt count);
NA_API void naWriteBufferu32v(NABuffer* buffer, const uint32* src, NAInt count);
NA_API void naWriteBufferu64v(NABuffer* buffer, const uint64* src, NAInt count);
NA_API void naWriteBufferfv  (NABuffer* buffer, const float*  src, NAInt count);
NA_API void naWriteBufferdv  (NABuffer* buffer, const double* src, NAInt count);



// //////////
// WRITING line-ending-aware
// //////////

// Writes a tab or a newline to the file. The newline character is dependent on
// the files setting. You can change it using naSetFileNewLine.
NA_API void naWriteBufferTab(NABuffer* buffer);
NA_API void naWriteBufferNewLine(NABuffer* buffer);

// Same as the functions above but automatically appends a newline character
// at the end. The newline character is dependent on the files setting. You can
// change it using naSetFileNewLine.
NA_API void naWriteBufferLine(              NABuffer* buffer,
                                      const NAString* string);
NA_API void naWriteBufferLineWithFormat(    NABuffer* buffer,
                                    const NAUTF8Char* format,
                                                      ...);
NA_API void naWriteBufferLineWithArguments( NABuffer* buffer,
                                    const NAUTF8Char* format,
                                              va_list argumentlist);




// Inline implementations are in a separate file:
#include "NAStruct/NAOldBufferII.h"








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
