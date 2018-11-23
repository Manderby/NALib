
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NABuffer.h"



//// Moves the given iterator to that position which contains offset. Will
//// point to the initial position if offset is not found.
//NA_HIDEF void naLocateBufferPartOffset(NAListIterator* iter, NAInt offset){
//  const NABufferPart* part;
//  const NABufferPart* newpart;
//
//  // We make sure, the iterator points to a buffer part. We choose to start
//  // with the last part by default, as usually, things are happening at the
//  // end.
//  if(naIsListAtInitial(iter)){naLocateListLast(iter);}
//  part = naGetListCurConst(iter);
//
//  while(part){
//    if(naGetBufferPartEnd(part) <= offset){
//      // If the current part is below the desired offset...
//      naIterateList(iter);
//      newpart = naGetListCurConst(iter);
//      #ifndef NDEBUG
//        if(newpart && (naGetBufferPartStart(newpart) != naGetBufferPartEnd(part)))
//          naError("naLocateBufferPartOffset", "next part not adjacent");
//      #endif
//      part = newpart;
//    }else if(naGetBufferPartStart(part) > offset){
//      // if the current part is above the desired offset
//      naIterateListBack(iter);
//      newpart = naGetListCurConst(iter);
//      #ifndef NDEBUG
//        if(newpart && (naGetBufferPartEnd(newpart) != naGetBufferPartStart(part)))
//          naError("naLocateBufferPartOffset", "prev part not adjacent");
//      #endif
//      part = newpart;
//    }else{
//      // This part should be the one containing offset.
//      #ifndef NDEBUG
//        if(!naContainsBufferPartOffset(part, offset))
//          naError("naLocateBufferPartOffset", "final buffer part does not contain desired offset");
//      #endif
//      break;
//    }
//  }
//}
//



// This function ensures that the full given range is available as parts in
// this buffer. If not available, the buffer is extended with sparse parts
// at the beginning and the end.
NA_HDEF void naEnsureBufferRange(NABuffer* buffer, NAInt start, NAInt end){
  NABufferPart* part;
  NAUInt length;
  
  #ifndef NDEBUG
    if((end - start) <= 0)
      naError("naEnsureBufferRange", "Range length shall be >= 0");
    if(naHasBufferFixedRange(buffer) && (start < buffer->range.origin))
      naError("naEnsureBufferRange", "Range of buffer is fixed but trying to access range below");
    if(naHasBufferFixedRange(buffer) && (end > naGetRangeiEnd(buffer->range)))
      naError("naEnsureBufferRange", "Range of buffer is fixed but trying to access range above");
  #endif

  length = (NAUInt)(end - start);
  
  if(naIsBufferEmpty(buffer)){
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    part = naNewBufferPartSparse(length);
    naAddTreeFirstMutable(&(buffer->parts), part);
    buffer->range = naMakeRangeiWithStartAndEnd(start, end);
    
  }else{
    
    // First, we test if we need to add a sparse part at the beginning.
    if(start < buffer->range.origin){
      NAUInt additionalbytes = (NAUInt)(buffer->range.origin - start);
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
      buffer->range = naMakeRangeiWithStartAndEnd(start, naGetRangeiEnd(buffer->range));
    }
    
    // Then, we test if we need to add a sparse part at the end.
    if(end > naGetRangeiEnd(buffer->range)){
      NAUInt additionalbytes = (NAUInt)(end - naGetRangeiEnd(buffer->range));
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
      buffer->range = naMakeRangeiWithStartAndEnd(buffer->range.origin, end);
    }    
  }
}



#if NA_BUFFER_PART_BYTESIZE == 0
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)naGetRuntimeMemoryPageSize())
#else
  #define NA_INTERNAL_BUFFER_PART_BYTESIZE ((NAInt)NA_BUFFER_PART_BYTESIZE)
#endif



