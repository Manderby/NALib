
// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABuffer.h"



#define NA_BUFFER_FLAG_RANGE_FIXED       0x01



NA_IDEF NABool naIsBufferEmpty(const NABuffer* buffer){
  return naIsRangeiEmpty(buffer->range);
}



NA_IDEF NARangei naGetBufferRange(const NABuffer* buffer){
  return buffer->range;
}



NA_IDEF NABool naHasBufferFixedRange(const NABuffer* buffer){
  return (NABool)(buffer->flags & NA_BUFFER_FLAG_RANGE_FIXED);
}



NA_IDEF void naFixBufferRange(NABuffer* buffer){
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;
}



NA_IDEF void naExtendBufferRange(NABuffer* buffer, NAInt bytesAtStart, NAInt bytesAtEnd){
  #ifndef NDEBUG
    if(bytesAtStart < 0)
      naError("bytesAtStart should not be negative");
    if(bytesAtEnd < 0)
      naError("bytesAtEnd should not be negative");
  #endif
  na_EnsureBufferRange(buffer, buffer->range.origin - bytesAtStart, naGetRangeiEnd(buffer->range) + bytesAtEnd);
}



NA_IDEF NANewlineEncoding naGetBufferNewlineEncoding(NABuffer* buffer){
  return buffer->newlineEncoding;
}



NA_IDEF void naSetBufferNewlineEncoding(NABuffer* buffer, NANewlineEncoding newlineEncoding){
  buffer->newlineEncoding = newlineEncoding;
}



NA_IDEF NAInt naGetBufferEndianness(NABuffer* buffer){
  return buffer->endianness;
}



NA_IDEF void naSetBufferEndianness(NABuffer* buffer, NAInt endianness){
  buffer->endianness = endianness;
}



NA_IDEF NAUInt naGetBufferLineNumber(NABufferIterator* iter){
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
