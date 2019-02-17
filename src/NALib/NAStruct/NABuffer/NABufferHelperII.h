
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


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
  return (buffer->flags & NA_BUFFER_FLAG_RANGE_FIXED);
}



NA_IDEF void naFixBufferRange(NABuffer* buffer){
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;
}



NA_IDEF void naExtendBufferRange(NABuffer* buffer, NAInt bytesatstart, NAInt bytesatend){
  #ifndef NDEBUG
    if(bytesatstart < 0)
      naError("bytesatstart should not be negative");
    if(bytesatend < 0)
      naError("bytesatend should not be negative");
  #endif
  naEnsureBufferRange(buffer, buffer->range.origin - bytesatstart, naGetRangeiEnd(buffer->range) + bytesatend);
}



NA_IDEF NANewlineEncoding naGetBufferNewlineEncoding(NABuffer* buffer){
  return buffer->newlineencoding;
}



NA_IDEF void naSetBufferNewlineEncoding(NABuffer* buffer, NANewlineEncoding newlineencoding){
  buffer->newlineencoding = newlineencoding;
}



NA_IDEF NAInt naGetBufferEndianness(NABuffer* buffer){
  return buffer->endianness;
}



NA_IDEF void naSetBufferEndianness(NABuffer* buffer, NAInt endianness){
  buffer->endianness = endianness;
}



NA_IDEF NAUInt naGetBufferLineNumber(NABufferIterator* iter){
  return iter->linenum;
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
