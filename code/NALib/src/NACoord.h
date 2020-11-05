
#ifndef NA_COORD_INCLUDED
#define NA_COORD_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// Following are definitions for integer and floating-point elements storing
// positions and lengths on 1-D, 2-D and 3-D coordinate systems. Namely:
//
// 1-D: NARange
// 2-D: NAPos, NASize, NARect
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
// About the naming of the elements:
//
// There is no final solution on how to name certain fields of the coord
// struct as they are used in various situations while programming. NALib
// tries to be somewhat consistent concerning the following naming scheme:
//
// pos      A "pos" denotes a position. In NALib, it is only used with the
//          NAPos type which is a two-dimensional position (x, y).
// origin   An "origin" always has the semantic meaning of a starting point
//          and does not exist as its own type. An origin can have various
//          types, sometimes denoting a coordinate or some kind of position
//          within a structure like a tree.
// offset   An "offset" denotes a position relative to a base like zero, an
//          origin or some other kind of starting point. An offset sometimes
//          is denoted with a one-word like "byteOffset" or "charoffset".
//
// start    A "start" is the first value of a range. It does not matter if
//          the range is of floating point or integer type. Semantically, it
//          is different to the "min" value, as "start" can be greater than
//          end and hence giving a range a negative length.
// end      The "end" is the counterpart of the "start": It denotes the last
//          value of a range. Note that the floating point "end" is different
//          from the integer "end". See paragraph below.
// min      The "min" has the semantical meaning of the minimal value of a
//          range. But it nontheless is always returning the first value of
//          a range. In case a range has a negative length and hence the
//          first value being greater than the last, a warning will be
//          emitted. The "min" value is only needed in integer ranges in
//          combination with the "max" value.
// max      The "max" value is only used for integer ranges and denotes one
//          value before the end. This is especially useful when working with
//          indices.
//
// size     A "size" is in natural speaking the general term for anything
//          using some kind of space to be existent. In NALib correspondingly,
//          a size describes the amount of bytes used in RAM. But to avoid
//          confusion, such a size is always denoted as a one-word like
//          "Bytesize", "typeSize", "Filesize" or "Pagesize" but never as
//          a plain "Size".
//          "NASize" on the other hand is a struct which stores two-dimensional
//          coordinate data (width, height) and in the corresponding situations
//          is always refered to as "size".
// length   A "length" is everything which has a one-dimensional size whereas
//          it has a semantic meaning of a "whole" rather than being a
//          collection of individual elements. A length can be looked at as
//          a measurement and can be both continuous (with floating point) or
//          discrete (with integer) and it can also be negative.
// count    A "count" is used, where the focus is on individual elements like
//          for example the number of elements in a list or an array. A count
//          should always be positive. In case that a count is negative, it
//          has a special meaning and is documented in the NALib API.
//
//
// Integer versus Floating-point
// -----------------------------
// Note that the integer variant of the structs behaves differently than
// the floating-point variant.
// The floating-point-variant treats sizes [inclusive - inclusive]
// The integer-       variant treats sizes [inclusive - exclusive)
// For example:
// The floating-point Range  (origin:0, length:5) contains offset 5.
// The integer        Rangei (origin:0, length:5) contains offset 4 but not 5.
//
// The integer variants are good for indexes. Therefore, all integer variants
// denote both a "Max" and an "End". The "End" is computed offset+size.
// The "Max" is computed End-1. For example, use the End for the terminating
// condition of a for-loop. Use the Max to access the last element.


#include "NABase.h"


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

// 3-D Coordinates: Vertex, Volume and Box
typedef struct NAVertex   NAVertex;
typedef struct NAVertexi  NAVertexi;
typedef struct NAVolume   NAVolume;
typedef struct NAVolumei  NAVolumei;
typedef struct NABox      NABox;
typedef struct NABoxi     NABoxi;

// 2-D Coordinates: Bounds (Top-Right-Bottom-Left)
typedef struct NABounds4  NABounds4;
typedef struct NABounds4i NABounds4i;

