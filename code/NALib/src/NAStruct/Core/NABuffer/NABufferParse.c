
#include "../../NABuffer.h"
#include "../../../NAUtility/NAString.h"



NA_DEF void naSkipBufferWhitespaces(NABufferIterator* iter) {
  NABool found = NA_FALSE;

  while(!found && !naIsBufferAtEnd(iter)) {
    const NAByte* curByte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = na_GetBufferPart(iter);
    
    if(naIsBufferAtInitial(iter))
      break;
    
    curByte = na_GetBufferPartDataPointerConst(iter);

    while(naSmalleri64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      if(*curByte > ' ') {
        found = NA_TRUE;
        break;
      }
      curByte++;
      naInci64(iter->partOffset);
    }
  }
}



NA_DEF void naSkipBufferDelimiter(NABufferIterator* iter) {
  if(!naIsBufferAtEnd(iter)) {
    const NAByte* curByte = (const NAByte*)na_GetBufferPartDataPointerConst(iter);
    if(*curByte <= ' ') {
      naSkipBufferWhitespaces(iter);
    }else{
      naInci64(iter->partOffset);
    }
  }
}



NA_DEF NAString* naParseBufferLine(NABufferIterator* iter, NABool skipEmpty) {
  NAString* string = NA_NULL;
  NABool found = NA_FALSE;
  NABool checkWindowsEnd = NA_FALSE;
  int64 start = naGetBufferLocation(iter);
  int64 cur = start;
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);

  while((!found || checkWindowsEnd) && !naIsBufferAtEnd(iter)) {
    const NAByte* curByte;
    const NABufferPart* part;

    na_PrepareBuffer(iter, 1);
    part = na_GetBufferPart(iter);
    
    if(naIsBufferAtInitial(iter))
      break;
      
    curByte = na_GetBufferPartDataPointerConst(iter);
    
    // Note: Do not use NA_NL_XXX macros here. That is semantically wrong.
    while(naSmalleri64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      if(checkWindowsEnd) {
        checkWindowsEnd = NA_FALSE;
        if(*curByte == '\n') {
          naInci64(iter->partOffset);
          break;
        }
      }
      
      if(found)
        break;
      
      if((*curByte == '\r') || (*curByte == '\n')) {
        if(skipEmpty && naEquali64(naSubi64(cur, start), NA_ZERO_i64)) {
          naInci64(start);
        }else{
          found = NA_TRUE;
          NARangei64 range = naMakeRangei64Combination(start, naMakeMaxWithEndi64(cur));
          if(naIsRangei64Useful(range)) {
            string = naNewStringWithBufferExtraction(buffer, range);
          }else{
            string = naNewString();
          }
        }
        checkWindowsEnd = (*curByte == '\r');
      }
      curByte++;
      naInci64(cur);
      naInci64(iter->partOffset);
    }
  }

  if(!found) {
    NARangei64 remainingrange = naMakeRangei64Combination(start, naGetRangei64Max(buffer->range));
    if(!naIsRangei64Empty(remainingrange)) {
      string = naNewStringWithBufferExtraction(buffer, remainingrange);
    }else{
      string = naNewString();
    }
  }
  
  #if NA_DEBUG
    if(!string)
      naError("string not initialized");
  #endif

  return string;
}



NA_DEF NAString* naParseBufferRemainder(NABufferIterator* iter) {
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);
  int64 absPos = naGetBufferLocation(iter);
  return naNewStringWithBufferExtraction(buffer, naMakeRangei64Combination(absPos, naGetRangei64Max(buffer->range)));
}



NA_DEF NAString* naParseBufferToken(NABufferIterator* iter) {
  NAString* string;
  NARangei64 range;
  NABool found = NA_FALSE;
  int64 start = naGetBufferLocation(iter);
  int64 end = start;
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);

  while(!found && !naIsBufferAtEnd(iter)) {
    const NAByte* curByte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = na_GetBufferPart(iter);
    
    if(naIsBufferAtInitial(iter))
      break;
    
    curByte = na_GetBufferPartDataPointerConst(iter);

    while(naSmalleri64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      if(*curByte <= ' ') {
        found = NA_TRUE;
        break;
      }
      curByte++;
      naInci64(end);
      naInci64(iter->partOffset);
    }
  }

  if(!found) {
    end = naGetRangei64End(buffer->range);
  }
  if(start != end) {
    range = naMakeRangei64Combination(start, naMakeMaxWithEndi64(end));
    string = naNewStringWithBufferExtraction(buffer, range);
  }else{
    string = naNewString();
  }
  naSkipBufferWhitespaces(iter);
  return string;
}



NA_DEF NAString* naParseBufferTokenWithDelimiter(NABufferIterator* iter, NAUTF8Char delimiter, NABool skipWhitespace) {
  NAString* string;
  NARangei64 range;
  NABool found = NA_FALSE;
  int64 start = naGetBufferLocation(iter);
  int64 end = start;
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);

  while(!found && !naIsBufferAtEnd(iter)) {
    const NAByte* curByte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = na_GetBufferPart(iter);
    
    if(naIsBufferAtInitial(iter))
      break;
    
    curByte = na_GetBufferPartDataPointerConst(iter);

    while(naSmalleri64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      if(*curByte == delimiter) {
        found = NA_TRUE;
        break;
      }
      curByte++;
      naInci64(end);
      naInci64(iter->partOffset);
    }
  }

  if(!found) {
    end = naGetRangei64End(buffer->range);
  }else{
    naIterateBuffer(iter, NA_ONE_i64);
  }
  range = naMakeRangei64Combination(start, naMakeMaxWithEndi64(end));
  if(!naIsRangei64Empty(range)) {
    string = naNewStringWithBufferExtraction(buffer, range);
  }else{
    string = naNewString();
  }
  
  if(skipWhitespace) {
    naSkipBufferWhitespaces(iter);
  }
  
  return string;
}



