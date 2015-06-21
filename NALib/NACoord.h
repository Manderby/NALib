
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_COORD_INCLUDED
#define NA_COORD_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

// Pos, Size and Rect
// Following are definitions for integer and floating-point elements storing
// a position, a size and a rectangle.
//
// When coordinate functions are used, they usually are called many times.
// In NALib, coordinate computation is always performed straight-forward. Even
// if the values don't make sense.
//
// Only when debugging, a lot of checks are performed by NALib to find invalid
// values. Checking for invalid values when not debugging would increase
// runtime considerably. The implementation would also become very complicated
// as there are many special cases.
//
// The programmer has to take care of invalid values. Use the debug checks
// and a few helper functions. This should be doable and in the end helps
// avoiding nasty errors which are not uncommon with coordinate computations.
//
// Following are the definitions of valid elements used in NALib:
// Pos:  A position is considered valid if none of the fields is NaN.
//       Integer positions are always valid.
// Size: A size is considered valid, if none of the fields is NaN.
//       A size of zero is considered valid because this is commonly used to
//       mark empty structures. But be careful! It is just NOT DEFINED as
//       invalid. This means: No special code is executed for empty elements!
//       Use the appropriate functions to check if an element is empty.
// Rect: A rectangle is considered valid, if its position and size is valid.
//
// Sizes or rects with zero width or height are considered "empty". But no
// special code will be executed! This might lead to unexpected behaviour.
// When debugging, NALib checks if a size or rect is empty where it should
// not be. But you as the programmer have to take care of that in your code! 
//
// Negative sizes are not uncommon as well. While not forbidden and perfectly
// meaningful, the functions of NALib only work properly with positive rects.
// Use the naMakeRectPositive function to convert a negative rect into a
// positive one.
//
// Integer versus Floating-point
// -----------------------------
// Note that the integer variant of size and rect behaves differently than
// the floating-point variant. The floating-point-variant treats sizes like
// "inclusive". For example:
// The floating-point Rect (Pos (0., 0.), Size (5., 5.)) contains Pos (5., 5.).
// The integer        Recti(Posi(0 , 0 ), Sizei(5 , 5 )) contains Posi(4 , 4 )
// but not Posi(5, 5).
//
// The integer variants are good for indexes. Therefore, the integer variants
// of NARect denote both a "Max" and an "End". The "End" is computed pos+width.
// The "Max" is computed End-1. For example, use the End for the terminating
// condition of a for-loop. Use the Max to access the last element.


#include "NAMinMax.h"


typedef struct NAPos      NAPos;
typedef struct NAPosi     NAPosi;
typedef struct NASize     NASize;
typedef struct NASizei    NASizei;
typedef struct NARect     NARect;
typedef struct NARecti    NARecti;
typedef struct NABounds4  NABounds4;
typedef struct NABounds4i NABounds4i;

// Please excuse this formatting. It is usually not recommended but here, it
// would be much more confusing if written otherwise.
struct NAPos{     double x;     double  y;     };
struct NAPosi{    NAInt  x;     NAInt   y;     };
struct NASize{    double width; double  height;};
struct NASizei{   NAInt  width; NAInt   height;};
struct NARect{    NAPos  pos;   NASize  size;  };
struct NARecti{   NAPosi pos;   NASizei size;  };
struct NABounds4{ double top;   double  right; double bottom; double left;};
struct NABounds4i{NAInt  top;   NAInt   right; NAInt  bottom; NAInt  left;};


// Create the various elements
// The Variants with E also allow the sizes and rects to be empty or negative.
NA_IAPI NAPos     naMakePos     (double x,      double  y);
NA_IAPI NASize    naMakeSize    (double width,  double  height);
NA_IAPI NASize    naMakeSizeE   (double width,  double  height);
NA_IAPI NARect    naMakeRect    (NAPos  pos,    NASize  size);
NA_IAPI NARect    naMakeRectE   (NAPos  pos,    NASize  size);

NA_IAPI NAPosi    naMakePosi    (NAInt  x,      NAInt   y);
NA_IAPI NASizei   naMakeSizei   (NAInt  width,  NAInt   height);
NA_IAPI NASizei   naMakeSizeiE  (NAInt  width,  NAInt   height);
NA_IAPI NARecti   naMakeRecti   (NAPosi pos,    NASizei size);
NA_IAPI NARecti   naMakeRectiE  (NAPosi pos,    NASizei size);

// Mac OS X specific stuff
#ifdef CGGEOMETRY_H_
NA_IAPI NAPos     naMakePosWithCGPoint(CGPoint cgpoint);
NA_IAPI NASize    naMakeSizeWithCGSize(CGSize cgsize);
NA_IAPI NARect    naMakeRectWithCGRect(CGRect cgrect);
#endif
#if defined __OBJC__ && defined __AVAILABILITYMACROS__
// Note: In order to make this compile, you need to include "NACoord.h" AFTER
// including <Cocoa/Cocoa.h>
NA_IAPI NAPos     naMakePosWithNSPoint(NSPoint nspoint);
NA_IAPI NASize    naMakeSizeWithNSSize(NSSize nssize);
NA_IAPI NARect    naMakeRectWithNSRect(NSRect nsrect);
#endif

NA_IAPI NAPos     naMakePosWithSizeCenteredInRect(NASize size, NARect rect);

NA_IAPI NABounds4   naMakeBounds4   (double top, double right, double bottom, double left);
NA_IAPI NABounds4i  naMakeBounds4i  (NAInt top, NAInt right, NAInt bottom, NAInt left);

// Creates a new pos which is floored to a multiple of alignment. Also works
// for negative positions.
NA_IAPI NAPosi    naMakePosiWithAlignment(NAPosi pos, NARecti alignrect);

// Convert from and to the NAMinMax structure.
NA_IAPI NARect      naMakeRectWithMinMax1   (NAMinMax1  minmax0, NAMinMax1  minmax1);
NA_IAPI NARecti     naMakeRectiWithMinMax1i (NAMinMax1i minmax0, NAMinMax1i minmax1);
NA_IAPI NAMinMax2   naMakeMinMax2WithRect   (NARect  rect);
NA_IAPI NAMinMax2i  naMakeMinMax2iWithRecti (NARecti rect);

