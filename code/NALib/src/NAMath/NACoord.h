
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
//          "ByteSize", "typeSize", "Filesize" or "Pagesize" but never as
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


#include "../NABase/NABase.h"
#include <stdlib.h>


// /////////////////////////////////
// TYPE DEFINITIONS


// 1-D Coordinates: Range
typedef struct NARange    NARange;
typedef struct NARangef   NARangef;
typedef struct NARangei32 NARangei32;
typedef struct NARangei64 NARangei64;
typedef struct NARanges   NARanges;

// 2-D Coordinates: Pos, Size and Rect
typedef struct NAPos      NAPos;
typedef struct NAPosf     NAPosf;
typedef struct NAPosi32   NAPosi32;
typedef struct NAPosi64   NAPosi64;
typedef struct NAPoss     NAPoss;

typedef struct NASize     NASize;
typedef struct NASizef    NASizef;
typedef struct NASizei32  NASizei32;
typedef struct NASizei64  NASizei64;
typedef struct NASizes    NASizes;

typedef struct NARect     NARect;
typedef struct NARectf    NARectf;
typedef struct NARecti32  NARecti32;
typedef struct NARecti64  NARecti64;
typedef struct NARects    NARects;

// 3-D Coordinates: Vertex, Volume and Box
typedef struct NAVertex    NAVertex;
typedef struct NAVertexf   NAVertexf;
typedef struct NAVertexi32 NAVertexi32;
typedef struct NAVertexi64 NAVertexi64;
typedef struct NAVertexs   NAVertexs;

typedef struct NAVolume    NAVolume;
typedef struct NAVolumef   NAVolumef;
typedef struct NAVolumei32 NAVolumei32;
typedef struct NAVolumei64 NAVolumei64;
typedef struct NAVolumes   NAVolumes;

typedef struct NABox       NABox;
typedef struct NABoxf      NABoxf;
typedef struct NABoxi32    NABoxi32;
typedef struct NABoxi64    NABoxi64;
typedef struct NABoxs      NABoxs;

// 2-D Coordinates: Bezel (Right-Top-Left-Bottom)
typedef struct NABezel4    NABezel4;
typedef struct NABezel4f   NABezel4f;
typedef struct NABezel4i32 NABezel4i32;
typedef struct NABezel4i64 NABezel4i64;
typedef struct NABezel4s   NABezel4s;



// The types are presented as non-opaque as the programmer wants to access the
// parts of the structs.
//
// Please excuse this formatting. It is usually not recommended but here, it
// would be much more confusing if written otherwise.
struct NARange{     double origin;   double length; };
struct NARangef{    float  origin;   float  length; };
struct NARangei32{  int32  origin;   int32  length; };
struct NARangei64{  int64  origin;   int64  length; };
struct NARanges{    size_t origin;   size_t length; };

struct NAPos{       double x;     double  y; };
struct NAPosf{      float  x;     float   y; };
struct NAPosi32{    int32  x;     int32   y; };
struct NAPosi64{    int64  x;     int64   y; };
struct NAPoss{      size_t x;     size_t  y; };

struct NASize{      double width; double  height; };
struct NASizef{     float  width; float   height; };
struct NASizei32{   int32  width; int32   height; };
struct NASizei64{   int64  width; int64   height; };
struct NASizes{     size_t width; size_t  height; };

struct NARect{      NAPos    pos; NASize    size; };
struct NARectf{     NAPosf   pos; NASizef   size; };
struct NARecti32{   NAPosi32 pos; NASizei32 size; };
struct NARecti64{   NAPosi64 pos; NASizei64 size; };
struct NARects{     NAPoss   pos; NASizes   size; };

struct NAVertex{    double      x;      double      y;        double z; };
struct NAVertexf{   float       x;      float       y;        float  z; };
struct NAVertexi32{ int32       x;      int32       y;        int32  z; };
struct NAVertexi64{ int64       x;      int64       y;        int64  z; };
struct NAVertexs{   size_t      x;      size_t      y;        size_t z; };

struct NAVolume{    double      width;  double      height;   double depth; };
struct NAVolumef{   float       width;  float       height;   float  depth; };
struct NAVolumei32{ int32       width;  int32       height;   int32  depth; };
struct NAVolumei64{ int64       width;  int64       height;   int64  depth; };
struct NAVolumes{   size_t      width;  size_t      height;   size_t depth; };

struct NABox{       NAVertex    vertex; NAVolume    volume; };
struct NABoxf{      NAVertexf   vertex; NAVolumef   volume; };
struct NABoxi32{    NAVertexi32 vertex; NAVolumei32 volume; };
struct NABoxi64{    NAVertexi64 vertex; NAVolumei64 volume; };
struct NABoxs{      NAVertexs   vertex; NAVolumes   volume; };

struct NABezel4{    double right; double top; double left; double bottom; };
struct NABezel4f{   float  right; float  top; float  left; float  bottom; };
struct NABezel4i32{ int32  right; int32  top; int32  left; int32  bottom; };
struct NABezel4i64{ int64  right; int64  top; int64  left; int64  bottom; };
struct NABezel4s{   size_t right; size_t top; size_t left; size_t bottom; };



// Create the various elements
// The Variants with E allow sizes and rects to be empty or negative.
// The Variants with S expect scalar values instead of structs
// The Variants with Zero initialize ALL members with the value zero.

// NARange

NA_IAPI NARange    naMakeRange    (double origin, double length);
NA_IAPI NARangef   naMakeRangef   (float  origin, float  length);
NA_IAPI NARangei32 naMakeRangei32 (int32  origin, int32  length);
NA_IAPI NARangei64 naMakeRangei64 (int64  origin, int64  length);
NA_IAPI NARanges   naMakeRanges   (size_t origin, size_t length);

NA_IAPI NARange    naMakeRangeE   (double origin, double length);
NA_IAPI NARangef   naMakeRangefE  (float  origin, float  length);
NA_IAPI NARangei32 naMakeRangei32E(int32  origin, int32  length);
NA_IAPI NARangei64 naMakeRangei64E(int64  origin, int64  length);
NA_IAPI NARanges   naMakeRangesE  (size_t origin, size_t length);

NA_IAPI NARange    naMakeRangeZero   (void);
NA_IAPI NARangef   naMakeRangefZero  (void);
NA_IAPI NARangei32 naMakeRangei32Zero(void);
NA_IAPI NARangei64 naMakeRangei64Zero(void);
NA_IAPI NARanges   naMakeRangesZero  (void);

NA_IAPI void       naFillRangeWithCopy   (NARange*    ranged, const NARange*    ranges);
NA_IAPI void       naFillRangefWithCopy  (NARangef*   ranged, const NARangef*   ranges);
NA_IAPI void       naFillRangei32WithCopy(NARangei32* ranged, const NARangei32* ranges);
NA_IAPI void       naFillRangei64WithCopy(NARangei64* ranged, const NARangei64* ranges);
NA_IAPI void       naFillRangesWithCopy  (NARanges*   ranged, const NARanges*   ranges);

// NAPos

NA_IAPI NAPos    naMakePos   (double x, double y);
NA_IAPI NAPosf   naMakePosf  (float  x, float  y);
NA_IAPI NAPosi32 naMakePosi32(int32  x, int32  y);
NA_IAPI NAPosi64 naMakePosi64(int64  x, int64  y);
NA_IAPI NAPoss   naMakePoss  (size_t x, size_t y);

NA_IAPI NAPos    naMakePosZero   (void);
NA_IAPI NAPosf   naMakePosfZero  (void);
NA_IAPI NAPosi32 naMakePosi32Zero(void);
NA_IAPI NAPosi64 naMakePosi64Zero(void);
NA_IAPI NAPoss   naMakePossZero  (void);

NA_IAPI void     naFillPosWithCopy   (NAPos*    posd, const NAPos*    poss);
NA_IAPI void     naFillPosfWithCopy  (NAPosf*   posd, const NAPosf*   poss);
NA_IAPI void     naFillPosi32WithCopy(NAPosi32* posd, const NAPosi32* poss);
NA_IAPI void     naFillPosi64WithCopy(NAPosi64* posd, const NAPosi64* poss);
NA_IAPI void     naFillPossWithCopy  (NAPoss*   posd, const NAPoss*   poss);

// NASize

NA_IAPI NASize    naMakeSize    (double width, double height);
NA_IAPI NASizef   naMakeSizef   (float  width, float  height);
NA_IAPI NASizei32 naMakeSizei32 (int32  width, int32  height);
NA_IAPI NASizei64 naMakeSizei64 (int64  width, int64  height);
NA_IAPI NASizes   naMakeSizes   (size_t width, size_t height);

