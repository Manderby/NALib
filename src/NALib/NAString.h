
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_STRING_INCLUDED
#define NA_STRING_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif



// The typedef needs to be here to resolve cyclic include problems.
typedef struct NAString NAString;

// The different newline-encodings as an enum type
typedef enum{
  NA_NEWLINE_UNIX,              // \n    Used in Unix, Linux, MacOSX
  NA_NEWLINE_MAC9,              // \r    Used in old Mac OS
  NA_NEWLINE_WIN,               // \r\n  Used in Windows
  NA_NEWLINE_NATIVE             //       Dependant on the local machines system
} NANewlineEncoding;



#include "NAMemory.h"
#include "NACoord.h"
#include "NABuffer.h"







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
NA_API NAString* naNewString(void);

// Returns an NAString representing a given C-String written directly in the
// programming code and encoded in UTF8. This function is useful for the
// programmers which like to write strings in the code like "Hello World".
// This function takes such strings and wraps it into a string struct. The
// string is marked explicitely as read-only. Note that this works only
// reliable if the source-code itself is encoded in UTF-8. DOES NOT COPY!
//
// Beware! Only use it for literals! Trying to use it with other char* may
// lead to serious trouble! Use naNewStringWithFormat for that. Also be
// advised to not convert from NAString to char* and back again!
NA_API NAString* naNewStringWithUTF8CStringLiteral(const NAUTF8Char* ptr);

// Returns an NAString with the given length using the given buffer. The buffer
// must be big enough! When destructor is anything but NA_NULL, the given
// buffer will be cleaned up when eventually it is no longer in use.
// When length is negative, the absolute value will be used but the buffer is
// expected to be null-terminated (the null character is not in length).
NA_API NAString* naNewStringWithMutableUTF8Buffer(  NAUTF8Char* buffer,
                                                          NAInt length,
                                                      NAMutator destructor);

// Creates an NAString just like sprintf.
NA_API NAString* naNewStringWithFormat(const NAUTF8Char* format,
                                                          ...);
// Does the same thing but with an existing va_list argument. The argumentlist
// argument will not be altered by this function.
NA_API NAString* naNewStringWithArguments(const NAUTF8Char* format,
                                                     va_list argumentlist);

// Fills deststring with a desired part of srcstring.
// Note that the returned string will automatically be cached.
//
// - if charoffset is negative, it denotes the number of chars from the end.
//   Note that the end has index [bytesize], meaning -1 denotes the index
//   [bytesize-1] which is the last char.
// - If the length is 0, the resulting string is empty.
// - if length is negative, it denotes the size up and including to the given
//   number of chars from the end, meaning -1 denotes the last char.
// - if the charoffset and length combination somehow leads to a length of
//   exactly 0, the resulting string will be empty without a warning emitted.
// - If the charoffset and length combination somehow leads to an over- or
//   underflow, a warning will be emitted if NDEBUG is defined. The resulting
//   string will be empty.
//
// Example: String ABCDEF with the pair (charoffset, length):
// - ( 2,  2)   ->   CD   (extraction)
// - ( 2, -1)   ->   CDEF (truncate start from left)
// - (-2, -1)   ->     EF (truncate start from right)
// - ( 0,  2)   -> AB     (truncate end from left)
// - ( 0, -3)   -> ABCD   (truncate end from right)
// - (-3, -2)   ->    DE  (mix it as you desire)
// - ( 0, -1)   -> ABCDEF (exact duplicate)
// - ( 1,  0)   ->        (empty string because of desired bytesize == 0)
// - ( 2, -5)   ->        (empty string because of resulting bytesize == 0)
// - (-9,  9)   ->        (empty string with warning when debugging)
NA_API NAString* naNewStringExtraction(const NAString* srcstring,
                                                  NAInt charoffset,
                                                  NAInt length);

NA_API NAString* naNewStringWithBufferExtraction( NABuffer* buffer,
                                                    NARangei range);



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

// Returns the char at the given index.
NA_API NAUTF8Char naGetStringChar(NAString* string, NAInt indx);


