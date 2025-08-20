
#include "../../NAStruct/NABuffer.h"
#include "../../NAUtility/NABinaryData.h"
#include "../../NAUtility/NAString.h"
#include <string.h>
#include <ctype.h>



NA_DEF NABool naEqualUTF8CStringLiterals(const NAUTF8Char* string1, const NAUTF8Char* string2, size_t length, NABool caseSensitive) {
  if(!length) {
    size_t length1 = naStrlen(string1);
    size_t length2 = naStrlen(string2);
    if(length1 != length2) {
      return NA_FALSE;
    }
    length = length1;
  }
  if(caseSensitive) {
    int result = memcmp(string1, string2, length);
    if(result) {
      return NA_FALSE;
    }
  }else{
    while(length) {
      NAUTF8Char curChar1;
      NAUTF8Char curChar2;
      if(isalpha((const char)*string1)) {
        curChar1 = (NAUTF8Char)tolower(*string1);
      }else{
        curChar1 = *string1;
      }
      if(isalpha((const char)*string2)) {
        curChar2 = (NAUTF8Char)tolower(*string2);
      }else{
        curChar2 = *string2;
      }
      
      if(curChar1 != curChar2)
        return NA_FALSE;
      
      string1++;
      string2++;
      length--;
    }
  }
  return NA_TRUE;
}



NA_DEF NAUTF8Char* naAllocSprintf(NABool useTmp, const NAUTF8Char* format, ...) {
  va_list argumentList;
  va_list argumentList2;
  va_start(argumentList, format);
  va_copy(argumentList2, argumentList);

  size_t stringLen = naVarargStringLength(format, argumentList);

  NAUTF8Char* stringBuf = (useTmp)
    ? naMallocTmp(stringLen + 1)
    : naMalloc(stringLen + 1);
  naVsnprintf(stringBuf, stringLen + 1, format, argumentList2);
  stringBuf[stringLen] = '\0';

  va_end(argumentList);
  va_end(argumentList2);

  return stringBuf;
}



