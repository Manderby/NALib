
#include "../../NABuffer.h"


NA_HAPI void na_DestructBuffer(NABuffer* buffer);
NA_RUNTIME_TYPE(NABuffer, na_DestructBuffer, NA_TRUE);



void naDestructBufferTreeLeaf(NAPtr leafData) {
  NABufferPart* part;
  part = naGetPtrMutable(leafData);
  naDelete(part);
}



NAPtr naConstructBufferTreeNode(const void* key) {
  NABufferTreeNodeData* nodeData;
  NA_UNUSED(key);
  nodeData = naAlloc(NABufferTreeNodeData);
  nodeData->len1 = 0;
  nodeData->len2 = 0;
  return naMakePtrWithDataMutable(nodeData);
}



void naDestructBufferTreeNode(NAPtr nodeData) {
  naFree(naGetPtrMutable(nodeData));
}



NABool naUpdateBufferTreeNode(NAPtr parentData, NAPtr* childDatas, NAInt childIndex, NAInt childMask) {
  NABufferTreeNodeData* parentNodeData;
  NA_UNUSED(childIndex);
  
  parentNodeData = (NABufferTreeNodeData*)naGetPtrMutable(parentData);

  size_t prevlen1 = parentNodeData->len1;
  if(childMask & 0x01) {
    NABufferPart* part = (NABufferPart*)naGetPtrMutable(childDatas[0]);
    parentNodeData->len1 = na_GetBufferPartByteSize(part);
  }else{
    NABufferTreeNodeData* childdata = (NABufferTreeNodeData*)naGetPtrMutable(childDatas[0]);
    parentNodeData->len1 = childdata->len1 + childdata->len2;
  }

  size_t prevlen2 = parentNodeData->len2;
  if(childMask & 0x02) {
    NABufferPart* part = (NABufferPart*)naGetPtrMutable(childDatas[1]);
    parentNodeData->len2 = na_GetBufferPartByteSize(part);
  }else{
    NABufferTreeNodeData* childdata = (NABufferTreeNodeData*)naGetPtrMutable(childDatas[1]);
    parentNodeData->len2 = childdata->len1 + childdata->len2;
  }

  return (parentNodeData->len1 != prevlen1) || (parentNodeData->len2 != prevlen2);
}




NA_HDEF void na_InitBufferStruct(NABuffer* buffer) {
  NATreeConfiguration* config;
  buffer->flags = 0;
  buffer->range = naMakeRangei64Zero();
  config = naCreateTreeConfiguration(NA_TREE_KEY_NOKEY | NA_TREE_BALANCE_AVL);
  naSetTreeConfigurationLeafCallbacks(config, NA_NULL, naDestructBufferTreeLeaf);
  naSetTreeConfigurationNodeCallbacks(config, naConstructBufferTreeNode, naDestructBufferTreeNode, naUpdateBufferTreeNode);
  naInitTree(&(buffer->parts), config);
  #if NA_DEBUG
    buffer->iterCount = 0;
  #endif
  naReleaseTreeConfiguration(config);
}



// This is the fill callback for a secure memory buffer. It simply fills all
// of the dst memory with binarz zero.
NA_HDEF void na_FillBufferSecureMemory(void* dst, NARangei64 sourceRange, void* sourceData) {
  NA_UNUSED(sourceData);
  naZeron(dst, (size_t)sourceRange.length);
}



