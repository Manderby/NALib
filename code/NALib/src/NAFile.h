
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_FILE_INCLUDED
#define NA_FILE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// The typedef needs to be here to resolve cyclic include problems.
typedef struct NAFile NAFile;



// This file contains the declaration of the NAFile structure, which is a
// structure managing a simple file descriptor but with a reference count.
// See further below.
//
// This file first contains the definitions for very basic file and directory
// handling.
//
// The structs and functions in ths file only provide very raw file handling
// functionality. If you are looking for powerful reading and writing
// functions, have a look at NABuffer.

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "NAString.h"

// File Input and output is handeled slightly differently on different
// operating systems. In the header files of each system, there are different
// definitions of standard C functions and hence, NALib is trying to map
// everything to one definition.
//
// New types and macros introduced:
// NAFilesize:          The file size integer. Guaranteed to be signed. May be
//                      32 or 64 bits depending on system compiled for. Note
//                      that it may differ from NAInt which can be 32 or 64
//                      bits too depending on the system.
// NA_FILESIZE_BITS and NA_FILESIZE_MAX     Two additional macros
//                      storing the bits needed for a file byteoffset.
// NAFileMode:          The filesystem permission mode. On Windows corresponds
//                      to a flag combination like _S_IREAD, and on Unix
//                      corresponds to the octal permission flags like 0644.
//
// NA_FILEMODE_DEFAULT: The default permission mode to create a file.
// NA_DIRMODE_DEFAULT:  The default permission mode to create a directory. Not
//                      needed on Windows.
//
// The following macros define the NALib standard file opening flags. They
// denote combinations with flags like O_RDONLY and are needed for the naOpen
// function. You may choose to set the flags manually or use one of these three
// macros, whereas note that all three macros open files in BINARY format:
//
// NA_FILE_OPEN_FLAGS_READ   Read-Only, Cursor placed at byte 0
// NA_FILE_OPEN_FLAGS_WRITE  Write-Only, File truncated. Cursor placed at byte 0
// NA_FILE_OPEN_FLAGS_APPEND Write-Only, Cursor placed at end of file

#if NA_OS == NA_OS_WINDOWS
  #include <windows.h>
  #include <direct.h>
  #include <io.h>
  #include <share.h>
  #include <sys/stat.h>
  #if NA_ADDRESS_BITS == 64
    typedef __int64 NAFilesize; // Is signed (Important for negative offsets)
    #define NA_FILESIZE_BITS 64
    #define NA_FILESIZE_MAX NA_INT64_MAX
  #elif NA_ADDRESS_BITS == 32
    typedef long NAFilesize;    // Is signed (Important for negative offsets)
  #define NA_FILESIZE_BITS 32
  #define NA_FILESIZE_MAX NA_INT32_MAX
#endif
  typedef int NAFileMode;
  #define NA_FILEMODE_DEFAULT (_S_IREAD | _S_IWRITE)
  #define NA_DIRMODE_DEFAULT 0 // No directory modes on windows needed.
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND | O_BINARY)
#elif NA_OS == NA_OS_MAC_OS_X
  #include <unistd.h>
  #include <dirent.h>
  #include <copyfile.h>
  typedef off_t NAFilesize;     // Is signed (Important for negative offsets)
  #define NA_FILESIZE_BITS 64
  #define NA_FILESIZE_MAX NA_INT64_MAX
  typedef int NAFileMode;
  #define NA_FILEMODE_DEFAULT 0644
  #define NA_DIRMODE_DEFAULT 0755
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY) // There is no binary flag in Unix.
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND)
#endif


// Mapping of standard library functions. They can be different depending on
// the system compiled. Note that only the types which are different depending
// on the system are mapped to NALib types. All other types are corresponding
// to the standards.

NA_IAPI NAFilesize  naLseek   (int fd, NAFilesize byteoffset, int origintype);
NA_IAPI NAFilesize  naTell(int fd);
NA_IAPI int         naOpen    (const char* path, int flags, int mode);
NA_IAPI int         naClose   (int fd);
NA_IAPI NAFilesize  naRead    (int fd, void* buf, NAFilesize bytesize);
NA_IAPI NAFilesize  naWrite   (int fd, const void* buf, NAFilesize bytesize);
NA_IAPI int         naMkDir   (const char* path, int mode);
NA_IAPI int         naChDir   (const char* path);
NA_IAPI NABool      naExists  (const char* path);
NA_API  NABool      naIsDir   (const char* path);
NA_API  NABool      naIsHidden(const char* path);
NA_IAPI int         naRemove  (const char* path);
NA_IAPI NABool      naCopyFile(const char* dstpath, const char* srcpath);
NA_IAPI NABool      naAccess  (const char* path,
                                    NABool doesExists,
                                    NABool canRead,
                                    NABool canWrite,
                                    NABool canExecute);
