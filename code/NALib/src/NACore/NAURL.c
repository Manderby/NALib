
#include "../NAURL.h"


NA_DEF NAURL* naInitURLWithUTF8CStringLiteral(NAURL* url, const NAUTF8Char* string){
  NAUTF8Char curchar;
  NAString* inputString;
  NAString* pathcomponent;
  NABufferIterator iter;

  url = naInitURL(url);
  if(!string){return url;}
  inputString = naNewStringWithUTF8CStringLiteral(string);

  curchar = *naGetStringUTF8Pointer(inputString);
  if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
    url->status |= NA_URL_PATH_ABSOLUTE;
    inputString = naNewStringExtraction(inputString, 1, -1);
  }

  iter = naMakeBufferAccessor(naGetStringBufferMutable(inputString));
  na_LocateBufferStart(&iter);

  while(!naIsBufferAtInitial(&iter)){
    // Test for erroneous duplicate or ending delimiters
    curchar = (NAUTF8Char)naGetBufferu8(&iter);
    if((curchar == NA_PATH_DELIMITER_UNIX) || (curchar == NA_PATH_DELIMITER_WIN)){
      naIterateBuffer(&iter, 1);
      continue;
    }

    pathcomponent = naParseBufferPathComponent(&iter);
    naAddListLastMutable(&(url->path), pathcomponent);
  }

  naClearBufferIterator(&iter);

  return url;
}



NA_DEF NAString* naNewStringWithURLFilename(NAURL* url){
  if(naGetListCount(&(url->path))){
    const NAString* lastcomponent = naGetListLastConst(&(url->path));
    return naNewStringExtraction(lastcomponent, 0, -1);
  }else{
    return naNewString();
  }
}



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
