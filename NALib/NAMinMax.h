
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// ///////////////////////////
// This file contains NAMinMax structs and functions dealing with them.
// NAMinMax structures store a min and a max as a one- or multiple-
// dimensional value.
//
// The 1- and 2-dimensional structs can be easily converted to NARange and
// NARect structs which are the directly corresponding intuitive structs.
// The intuitive counterparts work fine but sometimes, it is faster and more
// elegant to work with minmax values directly. Often times, they are more
// convenient to program with than with NARange or NARect.
//
// There are a lot of very basic helper functions. NALib uses these internally
// when working with NARange and NARect. Those files are dependent on this
// file.
//
// ----- IMPORTANT -----:
// There exist NAMinMax structures both for floating point as well as for
// integer types. They behave a little differently tough. While a floating-
// point range [start, end] has a size of (end-start), an integer range
// depicted by [min, max] has a size of (max-min+1) units.
//
// Therefore when working with integers, there is a crucial distinction between
// min-max pairs and start-end pairs. Note that the pairs min-max and start-end
// are reflected in the definitions of the different NAMinMax structures. The
// name of the structure itself though is always NAMinMax in order to provide
// a consistent API.
//
// When using the correct conversions, you will not run into problems though.
// If you want do convert manually, do the following: max = end - 1.
//
// Aside from that:
// MinMax functions are not especially difficult to understand or implement but
// take a lot of code writing. You are free to explore all the inline code
// below but there really isn't much to see.
// ///////////////////////////


#ifndef NA_MINMAX_INCLUDED
#define NA_MINMAX_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAVectorAlgebra.h"
#include "NAArray.h"


typedef struct NAMinMax1  NAMinMax1;
typedef struct NAMinMax1f NAMinMax1f;
typedef struct NAMinMax1i NAMinMax1i;
typedef struct NAMinMax2  NAMinMax2;
typedef struct NAMinMax2f NAMinMax2f;
typedef struct NAMinMax2i NAMinMax2i;

// Please excuse this formatting. It is usually not recommended but here, it
// would be much more confusing if written otherwise.
struct NAMinMax1{   NAVec1  start;  NAVec1  end;};
struct NAMinMax1f{  NAVec1f start;  NAVec1f end;};
struct NAMinMax1i{  NAVec1i min;    NAVec1i max;};
struct NAMinMax2{   NAVec2  start;  NAVec2  end;};
struct NAMinMax2f{  NAVec2f start;  NAVec2f end;};
struct NAMinMax2i{  NAVec2i min;    NAVec2i max;};


// Creating structs with values.
NA_IAPI NAMinMax1   naMakeMinMax1                  (double start,  double end);
NA_IAPI NAMinMax1f  naMakeMinMax1f                 (float  start,  float  end);
NA_IAPI NAMinMax1i  naMakeMinMax1iWithStartAndEnd  (NAInt  start,  NAInt  end);
NA_IAPI NAMinMax1i  naMakeMinMax1i                 (NAInt  min,    NAInt  max);
// Filling the given struct pointers with values.
NA_IAPI void        naFillMinMax1                  (NAMinMax1*  minmax, double start,  double end);
NA_IAPI void        naFillMinMax1f                 (NAMinMax1f* minmax, float  start,  float  end);
NA_IAPI void        naFillMinMax1iWithStartAndEnd  (NAMinMax1i* minmax, NAInt  start,  NAInt  end);
NA_IAPI void        naFillMinMax1i                 (NAMinMax1i* minmax, NAInt  min,    NAInt  max);


// Creating the structs with positions and sizes. The usual checks are performed
// but note that in contrast to NARange and NARect, empty sizes are fine.
NA_IAPI NAMinMax1   naMakeMinMax1WithPosAndSize   (double pos, double size);
NA_IAPI NAMinMax1f  naMakeMinMax1fWithPosAndSize  (float  pos, float  size);
NA_IAPI NAMinMax1i  naMakeMinMax1iWithPosAndSize  (NAInt  pos, NAInt  size);
// Filling the given struct pointers with values.
NA_IAPI void        naFillMinMax1WithPosAndSize   (NAMinMax1*  minmax, double pos, double size);
NA_IAPI void        naFillMinMax1fWithPosAndSize  (NAMinMax1f* minmax, float  pos, float  size);
NA_IAPI void        naFillMinMax1iWithPosAndSize  (NAMinMax1i* minmax, NAInt  pos, NAInt  size);