NA_HIDEF NAInt naGetBufferPartNormedStart(NAInt start){
  NAInt signshift = (start < 0);   // Note that (start < 0) either results in 0 or 1.
  return (((start + signshift) / NA_INTERNAL_BUFFER_PART_BYTESIZE) - signshift) * NA_INTERNAL_BUFFER_PART_BYTESIZE;
  // Examples explain best how this behaves (assume default partsize to be 10):
  //  11:  (( 11 + 0) / 10) - 0 * 10 =  10
  //  10:  (( 10 + 0) / 10) - 0 * 10 =  10
  //   9:  ((  9 + 0) / 10) - 0 * 10 =   0
  //   1:  ((  1 + 0) / 10) - 0 * 10 =   0
  //   0:  ((  0 + 0) / 10) - 0 * 10 =   0
  //  -1:  (( -1 + 1) / 10) - 1 * 10 = -10
  //  -9:  (( -9 + 1) / 10) - 1 * 10 = -10
  // -10:  ((-10 + 1) / 10) - 1 * 10 = -10
  // -11:  ((-11 + 1) / 10) - 1 * 10 = -20
}



NA_HIDEF NAInt naGetBufferPartNormedEnd(NAInt end){
  // Return the end coordinate, such that end-1 (=max) is within it.
  return naGetBufferPartNormedStart(naMakeMaxWithEndi(end)) + NA_INTERNAL_BUFFER_PART_BYTESIZE;
}



