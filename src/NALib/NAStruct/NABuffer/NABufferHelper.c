
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"



////NA_HAPI NABool naIsBufferAtInitial(NABufferIterator* iter){
////  return naIsListAtInitial(&(iter->partiter));
////}
//
//
//
////
////// Moves the given iterator to that position which contains offset. Will
////// point to the initial position if offset is not found.
////NA_HIDEF void naLocateBufferPartOffset(NAListIterator* iter, NAInt offset){
////  const NABufferPart* part;
////  const NABufferPart* newpart;
////
////  // We make sure, the iterator points to a buffer part. We choose to start
////  // with the last part by default, as usually, things are happening at the
////  // end.
////  if(naIsListAtInitial(iter)){naLocateListLast(iter);}
////  part = naGetListCurConst(iter);
////
////  while(part){
////    if(naGetBufferPartEnd(part) <= offset){
////      // If the current part is below the desired offset...
////      naIterateList(iter);
////      newpart = naGetListCurConst(iter);
////      #ifndef NDEBUG
////        if(newpart && (naGetBufferPartStart(newpart) != naGetBufferPartEnd(part)))
////          naError("naLocateBufferPartOffset", "next part not adjacent");
////      #endif
////      part = newpart;
////    }else if(naGetBufferPartStart(part) > offset){
////      // if the current part is above the desired offset
////      naIterateListBack(iter);
////      newpart = naGetListCurConst(iter);
////      #ifndef NDEBUG
////        if(newpart && (naGetBufferPartEnd(newpart) != naGetBufferPartStart(part)))
////          naError("naLocateBufferPartOffset", "prev part not adjacent");
////      #endif
////      part = newpart;
////    }else{
////      // This part should be the one containing offset.
////      #ifndef NDEBUG
////        if(!naContainsBufferPartOffset(part, offset))
////          naError("naLocateBufferPartOffset", "final buffer part does not contain desired offset");
////      #endif
////      break;
////    }
////  }
////}
////



