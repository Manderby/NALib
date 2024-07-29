
#include "../NAFile.h"
#include "../NAString.h"




NA_RUNTIME_TYPE(NAFile, na_DestroyFile, NA_TRUE);



// This function had to be moved here as it introcudes cyclic redundancies
// when compiling.
NA_DEF NAString* naNewStringWithCurWorkingDirectory() {
  NAString* string;
  NAUTF8Char* cwdbuf = naGetCwd(NA_NULL, 0);
  string = naNewStringWithMutableUTF8Buffer(cwdbuf, naStrlen(cwdbuf), (NAMutator)naFree);
  return string;
}




NA_DEF NABool naIsDir(const char* path) {
  #if NA_OS == NA_OS_WINDOWS
    TCHAR* sysstring;
    NABool retValue;
    sysstring = naAllocSystemStringWithUTF8String(path);
    retValue = (GetFileAttributes(sysstring)  & FILE_ATTRIBUTE_DIRECTORY) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retValue;
  #elif NA_IS_POSIX
    struct stat stat_struct;
    stat(path, &stat_struct);
    return (stat_struct.st_mode & S_IFDIR) ? NA_TRUE : NA_FALSE;
  #endif
}


NA_DEF NABool naIsHidden(const char* path) {
  #if NA_OS == NA_OS_WINDOWS
    TCHAR* sysstring;
    NABool retValue;
    sysstring = naAllocSystemStringWithUTF8String(path);
    retValue = (GetFileAttributes(sysstring) & FILE_ATTRIBUTE_HIDDEN) ? NA_TRUE : NA_FALSE;
    free(sysstring);
    return retValue;
  #elif NA_IS_POSIX
    return (path[0] == '.');
  #endif
}



NA_HDEF void na_DestroyFile(NAFile* file) {
  if(file->desc > 2) {
    naClose(file->desc);
  }
  file->desc = -1;
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
