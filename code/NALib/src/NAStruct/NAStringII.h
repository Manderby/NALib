
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAString.h
// Do not include this file directly! It will automatically be included when
// including "NAString.h"




#include <stdio.h>




NA_IDEF NAInt naStrlen(const NAUTF8Char* str){
  return (NAInt)strlen((const char*)str);
}



NA_IDEF NAInt naVsnprintf(NAUTF8Char* buffer, NAUInt length, const NAUTF8Char* newstr, va_list argumentlist){
  #if NA_OS == NA_OS_WINDOWS
    return (NAInt)_vsnprintf_s(buffer, (size_t)length, (size_t)length, newstr, argumentlist);
  #elif NA_OS == NA_OS_MAC_OS_X
    return (NAInt)vsnprintf((char*)buffer, (size_t)length, (const char*)newstr, argumentlist);
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
