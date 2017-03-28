
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "../NABuffer.h"
#include "NAList.h"





#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif





// //////////////////////////////////////
// Buffer Part
// //////////////////////////////////////

// Here are some author descriptions on how the NABufferPart struct shall be
// interpreted:
//
// The reader might have an intuition on how parts are combined together to
// form a linear sequence of memory. Further more, this gives the possibility
// to share parts of different sources. But there is one non-intuitive fact
// which must be understood:
// 
// Each buffer part only exists for one single NABuffer. Buffer parts are not
// shared amongst NABuffers, but the data which a buffer part stores is. By
// referencing the data with a reference counter, one and the same memory can
// be accessed from different buffers but using the buffer part struct, these
// memory parts can additionally be accessed with different offsets.
//
// Buffer parts come in basically two forms:
// 1. The content of a buffer part is precisely known. In that case, an already
//    existing const or mutable buffer is packed up into a buffer part (which
//    can be very large) and that buffer part will never change its range.
//    Other parts can reference this part.
// 2. The content of a buffer is yet unknown and/or shall be filled with bytes
//    defined by a source. In that case, a buffer part always starts as a so
//    called sparse part not containing any memory. Such a sparse buffer part
//    defines the range of where memory shall be available. In a later stage,
//    that memory will be allocated by splitting existing sparse parts into
//    small parts (with size NA_INTERNAL_BUFFER_PART_BYTESIZE) and then being
//    filled according to the source or being referenced.
//
//    Filling:     The function naCacheBuffer will prepare memory for the
//                 desired part and call the filler method of the source
//                 defined by NABufferSourceDescriptor.
//    Referencing: The part will not allocate its own memory but instead
//                 reference a sub-part of another part.

typedef struct NABufferPart NABufferPart;
struct NABufferPart{
  NAPointer* data;        // A reference counting pointer with the data.
  NAInt origin;           // The origin of the first byte in the data pointer
  NARangei range;         // The available range in this buffer part.
};

NA_HIAPI void naDestructBufferPart(NABufferPart* part);
NA_RUNTIME_TYPE(NABufferPart, naDestructBufferPart);




// Creates a memory block with sparse memory.
//
// A sparse buffer is always initialized with the origin being the same as the
// range.origin. This part will later on be changed with naReferenceBufferPart
// or naFillBufferSourcePart.
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
// already set in part.
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



// Allocates memory for the range defined in part.
NA_HIDEF void naAllocateBufferPartMemory(NABufferPart* part){
  #ifndef NDEBUG
    if(part->data)
      naError("naAllocateBufferPartMemory", "Part already has memory");
  #endif
  part->data = naNewPointerMutable(naMalloc(part->range.length), NA_MEMORY_CLEANUP_NA_FREE, NA_NULL);
}



// The destructor method which will automatically be called by naDelete.
NA_HIDEF void naDestructBufferPart(NABufferPart* part){
  if(part->data){naReleasePointer(part->data);}
}



// Returns NA_TRUE if the given part does not store memory.
NA_HIDEF NABool naIsBufferPartSparse(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naIsBufferPartSparse", "part is Null pointer"); return NA_FALSE;}
  #endif
  return (part->data == NA_NULL);
}



// Returns the range of the given part.
NA_HIDEF NARangei naGetBufferPartRange(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartRange", "part is Null pointer"); return naMakeRangei(0, 0);}
  #endif
  return part->range;
}



// Returns the start of the given part range.
NA_HIDEF NAInt naGetBufferPartStart(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartStart", "part is Null pointer"); return 0;}
  #endif
  return part->range.origin;
}



// Returns the end of the given part range.
NA_HIDEF NAInt naGetBufferPartEnd(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartEnd", "part is Null pointer"); return 0;}
  #endif
  return naGetRangeiEnd(part->range);
}



NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
  return (((start + (start < 0)) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - (start < 0)) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
// Note that (start < 0) either results in 0 or 1.
}



NA_HIDEF NAInt naGetBufferPartNormedEnd(NAInt end){
  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}




NA_HIDEF NABool naContainsBufferPartOffset(const NABufferPart* part, NAInt offset){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naContainsBufferPartOffset", "part is Null pointer"); return NA_FALSE;}
  #endif
  return naContainsRangeiOffset(part->range, offset);
}



