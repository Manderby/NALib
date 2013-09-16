
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAString.h"
#include <ctype.h>

NAString* naCreateString(NAString* string){
  string = naAllocateIfNull(string, sizeof(NAString));
  naCreateByteArray(&(string->array));
//  naNull32(&(string->flags));
  return string;
}


NAString* naCreateStringWithSize(NAString* string, NAInt size){
  #ifndef NDEBUG
    if(size < 0)
      naError("naCreateStringWithSize", "size is < 0");
  #endif
  if(size == 0){return naCreateString(string);}
  string = naAllocateIfNull(string, sizeof(NAString));
  naCreateByteArrayWithSize(&(string->array), -size);
  // The negative size argument ensures that there is at least one zero-byte
  // at the end.
  string->flags = NA_STRING_NULL_TERMINATED;
  return string;
}


NAString* naCreateStringWithUTF8CString(NAString* string, const NAUTF8Char* ptr){
  #ifndef NDEBUG
    if(!ptr)
      {naCrash("naCreateStringWithUTF8CString", "ptr is Null-Pointer"); return NA_NULL;}
  #endif
  NAInt size = naStrlen(ptr);
  if(!size){return naCreateString(string);}
  string = naAllocateIfNull(string, sizeof(NAString));
  naCreateByteArrayWithConstBuffer(&(string->array), ptr, size, NA_FALSE);

  // doto: remove comment?
  // Note that the NAByte* cast is dangerous as it overwrites the const. But
  // the other solution would be to create a separate set of functions dealing
  // with only const data. Which is (currently) not useful. Just be careful not
  // to alter strings which are const. Your program likely will crash anyway.
  // If you really want to be save, just decouple each String after creating.
  //
  // C-Strings are always expected to be already Null-terminated, meaning: The
  // Byte with index [size] must be binary zero. As we are not copying but just
  // referencing the pointer, we can safely use the array without this byte
  // and still be able to say: We are null-terminated!
  string->flags = NA_STRING_NULL_TERMINATED;
  return string;
}


