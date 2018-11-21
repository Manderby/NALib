
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"

// /////////////////////////////////////
// Whole Buffer Functions
// /////////////////////////////////////

NA_DEF NAString* naNewStringWithBufferBase64Encoded(const NABuffer* buffer, NABool appendendsign){
  NA_UNUSED(buffer);
  NA_UNUSED(appendendsign);
//  NAInt totalbytesize;
//  NAInt triples;
//  NAInt remainder;
//  NABuffer* dstbuffer;
//  NABufferIterator dstiter;
//  NAByte      srctriple[3];
//  NAUTF8Char  dsttriple[4];
//  NABufferIterator srciter;
//  NAString* retstring;
//  #ifndef NDEBUG
//    if(!naHasBufferFixedRange(buffer))
//      naError("naNewStringWithBufferBase64Encoded", "Buffer has no determined range. Use naFixBufferRange");
//  #endif
//  totalbytesize = buffer->range.length;
//  triples = totalbytesize / 3;
//  remainder = totalbytesize % 3;
//
//  dstbuffer = naNewBuffer(NA_FALSE);
//  dstiter = naMakeBufferModifier(dstbuffer);
//
//  srciter = naMakeBufferAccessor(buffer);
//  
//  while(triples){
//    naReadBufferBytes(&srciter, srctriple, 3);
//    dsttriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
//    dsttriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4) | (NAUTF8Char)(srctriple[1] >> 4);
//    dsttriple[2] = (NAUTF8Char)((srctriple[1] & 0x0f) << 2) | (NAUTF8Char)(srctriple[2] >> 6);
//    dsttriple[3] = (NAUTF8Char) (srctriple[2] & 0x3f);
//    naWriteBufferBytes(&dstiter, dsttriple, 4);
//    triples--;
//  }
//  if(remainder == 1){
//    naReadBufferBytes(&srciter, srctriple, 1);
//    dsttriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
//    dsttriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4);
//    naWriteBufferBytes(&dstiter, dsttriple, 2);
//  }
//  if(remainder == 2){
//    naReadBufferBytes(&srciter, srctriple, 2);
//    dsttriple[0] = (NAUTF8Char) (srctriple[0] >> 2);
//    dsttriple[1] = (NAUTF8Char)((srctriple[0] & 0x03) << 4) | (NAUTF8Char)(srctriple[1] >> 4);
//    dsttriple[2] = (NAUTF8Char)((srctriple[1] & 0x0f) << 2);
//    naWriteBufferBytes(&dstiter, dsttriple, 3);
//  }
//  
//  naClearBufferIterator(&srciter);
//  naClearBufferIterator(&dstiter);  
//
//  dstiter = naMakeBufferModifier(dstbuffer);
//  while(naIterateBuffer(&dstiter, 1)){
//    NAUTF8Char curchar = (NAUTF8Char)naGetBufferu8(&dstiter);
//    NAUTF8Char newchar;
//    if      (curchar < 26) {newchar = curchar + 'A';}
//    else if (curchar < 52) {newchar = curchar + ('a' - 26);}
//    else if (curchar < 62) {newchar = curchar + ('0' - 52);}
//    else if (curchar == 62){newchar = '+';}
//    else                   {newchar = '/';}
//    naSetBufferu8(&dstiter, (uint8)newchar);
//  }
//  if(appendendsign && remainder == 1){
//    naWriteBufferu8(&dstiter, '=');
//    naWriteBufferu8(&dstiter, '=');
//  }
//  if(appendendsign && remainder == 2){
//    naWriteBufferu8(&dstiter, '=');
//  }
//
//  naClearBufferIterator(&dstiter);  
//  retstring = naNewStringWithBufferExtraction(dstbuffer, dstbuffer->range);
//  naRelease(dstbuffer);
//  return retstring;
  return NA_NULL;
}