NA_DEF NAUTF8Char* naPriux8(uint8 value) {
  return naAllocSprintf(NA_TRUE, "%02x", (int)(value & NA_MAX_u8));
}
NA_DEF NAUTF8Char* naPriix8(int8 value) {
  return naAllocSprintf(NA_TRUE, "%02x", (int)(value & NA_MAX_u8));
}
NA_DEF NAUTF8Char* naPriux16(uint16 value) {
  return naAllocSprintf(NA_TRUE, "%04x", (int)(value & NA_MAX_u16));
}
NA_DEF NAUTF8Char* naPriix16(int16 value) {
  return naAllocSprintf(NA_TRUE, "%04x", (int)(value & NA_MAX_u16));
}
NA_DEF NAUTF8Char* naPriux32(uint32 value) {
  return naAllocSprintf(NA_TRUE, "%08x", (int)value);
}
NA_DEF NAUTF8Char* naPriix32(int32 value) {
  return naAllocSprintf(NA_TRUE, "%08x", (int)value);
}
NA_DEF NAUTF8Char* naPriux64(uint64 value) {
  return naAllocSprintf(NA_TRUE, "%08x%08x",
    naGetu64Hi(value),
    naGetu64Lo(value));
}
NA_DEF NAUTF8Char* naPriix64(int64 value) {
  return naAllocSprintf(NA_TRUE, "%08x%08x",
    naGeti64Hi(value),
    naGeti64Lo(value));
}
NA_DEF NAUTF8Char* naPriux128(uint128 value) {
  return naAllocSprintf(NA_TRUE, "%08x%08x%08x%08x",
    naGetu64Hi(naGetu128Hi(value)),
    naGetu64Lo(naGetu128Hi(value)),
    naGetu64Hi(naGetu128Lo(value)),
    naGetu64Lo(naGetu128Lo(value)));
}
NA_DEF NAUTF8Char* naPriix128(int128 value) {
  return naAllocSprintf(NA_TRUE, "%08x%08x%08x%08x",
    naGeti64Hi(naGeti128Hi(value)),
    naGeti64Lo(naGeti128Hi(value)),
    naGetu64Hi(naGeti128Lo(value)),
    naGetu64Lo(naGeti128Lo(value)));
}
NA_DEF NAUTF8Char* naPriux256(uint256 value) {
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
NA_DEF NAUTF8Char* naPriix256(int256 value) {
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
  #if NA_DEBUG
    const NAUTF8Char* cachedStr;
  #endif
};

NA_HAPI void na_DestructString(NAString* string);
NA_RUNTIME_TYPE(NAString, na_DestructString, NA_FALSE);




// We especially inline this definition as it is used many times in this file.
NA_DEF NAString* naNewString() {
  NAString* string = naNew(NAString);
  string->buffer = naCreateBuffer(NA_FALSE);
  #if NA_DEBUG
    string->cachedStr = NA_NULL;
  #endif
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithMutableUTF8Buffer(NAUTF8Char* buffer, size_t length, NAMutator destructor) {
  #if NA_DEBUG
    if(!destructor)
      naError("You must specify a destructor, as this string becomes the owner of the provided buffer.");
  #endif
  NAString* string = naNew(NAString);
  string->buffer = naCreateBufferWithMutableData(buffer, length, destructor);
  #if NA_DEBUG
    string->cachedStr = NA_NULL;
  #endif
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithFormat(const NAUTF8Char* format, ...) {
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



NA_DEF NAString* naNewStringWithArguments(const NAUTF8Char* format, va_list argumentList) {
  NAString* string;
  va_list argumentList2;
  va_list argumentList3;
  va_copy(argumentList2, argumentList);
  va_copy(argumentList3, argumentList);
  size_t stringLen = naVarargStringLength(format, argumentList2);
  if(stringLen) {
    NAUTF8Char* stringBuf = naMalloc(stringLen + 1);
    naVsnprintf(stringBuf, stringLen + 1, format, argumentList3);
    stringBuf[stringLen] = '\0';
    string = naNewStringWithMutableUTF8Buffer(stringBuf, stringLen, (NAMutator)naFree);
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



NA_DEF NAString* naNewStringExtraction(const NAString* srcString, int64 offset, int64 length) {
  NAString* string = naNewString();

  #if NA_DEBUG
    if(!srcString)
      naCrash("srcString is nullptr");
  #endif

  // Extract the string
  naRelease(string->buffer);
  string->buffer = naCreateBufferExtraction(srcString->buffer, offset, length);
  #if NA_DEBUG
    string->cachedStr = NA_NULL;
  #endif

  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithBufferExtraction(NABuffer* buffer, NARangei64 range) {
  NAString* string;
  #if NA_DEBUG
    if(!naIsLengthValueUsefuli64(buffer->range.length))
      naError("Buffer Range length is not useful.");
    if(!naIsLengthValueUsefuli64(range.length))
      naError("Range length is not useful.");
  #endif
  string = naNew(NAString);
  string->buffer = naCreateBufferExtraction(buffer, range.origin, range.length);
  #if NA_DEBUG
    string->cachedStr = NA_NULL;
  #endif
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_API NAString* naNewStringWithNewlineSanitization( NAString* string, NANewlineEncoding encoding) {
  NAString* newString;
  if(naIsStringEmpty(string)) {
    newString = naNewString();
  }else{
    NABufferIterator readIter;
    NABufferIterator writeIter;
    NABool writeNL;
    
    NABuffer* newbuffer = naCreateBuffer(NA_FALSE);
    naSetBufferNewlineEncoding(newbuffer, encoding);
    readIter = naMakeBufferAccessor(naGetStringBufferConst(string));
    writeIter = naMakeBufferModifier(newbuffer);
    writeNL = NA_FALSE;
    while(!naIsBufferAtEnd(&readIter)) {
      NAString* line;
      if(writeNL) {
        naWriteBufferNewLine(&writeIter);
      }else{
        writeNL = NA_TRUE;
      }
      line = naParseBufferLine(&readIter, NA_FALSE);
      naWriteBufferString(&writeIter, line);
      naDelete(line);
    }
    naClearBufferIterator(&readIter);
    naClearBufferIterator(&writeIter);
    newString = naNewStringWithBufferExtraction(newbuffer, naGetBufferRange(newbuffer));
    naRelease(newbuffer);
  }
  return newString;
}



NA_HDEF void na_DestructString(NAString* string) {
  naRelease(string->buffer);
}



NA_DEF size_t naGetStringByteSize(const NAString* string) {
  return naCasti64ToSize(naGetBufferRange(string->buffer).length);
}


NA_DEF const NAUTF8Char* naGetStringUTF8Pointer(const NAString* string) {
  #if NA_DEBUG
    NAString* mutablestring = (NAString*)string;
    if(!string) {
      naCrash("string is nullptr");
    }
  #endif

  if(naIsStringEmpty(string)) {
    return (const NAUTF8Char*)"";
  }else{
    int64 numChars;
    NAUTF8Char* newStr;
    numChars = naGetBufferRange(string->buffer).length;
    #if NA_DEBUG
      if(naEquali64(numChars, NA_ZERO_i64))
        naError("String is empty");
    #endif
    newStr = naMallocTmp(naCasti64ToSize(naAddi64(numChars, NA_ONE_i64)));
    naCacheBufferRange(string->buffer, naGetBufferRange(string->buffer));
    naWriteBufferToData(string->buffer, newStr);
    newStr[naCasti64ToSize(numChars)] = '\0';
    #if NA_DEBUG
      mutablestring->cachedStr = newStr;
    #endif
    return newStr;
  }
}



NA_DEF NABool naIsStringEmpty(const NAString* string) {
  #if NA_DEBUG
    if(!string) {
      naCrash("string is nullptr");
    }
  #endif
  return naIsBufferEmpty(string->buffer);
}




NA_API const NABuffer* naGetStringBufferConst(const NAString* string) {
  return string->buffer;
}
NA_API NABuffer* naGetStringBufferMutable(NAString* string) {
  return string->buffer;
}



NA_DEF NAUTF8Char naGetStringChar(NAString* string, size_t index) {
  return (NAUTF8Char)naGetBufferByteAtIndex(string->buffer, index);
}



NA_HDEF int64 naGetLastSlashOffset(const NAString* fileUrl) {
  int64 slashOffset = naSearchBufferByteOffset(
    fileUrl->buffer,
    NA_PATH_DELIMITER_UNIX,
    naGetRangei64Max(naGetBufferRange(fileUrl->buffer)),
    NA_FALSE);
    
  if(naEquali64(slashOffset, NA_INVALID_MEMORY_INDEX)) {
    int64 backSlashOffset = naSearchBufferByteOffset(
      fileUrl->buffer,
      NA_PATH_DELIMITER_WIN,
      naGetRangei64Max(naGetBufferRange(fileUrl->buffer)),
      NA_FALSE);
      
    return backSlashOffset;
  }

  return slashOffset;
}



NA_DEF NAString* naNewStringWithParentOfUrl(const NAString* fileUrl) {
  NAString* string;
  naCacheBufferRange(fileUrl->buffer, fileUrl->buffer->range);
  int64 slashOffset = naGetLastSlashOffset(fileUrl);
  if(!naEquali64(slashOffset, NA_INVALID_MEMORY_INDEX)) {
    string = naNewStringExtraction(fileUrl, NA_ZERO_i64, slashOffset);
  }else{
    string = naNewStringExtraction(fileUrl, NA_ZERO_i64, NA_MINUS_ONE_i64);
  }

  #if NA_STRING_ALWAYS_CACHE == 1
  naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithBaseNameOfUrl(const NAString* fileUrl) {
  NAString* string;
  naCacheBufferRange(fileUrl->buffer, fileUrl->buffer->range);
  int64 slashOffset = naGetLastSlashOffset(fileUrl);
  // If slashOffset is invalid, return all leading.
  if(naEquali64(slashOffset, NA_INVALID_MEMORY_INDEX)) {
    slashOffset = NA_MINUS_ONE_i64;
  }
  int64 dotOffset = naSearchBufferByteOffset(
    fileUrl->buffer,
    NA_SUFFIX_DELIMITER,
    naGetRangei64Max(naGetBufferRange(fileUrl->buffer)),
    NA_FALSE);
  // If dotpos is invalid, return the tailing characters.
  if(naEquali64(dotOffset, NA_INVALID_MEMORY_INDEX)) {
    string = naNewStringExtraction(fileUrl, naAddi64(slashOffset, NA_ONE_i64), NA_MINUS_ONE_i64);
  }else{
    string = naNewStringExtraction(fileUrl, naAddi64(slashOffset, NA_ONE_i64), naSubi64(naSubi64(dotOffset, slashOffset), NA_ONE_i64));
  }
  #if NA_STRING_ALWAYS_CACHE == 1
  naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringWithSuffixOfUrl(const NAString* fileUrl) {
  NAString* string;
  int64 dotOffset = naSearchBufferByteOffset(
    fileUrl->buffer,
    NA_SUFFIX_DELIMITER,
    naGetRangei64Max(naGetBufferRange(fileUrl->buffer)),
    NA_FALSE);
  if(naEquali64(dotOffset, NA_INVALID_MEMORY_INDEX)) {
    string = naNewString();
  }else{
    string = naNewStringExtraction(fileUrl, naAddi64(dotOffset, NA_ONE_i64), NA_MINUS_ONE_i64);
  }
  #if NA_STRING_ALWAYS_CACHE == 1
    naGetStringUTF8Pointer(string);
  #endif
  return string;
}



NA_DEF NAString* naNewStringCEscaped(const NAString* inputString) {
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)) {
    return naNewString();
  }
  iter = naMakeBufferAccessor(inputString->buffer);
  na_LocateBufferStart(&iter);
  buffer = naCreateBuffer(NA_FALSE);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)) {
    NAUTF8Char curChar = naReadBufferi8(&iter);
    switch(curChar) {
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
    default: naWriteBufferi8(&outiter, curChar); break;
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



NA_DEF NAString* naNewStringCUnescaped(const NAString* inputString) {
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)) {
    return naNewString();
  }
  buffer = naCreateBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  na_LocateBufferStart(&iter);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)) {
    NAUTF8Char curChar = naReadBufferi8(&iter);
    if(curChar == '\\') {
      curChar = naReadBufferi8(&iter);
      switch(curChar) {
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
        #if NA_DEBUG
          naError("Unrecognized escape character");
        #endif
        naWriteBufferi8(&outiter, curChar);
        break;
      }
    }else{
      naWriteBufferi8(&outiter, curChar);
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



NA_DEF NAString* naNewStringXMLEncoded(const NAString* inputString) {
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)) {
    return naNewString();
  }
  buffer = naCreateBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  naIterateBuffer(&iter, NA_ONE_i64);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)) {
    NAUTF8Char curChar = naReadBufferi8(&iter);
    switch(curChar) {
    case '&': naWriteBufferBytes(&outiter, "&amp;", 5); break;
    case '<': naWriteBufferBytes(&outiter, "&lt;", 4); break;
    case '>': naWriteBufferBytes(&outiter, "&gt;", 4); break;
    case '\"': naWriteBufferBytes(&outiter, "&quot;", 6); break;
    case '\'': naWriteBufferBytes(&outiter, "&apos;", 6); break;
    default: naWriteBufferi8(&outiter, curChar); break;
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}




NA_DEF NAString* naNewStringXMLDecoded(const NAString* inputString) {
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)) {
    return naNewString();
  }
  buffer = naCreateBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)) {
    NAUTF8Char curChar = naReadBufferi8(&iter);
    if(curChar == '&') {
      NAString* token = naParseBufferTokenWithDelimiter(&iter, ';', NA_FALSE);
      if(naEqualStringToUTF8CString(token, "amp",   NA_TRUE)) {
        naWriteBufferi8(&outiter, '&');
      }else if(naEqualStringToUTF8CString(token, "lt",    NA_TRUE)) {
        naWriteBufferi8(&outiter, '<');
      }else if(naEqualStringToUTF8CString(token, "gt",    NA_TRUE)) {
        naWriteBufferi8(&outiter, '>');
      }else if(naEqualStringToUTF8CString(token, "quot",  NA_TRUE)) {
        naWriteBufferi8(&outiter, '\"');
      }else if(naEqualStringToUTF8CString(token, "apos",  NA_TRUE)) {
        naWriteBufferi8(&outiter, '\'');
      }else{
        #if NA_DEBUG
          naError("Could not decode entity");
        #endif
        naWriteBufferi8(&outiter, curChar);
        naWriteBufferString(&outiter, token);
        naWriteBufferi8(&outiter, ';');
      }
      naDelete(token);
    }else{
      naWriteBufferi8(&outiter, curChar);
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



NA_DEF NAString* naNewStringEPSEncoded(const NAString* inputString) {
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)) {
    return naNewString();
  }
  buffer = naCreateBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)) {
    NAUTF8Char curChar = naReadBufferi8(&iter);
    switch(curChar) {
    case '\\': naWriteBufferBytes(&outiter, "\\\\", 2); break;
    case '(':  naWriteBufferBytes(&outiter, "\\(", 2); break;
    case ')':  naWriteBufferBytes(&outiter, "\\)", 2); break;
    default: naWriteBufferi8(&outiter, curChar); break;
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}


NA_DEF NAString* naNewStringEPSDecoded(const NAString* inputString) {
  NAString* string;
  NABuffer* buffer;
  NABufferIterator iter;
  NABufferIterator outiter;
  if(naIsStringEmpty(inputString)) {
    return naNewString();
  }
  buffer = naCreateBuffer(NA_FALSE);
  iter = naMakeBufferAccessor(inputString->buffer);
  outiter = naMakeBufferModifier(buffer);
  while(!naIsBufferAtInitial(&iter)) {
    NAUTF8Char curChar = naReadBufferi8(&iter);
    if(curChar == '\\') {
      curChar = naReadBufferi8(&iter);
      switch(curChar) {
      case '\\':  naWriteBufferi8(&outiter, '\\');  break;
      case '(':  naWriteBufferi8(&outiter, '(');  break;
      case ')':  naWriteBufferi8(&outiter, ')');  break;
      default:
        #if NA_DEBUG
          naError("Unrecognized escape character");
        #endif
        naWriteBufferi8(&outiter, curChar);
        break;
      }
    }else{
      naWriteBufferi8(&outiter, curChar);
    }
  }
  naClearBufferIterator(&outiter);
  naClearBufferIterator(&iter);
  string = naNewStringWithBufferExtraction(buffer, naGetBufferRange(buffer));
  naRelease(buffer);
  return string;
}



#if NA_OS == NA_OS_WINDOWS
  NA_DEF wchar_t* naAllocWideCharStringWithUTF8String(const NAUTF8Char* utf8String) {
    NAString* string = naNewStringWithFormat("%s", utf8String);
    NAString* newLineString = naNewStringWithNewlineSanitization(string, NA_NEWLINE_WIN);

    size_t length = naGetStringByteSize(newLineString);
    naDelete(string);
    size_t wideLength = (size_t)MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newLineString), (int)length, NULL, 0);
    wchar_t* outStr = (wchar_t*)naMalloc(((wideLength + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newLineString), (int)length, outStr, (int)wideLength);
    outStr[wideLength] = 0;
    naDelete(newLineString);
    return outStr;
  }

  NA_DEF char* naAllocAnsiStringWithUTF8String(const NAUTF8Char* utf8String) {
    NAString* string = naNewStringWithFormat("%s", utf8String);
    NAString* newLineString = naNewStringWithNewlineSanitization(string, NA_NEWLINE_WIN);
    size_t length = naGetStringByteSize(newLineString);
    naDelete(string);

    // We have to convert UTF8 first to WideChar, then back to 8 bit Ansi.
    size_t wideLength = (size_t)MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newLineString), (int)length, NULL, 0);
    wchar_t* wstr = naMalloc(((wideLength + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, naGetStringUTF8Pointer(newLineString), (int)length, wstr, (int)wideLength);
    wstr[wideLength] = 0;
    size_t ansiLength = (size_t)WideCharToMultiByte(CP_ACP, 0, wstr, (int)wideLength, NA_NULL, 0, NA_NULL, NA_NULL);
    char* outStr = (char*)naMalloc(((ansiLength + 1) * sizeof(char)));
    WideCharToMultiByte(CP_ACP, 0, wstr, (int)wideLength, outStr, (int)ansiLength, NA_NULL, NA_NULL);
    naFree(wstr);
    outStr[ansiLength] = 0;
    naDelete(newLineString);
    return outStr;
  }

  // Returns a newly allocated memory block containing the system-encoded
  // string. If you do not provide the length, it will be automatically
  // computed. The resulting string must be freed manually. COPIES ALWAYS!
  NA_DEF TCHAR* naAllocSystemStringWithUTF8String(const NAUTF8Char* utf8String) {
    #ifdef UNICODE
      return naAllocWideCharStringWithUTF8String(utf8String);
    #else
      return naAllocAnsiStringWithUTF8String(utf8String);
    #endif
  }

  NA_DEF NAString* naNewStringWithWideCharString(const wchar_t* wcharString) {
    size_t length = wcslen(wcharString);
    size_t utf8Length = WideCharToMultiByte(CP_UTF8, 0, wcharString, (int)length, NULL, 0, NULL, NULL);
    NAUTF8Char* stringBuf = naMalloc((size_t)(utf8Length + 1) * sizeof(NAUTF8Char));
    WideCharToMultiByte(CP_UTF8, 0, wcharString, (int)length, stringBuf, (int)utf8Length, NULL, NULL);
    NAString* newLineString = naNewStringWithMutableUTF8Buffer(stringBuf, (size_t)utf8Length, (NAMutator)naFree);
    NAString* string = naNewStringWithNewlineSanitization(newLineString, NA_NEWLINE_UNIX);
    naDelete(newLineString);
    return string;
  }
  
  NA_DEF NAString* naNewStringWithAnsiString(const char* ansiString) {
    size_t length = strlen(ansiString);
    size_t wideLength = (size_t)MultiByteToWideChar(CP_ACP, 0, ansiString, (int)length, NULL, 0);
    wchar_t* wstr = naMalloc(((wideLength + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_ACP, 0, ansiString, (int)length, wstr, (int)wideLength);
    wstr[wideLength] = 0;
    size_t utf8Length = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wideLength, NA_NULL, 0, NA_NULL, NA_NULL);
    NAUTF8Char* stringBuf = (NAUTF8Char*)naMalloc(((utf8Length + 1) * sizeof(NAUTF8Char)));
    WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wideLength, stringBuf, (int)utf8Length, NA_NULL, NA_NULL);
    naFree(wstr);
    stringBuf[utf8Length] = 0;
    NAString* newLineString = naNewStringWithMutableUTF8Buffer(stringBuf, utf8Length, (NAMutator)naFree);
    NAString* string = naNewStringWithNewlineSanitization(newLineString, NA_NEWLINE_UNIX);
    naDelete(newLineString);
    return string;
  }

  // Creates a new NAString from a system-encoded string. COPIES ALWAYS!
  NA_DEF NAString* naNewStringWithSystemString(const TCHAR* systemString) {
    #ifdef UNICODE
      return naNewStringWithWideCharString(systemString);
    #else
      return naNewStringWithAnsiString(systemString);
    #endif
  }
#endif



NA_DEF void naAppendStringString(NAString* originalString, const NAString* string2) {
  naAppendBufferToBuffer(originalString->buffer, string2->buffer);
}



NA_DEF void naAppendStringChar(NAString* originalString, NAUTF8Char newChar) {
  NAString* charString = naNewStringWithFormat("%c", newChar);
  naAppendBufferToBuffer(originalString->buffer, charString->buffer);
  naDelete(charString);
}



NA_DEF void naAppendStringFormat(NAString* originalString, const NAUTF8Char* format, ...) {
  va_list argumentList;
  va_start(argumentList, format);
  naAppendStringArguments(originalString, format, argumentList);
  va_end(argumentList);
}


NA_DEF void naAppendStringArguments(NAString* originalString, const NAUTF8Char* format, va_list argumentList) {
  NAString* formatString = naNewStringWithArguments(format, argumentList);
  naAppendBufferToBuffer(originalString->buffer, formatString->buffer);
  naDelete(formatString);
}





NA_DEF NABool naEqualStringToString(const NAString* string1, const NAString* string2, NABool caseSensitive) {
  return naEqualBufferToBuffer(string1->buffer, string2->buffer, caseSensitive);
}



NA_DEF NABool naEqualStringToUTF8CString(const NAString* string1, const NAUTF8Char* string2, NABool caseSensitive) {
  return naEqualBufferToData(string1->buffer, string2, naStrlen(string2), caseSensitive);
}



NA_DEF int8 naParseStringi8(const NAString* string) {
  int8 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferi8(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}
NA_DEF int16 naParseStringi16(const NAString* string) {
  int16 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferi16(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}
NA_DEF int32 naParseStringi32(const NAString* string) {
  int32 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferi32(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}
NA_DEF int64 naParseStringi64(const NAString* string) {
  int64 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferi64(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}


NA_DEF uint8 naParseStringu8(const NAString* string) {
  uint8 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferu8(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}
NA_DEF uint16 naParseStringu16(const NAString* string) {
  uint16 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferu16(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}
NA_DEF uint32 naParseStringu32(const NAString* string) {
  uint32 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferu32(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}
NA_DEF uint64 naParseStringu64(const NAString* string) {
  uint64 retValue;
  NABufferIterator iter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&iter, NA_ZERO_i64);
  retValue = naParseBufferu64(&iter, NA_FALSE);
  naClearBufferIterator(&iter);
  return retValue;
}

NA_DEF float naParseStringFloat(const NAString* string) {
  NAUTF8Char* buf;
  NABufferIterator bufIter;
  float retValue;
  size_t len = naGetStringByteSize(string);
  if(len > 20) {
    len = 20;
    #if NA_DEBUG
      naError("String truncated to 20 characters");
    #endif
  }
  buf = naMalloc((len + 1) * sizeof(NAUTF8Char));
  bufIter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&bufIter, NA_ZERO_i64);
  naReadBufferBytes(&bufIter, buf, len);
  naClearBufferIterator(&bufIter);
  retValue = (float)atof(buf);
  naFree(buf);
  return retValue;
}
NA_DEF double naParseStringDouble(const NAString* string) {
  NAUTF8Char* buf;
  NABufferIterator bufIter;
  double retValue;
  size_t len = naGetStringByteSize(string);
  if(len > 20) {
    len = 20;
    #if NA_DEBUG
      naError("String truncated to 20 characters");
    #endif
  }
  buf = naMalloc((len + 1) * sizeof(NAUTF8Char));
  bufIter = naMakeBufferAccessor(string->buffer);
  naLocateBufferAtStart(&bufIter, NA_ZERO_i64);
  naReadBufferBytes(&bufIter, buf, len);
  naClearBufferIterator(&bufIter);
  retValue = atof(buf);
  naFree(buf);
  return retValue;
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
