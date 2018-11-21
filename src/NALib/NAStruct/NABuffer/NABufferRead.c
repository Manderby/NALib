
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"


NA_DEF NAByte naGetBufferByteAtIndex(const NABuffer* buffer, NAInt indx){
  NA_UNUSED(buffer);
  NA_UNUSED(indx);
//  NAListIterator iter;
//  
//  const NAByte* retbyte = NA_NULL;
//  if(naIsBufferEmpty(buffer)){return '\0';}
//  
//  iter = naMakeListAccessor(&(buffer->parts));
//  naLocateListLast(&iter);
////  naLocateListPosition(&iter, naGetListCurPosition(&(buffer->iter)));
//  naLocateBufferPartOffset(&iter, indx);
//
//  if(!naIsListAtInitial(&iter)){
//    retbyte = naGetBufferPartDataPointerConst(naGetListCurConst(&iter), indx);
//  }
//  naClearListIterator(&iter);
//  return retbyte ? (*retbyte) : '\0';
  return 0;
}



NA_HDEF void naRetrieveBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize, NABool advance){
  NA_UNUSED(iter);
  NA_UNUSED(data);
  NA_UNUSED(bytesize);
  NA_UNUSED(advance);
//  NAByte* dst = data;
//  NAByte* src;
//
//  #ifndef NDEBUG
//    if(!data)
//      naError("naRetrieveBufferBytes", "data is Null pointer.");
//    if(iter->curbit != 0)
//      naError("naRetrieveBufferBytes", "Bit offset not 0.");
//  #endif
//  
//  // We prepare the buffer for the whole range. There might be no parts or
//  // sparse parts.
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, bytesize), NA_FALSE, NA_TRUE);
//  // After this function, all relevant parts should be present and filled with
//  // memory. The iterator should point to the buffer part containing offset.
//  
//  // do as long as there is a bytesize remaining. Remember that the data may
//  // be split into different buffer parts.
//  while(bytesize){
//    NABufferPart* part;
//    NAInt possiblelength;
//    
//    // The part pointed to by the iterator should be the one containing offset.
//    part = naGetListCurMutable(&(iter->partiter));
//    
//    // Reaching this point, we are sure, the current part contains offset and
//    // is filled with memory.
//    #ifndef NDEBUG
//      if(!part)
//        naError("naPutBufferBytes", "No more parts containing memory");
//      if(naIsBufferPartSparse(part))
//        naError("naPutBufferBytes", "Cur part is sparse");
////      if(!naContainsBufferPartOffset(part, iter->curoffset))
////        naError("naPutBufferBytes", "Cur part does not contain current offset");
//    #endif
//    
//    // We get the data pointer where we can read bytes.
//    src = naGetBufferPartDataPointerConst(part, iter->curoffset);
//    // We detect, how many bytes actually can be read from the current part.
//    possiblelength = naGetBufferPartEnd(part) - iter->curoffset;
//
//    #ifndef NDEBUG
//      if(possiblelength <= 0)
//        naError("naPutBufferBytes", "possible length invalid");
//      if(possiblelength > naGetBufferPartRange(part).length)
//        naError("naPutBufferBytes", "buffer overflow expected");
//    #endif
//    
//    if(possiblelength > bytesize){
//      // If we can get out more bytes than needed, we copy all remaining bytes
//      // and stay on this part.
//      naCopyn(dst, src, bytesize);
//      
//      if(advance){
//        iter->curoffset += bytesize;
//      }
//      
//      // dst += bytesize; // this line is not needed as the loop will end.
//      bytesize = 0;
//    }else{
//      // We can only get a portion of the current part into the destination. We
//      // copy as many bytes as possible and advance to the next part.
//      naCopyn(dst, src, possiblelength);
//
//      if(advance){
//        // Note that when possiblelength matches bytesize, the part will also
//        // advance which is correct behaviour, as after this function, the
//        // iterator shall always point at the part containing the current offset.
//        naIterateList(&(iter->partiter));
//        iter->curoffset += possiblelength;
//      }
//
//      dst += possiblelength;
//      bytesize -= possiblelength;
//    }
//  }
//  
//  // Reaching here, the whole range has been written to the parts and both
//  // curoffset and iter point to the current position again.
  return;
}



