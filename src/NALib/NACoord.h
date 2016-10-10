
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_COORD_INCLUDED
#define NA_COORD_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif



// Following are definitions for integer and floating-point elements storing
// positions and lenghts on 1-D, 2-D and 3-D coordinate systems. Namely:
//
// 1-D: NARange
// 2-D: NAPos, NASize, NARect, NABounds4
// 3-D: NAVertex, NAVolume, NABox
//
// When coordinate functions are used, they usually are used a lot hence having
// quite an impact on performance. Most of the functions could be written as
// macros. But in NALib, they are explicitely implemented as inline functions.
// This helps when debugging and reduces unwanted implicit arithmetic
// conversions.
//
// Further more in NALib, coordinate computation is always performed straight-
// forward with the assumption that everything works as normal, even if the
// values don't make sense. Only when debugging, a lot of checks are performed
// by NALib to find invalid values like negative sizes. Checking for invalid
// values when not debugging would increase runtime considerably. The
// implementation would also become very complicated as there are many special
// cases.
//
// Therefore, the programmer has to take care of invalid values. Use the debug
// checks and the testing functions in this file (far below). This should be
// doable and in the end helps avoiding nasty errors which are not uncommon
// with coordinate computations. Use these functions to make your code fly as
// well being very secure.
//
//
// Integer versus Floating-point
// -----------------------------
// Note that the integer variant of the structs behaves differently than
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


#include "NASystem.h"


// /////////////////////////////////
// TYPE DEFINITIONS


// 1-D Coordinates: Range
typedef struct NARange    NARange;
typedef struct NARangef   NARangef;
typedef struct NARangei   NARangei;

// 2-D Coordinates: Pos, Size and Rect
typedef struct NAPos      NAPos;
typedef struct NAPosi     NAPosi;
typedef struct NASize     NASize;
typedef struct NASizei    NASizei;
typedef struct NARect     NARect;
typedef struct NARecti    NARecti;

typedef struct NABounds4  NABounds4;
typedef struct NABounds4i NABounds4i;

// 3-D Coordinates: Vertex, Volume and Box
typedef struct NAVertex   NAVertex;
typedef struct NAVertexi  NAVertexi;
typedef struct NAVolume   NAVolume;
typedef struct NAVolumei  NAVolumei;
typedef struct NABox      NABox;
typedef struct NABoxi     NABoxi;

// The types are presented as non-opaque as the programmer wants to access the
// parts of the structs.
//
// Please excuse this formatting. It is usually not recommended but here, it
// would be much more confusing if written otherwise.
struct NARange{   double offset;   double length;};
struct NARangef{  float  offset;   float  length;};
struct NARangei{  NAInt  offset;   NAInt  length;};

struct NAPos{     double x;     double  y;     };
struct NAPosi{    NAInt  x;     NAInt   y;     };
struct NASize{    double width; double  height;};
struct NASizei{   NAInt  width; NAInt   height;};
struct NARect{    NAPos  pos;   NASize  size;  };
struct NARecti{   NAPosi pos;   NASizei size;  };

struct NABounds4{ double top;   double  right; double bottom; double left;};
struct NABounds4i{NAInt  top;   NAInt   right; NAInt  bottom; NAInt  left;};

struct NAVertex{  double    x;      double    y;        double  z;   };
struct NAVertexi{ NAInt     x;      NAInt     y;        NAInt   z;   };
struct NAVolume{  double    width;  double    height;   double depth;};
struct NAVolumei{ NAInt     width;  NAInt     height;   NAInt  depth;};
struct NABox{     NAVertex  vertex; NAVolume  volume;};
struct NABoxi{    NAVertexi vertex; NAVolumei volume;};





// Create the various elements
// The Variants with E also allow the sizes and rects to be empty or negative.
// The Variants with S expect scalar values instead of structs
NA_IAPI NARange  naMakeRange  (double offset, double length);
NA_IAPI NARangef naMakeRangef (float  offset, float  length);
NA_IAPI NARangei naMakeRangei (NAInt  offset, NAInt  length);

