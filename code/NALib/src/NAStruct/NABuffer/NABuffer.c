

// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "../../NABuffer.h"


NA_HAPI void naDeallocBuffer(NABuffer* buffer);
NA_RUNTIME_TYPE(NABuffer, naDeallocBuffer, NA_TRUE);



void naDestructBufferTreeLeaf(NAPtr leafdata){
  NABufferPart* part;
  part = naGetPtrMutable(leafdata);
  naDelete(part);
}



NAPtr naConstructBufferTreeNode(const void* key){
  NABufferTreeNodeData* nodedata;
  NA_UNUSED(key);
  nodedata = naAlloc(NABufferTreeNodeData);
  nodedata->len1 = 0;
  nodedata->len2 = 0;
  return naMakePtrWithDataMutable(nodedata);
}



void naDestructBufferTreeNode(NAPtr nodedata){
  naFree(naGetPtrMutable(nodedata));
}



NABool naUpdateBufferTreeNode(NAPtr parentdata, NAPtr* childdatas, NAInt childindx, NAInt childmask){
  NABufferTreeNodeData* parentnodedata;
  NAInt prevlen1;
  NAInt prevlen2;
  NA_UNUSED(childindx);
  
  parentnodedata = (NABufferTreeNodeData*)naGetPtrMutable(parentdata);

  prevlen1 = parentnodedata->len1;
  if(childmask & 0x01){
    NABufferPart* part = (NABufferPart*)naGetPtrMutable(childdatas[0]);
    parentnodedata->len1 = naGetBufferPartByteSize(part);
  }else{
    NABufferTreeNodeData* childdata = (NABufferTreeNodeData*)naGetPtrMutable(childdatas[0]);
    parentnodedata->len1 = childdata->len1 + childdata->len2;
  }

  prevlen2 = parentnodedata->len2;
  if(childmask & 0x02){
    NABufferPart* part = (NABufferPart*)naGetPtrMutable(childdatas[1]);
    parentnodedata->len2 = naGetBufferPartByteSize(part);
  }else{
    NABufferTreeNodeData* childdata = (NABufferTreeNodeData*)naGetPtrMutable(childdatas[1]);
    parentnodedata->len2 = childdata->len1 + childdata->len2;
  }

  return (parentnodedata->len1 != prevlen1) || (parentnodedata->len2 != prevlen2);
}




NA_HDEF void naInitBufferStruct(NABuffer* buffer){
  NATreeConfiguration* config;
  buffer->flags = 0;
  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);
  config = naCreateTreeConfiguration(NA_TREE_KEY_NOKEY | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(config, NA_NULL, naDestructBufferTreeLeaf);
  naSetTreeConfigurationNodeCallbacks(config, naConstructBufferTreeNode, naDestructBufferTreeNode, naUpdateBufferTreeNode);
  naInitTree(&(buffer->parts), config);
  #ifndef NDEBUG
    buffer->itercount = 0;
  #endif
  naReleaseTreeConfiguration(config);
}



// This is the fill callback for a secure memory buffer. It simply fills all
// of the dst memory with binarz zero.
NA_HDEF void naFillBufferSecureMemory(void* dst, NARangei sourcerange, void* sourcedata){
  NA_UNUSED(sourcedata);
  naZeron(dst, sourcerange.length);
}



