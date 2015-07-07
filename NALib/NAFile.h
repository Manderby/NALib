
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_FILE_INCLUDED
#define NA_FILE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAEndianness.h"
#include "NAString.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

// Input and output is handeled slightly differently on different systems. In
// the header files of each system, there are different definitions of standard
// C functions and hence, NALib is trying to map everything to one definition.
// This requires to define certain things in advance before the NAFile struct
// can be declared. This is cumbersome but there is no other meaningful place
// to put such definitions.



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
  #include <direct.h>
  #include <io.h>
  #include <share.h>
  #include <sys/stat.h>
  #if NA_SYSTEM_ADDRESS_BITS == 64
    typedef __int64 NAFileSize;
  #elif NA_SYSTEM_ADDRESS_BITS == 32
    typedef long NAFileSize;
  #endif
  typedef int NAFileMode;
  #define NA_FILE_MODE_DEFAULT (_S_IREAD | _S_IWRITE)
  #define NA_DIR_MODE_DEFAULT 0 // No directory modes on windows needed.
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND | O_BINARY)
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include <dirent.h>
  #include <copyfile.h>
  typedef off_t NAFileSize;
  typedef int NAFileMode;
  #define NA_FILE_MODE_DEFAULT 0644
  #define NA_DIR_MODE_DEFAULT 0755
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY)
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND)
#endif


// Mapping of standard library functions. They can be different depending on
// the system compiled.
NA_IAPI NAFileSize naLseek   (int fd, NAFileSize offset, int origin);
NA_IAPI int        naOpen    (const char* path, int flags, int mode);
NA_IAPI int        naClose   (int fd);
NA_IAPI NAFileSize naRead    (int fd, void* buf, NAFileSize count);
NA_IAPI NAFileSize naWrite   (int fd, const void* buf, NAFileSize count);
NA_IAPI int        naMkDir   (const char* path, int mode);
NA_IAPI int        naChDir   (const char* path);
NA_IAPI NABool     naExists  (const char* path);
NA_IAPI NABool     naIsDir   (const char* path);
NA_IAPI NABool     naIsHidden(const char* path);
NA_IAPI int        naRemove  (const char* path);
NA_IAPI NABool     naCopyFile(const char* dstpath, const char* srcpath);


// The different flushing-behaviours
typedef enum{
  NA_FILE_AUTOFLUSH_NONE,       // Writing is buffered
  NA_FILE_AUTOFLUSH_TEXT,       // Auto-flushes Strings, Lines and NewLines
  NA_FILE_AUTOFLUSH_MULTIBYTE,  // Auto-flushes Strings, Lines and Byte arrays
  NA_FILE_AUTOFLUSH_ALL         // No buffer. Auto-Flushing all output
} NAFileAutoFlushing;



typedef struct NAFile NAFile;
struct NAFile{
  int desc;                           // The descriptor
  NAFileSize pos;                     // The byte position within file.
  NAEndiannessConverter converter;    // The endianness converter.
  uint16 flags;                       // various flags
  uint16 remainingbytesinbuffer;      // remaining bytes inside the buffer
  NAByte* bufptr;                     // Pointer to the current buffer-byte
  NAByte* buf;                        // The buffer.
};



// Important remark about text and binary files:
// File types can roughly be split into two categories: Text files and
// binary files. Text files are also known as ASCII files. Both file types
// have their Pros and Cons. Which one to use is up to the user. The NAFile
// structure allows to read and write both type of files.
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
// single call to naNewStringFromFileContents. After that, use the parsing
// functions of the NAString struct.
//
// There are text-reading functions in NAFile but they are slower and less
// powerful as the parsing functions of NAString.


// Reads the full file and returns it as a new String.
// Note that this is the preferred way to handle text files in NALib: Just
// read the whole file into a string and then use the parsing functions of
// the NAString type.
// Important! Only UTF-8 is supported for now!
NA_API NAString* naNewStringFromFileContents(  const char* filename);

// Reads the full file and returns it as a new NAByteArray.
// Warning: This function is only useful if you read or store raw data! The 
// NAByteArray struct is not endianness-aware. Use the reading and writing
// functions of NAFile when handling multi-byte values instead.
NA_API NAByteArray* naInitByteArrayFromFileContents(NAByteArray* array,
                                                       const char* filename);

