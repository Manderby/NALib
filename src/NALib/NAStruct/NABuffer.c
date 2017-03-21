
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "../NABuffer.h"



// These are some implementation notes from the author. There are three structs
// important in this whole buffer implementation:
//
// NABuffer
// NABufferSource
// NABufferPart.
//
// An NABuffer is the realization of linear memory. This linear memory is split
// into small parts which can be shared among the different buffers.
//
// An NABufferPart stores a range of bytes. Either it is a sparse part, only
// defining the range which should be inside that part, or it is a filled part
// which has a memory block of actually allocated bytes.
//
// An NABufferSource defines, how the parts shall be filled. It can be for
// example insecure memory, secure memory (initialized to binarz zero), the
// contents of a file or the contents of another buffer.
//
// Most difficult piece to understand is the NABufferSource:
//
// When a source is available, this means, that there can be memory allocated
// and filled with the appropriate bytes. If a buffer has no source, it can not
// expand its range or reload contents which are versatile or which have only
// been added as sparse parts.
//
// A source holds a void pointer src* defining, how the bytes are filled wheres
// the type of the source defines, what that src* actually is is.
//
// Type            | type of src* | Byte-Fill                  | part align
// ----------------+--------------+----------------------------+--------------
// no source       | -            | no filling                 | Arbitrary
// MEMORY_INSECURE | NA_NULL      | Uninitialized memory       | Aligned
// MEMORY_SECURE   | NA_NULL      | Memory initialized to zero | Aligned
// FILE            | NAFile       | Contents of an input file  | Aligned
// BUFFER          | NABuffer     | Contents of another buffer | Arbitrary
//
// A source can be shared among different buffers. Each buffer which wants
// to use a source has to retain the src* pointer if available. Every other
// part of the NABufferSource structure is individual to the NABuffer.
//
// An NABufferSource can have an offset which denotes how the indices of
// the bytes behind src* are shifted relative to the indeces of the buffer
// which owns the NABufferSource.
//
// Important to understand is that buffers with memory and file sources will
// allocate buffer parts which are aligned at byte boundaries defined by
// NA_INTERNAL_BUFFER_PART_BYTESIZE.



#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif


//#define NA_BUFFER_SOURCE_NONE               0x0000
//#define NA_BUFFER_SOURCE_MEMORY_INSECURE    0x0001
//#define NA_BUFFER_SOURCE_MEMORY_SECURE      0x0002
//#define NA_BUFFER_SOURCE_BUFFER             0x0003
//#define NA_BUFFER_SOURCE_FILE               0x0004
//#define NA_BUFFER_SOURCE_TYPE_MASK         (NAUInt)0x0007
//
////#define NA_BUFFER_SOURCE_LIMITS_RANGE       0x0008
//
//#define NA_BUFFER_SOURCE_MASK               (NAUInt)0x000f



//#define NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED  0x0100
//#define NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED  0x0200





// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////


typedef struct NABufferPart NABufferPart;
struct NABufferPart{
  NAPointer* data;
  NAInt origin;           // The origin of the first byte in the data pointer
  NARangei range;         // The available range in this buffer part.
};

NA_HIAPI void naDestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, naDestructBufferPart);




// Creates a memory block with sparse memory.
// A sparse buffer is always initialized with the origin being the same as the
// range.origin. This can later on be changed with naReferenceBufferPart.
NA_HIDEF NABufferPart* naNewBufferPartSparse(NARangei range){
  NABufferPart* part = naNew(NABufferPart);
  part->data = NA_NULL;
  part->origin = range.origin;
  part->range = range;
  return part;
}



// Creates a memory block with given const data buffer
NA_HIDEF NABufferPart* naNewBufferPartConstData(const NAByte* data, NAInt bytesize){
  NABufferPart* part = naNew(NABufferPart);
  part->data = naNewPointerConst(data);
  part->origin = 0;
  part->range = naMakeRangei(0, bytesize);
  return part;
}



// Creates a memory block with given mutable data buffer
NA_HIDEF NABufferPart* naNewBufferPartMutableData(NAByte* data, NAInt bytesize, NAMemoryCleanup cleanup){
  NABufferPart* part = naNew(NABufferPart);
  part->data = naNewPointerMutable(data, cleanup, NA_NULL);
  part->origin = 0;
  part->range = naMakeRangei(0, bytesize);
  return part;
}



// This function takes a sparse part and makes it referencing a second part
// with a given offset. The range which is referenced is defined by the range
// stored in the part.
NA_HIDEF void naReferenceBufferPart(NABufferPart* part, NABufferPart* srcpart, NAInt srcoffset){
  #ifndef NDEBUG
    if(part->data)
      naError("naReferenceBufferPart", "Part already references a part");
  #endif
  part->data = naRetainPointer(srcpart->data);
  part->origin = srcpart->origin + srcoffset;
  #ifndef NDEBUG
    if(part->origin > part->range.origin)
      naError("naReferenceBufferPart", "origin wrong");
  #endif
}



NA_HIDEF void naAllocateBufferPartMemory(NABufferPart* part){
  #ifndef NDEBUG
    if(part->data)
      naError("naAllocateBufferPartMemory", "Part already has memory");
  #endif
  part->data = naNewPointerMutable(naMalloc(part->range.length), NA_MEMORY_CLEANUP_NA_FREE, NA_NULL);
}



NA_HIDEF void naDestructBufferPart(NABufferPart* part){
  if(part->data){naReleasePointer(part->data);}
}



NA_HIDEF NABool naIsBufferPartSparse(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naIsBufferPartSparse", "part is Null pointer"); return NA_FALSE;}
  #endif
  return (part->data == NA_NULL);
}



//NA_HIDEF NAInt naGetBufferPartRangeLength(const NABufferPart* part){
//  #ifndef NDEBUG
//    if(!part)
//      {naCrash("naGetBufferPartRangeLength", "part is Null pointer"); return 0;}
//  #endif
//  return part->range.length;
//}



NA_HIDEF NAInt naGetBufferPartEnd(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartEnd", "part is Null pointer"); return 0;}
  #endif
  return naGetRangeiEnd(part->range);
}