// The types are presented as non-opaque as the programmer wants to access the
// parts of the structs.
//
// Please excuse this formatting. It is usually not recommended but here, it
// would be much more confusing if written otherwise.
struct NARange{     double origin;   double length;};
struct NARangef{    float  origin;   float  length;};
struct NARangei{    NAInt  origin;   NAInt  length;};

struct NAPos{       double x;     double  y;     };
struct NAPosi{      NAInt  x;     NAInt   y;     };
struct NASize{      double width; double  height;};
struct NASizei{     NAInt  width; NAInt   height;};
struct NARect{      NAPos  pos;   NASize  size;  };
struct NARecti{     NAPosi pos;   NASizei size;  };

struct NAVertex{    double    x;      double    y;        double  z;   };
struct NAVertexi{   NAInt     x;      NAInt     y;        NAInt   z;   };
struct NAVolume{    double    width;  double    height;   double depth;};
struct NAVolumei{   NAInt     width;  NAInt     height;   NAInt  depth;};
struct NABox{       NAVertex  vertex; NAVolume  volume;};
struct NABoxi{      NAVertexi vertex; NAVolumei volume;};

struct NABounds4{   double top;   double  right; double bottom; double left;};
struct NABounds4i{  NAInt  top;   NAInt   right; NAInt  bottom; NAInt  left;};




// Create the various elements
// The Variants with E also allow the sizes and rects to be empty or negative.
// The Variants with S expect scalar values instead of structs
NA_IAPI NARange  naMakeRange  (double origin, double length);
NA_IAPI NARangef naMakeRangef (float  origin, float  length);
NA_IAPI NARangei naMakeRangei (NAInt  origin, NAInt  length);
NA_IAPI NARange  naMakeRangeE (double origin, double length);
NA_IAPI NARangei naMakeRangeiE(NAInt  origin, NAInt  length);
NA_IAPI NARange  naMakeRangeWithRangei(NARangei range);
NA_IAPI NARangei naMakeRangeiWithRange(NARange  range);
NA_IAPI NARange  naMakeRangeEmpty(void);
NA_IAPI NARangef naMakeRangefEmpty(void);
NA_IAPI NARangei naMakeRangeiEmpty(void);

NA_IAPI NAPos    naMakePos (double x, double y);
NA_IAPI NAPosi   naMakePosi(NAInt  x, NAInt  y);
NA_IAPI NAPos    naMakePosWithPosi(NAPosi pos);
NA_IAPI NAPosi   naMakePosiWithPos(NAPos  pos);
// The Integer variant assumes pos to only contain integers, hence using a
// more advanced and quicker conversion. But pos must be integral to work.
NA_IAPI NAPosi   naMakePosiWithIntegerPos(NAPos  pos);
NA_IAPI NAPos    naMakePosWithV2(double* v);

NA_IAPI NASize   naMakeSize   (double width,  double  height);
NA_IAPI NASizei  naMakeSizei  (NAInt  width,  NAInt   height);
NA_IAPI NASize   naMakeSizeE  (double width,  double  height);
NA_IAPI NASizei  naMakeSizeiE (NAInt  width,  NAInt   height);
NA_IAPI NASize   naMakeSizeWithSizei(NASizei size);
NA_IAPI NASizei  naMakeSizeiWithSize(NASize  size);
// The Integer variant assumes pos to only contain integers, hence using a
// more advanced and quicker conversion. But pos must be integral to work.
NA_IAPI NASizei  naMakeSizeiWithIntegerSize(NASize  size);
NA_IAPI NASize   naMakeSizeEmpty(void);
NA_IAPI NASizei  naMakeSizeiEmpty(void);

