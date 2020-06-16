
// This file contains inline implementations of the file NAString.h
// Do not include this file directly! It will automatically be included when
// including "NAString.h"




#include <stdio.h>




NA_IDEF NAInt naStrlen(const NAUTF8Char* str){
  return (NAInt)strlen((const char*)str);
}



NA_IDEF NAInt naVsnprintf(NAUTF8Char* buffer, size_t length, const NAUTF8Char* newstr, va_list argumentList){
  #if NA_OS == NA_OS_WINDOWS
    return (NAInt)_vsnprintf_s(buffer, (size_t)length, (size_t)length, newstr, argumentList);
  #elif NA_OS == NA_OS_MAC_OS_X
    return (NAInt)vsnprintf((char*)buffer, (size_t)length, (const char*)newstr, argumentList);
  #endif
}



// Returns the number of characters needed to transform the given string and
// arguments using sprintf.
NA_IDEF NAInt naVarargStringLength(const NAUTF8Char* string, va_list args){
#if NA_OS == NA_OS_WINDOWS
  return (NAInt)_vscprintf(string, args);
#elif NA_OS == NA_OS_MAC_OS_X
  return (NAInt)naVsnprintf(NA_NULL, 0, string, args);
#endif
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
