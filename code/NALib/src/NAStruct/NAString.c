
#include "../NABuffer.h"
#include "../NABinaryData.h"
#include "../NAString.h"
#include "../NAURL.h"
#include <string.h>
#include <ctype.h>



NA_DEF NABool naEqualUTF8CStringLiterals(const NAUTF8Char* string1, const NAUTF8Char* string2, NAInt length, NABool caseSensitive){
  if(!length){
    NAInt length1 = naStrlen(string1);
    NAInt length2 = naStrlen(string2);
    if(length1 != length2){return NA_FALSE;}
    length = length1;
  }
  if(caseSensitive){
    int result = memcmp(string1, string2, (NAUInt)length);
    if(result){return NA_FALSE;}
  }else{
    while(length){
      NAUTF8Char curchar1;
      NAUTF8Char curchar2;
      if(isalpha((const char)*string1)){curchar1 = (NAUTF8Char)tolower(*string1);}else{curchar1 = *string1;}
      if(isalpha((const char)*string2)){curchar2 = (NAUTF8Char)tolower(*string2);}else{curchar2 = *string2;}
      if(curchar1 != curchar2){return NA_FALSE;}
      string1++;
      string2++;
      length--;
    }
  }
  return NA_TRUE;
}



NA_DEF NAUTF8Char* naAllocSprintf(NABool useTmp, const NAUTF8Char* format, ...){
  va_list argumentList;
  va_list argumentList2;
  va_start(argumentList, format);
  va_copy(argumentList2, argumentList);
  NAInt stringlen = naVarargStringLength(format, argumentList);

  NAUTF8Char* stringbuf = (useTmp)
    ? naMallocTmp((size_t)stringlen + 1)
    : naMalloc((NAInt)stringlen + 1);
  naVsnprintf(stringbuf, (NAUInt)(stringlen + 1), format, argumentList2);
  stringbuf[stringlen] = '\0';

  return stringbuf;
}



NA_DEF NAUTF8Char* naPriux8(uint8 value){
  return naAllocSprintf(NA_TRUE, "%02x", (int)(value & NA_MAX_u8));
}
NA_DEF NAUTF8Char* naPriix8(int8 value){
  return naAllocSprintf(NA_TRUE, "%02x", (int)(value & NA_MAX_u8));
}
NA_DEF NAUTF8Char* naPriux16(uint16 value){
  return naAllocSprintf(NA_TRUE, "%04x", (int)(value & NA_MAX_u16));
}
NA_DEF NAUTF8Char* naPriix16(int16 value){
  return naAllocSprintf(NA_TRUE, "%04x", (int)(value & NA_MAX_u16));
}
NA_DEF NAUTF8Char* naPriux32(uint32 value){
  return naAllocSprintf(NA_TRUE, "%08x", (int)value);
}
NA_DEF NAUTF8Char* naPriix32(int32 value){
  return naAllocSprintf(NA_TRUE, "%08x", (int)value);
}
NA_DEF NAUTF8Char* naPriux64(NAu64 value){
  return naAllocSprintf(NA_TRUE, "%08x%08x",
    naGetu64Hi(value),
    naGetu64Lo(value));
}
NA_DEF NAUTF8Char* naPriix64(NAi64 value){
  return naAllocSprintf(NA_TRUE, "%08x%08x",
    naGeti64Hi(value),
    naGeti64Lo(value));
}
NA_DEF NAUTF8Char* naPriux128(NAu128 value){
  return naAllocSprintf(NA_TRUE, "%08x%08x%08x%08x",
    naGetu64Hi(naGetu128Hi(value)),
    naGetu64Lo(naGetu128Hi(value)),
    naGetu64Hi(naGetu128Lo(value)),
    naGetu64Lo(naGetu128Lo(value)));
}
NA_DEF NAUTF8Char* naPriix128(NAi128 value){
  return naAllocSprintf(NA_TRUE, "%08x%08x%08x%08x",
    naGeti64Hi(naGeti128Hi(value)),
    naGeti64Lo(naGeti128Hi(value)),
    naGetu64Hi(naGeti128Lo(value)),
    naGetu64Lo(naGeti128Lo(value)));
}
NA_DEF NAUTF8Char* naPriux256(NAu256 value){
  return naAllocSprintf(NA_TRUE, "%08x%08x%08x%08x%08x%08x%08x%08x",
    naGetu64Hi(naGetu128Hi(naGetu256Hi(value))),
    naGetu64Lo(naGetu128Hi(naGetu256Hi(value))),
    naGetu64Hi(naGetu128Lo(naGetu256Hi(value))),
    naGetu64Lo(naGetu128Lo(naGetu256Hi(value))),
    naGetu64Hi(naGetu128Hi(naGetu256Lo(value))),
    naGetu64Lo(naGetu128Hi(naGetu256Lo(value))),
    naGetu64Hi(naGetu128Lo(naGetu256Lo(value))),
    naGetu64Lo(naGetu128Lo(naGetu256Lo(value))));
}
NA_DEF NAUTF8Char* naPriix256(NAi256 value){
  return naAllocSprintf(NA_TRUE, "%08x%08x%08x%08x%08x%08x%08x%08x",
    naGeti64Hi(naGeti128Hi(naGeti256Hi(value))),
    naGeti64Lo(naGeti128Hi(naGeti256Hi(value))),
    naGetu64Hi(naGeti128Lo(naGeti256Hi(value))),
    naGetu64Lo(naGeti128Lo(naGeti256Hi(value))),
    naGetu64Hi(naGetu128Hi(naGeti256Lo(value))),
    naGetu64Lo(naGetu128Hi(naGeti256Lo(value))),
    naGetu64Hi(naGetu128Lo(naGeti256Lo(value))),
    naGetu64Lo(naGetu128Lo(naGeti256Lo(value))));
}



