
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
// NAFileSize:          The file size integer. Guaranteed to be signed. May be
//                      32 or 64 bits depending on system compiled for. Note
//                      that it may differ from NAInt which can be 32 or 64
//                      bits too depending on the system.
// NA_FILESIZE_BITS and NA_FILESIZE_MAX     Two additional macros
//                      storing the bits needed for a file byteOffset.
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
    typedef __int64 NAFileSize; // Is signed (Important for negative offsets)
    #define NA_FILESIZE_BITS 64
    #define NA_FILESIZE_MAX NA_MAX_i64
  #elif NA_ADDRESS_BITS == 32
    typedef long NAFileSize;    // Is signed (Important for negative offsets)
    #define NA_FILESIZE_BITS 32
    #define NA_FILESIZE_MAX NA_MAX_i32
  #endif
  typedef int NAFileMode;
  #define NA_FILEMODE_DEFAULT (_S_IREAD | _S_IWRITE)
  #define NA_DIRMODE_DEFAULT 0 // No directory modes on windows needed.
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC | O_BINARY)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND | O_BINARY)
#elif NA_IS_POSIX
  #include <unistd.h>
  #include <dirent.h>
  #if NA_OS == NA_OS_MAC_OS_X
    #include <copyfile.h>
  #endif
  typedef off_t NAFileSize;     // Is signed (Important for negative offsets)
  #define NA_FILESIZE_BITS 64
  #define NA_FILESIZE_MAX NA_MAX_i64
  typedef int NAFileMode;
  #define NA_FILEMODE_DEFAULT 0644
  #define NA_DIRMODE_DEFAULT 0755
  #define NA_FILE_OPEN_FLAGS_READ (O_RDONLY) // There is no binary flag in Unix.
  #define NA_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC)
  #define NA_FILE_OPEN_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND)
#else
  #warning File IO not implemented for this system
#endif


// Mapping of standard library functions. They can be different depending on
// the system compiled. Note that only the types which are different depending
// on the system are mapped to NALib types. All other types are corresponding
// to the standards.

NA_IAPI NAFileSize  naLseek   (int fd, NAFileSize byteOffset, int originType);
NA_IAPI NAFileSize  naTell(int fd);
NA_IAPI int         naOpen    (const char* path, int flags, int mode);
NA_IAPI int         naClose   (int fd);
NA_IAPI NAFileSize  naRead    (int fd, void* buf, NAFileSize byteSize);
NA_IAPI NAFileSize  naWrite   (int fd, const void* buf, NAFileSize byteSize);
NA_IAPI int         naMkDir   (const char* path, int mode);
NA_IAPI int         naChDir   (const char* path);
NA_IAPI NABool      naExists  (const char* path);
NA_API  NABool      naIsDir   (const char* path);
NA_API  NABool      naIsHidden(const char* path);
NA_IAPI int         naRemove  (const char* path);
NA_IAPI NABool      naCopyFile(const char* dstPath, const char* srcPath);
NA_IAPI NABool      naAccess  (const char* path,
                                    NABool doesExists,
                                    NABool canRead,
                                    NABool canWrite,
                                    NABool canExecute);
NA_IAPI NAUTF8Char* naGetCwd  (NAUTF8Char* buf, NAInt bufSize);



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

NA_IAPI NAFile* naCreateFileReadingPath(   const char* filePath);
NA_IAPI NAFile* naCreateFileWritingPath(   const char* filePath,
                                                NAFileMode mode);
NA_IAPI NAFile* naCreateFileAppendingPath( const char* filePath,
                                                NAFileMode mode);

// Makes an NAFile struct wrapped around the standard channels.
// Stdin:     Standard input  (file descriptor 0) as a  reading   file.
// Stdout:    Standard output (file descriptor 1) as an appending file.
// Stderr:    Standard error  (file descriptor 2) as an appending file.
//
// Note that the standard input and output channels will not be closed upon
// a call to naRelease. If you want to close them, you need to call
// naClose(0), naClose(1) or naClose(2) manually.
NA_IAPI NAFile* naCreateFileReadingStdin(void);
NA_IAPI NAFile* naCreateFileWritingStdout(void);
NA_IAPI NAFile* naCreateFileWritingStderr(void);

// Computes the fileSize (from first to last byte).
NA_IAPI NAFileSize naComputeFileByteSize(const NAFile* file);

// Tests if a file has been opened.
NA_IAPI NABool naIsFileOpen(const NAFile* file);

// Re-adjusts the internal file pointer to the given offset.
NA_IAPI void naSeekFileAbsolute(NAFile* file, NAFileSize byteOffset);
NA_IAPI void naSeekFileRelative(NAFile* file, NAFileSize byteOffset);

// Reads the given number of bytes and stores it without further manipulation
// in buf. The buffer must be big enough, no overflow check is made. This is
// basically just an encapsulating method for naRead(). Have a look at the
// NABuffer structure which gives you way more power for reading and writing
// files or with any kind of buffers.
//
// Returns the number of bytes read.
NA_IAPI NAFileSize naReadFileBytes( NAFile* file,
                                      void* buf,
                                 NAFileSize byteSize);

// Writes the given number of bytes from ptr to the file without further
// manipulation. The buffer must be big enough, no overflow check is made.
// This is basically just an encapsulating method for naWrite(). Have a look
// at the NABuffer structure which gives you way more power for reading and
// writing files with any kind of buffers.
//
// Returns the number of bytes written.
NA_IAPI NAFileSize naWriteFileBytes(  NAFile* file,
                                  const void* ptr,
                                   NAFileSize byteSize);


// //////////////////////////
// General input and output methods
//

// Scans a Decimal of the type int from the console.
NA_IAPI int naScanDecimal(void);

// Returns a string containing the current working directory
NA_API NAString* naNewStringWithCurWorkingDirectory(void);




// Inline implementations are in a separate file:
#include "NAFile/NAFileII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_FILE_INCLUDED



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
