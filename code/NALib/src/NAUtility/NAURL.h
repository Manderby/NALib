
#ifndef NA_URL_INCLUDED
#define NA_URL_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "../NAStruct/NAList.h"
#include "NAString.h"


#if NA_OS == NA_OS_WINDOWS
  #define NA_PATH_DELIMITER_SYSTEM NA_PATH_DELIMITER_WIN
#elif NA_OS == NA_OS_MAC_OS_X
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
NA_API NAURL* naInitURLWithUTF8CStringLiteral( NAURL* url,
                                     const NAUTF8Char* string);
NA_IAPI void naClearURL(NAURL* url);

// Creates a new string containing just the last path component.
// Note that there is no distinction if the last component is the name of a
// folder or of a file. If the file has a suffix, it is contained in the
// returned string.
NA_API NAString* naNewStringWithURLFilename(NAURL* url);





// Inline implementations are in a separate file:
#include "NAURL/NAURLII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_ARRAY_INCLUDED



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