NA_IAPI NARect   naMakeRect   (NAPos  pos, NASize  size);
NA_IAPI NARecti  naMakeRecti  (NAPosi pos, NASizei size);
NA_IAPI NARect   naMakeRectE  (NAPos  pos, NASize  size);
NA_IAPI NARecti  naMakeRectiE (NAPosi pos, NASizei size);
NA_IAPI NARect   naMakeRectS  (double x, double y, double width, double height);
NA_IAPI NARecti  naMakeRectiS (NAInt  x, NAInt  y, NAInt  width, NAInt  height);
NA_IAPI NARect   naMakeRectSE (double x, double y, double width, double height);
NA_IAPI NARecti  naMakeRectiSE(NAInt  x, NAInt  y, NAInt  width, NAInt  height);
NA_IAPI NARect   naMakeRectWithRecti(NARecti rect);
NA_IAPI NARecti  naMakeRectiWithRect(NARect  rect);
NA_IAPI NARect   naMakeRectEmpty(void);
NA_IAPI NARecti  naMakeRectiEmpty(void);

NA_IAPI NABounds4  naMakeBounds4 (double top, double right, double bottom, double left);
NA_IAPI NABounds4i naMakeBounds4i(NAInt  top, NAInt  right, NAInt  bottom, NAInt  left);
NA_IAPI NABounds4  naMakeBounds4WithBounds4i(NABounds4i bounds);
NA_IAPI NABounds4i naMakeBounds4iWithBounds4(NABounds4  bounds);

NA_IAPI NAVertex    naMakeVertex      (double x, double y, double z);
NA_IAPI NAVertexi   naMakeVertexi (NAInt  x, NAInt  y, NAInt  z);
NA_IAPI NAVertex    naMakeVertexWithVertexi(NAVertexi vertex);
NA_IAPI NAVertexi   naMakeVertexiWithVertex(NAVertex  vertex);
NA_IAPI NAVertex    naMakeVertexWithV3(double* v);

NA_IAPI NAVolume    naMakeVolume  (double  width, double height, double depth);
NA_IAPI NAVolumei   naMakeVolumei (NAInt   width, NAInt  height, NAInt  depth);
NA_IAPI NAVolume    naMakeVolumeE (double  width, double height, double depth);
NA_IAPI NAVolumei   naMakeVolumeiE(NAInt   width, NAInt  height, NAInt  depth);
NA_IAPI NAVolume    naMakeVolumeWithVolumei(NAVolumei volume);
NA_IAPI NAVolumei   naMakeVolumeiWithVolume(NAVolume  volume);
NA_IAPI NAVolume    naMakeVolumeEmpty(void);
NA_IAPI NAVolumei   naMakeVolumeiEmpty(void);

NA_IAPI NABox       naMakeBox     (NAVertex  vertex,  NAVolume  volume);
NA_IAPI NABoxi      naMakeBoxi    (NAVertexi vertex,  NAVolumei volume);
NA_IAPI NABox       naMakeBoxS    (double x, double y, double z, double width, double height, double depth);
NA_IAPI NABoxi      naMakeBoxiS   (NAInt  x, NAInt  y, NAInt  z, NAInt  width, NAInt  height, NAInt  depth);
NA_IAPI NABox       naMakeBoxE    (NAVertex  vertex,  NAVolume  volume);
NA_IAPI NABoxi      naMakeBoxiE   (NAVertexi vertex,  NAVolumei volume);
NA_IAPI NABox       naMakeBoxSE   (double x, double y, double z, double width, double height, double depth);
NA_IAPI NABoxi      naMakeBoxiSE  (NAInt  x, NAInt  y, NAInt  z, NAInt  width, NAInt  height, NAInt  depth);
NA_IAPI NABox       naMakeBoxWithBoxi(NABoxi box);
NA_IAPI NABoxi      naMakeBoxiWithBox(NABox  box);
NA_IAPI NABox       naMakeBoxEmpty(void);
NA_IAPI NABoxi      naMakeBoxiEmpty(void);


