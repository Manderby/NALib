
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"



NA_DEF void naSkipBufferWhitespaces(NABufferIterator* iter){
  NA_UNUSED(iter);
//  NABool nonwhitespacefound = NA_FALSE;
//
//  while(!naIsBufferAtInitial(iter)){
//    NAInt endoffset;
//    const NAByte* curbyte;
//        
////    naPrepareBuffer(iter, naMakeRangei(iter->curoffset, 1), NA_FALSE, NA_TRUE);
//
//    const NABufferPart* part = naGetListCurConst(&(iter->partiter));
//
////    printf("%p ", part);
//
//    #ifndef NDEBUG
//      if(naIsBufferPartSparse(part))
//        naError("naSkipBufferWhitespaces", "sparse part detected.");
//    #endif
//    curbyte = naGetBufferPartDataPointerConst(part, iter->curoffset); 
//    endoffset = naGetBufferPartEnd(part);
////    printf("%d %d %d\n", (int)part->origin, (int)part->range.origin, (int)part->range.length);
//    while(iter->curoffset < endoffset){
////      printf("%d %c\n", (int)*curbyte, (char)*curbyte);
//      if(*curbyte > ' '){
//        nonwhitespacefound = NA_TRUE;
//        break;
//      }
//      curbyte++;
//      iter->curoffset++;
//    }
//    if(!naContainsBufferPartOffset(part, iter->curoffset)){
//      naIterateList(&(iter->partiter));
//    }
//    if(nonwhitespacefound){break;}
//  }
}



NA_DEF void naSkipBufferDelimiter(NABufferIterator* iter){
  NA_UNUSED(iter);
//  NAByte curbyte;
//  if(!naIsBufferAtInitial(iter)){
//    curbyte = naGetBufferu8(iter);
//    if(curbyte <= ' '){
//      naSkipBufferWhitespaces(iter);
//    }else{
//      naSeekBufferRelative(iter, 1);
//    }
//  }
}



NA_DEF NAString* naParseBufferLine(NABufferIterator* iter, NABool skipempty){
  NA_UNUSED(iter);
  NA_UNUSED(skipempty);
//  NABool lineendingfound = NA_FALSE;
//  NABool checkwindowsend = NA_FALSE;
//  NAInt linestart = iter->curoffset;
//  NAString* string = naNewString();
//  
//  while(!naIsBufferAtInitial(iter)){
//    const NAByte* curbyte;
//    NAInt endoffset;
//    const NABufferPart* part;
//
//    if(lineendingfound && !checkwindowsend){break;}
//    part = naGetListCurConst(&(iter->partiter));
//    if(naIsBufferPartSparse(part)){
//      naGetBufferu8(iter);
//      part = naGetListCurConst(&(iter->partiter));
//    }
//    curbyte = naGetBufferPartDataPointerConst(part, iter->curoffset); 
//    endoffset = naGetBufferPartEnd(part);
//    while(iter->curoffset < endoffset){
//      if(checkwindowsend){
//        checkwindowsend = NA_FALSE;
//        if(*curbyte == '\n'){iter->curoffset++; break;}
//      }
//      if(lineendingfound){break;}
////      printf("%c\n", *curbyte);
//      if((*curbyte == '\r') || (*curbyte == '\n')){
////        iter->linenum++;
//        if(skipempty && ((iter->curoffset - linestart) == 0)){
//          linestart++;
//        }else{
//          lineendingfound = NA_TRUE;
//          naDelete(string);
//          string = naNewStringWithBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangeiWithStartAndEnd(linestart, iter->curoffset));
//        }
//        checkwindowsend = (*curbyte == '\r');
//      }
//      curbyte++;
//      iter->curoffset++;
//    }
//    if(!naContainsBufferPartOffset(part, iter->curoffset)){naIterateList(&(iter->partiter));}
//  }
//
//  if(!lineendingfound){
//    naDelete(string);
//    string = naNewStringWithBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangeiWithStartAndEnd(linestart, iter->curoffset));
//  }
//
//  return string;
  return NA_NULL;
}



NA_DEF NAUInt naGetBufferLineNumber(NABufferIterator* iter){
  NA_UNUSED(iter);
//  return iter->linenum;
  return 0;
}



NA_DEF NAString* naParseBufferRemainder(NABufferIterator* iter){
  NA_UNUSED(iter);
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  return naNewStringWithBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangeiWithStartAndEnd(iter->curoffset, naGetRangeiEnd(buffer->range)));
  return NA_NULL;
}



