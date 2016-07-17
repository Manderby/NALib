
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

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
NA_IAPI NAURL* naInitURLWithUTF8CStringLiteral(NAURL* url, const NAUTF8Char* string);
NA_IAPI void naClearURL(NAURL* url);

// Creates a new string containing just the last path component.
// Note that there is no distinction if the last component is the name of a
// folder or of a file. If the file has a suffix, it is contained in the
// returned string.
NA_IAPI NAString* naNewStringWithURLFilename(NAURL* url);







// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////



NA_IDEF NAURL* naInitURL(NAURL* url){
  #ifndef NDEBUG
    if(!url)
      {naCrash("naInitURL", "url is Null-Pointer"); return NA_NULL;}
  #endif
  url->status = 0;
  naInitList(&(url->path));
  return url;
}





NA_IDEF NAURL* naInitURLWithUTF8CStringLiteral(NAURL* url, const NAUTF8Char* string){
  NAUTF8Char curchar;
  NAString* inputstring;
  NAString* pathcomponent;

  url = naInitURL(url); 
  if(!string){return url;} 
  inputstring = naNewStringWithUTF8CStringLiteral(string);

  curchar = *naGetStringUTF8Pointer(inputstring);
  if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
    url->status |= NA_URL_PATH_ABSOLUTE;
    inputstring = naNewStringExtraction(inputstring, 1, -1);
  }
  
  while(naGetStringSize(inputstring)){
    // Test for erroneous duplicate or ending delimiters
    curchar = *naGetStringUTF8Pointer(inputstring);
    if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
      inputstring = naNewStringExtraction(inputstring, 1, -1);
      continue;
    }
    
    pathcomponent = naParseStringPathComponent(inputstring);
    naAddListLastMutable(&(url->path), pathcomponent);
  }
  return url;
}



NA_IDEF void naClearURL(NAURL* url){
  naClearList(&(url->path));
}



NA_IDEF NAString* naNewStringWithURLFilename(NAURL* url){
  if(naGetListCount(&(url->path))){
    const NAString* lastcomponent = naGetListLastConst(&(url->path));
    return naNewStringExtraction(lastcomponent, 0, -1);
  }else{
    return naNewString();
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
