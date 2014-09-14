
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


#ifndef NA_RANGE_INCLUDED
#define NA_RANGE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif




// ///////////////////////////////////
// Functions for testing single values
// ///////////////////////////////////


// naMin and naMax return the minimum or maximum of two values.
NA_IAPI double       naMin   (double a,      double b);
NA_IAPI float        naMinf  (float  a,      float  b);
NA_IAPI NAInt        naMini  (NAInt  a,      NAInt  b);
NA_IAPI int32        naMini32(int32  a,      int32  b);
NA_IAPI int64        naMini64(int64  a,      int64  b);

NA_IAPI double       naMax   (double a,      double b);
NA_IAPI float        naMaxf  (float  a,      float  b);
NA_IAPI NAInt        naMaxi  (NAInt  a,      NAInt  b);
NA_IAPI int32        naMaxi32(int32  a,      int32  b);
NA_IAPI int64        naMaxi64(int64  a,      int64  b);

// Note that in many implementations, Min and Max are implemented as the
// following macros:
// #define NA_MAX(a, b) (((a)>(b))?(a):(b))
// #define NA_MIN(a, b) (((a)<(b))?(a):(b))
// Nontheless, these are not used here in NALib as they can not be debugged
// as good as the functions: Quite often, the two parameters are only
// available as an expression and hence are not shown in the compiler. When
// using the functions, the two resulting values can be debugged when jumping
// inside the min or max function.


// naAlmostZero returns NA_TRUE, if the ABSOLUTE discance between x and 0 is
// smaller than NA_SINGULARITY.
NA_IAPI NABool naAlmostZero (double x);
NA_IAPI NABool naAlmostZerof(float  x);

// naAlmostOne returns NA_TRUE, if the ABSOLUTE discance between x and 1 is
// smaller than NA_SINGULARITY.
NA_IAPI NABool naAlmostOne (double x);
NA_IAPI NABool naAlmostOnef(float  x);

// naAlmost returns NA_TRUE, if the RELATIVE distance between x and y is
// smaller than NA_SINGULARITY. If y is close to 0, the absolute distance is
// checked. Warning: These functions are slow.
NA_IAPI NABool naAlmost (double x, double y);
NA_IAPI NABool naAlmostf(float  x, float  y);



// ///////////////////////////////////
// Functions for testing value ranges
// ///////////////////////////////////

// naInside returns true, if x is inside the span depicted by a and b. The
// value a should be smaller than b. The letters I and E depict whether a or b
// are compared inclusive or exclusive. For example:
// naInsideIE(3, 7, x) corresponds to a range of [3,7).

// inclusive, inclusive
NA_IAPI NABool naInsideII (double a, double b, double x);
NA_IAPI NABool naInsidefII(float  a, float  b, float  x);
// inclusive, exclusive
NA_IAPI NABool naInsideIE (double a, double b, double x); // use these if you
NA_IAPI NABool naInsidefIE(float  a, float  b, float  x); // are uncertain.
// exclusive, inclusive
NA_IAPI NABool naInsideEI (double a, double b, double x);
NA_IAPI NABool naInsidefEI(float  a, float  b, float  x);
// exclusive, exclusive
NA_IAPI NABool naInsideEE (double a, double b, double x);
NA_IAPI NABool naInsidefEE(float  a, float  b, float  x);

// Integer variants. a denotes Min, b denotes Max. Comparison is always
// inclusive - inclusive.
NA_IAPI NABool naInsidei  (NAInt  a, NAInt  b, NAInt  x);
NA_IAPI NABool naInsidei32(int32  a, int32  b, int32  x);
NA_IAPI NABool naInsidei64(int64  a, int64  b, int64  x);



// naAlmostInside checks if the given value is almost or completely inside
// the given values. Meaning, a small relative margin of NA_SINGULARITY on
// both sides is considered. Takes into consideration that a or b might be
// negative.
NA_IAPI NABool naAlmostInside (double a, double b, double x);
NA_IAPI NABool naAlmostInsidef(float  a, float  b, float  x);




// ///////////////////////////////////
// Normed Ranges
// ///////////////////////////////////


// naInsideNorm returns true, if x is inside a normed range. The following norms
// are impmemented:
// naInsideNormZI: [ 0, 1 ]
NA_IAPI NABool naInsideNormZI (double x);
NA_IAPI NABool naInsideNormZIf(float x);

