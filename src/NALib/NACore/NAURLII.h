
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// This file contains inline implementations of the file NAURL.h
// Do not include this file directly! It will automatically be included when
// including "NAURL.h"



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
  NAString inputstring;
  NAString pathcomponent;
  NABufferIterator iter;

  url = naInitURL(url); 
  if(!string){return url;} 
  inputstring = naMakeStringWithUTF8CStringLiteral(string);

  curchar = *naGetStringUTF8Pointer(&inputstring);
  if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
    url->status |= NA_URL_PATH_ABSOLUTE;
    inputstring = naMakeStringExtraction(&inputstring, 1, -1);
  }
  
  iter = naMakeBufferAccessor(naGetStringBufferConst(&inputstring));
  
  while(naGetStringBytesize(&inputstring)){
    // Test for erroneous duplicate or ending delimiters
    curchar = *naGetStringUTF8Pointer(&inputstring);
    if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
      inputstring = naMakeStringExtraction(&inputstring, 1, -1);
      continue;
    }
    
    pathcomponent = naParseBufferPathComponent(&iter);
    naAddListLastMutable(&(url->path), &pathcomponent);
  }
  
  naClearBufferIterator(&iter);
  
  return url;
}



NA_IDEF void naClearURL(NAURL* url){
  naClearList(&(url->path));
}



NA_IDEF NAString naMakeStringWithURLFilename(NAURL* url){
  if(naGetListCount(&(url->path))){
    const NAString* lastcomponent = naGetListLastConst(&(url->path));
    return naMakeStringExtraction(lastcomponent, 0, -1);
  }else{
    return naMakeString();
  }
}





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
