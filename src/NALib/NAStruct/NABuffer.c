
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.




#include "../NABuffer.h"


#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE (naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAUInt)NA_BUFFER_PART_BYTESIZE)
#endif


// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////


struct NAASDFBufferPart{
  NAPointer* data;
  NAInt origin;           // The origin of the first byte in the data pointer
  NARangei range;         // The available range in this buffer part.
};

NA_HIAPI void naDestructASDFASDFBufferPart(NAASDFBufferPart* part);
NA_RUNTIME_TYPE(NAASDFBufferPart, naDestructASDFASDFBufferPart);




// Creates a memory block with sparse memory.
// A sparse buffer is always initialized with the origin being the same as the
// range.origin. This can later on be changed with naReferenceASDFBufferPart.
NA_HIDEF NAASDFBufferPart* naNewASDFBufferPartSparse(NARangei range){
  NAASDFBufferPart* part = naNew(NAASDFBufferPart);
  part->data = NA_NULL;
  part->origin = range.origin;
  part->range = range;
  return part;
}



// Creates a memory block with given const data buffer
NA_HIDEF NAASDFBufferPart* naNewASDFBufferPartConstData(const NAByte* data, NAInt bytesize){
  NAASDFBufferPart* part = naNew(NAASDFBufferPart);
  part->data = naNewPointerConst(data);
  part->origin = 0;
  part->range = naMakeRangei(0, bytesize);
  return part;
}



// Creates a memory block with given mutable data buffer
NA_HIDEF NAASDFBufferPart* naNewASDFBufferPartMutableData(NAByte* data, NAInt bytesize, NAMemoryCleanup cleanup){
  NAASDFBufferPart* part = naNew(NAASDFBufferPart);
  part->data = naNewPointerMutable(data, cleanup, NA_NULL);
  part->origin = 0;
  part->range = naMakeRangei(0, bytesize);
  return part;
}



// This function takes a sparse part and makes it referencing a second part
// with a given offset. The range which is referenced is defined by the range
// stored in the part.
NA_HIDEF void naReferenceASDFBufferPart(NAASDFBufferPart* part, NAASDFBufferPart* srcpart, NAInt srcoffset){
  #ifndef NDEBUG
    if(part->data)
      naError("naReferenceASDFBufferPart", "Part already references a part");
  #endif
  part->data = naRetainPointer(srcpart->data);
  part->origin = srcpart->origin + srcoffset;
}



NA_HIDEF void naAllocateBufferPartMemory(NAASDFBufferPart* part){
  #ifndef NDEBUG
    if(part->data)
      naError("naAllocateBufferPartMemory", "Part already has memory");
  #endif
  part->data = naNewPointerMutable(naMalloc(part->range.length), NA_MEMORY_CLEANUP_FREE, NA_NULL);
}



NA_HIDEF void naDestructASDFASDFBufferPart(NAASDFBufferPart* part){
  if(part->data){naReleasePointer(part->data);}
}



NA_HIDEF NABool naIsASDFBufferPartSparse(const NAASDFBufferPart* part){
  return (part->data == NA_NULL);
}



NA_HIDEF NAInt naGetBufferPartRangeLength(const NAASDFBufferPart* part){
  return part->range.length;
}



NA_HIDEF NAInt naGetBufferPartEnd(const NAASDFBufferPart* part){
  return naGetRangeiEnd(part->range);
}



NA_HIDEF NARangei naGetBufferPartRange(const NAASDFBufferPart* part){
  return part->range;
}



NA_HIDEF NAInt naGetBufferPartStart(const NAASDFBufferPart* part){
  return part->range.origin;
}



NA_HIDEF NABool naContainsBufferPartOffset(const NAASDFBufferPart* part, NAInt offset){
  return naContainsRangeiOffset(part->range, offset);
}



// This function returns NA_TRUE, if both parts have the same data pointer
// and the ranges are adjacent.
NA_HIDEF NABool naAreASDFBufferPartsEqualAndAdjacent(NAASDFBufferPart* startpart, NAASDFBufferPart* endpart){
  #ifndef NDEBUG
    if(naIsASDFBufferPartSparse(startpart) && naIsASDFBufferPartSparse(endpart))
      naError("naAreASDFBufferPartsEqualAndAdjacent", "Both parts are sparse");
  #endif
  if((startpart->data == endpart->data) && (naGetRangeiEnd(startpart->range) == endpart->range.origin)){
    #ifndef NDEBUG
      if(startpart->origin != endpart->origin)
        naError("naAreASDFBufferPartsEqualAndAdjacent", "origin of the two parts do not match");
    #endif
  }else{
    return NA_FALSE;
  }
}



// This function combines the current part given by iter with its previous and
// next neighbor if possible. 
NA_HIDEF void naCombineBufferPartAdjacents(NAListIterator* iter){
  NAASDFBufferPart* part = naGetListCurrentMutable(iter);
  NAASDFBufferPart* prevpart = naGetListPrevMutable(iter);
  NAASDFBufferPart* nextpart = naGetListNextMutable(iter);
  
  if(prevpart && naAreASDFBufferPartsEqualAndAdjacent(prevpart, part)){
    // we can combine the two parts.
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(prevpart), naGetBufferPartEnd(part));
    // then, we remove the previous part and delete it.
    naRemoveListPrevMutable(iter);
    naDelete(prevpart);
  }
  if(nextpart && naAreASDFBufferPartsEqualAndAdjacent(part, nextpart)){
    // we can combine the two parts.
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), naGetBufferPartEnd(nextpart));
    // then, we remove the next part and delete it.
    naRemoveListNextMutable(iter);
    naDelete(nextpart);
  }
}