// naInsideNormZE: [ 0, 1 )
NA_IAPI NABool naInsideNormZE (double x);
NA_IAPI NABool naInsideNormZEf(float x);

// naInsideNormII: [-1, 1 ]
NA_IAPI NABool naInsideNormII (double x);
NA_IAPI NABool naInsideNormIIf(float x);

// naInsideNormIE: [-1, 1 )
NA_IAPI NABool naInsideNormIE (double x);
NA_IAPI NABool naInsideNormIEf(float x);

// naInsideNormEE: (-1, 1 )
NA_IAPI NABool naInsideNormEE (double x);
NA_IAPI NABool naInsideNormEEf(float x);



// ///////////////////////////////////
// Custom Ranges
// ///////////////////////////////////

// Additionally to the above min- and max-functions, NALib provides a
// datastructure NARange which holds an offset and a size for one dimension.
// For simplification, offset is usually called pos. First however, some
// helper functions working with single values rather than datastructures.

// This function alters the given pos and size such that size will become
// positive while retaining the depicted range. For example with doubles:
// pos = 3. and size = -2. will become pos = 1. and size = 2. The resulting
// range starts at 1. and ends at 1. + 2. = 3.
// Note: When using integers, the end must be computed with -1. Therefore
// the result differs:
// pos = 3 and size = -2 will become pos = 2 and size = 2. The resulting
// range starts at 2 and ends at (2 + 2) - 1 = 3
// If size already was positive, nothing will be changed.
NA_IAPI void naMakePositive      (double* NA_RESTRICT pos,
                                  double* NA_RESTRICT size);
NA_IAPI void naMakeiPositive     (NAInt*  NA_RESTRICT pos,
                                  NAInt*  NA_RESTRICT size);

// This function alters the given pos and size such that the resulting range
// will be fully be contained in a range given by [0, containingsize] for
// floating point values or [0, containingsize-1] for integers. Negative
// values are treated as follows and in the following order:
// - if pos is negative, it denotes the number of units from the end.
//   For integers, pos = -1 therefore corresponds to size-1.
// - If the size is now 0, the function will return.
// - if size is negative, it denotes the size up and including to the given
//   number of units from the end, meaning -1 denotes the last unit.
// - if the pos and size combination somehow leads to a size of exactly 0,
//   the resulting range will be empty without a warning emitted.
// - If the pos and size combination somehow leads to an over- or underflow,
//   a warning will be emitted if NDEBUG is defined. The resulting range will
//   be empty.
NA_IAPI void naMakePositiveInSize(  double* NA_RESTRICT pos,
                                    double* NA_RESTRICT size,
                                    double  containingsize);
NA_IAPI void naMakeiPositiveInSize( NAInt*  NA_RESTRICT pos,
                                    NAInt*  NA_RESTRICT size,
                                    NAInt   containingsize);

// These are the definitions of the datastructure:
typedef struct NARange  NARange;
typedef struct NARangef NARangef;
typedef struct NARangei NARangei;
struct NARange{
  double pos;
  double size;
};
struct NARangef{
  float pos;
  float size;
};
struct NARangei{
  NAInt pos;
  NAInt size;
};

// Filling the structs with values. Note that the integer functions also
// provide a function to work with end instead of max. end = max + 1
NA_IAPI NARange  naMakeRange( double pos, double size);
NA_IAPI NARangef naMakeRangef(float  pos, float  size);
NA_IAPI NARangei naMakeRangei(NAInt  pos, NAInt  size);
NA_IAPI NARange  naMakeRangeWithMinAndMax( double min, double max);
NA_IAPI NARangef naMakeRangefWithMinAndMax(float  min, float  max);
NA_IAPI NARangei naMakeRangeiWithMinAndMax(NAInt  min, NAInt  max);
NA_IAPI NARangei naMakeRangeiWithMinAndEnd(NAInt  min, NAInt  end);


// naInsideRange returns true, if x is inside the range. The letters I and E
// depict whether the range's min and max shall be compared inclusive or
// exclusive.
// For example: naInsideIE({3, 7}, x) corresponds to a range of [3,7).

