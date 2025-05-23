
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



struct NABufferIterator{
  NAPtr bufferPtr;
  NATreeIterator partIter;
  int64 partOffset; // The current byte offset in the referenced part.
  uint8 curBit;     // The current bit number
  size_t lineNum;   // The line number, starting with 1 after first line read.
};



NA_HIDEF const NABuffer* na_GetBufferIteratorBufferConst(const NABufferIterator* iter) {
  return (const NABuffer*)naGetPtrConst(iter->bufferPtr);
}



NA_HIDEF NABuffer* na_GetBufferIteratorBufferMutable(NABufferIterator* iter) {
  return (NABuffer*)naGetPtrMutable(iter->bufferPtr);
}



NA_IDEF NABool naLocateBufferRelative(NABufferIterator* iter, int64 offset) {
  int64 absPos = naGetBufferLocation(iter);
  return naLocateBufferAbsolute(iter, naAddi64(absPos, offset));
}



NA_IDEF NABool naLocateBufferAtStart(NABufferIterator* iter, int64 offset) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  return naLocateBufferAbsolute(iter, naAddi64(buffer->range.origin, offset));
}



NA_IDEF NABool naLocateBufferAtEnd(NABufferIterator* iter, int64 offset) {
  const NABuffer* buffer = na_GetBufferIteratorBufferConst(iter);
  return naLocateBufferAbsolute(iter, naAddi64(naGetRangei64End(buffer->range), offset));
}



NA_HIDEF int64 na_GetBufferIteratorPartOffset(NABufferIterator* iter) {
  return iter->partOffset;
}



NA_HIDEF NABufferPart* na_GetBufferPart(NABufferIterator* iter) {
  return (NABufferPart*)naGetTreeCurLeafMutable(&iter->partIter);
}



NA_IDEF NABool naIsBufferAtInitial(NABufferIterator* iter) {
  return naIsTreeAtInitial(&iter->partIter);
}



NA_IDEF NABool naIsBufferAtEnd(NABufferIterator* iter) {
  NABuffer* buffer = na_GetBufferIteratorBufferMutable(iter);
  if(!naIsBufferAtInitial(iter)) {
    NABufferPart* part = na_GetBufferPart(iter);
    if(naSmalleri64(iter->partOffset, NA_ZERO_i64) || naGreaterEquali64(iter->partOffset, naCastSizeToi64(na_GetBufferPartByteSize(part)))) {
      NABool found = naLocateBufferAbsolute(iter, naGetBufferLocation(iter));
      if(!found) {
        naResetTreeIterator(&iter->partIter);
        iter->partOffset = naGetRangei64End(buffer->range);
      }
    }
  }
  return naEquali64(buffer->range.length, NA_ZERO_i64)
    || (naIsBufferAtInitial(iter) && (naEquali64(iter->partOffset, naGetRangei64End(buffer->range))));
}



NA_IDEF uint8 naGetBufferCurBit(NABufferIterator* iter) {
  return iter->curBit;
}



NA_IDEF size_t naGetBufferLineNumber(NABufferIterator* iter) {
  return iter->lineNum;
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
