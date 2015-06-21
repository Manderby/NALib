
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NABinaryData.h"
#include "NAString.h"
#include "NAURL.h"
#include <ctype.h>
#include <stdarg.h>


#define NA_STRING_NULL_TERMINATED           0x0001



//NA_DEF NAStructInfo naGetStringStructInfo(){
//  static NAStructInfo stringstructinfo = {
//    sizeof(NAString),
//    naCreateString,
//    naClearString
//  };
//  return stringstructinfo;
//}


NA_DEF NAString* naCreateString(NAString* string){
  string = naAllocNALibStruct(string, NAString);
  naCreateByteArray(&(string->array));
  return string;
}


NA_DEF NAString* naCreateStringWithUTF8CStringLiteral(NAString* string, const NAUTF8Char* ptr){
  // Declaration before implementation. Needed for C90.
  NAUInt size;
  #ifndef NDEBUG
    if(!ptr){
      naCrash("naCreateStringWithUTF8CStringLiteral", "ptr is Null-Pointer");
      return NA_NULL;
    }
  #endif
  size = naStrlen(ptr);
  if(!size){return naCreateString(string);}
  string = naAllocNALibStruct(string, NAString);
  naCreateByteArrayWithConstBuffer(&(string->array), ptr, size);
  // C-Strings are always expected to be already Null-terminated, meaning: The
  // Byte with index [size] must be binary zero. As we are not copying but just
  // referencing the pointer, we can safely use the array without this byte
  // and still be able to say: We are null-terminated!
  string->flags = NA_STRING_NULL_TERMINATED;
  return string;
}


NA_DEF NAString* naCreateStringWithMutableUTF8Buffer(NAString* string, NAUTF8Char* buffer, NAInt size, NABool takeownership){
  string = naAllocNALibStruct(string, NAString);
  if(size < 0){
    naCreateByteArrayWithMutableBuffer(&(string->array), buffer, -size, takeownership);
    string->flags = NA_STRING_NULL_TERMINATED;
  }else{
    naCreateByteArrayWithMutableBuffer(&(string->array), buffer, size, takeownership);
    string->flags = 0;
  }
  return string;
}


NA_DEF NAString* naCreateStringWithFormat(NAString* string,
                           const NAUTF8Char* format,
                                             ...){
  va_list argumentlist;
  va_start(argumentlist, format);
  string = naCreateStringWithArguments(string, format, argumentlist);
  va_end(argumentlist);
  return string;
}


NA_DEF NAString* naCreateStringWithArguments(NAString* string,
                              const NAUTF8Char* format,
                                        va_list argumentlist){
  // Declaration before implementation. Needed for C90.
  NAUInt stringlen;
  va_list argumentlist2;
  va_list argumentlist3;
  va_copy(argumentlist2, argumentlist);
  va_copy(argumentlist3, argumentlist);
  stringlen = naVarargStringSize(format, argumentlist2);
  if(stringlen){
    NAUTF8Char* stringbuf = naAllocate(-(NAInt)stringlen);
    naVsnprintf(stringbuf, stringlen + 1, format, argumentlist3);
    string = naCreateStringWithMutableUTF8Buffer(string, stringbuf, -(NAInt)stringlen, NA_TRUE);
  }else{
    string = naCreateString(string);
  }
  va_end(argumentlist2);
  va_end(argumentlist3);
  return string;
}


NA_DEF NAString* naCreateStringExtraction( NAString* deststring,
                              const NAString* srcstring,
                                        NAInt offset,
                                        NAInt size){
  // Declaration before implementation. Needed for C90.
  const NAByte* srclastchar;
  const NAByte* destlastchar;

  #ifndef NDEBUG
    if(!srcstring){
      naCrash("naCreateStringExtraction", "srcstring is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  if(srcstring != deststring){
    deststring = naCreateString(deststring);
  }
  // Now, deststring is either the same as srcstring or empty.

  // If src is empty, return an empty string. But check if this is intentional
  // or an error:
  if(naIsStringEmpty(srcstring)){
//    #ifndef NDEBUG
//      if(offset || size)
//        naError("naCreateStringExtraction", "Invalid string extraction of empty string.");
//    #endif
    return deststring;
  }
  
  // In case src == dest, store all necessary data.
  srclastchar = naGetByteArrayConstByte(&(srcstring->array), -1);
  // The previous line is valid because we know that srcstring is not empty.
  
  // Extract the string
  naCreateByteArrayExtraction(&(deststring->array), &(srcstring->array), offset, size);
  if(!naIsByteArrayEmpty(&(deststring->array))){
    // Set the flags of the dest string
    deststring->flags = srcstring->flags;
    destlastchar = naGetByteArrayMutableByte(&(deststring->array), -1);
    if(srclastchar != destlastchar){
      // Null-termination can not be guaranteed anymore.
      deststring->flags &= ~NA_STRING_NULL_TERMINATED;
    }
  }
  
  return deststring;
}



NA_DEF NAString* naCreateStringWithBasenameOfFilename(NAString* deststring, const NAString* filename){
  NAInt dotpos = naGetStringCharacterRPos(filename, NA_SUFFIX_DELIMITER);
  if(dotpos){
    return naCreateStringExtraction(deststring, filename, 0, dotpos-1);
  }else{
    return naCreateString(deststring);
  }
}
NA_DEF NAString* naCreateStringWithSuffixOfFilename(NAString* deststring, const NAString* filename){
  NAInt dotpos = naGetStringCharacterRPos(filename, NA_SUFFIX_DELIMITER);
  if(dotpos){
    return naCreateStringExtraction(deststring, filename, dotpos+1, -1);
  }else{
    return naCreateString(deststring);
  }
}



NA_DEF NAString* naCreateStringXMLEncoded( NAString* deststring,
                              const NAString* inputstring){
  // Declaration before implementation. Needed for C90.
  NAUInt i;
  NAUInt inputsize;
  NAInt destsize;
  const NAUTF8Char* inptr;
  NAUTF8Char* destptr;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!inputstring){
      naCrash("naCreateStringXMLEncoded", "input string is Null-Pointer.");
      return NA_NULL;
    }
    if(deststring == inputstring)
      naError("naCreateStringXMLEncoded", "The two parameters are the same.");
  #endif
  inputsize = naGetStringSize(inputstring);
  if(!inputsize){return naCreateString(deststring);}
  
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
      naError("naCreateStringXMLEncoded", "encoded size invalid. String too long?");
  #endif
  // Create the string with the required length
  stringbuf = naAllocate(-destsize);
  deststring = naCreateStringWithMutableUTF8Buffer(deststring, stringbuf, -destsize, NA_TRUE);
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

  return deststring;
}