// inclusive, inclusive
NA_IAPI NABool naInsideRangeII (NARange  range, double x);
NA_IAPI NABool naInsideRangefII(NARangef range, float  x);
// inclusive, exclusive
NA_IAPI NABool naInsideRangeIE (NARange  range, double x); // Use these if you
NA_IAPI NABool naInsideRangefIE(NARangef range, float  x); // are uncertain.
// exclusive, inclusive
NA_IAPI NABool naInsideRangeEI (NARange  range, double x);
NA_IAPI NABool naInsideRangefEI(NARangef range, float  x);
// exclusive, exclusive
NA_IAPI NABool naInsideRangeEE (NARange  range, double x);
NA_IAPI NABool naInsideRangefEE(NARangef range, float  x);

// Integer variants. Comparison is always inclusive - inclusive.
NA_IAPI NABool naInsideRangei  (NARangei range, NAInt  x);

// naAlmostInside checks if the given value is almost or completely inside
// the given values. Meaning, a small relative margin of NA_SINGULARITY on
// both sides is considered. Warning: Slow.
NA_IAPI NABool naAlmostInsideRange (NARange  range, double x);
NA_IAPI NABool naAlmostInsideRangef(NARangef range, float  x);








// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAMathOperators.h"


// Note that the smaller- or greater-operator often is faster than the
// smallerequal- or greaterequal-operator.
// (a<=b) will always be written as !(a>b)
// (a>=b) will always be written as !(a<b)



NA_IDEF double naMin (double a, double b){
  return (a<b)?a:b;
}
NA_IDEF float naMinf(float a, float b){
  return (a<b)?a:b;
}
NA_IDEF NAInt naMini(NAInt a, NAInt b){
  return (a<b)?a:b;
}
NA_IDEF int32 naMini32(int32 a, int32 b){
  return (a<b)?a:b;
}
NA_IDEF int64 naMini64(int64 a, int64 b){
  return (a<b)?a:b;
}


NA_IDEF double naMax (double a, double b){
  return (a>b)?a:b;
}
NA_IDEF float naMaxf(float a, float b){
  return (a>b)?a:b;
}
NA_IDEF NAInt naMaxi  (NAInt a, NAInt b){
  return (a>b)?a:b;
}
NA_IDEF int32 naMaxi32(int32 a, int32 b){
  return (a>b)?a:b;
}
NA_IDEF int64 naMaxi64(int64 a, int64 b){
  return (a>b)?a:b;
}


NA_IDEF NABool naAlmostZero  (double x){
  return ((x < NA_SINGULARITY) && (x > -NA_SINGULARITY));
}
NA_IDEF NABool naAlmostZerof (float x){
  return ((x < NA_SINGULARITYf) && (x > -NA_SINGULARITYf));
}

NA_IDEF NABool naAlmostOne  (double x){
  return ((x < NA_SUP_NORM) && (x > NA_SUB_NORM));
}
NA_IDEF NABool naAlmostOnef (float x){
  return ((x < NA_SUP_NORMf) && (x > NA_SUB_NORMf));
}


NA_IDEF NABool naAlmost(double x, double y){
  if(naAlmostZero(y)){
    if(naAlmostZero(x)){
      return NA_TRUE;
    }else{
      return naAlmostZero(x-y);
    }
  }else{
    double relativeerror = x/y;
    if(relativeerror < 0.){
      return naAlmostOne(-relativeerror);
    }else{
      return naAlmostOne(relativeerror);
    }
  }
}
NA_IDEF NABool naAlmostf(float x, float y){
  if(naAlmostZerof(y)){
    if(naAlmostZerof(x)){
      return NA_TRUE;
    }else{
      return naAlmostZerof(x-y);
    }
  }else{
    float relativeerror = x/y;
    if(relativeerror < 0.f){
      return naAlmostOnef(-relativeerror);
    }else{
      return naAlmostOnef(relativeerror);
    }
  }
}