NA_DEF NABuffer* naNewBuffer(NABool securememory){
  NABuffer* buffer = naNew(NABuffer);
  naInitBufferStruct(buffer);

  // When requiring secure memory, we create a source which fills all allocated
  // memory with binary zero.
  if(securememory){
    buffer->source = naNewBufferSource(naFillBufferSecureMemory, NA_NULL);
  }else{
    buffer->source = NA_NULL;
  }
  buffer->sourceoffset = 0;

  buffer->newlineencoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



// Uses range to point into containingrange. Negative offset and length measure
// bytes from the end of the buffer.
NA_HIAPI NARangei naMakeRangeiAbsolute(NAInt offset, NAInt length, NARangei containingrange){
  NAInt start;
  NAInt end;
  #ifndef NDEBUG
    if(!naIsLengthValueUsefuli(containingrange.length))
      naError("Length of containing range is not useful.");
  #endif
  start = containingrange.origin + (offset < 0) * containingrange.length + offset;
  end = ((length >= 0) ? start : (naGetRangeiEnd(containingrange) + 1)) + length;
  #ifndef NDEBUG
    if(!naContainsRangeiOffset(containingrange, start))
      naError("Resulting range underflows containing range.");
    if(!naContainsRangeiOffset(containingrange, naMakeMaxWithEndi(end)))
      naError("Resulting range overflows containing range.");
    if(end < start)
      naError("Resulting range has negative length.");
  #endif
  return naMakeRangeiWithStartAndEnd(start, end);
}



NA_DEF NABuffer* naNewBufferExtraction(NABuffer* srcbuffer, NAInt offset, NAInt length){
  NARangei absoluterange;
  NABufferPart* part;
  
  NABuffer* buffer = naNew(NABuffer);
  naInitBufferStruct(buffer);

  absoluterange = naMakeRangeiAbsolute(offset, length, srcbuffer->range);

  buffer->source = NA_NULL;
  buffer->sourceoffset = 0;

  buffer->range = naMakeRangei(0, absoluterange.length);
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->source = naNewBufferSource(NA_NULL, srcbuffer);
  buffer->sourceoffset = -absoluterange.origin;

  // Add the const data to the list.
  part = naNewBufferPartSparse(buffer->source, absoluterange);
  naAddTreeFirstMutable(&(buffer->parts), part);

  buffer->flags = srcbuffer->flags | NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineencoding = srcbuffer->newlineencoding;
  buffer->endianness = srcbuffer->endianness;

  return buffer;
}



NA_DEF NABuffer* naNewBufferCopy(const NABuffer* srcbuffer, NARangei range, NABool securememory){
  NABufferIterator iter;
  NATreeIterator partiter;
  NABuffer* buffer = naNewBuffer(securememory);

  if(range.length == 0){return buffer;}

  // Borrow the contents of the src buffer
  iter = naMakeBufferModifier(buffer);
  naWriteBufferBuffer(&iter, srcbuffer, range);
  naClearBufferIterator(&iter);
  
  // Make the parts of this buffer unique.
  partiter = naMakeTreeMutator(&(buffer->parts));
  while(naIterateTree(&partiter, NA_NULL, NA_NULL)){
    NABufferPart* part = naGetTreeCurLeafMutable(&partiter);
    naSeparateBufferPart(part);
  }
  naClearTreeIterator(&partiter);

  return buffer;
}



NA_DEF NABuffer* naNewBufferWithSameSource(NABuffer* srcbuffer){
  NA_UNUSED(srcbuffer);
//  NABuffer* buffer = naNew(NABuffer);
//  naInitBufferStruct(buffer);
//
//  buffer->source = naRetain(srcbuffer->source);
//  buffer->srcoffset = srcbuffer->srcoffset;
//
//  buffer->flags = srcbuffer->flags;
//
//  buffer->newlineencoding = srcbuffer->newlineencoding;
//  buffer->endianness = srcbuffer->endianness;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_HOST);
//
//  return buffer;
  return NA_NULL;
}



// This is the filler method of the file input source descriptor
NA_HDEF void naFillBufferPartFile(void* dst, NARangei sourcerange, void* data){
  // todo: position at range.origin.
  naReadFileBytes(data, dst, sourcerange.length);
}



NA_DEF NABuffer* naNewBufferWithInputFile(const char* filename){
  NARangei range;
  NAFile* file;
  NABuffer* filebuffer;
  NABufferSource* readsource;
  NABufferSource* bufsource;

  NABuffer* buffer = naNew(NABuffer);
  naInitBufferStruct(buffer);

  file = naCreateFileReadingFilename(filename);
  range = naMakeRangei(0, (NAInt)naComputeFileBytesize(file));

  filebuffer = naNewBuffer(NA_FALSE);
  readsource = naNewBufferSource(naFillBufferPartFile, NA_NULL);
    naSetBufferSourceData(readsource, file, (NAMutator)naReleaseFile);
    naSetBufferSourceLimit(readsource, range);
    filebuffer->source = naRetain(readsource);
    filebuffer->sourceoffset = 0;
  naRelease(readsource);

  bufsource = naNewBufferSource(NA_NULL, filebuffer);
    buffer->source = naRetain(bufsource);
    buffer->sourceoffset = 0;
  naRelease(bufsource);
  naRelease(filebuffer);

  naEnsureBufferRange(buffer, 0, range.length);
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineencoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_DEF NABuffer* naNewBufferWithConstData(const void* data, NAInt bytesize){
  NABufferPart* part;
  NARangei range;

  NABuffer* buffer = naNew(NABuffer);
  naInitBufferStruct(buffer);

  range = naMakeRangeiWithStartAndEnd(0, (NAInt)bytesize);

  // Add the const data to the list.
  part = naNewBufferPartWithConstData(data, bytesize);
  naAddTreeFirstMutable(&(buffer->parts), part);

  buffer->source = NA_NULL;
  buffer->sourceoffset = 0;

  buffer->range = range;
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineencoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_DEF NABuffer* naNewBufferWithMutableData(void* data, NAInt bytesize, NAMutator destructor){
  NABufferPart* part;
  NARangei range;

  NABuffer* buffer = naNew(NABuffer);
  naInitBufferStruct(buffer);

  range = naMakeRangeiWithStartAndEnd(0, (NAInt)bytesize);

  // Add the mutable data to the list.
  part = naNewBufferPartWithMutableData(data, bytesize, destructor);
  naAddTreeFirstMutable(&(buffer->parts), part);

  buffer->source = NA_NULL;
  buffer->sourceoffset = 0;

  buffer->range = range;
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineencoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_DEF NABuffer* naNewBufferWithCustomSource(NABufferSource* source, NAInt sourceoffset){
  NABuffer* buffer = naNew(NABuffer);
  naInitBufferStruct(buffer);

  buffer->source = source;
  buffer->sourceoffset = sourceoffset;

  buffer->newlineencoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_HDEF void naDeallocBuffer(NABuffer* buffer){
  #ifndef NDEBUG
    if(buffer->itercount)
      naError("There are still iterators running. Did you forgot naClearBufferIterator?");
  #endif
  if(buffer->source){naRelease(buffer->source);}
  naClearTree(&(buffer->parts));
}



// This function ensures that the full given range is available as parts in
// this buffer. If not available, the buffer is extended with sparse parts
// at the beginning and the end.
NA_HDEF void naEnsureBufferRange(NABuffer* buffer, NAInt start, NAInt end){
  NAInt length = end - start;

  #ifndef NDEBUG
    if(length <= 0)
      naError("Range length shall be >= 0");
    if(naHasBufferFixedRange(buffer) && (start < buffer->range.origin))
      naError("Range of buffer is fixed but trying to access range below");
    if(naHasBufferFixedRange(buffer) && (end > naGetRangeiEnd(buffer->range)))
      naError("Range of buffer is fixed but trying to access range above");
  #endif

  if(naIsBufferEmpty(buffer)){
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    NABufferPart* part = naNewBufferPartSparse(buffer->source, naMakeRangei(start + buffer->sourceoffset, length));
    naAddTreeFirstMutable(&(buffer->parts), part);
    buffer->range = naMakeRangeiWithStartAndEnd(start, end);

  }else{

    NABufferIterator iter = naMakeBufferModifier(buffer);

    // First, we test if we need to add a sparse part at the beginning.
    if(start < buffer->range.origin){
      NAInt additionalbytes;
      naLocateBufferStart(&iter);
      additionalbytes = buffer->range.origin - start;
      if(naIsBufferIteratorSparse(&iter)){
        // If the first part of this list is already sparse, we simply extend
        // its range.
        naEnlargeBufferPart(naGetBufferPart(&iter), additionalbytes, 0);
      }else{
        // We create a sparse part at the beginning.
        NABufferPart* part = naNewBufferPartSparse(buffer->source, naMakeRangei(start + buffer->sourceoffset, additionalbytes));
        naAddTreeFirstMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(start, naGetRangeiEnd(buffer->range));
    }

    // Then, we test if we need to add a sparse part at the end.
    if(end > naGetRangeiEnd(buffer->range)){
      NAInt additionalbytes;
      naLocateBufferLastPart(&iter);
      additionalbytes = end - naGetRangeiEnd(buffer->range);
      if(naIsBufferIteratorSparse(&iter)){
        // If the last part of this list is already sparse, we simply extend
        // its range.
        naEnlargeBufferPart(naGetBufferPart(&iter), 0, additionalbytes);
      }else{
        // We create a sparse part at the end.
        NABufferPart* part = naNewBufferPartSparse(buffer->source, naMakeRangei(naGetRangeiEnd(buffer->range) + buffer->sourceoffset, additionalbytes));
        naAddTreeLastMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(buffer->range.origin, end);
    }

    naClearBufferIterator(&iter);
  }
}




// This function makes the bytes declared in range unavailable by replacing
// that range with a sparse part. As a consequence, certain buffer parts may
// not be used anymore and will be automatically deallocated.
NA_HDEF void naUnlinkBufferRange(NABuffer* buffer, NARangei range){
  NA_UNUSED(buffer);
  NA_UNUSED(range);
//  NAInt rangepos;
//  NABufferPart* sparsepart;
//  NAListIterator iter;
//
//  #ifndef NDEBUG
//    if(naIsBufferEmpty(buffer))
//      naError("naUnlinkBufferRange", "Buffer ist empty");
//    if(!naContainsRangeiRange(buffer->range, range))
//      naError("naUnlinkBufferRange", "Range of is not within buffer range");
//  #endif
//
//  iter = naMakeListModifier(&(buffer->parts));
//  naLocateBufferPartOffset(&iter, range.origin);
//
//  // We create a sparse part in place where the dismissed range lies.
//  // As the range will change during the alrogithm, we create it now and
//  // add it later.
//  sparsepart = naNewBufferPartSparse(range);
//
//  // We go through all parts being part of the given range and split them.
//  // The current position of the iterator always points at the one part
//  // containing the range.offset.
//  while(range.length){
//    NABufferPart* part;
//    NARangei partrange;
//
//    part = naGetListCurMutable(&iter);
//    partrange = naGetBufferPartRange(part);
//    if(naContainsRangeiRange(range, partrange)){
//      // If this parts range is contained completely in the range to dismiss,
//      // we remove the whole part.
//      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(partrange), naGetRangeiEnd(range));
//      naDelete(naRemoveListCurMutable(&iter, NA_TRUE));
//    }else if(range.origin == partrange.origin){
//      // If the origins match, we split the part such that the remaining bytes
//      // lie at the end of the current part.
//      part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partrange));
//      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
//    }else if(naGetRangeiEnd(range) >= naGetRangeiEnd(partrange)){
//      // If the desired range is equal or larger than the part range, we split
//      // the current part such that the remaining bytes lie at the beginning
//      // of the current part. After that, we iterate to the part after the
//      // removed subpart.
//      rangepos = naGetRangeiEnd(partrange);
//      part->range = naMakeRangeiWithStartAndEnd(partrange.origin, range.origin);
//      range = naMakeRangeiWithStartAndEnd(rangepos, naGetRangeiEnd(range));
//      naIterateList(&iter);
//    }else{
//      // Reaching here, the desired range lies completely within the current
//      // part. We subdivide the current part into two pieces.
//      NABufferPart* newpart = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partrange)));
//      naReferenceBufferPart(newpart, part, partrange.origin);
//      naAddListAfterMutable(&iter, newpart);
//      part->range = naMakeRangeiWithStartAndEnd(partrange.origin, range.origin);
//      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
//      naIterateList(&iter);
//    }
//  }
//  // The iterator now points to the one part after the insertion position of
//  // the new sparse part.
//
//  // We add the sparse part one before the current position and move the
//  // iterator at that position.
//  naAddListBeforeMutable(&iter, sparsepart);
//  naIterateListBack(&iter);
//  naCombineBufferPartAdjacents(&iter);
//
//  naClearListIterator(&iter);
}



NA_DEF NAInt naSearchBufferByteOffset(NABuffer* buffer, NAByte byte, NAInt startoffset, NABool forward){
  NABufferIterator iter;
  NAInt indexshift;
  NABool found;

  if(naIsBufferEmpty(buffer)){return NA_INVALID_MEMORY_INDEX;}
  indexshift = 0;
  found = NA_FALSE;

  iter = naMakeBufferAccessor(buffer);
  naLocateBufferAbsolute(&iter, startoffset);

  while(!naIsTreeAtInitial(&(iter.partiter))){
    const NABufferPart* part;
    const NAByte* curbyte;

    part = naGetBufferPart(&iter);
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("sparse part detected.");
    #endif
    curbyte = (const NAByte*)naGetBufferPartDataPointerConst(&iter);
    if(forward){
      NAInt remainingbytes = naGetBufferPartByteSize(part) - iter.partoffset;
      while(remainingbytes){
        if(*curbyte == byte){
          found = NA_TRUE;
          break;
        }
        indexshift++;
        curbyte++;
        remainingbytes--;
      }
    }else{
      NAInt remainingbytes = iter.partoffset;
      while(remainingbytes){
        if(*curbyte == byte){
          found = NA_TRUE;
          break;
        }
        indexshift--;
        curbyte--;
        remainingbytes--;
      }
    }
    if(found){break;}
    if(forward){
      naLocateBufferNextPart(&iter);
    }else{
      naLocateBufferPrevPartMax(&iter);
    }
  }

  naClearBufferIterator(&iter);
  return found ? (startoffset + indexshift) : NA_INVALID_MEMORY_INDEX;
}



NA_DEF NABool naEqualBufferToBuffer(const NABuffer* buffer1, const NABuffer* buffer2, NABool casesensitive){
  NABool resultequal;
  NAInt totalremainingbytes;
  NABufferIterator iter1;
  NABufferIterator iter2;

  if(buffer1 == buffer2){return NA_TRUE;}
  if(naGetBufferRange(buffer1).length != naGetBufferRange(buffer2).length){return NA_FALSE;}
  resultequal = NA_TRUE;

  iter1 = naMakeBufferAccessor(buffer1);
  iter2 = naMakeBufferAccessor(buffer2);
  totalremainingbytes = buffer1->range.length;
  naLocateBufferStart(&iter1);
  naLocateBufferStart(&iter2);

  while(resultequal && totalremainingbytes){
    NAInt remainingbytes;
    NAInt remainingbytes1;
    NAInt remainingbytes2;
    const NAByte* bufferbytes1;
    const NAByte* bufferbytes2;

    #ifndef NDEBUG
      const NABufferPart* part1 = naGetBufferPart(&iter1);
      const NABufferPart* part2 = naGetBufferPart(&iter2);
      if(naIsBufferPartSparse(part1))
        naError("Buffer 1 has sparse part");
      if(naIsBufferPartSparse(part2))
        naError("Buffer 2 has sparse part");
    #endif

    remainingbytes1 = naGetBufferPartRemainingBytes(&iter1);
    remainingbytes2 = naGetBufferPartRemainingBytes(&iter2);
    remainingbytes = naMini(remainingbytes1, remainingbytes2);
    bufferbytes1 = naGetBufferPartDataPointerConst(&iter1);
    bufferbytes2 = naGetBufferPartDataPointerConst(&iter2);

    if(bufferbytes1 != bufferbytes2){
      if(!naEqualUTF8CStringLiterals((NAUTF8Char*)bufferbytes1, (NAUTF8Char*)bufferbytes2, remainingbytes, casesensitive)){
        resultequal = NA_FALSE;
        break;
      }
    }
    
    totalremainingbytes -= remainingbytes;
    naIterateBuffer(&iter1, remainingbytes);
    naIterateBuffer(&iter2, remainingbytes);
  }

  naClearBufferIterator(&iter1);
  naClearBufferIterator(&iter2);
  return resultequal;
}



NA_DEF NABool naEqualBufferToData(NABuffer* buffer, const void* data, NAInt databytesize, NABool casesensitive){
  NABool resultequal = NA_TRUE;
  NAInt totalremainingbytes;
  const NAByte* bytes;
  NABufferIterator iter;

  if(databytesize != buffer->range.length){return NA_FALSE;}

  bytes = (const NAByte*)data;

  iter = naMakeBufferAccessor(buffer);
  totalremainingbytes = buffer->range.length;
  naLocateBufferStart(&iter);

  while(resultequal && totalremainingbytes){
    const NABufferPart* part;
    NAInt remainingbytes;
    const NAByte* bufferbytes;

    part = naGetBufferPart(&iter);
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("Buffer has sparse part");
    #endif

    remainingbytes = naGetBufferPartByteSize(part);
    bufferbytes = naGetBufferPartDataPointerConst(&iter);
    if(bufferbytes != bytes){
      if(!naEqualUTF8CStringLiterals((NAUTF8Char*)bufferbytes, (NAUTF8Char*)bytes, remainingbytes, casesensitive)){
        resultequal = NA_FALSE;
        break;
      }
    }
    bytes += remainingbytes;
    totalremainingbytes -= remainingbytes;
    naIterateBuffer(&iter, remainingbytes);
  }

  naClearBufferIterator(&iter);
  return resultequal;
}



NA_DEF void naAppendBufferToBuffer(NABuffer* dstbuffer, const NABuffer* srcbuffer){
  NABufferIterator iter = naMakeBufferModifier(dstbuffer);
  naLocateBufferEnd(&iter);
  naWriteBufferBuffer(&iter, srcbuffer, naGetBufferRange(srcbuffer));
  naClearBufferIterator(&iter);
  return;
}



NA_DEF void naCacheBufferRange(NABuffer* buffer, NARangei range){
  if(range.length){
    NABufferIterator iter = naMakeBufferModifier(buffer);
    naLocateBufferAbsolute(&iter, range.origin);
    naPrepareBuffer(&iter, range.length);
    naClearBufferIterator(&iter);
  }
}



NA_DEF void naDismissBufferRange(NABuffer* buffer, NARangei range){
  naUnlinkBufferRange(buffer, range);
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
