
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// ///////////////////////////
// This file contains many functions helping the programmer to work with
// comparing values and ranges. In NALib, ranges are simply defined by a
// start value and an end value and are considered to be one-dimensional.
//
// Most of the functions here could be written as macros. But in NALib, they
// are explicitely implemented as inline functions. This helps when debugging
// and reduces unwanted implicit arithmetic conversions.
//
// Range functions are not especially difficult to understand or implement but
// take a lot of code writing. You are free to explore all the implementation
// but there really isn't much to see. Seriously.
// ///////////////////////////


#ifndef NA_MINMAX_INCLUDED
#define NA_MINMAX_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// ///////////////////////////////////
// MinMax Ranges
// ///////////////////////////////////

#include "NAVectorAlgebra.h"

// MinMax ranges are the same as normal ranges, but stored differently. Often
// times, this is more convenient to program with than with ranges. NALib
// uses MinMax ranges often internally.
//typedef struct NAMinMax1  NAMinMax1;
//typedef struct NAMinMax1f NAMinMax1f;
typedef struct NAMinMax1i NAMinMax1i;
//struct NAMinMax1{
//  NAVec1 min;
//  NAVec1 max;
//};
//struct NAMinMax1f{
//  NAVec1f min;
//  NAVec1f max;
//};
struct NAMinMax1i{
  NAVec1i min;
  NAVec1i max;
};

//typedef struct NAMinMax2  NAMinMax2;
//typedef struct NAMinMax2f NAMinMax2f;
typedef struct NAMinMax2i NAMinMax2i;
//struct NAMinMax2{
//  NAVec2 min;
//  NAVec2 max;
//};
//struct NAMinMax2f{
//  NAVec2f min;
//  NAVec2f max;
//};
struct NAMinMax2i{
  NAVec2i min;
  NAVec2i max;
};

// Filling the structs with values. Note that the integer functions also
// provide a function to work with end instead of max. end = max + 1
//NA_IAPI NAMinMax1  naMakeMinMax1WithPosAndSize( double pos, double size);
//NA_IAPI NAMinMax1f naMakeMinMax1fWithPosAndSize(float  pos, float  size);
NA_IAPI NAMinMax1i naMakeMinMax1iWithPosAndSize(NAInt  pos, NAInt  size);
//NA_IAPI NAMinMax1  naMakeMinMax1( double min, double max);
//NA_IAPI NAMinMax1f naMakeMinMax1f(float  min, float  max);
//NA_IAPI NAMinMax1i naMakeMinMax1i(NAInt  min, NAInt  max);
//NA_IAPI NAMinMax1i naMakeMinMax1iWithMinAndEnd(NAInt  min, NAInt  end);

NA_IAPI NAMinMax2i naMakeMinMax2iFromRecti(NARecti rect);

NA_IAPI NABool naIsInsideMinMax1i(NAInt x, NAMinMax1i minmax);

NA_IAPI NABool naIsMinMax1iEmpty(NAMinMax1i minmax);
NA_IAPI NABool naIsMinMax2iEmpty(NAMinMax2i minmax);

NA_IAPI NAArray* naCreateMinMax1iAreasFromMinMax1i(NAInt numhabitants, ...);











// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAGrowingSpace.h"


NA_IAPI NAMinMax1i naMakeMinMax1iWithPosAndSize(NAInt pos, NAInt size){
  NAMinMax1i newminmax;
  newminmax.min[0] = pos;
  newminmax.max[0] = pos + size - 1;
  return newminmax;
}


NA_IDEF NAMinMax2i naMakeMinMax2iFromRecti(NARecti rect){
  NAMinMax2i newminmax;
  newminmax.min[0] = rect.pos.x;
  newminmax.max[0] = naGetRectiMaxX(rect);
  newminmax.min[1] = rect.pos.y;
  newminmax.max[1] = naGetRectiMaxY(rect);
  return newminmax;
}

NA_IDEF NARecti naMakeRectiFromMinMax1i(NAMinMax1i minmax0, NAMinMax1i minmax1){
  NARecti newrect;
  newrect.pos.x = minmax0.min[0];
  newrect.pos.y = minmax1.min[0];
  newrect.size.width  = minmax0.max[0] - minmax0.min[0] + 1;
  newrect.size.height = minmax1.max[0] - minmax1.min[0] + 1;
  return newrect;
}

NA_IDEF NABool naIsInsideMinMax1i(NAInt x, NAMinMax1i minmax){
  return ((x >= minmax.min[0]) && (x <= minmax.max[0]));
}


