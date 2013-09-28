
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_FILE_INCLUDED
#define NA_FILE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAString.h"
#include "NAEndianness.h"
#include <sys/stat.h>
#include <fcntl.h>

#define NA_FILE_FLAG_EOF                  0x01
#define NA_FILE_FLAG_WRITING              0x02
#define NA_FILE_FLAG_STREAM               0x04

#define NA_FILE_FLAG_NEWLINE_MASK         0x30
#define NA_FILE_FLAG_NEWLINE_UNIX         0x10
#define NA_FILE_FLAG_NEWLINE_MAC9         0x20
#define NA_FILE_FLAG_NEWLINE_WIN          0x30

#define NA_FILE_FLAG_AUTOFLUSH_MASK       0xc0
#define NA_FILE_FLAG_AUTOFLUSH_NONE       0x00
#define NA_FILE_FLAG_AUTOFLUSH_TEXT       0x40
#define NA_FILE_FLAG_AUTOFLUSH_MULTIBYTE  0x80
#define NA_FILE_FLAG_AUTOFLUSH_ALL        0xc0

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
  #define NA_DIRECTORY_DELIMITER "\\"
  #define NA_FILE_FLAG_NEWLINE_NATIVE NA_FILE_FLAG_NEWLINE_WIN
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
  #define NA_DIRECTORY_DELIMITER "/"
  #define NA_FILE_FLAG_NEWLINE_NATIVE NA_FILE_FLAG_NEWLINE_UNIX
#endif

// buffer must not be greater than 65536 as uint16 is used as counter.
// buffer must not be smaller than 16 as BinaryData reader may require 16 Bytes.
#define NA_FILE_BUFFER_SIZE 4096

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

typedef enum{
  NA_ENCODING_UTF_8
} NATextEncoding;


typedef struct NAFile{
  int desc;           // The descriptor
  NAFileSize pos;     // The byte position within the underlying file.
  NAEndiannessConverter converter;  // The endianness converter.
  uint16 flags;       // various flags
  uint16 remainingbytesinbuffer;  // remaining bytes inside the buffer
  NAByte* bufptr;     // Pointer to the current byte in the buffer
  NAByte buffer[NA_FILE_BUFFER_SIZE]; // The buffer.
} NAFile;



// Important remark about ASCII and binary files:
// File types can roughly be categorized into two classes: ASCII files and
// binary files. Both file types have their Pros and Cons. Which one to use
// is up to the user. The NAFile structure allows to read and write both type
// of files.
//
// Writing files is usually done sequentially for both ASCII and binary files:
// You write some bytes or a string, then you write some other bytes or another
// string, and so on, until the whole file is written.
//
// Reading binary files is also done moslty sequentially: You maybe read an
// integer, then you read a float, and so on. Maybe you jump to a specific
// location in the file and read on sequetially from there.
//
// Reading ASCII files on the other hand is usually done quite differently.
// ASCII files are usually subdivided into a hierarchical structure. An easy
// example is a logfile where each line is a sub-structure. XML-files for
// example have very complex sub-structures where each struture can span
// multiple lines or even the whole file. Parsing such files is anything but
// sequential.
//
// Therefore, when reading an ASCII file, NALib encourages the programmer to
// do the following: Read the whole file into an NAString struct by using a
// single call to naCreateStringFromFileContents. After that, use the parsing
// functions of the NAString struct.
//
// There are ASCII-reading functions in NAFile but they are slower and less
// powerful as the parsing functions of NAString.


// Reads the full file and returns it as a new String.
// Note that this is the preferred way to handle ASCII-files in NALib: Just
// read the whole file into a string and then use the parsing functions of
// the NAString type.
// The encoding defines what the file is encoded in. The resulting string will
// be encoded in UTF-8. todo: Add more encodings.
NAString* naCreateStringFromFileContents(NAString* string, const char* filename, NATextEncoding encoding);

// Reads the full file and returns it as a new NAByteArray.
// Warning: This function is only useful if you read or store raw data! The 
// NAByteArray struct is not endianness-aware. Use the reading and writing
// functions of NAFile when handling multi-byte values instead.
NAByteArray* naCreateByteArrayFromFileContents(NAByteArray* array, const char* filename);

// Opens the file.
// Reading:   Opens an existing file and places the read pointer to the first
//            byte of the stream. The endianness converter is set to native.
//            Reading is buffered by default.
// Writing:   Creates a new file or overwrites and truncates an existing file
//            and places the write pointer to the first byte of the stream.
//            The endianness converter is set to native. Writing is buffered by
//            default.
// Appending: Creates a new file or opens an existing file for appending. The
//            write pointer is set to the end of the stream. The endianness
//            converter is uninitialized! Text is auto-flushed by default.
// If you don't know, what the mode argument means, use NA_FILE_MODE_DEFAULT.
// You can change the buffering-behaviour using naSetFileAutoFlush.
NAFile naOpenFileForReading(const char* filename);
NAFile naOpenFileForWriting(const char* filename, NAFileMode mode);
NAFile naOpenFileForAppending(const char* filename, NAFileMode mode);