NA_IAPI NASize    naMakeSizeE   (double width, double height);
NA_IAPI NASizef   naMakeSizefE  (float  width, float  height);
NA_IAPI NASizei32 naMakeSizei32E(int32  width, int32  height);
NA_IAPI NASizei64 naMakeSizei64E(int64  width, int64  height);
NA_IAPI NASizes   naMakeSizesE  (size_t width, size_t height);

NA_IAPI NASize    naMakeSizeZero   (void);
NA_IAPI NASizef   naMakeSizefZero  (void);
NA_IAPI NASizei32 naMakeSizei32Zero(void);
NA_IAPI NASizei64 naMakeSizei64Zero(void);
NA_IAPI NASizes   naMakeSizesZero  (void);

NA_IAPI void      naFillSizeWithCopy   (NASize*    sized, const NASize*    sizes);
NA_IAPI void      naFillSizefWithCopy  (NASizef*   sized, const NASizef*   sizes);
NA_IAPI void      naFillSizei32WithCopy(NASizei32* sized, const NASizei32* sizes);
NA_IAPI void      naFillSizei64WithCopy(NASizei64* sized, const NASizei64* sizes);
NA_IAPI void      naFillSizesWithCopy  (NASizes*   sized, const NASizes*   sizes);

// NARect

NA_IAPI NARect    naMakeRect     (NAPos    pos, NASize    size);
NA_IAPI NARectf   naMakeRectf    (NAPosf   pos, NASizef   size);
NA_IAPI NARecti32 naMakeRecti32  (NAPosi32 pos, NASizei32 size);
NA_IAPI NARecti64 naMakeRecti64  (NAPosi64 pos, NASizei64 size);
NA_IAPI NARects   naMakeRects    (NAPoss   pos, NASizes   size);

NA_IAPI NARect    naMakeRectE    (NAPos    pos, NASize    size);
NA_IAPI NARectf   naMakeRectfE   (NAPosf   pos, NASizef   size);
NA_IAPI NARecti32 naMakeRecti32E (NAPosi32 pos, NASizei32 size);
NA_IAPI NARecti64 naMakeRecti64E (NAPosi64 pos, NASizei64 size);
NA_IAPI NARects   naMakeRectsE   (NAPoss   pos, NASizes   size);

NA_IAPI NARect    naMakeRectS    (double x, double y, double width, double height);
NA_IAPI NARectf   naMakeRectfS   (float  x, float  y, float  width, float  height);
NA_IAPI NARecti32 naMakeRecti32S (int32  x, int32  y, int32  width, int32  height);
NA_IAPI NARecti64 naMakeRecti64S (int64  x, int64  y, int64  width, int64  height);
NA_IAPI NARects   naMakeRectsS   (size_t x, size_t y, size_t width, size_t height);

NA_IAPI NARect    naMakeRectSE   (double x, double y, double width, double height);
NA_IAPI NARectf   naMakeRectfSE  (float  x, float  y, float  width, float  height);
NA_IAPI NARecti32 naMakeRecti32SE(int32  x, int32  y, int32  width, int32  height);
NA_IAPI NARecti64 naMakeRecti64SE(int64  x, int64  y, int64  width, int64  height);
NA_IAPI NARects   naMakeRectsSE  (size_t x, size_t y, size_t width, size_t height);

NA_IAPI NARect    naMakeRectZero   (void);
NA_IAPI NARectf   naMakeRectfZero  (void);
NA_IAPI NARecti32 naMakeRecti32Zero(void);
NA_IAPI NARecti64 naMakeRecti64Zero(void);
NA_IAPI NARects   naMakeRectsZero  (void);

NA_IAPI void      naFillRectWithCopy   (NARect*    rectd, const NARect*    rects);
NA_IAPI void      naFillRectfWithCopy  (NARectf*   rectd, const NARectf*   rects);
NA_IAPI void      naFillRecti32WithCopy(NARecti32* rectd, const NARecti32* rects);
NA_IAPI void      naFillRecti64WithCopy(NARecti64* rectd, const NARecti64* rects);
NA_IAPI void      naFillRectsWithCopy  (NARects*   rectd, const NARects*   rects);

// NAVertex

NA_IAPI NAVertex    naMakeVertex   (double x, double y, double z);
NA_IAPI NAVertexf   naMakeVertexf  (float  x, float  y, float  z);
NA_IAPI NAVertexi32 naMakeVertexi32(int32  x, int32  y, int32  z);
NA_IAPI NAVertexi64 naMakeVertexi64(int64  x, int64  y, int64  z);
NA_IAPI NAVertexs   naMakeVertexs  (size_t x, size_t y, size_t z);

NA_IAPI NAVertex    naMakeVertexZero   (void);
NA_IAPI NAVertexf   naMakeVertexfZero  (void);
NA_IAPI NAVertexi32 naMakeVertexi32Zero(void);
NA_IAPI NAVertexi64 naMakeVertexi64Zero(void);
NA_IAPI NAVertexs   naMakeVertexsZero  (void);

NA_IAPI void        naFillVertexWithCopy   (NAVertex*    vertexd, const NAVertex*    vertexs);
NA_IAPI void        naFillVertexfWithCopy  (NAVertexf*   vertexd, const NAVertexf*   vertexs);
NA_IAPI void        naFillVertexi32WithCopy(NAVertexi32* vertexd, const NAVertexi32* vertexs);
NA_IAPI void        naFillVertexi64WithCopy(NAVertexi64* vertexd, const NAVertexi64* vertexs);
NA_IAPI void        naFillVertexsWithCopy  (NAVertexs*   vertexd, const NAVertexs*   vertexs);

// NAVolume

NA_IAPI NAVolume    naMakeVolume    (double  width, double height, double depth);
NA_IAPI NAVolumef   naMakeVolumef   (float   width, float  height, float  depth);
NA_IAPI NAVolumei32 naMakeVolumei32 (int32   width, int32  height, int32  depth);
NA_IAPI NAVolumei64 naMakeVolumei64 (int64   width, int64  height, int64  depth);
NA_IAPI NAVolumes   naMakeVolumes   (size_t  width, size_t height, size_t depth);

NA_IAPI NAVolume    naMakeVolumeE   (double  width, double height, double depth);
NA_IAPI NAVolumef   naMakeVolumefE  (float   width, float  height, float  depth);
NA_IAPI NAVolumei32 naMakeVolumei32E(int32   width, int32  height, int32  depth);
NA_IAPI NAVolumei64 naMakeVolumei64E(int64   width, int64  height, int64  depth);
NA_IAPI NAVolumes   naMakeVolumesE  (size_t  width, size_t height, size_t depth);

NA_IAPI NAVolume    naMakeVolumeZero   (void);
NA_IAPI NAVolumef   naMakeVolumefZero  (void);
NA_IAPI NAVolumei32 naMakeVolumei32Zero(void);
NA_IAPI NAVolumei64 naMakeVolumei64Zero(void);
NA_IAPI NAVolumes   naMakeVolumesZero  (void);

NA_IAPI void        naFillVolumeWithCopy   (NAVolume*    volumed, const NAVolume*    volumes);
NA_IAPI void        naFillVolumefWithCopy  (NAVolumef*   volumed, const NAVolumef*   volumes);
NA_IAPI void        naFillVolumei32WithCopy(NAVolumei32* volumed, const NAVolumei32* volumes);
NA_IAPI void        naFillVolumei64WithCopy(NAVolumei64* volumed, const NAVolumei64* volumes);
NA_IAPI void        naFillVolumesWithCopy  (NAVolumes*   volumed, const NAVolumes*   volumes);

// NABox

NA_IAPI NABox       naMakeBox     (NAVertex    vertex,  NAVolume    volume);
NA_IAPI NABoxf      naMakeBoxf    (NAVertexf   vertex,  NAVolumef   volume);
NA_IAPI NABoxi32    naMakeBoxi32  (NAVertexi32 vertex,  NAVolumei32 volume);
NA_IAPI NABoxi64    naMakeBoxi64  (NAVertexi64 vertex,  NAVolumei64 volume);
NA_IAPI NABoxs      naMakeBoxs    (NAVertexs   vertex,  NAVolumes   volume);

NA_IAPI NABox       naMakeBoxS    (double x, double y, double z, double width, double height, double depth);
NA_IAPI NABoxf      naMakeBoxfS   (float  x, float  y, float  z, float  width, float  height, float  depth);
NA_IAPI NABoxi32    naMakeBoxi32S (int32  x, int32  y, int32  z, int32  width, int32  height, int32  depth);
NA_IAPI NABoxi64    naMakeBoxi64S (int64  x, int64  y, int64  z, int64  width, int64  height, int64  depth);
NA_IAPI NABoxs      naMakeBoxsS   (size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth);

