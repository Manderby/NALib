
// This file contains inline implementations of the file NAFile.h
// Do not include this file directly! It will automatically be included when
// including "NAFile.h"



// Use this function with originType = SEEK_SET, SEEK_CUR or SEEK_END.
// Note that all systems define fsize_t to be a signed integer. Therefore
// searching backwards with SEEK_CUR is possible.
NA_IDEF fsize_t naLseek(int fd, fsize_t byteOffset, int originType) {
  #if NA_OS == NA_OS_WINDOWS
    #if NA_ADDRESS_BITS == 64
      return _lseeki64(fd, byteOffset, originType);
    #elif NA_ADDRESS_BITS == 32
      return _lseek(fd, byteOffset, originType);
    #else
      #error "Address size not supported"
      return 0;
    #endif
  #elif NA_IS_POSIX
    return lseek(fd, byteOffset, originType);
  #endif
}



// Use this function with originType = SEEK_SET, SEEK_CUR or SEEK_END.
// Note that all systems define fsize_t to be a signed integer. Therefore
// searching backwards with SEEK_CUR is possible.
NA_IDEF fsize_t naTell(int fd) {
  #if NA_OS == NA_OS_WINDOWS
    #if NA_ADDRESS_BITS == 64
      return _lseeki64(fd, 0, SEEK_CUR);
    #elif NA_ADDRESS_BITS == 32
      return _lseek(fd, 0, SEEK_CUR);
    #else
      #error "Address size not supported"
      return 0;
    #endif
  #elif NA_IS_POSIX
    return lseek(fd, 0, SEEK_CUR);
  #endif
}


// Use the flags argument to define how to open the file.
//
// For the flags, use one of the macros NA_FILE_OPEN_FLAGS_READ,
// NA_FILE_OPEN_FLAGS_WRITE or NA_FILE_OPEN_FLAGS_APPEND if you just need the
// standard ways of accessing files.
//
// For writing files, use NA_FILEMODE_DEFAULT if you don't know what to use
// for the mode argument.
NA_IDEF int naOpen(const char* path, int flags, int mode) {
  #if NA_OS == NA_OS_WINDOWS
    int handle;
    _sopen_s(&handle, path, flags, _SH_DENYNO, mode);
    return handle;
  #elif NA_IS_POSIX
    return open(path, flags, mode);
  #endif
}


NA_IDEF int naClose(int fd) {
  #if NA_OS == NA_OS_WINDOWS
    return _close(fd);
  #elif NA_IS_POSIX
    return close(fd);
  #endif
}


NA_IDEF fsize_t naRead(int fd, void* buf, fsize_t byteSize) {
  #if NA_OS == NA_OS_WINDOWS
    return (fsize_t)_read(fd, buf, (unsigned int)byteSize);
  #elif NA_IS_POSIX
    return (fsize_t)read(fd, buf, (size_t)byteSize);
  #endif
}


NA_IDEF fsize_t naWrite(int fd, const void* buf, fsize_t byteSize) {
  #if NA_OS == NA_OS_WINDOWS
    return (fsize_t)_write(fd, buf, (unsigned int)byteSize);
  #elif NA_IS_POSIX
    return (fsize_t)write(fd, buf, (size_t)byteSize);
  #endif
}


// Use NA_DIRMODE_DEFAULT if you don't know what to use for the mode
// argument.
NA_IDEF int naMkDir(const char* path, int mode) {
  NA_UNUSED(mode);
  #if NA_OS == NA_OS_WINDOWS
    return _mkdir(path);
  #elif NA_IS_POSIX
    return mkdir(path, (mode_t)mode);
  #endif
}


NA_IDEF int naChDir(const char* path) {
  #if NA_OS == NA_OS_WINDOWS
    return _chdir(path);
  #elif NA_IS_POSIX
    return chdir(path);
  #endif
}


NA_IDEF NABool naExists(const char* path) {
  #if NA_OS == NA_OS_WINDOWS
    return !(_access(path, 0));
  #elif NA_IS_POSIX
    return !(access(path, 0));
  #endif
}


NA_IDEF int naRemove(const char* path) {
  #if NA_OS == NA_OS_WINDOWS
    return remove(path);
  #elif NA_IS_POSIX
    return remove(path);
  #endif
}


NA_IDEF NABool naCopyFile(const char* dstPath, const char* srcPath) {
  #if NA_OS == NA_OS_WINDOWS
    return (CopyFile( (LPCTSTR)(const char*)srcPath,
                      (LPCTSTR)(const char*)dstPath,
                      NA_FALSE) != 0);
  #elif NA_OS == NA_OS_MAC_OS_X
    return (copyfile(srcPath, dstPath, NULL, COPYFILE_ALL) == 0);
  #elif NA_OS == NA_OS_FREEBSD
    int srcFd = open(srcPath, O_RDONLY);
    if(srcFd < 0)
	    return NA_FALSE;

    int dstFd = open(dstPath, O_WRONLY|O_CREAT|O_TRUNC);
    if(dstFd < 0) {
	    close(srcFd);
	    return NA_FALSE;
    }

    int rc = copy_file_range(srcFd, NULL, dstFd, NULL, NA_FILESIZE_MAX, 0);

    close(dstFd);
    close(srcFd);

    return !rc;
  #endif
}



