
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "../../NABuffer.h"



NA_DEF void naSkipBufferWhitespaces(NABufferIterator* iter){
  NABool found = NA_FALSE;

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;
    
    naPrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = naGetBufferPartDataPointerConst(iter);

    while(iter->partoffset < naGetBufferPartByteSize(part)){
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
  const NAByte* curbyte;
  if(!naIsBufferAtEnd(iter)){
    curbyte = (const NAByte*)naGetBufferPartDataPointerConst(iter);
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
  NAInt start = naGetBufferLocation(iter);
  NAInt end = start;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);

  NABool checkwindowsend = NA_FALSE;

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;

    if(found && !checkwindowsend){break;}
    
    naPrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = naGetBufferPartDataPointerConst(iter);
    
    while(iter->partoffset < naGetBufferPartByteSize(part)){
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
    
    naPrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = naGetBufferPartDataPointerConst(iter);

    while(iter->partoffset < naGetBufferPartByteSize(part)){
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
//    if(naIsBufferPartSparse(part)){
//      naGetBufferu8(iter);
//      part = naGetListCurConst(&(iter->partiter));
//    }
//    curbyte = (NAUTF8Char*)naGetBufferPartDataPointerConst(part, iter->curoffset);
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
  NA_UNUSED(iter);
//  NAString* string;
//  NAInt tokenstart = iter->curoffset;
//  const NABufferPart* part;
//
//  while(!naIsBufferAtInitial(iter)){
//    const NAByte* curbyte;
//    NAInt endoffset;
//    NABool found = NA_FALSE;
//
//    part = naGetListCurConst(&(iter->partiter));
//    if(naIsBufferPartSparse(part)){
//      naGetBufferu8(iter);
//      part = naGetListCurConst(&(iter->partiter));
//    }
//    curbyte = naGetBufferPartDataPointerConst(part, iter->curoffset);
//    endoffset = naGetBufferPartEnd(part);
//    while(iter->curoffset < endoffset){
//      if((*curbyte == '/') || *curbyte == '\\'){found = NA_TRUE; break;}
//      curbyte++;
//      iter->curoffset++;
//    }
//    if(!naContainsBufferPartOffset(part, iter->curoffset)){naIterateList(&(iter->partiter));}
//    if(found){break;}
//  }
//
//  buffer = naGetBufferIteratorBufferMutable(iter);
//  string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(tokenstart, iter->curoffset));
//  naLocateBufferRelative(iter, 1);
//  return string;
  return NA_NULL;
}



NA_DEF NAInt naParseBufferDecimalUnsignedInteger(NABufferIterator* iter, uint64* retint, NAInt maxdigitcount, uint64 max){
  NAInt bytesused;
  uint64 prevval;
  NABool found = NA_FALSE;
  NAInt start = naGetBufferLocation(iter);
  NAInt end = start;
  NABuffer* buffer = naGetBufferIteratorBufferMutable(iter);

  *retint = NA_ZERO_64u;
  bytesused = 0;
  prevval = NA_ZERO_64u;
  if(maxdigitcount == 0){maxdigitcount = naGetRangeiEnd(buffer->range) - start;}

  while(!found && !naIsBufferAtEnd(iter)){
    const NAByte* curbyte;
    const NABufferPart* part;
    
    naIsBufferAtEnd(iter);
    
    naPrepareBuffer(iter, 1);
    part = naGetBufferPart(iter);
    if(naIsBufferAtInitial(iter)){break;}
    curbyte = naGetBufferPartDataPointerConst(iter);

    while(iter->partoffset < naGetBufferPartByteSize(part)){
      if(bytesused >= maxdigitcount){break;}

      if((*curbyte < '0') || (*curbyte > '9')){found = NA_TRUE; break;}
      *retint = naAddUInt64(naMulUInt64(*retint, naMakeUInt64WithLo(10)), naMakeUInt64WithLo(*curbyte - '0'));
      #ifndef NDEBUG
        if(naGreaterUInt64(*retint, max))
          naError("The value overflowed max.");
        if(naSmallerUInt64(*retint, prevval))
          naError("The value overflowed 64 bit integer space.");
      #endif
      if(naSmallerUInt64(*retint, prevval) || naGreaterUInt64(*retint, max)){
        *retint = max;
      }
      prevval = *retint;
      bytesused++;
      curbyte++;
      end++;
      iter->partoffset++;
    }
  }

  return bytesused;
}



NA_DEF NAInt naParseBufferDecimalSignedInteger(NABufferIterator* iter, int64* retint, NAInt maxdigitcount, int64 min, int64 max){
  int64 sign = NA_ONE_64;
  NAInt bytesused = 0;
  int64 limit = max;
  uint64 intvalue;
  const NAByte* curbyte;

  *retint = NA_ZERO_64;

  naPrepareBuffer(iter, 1);
  if(naIsBufferAtInitial(iter)){return 0;}
  curbyte = naGetBufferPartDataPointerConst(iter);

  // Check for a potential sign at the first character
  if(*curbyte == '+'){
    bytesused = 1;
    maxdigitcount--;
    iter->partoffset++;
  }else if(*curbyte == '-'){
    sign = naNegInt64(NA_ONE_64);
    limit = naNegInt64(min);
    bytesused = 1;
    maxdigitcount--;
    iter->partoffset++;
  }
  if(maxdigitcount == -1){maxdigitcount = 0;}

  bytesused += naParseBufferDecimalUnsignedInteger(iter, &intvalue, maxdigitcount, naCastInt64ToUInt64(limit));
  *retint = naMulInt64(sign, naCastUInt64ToInt64(intvalue));
  return bytesused;
}



// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
