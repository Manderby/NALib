int dummy;

//
//// This file is part of NALib, a collection of C source code.
//// Full license notice at the bottom.
//
//#include "NABuffer.h"
//
//
//// This is the internal function actually preparing and storing the bytes
//// delivered in the parameters.
//NA_HDEF void naStoreBufferBytes(NABufferIterator* iter, const void* data, NAInt bytesize, NABool prepare, NABool advance){
//  const NAByte* src = data;
//  NAByte* dst;
//
//  // We prepare the buffer for the whole range. There might be no parts or
//  // sparse parts.
//  if(prepare){
//    if(naIsBufferEmpty(naGetBufferIteratorBufferConst(iter))){
//      iter->curoffset = 0;
//    }else if(naIsBufferAtInitial(iter)){
//      iter->curoffset = naGetRangeiEnd(naGetBufferIteratorBufferConst(iter)->range);
//    }
//    naPrepareBuffer(iter, naMakeRangei(iter->curoffset, bytesize), NA_FALSE, NA_TRUE);
//  }
//  // After this function, all parts should be present and filled with memory.
//  // The iterator should point to the buffer part containing offset.
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
//      if(naIsBufferPartSparse(part))
//        naError("naPutBufferBytes", "Cur part is sparse");
//      if(!naContainsBufferPartOffset(part, iter->curoffset))
//        naError("naPutBufferBytes", "Cur part does not contain current offset");
//    #endif
//    
//    // We get the data pointer where we can write bytes.
//    dst = naGetBufferPartDataPointerMutable(part, iter->curoffset);
//    // We detect, how many bytes actually can be put into the current part.
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
//      // If we can put in more bytes than needed, we copy all remaining bytes
//      // and stay on this part.
//      naCopyn(dst, src, bytesize);
//      
//      if(advance){
//        iter->curoffset += bytesize;
//      }
//      
//      // src += bytesize; // this line is not needed as the loop will end.
//      bytesize = 0;
//    }else{
//      // We can only put a portion of the source into the current part. We
//      // copy as many bytes as possible and advance to the next part.
//      naCopyn(dst, src, possiblelength);
//
//      if(advance){
//        iter->curoffset += possiblelength;
//        // Note that when possiblelength matches bytesize, the part will also
//        // advance which is correct behaviour, as after this function, the
//        // iterator shall always point at the part containing the current offset.
//        naIterateList(&(iter->partiter));
//      }
//
//      src += possiblelength;
//      bytesize -= possiblelength;
//    }
//  }
//  
//  // Reaching here, the whole range has been written to the parts and both
//  // curoffset and iter point to the current position again.
//}
//
//
//
//
//// ////////////////////////////////////
//// SETTER
//// ////////////////////////////////////
//
//
//
//NA_DEF void naSetBufferi8(NABufferIterator* iter, int8 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//  naStoreBufferBytes(iter, &value, 1, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferi16(NABufferIterator* iter, int16 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  naStoreBufferBytes(iter, &value, 2, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferi32(NABufferIterator* iter, int32 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  naStoreBufferBytes(iter, &value, 4, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferi64(NABufferIterator* iter, int64 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  naStoreBufferBytes(iter, &value, 8, NA_TRUE, NA_FALSE);
//}
//
//
//
//NA_DEF void naSetBufferu8(NABufferIterator* iter, uint8 value){
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  buffer->converter.convert8(&value);
//  naStoreBufferBytes(iter, &value, 1, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferu16(NABufferIterator* iter, uint16 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  naStoreBufferBytes(iter, &value, 2, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferu32(NABufferIterator* iter, uint32 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  naStoreBufferBytes(iter, &value, 4, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferu64(NABufferIterator* iter, uint64 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  naStoreBufferBytes(iter, &value, 8, NA_TRUE, NA_FALSE);
//}
//
//
//NA_DEF void naSetBufferf(NABufferIterator* iter, float value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  naStoreBufferBytes(iter, &value, 4, NA_TRUE, NA_FALSE);
//}
//NA_DEF void naSetBufferd(NABufferIterator* iter, double value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  naStoreBufferBytes(iter, &value, 8, NA_TRUE, NA_FALSE);
//}
//
//
//
//
//// ////////////////////////////////////
//// WRITING
//// ////////////////////////////////////
//
//
//
//
//
//NA_DEF void naWriteBufferBytes(NABufferIterator* iter, const void* data, NAInt bytesize){
//  naStoreBufferBytes(iter, data, bytesize, NA_TRUE, NA_TRUE);
//}
//
//
//
//NA_DEF void naWriteBufferi8(NABufferIterator* iter, int8 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//  naStoreBufferBytes(iter, &value, 1, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferi16(NABufferIterator* iter, int16 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  naStoreBufferBytes(iter, &value, 2, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferi32(NABufferIterator* iter, int32 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  naStoreBufferBytes(iter, &value, 4, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferi64(NABufferIterator* iter, int64 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  naStoreBufferBytes(iter, &value, 8, NA_TRUE, NA_TRUE);
//}
//
//
//
//NA_DEF void naWriteBufferu8(NABufferIterator* iter, uint8 value){
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  buffer->converter.convert8(&value);
//  naStoreBufferBytes(iter, &value, 1, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferu16(NABufferIterator* iter, uint16 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//  naStoreBufferBytes(iter, &value, 2, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferu32(NABufferIterator* iter, uint32 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  naStoreBufferBytes(iter, &value, 4, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferu64(NABufferIterator* iter, uint64 value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  naStoreBufferBytes(iter, &value, 8, NA_TRUE, NA_TRUE);
//}
//
//
//
//NA_DEF void naWriteBufferf(NABufferIterator* iter, float value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//  naStoreBufferBytes(iter, &value, 4, NA_TRUE, NA_TRUE);
//}
//NA_DEF void naWriteBufferd(NABufferIterator* iter, double value){
//  naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//  naStoreBufferBytes(iter, &value, 8, NA_TRUE, NA_TRUE);
//}
//
//
//
//NA_DEF void naWriteBufferi8v(NABufferIterator* iter, const int8* src, NAInt count){
//  int8 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 1), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//    naStoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferi16v(NABufferIterator* iter, const int16* src, NAInt count){
//  int16 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 2), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//    naStoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferi32v(NABufferIterator* iter, const int32* src, NAInt count){
//  int32 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 4), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//    naStoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferi64v(NABufferIterator* iter, const int64* src, NAInt count){
//  int64 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 8), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//    naStoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferu8v(NABufferIterator* iter, const uint8* src, NAInt count){
//  uint8 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 1), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert8(&value);
//    naStoreBufferBytes(iter, &value, 1, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferu16v(NABufferIterator* iter, const uint16* src, NAInt count){
//  uint16 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 2), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert16(&value);
//    naStoreBufferBytes(iter, &value, 2, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferu32v(NABufferIterator* iter, const uint32* src, NAInt count){
//  uint32 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 4), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//    naStoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferu64v(NABufferIterator* iter, const uint64* src, NAInt count){
//  uint64 value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 8), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//    naStoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferfv(NABufferIterator* iter, const float* src, NAInt count){
//  float value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 4), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert32(&value);
//    naStoreBufferBytes(iter, &value, 4, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//NA_DEF void naWriteBufferdv(NABufferIterator* iter, const double* src, NAInt count){
//  double value;
//  naPrepareBuffer(iter, naMakeRangei(iter->curoffset, count * 8), NA_FALSE, NA_TRUE);
//  while(count){
//    value = *src;
//    naGetBufferIteratorBufferConst(iter)->converter.convert64(&value);
//    naStoreBufferBytes(iter, &value, 8, NA_FALSE, NA_TRUE);
//    src++;
//    count--;
//  }
//}
//
//
//
//NA_DEF void naWriteBufferBuffer(NABufferIterator* iter, const NABuffer* srcbuffer, NARangei srcrange){
//  NABuffer* dstbuffer;
//  NABufferSource* tmpsource;
//  NAInt tmpsrcoffset;
//  NABuffer* mutablesrcbuffer;
//
//  if(!naIsRangeiEmpty(srcrange)){
//    dstbuffer = naGetBufferIteratorBufferMutable(iter);
//    tmpsource = dstbuffer->source;
//    tmpsrcoffset = dstbuffer->srcoffset;
//    
//    mutablesrcbuffer = (NABuffer*)srcbuffer;
//    
//    dstbuffer->source = naNewBufferSourceWithBuffer(mutablesrcbuffer);
//    
//    if(!naIsBufferEmpty(naGetBufferIteratorBufferConst(iter)) && naIsBufferAtInitial(iter)){
//      iter->curoffset = naGetRangeiEnd(naGetBufferIteratorBufferConst(iter)->range);
//    }
//    dstbuffer->srcoffset = iter->curoffset - srcrange.origin;
//    
//    naCacheBufferRange(dstbuffer, naMakeRangei(iter->curoffset, srcrange.length), NA_FALSE);
//    iter->curoffset += srcrange.length;
//    
//    naRelease(dstbuffer->source);
//    
//    dstbuffer->source = tmpsource;
//    dstbuffer->srcoffset = tmpsrcoffset;
//  }
//}
//
//
//
//NA_DEF void naRepeatBufferBytes(NABufferIterator* iter, NAInt distance, NAInt bytesize){
//  NAInt writeoffset;
//  NAInt readoffset;
//  NABufferPart* writepart;
//  const NABufferPart* readpart;
//  NAInt remainingwrite;
//  NAInt remainingread;
//  NAListIterator readiter;
//
//  // Prepare the write part
//  writeoffset = iter->curoffset;
//  naPrepareBuffer(iter, naMakeRangei(writeoffset, bytesize), NA_FALSE, NA_TRUE);
//
//  // Prepare the read part.
//  readoffset = iter->curoffset - distance;
//  naPrepareBuffer(iter, naMakeRangei(readoffset, bytesize), NA_FALSE, NA_TRUE);
//
//  // Create the read iterator
//  // Important: Do this after the prepare calls as otherwise there might be
//  // an iterator on a part which needs to be removed from a list.
//  readiter = naMakeListAccessor(&(naGetBufferIteratorBufferConst(iter)->parts));
//  naLocateListIterator(&readiter, &(iter->partiter));
//
//  // Reposition the buffer iterator to the write part
//  naLocateBufferPartOffset(&(iter->partiter), iter->curoffset);
//
//  readpart = naGetListCurConst(&readiter);
//  writepart = naGetListCurMutable(&(iter->partiter));
//  
//  // Now start copying the buffers.
//  while(1){
//    NAInt remaining;
//    
//    remainingread = naGetBufferPartEnd(readpart) - readoffset;
//    remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;
//
//    remainingread = naMini(remainingread, distance);
//    remaining = (remainingwrite < remainingread) ? remainingwrite : remainingread;
//    remaining = naMini(remaining, bytesize);
//    
//    naCopyn(naGetBufferPartDataPointerMutable(writepart, writeoffset), naGetBufferPartDataPointerConst(readpart, readoffset), remaining);
//    bytesize -= remaining;
//    writeoffset += remaining;
//    readoffset += remaining;
//    iter->curoffset += remaining;
//
//    remainingread = naGetBufferPartEnd(readpart) - readoffset;
//    remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;
//    
//    if(remainingread == NA_ZERO){
//      naIterateList(&readiter);
//      readpart = naGetListCurConst(&readiter);
//    }
//    if(remainingwrite == NA_ZERO){
//      naIterateList(&(iter->partiter));
//      writepart = naGetListCurMutable(&(iter->partiter));
//    }
//    if(!bytesize){break;}
//  }
//
//  naClearListIterator(&readiter);
//
//}
//
//
//
//NA_DEF void naWriteBufferString(NABufferIterator* iter, const NAString* string){
//  naWriteBufferBuffer(iter, naGetStringBufferConst(string), naGetBufferRange(naGetStringBufferConst(string)));
//}
//
//
//
//NA_DEF void naWriteBufferStringWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...){
//  va_list argumentlist;
//  va_start(argumentlist, format);
//  naWriteBufferStringWithArguments(iter, format, argumentlist);
//  va_end(argumentlist);
//}
//
//
//
//NA_DEF void naWriteBufferStringWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentlist){
//  NAString* string;
//  string = naNewStringWithArguments(format, argumentlist);
//  naWriteBufferString(iter, string);
//  naDelete(string);
//}
//
//
//
//
//NA_DEF void naWriteBufferTab(NABufferIterator* iter){
//  naStoreBufferBytes(iter, NA_TAB, 1, NA_TRUE, NA_TRUE);
//}
//
//
//
//NA_DEF void naWriteBufferNewLine(NABufferIterator* iter){
//  const NABuffer* buffer = naGetBufferIteratorBufferConst(iter);
//  switch(buffer->newlineencoding){
//  case NA_NEWLINE_UNIX:
//    naStoreBufferBytes(iter, NA_NL_UNIX, 1, NA_TRUE, NA_TRUE);
//    break;
//  case NA_NEWLINE_MAC9:
//    naStoreBufferBytes(iter, NA_NL_MAC9, 1, NA_TRUE, NA_TRUE);
//    break;
//  case NA_NEWLINE_WIN:
//    naStoreBufferBytes(iter, NA_NL_WIN, 2, NA_TRUE, NA_TRUE);
//    break;
//  case NA_NEWLINE_NATIVE:
//    naStoreBufferBytes(iter, NA_NL, naStrlen(NA_NL), NA_TRUE, NA_TRUE);
//    break;
//  }
//}
//
//
//
//
//
//NA_DEF void naWriteBufferLine(NABufferIterator* iter, const NAString* string){
//  naWriteBufferString(iter, string);
//  naWriteBufferNewLine(iter);
//}
//NA_DEF void naWriteBufferLineWithFormat(NABufferIterator* iter, const NAUTF8Char* format, ...){
//  // Declaration before implementation. Needed for C90.
//  va_list argumentlist;
//  va_start(argumentlist, format);
//  naWriteBufferLineWithArguments(iter, format, argumentlist);
//  va_end(argumentlist);
//}
//NA_DEF void naWriteBufferLineWithArguments(NABufferIterator* iter, const NAUTF8Char* format, va_list argumentlist){
//  naWriteBufferStringWithArguments(iter, format, argumentlist);
//  naWriteBufferNewLine(iter);
//}
//
//
//
//
//// Copyright (c) NALib, Tobias Stamm
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