// inclusive, inclusive
NA_IDEF NABool naInsideII(double a, double b, double x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsideII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsidefII(float a, float b, float x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsidefII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}

NA_IDEF NABool naInsidei(NAInt a, NAInt b, NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naInsidei32(a, b, x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naInsidei64(a, b, x);
#endif
}
NA_IDEF NABool naInsidei32(int32 a, int32 b, int32 x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsidei32", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsidei64(int64 a, int64 b, int64 x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsidei64", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}



// inclusive, exclusive
NA_IDEF NABool naInsideIE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}
NA_IDEF NABool naInsidefIE(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsidefIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}



// exclusive, inclusive
NA_IDEF NABool naInsideEI(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}
NA_IDEF NABool naInsidefEI(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsidefEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}


// exclusive, exclusive
NA_IDEF NABool naInsideEE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideEE", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}
NA_IDEF NABool naInsidefEE(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsidefEE", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}


// Note that for the naAlmostInside functions, naInsideEE is used after
// adjusting the range. This cound be anything else but as this is a function
// which only needs to be almost accurate, the one function is chosen which is
// simplest.
NA_IDEF NABool naAlmostInside(double a, double b, double x){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInside", "a is greater b.");
  #endif
  a *= (a < 0.) ? NA_SUP_NORM : NA_SUB_NORM;
  b *= (b < 0.) ? NA_SUB_NORM : NA_SUP_NORM;
  return naInsideEE(a, b, x);
}
NA_IDEF NABool naAlmostInsidef(float a, float b, float x){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInsidef", "a is greater b.");
  #endif
  a *= (a < 0.f) ? NA_SUP_NORMf : NA_SUB_NORMf;
  b *= (b < 0.f) ? NA_SUB_NORMf : NA_SUP_NORMf;
  return naInsidefEE(a, b, x);
}


// naInsideNormZI: [ 0, 1]
NA_IDEF NABool naInsideNormZI (double x){
  return !((x < 0.) || (x > 1.));
}
NA_IDEF NABool naInsideNormZIf(float x){
  return !((x < 0.f) || (x > 1.f));
}


// naInsideNormZE: [ 0, 1)
NA_IDEF NABool naInsideNormZE (double x){
  return (!(x < 0.) && (x < 1.));
}
NA_IDEF NABool naInsideNormZEf(float x){
  return (!(x < 0.f) && (x < 1.f));
}


// naInsideNormII: [-1, 1]
NA_IDEF NABool naInsideNormII (double x){
  return !((x < -1.) || (x > 1.));
}
NA_IDEF NABool naInsideNormIIf(float x){
  return !((x < -1.f) || (x > 1.f));
}


// naInsideNormIE: [-1, 1)
NA_IDEF NABool naInsideNormIE (double x){
  return (!(x < -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormIEf(float x){
  return (!(x < -1.f) && (x < 1.f));
}


// naInsideNormEE: (-1, 1)
NA_IDEF NABool naInsideNormEE (double x){
  return ((x > -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormEEf(float x){
  return ((x > -1.f) && (x < 1.f));
}






// helper functions. Should be hidden.
NA_IHLP NABool naIsPosFieldValid(double a){
  return !naIsNaN(a);
}
NA_IHLP NABool naIsPosfFieldValid(float a){
  return !naIsNaN(a);
}
NA_IHLP NABool naIsPosiFieldValid(NAInt a){
  NA_UNUSED_PARAMETER(a);
  return NA_TRUE;
}

NA_IHLP NABool naIsSizeFieldValid(double a){
  return !naIsNaN(a);
}
NA_IHLP NABool naIsSizefFieldValid(float a){
  return !naIsNaNf(a);
}
NA_IHLP NABool naIsSizeiFieldValid(NAInt a){
  NA_UNUSED_PARAMETER(a);
  return NA_TRUE;
}

NA_IHLP NABool naIsPosFieldUseful(double a){
  return !naIsNaN(a);
}
NA_IHLP NABool naIsPosfFieldUseful(float a){
  return !naIsNaNf(a);
}
NA_IHLP NABool naIsPosiFieldUseful(NAInt a){
  NA_UNUSED_PARAMETER(a);
  return NA_TRUE;
}

NA_IHLP NABool naIsSizeFieldUseful(double a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_IHLP NABool naIsSizefFieldUseful(float a){
  // Note that this test will return NA_FALSE if a is NaN.
  return (a > 0.);
}
NA_IHLP NABool naIsSizeiFieldUseful(NAInt a){
  return (a > 0);
}




NA_IDEF void naMakePositive(double* NA_RESTRICT pos, double* NA_RESTRICT size){
  if(*size < 0.){
    *pos = *pos + *size;
    *size = -*size;
  }
}


NA_IDEF void naMakeiPositive(NAInt* NA_RESTRICT pos, NAInt* NA_RESTRICT size){
  if(*size < 0){
    *pos = *pos + *size + 1; // important + 1 !
    *size = -*size;
  }
}


NA_IDEF void naMakePositiveInSize(double* NA_RESTRICT pos, double* NA_RESTRICT size, double containingsize){
  // First, we ensure that pos is withing the containing range. After that
  // we will look at the size parameter.
  double remainingsize = containingsize - *pos;
  if(*pos < 0){
    *pos = *pos + containingsize;
    remainingsize -= containingsize;
  }
  if(remainingsize < 0){
    #ifndef NDEBUG
      naError("naMakePositiveInSize", "Invalid pos leads to range overflow. Correcting to empty range.");
    #endif
    *size = 0;
  }else if(remainingsize > containingsize){
    #ifndef NDEBUG
      naError("naMakePositiveInSize", "Invalid pos leads to range underflow. Correcting to empty range.");
    #endif
    *size = 0;
  }else{
    // The pos is positive. Now, adjust the size.
    if(*size < 0){ // negative size parameter
      *size = remainingsize + *size;
      if(*size < 0){
        // When the resulting size is smaller than 0, underflow.
        #ifndef NDEBUG
          naError("naMakePositiveInSize", "Invalid size leads to range underflow. Correcting to empty range.");
        #endif
        *size = 0;
      }
    }else{ // positive or 0 size parameter
      if(*size > remainingsize){
        // When the desired size is bigger than the size available, overflow.
        #ifndef NDEBUG
          naError("naMakePositiveInSize", "Invalid size leads to range overflow. Correcting to empty range.");
        #endif
        *size = 0;
      }
    }
  }
}


NA_IDEF void naMakeiPositiveInSize(NAInt*  NA_RESTRICT pos, NAInt* NA_RESTRICT size, NAInt containingsize){
  // First, we ensure that pos is withing the containing range. After that
  // we will look at the size parameter.
  NAInt remainingsize = containingsize - *pos;
  if(*pos < 0){
    *pos = *pos + containingsize;
    remainingsize -= containingsize;
  }
  if(remainingsize < 0){
    #ifndef NDEBUG
      naError("naMakeiPositiveInSize", "Invalid pos leads to range overflow. Correcting to empty range.");
    #endif
    *size = 0;
  }else if(remainingsize > containingsize){
    #ifndef NDEBUG
      naError("naMakeiPositiveInSize", "Invalid pos leads to range underflow. Correcting to empty range.");
    #endif
    *size = 0;
  }else{
    // The pos is positive. Now, adjust the size.
    if(*size < 0){ // negative size parameter
      *size = remainingsize + *size + 1;  // Important + 1 !
      if(*size < 0){
        // When the resulting size is smaller than 0, underflow.
        #ifndef NDEBUG
          naError("naMakeiPositiveInSize", "Invalid size leads to range underflow. Correcting to empty range.");
        #endif
        *size = 0;
      }
    }else{ // positive or 0 size parameter
      if(*size > remainingsize){
        // When the desired size is bigger than the size available, overflow.
        #ifndef NDEBUG
          naError("naMakeiPositiveInSize", "Invalid size leads to range overflow. Correcting to empty range.");
        #endif
        *size = 0;
      }
    }
  }
}


NA_IDEF NARange naMakeRange(double pos, double size){
  NARange newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosFieldValid(pos) && naIsSizeFieldValid(size)))
      naError("naMakeRange", "Invalid values given.");
  #endif
  newrange.pos = pos;
  newrange.size = size;
  return newrange;
}
NA_IDEF NARangef naMakeRangef(float pos, float size){
  NARangef newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosfFieldValid(pos) && naIsSizefFieldValid(size)))
      naError("naMakeRangef", "Invalid values given.");
  #endif
  newrange.pos = pos;
  newrange.size = size;
  return newrange;
}
NA_IDEF NARangei naMakeRangei(NAInt pos, NAInt size){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosiFieldValid(pos) && naIsSizeiFieldValid(size)))
      naError("naMakeRangei", "Invalid values given.");
  #endif
  newrange.pos = pos;
  newrange.size = size;
  return newrange;
}


NA_IDEF NARange naMakeRangeWithMinAndMax(double min, double max){
  NARange newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosFieldValid(min) && naIsPosFieldValid(max)))
      naError("naMakeRangeWithMinAndMax", "Invalid values given.");
  #endif
  newrange.pos = min;
  newrange.size = max - min;
  return newrange;
}
NA_IDEF NARangef naMakeRangefWithMinAndMax(float min, float max){
  NARangef newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosfFieldValid(min) && naIsPosfFieldValid(max)))
      naError("naMakeRangefWithMinAndMax", "Invalid values given.");
  #endif
  newrange.pos = min;
  newrange.size = max - min;
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithMinAndMax(NAInt min, NAInt max){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosiFieldValid(min) && naIsPosiFieldValid(max)))
      naError("naMakeRangeiWithMinAndMax", "Invalid values given.");
  #endif
  newrange.pos = min;
  newrange.size = max - min + 1;  // note the +1.
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithMinAndEnd(NAInt min, NAInt end){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosiFieldValid(min) && naIsPosiFieldValid(end)))
      naError("naMakeRangeiWithMinAndEnd", "Invalid values given.");
  #endif
  newrange.pos = min;
  newrange.size = end - min;
  return newrange;
}



