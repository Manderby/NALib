
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.




#include "NALinearMemory.h"
#include "NAList.h"
#include "NACoord.h"
#include "NAFile.h"


#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE (naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE NA_BUFFER_PART_BYTESIZE
#endif


// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////


typedef struct NAASDFBufferPart NAASDFBufferPart;
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



//// Creates a memory block with given const data buffer
//NA_HIDEF NAASDFBufferPart* naNewASDFBufferPartConstData(const NAByte* data, NAInt bytesize){
//  NAASDFBufferPart* part = naNew(NAASDFBufferPart);
//  part->data = naNewPointerConst(data);
//  part->origin = 0;
//  part->range = naMakeRangei(0, bytesize);
//  return part;
//}
//
//
//
//// Creates a memory block with given mutable data buffer
//NA_HIDEF NAASDFBufferPart* naNewASDFBufferPartMutableData(NAByte* data, NAInt bytesize, NAMemoryCleanup cleanup){
//  NAASDFBufferPart* part = naNew(NAASDFBufferPart);
//  part->data = naNewPointerMutable(data, cleanup, NA_NULL);
//  part->origin = 0;
//  part->range = naMakeRangei(0, bytesize);
//  return part;
//}



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
  naReleasePointer(part->data);
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


NA_HIDEF void naEnlargeBufferPart(NAASDFBufferPart* part, NAInt bytesize){
  part->range = naMakeRangeiWithStartAndEnd(part->range.origin, naGetRangeiEnd(part->range) + bytesize);
  #ifndef NDEBUG
    if(part->range.length > 4096)
      naError("naEnlargeBufferPart", "That's a bit much");
  #endif
}



NA_HIDEF NABool naContainsBufferPartOffset(const NAASDFBufferPart* part, NAInt offset){
  return naContainsRangeiOffset(part->range, offset);
}



NA_HIDEF NABool naAreASDFBufferPartsEqualAndAdjacent(NAASDFBufferPart* startpart, NAASDFBufferPart* endpart){
  #ifndef NDEBUG
    if(naIsASDFBufferPartSparse(startpart) && naIsASDFBufferPartSparse(endpart))
      naError("naAreASDFBufferPartsEqualAndAdjacent", "Both parts are sparse");
  #endif
  return (startpart->data == endpart->data) && (naGetRangeiEnd(startpart->range) == endpart->range.origin);
}



NA_HIDEF void naCombineBufferPartAdjacents(NAListIterator* iter){
  NAASDFBufferPart* part = naGetListCurrentMutable(iter);
  NAASDFBufferPart* prevpart = naGetListPrevMutable(iter);
  NAASDFBufferPart* nextpart = naGetListNextMutable(iter);
  if(prevpart && naAreASDFBufferPartsEqualAndAdjacent(prevpart, part)){
    // we can combine the two parts.
    part->range = naMakeRangeiWithRangeAndRange(naGetBufferPartRange(prevpart), naGetBufferPartRange(part));
    // then, we remove the current part and delete it.
    naRemoveListPrevMutable(iter);
    naDelete(prevpart);
  }
  if(nextpart && naAreASDFBufferPartsEqualAndAdjacent(part, nextpart)){
    // we can combine the two parts.
    part->range = naMakeRangeiWithRangeAndRange(naGetBufferPartRange(part), naGetBufferPartRange(nextpart));
    // then, we remove the next part and delete it.
    naRemoveListNextMutable(iter);
    naDelete(nextpart);
  }
}



NA_HIDEF NABool naSplitASDFBufferSparsePart(NAListIterator* iter, NARangei range){
  NAASDFBufferPart* part = naGetListCurrentMutable(iter);

  #ifndef NDEBUG
    if(!naIsASDFBufferPartSparse(part))
      naError("naSplitASDFBufferSparsePart", "current part is not sparse");
    if(!naContainsRangeiRange(naGetBufferPartRange(part), range))
      naError("naSplitASDFBufferSparsePart", "part range does not contain given range");
  #endif

  if(naEqualRangei(naGetBufferPartRange(part), range)){
    // This sparse part fits the desired range completely.
    // Nothing to do.
    
  }else if(naGetBufferPartStart(part) == range.origin){
    // A new part will be put at the start of this sparse part.
    part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part));
    part = naNewASDFBufferPartSparse(range);
    naAddListBeforeMutable(iter, part);
    naIterateList(iter, -1);
    
  }else if(naGetBufferPartEnd(part) == naGetRangeiEnd(range)){
    // A new part will be put at the end of this sparse part.
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
    part = naNewASDFBufferPartSparse(range);
    naAddListAfterMutable(iter, part);
    naIterateList(iter, 1);
    
  }else{
    // A filled part will be put in the middle of this sparse part.
    NAASDFBufferPart* lastpart = naNewASDFBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetBufferPartEnd(part)));
    naAddListAfterMutable(iter, lastpart);
    part->range = naMakeRangeiWithStartAndEnd(naGetBufferPartStart(part), range.origin);
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
// Buffer Source
// //////////////////////////////////////


