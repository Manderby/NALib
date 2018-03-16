
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "../NAFile.h"
#include "../NAString.h"




// This function had to be moved here as it introcudes cyclic redundancies
// when compiling.
NA_DEF NAString* naNewStringWithCurWorkingDirectory(){
  NAString* string;
  NAUTF8Char* cwdbuf = naGetCwd(NA_NULL, 0);
  string = naNewStringWithMutableUTF8Buffer(cwdbuf, -(NAInt)(naStrlen(cwdbuf)), (NAMutator)naFree);
  return string;
}




NA_DEF NABool naIsDir(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SystemChar* sysstring;
    NABool retvalue;
    sysstring = naAllocSystemStringWithUTF8String(path, 0);
    retvalue = (GetFileAttributes(sysstring)  & FILE_ATTRIBUTE_DIRECTORY) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retvalue;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    struct stat stat_struct;
    stat(path, &stat_struct);
    return (stat_struct.st_mode & S_IFDIR) ? NA_TRUE : NA_FALSE;
  #endif
}


NA_DEF NABool naIsHidden(const char* path){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SystemChar* sysstring;
    NABool retvalue;
    sysstring = naAllocSystemStringWithUTF8String(path, 0);
    retvalue = (GetFileAttributes(sysstring) & FILE_ATTRIBUTE_HIDDEN) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retvalue;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (path[0] == '.');
  #endif
}



NA_HDEF void naDeallocFile(NAFile* file){
  if(file->desc > 2){
    naClose(file->desc);
  }
  file->desc = -1;
  naFree(file);
}




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
