
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_COORD_INCLUDED
#define NA_COORD_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

// /////////////////////////////////////
// Warning: This is one huge file!
//
// Coordinate functions are not especially difficult to understand or implement
// but take a lot of code writing. You are free to explore all the inline code
// underneath the API but there really isn't much to see.
// /////////////////////////////////////


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
NA_IAPI NARange     naMakeRange   (double offset, double length);
NA_IAPI NARangef    naMakeRangef  (float  offset, float  length);
NA_IAPI NARangei    naMakeRangei  (NAInt  offset, NAInt  length);

NA_IAPI NAPos       naMakePos     (double x,      double  y);
NA_IAPI NASize      naMakeSize    (double width,  double  height);
NA_IAPI NASize      naMakeSizeE   (double width,  double  height);
NA_IAPI NARect      naMakeRect    (NAPos  pos,    NASize  size);
NA_IAPI NARect      naMakeRectE   (NAPos  pos,    NASize  size);

NA_IAPI NAPosi      naMakePosi    (NAInt  x,      NAInt   y);
NA_IAPI NASizei     naMakeSizei   (NAInt  width,  NAInt   height);
NA_IAPI NASizei     naMakeSizeiE  (NAInt  width,  NAInt   height);
NA_IAPI NARecti     naMakeRecti   (NAPosi pos,    NASizei size);
NA_IAPI NARecti     naMakeRectiE  (NAPosi pos,    NASizei size);

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
// the naIsUseful tests to test if the resulting element actually is a valid
// intersection.
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

// Adding and subtracting delta values: A delta is a size.
// Adding a size to a position returns a position. Subtracting two positions
// from each other returns a size. 
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
// The same holds true similarly for the 3D-elements.
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












// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAMathOperators.h"
#include "NAValueHelper.h"


NA_IDEF NARange naMakeRange(double offset, double length){
  NARange newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(offset) && naIsSizeValueValid(length)))
      naError("naMakeRange", "Invalid values given.");
    if(!naIsSizeValueUseful(length))
      naError("naMakeRange", "length is not useful.");
  #endif
  newrange.offset = offset;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARangef naMakeRangef(float offset, float length){
  NARangef newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidf(offset) && naIsSizeValueValidf(length)))
      naError("naMakeRangef", "Invalid values given.");
    if(!naIsSizeValueUsefulf(length))
      naError("naMakeRange", "length is not useful.");
  #endif
  newrange.offset = offset;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARangei naMakeRangei(NAInt offset, NAInt length){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(offset) && naIsSizeValueValidi(length)))
      naError("naMakeRangei", "Invalid values given.");
    if(!naIsSizeValueUsefuli(length))
      naError("naMakeRange", "length is not useful.");
  #endif
  newrange.offset = offset;
  newrange.length = length;
  return newrange;
}



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



