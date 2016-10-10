
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "../NABinaryData.h"
#include "../NAString.h"
#include "../NAURL.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>


struct NAString{
  NAByteArray array;
};
// Note that an NAString is considered empty if the underlying array is empty.
// If that is the case, encoding contains garbage values.


NA_HDEF void naDestructString(NAString* string);
NA_RUNTIME_TYPE(NAString, naDestructString);





NA_IDEF NAUInt naVsnprintf(NAUTF8Char* buffer, NAUInt size, const NAUTF8Char* newstr, va_list argumentlist){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAInt)_vsnprintf_s(buffer, (size_t)size, (size_t)size, newstr, argumentlist);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAInt)vsnprintf((char*)buffer, (size_t)size, (const char*)newstr, argumentlist);
  #endif
}



// Returns the number of characters needed to transform the given string and
// arguments using sprintf.
NA_IDEF NAUInt naVarargStringSize(const NAUTF8Char* string, va_list args){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return (NAInt)_vscprintf(string, args);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return (NAInt)naVsnprintf(NA_NULL, 0, string, args);
  #endif
}



// This is the destructor for a string. It is marked as a helper as it should
// only be called by the runtime system
NA_HDEF void naDestructString(NAString* string){
  if(!naIsStringEmpty(string)){naClearByteArray(&(string->array));}
}





// We especially inline this definition as it is used many times in this file.
NA_DEF NA_INLINE NAString* naNewString(void){
  NAString* string = naNew(NAString);
  naInitByteArray(&(string->array));
  return string;
}



NA_DEF NAString* naNewStringWithUTF8CStringLiteral(const NAUTF8Char* ptr){
  // Declaration before implementation. Needed for C90.
  NAString* string;
  NAInt size;
  
  #ifndef NDEBUG
    if(!ptr){
      naCrash("naNewStringWithUTF8CStringLiteral", "ptr is Null-Pointer");
      return NA_NULL;
    }
  #endif
  
  size = (NAInt)naStrlen(ptr);
  if(size){
    string = naNew(NAString);
    // C-Strings are always expected to be Null-terminated, meaning: The Byte
    // with index [size] must be binary zero. As we are not copying but just
    // referencing the pointer, we can safely use the array without this byte
    // and still be able to say: We are null-terminated!
    naInitByteArrayWithConstBuffer(&(string->array), ptr, -size);
  }else{
    string = naNewString();
  }
  
  return string;
}



NA_DEF NAString* naNewStringWithMutableUTF8Buffer(NAUTF8Char* buffer, NAInt size, NAPointerCleanup ownership){
  NAString* string = naNew(NAString);
  naInitByteArrayWithMutableBuffer(&(string->array), buffer, size, ownership);
  return string;
}



NA_DEF NAString* naNewStringWithFormat(const NAUTF8Char* format, ...){
  NAString* string;
  va_list argumentlist;
  va_start(argumentlist, format);
  string = naNewStringWithArguments(format, argumentlist);
  va_end(argumentlist);
  return string;
}



NA_DEF NAString* naNewStringWithArguments(const NAUTF8Char* format, va_list argumentlist){
  // Declaration before implementation. Needed for C90.
  NAString* string;
  NAUInt stringlen;
  va_list argumentlist2;
  va_list argumentlist3;
  va_copy(argumentlist2, argumentlist);
  va_copy(argumentlist3, argumentlist);
  stringlen = naVarargStringSize(format, argumentlist2);
  if(stringlen){
    NAUTF8Char* stringbuf = naMalloc(-(NAInt)stringlen);
    naVsnprintf(stringbuf, stringlen + 1, format, argumentlist3);
    string = naNewStringWithMutableUTF8Buffer(stringbuf, -(NAInt)stringlen, NA_MEMORY_CLEANUP_FREE);
  }else{
    string = naNewString();
  }
  va_end(argumentlist2);
  va_end(argumentlist3);
  return string;
}



NA_DEF NAString* naNewStringExtraction(const NAString* srcstring, NAInt offset, NAInt size){
  // Declaration before implementation. Needed for C90.
  NAString* string;

  #ifndef NDEBUG
    if(!srcstring){
      naCrash("naNewStringExtraction", "srcstring is Null-Pointer.");
      return NA_NULL;
    }
  #endif

  string = naNewString();

  if(naIsStringEmpty(srcstring)){
    #ifndef NDEBUG
      // If src is empty, return an empty string. But check if this is
      // intentional or an error:
      if(offset || size)
        naError("naNewStringExtraction", "Invalid string extraction of empty string.");
    #endif
  }else{
    // Extract the string
    naInitByteArrayExtraction(&(string->array), &(srcstring->array), offset, size);
  }
  
  return string;
}