NA_IAPI NAPos    naMakePos    (double x,      double  y);
NA_IAPI NASize   naMakeSize   (double width,  double  height);
NA_IAPI NASize   naMakeSizeE  (double width,  double  height);
NA_IAPI NARect   naMakeRect   (NAPos  pos,    NASize  size);
NA_IAPI NARect   naMakeRectE  (NAPos  pos,    NASize  size);
NA_IAPI NARect   naMakeRectS  (double x, double y, double width, double height);
NA_IAPI NARect   naMakeRectSE (double x, double y, double width, double height);

NA_IAPI NAPosi   naMakePosi   (NAInt  x,      NAInt   y);
NA_IAPI NASizei  naMakeSizei  (NAInt  width,  NAInt   height);
NA_IAPI NASizei  naMakeSizeiE (NAInt  width,  NAInt   height);
NA_IAPI NARecti  naMakeRecti  (NAPosi pos,    NASizei size);
NA_IAPI NARecti  naMakeRectiE (NAPosi pos,    NASizei size);
NA_IAPI NARecti  naMakeRectiS (NAInt x, NAInt y, NAInt width, NAInt height);
NA_IAPI NARecti  naMakeRectiSE(NAInt x, NAInt y, NAInt width, NAInt height);

NA_IAPI NABounds4   naMakeBounds4 (double top,
                                   double right,
                                   double bottom,
                                   double left);
NA_IAPI NABounds4i  naMakeBounds4i(NAInt  top,
                                   NAInt  right,
                                   NAInt  bottom,
                                   NAInt  left);

NA_IAPI NAVertex    naMakeVertex  (double   x,     double y,      double z);
NA_IAPI NAVolume    naMakeVolume  (double   width, double height, double depth);
NA_IAPI NAVolume    naMakeVolumeE (double   width, double height, double depth);
NA_IAPI NABox       naMakeBox     (NAVertex pos,   NAVolume size);
NA_IAPI NABox       naMakeBoxE    (NAVertex pos,   NAVolume size);

NA_IAPI NAVertexi   naMakeVertexi (NAInt x,      NAInt y,       NAInt z);
NA_IAPI NAVolumei   naMakeVolumei (NAInt width,  NAInt height,  NAInt depth);
NA_IAPI NAVolumei   naMakeVolumeiE(NAInt width,  NAInt height,  NAInt depth);
NA_IAPI NABoxi      naMakeBoxi    (NAVertexi pos,  NAVolumei size);
NA_IAPI NABoxi      naMakeBoxiE   (NAVertexi pos,  NAVolumei size);


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
NA_IAPI NSRect    naMakeNSRectWithRect(NARect narect);
#endif


// Copies the contents of source to dest
NA_IAPI void      naCopyPos       (NAPos*   posd,   NAPos*   poss);
NA_IAPI void      naCopyPosi      (NAPosi*  posd,   NAPosi*  poss);
NA_IAPI void      naCopySize      (NASize*  sized,  NASize*  sizes);
NA_IAPI void      naCopySizei     (NASizei* sized,  NASizei* sizes);
NA_IAPI void      naCopyRect      (NARect*  rectd,  NARect*  rects);
NA_IAPI void      naCopyRecti     (NARecti* rectd,  NARecti* rects);

NA_IAPI void      naCopyVertex    (NAVertex*  vertexd,  NAVertex*  vertexs);
NA_IAPI void      naCopyVertexi   (NAVertexi* vertexd,  NAVertexi* vertexs);
NA_IAPI void      naCopyVolume    (NAVolume*  volumed,  NAVolume*  volumes);
NA_IAPI void      naCopyVolumei   (NAVolumei* volumed,  NAVolumei* volumes);
NA_IAPI void      naCopyBox       (NABox*  boxd,  NABox*  boxs);
NA_IAPI void      naCopyBoxi      (NABoxi* boxd,  NABoxi* boxs);

