
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include "NAMinMax.h"
#include "NAGrowingSpace.h"


NA_DEF NAArray* naInitArrayWithMinMax1iAreasFromMinMax1iArray(NAArray* array, const NAArray* minmaxs){
  NAGrowingSpace* segments;
  NAInt curmin;
  NAInt curmax;
  NAUInt minmaxcount;
  NAInt maxareasperdimension;
  NAHeap* rangeheap0min;
  NAHeap* rangeheap0max;
  NAUInt m;

  #ifndef NDEBUG
    if(!array)
      {naCrash("", "array is NULL"); return NA_NULL;}
  #endif

  minmaxcount = naGetArrayCount(minmaxs);
  maxareasperdimension = (2 * minmaxcount - 1);
  rangeheap0min = naInitHeap(NA_NULL, maxareasperdimension, NA_HEAP_USES_INT_KEY);
  rangeheap0max = naInitHeap(NA_NULL, maxareasperdimension, NA_HEAP_USES_INT_KEY);
  // insert the min and max coordinates into the heaps
  for(m=0; m<minmaxcount; m++){
    NAMinMax1i* minmax = (NAMinMax1i*)naGetArrayConstElement(minmaxs, m);
//    if(naIsMinMax1iEmpty(*minmax)){continue;}
    naInsertHeapElementMutable(rangeheap0min, minmax, &(minmax->min[0]), NA_NULL);
    naInsertHeapElementMutable(rangeheap0max, minmax, &(minmax->max[0]), NA_NULL);
  }
  
  if(naGetHeapCount(rangeheap0min) == 0){
    array = naInitArray(array);
  }else{
    segments = naInitGrowingSpace(NA_NULL, sizeof(NAMinMax1i), NA_NULL);
    
    curmin = ((NAMinMax1i*)naRemoveHeapRootMutable(rangeheap0min))->min[0];
    curmax = ((NAMinMax1i*)naRemoveHeapRootMutable(rangeheap0max))->max[0];
    while(NA_TRUE){
      // Remove all mins which are equal to the current min.
      while(naGetHeapCount(rangeheap0min) && (curmin == ((NAMinMax1i*)naGetHeapRootMutable(rangeheap0min))->min[0])){
        naRemoveHeapRootMutable(rangeheap0min);
      }
      
      if(naGetHeapCount(rangeheap0min)){
        NAInt nextmin = ((NAMinMax1i*)naGetHeapRootMutable(rangeheap0min))->min[0];
        if(nextmin <= curmax){
          // Create a segment with the current min and one minus the next min
          NAMinMax1i* newsegment = naNewGrowingSpaceElement(segments);
          newsegment->min[0] = curmin;
          newsegment->max[0] = nextmin - 1;
          // Fetch the next min.
          curmin = ((NAMinMax1i*)naRemoveHeapRootMutable(rangeheap0min))->min[0];
        }else{
          // Create a segment with the current min and the current max
          NAMinMax1i* newsegment = naNewGrowingSpaceElement(segments);
          newsegment->min[0] = curmin;
          newsegment->max[0] = curmax;
          #ifndef NDEBUG
            if(naGetHeapCount(rangeheap0max) == 0)
              naError("naInitArrayWithMinMax1iAreasFromMinMax1iArray", "No more maxs while having mins. This should not happen.");
          #endif
          // Remove all maxs which are equal to the current max
          while(curmax == ((NAMinMax1i*)naGetHeapRootMutable(rangeheap0max))->max[0]){
            naRemoveHeapRootMutable(rangeheap0max);
          }
          // Setup the next min and fetch the new max.
          curmin = curmax + 1;
          curmax = ((NAMinMax1i*)naRemoveHeapRootMutable(rangeheap0max))->max[0];
        }
      }else{
        // Create a segment with the current min and the current max
        NAMinMax1i* newsegment = naNewGrowingSpaceElement(segments);
        newsegment->min[0] = curmin;
        newsegment->max[0] = curmax;
        // Remove all maxs which are equal to the current max
        while((naGetHeapCount(rangeheap0max)) && (curmax == ((NAMinMax1i*)naGetHeapRootMutable(rangeheap0max))->max[0])){
          naRemoveHeapRootMutable(rangeheap0max);
        }
        // If the max-heap is empty, everyhting is done.
        if(naGetHeapCount(rangeheap0max) == 0){break;}
        curmin = curmax + 1;
        curmax = ((NAMinMax1i*)naRemoveHeapRootMutable(rangeheap0max))->max[0];
      }
    }

    array = naInitArrayWithGrowingSpace(array, segments);
    naClearGrowingSpace(segments, NA_NULL);
    naFree(segments);
  }
  
  naClearHeap(rangeheap0min);
  naClearHeap(rangeheap0max);
  naFree(rangeheap0min);
  naFree(rangeheap0max);
  
  return array;
}




NA_HDEF void naMakePositiveiInSize(NAUInt* NA_RESTRICT positivepos, NAUInt* NA_RESTRICT positivesize, NAInt pos, NAInt size, NAUInt containingsize){
  // First, we ensure that pos is withing the containing range. After that
  // we will look at the size parameter.
  NAInt remainingsize = containingsize - pos;
  if(pos < 0){
    pos += containingsize;
    remainingsize -= containingsize;
  }
  if(remainingsize < 0){
    #ifndef NDEBUG
      naError("naMakePositiveiInSize", "Invalid pos leads to range overflow. Correcting to empty range.");
    #endif
    *positivepos = 0;
    *positivesize = 0;
  }else if((NAUInt)remainingsize > containingsize){
    #ifndef NDEBUG
      naError("naMakePositiveiInSize", "Invalid pos leads to range underflow. Correcting to empty range.");
    #endif
    *positivepos = 0;
    *positivesize = 0;
  }else{
    *positivepos = pos;
    // The pos is positive. Now, adjust the size.
    if(size < 0){ // negative size parameter
      size = remainingsize + size + 1;  // Important + 1 !
      if(size < 0){
        // When the resulting size is smaller than 0, underflow.
        #ifndef NDEBUG
          naError("naMakePositiveiInSize", "Invalid size leads to range underflow. Correcting to empty range.");
        #endif
        *positivepos = 0;
        *positivesize = 0;
      }else{
        *positivesize = size;
      }
    }else{ // positive or 0 size parameter
      if(size > remainingsize){
        // When the desired size is bigger than the size available, overflow.
        #ifndef NDEBUG
          naError("naMakePositiveiInSize", "Invalid size leads to range overflow. Correcting to empty range.");
        #endif
        *positivepos = 0;
        *positivesize = 0;
      }else{
        *positivesize = size;
      }
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