NA_DEF NAString* naParseBufferToken(NABufferIterator* iter){
  NA_UNUSED(iter);
//  NAString* string;
//  NAInt tokenstart = iter->curoffset;
//
//  // todo: this always requires a naSeekBufferFromStart call. Make this better.
//  while(!naIsBufferAtInitial(iter)){
//    const NAByte* curbyte;
//    NAInt endoffset;
//    NABool found = NA_FALSE;
//
//    const NABufferPart* part = naGetListCurConst(&(iter->partiter));
//    if(naIsBufferPartSparse(part)){
//      naGetBufferu8(iter);
//      part = naGetListCurConst(&(iter->partiter));
//    }
//    curbyte = naGetBufferPartDataPointerConst(part, iter->curoffset); 
//    endoffset = naGetBufferPartEnd(part);
//    while(iter->curoffset < endoffset){
//      if(*curbyte <= ' '){found = NA_TRUE; break;}
//      curbyte++;
//      iter->curoffset++;
//    }
//    if(!naContainsBufferPartOffset(part, iter->curoffset)){naIterateList(&(iter->partiter));}
//    if(found){break;}
//  }
//
//  string = naNewStringWithBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangeiWithStartAndEnd(tokenstart, iter->curoffset));
//  naSkipBufferWhitespaces(iter);
//  return string;
  return NA_NULL;
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
//  string = naNewStringWithBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangeiWithStartAndEnd(tokenstart, iter->curoffset));
//  naSeekBufferRelative(iter, 1);
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
//  string = naNewStringWithBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangeiWithStartAndEnd(tokenstart, iter->curoffset));
//  naSeekBufferRelative(iter, 1);
//  return string;
  return NA_NULL;
}



NA_DEF NAInt naParseBufferDecimalUnsignedInteger(NABufferIterator* iter, uint64* retint, NAInt maxdigitcount, uint64 max){
  NA_UNUSED(iter);
  NA_UNUSED(retint);
  NA_UNUSED(maxdigitcount);
  NA_UNUSED(max);
//  NAInt bytesused;
//  uint64 prevval;
//  NABool endfound = NA_FALSE;
//
//  *retint = NA_ZERO_64u;
//
//  #ifndef NDEBUG
//    if(!retint)
//      naCrash("naParseBufferDecimalUnsignedInteger", "retint is Null-Pointer");
//  #endif
//
//  if(naIsBufferEmpty(naGetBufferIteratorBufferConst(iter))){return 0;}
////  if(naIsBufferAtInitial(iter)){naSeekBufferFromStart(iter, 0);}
//  if(naIsBufferAtInitial(iter)){return 0;}
//
//  if(maxdigitcount == 0){maxdigitcount = naGetRangeiEnd(naGetBufferIteratorBufferConst(iter)->range) - iter->curoffset;}
//  bytesused = 0;
//  prevval = NA_ZERO_64u;
//  
//  while(!naIsBufferAtInitial(iter)){
//    const NAByte* curbyte;
//    NAInt endoffset;
//
//    const NABufferPart* part = naGetListCurConst(&(iter->partiter));
//    #ifndef NDEBUG
//      if(naIsBufferPartSparse(part))
//        naError("naParseBufferTokenWithDelimiter", "sparse part detected.");
//    #endif
//    curbyte = naGetBufferPartDataPointerConst(part, iter->curoffset); 
//    endoffset = naGetBufferPartEnd(part);
//    while(iter->curoffset < endoffset){
//      if(bytesused >= maxdigitcount){break;}
//
//      if((*curbyte < '0') || (*curbyte > '9')){endfound = NA_TRUE; break;}
//      *retint = naAddUInt64(naMulUInt64(*retint, naMakeUInt64WithLo(10)), naMakeUInt64WithLo(*curbyte - '0'));
//      #ifndef NDEBUG
//        if(naGreaterUInt64(*retint, max))
//          naError("naParseBufferDecimalUnsignedInteger", "The value overflowed max.");
//        if(naSmallerUInt64(*retint, prevval))
//          naError("naParseBufferDecimalUnsignedInteger", "The value overflowed 64 bit integer space.");
//      #endif
//      if(naSmallerUInt64(*retint, prevval) || naGreaterUInt64(*retint, max)){
//        *retint = max;
//      }
//      prevval = *retint;
//      bytesused++;
//      curbyte++;
//    }
//    if(bytesused >= maxdigitcount){break;}
//    iter->curoffset += bytesused;
//    if(!naContainsBufferPartOffset(part, iter->curoffset)){naIterateList(&(iter->partiter));}
//    if(endfound){break;}
//  }
//  
//  return bytesused;
  return 0;
}