NA_HIDEF NARangei naGetBufferPartRange(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartRange", "part is Null pointer"); return naMakeRangei(0, 0);}
  #endif
  return part->range;
}



NA_HIDEF NAInt naGetBufferPartStart(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartStart", "part is Null pointer"); return 0;}
  #endif
  return part->range.origin;
}



#ifndef NDEBUG
  NA_HIDEF NABool naContainsBufferPartOffset(const NABufferPart* part, NAInt offset){
    return naContainsRangeiOffset(part->range, offset);
  }
#endif



// This function returns NA_TRUE, if both parts have the same data pointer
// and the ranges are adjacent.
NA_HIDEF NABool naAreBufferPartsEqualAndAdjacent(NABufferPart* startpart, NABufferPart* endpart){
  #ifndef NDEBUG
    if(naIsBufferPartSparse(startpart) && naIsBufferPartSparse(endpart))
      naError("naAreBufferPartsEqualAndAdjacent", "Both parts are sparse");
  #endif
  if((startpart->data == endpart->data) && (naGetRangeiEnd(startpart->range) == endpart->range.origin)){
    #ifndef NDEBUG
      if(startpart->origin != endpart->origin)
        naError("naAreBufferPartsEqualAndAdjacent", "origin of the two parts do not match");
    #endif
    return NA_TRUE;
  }else{
    return NA_FALSE;
  }
}



// This function combines the current part given by iter with its previous and
// next neighbor if possible. 
NA_HIDEF void naCombineBufferPartAdjacents(NAListIterator* iter){
  NABufferPart* part = naGetListCurrentMutable(iter);
  NABufferPart* prevpart = naGetListPrevMutable(iter);
  NABufferPart* nextpart = naGetListNextMutable(iter);
  
  if(prevpart && naAreBufferPartsEqualAndAdjacent(prevpart, part)){
    // we can combine the two parts.
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(prevpart), naGetBufferPartEnd(part));
    // then, we remove the previous part and delete it.
    naRemoveListPrevMutable(iter);
    naDelete(prevpart);
  }
  if(nextpart && naAreBufferPartsEqualAndAdjacent(part, nextpart)){
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
NA_HIDEF void naSplitBufferSparsePart(NAListIterator* iter, NARangei range){
  NABufferPart* part = naGetListCurrentMutable(iter);

  #ifndef NDEBUG
    if(!part){
      naCrash("naSplitBufferSparsePart", "iterator must not be at initial position");
      return;
    }
    if(!naIsBufferPartSparse(part))
      naError("naSplitBufferSparsePart", "current part is not sparse");
    if(!naContainsRangeiRange(naGetBufferPartRange(part), range))
      naError("naSplitBufferSparsePart", "part range does not contain given range");
  #endif

  if(naEqualRangei(naGetBufferPartRange(part), range)){
    // This sparse part fits the desired range already.
    // Nothing to do.
    
  }else if(naGetBufferPartStart(part) == range.origin){
    // The desired part will be put at the start of this sparse part.
    part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part));
    part->origin = part->range.origin;
    part = naNewBufferPartSparse(range);
    naAddListBeforeMutable(iter, part);
    naIterateList(iter, -1);
    
  }else if(naGetBufferPartEnd(part) == naGetRangeiEnd(range)){
    // The desired part will be put at the end of this sparse part.
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
    part->origin = part->range.origin;
    part = naNewBufferPartSparse(range);
    naAddListAfterMutable(iter, part);
    naIterateList(iter, 1);
    
  }else{
    // The desired part will be put in the middle of this sparse part.
    NABufferPart* lastpart = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part)));
    naAddListAfterMutable(iter, lastpart);
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
    part->origin = part->range.origin;
    part = naNewBufferPartSparse(range);
    naAddListAfterMutable(iter, part);
    naIterateList(iter, 1);
  }
}



// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HIDEF NAByte* naGetBufferPartDataPointer(const NABufferPart* part, NAInt offset){
  #ifndef NDEBUG
    if(naIsBufferPartSparse(part))
      naError("naGetBufferPartDataPointer", "buffer part is sparse");
    if(!naContainsRangeiOffset(part->range, offset))
      naError("naGetBufferPartDataPointer", "offset not inside buffer part");
    if(offset - part->origin < 0)
      naError("naGetBufferPartDataPointer", "offset calculation wrong");
  #endif
  return &(((NAByte*)naGetPointerMutable(part->data))[offset - part->origin]);
}


// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HIDEF NAByte* naGetBufferPartBaseDataPointer(const NABufferPart* part){
  #ifndef NDEBUG
    if(naIsBufferPartSparse(part))
      naError("naGetBufferPartBaseDataPointer", "buffer part is sparse");
  #endif
  return (NAByte*)naGetPointerMutable(part->data);
}







// //////////////////////////////////////
// Buffer Source
// //////////////////////////////////////


#define NA_BUFFER_SOURCE_BUFFER   0x02



NA_HIDEF NABufferSource* naCreateBufferSource(NABufferSourceDescriptor descriptor){
  NABufferSource* source = naAlloc(NABufferSource);
  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);

  source->desc = descriptor;  
  
  return source;
}



NA_HIDEF NABufferSource* naCreateBufferSourceBuffer(NABufferSourceDescriptor descriptor){
  NABufferSource* source = naCreateBufferSource(descriptor);
  source->desc.flags |= NA_BUFFER_SOURCE_BUFFER;
  return source;
}




void naReadFileBufferPart(void* data, void* dst, NARangei range){
  naReadFileBytes(data, dst, range.length);
}