NA_DEF NAString* naCreateStringXMLDecoded( NAString* deststring,
                              const NAString* inputstring){
  // Declaration before implementation. Needed for C90.
  NAInt inputsize;
  const NAUTF8Char* inptr;
  NAUTF8Char* destptr;
  NAInt i;
  NAInt finalsize;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!inputstring){
      naCrash("naCreateStringXMLDecoded", "input string is Null-Pointer.");
      return NA_NULL;
    }
    if(deststring == inputstring)
      naError("naCreateStringXMLDecoded", "The two parameters are the same.");
  #endif

  inputsize = naGetStringSize(inputstring);
  if(!inputsize){return naCreateString(deststring);}

  // Create a string with sufficient characters. As XML entities are always
  // longer than their decoded character, we just use the same size.
  stringbuf = naAllocate(-inputsize);
  deststring = naCreateStringWithMutableUTF8Buffer(deststring, stringbuf, -inputsize, NA_TRUE);
  inptr = naGetStringUTF8Pointer(inputstring);
  destptr = stringbuf;

  // Copy all characters and decode them if necessary.
  for(i=0; i<inputsize; i++){
    if(inptr[i] == '&'){
      if(((inputsize - i) >= 5) && (inptr[i+1] == 'a') && (inptr[i+2] == 'm') && (inptr[i+3] == 'p') && (inptr[i+4] == ';')){ *destptr++ = '&'; i += 4; }
      else if(((inputsize - i) >= 4) && (inptr[i+1] == 'l') && (inptr[i+2] == 't') && (inptr[i+3] == ';')){ *destptr++ = '<'; i += 3; }
      else if(((inputsize - i) >= 4) && (inptr[i+1] == 'g') && (inptr[i+2] == 't') && (inptr[i+3] == ';')){ *destptr++ = '>'; i += 3; }
      else if(((inputsize - i) >= 6) && (inptr[i+1] == 'q') && (inptr[i+2] == 'u') && (inptr[i+3] == 'o') && (inptr[i+4] == 't') && (inptr[i+5] == ';')){ *destptr++ = '\"'; i += 5; }
      else if(((inputsize - i) >= 6) && (inptr[i+1] == 'a') && (inptr[i+2] == 'p') && (inptr[i+3] == 'o') && (inptr[i+4] == 's') && (inptr[i+5] == ';')){ *destptr++ = '\''; i += 5; }
      else{
        *destptr++ = inptr[i];
      }
    }else{
      *destptr++ = inptr[i];
    }
  }

  // todo: numeric entities.

  // The string is marked as NULL-Terminated. So we make sure this is the case.
  *destptr = '\0';
  // Finally, we shrink the string to its actual size
  finalsize = destptr - stringbuf;
  naCreateStringExtraction(deststring, deststring, 0, finalsize);

  return deststring;
}



NA_DEF NAString* naCreateStringEPSEncoded( NAString* deststring,
                              const NAString* inputstring){
  // Declaration before implementation. Needed for C90.
  NAUInt i;
  NAUInt inputsize;
  NAInt destsize;
  const NAUTF8Char* inptr;
  NAUTF8Char* destptr;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!inputstring){
      naCrash("naCreateStringEPSEncoded", "input string is Null-Pointer.");
      return NA_NULL;
    }
    if(deststring == inputstring)
      naError("naCreateStringEPSEncoded", "The two parameters are the same.");
  #endif
  inputsize = naGetStringSize(inputstring);
  if(!inputsize){return naCreateString(deststring);}
  
  // Count the required number of utf8 characters.
  destsize = 0;
  inptr = naGetStringUTF8Pointer(inputstring);
  for(i=0; i<inputsize; i++){
    switch(*inptr){
    case '\\': destsize += 2; break;
    case '(': destsize += 2; break;
    case ')': destsize += 2; break;
    default: destsize += 1; break;
    }
    inptr++;
  }

  #ifndef NDEBUG
    if(destsize <= 0)
      naError("naCreateStringEPSEncoded", "encoded size invalid. String too long?");
  #endif
  // Create the string with the required length
  stringbuf = naAllocate(-destsize);
  deststring = naCreateStringWithMutableUTF8Buffer(deststring, stringbuf, -destsize, NA_TRUE);
  inptr = naGetStringUTF8Pointer(inputstring);
  destptr = stringbuf;

  // Copy all characters and encode them if necessary.
  for(i=0; i<inputsize; i++){
    switch(*inptr){
    case '\\': *destptr++ = '\\'; *destptr++ = '\\'; break;
    case '(':  *destptr++ = '\\'; *destptr++ = '(';  break;
    case ')':  *destptr++ = '\\'; *destptr++ = ')';  break;
    default: *destptr++ = *inptr; break;
    }
    inptr++;
  }

  return deststring;
}