NA_IDEF NABool naAccess(const char* path, NABool doesExists, NABool canRead, NABool canWrite, NABool canExecute) {
  NA_UNUSED(doesExists);
  #if NA_OS == NA_OS_WINDOWS
    int testMode = 0;
    // testMode |= (exists?00:0); // This line has been commented out because of static code analysis.
    testMode |= (canRead?04:0);
    testMode |= (canWrite?02:0);
    NA_UNUSED(canExecute); // Under windows, the executable flag does not exist.
    return (_access(path, testMode) == 0);
  #elif NA_IS_POSIX
    int testMode = 0;
    testMode |= (doesExists?F_OK:0);
    testMode |= (canRead?R_OK:0);
    testMode |= (canWrite?W_OK:0);
    testMode |= (canExecute?X_OK:0);
    return (access(path, testMode) == 0);
  #endif
}


#include <errno.h>
NA_IDEF NAUTF8Char* naAllocCwd(NAUTF8Char* buf, size_t bufSize) {
  #if NA_OS == NA_OS_WINDOWS
    return _getcwd(buf, (int)bufSize);
  #elif NA_IS_POSIX
    if(!buf) {
      bufSize = PATH_MAX + 1;
      buf = naMalloc(bufSize);
    }
    char* ret = getcwd(buf, bufSize);
    return ret;
  #endif
}






// //////////////////////////////
// NAFile
// //////////////////////////////



struct NAFile{
  int desc;   // The descriptor
  #if NA_ADDRESS_BITS != NA_TYPE_NATIVE_INT_BITS
    void* dummy;  // We need more bytes otherwise the runtime system can not
                  // properly store the values.
  #endif
};

NA_HAPI void na_DestroyFile(NAFile* file);
NA_EXTERN_RUNTIME_TYPE(NAFile);



NA_IDEF NAFile* naCreateFileReadingPath(const char* filePath) {
  NAFile* file = naCreate(NAFile);
  file->desc = naOpen(filePath, NA_FILE_OPEN_FLAGS_READ, NA_FILEMODE_DEFAULT);
  #if NA_DEBUG
    if(file->desc < 0)
      naError("Could not open file.");
  #endif
  return file;
}



NA_IDEF NAFile* naCreateFileWritingPath(const char* filePath, NAFileMode mode) {
  NAFile* file = naCreate(NAFile);
  file->desc = naOpen(filePath, NA_FILE_OPEN_FLAGS_WRITE, mode);
  #if NA_DEBUG
    if(file->desc < 0)
      naError("Could not create file.");
  #endif
  return file;
}



NA_IDEF NAFile* naCreateFileAppendingPath(const char* filePath, NAFileMode mode) {
  NAFile* file = naCreate(NAFile);
  file->desc = naOpen(filePath, NA_FILE_OPEN_FLAGS_APPEND, mode);
  #if NA_DEBUG
    if(file->desc < 0)
      naError("Could not create file.");
  #endif
  return file;
}



NA_IDEF NAFile* naCreateFileReadingStdin() {
  NAFile* file = naCreate(NAFile);
  file->desc = 0;
  return file;
}



NA_IDEF NAFile* naCreateFileWritingStdout() {
  NAFile* file = naCreate(NAFile);
  file->desc = 1;
  return file;
}



NA_IDEF NAFile* naCreateFileWritingStderr() {
  NAFile* file = naCreate(NAFile);
  file->desc = 2;
  return file;
}



NA_IDEF fsize_t naComputeFileByteSize(const NAFile* file) {
  fsize_t curOffset;
  fsize_t fileSize;
  curOffset = naLseek(file->desc, 0, SEEK_CUR);
  #if NA_DEBUG
    if(curOffset == -1)
      naError("An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #endif
  fileSize = naLseek(file->desc, 0, SEEK_END);
  naLseek(file->desc, curOffset, SEEK_SET);
  return fileSize;
}



NA_IDEF NABool naIsFileOpen(const NAFile* file) {
  return (file->desc >= 0);
}



NA_IDEF void naSeekFileAbsolute(NAFile* file, fsize_t byteOffset) {
  fsize_t newOffset;
  #if NA_DEBUG
    if(byteOffset < 0)
      naError("Negative offset in absolute jump.");
  #endif
  newOffset = naLseek(file->desc, byteOffset, SEEK_SET);
  #if NA_DEBUG
    if(newOffset == -1)
      naError("An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #else
    NA_UNUSED(newOffset);
  #endif
}



NA_IDEF void naSeekFileRelative(NAFile* file, fsize_t byteOffset) {
  fsize_t newOffset;
  newOffset = naLseek(file->desc, byteOffset, SEEK_CUR);
  #if NA_DEBUG
    if(newOffset == -1)
      naError("An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #else
    NA_UNUSED(newOffset);
  #endif
}



NA_IDEF fsize_t naReadFileBytes(NAFile* file, void* buf, fsize_t byteSize) {
  if(!byteSize)
    return 0;

  #if NA_DEBUG
    if(!naIsFileOpen(file))
      naError("File is not open.");
    if(!byteSize)
      naError("Reading zero bytes.");
    if(byteSize < 0)
      naError("Negative count.");
  #endif
   
  return naRead(file->desc, buf, byteSize);
}


NA_IDEF fsize_t naWriteFileBytes(NAFile* file, const void* ptr, fsize_t byteSize) {
  if(!byteSize)
    return 0;

  #if NA_DEBUG
    if(!naIsFileOpen(file))
      naError("File is not open.");
    if(!byteSize)
      naError("Writing zero bytes.");
    if(byteSize < 0)
      naError("Negative count.");
  #endif

  return naWrite(file->desc, ptr, byteSize);
}




// ////////////////////////////
// General input output functions
// ////////////////////////////



NA_IDEF int naScanDecimal() {
  int retValue;
  #if NA_OS == NA_OS_WINDOWS
    scanf_s("%d", &retValue);
  #elif NA_IS_POSIX
    scanf("%d", &retValue);
  #else
    scanf("%d", &retValue);  // Might not compile on an undetected system.
  #endif
  return retValue;
}



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