// Makes an NAFile struct wrapped around the standard channels.
// Stdin:     Standard input (file descriptor 0) as a reading file.
// Stdout:    Standard output (file descriptor 1) as an appending file.
// Stderr:    Standard error (file descriptor 2) as an appending file.
// Stdin, stdout and stderr are unbuffered by default. You can change the
// buffering-behaviour using naSetFileAutoFlush.
NAFile naMakeFileAsStdin();
NAFile naMakeFileAsStdout();
NAFile naMakeFileAsStderr();

// Closes the file. Note: Can close stdin, stdout and stderr!
void naCloseFile(NAFile* file);

// Flushes the buffer. Only useful for writing files.
void naFlushFileBuffer(NAFile* file);

// Sets the endianness of the file and setups all converters.
// - If this file is a reading file, the endianness is the input encoding and
//   every multi-byte value will be converted from input to native endianness
//   after reading automatically.
// - If this file is a writing file, the endianness is the output encoding and
//   every multy-byte value will be converted from native to output endianness
//   before writing automatically.
// Use endianness constants like NA_ENDIANNESS_BIG for the endianness argument.
void naSetFileEndianness(NAFile* file, NAInt endianness);

// Only useful for Write-files. Defines, how writings should be flushed. The
// NAFile struct by default uses a buffer to speed up writing. You can manually
// configure what will automatically be flushed using the following flags:
// NA_FILE_FLAG_AUTOFLUSH_NONE    Writing is buffered
// NA_FILE_FLAG_AUTOFLUSH_TEXT    Auto-flushes Strings, Lines and NewLines
// NA_FILE_FLAG_AUTOFLUSH_ARRAY   Auto-flushes Strings, Lines and Byte arrays
// NA_FILE_FLAG_AUTOFLUSH_ALL     No buffer. Auto-Flushing all output
void naSetFileAutoFlush(NAFile* file, uint16 autoflushflag);

// Only useful for Write-files. Defines, how a line ending shall be encoded.
// NA_FILE_FLAG_NEWLINE_UNIX     \n    Used in Unix, Linux, MacOSX
// NA_FILE_FLAG_NEWLINE_MAC9     \r    Used in old Mac OS
// NA_FILE_FLAG_NEWLINE_WIN      \r\n  Used in Windows
// NA_FILE_FLAG_NEWLINE_NATIVE         Dependant on the local machines system
// Note that this function only affects writeLine and writeNewLine calls. If
// you somehow write a different line-ending for example as part of a string,
// that line-ending will not be converted!
void naSetFileNewLine(NAFile* file, uint16 newlineflag);

// computes the filesize (from first to last NAByte).
NAFileSize naComputeFileSize(const NAFile* file);
NABool naHasFileEnded(const NAFile* file);

// Readjusts the internal file pointer to the given offset.
void naJumpFileOffsetAbsolute(NAFile* file, NAFileSize offset);
void naJumpFileOffsetRelative(NAFile* file, NAFileSize offset);



// //////////
// READING
// //////////

// Reads the given number of bytes and stores it without further manipulation
// in buf. The buffer must be big enough, no overflow check is made. This is a
// low-level function. The result of the naCreateByteArrayFromFile function is
// more convenient to handle.
void    naReadFileBytes (NAFile* file, void* buf, NAFileSize count);

// Read different basic datatypes. Endianness-aware!
int8    naReadFileInt8  (NAFile* file);
int16   naReadFileInt16 (NAFile* file);
int32   naReadFileInt32 (NAFile* file);
int64   naReadFileInt64 (NAFile* file);
uint8   naReadFileUInt8 (NAFile* file);
uint16  naReadFileUInt16(NAFile* file);
uint32  naReadFileUInt32(NAFile* file);
uint64  naReadFileUInt64(NAFile* file);
float   naReadFileFloat (NAFile* file);
double  naReadFileDouble(NAFile* file);
// todo: Add the same thing for array converters.

// Creates or fills an NAByteArray or an NAString by reading the given number
// of bytes from the file and then COPYING them to the array or string.
// Note: If you want the whole file as an NAByteArray or NAString, use the
// naCreateByteArrayFromFileContents or naCreateStringFromFileContents function
// instead which directly reads the bytes to the array instead of copying them
// from an internal buffer.
// COPIES ALWAYS!
NAByteArray* naCreateByteArrayFromFile(NAByteArray* array,
                                            NAFile* file,
                                         NAFileSize count);
NAString*    naCreateStringFromFile(      NAString* string,
                                            NAFile* file,
                                         NAFileSize bytecount,
                                     NATextEncoding encoding);