NA_IAPI NABox       naMakeBoxE    (NAVertex    vertex, NAVolume    volume);
NA_IAPI NABoxf      naMakeBoxfE   (NAVertexf   vertex, NAVolumef   volume);
NA_IAPI NABoxi32    naMakeBoxi32E (NAVertexi32 vertex, NAVolumei32 volume);
NA_IAPI NABoxi64    naMakeBoxi64E (NAVertexi64 vertex, NAVolumei64 volume);
NA_IAPI NABoxs      naMakeBoxsE   (NAVertexs   vertex, NAVolumes   volume);

NA_IAPI NABox       naMakeBoxSE   (double x, double y, double z, double width, double height, double depth);
NA_IAPI NABoxf      naMakeBoxfSE  (float  x, float  y, float  z, float  width, float  height, float  depth);
NA_IAPI NABoxi32    naMakeBoxi32SE(int32  x, int32  y, int32  z, int32  width, int32  height, int32  depth);
NA_IAPI NABoxi64    naMakeBoxi64SE(int64  x, int64  y, int64  z, int64  width, int64  height, int64  depth);
NA_IAPI NABoxs      naMakeBoxsSE  (size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth);

NA_IAPI NABox       naMakeBoxEmpty   (void);
NA_IAPI NABoxf      naMakeBoxfEmpty  (void);
NA_IAPI NABoxi32    naMakeBoxi32Empty(void);
NA_IAPI NABoxi64    naMakeBoxi64Empty(void);
NA_IAPI NABoxs      naMakeBoxsEmpty  (void);

NA_IAPI void        naFillBoxWithCopy   (NABox*    boxd, const NABox*    boxs);
NA_IAPI void        naFillBoxfWithCopy  (NABoxf*   boxd, const NABoxf*   boxs);
NA_IAPI void        naFillBoxi32WithCopy(NABoxi32* boxd, const NABoxi32* boxs);
NA_IAPI void        naFillBoxi64WithCopy(NABoxi64* boxd, const NABoxi64* boxs);
NA_IAPI void        naFillBoxsWithCopy  (NABoxs*   boxd, const NABoxs*   boxs);

// NABezel

NA_IAPI NABezel4    naMakeBezel4   (double right, double top, double left, double bottom);
NA_IAPI NABezel4f   naMakeBezel4f  (float  right, float  top, float  left, float  bottom);
NA_IAPI NABezel4i32 naMakeBezel4i32(int32  right, int32  top, int32  left, int32  bottom);
NA_IAPI NABezel4i64 naMakeBezel4i64(int64  right, int64  top, int64  left, int64  bottom);
NA_IAPI NABezel4s   naMakeBezel4s  (size_t right, size_t top, size_t left, size_t bottom);

NA_IAPI NABezel4    naMakeBezel4Zero   (void);
NA_IAPI NABezel4f   naMakeBezel4fZero  (void);
NA_IAPI NABezel4i32 naMakeBezel4i32Zero(void);
NA_IAPI NABezel4i64 naMakeBezel4i64Zero(void);
NA_IAPI NABezel4s   naMakeBezel4sZero  (void);



// ////////////////
// Aligned ranges
//
// Creates a new origin which is the given origin floored to a multiple of
// the given alignment. Also works for negative input offsets (not for the
// s-variant though).
//
// The 1D case is covered by the naAlignValue functions.

NA_IAPI NAPos       naMakePosWithAlignment      (NAPos       origin, NARect    alignRect);
NA_IAPI NAPosf      naMakePosfWithAlignment     (NAPosf      origin, NARectf   alignRect);
NA_IAPI NAPosi32    naMakePosi32WithAlignment   (NAPosi32    origin, NARecti32 alignRect);
NA_IAPI NAPosi64    naMakePosi64WithAlignment   (NAPosi64    origin, NARecti64 alignRect);
NA_IAPI NAPoss      naMakePossWithAlignment     (NAPoss      origin, NARects   alignRect);

NA_IAPI NAVertex    naMakeVertexWithAlignment   (NAVertex    origin, NABox     alignBox);
NA_IAPI NAVertexf   naMakeVertexfWithAlignment  (NAVertexf   origin, NABoxf    alignBox);
NA_IAPI NAVertexi32 naMakeVertexi32WithAlignment(NAVertexi32 origin, NABoxi32  alignBox);
NA_IAPI NAVertexi64 naMakeVertexi64WithAlignment(NAVertexi64 origin, NABoxi64  alignBox);
NA_IAPI NAVertexs   naMakeVertexsWithAlignment  (NAVertexs   origin, NABoxs    alignBox);



// ///////////////////////
// Create structures by combining two elements
//
// The meaning in 2D is (1D and 3D accordingly):
// Combination:  Creates a rect such that both points are inside.
// Extension:    Enhances a rect such that the given point is inside.
// Union:        Creates the bounding box of two rects, hence containing both.
// Intersection: Returns the common rect when overlaying two rects.
// 
// The range, size or volume of the resulting struct will never be negative.
// But it might be empty.
//
// The Variants with E also allow the rects to be empty.

NA_IAPI NARange    naMakeRangeCombination    (double     point1, double     point2);
NA_IAPI NARangef   naMakeRangefCombination   (float      point1, float      point2);
NA_IAPI NARangei32 naMakeRangei32Combination (int32      point1, int32      point2);
NA_IAPI NARangei64 naMakeRangei64Combination (int64      point1, int64      point2);
NA_IAPI NARanges   naMakeRangesCombination   (size_t     point1, size_t     point2);

NA_IAPI NARange    naMakeRangeExtension      (NARange    range,  double     point);
NA_IAPI NARangef   naMakeRangefExtension     (NARangef   range,  float      point);
NA_IAPI NARangei32 naMakeRangei32Extension   (NARangei32 range,  int32      point);
NA_IAPI NARangei64 naMakeRangei64Extension   (NARangei64 range,  int64      point);
NA_IAPI NARanges   naMakeRangesExtension     (NARanges   range,  size_t     point);

NA_IAPI NARange    naMakeRangeExtensionE     (NARange    range,  double     point);
NA_IAPI NARangef   naMakeRangefExtensionE    (NARangef   range,  float      point);
NA_IAPI NARangei32 naMakeRangei32ExtensionE  (NARangei32 range,  int32      point);
NA_IAPI NARangei64 naMakeRangei64ExtensionE  (NARangei64 range,  int64      point);
NA_IAPI NARanges   naMakeRangesExtensionE    (NARanges   range,  size_t     point);
   
NA_IAPI NARange    naMakeRangeUnion          (NARange    range1, NARange    range2);
NA_IAPI NARangef   naMakeRangefUnion         (NARangef   range1, NARangef   range2);
NA_IAPI NARangei32 naMakeRangei32Union       (NARangei32 range1, NARangei32 range2);
NA_IAPI NARangei64 naMakeRangei64Union       (NARangei64 range1, NARangei64 range2);
NA_IAPI NARanges   naMakeRangesUnion         (NARanges   range1, NARanges   range2);

NA_IAPI NARange    naMakeRangeUnionE         (NARange    range1, NARange    range2);
NA_IAPI NARangef   naMakeRangefUnionE        (NARangef   range1, NARangef   range2);
NA_IAPI NARangei32 naMakeRangei32UnionE      (NARangei32 range1, NARangei32 range2);
NA_IAPI NARangei64 naMakeRangei64UnionE      (NARangei64 range1, NARangei64 range2);
NA_IAPI NARanges   naMakeRangesUnionE        (NARanges   range1, NARanges   range2);

NA_IAPI NARange    naMakeRangeIntersection   (NARange    range1, NARange    range2);
NA_IAPI NARangef   naMakeRangefIntersection  (NARangef   range1, NARangef   range2);
NA_IAPI NARangei32 naMakeRangei32Intersection(NARangei32 range1, NARangei32 range2);
NA_IAPI NARangei64 naMakeRangei64Intersection(NARangei64 range1, NARangei64 range2);
NA_IAPI NARanges   naMakeRangesIntersection  (NARanges   range1, NARanges   range2);



NA_IAPI NARect    naMakeRectCombination    (NAPos     point1, NAPos     point2);
NA_IAPI NARectf   naMakeRectfCombination   (NAPosf    point1, NAPosf    point2);
NA_IAPI NARecti32 naMakeRecti32Combination (NAPosi32  point1, NAPosi32  point2);
NA_IAPI NARecti64 naMakeRecti64Combination (NAPosi64  point1, NAPosi64  point2);
NA_IAPI NARects   naMakeRectsCombination   (NAPoss    point1, NAPoss    point2);

NA_IAPI NARect    naMakeRectExtension      (NARect    rect,   NAPos     point);
NA_IAPI NARectf   naMakeRectfExtension     (NARectf   rect,   NAPosf    point);
NA_IAPI NARecti32 naMakeRecti32Extension   (NARecti32 rect,   NAPosi32  point);
NA_IAPI NARecti64 naMakeRecti64Extension   (NARecti64 rect,   NAPosi64  point);
NA_IAPI NARects   naMakeRectsExtension     (NARects   rect,   NAPoss    point);