NA_DEF NABool naIsBufferSourceLimited(const NABufferSource* source){
  return (source->desc.flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
}



void naReleaseBufferSource(NABufferSource* source){
  naReleaseRefCount(&(source->refcount), source->desc.data, source->desc.destructor);
}



NABuffer* naGetBufferSourceBuffer(NABufferSource* source){
  if(source && (source->desc.flags & NA_BUFFER_SOURCE_BUFFER)){
    return source->desc.data;
  }else{
    return NA_NULL;
  }
}



void naFillBufferSourcePart(NABufferSource* source, NABufferPart* part){
  if(source && source->desc.filler){
    source->desc.filler(source->desc.data, naGetBufferPartBaseDataPointer(part), naGetBufferPartRange(part));
  }
}



NARangei naGetBufferSourceLimit(NABufferSource* source){
  return source->desc.limit;
}









// //////////////////////////////////////
// Buffer
// //////////////////////////////////////


#define NA_BUFFER_RANGE_FIXED 0x01


NA_HIDEF void naEnsureBufferRange(NABuffer* buffer, NARangei range);



// This is a special buffer creation method which is not visible to the
// programmer.
NA_HDEF NABuffer* naCreateBufferMemorySource(){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  buffer->source = naCreateBufferSource(desc);

  buffer->srcoffset = 0;
  buffer->flags = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBuffer(){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  desc.data = naCreateBufferMemorySource();
  desc.destructor = (NAMutator)naReleaseBuffer;
  buffer->source = naCreateBufferSourceBuffer(desc);

  buffer->srcoffset = 0;
  buffer->flags = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferExtraction(NABuffer* srcbuffer, NARangei range){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  desc.data = naRetainBuffer(srcbuffer);
  desc.destructor = (NAMutator)naReleaseBuffer;
  buffer->source = naCreateBufferSourceBuffer(desc);

  buffer->srcoffset = -range.origin;
  buffer->flags = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);
  
  if(range.length){
    naEnsureBufferRange(buffer, naMakeRangei(0, range.length));
  }
  buffer->endianness = srcbuffer->endianness;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferFile(const char* filename){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);\
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  desc.data = naCreateFileReadingFilename(filename);
  desc.destructor = (NAMutator)naReleaseFile;
  desc.filler = naReadFileBufferPart;
  desc.limit = naMakeRangei(0, (NAInt)naComputeFileBytesize(desc.data));
  desc.flags = NA_BUFFER_SOURCE_RANGE_LIMITED;  
  buffer->source = naCreateBufferSource(desc);

  buffer->srcoffset = 0;
  buffer->flags = 0;

  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;

  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);
  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferConstData(const void* data, NAInt bytesize){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  
  buffer->source = NA_NULL;

  buffer->srcoffset = 0;
  buffer->flags = 0;

  naInitList(&(buffer->parts));
  NABufferPart* part = naNewBufferPartConstData(data, bytesize);
  
  // Add the const data to the list.
  naAddListLastMutable(&(buffer->parts), part);
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  naIterateList(&(buffer->iter), 1);
  buffer->curoffset = 0;
  buffer->curbit = 0;
  
  buffer->range = naMakeRangeiWithStartAndEnd(0, bytesize);
  buffer->flags |= NA_BUFFER_RANGE_FIXED;
  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferMutableData(void* data, NAInt bytesize, NAMemoryCleanup cleanup){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  
  buffer->source = NA_NULL;
  buffer->srcoffset = 0;
  buffer->flags = 0;

  naInitList(&(buffer->parts));
  NABufferPart* part = naNewBufferPartMutableData(data, bytesize, cleanup);
  
  // Add the const data to the list.
  naAddListLastMutable(&(buffer->parts), part);
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  naIterateList(&(buffer->iter), 1);
  buffer->curoffset = 0;
  buffer->curbit = 0;
  
  buffer->range = naMakeRangeiWithStartAndEnd(0, bytesize);
  buffer->flags |= NA_BUFFER_RANGE_FIXED;
  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_HDEF void naDestructBuffer(NABuffer* buffer){
  if(buffer->source){naReleaseBufferSource(buffer->source);}
  naForeachListMutable(&(buffer->parts), naDelete);
  naClearListIterator(&(buffer->iter));
  naClearList(&(buffer->parts));
}



NA_API NABuffer* naRetainBuffer(NABuffer* buffer){
  return (NABuffer*)naRetainRefCount(&(buffer->refcount));
}



NA_API void naReleaseBuffer(NABuffer* buffer){
  naReleaseRefCount(&(buffer->refcount), NA_NULL, (NAMutator)naDestructBuffer);
}




NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
  return (((start + (start < 0)) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - (start < 0)) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
// Note that (start < 0) either results in 0 or 1.
}



NA_HIDEF NAInt naGetBufferPartNormedEnd(NAInt end){
  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



// Moves the buffer iterator to that position which contains offset. Will
// point to the initial position if offset is not found.
NA_HIDEF void naLocateBufferPartOffset(NAListIterator* iter, NAInt offset){
  #ifndef NDEBUG
    if(naIsListAtInitial(iter))
      naError("naLocateBufferPartOffset", "iterator should not be at initial position");
  #endif
  const NABufferPart* part = naGetListCurrentConst(iter);
  const NABufferPart* newpart;
  while(part){
    if(naGetBufferPartEnd(part) <= offset){
      naIterateList(iter, 1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
        if(newpart && (naGetBufferPartStart(newpart) != naGetBufferPartEnd(part)))
          naError("naLocateBufferPartOffset", "next part not adjacent");
      #endif
    }else if(naGetBufferPartStart(part) > offset){
      naIterateList(iter, -1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
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
NA_HIDEF void naEnsureBufferRange(NABuffer* buffer, NARangei range){
  NABufferPart* part;
  
  #ifndef NDEBUG
    if(range.length <= 0)
      naError("naEnsureBufferRangeMemory", "Range length shall be >= 0");
    if((buffer->flags & NA_BUFFER_RANGE_FIXED) && (range.origin < buffer->range.origin))
      naError("naEnsureBufferRangeMemory", "Range of buffer is fixed at minimum %d, trying to access range down to %d", buffer->range.origin, range.origin);
    if((buffer->flags & NA_BUFFER_RANGE_FIXED) && (naGetRangeiEnd(range) > naGetRangeiEnd(buffer->range)))
      naError("naEnsureBufferRangeMemory", "Range of buffer is fixed at maximum %d, trying to access range up to %d", naGetRangeiEnd(buffer->range), naGetRangeiEnd(range));
  #endif
  
  if(naIsRangeiEmpty(buffer->range)){
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    part = naNewBufferPartSparse(range);
    naAddListLastMutable(&(buffer->parts), part);
    buffer->range = range;
    // Make sure the iterator points to the part which containts the first
    // offset of range.
    naLocateListLast(&(buffer->iter));
    
  }else{
    // When the buffer is not empty, we need to make sure we add all desired
    // ranges but still can find the part containing range origin, even if
    // no sparse part needed to be added.
    NABool originpartlocated = NA_FALSE;
    
    // First, we test if we need to add a sparse part at the beginning.
    if(range.origin < buffer->range.origin){
      // We create a sparse part at the beginning.
      part = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(range.origin, buffer->range.origin));
      naAddListFirstMutable(&(buffer->parts), part);
      buffer->range = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(buffer->range));
      // Make sure the iterator points to the part which containts the first
      // offset of range.
      naLocateListFirst(&(buffer->iter));
      originpartlocated = NA_TRUE;
    }
    
    // Then, we test if we need to add a sparse part at the end.
    if(naGetRangeiEnd(range) > naGetRangeiEnd(buffer->range)){
      // We create a sparse part at the end.
      part = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(buffer->range), naGetRangeiEnd(range)));
      naAddListLastMutable(&(buffer->parts), part);
      buffer->range = naMakeRangeiWithStartAndEnd(buffer->range.origin, naGetRangeiEnd(range));
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











NA_HIDEF NABool naIsBufferVolatile(NABuffer* buffer){
  NA_UNUSED(buffer);
  return NA_FALSE;
}





// This function operates on two iterators, one being the source, the other
// the destination. The destination wants to reference a specified range within
// the source parts.
// At the end of this function, the iterators point to the parts at the end of
// the desired range.
NA_HIDEF void naFillBufferIterator(NABuffer* srcbuffer, NAListIterator* dstiter, NARangei dstrange, NAInt srcoffset){
  NAInt srcrangeorigin = dstrange.origin - srcoffset;

  if(naIsBufferVolatile(srcbuffer)){
    #ifndef NDEBUG
      naError("naFillBufferIterator", "volatile buffers not implemented yet");
    #endif
  }

  // We perform the preparation as long as there are still bytes left in range.
  while(dstrange.length){

    #ifndef NDEBUG
      if(dstrange.length < 0)
        naError("naFillBufferIterator", "length is negative");
    #endif

    // We get the current parts of src and dst.
    NABufferPart* srcpart = naGetListCurrentMutable(&(srcbuffer->iter));
    NABufferPart* dstpart = naGetListCurrentMutable(dstiter);
    // We count, how many bytes are left on both parts.
    NAInt remainingsrcbytes = (naGetBufferPartEnd(srcpart) - srcrangeorigin);
    NAInt remainingdstbytes = (naGetBufferPartEnd(dstpart) - dstrange.origin);
    // we choose the number of bytes which is smaller.
    NAInt remainingbytes = naMini(remainingsrcbytes, remainingdstbytes);
    remainingbytes = naMini(remainingbytes, dstrange.length);

    // We decide what to do based on the dst buffer being sparse or not.
    if(naIsBufferPartSparse(dstpart)){
      // If the dst buffer is sparse, we split the part fitting the desired
      // number of bytes...
      naSplitBufferSparsePart(dstiter, naMakeRangei(dstrange.origin, remainingbytes));
      
      // ... and replace the current part with a reference to the source part.
      dstpart = naGetListCurrentMutable(dstiter);
      naReferenceBufferPart(dstpart, srcpart, srcoffset);
      
      // Then we try to combine this newly available part with its neighbors.
      naCombineBufferPartAdjacents(dstiter);

      // Now, the current dst part is completely filled. Move to the next one.
      naIterateList(dstiter, 1);
      
    }else{
      // The current buffer part already contains memory.

      // These bytes can simply be skipped, except if the source buffer is
      // volatile
      if(naIsBufferVolatile(srcbuffer)){
        // if the source buffer is volatile, we need to replace the existing
        // bytes.
        #ifndef NDEBUG
          naError("naFillBufferIterator", "volatile buffers not implemented yet");
        #endif
      }
      
      if(remainingbytes == remainingdstbytes){naIterateList(dstiter, 1);}
    }

    if(remainingbytes == remainingsrcbytes){naIterateList(&(srcbuffer->iter), 1);}

    dstrange = naMakeRangeiE(dstrange.origin + remainingbytes, dstrange.length - remainingbytes);
    srcrangeorigin += remainingbytes;

  }
}


NA_HIDEF void naPrepareBuffer(NABuffer* buffer, NARangei range);


// This function prepares the given buffer such that in contains all parts
// storing memory for the given range. It is guaranteed, that at the end of
// this function, the iterator of the buffer points at that part containing
// the origin of the given range.
NA_HDEF void naCacheBuffer(NABuffer* buffer, NARangei range){
  NABufferPart* part;
  NABuffer* srcbuffer;
  
  srcbuffer = naGetBufferSourceBuffer(buffer->source);
  
  if(srcbuffer){
    // In case there is a parent buffer as a source, we prepare that source
    // buffer and just take references out of it.

    // First, we ensure, that there are buffer parts in the parts list of this
    // buffer, spanning the desired range by creating sparse buffers wherever
    // necessary.
    naEnsureBufferRange(buffer, range);
    // After this function call, the iterator points to the part containing
    // the origin of range.

    // Then, we make sure that parent buffer is fully prepared.
    NARangei srcrange = naMakeRangei(range.origin - buffer->srcoffset, range.length);
    naPrepareBuffer(srcbuffer, srcrange);

    // Finally, we simply reference all parts of the source.
    naFillBufferIterator(srcbuffer, &(buffer->iter), range, buffer->srcoffset);
    
  }else{
  
    // In case there is no parent buffer, we have to fill up our own memory.
    // This can of course only happen, when we have a source such as a memory
    // source or a file.
      
    NAInt normedstart;
    NAInt normedend;
    
    // This buffer manages its own linear memory. Such memory is usually
    // aligned by NA_INTERNAL_BUFFER_PART_BYTESIZE.
    
    normedstart = naGetBufferPartNormedStart(range.origin);
    if(buffer->flags & NA_BUFFER_RANGE_FIXED){normedstart = naMaxi(normedstart, buffer->range.origin);}
    normedend = naGetBufferPartNormedEnd(naGetRangeiEnd(range));
    if(buffer->flags & NA_BUFFER_RANGE_FIXED){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
    range = naMakeRangeiWithStartAndEnd(normedstart, normedend);

    // First, we ensure, that there are buffer parts in the parts list
    // spanning the desired normed range by creating sparse buffers if
    // necessary.
    naEnsureBufferRange(buffer, range);
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
          naError("naPrepareBuffer", "list overflow. This should not happen.");
      #endif

      // When the current part already contains bytes, we can simply overjump
      // all bytes contained in this part. If this part is sparse though ...
      if(naIsBufferPartSparse(part)){
        // ... we create buffer parts with actual memory.
        NARangei subrange;

        // We create a suitable range within the parts range which tries to
        // be aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE but has a certain
        // margin to be bigger and hence reduce the number of small parts.
        normedstart = naGetBufferPartNormedStart(range.origin);
        if((normedstart - naGetBufferPartStart(part)) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedstart = naGetBufferPartStart(part);}
        normedend = naGetBufferPartNormedEnd(range.origin + NA_INTERNAL_BUFFER_PART_BYTESIZE);
        if((naGetBufferPartEnd(part) - normedend) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedend = naGetBufferPartEnd(part);}

        // Note that the previous computation of normedstart and normedend also
        // automatically handles parts being smaller than the desired range.
        subrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);
        #ifndef NDEBUG
          if(!naContainsRangeiRange(naGetBufferPartRange(part), subrange))
            naError("naPrepareBuffer", "part range does not contain subrange");
        #endif

        // Then we split the sparse buffer if necessary.
        naSplitBufferSparsePart(&(buffer->iter), subrange);
        
        // Now, iter points at a sparse part exactly of range subrange
        // which can be filled.
        part = naGetListCurrentMutable(&(buffer->iter));
        #ifndef NDEBUG
          if(!naEqualRangei(subrange, naGetBufferPartRange(part)))
            naError("naPrepareBuffer", "range lenghts do not match");
        #endif
        naAllocateBufferPartMemory(part);
        
        naFillBufferSourcePart(buffer->source, part);
      }

      // Reaching here, the current part is filled with memory. We advance
      // to the next part. Note that the calculation of the new range will
      // most probably result in a negative length when reaching the end of
      // the desired range, hence breaking the loop condition.

      range = naMakeRangeiWithStartAndEnd(naGetBufferPartEnd(part), naGetRangeiEnd(range));
      naIterateList(&(buffer->iter), 1);

    }
 
  }
  
}




// This function prepares the given buffer such that in contains all parts
// storing memory for the given range. It is guaranteed, that at the end of
// this function, the iterator of the buffer points at that part containing
// the origin of the given range.
NA_HIDEF void naPrepareBuffer(NABuffer* buffer, NARangei range){

  // We store the desired start position for later.
  NAInt originalrangeorigin = range.origin;

  naCacheBuffer(buffer, range);
  
  // Reaching here, iter points to the part one after the part containing the
  // maximum offset of range. We locate the buffer iterator at the first
  // desired offset. Note that we can not use a cached NAListPos as this might
  // have changed during this preparation.
  if(naIsListAtInitial(&(buffer->iter))){naIterateList(&(buffer->iter), -1);}
  naLocateBufferPartOffset(&(buffer->iter), originalrangeorigin);

  #ifndef NDEBUG
    NABufferPart* part;
    part = naGetListCurrentMutable(&(buffer->iter));
    if(!naContainsBufferPartOffset(part, originalrangeorigin))
      naError("naPrepareBuffer", "start offset not in range of current part.");
  #endif
  
  // Reaching here, the buffer iterator points at the first part containing
  // the range origin.
}




NA_DEF void naDeallocBuffer(NABuffer* buffer){
  naClearListIterator(&(buffer->iter));
  naClearList(&(buffer->parts));
  naFree(buffer);
}




NA_API NARangei naExpandBufferRangeToSource(NABuffer* buffer){
  #ifndef NDEBUG
    if(naHasBufferFixedRange(buffer))
      naError("naExpandBufferRangeToSource", "buffer range is fixed.");
  #endif
  if(naIsBufferSourceLimited(buffer->source)){
    NARangei sourcerange = naGetBufferSourceLimit(buffer->source);
    naEnsureBufferRange(buffer, sourcerange);
    return sourcerange;
  }else{
    return buffer->range;
  }
}



NA_DEF void naFixBufferRange(NABuffer* buffer){
  buffer->flags |= NA_BUFFER_RANGE_FIXED;
}



NA_DEF NABool naHasBufferFixedRange(const NABuffer* buffer){
  return (buffer->flags & NA_BUFFER_RANGE_FIXED);
}



NA_DEF NARangei naGetBufferRange(const NABuffer* buffer){
  return buffer->range;
}



NA_DEF NABool naIsBufferEmpty(const NABuffer* buffer){
  return naIsRangeiEmpty(buffer->range);
}




NA_DEF NAInt naTellBuffer(const NABuffer* buffer){
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naTellBuffer", "Buffer bitcount is not null.");
  #endif
  return buffer->curoffset;
}



NA_DEF NABool naIsBufferAtEnd(const NABuffer* buffer){
  #ifndef NDEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("naIsBufferAtEnd", "Buffer has no fixed max position. Use naExpandBufferRangeToSource and naFixBufferRange.");
  #endif
  return (buffer->curoffset > naGetRangeiMax(buffer->range));
}



NA_DEF void naSetBufferEndianness(NABuffer* buffer, NAInt endianness){
  buffer->endianness = endianness;
  buffer->converter = naMakeEndiannessConverter(endianness, NA_ENDIANNESS_NATIVE);
}



NA_DEF NAInt naGetBufferEndianness(NABuffer* buffer){
  return buffer->endianness;
}



NA_DEF void naSeekBufferAbsolute(NABuffer* buffer, NAInt offset){
  buffer->curoffset = offset;
  if(!naIsBufferEmpty(buffer)){
    if(naIsListAtInitial(&(buffer->iter))){naLocateListLast(&(buffer->iter));}
    naLocateBufferPartOffset(&(buffer->iter), offset);
  }
}



NA_DEF void naSeekBufferRelative(NABuffer* buffer, NAInt offset){
  NAInt absoffset = buffer->curoffset + offset;
  naSeekBufferAbsolute(buffer, absoffset);
}



NA_DEF void naSeekBufferSource(NABuffer* buffer, NAInt offset){
  NAInt absoffset = buffer->srcoffset + offset;
  naSeekBufferAbsolute(buffer, absoffset);
}



NA_DEF const NAByte* naGetBufferByte(const NABuffer* buffer, NAInt indx){
  const NAByte* retbyte = NA_NULL;
  if(naIsBufferEmpty(buffer)){return NA_NULL;}
  
  NAListIterator iter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListPosition(&iter, naGetListCurrentPosition(&(buffer->iter)));
  if(naIsListAtInitial(&iter)){naIterateList(&iter, 1);}
  naLocateBufferPartOffset(&iter, indx);

  if(!naIsListAtInitial(&iter)){
    retbyte = naGetBufferPartDataPointer(naGetListCurrentConst(&iter), indx);
  }
  naClearListIterator(&iter);
  return retbyte;
}



NA_DEF NAInt naSearchBufferByteOffset(const NABuffer* buffer, NAByte byte, NAInt startoffset, NABool forward){
  NAInt retindex = NA_INVALID_MEMORY_INDEX;
  if(naIsBufferEmpty(buffer)){return NA_INVALID_MEMORY_INDEX;}

  NAListIterator iter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListPosition(&iter, naGetListCurrentPosition(&(buffer->iter)));
  if(naIsListAtInitial(&iter)){naIterateList(&iter, 1);}
  naLocateBufferPartOffset(&iter, startoffset);

  while(!naIsListAtInitial(&iter)){
    const NABufferPart* part = naGetListCurrentConst(&iter);
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naSearchBufferByteOffset", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointer(part, startoffset); 
    if(forward){
      NAInt endoffset = naGetBufferPartEnd(part);
      while(startoffset < endoffset){
        if(*curbyte == byte){
          retindex = startoffset;
          break;
        }
        curbyte++;
        startoffset++;
      }
    }else{
      NAInt beginoffset = naGetBufferPartStart(part) - 1;
      while(startoffset > beginoffset){
        if(*curbyte == byte){
          retindex = startoffset;
          break;
        }
        curbyte--;
        startoffset--;
      }
    }
    if(retindex != NA_INVALID_MEMORY_INDEX){break;}
  }

  naClearListIterator(&iter);
  return retindex;
}



NA_DEF void naWriteBufferBytes(NABuffer* buffer, const void* data, NAInt bytesize){
  const NAByte* src = data;
  NAByte* dst;

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, bytesize));
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  
  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts.
  while(bytesize){
    // The part pointed to by the iterator should be the one containing offset.
    NABufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    
    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naPutBufferBytes", "Current part is sparse");
      if(!naContainsBufferPartOffset(part, buffer->curoffset))
        naError("naPutBufferBytes", "Current part does not contain current offset");
    #endif
    
    // We get the data pointer where we can write bytes.
    dst = naGetBufferPartDataPointer(part, buffer->curoffset);
    // We detect, how many bytes actually can be put into the current part.
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("naPutBufferBytes", "possible length invalid");
      if(possiblelength > naGetBufferPartRange(part).length)
        naError("naPutBufferBytes", "buffer overflow expected");
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



NA_DEF void naWriteBufferi8(NABuffer* buffer, int8 value){
  buffer->converter.convert8(&value);
  naWriteBufferBytes(buffer, &value, 1);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi16(NABuffer* buffer, int16 value){
  buffer->converter.convert16(&value);
  naWriteBufferBytes(buffer, &value, 2);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi32(NABuffer* buffer, int32 value){
  buffer->converter.convert32(&value);
  naWriteBufferBytes(buffer, &value, 4);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi64(NABuffer* buffer, int64 value){
  buffer->converter.convert64(&value);
  naWriteBufferBytes(buffer, &value, 8);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferu8(NABuffer* buffer, uint8 value){
  buffer->converter.convert8(&value);
  naWriteBufferBytes(buffer, &value, 1);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu16(NABuffer* buffer, uint16 value){
  buffer->converter.convert16(&value);
  naWriteBufferBytes(buffer, &value, 2);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu32(NABuffer* buffer, uint32 value){
  buffer->converter.convert32(&value);
  naWriteBufferBytes(buffer, &value, 4);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu64(NABuffer* buffer, uint64 value){
  buffer->converter.convert64(&value);
  naWriteBufferBytes(buffer, &value, 8);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferf(NABuffer* buffer, float value){
  buffer->converter.convert32(&value);
  naWriteBufferBytes(buffer, &value, 4);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferd(NABuffer* buffer, double value){
  buffer->converter.convert64(&value);
  naWriteBufferBytes(buffer, &value, 8);
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) == NA_BUFFER_FLAG_AUTOFLUSH_ALL){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferi8v(NABuffer* buffer, const int8* src, NAInt count){
  int8 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 1));
  while(count){
    value = *src;
    buffer->converter.convert8(&value);
    naWriteBufferBytes(buffer, &value, 1);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi16v(NABuffer* buffer, const int16* src, NAInt count){
  int16 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 2));
  while(count){
    value = *src;
    buffer->converter.convert16(&value);
    naWriteBufferBytes(buffer, &value, 2);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi32v(NABuffer* buffer, const int32* src, NAInt count){
  int32 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4));
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naWriteBufferBytes(buffer, &value, 4);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferi64v(NABuffer* buffer, const int64* src, NAInt count){
  int64 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8));
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naWriteBufferBytes(buffer, &value, 8);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu8v(NABuffer* buffer, const uint8* src, NAInt count){
  uint8 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 1));
  while(count){
    value = *src;
    buffer->converter.convert8(&value);
    naWriteBufferBytes(buffer, &value, 1);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu16v(NABuffer* buffer, const uint16* src, NAInt count){
  uint16 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 2));
  while(count){
    value = *src;
    buffer->converter.convert16(&value);
    naWriteBufferBytes(buffer, &value, 2);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu32v(NABuffer* buffer, const uint32* src, NAInt count){
  uint32 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4));
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naWriteBufferBytes(buffer, &value, 4);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferu64v(NABuffer* buffer, const uint64* src, NAInt count){
  uint64 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8));
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naWriteBufferBytes(buffer, &value, 8);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferfv(NABuffer* buffer, const float* src, NAInt count){
  float value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4));
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naWriteBufferBytes(buffer, &value, 4);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}
NA_DEF void naWriteBufferdv(NABuffer* buffer, const double* src, NAInt count){
  double value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8));
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naWriteBufferBytes(buffer, &value, 8);
    src++;
    count--;
  }
//  if((buffer->flags & NA_BUFFER_FLAG_AUTOFLUSH_MASK) >= NA_BUFFER_FLAG_AUTOFLUSH_MULTIBYTE){naFlushBuffer(buffer);}
}



NA_DEF void naWriteBufferBuffer(NABuffer* dstbuffer, NABuffer* srcbuffer, NARangei srcrange){
  NABufferSource* tmpsource = dstbuffer->source;
  NAInt tmpsrcoffset = dstbuffer->srcoffset;
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  desc.data = naRetainBuffer(srcbuffer);
  desc.destructor = (NAMutator)naReleaseBuffer;
  dstbuffer->source = naCreateBufferSourceBuffer(desc);
  
  dstbuffer->srcoffset = dstbuffer->curoffset - srcrange.origin;
  
  naCacheBuffer(dstbuffer, naMakeRangei(dstbuffer->curoffset, srcrange.length));
  dstbuffer->curoffset += srcrange.length;
  
  naReleaseBufferSource(dstbuffer->source);
  
  dstbuffer->source = tmpsource;
  dstbuffer->srcoffset = tmpsrcoffset;
  
}



NA_DEF void naRepeatBufferBytes(NABuffer* buffer, NAInt distance, NAInt bytesize){
  NAInt writeoffset;
  NAInt readoffset;
  NABufferPart* writepart;
  const NABufferPart* readpart;
  NAInt remainingwrite;
  NAInt remainingread;
  
  // Prepare the write part
  writeoffset = buffer->curoffset;
  naPrepareBuffer(buffer, naMakeRangei(writeoffset, bytesize));

  // Prepare the read part.
  readoffset = buffer->curoffset - distance;
  naPrepareBuffer(buffer, naMakeRangei(readoffset, bytesize));

  // Create the read iterator
  // Important: Do this after the prepare calls as otherwise there might be
  // an iterator on a part which needs to be removed from a list.
  NAListIterator readiter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListPosition(&readiter, naGetListCurrentPosition(&(buffer->iter)));

  // Reposition the buffer iterator to the write part
  naLocateBufferPartOffset(&(buffer->iter), buffer->curoffset);

  readpart = naGetListCurrentConst(&readiter);
  writepart = naGetListCurrentMutable(&(buffer->iter));
  
  // Now start copying the buffers.
  while(1){
    remainingread = naGetBufferPartEnd(readpart) - readoffset;
    remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;

    remainingread = naMini(remainingread, distance);
    NAInt remaining = (remainingwrite < remainingread) ? remainingwrite : remainingread;
    remaining = naMini(remaining, bytesize);
    
    naCopyn(naGetBufferPartDataPointer(writepart, writeoffset), naGetBufferPartDataPointer(readpart, readoffset), (NAUInt)remaining);
    bytesize -= remaining;
    writeoffset += remaining;
    readoffset += remaining;
    buffer->curoffset += remaining;

    remainingread = naGetBufferPartEnd(readpart) - readoffset;
    remainingwrite = naGetBufferPartEnd(writepart) - writeoffset;
    
    if(remainingread == NA_ZERO){
      naIterateList(&readiter, 1);
      readpart = naGetListCurrentConst(&readiter);
    }
    if(remainingwrite == NA_ZERO){
      naIterateList(&buffer->iter, 1);
      writepart = naGetListCurrentMutable(&buffer->iter);
    }
    if(!bytesize){break;}
  }

  naClearListIterator(&readiter);

}




// ///////////////////////////////////////
// READING
// ///////////////////////////////////////


NA_API void naReadBufferBytes(NABuffer* buffer, void* data, NAInt bytesize){
  NAByte* dst = data;
  NAByte* src;

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, bytesize));
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  
  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts.
  while(bytesize){
    // The part pointed to by the iterator should be the one containing offset.
    NABufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    
    // Reaching this point, we are sure, the current part contains offset and
    // is filled with memory.
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naPutBufferBytes", "Current part is sparse");
      if(!naContainsBufferPartOffset(part, buffer->curoffset))
        naError("naPutBufferBytes", "Current part does not contain current offset");
    #endif
    
    // We get the data pointer where we can read bytes.
    src = naGetBufferPartDataPointer(part, buffer->curoffset);
    // We detect, how many bytes actually can be read from the current part.
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;

    #ifndef NDEBUG
      if(possiblelength <= 0)
        naError("naPutBufferBytes", "possible length invalid");
      if(possiblelength > naGetBufferPartRange(part).length)
        naError("naPutBufferBytes", "buffer overflow expected");
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



NA_DEF int8 naReadBufferi8(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int8 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi8", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 1);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF int16 naReadBufferi16(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int16 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi16", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 2);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF int32 naReadBufferi32(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int32 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi32", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 4);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF int64 naReadBufferi64(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  int64 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferi64", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 8);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF uint8 naReadBufferu8(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint8 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu8", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 1);
  buffer->converter.convert8(&value);
  return value;
}
NA_DEF uint16 naReadBufferu16(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint16 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu16", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 2);
  buffer->converter.convert16(&value);
  return value;
}
NA_DEF uint32 naReadBufferu32(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint32 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu32", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 4);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF uint64 naReadBufferu64(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  uint64 value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferu64", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 8);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF float naReadBufferf(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  float value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferf", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 4);
  buffer->converter.convert32(&value);
  return value;
}
NA_DEF double naReadBufferd(NABuffer* buffer){
  // Declaration before implementation. Needed for C90.
  double value;
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naReadBufferd", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, &value, 8);
  buffer->converter.convert64(&value);
  return value;
}



NA_DEF void naReadBufferi8v(NABuffer* buffer, int8* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi8v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi8v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 1);
  buffer->converter.convert8v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi16v(NABuffer* buffer, int16* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi16v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi16v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 2);
  buffer->converter.convert16v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi32v(NABuffer* buffer, int32* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi32v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi32v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 4);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferi64v(NABuffer* buffer, int64* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferi64v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferi64v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 8);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



NA_DEF void naReadBufferu8v(NABuffer* buffer, uint8* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu8v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu8v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 1);
  buffer->converter.convert8v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu16v(NABuffer* buffer, uint16* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu16v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu16v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 2);
  buffer->converter.convert16v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu32v(NABuffer* buffer, uint32* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu32v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu32v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 4);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferu64v(NABuffer* buffer, uint64* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferu64v", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferu64v", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 8);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



NA_DEF void naReadBufferfv(NABuffer* buffer, float* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      naError("naReadBufferfv", "dst is Null pointer.");
    if(buffer->curbit != 0)
      naError("naReadBufferfv", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 4);
  buffer->converter.convert32v(dst, (NAUInt)count);
}
NA_DEF void naReadBufferdv(NABuffer* buffer, double* dst, NAInt count){
  #ifndef NDEBUG
    if(!dst)
      {naCrash("naReadBufferdv", "dst is Null pointer."); return;}
    if(buffer->curbit != 0)
      naError("naReadBufferdv", "Bit offset not 0.");
  #endif
  naReadBufferBytes(buffer, dst, count * 8);
  buffer->converter.convert64v(dst, (NAUInt)count);
}



NA_DEF NABool naReadBufferBit(NABuffer* buffer){
  NAByte* src;
  NABool bit;

  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, 1));
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  NABufferPart* part = naGetListCurrentMutable(&(buffer->iter));
  src = naGetBufferPartDataPointer(part, buffer->curoffset);
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



NA_DEF NAUInt naReadBufferBits(NABuffer* buffer, uint8 count){
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



NA_DEF void naPadBufferBits(NABuffer* buffer){
  if(buffer->curbit != 0){
    buffer->curbit = 0;
    buffer->curoffset++;
    NABufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    NAInt possiblelength = naGetBufferPartEnd(part) - buffer->curoffset;
    if(!possiblelength){
      naIterateList(&(buffer->iter), 1);
    }
  }
}




NA_DEF void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum){
  NAInt bytesize;
  NAInt curoffset;
  NAListIterator iter;

//  #ifndef NDEBUG
//    if(!naHasBufferFixedRange(buffer))
//      naError("naAccumulateBufferToChecksum", "Buffer has no determined range. Use naExpandBufferRangeToSource and naFixBufferRange");
//  #endif

  bytesize = buffer->range.length;
  if(!bytesize){return;}

  curoffset = buffer->range.origin;
  iter = naMakeListIteratorMutator(&(buffer->parts));
  naLocateListFirst(&iter);
  
  while(bytesize){
    NABufferPart* curpart;
    NAInt remainingbytes;
    NAByte* src;

    curpart = naGetListCurrentMutable(&iter);
    remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
    src = naGetBufferPartDataPointer(curpart, curoffset);
    
    #ifndef NDEBUG
      if(naIsBufferPartSparse(curpart))
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



NA_DEF void naWriteBufferToFile(NABuffer* buffer, NAFile* file){
  NAInt bytesize;
  NAInt curoffset;
  NAListIterator iter;

  #ifndef NDEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("naWriteBufferToFile", "Buffer has no determined range. Use naExpandBufferRangeToSource and naFixBufferRange");
  #endif

  bytesize = buffer->range.length;
  if(!bytesize){return;}

  curoffset = buffer->range.origin;
  iter = naMakeListIteratorMutator(&(buffer->parts));
  naLocateListFirst(&iter);
  
  while(bytesize){
    NABufferPart* curpart;
    NAInt remainingbytes;
    NAByte* src;

    curpart = naGetListCurrentMutable(&iter);
    remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
    src = naGetBufferPartDataPointer(curpart, curoffset);
    
    #ifndef NDEBUG
      if(naIsBufferPartSparse(curpart))
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



NA_DEF void naWriteBufferToData(NABuffer* buffer, void* data){
  NAInt bytesize;
  NAInt curoffset;
  NAListIterator iter;
  NAByte* dst = data;

//  #ifndef NDEBUG
//    if(!naHasBufferFixedRange(buffer))
//      naError("naWriteBufferToFile", "Buffer has no determined range. Use naExpandBufferRangeToSource and naFixBufferRange");
//  #endif

  bytesize = buffer->range.length;
  if(!bytesize){return;}

  curoffset = buffer->range.origin;
  iter = naMakeListIteratorMutator(&(buffer->parts));
  naLocateListFirst(&iter);
  
  while(bytesize){
    NABufferPart* curpart;
    NAInt remainingbytes;
    NAByte* src;

    curpart = naGetListCurrentMutable(&iter);
    remainingbytes = naGetBufferPartEnd(curpart) - curoffset;
    src = naGetBufferPartDataPointer(curpart, curoffset);
    
    #ifndef NDEBUG
      if(naIsBufferPartSparse(curpart))
        naError("naWriteBufferToFile", "Buffer contains sparse parts.");
    #endif
    
    if(bytesize > remainingbytes){
      naCopyn(dst, src, (NAUInt)remainingbytes);
      naIterateList(&iter, 1);
      curoffset += remainingbytes;
      bytesize -= remainingbytes;
      dst += remainingbytes;
    }else{
      naCopyn(dst, src, (NAUInt)bytesize);
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