// Tests if the given points is inside the minmax.
NA_IAPI NABool      naInsideMinMax1II (NAMinMax1  minmax, double x);
NA_IAPI NABool      naInsideMinMax1fII(NAMinMax1f minmax, float  x);
NA_IAPI NABool      naInsideMinMax1IE (NAMinMax1  minmax, double x); // Use these if you
NA_IAPI NABool      naInsideMinMax1fIE(NAMinMax1f minmax, float  x); // are uncertain.
NA_IAPI NABool      naInsideMinMax1EI (NAMinMax1  minmax, double x);
NA_IAPI NABool      naInsideMinMax1fEI(NAMinMax1f minmax, float  x);
NA_IAPI NABool      naInsideMinMax1EE (NAMinMax1  minmax, double x);
NA_IAPI NABool      naInsideMinMax1fEE(NAMinMax1f minmax, float  x);
NA_IAPI NABool      naInsideMinMax1i  (NAMinMax1i minmax, NAInt  x);


// Tests if the given minmax is empty.
NA_IAPI NABool      naIsMinMax1Empty (NAMinMax1  minmax);
NA_IAPI NABool      naIsMinMax1fEmpty(NAMinMax1f minmax);
NA_IAPI NABool      naIsMinMax1iEmpty(NAMinMax1i minmax);
NA_IAPI NABool      naIsMinMax2Empty (NAMinMax2  minmax);
NA_IAPI NABool      naIsMinMax2fEmpty(NAMinMax2f minmax);
NA_IAPI NABool      naIsMinMax2iEmpty(NAMinMax2i minmax);


// This function returns an array of non-overlapping minmax's.
// Takes an array of minmax's which are possibly overlapping.
// The result contains minmax's which were not covered by the original array.
// For example entering the following four minmax's
//      |-----|     |-----------|
//  |-----------|   |-------|
//                                   Will result in the following 6 areas:
//  |---|-----|-|---|-------|---|
//
NA_API NAArray*     naInitArrayWithMinMax1iAreasFromMinMax1iArray(
                                                  NAArray* newarray,
                                            const NAArray* minmaxs);



// ///////////////////////////////////
// HELPER FUNCTIONS
// ///////////////////////////////////

// Most functions above use the following helper functions, which are also
// used in other files. You may use the functions for your own code if you
// like but they are mostly written here for completeness and are marked
// as helper functions.

// Converts an integer denoting an end or max to its counterpart. Also does
// some checks if the values over- or underflow.
NA_HIAPI NAInt  naEndToMaxi(NAInt end);
NA_HIAPI NAInt  naMaxToEndi(NAInt max);
NA_HIAPI double naPosAndSizeToEnd   (double  pos,   double size);
NA_HIAPI float  naPosAndSizeToEndf  (float   pos,   float  size);
NA_HIAPI NAInt  naPosAndSizeToEndi  (NAInt   pos,   NAInt  size);
NA_HIAPI NAInt  naPosAndSizeToMaxi  (NAInt   pos,   NAInt  size);
NA_HIAPI double naStartAndEndToSize (double  start, double end);
NA_HIAPI float  naStartAndEndToSizef(float   start, float  end);
NA_HIAPI NAInt  naStartAndEndToSizei(NAInt   start, NAInt  end);
NA_HIAPI NAInt  naMinAndMaxToSizei  (NAInt   min,   NAInt  max);

// This function alters the given pos and size such that size will become
// positive while retaining the depicted range. For example with doubles:
// pos = 3. and size = -2. will become pos = 1. and size = 2. The resulting
// range starts at 1. and ends at 1. + 2. = 3.
// Note: When using integers, the end must be computed with -1. Therefore
// the result differs:
// pos = 3 and size = -2 will become pos = 2 and size = 2. The resulting
// range starts at 2 and ends at (2 + 2) - 1 = 3
// If size already was positive, nothing will be changed.
NA_HIAPI void naMakePositive      (double* NA_RESTRICT pos,
                                   double* NA_RESTRICT size);
NA_HIAPI void naMakePositivei     (NAInt*  NA_RESTRICT pos,
                                   NAInt*  NA_RESTRICT size);
                                 
