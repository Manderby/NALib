
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_STRING_INCLUDED
#define NA_STRING_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAByteArray.h"
#include "NAMemory.h"

// Note that in NALib, a String is internally always encoded in UTF-8.
// Further more, the contents of a string can not be altered after its
// creation. But you can change the position and length of a string to
// reflect a part of the original string.

// Note that NAUTF8Char is defined as char to simplify debugging.
// Types like NAByte, int8 or uint8 can not be used as it these are
// defined to be incompatible with char. But at least we can be sure that
// a char consists of 8 Bits. See definition of NAByte for that.
typedef char NAUTF8Char;


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



NA_API NAUInt naStrlen(const NAUTF8Char* str);





// Opaque type. You won't find an inline declaration further below. The
// NAString type is just too fragile. You can only use it as a pointer.
typedef struct NAString NAString;


// Terminology:
//
// Note that in this implementation, a string has a leading and a trailing end.
// The leading end contains the first character and the trailing end the last.
// Words like "left" or "right" are misleading because there exist languages
// where the first character is not left but right (for example Arabic) or the
// characters are written from top to bottom (for example Chinese).



// ///////////////////////////
// Constructors and Destructors. Read the readme.txt for explanation.
// ///////////////////////////

// Returns a new, EMPTY NAString.
NA_API NAString* naNewString(void);

// Returns an NAString representing a given C-String written directly in the
// programming code and encoded in UTF8. This function is useful for the
// programmers which like to write strings in the code like "Hello World".
// This function takes such strings and wraps it into a string struct. The
// string is marked explicitely as read-only. Note that this works only
// reliable if the source-code itself is encoded in UTF-8. DOES NOT COPY!
NA_API NAString* naNewStringWithUTF8CStringLiteral(const NAUTF8Char* ptr);

// Returns an NAString with the given length using the given buffer. The buffer
// must be big enough! When ownership is set to true, this NAString will free
// the buffer when getting deleted.
// When length is negative, the absolute value will be used but the buffer is
// expected to be null-terminated (the null character is not in length).
NA_API NAString* naNewStringWithMutableUTF8Buffer(  NAUTF8Char* buffer,
                                                          NAInt length,
                                               NAMemoryCleanup ownership);

// Creates an NAString just like sprintf.
NA_API NAString* naNewStringWithFormat(const NAUTF8Char* format,
                                                     ...);
// Does the same thing but with an existing va_list argument. The argumentlist
// argument will not be altered by this function.
NA_API NAString* naNewStringWithArguments(const NAUTF8Char* format,
                                               va_list argumentlist);

// Fills deststring with a desired part of srcstring. Does not copy!
// See naInitByteArrayExtraction for an explanation of all arguments.
// Use charoffset = 0 and length = -1 to reference the whole string.
NA_API NAString* naNewStringExtraction(const NAString* srcstring,
                                               NAInt charoffset,
                                               NAInt length);

// The following two functions allow you to get either the basename or the
// suffix of a filename. For example, the file "folder/document.txt" returns
// the basename "folder/document" and the suffix "txt".
// The suffix is detected by the first dot '.' from the right. If no such
// dot is found, suffix is empty.
NA_API NAString* naNewStringWithBasenameOfFilename(const NAString* filename);
NA_API NAString* naNewStringWithSuffixOfFilename(const NAString* filename);

// Creates a new string by encoding or decoding the characters of inputstring.
//      |  Encoding example    |   Decoding example |  Notes
// -----+----------------------+--------------------+----------------------
// XML  |  " becomes &quot;    |   &quot; becomes " |  Do not use for HTML < 5
// EPS  |  ( becomes \(        |   \(     becomes ( |
//
// Note: Always creates a new string by copying the characters from the input.
// Therefore, the two parameters MUST not be the same!
// Warning: XML-Decoding does not support numeric entities yet.
NA_API NAString* naNewStringXMLEncoded(const NAString* inputstring);
NA_API NAString* naNewStringXMLDecoded(const NAString* inputstring);
NA_API NAString* naNewStringEPSEncoded(const NAString* inputstring);
NA_API NAString* naNewStringEPSDecoded(const NAString* inputstring);

// The following functions are system dependent.
// Currently, this is only necessary on windows.
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  // Returns a newly allocated memory block containing the system-encoded
  // string. If you do not provide the length, it will be automatically
  // computed. The resulting string must be freed manually. COPIES ALWAYS!
  NA_API SystemChar* naAllocSystemStringFromString(const NAUTF8Char* utf8str,
                                                               NAUInt length);
  // Creates a new NAString from a system-encoded string. COPIES ALWAYS!
  NA_API NAString* naNewStringFromSystemString(SystemChar* systemstring);
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