typedef enum{
  NA_BUFFER_SOURCE_NONE,
  NA_BUFFER_SOURCE_MEMORY,
  NA_BUFFER_SOURCE_MEMORY_SOURCE,
  NA_BUFFER_SOURCE_BUFFER,
  NA_BUFFER_SOURCE_FILE,
  NA_BUFFER_SOURCE_CONST_DATA,
  NA_BUFFER_SOURCE_MUTABLE_DATA,
} NAASDFBufferType;


#define NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED 0x01
#define NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED 0x02


struct NAASDFBuffer{
  NARefCount refcount;
  
  void* src;                // A pointer to a source if available.
  NAASDFBufferType srctype; // The type of the source
  NAInt srcoffset;       // The offset of the source relative to this range.
  
  NAList parts;             // List of all parts in this buffer
  NAListIterator iter;      // Iterator pointing at the current part.
  NAInt curoffset;          // The current absolute offset
  
  NAUInt flags;
  NARangei bufrange;
  void (*fillPart)(NAASDFBuffer*, NAASDFBufferPart*);
};



NA_HIAPI void naFillASDFBufferPartMemory(NAASDFBuffer* source, NAASDFBufferPart* part);
NA_HIAPI void naFillASDFBufferPartFile(NAASDFBuffer* source, NAASDFBufferPart* part);
NA_HIAPI void naFillASDFBufferPartConstData(NAASDFBuffer* source, NAASDFBufferPart* part);
NA_HIAPI void naFillASDFBufferPartMutableData(NAASDFBuffer* source, NAASDFBufferPart* part);



NA_DEF NAASDFBuffer* naCreateASDFBufferMemorySource(){
  NAASDFBuffer* source = naAlloc(NAASDFBuffer);
  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  source->src = NA_NULL;
  source->srctype = NA_BUFFER_SOURCE_MEMORY;
  source->srcoffset = 0;

  naInitList(&(source->parts));
  source->iter = naMakeListIteratorModifier(&(source->parts));
  source->curoffset = 0;

  source->flags = 0;
  source->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  source->fillPart = naFillASDFBufferPartMemory;
  return source;
}



NA_DEF NAASDFBuffer* naCreateASDFBuffer(){
  NAASDFBuffer* source = naAlloc(NAASDFBuffer);
  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  source->src = naCreateASDFBufferMemorySource();
  source->srctype = NA_BUFFER_SOURCE_MEMORY_SOURCE;
  source->srcoffset = 0;

  naInitList(&(source->parts));
  source->iter = naMakeListIteratorModifier(&(source->parts));
  source->curoffset = 0;

  source->flags = 0;
  source->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  source->fillPart = naFillASDFBufferPartMemory;
  return source;
}



NA_DEF NAASDFBuffer* naCreateASDFBufferFile(const char* filename){
  NAASDFBuffer* source = naAlloc(NAASDFBuffer);
  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
  source->src = naAlloc(NAFile);
  *((NAFile*)(source->src)) = naMakeFileReadingFilename(filename);
  source->srctype = NA_BUFFER_SOURCE_FILE;
  source->srcoffset = 0;

  naInitList(&(source->parts));
  source->iter = naMakeListIteratorModifier(&(source->parts));
  source->curoffset = 0;

  source->flags = NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED;
  source->bufrange = naMakeRangeiWithStartAndEnd(0, 0);
  source->fillPart = naFillASDFBufferPartFile;
  return source;
}



