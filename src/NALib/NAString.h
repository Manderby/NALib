
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_STRING_INCLUDED
#define NA_STRING_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif



// The typedef needs to be here to resolve cyclic include problems.
typedef struct NAString NAString;



// Note that in NALib, a String is internally always encoded in UTF-8.

// Note that NAUTF8Char is defined as char to simplify debugging.
// Types like NAByte, int8 or uint8 can not be used as it these are
// defined to be incompatible with char. But at least we can be sure that
// a char consists of 8 Bits. See definition of NAByte for that.
typedef char NAUTF8Char;


#include "NAMemory.h"
#include "NACoord.h"
#include "NABuffer.h"


// The tabs and newlines used on different systems. The native newline argument
// NA_NL will be defined below.
#define NA_NL_UNIX  "\n"
#define NA_NL_MAC9  "\r"
#define NA_NL_WIN   "\r\n"
#define NA_TAB      "\t"
#define NA_PATH_DELIMITER_UNIX '/'
#define NA_PATH_DELIMITER_WIN  '\\'
#define NA_SUFFIX_DELIMITER    '.'


// The different newline-encodings as an enum type
typedef enum{
  NA_NEWLINE_UNIX,              // \n    Used in Unix, Linux, MacOSX
  NA_NEWLINE_MAC9,              // \r    Used in old Mac OS
  NA_NEWLINE_WIN,               // \r\n  Used in Windows
  NA_NEWLINE_NATIVE             //       Dependant on the local machines system
} NANewlineEncoding;



// System dependant mapping of string functions and macros
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
  // The SystemChar is a character type which denotes the one used in the
  // project preferences. It is CHAR for "Multi Byte Character Set" and
  // WCHAR for "Unicode character set".
  #ifdef UNICODE
    typedef WCHAR SystemChar;
  #else
    typedef CHAR SystemChar;
  #endif
  #define NA_NL NL_WIN
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  // typedef short SystemChar;  // unused at the moment
  #define NA_NL NL_UNIX
#else
#endif



// Remapping of some starndard functions.
NA_IAPI NAInt naStrlen(               const NAUTF8Char* str);
NA_IAPI NAInt naVsnprintf(                  NAUTF8Char* buffer,
                                                 NAUInt length,
                                      const NAUTF8Char* newstr,
                                                va_list argumentlist);
NA_IAPI NAInt naVarargStringLength(   const NAUTF8Char* string,
                                                va_list args);

// Compares the string content of the two pointers being equal.
// if length is 0, the two strings are expected to be zero-terminated and
// the length of the two strings is computed automatically.
// If casesensitive is NA_TRUE, an exact match is tested.
NA_API NABool naEqualUTF8CStringLiterals( const NAUTF8Char* string1,
                                          const NAUTF8Char* string2,
                                                      NAInt length,
                                                     NABool casesensitive);


// /////////////////////////////////////////
// NAString
// /////////////////////////////////////////

// Makes a new, EMPTY NAString.
NA_API NAString naMakeString(void);

// Returns an NAString representing a given C-String written directly in the
// programming code and encoded in UTF8. This function is useful for the
// programmers which like to write strings in the code like "Hello World".
// This function takes such strings and wraps it into a string struct. The
// string is marked explicitely as read-only. Note that this works only
// reliable if the source-code itself is encoded in UTF-8. DOES NOT COPY!
NA_API NAString naMakeStringWithUTF8CStringLiteral(const NAUTF8Char* ptr);

// Returns an NAString with the given length using the given buffer. The buffer
// must be big enough! When cleanup is anything but NA_MEMORY_CLEANUP_NONE,
// this NAString will clean up the buffer when getting deleted.
// When length is negative, the absolute value will be used but the buffer is
// expected to be null-terminated (the null character is not in length).
NA_API NAString naMakeStringWithMutableUTF8Buffer(  NAUTF8Char* buffer,
                                                           NAInt length,
                                                 NAMemoryCleanup cleanup);

// Creates an NAString just like sprintf.
NA_API NAString naMakeStringWithFormat(const NAUTF8Char* format,
                                                          ...);
// Does the same thing but with an existing va_list argument. The argumentlist
// argument will not be altered by this function.
NA_API NAString naMakeStringWithArguments(const NAUTF8Char* format,
                                                     va_list argumentlist);

