
#include "../../NABuffer.h"



NA_DEF void naSkipBufferWhitespaces(NABufferIterator* iter){
  NABool found = NA_FALSE;

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = na_GetBufferPartDataPointerConst(iter);

    while(iter->partoffset < na_GetBufferPartByteSize(part)){
      if(*curbyte > ' '){
        found = NA_TRUE;
        break;
      }
      curbyte++;
      iter->partoffset++;
    }
  }
}



NA_DEF void naSkipBufferDelimiter(NABufferIterator* iter){
  if(!naIsBufferAtEnd(iter)){
    const NAByte* curbyte = (const NAByte*)na_GetBufferPartDataPointerConst(iter);
    if(*curbyte <= ' '){
      naSkipBufferWhitespaces(iter);
    }else{
      iter->partoffset++;
    }
  }
}



NA_DEF NAString* naParseBufferLine(NABufferIterator* iter, NABool skipempty){
  NAString* string = NA_NULL;
  NABool found = NA_FALSE;
  NABool checkwindowsend = NA_FALSE;
  NAInt start = naGetBufferLocation(iter);
  NAInt end = start + 1;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);

  while((!found || checkwindowsend) && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;

    na_PrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = na_GetBufferPartDataPointerConst(iter);
    
    // Note: Do not use NA_NL_XXX macros here. That is semantically wrong.
    while(iter->partoffset < na_GetBufferPartByteSize(part)){
      if(checkwindowsend){
        checkwindowsend = NA_FALSE;
        if(*curbyte == '\n'){iter->partoffset++; break;}
      }
      if(found){break;}
      if((*curbyte == '\r') || (*curbyte == '\n')){
        if(skipempty && ((iter->partoffset - start) == 0)){
          start++;
        }else{
          found = NA_TRUE;
          string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(start, end));
        }
        checkwindowsend = (*curbyte == '\r');
      }
      curbyte++;
      end++;
      iter->partoffset++;
    }
  }

  if(!found){
    NARangei remainingrange = naMakeRangeiWithStartAndEnd(start, naGetRangeiEnd(buffer->range));
    if(!naIsRangeiEmpty(remainingrange)){
      string = naNewStringWithBufferExtraction(buffer, remainingrange);
    }else{
      string = naNewString();
    }
  }
  
  #ifndef NDEBUG
    if(!string)
      naError("string not initialized");
  #endif

  return string;
}



NA_DEF NAString* naParseBufferRemainder(NABufferIterator* iter){
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
  NAInt abspos = naGetBufferLocation(iter);
  return naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(abspos, naGetRangeiEnd(buffer->range)));
}



NA_DEF NAString* naParseBufferToken(NABufferIterator* iter){
  NAString* string;
  NARangei range;
  NABool found = NA_FALSE;
  NAInt start = naGetBufferLocation(iter);
  NAInt end = start;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = na_GetBufferPartDataPointerConst(iter);

    while(iter->partoffset < na_GetBufferPartByteSize(part)){
      if(*curbyte <= ' '){
        found = NA_TRUE;
        break;
      }
      curbyte++;
      end++;
      iter->partoffset++;
    }
  }

  if(!found){
    end = naGetRangeiEnd(buffer->range);
  }
  range = naMakeRangeiWithStartAndEnd(start, end);
  if(!naIsRangeiEmpty(range)){
    string = naNewStringWithBufferExtraction(buffer, range);
  }else{
    string = naNewString();
  }
  naSkipBufferWhitespaces(iter);
  return string;
}



NA_DEF NAString* naParseBufferTokenWithDelimiter(NABufferIterator* iter, NAUTF8Char delimiter){
  NA_UNUSED(iter);
  NA_UNUSED(delimiter);
//  NAString* string;
//  NAInt tokenstart = iter->curoffset;
//  const NABufferPart* part;
//
//  while(!naIsBufferAtInitial(iter)){
//    const NAUTF8Char* curbyte;
//    NAInt endoffset;
//    NABool found = NA_FALSE;
//
//    part = naGetListCurConst(&(iter->partiter));
//    if(na_IsBufferPartSparse(part)){
//      naGetBufferu8(iter);
//      part = naGetListCurConst(&(iter->partiter));
//    }
//    curbyte = (NAUTF8Char*)na_GetBufferPartDataPointerConst(part, iter->curoffset);
//    endoffset = naGetBufferPartEnd(part);
//    while(iter->curoffset < endoffset){
//      if(*curbyte == delimiter){found = NA_TRUE; break;}
//      curbyte++;
//      iter->curoffset++;
//    }
//    if(!naContainsBufferPartOffset(part, iter->curoffset)){naIterateList(&(iter->partiter));}
//    if(found){break;}
//  }
//
//  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);
//  string = naNewStringWithBufferExtraction(buffer naMakeRangeiWithStartAndEnd(tokenstart, iter->curoffset));
//  naLocateBufferRelative(iter, 1);
//  return string;
  return NA_NULL;
}



