
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



NA_IDEF int8 naParseBufferi8(NABufferIterator* iter, NABool skipdelimiter){
  NAi64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakei64WithLo(NA_MIN_i8), naMakei64WithLo(NA_MAX_i8));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCasti64Toi8(intvalue);
}
NA_IDEF int16 naParseBufferi16(NABufferIterator* iter, NABool skipdelimiter){
  NAi64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakei64WithLo(NA_MIN_i16), naMakei64WithLo(NA_MAX_i16));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCasti64Toi16(intvalue);
}
NA_IDEF int32 naParseBufferi32(NABufferIterator* iter, NABool skipdelimiter){
  NAi64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakei64WithLo(NA_MIN_i32), naMakei64WithLo(NA_MAX_i32));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCasti64Toi32(intvalue);
}
NA_IDEF NAi64 naParseBufferi64(NABufferIterator* iter, NABool skipdelimiter){
  NAi64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, NA_MIN_i64, NA_MAX_i64);
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return intvalue;
}



NA_IDEF uint8 naParseBufferu8(NABufferIterator* iter, NABool skipdelimiter){
  NAu64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeu64WithLo(NA_MAX_u8));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastu64Tou8(uintvalue);
}
NA_IDEF uint16 naParseBufferu16(NABufferIterator* iter, NABool skipdelimiter){
  NAu64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeu64WithLo(NA_MAX_u16));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastu64Tou16(uintvalue);
}
NA_IDEF uint32 naParseBufferu32(NABufferIterator* iter, NABool skipdelimiter){
  NAu64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeu64WithLo(NA_MAX_u32));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastu64Tou32(uintvalue);
}
NA_IDEF NAu64 naParseBufferu64(NABufferIterator* iter, NABool skipdelimiter){
  NAu64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, NA_MAX_u64);
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return uintvalue;
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