// This function returns NA_TRUE if both parts have the same data pointer
// and the ranges are adjacent.
NA_HIDEF NABool naAreBufferPartsEqualAndAdjacent(NABufferPart* startpart, NABufferPart* endpart){
  if((startpart->data == endpart->data) && (naGetRangeiEnd(startpart->range) == endpart->range.origin)){
    #ifndef NDEBUG
      if(startpart->origin != endpart->origin)
        naError("naAreBufferPartsEqualAndAdjacent", "Internal consistency error: Origin of the two parts do not match");
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
NA_HIDEF NAByte* naGetBufferPartDataPointerConst(const NABufferPart* part, NAInt offset){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartDataPointerConst", "buffer part is Null pointer"); return NA_NULL;}
    if(naIsBufferPartSparse(part))
      naError("naGetBufferPartDataPointerConst", "buffer part is sparse");
    if(!naContainsRangeiOffset(part->range, offset))
      naError("naGetBufferPartDataPointerConst", "offset not inside buffer part");
    if(offset - part->origin < 0)
      naError("naGetBufferPartDataPointerConst", "offset calculation wrong");
  #endif
  return &(((NAByte*)naGetPointerConst(part->data))[offset - part->origin]);
}



// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HIDEF NAByte* naGetBufferPartDataPointerMutable(const NABufferPart* part, NAInt offset){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartDataPointerMutable", "buffer part is Null pointer"); return NA_NULL;}
    if(naIsBufferPartSparse(part))
      naError("naGetBufferPartDataPointerMutable", "buffer part is sparse");
    if(!naContainsRangeiOffset(part->range, offset))
      naError("naGetBufferPartDataPointerMutable", "offset not inside buffer part");
    if(offset - part->origin < 0)
      naError("naGetBufferPartDataPointerMutable", "offset calculation wrong");
  #endif
  return &(((NAByte*)naGetPointerMutable(part->data))[offset - part->origin]);
}



// Returns a direct pointer to the raw data of this buffer part, given its
// absolute address.
NA_HIDEF NAByte* naGetBufferPartBaseDataPointer(const NABufferPart* part){
  #ifndef NDEBUG
    if(!part)
      {naCrash("naGetBufferPartBaseDataPointer", "buffer part is Null pointer"); return NA_NULL;}
    if(naIsBufferPartSparse(part))
      naError("naGetBufferPartBaseDataPointer", "buffer part is sparse");
  #endif
  return (NAByte*)naGetPointerMutable(part->data);
}







// //////////////////////////////////////
// Buffer Source
// //////////////////////////////////////



#define NA_BUFFER_SOURCE_BUFFER           0x02


typedef struct NABufferSource NABufferSource;
struct NABufferSource{
  NARefCount refcount;
  NABufferSourceDescriptor desc;  
};



// Creates a buffer source with the given descriptor.
NA_HIDEF NABufferSource* naCreateBufferSource(NABufferSourceDescriptor descriptor){
  NABufferSource* source = naAlloc(NABufferSource);
  naInitRefCount(&(source->refcount), NA_MEMORY_CLEANUP_FREE, NA_MEMORY_CLEANUP_NONE);

  source->desc = descriptor;  
  
  return source;
}




// Creates a buffer source with the given buffer. Note that the buffer must
// already be retained if it shall reference an already existing buffer!
NA_HIDEF NABufferSource* naCreateBufferSourceBuffer(NABuffer* buffer){
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  desc.data = buffer;
  desc.destructor = (NAMutator)naReleaseBuffer;
  NABufferSource* source = naCreateBufferSource(desc);
  source->desc.flags |= NA_BUFFER_SOURCE_BUFFER;
  return source;
}



// Retains this buffer source.
NA_HIDEF NABufferSource* naRetainBufferSource(NABufferSource* source){
  return (NABufferSource*)naRetainRefCount(&(source->refcount));
}



// Releases this buffer source.
NA_HIDEF void naReleaseBufferSource(NABufferSource* source){
  naReleaseRefCount(&(source->refcount), source->desc.data, source->desc.destructor);
}



#ifndef NDEBUG
  // Returns NA_TRUE if the range is a valid limiting range.
  NA_HIDEF NABool naIsBufferSourceLimited(const NABufferSource* source){
    return (source->desc.flags & NA_BUFFER_SOURCE_RANGE_LIMITED);
  }
#endif


// Returns the limit range of this source
NA_HIDEF NARangei naGetBufferSourceLimit(NABufferSource* source){
  #ifndef NDEBUG
    if(!naIsBufferSourceLimited(source))
      naError("naGetBufferSourceLimit", "source is not limited");
  #endif
  return source->desc.limit;
}



// Returns the source data if there is a source and the it is defined to be
// a buffer source. returns NA_NULL otherwise.
NA_HIDEF NABuffer* naGetBufferSourceBuffer(NABufferSource* source){
  if(source && (source->desc.flags & NA_BUFFER_SOURCE_BUFFER)){
    return source->desc.data;
  }else{
    return NA_NULL;
  }
}



// Calls the filler of the source descriptor for the range of the given part.
// Hence filling the memory with the desired content.
void naFillBufferSourcePart(NABufferSource* source, NABufferPart* part){
  #ifndef NDEBUG
    if(naIsBufferSourceLimited(source)){
      if(!naContainsRangeiRange(naGetBufferSourceLimit(source), naGetBufferPartRange(part)))
        naError("naFillBufferSourcePart", "part overflows the limited range of the source");
    }
  #endif
  if(source && source->desc.filler){
    source->desc.filler(source->desc.data, naGetBufferPartBaseDataPointer(part), naGetBufferPartRange(part));
  }
}







// This is the filler method of the secure memory input source descriptor
NA_HIDEF void naFillBufferPartSecureMemory(void* data, void* dst, NARangei range){
  NA_UNUSED(data);
  naNulln(dst, (NAUInt)range.length);
}



// This is the filler method of the file input source descriptor
NA_HIDEF void naFillBufferPartFile(void* data, void* dst, NARangei range){
  naReadFileBytes(data, dst, range.length);
}






// ///////////////////////////
// NABuffer helper functions
// ///////////////////////////


#define NA_BUFFER_RANGE_FIXED       0x01
#define NA_BUFFER_VOLATILE_SOURCE   0x02


struct NABuffer{
  NARefCount refcount;
  
  NABufferSource* source;
  NAInt srcoffset;            // Offset the source relative to owners range.
  
  NAUInt flags;
  
  NAInt endianness;                 // The current endianness
  NAEndiannessConverter converter;  // The endianness converter.

  NAList parts;             // List of all parts in this buffer
  NAListIterator iter;      // Iterator pointing at the current part.
  NAInt curoffset;          // The current absolute offset
  uint8 curbit;             // The current bit number
  
  NARangei range;
};



NA_HAPI void naLocateBufferPartOffset(NAListIterator* iter, NAInt offset);
NA_HAPI void naEnsureBufferRange(NABuffer* buffer, NARangei range);
NA_HAPI void naPrepareBuffer(NABuffer* buffer, NARangei range, NABool forcevolatile, NABool locatestart);
NA_HAPI void naUnlinkBufferRange(NABuffer* buffer, NARangei range);



// Moves the given iterator to that position which contains offset. Will
// point to the initial position if offset is not found.
NA_HDEF void naLocateBufferPartOffset(NAListIterator* iter, NAInt offset){
  const NABufferPart* part;
  const NABufferPart* newpart;

  // We make sure, the iterator points to a buffer part. We choose to start
  // with the last part by default, as usually, things are happening at the
  // end.
  if(naIsListAtInitial(iter)){naLocateListLast(iter);}
  part = naGetListCurrentConst(iter);

  while(part){
    if(naGetBufferPartEnd(part) <= offset){
      // If the current part is below the desired offset...
      naIterateList(iter, 1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
        if(newpart && (naGetBufferPartStart(newpart) != naGetBufferPartEnd(part)))
          naError("naLocateBufferPartOffset", "next part not adjacent");
      #endif
    }else if(naGetBufferPartStart(part) > offset){
      // if the current part is above the desired offset
      naIterateList(iter, -1);
      newpart = naGetListCurrentConst(iter);
      #ifndef NDEBUG
        if(newpart && (naGetBufferPartEnd(newpart) != naGetBufferPartStart(part)))
          naError("naLocateBufferPartOffset", "prev part not adjacent");
      #endif
    }else{
      // This part should be the one containing offset.
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
//
// At return of this function, the iterator of the buffer points at the part
// containing the first offset of range.
NA_HDEF void naEnsureBufferRange(NABuffer* buffer, NARangei range){
  NABufferPart* part;
  
  #ifndef NDEBUG
    if(range.length <= 0)
      naError("naEnsureBufferRangeMemory", "Range length shall be >= 0");
    if((buffer->flags & NA_BUFFER_RANGE_FIXED) && (range.origin < buffer->range.origin))
      naError("naEnsureBufferRangeMemory", "Range of buffer is fixed at minimum %d, trying to access range down to %d", buffer->range.origin, range.origin);
    if((buffer->flags & NA_BUFFER_RANGE_FIXED) && (naGetRangeiEnd(range) > naGetRangeiEnd(buffer->range)))
      naError("naEnsureBufferRangeMemory", "Range of buffer is fixed at maximum %d, trying to access range up to %d", naGetRangeiEnd(buffer->range), naGetRangeiEnd(range));
  #endif
  
  if(naIsBufferEmpty(buffer)){
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
      part = naGetListFirstMutable(&(buffer->parts));
      if(naIsBufferPartSparse(part)){
        // If the first part of this list is already sparse, we simply extend
        // its range.
        part->range = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(part->range));
      }else{
        // We create a sparse part at the beginning.
        part = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(range.origin, buffer->range.origin));
        naAddListFirstMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(buffer->range));
      // Make sure the iterator points to the part which containts the first
      // offset of range.
      naLocateListFirst(&(buffer->iter));
      originpartlocated = NA_TRUE;
    }
    
    // Then, we test if we need to add a sparse part at the end.
    if(naGetRangeiEnd(range) > naGetRangeiEnd(buffer->range)){
      part = naGetListLastMutable(&(buffer->parts));
      if(naIsBufferPartSparse(part)){
        // If the last part of this list is already sparse, we simply extend
        // its range.
        part->range = naMakeRangeiWithStartAndEnd(part->range.origin, naGetRangeiEnd(range));
      }else{
        // We create a sparse part at the end.
        part = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(buffer->range), naGetRangeiEnd(range)));
        naAddListLastMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(buffer->range.origin, naGetRangeiEnd(range));
    }
    
    if(!originpartlocated){      
      // We locate the buffer iterator for the beginning position.
      naLocateBufferPartOffset(&(buffer->iter), range.origin);
    }
  }

}



// This is the core function of NABuffer.
//
// This function fills the specified range of the buffer with memory and
// content. If the volatil parameter is NA_TRUE or the buffer source is
// defined to be volatile, all bytes in that range are filled anew.
// if locatestart is set to NA_TRUE, the iterator of buffer will point to the
// part containing the start offset of range. Otherwise, it will point at the
// part containing the end of the range, possibly resulting in the initial
// position of the list.
NA_HDEF void naPrepareBuffer(NABuffer* buffer, NARangei range, NABool forcevolatile, NABool locatestart){
  NABufferPart* part;
  NABuffer* srcbuffer;
  NAInt originalrangeorigin = range.origin;
  NABool volatil = forcevolatile | naHasBufferVolatileSource(buffer);
  NARangei subrange;

  // We try to get a source buffer. The following function returns NA_NULL if
  // this buffer has not a buffer as a source.
  srcbuffer = naGetBufferSourceBuffer(buffer->source);
  
  if(srcbuffer){
    // In case there is a parent buffer as a source, we prepare that source
    // buffer furst and then just take references out of it.

    // First, we make sure that parent buffer is fully prepared.
    NARangei srcrange = naMakeRangei(range.origin - buffer->srcoffset, range.length);
    naPrepareBuffer(srcbuffer, srcrange, volatil, NA_TRUE);
    // After this function call, the iterator points to the part containing
    // the start of srcrange.

    // Then, we ensure, that there are buffer parts in the parts list of this
    // buffer, spanning the desired range by creating sparse buffers wherever
    // necessary.
    naEnsureBufferRange(buffer, range);
    // After this function call, the iterator points to the part containing
    // the start of range.

    // Then, we start referencing all parts of the source.
    NAInt srcrangeorigin = range.origin - buffer->srcoffset;

    // We perform the preparation as long as there are still bytes left in range.
    while(range.length){

      #ifndef NDEBUG
        if(range.length < 0)
          naError("naFillBufferIterator", "length is negative");
      #endif

      // We get the current parts of src and dst.
      NABufferPart* srcpart = naGetListCurrentMutable(&(srcbuffer->iter));
      NABufferPart* dstpart = naGetListCurrentMutable(&(buffer->iter));
      // We count, how many bytes are left on both parts.
      NAInt remainingsrcbytes = (naGetBufferPartEnd(srcpart) - srcrangeorigin);
      NAInt remainingdstbytes = (naGetBufferPartEnd(dstpart) - range.origin);
      // we choose the number of bytes which is smaller.
      NAInt remainingbytes = naMini(remainingsrcbytes, remainingdstbytes);
      remainingbytes = naMini(remainingbytes, range.length);

      // We decide what to do based on the dst buffer.
      if(naIsBufferPartSparse(dstpart)){
        // If the dst buffer is sparse, we split the part fitting the desired
        // number of bytes.
        naSplitBufferSparsePart(&(buffer->iter), naMakeRangei(range.origin, remainingbytes));
        // We need to reload the current dstpart, as it might have changed by
        // the call to naSplitBufferSparsePart.
        dstpart = naGetListCurrentMutable(&(buffer->iter));
        
        // We replace the current sparse part with a reference to the
        // source part.
        naReferenceBufferPart(dstpart, srcpart, buffer->srcoffset);
        
        // Then we try to combine this newly available part with its neighbors.
        naCombineBufferPartAdjacents(&(buffer->iter));

        // Now, the current dst part is completely filled. Move to the next one.
        naIterateList(&(buffer->iter), 1);
        
      }else{
        // The current buffer part already contains memory.

        // These bytes can usually be skipped, except if volatil is NA_TRUE and
        // the source buffers do not match.
        if(volatil && (naGetBufferPartDataPointerConst(dstpart, range.origin) != naGetBufferPartDataPointerConst(srcpart, srcrangeorigin))){
          // if the source buffer is volatile, we need to replace the existing
          // bytes in the desired subrange.
          subrange = naMakeRangeiWithStartAndEnd(range.origin, range.origin + remainingbytes);
          
          // First, we dismiss the remaining bytes from the dst buffer.
          naDismissBufferRange(buffer, subrange);
          // After this call, the iterator is at the new sparse part
          dstpart = naGetListCurrentMutable(&(buffer->iter));
          // This new sparse part might be bigger than anticipated. We split it
          // again into the size we need
          naSplitBufferSparsePart(&(buffer->iter), subrange);

          // We replace the current sparse part with a reference to the
          // source part.
          naReferenceBufferPart(dstpart, srcpart, buffer->srcoffset);
          
          // Then we try to combine this newly available part with its neighbors.
          naCombineBufferPartAdjacents(&(buffer->iter));

          // Now, the current dst part is completely filled. Move to the next one.
          naIterateList(&(buffer->iter), 1);
        }
        
        // We can skip ahead the dst buffer if the remaining bytes match.
        if(remainingbytes == remainingdstbytes){naIterateList(&(buffer->iter), 1);}
      }

      // If the remainingbytes matches the src bytes, the source part can be
      // iterated forward.
      if(remainingbytes == remainingsrcbytes){naIterateList(&(srcbuffer->iter), 1);}

      // We adapt the range and source origin.
      range = naMakeRangeiE(range.origin + remainingbytes, range.length - remainingbytes);
      srcrangeorigin += remainingbytes;

    }
    
  }else{
  
    // In case there is no parent buffer, we fill up our own linear memory.
    // Such memory is usually aligned by NA_INTERNAL_BUFFER_PART_BYTESIZE.
      
    NAInt normedstart;
    NAInt normedend;
        
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

    // Then, we go through the list of parts and change all sparse buffers into
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

      if(naIsBufferPartSparse(part)){
        // If the current part is sparse, we create buffer parts with actual
        // memory.

        // We create a suitable range within the parts range which tries to
        // be aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE but has a certain
        // margin to be bigger and hence reduce the number of small parts.
        normedstart = naGetBufferPartNormedStart(range.origin);
        if((normedstart - naGetBufferPartStart(part)) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedstart = naGetBufferPartStart(part);}
        normedend = naGetBufferPartNormedEnd(range.origin + NA_INTERNAL_BUFFER_PART_BYTESIZE);
        if((naGetBufferPartEnd(part) - normedend) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedend = naGetBufferPartEnd(part);}
        
        // Note that the previous computation of normedstart and normedend also
        // automatically handles parts being smaller than the normed range and
        // hence never resulting in a subrange bigger than the range of the
        // actual part.
        subrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);
        #ifndef NDEBUG
          if(!naContainsRangeiRange(naGetBufferPartRange(part), subrange))
            naError("naPrepareBuffer", "part range does not contain subrange");
        #endif

        // Then we split the sparse buffer if necessary.
        naSplitBufferSparsePart(&(buffer->iter), subrange);
        
        // Now, iter points at a sparse part containing subrange
        // which can be filled.
        part = naGetListCurrentMutable(&(buffer->iter));
        #ifndef NDEBUG
          if(!naEqualRangei(subrange, naGetBufferPartRange(part)))
            naError("naPrepareBuffer", "range lenghts do not match");
        #endif
        naAllocateBufferPartMemory(part);
        naFillBufferSourcePart(buffer->source, part);
        
      }else{
        // When the current part already contains bytes, we do not have to
        // allocate memory. But we might need to fill the memory anew if
        // volatil is NA_TRUE
        if(volatil){naFillBufferSourcePart(buffer->source, part);}
      }

      // Reaching here, the current part is filled with memory. We advance
      // to the next part. Note that the calculation of the new range will
      // most probably result in a negative length when reaching the end of
      // the desired range, hence breaking the loop condition.

      range = naMakeRangeiWithStartAndEnd(naGetBufferPartEnd(part), naGetRangeiEnd(range));
      naIterateList(&(buffer->iter), 1);
    }
 
  }
  
  if(locatestart){
    // Reaching here, iter points to the part one after the part containing the
    // maximum offset of range. We locate the buffer iterator at the first
    // desired offset. Note that we can not use a cached NAListPos as this might
    // have changed during this preparation.
    naLocateBufferPartOffset(&(buffer->iter), originalrangeorigin);

    #ifndef NDEBUG
      if(!naContainsBufferPartOffset(naGetListCurrentMutable(&(buffer->iter)), originalrangeorigin))
        naError("naPrepareBuffer", "start offset not in range of current part.");
    #endif
  }

}



