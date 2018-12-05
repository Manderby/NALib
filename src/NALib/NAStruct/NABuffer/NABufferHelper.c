
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
  NAInt length = end - start;

  #ifndef NDEBUG
    if(length <= 0)
      naError("naEnsureBufferRange", "Range length shall be >= 0");
    if(naHasBufferFixedRange(buffer) && (start < buffer->range.origin))
      naError("naEnsureBufferRange", "Range of buffer is fixed but trying to access range below");
    if(naHasBufferFixedRange(buffer) && (end > naGetRangeiEnd(buffer->range)))
      naError("naEnsureBufferRange", "Range of buffer is fixed but trying to access range above");
  #endif

  if(naIsBufferEmpty(buffer)){
    // If the buffer is empty, we just create one sparse part containing the
    // whole range.
    part = naNewBufferPartSparse(buffer->enhancesource, naMakeRangei(start + buffer->enhancesourceoffset, length));
    naAddTreeFirstMutable(&(buffer->parts), part);
    buffer->range = naMakeRangeiWithStartAndEnd(start, end);

  }else{

    // First, we test if we need to add a sparse part at the beginning.
    if(start < buffer->range.origin){
      NAInt additionalbytes = buffer->range.origin - start;
      part = naGetTreeFirstMutable(&(buffer->parts));
      if(naIsBufferPartSparse(part)){
        // If the first part of this list is already sparse, we simply extend
        // its range.
        part->bytesize += additionalbytes;
      }else{
        // We create a sparse part at the beginning.
        part = naNewBufferPartSparse(buffer->enhancesource, naMakeRangei(start + buffer->enhancesourceoffset, additionalbytes));
        naAddTreeFirstMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(start, naGetRangeiEnd(buffer->range));
    }

    // Then, we test if we need to add a sparse part at the end.
    if(end > naGetRangeiEnd(buffer->range)){
      NAInt additionalbytes = end - naGetRangeiEnd(buffer->range);
      part = naGetTreeLastMutable(&(buffer->parts));
      if(naIsBufferPartSparse(part)){
        // If the last part of this list is already sparse, we simply extend
        // its range.
        part->bytesize += additionalbytes;
      }else{
        // We create a sparse part at the end.
        part = naNewBufferPartSparse(buffer->enhancesource, naMakeRangei(naGetRangeiEnd(buffer->range) + buffer->enhancesourceoffset, additionalbytes));
        naAddTreeLastMutable(&(buffer->parts), part);
      }
      buffer->range = naMakeRangeiWithStartAndEnd(buffer->range.origin, end);
    }
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