// ////////////////////////////////////
// GETTER
// ////////////////////////////////////



NA_DEF int8 naGetBufferi8(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int8 value;
//  naRetrieveBufferBytes(iter, &value, 1, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//  return value;
  return 0;
}
NA_DEF int16 naGetBufferi16(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int16 value;
//  naRetrieveBufferBytes(iter, &value, 2, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  return value;
  return 0;
}
NA_DEF int32 naGetBufferi32(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int32 value;
//  naRetrieveBufferBytes(iter, &value, 4, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  return value;
  return 0;
}
NA_DEF int64 naGetBufferi64(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int64 value;
//  naRetrieveBufferBytes(iter, &value, 8, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  return value;
  return 0;
}



NA_DEF uint8 naGetBufferu8(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint8 value;
//  naRetrieveBufferBytes(iter, &value, 1, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//  return value;
  return 0;
}
NA_DEF uint16 naGetBufferu16(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint16 value;
//  naRetrieveBufferBytes(iter, &value, 2, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  return value;
  return 0;
}
NA_DEF uint32 naGetBufferu32(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint32 value;
//  naRetrieveBufferBytes(iter, &value, 4, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  return value;
  return 0;
}
NA_DEF uint64 naGetBufferu64(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint64 value;
//  naRetrieveBufferBytes(iter, &value, 8, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  return value;
  return 0;
}



NA_DEF float naGetBufferf(NABufferIterator* iter){
  NA_UNUSED(iter);
//  float value;
//  naRetrieveBufferBytes(iter, &value, 4, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  return value;
  return 0;
}
NA_DEF double naGetBufferd(NABufferIterator* iter){
  NA_UNUSED(iter);
//  double value;
//  naRetrieveBufferBytes(iter, &value, 8, NA_FALSE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  return value;
  return 0;
}



// ///////////////////////////////////////
// READING
// ///////////////////////////////////////

NA_DEF void naReadBufferBytes(NABufferIterator* iter, void* data, NAInt bytesize){
  NA_UNUSED(iter);
  NA_UNUSED(data);
  NA_UNUSED(bytesize);
//  naRetrieveBufferBytes(iter, data, bytesize, NA_TRUE);
  return;
}



NA_DEF int8 naReadBufferi8(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int8 value;
//  naRetrieveBufferBytes(iter, &value, 1, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//  return value;
  return 0;
}
NA_DEF int16 naReadBufferi16(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int16 value;
//  naRetrieveBufferBytes(iter, &value, 2, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  return value;
  return 0;
}
NA_DEF int32 naReadBufferi32(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int32 value;
//  naRetrieveBufferBytes(iter, &value, 4, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  return value;
  return 0;
}
NA_DEF int64 naReadBufferi64(NABufferIterator* iter){
  NA_UNUSED(iter);
//  int64 value;
//  naRetrieveBufferBytes(iter, &value, 8, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  return value;
  return 0;
}


NA_DEF uint8 naReadBufferu8(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint8 value;
//  naRetrieveBufferBytes(iter, &value, 1, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//  return value;
  return 0;
}
NA_DEF uint16 naReadBufferu16(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint16 value;
//  naRetrieveBufferBytes(iter, &value, 2, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  return value;
  return 0;
}
NA_DEF uint32 naReadBufferu32(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint32 value;
//  naRetrieveBufferBytes(iter, &value, 4, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  return value;
  return 0;
}
NA_DEF uint64 naReadBufferu64(NABufferIterator* iter){
  NA_UNUSED(iter);
//  uint64 value;
//  naRetrieveBufferBytes(iter, &value, 8, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  return value;
  return 0;
}


NA_DEF float naReadBufferf(NABufferIterator* iter){
  NA_UNUSED(iter);
//  float value;
//  naRetrieveBufferBytes(iter, &value, 4, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  return value;
  return 0;
}
NA_DEF double naReadBufferd(NABufferIterator* iter){
  NA_UNUSED(iter);
//  double value;
//  naRetrieveBufferBytes(iter, &value, 8, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  return value;
  return 0;
}