NA_DEF NABuffer* naCreateBuffer(NABool secureMemory) {
  NABuffer* buffer = naCreate(NABuffer);
  na_InitBufferStruct(buffer);

  // When requiring secure memory, we create a source which fills all allocated
  // memory with binary zero.
  if(secureMemory) {
    buffer->source = naCreateBufferSource(na_FillBufferSecureMemory, NA_NULL);
  }else{
    buffer->source = NA_NULL;
  }
  buffer->sourceOffset = 0;

  buffer->newlineEncoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



// Uses range to point into containingRange. Negative offset and length measure
// bytes from the end of the buffer.
NA_HIAPI NARangei64 na_MakeRangei64Absolute(NAInt offset, NAInt length, NARangei64 containingRange) {
  int64 start;
  int64 end;
  #if NA_DEBUG
    if(!naIsLengthValueUsefuli(containingRange.length))
      naError("Length of containing range is not useful.");
  #endif
  start = containingRange.origin + (offset < 0) * containingRange.length + offset;
  end = ((length >= 0) ? start : (naGetRangei64End(containingRange) + 1)) + length;
  #if NA_DEBUG
    if(!naContainsRangei64Offset(containingRange, start))
      naError("Resulting range underflows containing range.");
    if(!naContainsRangei64Offset(containingRange, naMakeMaxWithEndi(end)))
      naError("Resulting range overflows containing range.");
    if(end < start)
      naError("Resulting range has negative length.");
  #endif
  return naMakeRangei64Combination(start, naMakeMaxWithEndi64(end));
}



NA_DEF NABuffer* naCreateBufferExtraction(NABuffer* srcBuffer, NAInt offset, NAInt length) {
  NARangei64 absoluteRange;
  NABufferPart* part;
  
  NABuffer* buffer = naCreate(NABuffer);
  na_InitBufferStruct(buffer);

  absoluteRange = na_MakeRangei64Absolute(offset, length, srcBuffer->range);

  buffer->range = naMakeRangei64(0, absoluteRange.length);
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->source = naCreateBufferSource(NA_NULL, srcBuffer);
  buffer->sourceOffset = -absoluteRange.origin;

  // Add the const data to the list.
  part = na_NewBufferPartSparse(buffer->source, absoluteRange);
  naAddTreeFirstMutable(&(buffer->parts), part);

  buffer->flags = srcBuffer->flags | NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineEncoding = srcBuffer->newlineEncoding;
  buffer->endianness = srcBuffer->endianness;

  return buffer;
}



NA_DEF NABuffer* naCreateBufferCopy(const NABuffer* srcBuffer, NARangei64 range, NABool secureMemory) {
  NABufferIterator iter;
  NATreeIterator partIter;
  NABuffer* buffer = naCreateBuffer(secureMemory);

  if(range.length == 0)
    return buffer;

  // Borrow the contents of the src buffer
  iter = naMakeBufferModifier(buffer);
  naWriteBufferBuffer(&iter, srcBuffer, range);
  naClearBufferIterator(&iter);
  
  // Make the parts of this buffer unique.
  partIter = naMakeTreeMutator(&(buffer->parts));
  while(naIterateTree(&partIter, NA_NULL, NA_NULL)) {
    NABufferPart* part = naGetTreeCurLeafMutable(&partIter);
    na_DecoupleBufferPart(part);
  }
  naClearTreeIterator(&partIter);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferWithSameSource(NABuffer* srcBuffer) {
  NA_UNUSED(srcBuffer);
//  NABuffer* buffer = naCreate(NABuffer);
//  na_InitBufferStruct(buffer);
//
//  buffer->source = naRetain(srcBuffer->source);
//  buffer->srcoffset = srcBuffer->srcoffset;
//
//  buffer->flags = srcBuffer->flags;
//
//  buffer->newlineEncoding = srcBuffer->newlineEncoding;
//  buffer->endianness = srcBuffer->endianness;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_HOST);
//
//  return buffer;
  return NA_NULL;
}



// This is the filler method of the file input source descriptor
NA_HDEF void na_FillBufferPartFile(void* dst, NARangei64 sourceRange, void* data) {
  // todo: position at range.origin.
  naReadFileBytes(data, dst, sourceRange.length);
}



NA_DEF NABuffer* naCreateBufferWithInputPath(const char* filePath) {
  NARangei64 range;
  NAFile* file;
  NABuffer* fileBuffer;
  NABufferSource* readSource;
  NABufferSource* bufSource;

  NABuffer* buffer = naCreate(NABuffer);
  na_InitBufferStruct(buffer);

//  NAString* pwd = naNewStringWithCurWorkingDirectory();
  file = naCreateFileReadingPath(filePath);
  range = naMakeRangei64(0, (int64)naComputeFileByteSize(file));

  fileBuffer = naCreateBuffer(NA_FALSE);
  readSource = naCreateBufferSource(na_FillBufferPartFile, NA_NULL);
    naSetBufferSourceData(readSource, file, (NAMutator)naReleaseFile);
    naSetBufferSourceLimit(readSource, range);
    fileBuffer->source = naRetain(readSource);
    fileBuffer->sourceOffset = 0;
  naRelease(readSource);

  bufSource = naCreateBufferSource(NA_NULL, fileBuffer);
    buffer->source = naRetain(bufSource);
    buffer->sourceOffset = 0;
  naRelease(bufSource);
  naRelease(fileBuffer);

  na_EnsureBufferRange(buffer, 0, range.length);
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineEncoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_DEF NABuffer* naCreateBufferWithConstData(const void* data, size_t byteSize) {
  NABufferPart* part;
  NARangei64 range;

  NABuffer* buffer = naCreate(NABuffer);
  na_InitBufferStruct(buffer);

  range = naMakeRangei64Combination(0, naMakeMaxWithEndi64((int64)byteSize));

  // Add the const data to the list.
  part = na_NewBufferPartWithConstData(data, byteSize);
  naAddTreeFirstMutable(&(buffer->parts), part);

  buffer->source = NA_NULL;
  buffer->sourceOffset = 0;

  buffer->range = range;
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineEncoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_DEF NABuffer* naCreateBufferWithMutableData(void* data, size_t byteSize, NAMutator destructor) {
  NABufferPart* part;
  NARangei64 range;

  NABuffer* buffer = naCreate(NABuffer);
  na_InitBufferStruct(buffer);

  range = naMakeRangei64Combination(0, naMakeMaxWithEndi64((int64)byteSize));

  // Add the mutable data to the list.
  part = na_NewBufferPartWithMutableData(data, byteSize, destructor);
  naAddTreeFirstMutable(&(buffer->parts), part);

  buffer->source = NA_NULL;
  buffer->sourceOffset = 0;

  buffer->range = range;
  buffer->flags |= NA_BUFFER_FLAG_RANGE_FIXED;

  buffer->newlineEncoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_DEF NABuffer* naCreateBufferWithCustomSource(NABufferSource* source, NAInt sourceOffset) {
  NABuffer* buffer = naCreate(NABuffer);
  na_InitBufferStruct(buffer);

  buffer->source = source;
  buffer->sourceOffset = sourceOffset;

  buffer->newlineEncoding = NA_NEWLINE_NATIVE;
  buffer->endianness = NA_ENDIANNESS_HOST;

  return buffer;
}



NA_HDEF void na_DestructBuffer(NABuffer* buffer) {
  #if NA_DEBUG
    if(buffer->iterCount)
      naError("There are still iterators running. Did you forgot naClearBufferIterator?");
  #endif
  if(buffer->source) {
    naRelease(buffer->source);
  }
  naClearTree(&(buffer->parts));
}



// This function ensures that the full given range is available as parts in
// this buffer. If not available, the buffer is extended with sparse parts
// at the beginning and the end.
NA_HDEF void na_EnsureBufferRange(NABuffer* buffer, NAInt start, NAInt end) {
  NAInt length = end - start;

  #if NA_DEBUG
    if(length <= 0)
      naError("Range length shall be >= 0");
    if(naHasBufferFixedRange(buffer) && (start < buffer->range.origin))
      naError("Range of buffer is fixed but trying to access range below");
    if(naHasBufferFixedRange(buffer) && (end > naGetRangei64End(buffer->range)))
      naError("Range of buffer is fixed but trying to access range above");
  #endif

  if(naIsBufferEmpty(buffer)) {
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    NABufferPart* part = na_NewBufferPartSparse(buffer->source, naMakeRangei64(start + buffer->sourceOffset, length));
    naAddTreeFirstMutable(&(buffer->parts), part);
    buffer->range = naMakeRangei64Combination(start, naMakeMaxWithEndi64(end));

  }else{

    NABufferIterator iter = naMakeBufferModifier(buffer);

    // First, we test if we need to add a sparse part at the beginning.
    if(start < buffer->range.origin) {
      NAInt additionalbytes;
      na_LocateBufferStart(&iter);
      additionalbytes = buffer->range.origin - start;
      if(na_IsBufferIteratorSparse(&iter)) {
        // If the first part of this list is already sparse, we simply extend
        // its range.
        na_EnlargeBufferPart(na_GetBufferPart(&iter), (size_t)additionalbytes, 0);
      }else{
        // We create a sparse part at the beginning.
        NABufferPart* part = na_NewBufferPartSparse(buffer->source, naMakeRangei64(start + buffer->sourceOffset, additionalbytes));
        naAddTreeFirstMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangei64Combination(start, naGetRangei64Max(buffer->range));
    }

    // Then, we test if we need to add a sparse part at the end.
    if(end > naGetRangei64End(buffer->range)) {
      NAInt additionalbytes;
      na_LocateBufferLastPart(&iter);
      additionalbytes = end - naGetRangei64End(buffer->range);
      if(na_IsBufferIteratorSparse(&iter)) {
        // If the last part of this list is already sparse, we simply extend
        // its range.
        na_EnlargeBufferPart(na_GetBufferPart(&iter), 0, (size_t)additionalbytes);
      }else{
        // We create a sparse part at the end.
        NABufferPart* part = na_NewBufferPartSparse(buffer->source, naMakeRangei64(naGetRangei64End(buffer->range) + buffer->sourceOffset, additionalbytes));
        naAddTreeLastMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangei64Combination(buffer->range.origin, naMakeMaxWithEndi64(end));
    }

    naClearBufferIterator(&iter);
  }
}




// This function makes the bytes declared in range unavailable by replacing
// that range with a sparse part. As a consequence, certain buffer parts may
// not be used anymore and will be automatically deallocated.
NA_HDEF void na_UnlinkBufferRange(NABuffer* buffer, NARangei64 range) {
  NA_UNUSED(buffer);
  NA_UNUSED(range);
//  NAInt rangepos;
//  NABufferPart* sparsepart;
//  NAListIterator iter;
//
//  #if NA_DEBUG
//    if(naIsBufferEmpty(buffer))
//      naError("na_UnlinkBufferRange", "Buffer ist empty");
//    if(!naContainsRangeiRange(buffer->range, range))
//      naError("na_UnlinkBufferRange", "Range of is not within buffer range");
//  #endif
//
//  iter = naMakeListModifier(&(buffer->parts));
//  naLocateBufferPartOffset(&iter, range.origin);
//
//  // We create a sparse part in place where the dismissed range lies.
//  // As the range will change during the alrogithm, we create it now and
//  // add it later.
//  sparsepart = na_NewBufferPartSparse(range);
//
//  // We go through all parts being part of the given range and split them.
//  // The current position of the iterator always points at the one part
//  // containing the range.offset.
//  while(range.length) {
//    NABufferPart* part;
//    NARangei64 partRange;
//
//    part = naGetListCurMutable(&iter);
//    partRange = na_GetBufferPartRange(part);
//    if(naContainsRangeiRange(range, partRange)) {
//      // If this parts range is contained completely in the range to dismiss,
//      // we remove the whole part.
//      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(partRange), naGetRangeiEnd(range));
//      naDelete(naRemoveListCurMutable(&iter, NA_TRUE));
//    }else if(range.origin == partRange.origin) {
//      // If the origins match, we split the part such that the remaining bytes
//      // lie at the end of the current part.
//      part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partRange));
//      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
//    }else if(naGetRangeiEnd(range) >= naGetRangeiEnd(partRange)) {
//      // If the desired range is equal or larger than the part range, we split
//      // the current part such that the remaining bytes lie at the beginning
//      // of the current part. After that, we iterate to the part after the
//      // removed subpart.
//      rangepos = naGetRangeiEnd(partRange);
//      part->range = naMakeRangeiWithStartAndEnd(partRange.origin, range.origin);
//      range = naMakeRangeiWithStartAndEnd(rangepos, naGetRangeiEnd(range));
//      naIterateList(&iter);
//    }else{
//      // Reaching here, the desired range lies completely within the current
//      // part. We subdivide the current part into two pieces.
//      NABufferPart* newPart = na_NewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partRange)));
//      naReferenceBufferPart(newPart, part, partRange.origin);
//      naAddListAfterMutable(&iter, newPart);
//      part->range = naMakeRangeiWithStartAndEnd(partRange.origin, range.origin);
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



NA_DEF NAInt naSearchBufferByteOffset(NABuffer* buffer, NAByte byte, NAInt startOffset, NABool forward) {
  NABufferIterator iter;
  NAInt indexShift;
  NABool found;

  if(naIsBufferEmpty(buffer))
    return NA_INVALID_MEMORY_INDEX;
    
  indexShift = 0;
  found = NA_FALSE;

  iter = naMakeBufferAccessor(buffer);
  naLocateBufferAbsolute(&iter, startOffset);

  while(!naIsTreeAtInitial(&(iter.partIter))) {
    const NABufferPart* part;
    const NAByte* curByte;

    part = na_GetBufferPart(&iter);
    #if NA_DEBUG
      if(na_IsBufferPartSparse(part))
        naError("sparse part detected.");
    #endif
    curByte = (const NAByte*)na_GetBufferPartDataPointerConst(&iter);
    if(forward) {
      size_t remainingBytes = na_GetBufferPartByteSize(part) - (size_t)iter.partOffset;
      while(remainingBytes) {
        if(*curByte == byte) {
          found = NA_TRUE;
          break;
        }
        indexShift++;
        curByte++;
        remainingBytes--;
      }
    }else{
      size_t remainingBytes = (size_t)iter.partOffset;
      while(remainingBytes) {
        if(*curByte == byte) {
          found = NA_TRUE;
          break;
        }
        indexShift--;
        curByte--;
        remainingBytes--;
      }
    }
    
    if(found)
      break;
      
    if(forward) {
      na_LocateBufferNextPart(&iter);
    }else{
      na_LocateBufferPrevPartMax(&iter);
    }
  }

  naClearBufferIterator(&iter);
  return found ? (startOffset + indexShift) : NA_INVALID_MEMORY_INDEX;
}



NA_DEF NABool naEqualBufferToBuffer(const NABuffer* buffer1, const NABuffer* buffer2, NABool caseSensitive) {
  NABool resultequal;
  NAInt totalremainingBytes;
  NABufferIterator iter1;
  NABufferIterator iter2;

  if(buffer1 == buffer2)
    return NA_TRUE;
    
  if(naGetBufferRange(buffer1).length != naGetBufferRange(buffer2).length)
    return NA_FALSE;
    
  resultequal = NA_TRUE;

  iter1 = naMakeBufferAccessor(buffer1);
  iter2 = naMakeBufferAccessor(buffer2);
  totalremainingBytes = buffer1->range.length;
  na_LocateBufferStart(&iter1);
  na_LocateBufferStart(&iter2);

  while(resultequal && totalremainingBytes) {
    const NAByte* bufferbytes1;
    const NAByte* bufferbytes2;

    #if NA_DEBUG
      const NABufferPart* part1 = na_GetBufferPart(&iter1);
      const NABufferPart* part2 = na_GetBufferPart(&iter2);
      if(na_IsBufferPartSparse(part1))
        naError("Buffer 1 has sparse part");
      if(na_IsBufferPartSparse(part2))
        naError("Buffer 2 has sparse part");
    #endif

    size_t remainingBytes1 = na_GetBufferPartRemainingBytes(&iter1);
    size_t remainingBytes2 = na_GetBufferPartRemainingBytes(&iter2);
    size_t remainingBytes = naMins(remainingBytes1, remainingBytes2);
    na_PrepareBuffer(&iter1, remainingBytes);
    na_PrepareBuffer(&iter2, remainingBytes);
    bufferbytes1 = na_GetBufferPartDataPointerConst(&iter1);
    bufferbytes2 = na_GetBufferPartDataPointerConst(&iter2);

    if(bufferbytes1 != bufferbytes2) {
      if(!naEqualUTF8CStringLiterals((NAUTF8Char*)bufferbytes1, (NAUTF8Char*)bufferbytes2, remainingBytes, caseSensitive)) {
        resultequal = NA_FALSE;
        break;
      }
    }
    
    totalremainingBytes -= remainingBytes;
    naIterateBuffer(&iter1, (NAInt)remainingBytes);
    naIterateBuffer(&iter2, (NAInt)remainingBytes);
  }

  naClearBufferIterator(&iter1);
  naClearBufferIterator(&iter2);
  return resultequal;
}



NA_DEF NABool naEqualBufferToData(NABuffer* buffer, const void* data, size_t dataByteSize, NABool caseSensitive) {
  NABool resultequal = NA_TRUE;
  const NAByte* bytes;
  NABufferIterator iter;

  if(dataByteSize != (size_t)buffer->range.length)
    return NA_FALSE;

  bytes = (const NAByte*)data;

  iter = naMakeBufferAccessor(buffer);
  size_t totalremainingBytes = (size_t)buffer->range.length;
  na_LocateBufferStart(&iter);

  while(resultequal && totalremainingBytes) {
    const NABufferPart* part;
    const NAByte* bufferbytes;

    part = na_GetBufferPart(&iter);
    #if NA_DEBUG
      if(na_IsBufferPartSparse(part))
        naError("Buffer has sparse part");
    #endif

    size_t remainingBytes = na_GetBufferPartByteSize(part);
    // todo: Here happends a crash when no cached string is available.
    // Find out why!
    bufferbytes = na_GetBufferPartDataPointerConst(&iter);
    if(bufferbytes != bytes) {
      if(!naEqualUTF8CStringLiterals((NAUTF8Char*)bufferbytes, (NAUTF8Char*)bytes, remainingBytes, caseSensitive)) {
        resultequal = NA_FALSE;
        break;
      }
    }
    bytes += remainingBytes;
    totalremainingBytes -= remainingBytes;
    naIterateBuffer(&iter, (NAInt)remainingBytes);
  }

  naClearBufferIterator(&iter);
  return resultequal;
}



NA_DEF void naAppendBufferToBuffer(NABuffer* dstbuffer, const NABuffer* srcBuffer) {
  NABufferIterator iter = naMakeBufferModifier(dstbuffer);
  na_LocateBufferEnd(&iter);
  naWriteBufferBuffer(&iter, srcBuffer, naGetBufferRange(srcBuffer));
  naClearBufferIterator(&iter);
  return;
}



NA_DEF void naCacheBufferRange(NABuffer* buffer, NARangei64 range) {
  if(range.length) {
    NABufferIterator iter = naMakeBufferModifier(buffer);
    naLocateBufferAbsolute(&iter, range.origin);
    na_PrepareBuffer(&iter, (size_t)range.length);
    naClearBufferIterator(&iter);
  }
}



NA_DEF void naDismissBufferRange(NABuffer* buffer, NARangei64 range) {
  na_UnlinkBufferRange(buffer, range);
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