NA_IAPI NARect    naMakeRectExtensionE     (NARect    rect,   NAPos     point);
NA_IAPI NARectf   naMakeRectfExtensionE    (NARectf   rect,   NAPosf    point);
NA_IAPI NARecti32 naMakeRecti32ExtensionE  (NARecti32 rect,   NAPosi32  point);
NA_IAPI NARecti64 naMakeRecti64ExtensionE  (NARecti64 rect,   NAPosi64  point);
NA_IAPI NARects   naMakeRectsExtensionE    (NARects   rect,   NAPoss    point);

NA_IAPI NARect    naMakeRectUnion          (NARect    rect1,  NARect    rect2);
NA_IAPI NARectf   naMakeRectfUnion         (NARectf   rect1,  NARectf   rect2);
NA_IAPI NARecti32 naMakeRecti32Union       (NARecti32 rect1,  NARecti32 rect2);
NA_IAPI NARecti64 naMakeRecti64Union       (NARecti64 rect1,  NARecti64 rect2);
NA_IAPI NARects   naMakeRectsUnion         (NARects   rect1,  NARects   rect2);

NA_IAPI NARect    naMakeRectUnionE         (NARect    rect1,  NARect    rect2);
NA_IAPI NARectf   naMakeRectfUnionE        (NARectf   rect1,  NARectf   rect2);
NA_IAPI NARecti32 naMakeRecti32UnionE      (NARecti32 rect1,  NARecti32 rect2);
NA_IAPI NARecti64 naMakeRecti64UnionE      (NARecti64 rect1,  NARecti64 rect2);
NA_IAPI NARects   naMakeRectsUnionE        (NARects   rect1,  NARects   rect2);

NA_IAPI NARect    naMakeRectIntersection   (NARect    rect1,  NARect    rect2);
NA_IAPI NARectf   naMakeRectfIntersection  (NARectf   rect1,  NARectf   rect2);
NA_IAPI NARecti32 naMakeRecti32Intersection(NARecti32 rect1,  NARecti32 rect2);
NA_IAPI NARecti64 naMakeRecti64Intersection(NARecti64 rect1,  NARecti64 rect2);
NA_IAPI NARects   naMakeRectsIntersection  (NARects   rect1,  NARects   rect2);



NA_IAPI NABox    naMakeBoxCombination    (NAVertex    point1, NAVertex    point2);
NA_IAPI NABoxf   naMakeBoxfCombination   (NAVertexf   point1, NAVertexf   point2);
NA_IAPI NABoxi32 naMakeBoxi32Combination (NAVertexi32 point1, NAVertexi32 point2);
NA_IAPI NABoxi64 naMakeBoxi64Combination (NAVertexi64 point1, NAVertexi64 point2);
NA_IAPI NABoxs   naMakeBoxsCombination   (NAVertexs   point1, NAVertexs   point2);

NA_IAPI NABox    naMakeBoxExtension      (NABox       box,    NAVertex    vertex);
NA_IAPI NABoxf   naMakeBoxfExtension     (NABoxf      box,    NAVertexf   vertex);
NA_IAPI NABoxi32 naMakeBoxi32Extension   (NABoxi32    box,    NAVertexi32 vertex);
NA_IAPI NABoxi64 naMakeBoxi64Extension   (NABoxi64    box,    NAVertexi64 vertex);
NA_IAPI NABoxs   naMakeBoxsExtension     (NABoxs      box,    NAVertexs   vertex);

NA_IAPI NABox    naMakeBoxExtensionE     (NABox       box,    NAVertex    vertex);
NA_IAPI NABoxf   naMakeBoxfExtensionE    (NABoxf      box,    NAVertexf   vertex);
NA_IAPI NABoxi32 naMakeBoxi32ExtensionE  (NABoxi32    box,    NAVertexi32 vertex);
NA_IAPI NABoxi64 naMakeBoxi64ExtensionE  (NABoxi64    box,    NAVertexi64 vertex);
NA_IAPI NABoxs   naMakeBoxsExtensionE    (NABoxs      box,    NAVertexs   vertex);

NA_IAPI NABox    naMakeBoxUnion          (NABox       box1,   NABox       box2);
NA_IAPI NABoxf   naMakeBoxfUnion         (NABoxf      box1,   NABoxf      box2);
NA_IAPI NABoxi32 naMakeBoxi32Union       (NABoxi32    box1,   NABoxi32    box2);
NA_IAPI NABoxi64 naMakeBoxi64Union       (NABoxi64    box1,   NABoxi64    box2);
NA_IAPI NABoxs   naMakeBoxsUnion         (NABoxs      box1,   NABoxs      box2);

NA_IAPI NABox    naMakeBoxUnionE         (NABox       box1,   NABox       box2);
NA_IAPI NABoxf   naMakeBoxfUnionE        (NABoxf      box1,   NABoxf      box2);
NA_IAPI NABoxi32 naMakeBoxi32UnionE      (NABoxi32    box1,   NABoxi32    box2);
NA_IAPI NABoxi64 naMakeBoxi64UnionE      (NABoxi64    box1,   NABoxi64    box2);
NA_IAPI NABoxs   naMakeBoxsUnionE        (NABoxs      box1,   NABoxs      box2);

NA_IAPI NABox    naMakeBoxIntersection   (NABox       box1,   NABox       box2);
NA_IAPI NABoxf   naMakeBoxfIntersection  (NABoxf      box1,   NABoxf      box2);
NA_IAPI NABoxi32 naMakeBoxi32Intersection(NABoxi32    box1,   NABoxi32    box2);
NA_IAPI NABoxi64 naMakeBoxi64Intersection(NABoxi64    box1,   NABoxi64    box2);
NA_IAPI NABoxs   naMakeBoxsIntersection  (NABoxs      box1,   NABoxs      box2);



// Creates a new rect by adding the given border around the given rect.
NA_IAPI NARect    naMakeRectWithRectAndBorder  (NARect  rect, double border);
NA_IAPI NARecti32   naMakeRecti32WithRectAndBorder (NARecti32 rect, int32  border);
NA_IAPI NARecti64   naMakeRecti64WithRectAndBorder (NARecti64 rect, int64  border);

NA_IAPI NABox     naMakeBoxWithBoxAndBorder    (NABox  box, double border);
NA_IAPI NABoxi32    naMakeBoxi32WithBoxAndBorder   (NABoxi32 box, int32  border);
NA_IAPI NABoxi64    naMakeBoxi64WithBoxAndBorder   (NABoxi64 box, int64  border);



// The naMakeXXXXAbsolute function ensures that the lengths/sizes/volumes of
// the given struct are not negative. If the these already were positive or
// zero, they will not be changed.
// For example:  A rect with pos(4., 6.) and size (-2., 7.) will change to
//            -> a rect with pos(2., 6.) and size ( 2., 7.) .
// But: An integer rect with pos(4 , 6 ) and size (-2 , 7 ) will change to
//            -> a rect with pos(3 , 6 ) and size ( 2 , 7 ) !
NA_IAPI NARange naMakeRangePositive(NARange range);
NA_IAPI NARangei32 naMakeRangei32Positive(NARangei32 range);
NA_IAPI NARangei64 naMakeRangei64Positive(NARangei64 range);

NA_IAPI NARect naMakeRectPositive(NARect rect);
NA_IAPI NARecti32 naMakeRecti32Positive(NARecti32 rect);
NA_IAPI NARecti64 naMakeRecti64Positive(NARecti64 rect);

NA_IAPI NABoxi32  naMakeBoxi32Positive(NABoxi32 box);
NA_IAPI NABoxi64  naMakeBoxi64Positive(NABoxi64 box);


// Returns the center position / vertex
NA_IAPI NAPos     naGetRectCenter (NARect rect);
NA_IAPI NAPosi32  naGetRecti32Center(NARecti32 rect);
NA_IAPI NAPosi64  naGetRecti64Center(NARecti64 rect);

NA_IAPI NAVertex  naGetBoxCenter  (NABox box);
NA_IAPI NAVertexi32 naGetBoxi32Center (NABoxi32 box);
NA_IAPI NAVertexi64 naGetBoxi64Center (NABoxi64 box);

// Returns the offset of a size or volume being centered within a given rect
// or box relative to the origin of that rect or box.
NA_IAPI NAPos     naGetRectCenteredSizeOffset  (NARect rect, NASize   size);
NA_IAPI NAVertex  naGetBoxCeneteredVolumeOffset(NABox  box,  NAVolume volume);