NA_DEF void naReadBufferi8v(NABufferIterator* iter, int8* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 1, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert8v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferi16v(NABufferIterator* iter, int16* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 2, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert16v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferi32v(NABufferIterator* iter, int32* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferi64v(NABufferIterator* iter, int64* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64v(dst, (NAUInt)count);
  return;
}


NA_DEF void naReadBufferu8v(NABufferIterator* iter, uint8* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 1, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert8v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferu16v(NABufferIterator* iter, uint16* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 2, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert16v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferu32v(NABufferIterator* iter, uint32* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferu64v(NABufferIterator* iter, uint64* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64v(dst, (NAUInt)count);
  return;
}


NA_DEF void naReadBufferfv(NABufferIterator* iter, float* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 4, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert32v(dst, (NAUInt)count);
  return;
}
NA_DEF void naReadBufferdv(NABufferIterator* iter, double* dst, NAInt count){
  NA_UNUSED(iter);
  NA_UNUSED(dst);
  NA_UNUSED(count);
//  naRetrieveBufferBytes(iter, dst, count * 8, NA_TRUE);
//  naGetBufferIteratorBufferConst(iter)->converter.convert64v(dst, (NAUInt)count);
  return;
}


// /////////////////////////////////
// Bit manipulation
// /////////////////////////////////

NA_DEF NABool naReadBufferBit(NABufferIterator* iter){
  NA_UNUSED(iter);
//  NAByte* src;
//  NABool bit;
//  NABufferPart* part;
//
//  if(iter->curbit == 0){
//    naPrepareBuffer(iter, naMakeRangei(iter->curoffset, 1), NA_FALSE, NA_TRUE);
//  }
//  
//  // After this function, all parts should be present and filled with memory.
//  // The iterator should point to the buffer part containing offset.
//  part = naGetListCurMutable(&(iter->partiter));
//  src = naGetBufferPartDataPointerConst(part, iter->curoffset);
//  bit = (*src >> iter->curbit) & 0x01;
//  
//  iter->curbit++;
//  if(iter->curbit == 8){
//    NAInt possiblelength;
//    
//    // We advance to the next byte.
//    iter->curbit = 0;
//    iter->curoffset++;
//    possiblelength = naGetBufferPartEnd(part) - iter->curoffset;
//    if(!possiblelength){
//      naIterateList(&(iter->partiter));
//    }
//  }
//  
//  return bit;
  return NA_FALSE;
}



NA_DEF NAUInt naReadBufferBits(NABufferIterator* iter, uint8 count){
  NA_UNUSED(iter);
  NA_UNUSED(count);
//  NAUInt retint = 0;
//  NABool curbit;
//  NAUInt curmask = 1;
//  #ifndef NDEBUG
//    if(count > NA_TYPE_NAINT_BITS)
//      naError("naReadBufferBits", "Max bit readable per function call exceeded.");
//  #endif
//  while(count){
//    curbit = naReadBufferBit(iter);
//    retint |= curmask * (NAUInt)curbit;
//    curmask <<= 1;
//    count--;
//  }
//  return retint;
  return 0;
}



NA_DEF void naPadBufferBits(NABufferIterator* iter){
  NA_UNUSED(iter);
//  NABufferPart* part;
//  NAInt possiblelength;
//
//  if(iter->curbit != 0){
//    iter->curbit = 0;
//    iter->curoffset++;
//    part = naGetListCurMutable(&(iter->partiter));
//    possiblelength = naGetBufferPartEnd(part) - iter->curoffset;
//    if(!possiblelength){
//      naIterateList(&(iter->partiter));
//    }
//  }
  return;
}




NA_DEF NABuffer* naReadBufferBuffer(NABufferIterator* iter, NAInt bytesize){
  NA_UNUSED(iter);
  NA_UNUSED(bytesize);
//  return naNewBufferExtraction(naGetBufferIteratorBufferMutable(iter), naMakeRangei(iter->curoffset, bytesize));
  return NA_NULL;
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