// This function splits a sparse part such that there exists in the end a
// sparse part having precisely the desired range, possibly surrounded by
// other parts. At the end of this function, the iterator points to that
// very part.
NA_HIDEF void naSplitASDFBufferSparsePart(NAListIterator* iter, NARangei range){
  NAASDFBufferPart* part = naGetListCurrentMutable(iter);

  #ifndef NDEBUG
    if(!part)
      naError("naSplitASDFBufferSparsePart", "iterator must not be at initial position");
    if(!naIsASDFBufferPartSparse(part))
      naError("naSplitASDFBufferSparsePart", "current part is not sparse");
    if(!naContainsRangeiRange(naGetBufferPartRange(part), range))
      naError("naSplitASDFBufferSparsePart", "part range does not contain given range");
  #endif

  if(naEqualRangei(naGetBufferPartRange(part), range)){
    // This sparse part fits the desired range already.
    // Nothing to do.
    
  }else if(naGetBufferPartStart(part) == range.origin){
    // The desired part will be put at the start of this sparse part.
    part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part));
    part->origin = part->range.origin;
    part = naNewASDFBufferPartSparse(range);
    naAddListBeforeMutable(iter, part);
    naIterateList(iter, -1);
    
  }else if(naGetBufferPartEnd(part) == naGetRangeiEnd(range)){
    // The desired part will be put at the end of this sparse part.
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
    part->origin = part->range.origin;
    part = naNewASDFBufferPartSparse(range);
    naAddListAfterMutable(iter, part);
    naIterateList(iter, 1);
    
  }else{
    // The desired part will be put in the middle of this sparse part.
    NAASDFBufferPart* lastpart = naNewASDFBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part)));
    naAddListAfterMutable(iter, lastpart);
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
    part->origin = part->range.origin;
    part = naNewASDFBufferPartSparse(range);
    naAddListAfterMutable(iter, part);
    naIterateList(iter, 1);
  }
}



// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HIDEF NAByte* naGetASDFBufferPartDataPointer(NAASDFBufferPart* part, NAInt offset){
  #ifndef NDEBUG
    if(naIsASDFBufferPartSparse(part))
      naError("naGetASDFBufferPartDataPointer", "buffer part is sparse");
    if(!naContainsRangeiOffset(part->range, offset))
      naError("naGetASDFBufferPartDataPointer", "offset not inside buffer part");
    if(offset - part->origin < 0)
      naError("naGetASDFBufferPartDataPointer", "offset calculation wrong");
  #endif
  return &(((NAByte*)naGetPointerMutable(part->data))[offset - part->origin]);
}


// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HIDEF NAByte* naGetASDFBufferPartBaseDataPointer(NAASDFBufferPart* part){
  #ifndef NDEBUG
    if(naIsASDFBufferPartSparse(part))
      naError("naGetASDFBufferPartBaseDataPointer", "buffer part is sparse");
  #endif
  return (NAByte*)naGetPointerMutable(part->data);
}






// //////////////////////////////////////
// Buffer
// //////////////////////////////////////




#define NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED 0x01
#define NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED 0x02


NA_HIAPI void naFillASDFBufferPartMemory(NAASDFBuffer* buffer, NAASDFBufferPart* part);
NA_HIAPI void naFillASDFBufferPartFile(NAASDFBuffer* buffer, NAASDFBufferPart* part);
NA_HIAPI void naFillASDFBufferPartConstData(NAASDFBuffer* buffer, NAASDFBufferPart* part);
NA_HIAPI void naFillASDFBufferPartMutableData(NAASDFBuffer* buffer, NAASDFBufferPart* part);

NA_HIDEF void naEnsureASDFBufferRange(NAASDFBuffer* buffer, NARangei range);