// COPIES the contents of the string to a separate storage and decouples it
// from the existing storage. After this function, the decoupled string is
// ensured to be null-terminated. Copies always!
NA_API void naDecoupleString(NAString* string);



// Returns the length of the string in Characters. If the string is Null-terminated,
// the terminating Null-character is NOT included. Therefore "Hello" always
// returns 5, no matter if the string is null-terminated or not.
NA_API NAUInt naGetStringLength(const NAString* string);  // todo: lengt or bytesize?

// Returns a const pointer to the first character of this string. Use
// this for example for printf %s arguments but make sure the string is
// null-terminated. Use naDecoupleString if necessary. No debug warning will
// be emitted if the string is not null-terminated!
// When the string is empty, a pointer to a const char '\0' is returned.
NA_API const NAUTF8Char* naGetStringUTF8Pointer  (const NAString* string);

// Returns a pointer to the desired character. If index is negative, it
// is searched from the end of the array. For example -1 denotes the last byte.
// Warning: result is garbage if the array is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetStringUTF8Pointer or
// this function and use pointer arithmetic afterwards.
NA_API const NAUTF8Char* naGetStringChar  (const NAString* string, NAInt indx);

// Returns NA_TRUE if the string is empty.
NA_API NABool naIsStringEmpty(const NAString* string);


// ////////////////////////////
// Parsing methods
// All the following methods go through a const char string or an NAString,
// searching for desired values. Beware: Some functions change NAStrings!
// ////////////////////////////

// Returns the index of the first occurence of the given character in string.
// The startpos defines where to start and in what direction to search for.
// If startoffset is positive, the character is searched from leading to trailing.
// If startoffset is negative, the character is searched from trailing to leading.
// A negative startoffset denotes the character from the end of the string. For
// example, -1 denotes the character at [stringsize - 1].
// When the character is not found, NA_INVALID_MEMORY_INDEX is returned.
NA_API NAInt naGetStringCharacterPos(  const NAString* string,
                                            NAUTF8Char ch,
                                                 NAInt startoffset);

// naGetStringCharacterEscapeLength TowardsTrailing and TowardsLeading:
// These two functions test if the character at charoffset is the beginning or the
// end of an escape sequence. A length in bytes is returned which denotes the
// length and orientation of the escape sequence.
//
// This method is the core implementation of NALib's string parsing and can be
// configured using the escaping flags above.
//
// If the returned length is positive, the escape sequence spans to the trailing
// end. If the returned length is negative, the escape sequence spans to the
// leading end. The absolute value of the returned number denotes the number of
// bytes - 1 needed for the escape sequence. Examples:
// When checking the  \  in  \"   1 might be returned.
// When checking the  "  in  \"  -1 might be returned.
// When checking an escaped windows line ending (\-CR-LF), 2 might be returned.
// When checking the last double quote of "Hello World", -12 might be returned.
// If the character is not part of an escape sequence, 0 is returned.
//
// The Trailing-function will search towards the trailing end and will always
// return 0 or a positive number.
// The Leading-function will search towards the leading end and will always
// return 0 or a negative number.
// This distinction is important for example when checking the middle character
// of the string "\\n": Is the backslash an escaped backslash or does it escape
// the successing n?
//
// The charoffset argument can be positive or negative. A negative charoffset searches
// the position length - charoffset. For example -1 denotes the last character.
//
NA_API NAInt naGetStringCharacterEscapeLengthTowardsTrailing(NAString* string,
                                                               NAInt charoffset);
NA_API NAInt naGetStringCharacterEscapeLengthTowardsLeading( NAString* string,
                                                               NAInt charoffset);


// naSkipStringWhitespaces searches from leading to trailing for the first char
// which is > 32 (space). The string might be empty after this function.
NA_API void naSkipStringWhitespaces(NAString* string);

// Returns the next line delimited by CR, LF or CR-LF. The returned line will
// not contain any line-ending characters but all other whitespaces (ord <= 32)
// are still there. When skipempty is NA_TRUE, lines containing nothing but
// whitespaces will be skipped.
// Returns the number of lines read. Will usually be 1 but will be greater than
// 1 when skipping empty lines. Will be 0 when the string argument was empty.
// Warning: the two arguments shall not be the same as the result is
// undefined.
NA_API NAInt naParseStringLine(NAString* string, NAString* line, NABool skipempty);

