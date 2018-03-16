
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_URL_INCLUDED
#define NA_URL_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAList.h"


#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #define NA_PATH_DELIMITER_SYSTEM NA_PATH_DELIMITER_WIN
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #define NA_PATH_DELIMITER_SYSTEM NA_PATH_DELIMITER_UNIX
#endif


#define NA_URL_PATH_ABSOLUTE 0x01

typedef struct NAURL NAURL;
struct NAURL{
  uint32 status;
  NAList path;
};


// Creates an empty URL
NA_IAPI NAURL* naInitURL(NAURL* url);

// - Both delimiters / and \ will be detected.
// - If the path starts with a path delimiter, it is considered absolute.
//   Example: /usr/local is an absolute path, src/NALib is not.
// - Erroneous duplicate path delimiters or ending delimiters will be ignored.
//   Example: /usr//local/ results in /usr/local
NA_IAPI NAURL* naInitURLWithUTF8CStringLiteral( NAURL* url,
                                     const NAUTF8Char* string);
NA_IAPI void naClearURL(NAURL* url);

// Creates a new string containing just the last path component.
// Note that there is no distinction if the last component is the name of a
// folder or of a file. If the file has a suffix, it is contained in the
// returned string.
NA_IAPI NAString* naNewStringWithURLFilename(NAURL* url);





// Inline implementations are in a separate file:
#include "NACore/NAURLII.h"



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_ARRAY_INCLUDED



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