// Opens the file.
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
// If you don't know, what the mode argument means, use NA_FILE_MODE_DEFAULT.
// You can change the buffering-behaviour using naSetFileAutoFlush.
NA_API NAFile naOpenFileForReading(const char* filename);
NA_API NAFile naOpenFileForWriting(const char* filename, NAFileMode mode);
NA_API NAFile naOpenFileForAppending(const char* filename, NAFileMode mode);

// Makes an NAFile struct wrapped around the standard channels.
// Stdin:     Standard input (file descriptor 0) as a reading file.
// Stdout:    Standard output (file descriptor 1) as an appending file.
// Stderr:    Standard error (file descriptor 2) as an appending file.
// Stdin, stdout and stderr are unbuffered by default. You can change the
// buffering-behaviour using naSetFileAutoFlush.
// Note that in contrast to other appending files (see naOpenFileForAppending),
// the binary converter is initialized and set to native for all standard
// channels.
NA_API NAFile naMakeFileAsStdin();
NA_API NAFile naMakeFileAsStdout();
NA_API NAFile naMakeFileAsStderr();

// Closes the file. Note: Can close stdin, stdout and stderr!
NA_API void naCloseFile(NAFile* file);

// Flushes the buffer. Only useful for writing files. Undefined behaviour for
// reading files.
NA_API void naFlushFileBuffer(NAFile* file);

// Sets the endianness of the file and prepares all converters.
// - If this file is a reading file, the endianness is the input encoding for
//   binary values and every binary multi-byte value will be converted from
//   input to native endianness automatically while reading.
// - If this file is a writing file, the endianness is the output encoding for
//   binary values and every binary multy-byte value will be converted from
//   native to output endianness automatically while writing.
// Use endianness constants like NA_ENDIANNESS_BIG for the endianness argument.
NA_API void naSetFileEndianness(NAFile* file, NAInt endianness);

// Only useful for write-files. Defines, how writings should be flushed. The
// NAFile struct by default uses a buffer to speed up writing.
NA_API void naSetFileAutoFlush(NAFile* file, NAFileAutoFlushing autoflushing);

// Only useful for Write-files. Defines, how a line ending shall be encoded.
// Note that this function only affects writeLine and writeNewLine calls. If
// you somehow write a different line-ending for example as part of a string,
// that line-ending will NOT be converted!
NA_API void naSetFileNewLine(NAFile* file, NANewlineEncoding newlineencoding);

// Computes the filesize (from first to last byte).
NA_API NAFileSize naComputeFileSize(const NAFile* file);
NA_API NABool naIsFileOpen(const NAFile* file);
NA_API NABool naHasFileEnded(const NAFile* file);

// Re-adjusts the internal file pointer to the given offset.
NA_API void naJumpFileOffsetAbsolute(NAFile* file, NAFileSize offset);
NA_API void naJumpFileOffsetRelative(NAFile* file, NAFileSize offset);



// //////////
// READING
// //////////

// Reads the given number of bytes and stores it without further manipulation
// in buf. The buffer must be big enough, no overflow check is made. This is a
// low-level function. The result of the naInitByteArrayFromFile function is
// more convenient to handle.
// This function is NOT endianness-aware.
NA_API void    naReadFileBytes (NAFile* file, void* buf, NAFileSize count);

// Read different basic datatypes. These functions ARE endianness-aware!
NA_API int8    naReadFileInt8  (NAFile* file);
NA_API int16   naReadFileInt16 (NAFile* file);
NA_API int32   naReadFileInt32 (NAFile* file);
NA_API int64   naReadFileInt64 (NAFile* file);
NA_API uint8   naReadFileUInt8 (NAFile* file);
NA_API uint16  naReadFileUInt16(NAFile* file);
NA_API uint32  naReadFileUInt32(NAFile* file);
NA_API uint64  naReadFileUInt64(NAFile* file);
NA_API float   naReadFileFloat (NAFile* file);
NA_API double  naReadFileDouble(NAFile* file);