//NA_DEF NAASDFBuffer* naCreateASDFBufferConstData(const void* data, NAInt bytesize){
//  NAASDFBuffer* source = naAlloc(NAASDFBuffer);
//  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
//  source->src = NA_NULL;
//  source->srctype = NA_BUFFER_SOURCE_CONST_DATA;
//  source->srcoffset = 0;
//
//  naInitList(&(source->parts));
//  NAASDFBufferPart* part = naNewASDFBufferPartConstData(data, bytesize);
//  
//  // Add the const data to the list.
//  naAddListLastMutable(&(source->parts), part);
//  source->iter = naMakeListIteratorModifier(&(source->parts));
//  naIterateList(&(source->iter), 1);
//  source->curoffset = 0;
//  
//  source->flags = NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED | NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED;
//  source->bufrange = naMakeRangeiWithStartAndEnd(0, bytesize);
//  source->fillPart = naFillASDFBufferPartConstData;
//  return source;
//}
//
//
//
//NA_DEF NAASDFBuffer* naCreateASDFBufferMutableData(void* data, NAInt bytesize, NAMemoryCleanup cleanup){
//  NAASDFBuffer* source = naAlloc(NAASDFBuffer);
//  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);
//  source->src = NA_NULL;
//  source->srctype = NA_BUFFER_SOURCE_MUTABLE_DATA;
//  source->srcoffset = 0;
//
//  naInitList(&(source->parts));
//  NAASDFBufferPart* part = naNewASDFBufferPartMutableData(data, bytesize, cleanup);
//  
//  // Add the const data to the list.
//  naAddListLastMutable(&(source->parts), part);
//  source->iter = naMakeListIteratorModifier(&(source->parts));
//  naIterateList(&(source->iter), 1);
//  source->curoffset = 0;
//  
//  source->flags = NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED | NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED;
//  source->bufrange = naMakeRangeiWithStartAndEnd(0, bytesize);
//  source->fillPart = naFillASDFBufferPartMutableData;
//  return source;
//}



NA_HDEF void naDestructASDFBuffer(NAASDFBuffer* source){
  switch(source->srctype){
  case NA_BUFFER_SOURCE_MEMORY:
    break;
  case NA_BUFFER_SOURCE_MEMORY_SOURCE:
    break;
  case NA_BUFFER_SOURCE_BUFFER:
    naReleaseASDFBuffer(source->src);
    break;
  case NA_BUFFER_SOURCE_FILE:
    naCloseFile(source->src);
    naFree(source->src);
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
  naForeachListMutable(&(source->parts), naDelete);
  naClearListIterator(&(source->iter));
  naClearList(&(source->parts));
}



NA_API NAASDFBuffer* naRetainASDFBuffer(NAASDFBuffer* source){
  return (NAASDFBuffer*)naRetainRefCount(&(source->refcount));
}



NA_API void naReleaseASDFBuffer(NAASDFBuffer* source){
  naReleaseRefCount(&(source->refcount), NA_NULL, (NAMutator)naDestructASDFBuffer);
}




NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
  return ((start / (NAInt)NA_INTERNAL_BUFFER_PART_BYTESIZE) - (start < 0)) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
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



// Moves the source iterator to that position which contains offset. Will
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
        if(!newpart)
          {naCrash("naLocateBufferPartOffset", "next part inexistent"); return;}
        if(naGetBufferPartStart(newpart) != naGetBufferPartEnd(part))
          naError("naLocateBufferPartOffset", "next part not adjacent");
      #endif
    }else if(naGetBufferPartStart(part) > offset){
      naIterateList(iter, -1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
        if(!newpart)
          {naCrash("naLocateBufferPartOffset", "prev part inexistent"); return;}
        if(naGetBufferPartEnd(newpart) != naGetBufferPartStart(part))
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




NA_HIDEF void naEnsureASDFBufferRange(NAASDFBuffer* buffer, NARangei range){
  NAASDFBufferPart* part;
  #ifndef NDEBUG
    if(range.length <= 0)
      naError("naEnsureASDFBufferRangeMemory", "Range length shall be >= 0");
    if((buffer->flags & NA_BUFFER_SOURCE_FLAG_MINPOS_FIXED) && (range.origin < buffer->bufrange.origin))
      naError("naEnsureASDFBufferRangeMemory", "Range of buffer is fixed at minimum %d, trying to access range down to %d", buffer->bufrange.origin, range.origin);
    if((buffer->flags & NA_BUFFER_SOURCE_FLAG_MAXPOS_FIXED) && (naGetRangeiEnd(range) < naGetRangeiEnd(buffer->bufrange)))
      naError("naEnsureASDFBufferRangeMemory", "Range of buffer is fixed at maximum %d, trying to access range up to %d", naGetRangeiEnd(buffer->bufrange), naGetRangeiEnd(range));
  #endif
  if(naIsRangeiEmpty(buffer->bufrange)){
    part = naNewASDFBufferPartSparse(range);
    naAddListLastMutable(&(buffer->parts), part);
    buffer->bufrange = range;
    // Make sure the iterator points to the part which containts the first
    // offset of range.
    naLocateListLast(&(buffer->iter));
  }else{
    NABool startoffsetfound = NA_FALSE;
    
    if(range.origin < buffer->bufrange.origin){
      // We create all necessary parts at the beginning.
      part = naNewASDFBufferPartSparse(naMakeRangeiWithStartAndEnd(range.origin, buffer->bufrange.origin));
      naAddListFirstMutable(&(buffer->parts), part);
      buffer->bufrange = naMakeRangeiWithRangeAndRange(buffer->bufrange, range);
      // Make sure the iterator points to the part which containts the first
      // offset of range.
      naLocateListFirst(&(buffer->iter));
      startoffsetfound = NA_TRUE;
    }
    
    if(naGetRangeiEnd(range) > naGetRangeiEnd(buffer->bufrange)){
      // We create all necessary parts at the end.
      part = naNewASDFBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(buffer->bufrange), naGetRangeiEnd(range)));
      naAddListLastMutable(&(buffer->parts), part);
      buffer->bufrange = naMakeRangeiWithRangeAndRange(buffer->bufrange, range);
    }
    
    if(!startoffsetfound){
      // We make sure, the buffer iterator points to a buffer part.
      // We choose to start with the last part as usually, parts are getting
      // added at the end.
      if(naIsListAtInitial(&(buffer->iter))){naLocateListLast(&(buffer->iter));}
      
      // We locate the buffer iterator for the beginning position.
      naLocateBufferPartOffset(&(buffer->iter), range.origin);
    }
  }

}




