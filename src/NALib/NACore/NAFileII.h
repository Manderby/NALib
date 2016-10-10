
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAFile.h
// Do not include this file directly! It will automatically be included when
// including "NAFile.h"



// Use this function with origin = SEEK_SET, SEEK_CUR or SEEK_END.
// Note that all systems define NAFileSize to be a signed integer. Therefore
// searching backwards with SEEK_CUR is possible.
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


// Use the flags argument to define how to open the file.
//
// For the flags, use one of the macros NA_FILE_OPEN_FLAGS_READ,
// NA_FILE_OPEN_FLAGS_WRITE or NA_FILE_OPEN_FLAGS_APPEND if you just need the
// standard ways of accessing files.
//
// For writing files, use NA_FILEMODE_DEFAULT if you don't know what to use
// for the mode argument.
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
    return (NAFileSize)_write(fd, buf, (unsigned int)count);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAFileSize)write(fd, buf, (size_t)count);
  #endif
}


// Use NA_DIRMODE_DEFAULT if you don't know what to use for the mode
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



NA_IDEF NABool naAccess(const char* path, NABool exists, NABool read, NABool write, NABool execute){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    int testmode = 0;
    testmode |= (exists?00:0);
    testmode |= (read?04:0);
    testmode |= (write?02:0);
    NA_UNUSED(execute); // Under windows, the executable flag does not exist.
    return (_access(path, testmode) == 0);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    int testmode = 0;
    testmode |= (exists?F_OK:0);
    testmode |= (read?R_OK:0);
    testmode |= (write?W_OK:0);
    testmode |= (execute?X_OK:0);
    return (access(path, testmode) == 0);
  #endif
}


NA_IDEF NAUTF8Char* naGetCwd(NAUTF8Char* buf, NAInt bufsize){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _getcwd(buf, (int)bufsize);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return getcwd(buf, (size_t)bufsize);
  #endif
}






// //////////////////////////////
// NAFile
// //////////////////////////////


struct NAFile{
  int desc;   // The descriptor
};




NA_IDEF NAFile naMakeFileReadingFilename(const char* filename){
  NAFile file;
  file.desc = naOpen(filename, NA_FILE_OPEN_FLAGS_READ, NA_FILEMODE_DEFAULT);
  if(file.desc < 0){
    #ifndef NDEBUG
      naError("naInitFileReadingFilename", "Could not open file.");
    #endif
  }
  return file;
}



NA_IDEF NAFile naMakeFileWritingFilename(const char* filename, NAFileMode mode){
  NAFile file;
  file.desc = naOpen(filename, NA_FILE_OPEN_FLAGS_WRITE, mode);
  if(file.desc < 0){
    #ifndef NDEBUG
      naError("naInitFileWritingFilename", "Could not create file.");
    #endif
  }
  return file;
}



NA_IDEF NAFile naMakeFileAppendingFilename(const char* filename, NAFileMode mode){
  NAFile file;
  file.desc = naOpen(filename, NA_FILE_OPEN_FLAGS_APPEND, mode);
  if(file.desc < 0){
    #ifndef NDEBUG
      naError("naInitFileAppendingFilename", "Could not create file.");
    #endif
  }
  return file;
}



NA_IDEF NAFile naMakeFileReadingStdin(){
  NAFile file;
  file.desc = 0;
  return file;
}



NA_IDEF NAFile naMakeFileWritingStdout(){
  NAFile file;
  file.desc = 1;
  return file;
}



NA_IDEF NAFile naMakeFileWritingStderr(){
  NAFile file;
  file.desc = 2;
  return file;
}



NA_IDEF void naCloseFile(NAFile* file){
  // Note that stdin, stdout and stderr can be closed as well.
  naClose(file->desc);
  file->desc = -1;
}