NA_DEF NAString* naCreateStringEPSDecoded( NAString* deststring,
                              const NAString* inputstring){

  // Declaration before implementation. Needed for C90.
  NAInt i;
  NAInt inputsize;
  const NAUTF8Char* inptr;
  NAUTF8Char* destptr;
  NAInt finalsize;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!inputstring){
      naCrash("naCreateStringEPSDecoded", "input string is Null-Pointer.");
      return NA_NULL;
    }
    if(deststring == inputstring)
      naError("naCreateStringEPSDecoded", "The two parameters are the same.");
  #endif

  inputsize = naGetStringSize(inputstring);
  if(!inputsize){return naCreateString(deststring);}

  // Create a string with sufficient characters. As EPS entities are always
  // longer than their decoded character, we just use the same size.
  stringbuf = naAllocate(-inputsize);
  deststring = naCreateStringWithMutableUTF8Buffer(deststring, stringbuf, -inputsize, NA_TRUE);
  inptr = naGetStringUTF8Pointer(inputstring);
  destptr = stringbuf;

  // Copy all characters and decode them if necessary.
  for(i=0; i<inputsize; i++){
    if(inptr[i] == '\\'){
      if(((inputsize - i) >= 2) && (inptr[i+1] == '\\')){     *destptr++ = '\\'; i += 1; }
      else if(((inputsize - i) >= 2) && (inptr[i+1] == '(')){ *destptr++ = '(';  i += 1; }
      else if(((inputsize - i) >= 2) && (inptr[i+1] == ')')){ *destptr++ = ')';  i += 1; }
      else{
        *destptr++ = inptr[i];
      }
    }else{
      *destptr++ = inptr[i];
    }
  }

  // The string is marked as NULL-Terminated. So we make sure this is the case.
  *destptr = '\0';
  // Finally, we shrink the string to its actual size
  finalsize = destptr - stringbuf;
  naCreateStringExtraction(deststring, deststring, 0, finalsize);

  return deststring;
}



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  NA_DEF SystemChar* naCreateSystemStringFromString(const NAUTF8Char* utf8string, NAUInt size){
    SystemChar* outstr;
    NAUInt newsize;
    if(!size){size = naStrlen(utf8string);}
    #ifdef UNICODE
      newsize = MultiByteToWideChar(CP_UTF8, 0, utf8string, size, NULL, 0);
      outstr = (SystemChar*)naAllocate(sizeof(SystemChar) * (newsize + 1));
      MultiByteToWideChar(CP_UTF8, 0, utf8string, size, outstr, newsize);
    #else
      newsize = size;
      outstr = (SystemChar*)naAllocate(sizeof(SystemChar) * (newsize + 1));
      naCpyn(outstr, utf8string, newsize);
    #endif
    outstr[newsize] = 0;
    return outstr;
  }


  NA_DEF NAString* naCreateStringFromSystemString( NAString* string, SystemChar* systemstring){
    NAInt newsize;
    NAUTF8Char* stringbuf;
    #ifdef UNICODE
      newsize = WideCharToMultiByte(CP_UTF8, 0, systemstring, -1, NULL, 0, NULL, NULL);
      stringbuf = naAllocate(-newsize);
      string = naCreateStringWithMutableUTF8Buffer(string, stringbuf, -newsize, NA_TRUE);
      WideCharToMultiByte(CP_UTF8, 0, systemstring, -1, stringbuf, newsize, NULL, NULL);
    #else
      newsize = naStrlen(systemstring);
      stringbuf = naAllocate(-newsize);
      string = naCreateStringWithMutableUTF8Buffer(string, stringbuf, -newsize, NA_TRUE);
      naCpyn(stringbuf, systemstring, newsize);
    #endif
    return string;
  }
#endif


NA_DEF void naClearString(NAString* string){
  if(!naIsStringEmpty(string)){naClearByteArray(&(string->array));}
}



NA_DEF void naDestroyString(NAString* string){
  naClearString(string);
  free(string);
}




NA_DEF NAString* naCreateStringWithStringAppendingString(    NAString* string,
                            const NAString* string1,
                            const NAString* string2){
  NAUInt stringsize1 = naGetStringSize(string1);
  NAUInt stringsize2 = naGetStringSize(string2);
  NAInt totalstringsize = stringsize1 + stringsize2;
  NAUTF8Char* stringbuf = naAllocate(-totalstringsize);
  if(stringsize1){naCpyn(stringbuf, naGetStringUTF8Pointer(string1), stringsize1);}
  if(stringsize2){naCpyn(&(stringbuf[stringsize1]), naGetStringUTF8Pointer(string2), stringsize2);}
  string = naCreateStringWithMutableUTF8Buffer(string, stringbuf, -totalstringsize, NA_TRUE);
  return string;
}