NA_IDEF NAVertex naMakeVertex(double x, double y, double z){
  NAVertex newvertex;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(x) && naIsPosValueValid(y) && naIsPosValueValid(z)))
      naError("naMakeVertex", "Invalid values given.");
    if(!(naIsPosValueUseful(x) && naIsPosValueUseful(y) && naIsPosValueUseful(z)))
      naError("naMakeVertex", "Values given are not useful.");
  #endif
  newvertex.x = x;
  newvertex.y = y;
  newvertex.z = z;
  return newvertex;
}
NA_IDEF NAVolume naMakeVolume(double width, double height, double depth){
  NAVolume newvolume;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValid(width) && naIsSizeValueValid(height) && naIsSizeValueValid(depth)))
      naError("naMakeVolume", "Invalid values given.");
    if(!(naIsSizeValueUseful(width) && naIsSizeValueUseful(height) && naIsSizeValueUseful(depth)))
      naError("naMakeVolume", "Values given are not useful.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolume naMakeVolumeE(double width, double height, double depth){
  NAVolume newvolume;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValid(width) && naIsSizeValueValid(height) && naIsSizeValueValid(depth)))
      naError("naMakeVolume", "Invalid values given.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NABox naMakeBox(NAVertex vertex, NAVolume volume){
  NABox newbox;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("naMakeBox", "Invalid values given.");
    if(!naIsVertexUseful(vertex) || !naIsVolumeUseful(volume))
      naError("naMakeBox", "Values given are not useful.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABox naMakeBoxE(NAVertex vertex, NAVolume volume){
  NABox newbox;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("naMakeBox", "Invalid values given.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}




NA_IDEF NAVertexi naMakeVertexi(NAInt x, NAInt y, NAInt z){
  NAVertexi newvertex;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(x) && naIsPosValueValidi(y) && naIsPosValueValidi(z)))
      naError("naMakeVertexi", "Invalid values given.");
    if(!(naIsPosValueUsefuli(x) && naIsPosValueUsefuli(y) && naIsPosValueUsefuli(z)))
      naError("naMakeVertexi", "Values given are not useful.");
  #endif
  newvertex.x = x;
  newvertex.y = y;
  newvertex.z = z;
  return newvertex;
}
NA_IDEF NAVolumei naMakeVolumei(NAInt width, NAInt height, NAInt depth){
  NAVolumei newvolume;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValidi(width) && naIsSizeValueValidi(height) && naIsSizeValueValidi(depth)))
      naError("naMakeVolumei", "Invalid values given.");
    if(!(naIsSizeValueUsefuli(width) && naIsSizeValueUsefuli(height) && naIsSizeValueUsefuli(depth)))
      naError("naMakeVolumei", "Values given are not useful.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumeiE(NAInt width, NAInt height, NAInt depth){
  NAVolumei newvolume;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsSizeValueValidi(width) && naIsSizeValueValidi(height) && naIsSizeValueValidi(depth)))
      naError("naMakeVolumei", "Invalid values given.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NABoxi naMakeBoxi(NAVertexi vertex, NAVolumei volume){
  NABoxi newbox;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("naMakeBoxi", "Invalid values given.");
    if(!naIsVertexiUseful(vertex) || !naIsVolumeiUseful(volume))
      naError("naMakeBoxi", "Values given are not useful.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiE(NAVertexi vertex, NAVolumei volume){
  NABoxi newbox;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("naMakeBoxi", "Invalid values given.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
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
NA_IDEF NASize naMakeSizeWithCGSize(CGSize cgsize){
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
NA_IDEF NARect naMakeRectWithCGRect(CGRect cgrect){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  newrect.pos = naMakePosWithCGPoint(cgrect.origin);
  newrect.size = naMakeSizeWithCGSize(cgrect.size);
  return newrect;
}
#endif


#if defined __OBJC__ && defined __AVAILABILITYMACROS__
NA_IDEF NAPos naMakePosWithNSPoint(NSPoint nspoint){
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
NA_IDEF NASize naMakeSizeWithNSSize(NSSize nssize){
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
NA_IDEF NARect naMakeRectWithNSRect(NSRect nsrect){
  NARect newrect;  // Declaration before implementation. Needed for C90.
  newrect.pos = naMakePosWithNSPoint(nsrect.origin);
  newrect.size = naMakeSizeWithNSSize(nsrect.size);
  return newrect;
}
NA_IDEF NSRect naMakeNSRectWithRect(NARect narect){
  NSRect newrect;  // Declaration before implementation. Needed for C90.
  newrect = NSMakeRect((CGFloat)narect.pos.x, (CGFloat)narect.pos.y, (CGFloat)narect.size.width, (CGFloat)narect.size.height);
  return newrect;
}
#endif



NA_IDEF void naCopyPos(NAPos* posd,  NAPos* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCopyPosi(NAPosi* posd,  NAPosi* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCopySize(NASize* sized,  NASize* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCopySizei(NASizei* sized,  NASizei* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCopyRect(NARect* rectd,  NARect* rects){
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}
NA_IDEF void naCopyRecti(NARecti* rectd,  NARecti* rects){
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}



NA_IDEF void naCopyVertex(NAVertex* vertexd,  NAVertex* vertexs){
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naCopyVertexi(NAVertexi* vertexd,  NAVertexi* vertexs){
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naCopyVolume(NAVolume* volumed,  NAVolume* volumes){
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naCopyVolumei(NAVolumei* volumed,  NAVolumei* volumes){
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naCopyBox(NABox* boxd,  NABox* boxs){
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}
NA_IDEF void naCopyBoxi(NABoxi* boxd,  NABoxi* boxs){
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}





NA_IDEF NAPos naMakePosWithSizeCenteredInRect(NASize size, NARect rect){
  NAPos newpos;
  newpos.x = rect.pos.x + 0.5 * (rect.size.width - size.width);
  newpos.y = rect.pos.y + 0.5 * (rect.size.height - size.height);
  return newpos;
}
NA_IDEF NAVertex naMakeVertexWithVolumeCenteredInBox(NAVolume volume, NABox box){
  NAVertex newvertex;
  newvertex.x = box.vertex.x + 0.5 * (box.volume.width - volume.width);
  newvertex.y = box.vertex.y + 0.5 * (box.volume.height - volume.height);
  newvertex.z = box.vertex.z + 0.5 * (box.volume.depth - volume.depth);
  return newvertex;
}




NA_IDEF NAInt naAlignCoord(NAInt x, NAInt offset, NAInt align){
  NAInt shiftx = x - offset;
  if(shiftx < 0){
    return ((((shiftx + 1) / align) - 1) * align) + offset;
  }else{
    return (((shiftx) / align) * align) + offset;
  }
}



NA_IDEF NAPosi naMakePosiWithAlignment(NAPosi pos, NARecti alignrect){
  NAPosi newpos;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(alignrect)){
      naCrash("naMakePosiWithAlignment", "alignment rect is empty");
      return naMakePosi(0, 0);
    }
  #endif
  newpos.x = naAlignCoord(pos.x, alignrect.pos.x, alignrect.size.width);
  newpos.y = naAlignCoord(pos.y, alignrect.pos.y, alignrect.size.height);
  return newpos;
}
NA_IDEF NAVertexi naMakeVertexiWithAlignment(NAVertexi vertex, NABoxi alignbox){
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(alignbox)){
      naCrash("naMakePosiWithAlignment", "alignment box is empty");
      return naMakeVertexi(0, 0, 0);
    }
  #endif
  newvertex.x = naAlignCoord(vertex.x, alignbox.vertex.x, alignbox.volume.width);
  newvertex.y = naAlignCoord(vertex.y, alignbox.vertex.y, alignbox.volume.height);
  newvertex.z = naAlignCoord(vertex.z, alignbox.vertex.z, alignbox.volume.depth);
  return newvertex;
}




NA_IDEF NARange naMakeRangeWithStartAndEnd(double start, double end){
  NARange newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValid(start) && naIsPosValueValid(end)))
      naError("naMakeRangeWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.offset = start;
  newrange.length = naStartAndEndToLength(start, end);
  return newrange;
}
NA_IDEF NARangef naMakeRangefWithStartAndEnd(float start, float end){
  NARangef newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidf(start) && naIsPosValueValidf(end)))
      naError("naMakeRangefWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.offset = start;
  newrange.length = naStartAndEndToLengthf(start, end);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithStartAndEnd(NAInt start, NAInt end){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(start) && naIsPosValueValidi(end)))
      naError("naMakeRangeiWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.offset = start;
  newrange.length = naStartAndEndToLengthi(start, end);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithMinAndMax(NAInt min, NAInt max){
  NARangei newrange; // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!(naIsPosValueValidi(min) && naIsPosValueValidi(max)))
      naError("naMakeRangeiWithMinAndMax", "Invalid values given.");
  #endif
  newrange.offset = min;
  newrange.length = naMinAndMaxToLengthi(min, max);
  return newrange;
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
    newrect.size.width = naStartAndEndToLength(pos1.x, pos2.x);
  }else{
    newrect.pos.x = pos2.x;
    newrect.size.width = naStartAndEndToLength(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newrect.pos.y = pos1.y;
    newrect.size.height = naStartAndEndToLength(pos1.y, pos2.y);
  }else{
    newrect.pos.y = pos2.y;
    newrect.size.height = naStartAndEndToLength(pos2.y, pos1.y);
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
  newrect.size.width  = naStartAndEndToLength(newrect.pos.x, naMax(end, pos.x));
  end = naGetRectEndY(rect);
  newrect.size.height = naStartAndEndToLength(newrect.pos.y, naMax(end, pos.y));
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
  newrect.size.width  = naStartAndEndToLength(newrect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naStartAndEndToLength(newrect.pos.y, naMax(end1, end2));
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
  newrect.size.width  = naStartAndEndToLength(newrect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naStartAndEndToLength(newrect.pos.y, naMax(end1, end2));
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
    newrect.size.width = naMinAndMaxToLengthi(pos1.x, pos2.x);
  }else{
    newrect.pos.x = pos2.x;
    newrect.size.width = naMinAndMaxToLengthi(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newrect.pos.y = pos1.y;
    newrect.size.height = naMinAndMaxToLengthi(pos1.y, pos2.y);
  }else{
    newrect.pos.y = pos2.y;
    newrect.size.height = naMinAndMaxToLengthi(pos2.y, pos1.y);
  }
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndPosi(NARecti rect, NAPosi pos){
  // Declaration before implementation. Needed for C90.
  NARecti newrect;
  NAInt max;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
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
  newrect.size.width  = naMinAndMaxToLengthi(newrect.pos.x, naMaxi(max, pos.x));
  max = naGetRectiMaxY(rect);
  newrect.size.height = naMinAndMaxToLengthi(newrect.pos.y, naMaxi(max, pos.y));
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
    newrect.size.width  = naMinAndMaxToLengthi(newrect.pos.x, naMaxi(max, pos.x));
    max = naGetRectiMaxY(rect);
    newrect.size.height = naMinAndMaxToLengthi(newrect.pos.y, naMaxi(max, pos.y));
  }
  return newrect;
}


NA_IDEF NARecti naMakeRectiWithRectiAndRecti(NARecti rect1, NARecti rect2){
  // Declaration before implementation. Needed for C90.
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is empty.");
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is invalid.");
    if(naIsRectiEmptySlow(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is empty.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naStartAndEndToLengthi(newrect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height = naStartAndEndToLengthi(newrect.pos.y, naMaxi(end1, end2));
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
  newrect.size.width  = naStartAndEndToLengthi(newrect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height = naStartAndEndToLengthi(newrect.pos.y, naMaxi(end1, end2));
  return newrect;
}



NA_IDEF NABox naMakeBoxWithVertexAndVertex(NAVertex vertex1, NAVertex vertex2){
  NABox newbox;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex1))
      naError("naMakeBoxWithVertexAndVertex", "vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("naMakeBoxWithVertexAndVertex", "vertex2 is invalid.");
  #endif
  if(vertex2.x > vertex1.x){
    newbox.vertex.x = vertex1.x;
    newbox.volume.width = naStartAndEndToLength(vertex1.x, vertex2.x);
  }else{
    newbox.vertex.x = vertex2.x;
    newbox.volume.width = naStartAndEndToLength(vertex2.x, vertex1.x);
  }
  if(vertex2.y > vertex1.y){
    newbox.vertex.y = vertex1.y;
    newbox.volume.height = naStartAndEndToLength(vertex1.y, vertex2.y);
  }else{
    newbox.vertex.y = vertex2.y;
    newbox.volume.height = naStartAndEndToLength(vertex2.y, vertex1.y);
  }
  if(vertex2.z > vertex1.z){
    newbox.vertex.z = vertex1.z;
    newbox.volume.depth = naStartAndEndToLength(vertex1.z, vertex2.z);
  }else{
    newbox.vertex.z = vertex2.z;
    newbox.volume.depth = naStartAndEndToLength(vertex2.z, vertex1.z);
  }
  return newbox;
}


NA_IDEF NABox naMakeBoxWithBoxAndVertex(NABox box, NAVertex vertex){
  // Declaration before implementation. Needed for C90.
  NABox newbox;
  double end;
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naMakeBoxWithBoxAndVertex", "box is empty.");
    if(!naIsBoxUseful(box))
      naError("naMakeBoxWithBoxAndVertex", "box is not useful.");
    if(!naIsVertexValid(vertex))
      naError("naMakeBoxWithBoxAndVertex", "vertex is invalid.");
  #endif
  newbox.vertex.x = naMin(box.vertex.x, vertex.x);
  newbox.vertex.y = naMin(box.vertex.y, vertex.y);
  newbox.vertex.z = naMin(box.vertex.z, vertex.z);
  end = naGetBoxEndX(box);
  newbox.volume.width  = naStartAndEndToLength(newbox.vertex.x, naMax(end, vertex.x));
  end = naGetBoxEndY(box);
  newbox.volume.height = naStartAndEndToLength(newbox.vertex.y, naMax(end, vertex.y));
  end = naGetBoxEndZ(box);
  newbox.volume.depth = naStartAndEndToLength(newbox.vertex.z, naMax(end, vertex.z));
  return newbox;
}



NA_IDEF NABox naMakeBoxWithBoxAndBox(NABox box1, NABox box2){
  // Declaration before implementation. Needed for C90.
  NABox newbox;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(naIsBoxEmpty(box1))
      naError("naMakeBoxWithBoxAndBox", "box1 is empty.");
    if(!naIsBoxValid(box1))
      naError("naMakeBoxWithBoxAndBox", "box1 is invalid.");
    if(naIsBoxEmpty(box2))
      naError("naMakeBoxWithBoxAndBox", "box2 is empty.");
    if(!naIsBoxValid(box2))
      naError("naMakeBoxWithBoxAndBox", "box2 is invalid.");
  #endif
  newbox.vertex.x = naMin(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMin(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMin(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newbox.volume.width  = naStartAndEndToLength(newbox.vertex.x, naMax(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newbox.volume.height  = naStartAndEndToLength(newbox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newbox.volume.depth  = naStartAndEndToLength(newbox.vertex.z, naMax(end1, end2));
  return newbox;
}


NA_IDEF NABox naMakeBoxWithBoxAndBoxE(NABox box1, NABox box2){
  // Declaration before implementation. Needed for C90.
  NABox newbox;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(!naIsBoxValid(box1))
      naError("naMakeBoxWithBoxAndBox", "box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("naMakeBoxWithBoxAndBox", "box2 is invalid.");
  #endif
  if(naIsBoxEmpty(box1)){return box2;}
  if(naIsBoxEmpty(box2)){return box1;}
  newbox.vertex.x = naMin(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMin(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMin(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newbox.volume.width  = naStartAndEndToLength(newbox.vertex.x, naMax(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newbox.volume.height  = naStartAndEndToLength(newbox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newbox.volume.depth  = naStartAndEndToLength(newbox.vertex.z, naMax(end1, end2));
  return newbox;
}


NA_IDEF NABoxi naMakeBoxiWithVertexiAndVertexi(NAVertexi vertex1, NAVertexi vertex2){
  NABoxi newbox;  // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex1))
      naError("naMakeBoxiWithVertexiAndVertexi", "vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("naMakeBoxiWithVertexiAndVertexi", "vertex2 is invalid.");
  #endif
  if(vertex2.x > vertex1.x){
    newbox.vertex.x = vertex1.x;
    newbox.volume.width = naMinAndMaxToLengthi(vertex1.x, vertex2.x);
  }else{
    newbox.vertex.x = vertex2.x;
    newbox.volume.width = naMinAndMaxToLengthi(vertex2.x, vertex1.x);
  }
  if(vertex2.y > vertex1.y){
    newbox.vertex.y = vertex1.y;
    newbox.volume.height = naMinAndMaxToLengthi(vertex1.y, vertex2.y);
  }else{
    newbox.vertex.y = vertex2.y;
    newbox.volume.height = naMinAndMaxToLengthi(vertex2.y, vertex1.y);
  }
  if(vertex2.z > vertex1.z){
    newbox.vertex.z = vertex1.z;
    newbox.volume.depth = naMinAndMaxToLengthi(vertex1.z, vertex2.z);
  }else{
    newbox.vertex.z = vertex2.z;
    newbox.volume.depth = naMinAndMaxToLengthi(vertex2.z, vertex1.z);
  }
  return newbox;
}


NA_IDEF NABoxi naMakeBoxiWithBoxiAndVertexi(NABoxi box, NAVertexi vertex){
  // Declaration before implementation. Needed for C90.
  NABoxi newbox;
  NAInt max;
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naMakeBoxiWithBoxiAndVertexi", "box is empty.");
    if(!naIsBoxiUseful(box))
      naError("naMakeBoxiWithBoxiAndVertexi", "box is not useful.");
    if(!naIsVertexiValid(vertex))
      naError("naMakeBoxiWithBoxiAndVertexi", "vertex is invalid.");
  #endif
  newbox.vertex.x = naMini(box.vertex.x, vertex.x);
  newbox.vertex.y = naMini(box.vertex.y, vertex.y);
  newbox.vertex.z = naMini(box.vertex.z, vertex.z);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetBoxiMaxX(box);
  newbox.volume.width  = naMinAndMaxToLengthi(newbox.vertex.x, naMaxi(max, vertex.x));
  max = naGetBoxiMaxY(box);
  newbox.volume.height = naMinAndMaxToLengthi(newbox.vertex.y, naMaxi(max, vertex.y));
  max = naGetBoxiMaxZ(box);
  newbox.volume.depth = naMinAndMaxToLengthi(newbox.vertex.z, naMaxi(max, vertex.z));
  return newbox;
}


NA_IDEF NABoxi naMakeBoxiWithBoxiAndVertexiE(NABoxi box, NAVertexi vertex){
  // Declaration before implementation. Needed for C90.
  NABoxi newbox;
  NAInt max;
  #ifndef NDEBUG
    if(!naIsBoxiValid(box))
      naError("naMakeBoxiWithBoxiAndVertexi", "box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("naMakeBoxiWithBoxiAndVertexi", "vertex is invalid.");
  #endif
  if(naIsBoxiEmpty(box)){
    newbox.vertex = vertex;
    newbox.volume = naMakeVolumei(1, 1, 1);
  }else{
    newbox.vertex.x = naMini(box.vertex.x, vertex.x);
    newbox.vertex.y = naMini(box.vertex.y, vertex.y);
    newbox.vertex.z = naMini(box.vertex.z, vertex.z);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetBoxiMaxX(box);
    newbox.volume.width  = naMinAndMaxToLengthi(newbox.vertex.x, naMaxi(max, vertex.x));
    max = naGetBoxiMaxY(box);
    newbox.volume.height = naMinAndMaxToLengthi(newbox.vertex.y, naMaxi(max, vertex.y));
    max = naGetBoxiMaxZ(box);
    newbox.volume.depth = naMinAndMaxToLengthi(newbox.vertex.z, naMaxi(max, vertex.z));
  }
  return newbox;
}


NA_IDEF NABoxi naMakeBoxiWithBoxiAndBoxi(NABoxi box1, NABoxi box2){
  // Declaration before implementation. Needed for C90.
  NABoxi newbox;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box1))
      naError("naMakeBoxiWithBoxiAndBoxi", "box1 is empty.");
    if(!naIsBoxiValid(box1))
      naError("naMakeBoxiWithBoxiAndBoxi", "box1 is invalid.");
    if(naIsBoxiEmptySlow(box2))
      naError("naMakeBoxiWithBoxiAndBoxi", "box2 is empty.");
    if(!naIsBoxiValid(box2))
      naError("naMakeBoxiWithBoxiAndBoxi", "box2 is invalid.");
  #endif
  newbox.vertex.x = naMini(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMini(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMini(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newbox.volume.width  = naStartAndEndToLengthi(newbox.vertex.x, naMaxi(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newbox.volume.height = naStartAndEndToLengthi(newbox.vertex.y, naMaxi(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newbox.volume.depth = naStartAndEndToLengthi(newbox.vertex.z, naMaxi(end1, end2));
  return newbox;
}


NA_IDEF NABoxi naMakeBoxiWithBoxiAndBoxiE(NABoxi box1, NABoxi box2){
  // Declaration before implementation. Needed for C90.
  NABoxi newbox;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsBoxiValid(box1))
      naError("naMakeBoxiWithBoxiAndBoxi", "box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("naMakeBoxiWithBoxiAndBoxi", "box2 is invalid.");
  #endif
  if(naIsBoxiEmpty(box1)){return box2;}
  if(naIsBoxiEmpty(box2)){return box1;}
  newbox.vertex.x = naMini(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMini(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMini(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newbox.volume.width  = naStartAndEndToLengthi(newbox.vertex.x, naMaxi(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newbox.volume.height = naStartAndEndToLengthi(newbox.vertex.y, naMaxi(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newbox.volume.depth = naStartAndEndToLengthi(newbox.vertex.z, naMaxi(end1, end2));
  return newbox;
}



NA_IDEF NARangei naMakeRangeiWithRangeiIntersection(NARangei range1, NARangei range2){
  NARangei newrange;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsRangeiValid(range1))
      naError("naMakeRangeiWithRangeiAndRangei", "range1 is invalid.");
    if(!naIsRangeiValid(range2))
      naError("naMakeRangeiWithRangeiAndRangei", "range2 is invalid.");
  #endif
  newrange.offset = naMaxi(range1.offset, range2.offset);
  end1 = naGetRangeiEnd(range1);
  end2 = naGetRangeiEnd(range2);
  newrange.length  = naStartAndEndToLengthi(newrange.offset, naMini(end1, end2));
  return newrange;
}
NA_IDEF NARecti naMakeRectiWithRectiIntersection(NARecti rect1, NARecti rect2){
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectiAndRecti", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectiAndRecti", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMaxi(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMaxi(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naStartAndEndToLengthi(newrect.pos.x, naMini(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height  = naStartAndEndToLengthi(newrect.pos.y, naMini(end1, end2));
  return newrect;
}
NA_IDEF NABoxi naMakeBoxiWithBoxiIntersection(NABoxi box1, NABoxi box2){
  NABoxi newbox;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsBoxiValid(box1))
      naError("naMakeBoxiWithBoxiAndBoxi", "box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("naMakeBoxiWithBoxiAndBoxi", "box2 is invalid.");
  #endif
  newbox.vertex.x = naMaxi(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMaxi(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMaxi(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newbox.volume.width  = naStartAndEndToLengthi(newbox.vertex.x, naMini(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newbox.volume.height  = naStartAndEndToLengthi(newbox.vertex.y, naMini(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newbox.volume.depth  = naStartAndEndToLengthi(newbox.vertex.z, naMini(end1, end2));
  return newbox;
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



NA_IDEF NABox naMakeBoxWithBoxAndBorder(NABox box, double border){
  NABox newbox;
  newbox.vertex.x = box.vertex.x - border;
  newbox.vertex.y = box.vertex.y - border;
  newbox.vertex.z = box.vertex.z - border;
  newbox.volume.width = box.volume.width + 2 * border;
  newbox.volume.height = box.volume.height + 2 * border;
  newbox.volume.depth = box.volume.depth + 2 * border;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxiAndBorder(NABoxi box, NAInt border){
  NABoxi newbox;
  newbox.vertex.x = box.vertex.x - border;
  newbox.vertex.y = box.vertex.y - border;
  newbox.vertex.z = box.vertex.z - border;
  newbox.volume.width = box.volume.width + 2 * border;
  newbox.volume.height = box.volume.height + 2 * border;
  newbox.volume.depth = box.volume.depth + 2 * border;
  return newbox;
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
  // Todo: use helper functions in this function.
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
  // Adjust in first dimension
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectEndX(clamprect);
  if(naGetRectEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
  }
  
  // Adjust in second dimension
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
    if(naIsRectiEmptySlow(clamprect))
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
  // Todo: use helper functions in this function.
  // Declaration before implementation. Needed for C90.
  NAInt value;
  NARecti newrect;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naClampRectiToRecti", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naClampRectiToRecti", "rect is invalid.");
    if(naIsRectiEmptySlow(clamprect))
      naError("naClampRectiToRecti", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampRectiToRecti", "clamprect is invalid.");
  #endif

  newrect = rect;

  // Adjust in first dimension
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    if(newrect.size.width <= 0){return newrect;}
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectiEndX(clamprect);
  if(naGetRectiEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
    if(newrect.size.width <= 0){return newrect;}
  }
  
  // Adjust in second dimension
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    if(newrect.size.height <= 0){return newrect;}
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectiEndY(clamprect);
  if(naGetRectiEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
    if(newrect.size.height <= 0){return newrect;}
  }
  return newrect;
}



NA_IDEF NAVertex naClampVertexToBox(NAVertex vertex, NABox clampbox){
  // Declaration before implementation. Needed for C90.
  double end;
  NAVertex newvertex;
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naClampVertexToBox", "vertex is invalid.");
    if(naIsBoxEmpty(clampbox))
      naError("naClampVertexToBox", "clampbox is empty.");
    if(!naIsBoxValid(clampbox))
      naError("naClampVertexToBox", "clampbox is invalid.");
  #endif
  newvertex = vertex;
  if(newvertex.x < clampbox.vertex.x) {newvertex.x = clampbox.vertex.x;}
  end = naGetBoxEndX(clampbox);
  if(newvertex.x > end){newvertex.x = end;}
  if(newvertex.y < clampbox.vertex.y) {newvertex.y = clampbox.vertex.y;}
  end = naGetBoxEndY(clampbox);
  if(newvertex.y > end){newvertex.y = end;}
  if(newvertex.z < clampbox.vertex.z) {newvertex.z = clampbox.vertex.z;}
  end = naGetBoxEndZ(clampbox);
  if(newvertex.z > end){newvertex.z = end;}
  return newvertex;
}


NA_IDEF NABox naClampBoxToBox(NABox box, NABox clampbox){
  // Todo: use helper functions in this function.
  // Declaration before implementation. Needed for C90.
  double value;
  NABox newbox;
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naClampBoxToBox", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naClampBoxToBox", "box is invalid.");
    if(naIsBoxEmpty(clampbox))
      naError("naClampBoxToBox", "clampbox is empty.");
    if(!naIsBoxValid(clampbox))
      naError("naClampBoxToBox", "clampbox is invalid.");
  #endif
  newbox = box;
  // Adjust in first dimension
  value = clampbox.vertex.x - newbox.vertex.x;
  if(value > 0){
    newbox.volume.width -= value;
    newbox.vertex.x = clampbox.vertex.x;
  }
  value = naGetBoxEndX(clampbox);
  if(naGetBoxEndX(newbox) > value){
    newbox.volume.width = value - newbox.vertex.x;
  }
  
  // Adjust in second dimension
  value = clampbox.vertex.y - newbox.vertex.y;
  if(value > 0){
    newbox.volume.height -= value;
    newbox.vertex.y = clampbox.vertex.y;
  }
  value = naGetBoxEndY(clampbox);
  if(naGetBoxEndY(newbox) > value){
    newbox.volume.height = value - newbox.vertex.y;
  }

  // Adjust in third dimension
  value = clampbox.vertex.z - newbox.vertex.z;
  if(value > 0){
    newbox.volume.depth -= value;
    newbox.vertex.z = clampbox.vertex.z;
  }
  value = naGetBoxEndZ(clampbox);
  if(naGetBoxEndZ(newbox) > value){
    newbox.volume.depth = value - newbox.vertex.z;
  }
  return newbox;
}


NA_IDEF NAVertexi naClampVertexiToBoxi(NAVertexi vertex, NABoxi clampbox){
  // Declaration before implementation. Needed for C90.
  NAInt max;
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naClampVertexiToBoxi", "vertex is invalid.");
    if(naIsBoxiEmptySlow(clampbox))
      naError("naClampVertexiToBoxi", "clampbox is empty.");
    if(!naIsBoxiValid(clampbox))
      naError("naClampVertexiToBoxi", "clampbox is invalid.");
  #endif
  newvertex = vertex;
  if(newvertex.x <  clampbox.vertex.x) {newvertex.x = clampbox.vertex.x;}
  max = naGetBoxiMaxX(clampbox);
  if(newvertex.x > max){newvertex.x = max;}
  if(newvertex.y <  clampbox.vertex.y) {newvertex.y = clampbox.vertex.y;}
  max = naGetBoxiMaxY(clampbox);
  if(newvertex.y > max){newvertex.y = max;}
  if(newvertex.z <  clampbox.vertex.z) {newvertex.z = clampbox.vertex.z;}
  max = naGetBoxiMaxZ(clampbox);
  if(newvertex.z > max){newvertex.z = max;}
  return newvertex;
}


NA_IDEF NABoxi naClampBoxiToBoxi(NABoxi box, NABoxi clampbox){
  // Todo: use helper functions in this function.
  // Declaration before implementation. Needed for C90.
  NAInt value;
  NABoxi newbox;
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naClampBoxiToBoxi", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naClampBoxiToBoxi", "box is invalid.");
    if(naIsBoxiEmptySlow(clampbox))
      naError("naClampBoxiToBoxi", "clampbox is empty.");
    if(!naIsBoxiValid(clampbox))
      naError("naClampBoxiToBoxi", "clampbox is invalid.");
  #endif

  newbox = box;

  // Adjust in first dimension
  value = clampbox.vertex.x - newbox.vertex.x;
  if(value > 0){
    newbox.volume.width -= value;
    if(newbox.volume.width <= 0){return newbox;}
    newbox.vertex.x = clampbox.vertex.x;
  }
  value = naGetBoxiEndX(clampbox);
  if(naGetBoxiEndX(newbox) > value){
    newbox.volume.width = value - newbox.vertex.x;
    if(newbox.volume.width <= 0){return newbox;}
  }
  
  // Adjust in second dimension
  value = clampbox.vertex.y - newbox.vertex.y;
  if(value > 0){
    newbox.volume.height -= value;
    if(newbox.volume.height <= 0){return newbox;}
    newbox.vertex.y = clampbox.vertex.y;
  }
  value = naGetBoxiEndY(clampbox);
  if(naGetBoxiEndY(newbox) > value){
    newbox.volume.height = value - newbox.vertex.y;
    if(newbox.volume.height <= 0){return newbox;}
  }

  // Adjust in third dimension
  value = clampbox.vertex.z - newbox.vertex.z;
  if(value > 0){
    newbox.volume.depth -= value;
    if(newbox.volume.depth <= 0){return newbox;}
    newbox.vertex.z = clampbox.vertex.z;
  }
  value = naGetBoxiEndZ(clampbox);
  if(naGetBoxiEndZ(newbox) > value){
    newbox.volume.depth = value - newbox.vertex.z;
    if(newbox.volume.depth <= 0){return newbox;}
  }
  return newbox;
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



NA_IDEF NABool naEqualVertex(NAVertex vertex1, NAVertex vertex2){
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex1))
      naError("naEqualVertex", "vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("naEqualVertex", "vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVolume(NAVolume volume1, NAVolume volume2){
  #ifndef NDEBUG
    if(naIsVolumeEmpty(volume1) || naIsVolumeEmpty(volume2))
      naError("naEqualVolume", "Equality test not valid for empty volumes.");
    if(!naIsVolumeValid(volume1))
      naError("naEqualVolume", "volume1 is invalid.");
    if(!naIsVolumeValid(volume2))
      naError("naEqualVolume", "volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualBox(NABox box1, NABox box2){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box1) || naIsBoxEmpty(box2))
      naError("naEqualBox", "Equality test not valid for empty boxs.");
    if(!naIsBoxValid(box1))
      naError("naEqualBox", "box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("naEqualBox", "box2 is invalid.");
  #endif
  return     (naEqualVertex (box1.vertex,  box2.vertex)
          &&  naEqualVolume(box1.volume, box2.volume));
}
NA_IDEF NABool naEqualVertexi(NAVertexi vertex1, NAVertexi vertex2){
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex1))
      naError("naEqualVertexi", "vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("naEqualVertexi", "vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVolumei(NAVolumei volume1, NAVolumei volume2){
  #ifndef NDEBUG
    if(naIsVolumeiEmpty(volume1) || naIsVolumeiEmpty(volume2))
      naError("naEqualVolumei", "Equality test not valid for empty volumes.");
    if(!naIsVolumeiValid(volume1))
      naError("naEqualVolumei", "volume1 is invalid.");
    if(!naIsVolumeiValid(volume2))
      naError("naEqualVolumei", "volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualBoxi(NABoxi box1, NABoxi box2){
  #ifndef NDEBUG
    if(naIsBoxiEmpty(box1) || naIsBoxiEmpty(box2))
      naError("naEqualBoxi", "Equality test not valid for empty boxs.");
    if(!naIsBoxiValid(box1))
      naError("naEqualBoxi", "box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("naEqualBoxi", "box2 is invalid.");
  #endif
  return    (naEqualVertexi (box1.vertex,  box2.vertex)
          && naEqualVolumei(box1.volume, box2.volume));
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



NA_IDEF NAVertex naAddVertexVolume(NAVertex vertex, NAVolume volume){
  NAVertex newvertex;
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naAddVertexVolume", "vertex is invalid.");
    if(!naIsVolumeValid(volume))
      naError("naAddVertexVolume", "volume is invalid.");
  #endif
  newvertex.x = vertex.x + volume.width;
  newvertex.y = vertex.y + volume.height;
  newvertex.z = vertex.z + volume.depth;
  return newvertex;
}
NA_IDEF NAVertexi naAddVertexiVolumei(NAVertexi vertex, NAVolumei volume){
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naAddVertexiVolumei", "vertex is invalid.");
    if(!naIsVolumeiValid(volume))
      naError("naAddVertexiVolumei", "volume is invalid.");
  #endif
  newvertex.x = vertex.x + volume.width;
  newvertex.y = vertex.y + volume.height;
  newvertex.z = vertex.z + volume.depth;
  return newvertex;
}

NA_IDEF NAVolume naSubVertexPos(NAVertex vertex1, NAVertex vertex2){
  NAVolume newvolume;
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex1))
      naError("naSubVertexPos", "vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("naSubVertexPos", "vertex2 is invalid.");
  #endif
  newvolume.width  = vertex1.x - vertex2.x;
  newvolume.height = vertex1.y - vertex2.y;
  newvolume.depth  = vertex1.z - vertex2.z;
  return newvolume;
}
NA_IDEF NAVolumei naSubVertexiVertexi(NAVertexi vertex1, NAVertexi vertex2){
  NAVolumei newvolume;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex1))
      naError("naSubVertexiVertexi", "vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("naSubVertexiVertexi", "vertex2 is invalid.");
  #endif
  newvolume.width  = vertex1.x - vertex2.x;
  newvolume.height = vertex1.y - vertex2.y;
  newvolume.depth  = vertex1.z - vertex2.y;
  return newvolume;
}




NA_IDEF NABool naIsValueInRange(double value, NARange outerrange){
  #ifndef NDEBUG
    if(!naIsPosValueValid(value))
      naError("naIsValueInRange", "value is invalid.");
    if(!naIsRangeUseful(outerrange))
      naError("naIsValueInRange", "Inside test not valid for ranges which are not useful.");
  #endif
  return !((value < outerrange.offset)
        || (value > naGetRangeEnd(outerrange)));
}
NA_IDEF NABool naIsValueInRangef(float value, NARangef outerrange){
  #ifndef NDEBUG
    if(!naIsPosValueValidf(value))
      naError("naIsValueInRangef", "value is invalid.");
    if(!naIsRangefUseful(outerrange))
      naError("naIsValueInRangef", "Inside test not valid for ranges which are not useful.");
  #endif
  return !((value < outerrange.offset)
        || (value > naGetRangefEnd(outerrange)));
}
NA_IDEF NABool naIsValueInRangei(NAInt value, NARangei outerrange){
  #ifndef NDEBUG
    if(!naIsPosValueValidi(value))
      naError("naIsValueInRangei", "value is invalid.");
    if(!naIsRangeiUseful(outerrange))
      naError("naIsValueInRangei", "Inside test not valid for ranges which are not useful.");
  #endif
  return !((value < outerrange.offset)
        || (value > naGetRangeiMax(outerrange)));
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
    if(naIsRectiEmptySlow(outerrect))
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



NA_IDEF NABool naIsVertexInVolume(NAVertex vertex, NAVolume outervolume){
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naIsVertexInVolume", "vertex is invalid.");
    if(!naIsVolumeUseful(outervolume))
      naError("naIsVertexInVolume", "Inside test not valid for volumes which are not useful.");
  #endif
  return !((vertex.x < 0)
        || (vertex.x > outervolume.width)
        || (vertex.y < 0)
        || (vertex.y > outervolume.height)
        || (vertex.z < 0)
        || (vertex.z > outervolume.depth));
}
NA_IDEF NABool naIsVertexInBox(NAVertex vertex, NABox outerbox){
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naIsVertexInBox", "vertex is invalid.");
    if(naIsBoxEmpty(outerbox))
      naError("naIsVertexInBox", "Inside test not valid for empty boxs.");
    if(!naIsBoxValid(outerbox))
      naError("naIsVertexInBox", "outerbox is invalid.");
  #endif
  return !((vertex.x < outerbox.vertex.x)
        || (vertex.x > naGetBoxEndX(outerbox))
        || (vertex.y < outerbox.vertex.y)
        || (vertex.y > naGetBoxEndY(outerbox))
        || (vertex.z < outerbox.vertex.z)
        || (vertex.z > naGetBoxEndZ(outerbox)));
}
NA_IDEF NABool naIsVolumeInVolume(NAVolume volume, NAVolume outervolume){
  #ifndef NDEBUG
    if(!naIsVolumeUseful(volume))
      naError("naIsVolumeInVolume", "Inside test not valid if volume is not useful.");
    if(!naIsVolumeUseful(outervolume))
      naError("naIsVolumeInVolume", "Inside test not valid if outervolume is not useful.");
  #endif
  return !((volume.width  > outervolume.width)
        || (volume.height > outervolume.height)
        || (volume.depth  > outervolume.depth));
}
NA_IDEF NABool naIsBoxInBox(NABox box, NABox outerbox){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box) || naIsBoxEmpty(outerbox))
      naError("naIsBoxInBox", "Inside test not valid for empty boxs.");
    if(!naIsBoxValid(box))
      naError("naIsBoxInBox", "box is invalid.");
    if(!naIsBoxValid(outerbox))
      naError("naIsBoxInBox", "outerbox is invalid.");
  #endif
  return !((box.vertex.x          < outerbox.vertex.x)
        || (naGetBoxEndX(box) > naGetBoxEndX(outerbox))
        || (box.vertex.y          < outerbox.vertex.y)
        || (naGetBoxEndY(box) > naGetBoxEndY(outerbox))
        || (box.vertex.z          < outerbox.vertex.z)
        || (naGetBoxEndZ(box) > naGetBoxEndZ(outerbox)));
}
NA_IDEF NABool naIsVertexiInVolumei(NAVertexi vertex, NAVolumei outervolume){
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naIsVertexiInVolumei", "vertex is invalid.");
    if(naIsVolumeiUseful(outervolume))
      naError("naIsVertexiInVolumei", "Inside test not valid for volumes which are not useful.");
  #endif
  return  ((vertex.x >= 0)
        && (vertex.x <  outervolume.width)
        && (vertex.y >= 0)
        && (vertex.y <  outervolume.height)
        && (vertex.z >= 0)
        && (vertex.z <  outervolume.depth));
}
NA_IDEF NABool naIsVertexiInBoxi(NAVertexi vertex, NABoxi outerbox){
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naIsVertexiInBoxi", "vertex is invalid.");
    if(naIsBoxiEmptySlow(outerbox))
      naError("naIsVertexiInBoxi", "Inside test not valid for empty boxs.");
    if(!naIsBoxiValid(outerbox))
      naError("naIsVertexiInBoxi", "outerbox is invalid.");
  #endif
  return  ((vertex.x >= outerbox.vertex.x)
        && (vertex.x <  naGetBoxiEndX(outerbox))
        && (vertex.y >= outerbox.vertex.y)
        && (vertex.y <  naGetBoxiEndY(outerbox))
        && (vertex.z >= outerbox.vertex.z)
        && (vertex.z <  naGetBoxiEndZ(outerbox)));
}
NA_IDEF NABool naIsVolumeiInVolumei(NAVolumei volume, NAVolumei outervolume){
  #ifndef NDEBUG
    if(!naIsVolumeiUseful(volume))
      naError("naIsVolumeiInVolumei", "Inside test not valid if volume is not useful.");
    if(!naIsVolumeiUseful(outervolume))
      naError("naIsVolumeiInVolumei", "Inside test not valid if outervolume is not useful.");
  #endif
  return !((volume.width  > outervolume.width)
        || (volume.height > outervolume.height)
        || (volume.depth  > outervolume.depth));
}
NA_IDEF NABool naIsBoxiInBoxi(NABoxi box, NABoxi outerbox){
  #ifndef NDEBUG
    if(naIsBoxiEmpty(box) || naIsBoxiEmpty(outerbox))
      naError("naIsBoxiInBoxi", "Inside test not valid for empty boxs.");
    if(!naIsBoxiValid(box))
      naError("naIsBoxiInBoxi", "box is invalid.");
    if(!naIsBoxiValid(outerbox))
      naError("naIsBoxiInBoxi", "outerbox is invalid.");
  #endif
  return  ((box.vertex.x           >= outerbox.vertex.x)
        && (naGetBoxiEndX(box) <=  naGetBoxiEndX(outerbox))
        && (box.vertex.y           >= outerbox.vertex.y)
        && (naGetBoxiEndY(box) <=  naGetBoxiEndY(outerbox))
        && (box.vertex.z           >= outerbox.vertex.z)
        && (naGetBoxiEndZ(box) <=  naGetBoxiEndZ(outerbox)));
}




NA_IDEF double naGetRangeEnd(NARange range){
  #ifndef NDEBUG
    if(naIsRangeEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naOriginAndLengthToEnd(range.offset, range.length);
}
NA_IDEF float naGetRangefEnd(NARangef range){
  #ifndef NDEBUG
    if(naIsRangefEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangefValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naOriginAndLengthToEndf(range.offset, range.length);
}



NA_IDEF double naGetRectEndX (NARect  rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndX", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndX", "rect is invalid.");
  #endif
  return naOriginAndLengthToEnd(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndY (NARect  rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndY", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndY", "rect is invalid.");
  #endif
  return naOriginAndLengthToEnd(rect.pos.y, rect.size.height);
}



NA_IDEF double naGetBoxEndX (NABox  box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEndX", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEndX", "box is invalid.");
  #endif
  return naOriginAndLengthToEnd(box.vertex.x, box.volume.width);
}
NA_IDEF double naGetBoxEndY (NABox  box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEndY", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEndY", "box is invalid.");
  #endif
  return naOriginAndLengthToEnd(box.vertex.y, box.volume.height);
}
NA_IDEF double naGetBoxEndZ (NABox  box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEndZ", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEndZ", "box is invalid.");
  #endif
  return naOriginAndLengthToEnd(box.vertex.z, box.volume.depth);
}



NA_IDEF NAInt naGetRangeiEnd(NARangei range){
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naOriginAndLengthToEndi(range.offset, range.length);
}
NA_IDEF NAInt naGetRangeiMax(NARangei range){
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naOriginAndLengthToMaxi(range.offset, range.length);
}




NA_IDEF NAPosi naGetRectiEnd (NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiEnd", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEnd", "rect is invalid.");
  #endif
  return naMakePosi(naGetRectiEndX(rect), naGetRectiEndY(rect));
}
NA_IDEF NAInt naGetRectiEndX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiEndX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndX", "rect is invalid.");
  #endif
  return naOriginAndLengthToEndi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiEndY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiEndY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndY", "rect is invalid.");
  #endif
  return naOriginAndLengthToEndi(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiEndPos(NARecti rect){
  NAPosi newpos;
  newpos.x = naGetRectiEndX(rect);
  newpos.y = naGetRectiEndY(rect);
  return newpos;
}
NA_IDEF NAInt naGetRectiMaxX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiMaxX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxX", "rect is invalid.");
  #endif
  return naOriginAndLengthToMaxi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiMaxY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiMaxY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxY", "rect is invalid.");
  #endif
  return naOriginAndLengthToMaxi(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiMaxPos(NARecti rect){
  NAPosi newpos;
  newpos.x = naGetRectiMaxX(rect);
  newpos.y = naGetRectiMaxY(rect);
  return newpos;
}



NA_IDEF NAVertexi naGetBoxiEnd (NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEnd", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEnd", "box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiEndX(box), naGetBoxiEndY(box), naGetBoxiEndZ(box));
}
NA_IDEF NAInt naGetBoxiEndX(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEndX", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEndX", "box is invalid.");
  #endif
  return naOriginAndLengthToEndi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiEndY(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEndY", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEndY", "box is invalid.");
  #endif
  return naOriginAndLengthToEndi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiEndZ(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEndZ", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEndZ", "box is invalid.");
  #endif
  return naOriginAndLengthToEndi(box.vertex.z, box.volume.depth);
}
NA_IDEF NAVertexi naGetBoxiEndVertex(NABoxi box){
  NAVertexi newvertex;
  newvertex.x = naGetBoxiEndX(box);
  newvertex.y = naGetBoxiEndY(box);
  return newvertex;
}
NA_IDEF NAInt naGetBoxiMaxX(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMaxX", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMaxX", "box is invalid.");
  #endif
  return naOriginAndLengthToMaxi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiMaxY(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMaxY", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMaxY", "box is invalid.");
  #endif
  return naOriginAndLengthToMaxi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiMaxZ(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMaxZ", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMaxZ", "box is invalid.");
  #endif
  return naOriginAndLengthToMaxi(box.vertex.z, box.volume.depth);
}
NA_IDEF NAVertexi naGetBoxiMaxVertex(NABoxi box){
  NAVertexi newvertex;
  newvertex.x = naGetBoxiMaxX(box);
  newvertex.y = naGetBoxiMaxY(box);
  return newvertex;
}



NA_IDEF NAUInt naGetIndexWithOriginAndPosiRowFirst(NAPosi origin, NAPosi pos, NAUInt width){
  return ((pos.y - origin.y)) * width + (pos.x - origin.x);
}
NA_IDEF NAUInt naGetIndexWithOriginAndPosiColumnFirst(NAPosi origin, NAPosi pos, NAUInt height){
  return ((pos.x - origin.x)) * height + (pos.y - origin.y);
}
NA_IDEF NAUInt naGetIndexWithOriginAndVertexiRowFirst(NAVertexi origin, NAVertexi vertex, NAUInt width, NAUInt height){
  return (((vertex.z - origin.z)) * height + (vertex.y - origin.y)) * width + (vertex.x - origin.x);
}
NA_IDEF NAUInt naGetIndexWithOriginAndVertexiColumnFirst(NAVertexi origin, NAVertexi vertex, NAUInt depth, NAUInt height){
  return ((vertex.x - origin.x) * height + (vertex.y - origin.y)) * depth + (vertex.z - origin.z);
}




NA_IDEF NAInt naGetRectiIndexCount(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiIndexCount", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexCount", "rect is invalid.");
  #endif
  return rect.size.width * rect.size.height;
}
NA_IDEF NAUInt naGetRectiIndexOfPosiRowFirst(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiIndexOfPosiRowFirst", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosiRowFirst", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosiRowFirst", "pos is invalid.");
    if(!naIsPosiInRecti(pos, rect))
      naError("naGetRectiIndexOfPosiRowFirst", "pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosiRowFirst(rect.pos, pos, rect.size.width);
}
NA_IDEF NAUInt naGetRectiIndexOfPosiColumnFirst(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiIndexOfPosiColumnFirst", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosiColumnFirst", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosiColumnFirst", "pos is invalid.");
    if(!naIsPosiInRecti(pos, rect))
      naError("naGetRectiIndexOfPosiColumnFirst", "pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosiColumnFirst(rect.pos, pos, rect.size.height);
}



NA_IDEF NAInt naGetBoxiIndexCount(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiIndexCount", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiIndexCount", "box is invalid.");
  #endif
  return box.volume.width * box.volume.height * box.volume.depth;
}
NA_IDEF NAUInt naGetBoxiIndexOfVertexiRowFirst(NABoxi box, NAVertexi vertex){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiIndexOfVertexiRowFirst", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiIndexOfVertexiRowFirst", "box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("naGetBoxiIndexOfVertexiRowFirst", "vertex is invalid.");
    if(!naIsVertexiInBoxi(vertex, box))
      naError("naGetBoxiIndexOfVertexiRowFirst", "vertex is not inside box.");
  #endif
  return naGetIndexWithOriginAndVertexiRowFirst(box.vertex, vertex, box.volume.width, box.volume.height);
}
NA_IDEF NAUInt naGetBoxiIndexOfVertexiColumnFirst(NABoxi box, NAVertexi vertex){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiIndexOfVertexiColumnFirst", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiIndexOfVertexiColumnFirst", "box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("naGetBoxiIndexOfVertexiColumnFirst", "vertex is invalid.");
    if(!naIsVertexiInBoxi(vertex, box))
      naError("naGetBoxiIndexOfVertexiColumnFirst", "vertex is not inside box.");
  #endif
  return naGetIndexWithOriginAndVertexiColumnFirst(box.vertex, vertex, box.volume.depth, box.volume.height);
}



NA_IDEF NABool naIsRangeValid(NARange range){
  return naIsPosValueValid(range.offset) && naIsSizeValueValid(range.length);
}
NA_IDEF NABool naIsRangefValid(NARangef range){
  return naIsPosValueValidf(range.offset) && naIsSizeValueValidf(range.length);
}
NA_IDEF NABool naIsRangeiValid(NARangei range){
  return naIsPosValueValidi(range.offset) && naIsSizeValueValidi(range.length);
}



NA_IDEF NABool naIsPosValid(NAPos pos){
  return naIsPosValueValid(pos.x) && naIsPosValueValid(pos.y);
}
NA_IDEF NABool naIsPosiValid(NAPosi pos){
  return naIsPosValueValidi(pos.x) && naIsPosValueValidi(pos.y);
}
NA_IDEF NABool naIsSizeValid(NASize size){
  return naIsSizeValueValid(size.width) && naIsSizeValueValid(size.height);
}
NA_IDEF NABool naIsSizeiValid(NASizei size){
  return naIsSizeValueValidi(size.width) && naIsSizeValueValidi(size.height);
}
NA_IDEF NABool naIsRectValid(NARect rect){
  return (naIsPosValid(rect.pos) && naIsSizeValid(rect.size));
}
NA_IDEF NABool naIsRectiValid(NARecti rect){
  return (naIsPosiValid(rect.pos) && naIsSizeiValid(rect.size));
}



NA_IDEF NABool naIsVertexValid(NAVertex vertex){
  return naIsPosValueValid(vertex.x) && naIsPosValueValid(vertex.y) && naIsPosValueValid(vertex.z);
}
NA_IDEF NABool naIsVertexiValid(NAVertexi vertex){
  return naIsPosValueValidi(vertex.x) && naIsPosValueValidi(vertex.y) && naIsPosValueValidi(vertex.z);
}
NA_IDEF NABool naIsVolumeValid(NAVolume volume){
  return naIsSizeValueValid(volume.width) && naIsSizeValueValid(volume.height) && naIsSizeValueValid(volume.depth);
}
NA_IDEF NABool naIsVolumeiValid(NAVolumei volume){
  return naIsSizeValueValidi(volume.width) && naIsSizeValueValidi(volume.height) && naIsSizeValueValidi(volume.depth);
}
NA_IDEF NABool naIsBoxValid(NABox box){
  return (naIsVertexValid(box.vertex) && naIsVolumeValid(box.volume));
}
NA_IDEF NABool naIsBoxiValid(NABoxi box){
  return (naIsVertexiValid(box.vertex) && naIsVolumeiValid(box.volume));
}



NA_IDEF NABool naIsRangeEmpty(NARange range){
  return naIsSizeValueEmpty(range.length);
}
NA_IDEF NABool naIsRangefEmpty(NARangef range){
  return naIsSizeValueEmptyf(range.length);
}
NA_IDEF NABool naIsRangeiEmpty(NARangei range){
  return naIsSizeValueEmptyi(range.length);
}



NA_IDEF NABool naIsSizeEmpty (NASize size){
  return naIsSizeValueEmpty(size.width * size.height);
}
NA_IDEF NABool naIsSizeiEmpty(NASizei size){
  NAInt totalsize = size.width * size.height;
  #ifndef NDEBUG
    if(naIsSizeValueEmptyi(totalsize) && !naIsSizeValueEmptyi(size.width) && !naIsSizeValueEmptyi(size.height))
      naError("naIsSizeiEmpty", "You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsSizeValueEmptyi(totalsize);
}
NA_IDEF NABool naIsSizeiEmptySlow(NASizei size){
  return (naIsSizeValueEmptyi(size.width) || naIsSizeValueEmptyi(size.height));
}
NA_IDEF NABool naIsRectEmpty (NARect rect){
  return naIsSizeEmpty(rect.size);
}
NA_IDEF NABool naIsRectiEmpty(NARecti rect){
  return naIsSizeiEmpty(rect.size);
}
NA_IDEF NABool naIsRectiEmptySlow(NARecti rect){
  return naIsSizeiEmptySlow(rect.size);
}



NA_IDEF NABool naIsVolumeEmpty (NAVolume volume){
  return naIsSizeValueEmpty(volume.width * volume.height * volume.depth);
}
NA_IDEF NABool naIsVolumeiEmpty(NAVolumei volume){
  NAInt totalvolume = volume.width * volume.height * volume.depth;
  #ifndef NDEBUG
    if(naIsSizeValueEmptyi(totalvolume) && !naIsSizeValueEmptyi(volume.width) && !naIsSizeValueEmptyi(volume.height) && !naIsSizeValueEmptyi(volume.depth))
      naError("naIsVolumeiEmpty", "You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsSizeValueEmptyi(totalvolume);
}
NA_IDEF NABool naIsVolumeiEmptySlow(NAVolumei volume){
  return (naIsSizeValueEmptyi(volume.width) || naIsSizeValueEmptyi(volume.height) || naIsSizeValueEmptyi(volume.depth));
}
NA_IDEF NABool naIsBoxEmpty (NABox box){
  return naIsVolumeEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmpty(NABoxi box){
  return naIsVolumeiEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmptySlow(NABoxi box){
  return naIsVolumeiEmptySlow(box.volume);
}



NA_IDEF NABool naIsRangeNegative(NARange range){
  return naIsSizeValueNegative(range.length);
}
NA_IDEF NABool naIsRangefNegative(NARangef range){
  return naIsSizeValueNegativef(range.length);
}
NA_IDEF NABool naIsRangeiNegative(NARangei range){
  return naIsSizeValueNegativei(range.length);
}



NA_IDEF NABool naIsSizeNegative(NASize size){
  return (naIsSizeValueNegative(size.width) || naIsSizeValueNegative(size.height));
}
NA_IDEF NABool naIsSizeiNegative(NASizei size){
  return (naIsSizeValueNegativei(size.width) || naIsSizeValueNegativei(size.height));
}
NA_IDEF NABool naIsRectNegative(NARect rect){
  return naIsSizeNegative(rect.size);
}
NA_IDEF NABool naIsRectiNegative(NARecti rect){
  return naIsSizeiNegative(rect.size);
}



NA_IDEF NABool naIsVolumeNegative(NAVolume volume){
  return (naIsSizeValueNegative(volume.width) || naIsSizeValueNegative(volume.height) || naIsSizeValueNegative(volume.depth));
}
NA_IDEF NABool naIsVolumeiNegative(NAVolumei volume){
  return (naIsSizeValueNegativei(volume.width) || naIsSizeValueNegativei(volume.height) || naIsSizeValueNegativei(volume.depth));
}
NA_IDEF NABool naIsBoxNegative(NABox box){
  return naIsVolumeNegative(box.volume);
}
NA_IDEF NABool naIsBoxiNegative(NABoxi box){
  return naIsVolumeiNegative(box.volume);
}



NA_IDEF NABool naIsRangeUseful(NARange range){
  return (naIsPosValueUseful(range.offset) && naIsSizeValueUseful(range.length));
}
NA_IDEF NABool naIsRangefUseful(NARangef range){
  return (naIsPosValueUsefulf(range.offset) && naIsSizeValueUsefulf(range.length));
}
NA_IDEF NABool naIsRangeiUseful(NARangei range){
  return (naIsPosValueUsefuli(range.offset) && naIsSizeValueUsefuli(range.length));
}



NA_IDEF NABool naIsPosUseful(NAPos pos){
  return naIsPosValueUseful(pos.x) && naIsPosValueUseful(pos.y);
}
NA_IDEF NABool naIsPosiUseful(NAPosi pos){
  return naIsPosValueUsefuli(pos.x) && naIsPosValueUsefuli(pos.y);
}
NA_IDEF NABool naIsSizeUseful(NASize size){
  return naIsSizeValueUseful(size.width) && naIsSizeValueUseful(size.height);
}
NA_IDEF NABool naIsSizeiUseful(NASizei size){
  return naIsSizeValueUsefuli(size.width) && naIsSizeValueUsefuli(size.height);
}
NA_IDEF NABool naIsRectUseful(NARect rect){
  return (naIsPosUseful(rect.pos) && naIsSizeUseful(rect.size));
}
NA_IDEF NABool naIsRectiUseful(NARecti rect){
  return (naIsPosiUseful(rect.pos) && naIsSizeiUseful(rect.size));
}


NA_IDEF NABool naIsVertexUseful(NAVertex vertex){
  return naIsPosValueUseful(vertex.x) && naIsPosValueUseful(vertex.y) && naIsPosValueUseful(vertex.x);
}
NA_IDEF NABool naIsVertexiUseful(NAVertexi vertex){
  return naIsPosValueUsefuli(vertex.x) && naIsPosValueUsefuli(vertex.y) && naIsPosValueUsefuli(vertex.x);
}
NA_IDEF NABool naIsVolumeUseful(NAVolume volume){
  return naIsSizeValueUseful(volume.width) && naIsSizeValueUseful(volume.height) && naIsSizeValueUseful(volume.depth);
}
NA_IDEF NABool naIsVolumeiUseful(NAVolumei volume){
  return naIsSizeValueUsefuli(volume.width) && naIsSizeValueUsefuli(volume.height) && naIsSizeValueUsefuli(volume.depth);
}
NA_IDEF NABool naIsBoxUseful(NABox box){
  return (naIsVertexUseful(box.vertex) && naIsVolumeUseful(box.volume));
}
NA_IDEF NABool naIsBoxiUseful(NABoxi box){
  return (naIsVertexiUseful(box.vertex) && naIsVolumeiUseful(box.volume));
}




NA_IDEF NARect naMakeRectPositive(NARect rect){
  naMakePositive(&(rect.pos.x), &(rect.size.width));
  naMakePositive(&(rect.pos.y), &(rect.size.height));
  return rect;
}
NA_IDEF NARecti naMakeRectiPositive(NARecti rect){
  naMakePositivei(&(rect.pos.x), &(rect.size.width));
  naMakePositivei(&(rect.pos.y), &(rect.size.height));
  return rect;
}



NA_IDEF NABox naMakeBoxPositive(NABox box){
  naMakePositive(&(box.vertex.x), &(box.volume.width));
  naMakePositive(&(box.vertex.y), &(box.volume.height));
  naMakePositive(&(box.vertex.z), &(box.volume.depth));
  return box;
}
NA_IDEF NABoxi naMakeBoxiPositive(NABoxi box){
  naMakePositivei(&(box.vertex.x), &(box.volume.width));
  naMakePositivei(&(box.vertex.y), &(box.volume.height));
  naMakePositivei(&(box.vertex.z), &(box.volume.depth));
  return box;
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