// Copies the contents of source to dest
NA_IAPI void      naCpyPos       (NAPos*   posd,   NAPos*   poss);
NA_IAPI void      naCpyPosi      (NAPosi*  posd,   NAPosi*  poss);
NA_IAPI void      naCpySize      (NASize*  sized,  NASize*  sizes);
NA_IAPI void      naCpySizei     (NASizei* sized,  NASizei* sizes);
NA_IAPI void      naCpyRect      (NARect*  rectd,  NARect*  rects);
NA_IAPI void      naCpyRecti     (NARecti* rectd,  NARecti* rects);

// Create the bounding box of two elements. The size of the resulting rect will
// never be negative.
// The Variants with E also allow the rects to be empty.
NA_IAPI NARect    naMakeRectWithPosAndPos       (NAPos   pos1,  NAPos   pos2);
NA_IAPI NARect    naMakeRectWithRectAndPos      (NARect  rect,  NAPos   pos);
NA_IAPI NARect    naMakeRectWithRectAndRect     (NARect  rect1, NARect  rect2);
NA_IAPI NARect    naMakeRectWithRectAndRectE    (NARect  rect1, NARect  rect2);
NA_IAPI NARecti   naMakeRectiWithPosiAndPosi    (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NARecti   naMakeRectiWithRectiAndPosi   (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti   naMakeRectiWithRectiAndPosiE  (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti   naMakeRectiWithRectiAndRecti  (NARecti rect1, NARecti rect2);
NA_IAPI NARecti   naMakeRectiWithRectiAndRectiE (NARecti rect1, NARecti rect2);

// Creates a new rect by adding the given border around the given rect.
NA_IAPI NARect    naMakeRectWithRectAndBorder   (NARect  rect, double border);
NA_IAPI NARecti   naMakeRectiWithRectiAndBorder (NARecti rect, NAInt  border);

// Clamping functions. Returns the element which is completely within another.
NA_IAPI NAPos     naClampPosToRect(NAPos pos, NARect clamprect);
NA_IAPI NARect    naClampRectToRect(NARect rect, NARect clamprect);
NA_IAPI NAPosi    naClampPosiToRecti(NAPosi pos, NARecti clamprect);
NA_IAPI NARecti   naClampRectiToRecti(NARecti rect, NARecti clamprect);

// The same but wiht real and integer mixed.
NA_IAPI NAPos     naClampPosToRecti(NAPos pos, NARecti clamprect);

// Use the following functions to make equality tests (==) between elements.
NA_IAPI NABool    naEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naEqualSize (NASize  size1, NASize  size2);
NA_IAPI NABool    naEqualRect (NARect  rect1, NARect  rect2);
NA_IAPI NABool    naEqualPosi (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NABool    naEqualSizei(NASizei size1, NASizei size2);
NA_IAPI NABool    naEqualRecti(NARecti rect1, NARecti rect2);

// Adding and subtracting delta values: A delta is a size.
// Adding a size to a position returns a position. Subtraction of two positions
// from each other returns a size. 
// The returned NASize can be negative or even invalid.
NA_IAPI NAPos     naAddPosSize  (NAPos   pos,   NASize  size);
NA_IAPI NAPosi    naAddPosiSizei(NAPosi  pos,   NASizei size);
NA_IAPI NASize    naSubPosPos   (NAPos   pos1,  NAPos   pos2);
NA_IAPI NASizei   naSubPosiPosi (NAPosi  pos1,  NAPosi  pos2);

// Containing functions. Test if an element is within another.
NA_IAPI NABool    naIsPosInSize   (NAPos   pos,  NASize  outersize);
NA_IAPI NABool    naIsPosInRect   (NAPos   pos,  NARect  outerrect);
NA_IAPI NABool    naIsSizeInSize  (NASize  size, NASize  outersize);
NA_IAPI NABool    naIsRectInRect  (NARect  rect, NARect  outerrect);
NA_IAPI NABool    naIsPosiInSizei (NAPosi  pos,  NASizei outersize);
NA_IAPI NABool    naIsPosiInRecti (NAPosi  pos,  NARecti outerrect);
NA_IAPI NABool    naIsSizeiInSizei(NASizei size, NASizei outersize);
NA_IAPI NABool    naIsRectiInRecti(NARecti rect, NARecti outerrect);

// Get the End-Point of a rect. Is the same as pos + size.
NA_IAPI double    naGetRectEndX (NARect  rect);
NA_IAPI double    naGetRectEndY (NARect  rect);

// Raw Index functions. Assuming a 2-dimensional array with row-first
// or column-first ordering. Row-first is the default C ordering.
NA_IAPI NAUInt   naGetIndexWithOriginAndPosiRowFirst(   NAPosi origin,
                                                        NAPosi pos,
                                                        NAUInt width);
NA_IAPI NAUInt   naGetIndexWithOriginAndPosiColumnFirst(NAPosi origin,
                                                        NAPosi pos,
                                                        NAUInt height);

// Computes the positions opposite to the origin (pos) of the rect.
// End returns position + size
// Max returns position + size - 1
NA_IAPI NAPosi    naGetRectiEnd         (NARecti rect);
NA_IAPI NAInt     naGetRectiEndX        (NARecti rect);
NA_IAPI NAInt     naGetRectiEndY        (NARecti rect);
NA_IAPI NAPosi    naGetRectiEndPos      (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxX        (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxY        (NARecti rect);
NA_IAPI NAPosi    naGetRectiMaxPos      (NARecti rect);

// Count returns the total count width*height of a rect
// IndexOf returns the index of the corresponding 1-dimensional array.
// Note that row-first is the default C ordering.
NA_IAPI NAInt     naGetRectiIndexCount              (NARecti rect);
NA_IAPI NAUInt    naGetRectiIndexOfPosiRowFirst     (NARecti rect, NAPosi pos);
NA_IAPI NAUInt    naGetRectiIndexOfPosiColumnFirst  (NARecti rect, NAPosi pos);
// Example:    Rect(Pos(2, 5), Size(4, 3)) describes a 2D-field. It stores
//             a corresponding 1D-Array {a, b, c, d, e, f, g, h, i, j, k, l}.
// (a b c d)   End is (6, 8), Max is (5, 7). Count is 3*4 = 12
// (e f g h)   The result of naGetRectiIndexOfPosi with position (5, 6)
// (i j k l)   returns 7, the index of 'h'



// Testing functions
// -----------------
// Returns NA_TRUE when all values of the fields are valid. Will not perform
// any debugging tests.
NA_IAPI NABool    naIsPosValid  (NAPos   pos);
NA_IAPI NABool    naIsSizeValid (NASize  size);
NA_IAPI NABool    naIsRectValid (NARect  rect);
NA_IAPI NABool    naIsPosiValid (NAPosi  pos);
NA_IAPI NABool    naIsSizeiValid(NASizei size);
NA_IAPI NABool    naIsRectiValid(NARecti rect);

// Sizes with zero-entries.  Will not perform any debugging tests.
NA_IAPI NABool    naIsSizeEmpty (NASize  size);
NA_IAPI NABool    naIsRectEmpty (NARect  rect);
NA_IAPI NABool    naIsSizeiEmpty(NASizei size);
NA_IAPI NABool    naIsRectiEmpty(NARecti rect);

// Returns NA_TRUE for sizes with negative entries. Will not perform any
// debugging tests.
// Note from the author: These functions test for Negativity, not Positivity.
// Using a negative word in a function name is usually not good practice as it
// tends to lead to double negatives when reading the source code. There are
// situations though when using a negative word is acceptable or even better
// than its positive counterpart. Like here for example. Or the NDEBUG macro.
// Or the "End of file" marker EOF for example. But such situations are rare.
// Try to use positive words whenever possible! For example, test for "Inside"
// rather than "Outside".
NA_IAPI NABool    naIsSizeNegative (NASize  size);
NA_IAPI NABool    naIsRectNegative (NARect  rect);
NA_IAPI NABool    naIsSizeiNegative(NASizei size);
NA_IAPI NABool    naIsRectiNegative(NARecti rect);

// Returns NA_TRUE when all values of the element are useful. Will not perform
// any debugging tests. Useful elements are defined to be not invalid, not
// empty and not negative. Hence for example a rect is useful if it has no
// invalid values, is positive and has a size greater than zero.
// This function often is useful after an operation which might lead to
// negative sizes, empty rects or otherwise invalid values.
NA_IAPI NABool    naIsPosUseful  (NAPos   pos);
NA_IAPI NABool    naIsSizeUseful (NASize  size);
NA_IAPI NABool    naIsRectUseful (NARect  rect);
NA_IAPI NABool    naIsPosiUseful (NAPosi  pos);
NA_IAPI NABool    naIsSizeiUseful(NASizei size);
NA_IAPI NABool    naIsRectiUseful(NARecti rect);

// The naMakeRectPositive function ensures that the sizes of the given rect
// are not negative. If the sizes already were positive or zero, they will not
// be changed.
// For example:  A rect with pos(4., 6.) and size (-2., 7.) will change to
//            -> a rect with pos(2., 6.) and size ( 2., 7.) .
// But: An integer rect with pos(4 , 6 ) and size (-2 , 7 ) will change to
//            -> a rect with pos(3 , 6 ) and size ( 2 , 7 ) !
// Will not perform any debugging tests.
NA_IAPI NARect    naMakeRectPositive (NARect  rect);
NA_IAPI NARecti   naMakeRectiPositive(NARecti rect);













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NARange.h"


NA_IDEF NAPos naMakePos(double x, double y){
  NAPos newpos;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(x) && naIsPosValueValid(y)))
      naError("naMakePos", "Invalid values given.");
    if(!(naIsPosValueUseful(x) && naIsPosValueUseful(y)))
      naError("naMakePos", "Values given are not useful.");
  #endif
  newpos.x = x;
  newpos.y = y;
  return newpos;
}
NA_IDEF NASize naMakeSize(double width, double height){
  NASize newsize;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValid(width) && naIsSizeValueValid(height)))
      naError("naMakeSize", "Invalid values given.");
    if(!(naIsSizeValueUseful(width) && naIsSizeValueUseful(height)))
      naError("naMakeSize", "Values given are not useful.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASize naMakeSizeE(double width, double height){
  NASize newsize;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValid(width) && naIsSizeValueValid(height)))
      naError("naMakeSize", "Invalid values given.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NARect naMakeRect(NAPos pos, NASize size){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("naMakeRect", "Invalid values given.");
    if(!naIsPosUseful(pos) || !naIsSizeUseful(size))
      naError("naMakeRect", "Values given are not useful.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NARect naMakeRectE(NAPos pos, NASize size){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("naMakeRect", "Invalid values given.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NAPosi naMakePosi(NAInt x, NAInt y){
  NAPosi newpos;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(x) && naIsPosValueValidi(y)))
      naError("naMakePosi", "Invalid values given.");
    if(!(naIsPosValueUsefuli(x) && naIsPosValueUsefuli(y)))
      naError("naMakePosi", "Values given are not useful.");
  #endif
  newpos.x = x;
  newpos.y = y;
  return newpos;
}
NA_IDEF NASizei naMakeSizei(NAInt width, NAInt height){
  NASizei newsize;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValidi(width) && naIsSizeValueValidi(height)))
      naError("naMakeSizei", "Invalid values given.");
    if(!(naIsSizeValueUsefuli(width) && naIsSizeValueUsefuli(height)))
      naError("naMakeSizei", "Values given are not useful.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASizei naMakeSizeiE(NAInt width, NAInt height){
  NASizei newsize;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValidi(width) && naIsSizeValueValidi(height)))
      naError("naMakeSizei", "Invalid values given.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NARecti naMakeRecti(NAPosi pos, NASizei size){
  NARecti newrect;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("naMakeRecti", "Invalid values given.");
    if(!naIsPosiUseful(pos) || !naIsSizeiUseful(size))
      naError("naMakeRecti", "Values given are not useful.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NARecti naMakeRectiE(NAPosi pos, NASizei size){
  NARecti newrect;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("naMakeRecti", "Invalid values given.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}


#ifdef CGGEOMETRY_H_
NA_IDEF NAPos naMakePosWithCGPoint(CGPoint cgpoint){
  NAPos newpos;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(cgpoint.x) && naIsPosValueValid(cgpoint.y)))
      naError("naMakePosWithCGPoint", "Invalid values given.");
    if(!(naIsPosValueUseful(cgpoint.x) && naIsPosValueUseful(cgpoint.y)))
      naError("naMakePosWithCGPoint", "Values given are not useful.");
  #endif
  newpos.x = cgpoint.x;
  newpos.y = cgpoint.y;
  return newpos;
}
NA_IAPI NASize naMakeSizeWithCGSize(CGSize cgsize){
  NASize newsize;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValid(cgsize.width) && naIsSizeValueValid(cgsize.height)))
      naError("naMakeSizeWithCGSize", "Invalid values given.");
    if(!(naIsSizeValueUseful(cgsize.width) && naIsSizeValueUseful(cgsize.height)))
      naError("naMakeSizeWithCGSize", "Values given are not useful.");
  #endif
  newsize.width = cgsize.width;
  newsize.height = cgsize.height;
  return newsize;
}
NA_IAPI NARect naMakeRectWithCGRect(CGRect cgrect){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  newrect.pos = naMakePosWithCGPoint(cgrect.origin);
  newrect.size = naMakeSizeWithCGSize(cgrect.size);
  return newrect;
}
#endif