// This function makes the bytes declared in range unavailable by replacing
// that range with a sparse part. As a consequence, certain buffer parts may
// not be used anymore and will be automatically deallocated.
NA_HDEF void naUnlinkBufferRange(NABuffer* buffer, NARangei range){
  NAInt rangepos;
  NABufferPart* sparsepart;
  
  #ifndef NDEBUG
    if(naIsBufferEmpty(buffer))
      naError("naDismissBufferRange", "Buffer ist empty");
    if(!naContainsRangeiRange(buffer->range, range))
      naError("naDismissBufferRange", "Range of is not within buffer range");
  #endif
  
  NAListIterator iter = naMakeListIteratorModifier(&(buffer->parts));
  naLocateBufferPartOffset(&iter, range.origin);
  
  // We create a sparse part in place where the dismissed range lies.
  // As the range will change during the alrogithm, we create it now and
  // add it later.
  sparsepart = naNewBufferPartSparse(range);

  // We go through all parts being part of the given range and split them.
  // The current position of the iterator always points at the one part
  // containing the range.offset.
  while(range.length){
    NABufferPart* part = naGetListCurrentMutable(&iter);
    NARangei partrange = naGetBufferPartRange(part);
    if(naContainsRangeiRange(range, partrange)){
      // If this parts range is contained completely in the range to dismiss,
      // we remove the whole part.
      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(partrange), naGetRangeiEnd(range));
      naDelete(naRemoveListCurrentMutable(&iter, NA_TRUE));
    }else if(range.origin == partrange.origin){
      // If the origins match, we split the part such that the remaining bytes
      // lie at the end of the current part.
      part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partrange));
      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
    }else if(naGetRangeiEnd(range) >= naGetRangeiEnd(partrange)){
      // If the desired range is equal or larger than the part range, we split
      // the current part such that the remaining bytes lie at the beginning
      // of the current part. After that, we iterate to the part after the
      // removed subpart.
      rangepos = naGetRangeiEnd(partrange);
      part->range = naMakeRangeiWithStartAndEnd(partrange.origin, range.origin);
      range = naMakeRangeiWithStartAndEnd(rangepos, naGetRangeiEnd(range));
      naIterateList(&iter, 1);
    }else{
      // Reaching here, the desired range lies completely within the current
      // part. We subdivide the current part into two pieces.
      NABufferPart* newpart = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partrange)));
      naReferenceBufferPart(newpart, part, partrange.origin);
      naAddListAfterMutable(&iter, newpart);
      part->range = naMakeRangeiWithStartAndEnd(partrange.origin, range.origin);
      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
      naIterateList(&iter, 1);
    }
  }
  // The iterator now points to the one part after the insertion position of
  // the new sparse part.

  // We add the sparse part one before the current position and move the
  // iterator at that position.
  naAddListBeforeMutable(&iter, sparsepart);
  naIterateList(&iter, -1);
  naCombineBufferPartAdjacents(&iter);

  naClearListIterator(&iter);
}