NAString* naCreateStringExtraction( NAString* deststring,
                              const NAString* srcstring,
                                        NAInt offset,
                                        NAInt size){
  // Declaration before implementation. Needed for C90.
  const NAByte* srclastchar;
  const NAByte* destlastchar;

  #ifndef NDEBUG
    if(!srcstring)
      {naCrash("naCreateStringExtraction", "src is Null-Pointer."); return NA_NULL;}
  #endif
  if(srcstring != deststring){
    deststring = naCreateString(deststring);
  }
  // Now, deststring is either the same as srcstring or empty.

  // If src is empty, return an empty string. But check if this is intentional
  // or an error:
  if(naIsStringEmpty(srcstring)){
    #ifndef NDEBUG
      if(offset || size)
        naError("naCreateStringExtraction", "Invalid string extraction of empty string.");
    #endif
    return deststring;
  }
  
  // In case src == dest, store all necessary data.
  srclastchar = naGetByteArrayConstByte(&(srcstring->array), -1);
  // The previous line is valid because we know that srcstring is not empty.
  
  // Extract the string
  naCreateByteArrayExtraction(&(deststring->array), &(srcstring->array), offset, size);
  if(!naIsByteArrayEmpty(&(deststring->array))){
    // Set the flags for an empty string.
//    naNull32(&(deststring->flags));
//  }else{
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



void naClearString(NAString* string, NABool reinitialize){
  if(!naIsStringEmpty(string)){naClearByteArray(&(string->array));}
  if(reinitialize){naCreateString(string);}
}



void naDestroyString(NAString* string){
  naClearString(string, NA_FALSE);
  free(string);
}




void naDecoupleString(NAString* string){
  if(naIsStringEmpty(string)){return;}
  // When decoupling, make sure, the new string is always null termiated.
  naDecoupleByteArray(&(string->array), NA_TRUE);
  string->flags |= NA_STRING_NULL_TERMINATED;
}




NAInt naGetStringSize(const NAString* string){
  return naGetByteArraySize(&(string->array));
}


const NAUTF8Char* naGetStringUTF8Pointer(const NAString* string){
  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringUTF8Pointer", "string is Null-Pointer."); return NA_NULL;}
    if(!naIsStringEmpty(string) && !(string->flags & NA_STRING_NULL_TERMINATED))
      naError("naGetStringUTF8Pointer", "string is not null-terminated.");
  #endif
  if(naIsStringEmpty(string)){
    return (const NAUTF8Char*)"";
  }else{
    return (const NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));
  }
}

const NAUTF8Char* naGetStringConstChar(const NAString* string, NAInt index){
  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringChar", "string is Null-Pointer."); return NA_NULL;}
  #endif
  return (const NAUTF8Char*)naGetByteArrayConstByte(&(string->array), index);
}

NAUTF8Char* naGetStringMutableChar(NAString* string, NAInt index){
  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringMutableChar", "string is Null-Pointer."); return NA_NULL;}
  #endif
  return (NAUTF8Char*)naGetByteArrayMutableByte(&(string->array), index);
}


NABool naIsStringEmpty(const NAString* string){
  #ifndef NDEBUG
    if(!string)
      {naCrash("naIsStringEmpty", "string is Null-Pointer."); return NA_TRUE;}
  #endif
  return naIsByteArrayEmpty(&(string->array));
}





NAInt naGetStringCharacterEscapeSizeTowardsTrailing(NAString* string, NAInt offset){
  // Note: The flags can be combined with the binary OR operator. This means
  // that multiple flags can be active. Therefore all escape methods need to
  // be checked one by one.
  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringCharacterEscapeSizeTowardsTrailing", "string is Null-Pointer."); return 0;}
  #endif
  
  NAInt stringsize = naGetStringSize(string);
  if(offset < 0){offset += stringsize;}
  #ifndef NDEBUG
    if(offset < 0)
      naError("naGetStringCharacterEscapeSizeTowardsTrailing", "offset underflows string.");
    if(offset >= stringsize)
      naError("naGetStringCharacterEscapeSizeTowardsTrailing", "offset overflows string.");
  #endif

  if(offset >= stringsize - 1){return 0;}
  
  NAUTF8Char char0 = *naGetStringConstChar(string, offset);
  NAUTF8Char char1 = *naGetStringConstChar(string, offset + 1);
  NAUTF8Char char2 = '\0';
  if(offset < stringsize - 2){char2 = *naGetStringConstChar(string, offset + 2);}

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
        while((offset + curpos) < stringsize){
          escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, offset + curpos);
          if(escapesize){
            // Something has been escaped.
            curpos += escapesize;
          }else{
            // The current character has not been escaped. Maybe it is the end
            // quote?
            if(*naGetStringConstChar(string, offset + curpos) == '\"'){break;}
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


NAInt naGetStringCharacterEscapeSizeTowardsLeading(NAString* string, NAInt offset){
  // Note: The flags can be combined with the binary OR operator. This means
  // that multiple flags can be active. Therefore all escape methods need to
  // be checked one by one.
  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringCharacterEscapeSizeTowardsLeading", "string is Null-Pointer."); return 0;}
  #endif
  
  NAInt stringsize = naGetStringSize(string);
  if(offset < 0){offset += stringsize;}
  #ifndef NDEBUG
    if(offset < 0)
      naError("naGetStringCharacterEscapeSizeTowardsLeading", "offset underflows string.");
    if(offset >= stringsize)
      naError("naGetStringCharacterEscapeSizeTowardsLeading", "offset overflows string.");
  #endif

  if(offset < 1){return 0;}
  
  NAUTF8Char char0 = *naGetStringConstChar(string, offset);
  NAUTF8Char char1 = *naGetStringConstChar(string, offset - 1);
  NAUTF8Char char2 = '\0';
  if(offset >= 2){char2 = *naGetStringConstChar(string, offset - 2);}
  
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
            if(*naGetStringConstChar(string, offset + curpos) == '\"'){break;}
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




void naSkipStringWhitespaces(NAString* string){
  #ifndef NDEBUG
    if(!string)
      {naCrash("naSkipStringWhitespaces", "Parameter is Null-Pointer."); return;}
  #endif
  if(naIsStringEmpty(string)){return;}
  // Note: There is no check for escape characters as so far, nobody ever used
  // a whitespace as an escape character. Therefore stripping whitespaces from
  // leading to trailing is safe without checking for escape characters.
  NAInt stringsize = naGetStringSize(string);
  const NAUTF8Char* charptr = naGetStringConstChar(string, 0);
  while(stringsize && (*charptr <= ' ')){
    stringsize--;
    charptr++;
  }
  naCreateByteArrayExtraction(&(string->array), &(string->array), -stringsize, -1);
}



NAInt naGetStringLine(NAString* line, NAString* string, NABool skipempty){
  NAString emptytest;
  NAInt numlines = 0;
  NAInt nextoffset = 0; // the start offset of the line after the current line
  NABool found;
  
  #ifndef NDEBUG
    if(!string)
      naError("naGetStringLine", "string is Null-Pointer.");
    if(line == string)
      naError("naGetStringLine", "line and string shall not be the same.");
  #endif

  if(!line){
    line = naCreateString(NA_NULL);
  }else{
    naClearString(line, NA_TRUE);
  }
  // We now are sure that line is empty.
  
  if(naIsStringEmpty(string)){return 0;}
  if(skipempty){
    naCreateString(&emptytest);
  }
  NAInt stringsize = naGetStringSize(string);
  
  // This while-loop is here for the skipempty-test.
  while(1){
    NAInt linesize = 0;
    NAInt escapesize;
    found = NA_FALSE;
    const NAUTF8Char* charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

    while(linesize < stringsize){
      escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, linesize);
      if(escapesize){
        #ifndef NDEBUG
          if(escapesize < 0)
            naError("naGetStringLine", "Internal Error: escapesize should not be negative.");
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
      naClearString(string, NA_TRUE);
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
          naClearString(line, NA_TRUE);
          // emptytest will already have been cleared.
          // Go on with the next line if the string is not finished yet.
          if(found){continue;}
        }else{
          // Line has content. Clear the emptytest
          naClearString(&emptytest, NA_FALSE);
        }
      }
    }
    break;
  }
  return numlines;
}



void naGetStringToken(NAString* token, NAString* string){
  NAInt tokensize = 0;
  NAInt escapesize;

  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringToken", "string is Null-Pointer."); return;}
    if(token == string)
      naError("naGetStringToken", "token and string shall not be the same.");
  #endif
  if(!token){
    token = naCreateString(NA_NULL);
  }else{
    naClearString(token, NA_TRUE);
  }
  // We now are sure that token is empty.
  
  if(naIsStringEmpty(string)){return;}
  naSkipStringWhitespaces(string);
  if(naIsStringEmpty(string)){return;}
  // We now are sure that token will not be empty. String now points to the
  // first non-whitespace character.

  NAInt stringsize = naGetStringSize(string);
  const NAUTF8Char* charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, tokensize);
    if(escapesize){
      #ifndef NDEBUG
        if(escapesize < 0)
          naError("naGetStringToken", "Internal Error: escapesize should not be negative.");
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
  naClearString(string, NA_TRUE);

  return;
}



void naGetStringTokenDelimitedBy(NAString* token, NAString* string, NAUTF8Char delimiter){
  NAInt tokensize = 0;
  NAInt escapesize;

  #ifndef NDEBUG
    if(!string)
      {naCrash("naGetStringTokenDelimitedBy", "string is Null-Pointer."); return;}
    if(token == string)
      naError("naGetStringTokenDelimitedBy", "token and string shall not be the same.");
  #endif
  if(!token){
    token = naCreateString(NA_NULL);
  }else{
    naClearString(token, NA_TRUE);
  }
  // We now are sure that token is empty.
  if(naIsStringEmpty(string)){return;}

  NAInt stringsize = naGetStringSize(string);
  const NAUTF8Char* charptr = (NAUTF8Char*)naGetByteArrayConstPointer(&(string->array));

  while(tokensize < stringsize){
    escapesize = naGetStringCharacterEscapeSizeTowardsTrailing(string, tokensize);
    if(escapesize){
      #ifndef NDEBUG
        if(escapesize < 0)
          naError("naGetStringToken", "Internal Error: escapesize should not be negative.");
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
        return;
      }
    }
    tokensize++;
    charptr++;
  }


  // Reaching here, no delimiter was found till the end of string.
  // String has ended. The token is the whole string.
  naCreateStringExtraction(token, string, 0, -1);
  naClearString(string, NA_TRUE);
  return;
}


//void naGetStringTokenEncapsulatedBy(  NAString* token,
//                              NAString* string,
//                             NAUTF8Char startdelimiter,
//                             NAUTF8Char enddelimiter,
//                 NAStringEscapingMethod escapemethod){
//  #ifndef NDEBUG
//    if(!string)
//      naError("naGetStringTokenEncapsulatedBy", "string is Null-Pointer.");
//    if(token == string)
//      naError("naGetStringTokenEncapsulatedBy", "token and string shall not be the same.");
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


uint8 naGetStringUInt8(const NAString* string){
  NAInt offset = 0;
  uint8 value = 0;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      uint8 nextvalue = value * 10 + (*curptr - '0');
      if(nextvalue < value){
        #ifndef NDEBUG
          naError("naGetStringUInt8", "The value overflowed.");
        #endif
        return NA_UINT8_MAX;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}



uint16 naGetStringUInt16(const NAString* string){
  NAInt offset = 0;
  uint16 value = 0;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      uint16 nextvalue = value * 10 + (*curptr - '0');
      if(nextvalue < value){
        #ifndef NDEBUG
          naError("naGetStringUInt16", "The value overflowed.");
        #endif
        return NA_UINT16_MAX;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}



uint32 naGetStringUInt32(const NAString* string){
  NAInt offset = 0;
  uint32 value = 0;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      uint32 nextvalue = value * 10 + (*curptr - '0');
      if(nextvalue < value){
        #ifndef NDEBUG
          naError("naGetStringUInt32", "The value overflowed.");
        #endif
        return NA_UINT32_MAX;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}




uint64 naGetStringUInt64(const NAString* string){
  NAInt offset = 0;
  uint64 value = 0LL;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      uint64 nextvalue = value * 10LL + (*curptr - '0');
      if(nextvalue < value){
        #ifndef NDEBUG
          naError("naGetStringUInt64", "The value overflowed.");
        #endif
        return NA_UINT64_MAX;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}




int8 naGetStringInt8(const NAString* string){
  NAInt offset = 0;
  int8 value = 0;
  int8 sign = 1;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Check for a potential sign at the first character
  if(*curptr == '+'){
    sign = 1;
    offset++;
    curptr++;
  }else if(*curptr == '-'){
    sign = -1;
    offset++;
    curptr++;
  }

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      int8 nextvalue = value * 10 + sign * (*curptr - '0');
      if((sign ==  1) && (nextvalue < value)){
        #ifndef NDEBUG
          naError("naGetStringInt8", "The value overflowed.");
        #endif
        return NA_INT8_MAX;
      }
      if((sign == -1) && (nextvalue > value)){
        #ifndef NDEBUG
          naError("naGetStringInt8", "The value underflowed.");
        #endif
        return NA_INT8_MIN;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}



int16 naGetStringInt16(const NAString* string){
  NAInt offset = 0;
  int16 value = 0;
  int16 sign = 1;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Check for a potential sign at the first character
  if(*curptr == '+'){
    sign = 1;
    offset++;
    curptr++;
  }else if(*curptr == '-'){
    sign = -1;
    offset++;
    curptr++;
  }

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      int16 nextvalue = value * 10 + sign * (*curptr - '0');
      if((sign ==  1) && (nextvalue < value)){
        #ifndef NDEBUG
          naError("naGetStringInt16", "The value overflowed.");
        #endif
        return NA_INT16_MAX;
      }
      if((sign == -1) && (nextvalue > value)){
        #ifndef NDEBUG
          naError("naGetStringInt16", "The value underflowed.");
        #endif
        return NA_INT16_MIN;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}



int32 naGetStringInt32(const NAString* string){
  NAInt offset = 0;
  int32 value = 0;
  int32 sign = 1;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Check for a potential sign at the first character
  if(*curptr == '+'){
    sign = 1;
    offset++;
    curptr++;
  }else if(*curptr == '-'){
    sign = -1;
    offset++;
    curptr++;
  }

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      int32 nextvalue = value * 10 + sign * (*curptr - '0');
      if((sign ==  1) && (nextvalue < value)){
        #ifndef NDEBUG
          naError("naGetStringInt32", "The value overflowed.");
        #endif
        return NA_INT32_MAX;
      }
      if((sign == -1) && (nextvalue > value)){
        #ifndef NDEBUG
          naError("naGetStringInt32", "The value underflowed.");
        #endif
        return NA_INT32_MIN;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}



int64 naGetStringInt64(const NAString* string){
  NAInt offset = 0;
  int64 value = 0LL;
  int64 sign = 1LL;
  const NAUTF8Char* curptr = naGetStringUTF8Pointer(string);

  // Check for a potential sign at the first character
  if(*curptr == '+'){
    sign = 1LL;
    offset++;
    curptr++;
  }else if(*curptr == '-'){
    sign = -1LL;
    offset++;
    curptr++;
  }

  // Accumulate the value as long as there are decimal digits.
  NAInt size = naGetStringSize(string);
  while(offset < size){
    if((*curptr >= '0') && (*curptr <= '9')){
      int64 nextvalue = value * 10LL + sign * (*curptr - '0');
      if((sign ==  1LL) && (nextvalue < value)){
        #ifndef NDEBUG
          naError("naGetStringInt64", "The value overflowed.");
        #endif
        return NA_INT64_MAX;
      }
      if((sign == -1LL) && (nextvalue > value)){
        #ifndef NDEBUG
          naError("naGetStringInt64", "The value underflowed.");
        #endif
        return NA_INT64_MIN;
      }
      value = nextvalue;
    }else{
      return value;
    }
    offset++;
    curptr++;
  }
  
  return value;
}





NABool naIsStringEqualToUTF8Pointer(const NAString* string,
                                  const NAUTF8Char* ptr){
  NAInt stringsize = naGetStringSize(string);
  NAInt ptrsize = naStrlen(ptr);
  if(stringsize != ptrsize){return NA_FALSE;}
  return !memcmp(string, ptr, (size_t)stringsize);
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