// This function ensures that the full given range is available as parts in
// this buffer. If not available, the buffer is extended with sparse parts
// at the beginning and the end.
NA_HDEF void naEnsureBufferRange(NABuffer* buffer, NARangei range){
  NABufferPart* part;
  
  #ifndef NDEBUG
    if(range.length <= 0)
      naError("naEnsureBufferRange", "Range length shall be >= 0");
    if(naHasBufferFixedRange(buffer) && (range.origin < buffer->range.origin))
      naError("naEnsureBufferRange", "Range of buffer is fixed but trying to access range below");
    if(naHasBufferFixedRange(buffer) && (naGetRangeiEnd(range) > naGetRangeiEnd(buffer->range)))
      naError("naEnsureBufferRange", "Range of buffer is fixed but trying to access range above");
  #endif
  
  if(naIsBufferEmpty(buffer)){
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    part = naNewBufferPartSparse((NAUInt)range.length);
    naAddTreeFirstMutable(&(buffer->parts), part);
    buffer->range = range;
    
  }else{
    
    // First, we test if we need to add a sparse part at the beginning.
    if(range.origin < buffer->range.origin){
      NAUInt additionalbytes = (NAUInt)(buffer->range.origin - range.origin);
      part = naGetTreeFirstMutable(&(buffer->parts));
      if(naIsBufferPartSparse(part)){
        // If the first part of this list is already sparse, we simply extend
        // its range.
        part->bytesize += additionalbytes;
      }else{
        // We create a sparse part at the beginning.
        part = naNewBufferPartSparse(additionalbytes);
        naAddTreeFirstMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(range.origin, naGetRangeiEnd(buffer->range));
    }
    
    // Then, we test if we need to add a sparse part at the end.
    if(naGetRangeiEnd(range) > naGetRangeiEnd(buffer->range)){
      NAUInt additionalbytes = (NAUInt)(naGetRangeiEnd(range) - naGetRangeiEnd(buffer->range));
      part = naGetTreeLastMutable(&(buffer->parts));
      if(naIsBufferPartSparse(part)){
        // If the last part of this list is already sparse, we simply extend
        // its range.
        part->bytesize += additionalbytes;
      }else{
        // We create a sparse part at the end.
        part = naNewBufferPartSparse(additionalbytes);
        naAddTreeLastMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(buffer->range.origin, naGetRangeiEnd(range));
    }    
  }
}



////
////
////// This is the core function of NABuffer.
//////
////// This function fills the specified range of the buffer with memory and
////// content. If the forcevolatile parameter is NA_TRUE or the buffer source is
////// defined to be volatile, all bytes in that range are filled anew.
////// if locatestart is set to NA_TRUE, the iterator of buffer will point to the
////// part containing the start offset of range. Otherwise, it will point at the
////// part containing the end of the range, possibly resulting in the initial
////// position of the list.
////NA_HDEF void naPrepareBuffer(NABufferIterator* iter, NARangei range, NABool forcevolatile, NABool locatestart){
////  NABufferPart* part;
////  NABuffer* buffer;
////  NABufferIterator* itersrc;
////  NAInt originalrangeorigin;
////  NABool volatil;
////  NARangei subrange;
////
////  originalrangeorigin = range.origin;
////  buffer = naGetBufferIteratorBufferMutable(iter);
////  volatil = forcevolatile | naHasBufferVolatileSource(buffer);
////
////  // We try to get a source buffer. The following function returns NA_NULL if
////  // this buffer has not a buffer as a source.
////  itersrc = naGetBufferSourceBufferIterator(buffer->source);
////  
////  if(itersrc){
////    // In case there is a parent buffer as a source, we prepare that source
////    // buffer furst and then just take references out of it.
////    NAInt srcrangeorigin;
////
////    // First, we make sure that parent buffer is fully prepared.
////    NARangei srcrange = naMakeRangei(range.origin - buffer->srcoffset, range.length);
////    naPrepareBuffer(itersrc, srcrange, volatil, NA_TRUE);
////    // After this function call, the iterator points to the part containing
////    // the start of srcrange.
////
////    // Then, we ensure, that there are buffer parts in the parts list of this
////    // buffer, spanning the desired range by creating sparse buffers wherever
////    // necessary.
////    naEnsureBufferRange(buffer, range);
////    naLocateBufferPartOffset(&(iter->partiter), range.origin);
////
////    // After this function call, the iterator points to the part containing
////    // the start of range.
////
////    // Then, we start referencing all parts of the source.
////    srcrangeorigin = range.origin - buffer->srcoffset;
////
////    // We perform the preparation as long as there are still bytes left in range.
////    while(range.length){
////      NABufferPart* srcpart;
////      NABufferPart* dstpart;
////      NAInt remainingsrcbytes;
////      NAInt remainingdstbytes;
////      NAInt remainingbytes;
////
////      #ifndef NDEBUG
////        if(range.length < 0)
////          naError("naPrepareBuffer", "length is negative");
////      #endif
////
////      // We get the current parts of src and dst.
////      srcpart = naGetListCurMutable(&(itersrc->partiter));
////      dstpart = naGetListCurMutable(&(iter->partiter));
////      // We count, how many bytes are left on both parts.
////      remainingsrcbytes = (naGetBufferPartEnd(srcpart) - srcrangeorigin);
////      remainingdstbytes = (naGetBufferPartEnd(dstpart) - range.origin);
////      // we choose the number of bytes which is smaller.
////      remainingbytes = naMini(remainingsrcbytes, remainingdstbytes);
////      remainingbytes = naMini(remainingbytes, range.length);
////
////      // We decide what to do based on the dst buffer.
////      if(naIsBufferPartSparse(dstpart)){
////        // If the dst buffer is sparse, we split the part fitting the desired
////        // number of bytes.
////        naSplitBufferSparsePart(&(iter->partiter), naMakeRangei(range.origin, remainingbytes));
////        // We need to reload the current dstpart, as it might have changed by
////        // the call to naSplitBufferSparsePart.
////        dstpart = naGetListCurMutable(&(iter->partiter));
////        
////        // We replace the current sparse part with a reference to the
////        // source part.
////        naReferenceBufferPart(dstpart, srcpart, buffer->srcoffset);
////        
////        // Then we try to combine this newly available part with its neighbors.
////        naCombineBufferPartAdjacents(&(iter->partiter));
////
////        // Now, the current dst part is completely filled. Move to the next one.
////        naIterateList(&(iter->partiter));
////        
////      }else{
////        // The current buffer part already contains memory.
////
////        // These bytes can usually be skipped, except if volatil is NA_TRUE and
////        // the source buffers do not match.
////        if(volatil && (naGetBufferPartDataPointerConst(dstpart, range.origin) != naGetBufferPartDataPointerConst(srcpart, srcrangeorigin))){
////          // if the source buffer is volatile, we need to replace the existing
////          // bytes in the desired subrange.
////          subrange = naMakeRangeiWithStartAndEnd(range.origin, range.origin + remainingbytes);
////          
////          // First, we dismiss the remaining bytes from the dst buffer.
////          naDismissBufferRange(naGetBufferIteratorBufferMutable(iter), subrange);
////          // After this call, the iterator is at the new sparse part
////          dstpart = naGetListCurMutable(&(iter->partiter));
////          // This new sparse part might be bigger than anticipated. We split it
////          // again into the size we need
////          naSplitBufferSparsePart(&(iter->partiter), subrange);
////
////          // We replace the current sparse part with a reference to the
////          // source part.
////          naReferenceBufferPart(dstpart, srcpart, buffer->srcoffset);
////          
////          // Then we try to combine this newly available part with its neighbors.
////          naCombineBufferPartAdjacents(&(iter->partiter));
////
////          // Now, the current dst part is completely filled. Move to the next one.
////          naIterateList(&(iter->partiter));
////        }
////        
////        // We can skip ahead the dst buffer if the remaining bytes match.
////        if(remainingbytes == remainingdstbytes){naIterateList(&(iter->partiter));}
////      }
////
////      // If the remainingbytes matches the src bytes, the source part can be
////      // iterated forward.
////      if(remainingbytes == remainingsrcbytes){naIterateList(&(itersrc->partiter));}
////
////      // We adapt the range and source origin.
////      range = naMakeRangeiE(range.origin + remainingbytes, range.length - remainingbytes);
////      srcrangeorigin += remainingbytes;
////
////    }
////    
////  }else{
////  
////    // In case there is no parent buffer, we fill up our own linear memory.
////    // Such memory is usually aligned by NA_INTERNAL_BUFFER_PART_BYTESIZE.
////      
////    NAInt normedstart;
////    NAInt normedend;
////        
////    normedstart = naGetBufferPartNormedStart(range.origin);
////    if(naHasBufferFixedRange(buffer)){normedstart = naMaxi(normedstart, buffer->range.origin);}
////    normedend = naGetBufferPartNormedEnd(naGetRangeiEnd(range));
////    if(naHasBufferFixedRange(buffer)){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
////    range = naMakeRangeiWithStartAndEnd(normedstart, normedend);
////
////    // First, we ensure, that there are buffer parts in the parts list
////    // spanning the desired normed range by creating sparse buffers if
////    // necessary.
////    naEnsureBufferRange(buffer, range);
////    naLocateBufferPartOffset(&(iter->partiter), range.origin);
////
////    // After this function call, the iterator points to the part containing
////    // the origin of range.
////
////    // Then, we go through the list of parts and change all sparse buffers into
////    // memory when within range.
////
////    // We perform the preparation as long as there are bytes left in range.
////    while(range.length > 0){
////      
////      part = naGetListCurMutable(&(iter->partiter));
////
////      #ifndef NDEBUG
////        // This should not happen, as range.length shall be smallerequal 0 when
////        // the list comes to an end.
////        if(!part)
////          naError("naPrepareBuffer", "list overflow. This should not happen.");
////      #endif
////
////      if(naIsBufferPartSparse(part)){
////        // If the current part is sparse, we create buffer parts with actual
////        // memory.
////
////        // We create a suitable range within the parts range which tries to
////        // be aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE but has a certain
////        // margin to be bigger and hence reduce the number of small parts.
////        normedstart = naGetBufferPartNormedStart(range.origin);
////        if((normedstart - naGetBufferPartStart(part)) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedstart = naGetBufferPartStart(part);}
////        normedend = naGetBufferPartNormedEnd(range.origin + NA_INTERNAL_BUFFER_PART_BYTESIZE);
////        if((naGetBufferPartEnd(part) - normedend) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedend = naGetBufferPartEnd(part);}
////        
////        // Note that the previous computation of normedstart and normedend also
////        // automatically handles parts being smaller than the normed range and
////        // hence never resulting in a subrange bigger than the range of the
////        // actual part.
////        subrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);
////        #ifndef NDEBUG
////          if(!naContainsRangeiRange(naGetBufferPartRange(part), subrange))
////            naError("naPrepareBuffer", "part range does not contain subrange");
////        #endif
////
////        // Then we split the sparse buffer if necessary.
////        naSplitBufferSparsePart(&(iter->partiter), subrange);
////        
////        // Now, iter points at a sparse part containing subrange
////        // which can be filled.
////        part = naGetListCurMutable(&(iter->partiter));
////        #ifndef NDEBUG
////          if(!naEqualRangei(subrange, naGetBufferPartRange(part)))
////            naError("naPrepareBuffer", "range lengths do not match");
////        #endif
////        naAllocateBufferPartMemory(part);
////        naFillBufferSourcePart(buffer->source, part);
////        
////      }else{
////        // When the current part already contains bytes, we do not have to
////        // allocate memory. But we might need to fill the memory anew if
////        // volatil is NA_TRUE
////        if(volatil){naFillBufferSourcePart(buffer->source, part);}
////      }
////
////      // Reaching here, the current part is filled with memory. We advance
////      // to the next part. Note that the calculation of the new range will
////      // most probably result in a negative length when reaching the end of
////      // the desired range, hence breaking the loop condition.
////
////      range = naMakeRangeiWithStartAndEnd(naGetBufferPartEnd(part), naGetRangeiEnd(range));
////      naIterateList(&(iter->partiter));
////    }
////  }
////    
////  if(locatestart){
////    // Reaching here, iter points to the part one after the part containing the
////    // maximum offset of range. We locate the buffer iterator at the first
////    // desired offset. Note that we can not use a cached list iterator as this
////    // might have changed during this preparation.
////    naLocateBufferPartOffset(&(iter->partiter), originalrangeorigin);
////
////    #ifndef NDEBUG
////      if(!naContainsBufferPartOffset(naGetListCurMutable(&(iter->partiter)), originalrangeorigin))
////        naError("naPrepareBuffer", "start offset not in range of current part.");
////    #endif
////  }
////
////}
//
//
////
////// This function makes the bytes declared in range unavailable by replacing
////// that range with a sparse part. As a consequence, certain buffer parts may
////// not be used anymore and will be automatically deallocated.
////NA_HDEF void naUnlinkBufferRange(NABuffer* buffer, NARangei range){
////  NAInt rangepos;
////  NABufferPart* sparsepart;
////  NAListIterator iter;
////  
////  #ifndef NDEBUG
////    if(naIsBufferEmpty(buffer))
////      naError("naDismissBufferRange", "Buffer ist empty");
////    if(!naContainsRangeiRange(buffer->range, range))
////      naError("naDismissBufferRange", "Range of is not within buffer range");
////  #endif
////  
////  iter = naMakeListModifier(&(buffer->parts));
////  naLocateBufferPartOffset(&iter, range.origin);
////  
////  // We create a sparse part in place where the dismissed range lies.
////  // As the range will change during the alrogithm, we create it now and
////  // add it later.
////  sparsepart = naNewBufferPartSparse(range);
////
////  // We go through all parts being part of the given range and split them.
////  // The current position of the iterator always points at the one part
////  // containing the range.offset.
////  while(range.length){
////    NABufferPart* part;
////    NARangei partrange;
////    
////    part = naGetListCurMutable(&iter);
////    partrange = naGetBufferPartRange(part);
////    if(naContainsRangeiRange(range, partrange)){
////      // If this parts range is contained completely in the range to dismiss,
////      // we remove the whole part.
////      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(partrange), naGetRangeiEnd(range));
////      naDelete(naRemoveListCurMutable(&iter, NA_TRUE));
////    }else if(range.origin == partrange.origin){
////      // If the origins match, we split the part such that the remaining bytes
////      // lie at the end of the current part.
////      part->range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partrange));
////      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
////    }else if(naGetRangeiEnd(range) >= naGetRangeiEnd(partrange)){
////      // If the desired range is equal or larger than the part range, we split
////      // the current part such that the remaining bytes lie at the beginning
////      // of the current part. After that, we iterate to the part after the
////      // removed subpart.
////      rangepos = naGetRangeiEnd(partrange);
////      part->range = naMakeRangeiWithStartAndEnd(partrange.origin, range.origin);
////      range = naMakeRangeiWithStartAndEnd(rangepos, naGetRangeiEnd(range));
////      naIterateList(&iter);
////    }else{
////      // Reaching here, the desired range lies completely within the current
////      // part. We subdivide the current part into two pieces.
////      NABufferPart* newpart = naNewBufferPartSparse(naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(partrange)));
////      naReferenceBufferPart(newpart, part, partrange.origin);
////      naAddListAfterMutable(&iter, newpart);
////      part->range = naMakeRangeiWithStartAndEnd(partrange.origin, range.origin);
////      range = naMakeRangeiWithStartAndEnd(naGetRangeiEnd(range), naGetRangeiEnd(range));
////      naIterateList(&iter);
////    }
////  }
////  // The iterator now points to the one part after the insertion position of
////  // the new sparse part.
////
////  // We add the sparse part one before the current position and move the
////  // iterator at that position.
////  naAddListBeforeMutable(&iter, sparsepart);
////  naIterateListBack(&iter);
////  naCombineBufferPartAdjacents(&iter);
////
////  naClearListIterator(&iter);
////}
//



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