NA_DEF NAString* naParseBufferPathComponent(NABufferIterator* iter){
  NAString* string;
  NARangei range;
  NABool found = NA_FALSE;
  NAInt start = naGetBufferLocation(iter);
  NAInt end = start;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = na_GetBufferPartDataPointerConst(iter);

    while(iter->partoffset < na_GetBufferPartByteSize(part)){
      if(*curbyte == '/' || *curbyte == '\\'){
        found = NA_TRUE;
        break;
      }
      curbyte++;
      end++;
      iter->partoffset++;
    }
  }

  if(!found){
    end = naGetRangeiEnd(buffer->range);
  }
  range = naMakeRangeiWithStartAndEnd(start, end);
  if(!naIsRangeiEmpty(range)){
    string = naNewStringWithBufferExtraction(buffer, range);
  }else{
    string = naNewString();
  }
  naSkipBufferWhitespaces(iter);
  return string;
}



NA_DEF NAInt naParseBufferDecimalUnsignedInteger(NABufferIterator* iter, NAu64* retValuei, NAInt maxdigitcount, NAu64 max){
  NAInt bytesused;
  NAu64 prevval;
  NABool found = NA_FALSE;
  NAInt start = naGetBufferLocation(iter);
  NAInt end = start;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);

  *retValuei = NA_ZERO_u64;
  bytesused = 0;
  prevval = NA_ZERO_u64;
  if(maxdigitcount == 0){maxdigitcount = naGetRangeiEnd(buffer->range) - start;}

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;
    
    naIsBufferAtEnd(iter);
    
    na_PrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = na_GetBufferPartDataPointerConst(iter);

    while(iter->partoffset < na_GetBufferPartByteSize(part)){
      if(bytesused >= maxdigitcount){break;}

      if((*curbyte < '0') || (*curbyte > '9')){found = NA_TRUE; break;}
      *retValuei = naAddu64(naMulu64(*retValuei, naMakeu64WithLo(10)), naMakeu64WithLo(*curbyte - '0'));
      #ifndef NDEBUG
        if(naGreateru64(*retValuei, max))
          naError("The value overflowed max.");
        if(naSmalleru64(*retValuei, prevval))
          naError("The value overflowed 64 bit integer space.");
      #endif
      if(naSmalleru64(*retValuei, prevval) || naGreateru64(*retValuei, max)){
        *retValuei = max;
      }
      prevval = *retValuei;
      bytesused++;
      curbyte++;
      end++;
      iter->partoffset++;
    }
  }

  return bytesused;
}



NA_DEF NAInt naParseBufferDecimalSignedInteger(NABufferIterator* iter, NAi64* retValuei, NAInt maxdigitcount, NAi64 min, NAi64 max){
  NAi64 sign = NA_ONE_i64;
  NAInt bytesused = 0;
  NAi64 limit = max;
  NAu64 intvalue;
  const NAByte* curbyte;

  *retValuei = NA_ZERO_i64;

  na_PrepareBuffer(iter, 1);
  if(naIsBufferAtInitial(iter)){return 0;}
  curbyte = na_GetBufferPartDataPointerConst(iter);

  // Check for a potential sign at the first character
  if(*curbyte == '+'){
    bytesused = 1;
    maxdigitcount--;
    iter->partoffset++;
  }else if(*curbyte == '-'){
    sign = naNegi64(NA_ONE_i64);
    limit = naNegi64(min);
    bytesused = 1;
    maxdigitcount--;
    iter->partoffset++;
  }
  if(maxdigitcount == -1){maxdigitcount = 0;}

  bytesused += naParseBufferDecimalUnsignedInteger(iter, &intvalue, maxdigitcount, naCasti64Tou64(limit));
  *retValuei = naMuli64(sign, naCastu64Toi64(intvalue));
  return bytesused;
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