#if defined __OBJC__ && defined __AVAILABILITYMACROS__
NA_IAPI NAPos naMakePosWithNSPoint(NSPoint nspoint){
  NAPos newpos;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(nspoint.x) && naIsPosValueValid(nspoint.y)))
      naError("naMakePosWithNSPoint", "Invalid values given.");
    if(!(naIsPosValueUseful(nspoint.x) && naIsPosValueUseful(nspoint.y)))
      naError("naMakePosWithNSPoint", "Values given are not useful.");
  #endif
  newpos.x = nspoint.x;
  newpos.y = nspoint.y;
  return newpos;
}
NA_IAPI NASize naMakeSizeWithNSSize(NSSize nssize){
  NASize newsize;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValid(nssize.width) && naIsSizeValueValid(nssize.height)))
      naError("naMakeSizeWithNSSize", "Invalid values given.");
    if(!(naIsSizeValueUseful(nssize.width) && naIsSizeValueUseful(nssize.height)))
      naError("naMakeSizeWithNSSize", "Values given are not useful.");
  #endif
  newsize.width = nssize.width;
  newsize.height = nssize.height;
  return newsize;
}
NA_IAPI NARect naMakeRectWithNSRect(NSRect nsrect){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  newrect.pos = naMakePosWithNSPoint(nsrect.origin);
  newrect.size = naMakeSizeWithNSSize(nsrect.size);
  return newrect;
}
#endif