// This function returns a pair of positive integers (positivepos,positivesize)
// out of a possibly negative pair (pos,size) such that the resulting range
// will be fully contained in a range given by [0, containingsize-1].
// Negative values are treated as follows and in the following order:
// - if pos is negative, it denotes the number of units from the end.
//   For integers, pos -1 therefore corresponds to size-1.
// - If the size is now 0, the function will return.
// - if size is negative, it denotes the size up and including to the given
//   number of units from the end, meaning -1 denotes the last unit.
// - if the pos and size combination somehow leads to a size of exactly 0,
//   the resulting range will be empty without a warning emitted.
// - If the pos and size combination somehow leads to an over- or underflow,
//   a warning will be emitted if NDEBUG is defined. The resulting range will
//   be empty.
NA_HAPI void naMakePositiveiInSize(  NAUInt* NA_RESTRICT positivepos,
                                     NAUInt* NA_RESTRICT positivesize,
                                     NAInt               pos,
                                     NAInt               size,
                                     NAUInt              containingsize);

// The following functions are mostly used in other datastructures such as
// NARect and NARange. They define the default semantics against which values
// are checked in NALib when debugging. For example, a size is defined to be
// not useful when negative.

// VALID means: Anything but NaN. Integer values therefore are always valid.
NA_HIAPI NABool naIsPosValueValid(double a);
NA_HIAPI NABool naIsPosValueValidf(float a);
NA_HIAPI NABool naIsPosValueValidi(NAInt a);
NA_HIAPI NABool naIsSizeValueValid(double a);
NA_HIAPI NABool naIsSizeValueValidf(float a);
NA_HIAPI NABool naIsSizeValueValidi(NAInt a);

// EMPTY means: Precisely Zero.
NA_HIAPI NABool naIsSizeValueEmpty(double a);
NA_HIAPI NABool naIsSizeValueEmptyf(float a);
NA_HIAPI NABool naIsSizeValueEmptyi(NAInt a);

// NEGATIVE means: Smaller than Zero.
//
// Note from the author: These functions test for Negativity, not Positivity.
// Using a negative word in a function name is usually not good practice as it
// tends to lead to double negatives when reading the source code. There are
// situations though when using a negative word is acceptable or even better
// than its positive counterpart. Like here for example. Or the NDEBUG macro.
// Or the "End of file" marker EOF for example. But such situations are rare.
// Try to use positive words whenever possible! For example, test for "Inside"
// rather than "Outside".
NA_HIAPI NABool naIsSizeValueNegative(double a);
NA_HIAPI NABool naIsSizeValueNegativef(float a);
NA_HIAPI NABool naIsSizeValueNegativei(NAInt a);

// USEFUL means: Positions must be valid. Sizes must be valid, not empty and
// not negative.
NA_HIAPI NABool naIsPosValueUseful(double a);
NA_HIAPI NABool naIsPosValueUsefulf(float a);
NA_HIAPI NABool naIsPosValueUsefuli(NAInt a);
NA_HIAPI NABool naIsSizeValueUseful(double a);
NA_HIAPI NABool naIsSizeValueUsefulf(float a);
NA_HIAPI NABool naIsSizeValueUsefuli(NAInt a);










// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAHeap.h"


NA_IAPI NAMinMax1 naMakeMinMax1(double start, double end){
  NAMinMax1 newminmax;
  #ifndef NDEBUG
    if(end < start)
      naError("naMakeMinMax1", "end is smaller than start");
  #endif
  newminmax.start[0] = start;
  newminmax.end[0] = end;
  return newminmax;
}
NA_IAPI NAMinMax1f naMakeMinMax1f(float start, float end){
  NAMinMax1f newminmax;
  #ifndef NDEBUG
    if(end < start)
      naError("naMakeMinMax1f", "end is smaller than start");
  #endif
  newminmax.start[0] = start;
  newminmax.end[0] = end;
  return newminmax;
}
NA_IAPI NAMinMax1i naMakeMinMax1iWithStartAndEnd(NAInt start, NAInt end){
  NAMinMax1i newminmax; // Declaration before Definition. Needed for C90
  NAInt newmax = naEndToMaxi(end);
  #ifndef NDEBUG
    if(newmax < start)
      naError("naMakeMinMax1iWithMinAndEnd", "max is smaller than start");
  #endif
  newminmax.min[0] = start;
  newminmax.max[0] = newmax;
  return newminmax;
}
NA_IAPI NAMinMax1i naMakeMinMax1i(NAInt min, NAInt max){
  NAMinMax1i newminmax;
  #ifndef NDEBUG
    if(max < min)
      naError("naMakeMinMax1i", "max is smaller than min");
  #endif
  newminmax.min[0] = min;
  newminmax.max[0] = max;
  return newminmax;
}