NA_DEF NAString* naCreateStringWithStringAppendingChar(      NAString* string,
                                 const NAString* originalstring,
                                 NAUTF8Char newchar){
  NAUInt stringsize = naGetStringSize(originalstring);
  NAInt totalstringsize = stringsize + 1;
  NAUTF8Char* stringbuf = naAllocate(-totalstringsize);
  naCpyn(stringbuf, naGetStringUTF8Pointer(originalstring), stringsize);
  stringbuf[stringsize] = newchar;
  string = naCreateStringWithMutableUTF8Buffer(string, stringbuf, -totalstringsize, NA_TRUE);
  return string;
}

NA_DEF NAString* naCreateStringWithStringAppendingFormat(    NAString* string,
                          const NAString* originalstring,
                          const NAUTF8Char* format,
                                            ...){
  va_list argumentlist;
  va_start(argumentlist, format);
  string = naCreateStringWithStringAppendingArguments(string, originalstring, format, argumentlist);
  va_end(argumentlist);
  return string;
}


NA_DEF NAString* naCreateStringWithStringAppendingArguments( NAString* string,
                          const NAString* originalstring,
                          const NAUTF8Char* format,
                                    va_list argumentlist){
  NA_UNUSED(originalstring);
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize1 = naGetStringSize(string);
  NAUInt stringsize2;
  NAInt totalstringsize;
  va_list argumentlist2;
  va_list argumentlist3;
  NAUTF8Char* stringbuf;

  #ifndef NDEBUG
    if(!string){
      naCrash("naCreateStringWithStringAppendingArguments", "string is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  va_copy(argumentlist2, argumentlist);
  va_copy(argumentlist3, argumentlist);
  stringsize2 = naVarargStringSize(format, argumentlist2);
  totalstringsize = stringsize1 + stringsize2;
  stringbuf = naAllocate(-totalstringsize);
  if(stringsize1){naCpyn(stringbuf, naGetStringUTF8Pointer(string), stringsize1);}
  naVsnprintf(&(stringbuf[stringsize1]), stringsize2 + 1, format, argumentlist3);
  va_end(argumentlist2);
  va_end(argumentlist3);
  string = naCreateStringWithMutableUTF8Buffer(string, stringbuf, -totalstringsize, NA_TRUE);
  return string;
}




NA_DEF void naDecoupleString(NAString* string){
  if(naIsStringEmpty(string)){return;}
  // When decoupling, make sure, the new string is always null termiated.
  naDecoupleByteArray(&(string->array), NA_TRUE);
  string->flags |= NA_STRING_NULL_TERMINATED;
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
      if(!(string->flags & NA_STRING_NULL_TERMINATED)){
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




NA_DEF NAInt naGetStringCharacterPos(  const NAString* string, NAUTF8Char ch){
  const NAUTF8Char* curchar;
  NAUInt i;
  NAUInt stringsize = naGetStringSize(string);
  curchar = naGetStringUTF8Pointer(string);
  for(i=0; i<stringsize; i++){
    if(*curchar == ch){return i;}
    curchar++;
  }
  // Not found
  return -1;
}
NA_DEF NAInt naGetStringCharacterRPos( const NAString* string, NAUTF8Char ch){
  const NAUTF8Char* curchar;
  NAInt i;
  NAInt negativestringsize = (-1 - naGetStringSize(string));
  curchar = naGetStringChar(string, -1);
  for(i=-1; i>negativestringsize; i--){
    if(*curchar == ch){return i;}
    curchar--;
  }
  // Not found
  return 0;
}



NA_DEF NAInt naGetStringCharacterEscapeSizeTowardsTrailing(NAString* string, NAInt offset){
  // Note: The flags can be combined with the binary OR operator. This means
  // that multiple flags can be active. Therefore all escape methods need to
  // be checked one by one.
  
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUTF8Char char0;
  NAUTF8Char char1;
  NAUTF8Char char2;
  #ifndef NDEBUG
    if(!string){
      naCrash("naGetStringCharacterEscapeSizeTowardsTrailing", "string is Null-Pointer.");
      return 0;
    }
  #endif
  
  stringsize = naGetStringSize(string);
  if(offset < 0){offset += stringsize;}
  #ifndef NDEBUG
    if(offset < 0)
      naError("naGetStringCharacterEscapeSizeTowardsTrailing", "offset underflows string.");
    if(offset >= (NAInt)stringsize)
      naError("naGetStringCharacterEscapeSizeTowardsTrailing", "offset overflows string.");
  #endif

  if((NAUInt)offset >= stringsize - 1){return 0;}
  
  char0 = *naGetStringChar(string, offset);
  char1 = *naGetStringChar(string, offset + 1);
  char2 = '\0';
  if((NAUInt)offset < stringsize - 2){char2 = *naGetStringChar(string, offset + 2);}

  if(string->flags & NA_STRING_IS_INSIDE_DOUBLE_QUOTES){
    if(string->flags & NA_STRING_ESCAPE_DOUBLE_QUOTE_DOUBLING_WITHIN_DOUBLE_QUOTES){
      // Escaping double qoutes "
      if((char0 == '\"') && (char1 == '\"')){return 1;}
    }
  }else{
    if(string->flags & NA_STRING_ENCAPSULATE_DOUBLE_QUOTES){
      if(char0 == '\"'){
        // A double quoted string has started. Set the flag temporarily
        NAInt escapesize;
        NAInt curpos = 1;
        string->flags |= NA_STRING_IS_INSIDE_DOUBLE_QUOTES;
        while((NAUInt)(offset + curpos) < stringsize){
          escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, offset + curpos);
          if(escapesize){
            // Something has been escaped.
            curpos += escapesize;
          }else{
            // The current character has not been escaped. Maybe it is the end
            // quote?
            if(*naGetStringChar(string, offset + curpos) == '\"'){break;}
          }
          curpos++;
        }
        // Reset the flag
        string->flags &= ~NA_STRING_IS_INSIDE_DOUBLE_QUOTES;
        return curpos;
      }
    }
  }
  

  if(string->flags & NA_STRING_ESCAPE_BACKSLASH){
    if(char0 == '\\'){
      // Escaping the windows line ending
      if((char1 == '\r') && (char2 == '\n')){return 2;}
      // Escaping any 7-bit ASCII character. But not binary Null! Note: Do not
      // use something like (char1 < 128) because a char might be signed by
      // default. Use the isascii function of the ctype standard library.
      if(char1 && isascii(char1)){return 1;}
    }
  }

  return 0;
}


NA_DEF NAInt naGetStringCharacterEscapeSizeTowardsLeading(NAString* string, NAInt offset){
  // Note: The flags can be combined with the binary OR operator. This means
  // that multiple flags can be active. Therefore all escape methods need to
  // be checked one by one.
  
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUTF8Char char0;
  NAUTF8Char char1;
  NAUTF8Char char2;
  #ifndef NDEBUG
    if(!string){
      naCrash("naGetStringCharacterEscapeSizeTowardsLeading", "string is Null-Pointer.");
      return 0;
    }
  #endif
  
  stringsize = naGetStringSize(string);
  if(offset < 0){offset += stringsize;}
  #ifndef NDEBUG
    if(offset < 0)
      naError("naGetStringCharacterEscapeSizeTowardsLeading", "offset underflows string.");
    if(offset >= (NAInt)stringsize)
      naError("naGetStringCharacterEscapeSizeTowardsLeading", "offset overflows string.");
  #endif

  if(offset < 1){return 0;}
  
  char0 = *naGetStringChar(string, offset);
  char1 = *naGetStringChar(string, offset - 1);
  char2 = '\0';
  if(offset >= 2){char2 = *naGetStringChar(string, offset - 2);}
  
  if(string->flags & NA_STRING_IS_INSIDE_DOUBLE_QUOTES){
    if(string->flags & NA_STRING_ESCAPE_DOUBLE_QUOTE_DOUBLING_WITHIN_DOUBLE_QUOTES){
      if(offset >= 1){
        // Escaping double quotes "
        if((char1 == '\"') && (char0 == '\"')){return -1;}
      }
    }
  }else{
    if(string->flags & NA_STRING_ENCAPSULATE_DOUBLE_QUOTES){
      if(char0 == '\"'){
        // A double quoted string has ended. Set the flag temporarily
        NAInt escapesize;
        NAInt curpos = -1;
        string->flags |= NA_STRING_IS_INSIDE_DOUBLE_QUOTES;
        while((offset + curpos) >= 0){
          escapesize = naGetStringCharacterEscapeSizeTowardsLeading(string, offset + curpos);
          if(escapesize){
            // Something has been escaped.
            curpos += escapesize;
          }else{
            // The current character has not been escaped. Maybe it is the start
            // quote?
            if(*naGetStringChar(string, offset + curpos) == '\"'){break;}
          }
          curpos--;
        }
        // Reset the flag
        string->flags &= ~NA_STRING_IS_INSIDE_DOUBLE_QUOTES;
        return curpos;
      }
    }
  }

  if(string->flags & NA_STRING_ESCAPE_BACKSLASH){
    // Every 7-bit ASCII character can be escaped except binary Null. Note:
    // Do not use something like (char0 < 128) because a char might be signed
    // by default. Use the isascii function of the ctype standard library.
    if(char0 && isascii(char0)){
      // Escaping the windows line ending
      if((char2 == '\\') && (char1 == '\r') && (char0 == '\n')){return -2;}
      // Escaping everything else
      if(char1 == '\\'){return -1;}
    }
  }

  return 0;
}




NA_DEF void naSkipStringWhitespaces(NAString* string){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  const NAUTF8Char* charptr;

  #ifndef NDEBUG
    if(!string){
      naCrash("naSkipStringWhitespaces", "Parameter is Null-Pointer.");
      return;
    }
  #endif
  if(naIsStringEmpty(string)){return;}
  // Note: There is no check for escape characters as so far, nobody ever used
  // a whitespace as an escape character. Therefore stripping whitespaces from
  // leading to trailing is safe without checking for escape characters.
  stringsize = naGetStringSize(string);
  charptr = naGetStringChar(string, 0);
  while(stringsize && (*charptr <= ' ')){
    stringsize--;
    charptr++;
  }
  naCreateByteArrayExtraction(&(string->array), &(string->array), -(NAInt)stringsize, -1);
}



NA_DEF NAInt naParseStringLine(NAString* string, NAString* line, NABool skipempty){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAString emptytest;
  NAInt numlines = 0;
  NAInt nextoffset = 0; // the start offset of the line after the current line
  NABool found;
  const NAUTF8Char* charptr;

  #ifndef NDEBUG
    if(!string)
      naError("naParseStringLine", "string is Null-Pointer.");
    if(line == string)
      naError("naParseStringLine", "line and string shall not be the same.");
  #endif

  if(!line){
    line = naCreateString(NA_NULL);
  }else{
    naClearString(line);
    naCreateString(line);
  }
  // We now are sure that line is empty.
  
  if(naIsStringEmpty(string)){return 0;}
  if(skipempty){
    naCreateString(&emptytest);
  }
  stringsize = naGetStringSize(string);
  
  // This while-loop is here for the skipempty-test.
  while(1){
    if(naIsStringEmpty(string)){return 0;}
    NAUInt linesize = 0;
    NAInt escapesize;
    found = NA_FALSE;
    charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

    while(linesize < stringsize){
      escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, linesize);
      if(escapesize){
        #ifndef NDEBUG
          if(escapesize < 0)
            naError("naParseStringLine", "Internal Error: escapesize should not be negative.");
        #endif
        linesize += escapesize;
        charptr += escapesize;
        // this surely is no whitespace. Yes, an escape sequence may represent
        // a white space but in the string it is not comprised of whitespace
        // characters.
      }else{
        // Nothing is escaped. Maybe this is a line ending?
        if(*charptr == '\n'){
          // unix end
          found = NA_TRUE;
          nextoffset = linesize + 1;
          break;
        }else if(*charptr == '\r'){
          if(((linesize + 1) < stringsize) && (*(charptr+1) == '\n')){
            // windows end
            found = NA_TRUE;
            nextoffset = linesize + 2;
            break;
          }else{
            // macintosh end
            found = NA_TRUE;
            nextoffset = linesize + 1;
            break;
          }
        }
      }
      // Go 1 character forward.
      linesize++;
      charptr++;
    }
    
    // If this point has been reached and the string was not empty, we advanced
    // by 1 line. Only if string was empty, we would be hitting the end of the
    // string and not increase the line number.
    if(!naIsStringEmpty(string)){numlines++;}

    if(!found){
      // String has ended.
      if(!naIsStringEmpty(string)){naCreateStringExtraction(line, string, 0, -1);}
      naClearString(string);
      naCreateString(string);
      // line now contains the last characters of string.
    }else{
      naCreateStringExtraction(line, string, 0, linesize);
      naCreateStringExtraction(string, string, nextoffset, -1);
      // a line has been read.
    }
    // Reaching here, line may or may not contain whitespaces.
    
    if(skipempty){
      // If skipempty is true, we test if the line will be empty when skipping
      // its whitespaces.
      if(naIsStringEmpty(line)){
        // The line itself is already empty. Skip it.
        if(found){continue;}
      }else{
        naCreateStringExtraction(&emptytest, line, 0, -1);
        naSkipStringWhitespaces(&emptytest);
        if(naIsStringEmpty(&emptytest)){
          // If there are indeed just whitespaces, clear the line.
          naClearString(line);
          naCreateString(line);
          // emptytest will already have been cleared.
          // Go on with the next line if the string is not finished yet.
          if(found){continue;}
        }else{
          // Line has content. Clear the emptytest
          naClearString(&emptytest);
        }
      }
    }
    break;
  }
  return numlines;
}



NA_DEF void naParseStringToken(NAString* string, NAString* token){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  const NAUTF8Char* charptr;
  NAUInt tokensize = 0;
  NAInt escapesize;

  #ifndef NDEBUG
    if(!string){
      naCrash("naParseStringToken", "string is Null-Pointer.");
      return;
    }
    if(token == string)
      naError("naParseStringToken", "token and string shall not be the same.");
  #endif
  if(!token){
    token = naCreateString(NA_NULL);
  }else{
    naClearString(token);
    naCreateString(token);
  }
  // We now are sure that token is empty.
  
  if(naIsStringEmpty(string)){return;}
  naSkipStringWhitespaces(string);
  if(naIsStringEmpty(string)){return;}
  // We now are sure that token will not be empty. String now points to the
  // first non-whitespace character.

  stringsize = naGetStringSize(string);
  charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, tokensize);
    if(escapesize){
      #ifndef NDEBUG
        if(escapesize < 0)
          naError("naParseStringToken", "Internal Error: escapesize should not be negative.");
      #endif
      tokensize += escapesize;
      charptr += escapesize;
      // this surely is no whitespace. Yes, an escape sequence may represent
      // a white space but it is escaped.
    }else{
      // an unescaped character was found.
      if(*charptr <= ' '){
        // whitespace found which is not escaped.
        // Remember, we are sure that token is not empty.
        naCreateStringExtraction(token, string, 0, tokensize);
        naCreateStringExtraction(string, string, tokensize + 1, -1);
        naSkipStringWhitespaces(string);
        return;
      }
    }
    // Go one character forward.
    tokensize++;
    charptr++;
  }
  
  // Reaching here, no whitespace was found till the end of string.
  // String has ended. The token is the whole string.
  naCreateStringExtraction(token, string, 0, -1);
  naClearString(string);
  naCreateString(string);

  return;
}