NA_IDEF NAPos naMakePosWithSizeCenteredInRect(NASize size, NARect rect){
  NAPos newpos;
  newpos.x = rect.pos.x + 0.5 * (rect.size.width - size.width);
  newpos.y = rect.pos.y + 0.5 * (rect.size.height - size.height);
  return newpos;
}


NA_IDEF NABounds4 naMakeBounds4(double top, double right, double bottom, double left){
  NABounds4 newbounds;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsSizeValueValid(top) || !naIsSizeValueValid(right) || !naIsSizeValueValid(bottom) || !naIsSizeValueValid(left))
      naError("naMakeBounds4", "Invalid values given.");
    if((top < 0.) || (right < 0.) || (bottom < 0.) || (left < 0.))
      naError("naMakeBounds4", "Invalid values given.");
  #endif
  newbounds.top = top;
  newbounds.right = right;
  newbounds.bottom = bottom;
  newbounds.left = left;
  return newbounds;
}


NA_IDEF NABounds4i naMakeBounds4i(NAInt top, NAInt right, NAInt bottom, NAInt left){
  NABounds4i newbounds;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsSizeValueValidi(top) || !naIsSizeValueValidi(right) || !naIsSizeValueValidi(bottom) || !naIsSizeValueValidi(left))
      naError("naMakeBounds4i", "Invalid values given.");
    if((top < 0) || (right < 0) || (bottom < 0) || (left < 0))
      naError("naMakeBounds4i", "Invalid values given.");
  #endif
  newbounds.top = top;
  newbounds.right = right;
  newbounds.bottom = bottom;
  newbounds.left = left;
  return newbounds;
}



NA_IDEF NAPosi naMakePosiWithAlignment(NAPosi pos, NARecti alignrect){
  #ifndef NDEBUG
    if((alignrect.size.width == 0) || (alignrect.size.height == 0)){
      naCrash("naMakePosiWithAlignment", "alignment rect is empty");
      return naMakePosi(0, 0);
    }
  #endif
  NAPosi newpos;
  pos.x -= alignrect.pos.x;
  pos.y -= alignrect.pos.y;
  if(pos.x < 0){
    newpos.x = (((pos.x + 1) / alignrect.size.width) - 1) * alignrect.size.width;
  }else{
    newpos.x = (pos.x / alignrect.size.width) * alignrect.size.width;
  }
  if(pos.y < 0){
    newpos.y = (((pos.y + 1) / alignrect.size.height) - 1) * alignrect.size.height;
  }else{
    newpos.y = (pos.y / alignrect.size.height) * alignrect.size.height;
  }
  newpos.x += alignrect.pos.x;
  newpos.y += alignrect.pos.y;
  return newpos;
}