NA_IAPI void naFillMinMax1(NAMinMax1* minmax, double start, double end){
  #ifndef NDEBUG
    if(end < start)
      naError("naFillMinMax1", "end is smaller than start");
  #endif
  minmax->start[0] = start;
  minmax->end[0] = end;
}
NA_IAPI void naFillMinMax1f(NAMinMax1f* minmax, float start, float end){
  #ifndef NDEBUG
    if(end < start)
      naError("naFillMinMax1f", "end is smaller than start");
  #endif
  minmax->start[0] = start;
  minmax->end[0] = end;
}
NA_IAPI void naFillMinMax1iWithStartAndEnd(NAMinMax1i* minmax, NAInt start, NAInt end){
  NAInt newmax = naEndToMaxi(end);
  #ifndef NDEBUG
    if(newmax < start)
      naError("naFillMinMax1iWithStartAndEnd", "max is smaller than start");
  #endif
  minmax->min[0] = start;
  minmax->max[0] = newmax;
}
NA_IAPI void naFillMinMax1i(NAMinMax1i* minmax, NAInt min, NAInt max){
  #ifndef NDEBUG
    if(max < min)
      naError("naFillMinMax1i", "max is smaller than min");
  #endif
  minmax->min[0] = min;
  minmax->max[0] = max;
}



NA_IAPI NAMinMax1 naMakeMinMax1WithPosAndSize(double pos, double size){
  NAMinMax1 newminmax; // Declaration before Definition. Needed for C90
  double newend = naPosAndSizeToEnd(pos, size);
  #ifndef NDEBUG
    if(!naIsPosValueValid(pos))
      naError("naMakeMinMax1WithPosAndSize", "pos not valid");
    if(!naIsSizeValueValid(size))
      naError("naMakeMinMax1WithPosAndSize", "size not valid");
    if(naIsSizeValueNegative(size))
      naError("naMakeMinMax1WithPosAndSize", "size is negative");
    if(newend < pos)
      naError("naMakeMinMax1WithPosAndSize", "max is smaller than min");
  #endif
  newminmax.start[0] = pos;
  newminmax.end[0] = newend;
  return newminmax;
}
NA_IAPI NAMinMax1f naMakeMinMax1fWithPosAndSize(float pos, float size){
  NAMinMax1f newminmax; // Declaration before Definition. Needed for C90
  float newend = naPosAndSizeToEndf(pos, size);
  #ifndef NDEBUG
    if(!naIsPosValueValidf(pos))
      naError("naMakeMinMax1fWithPosAndSize", "pos not valid");
    if(!naIsSizeValueValidf(size))
      naError("naMakeMinMax1fWithPosAndSize", "size not valid");
    if(naIsSizeValueNegativef(size))
      naError("naMakeMinMax1fWithPosAndSize", "size is negative");
    if(newend < pos)
      naError("naMakeMinMax1fWithPosAndSize", "max is smaller than min");
  #endif
  newminmax.start[0] = pos;
  newminmax.end[0] = newend;
  return newminmax;
}
NA_IAPI NAMinMax1i naMakeMinMax1iWithPosAndSize(NAInt pos, NAInt size){
  NAMinMax1i newminmax; // Declaration before Definition. Needed for C90
  NAInt newmax = naPosAndSizeToMaxi(pos, size);
  #ifndef NDEBUG
    if(!naIsPosValueValidi(pos))
      naError("naMakeMinMax1iWithPosAndSize", "pos not valid");
    if(!naIsSizeValueValidi(size))
      naError("naMakeMinMax1iWithPosAndSize", "size not valid");
    if(naIsSizeValueNegativei(size))
      naError("naMakeMinMax1iWithPosAndSize", "size is negative");
    if(newmax < pos)
      naError("naMakeMinMax1iWithPosAndSize", "max is smaller than min");
  #endif
  newminmax.min[0] = pos;
  newminmax.max[0] = newmax;
  return newminmax;
}