NA_DEF NAString* naNewStringWithBasenameOfFilename(const NAString* filename){
  NAInt dotpos = naGetStringCharacterPos(filename, NA_SUFFIX_DELIMITER, -1);
  // If dotpos is invalid, return the full string.
  if(dotpos == NA_INVALID_MEMORY_INDEX){
    return naNewStringExtraction(filename, 0, -1);
  }else{
    return naNewStringExtraction(filename, 0, dotpos - 1);
  }
}



NA_DEF NAString* naNewStringWithSuffixOfFilename(const NAString* filename){
  NAInt dotpos = naGetStringCharacterPos(filename, NA_SUFFIX_DELIMITER, -1);
  if(dotpos == NA_INVALID_MEMORY_INDEX){
    return naNewString();
  }else{
    return naNewStringExtraction(filename, dotpos + 1, -1);
  }
}



NA_DEF NAString* naNewStringXMLEncoded(const NAString* inputstring){
  // Declaration before implementation. Needed for C90.
  NAUInt i;
  NAUInt inputsize;
  NAInt destsize;
  const NAUTF8Char* inptr;
  NAUTF8Char* destptr;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!inputstring){
      naCrash("naNewStringXMLEncoded", "input string is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  inputsize = naGetStringSize(inputstring);
  if(!inputsize){return naNewString();}
  
  // Count the required number of utf8 characters.
  destsize = 0;
  inptr = naGetStringUTF8Pointer(inputstring);
  for(i=0; i<inputsize; i++){
    switch(*inptr){
    case '&': destsize += 5; break;
    case '<': destsize += 4; break;
    case '>': destsize += 4; break;
    case '\"': destsize += 6; break;
    case '\'': destsize += 6; break;
    default: destsize += 1; break;
    }
    inptr++;
  }

  #ifndef NDEBUG
    if(destsize <= 0)
      naError("naNewStringXMLEncoded", "encoded size invalid. String too long?");
  #endif
  // Create the string with the required length
  stringbuf = naMalloc(-destsize);
  inptr = naGetStringUTF8Pointer(inputstring);
  destptr = stringbuf;

  // Copy all characters and encode them if necessary.
  for(i=0; i<inputsize; i++){
    switch(*inptr){
    case '&':  *destptr++ = '&'; *destptr++ = 'a'; *destptr++ = 'm'; *destptr++ = 'p'; *destptr++ = ';'; break;
    case '<':  *destptr++ = '&'; *destptr++ = 'l'; *destptr++ = 't'; *destptr++ = ';'; break;
    case '>':  *destptr++ = '&'; *destptr++ = 'g'; *destptr++ = 't'; *destptr++ = ';'; break;
    case '\"': *destptr++ = '&'; *destptr++ = 'q'; *destptr++ = 'u'; *destptr++ = 'o'; *destptr++ = 't'; *destptr++ = ';'; break;
    case '\'': *destptr++ = '&'; *destptr++ = 'a'; *destptr++ = 'p'; *destptr++ = 'o'; *destptr++ = 's'; *destptr++ = ';'; break;
    default: *destptr++ = *inptr; break;
    }
    inptr++;
  }

  return naNewStringWithMutableUTF8Buffer(stringbuf, -destsize, NA_MEMORY_CLEANUP_FREE);
}




//NA_DEF NAString* naNewStringXMLDecoded(const NAString* inputstring){
//  // Declaration before implementation. Needed for C90.
//  NAInt inputsize;
//  const NAUTF8Char* inptr;
//  NAUTF8Char* destptr;
//  NAInt i;
//  NAInt finalsize;
//  NAUTF8Char* stringbuf;
//  NAString* string;
//
//  #ifndef NDEBUG
//    if(!inputstring){
//      naCrash("naNewStringXMLDecoded", "input string is Null-Pointer.");
//      return NA_NULL;
//    }
//  #endif
//
//  inputsize = naGetStringSize(inputstring);
//  if(!inputsize){return naNewString();}
//
//  // Create a string with sufficient characters. As XML entities are always
//  // longer than their decoded character, we just use the same size.
//  stringbuf = naMalloc(-inputsize);
//  string = naNewStringWithMutableUTF8Buffer(stringbuf, -inputsize, NA_MEMORY_CLEANUP_FREE);
//  inptr = naGetStringUTF8Pointer(inputstring);
//  destptr = stringbuf;
//
//  // Copy all characters and decode them if necessary.
//  for(i=0; i<inputsize; i++){
//    if(inptr[i] == '&'){
//      if(((inputsize - i) >= 5) && (inptr[i+1] == 'a') && (inptr[i+2] == 'm') && (inptr[i+3] == 'p') && (inptr[i+4] == ';')){ *destptr++ = '&'; i += 4; }
//      else if(((inputsize - i) >= 4) && (inptr[i+1] == 'l') && (inptr[i+2] == 't') && (inptr[i+3] == ';')){ *destptr++ = '<'; i += 3; }
//      else if(((inputsize - i) >= 4) && (inptr[i+1] == 'g') && (inptr[i+2] == 't') && (inptr[i+3] == ';')){ *destptr++ = '>'; i += 3; }
//      else if(((inputsize - i) >= 6) && (inptr[i+1] == 'q') && (inptr[i+2] == 'u') && (inptr[i+3] == 'o') && (inptr[i+4] == 't') && (inptr[i+5] == ';')){ *destptr++ = '\"'; i += 5; }
//      else if(((inputsize - i) >= 6) && (inptr[i+1] == 'a') && (inptr[i+2] == 'p') && (inptr[i+3] == 'o') && (inptr[i+4] == 's') && (inptr[i+5] == ';')){ *destptr++ = '\''; i += 5; }
//      else{
//        *destptr++ = inptr[i];
//      }
//    }else{
//      *destptr++ = inptr[i];
//    }
//  }
//
//  // todo: numeric entities.
//
//  // The string is marked as NULL-Terminated. So we make sure this is the case.
//  *destptr = '\0';
//  // Finally, we shrink the string to its actual size
//  finalsize = destptr - stringbuf;
//  string = naNewStringExtraction(string, 0, finalsize);
//
//  return string;
//}



//NA_DEF NAString* naNewStringEPSEncoded(const NAString* inputstring){
//  // Declaration before implementation. Needed for C90.
//  NAUInt i;
//  NAUInt inputsize;
//  NAInt destsize;
//  const NAUTF8Char* inptr;
//  NAUTF8Char* destptr;
//  NAUTF8Char* stringbuf;
//  NAString* string;
//
//  #ifndef NDEBUG
//    if(!inputstring){
//      naCrash("naNewStringEPSEncoded", "input string is Null-Pointer.");
//      return NA_NULL;
//    }
//  #endif
//  inputsize = naGetStringSize(inputstring);
//  if(!inputsize){return naNewString();}
//  
//  // Count the required number of utf8 characters.
//  destsize = 0;
//  inptr = naGetStringUTF8Pointer(inputstring);
//  for(i=0; i<inputsize; i++){
//    switch(*inptr){
//    case '\\': destsize += 2; break;
//    case '(': destsize += 2; break;
//    case ')': destsize += 2; break;
//    default: destsize += 1; break;
//    }
//    inptr++;
//  }
//
//  #ifndef NDEBUG
//    if(destsize <= 0)
//      naError("naNewStringEPSEncoded", "encoded size invalid. String too long?");
//  #endif
//  // Create the string with the required length
//  stringbuf = naMalloc(-destsize);
//  string = naNewStringWithMutableUTF8Buffer(stringbuf, -destsize, NA_MEMORY_CLEANUP_FREE);
//  inptr = naGetStringUTF8Pointer(inputstring);
//  destptr = stringbuf;
//
//  // Copy all characters and encode them if necessary.
//  for(i=0; i<inputsize; i++){
//    switch(*inptr){
//    case '\\': *destptr++ = '\\'; *destptr++ = '\\'; break;
//    case '(':  *destptr++ = '\\'; *destptr++ = '(';  break;
//    case ')':  *destptr++ = '\\'; *destptr++ = ')';  break;
//    default: *destptr++ = *inptr; break;
//    }
//    inptr++;
//  }
//
//  return string;
//}


//NA_DEF NAString* naNewStringEPSDecoded(const NAString* inputstring){
//
//  // Declaration before implementation. Needed for C90.
//  NAInt i;
//  NAInt inputsize;
//  const NAUTF8Char* inptr;
//  NAUTF8Char* destptr;
//  NAInt finalsize;
//  NAUTF8Char* stringbuf;
//  NAString* string;
//
//  #ifndef NDEBUG
//    if(!inputstring){
//      naCrash("naNewStringEPSDecoded", "input string is Null-Pointer.");
//      return NA_NULL;
//    }
//  #endif
//
//  inputsize = naGetStringSize(inputstring);
//  if(!inputsize){return naNewString();}
//
//  // Create a string with sufficient characters. As EPS entities are always
//  // longer than their decoded character, we just use the same size.
//  stringbuf = naMalloc(-inputsize);
//  string = naNewStringWithMutableUTF8Buffer(stringbuf, -inputsize, NA_MEMORY_CLEANUP_FREE);
//  inptr = naGetStringUTF8Pointer(inputstring);
//  destptr = stringbuf;
//
//  // Copy all characters and decode them if necessary.
//  for(i=0; i<inputsize; i++){
//    if(inptr[i] == '\\'){
//      if(((inputsize - i) >= 2) && (inptr[i+1] == '\\')){     *destptr++ = '\\'; i += 1; }
//      else if(((inputsize - i) >= 2) && (inptr[i+1] == '(')){ *destptr++ = '(';  i += 1; }
//      else if(((inputsize - i) >= 2) && (inptr[i+1] == ')')){ *destptr++ = ')';  i += 1; }
//      else{
//        *destptr++ = inptr[i];
//      }
//    }else{
//      *destptr++ = inptr[i];
//    }
//  }
//
//  // The string is marked as NULL-Terminated. So we make sure this is the case.
//  *destptr = '\0';
//  // Finally, we shrink the string to its actual size
//  finalsize = destptr - stringbuf;
//  string = naNewStringExtraction(string, 0, finalsize);
//
//  return string;
//}



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
//  NA_DEF SystemChar* naAllocSystemStringFromString(const NAUTF8Char* utf8string, NAUInt size){
//    SystemChar* outstr;
//    NAUInt newsize;
//    if(!size){size = naStrlen(utf8string);}
//    #ifdef UNICODE
//      newsize = MultiByteToWideChar(CP_UTF8, 0, utf8string, size, NULL, 0);
//      outstr = (SystemChar*)naMalloc((newsize + 1 * sizeof(SystemChar)));
//      MultiByteToWideChar(CP_UTF8, 0, utf8string, size, outstr, newsize);
//    #else
//      newsize = size;
//      outstr = (SystemChar*)naMalloc((newsize + 1) * sizeof(SystemChar));
//      naCopyn(outstr, utf8string, newsize);
//    #endif
//    outstr[newsize] = 0;
//    return outstr;
//  }


//  NA_DEF NAString* naNewStringFromSystemString( SystemChar* systemstring){
//    NAInt newsize;
//    NAUTF8Char* stringbuf;
//    #ifdef UNICODE
//      newsize = WideCharToMultiByte(CP_UTF8, 0, systemstring, -1, NULL, 0, NULL, NULL);
//      stringbuf = naMalloc(-newsize);
//      string = naNewStringWithMutableUTF8Buffer(string, stringbuf, -newsize, NA_MEMORY_CLEANUP_FREE);
//      WideCharToMultiByte(CP_UTF8, 0, systemstring, -1, stringbuf, newsize, NULL, NULL);
//    #else
//      newsize = naStrlen(systemstring);
//      stringbuf = naMalloc(-newsize);
//      string = naNewStringWithMutableUTF8Buffer(string, stringbuf, -newsize, NA_MEMORY_CLEANUP_FREE);
//      naCopyn(stringbuf, systemstring, newsize);
//    #endif
//    return string;
//  }
#endif



NA_DEF void naAppendStringString(NAString* originalstring, const NAString* string2){
  NAUInt stringsize1 = naGetStringSize(originalstring);
  NAUInt stringsize2 = naGetStringSize(string2);
  NAInt totalstringsize = stringsize1 + stringsize2;
  if(totalstringsize){
    NAUTF8Char* stringbuf = naMalloc(-totalstringsize);
    if(stringsize1){naCopyn(stringbuf, naGetByteArrayConstPointer(&(originalstring->array)), stringsize1);}
    if(stringsize2){naCopyn(&(stringbuf[stringsize1]), naGetByteArrayConstPointer(&(string2->array)), stringsize2);}
    naClearByteArray(&(originalstring->array));
    naInitByteArrayWithMutableBuffer(&(originalstring->array), stringbuf, -totalstringsize, NA_MEMORY_CLEANUP_FREE);
  }else{
    // The string was empty and remains empty. Nothing to be done here.
  }
}



NA_DEF void naAppendStringChar(NAString* originalstring, NAUTF8Char newchar){
  NAUInt stringsize = naGetStringSize(originalstring);
  NAInt totalstringsize = stringsize + 1;
  NAUTF8Char* stringbuf = naMalloc(-totalstringsize);
  if(stringsize){naCopyn(stringbuf, naGetByteArrayConstPointer(&(originalstring->array)), stringsize);}
  stringbuf[stringsize] = newchar;
  naClearByteArray(&(originalstring->array));
  naInitByteArrayWithMutableBuffer(&(originalstring->array), stringbuf, -totalstringsize, NA_MEMORY_CLEANUP_FREE);
}



NA_DEF void naAppendStringFormat(NAString* originalstring, const NAUTF8Char* format, ...){
  va_list argumentlist;
  va_start(argumentlist, format);
  naAppendStringArguments(originalstring, format, argumentlist);
  va_end(argumentlist);
}


NA_DEF void naAppendStringArguments(NAString* originalstring, const NAUTF8Char* format, va_list argumentlist){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize1;
  NAUInt stringsize2;
  NAInt totalstringsize;
  va_list argumentlist2;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!originalstring){
      naCrash("naAppendStringArguments", "string is Null-Pointer.");
      return;
    }
  #endif
  
  stringsize1 = naGetStringSize(originalstring);
  va_copy(argumentlist2, argumentlist);
  stringsize2 = naVarargStringSize(format, argumentlist2);
  va_end(argumentlist2);
  totalstringsize = stringsize1 + stringsize2;
  if(totalstringsize){
    stringbuf = naMalloc(-totalstringsize);
    if(stringsize1){naCopyn(stringbuf, naGetByteArrayConstPointer(&(originalstring->array)), stringsize1);}
    va_copy(argumentlist2, argumentlist);
    naVsnprintf(&(stringbuf[stringsize1]), stringsize2 + 1, format, argumentlist2);
    va_end(argumentlist2);
    naClearByteArray(&(originalstring->array));
    naInitByteArrayWithMutableBuffer(&(originalstring->array), stringbuf, -totalstringsize, NA_MEMORY_CLEANUP_FREE);
  }else{
    // The string was empty and remains empty. Nothing to be done here.
  }
}