// Clamping functions. Returns the new range.
NA_IAPI NARangei32 naClampRangei32ToRange(NARangei32 range, NARangei32 clampRange);
NA_IAPI NARangei64 naClampRangei64ToRange(NARangei64 range, NARangei64 clampRange);
NA_IAPI NARangei32 naClampRangei32ToMin  (NARangei32 range, int32    min);
NA_IAPI NARangei64 naClampRangei64ToMin  (NARangei64 range, int64    min);
NA_IAPI NARangei32 naClampRangei32ToEnd  (NARangei32 range, int32    end);
NA_IAPI NARangei64 naClampRangei64ToEnd  (NARangei64 range, int64    end);

NA_IAPI NAPos     naClampPosToRect  (NAPos   pos,  NARect  clampRect);
NA_IAPI NAPosi32    naClampPosi32ToRect (NAPosi32  pos,  NARecti32 clampRect);
NA_IAPI NAPosi64    naClampPosi64ToRect (NAPosi64  pos,  NARecti64 clampRect);

NA_IAPI NARect    naClampRect (NARect  rect, NARect  clampRect);
NA_IAPI NARecti32 naClampRecti32(NARecti32 rect, NARecti32 clampRect);
NA_IAPI NARecti64 naClampRecti64(NARecti64 rect, NARecti64 clampRect);

NA_IAPI NAVertex  naClampVertexToBox (NAVertex  vertex, NABox  clampBox);
NA_IAPI NAVertexi32 naClampVertexi32ToBox(NAVertexi32 vertex, NABoxi32 clampBox);
NA_IAPI NAVertexi64 naClampVertexi64ToBox(NAVertexi64 vertex, NABoxi64 clampBox);

NA_IAPI NABox     naClampBoxToBox    (NABox     box,    NABox  clampBox);
NA_IAPI NABoxi32    naClampBoxi32ToBox   (NABoxi32    box,    NABoxi32 clampBox);
NA_IAPI NABoxi64    naClampBoxi64ToBox   (NABoxi64    box,    NABoxi64 clampBox);

// Use the following functions to make equality tests (==) between elements.
NA_IAPI NABool    naEqualRangei32(NARangei32 range1, NARangei32 range2);
NA_IAPI NABool    naEqualRangei64(NARangei64 range1, NARangei64 range2);

NA_IAPI NABool    naEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naEqualPosi32 (NAPosi32  pos1,  NAPosi32  pos2);
NA_IAPI NABool    naEqualPosi64 (NAPosi64  pos1,  NAPosi64  pos2);

NA_IAPI NABool    naEqualSize (NASize  size1, NASize  size2);
NA_IAPI NABool    naEqualSizei32(NASizei32 size1, NASizei32 size2);
NA_IAPI NABool    naEqualSizei64(NASizei64 size1, NASizei64 size2);
NA_IAPI NABool    naEqualSizes(NASizes size1, NASizes size2);

NA_IAPI NABool    naEqualRect (NARect  rect1, NARect  rect2);
NA_IAPI NABool    naEqualRecti32(NARecti32 rect1, NARecti32 rect2);
NA_IAPI NABool    naEqualRecti64(NARecti64 rect1, NARecti64 rect2);