NA_DEF NAASDFBuffer* naCreateASDFBufferMemorySource(){
  NAASDFBuffer* buffer = naAlloc(NAASDFBuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  buffer->src = NA_NULL;
  buffer->srctype = NA_BUFFER_SOURCE_MEMORY;
  buffer->srcoffset = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->flags = 0;
  buffer->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  buffer->fillPart = naFillASDFBufferPartMemory;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NAASDFBuffer* naCreateASDFBuffer(){
  NAASDFBuffer* buffer = naAlloc(NAASDFBuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  buffer->src = naCreateASDFBufferMemorySource();
  buffer->srctype = NA_BUFFER_SOURCE_MEMORY_SOURCE;
  buffer->srcoffset = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->flags = 0;
  buffer->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  buffer->fillPart = naFillASDFBufferPartMemory;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NAASDFBuffer* naCreateASDFBufferExtraction(NAASDFBuffer* srcbuf, NARangei range){
  NAASDFBuffer* buffer = naAlloc(NAASDFBuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  buffer->src = srcbuf;
  buffer->srctype = NA_BUFFER_SOURCE_BUFFER;
  buffer->srcoffset = -range.origin;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->flags = 0;
  buffer->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  buffer->fillPart = naFillASDFBufferPartMemory;
  
  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  naEnsureASDFBufferRange(buffer, naMakeRangei(0, range.length));
  buffer->flags |= NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED | NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED;

  return buffer;
}



NA_DEF NAASDFBuffer* naCreateASDFBufferFile(const char* filename){
  NAASDFBuffer* buffer = naAlloc(NAASDFBuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  buffer->src = naAlloc(NAFile);
  *((NAFile*)(buffer->src)) = naMakeFileReadingFilename(filename);
  buffer->srctype = NA_BUFFER_SOURCE_FILE;
  buffer->srcoffset = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->flags = NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED;
  buffer->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  buffer->fillPart = naFillASDFBufferPartFile;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



//NA_DEF NAASDFBuffer* naCreateASDFBufferConstData(const void* data, NAInt bytesize){
//  NAASDFBuffer* buffer = naAlloc(NAASDFBuffer);
//  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
//  buffer->src = NA_NULL;
//  buffer->srctype = NA_BUFFER_SOURCE_CONST_DATA;
//  buffer->srcoffset = 0;
//
//  naInitList(&(buffer->parts));
//  NAASDFBufferPart* part = naNewASDFBufferPartConstData(data, bytesize);
//  
//  // Add the const data to the list.
//  naAddListLastMutable(&(buffer->parts), part);
//  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
//  naIterateList(&(buffer->iter), 1);
//  buffer->curoffset = 0;
//  buffer->curbit = 0;
//  
//  buffer->flags = NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED | NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED;
//  buffer->bufrange = naMakeRangeiWithStartAndEnd(0, bytesize);
//  buffer->fillPart = naFillASDFBufferPartConstData;
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  return buffer;
//}
//
//
//
//NA_DEF NAASDFBuffer* naCreateASDFBufferMutableData(void* data, NAInt bytesize, NAMemoryCleanup cleanup){
//  NAASDFBuffer* buffer = naAlloc(NAASDFBuffer);
//  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
//  buffer->src = NA_NULL;
//  buffer->srctype = NA_BUFFER_SOURCE_MUTABLE_DATA;
//  buffer->srcoffset = 0;
//
//  naInitList(&(buffer->parts));
//  NAASDFBufferPart* part = naNewASDFBufferPartMutableData(data, bytesize, cleanup);
//  
//  // Add the const data to the list.
//  naAddListLastMutable(&(buffer->parts), part);
//  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
//  naIterateList(&(buffer->iter), 1);
//  buffer->curoffset = 0;
//  buffer->curbit = 0;
//  
//  buffer->flags = NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED | NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED;
//  buffer->bufrange = naMakeRangeiWithStartAndEnd(0, bytesize);
//  buffer->fillPart = naFillASDFBufferPartMutableData;
//
//  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
//  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);
//
//  return buffer;
//}



NA_HDEF void naDestructASDFBuffer(NAASDFBuffer* buffer){
  switch(buffer->srctype){
  case NA_BUFFER_SOURCE_MEMORY:
    break;
  case NA_BUFFER_SOURCE_MEMORY_SOURCE:
    naReleaseASDFBuffer(buffer->src);
    break;
  case NA_BUFFER_SOURCE_BUFFER:
    naReleaseASDFBuffer(buffer->src);
    break;
  case NA_BUFFER_SOURCE_FILE:
    naCloseFile(buffer->src);
    naFree(buffer->src);
    break;
  case NA_BUFFER_SOURCE_CONST_DATA:
    break;
  case NA_BUFFER_SOURCE_MUTABLE_DATA:
    break;
  default:
    #ifndef NDEBUG
      naError("naClearASDFBuffer", "Invalid Source type");
    #endif
    break;
  }
  naForeachListMutable(&(buffer->parts), naDelete);
  naClearListIterator(&(buffer->iter));
  naClearList(&(buffer->parts));
}



NA_API NAASDFBuffer* naRetainASDFBuffer(NAASDFBuffer* buffer){
  return (NAASDFBuffer*)naRetainRefCount(&(buffer->refcount));
}



NA_API void naReleaseASDFBuffer(NAASDFBuffer* buffer){
  naReleaseRefCount(&(buffer->refcount), NA_NULL, (NAMutator)naDestructASDFBuffer);
}




NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
  return (((start + (start < 0)) / (NAInt)NA_INTERNAL_BUFFER_PART_BYTESIZE) - (start < 0)) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
// This is short for:
//  if(start < 0){
//    start = ((start / NA_INTERNAL_BUFFER_PART_BYTESIZE) - 1) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
//  }else{
//    start = ((start / NA_INTERNAL_BUFFER_PART_BYTESIZE) - 0) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
//  }
//  return start;
}



NA_HIDEF NAInt naGetBufferPartNormedEnd(NAInt end){
  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



// Moves the buffer iterator to that position which contains offset. Will
// point to the initial position if offset is not found.
NA_HIDEF void naLocateBufferPartOffset(NAListIterator* iter, NAInt offset){
  #ifndef NDEBUG
    if(naIsListIteratorEmpty(iter))
      naError("naLocateBufferPartOffset", "list should not be empty");
    if(naIsListAtInitial(iter))
      naError("naLocateBufferPartOffset", "iterator should not be at initial position");
  #endif
  const NAASDFBufferPart* part = naGetListCurrentConst(iter);
  const NAASDFBufferPart* newpart;
  while(part){
    if(naGetBufferPartEnd(part) <= offset){
      naIterateList(iter, 1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
//        if(!newpart)
//          {naCrash("naLocateBufferPartOffset", "next part inexistent"); return;}
        if(newpart && (naGetBufferPartStart(newpart) != naGetBufferPartEnd(part)))
          naError("naLocateBufferPartOffset", "next part not adjacent");
      #endif
    }else if(naGetBufferPartStart(part) > offset){
      naIterateList(iter, -1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
//        if(!newpart)
//          {naCrash("naLocateBufferPartOffset", "prev part inexistent"); return;}
        if(newpart && (naGetBufferPartEnd(newpart) != naGetBufferPartStart(part)))
          naError("naLocateBufferPartOffset", "prev part not adjacent");
      #endif
    }else{
      #ifndef NDEBUG
        if(!naContainsBufferPartOffset(part, offset))
          naError("naLocateBufferPartOffset", "final buffer part does not contain desired offset");
      #endif
      return;
    }
    part = newpart;
  }
}



// This function ensures that the full given range is available as parts in
// this buffer. If not available, the buffer is extended with sparse parts
// at the beginning and the end.
NA_HIDEF void naEnsureASDFBufferRange(NAASDFBuffer* buffer, NARangei range){
  NAASDFBufferPart* part;
  
  #ifndef NDEBUG
    if(range.length <= 0)
      naError("naEnsureASDFBufferRangeMemory", "Range length shall be >= 0");
    if((buffer->flags & NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED) && (range.origin < buffer->bufrange.origin))
      naError("naEnsureASDFBufferRangeMemory", "Range of buffer is fixed at minimum %d, trying to access range down to %d", buffer->bufrange.origin, range.origin);
    if((buffer->flags & NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED) && (naGetRangeiEnd(range) > naGetRangeiEnd(buffer->bufrange)))
      naError("naEnsureASDFBufferRangeMemory", "Range of buffer is fixed at maximum %d, trying to access range up to %d", naGetRangeiEnd(buffer->bufrange), naGetRangeiEnd(range));
  #endif
  
  if(naIsRangeiEmpty(buffer->bufrange)){
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    part = naNewASDFBufferPartSparse(range);
    naAddListLastMutable(&(buffer->parts), part);
    buffer->bufrange = range;
    // Make sure the iterator points to the part which containts the first
    // offset of range.
    naLocateListLast(&(buffer->iter));
    
  }else{
    // When the buffer is not empty, we need to make sure we add all desired
    // ranges but still can find the part containing range origin, even if
    // no sparse part needed to be added.
    NABool originpartlocated = NA_FALSE;
    
    // First, we test if we need to add a sparse part at the beginning.
    if(range.origin < buffer->bufrange.origin){
      // We create a sparse part at the beginning.
      part = naNewASDFBufferPartSparse(naMakeRangeiWithStartAndEnd(range.origin, buffer->bufrange.origin));
      naAddListFirstMutable(&(buffer->parts), part);
      buffer->bufrange = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(buffer->bufrange));
      // Make sure the iterator points to the part which containts the first
      // offset of range.
      naLocateListFirst(&(buffer->iter));
      originpartlocated = NA_TRUE;
    }
    
    // Then, we test if we need to add a sparse part at the end.
    if(naGetRangeiEnd(range) > naGetRangeiEnd(buffer->bufrange)){
      // We create a sparse part at the end.
      part = naNewASDFBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(buffer->bufrange), naGetRangeiEnd(range)));
      naAddListLastMutable(&(buffer->parts), part);
      buffer->bufrange = naMakeRangeiWithStartAndEnd(buffer->bufrange.origin, naGetRangeiEnd(range));
    }
    
    if(!originpartlocated){
      // We make sure, the buffer iterator points to a buffer part.
      // We choose to start with the last part, as usually, parts are getting
      // added at the end.
      if(naIsListAtInitial(&(buffer->iter))){naLocateListLast(&(buffer->iter));}
      
      // We locate the buffer iterator for the beginning position.
      naLocateBufferPartOffset(&(buffer->iter), range.origin);
    }
  }

}




NA_HIDEF void naFillASDFBufferPartMemory(NAASDFBuffer* buffer, NAASDFBufferPart* part){
//  naNulln(naGetASDFBufferPartBaseDataPointer(part), naGetBufferPartRangeLength(part));
}



NA_HIDEF void naFillASDFBufferPartFile(NAASDFBuffer* buffer, NAASDFBufferPart* part){
  naReadFileBytes(buffer->src, naGetASDFBufferPartBaseDataPointer(part), naGetBufferPartRangeLength(part));
}



NA_HIDEF void naFillASDFBufferPartConstData(NAASDFBuffer* buffer, NAASDFBufferPart* part){
}
NA_HIDEF void naFillASDFBufferPartMutableData(NAASDFBuffer* buffer, NAASDFBufferPart* part){
}




NA_HIDEF NABool naIsASDFBufferVolatile(NAASDFBuffer* buffer){
  return NA_FALSE;
}



NA_HIDEF NABool naHasASDFBufferParentBuffer(NAASDFBuffer* buffer){
  return (buffer->src) && (buffer->srctype != NA_BUFFER_SOURCE_NONE) && (buffer->srctype != NA_BUFFER_SOURCE_FILE);
}




// This function operates on two iterators, one being the source, the other
// the destination. The destination wants to reference a specified range within
// the source parts.
// At the end of this function, the iterators point to the parts at the end of
// the desired range.
NA_HIDEF void naFillASDFBufferIterator(NAASDFBuffer* srcbuffer, NAListIterator* dstiter, NARangei dstrange, NAInt srcoffset){
  NAInt srcrangeorigin = dstrange.origin - srcoffset;

  if(naIsASDFBufferVolatile(srcbuffer)){
    #ifndef NDEBUG
      naError("naFillASDFBufferIterator", "volatile buffers not implemented yet");
    #endif
  }

  // We perform the preparation as long as there are still bytes left in range.
  while(dstrange.length){

    #ifndef NDEBUG
      if(dstrange.length < 0)
        naError("naFillASDFBufferIterator", "length is negative");
    #endif

    // We get the current parts of src and dst.
    NAASDFBufferPart* srcpart = naGetListCurrentMutable(&(srcbuffer->iter));
    NAASDFBufferPart* dstpart = naGetListCurrentMutable(dstiter);
    // We count, how many bytes are left on both parts.
    NAInt remainingsrcbytes = (naGetBufferPartEnd(srcpart) - srcrangeorigin);
    NAInt remainingdstbytes = (naGetBufferPartEnd(dstpart) - dstrange.origin);
    // we choose the number of bytes which is smaller.
    NAInt remainingbytes = naMini(remainingsrcbytes, remainingdstbytes);
    remainingbytes = naMini(remainingbytes, dstrange.length);

    // We decide what to do based on the dst buffer being sparse or not.
    if(naIsASDFBufferPartSparse(dstpart)){
      // If the dst buffer is sparse, we split the part fitting the desired
      // number of bytes...
      naSplitASDFBufferSparsePart(dstiter, naMakeRangei(dstrange.origin, remainingbytes));
      
      // ... and replace the current part with a reference to the source part.
      dstpart = naGetListCurrentMutable(dstiter);
      naReferenceASDFBufferPart(dstpart, srcpart, srcoffset);
      
      // Then we try to combine this newly available part with its neighbors.
      naCombineBufferPartAdjacents(dstiter);

      // Now, the current dst part is completely filled. Move to the next one.
      naIterateList(dstiter, 1);
      
    }else{
      // The current buffer part already contains memory.

      // These bytes can simply be skipped, except if the source buffer is
      // volatile
      if(naIsASDFBufferVolatile(srcbuffer)){
        // if the source buffer is volatile, we need to replace the existing
        // bytes.
        #ifndef NDEBUG
          naError("naFillASDFBufferIterator", "volatile buffers not implemented yet");
        #endif
      }
      
      if(remainingbytes == remainingdstbytes){naIterateList(dstiter, 1);}
    }

    if(remainingbytes == remainingsrcbytes){naIterateList(&(srcbuffer->iter), 1);}

    dstrange = naMakeRangeiE(dstrange.origin + remainingbytes, dstrange.length - remainingbytes);
    srcrangeorigin += remainingbytes;

  }
}



// This function prepares the given buffer such that in contains all parts
// storing memory for the given range. It is guaranteed, that at the end of
// this function, the iterator of the buffer points at that part containing
// the origin of the given range.
NA_HIDEF void naPrepareASDFBuffer(NAASDFBuffer* buffer, NARangei range){
  NAASDFBufferPart* part;
  
  // We store the desired start position for later.
  NAInt originalrangeorigin = range.origin;

  if(naHasASDFBufferParentBuffer(buffer)){

    // First, we ensure, that there are buffer parts in the parts list of this
    // buffer, spanning the desired range by creating sparse buffers wherever
    // necessary.
    naEnsureASDFBufferRange(buffer, range);
    // After this function call, the iterator points to the part containing
    // the origin of range.

    // Then, we make sure that parent buffer is fully prepared.
    NARangei srcrange = naMakeRangei(range.origin - buffer->srcoffset, range.length);
    naPrepareASDFBuffer(buffer->src, srcrange);

    // Finally, we simply reference all parts of the source.
    naFillASDFBufferIterator(buffer->src, &(buffer->iter), range, buffer->srcoffset);
    
  }else{
    // This buffer manages its own linear memory. Such memory is usually
    // aligned by NA_INTERNAL_BUFFER_PART_BYTESIZE.
    
    range = naMakeRangeiWithStartAndEnd(naGetBufferPartNormedStart(range.origin), naGetBufferPartNormedEnd(naGetRangeiEnd(range)));

    // First, we ensure, that there are buffer parts in the parts list
    // spanning the desired normed range by creating sparse buffers if
    // necessary.
    naEnsureASDFBufferRange(buffer, range);
    // After this function call, the iterator points to the part containing
    // the origin of range.

    // The, we go through the list of parts and change all sparse buffers into
    // memory when within range.

    // We perform the preparation as long as there are bytes left in range.
    while(range.length > 0){
      
      part = naGetListCurrentMutable(&(buffer->iter));

      #ifndef NDEBUG
        // This should not happen, as range.length shall be smallerequal 0 when
        // the list comes to an end.
        if(!part)
          naError("naPrepareASDFBuffer", "list overflow. This should not happen.");
      #endif

      // When the current part already contains bytes, we can simply overjump
      // all bytes contained in this part. If this part is sparse though ...
      if(naIsASDFBufferPartSparse(part)){
        // ... we create buffer parts with actual memory.
        NARangei subrange;

        // We create a suitable range within the parts range which tries to
        // be aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE but has a certain
        // margin to be bigger and hence reduce the number of small parts.
        NAInt normedstart = naGetBufferPartNormedStart(range.origin);
        if((normedstart - naGetBufferPartStart(part)) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedstart = naGetBufferPartStart(part);}
        NAInt normedend = naGetBufferPartNormedEnd(range.origin + NA_INTERNAL_BUFFER_PART_BYTESIZE);
        if((naGetBufferPartEnd(part) - normedend) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedend = naGetBufferPartEnd(part);}

        // Note that the previous computation of normedstart and normedend also
        // automatically handles parts being smaller than the desired range.
        subrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);
        #ifndef NDEBUG
          if(!naContainsRangeiRange(naGetBufferPartRange(part), subrange))
            naError("naPrepareASDFBuffer", "part range does not contain subrange");
        #endif

        // Then we split the sparse buffer if necessary.
        naSplitASDFBufferSparsePart(&(buffer->iter), subrange);
        
        // Now, iter points at a sparse part exactly of range subrange
        // which can be filled.
        part = naGetListCurrentMutable(&(buffer->iter));
        #ifndef NDEBUG
          if(!naEqualRangei(subrange, naGetBufferPartRange(part)))
            naError("naPrepareASDFBuffer", "range lenghts do not match");
        #endif
        naAllocateBufferPartMemory(part);
        buffer->fillPart(buffer, part);

      }

      // Reaching here, the current part is filled with memory. We advance
      // to the next part. Note that the calculation of the new range will
      // most probably result in a negative length when reaching the end of
      // the desired range, hence breaking the loop condition.

      range = naMakeRangeiWithStartAndEnd(naGetBufferPartEnd(part), naGetRangeiEnd(range));
      naIterateList(&(buffer->iter), 1);

    }
 
  }
  
  // Reaching here, iter points to the part one after the part containing the
  // maximum offset of range. We locate the buffer iterator at the first
  // desired offset. Note that we can not use a cached NAListPos as this might
  // have changed during this preparation.
  if(naIsListAtInitial(&(buffer->iter))){naIterateList(&(buffer->iter), -1);}
  naLocateBufferPartOffset(&(buffer->iter), originalrangeorigin);

  #ifndef NDEBUG
    part = naGetListCurrentMutable(&(buffer->iter));
    if(!naContainsBufferPartOffset(part, originalrangeorigin))
      naError("naPrepareASDFBuffer", "start offset not in range of current part.");
  #endif
  
  // Reaching here, the buffer iterator points at the first part containing
  // the range origin.
}




NA_DEF void naDeallocASDFBuffer(NAASDFBuffer* buffer){
  naClearListIterator(&(buffer->iter));
  naClearList(&(buffer->parts));
  naFree(buffer);
}




NA_API NARangei naDetermineBufferRange(NAASDFBuffer* buffer){
  NAFilesize filesize;
  if(!naHasBufferDeterminedRange(buffer)){
    if(buffer->srctype == NA_BUFFER_SOURCE_FILE){
      filesize = naComputeFileBytesize((NAFile*)(buffer->src));
      #ifndef NDEBUG
        #if (NA_FILESIZE_BITS > NA_SYSTEM_ADDRESS_BITS)
          if(filesize > NA_INT_MAX)
            naError("naDetermineBufferBytesize", "Can not store full file with %d bits", NA_SYSTEM_ADDRESS_BITS);
        #endif
      #endif
      naEnsureASDFBufferRange(buffer, naMakeRangei(0, filesize));
    }
    buffer->flags |= NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED | NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED;
  }
  return buffer->bufrange;
}



NA_DEF NABool naHasBufferDeterminedRange(const NAASDFBuffer* buffer){
  return ((buffer->flags & NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED) && (buffer->flags & NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED));
}



NA_DEF NARangei naGetASDFBufferRange(const NAASDFBuffer* buffer){
  return buffer->bufrange;
}



NA_DEF NABool naIsASDFBufferEmpty(const NAASDFBuffer* buffer){
  return naIsRangeiEmpty(buffer->bufrange);
}



NA_DEF NAInt naTellASDFBuffer(const NAASDFBuffer* buffer){
  return buffer->curoffset;
}



NA_DEF NABool naIsBufferAtEnd(const NAASDFBuffer* buffer){
  #ifndef NDEBUG
    if(!naHasBufferDeterminedRange(buffer))
      naError("naIsBufferAtEnd", "Buffer has no fixed max position. Use naDetermineBufferRange.");
  #endif
  return (buffer->curoffset > naGetRangeiMax(buffer->bufrange));
}



NA_DEF void naSetBufferEndianness(NAASDFBuffer* buffer, NAInt endianness){
  buffer->endianness = endianness;
  buffer->converter = naMakeEndiannessConverter(endianness, NA_ENDIANNESS_NATIVE);
}



NA_DEF NAInt naGetBufferEndianness(NAASDFBuffer* buffer){
  return buffer->endianness;
}



NA_DEF void naSeekASDFBufferAbsolute(NAASDFBuffer* buffer, NAInt offset){
  buffer->curoffset = offset;
  if(!naIsASDFBufferEmpty(buffer)){
    if(naIsListAtInitial(&(buffer->iter))){naLocateListLast(&(buffer->iter));}
    naLocateBufferPartOffset(&(buffer->iter), offset);
  }
}



NA_DEF void naSeekASDFBufferRelative(NAASDFBuffer* buffer, NAInt offset){
  NAInt absoffset = buffer->curoffset + offset;
  naSeekASDFBufferAbsolute(buffer, absoffset);
}



NA_DEF void naSeekASDFASDFBuffer(NAASDFBuffer* buffer, NAInt offset){
  NAInt absoffset = buffer->srcoffset + offset;
  naSeekASDFBufferAbsolute(buffer, absoffset);
}



NA_DEF void naWriteASDFBufferBytes(NAASDFBuffer* buffer, const void* data, NAInt bytesize){
  const NAByte* src = data;
  NAByte* dst;

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, bytesize));
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  
  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts.
  while(bytesize){
    // The part pointed to by the iterator should be the one containing offset.
    NAASDFBufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    
    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.
    #ifndef NDEBUG
      if(naIsASDFBufferPartSparse(part))
        naError("naPutASDFBufferBytes", "Current part is sparse");
      if(!naContainsBufferPartOffset(part, buffer->curoffset))
        naError("naPutASDFBufferBytes", "Current part does not contain current offset");
    #endif
    
    // We get the data pointer where we can write bytes.
    dst = naGetASDFBufferPartDataPointer(part, buffer->curoffset);
    // We detect, how many bytes actually can be put into the current part.
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("naPutASDFBufferBytes", "possible length invalid");
      if(possiblelength > naGetBufferPartRangeLength(part))
        naError("naPutASDFBufferBytes", "buffer overflow expected");
    #endif
    
    if(possiblelength > bytesize){
      // If we can put in more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      naCopyn(dst, src, (NAUInt)bytesize);
      buffer->curoffset += bytesize;
      // src += bytesize; // this line is not needed as the loop will end.
      bytesize = 0;
    }else{
      // We can only put a portion of the source into the current part. We
      // copy as many bytes as possible and advance to the next part.
      naCopyn(dst, src, (NAUInt)possiblelength);
      buffer->curoffset += possiblelength;
      src += possiblelength;
      bytesize -= possiblelength;
      // Note that when possiblelength matches bytesize, the part will also
      // advance which is correct behaviour, as after this function, the
      // iterator shall always point at the part containing the current offset.
      naIterateList(&(buffer->iter), 1);
    }
  }
  
  // Reaching here, the whole range has been written to the parts and both
  // curoffset and iter point to the current position again.
}



NA_DEF void naWriteBufferi8(NAASDFBuffer* buffer, int8 value){
  buffer->converter.convert8(&value);
  naWriteASDFBufferBytes(buffer, &value, 1);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi16(NAASDFBuffer* buffer, int16 value){
  buffer->converter.convert16(&value);
  naWriteASDFBufferBytes(buffer, &value, 2);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi32(NAASDFBuffer* buffer, int32 value){
  buffer->converter.convert32(&value);
  naWriteASDFBufferBytes(buffer, &value, 4);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi64(NAASDFBuffer* buffer, int64 value){
  buffer->converter.convert64(&value);
  naWriteASDFBufferBytes(buffer, &value, 8);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferu8(NAASDFBuffer* buffer, uint8 value){
  buffer->converter.convert8(&value);
  naWriteASDFBufferBytes(buffer, &value, 1);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu16(NAASDFBuffer* buffer, uint16 value){
  buffer->converter.convert16(&value);
  naWriteASDFBufferBytes(buffer, &value, 2);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu32(NAASDFBuffer* buffer, uint32 value){
  buffer->converter.convert32(&value);
  naWriteASDFBufferBytes(buffer, &value, 4);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu64(NAASDFBuffer* buffer, uint64 value){
  buffer->converter.convert64(&value);
  naWriteASDFBufferBytes(buffer, &value, 8);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferf(NAASDFBuffer* buffer, float value){
  buffer->converter.convert32(&value);
  naWriteASDFBufferBytes(buffer, &value, 4);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferd(NAASDFBuffer* buffer, double value){
  buffer->converter.convert64(&value);
  naWriteASDFBufferBytes(buffer, &value, 8);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferi8v(NAASDFBuffer* buffer, const int8* src, NAInt count){
  int8 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 1));
  while(count){
    value = *src;
    buffer->converter.convert8(&value);
    naWriteASDFBufferBytes(buffer, &value, 1);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi16v(NAASDFBuffer* buffer, const int16* src, NAInt count){
  int16 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 2));
  while(count){
    value = *src;
    buffer->converter.convert16(&value);
    naWriteASDFBufferBytes(buffer, &value, 2);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi32v(NAASDFBuffer* buffer, const int32* src, NAInt count){
  int32 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4));
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naWriteASDFBufferBytes(buffer, &value, 4);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi64v(NAASDFBuffer* buffer, const int64* src, NAInt count){
  int64 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8));
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naWriteASDFBufferBytes(buffer, &value, 8);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu8v(NAASDFBuffer* buffer, const uint8* src, NAInt count){
  uint8 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 1));
  while(count){
    value = *src;
    buffer->converter.convert8(&value);
    naWriteASDFBufferBytes(buffer, &value, 1);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu16v(NAASDFBuffer* buffer, const uint16* src, NAInt count){
  uint16 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 2));
  while(count){
    value = *src;
    buffer->converter.convert16(&value);
    naWriteASDFBufferBytes(buffer, &value, 2);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu32v(NAASDFBuffer* buffer, const uint32* src, NAInt count){
  uint32 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4));
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naWriteASDFBufferBytes(buffer, &value, 4);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu64v(NAASDFBuffer* buffer, const uint64* src, NAInt count){
  uint64 value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8));
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naWriteASDFBufferBytes(buffer, &value, 8);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferfv(NAASDFBuffer* buffer, const float* src, NAInt count){
  float value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4));
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naWriteASDFBufferBytes(buffer, &value, 4);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferdv(NAASDFBuffer* buffer, const double* src, NAInt count){
  double value;
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8));
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naWriteASDFBufferBytes(buffer, &value, 8);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferBuffer(NAASDFBuffer* dstbuffer, NAASDFBuffer* srcbuffer, NAInt bytesize){
  void* prevsrc = dstbuffer->src;
  NAASDFBufferType prevsrctype = dstbuffer->srctype;
  NAInt prevsrcoffset = dstbuffer->srcoffset;
  // todo: store volatile flag
  // todo: Make BufferSource struct
  
  dstbuffer->src = srcbuffer;
  dstbuffer->srctype = NA_BUFFER_SOURCE_BUFFER;
  dstbuffer->srcoffset = dstbuffer->curoffset;
  naPrepareASDFBuffer(dstbuffer, naMakeRangei(dstbuffer->curoffset, srcbuffer->bufrange.length));

  dstbuffer->src = prevsrc;
  dstbuffer->srctype = prevsrctype;
  dstbuffer->srcoffset = prevsrcoffset;
}



NA_DEF void naRepeatBufferBytes(NAASDFBuffer* buffer, NAInt distance, NAInt bytesize){
  NAList* partlist;
  NAInt finalpos;
  NAInt writeoffset;
  NAInt readoffset;
  NAASDFBufferPart* writepart;
  NAASDFBufferPart* readpart;
  NAInt remainingwrite;
  NAInt remainingread;
  
  // Prepare the read part
  readoffset = buffer->curoffset - distance;
  naPrepareASDFBuffer(buffer, naMakeRangei(readoffset, bytesize));
  NAListIterator readiter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListPosition(&readiter, naGetListCurrentPosition(&(buffer->iter)));
  readpart = naGetListCurrentMutable(&readiter);
  remainingread = naGetBufferPartEnd(readpart) - readoffset;

  // Prepare the write part
  writeoffset = buffer->curoffset;
  naPrepareASDFBuffer(buffer, naMakeRangei(writeoffset, bytesize));
  writepart = naGetListCurrentMutable(&(buffer->iter));
  remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;
  
  // Now start copying the buffers.
  while(1){
    remainingread = naMini(remainingread, distance);
    NAInt remaing = (remainingwrite < remainingread) ? remainingwrite : remainingread;
    remaing = naMini(remaing, bytesize);
    
    naCopyn(naGetASDFBufferPartDataPointer(writepart, writeoffset), naGetASDFBufferPartDataPointer(readpart, readoffset), remaing);
    bytesize -= remaing;
    writeoffset += remaing;
    readoffset += remaing;

    remainingread = naGetBufferPartEnd(readpart) - readoffset;
    remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;
    
    if(remainingread == NA_ZERO){
      naIterateList(&readiter, 1);
      readpart = naGetListCurrentMutable(&readiter);
      remainingread = naGetBufferPartEnd(readpart) - readoffset;
    }
    if(remainingwrite == NA_ZERO){
      naIterateList(&buffer->iter, 1);
      writepart = naGetListCurrentMutable(&buffer->iter);
      remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;
    }
    if(!bytesize){break;}
  }

  naClearListIterator(&readiter);

}



NA_DEF void naWriteBufferToFile(NAASDFBuffer* buffer, NAFile* file){
  NAInt bytesize;
  NAInt curoffset;
  NAListIterator iter;

  #ifndef NDEBUG
    if(!naHasBufferDeterminedRange(buffer))
      naError("naWriteBufferToFile", "Buffer has no determined range. Use naDetermineBufferRange");
  #endif

  bytesize = buffer->bufrange.length;
  if(!bytesize){return;}

  curoffset = buffer->bufrange.origin;
  iter = naMakeListIteratorMutator(&(buffer->parts));
  naLocateListFirst(&iter);
  
  while(bytesize){
    NAASDFBufferPart* curpart;
    NAInt remainingbytes;
    NAByte* src;

    curpart = naGetListCurrentMutable(&iter);
    remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
    src = naGetASDFBufferPartDataPointer(curpart, curoffset);
    
    #ifndef NDEBUG
      if(naIsASDFBufferPartSparse(curpart))
        naError("naWriteBufferToFile", "Buffer contains sparse parts.");
    #endif
    
    if(bytesize > remainingbytes){
      naWriteFileBytes(file, src, remainingbytes);
      naIterateList(&iter, 1);
      curoffset += remainingbytes;
      bytesize -= remainingbytes;
    }else{
      naWriteFileBytes(file, src, bytesize);
      bytesize = 0;
    }
  }
  
  naClearListIterator(&iter);

}



// ///////////////////////////////////////
// READING
// ///////////////////////////////////////


NA_API void naReadASDFBufferBytes(NAASDFBuffer* buffer, void* data, NAInt bytesize){
  NAByte* dst = data;
  NAByte* src;

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, bytesize));
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  
  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts.
  while(bytesize){
    // The part pointed to by the iterator should be the one containing offset.
    NAASDFBufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    
    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.
    #ifndef NDEBUG
      if(naIsASDFBufferPartSparse(part))
        naError("naPutASDFBufferBytes", "Current part is sparse");
      if(!naContainsBufferPartOffset(part, buffer->curoffset))
        naError("naPutASDFBufferBytes", "Current part does not contain current offset");
    #endif
    
    // We get the data pointer where we can read bytes.
    src = naGetASDFBufferPartDataPointer(part, buffer->curoffset);
    // We detect, how many bytes actually can be read from the current part.
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("naPutASDFBufferBytes", "possible length invalid");
      if(possiblelength > naGetBufferPartRangeLength(part))
        naError("naPutASDFBufferBytes", "buffer overflow expected");
    #endif
    
    if(possiblelength > bytesize){
      // If we can get out more bytes than needed, we copy all remaining bytes
      // and stay on this part.
      naCopyn(dst, src, (NAUInt)bytesize);
      buffer->curoffset += bytesize;
      // dst += bytesize; // this line is not needed as the loop will end.
      bytesize = 0;
    }else{
      // We can only get a portion of the current part into the destination. We
      // copy as many bytes as possible and advance to the next part.
      naCopyn(dst, src, (NAUInt)possiblelength);
      buffer->curoffset += possiblelength;
      dst += possiblelength;
      bytesize -= possiblelength;
      // Note that when possiblelength matches bytesize, the part will also
      // advance which is correct behaviour, as after this function, the
      // iterator shall always point at the part containing the current offset.
      naIterateList(&(buffer->iter), 1);
    }
  }
  
  // Reaching here, the whole range has been written to the parts and both
  // curoffset and iter point to the current position again.
}



NA_DEF int8 naReadBufferi8(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int8 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi8", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 1);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF int16 naReadBufferi16(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int16 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi16", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 2);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF int32 naReadBufferi32(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int32 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi32", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 4);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF int64 naReadBufferi64(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int64 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi64", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 8);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF uint8 naReadBufferu8(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint8 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu8", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 1);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF uint16 naReadBufferu16(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint16 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu16", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 2);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF uint32 naReadBufferu32(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint32 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu32", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 4);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF uint64 naReadBufferu64(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint64 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu64", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 8);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF float naReadBufferf(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  float value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferf", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 4);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF double naReadBufferd(NAASDFBuffer* buffer){
  // Declaration before implementation. Needed for C90.
  double value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferd", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, &value, 8);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF void naReadBufferi8v(NAASDFBuffer* buffer, int8* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi8v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi8v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 1);
  buffer->converter.convert8v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi16v(NAASDFBuffer* buffer, int16* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi16v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi16v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 2);
  buffer->converter.convert16v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi32v(NAASDFBuffer* buffer, int32* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi32v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi32v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 4);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi64v(NAASDFBuffer* buffer, int64* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi64v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi64v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 8);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



NA_DEF void naReadBufferu8v(NAASDFBuffer* buffer, uint8* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu8v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu8v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 1);
  buffer->converter.convert8v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu16v(NAASDFBuffer* buffer, uint16* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu16v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu16v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 2);
  buffer->converter.convert16v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu32v(NAASDFBuffer* buffer, uint32* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu32v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu32v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 4);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu64v(NAASDFBuffer* buffer, uint64* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu64v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu64v", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 8);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



NA_DEF void naReadBufferfv(NAASDFBuffer* buffer, float* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferfv", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferfv", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 4);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferdv(NAASDFBuffer* buffer, double* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      {naCrash("naReadBufferdv", "dst is Null pointer."); return;}
    if(buffer->curbit != 0)
      naError("naReadBufferdv", "Bit offset not 0.");
  #endif
  naReadASDFBufferBytes(buffer, dst, count * 8);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



NA_DEF NABool naReadBufferBit(NAASDFBuffer* buffer){
  NAByte* src;
  NABool bit;

  naPrepareASDFBuffer(buffer, naMakeRangei(buffer->curoffset, 1));
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  NAASDFBufferPart* part = naGetListCurrentMutable(&(buffer->iter));
  src = naGetASDFBufferPartDataPointer(part, buffer->curoffset);
  bit = (*src >> buffer->curbit) & 0x01;
  
  buffer->curbit++;
  if(buffer->curbit == 8){
    // We advance to the next byte.
    buffer->curbit = 0;
    buffer->curoffset++;
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;
    if(!possiblelength){
      naIterateList(&(buffer->iter), 1);
    }
  }
  
  return bit;
}



NA_DEF NAUInt naReadBufferBits(NAASDFBuffer* buffer, uint8 count){
  NAUInt retint = 0;
  NABool curbit;
  NAUInt curmask = 1;
  #ifndef NDEBUG
    if(count > NA_SYSTEM_ADDRESS_BITS)
      naError("naReadBufferBits", "Can read %d bits at max.", NA_SYSTEM_ADDRESS_BITS);
  #endif
  while(count){
    curbit = naReadBufferBit(buffer);
    retint |= curmask * (NAUInt)curbit;
    curmask <<= 1;
    count--;
  }
  return retint;
}



NA_DEF void naPadBufferBits(NAASDFBuffer* buffer){
  if(buffer->curbit != 0){
    buffer->curbit = 0;
    buffer->curoffset++;
    NAASDFBufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;
    if(!possiblelength){
      naIterateList(&(buffer->iter), 1);
    }
  }
}




NA_DEF void naAccumulateBufferToChecksum(NAASDFBuffer* buffer, NAChecksum* checksum){
  NAInt bytesize;
  NAInt curoffset;
  NAListIterator iter;

  #ifndef NDEBUG
    if(!naHasBufferDeterminedRange(buffer))
      naError("naAccumulateBufferToChecksum", "Buffer has no determined range. Use naDetermineBufferRange");
  #endif

  bytesize = buffer->bufrange.length;
  if(!bytesize){return;}

  curoffset = buffer->bufrange.origin;
  iter = naMakeListIteratorMutator(&(buffer->parts));
  naLocateListFirst(&iter);
  
  while(bytesize){
    NAASDFBufferPart* curpart;
    NAInt remainingbytes;
    NAByte* src;

    curpart = naGetListCurrentMutable(&iter);
    remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
    src = naGetASDFBufferPartDataPointer(curpart, curoffset);
    
    #ifndef NDEBUG
      if(naIsASDFBufferPartSparse(curpart))
        naError("naAccumulateBufferToChecksum", "Buffer contains sparse parts. Can not compute checksum");
    #endif
    
    if(bytesize > remainingbytes){
      naAccumulateChecksum(checksum, src, remainingbytes);
      naIterateList(&iter, 1);
      curoffset += remainingbytes;
      bytesize -= remainingbytes;
    }else{
      naAccumulateChecksum(checksum, src, bytesize);
      bytesize = 0;
    }
  }
  
  naClearListIterator(&iter);
}


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