NA_DEF void naDecoupleString(NAString* string){
  if(naIsStringEmpty(string)){return;}
  // When decoupling, make sure, the new string is always null termiated.
  naDecoupleByteArray(&(string->array), NA_TRUE);
}




NA_DEF NAUInt naGetStringSize(const NAString* string){
  return naGetByteArraySize(&(string->array));
}



NA_DEF const NAUTF8Char* naGetStringUTF8Pointer(const NAString* string){
  #ifndef NDEBUG
    if(!string){
      naCrash("naGetStringUTF8Pointer", "string is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(naIsStringEmpty(string)){
    return (const NAUTF8Char*)"";
  }else{
    #ifndef NDEBUG
      if(!naIsByteArrayNullTerminated(&(string->array))){
        naError("naGetStringUTF8Pointer", "string is not Null-Terminated.");
      }
    #endif
    return (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  }
}



NA_DEF const NAUTF8Char* naGetStringChar(const NAString* string, NAInt indx){
  #ifndef NDEBUG
    if(!string){
      naCrash("naGetStringChar", "string is Null-Pointer.");
      return NA_NULL;
    }
    if(indx == NA_INVALID_MEMORY_INDEX)
      naError("naGetStringChar", "Invalid index");
  #endif
  return (const NAUTF8Char*)naGetByteArrayConstByte(&(string->array), indx);
}



NA_DEF NABool naIsStringEmpty(const NAString* string){
  #ifndef NDEBUG
    if(!string){
      naCrash("naIsStringEmpty", "string is Null-Pointer.");
      return NA_TRUE;
    }
  #endif
  return naIsByteArrayEmpty(&(string->array));
}




NA_DEF NAInt naGetStringCharacterPos(const NAString* string, NAUTF8Char ch, NAInt startpos){
  const NAUTF8Char* curchar;
  NAInt stringsize = (NAInt)naGetStringSize(string);
  curchar = (const NAUTF8Char*)naGetByteArrayConstByte(&(string->array), startpos);
  if(startpos < 0){
    while(!(startpos < -stringsize)){
      if(*curchar == ch){return startpos;}
      curchar--;
      startpos--;
    }
  }else{
    while(startpos < stringsize){
      if(*curchar == ch){return startpos;}
      curchar++;
      startpos++;
    }
  }
  // Not found
  return NA_INVALID_MEMORY_INDEX;
}



NA_DEF void naSkipStringWhitespaces(NAString* string){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  const NAUTF8Char* charptr;

  #ifndef NDEBUG
    if(!string){
      naCrash("naSkipStringWhitespaces", "string is Null-Pointer.");
      return;
    }
  #endif
  if(naIsStringEmpty(string)){return;}
  charptr = naGetStringChar(string, 0);
  if(*charptr > ' '){return;} // quick exit
  stringsize = naGetStringSize(string);
  while(stringsize && (*charptr <= ' ')){
    stringsize--;
    charptr++;
  }
  naInitByteArrayExtraction(&(string->array), &(string->array), -(NAInt)stringsize, -1);
}



//NA_DEF NAInt naParseStringLine(NAString* string, NAString* line, NABool skipempty){
//  // Declaration before implementation. Needed for C90.
//  NAUInt stringsize;
//  NAString* emptytest;
//  NAInt numlines = 0;
//  NAInt nextoffset = 0; // the start offset of the line after the current line
//  NABool found;
//  const NAUTF8Char* charptr;
//
//  #ifndef NDEBUG
//    if(!string)
//      naError("naParseStringLine", "string is Null-Pointer.");
//    if(line == string)
//      naError("naParseStringLine", "line and string shall not be the same.");
//  #endif
//
//  if(!line){
//    line = naNewString();
//  }else{
//    naDelete(line);
//    line = naNewString();
//  }
//  // We now are sure that line is empty.
//  
//  if(naIsStringEmpty(string)){return 0;}
//  if(skipempty){
//    emptytest = naNewString();
//  }
//  stringsize = naGetStringSize(string);
//  
//  // This while-loop is here for the skipempty-test.
//  while(1){
//    if(naIsStringEmpty(string)){return 0;}
//    NAUInt linesize = 0;
//    NAInt escapesize;
//    found = NA_FALSE;
//    charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
//
//    while(linesize < stringsize){
//      escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, linesize);
//      if(escapesize){
//        #ifndef NDEBUG
//          if(escapesize < 0)
//            naError("naParseStringLine", "Internal Error: escapesize should not be negative.");
//        #endif
//        linesize += escapesize;
//        charptr += escapesize;
//        // this surely is no whitespace. Yes, an escape sequence may represent
//        // a white space but in the string it is not comprised of whitespace
//        // characters.
//      }else{
//        // Nothing is escaped. Maybe this is a line ending?
//        if(*charptr == '\n'){
//          // unix end
//          found = NA_TRUE;
//          nextoffset = linesize + 1;
//          break;
//        }else if(*charptr == '\r'){
//          if(((linesize + 1) < stringsize) && (*(charptr+1) == '\n')){
//            // windows end
//            found = NA_TRUE;
//            nextoffset = linesize + 2;
//            break;
//          }else{
//            // macintosh end
//            found = NA_TRUE;
//            nextoffset = linesize + 1;
//            break;
//          }
//        }
//      }
//      // Go 1 character forward.
//      linesize++;
//      charptr++;
//    }
//    
//    // If this point has been reached and the string was not empty, we advanced
//    // by 1 line. Only if string was empty, we would be hitting the end of the
//    // string and not increase the line number.
//    if(!naIsStringEmpty(string)){numlines++;}
//
//    if(!found){
//      // String has ended.
//      if(!naIsStringEmpty(string)){line = naNewStringExtraction(string, 0, -1);}
//      naDelete(string);
//      string = naNewString();
//      // line now contains the last characters of string.
//    }else{
//      line = naNewStringExtraction(string, 0, linesize);
//      string = naNewStringExtraction(string, nextoffset, -1);
//      // a line has been read.
//    }
//    // Reaching here, line may or may not contain whitespaces.
//    
//    if(skipempty){
//      // If skipempty is true, we test if the line will be empty when skipping
//      // its whitespaces.
//      if(naIsStringEmpty(line)){
//        // The line itself is already empty. Skip it.
//        if(found){continue;}
//      }else{
//        emptytest = naNewStringExtraction(line, 0, -1);
//        naSkipStringWhitespaces(&emptytest);
//        if(naIsStringEmpty(&emptytest)){
//          // If there are indeed just whitespaces, clear the line.
//          naDelete(line);
//          line = naNewString();
//          // emptytest will already have been cleared.
//          // Go on with the next line if the string is not finished yet.
//          if(found){continue;}
//        }else{
//          // Line has content. Clear the emptytest
//          naDestructString(&emptytest);
//        }
//      }
//    }
//    break;
//  }
//  return numlines;
//}



NA_DEF NAString* naParseStringToken(NAString* string){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUInt tokensize;
  const NAUTF8Char* charptr;
  NAString* token;

  #ifndef NDEBUG
    if(!string){
      naCrash("naParseStringToken", "string is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  
  if(naIsStringEmpty(string)){return naNewString();}
  #ifndef NDEBUG
    if(*((const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array))) <= ' '){
      naError("naParseStringToken", "string starts with a whitespace. This function expects the string to start at a non-whitespace. Empty string will be returned.");
      return NA_NULL;
    }
  #endif

  stringsize = naGetStringSize(string);
  tokensize = 0;
  charptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    if(*charptr <= ' '){
      // Whitespace found.
      token = naNewStringExtraction(string, 0, tokensize);
      naInitByteArrayExtraction(&(string->array), &(string->array), tokensize + 1, -1);
      naSkipStringWhitespaces(string);
      return token;
    }
    // Go one character forward.
    tokensize++;
    charptr++;
  }
  
  // Reaching here, no whitespace was found till the end of string.
  // String has ended. The token is the whole string.
  token = naNewStringExtraction(string, 0, -1);
  naEmptyByteArray(&(string->array));
  return token;
}



NA_DEF NAString* naParseStringTokenWithDelimiter(NAString* string, NAUTF8Char delimiter){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUInt tokensize;
  const NAUTF8Char* charptr;
  NAString* token;

  #ifndef NDEBUG
    if(!string){
      naCrash("naParseStringTokenWithDelimiter", "string is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  
  if(naIsStringEmpty(string)){return naNewString();}
  // Note that we do not check if the first character is equal to the delimiter
  // as quite frequently, this can occur. For example when parsing csv-files.

  stringsize = naGetStringSize(string);
  tokensize = 0;
  charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    if(*charptr == delimiter){
      // delimiter found.
      token = naNewStringExtraction(string, 0, tokensize);
      naInitByteArrayExtraction(&(string->array), &(string->array), tokensize + 1, -1);
      return token;
    }
    tokensize++;
    charptr++;
  }

  // Reaching here, no delimiter was found till the end of string.
  // String has ended. The token is the whole string.
  token = naNewStringExtraction(string, 0, -1);
  naEmptyByteArray(&(string->array));
  return token;
}



NA_DEF NAString* naParseStringPathComponent(NAString* string){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUInt tokensize;
  const NAUTF8Char* charptr;
  NAString* token;

  #ifndef NDEBUG
    if(!string){
      naCrash("naParseStringTokenWithDelimiter", "string is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  
  if(naIsStringEmpty(string)){return naNewString();}
  // Note that we do not check if the first character is equal to a path
  // delimiter as quite frequently, this can occur.
  
  stringsize = naGetStringSize(string);
  tokensize = 0;
  charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    if((*charptr == NA_PATH_DELIMITER_UNIX) || *charptr == NA_PATH_DELIMITER_WIN){
      // delimiter found.
      token = naNewStringExtraction(string, 0, tokensize);
      naInitByteArrayExtraction(&(string->array), &(string->array), tokensize + 1, -1);
      return token;
    }
    tokensize++;
    charptr++;
  }

  // Reaching here, no delimiter was found till the end of string.
  // String has ended. The token is the whole string.
  token = naNewStringExtraction(string, 0, -1);
  naEmptyByteArray(&(string->array));
  return token;
}






NA_DEF NAUInt naParseUTF8StringForDecimalUnsignedInteger(const NAUTF8Char* string, uint64* retint, NAUInt maxcharcount, uint64 max){
  NAUInt bytesused;
  uint64 prevval;
  #ifndef NDEBUG
    if(!retint)
      {naCrash("naParseUTF8StringForDecimalUnsignedInteger", "retint is Null-Pointer"); return 0;}
    if(!maxcharcount)
      naError("naParseUTF8StringForDecimalUnsignedInteger", "maxcharcount is 0");
  #endif
  bytesused = 0;
  prevval = 0LL;
  *retint = 0LL;
  while(bytesused < maxcharcount){
    if((*string < '0') || (*string > '9')){break;}
    *retint = *retint * 10LL + (*string - '0');
    #ifndef NDEBUG
      if(*retint < prevval)
        naError("naParseUTF8StringForDecimalUnsignedInteger", "The value overflowed 64 bit integer space.");
    #endif
    if((*retint < prevval) || (*retint > max)){
      *retint = max;
    }
    prevval = *retint;
    bytesused++;
    string++;
  }
  return bytesused;
}



NA_DEF NAUInt naParseUTF8StringForDecimalSignedInteger( const NAUTF8Char* string, int64* retint, NAUInt maxcharcount, int64 min, int64 max){
  int64 sign = 1LL;
  NAUInt bytesused = 0;
  uint64 limit = max;
  uint64 intvalue;

  // Check for a potential sign at the first character
  if(*string == '+'){
    bytesused = 1;
    maxcharcount--;
    string++;
  }else if(*string == '-'){
    sign = -1LL;
    limit = -min;
    bytesused = 1;
    maxcharcount--;
    string++;
  }

  bytesused += naParseUTF8StringForDecimalUnsignedInteger(string, &intvalue, maxcharcount, limit);
  *retint = (sign * intvalue);
  return bytesused;
}




NA_DEF int8 naParseStringInt8(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT8_MIN, NA_INT8_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (int8)intvalue;
}


NA_DEF int16 naParseStringInt16(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT16_MIN, NA_INT16_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (int16)intvalue;
}


NA_DEF int32 naParseStringInt32(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT32_MIN, NA_INT32_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (int32)intvalue;
}


NA_DEF int64 naParseStringInt64(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT64_MIN, NA_INT64_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (int64)intvalue;
}


NA_DEF uint8 naParseStringUInt8(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT8_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (uint8)intvalue;
}


NA_DEF uint16 naParseStringUInt16(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT16_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (uint16)intvalue;
}


NA_DEF uint32 naParseStringUInt32(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT32_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (uint32)intvalue;
}


NA_DEF uint64 naParseStringUInt64(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT64_MAX);
  if(skipdelimiter && (size > bytesused)){
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused + 1, -1);
  }else{
    naInitByteArrayExtraction(&(string->array), &(string->array), bytesused, -1);
  }
  return (uint64)intvalue;
}





NA_DEF int8 naGetStringInt8(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT8_MIN, NA_INT8_MAX);
  return (int8)intvalue;
}


NA_DEF int16 naGetStringInt16(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT16_MIN, NA_INT16_MAX);
  return (int16)intvalue;
}