NA_DEF NABuffer* naNewBufferWithStringBase64Decoded(const NAString* string){
  NA_UNUSED(string);
//  NAInt totalcharsize;
//  NAInt triples;
//  NAInt remainder;
//  NABuffer* dstbuffer;
//  NABufferIterator dstiter;
//  NAUTF8Char  asctriple[4];
//  NAByte      dsttriple[3];
//  NABufferIterator srciter = naMakeBufferAccessor(naGetStringBufferConst(string));
//  NABuffer* ascbuffer = naNewBuffer(NA_FALSE);
//  NABufferIterator asciter = naMakeBufferModifier(ascbuffer);
//  
//  while(naIterateBuffer(&srciter, 1)){
//    NAUTF8Char curchar = (NAUTF8Char)naGetBufferu8(&srciter);
//    NAUTF8Char newchar;
//    if      (curchar == '+'){newchar = 62;}
//    else if (curchar == '/'){newchar = 63;}
//    else if (curchar <= '9'){newchar = curchar - ('0' - 52);}
//    else if (curchar == '='){break;}
//    else if (curchar <= 'Z'){newchar = curchar - 'A';}
//    else if (curchar <= 'z'){newchar = curchar - ('a' - 26);}
//    else{
//      #ifndef NDEBUG
//        naError("naNewBufferWithStringBase64Decoded", "Invalid character. This does not seem to be a Base64 encoding");
//      #endif
//      newchar = '\0';
//    }
//    naWriteBufferu8(&asciter, (uint8)newchar);
//  }
//
//  naClearBufferIterator(&srciter);
//  naClearBufferIterator(&asciter);
//
//  totalcharsize = ascbuffer->range.length;
//  triples = totalcharsize / 4;
//  remainder = totalcharsize % 4;
//  
//  dstbuffer = naNewBuffer(NA_FALSE);
//  dstiter = naMakeBufferModifier(dstbuffer);
//  
//  asciter = naMakeBufferAccessor(ascbuffer);
//
//  while(triples){
//    naReadBufferBytes(&asciter, asctriple, 4);
//    dsttriple[0] = (NAByte) (asctriple[0] << 2)         | (NAByte)(asctriple[1] >> 4);
//    dsttriple[1] = (NAByte)((asctriple[1] & 0x0f) << 4) | (NAByte)(asctriple[2] >> 2);
//    dsttriple[2] = (NAByte)((asctriple[2] & 0x03) << 6) | (NAByte)(asctriple[3]);
//    triples--;
//    naWriteBufferBytes(&dstiter, dsttriple, 3);
//  }
//  #ifndef NDEBUG
//  if(remainder == 1)
//    naError("naNewBufferWithStringBase64Decoded", "This remainder should not happen");
//  #endif
//  if(remainder == 2){
//    naReadBufferBytes(&asciter, asctriple, 2);
//    dsttriple[0] = (NAByte) (asctriple[0] << 2)         | (NAByte)(asctriple[1] >> 4);
//    #ifndef NDEBUG
//    if((asctriple[1] & 0x0f) << 4)
//      naError("naNewBufferWithStringBase64Decoded", "Security breach: Data in unobserved bits of second character");
//    #endif
//    naWriteBufferBytes(&dstiter, dsttriple, 1);
//  }
//  if(remainder == 3){
//    naReadBufferBytes(&asciter, asctriple, 3);
//    dsttriple[0] = (NAByte) (asctriple[0] << 2)         | (NAByte)(asctriple[1] >> 4);
//    dsttriple[1] = (NAByte)((asctriple[1] & 0x0f) << 4) | (NAByte)(asctriple[2] >> 2);
//    #ifndef NDEBUG
//    if((asctriple[2] & 0x03) << 6)
//      naError("naNewBufferWithStringBase64Decoded", "Security breach: Data in unobserved bits of third character");
//    #endif
//    naWriteBufferBytes(&dstiter, dsttriple, 2);
//  }
//
//  naClearBufferIterator(&asciter);
//  naClearBufferIterator(&dstiter);
//  naRelease(ascbuffer);
//  return dstbuffer;
  return NA_NULL;
}



