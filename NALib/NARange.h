
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// ///////////////////////////
// This file contains many functions helping the programmer to work with
// one-dimensional ranges. The datastructure NARange holds an offset and a size
// for one dimension. For simplification, offset is called pos.
//
// Ranges are an intuitive way to handle one-dimensional spans. The two-
// dimensional complement would be NARect. If you rather store min and max
// values, look for NAMinMax1 and NAMinMax2. If you want to use ranges in
// a different way, look for the basic range functions in NAMathOperators.
//
// Most of the functions here could be written as macros. But in NALib, they
// are explicitely implemented as inline functions. This helps when debugging
// and reduces unwanted implicit arithmetic conversions.
//
// Range functions are not especially difficult to understand or implement but
// take a lot of code writing. You are free to explore all the inline code
// below but there really isn't much to see.
// ///////////////////////////


#ifndef NA_RANGE_INCLUDED
#define NA_RANGE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAMinMax.h"


// These are the definitions of the datastructure:
typedef struct NARange  NARange;
typedef struct NARangef NARangef;
typedef struct NARangei NARangei;

// Please excuse this formatting. It is usually not recommended but here, it
// would be much more confusing if written otherwise.
struct NARange{   double pos; double size;};
struct NARangef{  float  pos; float  size;};
struct NARangei{  NAInt  pos; NAInt  size;};


// Note that when working with integers, there is a crucial distinction between
// min-max pairs and start-end pairs.
//
// Min-Max is the discrete definition of a range [min, max]. The start-end
// pair on the other hand defines a range of [start, end-1]. This has proven
// useful in certain cases but if you are unsure about what to use, use the
// min-max pairs.
//
// You could use the start-end pair for example in a for-loop:
// for(i = start; i < end; i++){}


// Creating the structs with values.
NA_IAPI NARange  naMakeRange  (double pos, double size);
NA_IAPI NARangef naMakeRangef (float  pos, float  size);
NA_IAPI NARangei naMakeRangei (NAInt  pos, NAInt  size);
// Filling the given struct pointers with values
NA_IAPI void     naFillRange  (NARange*  range, double pos, double size);
NA_IAPI void     naFillRangef (NARangef* range, float  pos, float  size);
NA_IAPI void     naFillRangei (NARangei* range, NAInt  pos, NAInt  size);


// Creating the structs with start and end values. Integer ranges can also
// be created using min and max.
NA_IAPI NARange  naMakeRangeWithStartAndEnd (double start, double end);
NA_IAPI NARangef naMakeRangefWithStartAndEnd(float  start, float  end);
NA_IAPI NARangei naMakeRangeiWithStartAndEnd(NAInt  start, NAInt  end);
NA_IAPI NARangei naMakeRangeiWithMinAndMax  (NAInt  min,   NAInt  max);
// Filling the given struct pointers.
NA_IAPI void     naFillRangeWithStartAndEnd (NARange*  range, double start, double end);
NA_IAPI void     naFillRangefWithStartAndEnd(NARangef* range, float  start, float  end);
NA_IAPI void     naFillRangeiWithStartAndEnd(NARangei* range, NAInt  start, NAInt  end);
NA_IAPI void     naFillRangeiWithMinAndMax  (NARangei* range, NAInt  min,   NAInt  max);


// Convert from and to the NAMinMax structure.
NA_IAPI NARange     naMakeRangeWithMinMax1    (NAMinMax1  minmax);
NA_IAPI NARangef    naMakeRangefWithMinMax1f  (NAMinMax1f minmax);
NA_IAPI NARangei    naMakeRangeiWithMinMax1i  (NAMinMax1i minmax);
NA_IAPI NAMinMax1   naMakeMinMax1WithRange    (NARange    range);
NA_IAPI NAMinMax1f  naMakeMinMax1fWithRangef  (NARangef   range);
NA_IAPI NAMinMax1i  naMakeMinMax1iWithRangei  (NARangei   range);
// Convert by using pointers.
NA_IAPI void        naFillRangeWithMinMax1    (NARange*    range,  NAMinMax1*  minmax);
NA_IAPI void        naFillRangefWithMinMax1f  (NARangef*   range,  NAMinMax1f* minmax);
NA_IAPI void        naFillRangeiWithMinMax1i  (NARangei*   range,  NAMinMax1i* minmax);
NA_IAPI void        naFillMinMax1WithRange    (NAMinMax1*  minmax, NARange*    range);
NA_IAPI void        naFillMinMax1fWithRangef  (NAMinMax1f* minmax, NARangef*   range);
NA_IAPI void        naFillMinMax1iWithRangei  (NAMinMax1i* minmax, NARangei*   range);