NA_DEF int32 naGetStringInt32(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT32_MIN, NA_INT32_MAX);
  return (int32)intvalue;
}


NA_DEF int64 naGetStringInt64(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT64_MIN, NA_INT64_MAX);
  return (int64)intvalue;
}


NA_DEF uint8 naGetStringUInt8(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT8_MAX);
  return (uint8)intvalue;
}


NA_DEF uint16 naGetStringUInt16(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT16_MAX);
  return (uint16)intvalue;
}


NA_DEF uint32 naGetStringUInt32(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT32_MAX);
  return (uint32)intvalue;
}


NA_DEF uint64 naGetStringUInt64(const NAString* string){
  const NAUTF8Char* curptr = (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT64_MAX);
  return (uint64)intvalue;
}









NA_DEF NABool naEqualStringToUTF8CStringLiteral(const NAString* string, const NAUTF8Char* ptr){
  NAUInt stringsize = naGetStringSize(string);
  NAUInt ptrsize = naStrlen(ptr);
  if(stringsize != ptrsize){return NA_FALSE;}
  return !memcmp(naGetByteArrayConstPointer(&(string->array)), ptr, (size_t)stringsize);
}



NA_DEF NABool naEqualStringToString(     const NAString* string1,
                                  const NAString* string2){
  NAUInt stringsize1 = naGetStringSize(string1);
  NAUInt stringsize2 = naGetStringSize(string2);
  if(stringsize1 != stringsize2){return NA_FALSE;}
  if(stringsize1 == 0){return NA_TRUE;}
  return !memcmp(naGetByteArrayConstPointer(&(string1->array)), naGetByteArrayConstPointer(&(string2->array)), (size_t)stringsize1);
}