// inclusive, inclusive
NA_IDEF NABool naInsideRangeII(NARange range, double x){
  #ifndef NDEBUG
//    if(a>b)
//      naError("naInsideII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < range.pos) || (x > (range.pos + range.size))));
}
NA_IDEF NABool naInsideRangefII(NARangef range, float x){
  #ifndef NDEBUG
//    if(a>b)
//      naError("naInsidefII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < range.pos) || (x > (range.pos + range.size))));
}

NA_IDEF NABool naInsideRangei(NARangei range, NAInt x){
  return (!((x < range.pos) || (x > (range.pos + range.size))));
}



// inclusive, exclusive
NA_IDEF NABool naInsideRangeIE(NARange range, double x){
  #ifndef NDEBUG
//    if(!(a<b))
//      naError("naInsideIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < range.pos) && (x < (range.pos + range.size)));
}
NA_IDEF NABool naInsideRangefIE(NARangef range, float x){
  #ifndef NDEBUG
//    if(!(a<b))
//      naError("naInsidefIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < range.pos) && (x < (range.pos + range.size)));
}



// exclusive, inclusive
NA_IDEF NABool naInsideRangeEI(NARange range, double x){
  #ifndef NDEBUG
//    if(!(a<b))
//      naError("naInsideEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > range.pos) && !(x > (range.pos + range.size)));
}
NA_IDEF NABool naInsideRangefEI(NARangef range, float x){
  #ifndef NDEBUG
//    if(!(a<b))
//      naError("naInsidefEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > range.pos) && !(x > (range.pos + range.size)));
}