// Read whole arrays of basic datatypes. If the given buf argument is a Null-
// pointer, a sufficiently large memory block will be allocated on the heap
// using malloc. This block must be freed manually! If the given argument is
// not a Null-Pointer, the buffer at the given address is used and must be
// big enough to hold count items. No overflow check will be performed. The
// functions always return a pointer to the buffer.
// These functions ARE endianness-aware!
NA_API int8*   naReadFileArrayInt8  (NAFile* file, int8*   buf, NAInt count);
NA_API int16*  naReadFileArrayInt16 (NAFile* file, int16*  buf, NAInt count);
NA_API int32*  naReadFileArrayInt32 (NAFile* file, int32*  buf, NAInt count);
NA_API int64*  naReadFileArrayInt64 (NAFile* file, int64*  buf, NAInt count);
NA_API uint8*  naReadFileArrayUInt8 (NAFile* file, uint8*  buf, NAInt count);
NA_API uint16* naReadFileArrayUInt16(NAFile* file, uint16* buf, NAInt count);
NA_API uint32* naReadFileArrayUInt32(NAFile* file, uint32* buf, NAInt count);
NA_API uint64* naReadFileArrayUInt64(NAFile* file, uint64* buf, NAInt count);
NA_API float*  naReadFileArrayFloat (NAFile* file, float*  buf, NAInt count);
NA_API double* naReadFileArrayDouble(NAFile* file, double* buf, NAInt count);

// Creates or fills an NAByteArray or an NAString by reading the given number
// of bytes from the file and then COPYING them to the array or string.
// Note: If you want the whole file as an NAByteArray or NAString, use the
// naInitByteArrayFromFileContents or naNewStringFromFileContents function
// instead which directly reads the bytes to the array instead of copying them
// from an internal buffer.
// COPIES ALWAYS!
// The ByteArray function is NOT endianness-aware.
NA_API NAByteArray* naInitByteArrayFromFile(NAByteArray* array,
                                                   NAFile* file,
                                                NAFileSize count);
NA_API NAString*    naNewStringFromFile(           NAFile* file,
                                                NAFileSize bytecount);

// //////////
// BINARY WRITING
// //////////

// Writes the given number of bytes from buf to the file without further
// manipulation. The buffer must be big enough, no overflow check is made.
// This is a low-level function. The naWriteFileByteArray function is more
// convenient to handle if you are working with byte arrays.
// This function is NOT endianness-aware.
// Note that due to consistency with NAByteArray, count can also be negative
// which will simply be converted to the absolute number.
NA_API void naWriteFileBytes(NAFile* file, const void* ptr, NAInt count);

// Writes some standard data type. These functions ARE endianness-aware!
NA_API void naWriteFileInt8  (NAFile* file, int8 value);
NA_API void naWriteFileInt16 (NAFile* file, int16 value);
NA_API void naWriteFileInt32 (NAFile* file, int32 value);
NA_API void naWriteFileInt64 (NAFile* file, int64 value);
NA_API void naWriteFileUInt8 (NAFile* file, uint8 value);
NA_API void naWriteFileUInt16(NAFile* file, uint16 value);
NA_API void naWriteFileUInt32(NAFile* file, uint32 value);
NA_API void naWriteFileUInt64(NAFile* file, uint64 value);
NA_API void naWriteFileFloat (NAFile* file, float value);
NA_API void naWriteFileDouble(NAFile* file, double value);

// Writes whole arrays of basic datatypes. The buffer is expected to hold
// count items. No overflow check will be performed.
// These functions ARE endianness-aware!
NA_API void naWriteFileArrayInt8  (NAFile* file, const int8* buf,   NAInt count);
NA_API void naWriteFileArrayInt16 (NAFile* file, const int16* buf,  NAInt count);
NA_API void naWriteFileArrayInt32 (NAFile* file, const int32* buf,  NAInt count);
NA_API void naWriteFileArrayInt64 (NAFile* file, const int64* buf,  NAInt count);
NA_API void naWriteFileArrayUInt8 (NAFile* file, const uint8* buf,  NAInt count);
NA_API void naWriteFileArrayUInt16(NAFile* file, const uint16* buf, NAInt count);
NA_API void naWriteFileArrayUInt32(NAFile* file, const uint32* buf, NAInt count);
NA_API void naWriteFileArrayUInt64(NAFile* file, const uint64* buf, NAInt count);
NA_API void naWriteFileArrayFloat (NAFile* file, const float* buf,  NAInt count);
NA_API void naWriteFileArrayDouble(NAFile* file, const double* buf, NAInt count);

// Writes a byte array to the file. This function is NOT endianness-aware.
NA_API void naWriteFileByteArray( NAFile* file, 
                       const NAByteArray* array);


// //////////
// TEXT WRITING (also called ASCII WRITING)
// //////////

// Writes a tab or a newline to the file. The newline character is dependent on
// the files setting. You can change it using naSetFileNewLine.
NA_API void naWriteFileTab(NAFile* file);
NA_API void naWriteFileNewLine(NAFile* file);