// This is the core function of NABuffer.
//
// This function fills the specified number of bytes in the buffer with memory
// starting from the current position. If the forcevolatile parameter is set
// to NA_TRUE or the buffer source is defined to be volatile, all bytes in that
// range are filled anew. If locatestart is set to NA_TRUE, the iterator of
// buffer will point to the part containing the start offset of range.
// Otherwise, it will point at the part containing the end of the range,
// possibly resulting in the initial position of the list.
NA_HDEF void naPrepareBuffer(NABufferIterator* iter, NAInt bytecount, NABool forcevolatile, NABool locatestart){
  NABuffer* buffer;
  NAInt originalbytecount;
  NABool volatil;

  #ifndef NDEBUG
    if(iter->curbit)
      naError("naPrepareBuffer", "bitcount should be 0");
    if(bytecount <= 0)
      naError("naPrepareBuffer", "bytecount should be >= 1");
  #endif

  originalbytecount = bytecount;
  buffer = naGetBufferIteratorBufferMutable(iter);
  volatil = forcevolatile | naHasBufferVolatileSource(buffer);
  
  if(naIsTreeAtInitial(&(iter->partiter))){
    // Reaching here means one of two things: Either the iterator has just not
    // been used yet or the buffer is completely empty. In the latter case, we
    // right now ensure the whole desired buffer range including the bytes
    // requested for preparing. In the former case, we just search for the
    // first buffer part and will enhace later if necessary.
    //
    // Note that enhancing and even just the testing if enhancing is required
    // uses a considerable amount of time hence should be avoided whenever
    // possible.
    if(naIsBufferEmpty(buffer)){
      NAInt normedstart = naGetBufferPartNormedStart(buffer->range.origin);
      if(naHasBufferFixedRange(buffer)){normedstart = naMaxi(normedstart, buffer->range.origin);}
      NAInt normedend = naGetBufferPartNormedEnd(buffer->range.origin + bytecount);
      if(naHasBufferFixedRange(buffer)){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
      naEnsureBufferRange(buffer, normedstart, normedend);
    }
    
    // Now, we can be sure that there is a part. We start at the beginning.
    naLocateTreeFirst(&(iter->partiter));
    iter->partoffset = 0;
    #ifndef NDEBUG
      if(naIsTreeAtInitial(&(iter->partiter)))
        naError("naPrepareBuffer", "No Buffer part found after being in initial state");
    #endif
  }
  
  // We perform the preparation as long as there are still bytes left.
  while(bytecount){
    NABufferPart* part = naGetTreeCurMutable(&(iter->partiter));
      #ifndef NDEBUG
        if(!part)
          naError("naPrepareBuffer", "No buffer part available.");
      #endif

    if(naIsBufferPartSparse(part)){
      // If the current part is sparse, we create buffer parts with actual
      // memory.   

      // We create a suitable range within the parts range which tries to
      // be aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE but has a certain
      // margin to be bigger and hence reduce the number of small parts.
      NAInt normedstart = naGetBufferPartNormedStart(iter->partoffset);
      NAInt normedend = naGetBufferPartNormedEnd(iter->partoffset + bytecount);
      if(((NAInt)part->bytesize - normedend) < (NA_INTERNAL_BUFFER_PART_BYTESIZE / 2)){normedend = (NAInt)part->bytesize;}
      
      // Note that the previous computation of normedstart and normedend also
      // automatically handles parts being smaller than the normed range and
      // hence never resulting in a subrange bigger than the range of the
      // actual part.

      // We split the sparse buffer if necessary.
      naSplitBufferSparsePart(iter, (NAUInt)normedstart, (NAUInt)normedend);      
      
      // Now, iter points at a sparse part containing the desired subrange
      // which can be filled.
      naAllocateBufferPartMemory(part);
//      naFillBufferSourcePart(buffer->source, part);
    }else{
      // When the current part already contains bytes, we do not have to
      // allocate memory. But we might need to fill the memory anew if
      // volatil is NA_TRUE
//      if(volatil){naFillBufferSourcePart(buffer->source, part);}
    }
    
    // Reaching here, the current part is filled with memory. We take as many
    // bytes as we can. If there are enough bytes, we set bytecount to zero.

    NAInt remainingbytes = (NAInt)part->bytesize - (NAInt)iter->partoffset;
    if(remainingbytes > bytecount){
      // There are still bytes left in the current part.
      iter->partoffset += bytecount;
      bytecount = 0;
    }else{
      // The current part is exhausted. Go to the next one.
      bytecount -= remainingbytes;
      naIterateTree(&(iter->partiter));
      iter->partoffset = 0;
    }

    // If there are still bytes required but there is no more part, we enlarge
    // the buffer range at the end.
    if(bytecount && naIsTreeAtInitial(&(iter->partiter))){
      // First, we place the iterator at the currently existing last part.
      naLocateTreeLast(&(iter->partiter));
      
      // Then, we enhance the end of the buffer.
      NAInt normedend = naGetBufferPartNormedEnd(buffer->range.origin + bytecount);
      if(naHasBufferFixedRange(buffer)){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
      naEnsureBufferRange(buffer, naGetRangeiEnd(buffer->range), normedend);
      // Then, we iterate one step and set the partoffset to the beginning
      // of that new part.
      
      naIterateTree(&(iter->partiter));
      iter->partoffset = 0;
      #ifndef NDEBUG
        if(naIsTreeAtInitial(&(iter->partiter)))
          naError("naPrepareBuffer", "No Buffer part available after enlarging range at end");
      #endif
    }
  }

//  // We try to get a source buffer. The following function returns NA_NULL if
//  // this buffer has not a buffer as a source.
//  NABufferIterator * itersrc = naGetBufferSourceBufferIterator(buffer->source);
//  
//  if(itersrc){
//    // In case there is a parent buffer as a source, we prepare that source
//    // buffer furst and then just take references out of it.
//    NAInt srcrangeorigin;
//
//    // First, we make sure that parent buffer is fully prepared.
//    NARangei srcrange = naMakeRangei(range.origin - buffer->srcoffset, range.length);
//    naPrepareBuffer(itersrc, srcrange, volatil, NA_TRUE);
//    // After this function call, the iterator points to the part containing
//    // the start of srcrange.
//
//    // Then, we ensure, that there are buffer parts in the parts list of this
//    // buffer, spanning the desired range by creating sparse buffers wherever
//    // necessary.
//    naEnsureBufferRange(buffer, range);
//    naLocateBufferPartOffset(&(iter->partiter), range.origin);
//
//    // After this function call, the iterator points to the part containing
//    // the start of range.
//
//    // Then, we start referencing all parts of the source.
//    srcrangeorigin = range.origin - buffer->srcoffset;
//
//    // We perform the preparation as long as there are still bytes left in range.
//    while(range.length){
//      NABufferPart* srcpart;
//      NABufferPart* dstpart;
//      NAInt remainingsrcbytes;
//      NAInt remainingdstbytes;
//      NAInt remainingbytes;
//
//      #ifndef NDEBUG
//        if(range.length < 0)
//          naError("naPrepareBuffer", "length is negative");
//      #endif
//
//      // We get the current parts of src and dst.
//      srcpart = naGetListCurMutable(&(itersrc->partiter));
//      dstpart = naGetListCurMutable(&(iter->partiter));
//      // We count, how many bytes are left on both parts.
//      remainingsrcbytes = (naGetBufferPartEnd(srcpart) - srcrangeorigin);
//      remainingdstbytes = (naGetBufferPartEnd(dstpart) - range.origin);
//      // we choose the number of bytes which is smaller.
//      remainingbytes = naMini(remainingsrcbytes, remainingdstbytes);
//      remainingbytes = naMini(remainingbytes, range.length);
//
//      // We decide what to do based on the dst buffer.
//      if(naIsBufferPartSparse(dstpart)){
//        // If the dst buffer is sparse, we split the part fitting the desired
//        // number of bytes.
//        naSplitBufferSparsePart(&(iter->partiter), naMakeRangei(range.origin, remainingbytes));
//        // We need to reload the current dstpart, as it might have changed by
//        // the call to naSplitBufferSparsePart.
//        dstpart = naGetListCurMutable(&(iter->partiter));
//        
//        // We replace the current sparse part with a reference to the
//        // source part.
//        naReferenceBufferPart(dstpart, srcpart, buffer->srcoffset);
//        
//        // Then we try to combine this newly available part with its neighbors.
//        naCombineBufferPartAdjacents(&(iter->partiter));
//
//        // Now, the current dst part is completely filled. Move to the next one.
//        naIterateList(&(iter->partiter));
//        
//      }else{
//        // The current buffer part already contains memory.
//
//        // These bytes can usually be skipped, except if volatil is NA_TRUE and
//        // the source buffers do not match.
//        if(volatil && (naGetBufferPartDataPointerConst(dstpart, range.origin) != naGetBufferPartDataPointerConst(srcpart, srcrangeorigin))){
//          // if the source buffer is volatile, we need to replace the existing
//          // bytes in the desired subrange.
//          NARangei subrange = naMakeRangeiWithStartAndEnd(range.origin, range.origin + remainingbytes);
//          
//          // First, we dismiss the remaining bytes from the dst buffer.
//          naDismissBufferRange(buffer, subrange);
//          // After this call, the iterator is at the new sparse part
//          dstpart = naGetListCurMutable(&(iter->partiter));
//          // This new sparse part might be bigger than anticipated. We split it
//          // again into the size we need
//          naSplitBufferSparsePart(&(iter->partiter), subrange);
//
//          // We replace the current sparse part with a reference to the
//          // source part.
//          naReferenceBufferPart(dstpart, srcpart, buffer->srcoffset);
//          
//          // Then we try to combine this newly available part with its neighbors.
//          naCombineBufferPartAdjacents(&(iter->partiter));
//
//          // Now, the current dst part is completely filled. Move to the next one.
//          naIterateList(&(iter->partiter));
//        }
//        
//        // We can skip ahead the dst buffer if the remaining bytes match.
//        if(remainingbytes == remainingdstbytes){naIterateList(&(iter->partiter));}
//      }
//
//      // If the remainingbytes matches the src bytes, the source part can be
//      // iterated forward.
//      if(remainingbytes == remainingsrcbytes){naIterateList(&(itersrc->partiter));}
//
//      // We adapt the range and source origin.
//      range = naMakeRangeiE(range.origin + remainingbytes, range.length - remainingbytes);
//      srcrangeorigin += remainingbytes;
//
//    }
//    
//  }else{
  
    // In case there is no source or the source does not use a buffer, we fill
    // up our own linear memory. Such memory is generally aligned by
    // NA_INTERNAL_BUFFER_PART_BYTESIZE.
      
//    NAInt normedstart;
//    NAInt normedend;
//        
//    normedstart = naGetBufferPartNormedStart(range.origin);
//    if(naHasBufferFixedRange(buffer)){normedstart = naMaxi(normedstart, buffer->range.origin);}
//    normedend = naGetBufferPartNormedEnd(naGetRangeiEnd(range));
//    if(naHasBufferFixedRange(buffer)){normedend = naMini(normedend, naGetRangeiEnd(buffer->range));}
//    range = naMakeRangeiWithStartAndEnd(normedstart, normedend);
//
//    // First, we ensure, that there are buffer parts in the parts list
//    // spanning the desired normed range by creating sparse buffers if
//    // necessary.
//    naEnsureBufferRange(buffer, range);
//    naLocateBufferPartOffset(&(iter->partiter), range.origin);
//
//    // After this function call, the iterator points to the part containing
//    // the origin of range.
//
//    // Then, we go through the list of parts and change all sparse buffers into
//    // memory when within range.
//
//    // We perform the preparation as long as there are bytes left in range.
//    while(range.length > 0){
//      
//      NABufferPart* part = naGetListCurMutable(&(iter->partiter));






    //      #ifndef NDEBUG
    //        // This should not happen, as range.length shall be smallerequal 0 when
    //        // the list comes to an end.
    //        if(!part)
    //          naError("naPrepareBuffer", "list overflow. This should not happen.");
    //      #endif
    //
    //      if(naIsBufferPartSparse(part)){
    //        // If the current part is sparse, we create buffer parts with actual
    //        // memory.
    //
    //        // We create a suitable range within the parts range which tries to
    //        // be aligned at NA_INTERNAL_BUFFER_PART_BYTESIZE but has a certain
    //        // margin to be bigger and hence reduce the number of small parts.
    //        normedstart = naGetBufferPartNormedStart(range.origin);
    //        if((normedstart - naGetBufferPartStart(part)) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedstart = naGetBufferPartStart(part);}
    //        normedend = naGetBufferPartNormedEnd(range.origin + NA_INTERNAL_BUFFER_PART_BYTESIZE);
    //        if((naGetBufferPartEnd(part) - normedend) < NA_INTERNAL_BUFFER_PART_BYTESIZE){normedend = naGetBufferPartEnd(part);}
    //        
    //        // Note that the previous computation of normedstart and normedend also
    //        // automatically handles parts being smaller than the normed range and
    //        // hence never resulting in a subrange bigger than the range of the
    //        // actual part.
    //        NARangei subrange = naMakeRangeiWithStartAndEnd(normedstart, normedend);
    //        #ifndef NDEBUG
    //          if(!naContainsRangeiRange(naGetBufferPartRange(part), subrange))
    //            naError("naPrepareBuffer", "part range does not contain subrange");
    //        #endif
    //
    //        // Then we split the sparse buffer if necessary.
    //        naSplitBufferSparsePart(&(iter->partiter), subrange);
    //        
    //        // Now, iter points at a sparse part containing subrange
    //        // which can be filled.
    //        part = naGetListCurMutable(&(iter->partiter));
    //        #ifndef NDEBUG
    //          if(!naEqualRangei(subrange, naGetBufferPartRange(part)))
    //            naError("naPrepareBuffer", "range lengths do not match");
    //        #endif
    //        naAllocateBufferPartMemory(part);
    //        naFillBufferSourcePart(buffer->source, part);
    //        
    //      }else{
    //        // When the current part already contains bytes, we do not have to
    //        // allocate memory. But we might need to fill the memory anew if
    //        // volatil is NA_TRUE
    //        if(volatil){naFillBufferSourcePart(buffer->source, part);}
    //      }
    //
    //      // Reaching here, the current part is filled with memory. We advance
    //      // to the next part. Note that the calculation of the new range will
    //      // most probably result in a negative length when reaching the end of
    //      // the desired range, hence breaking the loop condition.
    //
    //      range = naMakeRangeiWithStartAndEnd(naGetBufferPartEnd(part), naGetRangeiEnd(range));
    //      naIterateList(&(iter->partiter));
    //    }
    //  }
    
  if(locatestart){
//    // Reaching here, iter points to the part one after the part containing the
//    // maximum offset of range. We locate the buffer iterator at the first
//    // desired offset. Note that we can not use a cached list iterator as this
//    // might have changed during this preparation.
//    naLocateBufferPartOffset(&(iter->partiter), originalbytecount);
//
//    #ifndef NDEBUG
//      if(!naContainsBufferPartOffset(naGetListCurMutable(&(iter->partiter)), originalbytecount))
//        naError("naPrepareBuffer", "start offset not in range of current part.");
//    #endif
  }
}



//// This function makes the bytes declared in range unavailable by replacing
//// that range with a sparse part. As a consequence, certain buffer parts may
//// not be used anymore and will be automatically deallocated.
//NA_HDEF void naUnlinkBufferRange(NABuffer* buffer, NARangei range){
//  NAInt rangepos;
//  NABufferPart* sparsepart;
//  NAListIterator iter;
//  
//  #ifndef NDEBUG
//    if(naIsBufferEmpty(buffer))
//      naError("naDismissBufferRange", "Buffer ist empty");
//    if(!naContainsRangeiRange(buffer->range, range))
//      naError("naDismissBufferRange", "Range of is not within buffer range");
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
//}



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
  