NA_DEF NAString* naParseStringTokenWithDelimiter(NAString* string, NAString* token, NAUTF8Char delimiter){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUInt tokensize = 0;
  NAInt escapesize;
  const NAUTF8Char* charptr;

  #ifndef NDEBUG
    if(!string){
      naCrash("naParseStringTokenWithDelimiter", "string is Null-Pointer.");
      return NA_NULL;
    }
    if(token == string)
      naError("naParseStringTokenWithDelimiter", "token and string shall not be the same.");
  #endif
  if(!token){
    token = naCreateString(NA_NULL);
  }else{
    naClearString(token);
    naCreateString(token);
  }
  // We now are sure that token is empty.
  if(naIsStringEmpty(string)){return token;}

  stringsize = naGetStringSize(string);
  charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, tokensize);
    if(escapesize){
      #ifndef NDEBUG
        if(escapesize < 0)
          naError("naParseStringTokenWithDelimiter", "Internal Error: escapesize should not be negative.");
      #endif
      tokensize += escapesize;
      charptr += escapesize;
      // this surely is no delimiter. Yes, an escape sequence may represent
      // a certain delimiter character but it is escaped.
    }else{
      if(*charptr == delimiter){
        // delimiter found which is not escaped.
        // Remember, we are sure that token is not empty.
        naCreateStringExtraction(token, string, 0, tokensize);
        naCreateStringExtraction(string, string, tokensize + 1, -1);
        return token;
      }
    }
    tokensize++;
    charptr++;
  }


  // Reaching here, no delimiter was found till the end of string.
  // String has ended. The token is the whole string.
  naCreateStringExtraction(token, string, 0, -1);
  naClearString(string);
  naCreateString(string);
  return token;
}