// Writes the given string to the file. If the string is null-terminated, that
// NULL will NOT be written to the file. Newline characters are NOT converted!
// The string will automatically be converted to the text encoding of the file.
NA_API void naWriteFileString(    NAFile* file,
                         const NAString* string);

// Writes a string to the file which can be written like a printf format. You
// can also use this function just to write a simple const char* string. The
// encoding is UTF-8.
NA_API void naWriteFileStringWithFormat(NAFile* file,
                              const NAUTF8Char* format,
                                                ...);
// Same as naWriteFileStringWithFormat but with an existing va_list argument.
// The argumentlist argument will not be altered by this function. The encoding
// is UTF-8.
NA_API void naWriteFileStringWithArguments(NAFile* file,
                                 const NAUTF8Char* format,
                                           va_list argumentlist);

// Same as the functions above but automatically appends a newline character
// at the end. The newline character is dependent on the files setting. You can
// change it using naSetFileNewLine.
NA_API void naWriteFileLine(             NAFile* file,
                                 const NAString* string);
NA_API void naWriteFileLineWithFormat(   NAFile* file,
                               const NAUTF8Char* format,
                                                 ...);
NA_API void naWriteFileLineWithArguments(NAFile* file,
                               const NAUTF8Char* format,
                                         va_list argumentlist);










// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


NA_IDEF NAFileSize naLseek(int fd, NAFileSize offset, int origin){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    #if NA_SYSTEM_ADDRESS_BITS == 64
      return _lseeki64(fd, offset, origin);
    #elif NA_SYSTEM_ADDRESS_BITS == 32
      return _lseek(fd, offset, origin);
    #endif
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return lseek(fd, offset, origin);
  #endif
}


// Use the flags argument to define how to open the file. Use the macros
// NA_FILE_OPEN_FLAGS_READ, NA_FILE_OPEN_FLAGS_WRITE and NA_FILE_OPEN_FLAGS_APPEND if you
// just need the standard ways of accessing files.
// Use NA_FILE_MODE_DEFAULT if you don't know what to use for the mode
// argument.
NA_IDEF int naOpen(const char* path, int flags, int mode){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    int handle;
    _sopen_s(&handle, path, flags, _SH_DENYNO, mode);
    return handle;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return open(path, flags, mode);
  #endif
}


NA_IDEF int naClose(int fd){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _close(fd);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return close(fd);
  #endif
}


NA_IDEF NAFileSize naRead(int fd, void* buf, NAFileSize count){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAFileSize)_read(fd, buf, (unsigned int)count);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAFileSize)read(fd, buf, (size_t)count);
  #endif
}


NA_IDEF NAFileSize naWrite(int fd, const void* buf, NAFileSize count){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAFileSize)_write(fd, buf, (size_t)count);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAFileSize)write(fd, buf, (size_t)count);
  #endif
}


// Use NA_DIR_MODE_DEFAULT if you don't know what to use for the mode
// argument.
NA_IDEF int naMkDir(const char* path, int mode){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _mkdir(path);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return mkdir(path, mode);
  #endif
}


NA_IDEF int naChDir(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _chdir(path);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return chdir(path);
  #endif
}


NA_IDEF NABool naExists(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return !(_access(path, 0));
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return !(access(path, 0));
  #endif
}


NA_IDEF NABool naIsDir(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SystemChar* sysstring = naAllocSystemStringFromString(path, 0);
    NABool retvalue = (GetFileAttributes(sysstring)
            & FILE_ATTRIBUTE_DIRECTORY) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retvalue;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    struct stat stat_struct;
    stat(path, &stat_struct);
    return (stat_struct.st_mode & S_IFDIR) ? NA_TRUE : NA_FALSE;
  #endif
}


NA_IDEF NABool naIsHidden(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SystemChar* sysstring = naAllocSystemStringFromString(path, 0);
    NABool retvalue = (GetFileAttributes(sysstring)
            & FILE_ATTRIBUTE_HIDDEN) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retvalue;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (path[0] == '.');
  #endif
}


NA_IDEF int naRemove(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return remove(path);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return remove(path);
  #endif
}


NA_IDEF NABool naCopyFile(const char* dstpath, const char* srcpath){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (CopyFile( (LPCTSTR)(const char*)srcpath,
                      (LPCTSTR)(const char*)dstpath,
                      NA_FALSE) != 0);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (copyfile(srcpath, dstpath, NULL, COPYFILE_ALL) == 0);
  #endif
}





#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_FILE_INCLUDED

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
