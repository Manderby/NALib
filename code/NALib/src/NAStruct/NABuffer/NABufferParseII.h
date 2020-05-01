
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABuffer.h
// Do not include this file directly! It will automatically be included when
// including "NABufferII.h"



NA_IDEF int8 naParseBufferInt8(NABufferIterator* iter, NABool skipdelimiter){
  NAInt64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakeInt64WithLo(NA_MIN_8), naMakeInt64WithLo(NA_MAX_8));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastInt64ToInt8(intvalue);
}
NA_IDEF int16 naParseBufferInt16(NABufferIterator* iter, NABool skipdelimiter){
  NAInt64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakeInt64WithLo(NA_MIN_16), naMakeInt64WithLo(NA_MAX_16));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastInt64ToInt16(intvalue);
}
NA_IDEF int32 naParseBufferInt32(NABufferIterator* iter, NABool skipdelimiter){
  NAInt64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, naMakeInt64WithLo(NA_MIN_32), naMakeInt64WithLo(NA_MAX_32));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastInt64ToInt32(intvalue);
}
NA_IDEF NAInt64 naParseBufferInt64(NABufferIterator* iter, NABool skipdelimiter){
  NAInt64 intvalue;
  naParseBufferDecimalSignedInteger(iter, &intvalue, 0, NA_MIN_64, NA_MAX_64);
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return intvalue;
}



NA_IDEF uint8 naParseBufferUInt8(NABufferIterator* iter, NABool skipdelimiter){
  NAUInt64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeUInt64WithLo(NA_MAX_8u));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastUInt64ToUInt8(uintvalue);
}
NA_IDEF uint16 naParseBufferUInt16(NABufferIterator* iter, NABool skipdelimiter){
  NAUInt64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeUInt64WithLo(NA_MAX_16u));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastUInt64ToUInt16(uintvalue);
}
NA_IDEF uint32 naParseBufferUInt32(NABufferIterator* iter, NABool skipdelimiter){
  NAUInt64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, naMakeUInt64WithLo(NA_MAX_32u));
  if(skipdelimiter){naSkipBufferDelimiter(iter);}
  return naCastUInt64ToUInt32(uintvalue);
}
NA_IDEF NAUInt64 naParseBufferUInt64(NABufferIterator* iter, NABool skipdelimiter){
  NAUInt64 uintvalue;
  naParseBufferDecimalUnsignedInteger(iter, &uintvalue, 0, NA_MAX_64u);
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