// ///////////////////////////
// NABuffer public functions
// ///////////////////////////


NA_HDEF void naInitBufferStruct(NABuffer* buffer){
  buffer->flags = 0;
  naInitList(&(buffer->parts));
  buffer->iter = naMakeListIteratorModifier(&(buffer->parts));
  buffer->curoffset = 0;
  buffer->curbit = 0;
  buffer->range = naMakeRangeiWithStartAndEnd(0, 0);
}



// This is a special buffer creation method which is not visible to the
// programmer.
NA_HDEF NABuffer* naCreateBufferMemorySource(NABool secure){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  if(secure){desc.filler = naFillBufferPartSecureMemory;}
  buffer->source = naCreateBufferSource(desc);
  buffer->srcoffset = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBuffer(NABool securememory){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  // The source is a memory buffer.
  buffer->source = naCreateBufferSourceBuffer(naCreateBufferMemorySource(securememory));
  buffer->srcoffset = 0;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferExtraction(NABuffer* srcbuffer, NARangei range){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  buffer->source = naCreateBufferSourceBuffer(naRetainBuffer(srcbuffer));
  buffer->srcoffset = -range.origin;

  // We initialize with length 0 and ensure the buffer range to the full range
  // after.
  if(range.length){
    naEnsureBufferRange(buffer, naMakeRangei(0, range.length));
  }
  buffer->flags = srcbuffer->flags | NA_BUFFER_RANGE_FIXED;
  
  buffer->endianness = srcbuffer->endianness;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferSameSource(NABuffer* srcbuffer){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  buffer->source = naRetainBufferSource(srcbuffer->source);
  buffer->srcoffset = srcbuffer->srcoffset;

  buffer->flags = srcbuffer->flags;
  
  buffer->endianness = srcbuffer->endianness;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferFile(const char* filename){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  NABufferSourceDescriptor desc;
  naNulln(&desc, sizeof(desc));
  desc.data = naCreateFileReadingFilename(filename);
  desc.destructor = (NAMutator)naReleaseFile;
  desc.filler = naFillBufferPartFile;
  desc.limit = naMakeRangei(0, (NAInt)naComputeFileBytesize(desc.data));
  desc.flags = NA_BUFFER_SOURCE_RANGE_LIMITED;  
  buffer->source = naCreateBufferSource(desc);
  buffer->srcoffset = 0;

  naEnsureBufferRange(buffer, naGetBufferSourceLimit(buffer->source));
  buffer->flags |= NA_BUFFER_RANGE_FIXED;

  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferConstData(const void* data, NAInt bytesize){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  buffer->source = NA_NULL;
  buffer->srcoffset = 0;

  // Add the const data to the list.
  NABufferPart* part = naNewBufferPartConstData(data, bytesize);
  naAddListLastMutable(&(buffer->parts), part);
  naIterateList(&(buffer->iter), 1);
  
  buffer->range = naMakeRangeiWithStartAndEnd(0, bytesize);
  buffer->flags |= NA_BUFFER_RANGE_FIXED;
  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}



NA_DEF NABuffer* naCreateBufferMutableData(void* data, NAInt bytesize, NAMemoryCleanup cleanup){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  buffer->source = NA_NULL;
  buffer->srcoffset = 0;

  // Add the const data to the list.
  NABufferPart* part = naNewBufferPartMutableData(data, bytesize, cleanup);
  naAddListLastMutable(&(buffer->parts), part);
  naIterateList(&(buffer->iter), 1);
  
  buffer->range = naMakeRangeiWithStartAndEnd(0, bytesize);
  buffer->flags |= NA_BUFFER_RANGE_FIXED;
  buffer->endianness = NA_ENDIANNESS_UNKNOWN;
  buffer->converter = naMakeEndiannessConverter(buffer->endianness, NA_ENDIANNESS_NATIVE);

  return buffer;
}


NA_DEF NABuffer* naCreateBufferWithCustomSource(NABufferSourceDescriptor desc){
  NABuffer* buffer = naAlloc(NABuffer);
  naInitRefCount(&(buffer->refcount), NA_MEMORY_CLEANUP_NA_FREE, NA_MEMORY_CLEANUP_NONE);
  naInitBufferStruct(buffer);
  
  buffer->source = naCreateBufferSource(desc);
  buffer->srcoffset = 0;

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



NA_DEF void naDeallocBuffer(NABuffer* buffer){
  naClearListIterator(&(buffer->iter));
  naClearList(&(buffer->parts));
  naFree(buffer);
}



NA_DEF NAInt naTellBuffer(const NABuffer* buffer){
  #ifndef NDEBUG
    if(buffer->curbit != 0)
      naError("naTellBuffer", "Buffer bitcount is not null.");
  #endif
  return buffer->curoffset;
}



NA_DEF void naSeekBufferAbsolute(NABuffer* buffer, NAInt offset){
  buffer->curoffset = offset;
  if(!naIsBufferEmpty(buffer)){
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



NA_DEF NARangei naGetBufferRange(const NABuffer* buffer){
  return buffer->range;
}



NA_DEF NABool naIsBufferEmpty(const NABuffer* buffer){
  return naIsRangeiEmpty(buffer->range);
}



NA_DEF NABool naIsBufferAtEnd(const NABuffer* buffer){
  #ifndef NDEBUG
    if(!naHasBufferFixedRange(buffer))
      naError("naIsBufferAtEnd", "Buffer has no fixed max position. Use naFixBufferRange.");
  #endif
  return (buffer->curoffset > naGetRangeiMax(buffer->range));
}



NA_DEF void naFixBufferRange(NABuffer* buffer){
  buffer->flags |= NA_BUFFER_RANGE_FIXED;
}



NA_DEF NABool naHasBufferFixedRange(const NABuffer* buffer){
  return (buffer->flags & NA_BUFFER_RANGE_FIXED);
}




NA_DEF NABool naHasBufferVolatileSource(const NABuffer* buffer){
  return (buffer->flags & NA_BUFFER_VOLATILE_SOURCE);
}



NA_DEF void naSetBufferVolatileSource(NABuffer* buffer, NABool volatil){
  if(volatil){
    buffer->flags |= NA_BUFFER_VOLATILE_SOURCE;
  }else{
    buffer->flags &= (NAUInt)~NA_BUFFER_VOLATILE_SOURCE;
  }
}



NA_DEF void naSetBufferEndianness(NABuffer* buffer, NAInt endianness){
  buffer->endianness = endianness;
  buffer->converter = naMakeEndiannessConverter(endianness, NA_ENDIANNESS_NATIVE);
}



NA_DEF NAInt naGetBufferEndianness(NABuffer* buffer){
  return buffer->endianness;
}



NA_DEF NAByte naGetBufferByte(const NABuffer* buffer, NAInt indx){
  const NAByte* retbyte = NA_NULL;
  if(naIsBufferEmpty(buffer)){return '\0';}
  
  NAListIterator iter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListPosition(&iter, naGetListCurrentPosition(&(buffer->iter)));
  naLocateBufferPartOffset(&iter, indx);

  if(!naIsListAtInitial(&iter)){
    retbyte = naGetBufferPartDataPointerConst(naGetListCurrentConst(&iter), indx);
  }
  naClearListIterator(&iter);
  return retbyte ? (*retbyte) : '\0';
}



NA_DEF NAByte naGetBufferCurrentByte(const NABuffer* buffer){
  return naGetBufferByte(buffer, buffer->curoffset);
}


NA_DEF NAInt naSearchBufferByteOffset(const NABuffer* buffer, NAByte byte, NAInt startoffset, NABool leadtotrail){
  NAInt retindex = NA_INVALID_MEMORY_INDEX;
  if(naIsBufferEmpty(buffer)){return NA_INVALID_MEMORY_INDEX;}

  NAListIterator iter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListPosition(&iter, naGetListCurrentPosition(&(buffer->iter)));
  naLocateBufferPartOffset(&iter, startoffset);

  while(!naIsListAtInitial(&iter)){
    const NABufferPart* part = naGetListCurrentConst(&iter);
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naSearchBufferByteOffset", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, startoffset); 
    if(leadtotrail){
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
    if(leadtotrail){naIterateList(&iter, 1);}else{naIterateList(&iter, -1);}
  }

  naClearListIterator(&iter);
  return retindex;
}





NA_DEF NABool naEqualBufferToBuffer(const NABuffer* buffer1, const NABuffer* buffer2, NABool casesensitive){
  if(buffer1 == buffer2){return NA_TRUE;}
  if(naGetBufferRange(buffer1).length != naGetBufferRange(buffer2).length){return NA_FALSE;}
  NABool resultequal = NA_TRUE;;
  
  NAListIterator iter1 = naMakeListIteratorAccessor(&(buffer1->parts));
  NAListIterator iter2 = naMakeListIteratorAccessor(&(buffer2->parts));
  naLocateListFirst(&iter1);
  naLocateListFirst(&iter2);
  const NABufferPart* part1 = naGetListCurrentConst(&iter1);
  const NABufferPart* part2 = naGetListCurrentConst(&iter2);
  
  NAInt offset1 = naGetBufferRange(buffer1).origin;
  NAInt offset2 = naGetBufferRange(buffer2).origin;
  
  while(part1 && part2){
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part1))
        naError("naEqualBufferToBuffer", "Buffer 1 has sparse part");
      if(naIsBufferPartSparse(part2))
        naError("naEqualBufferToBuffer", "Buffer 2 has sparse part");
    #endif
    NAInt remainingbytes1 = naGetBufferPartEnd(part1) - offset1;
    NAInt remainingbytes2 = naGetBufferPartEnd(part2) - offset2;
    NAInt remainingbytes = naMini(remainingbytes1, remainingbytes2);
    const NAByte* bytes1 = naGetBufferPartDataPointerConst(part1, offset1);
    const NAByte* bytes2 = naGetBufferPartDataPointerConst(part2, offset2);
    if(bytes1 != bytes2){
      if(!naEqualUTF8CStringLiterals((NAUTF8Char*)bytes1, (NAUTF8Char*)bytes2, remainingbytes, casesensitive)){
        resultequal = NA_FALSE;
        break;
      }
    }
    offset1 += remainingbytes;
    offset2 += remainingbytes;
    if(remainingbytes1 == remainingbytes){
      naIterateList(&iter1, 1);
      part1 = naGetListCurrentConst(&iter1);;
    }
    if(remainingbytes2 == remainingbytes){
      naIterateList(&iter2, 1);
      part2 = naGetListCurrentConst(&iter2);;
    }
  }
  
  naClearListIterator(&iter1);
  naClearListIterator(&iter2);
  return resultequal;
}



NA_DEF NABool naEqualBufferToData(const NABuffer* buffer, const void* data, NABool casesensitive){
  NABool resultequal = NA_TRUE;
  const NAByte* bytes = (const NAByte*)data;
  
  NAListIterator iter = naMakeListIteratorAccessor(&(buffer->parts));
  naLocateListFirst(&iter);
  const NABufferPart* part = naGetListCurrentConst(&iter);
  
  NAInt offset = naGetBufferRange(buffer).origin;
  NAInt remaininglength = buffer->range.length;
  
  while(remaininglength){
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naEqualBufferToData", "Buffer has sparse part");
    #endif
    NAInt remainingbytes = naGetBufferPartEnd(part) - offset;
    const NAByte* bufferbytes = naGetBufferPartDataPointerConst(part, offset);
    if(bufferbytes != bytes){
      if(!naEqualUTF8CStringLiterals((NAUTF8Char*)bufferbytes, (NAUTF8Char*)bytes, remainingbytes, casesensitive)){
        resultequal = NA_FALSE;
        break;
      }
    }
    offset += remainingbytes;
    bytes += remainingbytes;
    naIterateList(&iter, 1);
    part = naGetListCurrentConst(&iter);;
  }
  
  naClearListIterator(&iter);
  return resultequal;
}




NA_DEF void naCacheBufferRange(NABuffer* buffer, NARangei range, NABool forcevolatile){
  naPrepareBuffer(buffer, range, forcevolatile, NA_FALSE);
}



NA_DEF void naDismissBufferRange(NABuffer* buffer, NARangei range){
  naUnlinkBufferRange(buffer, range);
}





NA_DEF void naAccumulateBufferToChecksum(NABuffer* buffer, NAChecksum* checksum){
  NAInt bytesize;
  NAInt curoffset;
  NAListIterator iter;

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
    src = naGetBufferPartDataPointerConst(curpart, curoffset);
    
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
      naError("naWriteBufferToFile", "Buffer has no determined range. Use naFixBufferRange");
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
    src = naGetBufferPartDataPointerConst(curpart, curoffset);
    
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
    src = naGetBufferPartDataPointerConst(curpart, curoffset);
    
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





// ////////////////////////////////////
// WRITING
// ////////////////////////////////////



// This is the internal function actually preparing and storing the bytes
// delivered in the parameters.
NA_HDEF void naStoreBufferBytes(NABuffer* buffer, const void* data, NAInt bytesize, NABool prepare){
  const NAByte* src = data;
  NAByte* dst;

  // We prepare the buffer for the whole range. There might be no parts or
  // sparse parts.
  if(prepare){
    naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, bytesize), NA_FALSE, NA_TRUE);
  }
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
    dst = naGetBufferPartDataPointerMutable(part, buffer->curoffset);
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


NA_DEF void naWriteBufferBytes(NABuffer* buffer, const void* data, NAInt bytesize){
  naStoreBufferBytes(buffer, data, bytesize, NA_TRUE);
}



NA_DEF void naWriteBufferi8(NABuffer* buffer, int8 value){
  buffer->converter.convert8(&value);
  naStoreBufferBytes(buffer, &value, 1, NA_TRUE);
}
NA_DEF void naWriteBufferi16(NABuffer* buffer, int16 value){
  buffer->converter.convert16(&value);
  naStoreBufferBytes(buffer, &value, 2, NA_TRUE);
}
NA_DEF void naWriteBufferi32(NABuffer* buffer, int32 value){
  buffer->converter.convert32(&value);
  naStoreBufferBytes(buffer, &value, 4, NA_TRUE);
}
NA_DEF void naWriteBufferi64(NABuffer* buffer, int64 value){
  buffer->converter.convert64(&value);
  naStoreBufferBytes(buffer, &value, 8, NA_TRUE);
}



NA_DEF void naWriteBufferu8(NABuffer* buffer, uint8 value){
  buffer->converter.convert8(&value);
  naStoreBufferBytes(buffer, &value, 1, NA_TRUE);
}
NA_DEF void naWriteBufferu16(NABuffer* buffer, uint16 value){
  buffer->converter.convert16(&value);
  naStoreBufferBytes(buffer, &value, 2, NA_TRUE);
}
NA_DEF void naWriteBufferu32(NABuffer* buffer, uint32 value){
  buffer->converter.convert32(&value);
  naStoreBufferBytes(buffer, &value, 4, NA_TRUE);
}
NA_DEF void naWriteBufferu64(NABuffer* buffer, uint64 value){
  buffer->converter.convert64(&value);
  naStoreBufferBytes(buffer, &value, 8, NA_TRUE);
}



NA_DEF void naWriteBufferf(NABuffer* buffer, float value){
  buffer->converter.convert32(&value);
  naStoreBufferBytes(buffer, &value, 4, NA_TRUE);
}
NA_DEF void naWriteBufferd(NABuffer* buffer, double value){
  buffer->converter.convert64(&value);
  naStoreBufferBytes(buffer, &value, 8, NA_TRUE);
}



NA_DEF void naWriteBufferi8v(NABuffer* buffer, const int8* src, NAInt count){
  int8 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 1), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert8(&value);
    naStoreBufferBytes(buffer, &value, 1, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi16v(NABuffer* buffer, const int16* src, NAInt count){
  int16 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 2), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert16(&value);
    naStoreBufferBytes(buffer, &value, 2, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi32v(NABuffer* buffer, const int32* src, NAInt count){
  int32 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naStoreBufferBytes(buffer, &value, 4, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferi64v(NABuffer* buffer, const int64* src, NAInt count){
  int64 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naStoreBufferBytes(buffer, &value, 8, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu8v(NABuffer* buffer, const uint8* src, NAInt count){
  uint8 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 1), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert8(&value);
    naStoreBufferBytes(buffer, &value, 1, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu16v(NABuffer* buffer, const uint16* src, NAInt count){
  uint16 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 2), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert16(&value);
    naStoreBufferBytes(buffer, &value, 2, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu32v(NABuffer* buffer, const uint32* src, NAInt count){
  uint32 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naStoreBufferBytes(buffer, &value, 4, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferu64v(NABuffer* buffer, const uint64* src, NAInt count){
  uint64 value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naStoreBufferBytes(buffer, &value, 8, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferfv(NABuffer* buffer, const float* src, NAInt count){
  float value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 4), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert32(&value);
    naStoreBufferBytes(buffer, &value, 4, NA_FALSE);
    src++;
    count--;
  }
}
NA_DEF void naWriteBufferdv(NABuffer* buffer, const double* src, NAInt count){
  double value;
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, count * 8), NA_FALSE, NA_TRUE);
  while(count){
    value = *src;
    buffer->converter.convert64(&value);
    naStoreBufferBytes(buffer, &value, 8, NA_FALSE);
    src++;
    count--;
  }
}



NA_DEF void naWriteBufferBuffer(NABuffer* dstbuffer, NABuffer* srcbuffer, NARangei srcrange){
  NABufferSource* tmpsource = dstbuffer->source;
  NAInt tmpsrcoffset = dstbuffer->srcoffset;
  
  dstbuffer->source = naCreateBufferSourceBuffer(naRetainBuffer(srcbuffer));
  
  dstbuffer->srcoffset = dstbuffer->curoffset - srcrange.origin;
  
  naCacheBufferRange(dstbuffer, naMakeRangei(dstbuffer->curoffset, srcrange.length), NA_FALSE);
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
  naPrepareBuffer(buffer, naMakeRangei(writeoffset, bytesize), NA_FALSE, NA_TRUE);

  // Prepare the read part.
  readoffset = buffer->curoffset - distance;
  naPrepareBuffer(buffer, naMakeRangei(readoffset, bytesize), NA_FALSE, NA_TRUE);

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
    
    naCopyn(naGetBufferPartDataPointerMutable(writepart, writeoffset), naGetBufferPartDataPointerConst(readpart, readoffset), (NAUInt)remaining);
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
  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, bytesize), NA_FALSE, NA_TRUE);
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
      if(!part)
        naError("naPutBufferBytes", "No more parts containing memory");
      if(naIsBufferPartSparse(part))
        naError("naPutBufferBytes", "Current part is sparse");
      if(!naContainsBufferPartOffset(part, buffer->curoffset))
        naError("naPutBufferBytes", "Current part does not contain current offset");
    #endif
    
    // We get the data pointer where we can read bytes.
    src = naGetBufferPartDataPointerConst(part, buffer->curoffset);
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

  naPrepareBuffer(buffer, naMakeRangei(buffer->curoffset, 1), NA_FALSE, NA_TRUE);
  // After this function, all parts should be present and filled with memory.
  // The iterator should point to the buffer part containing offset.
  NABufferPart* part = naGetListCurrentMutable(&(buffer->iter));
  src = naGetBufferPartDataPointerConst(part, buffer->curoffset);
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




NA_DEF void naSkipBufferWhitespaces(NABuffer* buffer){
  NABool nonwhitespacefound = NA_FALSE;

  while(!naIsListAtInitial(&(buffer->iter))){
    const NABufferPart* part = naGetListCurrentConst(&(buffer->iter));
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naSkipBufferWhitespaces", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 
    NAInt endoffset = naGetBufferPartEnd(part);
    while(buffer->curoffset < endoffset){
      if(*curbyte > ' '){
        nonwhitespacefound = NA_TRUE;
        break;
      }
      curbyte++;
      buffer->curoffset++;
    }
    if(!naContainsBufferPartOffset(part, buffer->curoffset)){naIterateList(&(buffer->iter), 1);}
    if(nonwhitespacefound){break;}
  }
}



NA_DEF void naSkipBufferDelimiter(NABuffer* buffer){
  NAByte curbyte = naGetBufferCurrentByte(buffer);
  if(curbyte <= ' '){
    naSkipBufferWhitespaces(buffer);
  }else{
    naSeekBufferRelative(buffer, 1);
  }
}



NA_DEF NAString* naParseBufferLine(NABuffer* buffer, NABool skipempty, NAInt* linesread){
  NABool lineendingfound = NA_FALSE;
  NABool checkwindowsend = NA_FALSE;
  NAInt linestart = buffer->curoffset;
  NAString* string = NA_NULL;
  if(linesread){*linesread = 0;}

  while(!naIsListAtInitial(&(buffer->iter))){
    if(lineendingfound && !checkwindowsend){break;}
    const NABufferPart* part = naGetListCurrentConst(&(buffer->iter));
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naParseBufferLine", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 
    NAInt endoffset = naGetBufferPartEnd(part);
    while(buffer->curoffset < endoffset){
      if(checkwindowsend){
        checkwindowsend = NA_FALSE;
        if(*curbyte == '\n'){buffer->curoffset++; break;}
      }
      if(lineendingfound){break;}
      if((*curbyte == '\r') || (*curbyte == '\n')){
        if(linesread){(*linesread)++;}
        if(skipempty && ((buffer->curoffset - linestart) == 0)){
          linestart++;
        }else{
          lineendingfound = NA_TRUE;
          string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(linestart, buffer->curoffset));
        }
        checkwindowsend = (*curbyte == '\r');
      }
      curbyte++;
      buffer->curoffset++;
    }
    if(!naContainsBufferPartOffset(part, buffer->curoffset)){naIterateList(&(buffer->iter), 1);}
  }

  if(!string){string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(linestart, buffer->curoffset));}

  return string;
}



NA_DEF NAString* naParseBufferToken(NABuffer* buffer){
  NAString* string = NA_NULL;
  NAInt tokenstart = buffer->curoffset;

  while(!naIsListAtInitial(&(buffer->iter))){
    const NABufferPart* part = naGetListCurrentConst(&(buffer->iter));
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naParseBufferToken", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 
    NAInt endoffset = naGetBufferPartEnd(part);
    while(buffer->curoffset < endoffset){
      if(*curbyte <= ' '){break;}
      curbyte++;
      buffer->curoffset++;
    }
    if(!naContainsBufferPartOffset(part, buffer->curoffset)){naIterateList(&(buffer->iter), 1);}
    if(string){break;}
  }

  string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(tokenstart, buffer->curoffset));
  naSkipBufferWhitespaces(buffer);
  return string;
}



NA_DEF NAString* naParseBufferTokenWithDelimiter(NABuffer* buffer, NAByte delimiter){
  NAString* string = NA_NULL;
  NAInt tokenstart = buffer->curoffset;
  const NABufferPart* part;
  
  while(!naIsListAtInitial(&(buffer->iter))){
    part = naGetListCurrentConst(&(buffer->iter));
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naParseBufferTokenWithDelimiter", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 
    NAInt endoffset = naGetBufferPartEnd(part);
    while(buffer->curoffset < endoffset){
      if(*curbyte == delimiter){break;}
      curbyte++;
      buffer->curoffset++;
    }
    if(!naContainsBufferPartOffset(part, buffer->curoffset)){naIterateList(&(buffer->iter), 1);}
    if(string){break;}
  }

  string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(tokenstart, buffer->curoffset));
  naSeekBufferRelative(buffer, 1);
  return string;
}



NA_DEF NAString* naParseBufferPathComponent(NABuffer* buffer){
  NAString* string = NA_NULL;
  NAInt tokenstart = buffer->curoffset;
  const NABufferPart* part;
  
  while(!naIsListAtInitial(&(buffer->iter))){
    part = naGetListCurrentConst(&(buffer->iter));
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naParseBufferTokenWithDelimiter", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 
    NAInt endoffset = naGetBufferPartEnd(part);
    while(buffer->curoffset < endoffset){
      if((*curbyte == '/') || *curbyte == '\\'){break;}
      curbyte++;
      buffer->curoffset++;
    }
    if(!naContainsBufferPartOffset(part, buffer->curoffset)){naIterateList(&(buffer->iter), 1);}
    if(string){break;}
  }

  string = naNewStringWithBufferExtraction(buffer, naMakeRangeiWithStartAndEnd(tokenstart, buffer->curoffset));
  naSeekBufferRelative(buffer, 1);
  return string;
}



NA_DEF NAInt naParseBufferDecimalUnsignedInteger(NABuffer* buffer, uint64* retint, NAInt maxdigitcount, uint64 max){
  NAInt bytesused;
  uint64 prevval;
  NABool endfound = NA_FALSE;
  #ifndef NDEBUG
    if(!retint)
      {naCrash("naParseBufferDecimalUnsignedInteger", "retint is Null-Pointer"); return 0;}
  #endif
  if(maxdigitcount == 0){maxdigitcount = naGetRangeiEnd(buffer->range) - buffer->curoffset;}
  bytesused = 0;
  prevval = 0LL;
  *retint = 0LL;
  
  while(!naIsListAtInitial(&(buffer->iter))){
    const NABufferPart* part = naGetListCurrentConst(&(buffer->iter));
    #ifndef NDEBUG
      if(naIsBufferPartSparse(part))
        naError("naParseBufferTokenWithDelimiter", "sparse part detected.");
    #endif
    const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 
    NAInt endoffset = naGetBufferPartEnd(part);
    while(buffer->curoffset < endoffset){
      if(bytesused >= maxdigitcount){break;}

      if((*curbyte < '0') || (*curbyte > '9')){endfound = NA_TRUE; break;}
      *retint = *retint * 10LL + (uint64)(*curbyte - '0');
      #ifndef NDEBUG
        if(*retint > max)
          naError("naParseBufferDecimalUnsignedInteger", "The value overflowed max.");
        if(*retint < prevval)
          naError("naParseBufferDecimalUnsignedInteger", "The value overflowed 64 bit integer space.");
      #endif
      if((*retint < prevval) || (*retint > max)){
        *retint = max;
      }
      prevval = *retint;
      bytesused++;
      curbyte++;
    }
    if(!naContainsBufferPartOffset(part, buffer->curoffset)){naIterateList(&(buffer->iter), 1);}
    if(endfound){break;}
  }
  
  return bytesused;
}



NA_DEF NAInt naParseBufferDecimalSignedInteger(NABuffer* buffer, int64* retint, NAInt maxdigitcount, int64 min, int64 max){
  int64 sign = 1LL;
  NAInt bytesused = 0;
  int64 limit = max;
  uint64 intvalue;

  const NABufferPart* part = naGetListCurrentConst(&(buffer->iter));
  const NAByte* curbyte = naGetBufferPartDataPointerConst(part, buffer->curoffset); 

  // Check for a potential sign at the first character
  if(*curbyte == '+'){
    bytesused = 1;
    maxdigitcount--;
    naSeekBufferRelative(buffer, 1);
  }else if(*curbyte == '-'){
    sign = -1LL;
    limit = -min;
    bytesused = 1;
    maxdigitcount--;
    naSeekBufferRelative(buffer, 1);
  }

  bytesused += naParseBufferDecimalUnsignedInteger(buffer, &intvalue, maxdigitcount, (uint64)limit);
  *retint = sign * (int64)intvalue;
  return bytesused;
}



NA_DEF int8 naParseBufferInt8(NABuffer* buffer, NABool skipdelimiter){
  int64 intvalue;  
  naParseBufferDecimalSignedInteger(buffer, &intvalue, 0, NA_INT8_MIN, NA_INT8_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (int8)intvalue;
}
NA_DEF int16 naParseBufferInt16(NABuffer* buffer, NABool skipdelimiter){
  int64 intvalue;  
  naParseBufferDecimalSignedInteger(buffer, &intvalue, 0, NA_INT16_MIN, NA_INT16_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (int16)intvalue;
}
NA_DEF int32 naParseBufferInt32(NABuffer* buffer, NABool skipdelimiter){
  int64 intvalue;  
  naParseBufferDecimalSignedInteger(buffer, &intvalue, 0, NA_INT32_MIN, NA_INT32_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (int32)intvalue;
}
NA_DEF int64 naParseBufferInt64(NABuffer* buffer, NABool skipdelimiter){
  int64 intvalue;  
  naParseBufferDecimalSignedInteger(buffer, &intvalue, 0, NA_INT64_MIN, NA_INT64_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (int64)intvalue;
}



NA_DEF uint8 naParseBufferUInt8(NABuffer* buffer, NABool skipdelimiter){
  uint64 uintvalue;  
  naParseBufferDecimalUnsignedInteger(buffer, &uintvalue, 0, NA_UINT8_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (uint8)uintvalue;
}
NA_DEF uint16 naParseBufferUInt16(NABuffer* buffer, NABool skipdelimiter){
  uint64 uintvalue;  
  naParseBufferDecimalUnsignedInteger(buffer, &uintvalue, 0, NA_UINT16_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (uint16)uintvalue;
}
NA_DEF uint32 naParseBufferUInt32(NABuffer* buffer, NABool skipdelimiter){
  uint64 uintvalue;  
  naParseBufferDecimalUnsignedInteger(buffer, &uintvalue, 0, NA_UINT32_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (uint32)uintvalue;
}
NA_DEF uint64 naParseBufferUInt64(NABuffer* buffer, NABool skipdelimiter){
  uint64 uintvalue;  
  naParseBufferDecimalUnsignedInteger(buffer, &uintvalue, 0, NA_UINT64_MAX);
  if(skipdelimiter){naSkipBufferDelimiter(buffer);}
  return (uint64)uintvalue;
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