// Perform Range checks similar to the ones found in NAMathOperators.
// naInsideRange returns true, if x is inside the range. The letters I and E
// show how the range's min and max will be compared: Inclusive or Exclusive.
// For example: naInsideIE({3, 7}, x) corresponds to a range of [3,7).
NA_IAPI NABool naInsideRangeII (NARange  range, double x);
NA_IAPI NABool naInsideRangefII(NARangef range, float  x);
NA_IAPI NABool naInsideRangeIE (NARange  range, double x); // Use these if you
NA_IAPI NABool naInsideRangefIE(NARangef range, float  x); // are uncertain.
NA_IAPI NABool naInsideRangeEI (NARange  range, double x);
NA_IAPI NABool naInsideRangefEI(NARangef range, float  x);
NA_IAPI NABool naInsideRangeEE (NARange  range, double x);
NA_IAPI NABool naInsideRangefEE(NARangef range, float  x);
// Integer variant. Comparison is always inclusive - inclusive.
NA_IAPI NABool naInsideRangei  (NARangei range, NAInt  x);


// naAlmostInside checks if the given value is almost or completely inside
// the given values. Meaning, a small relative margin of NA_SINGULARITY on
// both sides is considered. Warning: Slow.
NA_IAPI NABool naAlmostInsideRange (NARange  range, double x);
NA_IAPI NABool naAlmostInsideRangef(NARangef range, float  x);


// Consistency checks
NA_IAPI NABool naIsRangeValid     (NARange  range);
NA_IAPI NABool naIsRangefValid    (NARangef range);
NA_IAPI NABool naIsRangeiValid    (NARangei range);
NA_IAPI NABool naIsRangeEmpty     (NARange  range);
NA_IAPI NABool naIsRangefEmpty    (NARangef range);
NA_IAPI NABool naIsRangeiEmpty    (NARangei range);
NA_IAPI NABool naIsRangeNegative  (NARange  range);
NA_IAPI NABool naIsRangefNegative (NARangef range);
NA_IAPI NABool naIsRangeiNegative (NARangei range);
NA_IAPI NABool naIsRangeUseful    (NARange  range);
NA_IAPI NABool naIsRangefUseful   (NARangef range);
NA_IAPI NABool naIsRangeiUseful   (NARangei range);


// Returns the end of the given range which is (pos + size). For the integer
// variant, there also exists a max function whitch returns (pos + size - 1).
NA_IAPI double naGetRangeEnd   (NARange  range);
NA_IAPI float  naGetRangefEnd  (NARangef range);
NA_IAPI NAInt  naGetRangeiEnd  (NARangei range);
NA_IAPI NAInt  naGetRangeiMax  (NARangei range);











// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAMathOperators.h"


NA_IDEF NARange naMakeRange(double pos, double size){
  NARange newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(pos) && naIsSizeValueValid(size)))
      naError("naMakeRange", "Invalid values given.");
    if(!naIsSizeValueUseful(size))
      naError("naMakeRange", "size is not useful.");
  #endif
  newrange.pos = pos;
  newrange.size = size;
  return newrange;
}
NA_IDEF NARangef naMakeRangef(float pos, float size){
  NARangef newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidf(pos) && naIsSizeValueValidf(size)))
      naError("naMakeRangef", "Invalid values given.");
    if(!naIsSizeValueUsefulf(size))
      naError("naMakeRange", "size is not useful.");
  #endif
  newrange.pos = pos;
  newrange.size = size;
  return newrange;
}
NA_IDEF NARangei naMakeRangei(NAInt pos, NAInt size){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(pos) && naIsSizeValueValidi(size)))
      naError("naMakeRangei", "Invalid values given.");
    if(!naIsSizeValueUsefuli(size))
      naError("naMakeRange", "size is not useful.");
  #endif
  newrange.pos = pos;
  newrange.size = size;
  return newrange;
}


NA_IDEF void naFillRange(NARange* range, double pos, double size){
  #ifndef NDEBUG
    if(!(naIsPosValueValid(pos) && naIsSizeValueValid(size)))
      naError("naFillRange", "Invalid values given.");
    if(!naIsSizeValueUseful(size))
      naError("naFillRange", "size is not useful.");
  #endif
  range->pos = pos;
  range->size = size;
}
NA_IDEF void naFillRangef(NARangef* range, float pos, float size){
  #ifndef NDEBUG
    if(!(naIsPosValueValidf(pos) && naIsSizeValueValidf(size)))
      naError("naFillRangef", "Invalid values given.");
    if(!naIsSizeValueUsefulf(size))
      naError("naFillRangef", "size is not useful.");
  #endif
  range->pos = pos;
  range->size = size;
}
NA_IDEF void naFillRangei(NARangei* range, NAInt pos, NAInt size){
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(pos) && naIsSizeValueValidi(size)))
      naError("naFillRangei", "Invalid values given.");
    if(!naIsSizeValueUsefuli(size))
      naError("naFillRangei", "size is not useful.");
  #endif
  range->pos = pos;
  range->size = size;
}


