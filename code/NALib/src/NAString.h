
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

// Returns the number of characters needed to transform the given string and
// arguments using sprintf.
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

// Allocates and returns memory and formats the given string like printf.
// When useTmp is true, the returned pointer will be automatically collected
// by the garbage collector of NARuntime.
NA_API NAUTF8Char* naAllocSprintf( NABool useTmp,
                         const NAUTF8Char* format,
                                           ...);

// Returns a garbage collected pointer to a formatted string of the value.
NA_API NAUTF8Char* naPriux8(uint8 value);
NA_API NAUTF8Char* naPriix8(int8 value);
NA_API NAUTF8Char* naPriux16(uint16 value);
NA_API NAUTF8Char* naPriix16(int16 value);
NA_API NAUTF8Char* naPriux32(uint32 value);
NA_API NAUTF8Char* naPriix32(int32 value);
NA_API NAUTF8Char* naPriux64(NAu64 value);
NA_API NAUTF8Char* naPriix64(NAi64 value);
NA_API NAUTF8Char* naPriux128(NAu128 value);
NA_API NAUTF8Char* naPriix128(NAi128 value);
NA_API NAUTF8Char* naPriux256(NAu256 value);
NA_API NAUTF8Char* naPriix256(NAi256 value);


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
//
// - if charoffset is negative, it denotes the number of chars from the end.
//   Note that the end has index [bytesize], meaning -1 denotes the index
//   [bytesize-1] which is the last char.
// - if length is negative, it denotes the size up and including to the given
//   number of chars from the end, meaning -1 denotes the last char.
// - if the charoffset and length combination somehow leads to a length of
//   exactly 0, the resulting string will be empty without a warning emitted.
// - If the charoffset and length combination somehow leads to an over- or
//   underflow, a warning will be emitted if NDEBUG is defined. The behaviour
//   when using such a string is undefined.
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
                                                 NAInt offset,
                                                 NAInt length);

NA_API NAString* naNewStringWithBufferExtraction( NABuffer* buffer,
                                                      NARangei range);

// Returns a string equal to the given string but all newlines are
// converted to the given newline encoding.
NA_API NAString* naNewStringWithNewlineSanitization( NAString* string,
                                             NANewlineEncoding encoding);


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
NA_API NAUTF8Char naGetStringChar(NAString* string, NAInt index);


// The following two functions allow you to get either the basename or the
// suffix of a filename. For example, the file "folder/document.txt" returns
// the basename "folder/document" and the suffix "txt".
// The suffix is detected by the first dot '.' from the right. If no such
// dot is found, suffix is empty.
NA_API NAString* naNewStringWithBasenameOfFilename(const NAString* filename);
NA_API NAString* naNewStringWithSuffixOfFilename  (const NAString* filename);



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


#if NA_OS == NA_OS_WINDOWS

  // Basic conversion between UTF8 and either UTF16 or Ansi
  // Returns a newly mallocated memory block containing the encoded
  // string. The resulting string must be freed manually. COPIES ALWAYS!
  NA_API wchar_t* naAllocWideCharStringWithUTF8String(const NAUTF8Char* utf8string);
  NA_API char* naAllocAnsiStringWithUTF8String(const NAUTF8Char* utf8string);

  // Creates a new NAString from an encoded string. COPIES ALWAYS!
  NA_API NAString* naNewStringFromWideCharString(wchar_t* wcharstring);
  NA_API NAString* naNewStringFromAnsiString(char* ansistring);

  // Conversions based on the application setting.
  // (See project properties, character set)
  NA_API TCHAR* naAllocSystemStringWithUTF8String(const NAUTF8Char* utf8string);
  NA_API NAString* naNewStringFromSystemString(TCHAR* systemstring);

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
NA_API NABool naEqualStringToUTF8CString(const NAString* string1,
                                              const NAUTF8Char* string2,
                                                         NABool casesensitive);


// These functions provide basic functionality for parsing numbers from a given
// string. If you need more advanced parsing, use one of the naGetStringBuffer
// functions and parse the buffer.
NA_API int8     naParseStringi8  (const NAString* string);
NA_API int16    naParseStringi16 (const NAString* string);
NA_API int32    naParseStringi32 (const NAString* string);
NA_API NAi64  naParseStringi64 (const NAString* string);
NA_API uint8    naParseStringu8 (const NAString* string);
NA_API uint16   naParseStringu16(const NAString* string);
NA_API uint32   naParseStringu32(const NAString* string);
NA_API NAu64 naParseStringu64(const NAString* string);
NA_API float    naParseStringFloat (const NAString* string);
NA_API double   naParseStringDouble(const NAString* string);


// Inline implementations are in a separate file:
#include "NAStruct/NAStringII.h"




#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_STRING_INCLUDED



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