NA_IDEF NARect naMakeRectWithMinMax1(NAMinMax1 minmax0, NAMinMax1 minmax1){
  NARect newrect;
  newrect.pos.x = minmax0.start[0];
  newrect.pos.y = minmax1.start[0];
  newrect.size.width  = naStartAndEndToSize(minmax0.start[0], minmax0.end[0]);
  newrect.size.height = naStartAndEndToSize(minmax1.start[0], minmax1.end[0]);
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithMinMax1i(NAMinMax1i minmax0, NAMinMax1i minmax1){
  NARecti newrect;
  newrect.pos.x = minmax0.min[0];
  newrect.pos.y = minmax1.min[0];
  newrect.size.width  = naMinAndMaxToSizei(minmax0.min[0], minmax0.max[0]);
  newrect.size.height = naMinAndMaxToSizei(minmax1.min[0], minmax1.max[0]);
  return newrect;
}


NA_IDEF NAMinMax2 naMakeMinMax2WithRect(NARect rect){
  NAMinMax2 newminmax;
  newminmax.start[0] = rect.pos.x;
  newminmax.end[0] = naGetRectEndY(rect);
  newminmax.start[1] = rect.pos.y;
  newminmax.end[1] = naGetRectEndY(rect);
  return newminmax;
}
NA_IDEF NAMinMax2i naMakeMinMax2iWithRecti(NARecti rect){
  NAMinMax2i newminmax;
  newminmax.min[0] = rect.pos.x;
  newminmax.max[0] = naGetRectiMaxX(rect);
  newminmax.min[1] = rect.pos.y;
  newminmax.max[1] = naGetRectiMaxY(rect);
  return newminmax;
}


NA_IDEF void naCpyPos(NAPos* posd,  NAPos* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCpyPosi(NAPosi* posd,  NAPosi* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCpySize(NASize* sized,  NASize* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCpySizei(NASizei* sized,  NASizei* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCpyRect(NARect* rectd,  NARect* rects){
  rectd->pos.x = rects->pos.x;
  rectd->pos.y = rects->pos.y;
  rectd->size.width  = rects->size.width;
  rectd->size.height = rects->size.height;
}
NA_IDEF void naCpyRecti(NARecti* rectd,  NARecti* rects){
  rectd->pos.x = rects->pos.x;
  rectd->pos.y = rects->pos.y;
  rectd->size.width  = rects->size.width;
  rectd->size.height = rects->size.height;
}



NA_IDEF NARect naMakeRectWithPosAndPos(NAPos pos1, NAPos pos2){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naMakeRectWithPosAndPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naMakeRectWithPosAndPos", "pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x){
    newrect.pos.x = pos1.x;
    newrect.size.width = naStartAndEndToSize(pos1.x, pos2.x);
  }else{
    newrect.pos.x = pos2.x;
    newrect.size.width = naStartAndEndToSize(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newrect.pos.y = pos1.y;
    newrect.size.height = naStartAndEndToSize(pos1.y, pos2.y);
  }else{
    newrect.pos.y = pos2.y;
    newrect.size.height = naStartAndEndToSize(pos2.y, pos1.y);
  }
  return newrect;
}


NA_IDEF NARect naMakeRectWithRectAndPos(NARect rect, NAPos pos){
  // Declaration before implementation. Needed for C90.
  NARect newrect;
  double end;
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naMakeRectWithRectAndPos", "rect is empty.");
    if(!naIsRectUseful(rect))
      naError("naMakeRectWithRectAndPos", "rect is not useful.");
    if(!naIsPosValid(pos))
      naError("naMakeRectWithRectAndPos", "pos is invalid.");
  #endif
  newrect.pos.x = naMin(rect.pos.x, pos.x);
  newrect.pos.y = naMin(rect.pos.y, pos.y);
  end = naGetRectEndX(rect);
  newrect.size.width  = naStartAndEndToSize(newrect.pos.x, naMax(end, pos.x));
  end = naGetRectEndY(rect);
  newrect.size.height = naStartAndEndToSize(newrect.pos.y, naMax(end, pos.y));
  return newrect;
}



NA_IDEF NARect naMakeRectWithRectAndRect(NARect rect1, NARect rect2){
  // Declaration before implementation. Needed for C90.
  NARect newrect;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(naIsRectEmpty(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is empty.");
    if(!naIsRectValid(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is invalid.");
    if(naIsRectEmpty(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is empty.");
    if(!naIsRectValid(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMin(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMin(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newrect.size.width  = naStartAndEndToSize(newrect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naStartAndEndToSize(newrect.pos.y, naMax(end1, end2));
  return newrect;
}


NA_IDEF NARect naMakeRectWithRectAndRectE(NARect rect1, NARect rect2){
  // Declaration before implementation. Needed for C90.
  NARect newrect;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(!naIsRectValid(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is invalid.");
  #endif
  if(naIsRectEmpty(rect1)){return rect2;}
  if(naIsRectEmpty(rect2)){return rect1;}
  newrect.pos.x = naMin(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMin(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newrect.size.width  = naStartAndEndToSize(newrect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naStartAndEndToSize(newrect.pos.y, naMax(end1, end2));
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithPosiAndPosi(NAPosi pos1, NAPosi pos2){
  NARecti newrect;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naMakeRectiWithPosiAndPosi", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naMakeRectiWithPosiAndPosi", "pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x){
    newrect.pos.x = pos1.x;
    newrect.size.width = naMinAndMaxToSizei(pos1.x, pos2.x);
  }else{
    newrect.pos.x = pos2.x;
    newrect.size.width = naMinAndMaxToSizei(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newrect.pos.y = pos1.y;
    newrect.size.height = naMinAndMaxToSizei(pos1.y, pos2.y);
  }else{
    newrect.pos.y = pos2.y;
    newrect.size.height = naMinAndMaxToSizei(pos2.y, pos1.y);
  }
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndPosi(NARecti rect, NAPosi pos){
  // Declaration before implementation. Needed for C90.
  NARecti newrect;
  NAInt max;
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naMakeRectiWithRectiAndPosi", "rect is empty.");
    if(!naIsRectiUseful(rect))
      naError("naMakeRectiWithRectiAndPosi", "rect is not useful.");
    if(!naIsPosiValid(pos))
      naError("naMakeRectiWithRectiAndPosi", "pos is invalid.");
  #endif
  newrect.pos.x = naMini(rect.pos.x, pos.x);
  newrect.pos.y = naMini(rect.pos.y, pos.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRectiMaxX(rect);
  newrect.size.width  = naMinAndMaxToSizei(newrect.pos.x, naMaxi(max, pos.x));
  max = naGetRectiMaxY(rect);
  newrect.size.height = naMinAndMaxToSizei(newrect.pos.y, naMaxi(max, pos.y));
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndPosiE(NARecti rect, NAPosi pos){
  // Declaration before implementation. Needed for C90.
  NARecti newrect;
  NAInt max;
  #ifndef NDEBUG
    if(!naIsRectiValid(rect))
      naError("naMakeRectiWithRectiAndPosi", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naMakeRectiWithRectiAndPosi", "pos is invalid.");
  #endif
  if(naIsRectiEmpty(rect)){
    newrect.pos = pos;
    newrect.size = naMakeSizei(1, 1);
  }else{
    newrect.pos.x = naMini(rect.pos.x, pos.x);
    newrect.pos.y = naMini(rect.pos.y, pos.y);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetRectiMaxX(rect);
    newrect.size.width  = naMinAndMaxToSizei(newrect.pos.x, naMaxi(max, pos.x));
    max = naGetRectiMaxY(rect);
    newrect.size.height = naMinAndMaxToSizei(newrect.pos.y, naMaxi(max, pos.y));
  }
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndRecti(NARecti rect1, NARecti rect2){
  // Declaration before implementation. Needed for C90.
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is empty.");
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is invalid.");
    if(naIsRectiEmpty(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is empty.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naStartAndEndToSizei(newrect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height = naStartAndEndToSizei(newrect.pos.y, naMaxi(end1, end2));
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndRectiE(NARecti rect1, NARecti rect2){
  // Declaration before implementation. Needed for C90.
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is invalid.");
  #endif
  if(naIsRectiEmpty(rect1)){return rect2;}
  if(naIsRectiEmpty(rect2)){return rect1;}
  newrect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naStartAndEndToSizei(newrect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height = naStartAndEndToSizei(newrect.pos.y, naMaxi(end1, end2));
  return newrect;
}



NA_IDEF NARect naMakeRectWithRectAndBorder(NARect rect, double border){
  NARect newrect;
  newrect.pos.x = rect.pos.x - border;
  newrect.pos.y = rect.pos.y - border;
  newrect.size.width = rect.size.width + 2 * border;
  newrect.size.height = rect.size.height + 2 * border;
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndBorder(NARecti rect, NAInt border){
  NARecti newrect;
  newrect.pos.x = rect.pos.x - border;
  newrect.pos.y = rect.pos.y - border;
  newrect.size.width = rect.size.width + 2 * border;
  newrect.size.height = rect.size.height + 2 * border;
  return newrect;
}



NA_IDEF NAPos naClampPosToRect(NAPos pos, NARect clamprect){
  // Declaration before implementation. Needed for C90.
  double end;
  NAPos newpos;
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naClampPosToRect", "pos is invalid.");
    if(naIsRectEmpty(clamprect))
      naError("naClampPosToRect", "clamprect is empty.");
    if(!naIsRectValid(clamprect))
      naError("naClampPosToRect", "clamprect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x < clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  end = naGetRectEndX(clamprect);
  if(newpos.x > end){newpos.x = end;}
  if(newpos.y < clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  end = naGetRectEndY(clamprect);
  if(newpos.y > end){newpos.y = end;}
  return newpos;
}


NA_IDEF NARect naClampRectToRect(NARect rect, NARect clamprect){
  // Todo: use helper functions of MinMax in this function.
  // Declaration before implementation. Needed for C90.
  double value;
  NARect newrect;
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naClampRectToRect", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naClampRectToRect", "rect is invalid.");
    if(naIsRectEmpty(clamprect))
      naError("naClampRectToRect", "clamprect is empty.");
    if(!naIsRectValid(clamprect))
      naError("naClampRectToRect", "clamprect is invalid.");
  #endif
  newrect = rect;
  // Adjust horizontally
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectEndX(clamprect);
  if(naGetRectEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
  }
  
  // Adjust vertically
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectEndY(clamprect);
  if(naGetRectEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
  }
  return newrect;
}


NA_IDEF NAPosi naClampPosiToRecti(NAPosi pos, NARecti clamprect){
  // Declaration before implementation. Needed for C90.
  NAInt max;
  NAPosi newpos;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naClampPosiToRecti", "pos is invalid.");
    if(naIsRectiEmpty(clamprect))
      naError("naClampPosiToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampPosiToRecti", "clamprect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x <  clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  max = naGetRectiMaxX(clamprect);
  if(newpos.x > max){newpos.x = max;}
  if(newpos.y <  clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  max = naGetRectiMaxY(clamprect);
  if(newpos.y > max){newpos.y = max;}
  return newpos;
}


NA_IDEF NARecti naClampRectiToRecti(NARecti rect, NARecti clamprect){
  // Todo: use helper functions of MinMax in this function.
  // Declaration before implementation. Needed for C90.
  NAInt value;
  NARecti newrect;
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naClampRectiToRecti", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naClampRectiToRecti", "rect is invalid.");
    if(naIsRectiEmpty(clamprect))
      naError("naClampRectiToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampRectiToRecti", "clamprect is invalid.");
  #endif

  newrect = rect;

  // Adjust horizontally
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectiEndX(clamprect);
  if(naGetRectiEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
  }
  
  // Adjust vertically
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectiEndY(clamprect);
  if(naGetRectiEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
  }
  return newrect;
}



NA_IDEF NAPos naClampPosToRecti(NAPos pos, NARecti clamprect){
  // Declaration before implementation. Needed for C90.
  NAInt max;
  NAPos newpos;
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naClampPosToRecti", "pos is invalid.");
    if(naIsRectiEmpty(clamprect))
      naError("naClampPosToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampPosToRecti", "clamprect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x <  clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  max = naGetRectiMaxX(clamprect);
  if(newpos.x > max){newpos.x = max;}
  if(newpos.y <  clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  max = naGetRectiMaxY(clamprect);
  if(newpos.y > max){newpos.y = max;}
  return newpos;
}



NA_IDEF NABool naEqualPos(NAPos pos1, NAPos pos2){
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naEqualPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naEqualPos", "pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualSize(NASize size1, NASize size2){
  #ifndef NDEBUG
    if(naIsSizeEmpty(size1) || naIsSizeEmpty(size2))
      naError("naEqualSize", "Equality test not valid for empty sizes.");
    if(!naIsSizeValid(size1))
      naError("naEqualSize", "size1 is invalid.");
    if(!naIsSizeValid(size2))
      naError("naEqualSize", "size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualRect(NARect rect1, NARect rect2){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect1) || naIsRectEmpty(rect2))
      naError("naEqualRect", "Equality test not valid for empty rects.");
    if(!naIsRectValid(rect1))
      naError("naEqualRect", "rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("naEqualRect", "rect2 is invalid.");
  #endif
  return     (naEqualPos (rect1.pos,  rect2.pos)
          &&  naEqualSize(rect1.size, rect2.size));
}
NA_IDEF NABool naEqualPosi(NAPosi pos1, NAPosi pos2){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naEqualPosi", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naEqualPosi", "pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualSizei(NASizei size1, NASizei size2){
  #ifndef NDEBUG
    if(naIsSizeiEmpty(size1) || naIsSizeiEmpty(size2))
      naError("naEqualSizei", "Equality test not valid for empty sizes.");
    if(!naIsSizeiValid(size1))
      naError("naEqualSizei", "size1 is invalid.");
    if(!naIsSizeiValid(size2))
      naError("naEqualSizei", "size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualRecti(NARecti rect1, NARecti rect2){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect1) || naIsRectiEmpty(rect2))
      naError("naEqualRecti", "Equality test not valid for empty rects.");
    if(!naIsRectiValid(rect1))
      naError("naEqualRecti", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naEqualRecti", "rect2 is invalid.");
  #endif
  return    (naEqualPosi (rect1.pos,  rect2.pos)
          && naEqualSizei(rect1.size, rect2.size));
}




NA_IDEF NAPos naAddPosSize(NAPos pos, NASize size){
  NAPos newpos;
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naAddPosSize", "pos is invalid.");
    if(!naIsSizeValid(size))
      naError("naAddPosSize", "size is invalid.");
  #endif
  newpos.x = pos.x + size.width;
  newpos.y = pos.y + size.height;
  return newpos;
}
NA_IDEF NAPosi naAddPosiSizei(NAPosi pos, NASizei size){
  NAPosi newpos;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naAddPosiSizei", "pos is invalid.");
    if(!naIsSizeiValid(size))
      naError("naAddPosiSizei", "size is invalid.");
  #endif
  newpos.x = pos.x + size.width;
  newpos.y = pos.y + size.height;
  return newpos;
}

NA_IDEF NASize naSubPosPos(NAPos pos1, NAPos pos2){
  NASize newsize;
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naSubPosPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naSubPosPos", "pos2 is invalid.");
  #endif
  newsize.width  = pos1.x - pos2.x;
  newsize.height = pos1.y - pos2.y;
  return newsize;
}
NA_IDEF NASizei naSubPosiPosi(NAPosi pos1, NAPosi pos2){
  NASizei newsize;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naSubPosiPosi", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naSubPosiPosi", "pos2 is invalid.");
  #endif
  newsize.width  = pos1.x - pos2.x;
  newsize.height = pos1.y - pos2.y;
  return newsize;
}





NA_IDEF NABool naIsPosInSize(NAPos pos, NASize outersize){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naIsPosInSize", "pos is invalid.");
    if(!naIsSizeUseful(outersize))
      naError("naIsPosInSize", "Inside test not valid for sizes which are not useful.");
  #endif
  return !((pos.x < 0)
        || (pos.x > outersize.width)
        || (pos.y < 0)
        || (pos.y > outersize.height));
}
NA_IDEF NABool naIsPosInRect(NAPos pos, NARect outerrect){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naIsPosInRect", "pos is invalid.");
    if(naIsRectEmpty(outerrect))
      naError("naIsPosInRect", "Inside test not valid for empty rects.");
    if(!naIsRectValid(outerrect))
      naError("naIsPosInRect", "outerrect is invalid.");
  #endif
  return !((pos.x < outerrect.pos.x)
        || (pos.x > naGetRectEndX(outerrect))
        || (pos.y < outerrect.pos.y)
        || (pos.y > naGetRectEndY(outerrect)));
}
NA_IDEF NABool naIsSizeInSize(NASize size, NASize outersize){
  #ifndef NDEBUG
    if(!naIsSizeUseful(size))
      naError("naIsSizeInSize", "Inside test not valid if size is not useful.");
    if(!naIsSizeUseful(outersize))
      naError("naIsSizeInSize", "Inside test not valid if outersize is not useful.");
  #endif
  return !((size.width  > outersize.width)
        || (size.height > outersize.height));
}
NA_IDEF NABool naIsRectInRect(NARect rect, NARect outerrect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect) || naIsRectEmpty(outerrect))
      naError("naIsRectInRect", "Inside test not valid for empty rects.");
    if(!naIsRectValid(rect))
      naError("naIsRectInRect", "rect is invalid.");
    if(!naIsRectValid(outerrect))
      naError("naIsRectInRect", "outerrect is invalid.");
  #endif
  return !((rect.pos.x          < outerrect.pos.x)
        || (naGetRectEndX(rect) > naGetRectEndX(outerrect))
        || (rect.pos.y          < outerrect.pos.y)
        || (naGetRectEndY(rect) > naGetRectEndY(outerrect)));
}
NA_IDEF NABool naIsPosiInSizei(NAPosi pos, NASizei outersize){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naIsPosiInSizei", "pos is invalid.");
    if(naIsSizeiUseful(outersize))
      naError("naIsPosiInSizei", "Inside test not valid for sizes which are not useful.");
  #endif
  return  ((pos.x >= 0)
        && (pos.x <  outersize.width)
        && (pos.y >= 0)
        && (pos.y <  outersize.height));
}
NA_IDEF NABool naIsPosiInRecti(NAPosi pos, NARecti outerrect){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naIsPosiInRecti", "pos is invalid.");
    if(naIsRectiEmpty(outerrect))
      naError("naIsPosiInRecti", "Inside test not valid for empty rects.");
    if(!naIsRectiValid(outerrect))
      naError("naIsPosiInRecti", "outerrect is invalid.");
  #endif
  return  ((pos.x >= outerrect.pos.x)
        && (pos.x <  naGetRectiEndX(outerrect))
        && (pos.y >= outerrect.pos.y)
        && (pos.y <  naGetRectiEndY(outerrect)));
}
NA_IDEF NABool naIsSizeiInSizei(NASizei size, NASizei outersize){
  #ifndef NDEBUG
    if(!naIsSizeiUseful(size))
      naError("naIsSizeiInSizei", "Inside test not valid if size is not useful.");
    if(!naIsSizeiUseful(outersize))
      naError("naIsSizeiInSizei", "Inside test not valid if outersize is not useful.");
  #endif
  return !((size.width  > outersize.width)
        || (size.height > outersize.height));
}
NA_IDEF NABool naIsRectiInRecti(NARecti rect, NARecti outerrect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect) || naIsRectiEmpty(outerrect))
      naError("naIsRectiInRecti", "Inside test not valid for empty rects.");
    if(!naIsRectiValid(rect))
      naError("naIsRectiInRecti", "rect is invalid.");
    if(!naIsRectiValid(outerrect))
      naError("naIsRectiInRecti", "outerrect is invalid.");
  #endif
  return  ((rect.pos.x           >= outerrect.pos.x)
        && (naGetRectiEndX(rect) <=  naGetRectiEndX(outerrect))
        && (rect.pos.y           >= outerrect.pos.y)
        && (naGetRectiEndY(rect) <=  naGetRectiEndY(outerrect)));
}


NA_IDEF NAUInt naGetIndexWithOriginAndPosiRowFirst(NAPosi origin, NAPosi pos, NAUInt width){
  return (pos.y - origin.y) * width + (pos.x - origin.x);
}
NA_IDEF NAUInt naGetIndexWithOriginAndPosiColumnFirst(NAPosi origin, NAPosi pos, NAUInt height){
  return (pos.x - origin.x) * height + (pos.y - origin.y);
}


NA_IDEF double naGetRectEndX (NARect  rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndX", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndX", "rect is invalid.");
  #endif
  return naPosAndSizeToEnd(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndY (NARect  rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndY", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndY", "rect is invalid.");
  #endif
  return naPosAndSizeToEnd(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiEnd (NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiEnd", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEnd", "rect is invalid.");
  #endif
  return naMakePosi(naGetRectiEndX(rect), naGetRectiEndY(rect));
}
NA_IDEF NAInt naGetRectiEndX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiEndX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndX", "rect is invalid.");
  #endif
  return naPosAndSizeToEndi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiEndY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiEndY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndY", "rect is invalid.");
  #endif
  return naPosAndSizeToEndi(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiEndPos(NARecti rect){
  NAPosi newpos;
  newpos.x = naGetRectiEndX(rect);
  newpos.y = naGetRectiEndY(rect);
  return newpos;
}
NA_IDEF NAInt naGetRectiMaxX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiMaxX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxX", "rect is invalid.");
  #endif
  return naPosAndSizeToMaxi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiMaxY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiMaxY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxY", "rect is invalid.");
  #endif
  return naPosAndSizeToMaxi(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiMaxPos(NARecti rect){
  NAPosi newpos;
  newpos.x = naGetRectiMaxX(rect);
  newpos.y = naGetRectiMaxY(rect);
  return newpos;
}
NA_IDEF NAInt naGetRectiIndexCount(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiMaxY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxY", "rect is invalid.");
  #endif
  return rect.size.width * rect.size.height;
}
NA_IDEF NAUInt naGetRectiIndexOfPosiRowFirst(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiIndexOfPosi", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosi", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosi", "pos is invalid.");
    if(!naIsPosiInRecti(pos, rect))
      naError("naGetRectiIndexOfPosi", "pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosiRowFirst(rect.pos, pos, rect.size.width);
}
NA_IDEF NAUInt naGetRectiIndexOfPosiColumnFirst(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect))
      naError("naGetRectiIndexOfPosi", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosi", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosi", "pos is invalid.");
    if(!naIsPosiInRecti(pos, rect))
      naError("naGetRectiIndexOfPosi", "pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosiColumnFirst(rect.pos, pos, rect.size.height);
}


NA_IDEF NABool naIsPosValid(NAPos pos){
  return naIsPosValueValid(pos.x) && naIsPosValueValid(pos.y);
}
NA_IDEF NABool naIsSizeValid(NASize size){
  return naIsSizeValueValid(size.width) && naIsSizeValueValid(size.height);
}
NA_IDEF NABool naIsRectValid(NARect rect){
  return (naIsPosValid(rect.pos) && naIsSizeValid(rect.size));
}
NA_IDEF NABool naIsPosiValid(NAPosi pos){
  return naIsPosValueValidi(pos.x) && naIsPosValueValidi(pos.y);
}
NA_IDEF NABool naIsSizeiValid(NASizei size){
  return naIsSizeValueValidi(size.width) && naIsSizeValueValidi(size.height);
}
NA_IDEF NABool naIsRectiValid(NARecti rect){
  return (naIsPosiValid(rect.pos) && naIsSizeiValid(rect.size));
}


NA_IDEF NABool naIsSizeEmpty (NASize  size){
  return naIsSizeValueEmpty(size.width * size.height);
}
NA_IDEF NABool naIsRectEmpty (NARect  rect){
  return naIsSizeValueEmpty(rect.size.width * rect.size.height);
}
NA_IDEF NABool naIsSizeiEmpty(NASizei size){
  return naIsSizeValueEmptyi(size.width * size.height);
}
NA_IDEF NABool naIsRectiEmpty(NARecti rect){
  return naIsSizeValueEmpty(rect.size.width * rect.size.height);
}


NA_IDEF NABool naIsSizeNegative(NASize size){
  return (naIsSizeValueNegative(size.width) || naIsSizeValueNegative(size.height));
}
NA_IDEF NABool naIsRectNegative(NARect rect){
  return naIsSizeNegative(rect.size);
}
NA_IDEF NABool naIsSizeiNegative(NASizei size){
  return (naIsSizeValueNegativei(size.width) || naIsSizeValueNegativei(size.height));
}
NA_IDEF NABool naIsRectiNegative(NARecti rect){
  return naIsSizeiNegative(rect.size);
}


NA_IDEF NABool naIsPosUseful(NAPos pos){
  return naIsPosValueUseful(pos.x) && naIsPosValueUseful(pos.y);
}
NA_IDEF NABool naIsSizeUseful(NASize size){
  return naIsSizeValueUseful(size.width) && naIsSizeValueUseful(size.height);
}
NA_IDEF NABool naIsRectUseful(NARect rect){
  return (naIsPosUseful(rect.pos) && naIsSizeUseful(rect.size));
}
NA_IDEF NABool naIsPosiUseful(NAPosi pos){
  return naIsPosValueUsefuli(pos.x) && naIsPosValueUsefuli(pos.y);
}
NA_IDEF NABool naIsSizeiUseful(NASizei size){
  return naIsSizeValueUsefuli(size.width) && naIsSizeValueUsefuli(size.height);
}
NA_IDEF NABool naIsRectiUseful(NARecti rect){
  return (naIsPosiUseful(rect.pos) && naIsSizeiUseful(rect.size));
}




NA_IDEF NARect naMakeRectPositive(NARect rect){
  NARect newrect;
  newrect = rect; // Do not put this into the initialization. The compiler
                  // might not be able to optimize that.
  naMakePositive(&(newrect.pos.x), &(newrect.size.width));
  naMakePositive(&(newrect.pos.y), &(newrect.size.height));
  return newrect;
}
NA_IDEF NARecti naMakeRectiPositive(NARecti rect){
  NARecti newrect;
  newrect = rect; // Do not put this into the initialization. The compiler
                  // might not be able to optimize that.
  naMakePositivei(&(newrect.pos.x), &(newrect.size.width));
  naMakePositivei(&(newrect.pos.y), &(newrect.size.height));
  return newrect;
}








#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_COORD_INCLUDED

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
