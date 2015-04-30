
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_URL_INCLUDED
#define NA_URL_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAList.h"


#define NA_PATH_DELIMITER_UNIX '/'
#define NA_PATH_DELIMITER_WIN  '\\'
#define NA_SUFFIX_DELIMITER    '.'

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


NA_IAPI NAURL* naCreateURL(NAURL* url);

// - Both delimiters / and \ will be detected.
// - If the path starts with a path delimiter, it is considered absolute.
//   Example: /usr/local is an absolute path, src/NALib is not.
// - Erroneous duplicate path delimiters or ending delimiters will be ignored.
//   Example: /usr//local/ results in /usr/local
NA_IAPI NAURL* naCreateURLWithUTF8CStringLiteral(NAURL* url, const NAUTF8Char* string);
NA_IAPI void naClearURL(NAURL* url);
NA_IAPI void naDestroyURL(NAURL* url);

// Creates a new string containing just the last path component.
// Note that there is no distinction if the last component is the name of a
// folder or of a file. If the file has a suffix, it is contained in the
// returned string.
NA_IAPI NAString* naCreateStringWithURLFilename(NAString* string, NAURL* url);






NA_IDEF NAURL* naCreateURL(NAURL* url){
  url = naAllocateIfNull(url, sizeof(NAURL));
  url->status = 0;
  naCreateList(&(url->path));
  return url;
}


NA_IDEF NAURL* naCreateURLWithUTF8CStringLiteral(NAURL* url, const NAUTF8Char* string){
  NAUTF8Char curchar;
  NAString inputstring;
  NAString pathcomponent;
  NAString* newpathcomponent;

  url = naCreateURL(url); 
  if(!string){return url;} 
  naCreateStringWithUTF8CStringLiteral(&inputstring, string);

  curchar = *naGetStringUTF8Pointer(&inputstring);
  if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
    url->status |= NA_URL_PATH_ABSOLUTE;
    naCreateStringExtraction(&inputstring, &inputstring, 1, -1);
  }
  
  naCreateString(&pathcomponent);
  while(naGetStringSize(&inputstring)){
    // Test for erroneous duplicate or ending delimiters
    curchar = *naGetStringUTF8Pointer(&inputstring);
    if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
      naCreateStringExtraction(&inputstring, &inputstring, 1, -1);
      continue;
    }
    
    naParseStringPathComponent(&inputstring, &pathcomponent);
    newpathcomponent = naCreateStringExtraction(NA_NULL, &pathcomponent, 0, -1);
    naAddListLastMutable(&(url->path), newpathcomponent);
  }
  return url;
}



NA_IDEF void naClearURL(NAURL* url){
  naClearList(&(url->path), (NADestructor)naDestroyString);
}



NA_IDEF void naDestroyURL(NAURL* url){
  naClearURL(url);
  free(url);
}


NA_IDEF NAString* naCreateStringWithURLFilename(NAString* string, NAURL* url){
  if(naGetListCount(&(url->path))){
    const NAString* lastcomponent = naGetListLastConst(&(url->path));
    return naCreateStringExtraction(string, lastcomponent, 0, -1);
  }else{
    return naCreateString(string);
  }
}


#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_ARRAY_INCLUDED



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