NA_HIDEF void naFillASDFBufferPartMemory(NAASDFBuffer* source, NAASDFBufferPart* part){
  naNulln(naGetASDFBufferPartBaseDataPointer(part), naGetBufferPartRangeLength(part));
}



NA_HIDEF void naFillASDFBufferPartFile(NAASDFBuffer* source, NAASDFBufferPart* part){
  naReadFileBytes(source->src, naGetASDFBufferPartBaseDataPointer(part), naGetBufferPartRangeLength(part));
}



NA_HIDEF void naFillASDFBufferPartConstData(NAASDFBuffer* source, NAASDFBufferPart* part){
}
NA_HIDEF void naFillASDFBufferPartMutableData(NAASDFBuffer* source, NAASDFBufferPart* part){
}





NA_HIDEF NABool naIsASDFBufferVolatile(NAASDFBuffer* buffer){
  return NA_FALSE;
}



NA_HIDEF NABool naHasASDFBufferParentBuffer(NAASDFBuffer* buffer){
  return (buffer->src) && (buffer->srctype != NA_BUFFER_SOURCE_NONE) && (buffer->srctype != NA_BUFFER_SOURCE_FILE);
}




NA_HIDEF void naFillASDFBufferIterator(NAASDFBuffer* sourcebuf, NAListIterator* dstiter, NARangei dstrange, NAInt srcoffset){
  NAInt srcrangeorigin = dstrange.origin - srcoffset;

  if(naIsASDFBufferVolatile(sourcebuf)){
    #ifndef NDEBUG
      naError("naFillASDFBufferIterator", "volatile buffers not implemented yet");
    #endif
  }

  // We perform the preparation as long as there are still bytes left in range.
  while(dstrange.length){

    // We get the current parts of src and buf.
    NAASDFBufferPart* srcpart = naGetListCurrentMutable(&(sourcebuf->iter));
    NAASDFBufferPart* dstpart = naGetListCurrentMutable(dstiter);
    NAInt remainingsrcbytes = (naGetBufferPartEnd(srcpart) - srcrangeorigin);
    NAInt remainingdstbytes = (naGetBufferPartEnd(dstpart) - dstrange.origin);
    
    NAInt remainingbytes = naMini(remainingsrcbytes, remainingdstbytes);

    if(naIsASDFBufferPartSparse(dstpart)){
      // We split the sparse buffer ...
      naSplitASDFBufferSparsePart(dstiter, naMakeRangei(dstrange.origin, remainingbytes));
      
      // ... And replace the current one with a reference to the source part.
      dstpart = naGetListCurrentMutable(dstiter);
      naReferenceASDFBufferPart(dstpart, srcpart, srcoffset);
      
      // Then we try to combine this newly available part with its neighbors.
      naCombineBufferPartAdjacents(dstiter);

      // Now, the current part is completely filled. Move to the next one.
      naIterateList(dstiter, 1);
      
    }else{
      // The current buffer part can just be extended to contain all remaining
      // bytes of the source part.
      naEnlargeBufferPart(dstpart, remainingbytes);

      naIterateList(&(sourcebuf->iter), 1);
      naIterateList(dstiter, 1);
    }

    if(remainingbytes == remainingsrcbytes){naIterateList(&(sourcebuf->iter), 1);}

    dstrange = naMakeRangeiE(dstrange.origin + remainingbytes, dstrange.length - remainingbytes);
    srcrangeorigin += remainingbytes;

  }
}