NA_DEF void naParseStringPathComponent(NAString* string, NAString* token){
  // Declaration before implementation. Needed for C90.
  NAUInt stringsize;
  NAUInt tokensize = 0;
  NAInt escapesize;
  const NAUTF8Char* charptr;

  #ifndef NDEBUG
    if(!string){
      naCrash("naParseStringTokenWithDelimiter", "string is Null-Pointer.");
      return;
    }
    if(token == string)
      naError("naParseStringTokenWithDelimiter", "token and string shall not be the same.");
  #endif
  if(!token){
    token = naCreateString(NA_NULL);
  }else{
    naClearString(token);
    naCreateString(token);
  }
  // We now are sure that token is empty.
  if(naIsStringEmpty(string)){return;}

  stringsize = naGetStringSize(string);
  charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, tokensize);
    if(escapesize){
      #ifndef NDEBUG
        if(escapesize < 0)
          naError("naParseStringTokenWithDelimiter", "Internal Error: escapesize should not be negative.");
      #endif
      tokensize += escapesize;
      charptr += escapesize;
      // this surely is no delimiter. Yes, an escape sequence may represent
      // a certain delimiter character but it is escaped.
    }else{
      if((*charptr == NA_PATH_DELIMITER_UNIX) || *charptr == NA_PATH_DELIMITER_WIN){
        // delimiter found which is not escaped.
        // Remember, we are sure that token is not empty.
        naCreateStringExtraction(token, string, 0, tokensize);
        naCreateStringExtraction(string, string, tokensize + 1, -1);
        return;
      }
    }
    tokensize++;
    charptr++;
  }


  // Reaching here, no delimiter was found till the end of string.
  // String has ended. The token is the whole string.
  naCreateStringExtraction(token, string, 0, -1);
  naClearString(string);
  naCreateString(string);
  return;
}


