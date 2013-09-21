
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_FILE_INCLUDED
#define NA_FILE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAString.h"

#include <fcntl.h>

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <io.h>
  #include <share.h>
  #include <sys/stat.h>
  #if NA_SYSTEM_ADDRESS_BITS == 64
    typedef __int64 NAFileSize;
  #elif NA_SYSTEM_ADDRESS_BITS == 32
    typedef long NAFileSize;
  #endif
  #define NA_STANDARD_FILE_MODE (_S_IREAD | _S_IWRITE)
  #define NA_FILE_READ_MODE (O_RDONLY | O_BINARY)
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  typedef off_t NAFileSize;
  #define NA_STANDARD_FILE_MODE 0644
  #define NA_FILE_READ_MODE (O_RDONLY)
#endif


// Mapping of standard library functions. They can be different depending on
// the system compiled.
NA_INLINE_API NAFileSize naLseek (int fd, NAFileSize offset, int origin);
NA_INLINE_API int        naOpen  (const char* filename, int flags, int mode);
NA_INLINE_API int        naClose (int fd);
NA_INLINE_API NAFileSize naRead  (int fd, void* buffer, NAFileSize count);


typedef enum{
  NA_ENCODING_UTF_8
} NATextEncoding;

typedef struct NAFile{
  int desc;           // The descriptor
  NABool eof;         // Flag indicating end of file.
  NAFileSize pos;     // The byte position within the file.
//  String path;        // The path
} NAFile;



// Reads the full file and returns it as a new NAByteArray. This function is
// useful if you read or store raw data.
// Warning: The NAByteArray struct is not Endianness-aware! Use the reading
// and writing functions of NAFile when handling multi-byte values.
NAByteArray* naCreateByteArrayFromFilename(NAByteArray* array, const char* filename);

// Reads the full file and returns it as a new String.
// Note that this is the preferred way to handle ASCII-files: Just read the
// whole file into a string and then use the parsing functions of the NAString
// type.
NAString* naCreateStringFromFilename(NAString* string, const char* filename, NATextEncoding encoding);

// Opens the file.
void naFileOpen(NAFile* file, const char* filename, NABool forwriting);

// Closes the file
void naFileClose(NAFile* file);

void naFileReadBytes(NAFile* file, void* buffer, NAFileSize count);

// computes the filesize (from first to last NAByte).
NAFileSize naFileComputeSize(const NAFile* file);





// ////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////
//
// End of API. Everything below this point is implementation and details.
//
// ////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////




NA_INLINE_API NAFileSize naLseek(int fd, NAFileSize offset, int origin){
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


NA_INLINE_API int naOpen(const char* filename, int flags, int mode){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    int handle;
    _sopen_s(&handle, (const char*)filename, flags, _SH_DENYNO, mode);
    return handle;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return open((const char*)filename, flags, mode);
  #endif
}


NA_INLINE_API int naClose(int fd){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _close(fd);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return close(fd);
  #endif
}


NA_INLINE_API NAFileSize naRead(int fd, void* buffer, NAFileSize count){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAFileSize)_read(fd, buffer, (unsigned int)count);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAFileSize)read(fd, buffer, (size_t)count);
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