// Mac OS X specific stuff
#ifdef CGGEOMETRY_H_
NA_IAPI NAPos     naMakePosWithCGPoint(CGPoint cgPoint);
NA_IAPI NASize    naMakeSizeWithCGSize(CGSize  cgSize);
NA_IAPI NARect    naMakeRectWithCGRect(CGRect  cgRect);
#endif
#if defined __OBJC__ && defined __AVAILABILITYMACROS__
// Note: In order to make this compile, you need to include "NACoord.h" AFTER
// including <Cocoa/Cocoa.h>
NA_IAPI NAPos     naMakePosWithNSPoint  (NSPoint nsPoint);
NA_IAPI NSPoint   naMakeNSPointWithPos  (NAPos   naPos);
NA_IAPI NASize    naMakeSizeWithNSSize  (NSSize  nsSize);
NA_IAPI NSSize    naMakeNSSizeWithSize  (NASize  naSize);
NA_IAPI NARect    naMakeRectWithNSRect  (NSRect  nsRect);
NA_IAPI NSRect    naMakeNSRectWithSize  (NASize  naSize); // origin = (0, 0)
NA_IAPI NSRect    naMakeNSRectWithRect  (NARect  naRect);
#endif

// Creates a new origin which is the given origin floored to a multiple of
// the given alignment. Also works for negative input offsets.
NA_IAPI NAPosi    naMakePosiWithAlignment   (NAPosi    origin, NARecti alignRect);
NA_IAPI NAPos     naMakePosWithAlignment    (NAPos     origin, NARect  alignRect);
NA_IAPI NAVertexi naMakeVertexiWithAlignment(NAVertexi origin, NABoxi  alignBox);
NA_IAPI NAVertex  naMakeVertexWithAlignment (NAVertex  origin, NABox   alignBox);

// Create the bounding box of two elements. The range, size or volume of the
// resulting struct will never be negative. Note that the integer variant of
// NARange also allows to be made with min and max instead of start and end.
// The naMakeRectiWithPosAndPos and naMakeBoxiWithVertexAndVertex functions
// assume the two vertexes to be inside the desired rect or box.
//
// The Variants with E also allow the rects to be empty.
NA_IAPI NARange  naMakeRangeWithStartAndEnd    (double   start,  double   end);
NA_IAPI NARangef naMakeRangefWithStartAndEnd   (float    start,  float    end);
NA_IAPI NARangei naMakeRangeiWithStartAndEnd   (NAInt    start,  NAInt    end);
NA_IAPI NARangei naMakeRangeiWithMinAndMax     (NAInt    min,    NAInt    max);
NA_IAPI NARangei naMakeRangeiWithRangeAndOffset(NARangei range,  NAInt    offset);
NA_IAPI NARangei naMakeRangeiWithRangeAndRange (NARangei range1, NARangei range2);