NA_DEF NAInt naParseBufferDecimalSignedInteger(NABufferIterator* iter, int64* retint, NAInt maxdigitcount, int64 min, int64 max){
  NA_UNUSED(iter);
  NA_UNUSED(retint);
  NA_UNUSED(maxdigitcount);
  NA_UNUSED(min);
  NA_UNUSED(max);
//  int64 sign = NA_ONE_64;
//  NAInt bytesused = 0;
//  int64 limit = max;
//  uint64 intvalue;
//  const NABufferPart* part;
//  const NAByte* curbyte;
//
//  *retint = NA_ZERO_64;
//
//  if(naIsBufferEmpty(naGetBufferIteratorBufferConst(iter))){return 0;}
//  if(naIsBufferAtInitial(iter)){return 0;}
////  if(naIsBufferAtInitial(iter)){naSeekBufferFromStart(iter, 0);}
//  part = naGetListCurConst(&(iter->partiter));
//  if(naIsBufferPartSparse(part)){naPrepareBuffer(iter, part->range, NA_FALSE, NA_TRUE);}
//  curbyte = naGetBufferPartDataPointerConst(part, iter->curoffset); 
//
//  // Check for a potential sign at the first character
//  if(*curbyte == '+'){
//    bytesused = 1;
//    maxdigitcount--;
//    naSeekBufferRelative(iter, 1);
//  }else if(*curbyte == '-'){
//    sign = naNegInt64(NA_ONE_64);
//    limit = naNegInt64(min);
//    bytesused = 1;
//    maxdigitcount--;
//    naSeekBufferRelative(iter, 1);
//  }
//  if(maxdigitcount == -1){maxdigitcount = 0;}
//
//  bytesused += naParseBufferDecimalUnsignedInteger(iter, &intvalue, maxdigitcount, naCastInt64ToUInt64(limit));
//  *retint = naMulInt64(sign, naCastUInt64ToInt64(intvalue));
//  return bytesused;
  return 0;
}



NA_DEF int8 naParseBufferInt8(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  int64 intvalue;  
//  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakeInt64WithLo(NA_INT8_MIN), naMakeInt64WithLo(NA_INT8_MAX));
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return naCastInt64ToInt8(intvalue);
  return 0;
}
NA_DEF int16 naParseBufferInt16(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  int64 intvalue;  
//  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakeInt64WithLo(NA_INT16_MIN), naMakeInt64WithLo(NA_INT16_MAX));
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return naCastInt64ToInt16(intvalue);
  return 0;
}
NA_DEF int32 naParseBufferInt32(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  int64 intvalue;  
//  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakeInt64WithLo(NA_INT32_MIN), naMakeInt64WithLo(NA_INT32_MAX));
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return naCastInt64ToInt32(intvalue);
  return 0;
}
NA_DEF int64 naParseBufferInt64(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  int64 intvalue;  
//  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, NA_INT64_MIN, NA_INT64_MAX);
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return intvalue;
  return 0;
}



NA_DEF uint8 naParseBufferUInt8(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  uint64 uintvalue;  
//  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeUInt64WithLo(NA_UINT8_MAX));
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return naCastUInt64ToUInt8(uintvalue);
  return 0;
}
NA_DEF uint16 naParseBufferUInt16(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  uint64 uintvalue;  
//  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeUInt64WithLo(NA_UINT16_MAX));
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return naCastUInt64ToUInt16(uintvalue);
  return 0;
}
NA_DEF uint32 naParseBufferUInt32(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  uint64 uintvalue;  
//  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeUInt64WithLo(NA_UINT32_MAX));
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return naCastUInt64ToUInt32(uintvalue);
  return 0;
}
NA_DEF uint64 naParseBufferUInt64(NABufferIterator* iter, NABool skipdelimiter){
  NA_UNUSED(iter);
  NA_UNUSED(skipdelimiter);
//  uint64 uintvalue;  
//  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, NA_UINT64_MAX);
//  if(skipdelimiter){naSkipBufferDelimiter(iter);}
//  return uintvalue;
  return 0;
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