NA_IDEF NAFileSize naComputeFileSize(const NAFile* file){
  // Declaration before implementation. Needed for C90.
  NAFileSize curpos;
  NAFileSize filesize;
  curpos = naLseek(file->desc, 0, SEEK_CUR);
  #ifndef NDEBUG
    if(curpos == -1)
      naError("naComputeFileSize", "An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #endif
  filesize = naLseek(file->desc, 0, SEEK_END);
  naLseek(file->desc, curpos, SEEK_SET);
  return filesize;
}



NA_IDEF NABool naIsFileOpen(const NAFile* file){
  return (file->desc >= 0);
}



NA_IDEF void naSeekFileAbsolute(NAFile* file, NAFileSize offset){
  NAFileSize result;
  #ifndef NDEBUG
    if(offset < 0)
      naError("naJumpFileOffsetAbsolute", "Negative offset in absolute jump.");
  #endif
  result = naLseek(file->desc, offset, SEEK_SET);
  #ifndef NDEBUG
    if(result == -1)
      naError("naSeekFileAbsolute", "An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #else
    NA_UNUSED(result);
  #endif
}



NA_IDEF void naSeekFileRelative(NAFile* file, NAFileSize offset){
  NAFileSize result;
  result = naLseek(file->desc, offset, SEEK_CUR);
  #ifndef NDEBUG
    if(result == -1)
      naError("naSeekFileRelative", "An error occured while seeking the file. Maybe file not open or a stream? Undefined behaviour.");
  #else
    NA_UNUSED(result);
  #endif
}



NA_IDEF NAFileSize naReadFileBytes(NAFile* file, void* buf, NAFileSize count){
  #ifndef NDEBUG
    if(!naIsFileOpen(file))
      naError("naReadFileBytes", "File is not open.");
    if(!count)
      naError("naReadFileBytes", "Reading zero bytes.");
    if(count < 0)
      naError("naReadFileBytes", "Negative count.");
  #endif
  if(!count){return 0;}
  return naRead(file->desc, buf, count);
}


NA_IDEF NAFileSize naWriteFileBytes(NAFile* file, const void* ptr, NAInt count){
  #ifndef NDEBUG
    if(!naIsFileOpen(file))
      naError("naReadFileBytes", "File is not open.");
    if(!count)
      naError("naReadFileBytes", "Writing zero bytes.");
    if(count < 0)
      naError("naReadFileBytes", "Negative count.");
  #endif
  if(!count){return 0;}
  return naWrite(file->desc, ptr, count);
}



NA_IDEF NAByteArray* naInitByteArrayFromFile(NAByteArray* bytearray, NAFile* file, NAFileSize count){
  bytearray = naInitByteArrayWithSize(bytearray, (NAInt)count);
  naReadFileBytes(file, naGetByteArrayMutablePointer(bytearray), count);
  return bytearray;
}


NA_IDEF NAString* naNewStringFromFile(NAFile* file, NAFileSize bytecount){
  NAUTF8Char* stringbuf;
  stringbuf = (NAUTF8Char*)naMalloc(-(NAInt)bytecount);
  naReadFileBytes(file, stringbuf, bytecount);
  return naNewStringWithMutableUTF8Buffer(stringbuf, -(NAInt)bytecount, NA_MEMORY_CLEANUP_FREE);
}



// ////////////////////////////
// General input output functions
// ////////////////////////////


NA_IDEF NAString* naNewStringWithFileContents(const char* filename){
  NAString* string;
  NAFile file;
  NAFileSize totalsize;
  file = naMakeFileReadingFilename(filename);
  totalsize = naComputeFileSize(&file);
  #ifndef NDEBUG
    if((NA_SYSTEM_ADDRESS_BITS <= 32) && (totalsize > NA_INT32_MAX))
      naError("naInitByteArrayWithFileContents", "Trying to read more than 2 GiB of data from file on a system not using 64 bits.");
  #endif
 string = naNewStringFromFile(&file, totalsize);
 naCloseFile(&file);
 return string;
}



NA_IDEF NAByteArray* naInitByteArrayWithFileContents(NAByteArray* bytearray, const char* filename){
  NAFile file;
  NAFileSize totalsize;
  file = naMakeFileReadingFilename(filename);
  if(!naIsFileOpen(&file)){return NA_NULL;}
  totalsize = naComputeFileSize(&file);
  #ifndef NDEBUG
    if((NA_SYSTEM_ADDRESS_BITS <= 32) && (totalsize > NA_INT32_MAX))
      naError("naInitByteArrayWithFileContents", "Trying to read more than 2 GiB of data from file on a system not using 64 bits.");
  #endif
  bytearray = naInitByteArrayFromFile(bytearray, &file, totalsize);
  naCloseFile(&file);
  return bytearray;
}



NA_IDEF NAInt naScanDecimal(){
  NAInt retvalue;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    scanf_s("%" NA_SCNi, &retvalue);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    scanf("%" NA_SCNi, &retvalue);
  #endif
  return retvalue;
}



NA_IDEF NAString* naNewStringWithCurrentWorkingDirectory(){
  NAString* string;
  NAUTF8Char* cwdbuf = naGetCwd(NA_NULL, 0);
  string = naNewStringWithMutableUTF8Buffer(cwdbuf, -(NAInt)(naStrlen(cwdbuf)), NA_MEMORY_CLEANUP_FREE);
  return string;
}



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