NA_IAPI NARect  naMakeRectWithPosAndPos    (NAPos   pos1,  NAPos   pos2);
NA_IAPI NARect  naMakeRectWithRectAndPos   (NARect  rect,  NAPos   pos);
NA_IAPI NARect  naMakeRectWithRectAndPosE  (NARect  rect,  NAPos   pos);
NA_IAPI NARect  naMakeRectWithRectAndRect  (NARect  rect1, NARect  rect2);
NA_IAPI NARect  naMakeRectWithRectAndRectE (NARect  rect1, NARect  rect2);
NA_IAPI NARecti naMakeRectiWithPosAndPos   (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NARecti naMakeRectiWithRectAndPos  (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti naMakeRectiWithRectAndPosE (NARecti rect,  NAPosi  pos);
NA_IAPI NARecti naMakeRectiWithRectAndRect (NARecti rect1, NARecti rect2);
NA_IAPI NARecti naMakeRectiWithRectAndRectE(NARecti rect1, NARecti rect2);

NA_IAPI NABox  naMakeBoxWithVertexAndVertex (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABox  naMakeBoxWithBoxAndVertex    (NABox     box,     NAVertex  vertex);
NA_IAPI NABox  naMakeBoxWithBoxAndBoxE      (NABox     box1,    NABox     box2);
NA_IAPI NABoxi naMakeBoxiWithVertexAndVertex(NAVertexi vertex1, NAVertexi vertex2);
NA_IAPI NABoxi naMakeBoxiWithBoxAndVertex   (NABoxi    box,     NAVertexi vertex);
NA_IAPI NABoxi naMakeBoxiWithBoxAndVertexE  (NABoxi    box,     NAVertexi vertex);
NA_IAPI NABoxi naMakeBoxiWithBoxAndBox      (NABoxi    box1,    NABoxi    box2);
NA_IAPI NABoxi naMakeBoxiWithBoxAndBoxE     (NABoxi    box1,    NABoxi    box2);

// Returns the intersection of the two elements. May be empty or negative.
// Note: This is basically the inverse of naMakeBoxiWithBoxAndBox. Use
// the naIsXXXUseful tests to test if the resulting element actually is a
// valid intersection.
NA_IAPI NARange  naMakeRangeWithRangeIntersection (NARange  range1, NARange  range2);
NA_IAPI NARangei naMakeRangeiWithRangeIntersection(NARangei range1, NARangei range2);
NA_IAPI NARect   naMakeRectWithRectIntersection   (NARect   rect1,  NARect   rect2);
NA_IAPI NARecti  naMakeRectiWithRectIntersection  (NARecti  rect1,  NARecti  rect2);
NA_IAPI NABox    naMakeBoxWithBoxIntersection     (NABox    box1,   NABox    box2);
NA_IAPI NABoxi   naMakeBoxiWithBoxIntersection    (NABoxi   box1,   NABoxi   box2);

// Creates a new rect by adding the given border around the given rect.
NA_IAPI NARect    naMakeRectWithRectAndBorder  (NARect  rect, double border);
NA_IAPI NARecti   naMakeRectiWithRectAndBorder (NARecti rect, NAInt  border);

NA_IAPI NABox     naMakeBoxWithBoxAndBorder    (NABox  box, double border);
NA_IAPI NABoxi    naMakeBoxiWithBoxAndBorder   (NABoxi box, NAInt  border);

// The naMakeXXXXAbsolute function ensures that the lengths/sizes/volumes of
// the given struct are not negative. If the these already were positive or
// zero, they will not be changed.
// For example:  A rect with pos(4., 6.) and size (-2., 7.) will change to
//            -> a rect with pos(2., 6.) and size ( 2., 7.) .
// But: An integer rect with pos(4 , 6 ) and size (-2 , 7 ) will change to
//            -> a rect with pos(3 , 6 ) and size ( 2 , 7 ) !
NA_IAPI NARange   naMakeRangePositive (NARange  range);
NA_IAPI NARangei  naMakeRangeiPositive(NARangei range);

NA_IAPI NARect    naMakeRectPositive (NARect  rect);
NA_IAPI NARecti   naMakeRectiPositive(NARecti rect);

NA_IAPI NABox     naMakeBoxPositive (NABox  box);
NA_IAPI NABoxi    naMakeBoxiPositive(NABoxi box);

// Copies the contents of source s to destination d
NA_IAPI void      naCopyPos    (NAPos*   posd,   const NAPos*   poss);
NA_IAPI void      naCopyPosi   (NAPosi*  posd,   const NAPosi*  poss);
NA_IAPI void      naCopySize   (NASize*  sized,  const NASize*  sizes);
NA_IAPI void      naCopySizei  (NASizei* sized,  const NASizei* sizes);
NA_IAPI void      naCopyRect   (NARect*  rectd,  const NARect*  rects);
NA_IAPI void      naCopyRecti  (NARecti* rectd,  const NARecti* rects);

NA_IAPI void      naCopyVertex (NAVertex*  vertexd,  const NAVertex*  vertexs);
NA_IAPI void      naCopyVertexi(NAVertexi* vertexd,  const NAVertexi* vertexs);
NA_IAPI void      naCopyVolume (NAVolume*  volumed,  const NAVolume*  volumes);
NA_IAPI void      naCopyVolumei(NAVolumei* volumed,  const NAVolumei* volumes);
NA_IAPI void      naCopyBox    (NABox*  boxd,  const NABox*  boxs);
NA_IAPI void      naCopyBoxi   (NABoxi* boxd,  const NABoxi* boxs);

// Returns the center position / vertex
NA_IAPI NAPos     naGetRectCenter (NARect rect);
NA_IAPI NAPosi    naGetRectiCenter(NARecti rect);
NA_IAPI NAVertex  naGetBoxCenter  (NABox box);
NA_IAPI NAVertexi naGetBoxiCenter (NABoxi box);
// Returns the offset of a size or volume being centered within a given rect
// or box relative to the origin of that rect or box.
NA_IAPI NAPos     naGetRectCenteredSizeOffset  (NARect rect, NASize   size);
NA_IAPI NAVertex  naGetBoxCeneteredVolumeOffset(NABox  box,  NAVolume volume);

// Clamping functions. Returns the new range.
NA_IAPI NARangei  naClampRangeiToRange(NARangei range, NARangei clampRange);
NA_IAPI NARangei  naClampRangeiToMin  (NARangei range, NAInt    min);
NA_IAPI NARangei  naClampRangeiToEnd  (NARangei range, NAInt    end);

NA_IAPI NAPos     naClampPosToRect  (NAPos   pos,  NARect  clampRect);
NA_IAPI NAPosi    naClampPosiToRect (NAPosi  pos,  NARecti clampRect);
NA_IAPI NARect    naClampRectToRect (NARect  rect, NARect  clampRect);
NA_IAPI NARecti   naClampRectiToRect(NARecti rect, NARecti clampRect);

NA_IAPI NAVertex  naClampVertexToBox (NAVertex  vertex, NABox  clampBox);
NA_IAPI NAVertexi naClampVertexiToBox(NAVertexi vertex, NABoxi clampBox);
NA_IAPI NABox     naClampBoxToBox    (NABox     box,    NABox  clampBox);
NA_IAPI NABoxi    naClampBoxiToBox   (NABoxi    box,    NABoxi clampBox);

// Use the following functions to make equality tests (==) between elements.
NA_IAPI NABool    naEqualRangei(NARangei range1, NARangei range2);

NA_IAPI NABool    naEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naEqualPosi (NAPosi  pos1,  NAPosi  pos2);
NA_IAPI NABool    naEqualSize (NASize  size1, NASize  size2);
NA_IAPI NABool    naEqualSizei(NASizei size1, NASizei size2);
NA_IAPI NABool    naEqualRect (NARect  rect1, NARect  rect2);
NA_IAPI NABool    naEqualRecti(NARecti rect1, NARecti rect2);

NA_IAPI NABool    naEqualVertex (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naEqualVertexi(NAVertexi vertex1, NAVertexi vertex2);
NA_IAPI NABool    naEqualVolume (NAVolume  volume1, NAVolume  volume2);
NA_IAPI NABool    naEqualBox    (NABox     box1,    NABox     box2);
NA_IAPI NABool    naEqualVolumei(NAVolumei volume1, NAVolumei volume2);
NA_IAPI NABool    naEqualBoxi   (NABoxi    box1,    NABoxi    box2);

// Use the following functions to make comparison tests between elements.
// They only return true if the condition is true for all components: xyz.
NA_IAPI NABool    naLowerPos       (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naLowerEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naGreaterEqualPos(NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naGreaterPos     (NAPos   pos1,  NAPos   pos2);

NA_IAPI NABool    naLowerVertex        (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naLowerEqualVertex   (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naGreaterEqualVertex (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naGreaterVertex      (NAVertex  vertex1, NAVertex  vertex2);

// Adding and subtracting delta values: A delta is a size or volume.
// Adding a size to a position returns a position. Subtracting two positions
// from each other returns a size.
//
// The returned NASize can be negative or even invalid.
NA_IAPI NAPos     naAddPosSize (NAPos   pos,   NASize  size);
NA_IAPI NAPosi    naAddPosiSize(NAPosi  pos,   NASizei size);
NA_IAPI NASize    naSubPosPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NASizei   naSubPosiPos (NAPosi  pos1,  NAPosi  pos2);

NA_IAPI double    naDistancePosPos  (NAPos   pos1,  NAPos   pos2);

NA_IAPI NAVertex  naAddVertexVolume (NAVertex   vertex,   NAVolume  volume);
NA_IAPI NAVertexi naAddVertexiVolume(NAVertexi  vertex,   NAVolumei volume);
NA_IAPI NAVolume  naSubVertexPos    (NAVertex   vertex1,  NAVertex  vertex2);
NA_IAPI NAVolumei naSubVertexiVertex(NAVertexi  vertex1,  NAVertexi vertex2);

// Containing functions. Test if an element is within another.
// The E variant for the double types denotes an exclusive rangecheck [...)
// instead of an includsive [...]
NA_IAPI NABool    naContainsRangeOffset (NARange  outerRange, double   offset);
NA_IAPI NABool    naContainsRangefOffset(NARangef outerRange, float    offset);
NA_IAPI NABool    naContainsRangeiOffset(NARangei outerRange, NAInt    offset);
NA_IAPI NABool    naContainsRangeiRange (NARangei outerRange, NARangei range);

NA_IAPI NABool    naContainsSizePos  (NASize  outerSize, NAPos   pos);
NA_IAPI NABool    naContainsSizeiPos (NASizei outerSize, NAPosi  pos);
NA_IAPI NABool    naContainsRectPos  (NARect  outerRect, NAPos   pos);
NA_IAPI NABool    naContainsRectPosE (NARect  outerRect, NAPos   pos);
NA_IAPI NABool    naContainsRectiPos (NARecti outerRect, NAPosi  pos);
NA_IAPI NABool    naContainsSizeSize (NASize  outerSize, NASize  size);
NA_IAPI NABool    naContainsSizeiSize(NASizei outerSize, NASizei size);
NA_IAPI NABool    naContainsRectRect (NARect  outerRect, NARect  rect);
NA_IAPI NABool    naContainsRectiRect(NARecti outerRect, NARecti rect);

NA_IAPI NABool    naContainsVolumeVertex (NAVolume  outerVolume, NAVertex  vertex);
NA_IAPI NABool    naContainsVolumeiVertex(NAVolumei outerVolume, NAVertexi vertex);
NA_IAPI NABool    naContainsBoxVertex    (NABox     outerBox,    NAVertex  vertex);
NA_IAPI NABool    naContainsBoxVertexE   (NABox     outerBox,    NAVertex  vertex);
NA_IAPI NABool    naContainsBoxiVertex   (NABoxi    outerBox,    NAVertexi vertex);
NA_IAPI NABool    naContainsVolumeVolume (NAVolume  outerVolume, NAVolume  volume);
NA_IAPI NABool    naContainsVolumeiVolume(NAVolumei outerVolume, NAVolumei volume);
NA_IAPI NABool    naContainsBoxBox       (NABox     outerBox,    NABox     box);
NA_IAPI NABool    naContainsBoxiBox      (NABoxi    outerBox,    NABoxi    box);

// Get the End-Point of a rect. Is the same as origin + size.
// The E variants allow the given struct to be empty.
NA_IAPI double    naGetRangeEnd   (NARange  range);
NA_IAPI float     naGetRangefEnd  (NARangef range);
NA_IAPI double    naGetRangeEndE  (NARange  range);
NA_IAPI float     naGetRangefEndE (NARangef range);

NA_IAPI NAPos     naGetRectEnd    (NARect rect);
NA_IAPI double    naGetRectEndX   (NARect  rect);
NA_IAPI double    naGetRectEndY   (NARect  rect);
NA_IAPI double    naGetRectEndXE  (NARect  rect);
NA_IAPI double    naGetRectEndYE  (NARect  rect);

NA_IAPI NAVertex  naGetBoxEnd     (NABox  box);
NA_IAPI double    naGetBoxEndX    (NABox  box);
NA_IAPI double    naGetBoxEndY    (NABox  box);
NA_IAPI double    naGetBoxEndZ    (NABox  box);
NA_IAPI double    naGetBoxEndXE   (NABox  box);
NA_IAPI double    naGetBoxEndYE   (NABox  box);
NA_IAPI double    naGetBoxEndZE   (NABox  box);

// Computes the positions opposite to the origin of the rect.
// End returns origin + size
// Max returns oritin + size - 1
NA_IAPI NAInt     naGetRangeiEnd (NARangei range);
NA_IAPI NAInt     naGetRangeiMax (NARangei range);

NA_IAPI NAPosi    naGetRectiEnd  (NARecti rect);
NA_IAPI NAInt     naGetRectiEndX (NARecti rect);
NA_IAPI NAInt     naGetRectiEndY (NARecti rect);
NA_IAPI NAPosi    naGetRectiMax  (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxX (NARecti rect);
NA_IAPI NAInt     naGetRectiMaxY (NARecti rect);

NA_IAPI NAVertexi naGetBoxiEnd   (NABoxi box);
NA_IAPI NAInt     naGetBoxiEndX  (NABoxi box);
NA_IAPI NAInt     naGetBoxiEndY  (NABoxi box);
NA_IAPI NAInt     naGetBoxiEndZ  (NABoxi box);
NA_IAPI NAVertexi naGetBoxiMax   (NABoxi box);
NA_IAPI NAInt     naGetBoxiMaxX  (NABoxi box);
NA_IAPI NAInt     naGetBoxiMaxY  (NABoxi box);
NA_IAPI NAInt     naGetBoxiMaxZ  (NABoxi box);

// Raw Index functions. Assuming a 2-dimensional or 3-dimensional array with
// row-first or column-first ordering. Row-first is the default C ordering.
NA_IAPI NAInt    naGetIndexWithOriginAndPosRowFirst(       NAPosi    origin, NAPosi    offset, NAInt width);
NA_IAPI NAInt    naGetIndexWithOriginAndPosColumnFirst(    NAPosi    origin, NAPosi    offset, NAInt height);
NA_IAPI NAInt    naGetIndexWithOriginAndVertexRowFirst(    NAVertexi origin, NAVertexi vertex, NAInt width, NAInt height);
NA_IAPI NAInt    naGetIndexWithOriginAndVertexColumnFirst( NAVertexi origin, NAVertexi vertex, NAInt depth, NAInt height);

// Count returns the total number of indices (width*height) within a rect.
// IndexOf returns the index of the corresponding 1-dimensional array.
// Note that row-first is the default C ordering.
NA_IAPI NAInt     naGetRectiIndexCount             (NARecti rect);
NA_IAPI NAInt     naGetRectiIndexOfPosRowFirst     (NARecti rect, NAPosi pos);
NA_IAPI NAInt     naGetRectiIndexOfPosColumnFirst  (NARecti rect, NAPosi pos);

NA_IAPI NAInt     naGetBoxiIndexCount              (NABoxi box);
NA_IAPI NAInt     naGetBoxiIndexOfVertexRowFirst   (NABoxi box, NAVertexi vertex);
NA_IAPI NAInt     naGetBoxiIndexOfVertexColumnFirst(NABoxi box, NAVertexi vertex);
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
NA_IAPI NABool    naIsRectEmpty     (NARect  rect);
NA_IAPI NABool    naIsRectiEmpty    (NARecti rect);
NA_IAPI NABool    naIsRectiEmptySlow(NARecti rect);

NA_IAPI NABool    naIsVolumeEmpty     (NAVolume  volume);
NA_IAPI NABool    naIsVolumeiEmpty    (NAVolumei volume);
NA_IDEF NABool    naIsVolumeiEmptySlow(NAVolumei volume);
NA_IAPI NABool    naIsBoxEmpty        (NABox     box);
NA_IAPI NABool    naIsBoxiEmpty       (NABoxi    box);
NA_IAPI NABool    naIsBoxiEmptySlow   (NABoxi    box);

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






// Inline implementations are in a separate file:
#include "NAMath/NACoordII.h"






#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_COORD_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