// //////////
// BINARY WRITING
// //////////

// Writes the given number of bytes from buf to the file without further
// manipulation. The buffer must be big enough, no overflow check is made.
// This is a low-level function. The naWriteFileByteArray function is more
// convenient to handle if you are working with byte arrays.
void naWriteFileBytes(NAFile* file, const void* ptr, NAFileSize count);

// Writes some standard data type. Endianness-aware!
void naWriteFileInt8  (NAFile* file, int8 value);
void naWriteFileInt16 (NAFile* file, int16 value);
void naWriteFileInt32 (NAFile* file, int32 value);
void naWriteFileInt64 (NAFile* file, int64 value);
void naWriteFileUInt8 (NAFile* file, uint8 value);
void naWriteFileUInt16(NAFile* file, uint16 value);
void naWriteFileUInt32(NAFile* file, uint32 value);
void naWriteFileUInt64(NAFile* file, uint64 value);
void naWriteFileFloat (NAFile* file, float value);
void naWriteFileDouble(NAFile* file, double value);
// todo: Add the same thing for array converters.

// Writes a byte array to the file.
void naWriteFileByteArray( NAFile* file, 
                const NAByteArray* array);


// //////////
// ASCII WRITING
// //////////

// Writes a tab or a newline to the file. The newline character is dependent on
// the files setting. You can change it using naSetFileNewLine.
void naWriteFileTab(NAFile* file);
void naWriteFileNewLine(NAFile* file);

// Writes the given string to the file. If the string is null-terminated, that
// NULL will not be written to the file. Newline characters are not converted!
// Currently, only the UTF-8 encoding is possible.
void naWriteFileString(    NAFile* file,
                   const NAString* string,
                    NATextEncoding encoding);

// Writes a string to the file which can be written like a printf format. You
// can also use this function just to write a simple const char* string. The
// encoding is UTF-8.
void naWriteFileStringWithFormat(NAFile* file,
                       const NAUTF8Char* format,
                                         ...);
// Same as naWriteFileStringWithFormat but with an existing va_list argument.
// The argumentlist argument will not be altered by this function. The encoding
// is UTF-8.
void naWriteFileStringWithArguments(NAFile* file,
                          const NAUTF8Char* format,
                                    va_list argumentlist);

// Same as the functions above but automatically appends a newline character
// at the end. The newline character is dependent on the files setting. You can
// change it using naSetFileNewLine.
void naWriteFileLine(             NAFile* file,
                          const NAString* string,
                           NATextEncoding encoding);
void naWriteFileLineWithFormat(   NAFile* file,
                        const NAUTF8Char* format,
                                          ...);
void naWriteFileLineWithArguments(NAFile* file,
                        const NAUTF8Char* format,
                                  va_list argumentlist);











// ////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////
//
// End of API. Everything below this point is implementation and details.
//
// ////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////




NA_IAPI NAFileSize naLseek(int fd, NAFileSize offset, int origin){
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
NA_IAPI int naOpen(const char* path, int flags, int mode){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    int handle;
    _sopen_s(&handle, path, flags, _SH_DENYNO, mode);
    return handle;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return open(path, flags, mode);
  #endif
}


NA_IAPI int naClose(int fd){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _close(fd);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return close(fd);
  #endif
}


NA_IAPI NAFileSize naRead(int fd, void* buf, NAFileSize count){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAFileSize)_read(fd, buf, (unsigned int)count);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAFileSize)read(fd, buf, (size_t)count);
  #endif
}


NA_IAPI NAFileSize naWrite(int fd, const void* buf, NAFileSize count){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAFileSize)_write(fd, buf, (size_t)count);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAFileSize)write(fd, buf, (size_t)count);
  #endif
}


// Use NA_DIR_MODE_DEFAULT if you don't know what to use for the mode
// argument.
NA_IAPI int naMkDir(const char* path, int mode){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _mkdir(path);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return mkdir(path, mode);
  #endif
}


NA_IAPI int naChDir(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _chdir(path);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return chdir(path);
  #endif
}


NA_IAPI NABool naExists(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return !(_access(path, 0));
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return !(access(path, 0));
  #endif
}


NA_IAPI NABool naIsDir(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SystemChar* sysstring = naCreateSystemStringFromString(path, 0);
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


NA_IAPI NABool naIsHidden(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SystemChar* sysstring = naCreateSystemStringFromString(path, 0);
    NABool retvalue = (GetFileAttributes(sysstring)
            & FILE_ATTRIBUTE_HIDDEN) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retvalue;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (path[0] == '.');
  #endif
}


NA_IAPI int naRemove(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return remove(path);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return remove(path);
  #endif
}


NA_IAPI NABool naCopyFile(const char* dstpath, const char* srcpath){
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