NA_DEF void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum){
  NA_UNUSED(buffer);
  NA_UNUSED(checksum);
//  NAInt bytesize;
//  NAInt curoffset;
//  NAListIterator iter;
//
//  bytesize = buffer->range.length;
//  if(bytesize){
//    curoffset = buffer->range.origin;
//    iter = naMakeListMutator(&(buffer->parts));
//    naLocateListFirst(&iter);
//    
//    while(bytesize){
//      NABufferPart* curpart;
//      NAInt remainingbytes;
//      NAByte* src;
//
//      curpart = naGetListCurMutable(&iter);
//      remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
//      src = naGetBufferPartDataPointerConst(curpart, curoffset);
//      
//      #ifndef NDEBUG
//        if(naIsBufferPartSparse(curpart))
//          naError("naAccumulateBufferToChecksum", "Buffer contains sparse parts. Can not compute checksum");
//      #endif
//      
//      if(bytesize > remainingbytes){
//        naAccumulateChecksum(checksum, src, remainingbytes);
//        naIterateList(&iter);
//        curoffset += remainingbytes;
//        bytesize -= remainingbytes;
//      }else{
//        naAccumulateChecksum(checksum, src, bytesize);
//        bytesize = 0;
//      }
//    }
//    
//    naClearListIterator(&iter);
//  }
  return;
}



NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file){
  NA_UNUSED(buffer);
  NA_UNUSED(file);
//  NAInt bytesize;
//  NAInt curoffset;
//  NAListIterator iter;
//
//  #ifndef NDEBUG
//    if(!naHasBufferFixedRange(buffer))
//      naError("naWriteBufferToFile", "Buffer has no determined range. Use naFixBufferRange");
//  #endif
//
//  bytesize = buffer->range.length;
//  if(bytesize){
//    curoffset = buffer->range.origin;
//    iter = naMakeListMutator(&(buffer->parts));
//    naLocateListFirst(&iter);
//    
//    while(bytesize){
//      NABufferPart* curpart;
//      NAInt remainingbytes;
//      NAByte* src;
//
//      curpart = naGetListCurMutable(&iter);
//      remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
//      src = naGetBufferPartDataPointerConst(curpart, curoffset);
//      
//      #ifndef NDEBUG
//        if(naIsBufferPartSparse(curpart))
//          naError("naWriteBufferToFile", "Buffer contains sparse parts.");
//      #endif
//      
//      if(bytesize > remainingbytes){
//        naWriteFileBytes(file, src, remainingbytes);
//        naIterateList(&iter);
//        curoffset += remainingbytes;
//        bytesize -= remainingbytes;
//      }else{
//        naWriteFileBytes(file, src, bytesize);
//        bytesize = 0;
//      }
//    }
//    
//    naClearListIterator(&iter);
//  }
  return;
}



NA_DEF void naWriteBufferToData(NABuffer* buffer, void* data){
  NA_UNUSED(buffer);
  NA_UNUSED(data);
//  NAInt bytesize;
//  NAInt curoffset;
//  NAListIterator iter;
//  NAByte* dst = data;
//
//  bytesize = buffer->range.length;
//  if(bytesize){
//    curoffset = buffer->range.origin;
//    iter = naMakeListMutator(&(buffer->parts));
//    naLocateListFirst(&iter);
//    
//    while(bytesize){
//      NABufferPart* curpart;
//      NAInt remainingbytes;
//      NAByte* src;
//
//      curpart = naGetListCurMutable(&iter);
//      remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
//      src = naGetBufferPartDataPointerConst(curpart, curoffset);
//      
//      #ifndef NDEBUG
//        if(naIsBufferPartSparse(curpart))
//          naError("naWriteBufferToFile", "Buffer contains sparse parts.");
//      #endif
//      
//      if(bytesize > remainingbytes){
//        naCopyn(dst, src, remainingbytes);
//        naIterateList(&iter);
//        curoffset += remainingbytes;
//        bytesize -= remainingbytes;
//        dst += remainingbytes;
//      }else{
//        naCopyn(dst, src, bytesize);
//        bytesize = 0;
//      }
//    }
//    
//    naClearListIterator(&iter);
//  }
  return;
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