// Returns the starting offset of a size or volume being centered within a
// given rect or box.
NA_IAPI NAPos     naMakePosWithSizeCenteredInRect(NASize size, NARect rect);
NA_IAPI NAVertex  naMakeVertexWithVolumeCenteredInBox(NAVolume volume,
                                                         NABox box);

// Creates a new pos which is floored to a multiple of alignment. Also works
// for negative positions.
NA_IAPI NAPosi    naMakePosiWithAlignment(NAPosi pos, NARecti alignrect);
NA_IAPI NAVertexi naMakeVertexiWithAlignment(NAVertexi vertex, NABoxi alignbox);


// Create the bounding box of two elements. The size of the resulting rect will
// never be negative. Note that the integer variant of NARange also allows to
// be made with start and end instead with min and max.
//
// The Variants with E also allow the rects to be empty.
NA_IAPI NARange  naMakeRangeWithStartAndEnd (double start, double end);
NA_IAPI NARangef naMakeRangefWithStartAndEnd(float  start, float  end);
NA_IAPI NARangei naMakeRangeiWithStartAndEnd(NAInt  start, NAInt  end);
NA_IAPI NARangei naMakeRangeiWithMinAndMax  (NAInt  min,   NAInt  max);

NA_IAPI NARect    naMakeRectWithPosAndPos       (NAPos   pos1,  NAPos   pos2);
NA_IAPI NARect    naMakeRectWithRectAndPos      (NARect  rect,  NAPos   pos);
NA_IAPI NARect    naMakeRectWithRectAndRect     (NARect  rect1, NARect  rect2);
NA_IAPI NARect    naMakeRectWithRectAndRectE    (NARect  rect1, NARect  rect2);
NA_IAPI NARecti   naMakeRectiWithPosiAndPosi    (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NARecti   naMakeRectiWithRectiAndPosi   (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti   naMakeRectiWithRectiAndPosiE  (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti   naMakeRectiWithRectiAndRecti  (NARecti rect1, NARecti rect2);
NA_IAPI NARecti   naMakeRectiWithRectiAndRectiE (NARecti rect1, NARecti rect2);

NA_IAPI NABox  naMakeBoxWithVertexAndVertex     ( NAVertex vertex1,
                                                  NAVertex vertex2);
NA_IAPI NABox  naMakeBoxWithBoxAndVertex        (    NABox box,
                                                  NAVertex vertex);
NA_IAPI NABox  naMakeBoxWithBoxAndRect          (    NABox box1,
                                                     NABox box2);
NA_IAPI NABox  naMakeBoxWithBoxAndBoxE          (    NABox box1,
                                                     NABox box2);
NA_IAPI NABoxi naMakeBoxiWithVertexiAndVertexi  (NAVertexi vertex1,
                                                 NAVertexi vertex2);
NA_IAPI NABoxi naMakeBoxiWithBoxiAndVertexi     (   NABoxi box,
                                                 NAVertexi vertex);
NA_IAPI NABoxi naMakeBoxiWithBoxiAndVertexiE    (   NABoxi box,
                                                 NAVertexi vertex);
NA_IAPI NABoxi naMakeBoxiWithBoxiAndBoxi        (   NABoxi box1,
                                                    NABoxi box2);
NA_IAPI NABoxi naMakeBoxiWithBoxiAndBoxiE       (   NABoxi box1,
                                                    NABoxi box2);

// Returns the intersection of the two elements. May be empty or negative.
// Note: This is basically the inverse of naMakeBoxiWithBoxiAndBoxi. Use
// the naIsXXXUseful tests to test if the resulting element actually is a
// valid intersection.
NA_IAPI NARangei naMakeRangeiWithRangeiIntersection   ( NARangei  range1,
                                                        NARangei  range2);
NA_IAPI NARecti  naMakeRectiWithRectiIntersection     ( NARecti   rect1,
                                                        NARecti   rect2);
NA_IAPI NABoxi   naMakeBoxiWithBoxiIntersection       ( NABoxi    box1,
                                                        NABoxi    box2);

// Creates a new rect by adding the given border around the given rect.
NA_IAPI NARect    naMakeRectWithRectAndBorder   (NARect  rect, double border);
NA_IAPI NARecti   naMakeRectiWithRectiAndBorder (NARecti rect, NAInt  border);

NA_IAPI NABox     naMakeBoxWithBoxAndBorder   (NABox  box, double border);
NA_IAPI NABoxi    naMakeBoxiWithBoxiAndBorder (NABoxi box, NAInt border);

// Clamping functions. Returns the element which is completely within another.
NA_IAPI NAPos     naClampPosToRect(NAPos pos, NARect clamprect);
NA_IAPI NARect    naClampRectToRect(NARect rect, NARect clamprect);
NA_IAPI NAPosi    naClampPosiToRecti(NAPosi pos, NARecti clamprect);
NA_IAPI NARecti   naClampRectiToRecti(NARecti rect, NARecti clamprect);

NA_IAPI NAVertex  naClampVertexToBox(NAVertex vertex, NABox clampbox);
NA_IAPI NABox     naClampBoxToBox(NABox box, NABox clampbox);
NA_IAPI NAVertexi naClampVertexiToBoxi(NAVertexi vertex, NABoxi clampbox);
NA_IAPI NABoxi    naClampBoxiToBoxi(NABoxi box, NABoxi clampbox);

// Use the following functions to make equality tests (==) between elements.
NA_IAPI NABool    naEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naEqualSize (NASize  size1, NASize  size2);
NA_IAPI NABool    naEqualRect (NARect  rect1, NARect  rect2);
NA_IAPI NABool    naEqualPosi (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NABool    naEqualSizei(NASizei size1, NASizei size2);
NA_IAPI NABool    naEqualRecti(NARecti rect1, NARecti rect2);

NA_IAPI NABool    naEqualVertex (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naEqualVolume (NAVolume  volume1, NAVolume  volume2);
NA_IAPI NABool    naEqualBox    (NABox     box1,    NABox     box2);
NA_IAPI NABool    naEqualVertexi(NAVertexi vertex1, NAVertexi vertex2);
NA_IAPI NABool    naEqualVolumei(NAVolumei volume1, NAVolumei volume2);
NA_IAPI NABool    naEqualBoxi   (NABoxi    box1,    NABoxi    box2);

// Adding and subtracting delta values: A delta is a size or volume.
// Adding a size to a position returns a position. Subtracting two positions
// from each other returns a size.
//
// The returned NASize can be negative or even invalid.
NA_IAPI NAPos     naAddPosSize  (NAPos   pos,   NASize  size);
NA_IAPI NAPosi    naAddPosiSizei(NAPosi  pos,   NASizei size);
NA_IAPI NASize    naSubPosPos   (NAPos   pos1,  NAPos   pos2);
NA_IAPI NASizei   naSubPosiPosi (NAPosi  pos1,  NAPosi  pos2);

NA_IAPI NAVertex  naAddVertexVolume   (NAVertex   vertex,   NAVolume  volume);
NA_IAPI NAVertexi naAddVertexiVolumei (NAVertexi  vertex,   NAVolumei volume);
NA_IAPI NAVolume  naSubVertexPos      (NAVertex   vertex1,  NAVertex  vertex2);
NA_IAPI NAVolumei naSubVertexiVertexi (NAVertexi  vertex1,  NAVertexi vertex2);

// Containing functions. Test if an element is within another.
NA_IAPI NABool    naIsValueInRange    (double  value,  NARange  outerrange);
NA_IAPI NABool    naIsValueInRangef   (float   value,  NARangef outerrange);
NA_IAPI NABool    naIsValueInRangei   (NAInt   value,  NARangei outerrange);

NA_IAPI NABool    naIsPosInSize   (NAPos   pos,  NASize  outersize);
NA_IAPI NABool    naIsPosInRect   (NAPos   pos,  NARect  outerrect);
NA_IAPI NABool    naIsSizeInSize  (NASize  size, NASize  outersize);
NA_IAPI NABool    naIsRectInRect  (NARect  rect, NARect  outerrect);
NA_IAPI NABool    naIsPosiInSizei (NAPosi  pos,  NASizei outersize);
NA_IAPI NABool    naIsPosiInRecti (NAPosi  pos,  NARecti outerrect);
NA_IAPI NABool    naIsSizeiInSizei(NASizei size, NASizei outersize);
NA_IAPI NABool    naIsRectiInRecti(NARecti rect, NARecti outerrect);

NA_IAPI NABool    naIsVertexInVolume  (NAVertex  vertex, NAVolume  outervolume);
NA_IAPI NABool    naIsVertexInBox  (NAVertex  vertex, NABox  outerbox);
NA_IAPI NABool    naIsVolumeInVolume  (NAVolume  volume, NAVolume  outervolume);
NA_IAPI NABool    naIsBoxInBox  (NABox  box, NABox  outerbox);
NA_IAPI NABool    naIsVertexiInVolumei(NAVertexi vertex, NAVolumei outervolume);
NA_IAPI NABool    naIsVertexiInBoxi(NAVertexi vertex, NABoxi outerbox);
NA_IAPI NABool    naIsVolumeiInVolumei(NAVolumei volume, NAVolumei outervolume);
NA_IAPI NABool    naIsBoxiInBoxi(NABoxi box, NABoxi outerbox);

// Get the End-Point of a rect. Is the same as origin + size.
NA_IAPI double    naGetRangeEnd   (NARange  range);
NA_IAPI float     naGetRangefEnd  (NARangef range);

NA_IAPI double    naGetRectEndX (NARect  rect);
NA_IAPI double    naGetRectEndY (NARect  rect);

NA_IAPI double    naGetBoxEndX (NABox  box);
NA_IAPI double    naGetBoxEndY (NABox  box);
NA_IAPI double    naGetBoxEndZ (NABox  box);

// Computes the positions opposite to the origin (pos) of the rect.
// End returns position + size
// Max returns position + size - 1
NA_IAPI NAInt     naGetRangeiEnd      (NARangei range);
NA_IAPI NAInt     naGetRangeiMax      (NARangei range);

NA_IAPI NAPosi    naGetRectiEnd         (NARecti rect);
NA_IAPI NAInt     naGetRectiEndX        (NARecti rect);
NA_IAPI NAInt     naGetRectiEndY        (NARecti rect);
NA_IAPI NAPosi    naGetRectiEndPos      (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxX        (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxY        (NARecti rect);
NA_IAPI NAPosi    naGetRectiMaxPos      (NARecti rect);

NA_IAPI NAVertexi naGetBoxiEnd         (NABoxi box);
NA_IAPI NAInt     naGetBoxiEndX        (NABoxi box);
NA_IAPI NAInt     naGetBoxiEndY        (NABoxi box);
NA_IAPI NAInt     naGetBoxiEndZ        (NABoxi box);
NA_IAPI NAVertexi naGetBoxiEndVertex   (NABoxi box);
NA_IAPI NAInt     naGetBoxiMaxX        (NABoxi box);
NA_IAPI NAInt     naGetBoxiMaxY        (NABoxi box);
NA_IAPI NAInt     naGetBoxiMaxZ        (NABoxi box);
NA_IAPI NAVertexi naGetBoxiMaxVertex   (NABoxi box);

// Raw Index functions. Assuming a 2-dimensional or 3-dimensional array with
// row-first or column-first ordering. Row-first is the default C ordering.
NA_IAPI NAUInt   naGetIndexWithOriginAndPosiRowFirst(       NAPosi origin,
                                                            NAPosi pos,
                                                            NAUInt width);
NA_IAPI NAUInt   naGetIndexWithOriginAndPosiColumnFirst(    NAPosi origin,
                                                            NAPosi pos,
                                                            NAUInt height);
NA_IAPI NAUInt   naGetIndexWithOriginAndVertexiRowFirst(    NAVertexi origin,
                                                            NAVertexi vertex,
                                                            NAUInt width,
                                                            NAUInt height);
NA_IAPI NAUInt   naGetIndexWithOriginAndVertexiColumnFirst( NAVertexi origin,
                                                            NAVertexi vertex,
                                                            NAUInt depth,
                                                            NAUInt height);

// Count returns the total count width*height of a rect
// IndexOf returns the index of the corresponding 1-dimensional array.
// Note that row-first is the default C ordering.
NA_IAPI NAInt     naGetRectiIndexCount              (NARecti rect);
NA_IAPI NAUInt    naGetRectiIndexOfPosiRowFirst     (NARecti rect, NAPosi pos);
NA_IAPI NAUInt    naGetRectiIndexOfPosiColumnFirst  (NARecti rect, NAPosi pos);

NA_IAPI NAInt     naGetBoxiIndexCount                 (NABoxi box);
NA_IAPI NAUInt    naGetBoxiIndexOfVertexiRowFirst     (NABoxi box,
                                                          NAVertexi vertex);
NA_IAPI NAUInt    naGetBoxiIndexOfVertexiColumnFirst  (NABoxi box,
                                                          NAVertexi vertex);
// Example:    Rect(Pos(2, 5), Size(4, 3)) describes a 2D-field. It stores
//             a corresponding 1D-Array {a, b, c, d, e, f, g, h, i, j, k, l}.
// (a b c d)   End is (6, 8), Max is (5, 7). Count is 3*4 = 12
// (e f g h)   The result of naGetRectiIndexOfPosi with position (5, 6)
// (i j k l)   returns 7, the index of 'h'




// //////////////////////////////////////
// TESTING FUNCTIONS
//
// Following are the definitions of "valid" elements used in NALib:
// Pos:  A position is considered valid if none of the fields is NaN.
//       Integer positions are always valid.
// Size: A size is considered valid, if none of the fields is NaN.
//       A size of zero is considered valid because this is commonly used to
//       mark empty structures. But be careful! It is just NOT DEFINED as
//       invalid. This means: No special code is executed for empty elements!
//       Use the appropriate functions to check if an element is empty. And
//       use the functions marked with E if you are sure empty elements are ok.
// Rect: A rectangle is considered valid, if its position and size is valid.
//
// The same holds true similarly for the 1D- and 3D-elements.
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


// Returns NA_TRUE when all values of the fields are valid. Will not perform
// any debugging tests.
NA_IAPI NABool    naIsRangeValid    (NARange  range);
NA_IAPI NABool    naIsRangefValid   (NARangef range);
NA_IAPI NABool    naIsRangeiValid   (NARangei range);

NA_IAPI NABool    naIsPosValid      (NAPos   pos);
NA_IAPI NABool    naIsPosiValid     (NAPosi  pos);
NA_IAPI NABool    naIsSizeValid     (NASize  size);
NA_IAPI NABool    naIsSizeiValid    (NASizei size);
NA_IAPI NABool    naIsRectValid     (NARect  rect);
NA_IAPI NABool    naIsRectiValid    (NARecti rect);

NA_IAPI NABool    naIsVertexValid   (NAVertex   vertex);
NA_IAPI NABool    naIsVertexiValid  (NAVertexi  vertex);
NA_IAPI NABool    naIsVolumeValid   (NAVolume   volume);
NA_IAPI NABool    naIsVolumeiValid  (NAVolumei  volume);
NA_IAPI NABool    naIsBoxValid      (NABox      box);
NA_IAPI NABool    naIsBoxiValid     (NABoxi     box);

// Sizes with zero-entries.  Will not perform any debugging tests.
NA_IAPI NABool    naIsRangeEmpty    (NARange  range);
NA_IAPI NABool    naIsRangefEmpty   (NARangef range);
NA_IAPI NABool    naIsRangeiEmpty   (NARangei range);

// The slow method might be necessary if you experience integer overflows.
// NALib will tell you when debugging.
NA_IAPI NABool    naIsSizeEmpty     (NASize  size);
NA_IAPI NABool    naIsSizeiEmpty    (NASizei size);
NA_IAPI NABool    naIsSizeiEmptySlow(NASizei size);
NA_IAPI NABool    naIsRectEmpty     (NARect  rect);
NA_IAPI NABool    naIsRectiEmpty    (NARecti rect);
NA_IAPI NABool    naIsRectiEmptySlow(NARecti rect);

NA_IAPI NABool    naIsVolumeEmpty     (NAVolume   volume);
NA_IAPI NABool    naIsVolumeiEmpty    (NAVolumei  volume);
NA_IDEF NABool    naIsVolumeiEmptySlow(NAVolumei volume);
NA_IAPI NABool    naIsBoxEmpty        (NABox      box);
NA_IAPI NABool    naIsBoxiEmpty       (NABoxi     box);
NA_IAPI NABool    naIsBoxiEmptySlow   (NABoxi     box);

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
NA_IAPI NABool    naIsRangeNegative  (NARange  range);
NA_IAPI NABool    naIsRangefNegative (NARangef range);
NA_IAPI NABool    naIsRangeiNegative (NARangei range);

NA_IAPI NABool    naIsSizeNegative (NASize  size);
NA_IAPI NABool    naIsSizeiNegative(NASizei size);
NA_IAPI NABool    naIsRectNegative (NARect  rect);
NA_IAPI NABool    naIsRectiNegative(NARecti rect);

NA_IAPI NABool    naIsVolumeNegative (NAVolume  volume);
NA_IAPI NABool    naIsVolumeiNegative(NAVolumei volume);
NA_IAPI NABool    naIsBoxNegative (NABox  box);
NA_IAPI NABool    naIsBoxiNegative(NABoxi box);

// Returns NA_TRUE when all values of the element are useful. Will not perform
// any debugging tests. Useful elements are defined to be not invalid, not
// empty and not negative. Hence for example a rect is useful if it has no
// invalid values, is positive and has a size greater than zero.
// This function often is useful after an operation which might lead to
// negative sizes, empty rects or otherwise invalid values.
NA_IAPI NABool    naIsRangeUseful    (NARange  range);
NA_IAPI NABool    naIsRangefUseful   (NARangef range);
NA_IAPI NABool    naIsRangeiUseful   (NARangei range);

NA_IAPI NABool    naIsPosUseful  (NAPos   pos);
NA_IAPI NABool    naIsPosiUseful (NAPosi  pos);
NA_IAPI NABool    naIsSizeUseful (NASize  size);
NA_IAPI NABool    naIsSizeiUseful(NASizei size);
NA_IAPI NABool    naIsRectUseful (NARect  rect);
NA_IAPI NABool    naIsRectiUseful(NARecti rect);

NA_IAPI NABool    naIsVertexUseful  (NAVertex  vertex);
NA_IAPI NABool    naIsVertexiUseful (NAVertexi vertex);
NA_IAPI NABool    naIsVolumeUseful  (NAVolume  volume);
NA_IAPI NABool    naIsVolumeiUseful (NAVolumei volume);
NA_IAPI NABool    naIsBoxUseful  (NABox  box);
NA_IAPI NABool    naIsBoxiUseful (NABoxi box);

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

NA_IAPI NABox     naMakeBoxPositive (NABox  box);
NA_IAPI NABoxi    naMakeBoxiPositive(NABoxi box);





// Inline implementations are in a separate file:
#include "NAMath/NACoordII.h"






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
