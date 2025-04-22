
#if defined NA_CHAR_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_CHAR_INCLUDED
#define NA_CHAR_INCLUDED



// The newlines used on different systems as utf8 strings.
#if defined NA_C11 || defined NA_CPP11
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
// naAllocSystemStringWithUTF8String and naNewStringWithSystemString.
//
// Note that NAUTF8Char is defined as char to simplify debugging.
// Types like NAByte, int8 or uint8 can not be used as it these are
// defined to be incompatible with char. But at least we can be sure that
// a char consists of 8 Bits. See definition of NAByte for that.
#if (defined NA_C23)
  typedef char8_t NAUTF8Char;
#else
  typedef char NAUTF8Char;
#endif



// The different newline-encodings as an enum type
typedef enum{
  NA_NEWLINE_UNIX,              // \n    Used in Unix, Linux, MacOSX
  NA_NEWLINE_MAC9,              // \r    Used in old Mac OS
  NA_NEWLINE_WIN,               // \r\n  Used in Windows
  NA_NEWLINE_NATIVE             //       Dependant on the local machines system
} NANewlineEncoding;



#endif // NA_CHAR_INCLUDED



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