NA_HIDEF void naPrepareASDFBuffer2(NAASDFBuffer* buffer, NARangei range){
  NAASDFBufferPart* part;
  
  // We store the desired start position for later.
  NAInt originalbufoffset = range.origin;

  if(naHasASDFBufferParentBuffer(buffer)){

    // If there is a parent source buffer, we make sure that parent buffer is
    // fully prepared.
    NARangei srcrange = naMakeRangei(range.origin - buffer->srcoffset, range.length);
    naPrepareASDFBuffer2(buffer->src, srcrange);

    // We ensure, that there are source buffer parts in the source list
    // spanning the desired src range by creating sparse buffers if necessary.
    naEnsureASDFBufferRange(buffer, range);
    // After this function call, the iterator points to the part containing
    // the first offset of range.

    naFillASDFBufferIterator(buffer->src, &(buffer->iter), range, buffer->srcoffset);
    
  }else{
    // This buffer manages its own linear memory. We go through the list and
    // change all sparse buffers into memory when within range.

    NAInt normedstart = naGetBufferPartNormedStart(range.origin);
    NAInt normedend = naGetBufferPartNormedEnd(naGetRangeiEnd(range));
    NARangei normedrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);

    // We ensure, that there are source buffer parts in the source list
    // spanning the desired src range by creating sparse buffers if necessary.
    naEnsureASDFBufferRange(buffer, normedrange);
    // After this function call, the iterator points to the part containing
    // the first offset of range.

    // We perform the preparation as long as there are bytes left in range.
    while(range.length > 0){
      
      part = naGetListCurrentMutable(&(buffer->iter));
      normedstart = naGetBufferPartNormedStart(range.origin);
      normedend = naGetBufferPartNormedEnd(naGetRangeiEnd(range));

      #ifndef NDEBUG
        // This should not happen, as range.lenght shall be smallerequal 0 when
        // the list comes to an end.
        if(!part)
          naError("naPrepareASDFBuffer2", "list overflow. This should not happen.");
      #endif

      if(naIsASDFBufferPartSparse(part)){
        // If the current part is sparse, we create buffer parts with actual
        // memory.

        if(normedstart < naGetBufferPartStart(part)){normedstart = naGetBufferPartStart(part);}
        if(normedend > naGetBufferPartEnd(part)){normedend = naGetBufferPartEnd(part);}
        if((normedend - normedstart) >= (2 * NA_INTERNAL_BUFFER_PART_BYTESIZE)){
          normedend = naGetBufferPartNormedStart(normedstart + NA_INTERNAL_BUFFER_PART_BYTESIZE);
        }
        normedrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);

        // Then we split the sparse buffer.
        naSplitASDFBufferSparsePart(&(buffer->iter), normedrange);
        
        // Now, iter points at a sparse part exactly of range normedrange
        // which can be filled.
        part = naGetListCurrentMutable(&(buffer->iter));
        #ifndef NDEBUG
          if(normedrange.length != naGetBufferPartRangeLength(part))
            naError("naPrepareASDFBuffer2", "range lenghts do not match");
        #endif
        naAllocateBufferPartMemory(part);
        buffer->fillPart(buffer, part);

      }else{
        // This part is already available as memory. We progress till the end
        // of this part.
      }

      // Reaching here, the current part is filled with memory. We advance
      // to the next part. Note that the calculation of the new range will
      // most probably result in a negative length when reaching the end of
      // the desired range.

      range = naMakeRangeiWithStartAndEnd(naGetBufferPartEnd(part), naGetRangeiEnd(range));
      naIterateList(&(buffer->iter), 1);

    }
 
  }
  
  // Reaching here, iter points to the part one after the part containing the
  // maximum offset of range.
    
  // Then we locate the buffer iterator at the first desired offset. Note that
  // we can not use a cached NAListPos as this might have changed during this
  // preparation.
  if(naIsListAtInitial(&(buffer->iter))){naIterateList(&(buffer->iter), -1);}
  naLocateBufferPartOffset(&(buffer->iter), originalbufoffset);

  #ifndef NDEBUG
    part = naGetListCurrentMutable(&(buffer->iter));
    if(!naContainsBufferPartOffset(part, originalbufoffset))
      naError("naPrepareASDFBuffer2", "start offset not in range of current part.");
  #endif
  
}