NA_IAPI NABool    naEqualVertex (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naEqualVertexi32(NAVertexi32 vertex1, NAVertexi32 vertex2);
NA_IAPI NABool    naEqualVertexi64(NAVertexi64 vertex1, NAVertexi64 vertex2);

NA_IAPI NABool    naEqualVolume (NAVolume  volume1, NAVolume  volume2);
NA_IAPI NABool    naEqualVolumei32(NAVolumei32 volume1, NAVolumei32 volume2);
NA_IAPI NABool    naEqualVolumei64(NAVolumei64 volume1, NAVolumei64 volume2);

NA_IAPI NABool    naEqualBox    (NABox     box1,    NABox     box2);
NA_IAPI NABool    naEqualBoxi32   (NABoxi32    box1,    NABoxi32    box2);
NA_IAPI NABool    naEqualBoxi64   (NABoxi64    box1,    NABoxi64    box2);

// Use the following functions to make comparison tests between elements.
// They only return true if the condition is true for all components: xyz.
NA_IAPI NABool    naLessPos       (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naLessEqualPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naGreaterEqualPos(NAPos   pos1,  NAPos   pos2);
NA_IAPI NABool    naGreaterPos     (NAPos   pos1,  NAPos   pos2);

NA_IAPI NABool    naLessVertex        (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naLessEqualVertex   (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naGreaterEqualVertex (NAVertex  vertex1, NAVertex  vertex2);
NA_IAPI NABool    naGreaterVertex      (NAVertex  vertex1, NAVertex  vertex2);

// Adding and subtracting delta values: A delta is a size or volume.
// Adding a size to a position returns a position. Subtracting two positions
// from each other returns a size.
//
// The returned NASize can be negative or even invalid.
NA_IAPI NAPos     naAddPosSize (NAPos   pos,   NASize  size);
NA_IAPI NAPosi32    naAddPosi32Size(NAPosi32  pos,   NASizei32 size);
NA_IAPI NAPosi64    naAddPosi64Size(NAPosi64  pos,   NASizei64 size);

NA_IAPI NASize    naSubPosPos  (NAPos   pos1,  NAPos   pos2);
NA_IAPI NASizei32   naSubPosi32Pos (NAPosi32  pos1,  NAPosi32  pos2);
NA_IAPI NASizei64   naSubPosi64Pos (NAPosi64  pos1,  NAPosi64  pos2);

NA_IAPI double    naDistancePosPos  (NAPos   pos1,  NAPos   pos2);

NA_IAPI NAVertex  naAddVertexVolume (NAVertex   vertex,   NAVolume  volume);
NA_IAPI NAVertexi32 naAddVertexi32Volume(NAVertexi32  vertex,   NAVolumei32 volume);
NA_IAPI NAVertexi64 naAddVertexi64Volume(NAVertexi64  vertex,   NAVolumei64 volume);

NA_IAPI NAVolume  naSubVertexPos    (NAVertex   vertex1,  NAVertex  vertex2);
NA_IAPI NAVolumei32 naSubVertexi32Vertex(NAVertexi32  vertex1,  NAVertexi32 vertex2);
NA_IAPI NAVolumei64 naSubVertexi64Vertex(NAVertexi64  vertex1,  NAVertexi64 vertex2);

// Containing functions. Test if an element is within another.
// The E variant for the double types denotes an exclusive rangecheck [...)
// instead of an includsive [...]
NA_IAPI NABool naContainsRangeOffset   (NARange  outerRange,   double     offset);
NA_IAPI NABool naContainsRangefOffset  (NARangef outerRange,   float      offset);
NA_IAPI NABool naContainsRangei32Offset(NARangei32 outerRange, int32      offset);
NA_IAPI NABool naContainsRangei64Offset(NARangei64 outerRange, int64      offset);

NA_IAPI NABool naContainsRangei32Range (NARangei32 outerRange, NARangei32 range);
NA_IAPI NABool naContainsRangei64Range (NARangei64 outerRange, NARangei64 range);

NA_IAPI NABool    naContainsSizePos    (NASize    outerSize, NAPos     pos);
NA_IAPI NABool    naContainsSizei32Pos   (NASizei32   outerSize, NAPosi32    pos);
NA_IAPI NABool    naContainsSizei64Pos   (NASizei64   outerSize, NAPosi64    pos);

NA_IAPI NABool    naContainsRectPos    (NARect    outerRect, NAPos     pos);
NA_IAPI NABool    naContainsRecti32Pos (NARecti32 outerRect, NAPosi32  pos);
NA_IAPI NABool    naContainsRecti64Pos (NARecti64 outerRect, NAPosi64  pos);
NA_IAPI NABool    naContainsRectsPos   (NARects   outerRect, NAPoss    pos);

NA_IAPI NABool    naContainsRectPosE   (NARect    outerRect, NAPos     pos);

NA_IAPI NABool    naContainsSizeSize   (NASize    outerSize, NASize    size);
NA_IAPI NABool    naContainsSizei32Size  (NASizei32   outerSize, NASizei32   size);
NA_IAPI NABool    naContainsSizei64Size  (NASizei64   outerSize, NASizei64   size);

NA_IAPI NABool    naContainsRectRect   (NARect    outerRect, NARect    rect);
NA_IAPI NABool    naContainsRecti32Rect(NARecti32 outerRect, NARecti32 rect);
NA_IAPI NABool    naContainsRecti64Rect(NARecti64 outerRect, NARecti64 rect);

NA_IAPI NABool    naContainsVolumeVertex (NAVolume  outerVolume, NAVertex  vertex);
NA_IAPI NABool    naContainsVolumei32Vertex(NAVolumei32 outerVolume, NAVertexi32 vertex);
NA_IAPI NABool    naContainsVolumei64Vertex(NAVolumei64 outerVolume, NAVertexi64 vertex);

NA_IAPI NABool    naContainsBoxVertex    (NABox     outerBox,    NAVertex  vertex);
NA_IAPI NABool    naContainsBoxVertexE   (NABox     outerBox,    NAVertex  vertex);
NA_IAPI NABool    naContainsBoxi32Vertex (NABoxi32  outerBox,    NAVertexi32 vertex);
NA_IAPI NABool    naContainsBoxi64Vertex (NABoxi64  outerBox,    NAVertexi64 vertex);
NA_IAPI NABool    naContainsBoxsVertex   (NABoxs    outerBox,    NAVertexs   vertex);

NA_IAPI NABool    naContainsVolumeVolume (NAVolume  outerVolume, NAVolume  volume);
NA_IAPI NABool    naContainsVolumei32Volume(NAVolumei32 outerVolume, NAVolumei32 volume);
NA_IAPI NABool    naContainsVolumei64Volume(NAVolumei64 outerVolume, NAVolumei64 volume);

NA_IAPI NABool    naContainsBoxBox       (NABox     outerBox,    NABox     box);
NA_IAPI NABool    naContainsBoxi32Box      (NABoxi32    outerBox,    NABoxi32    box);
NA_IAPI NABool    naContainsBoxi64Box      (NABoxi64    outerBox,    NABoxi64    box);



// /////////////////////////
// End and Max functions
//
// Computes the positions opposite to the origin of the rect.
// End returns origin + size
// Max returns oritin + size - 1   (only useful for integer variants)
// The E variants allow the given struct to be empty.

NA_IAPI double    naGetRangeEnd    (NARange    range);
NA_IAPI float     naGetRangefEnd   (NARangef   range);
NA_IAPI int32     naGetRangei32End (NARangei32 range);
NA_IAPI int64     naGetRangei64End (NARangei64 range);
NA_IAPI size_t    naGetRangesEnd   (NARanges   range);

NA_IAPI double    naGetRangeEndE   (NARange    range);
NA_IAPI float     naGetRangefEndE  (NARangef   range);
NA_IAPI int32     naGetRangei32EndE(NARangei32 range);
NA_IAPI int64     naGetRangei64EndE(NARangei64 range);
NA_IAPI size_t    naGetRangesEndE  (NARanges   range);

NA_IAPI int32     naGetRangei32Max (NARangei32 range);
NA_IAPI int64     naGetRangei64Max (NARangei64 range);
NA_IAPI size_t    naGetRangesMax   (NARanges   range);



NA_IAPI NAPos     naGetRectEnd     (NARect     rect);
NA_IAPI NAPosf    naGetRectfEnd    (NARectf    rect);
NA_IAPI NAPosi32  naGetRecti32End  (NARecti32  rect);
NA_IAPI NAPosi64  naGetRecti64End  (NARecti64  rect);
NA_IAPI NAPoss    naGetRectsEnd    (NARects    rect);

NA_IAPI double    naGetRectEndX    (NARect     rect);
NA_IAPI float     naGetRectfEndX   (NARectf    rect);
NA_IAPI int32     naGetRecti32EndX (NARecti32  rect);
NA_IAPI int64     naGetRecti64EndX (NARecti64  rect);
NA_IAPI size_t    naGetRectsEndX   (NARects    rect);

NA_IAPI double    naGetRectEndY    (NARect     rect);
NA_IAPI float     naGetRectfEndY   (NARectf    rect);
NA_IAPI int32     naGetRecti32EndY (NARecti32  rect);
NA_IAPI int64     naGetRecti64EndY (NARecti64  rect);
NA_IAPI size_t    naGetRectsEndY   (NARects    rect);

NA_IAPI double    naGetRectEndXE   (NARect     rect);
NA_IAPI float     naGetRectfEndXE  (NARectf    rect);
NA_IAPI int32     naGetRecti32EndXE(NARecti32  rect);
NA_IAPI int64     naGetRecti64EndXE(NARecti64  rect);
NA_IAPI size_t    naGetRectsEndXE  (NARects    rect);

NA_IAPI double    naGetRectEndYE   (NARect     rect);
NA_IAPI float     naGetRectfEndYE  (NARectf    rect);
NA_IAPI int32     naGetRecti32EndYE(NARecti32  rect);
NA_IAPI int64     naGetRecti64EndYE(NARecti64  rect);
NA_IAPI size_t    naGetRectsEndYE  (NARects    rect);

NA_IAPI NAPosi32  naGetRecti32Max  (NARecti32  rect);
NA_IAPI NAPosi64  naGetRecti64Max  (NARecti64  rect);
NA_IAPI NAPoss    naGetRectsMax    (NARects    rect);

NA_IAPI int32     naGetRecti32MaxX (NARecti32  rect);
NA_IAPI int64     naGetRecti64MaxX (NARecti64  rect);
NA_IAPI size_t    naGetRectsMaxX   (NARects    rect);

NA_IAPI int32     naGetRecti32MaxY (NARecti32  rect);
NA_IAPI int64     naGetRecti64MaxY (NARecti64  rect);
NA_IAPI size_t    naGetRectsMaxY   (NARects    rect);



NA_IAPI NAVertex    naGetBoxEnd     (NABox    box);
NA_IAPI NAVertexf   naGetBoxfEnd    (NABoxf   box);
NA_IAPI NAVertexi32 naGetBoxi32End  (NABoxi32 box);
NA_IAPI NAVertexi64 naGetBoxi64End  (NABoxi64 box);
NA_IAPI NAVertexs   naGetBoxsEnd    (NABoxs   box);

NA_IAPI double      naGetBoxEndX    (NABox    box);
NA_IAPI float       naGetBoxfEndX   (NABoxf   box);
NA_IAPI int32       naGetBoxi32EndX (NABoxi32 box);
NA_IAPI int64       naGetBoxi64EndX (NABoxi64 box);
NA_IAPI size_t      naGetBoxsEndX   (NABoxs   box);

NA_IAPI double      naGetBoxEndY    (NABox    box);
NA_IAPI float       naGetBoxfEndY   (NABoxf   box);
NA_IAPI int32       naGetBoxi32EndY (NABoxi32 box);
NA_IAPI int64       naGetBoxi64EndY (NABoxi64 box);
NA_IAPI size_t      naGetBoxsEndY   (NABoxs   box);

NA_IAPI double      naGetBoxEndZ    (NABox    box);
NA_IAPI float       naGetBoxfEndZ   (NABoxf   box);
NA_IAPI int32       naGetBoxi32EndZ (NABoxi32 box);
NA_IAPI int64       naGetBoxi64EndZ (NABoxi64 box);
NA_IAPI size_t      naGetBoxsEndZ   (NABoxs   box);

NA_IAPI double      naGetBoxEndXE   (NABox    box);
NA_IAPI float       naGetBoxfEndXE  (NABoxf   box);
NA_IAPI int32       naGetBoxi32EndXE(NABoxi32 box);
NA_IAPI int64       naGetBoxi64EndXE(NABoxi64 box);
NA_IAPI size_t      naGetBoxsEndXE  (NABoxs   box);

NA_IAPI double      naGetBoxEndYE   (NABox    box);
NA_IAPI float       naGetBoxfEndYE  (NABoxf   box);
NA_IAPI int32       naGetBoxi32EndYE(NABoxi32 box);
NA_IAPI int64       naGetBoxi64EndYE(NABoxi64 box);
NA_IAPI size_t      naGetBoxsEndYE  (NABoxs   box);

NA_IAPI double      naGetBoxEndZE   (NABox    box);
NA_IAPI float       naGetBoxfEndZE  (NABoxf   box);
NA_IAPI int32       naGetBoxi32EndZE(NABoxi32 box);
NA_IAPI int64       naGetBoxi64EndZE(NABoxi64 box);
NA_IAPI size_t      naGetBoxsEndZE  (NABoxs   box);

NA_IAPI NAVertexi32 naGetBoxi32Max  (NABoxi32 box);
NA_IAPI NAVertexi64 naGetBoxi64Max  (NABoxi64 box);
NA_IAPI NAVertexs   naGetBoxsMax    (NABoxs   box);

NA_IAPI int32       naGetBoxi32MaxX (NABoxi32 box);
NA_IAPI int64       naGetBoxi64MaxX (NABoxi64 box);
NA_IAPI size_t      naGetBoxsMaxX   (NABoxs   box);

NA_IAPI int32       naGetBoxi32MaxY (NABoxi32 box);
NA_IAPI int64       naGetBoxi64MaxY (NABoxi64 box);
NA_IAPI size_t      naGetBoxsMaxY   (NABoxs   box);

NA_IAPI int32       naGetBoxi32MaxZ (NABoxi32 box);
NA_IAPI int64       naGetBoxi64MaxZ (NABoxi64 box);
NA_IAPI size_t      naGetBoxsMaxZ   (NABoxs   box);



// //////////////////////
// Index functions

// Count returns the total number of indices (width*height) within a size.
NA_IAPI size_t naGetSizei32IndexCount  (NASizei32 size);
NA_IAPI size_t naGetSizei64IndexCount  (NASizei64 size);
NA_IAPI size_t naGetSizesIndexCount    (NASizes   size);

NA_IAPI size_t naGetVolumei32IndexCount(NAVolumei32 volume);
NA_IAPI size_t naGetVolumei64IndexCount(NAVolumei64 volume);
NA_IAPI size_t naGetVolumesIndexCount  (NAVolumes volume);

// Raw Index functions. Assuming origin and width denote the indices in a
// multi-dimensional array which is stored as a 1D-Array.
// Note that row-first is the default C ordering.
NA_IAPI size_t naGetIndexWithOrigini32AndPosRowFirst      (NAPosi32    origin, NAPosi32  offset, int32  width);
NA_IAPI size_t naGetIndexWithOrigini64AndPosRowFirst      (NAPosi64    origin, NAPosi64  offset, int64  width);
NA_IAPI size_t naGetIndexWithOriginsAndPosRowFirst        (NAPoss      origin, NAPoss    offset, size_t width);

NA_IAPI size_t naGetIndexWithOrigini32AndPosColumnFirst   (NAPosi32    origin, NAPosi32  offset, int32  height);
NA_IAPI size_t naGetIndexWithOrigini64AndPosColumnFirst   (NAPosi64    origin, NAPosi64  offset, int64  height);
NA_IAPI size_t naGetIndexWithOriginsAndPosColumnFirst     (NAPoss      origin, NAPoss    offset, size_t height);

NA_IAPI size_t naGetIndexWithOrigini32AndVertexRowFirst   (NAVertexi32 origin, NAVertexi32 vertex, int32  width, int32  height);
NA_IAPI size_t naGetIndexWithOrigini64AndVertexRowFirst   (NAVertexi64 origin, NAVertexi64 vertex, int64  width, int64  height);
NA_IAPI size_t naGetIndexWithOriginsAndVertexRowFirst     (NAVertexs   origin, NAVertexs   vertex, size_t width, size_t height);

NA_IAPI size_t naGetIndexWithOrigini32AndVertexColumnFirst(NAVertexi32 origin, NAVertexi32 vertex, int32  depth, int32  height);
NA_IAPI size_t naGetIndexWithOrigini64AndVertexColumnFirst(NAVertexi64 origin, NAVertexi64 vertex, int64  depth, int64  height);
NA_IAPI size_t naGetIndexWithOriginsAndVertexColumnFirst  (NAVertexs   origin, NAVertexs   vertex, size_t depth, size_t height);

// IndexOf returns the index of the corresponding 1-dimensional array.
//
// Example:    Rect(Pos(2, 5), Size(4, 3)) describes a 2D-field. It stores
//             a corresponding 1D-Array {a, b, c, d, e, f, g, h, i, j, k, l}.
// (a b c d)   End is (6, 8), Max is (5, 7). Count is 3*4 = 12
// (e f g h)   The result of naGetRectiIndexOfPosi with position (5, 6)
// (i j k l)   returns 7, the index of 'h'
NA_IAPI size_t naGetRecti32IndexOfPosRowFirst     (NARecti32 rect, NAPosi32 pos);
NA_IAPI size_t naGetRecti64IndexOfPosRowFirst     (NARecti64 rect, NAPosi64 pos);
NA_IAPI size_t naGetRectsIndexOfPosRowFirst       (NARects   rect, NAPoss   pos);

NA_IAPI size_t naGetRecti32IndexOfPosColumnFirst  (NARecti32 rect, NAPosi32 pos);
NA_IAPI size_t naGetRecti64IndexOfPosColumnFirst  (NARecti64 rect, NAPosi64 pos);
NA_IAPI size_t naGetRectsIndexOfPosColumnFirst    (NARects   rect, NAPoss   pos);

NA_IAPI size_t naGetBoxi32IndexOfVertexRowFirst   (NABoxi32  box, NAVertexi32 vertex);
NA_IAPI size_t naGetBoxi64IndexOfVertexRowFirst   (NABoxi64  box, NAVertexi64 vertex);
NA_IAPI size_t naGetBoxsIndexOfVertexRowFirst     (NABoxs    box, NAVertexs   vertex);

NA_IAPI size_t naGetBoxi32IndexOfVertexColumnFirst(NABoxi32  box, NAVertexi32 vertex);
NA_IAPI size_t naGetBoxi64IndexOfVertexColumnFirst(NABoxi64  box, NAVertexi64 vertex);
NA_IAPI size_t naGetBoxsIndexOfVertexColumnFirst  (NABoxs    box, NAVertexs   vertex);



// //////////////////////
// Test functions

// IsValid:
//
// Returns NA_TRUE when all values of the fields are valid.
// Following are the definitions of "valid" elements used in NALib:
// Pos:  A position is considered valid if none of the fields is NaN.
//       Integer positions are always valid.
// Size: A size is considered valid, if none of the fields is NaN.
//       A size of zero is considered valid because this is commonly used to
//       mark empty sizes.
// Rect: A rectangle is considered valid, if its position and size is valid.
//
// The same holds true similarly for the 1D- and 3D-elements.

NA_IAPI NABool    naIsRangeValid    (NARange     range);
NA_IAPI NABool    naIsRangefValid   (NARangef    range);
NA_IAPI NABool    naIsRangei32Valid (NARangei32  range);
NA_IAPI NABool    naIsRangei64Valid (NARangei64  range);
NA_IAPI NABool    naIsRangesValid   (NARanges    range);

NA_IAPI NABool    naIsPosValid      (NAPos       pos);
NA_IAPI NABool    naIsPosfValid     (NAPosf      pos);
NA_IAPI NABool    naIsPosi32Valid   (NAPosi32    pos);
NA_IAPI NABool    naIsPosi64Valid   (NAPosi64    pos);
NA_IAPI NABool    naIsPossValid     (NAPoss      pos);

NA_IAPI NABool    naIsSizeValid     (NASize      size);
NA_IAPI NABool    naIsSizefValid    (NASizef     size);
NA_IAPI NABool    naIsSizei32Valid  (NASizei32   size);
NA_IAPI NABool    naIsSizei64Valid  (NASizei64   size);
NA_IAPI NABool    naIsSizesValid    (NASizes     size);

NA_IAPI NABool    naIsRectValid     (NARect      rect);
NA_IAPI NABool    naIsRectfValid    (NARectf     rect);
NA_IAPI NABool    naIsRecti32Valid  (NARecti32   rect);
NA_IAPI NABool    naIsRecti64Valid  (NARecti64   rect);
NA_IAPI NABool    naIsRectsValid    (NARects     rect);

NA_IAPI NABool    naIsVertexValid   (NAVertex    vertex);
NA_IAPI NABool    naIsVertexfValid  (NAVertexf   vertex);
NA_IAPI NABool    naIsVertexi32Valid(NAVertexi32 vertex);
NA_IAPI NABool    naIsVertexi64Valid(NAVertexi64 vertex);
NA_IAPI NABool    naIsVertexsValid  (NAVertexs   vertex);

NA_IAPI NABool    naIsVolumeValid   (NAVolume    volume);
NA_IAPI NABool    naIsVolumefValid  (NAVolumef   volume);
NA_IAPI NABool    naIsVolumei32Valid(NAVolumei32 volume);
NA_IAPI NABool    naIsVolumei64Valid(NAVolumei64 volume);
NA_IAPI NABool    naIsVolumesValid  (NAVolumes   volume);

NA_IAPI NABool    naIsBoxValid      (NABox       box);
NA_IAPI NABool    naIsBoxfValid     (NABoxf      box);
NA_IAPI NABool    naIsBoxi32Valid   (NABoxi32    box);
NA_IAPI NABool    naIsBoxi64Valid   (NABoxi64    box);
NA_IAPI NABool    naIsBoxsValid     (NABoxs      box);

// IsEmpty:
//
// Tests whether the range, size of volume evaluates to zero.
// The slow method might be necessary if you experience integer overflows.
// NALib will tell you when debugging.
//
// When debugging, NALib checks if a size or rect is empty where it should
// not be. Use the functions marked with E if you are sure empty sizes are ok.
//
// Negative sizes are not uncommon as well. While not forbidden and perfectly
// meaningful, the functions of NALib only work properly with positive rects.
// Use the naMakeRectPositive function to convert a negative rect into a
// positive one.

NA_IAPI NABool    naIsRangeEmpty        (NARange     range);
NA_IAPI NABool    naIsRangefEmpty       (NARangef    range);
NA_IAPI NABool    naIsRangei32Empty     (NARangei32  range);
NA_IAPI NABool    naIsRangei64Empty     (NARangei64  range);
NA_IAPI NABool    naIsRangesEmpty       (NARanges    range);

NA_IAPI NABool    naIsSizeEmpty         (NASize      size);
NA_IAPI NABool    naIsSizefEmpty        (NASizef     size);
NA_IAPI NABool    naIsSizei32Empty      (NASizei32   size);
NA_IAPI NABool    naIsSizei64Empty      (NASizei64   size);
NA_IAPI NABool    naIsSizesEmpty        (NASizes     size);

NA_IAPI NABool    naIsSizei32EmptySlow  (NASizei32   size);
NA_IAPI NABool    naIsSizei64EmptySlow  (NASizei64   size);
NA_IAPI NABool    naIsSizesEmptySlow    (NASizes     size);

NA_IAPI NABool    naIsRectEmpty         (NARect      rect);
NA_IAPI NABool    naIsRectfEmpty        (NARectf     rect);
NA_IAPI NABool    naIsRecti32Empty      (NARecti32   rect);
NA_IAPI NABool    naIsRecti64Empty      (NARecti64   rect);
NA_IAPI NABool    naIsRectsEmpty        (NARects     rect);

NA_IAPI NABool    naIsRecti32EmptySlow  (NARecti32   rect);
NA_IAPI NABool    naIsRecti64EmptySlow  (NARecti64   rect);
NA_IAPI NABool    naIsRectsEmptySlow    (NARects     rect);

NA_IAPI NABool    naIsVolumeEmpty       (NAVolume    volume);
NA_IAPI NABool    naIsVolumefEmpty      (NAVolumef   volume);
NA_IAPI NABool    naIsVolumei32Empty    (NAVolumei32 volume);
NA_IAPI NABool    naIsVolumei64Empty    (NAVolumei64 volume);
NA_IAPI NABool    naIsVolumesEmpty      (NAVolumes   volume);

NA_IAPI NABool    naIsVolumei32EmptySlow(NAVolumei32 volume);
NA_IAPI NABool    naIsVolumei64EmptySlow(NAVolumei64 volume);
NA_IAPI NABool    naIsVolumesEmptySlow  (NAVolumes   volume);

NA_IAPI NABool    naIsBoxEmpty          (NABox       box);
NA_IAPI NABool    naIsBoxfEmpty         (NABoxf      box);
NA_IAPI NABool    naIsBoxi32Empty       (NABoxi32    box);
NA_IAPI NABool    naIsBoxi64Empty       (NABoxi64    box);
NA_IAPI NABool    naIsBoxsEmpty         (NABoxs      box);

NA_IAPI NABool    naIsBoxi32EmptySlow   (NABoxi32    box);
NA_IAPI NABool    naIsBoxi64EmptySlow   (NABoxi64    box);
NA_IAPI NABool    naIsBoxsEmptySlow     (NABoxs      box);

// IsNegative:
//
// Returns NA_TRUE for sizes with negative entries.
//
// Note from the author: These functions test for Negativity, not Positivity.
// Using a negative word in a function name is usually not good practice as it
// tends to lead to double negatives when reading the source code. There are
// situations though when using a negative word is acceptable or even better
// than its positive counterpart. Like here for example. Or the NDEBUG macro.
// Or the "End of file" marker EOF for example. But such situations are rare.
// Try to use positive words whenever possible! For example, test for "Inside"
// rather than "Outside".

NA_IAPI NABool    naIsRangeNegative    (NARange     range);
NA_IAPI NABool    naIsRangefNegative   (NARangef    range);
NA_IAPI NABool    naIsRangei32Negative (NARangei32  range);
NA_IAPI NABool    naIsRangei64Negative (NARangei64  range);
NA_IAPI NABool    naIsRangesNegative   (NARanges    range);

NA_IAPI NABool    naIsSizeNegative     (NASize      size);
NA_IAPI NABool    naIsSizefNegative    (NASizef     size);
NA_IAPI NABool    naIsSizei32Negative  (NASizei32   size);
NA_IAPI NABool    naIsSizei64Negative  (NASizei64   size);
NA_IAPI NABool    naIsSizesNegative    (NASizes     size);

NA_IAPI NABool    naIsRectNegative     (NARect      rect);
NA_IAPI NABool    naIsRectfNegative    (NARectf     rect);
NA_IAPI NABool    naIsRecti32Negative  (NARecti32   rect);
NA_IAPI NABool    naIsRecti64Negative  (NARecti64   rect);
NA_IAPI NABool    naIsRectsNegative    (NARects     rect);

NA_IAPI NABool    naIsVolumeNegative   (NAVolume    volume);
NA_IAPI NABool    naIsVolumefNegative  (NAVolumef   volume);
NA_IAPI NABool    naIsVolumei32Negative(NAVolumei32 volume);
NA_IAPI NABool    naIsVolumei64Negative(NAVolumei64 volume);
NA_IAPI NABool    naIsVolumesNegative  (NAVolumes   volume);

NA_IAPI NABool    naIsBoxNegative      (NABox       box);
NA_IAPI NABool    naIsBoxfNegative     (NABoxf      box);
NA_IAPI NABool    naIsBoxi32Negative   (NABoxi32    box);
NA_IAPI NABool    naIsBoxi64Negative   (NABoxi64    box);
NA_IAPI NABool    naIsBoxsNegative     (NABoxs      box);

// IsUseful:
//
// Returns NA_TRUE when all values of the element are useful. Useful elements
// are defined to be not invalid, not empty and not negative. Hence for example
// a rect is useful if it has no invalid values, is positive and has a size
// greater than zero. This function often is useful after an operation which
// might lead to negative sizes, empty rects or otherwise invalid values.

NA_IAPI NABool    naIsRangeUseful    (NARange     range);
NA_IAPI NABool    naIsRangefUseful   (NARangef    range);
NA_IAPI NABool    naIsRangei32Useful (NARangei32  range);
NA_IAPI NABool    naIsRangei64Useful (NARangei64  range);
NA_IAPI NABool    naIsRangesUseful   (NARanges    range);

NA_IAPI NABool    naIsPosUseful      (NAPos       pos);
NA_IAPI NABool    naIsPosfUseful     (NAPosf      pos);
NA_IAPI NABool    naIsPosi32Useful   (NAPosi32    pos);
NA_IAPI NABool    naIsPosi64Useful   (NAPosi64    pos);
NA_IAPI NABool    naIsPossUseful     (NAPoss      pos);

NA_IAPI NABool    naIsSizeUseful     (NASize      size);
NA_IAPI NABool    naIsSizefUseful    (NASizef     size);
NA_IAPI NABool    naIsSizei32Useful  (NASizei32   size);
NA_IAPI NABool    naIsSizei64Useful  (NASizei64   size);
NA_IAPI NABool    naIsSizesUseful    (NASizes     size);

NA_IAPI NABool    naIsRectUseful     (NARect      rect);
NA_IAPI NABool    naIsRectfUseful    (NARectf     rect);
NA_IAPI NABool    naIsRecti32Useful  (NARecti32   rect);
NA_IAPI NABool    naIsRecti64Useful  (NARecti64   rect);
NA_IAPI NABool    naIsRectsUseful    (NARects     rect);

NA_IAPI NABool    naIsVertexUseful   (NAVertex    vertex);
NA_IAPI NABool    naIsVertexfUseful  (NAVertexf   vertex);
NA_IAPI NABool    naIsVertexi32Useful(NAVertexi32 vertex);
NA_IAPI NABool    naIsVertexi64Useful(NAVertexi64 vertex);
NA_IAPI NABool    naIsVertexsUseful  (NAVertexs   vertex);

NA_IAPI NABool    naIsVolumeUseful   (NAVolume    volume);
NA_IAPI NABool    naIsVolumefUseful  (NAVolumef   volume);
NA_IAPI NABool    naIsVolumei32Useful(NAVolumei32 volume);
NA_IAPI NABool    naIsVolumei64Useful(NAVolumei64 volume);
NA_IAPI NABool    naIsVolumesUseful  (NAVolumes   volume);

NA_IAPI NABool    naIsBoxUseful      (NABox       box);
NA_IAPI NABool    naIsBoxfUseful     (NABoxf      box);
NA_IAPI NABool    naIsBoxi32Useful   (NABoxi32    box);
NA_IAPI NABool    naIsBoxi64Useful   (NABoxi64    box);
NA_IAPI NABool    naIsBoxsUseful     (NABoxs      box);



// /////////////////
// macOS specific stuff

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



// Inline implementations are in a separate file:
#include "Core/NACoord/NACoordII.h"



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