NA_IDEF NARange naMakeRangeWithStartAndEnd(double start, double end){
  NARange newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(start) && naIsPosValueValid(end)))
      naError("naMakeRangeWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.pos = start;
  newrange.size = naStartAndEndToSize(start, end);
  return newrange;
}
NA_IDEF NARangef naMakeRangefWithStartAndEnd(float start, float end){
  NARangef newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidf(start) && naIsPosValueValidf(end)))
      naError("naMakeRangefWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.pos = start;
  newrange.size = naStartAndEndToSizef(start, end);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithStartAndEnd(NAInt start, NAInt end){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(start) && naIsPosValueValidi(end)))
      naError("naMakeRangeiWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.pos = start;
  newrange.size = naStartAndEndToSizei(start, end);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithMinAndMax(NAInt min, NAInt max){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(min) && naIsPosValueValidi(max)))
      naError("naMakeRangeiWithMinAndMax", "Invalid values given.");
  #endif
  newrange.pos = min;
  newrange.size = naMinAndMaxToSizei(min, max);
  return newrange;
}



NA_IDEF void naFillRangeWithStartAndEnd(NARange* range, double start, double end){
  #ifndef NDEBUG
    if(!(naIsPosValueValid(start) && naIsPosValueValid(end)))
      naError("naMakeRangeWithStartAndEnd", "Invalid values given.");
  #endif
  range->pos = start;
  range->size = naStartAndEndToSize(start, end);
}
NA_IDEF void naFillRangefWithStartAndEnd(NARangef* range, float start, float end){
  #ifndef NDEBUG
    if(!(naIsPosValueValidf(start) && naIsPosValueValidf(end)))
      naError("naMakeRangefWithStartAndEnd", "Invalid values given.");
  #endif
  range->pos = start;
  range->size = naStartAndEndToSizef(start, end);
}
NA_IDEF void naFillRangeiWithStartAndEnd(NARangei* range, NAInt start, NAInt end){
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(start) && naIsPosValueValidi(end)))
      naError("naMakeRangeiWithStartAndEnd", "Invalid values given.");
  #endif
  range->pos = start;
  range->size = naStartAndEndToSizei(start, end);
}
NA_IDEF void naFillRangeiWithMinAndMax(NARangei* range, NAInt min, NAInt max){
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(min) && naIsPosValueValidi(max)))
      naError("naMakeRangeiWithMinAndMax", "Invalid values given.");
  #endif
  range->pos = min;
  range->size = naMinAndMaxToSizei(min, max);
}