NA_IAPI void naFillMinMax1WithPosAndSize(NAMinMax1* minmax, double pos, double size){
  double newend = naPosAndSizeToEnd(pos, size);
  #ifndef NDEBUG
    if(!naIsPosValueValid(pos))
      naError("naFillMinMax1WithPosAndSize", "pos not valid");
    if(!naIsSizeValueValid(size))
      naError("naFillMinMax1WithPosAndSize", "size not valid");
    if(naIsSizeValueNegative(size))
      naError("naFillMinMax1WithPosAndSize", "size is negative");
    if(newend < pos)
      naError("naFillMinMax1WithPosAndSize", "max is smaller than min");
  #endif
  minmax->start[0] = pos;
  minmax->end[0] = newend;
}
NA_IAPI void naFillMinMax1fWithPosAndSize(NAMinMax1f* minmax, float pos, float size){
  float newend = naPosAndSizeToEndf(pos, size);
  #ifndef NDEBUG
    if(!naIsPosValueValidf(pos))
      naError("naFillMinMax1fWithPosAndSize", "pos not valid");
    if(!naIsSizeValueValidf(size))
      naError("naFillMinMax1fWithPosAndSize", "size not valid");
    if(naIsSizeValueNegativef(size))
      naError("naFillMinMax1fWithPosAndSize", "size is negative");
    if(newend < pos)
      naError("naFillMinMax1fWithPosAndSize", "max is smaller than min");
  #endif
  minmax->start[0] = pos;
  minmax->end[0] = newend;
}
NA_IAPI void naFillMinMax1iWithPosAndSize(NAMinMax1i* minmax, NAInt pos, NAInt size){
  NAInt newmax = naPosAndSizeToMaxi(pos, size);
  #ifndef NDEBUG
    if(!naIsPosValueValidi(pos))
      naError("naFillMinMax1iWithPosAndSize", "pos not valid");
    if(!naIsSizeValueValidi(size))
      naError("naFillMinMax1iWithPosAndSize", "size not valid");
    if(naIsSizeValueNegativei(size))
      naError("naFillMinMax1iWithPosAndSize", "size is negative");
    if(newmax < pos)
      naError("naFillMinMax1iWithPosAndSize", "max is smaller than min");
  #endif
  minmax->min[0] = pos;
  minmax->max[0] = newmax;
}