NA_DEF void naDeallocASDFBuffer(NAASDFBuffer* buffer){
  naClearListIterator(&(buffer->iter));
  naClearList(&(buffer->parts));
  naFree(buffer);
}



NA_DEF NABool naIsASDFBufferEmpty(const NAASDFBuffer* buffer){
  return naIsListEmpty(&(buffer->parts));
}



NA_DEF void naSeekASDFBufferAbsolute(NAASDFBuffer* buffer, NAInt offset){
  buffer->curoffset = offset;
  #ifndef NDEBUG
    if(naIsASDFBufferEmpty(buffer))
      naError("naSeekASDFBufferAbsolute", "buffer is empty.");
  #endif
  if(naIsListAtInitial(&(buffer->iter))){naIterateList(&(buffer->iter), 1);}
  naLocateBufferPartOffset(&(buffer->iter), offset);
}



NA_DEF void naSeekASDFBufferRelative(NAASDFBuffer* buffer, NAInt offset){
  NAInt absoffset = buffer->curoffset + offset;
  naSeekASDFBufferAbsolute(buffer, absoffset);
}



NA_DEF void naSeekASDFASDFBuffer(NAASDFBuffer* buffer, NAInt offset){
  NAInt absoffset = buffer->srcoffset + offset;
  naSeekASDFBufferAbsolute(buffer, absoffset);
}



NA_API void naWriteASDFBufferBytes(NAASDFBuffer* buffer, const void* data, NAInt bytesize){
  const NAByte* src = data;
  NAByte* dst;
  NABool needcheck = NA_TRUE;
  
  // do as long as there is a bytesize remaining. Remember that the data may
  // be split into different buffer parts which additionally do not need to
  // be adjacent
  while(bytesize){
    // The part pointed to by the iterator should be the one containing offset.
    NAASDFBufferPart* part = naGetListCurrentMutable(&(buffer->iter));
    if(needcheck && (!part || !naContainsBufferPartOffset(part, buffer->curoffset))){
      // If that is not the case or if there is no part, there is no part in
      // the buffer list containing offset. Hence, we prepare the buffer.
      naPrepareASDFBuffer2(buffer, naMakeRangei(buffer->curoffset, bytesize));
      // after this function, the iterator should point to the buffer part
      // containing offset. We reload it.
      part = naGetListCurrentMutable(&(buffer->iter));
      // After having prepared the buffer, we are sure, all successing buffer
      // parts will be in perfect order. We do not need to check anymore.
      needcheck = NA_FALSE;
    }
    
    // Reaching this point, we are sure, the current part contains offset.
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
      // We copy all remaining bytes
      naCopyn(dst, src, (NAUInt)bytesize);
      buffer->curoffset += bytesize;
      // src += bytesize; // this line is not needed as the loop will end.
      bytesize = 0;
    }else{
      // We copy as many bytes as possible
      naCopyn(dst, src, (NAUInt)possiblelength);
      buffer->curoffset += possiblelength;
      src += possiblelength;
      bytesize -= possiblelength;
      // When the possible length is smallerequal bytesize, we assume that the
      // next buffer part will contain the next curoffset. Hence, we iterate
      // forward.
      naIterateList(&(buffer->iter), 1);
    }
  }
  
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