// Gathers the first token of string which is delimited by whitespaces. The
// string is expected to start at a non-whitespace! If it starts with a
// whitespace, an empty string is returned. In any case, the returned token
// will not have any leading or trailing whitespaces.
// After this function, the string argument will point to the next character
// after the token not being a whitespace. Both the string and the token might
// be empty after this function.
NA_API NAString* naParseStringToken(NAString* string);

// Gathers the first token within string which ends in the given delimiter.
// The delimiter will not be included. After this function, string will point
// to the first character after the delimiter. Leading or trailing whitespaces
// will NOT be stripped at all. Both strings might also be empty after this
// function.
NA_API NAString* naParseStringTokenWithDelimiter(  NAString* string,
                                                  NAUTF8Char delimiter);

// Gathers the first token within string which ends in a path delimiter. Both
// path delimiters / and \ are detected. The delimiter will not be included.
// After this function, string will point to the first character after the
// delimiter. Leading or trailing whitespaces will NOT be stripped at all.
// Both strings might also be empty after this function.
NA_API NAString* naParseStringPathComponent(NAString* string);

// Parses the given string for decimal digits and accumulates them into an
// unsigned integer. The function will start at the first character and parse
// as long as there is a decimal digit. The number of bytes considered is
// returned. The resulting integer value is returned in retint. If retint is
// a Null-Pointer, the function just returns the number of bytes considered.
//
// The function will not parse more than maxdigitcount characters. This value
// should not be null.
//
// If the parsed value exceeds max, retint will be max and a warning will be
// emitted when debugging. But note that the returned number of bytes contains
// all digits considered.
NA_API NAUInt naParseUTF8StringForDecimalUnsignedInteger(const NAUTF8Char* string,
                                                                   uint64* retint,
                                                                    NAUInt maxdigitcount,
                                                                    uint64 max);
// Same as above but parses a signed integer. Note that there is an addidional
// min parameter.
NA_API NAUInt naParseUTF8StringForDecimalSignedInteger( const NAUTF8Char* string,
                                                                   int64* retint,
                                                                   NAUInt maxdigitcount,
                                                                    int64 min,
                                                                    int64 max);

// Parses the given string for an integer value and returns the value in the
// desired type. Note: These are parsing functions which alter string such that
// the string points to the limiting character after this function. If you only
// want the value but not alter the string, look for the naGetStringIntXXX
// functions below.
//
// Whe skipdelimiter is true, the limiting character is skipped and the string
// points to the character right after the delimiter.
//
// The unsigned variants only detect digits and assume a positive value. The
// signed variants detect when the first character is '+' or '-'. Multiple
// signes are not detected.
//
// If any of the strings exceeds the type range, the maximal / minimal value
// of that type is returned. When debugging, a warning is emitted, when the
// range of an uint64 is insufficient to hold the parsed value.
NA_API int8   naParseStringInt8  (NAString* string, NABool skipdelimiter);
NA_API int16  naParseStringInt16 (NAString* string, NABool skipdelimiter);
NA_API int32  naParseStringInt32 (NAString* string, NABool skipdelimiter);
NA_API int64  naParseStringInt64 (NAString* string, NABool skipdelimiter);
NA_API uint8  naParseStringUInt8 (NAString* string, NABool skipdelimiter);
NA_API uint16 naParseStringUInt16(NAString* string, NABool skipdelimiter);
NA_API uint32 naParseStringUInt32(NAString* string, NABool skipdelimiter);
NA_API uint64 naParseStringUInt64(NAString* string, NABool skipdelimiter);

// Returns the appropriate representation while leaving the string untouched.
NA_API int8   naGetStringInt8  (const NAString* string);
NA_API int16  naGetStringInt16 (const NAString* string);
NA_API int32  naGetStringInt32 (const NAString* string);
NA_API int64  naGetStringInt64 (const NAString* string);
NA_API uint8  naGetStringUInt8 (const NAString* string);
NA_API uint16 naGetStringUInt16(const NAString* string);
NA_API uint32 naGetStringUInt32(const NAString* string);
NA_API uint64 naGetStringUInt64(const NAString* string);

// Tests if the content of the given pointer is equal to the contents of string.
NA_API NABool naEqualStringToUTF8CStringLiteral(const NAString* string,
                                              const NAUTF8Char* ptr);
// Tests the equality of two NAStrings
NA_API NABool naEqualStringToString(     const NAString* string1,
                                         const NAString* string2);
// Compares two pointers case insensitive. Note that this function is not
// fully developed and will likely change in the future. No collation setting
// available yet. Currently, only ASCII characters in the range [32-126] are
// considered.
NA_API NABool naEqualUTF8CStringLiteralsCaseInsensitive( const NAUTF8Char* string1,
                                                         const NAUTF8Char* string2);






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