// commented out because the autor does not like this function. todo.
//void naParseStringTokenEncapsulatedBy(  NAString* string,
//                              NAString* token,
//                             NAUTF8Char startdelimiter,
//                             NAUTF8Char enddelimiter,
//                 NAStringEscapingMethod escapemethod){
//  #ifndef NDEBUG
//    if(!string)
//      naError("naParseStringTokenEncapsulatedBy", "string is Null-Pointer.");
//    if(token == string)
//      naError("naParseStringTokenEncapsulatedBy", "token and string shall not be the same.");
//  #endif
//  if(!token){
//    token = naCreateString(NA_NULL);
//  }else{
//    naClearString(token);
//  }
//  // We now are sure that token is empty.
//  if(string->flags & NA_STRING_EMPTY){return;}
//
//  NAInt tokenoffset = 0;
//  NAInt tokensize = 0;
//  NAByte* curptr = string->array.ptr;
//  NABool foundstart = NA_FALSE;
//  NABool foundend = NA_FALSE;
//
//  while(tokensize < string->array.size){
//    if(*curptr == startdelimiter){
//      // first delimiter found. No escaping! Important for empty string ""
//      foundstart = NA_TRUE;
//      tokenoffset++;
//      curptr++;
//      continue;
//    }
//
//    // escaping:
//    if((tokensize + 1) < string->array.size){
//      switch(escapemethod){
//      case NA_STRING_ESCAPE_END_DELIMITER_DOUBLING:
//        if(*(curptr+1) == enddelimiter){
//          tokensize++;
//          curptr++;
//          continue; // continues the while loop
//        }
//        break;  // breaks the switch
//      }
//    }
//
//    if(*curptr == enddelimiter){
//      foundend = NA_TRUE;
//      break;
////      naCreateStringExtraction(token, string, 1, tokensize - 1);
////      naCreateStringExtraction(string, string, tokensize + 1, 0);
////      return;
//    }
//
//    if(foundstart){
//      tokensize++;
//    }else{
//      tokenoffset++;
//    }
//    curptr++;
//  }
//  
//  if(!found){
//    // String has ended. The token is the whole string.
//    naCreateStringExtraction(token, string, 0, 0);
//    naClearString(string);
//  }
//
//  return;
//}
// commented out because the autor does not like this function. todo.


