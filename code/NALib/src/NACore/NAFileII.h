
// This file contains inline implementations of the file NAFile.h
// Do not include this file directly! It will automatically be included when
// including "NAFile.h"



// Use this function with originType = SEEK_SET, SEEK_CUR or SEEK_END.
// Note that all systems define NAFileSize to be a signed integer. Therefore
// searching backwards with SEEK_CUR is possible.
NA_IDEF NAFileSize naLseek(int fd, NAFileSize byteOffset, int originType){
  #if NA_OS == NA_OS_WINDOWS
    #if NA_ADDRESS_BITS == 64
      return _lseeki64(fd, byteOffset, originType);
    #elif NA_ADDRESS_BITS == 32
      return _lseek(fd, byteOffset, originType);
    #endif
  #elif NA_OS == NA_OS_MAC_OS_X
    return lseek(fd, byteOffset, originType);
  #endif
}



// Use this function with originType = SEEK_SET, SEEK_CUR or SEEK_END.
// Note that all systems define NAFileSize to be a signed integer. Therefore
// searching backwards with SEEK_CUR is possible.
NA_IDEF NAFileSize naTell(int fd){
  #if NA_OS == NA_OS_WINDOWS
    #if NA_ADDRESS_BITS == 64
      return _lseeki64(fd, 0, SEEK_CUR);
    #elif NA_ADDRESS_BITS == 32
      return _lseek(fd, 0, SEEK_CUR);
    #endif
  #elif NA_OS == NA_OS_MAC_OS_X
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
NA_IDEF int naOpen(const char* path, int flags, int mode){
  #if NA_OS == NA_OS_WINDOWS
    int handle;
    _sopen_s(&handle, path, flags, _SH_DENYNO, mode);
    return handle;
  #elif NA_OS == NA_OS_MAC_OS_X
    return open(path, flags, mode);
  #endif
}


NA_IDEF int naClose(int fd){
  #if NA_OS == NA_OS_WINDOWS
    return _close(fd);
  #elif NA_OS == NA_OS_MAC_OS_X
    return close(fd);
  #endif
}


NA_IDEF NAFileSize naRead(int fd, void* buf, NAFileSize byteSize){
  #if NA_OS == NA_OS_WINDOWS
    return (NAFileSize)_read(fd, buf, (unsigned int)byteSize);
  #elif NA_OS == NA_OS_MAC_OS_X
    return (NAFileSize)read(fd, buf, (size_t)byteSize);
  #endif
}


NA_IDEF NAFileSize naWrite(int fd, const void* buf, NAFileSize byteSize){
  #if NA_OS == NA_OS_WINDOWS
    return (NAFileSize)_write(fd, buf, (unsigned int)byteSize);
  #elif NA_OS == NA_OS_MAC_OS_X
    return (NAFileSize)write(fd, buf, (size_t)byteSize);
  #endif
}


// Use NA_DIRMODE_DEFAULT if you don't know what to use for the mode
// argument.
NA_IDEF int naMkDir(const char* path, int mode){
  #if NA_OS == NA_OS_WINDOWS
    return _mkdir(path);
  #elif NA_OS == NA_OS_MAC_OS_X
    return mkdir(path, (mode_t)mode);
  #endif
}


NA_IDEF int naChDir(const char* path){
  #if NA_OS == NA_OS_WINDOWS
    return _chdir(path);
  #elif NA_OS == NA_OS_MAC_OS_X
    return chdir(path);
  #endif
}


NA_IDEF NABool naExists(const char* path){
  #if NA_OS == NA_OS_WINDOWS
    return !(_access(path, 0));
  #elif NA_OS == NA_OS_MAC_OS_X
    return !(access(path, 0));
  #endif
}


NA_IDEF int naRemove(const char* path){
  #if NA_OS == NA_OS_WINDOWS
    return remove(path);
  #elif NA_OS == NA_OS_MAC_OS_X
    return remove(path);
  #endif
}


NA_IDEF NABool naCopyFile(const char* dstPath, const char* srcPath){
  #if NA_OS == NA_OS_WINDOWS
    return (CopyFile( (LPCTSTR)(const char*)srcPath,
                      (LPCTSTR)(const char*)dstPath,
                      NA_FALSE) != 0);
  #elif NA_OS == NA_OS_MAC_OS_X
    return (copyfile(srcPath, dstPath, NULL, COPYFILE_ALL) == 0);
  #endif
}



NA_IDEF NABool naAccess(const char* path, NABool doesExists, NABool canRead, NABool canWrite, NABool canExecute){
  #if NA_OS == NA_OS_WINDOWS
    int testMode = 0;
    // testMode |= (exists?00:0); // This line has been commented out because of static code analysis.
    testMode |= (canRead?04:0);
    testMode |= (canWrite?02:0);
    NA_UNUSED(canExecute); // Under windows, the executable flag does not exist.
    return (_access(path, testMode) == 0);
  #elif NA_OS == NA_OS_MAC_OS_X
    int testMode = 0;
    testMode |= (doesExists?F_OK:0);
    testMode |= (canRead?R_OK:0);
    testMode |= (canWrite?W_OK:0);
    testMode |= (canExecute?X_OK:0);
    return (access(path, testMode) == 0);
  #endif
}


NA_IDEF NAUTF8Char* naGetCwd(NAUTF8Char* buf, NAInt bufSize){
  #if NA_OS == NA_OS_WINDOWS
    return _getcwd(buf, (int)bufSize);
  #elif NA_OS == NA_OS_MAC_OS_X
    return getcwd(buf, (size_t)bufSize);
  #endif
}






// //////////////////////////////
// NAFile
// //////////////////////////////



struct NAFile{
  NARefCount refCount;
  int desc;   // The descriptor
};




NA_IDEF NAFile* naCreateFileReadingPath(const char* filePath){
  NAFile* file = naAlloc(NAFile);
  naInitRefCount(&(file->refCount));
  file->desc = naOpen(filePath, NA_FILE_OPEN_FLAGS_READ, NA_FILEMODE_DEFAULT);
  #ifndef NDEBUG
    if(file->desc < 0)
      naError("Could not open file.");
  #endif
  return file;
}



NA_IDEF NAFile* naCreateFileWritingPath(const char* filePath, NAFileMode mode){
  NAFile* file = naAlloc(NAFile);
  naInitRefCount(&(file->refCount));
  file->desc = naOpen(filePath, NA_FILE_OPEN_FLAGS_WRITE, mode);
  #ifndef NDEBUG
    if(file->desc < 0)
      naError("Could not create file.");
  #endif
  return file;
}



NA_IDEF NAFile* naCreateFileAppendingPath(const char* filePath, NAFileMode mode){
  NAFile* file = naAlloc(NAFile);
  naInitRefCount(&(file->refCount));
  file->desc = naOpen(filePath, NA_FILE_OPEN_FLAGS_APPEND, mode);
  #ifndef NDEBUG
    if(file->desc < 0)
      naError("Could not create file.");
  #endif
  return file;
}



NA_IDEF NAFile* naCreateFileReadingStdin(){
  NAFile* file = naAlloc(NAFile);
  naInitRefCount(&(file->refCount));
  file->desc = 0;
  return file;
}



NA_IDEF NAFile* naCreateFileWritingStdout(){
  NAFile* file = naAlloc(NAFile);
  naInitRefCount(&(file->refCount));
  file->desc = 1;
  return file;
}



NA_IDEF NAFile* naCreateFileWritingStderr(){
  NAFile* file = naAlloc(NAFile);
  naInitRefCount(&(file->refCount));
  file->desc = 2;
  return file;
}



NA_HAPI void na_DeallocFile(NAFile* file);



NA_IDEF void naReleaseFile(NAFile* file){
  naReleaseRefCount(&file->refCount, file, (NAMutator)na_DeallocFile);
}



NA_IDEF NAFileSize naComputeFileByteSize(const NAFile* file){
  NAFileSize curOffset;
  NAFileSize fileSize;
  curOffset = naLseek(file->desc, 0, SEEK_CUR);
  #ifndef NDEBUG
    if(curOffset == -1)
      naError("An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #endif
  fileSize = naLseek(file->desc, 0, SEEK_END);
  naLseek(file->desc, curOffset, SEEK_SET);
  return fileSize;  // todo: check if fileSize too big for NAInt
}



NA_IDEF NABool naIsFileOpen(const NAFile* file){
  return (file->desc >= 0);
}



NA_IDEF void naSeekFileAbsolute(NAFile* file, NAFileSize byteOffset){
  NAFileSize newOffset;
  #ifndef NDEBUG
    if(byteOffset < 0)
      naError("Negative offset in absolute jump.");
  #endif
  newOffset = naLseek(file->desc, byteOffset, SEEK_SET);
  #ifndef NDEBUG
    if(newOffset == -1)
      naError("An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #else
    NA_UNUSED(newOffset);
  #endif
}



NA_IDEF void naSeekFileRelative(NAFile* file, NAFileSize byteOffset){
  NAFileSize newOffset;
  newOffset = naLseek(file->desc, byteOffset, SEEK_CUR);
  #ifndef NDEBUG
    if(newOffset == -1)
      naError("An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #else
    NA_UNUSED(newOffset);
  #endif
}



NA_IDEF NAFileSize naReadFileBytes(NAFile* file, void* buf, NAFileSize byteSize){
  #ifndef NDEBUG
    if(!naIsFileOpen(file))
      naError("File is not open.");
    if(!byteSize)
      naError("Reading zero bytes.");
    if(byteSize < 0)
      naError("Negative count.");
  #endif
  if(!byteSize){return 0;}
  return naRead(file->desc, buf, byteSize);
}


NA_IDEF NAFileSize naWriteFileBytes(NAFile* file, const void* ptr, NAFileSize byteSize){
  #ifndef NDEBUG
    if(!naIsFileOpen(file))
      naError("File is not open.");
    if(!byteSize)
      naError("Writing zero bytes.");
    if(byteSize < 0)
      naError("Negative count.");
  #endif
  if(!byteSize){return 0;}
  return naWrite(file->desc, ptr, byteSize);
}




// ////////////////////////////
// General input output functions
// ////////////////////////////



NA_IDEF int naScanDecimal(){
  int retValue;
  #if NA_OS == NA_OS_WINDOWS
    scanf_s("%d", &retValue);
  #elif NA_OS == NA_OS_MAC_OS_X
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