NA_IDEF NABool naIsMinMax1iEmpty(NAMinMax1i minmax){
  return ((minmax.min[0] == minmax.max[0] + 1));
}
NA_IDEF NABool naIsMinMax2iEmpty(NAMinMax2i minmax){
  return ((minmax.min[0] == minmax.max[0] + 1) || (minmax.min[1] == minmax.max[1] + 1));
}


NA_IDEF NAArray* naCreateAreasWithMinMax1iFromMinMax1iArray(NAArray* newarray, const NAArray* minmaxs){

  NAInt minmaxcount = naGetArrayCount(minmaxs);
  NAInt maxareasperdimension = (2 * minmaxcount - 1);
  NAHeap* rangeheap0min = naCreateHeap(NA_NULL, maxareasperdimension, NA_HEAP_USES_INT_KEY);
  NAHeap* rangeheap0max = naCreateHeap(NA_NULL, maxareasperdimension, NA_HEAP_USES_INT_KEY);

  // insert the min and max coordinates into the heaps
  for(NAInt m=0; m<minmaxcount; m++){
    NAMinMax1i* minmax = (NAMinMax1i*)naGetArrayConstElement(minmaxs, m);
//    if(naIsMinMax1iEmpty(*minmax)){continue;}
    naInsertHeapElement(rangeheap0min, minmax, &(minmax->min[0]), NA_NULL);
    naInsertHeapElement(rangeheap0max, minmax, &(minmax->max[0]), NA_NULL);
  }
  
  if(naIsHeapEmpty(rangeheap0min)){
    newarray = naCreateArray(newarray);
    return newarray;
  }
  NAGrowingSpace* segments = naCreateGrowingSpace(NA_NULL, sizeof(NAMinMax1i), maxareasperdimension);
  
  NAInt curmin = ((NAMinMax1i*)naRemoveHeapRoot(rangeheap0min))->min[0];
  NAInt curmax = ((NAMinMax1i*)naRemoveHeapRoot(rangeheap0max))->max[0];
  while(NA_TRUE){
    // Remove all mins which are equal to the current min.
    while(!naIsHeapEmpty(rangeheap0min) && (curmin == ((NAMinMax1i*)naGetHeapRoot(rangeheap0min))->min[0])){
      naRemoveHeapRoot(rangeheap0min);
    }
    
    if(!naIsHeapEmpty(rangeheap0min)){
      NAInt nextmin = ((NAMinMax1i*)naGetHeapRoot(rangeheap0min))->min[0];
      if(nextmin <= curmax){
        // Create a segment with the current min and one minus the next min
        NAMinMax1i newsegment;
        newsegment.min[0] = curmin;
        newsegment.max[0] = nextmin - 1;
        naAddGrowingSpaceElement(segments, &newsegment);
        // Fetch the next min.
        curmin = ((NAMinMax1i*)naRemoveHeapRoot(rangeheap0min))->min[0];
      }else{
        // Create a segment with the current min and the current max
        NAMinMax1i newsegment;
        newsegment.min[0] = curmin;
        newsegment.max[0] = curmax;
        naAddGrowingSpaceElement(segments, &newsegment);
        #ifndef NDEBUG
          if(naIsHeapEmpty(rangeheap0max))
            naError("naCreateAreasWithMinMax1iFromMinMax1iArray", "No more maxs while having mins. This should not happen.");
        #endif
        // Remove all maxs which are equal to the current max
        while(curmax == ((NAMinMax1i*)naGetHeapRoot(rangeheap0max))->max[0]){
          naRemoveHeapRoot(rangeheap0max);
        }
        // Setup the next min and fetch the new max.
        curmin = curmax + 1;
        curmax = ((NAMinMax1i*)naRemoveHeapRoot(rangeheap0max))->max[0];
      }
    }else{
      // Create a segment with the current min and the current max
      NAMinMax1i newsegment;
        newsegment.min[0] = curmin;
        newsegment.max[0] = curmax;
      naAddGrowingSpaceElement(segments, &newsegment);
      // Remove all maxs which are equal to the current max
      while((!naIsHeapEmpty(rangeheap0max)) && (curmax == ((NAMinMax1i*)naGetHeapRoot(rangeheap0max))->max[0])){
        naRemoveHeapRoot(rangeheap0max);
      }
      // If the max-heap is empty, everyhting is done.
      if(naIsHeapEmpty(rangeheap0max)){break;}
      curmin = curmax + 1;
      curmax = ((NAMinMax1i*)naRemoveHeapRoot(rangeheap0max))->max[0];
    }
  }

  newarray = naCreateArrayOutOfGrowingSpace(newarray, segments, NA_FALSE);
  naDestroyGrowingSpace(segments, NA_NULL);
  
  return newarray;
}




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_MINMAX_INCLUDED


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