NA_IAPI NAUTF8Char* naGetCwd  (NAUTF8Char* buf, NAInt bufsize);



// ///////////////////////////////////////////
// NAFile
//
// These are the declarations of NAFile. It is designed to be a very simple
// struct with only a file descriptor but with a reference count. Therefore
// you can retain and release files.
//
// An NAFile can either be a Read-File or a Write-File. Never both.
//
// Again, this struct only provides the most basic functionality of file input
// and output. If you look for more elaborate functions, see NABuffer.




// Opens the file.
// Reading:   Opens an existing file and places the read pointer to the first
//            byte of the stream.
// Writing:   Creates a new file or overwrites and truncates an existing file
//            and places the write pointer to the first byte of the stream.
// Appending: Creates a new file or opens an existing file for appending. The
//            write pointer is set to the end of the stream.
//
// If you don't know, what the mode argument means, use NA_FILEMODE_DEFAULT.

NA_IAPI NAFile* naCreateFileReadingFilename(   const char* filename);
NA_IAPI NAFile* naCreateFileWritingFilename(   const char* filename,
                                                NAFileMode mode);
NA_IAPI NAFile* naCreateFileAppendingFilename( const char* filename,
                                                NAFileMode mode);

// Makes an NAFile struct wrapped around the standard channels.
// Stdin:     Standard input  (file descriptor 0) as a  reading   file.
// Stdout:    Standard output (file descriptor 1) as an appending file.
// Stderr:    Standard error  (file descriptor 2) as an appending file.
//
// Note that the standard input and output channels will not be closed upon
// a call to naReleaseFile. If you want to close them, you need to call
// naClose(0), naClose(1) or naClose(2) manually.
NA_IAPI NAFile* naCreateFileReadingStdin(void);
NA_IAPI NAFile* naCreateFileWritingStdout(void);
NA_IAPI NAFile* naCreateFileWritingStderr(void);

// Releases the file. When the reference count reaches zero, the file will be
// closed.
//
// Again: stdin, stdout and stderr will never be closed by these calls. Use
// naClose(0), naClose(1) or naClose(2) for that.
NA_IAPI void    naReleaseFile(NAFile* file);

// Computes the filesize (from first to last byte).
NA_IAPI NAFilesize naComputeFileBytesize(const NAFile* file);

// Tests if a file has been opened.
NA_IAPI NABool naIsFileOpen(const NAFile* file);

// Re-adjusts the internal file pointer to the given offset.
NA_IAPI void naSeekFileAbsolute(NAFile* file, NAFilesize byteoffset);
NA_IAPI void naSeekFileRelative(NAFile* file, NAFilesize byteoffset);

// Reads the given number of bytes and stores it without further manipulation
// in buf. The buffer must be big enough, no overflow check is made. This is
// basically just an encapsulating method for naRead(). Have a look at the
// NABuffer structure which gives you way more power for reading and writing
// files or with any kind of buffers.
//
// Returns the number of bytes read.
NA_IAPI NAFilesize naReadFileBytes( NAFile* file,
                                      void* buf,
                                 NAFilesize bytesize);

// Writes the given number of bytes from ptr to the file without further
// manipulation. The buffer must be big enough, no overflow check is made.
// This is basically just an encapsulating method for naWrite(). Have a look
// at the NABuffer structure which gives you way more power for reading and
// writing files with any kind of buffers.
//
// Returns the number of bytes written.
NA_IAPI NAFilesize naWriteFileBytes(  NAFile* file,
                                  const void* ptr,
                                   NAFilesize bytesize);


// //////////////////////////
// General input and output methods
//

// Scans a Decimal of the type NAInt from the console.
NA_IAPI NAInt naScanDecimal(void);

// Returns a string containing the current working directory
NA_API NAString* naNewStringWithCurWorkingDirectory(void);




// Inline implementations are in a separate file:
#include "NACore/NAFileII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_FILE_INCLUDED

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