NA_DEF NABool naEqualUTF8CStringLiteralsCaseInsensitive( const NAUTF8Char* string1,
                                                  const NAUTF8Char* string2){
  // declaration before implementaiton. Needed for C90
  NAUInt i;
  const NAUTF8Char* curchar1ptr;
  const NAUTF8Char* curchar2ptr;

  NAUInt stringsize1 = naStrlen(string1);
  NAUInt stringsize2 = naStrlen(string2);
  if(stringsize1 != stringsize2){return NA_FALSE;}
  if(stringsize1 == 0){return NA_TRUE;}
  curchar1ptr = string1;
  curchar2ptr = string2;
  for(i=0; i<stringsize1; i++){
    NAUTF8Char curchar1;
    NAUTF8Char curchar2;
    if(isalpha(*curchar1ptr)){curchar1 = tolower(*curchar1ptr);}else{curchar1 = *curchar1ptr;}
    if(isalpha(*curchar2ptr)){curchar2 = tolower(*curchar2ptr);}else{curchar2 = *curchar2ptr;}
    if(curchar1 != curchar2){return NA_FALSE;}
    curchar1ptr++;
    curchar2ptr++;
  }
  return NA_TRUE;
}



NA_DEF NAUInt naStrlen(const NAUTF8Char* str){
  return (NAUInt)strlen((const char*)str);
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