NA_IDEF NABool naInsideMinMax1II(NAMinMax1 minmax, double x){
  return naInsideII(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1fII(NAMinMax1f minmax, float x){
  return naInsidefII(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1IE(NAMinMax1 minmax, double x){
  return naInsideIE(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1fIE(NAMinMax1f minmax, float x){
  return naInsidefIE(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1EI(NAMinMax1 minmax, double x){
  return naInsideEI(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1fEI(NAMinMax1f minmax, float x){
  return naInsidefEI(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1EE(NAMinMax1 minmax, double x){
  return naInsideEE(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1fEE(NAMinMax1f minmax, float x){
  return naInsidefEE(minmax.start[0], minmax.end[0], x);
}
NA_IDEF NABool naInsideMinMax1i(NAMinMax1i minmax, NAInt x){
  return naInsidei(minmax.min[0], minmax.max[0], x);
}


NA_IDEF NABool naIsMinMax1Empty (NAMinMax1  minmax){
  return (minmax.start[0] == minmax.end[0]);
}
NA_IDEF NABool naIsMinMax1fEmpty(NAMinMax1f minmax){
  return (minmax.start[0] == minmax.end[0]);
}
NA_IDEF NABool naIsMinMax1iEmpty(NAMinMax1i minmax){
  return (minmax.min[0] == naMaxToEndi(minmax.max[0]));
}
NA_IDEF NABool naIsMinMax2Empty(NAMinMax2 minmax){
  return ((minmax.start[0] == minmax.end[0]) || (minmax.start[1] == minmax.end[1]));
}
NA_IDEF NABool naIsMinMax2fEmpty(NAMinMax2f minmax){
  return ((minmax.start[0] == minmax.end[0]) || (minmax.start[1] == minmax.end[1]));
}
NA_IDEF NABool naIsMinMax2iEmpty(NAMinMax2i minmax){
  return ((minmax.min[0] == naMaxToEndi(minmax.max[0])) || (minmax.min[1] == naMaxToEndi(minmax.max[1])));
}









NA_HIDEF NAInt naEndToMaxi(NAInt end){
  #ifndef NDEBUG
    if(end == NA_INT_MIN)
      naError("naEndToMaxi", "Integer underflow");
  #endif
  return end - 1;
}
NA_HIDEF NAInt naMaxToEndi(NAInt max){
  #ifndef NDEBUG
    if(max == NA_INT_MAX)
      naError("naMaxToEndi", "Integer overflow");
  #endif
  return max + 1;
}
NA_HIDEF double naPosAndSizeToEnd(double pos, double size){
  double result = pos + size;
  #ifndef NDEBUG
    if(!naIsPosValueValid(result)){
      naError("naPosAndSizeToEnd", "result invalid");
    }
  #endif
  return result;
}
NA_HIDEF float naPosAndSizeToEndf(float pos, float size){
  float result = pos + size;
  #ifndef NDEBUG
    if(!naIsPosValueValidf(result)){
      naError("naPosAndSizeToEndf", "result invalid");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naPosAndSizeToEndi(NAInt pos, NAInt size){
  NAInt result = pos + size;
  #ifndef NDEBUG
    if(size > 0){
      if(result < pos)
        naError("naPosAndSizeToEndi", "Integer overflow");
    }else{
      if(result > pos)
        naError("naPosAndSizeToEndi", "Integer underflow");
    }
  #endif
  return result;
}
NA_HIDEF NAInt naPosAndSizeToMaxi(NAInt pos, NAInt size){
  NAInt result = naEndToMaxi(pos + size);
  #ifndef NDEBUG
    if(size > 0){
      if(result < pos)
        naError("naPosAndSizeToMaxi", "Integer overflow");
    }else{
      if(result > pos)
        naError("naPosAndSizeToMaxi", "Integer underflow");
    }
  #endif
  return result;
}


NA_HIDEF double naStartAndEndToSize(double min, double end){
  return end - min;
}
NA_HIDEF float naStartAndEndToSizef(float min, float end){
  return end - min;
}
NA_HIDEF NAInt  naStartAndEndToSizei(NAInt min, NAInt end){
  return end - min;
}
NA_HIDEF NAInt  naMinAndMaxToSizei(NAInt min, NAInt max){
  return naMaxToEndi(max) - min;
}


NA_HIDEF void naMakePositive(double* NA_RESTRICT pos, double* NA_RESTRICT size){
  if(*size < 0.){
    *pos = *pos + *size;
    *size = -*size;
  }
}


NA_HIDEF void naMakePositivei(NAInt* NA_RESTRICT pos, NAInt* NA_RESTRICT size){
  if(*size < 0){
    *pos = *pos + *size + 1; // important + 1 !
    *size = -*size;
  }
}


NA_HIDEF NABool naIsPosValueValid(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsPosValueValidf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsPosValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_HIDEF NABool naIsSizeValueValid(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsSizeValueValidf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsSizeValueValidi(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}

NA_HIDEF NABool naIsSizeValueEmpty(double a){
  return (a == 0.);
}
NA_HIDEF NABool naIsSizeValueEmptyf(float a){
  return (a == 0.f);
}
NA_HIDEF NABool naIsSizeValueEmptyi(NAInt a){
  return (a == 0);
}


NA_HIDEF NABool naIsSizeValueNegative(double a){
  return (a < 0.);
}
NA_HIDEF NABool naIsSizeValueNegativef(float a){
  return (a < 0.f);
}
NA_HIDEF NABool naIsSizeValueNegativei(NAInt a){
  return (a < 0);
}


NA_HIDEF NABool naIsPosValueUseful(double a){
  return !naIsNaN(a);
}
NA_HIDEF NABool naIsPosValueUsefulf(float a){
  return !naIsNaNf(a);
}
NA_HIDEF NABool naIsPosValueUsefuli(NAInt a){
  NA_UNUSED(a);
  return NA_TRUE;
}


NA_HIDEF NABool naIsSizeValueUseful(double a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_HIDEF NABool naIsSizeValueUsefulf(float a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_HIDEF NABool naIsSizeValueUsefuli(NAInt a){
  return (a > 0);
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