struct NAString{
  NABuffer* buffer;
  #ifndef NDEBUG
    const NAUTF8Char* cachedstr;
  #endif
};

NA_HAPI void na_DestructString(NAString* string);
NA_RUNTIME_TYPE(NAString, na_DestructString, NA_FALSE);




// We especially inline this definition as it is used many times in this file.
NA_DEF NAString* naNewString(){
  NAString* string = naNew(NAString);
  string->buffer = naNewBuffer(NA_FALSE);
  #ifndef NDEBUG
    string->cachedstr = NA_NULL;
  #endif
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithMutableUTF8Buffer(NAUTF8Char* buffer, NAInt length, NAMutator destructor){
  #ifndef NDEBUG
    if(!destructor)
      naError("You must specify a destructor, as this string becomes the owner of the provided buffer.");
  #endif
  NAString* string = naNew(NAString);
  string->buffer = naNewBufferWithMutableData(buffer, length, destructor);
  #ifndef NDEBUG
    string->cachedstr = NA_NULL;
  #endif
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithFormat(const NAUTF8Char* format, ...){
  NAString* string;
  va_list argumentList;
  va_start(argumentList, format);
  string = naNewStringWithArguments(format, argumentList);
  va_end(argumentList);
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithArguments(const NAUTF8Char* format, va_list argumentList){
  NAString* string;
  NAInt stringlen;
  va_list argumentList2;
  va_list argumentList3;
  va_copy(argumentList2, argumentList);
  va_copy(argumentList3, argumentList);
  stringlen = naVarargStringLength(format, argumentList2);
  if(stringlen){
    NAUTF8Char* stringbuf = naMalloc((NAInt)stringlen + 1);
    naVsnprintf(stringbuf, (NAUInt)(stringlen + 1), format, argumentList3);
    stringbuf[stringlen] = '\0';
    string = naNewStringWithMutableUTF8Buffer(stringbuf, (NAInt)stringlen, (NAMutator)naFree);
  }else{
    string = naNewString();
  }
  va_end(argumentList2);
  va_end(argumentList3);
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringExtraction(const NAString* srcString, NAInt offset, NAInt length){
  NAString* string = naNewString();

  #ifndef NDEBUG
    if(!srcString)
      naCrash("srcString is Null-Pointer.");
  #endif

  // Extract the string
  naRelease(string->buffer);
  string->buffer = naNewBufferExtraction(srcString->buffer, offset, length);
  #ifndef NDEBUG
    string->cachedstr = NA_NULL;
  #endif

  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithBufferExtraction(NABuffer* buffer, NARangei range){
  NAString* string;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(buffer->range.length))
      naError("Buffer Range length is not useful.");
    if(!naIsLengthValueUsefuli(range.length))
      naError("Range length is not useful.");
  #endif
  string = naNew(NAString);
  string->buffer = naNewBufferExtraction(buffer, range.origin, range.length);
  #ifndef NDEBUG
    string->cachedstr = NA_NULL;
  #endif
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_API NAString* naNewStringWithNewlineSanitization( NAString* string, NANewlineEncoding encoding){
  NAString* newstring;
  if(naIsStringEmpty(string)){
    newstring = naNewString();
  }else{
    NABufferIterator readiter;
    NABufferIterator writeiter;
    NABool writeNL;
    
    NABuffer* newbuffer = naNewBuffer(NA_FALSE);
    naSetBufferNewlineEncoding(newbuffer, encoding);
    readiter = naMakeBufferAccessor(naGetStringBufferConst(string));
    writeiter = naMakeBufferModifier(newbuffer);
    writeNL = NA_FALSE;
    while(!naIsBufferAtEnd(&readiter)){
      NAString* line;
      if(writeNL){
        naWriteBufferNewLine(&writeiter);
      }else{
        writeNL = NA_TRUE;
      }
      line = naParseBufferLine(&readiter, NA_FALSE);
      naWriteBufferString(&writeiter, line);
      naDelete(line);
    }
    naClearBufferIterator(&readiter);
    naClearBufferIterator(&writeiter);
    newstring = naNewStringWithBufferExtraction(newbuffer, naGetBufferRange(newbuffer));
    naRelease(newbuffer);
  }
  return newstring;
}



NA_HDEF void na_DestructString(NAString* string){
  naRelease(string->buffer);
}



NA_DEF NAInt naGetStringBytesize(const NAString* string){
  return naGetBufferRange(string->buffer).length;
}


NA_DEF const NAUTF8Char* naGetStringUTF8Pointer(const NAString* string){
  #ifndef NDEBUG
    NAString* mutablestring = (NAString*)string;
    if(!string){
      naCrash("string is Null-Pointer.");
      return NA_NULL;
    }
  #endif

  if(naIsStringEmpty(string)){
    return (const NAUTF8Char*)"";
  }else{
    NAInt numchars;
    NAUTF8Char* newstr;
    numchars = naGetBufferRange(string->buffer).length;
    #ifndef NDEBUG
      if(!numchars)
        naError("String is empty");
    #endif
    newstr = naMallocTmp((size_t)(numchars + 1));
    naCacheBufferRange(string->buffer, naGetBufferRange(string->buffer));
    naWriteBufferToData(string->buffer, newstr);
    newstr[numchars] = '\0';
    #ifndef NDEBUG
      mutablestring->cachedstr = newstr;
    #endif
    return newstr;
  }
}



NA_DEF NABool naIsStringEmpty(const NAString* string){
  #ifndef NDEBUG
    if(!string){
      naCrash("string is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return naIsBufferEmpty(string->buffer);
}




NA_API const NABuffer* naGetStringBufferConst(const NAString* string){
  return string->buffer;
}
NA_API NABuffer* naGetStringBufferMutable(NAString* string){
  return string->buffer;
}



NA_DEF NAUTF8Char naGetStringChar(NAString* string, NAInt index){
  return (NAUTF8Char)naGetBufferByteAtIndex(string->buffer, index);
}



NA_DEF NAString* naNewStringWithBasenameOfPath(const NAString* filePath){
  NAString* string;
  naCacheBufferRange(filePath->buffer, filePath->buffer->range);
  NAInt dotoffset = naSearchBufferByteOffset(filePath->buffer, NA_SUFFIX_DELIMITER, naGetRangeiMax(naGetBufferRange(filePath->buffer)), NA_FALSE);
  // If dotpos is invalid, return the full string.
  if(dotoffset == NA_INVALID_MEMORY_INDEX){
    string = naNewStringExtraction(filePath, 0, -1);
  }else{
    string = naNewStringExtraction(filePath, 0, dotoffset);
  }
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithSuffixOfPath(const NAString* filePath){
  NAString* string;
  NAInt dotoffset = naSearchBufferByteOffset(filePath->buffer, NA_SUFFIX_DELIMITER, naGetRangeiMax(naGetBufferRange(filePath->buffer)), NA_FALSE);
  if(dotoffset == NA_INVALID_MEMORY_INDEX){
    string = naNewString();
  }else{
    string = naNewStringExtraction(filePath, dotoffset + 1, -1);
  }
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringCEscaped(const NAString* inputString){
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)){
    return naNewString();
  }
  iter = naMakeBufferAccessor(inputString->buffer);
  na_LocateBufferStart(&iter);
  buffer = naNewBuffer(NA_FALSE);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)){
    NAUTF8Char curchar = naReadBufferi8(&iter);
    switch(curchar){
    case '\a': naWriteBufferBytes(&outiter, "\\a", 2); break;
    case '\b': naWriteBufferBytes(&outiter, "\\b", 2); break;
    case '\f': naWriteBufferBytes(&outiter, "\\f", 2); break;
    case '\n': naWriteBufferBytes(&outiter, "\\n", 2); break;
    case '\r': naWriteBufferBytes(&outiter, "\\r", 2); break;
    case '\t': naWriteBufferBytes(&outiter, "\\t", 2); break;
    case '\v': naWriteBufferBytes(&outiter, "\\v", 2); break;
    case '\\': naWriteBufferBytes(&outiter, "\\\\", 2); break;
    case '\'': naWriteBufferBytes(&outiter, "\\\'", 2); break;
    case '\"': naWriteBufferBytes(&outiter, "\\\"", 2); break;
    case '\?': naWriteBufferBytes(&outiter, "\\?", 2); break;
    // todo: Add more escapes
    default: naWriteBufferi8(&outiter, curchar); break;
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



NA_DEF NAString* naNewStringCUnescaped(const NAString* inputString){
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)){
    return naNewString();
  }
  buffer = naNewBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  na_LocateBufferStart(&iter);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)){
    NAUTF8Char curchar = naReadBufferi8(&iter);
    if(curchar == '\\'){
      curchar = naReadBufferi8(&iter);
      switch(curchar){
      case 'a':  naWriteBufferi8(&outiter, '\a');  break;
      case 'b':  naWriteBufferi8(&outiter, '\b');  break;
      case 'f':  naWriteBufferi8(&outiter, '\f');  break;
      case 'n':  naWriteBufferi8(&outiter, '\n');  break;
      case 'r':  naWriteBufferi8(&outiter, '\r');  break;
      case 't':  naWriteBufferi8(&outiter, '\t');  break;
      case 'v':  naWriteBufferi8(&outiter, '\v');  break;
      case '\\': naWriteBufferi8(&outiter, '\\'); break;
      case '\'': naWriteBufferi8(&outiter, '\''); break;
      case '\"': naWriteBufferi8(&outiter, '\"'); break;
      case '?':  naWriteBufferi8(&outiter, '\?');  break;
    // todo: Add more escapes
      default:
        #ifndef NDEBUG
          naError("Unrecognized escape character");
        #endif
        naWriteBufferi8(&outiter, curchar);
        break;
      }
    }else{
      naWriteBufferi8(&outiter, curchar);
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



NA_DEF NAString* naNewStringXMLEncoded(const NAString* inputString){
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)){
    return naNewString();
  }
  buffer = naNewBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)){
    NAUTF8Char curchar = naReadBufferi8(&iter);
    switch(curchar){
    case '&': naWriteBufferBytes(&outiter, "&amp;", 5); break;
    case '<': naWriteBufferBytes(&outiter, "&lt;", 4); break;
    case '>': naWriteBufferBytes(&outiter, "&gt;", 4); break;
    case '\"': naWriteBufferBytes(&outiter, "&quot;", 6); break;
    case '\'': naWriteBufferBytes(&outiter, "&apos;", 6); break;
    default: naWriteBufferi8(&outiter, curchar); break;
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}




NA_DEF NAString* naNewStringXMLDecoded(const NAString* inputString){
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)){
    return naNewString();
  }
  buffer = naNewBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)){
    NAUTF8Char curchar = naReadBufferi8(&iter);
    if(curchar == '&'){
      NAString* token = naParseBufferTokenWithDelimiter(&iter, ';');
      if     (naEqualStringToUTF8CString(token, "amp",   NA_TRUE)){naWriteBufferi8(&outiter, '&');}
      else if(naEqualStringToUTF8CString(token, "lt",    NA_TRUE)){naWriteBufferi8(&outiter, '<');}
      else if(naEqualStringToUTF8CString(token, "gt",    NA_TRUE)){naWriteBufferi8(&outiter, '>');}
      else if(naEqualStringToUTF8CString(token, "quot",  NA_TRUE)){naWriteBufferi8(&outiter, '\"');}
      else if(naEqualStringToUTF8CString(token, "apos",  NA_TRUE)){naWriteBufferi8(&outiter, '\'');}
      else{
        #ifndef NDEBUG
          naError("Could not decode entity");
        #endif
        naWriteBufferi8(&outiter, curchar);
        naWriteBufferString(&outiter, token);
        naWriteBufferi8(&outiter, ';');
      }
      naDelete(token);
    }else{
      naWriteBufferi8(&outiter, curchar);
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



NA_DEF NAString* naNewStringEPSEncoded(const NAString* inputString){
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)){
    return naNewString();
  }
  buffer = naNewBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)){
    NAUTF8Char curchar = naReadBufferi8(&iter);
    switch(curchar){
    case '\\': naWriteBufferBytes(&outiter, "\\\\", 2); break;
    case '(':  naWriteBufferBytes(&outiter, "\\(", 2); break;
    case ')':  naWriteBufferBytes(&outiter, "\\)", 2); break;
    default: naWriteBufferi8(&outiter, curchar); break;
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}


NA_DEF NAString* naNewStringEPSDecoded(const NAString* inputString){
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)){
    return naNewString();
  }
  buffer = naNewBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)){
    NAUTF8Char curchar = naReadBufferi8(&iter);
    if(curchar == '\\'){
      curchar = naReadBufferi8(&iter);
      switch(curchar){
      case '\\':  naWriteBufferi8(&outiter, '\\');  break;
      case '(':  naWriteBufferi8(&outiter, '(');  break;
      case ')':  naWriteBufferi8(&outiter, ')');  break;
      default:
        #ifndef NDEBUG
          naError("Unrecognized escape character");
        #endif
        naWriteBufferi8(&outiter, curchar);
        break;
      }
    }else{
      naWriteBufferi8(&outiter, curchar);
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



#if NA_OS == NA_OS_WINDOWS
  NA_DEF wchar_t* naAllocWideCharStringWithUTF8String(const NAUTF8Char* utf8String){
    wchar_t* outstr;
    NAUInt length;
    NAUInt widelength;
    NAString* string = naNewStringWithFormat("%s", utf8String);
    NAString* newlinestring = naNewStringWithNewlineSanitization(string, NA_NEWLINE_WIN);
    length = naGetStringBytesize(newlinestring);
    naDelete(string);
    widelength = MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newlinestring), (int)length, NULL, 0);
    outstr = (wchar_t*)naMalloc(((widelength + 1) * naSizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newlinestring), (int)length, outstr, (int)widelength);
    outstr[widelength] = 0;
    naDelete(newlinestring);
    return outstr;
  }

  NA_DEF char* naAllocAnsiStringWithUTF8String(const NAUTF8Char* utf8String){
    char* outstr;
    NAUInt length;
    NAUInt widelength;
    wchar_t* wstr;
    NAUInt ansilength;
    NAString* string = naNewStringWithFormat("%s", utf8String);
    NAString* newlinestring = naNewStringWithNewlineSanitization(string, NA_NEWLINE_WIN);
    length = naGetStringBytesize(newlinestring);
    naDelete(string);
    // We have to convert UTF8 first to WideChar, then back to 8 bit Ansi.
    widelength = MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newlinestring), (int)length, NULL, 0);
    wstr = naMalloc(((widelength + 1) * naSizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newlinestring), (int)length, wstr, (int)widelength);
    wstr[widelength] = 0;
    ansilength = WideCharToMultiByte(CP_ACP, 0, wstr, (int)widelength, NA_NULL, 0, NA_NULL, NA_NULL);
    outstr = (char*)naMalloc(((ansilength + 1) * naSizeof(char)));
    WideCharToMultiByte(CP_ACP, 0, wstr, (int)widelength, outstr, (int)ansilength, NA_NULL, NA_NULL);
    naFree(wstr);
    outstr[ansilength] = 0;
    naDelete(newlinestring);
    return outstr;
  }

  // Returns a newly allocated memory block containing the system-encoded
  // string. If you do not provide the length, it will be automatically
  // computed. The resulting string must be freed manually. COPIES ALWAYS!
  NA_DEF TCHAR* naAllocSystemStringWithUTF8String(const NAUTF8Char* utf8String){
    #ifdef UNICODE
      return naAllocWideCharStringWithUTF8String(utf8String);
    #else
      return naAllocAnsiStringWithUTF8String(utf8String);
    #endif
  }

  NA_DEF NAString* naNewStringFromWideCharString(wchar_t* wcharString){
    NAString* string;
    NAString* newlinestring;
    NAUInt length;
    NAInt utf8length;
    NAUTF8Char* stringbuf;
    length = wcslen(wcharString);
    utf8length = WideCharToMultiByte(CP_UTF8, 0, wcharString, (int)length, NULL, 0, NULL, NULL);
    stringbuf = naMalloc((utf8length + 1) * naSizeof(NAUTF8Char));
    WideCharToMultiByte(CP_UTF8, 0, wcharString, (int)length, stringbuf, (int)utf8length, NULL, NULL);
    newlinestring = naNewStringWithMutableUTF8Buffer(stringbuf, utf8length, (NAMutator)naFree);
    string = naNewStringWithNewlineSanitization(newlinestring, NA_NEWLINE_UNIX);
    naDelete(newlinestring);
    return string;
  }

  NA_DEF NAString* naNewStringFromAnsiString(char* ansiString){
    NAString* string;
    NAString* newlinestring;
    NAUInt length;
    NAUInt widelength;
    wchar_t* wstr;
    NAUInt utf8length;
    NAUTF8Char* stringbuf;
    length = strlen(ansiString);
    widelength = MultiByteToWideChar(CP_ACP, 0, ansiString, (int)length, NULL, 0);
    wstr = naMalloc(((widelength + 1) * naSizeof(wchar_t)));
    MultiByteToWideChar(CP_ACP, 0, ansiString, (int)length, wstr, (int)widelength);
    wstr[widelength] = 0;
    utf8length = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)widelength, NA_NULL, 0, NA_NULL, NA_NULL);
    stringbuf = (NAUTF8Char*)naMalloc(((utf8length + 1) * naSizeof(NAUTF8Char)));
    WideCharToMultiByte(CP_UTF8, 0, wstr, (int)widelength, stringbuf, (int)utf8length, NA_NULL, NA_NULL);
    naFree(wstr);
    stringbuf[utf8length] = 0;
    newlinestring = naNewStringWithMutableUTF8Buffer(stringbuf, utf8length, (NAMutator)naFree);
    string = naNewStringWithNewlineSanitization(newlinestring, NA_NEWLINE_UNIX);
    naDelete(newlinestring);
    return string;
  }

  // Creates a new NAString from a system-encoded string. COPIES ALWAYS!
  NA_DEF NAString* naNewStringFromSystemString(TCHAR* systemString){
    #ifdef UNICODE
      return naNewStringFromWideCharString(systemString);
    #else
      return naNewStringFromAnsiString(systemString);
    #endif
  }