NA_IDEF NARange naMakeRangeWithMinMax1(NAMinMax1 minmax){
  NARange newrange;
  newrange.pos = minmax.start[0];
  newrange.size = naStartAndEndToSize(minmax.start[0], minmax.end[0]);
  return newrange;
}
NA_IDEF NARangef naMakeRangefWithMinMax1f(NAMinMax1f minmax){
  NARangef newrange;
  newrange.pos = minmax.start[0];
  newrange.size = naStartAndEndToSizef(minmax.start[0], minmax.end[0]);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithMinMax1i(NAMinMax1i minmax){
  NARangei newrange;
  newrange.pos = minmax.min[0];
  newrange.size = naMinAndMaxToSizei(minmax.min[0], minmax.max[0]);
  return newrange;
}
NA_IDEF NAMinMax1 naMakeMinMax1WithRange(NARange range){
  NAMinMax1 newminmax;
  newminmax.start[0] = range.pos;
  newminmax.end[0] = naGetRangeEnd(range);
  return newminmax;
}
NA_IDEF NAMinMax1f naMakeMinMax1fWithRangef(NARangef range){
  NAMinMax1f newminmax;
  newminmax.start[0] = range.pos;
  newminmax.end[0] = naGetRangefEnd(range);
  return newminmax;
}
NA_IDEF NAMinMax1i naMakeMinMax1iWithRangei(NARangei range){
  NAMinMax1i newminmax;
  newminmax.min[0] = range.pos;
  newminmax.max[0] = naGetRangeiMax(range);
  return newminmax;
}



NA_IDEF void naFillRangeWithMinMax1(NARange* range, NAMinMax1* minmax){
  range->pos = minmax->start[0];
  range->size = naStartAndEndToSize(minmax->start[0], minmax->end[0]);
}
NA_IDEF void naFillRangefWithMinMax1f(NARangef* range, NAMinMax1f* minmax){
  range->pos = minmax->start[0];
  range->size = naStartAndEndToSizef(minmax->start[0], minmax->end[0]);
}
NA_IDEF void naFillRangeiWithMinMax1i(NARangei* range, NAMinMax1i* minmax){
  range->pos = minmax->min[0];
  range->size = naMinAndMaxToSizei(minmax->min[0], minmax->max[0]);
}
NA_IDEF void naFillMinMax1WithRange(NAMinMax1* minmax, NARange* range){
  minmax->start[0] = range->pos;
  minmax->end[0] = naGetRangeEnd(*range);
}
NA_IDEF void naFillMinMax1fWithRangef(NAMinMax1f* minmax, NARangef* range){
  minmax->start[0] = range->pos;
  minmax->end[0] = naGetRangefEnd(*range);
}
NA_IDEF void naFillMinMax1iWithRangei(NAMinMax1i* minmax, NARangei* range){
  minmax->min[0] = range->pos;
  minmax->max[0] = naGetRangeiMax(*range);
}




// inclusive, inclusive
NA_IDEF NABool naInsideRangeII(NARange range, double x){
  return naInsideII(range.pos, naGetRangeEnd(range), x);
}
NA_IDEF NABool naInsideRangefII(NARangef range, float x){
  return naInsidefII(range.pos, naGetRangefEnd(range), x);
}
NA_IDEF NABool naInsideRangeIE(NARange range, double x){
  return naInsideIE(range.pos, naGetRangeEnd(range), x);
}
NA_IDEF NABool naInsideRangefIE(NARangef range, float x){
  return naInsidefIE(range.pos, naGetRangefEnd(range), x);
}
NA_IDEF NABool naInsideRangeEI(NARange range, double x){
  return naInsideEI(range.pos, naGetRangeEnd(range), x);
}
NA_IDEF NABool naInsideRangefEI(NARangef range, float x){
  return naInsidefEI(range.pos, naGetRangefEnd(range), x);
}
NA_IDEF NABool naInsideRangeEE(NARange range, double x){
  return naInsideEE(range.pos, naGetRangeEnd(range), x);
}
NA_IDEF NABool naInsideRangefEE(NARangef range, float x){
  return naInsidefEE(range.pos, naGetRangefEnd(range), x);
}



NA_IDEF NABool naInsideRangei(NARangei range, NAInt x){
  return naInsidei(range.pos, naGetRangeiMax(range), x);
}



NA_IDEF NABool naAlmostInsideRange(NARange range, double x){
  return naAlmostInside(range.pos, naGetRangeEnd(range), x);
}
NA_IDEF NABool naAlmostInsideRangef(NARangef range, float x){
  return naAlmostInsidef(range.pos, naGetRangefEnd(range), x);
}



NA_IDEF NABool naIsRangeValid(NARange range){
  return naIsPosValueValid(range.pos) && naIsSizeValueValid(range.size);
}
NA_IDEF NABool naIsRangefValid(NARangef range){
  return naIsPosValueValidf(range.pos) && naIsSizeValueValidf(range.size);
}
NA_IDEF NABool naIsRangeiValid(NARangei range){
  return naIsPosValueValidi(range.pos) && naIsSizeValueValidi(range.size);
}
NA_IDEF NABool naIsRangeEmpty(NARange range){
  return naIsSizeValueEmpty(range.size);
}
NA_IDEF NABool naIsRangefEmpty(NARangef range){
  return naIsSizeValueEmptyf(range.size);
}
NA_IDEF NABool naIsRangeiEmpty(NARangei range){
  return naIsSizeValueEmptyi(range.size);
}
NA_IDEF NABool naIsRangeNegative(NARange range){
  return naIsSizeValueNegative(range.size);
}
NA_IDEF NABool naIsRangefNegative(NARangef range){
  return naIsSizeValueNegativef(range.size);
}
NA_IDEF NABool naIsRangeiNegative(NARangei range){
  return naIsSizeValueNegativei(range.size);
}
NA_IDEF NABool naIsRangeUseful(NARange range){
  return (naIsPosValueUseful(range.pos) && naIsSizeValueUseful(range.size));
}
NA_IDEF NABool naIsRangefUseful(NARangef range){
  return (naIsPosValueUsefulf(range.pos) && naIsSizeValueUsefulf(range.size));
}
NA_IDEF NABool naIsRangeiUseful(NARangei range){
  return (naIsPosValueUsefuli(range.pos) && naIsSizeValueUsefuli(range.size));
}


NA_IDEF double naGetRangeEnd(NARange range){
  #ifndef NDEBUG
    if(naIsRangeEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naPosAndSizeToEnd(range.pos, range.size);
}
NA_IDEF float naGetRangefEnd(NARangef range){
  #ifndef NDEBUG
    if(naIsRangefEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangefValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naPosAndSizeToEndf(range.pos, range.size);
}
NA_IDEF NAInt naGetRangeiEnd(NARangei range){
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naPosAndSizeToEndi(range.pos, range.size);
}
NA_IDEF NAInt naGetRangeiMax(NARangei range){
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naPosAndSizeToMaxi(range.pos, range.size);
}








#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_RANGE_INCLUDED


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