// The following two functions allow you to get either the basename or the
// suffix of a filename. For example, the file "folder/document.txt" returns
// the basename "folder/document" and the suffix "txt".
// The suffix is detected by the first dot '.' from the right. If no such
// dot is found, suffix is empty.
NA_API NAString* naNewStringWithBasenameOfFilename(const NAString* filename);
NA_API NAString* naNewStringWithSuffixOfFilename(const NAString* filename);



// Creates a new string by encoding or decoding the characters of inputstring.
//         | Encoding example  | Decoding example |  Notes
// --------+-------------------+------------------+----------------------
// CEscape | " becomes \"      | \" becomes "     |
// XML     | " becomes &quot;  | &quot; becomes " |  Do not use for HTML < 5
// EPS     | ( becomes \(      | \(     becomes ( |
//
// Note: Always creates a new string by copying the characters from the input.
// Therefore, the two parameters MUST not be the same!
// Warning: XML-Decoding does not support numeric entities yet.
//
NA_API NAString* naNewStringCEscaped  (const NAString* inputstring);
NA_API NAString* naNewStringCUnescaped(const NAString* inputstring);
NA_API NAString* naNewStringXMLEncoded(const NAString* inputstring);
NA_API NAString* naNewStringXMLDecoded(const NAString* inputstring);
NA_API NAString* naNewStringEPSEncoded(const NAString* inputstring);
NA_API NAString* naNewStringEPSDecoded(const NAString* inputstring);

// The following functions are system dependent.
// Curly, this is only necessary on windows.
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  // Returns a newly allocated memory block containing the system-encoded
  // string. If you do not provide the length, it will be automatically
  // computed. The resulting string must be freed manually. COPIES ALWAYS!
  NA_API SystemChar* naAllocSystemStringWithUTF8String(const NAUTF8Char* utf8str,
                                                               NAUInt length);
  //// Creates a new NAString from a system-encoded string. COPIES ALWAYS!
  //NA_API NAString* naNewStringFromSystemString(SystemChar* systemstring);
#endif


// Appending functions: Appends something at the end of originalstring.
// The storage of originalstring will be detached and deleted if necessary.
// Before that, all content will be COPIED. The resulting string will be
// NULL-terminated.

// Appends another NAString
NA_API void naAppendStringString(       NAString* originalstring,
                                  const NAString* string2);
// Appends an UTF-8 character
NA_API void naAppendStringChar(
                                        NAString* originalstring,
                                       NAUTF8Char newchar);
// Appends an UTF-8 C-String formatted just like sprintf. You can use this
// function to append C-Strings without arguments as well.
NA_API void naAppendStringFormat(
                                         NAString* originalstring,
                                 const NAUTF8Char* format,
                                                   ...);
// Does the same thing but with an existing va_list argument. The argumentlist
// argument will not be altered by this function.
NA_API void naAppendStringArguments(
                                         NAString* originalstring,
                                 const NAUTF8Char* format,
                                           va_list argumentlist);




// Compares two strings and returns NA_TRUE if they are equal.
NA_API NABool naEqualStringToString(            const NAString* string1,
                                                const NAString* string2,
                                                         NABool casesensitive);
// Compares a string with a C string literal and returns NA_TRUE if they are
// equal.
NA_API NABool naEqualStringToUTF8CStringLiteral(const NAString* string1,
                                              const NAUTF8Char* string2,
                                                         NABool casesensitive);


// These functions provide basic functionality for parsing numbers from a given
// string. If you need more advanced parsing, use one of the naGetStringBuffer
// functions and parse the buffer.
NA_API int8   naParseStringInt8  (const NAString* string);
NA_API int16  naParseStringInt16 (const NAString* string);
NA_API int32  naParseStringInt32 (const NAString* string);
NA_API int64  naParseStringInt64 (const NAString* string);
NA_API uint8  naParseStringUInt8 (const NAString* string);
NA_API uint16 naParseStringUInt16(const NAString* string);
NA_API uint32 naParseStringUInt32(const NAString* string);
NA_API uint64 naParseStringUInt64(const NAString* string);
NA_API float  naParseStringFloat (const NAString* string);
NA_API double naParseStringDouble(const NAString* string);


// Inline implementations are in a separate file:
#include "NAStruct/NAStringII.h"




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_STRING_INCLUDED


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