#endif



NA_DEF void naAppendStringString(NAString* originalString, const NAString* string2){
  naAppendBufferToBuffer(originalString->buffer, string2->buffer);
}



NA_DEF void naAppendStringChar(NAString* originalString, NAUTF8Char newChar){
  NAString* charstring = naNewStringWithFormat("%c", newChar);
  naAppendBufferToBuffer(originalString->buffer, charstring->buffer);
  naDelete(charstring);
}



NA_DEF void naAppendStringFormat(NAString* originalString, const NAUTF8Char* format, ...){
  va_list argumentList;
  va_start(argumentList, format);
  naAppendStringArguments(originalString, format, argumentList);
  va_end(argumentList);
}


NA_DEF void naAppendStringArguments(NAString* originalString, const NAUTF8Char* format, va_list argumentList){
  NAString* formatstring = naNewStringWithArguments(format, argumentList);
  naAppendBufferToBuffer(originalString->buffer, formatstring->buffer);
  naDelete(formatstring);
}





NA_DEF NABool naEqualStringToString(const NAString* string1, const NAString* string2, NABool caseSensitive){
  return naEqualBufferToBuffer(string1->buffer, string2->buffer, caseSensitive);
}



NA_DEF NABool naEqualStringToUTF8CString(const NAString* string1, const NAUTF8Char* string2, NABool caseSensitive){
  return naEqualBufferToData(string1->buffer, string2, naStrlen(string2), caseSensitive);
}