NA_DEF NAUInt naParseUTF8StringForDecimalUnsignedInteger(const NAUTF8Char* string,
                                                           uint64* retint,
                                                            NAUInt maxbytecount,
                                                            uint64 max){
  NAUInt bytesused = 0LL;
  uint64 prevval = 0LL;
  *retint = 0;
  while(!maxbytecount || (bytesused < maxbytecount)){
    register NAUTF8Char curchar = *string;
    if((curchar < '0') || (curchar > '9')){break;}
    if(retint){
      *retint = *retint * 10LL + curchar - '0';
      if((*retint < prevval) || (*retint > max)){
        #ifndef NDEBUG
          naError("naParseUTF8StringForDecimalUnsignedInteger", "The value overflowed.");
        #endif
        *retint = max;
        retint = NA_NULL; // Trick to overjump all remaining digits.
      }else{
        prevval = *retint;
      }
    }
    bytesused++;
    string++;
  }
  return bytesused;
}



NA_DEF NAUInt naParseUTF8StringForDecimalSignedInteger( const NAUTF8Char* string,
                                                           int64* retint,
                                                           NAUInt maxbytecount,
                                                            int64 min,
                                                            int64 max){
  int64 sign = 1;
  NAUInt bytesused = 0;
  uint64 limit = max;
  uint64 intvalue;

  // Check for a potential sign at the first character
  if(*string == '+'){
    bytesused = 1;
    maxbytecount--;
    string++;
  }else if(*string == '-'){
    sign = -1;
    limit = min; // note that using integer arithmetic, this is correct.
    bytesused = 1;
    maxbytecount--;
    string++;
  }

  bytesused += naParseUTF8StringForDecimalUnsignedInteger(string, &intvalue, maxbytecount, limit);
  *retint = (sign * intvalue);
  return bytesused;
}




NA_DEF int8 naParseStringInt8(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT8_MIN, NA_INT8_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (int8)intvalue;
}


NA_DEF int16 naParseStringInt16(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT16_MIN, NA_INT16_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (int16)intvalue;
}


NA_DEF int32 naParseStringInt32(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT32_MIN, NA_INT32_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (int32)intvalue;
}


NA_DEF int64 naParseStringInt64(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT64_MIN, NA_INT64_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (int64)intvalue;
}


NA_DEF uint8 naParseStringUInt8(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT8_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (uint8)intvalue;
}


NA_DEF uint16 naParseStringUInt16(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT16_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (uint16)intvalue;
}


NA_DEF uint32 naParseStringUInt32(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT32_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (uint32)intvalue;
}


NA_DEF uint64 naParseStringUInt64(NAString* string, NABool skipdelimiter){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  NAUInt bytesused = naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT64_MAX);
  if(skipdelimiter && (size > bytesused)){
    naCreateStringExtraction(string, string, bytesused+1, -1);
  }else{
    naCreateStringExtraction(string, string, bytesused, -1);
  }
  return (uint64)intvalue;
}





NA_DEF int8 naGetStringInt8(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT8_MIN, NA_INT8_MAX);
  return (int8)intvalue;
}


NA_DEF int16 naGetStringInt16(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT16_MIN, NA_INT16_MAX);
  return (int16)intvalue;
}


NA_DEF int32 naGetStringInt32(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT32_MIN, NA_INT32_MAX);
  return (int32)intvalue;
}


NA_DEF int64 naGetStringInt64(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  int64 intvalue;
  naParseUTF8StringForDecimalSignedInteger(curptr, &intvalue, size, NA_INT64_MIN, NA_INT64_MAX);
  return (int64)intvalue;
}


NA_DEF uint8 naGetStringUInt8(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT8_MAX);
  return (uint8)intvalue;
}


NA_DEF uint16 naGetStringUInt16(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT16_MAX);
  return (uint16)intvalue;
}


NA_DEF uint32 naGetStringUInt32(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT32_MAX);
  return (uint32)intvalue;
}


NA_DEF uint64 naGetStringUInt64(const NAString* string){
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);
  NAUInt size = naGetStringSize(string);
  uint64 intvalue;
  naParseUTF8StringForDecimalUnsignedInteger(curptr, &intvalue, size, NA_UINT64_MAX);
  return (uint64)intvalue;
}









NA_DEF NABool naEqualStringToUTF8CStringLiteral(const NAString* string,
                                       const NAUTF8Char* ptr){
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