// Fills deststring with a desired part of srcstring. Does not copy!
// See naInitByteArrayExtraction for an explanation of all arguments.
// Use charoffset = 0 and length = -1 to reference the whole string.
NA_API NAString naMakeStringExtraction(const NAString* srcstring,
                                                  NAInt charoffset,
                                                  NAInt length);

NA_API NAString naMakeStringWithBufferExtraction( NABuffer* buffer,
                                                    NARangei range);

// Clears the given string.
NA_API void naClearString(NAString* string);




// Returns the length of the string in bytes.
NA_API NAInt naGetStringBytesize(const NAString* string);

// Returns a const pointer to a C-style representation of the string.
// Note that this is a costly function as the representation is always created
// anew.
NA_API const NAUTF8Char* naGetStringUTF8Pointer(const NAString* string);

// Returns NA_TRUE if the string is empty.
NA_API NABool naIsStringEmpty(const NAString* string);

// Returns the buffer storing the contents of this string.
NA_API const NABuffer* naGetStringBufferConst(const NAString* string);
NA_API NABuffer* naGetStringBufferMutable(NAString* string);




// The following two functions allow you to get either the basename or the
// suffix of a filename. For example, the file "folder/document.txt" returns
// the basename "folder/document" and the suffix "txt".
// The suffix is detected by the first dot '.' from the right. If no such
// dot is found, suffix is empty.
NA_API NAString naMakeStringWithBasenameOfFilename(const NAString* filename);
NA_API NAString naMakeStringWithSuffixOfFilename(const NAString* filename);



// Creates a new string by encoding or decoding the characters of inputstring.
//      |  Encoding example    |   Decoding example |  Notes
// -----+----------------------+--------------------+----------------------
// XML  |  " becomes &quot;    |   &quot; becomes " |  Do not use for HTML < 5
// EPS  |  ( becomes \(        |   \(     becomes ( |
//
// Note: Always creates a new string by copying the characters from the input.
// Therefore, the two parameters MUST not be the same!
// Warning: XML-Decoding does not support numeric entities yet.
//
// Currently commented out as not fully functional.
//NA_API NAString* naMakeStringXMLEncoded(const NAString* inputstring);
//NA_API NAString* naMakeStringXMLDecoded(const NAString* inputstring);
//NA_API NAString* naMakeStringEPSEncoded(const NAString* inputstring);
//NA_API NAString* naMakeStringEPSDecoded(const NAString* inputstring);

// The following functions are system dependent.
// Currently, this is only necessary on windows.
//
// Currently commented out as not fully functional.
//#if NA_SYSTEM == NA_SYSTEM_WINDOWS
//  // Returns a newly allocated memory block containing the system-encoded
//  // string. If you do not provide the length, it will be automatically
//  // computed. The resulting string must be freed manually. COPIES ALWAYS!
//  NA_API SystemChar* naAllocSystemStringFromString(const NAUTF8Char* utf8str,
//                                                               NAUInt length);
//  // Creates a new NAString from a system-encoded string. COPIES ALWAYS!
//  NA_API NAString* naMakeStringFromSystemString(SystemChar* systemstring);
//#endif


// Appending functions: Appends something at the end of originalstring.
// The storage of originalstring will be detached and deleted if necessary.
// Before that, all content will be COPIED. The resulting string will be
// NULL-terminated.
//
// Currently commented out as not fully functional
//
//// Appends another NAString
//NA_API void naAppendStringString(       NAString* originalstring,
//                                  const NAString* string2);
//// Appends an UTF-8 character
//NA_API void naAppendStringChar(
//                                        NAString* originalstring,
//                                       NAUTF8Char newchar);
//// Appends an UTF-8 C-String formatted just like sprintf. You can use this
//// function to append C-Strings without arguments as well.
//NA_API void naAppendStringFormat(
//                                         NAString* originalstring,
//                                 const NAUTF8Char* format,
//                                                   ...);
//// Does the same thing but with an existing va_list argument. The argumentlist
//// argument will not be altered by this function.
//NA_API void naAppendStringArguments(
//                                         NAString* originalstring,
//                                 const NAUTF8Char* format,
//                                           va_list argumentlist);








// Inline implementations are in a separate file:
#include "NAStruct/NAStringII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_STRING_INCLUDED


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