NA_DEF NAString* naParseBufferPathComponent(NABufferIterator* iter) {
  NAString* string;
  NARangei64 range;
  NABool found = NA_FALSE;
  int64 start = naGetBufferLocation(iter);
  int64 end = start;
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);

  while(!found && !naIsBufferAtEnd(iter)) {
    const NAByte* curByte;
    const NABufferPart* part;
    
    na_PrepareBuffer(iter, 1);
    part = na_GetBufferPart(iter);
    
    if(naIsBufferAtInitial(iter))
      break;
    
    curByte = na_GetBufferPartDataPointerConst(iter);

    while(naSmalleri64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      if(*curByte == '/' || *curByte == '\\') {
        found = NA_TRUE;
        break;
      }
      curByte++;
      naInci64(end);
      naInci64(iter->partOffset);
    }
  }

  if(!found) {
    end = naGetRangei64End(buffer->range);
  }
  range = naMakeRangei64Combination(start, naMakeMaxWithEndi64(end));
  if(!naIsRangei64Empty(range)) {
    string = naNewStringWithBufferExtraction(buffer, range);
  }else{
    string = naNewString();
  }
  naSkipBufferWhitespaces(iter);
  return string;
}



NA_DEF int64 naParseBufferDecimalUnsignedInteger(NABufferIterator* iter, uint64* retValuei, int64 maxDigitCount, uint64 max) {
  int64 bytesUsed;
  uint64 prevVal;
  NABool found = NA_FALSE;
  int64 start = naGetBufferLocation(iter);
  int64 end = start;
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);

  *retValuei = NA_ZERO_u64;
  bytesUsed = NA_ZERO_i64;
  prevVal = NA_ZERO_u64;
  if(naEquali64(maxDigitCount, NA_ZERO_i64)) {
    maxDigitCount = naSubi64(naGetRangei64End(buffer->range), start);
  }

  while(!found && !naIsBufferAtEnd(iter)) {
    const NAByte* curByte;
    const NABufferPart* part;
    
    naIsBufferAtEnd(iter);
    
    na_PrepareBuffer(iter, 1);
    part = na_GetBufferPart(iter);
    
    if(naIsBufferAtInitial(iter))
      break;
      
    curByte = na_GetBufferPartDataPointerConst(iter);

    while(naSmalleri64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      if(naGreaterEquali64(bytesUsed, maxDigitCount))
        break;

      if((*curByte < '0') || (*curByte > '9')) {
        found = NA_TRUE;
        break;
      }
      *retValuei = naAddu64(naMulu64(*retValuei, naMakeu64WithLo(10)), naMakeu64WithLo(*curByte - '0'));
      #if NA_DEBUG
        if(naGreateru64(*retValuei, max))
          naError("The value overflowed max.");
        if(naSmalleru64(*retValuei, prevVal))
          naError("The value overflowed 64 bit integer space.");
      #endif
      if(naSmalleru64(*retValuei, prevVal) || naGreateru64(*retValuei, max)) {
        *retValuei = max;
      }
      prevVal = *retValuei;
      naInci64(bytesUsed);
      curByte++;
      naInci64(end);
      naInci64(iter->partOffset);
    }
  }

  return bytesUsed;
}



NA_DEF int64 naParseBufferDecimalSignedInteger(NABufferIterator* iter, int64* retValuei, int64 maxDigitCount, int64 min, int64 max) {
  int64 sign = NA_ONE_i64;
  int64 bytesUsed = NA_ZERO_i64;
  int64 limit = max;
  uint64 intValue;
  const NAByte* curByte;

  *retValuei = NA_ZERO_i64;

  na_PrepareBuffer(iter, 1);
  
  if(naIsBufferAtInitial(iter))
    return NA_ZERO_i64;
  
  curByte = na_GetBufferPartDataPointerConst(iter);

  // Check for a potential sign at the first character
  if(*curByte == '+') {
    bytesUsed = NA_ONE_i64;
    naDeci64(maxDigitCount);
    naInci64(iter->partOffset);
  }else if(*curByte == '-') {
    sign = naNegi64(NA_ONE_i64);
    limit = naNegi64(min);
    bytesUsed = NA_ONE_i64;
    naDeci64(maxDigitCount);
    naInci64(iter->partOffset);
  }
  if(naEquali64(maxDigitCount, NA_MINUS_ONE_i64)) {
    maxDigitCount = NA_ZERO_i64;
  }

  bytesUsed = naAddi64(bytesUsed, naParseBufferDecimalUnsignedInteger(iter, &intValue, maxDigitCount, naCasti64Tou64(limit)));
  *retValuei = naMuli64(sign, naCastu64Toi64(intValue));
  return bytesUsed;
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
