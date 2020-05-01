
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#if defined NA_CHAR_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_CHAR_INCLUDED
#define NA_CHAR_INCLUDED



// The newlines used on different systems as utf8 strings.
#if defined NA_C11
  #define NA_NL_UNIX  u8"\n"
  #define NA_NL_MAC9  u8"\r"
  #define NA_NL_WIN   u8"\r\n"
#else
  #define NA_NL_UNIX  "\n"
  #define NA_NL_MAC9  "\r"
  #define NA_NL_WIN   "\r\n"
#endif

// The native newline of the current system as an utf8 string.
#if NA_OS == NA_OS_WINDOWS
  #define NA_NL NA_NL_WIN
#else
  #define NA_NL NA_NL_UNIX
#endif

// A macro for the tab character as an utf8 string.
#if defined NA_C11
  #define NA_TAB      u8"\t"
#else
  #define NA_TAB      "\t"
#endif

// Macros for different path delimiters as utf8 chars.
#define NA_PATH_DELIMITER_UNIX '/'
#define NA_PATH_DELIMITER_WIN  '\\'
#define NA_SUFFIX_DELIMITER    '.'

// Type holding a single byte UTF8 character
//
// Note that in NALib, a String is internally always encoded in UTF-8.
// The encoding of strings given by system calls though may be of
// different kind. Especially on windows, one should use TCHAR which is
// either CHAR or WCHAR depending on the project settings and then use
// naAllocSystemStringWithUTF8String and naNewStringFromSystemString.
//
// Note that NAUTF8Char is defined as char to simplify debugging.
// Types like NAByte, int8 or uint8 can not be used as it these are
// defined to be incompatible with char. But at least we can be sure that
// a char consists of 8 Bits. See definition of NAByte for that.
typedef char NAUTF8Char;



#endif // NA_CHAR_INCLUDED

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