NA_DEF int8 naParseStringi8(const NAString* string){
  int8 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferi8(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}
NA_DEF int16 naParseStringi16(const NAString* string){
  int16 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferi16(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}
NA_DEF int32 naParseStringi32(const NAString* string){
  int32 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferi32(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}
NA_DEF NAi64 naParseStringi64(const NAString* string){
  NAi64 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferi64(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}


NA_DEF uint8 naParseStringu8(const NAString* string){
  uint8 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferu8(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}
NA_DEF uint16 naParseStringu16(const NAString* string){
  uint16 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferu16(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}
NA_DEF uint32 naParseStringu32(const NAString* string){
  uint32 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferu32(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}
NA_DEF NAu64 naParseStringu64(const NAString* string){
  NAu64 retvalue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&iter, 0);
  retvalue = naParseBufferu64(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retvalue;
}

NA_DEF float naParseStringFloat(const NAString* string){
  NAUTF8Char* buf;
  NABufferIterator bufiter;
  float retvalue;
  NAInt len = naGetStringBytesize(string);
  if(len > 20){
    len = 20;
    #ifndef NDEBUG
      naError("String truncated to 20 characters");
    #endif
  }
  buf = naMalloc((len + 1) * naSizeof(NAUTF8Char));
  bufiter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&bufiter, 0);
  naReadBufferBytes(&bufiter, buf, len);
  naClearBufferIterator(&bufiter);
  retvalue = (float)atof(buf);
  naFree(buf);
  return retvalue;
}
NA_DEF double naParseStringDouble(const NAString* string){
  NAUTF8Char* buf;
  NABufferIterator bufiter;
  double retvalue;
  NAInt len = naGetStringBytesize(string);
  if(len > 20){
    len = 20;
    #ifndef NDEBUG
      naError("String truncated to 20 characters");
    #endif
  }
  buf = naMalloc((len + 1) * naSizeof(NAUTF8Char));
  bufiter = naMakeBufferAccessor(string->buffer);
  naLocateBufferFromStart(&bufiter, 0);
  naReadBufferBytes(&bufiter, buf, len);
  naClearBufferIterator(&bufiter);
  retvalue = atof(buf);
  naFree(buf);
  return retvalue;
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