// exclusive, exclusive
NA_IDEF NABool naInsideRangeEE(NARange range, double x){
  #ifndef NDEBUG
//    if(!(a<b))
//      naError("naInsideEE", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > range.pos) && (x < (range.pos + range.size)));
}
NA_IDEF NABool naInsideRangefEE(NARangef range, float x){
  #ifndef NDEBUG
//    if(!(a<b))
//      naError("naInsidefEE", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > range.pos) && (x < (range.pos + range.size)));
}


// Note that for the naAlmostInside functions, naInsideEE is used after
// adjusting the range. This cound be anything else but as this is a function
// which only needs to be almost accurate, the one function is chosen which is
// simplest.
NA_IDEF NABool naAlmostInsideRange(NARange range, double x){
  #ifndef NDEBUG
//    if(a>b)
//      naError("naAlmostInside", "a is greater b.");
  #endif
  double a = range.pos;
  double b = range.pos + range.size;
  a = a * ((a < 0.) ? NA_SUP_NORM : NA_SUB_NORM);
  b = b * ((b < 0.) ? NA_SUB_NORM : NA_SUP_NORM);
  return naInsideEE(a, b, x);
}
NA_IDEF NABool naAlmostInsideRangef(NARangef range, float x){
  #ifndef NDEBUG
//    if(a>b)
//      naError("naAlmostInsidef", "a is greater b.");
  #endif
  float a = range.pos;
  float b = range.pos + range.size;
  a = a * ((a < 0.f) ? NA_SUP_NORMf : NA_SUB_NORMf);
  b = b * ((b < 0.f) ? NA_SUB_NORMf : NA_SUP_NORMf);
  return naInsidefEE(a, b, x);
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
