
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_BYTE_MAP_2D_INCLUDED
#define NA_BYTE_MAP_2D_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


// ////////////////
// Warning: Legacy code. todo
// ////////////////


// An NAByteMap2D stores a rectangular, two-dimensional field of bytes with a
// specific origin and size. Its components can be accessed using the integer
// variants of NAPos, NASize and NARect.
//
// Maps are designed to be enhanced and shrunk. New entries are automatically
// initialized to binary zero. Maps are not designed for creating sub-maps!


#include "NACoord.h"


// The full type definitions are in the file "NAByteMap2DII.h"
typedef struct NAMapi NAMapi;
typedef struct NAMap3i NAMap3i;
typedef struct NAByteMap2D NAByteMap2D;


// Creates or fills a new, EMPTY NAByteMap2D and returns the pointer to the
// NAByteMap2D struct. Other than the struct itself, no additional memory will
// be allocated. An empty map is defined such that it does not need to be
// cleared. No harm is done if you do it anyway.
// The width and height field of the rect are guaranteed to be zero.
NA_IAPI NAByteMap2D* naInitByteMap2D(NAByteMap2D* map2d);

// Creates or fills a new NAByteMap2D with the desired rect and returns the
// pointer. Allocates memory on the heap. If the rect size is empty, an empty
// array is returned. Negative rects are invalid.
NA_IAPI NAByteMap2D* naInitByteMap2DWithRecti( NAByteMap2D* map2d,
                                                      NARecti rect);
// Creates or fills a new ByteMap2D with the desired rect and COPIES the values
// provided in buffer to a newly allocated area owned by this NAByteMap2D. The
// buffer must be big enough to provide all data.
NA_IAPI NAByteMap2D* naInitByteMap2DWithRectiCopyingBuffer(
                                                      NAByteMap2D* map2d,
                                                           NARecti rect,
                                                    const NAByte* buffer);

// Clears the given map.
NA_IAPI void naClearByteMap2D  (NAByteMap2D* map2d);

// Clears the given map and reinitializes it as an empty map
NA_IAPI void naEmptyByteMap2D  (NAByteMap2D* map2d);

// Enhances the given map such that it contains the given rect afterwards.
// All newly created content (whether it is inside the given rect or somewhere
// else) is initialized to binary zero. The old content is copied to the new
// map at the correct position. If the rect was already contained in the map,
// nothing is changed.
// Warning: Copies as soon as something needs to be enhanced!
NA_IAPI void naEnhanceByteMap2DWithRecti(NAByteMap2D* map2d, NARecti rect);
// Enhances the map such that the given position is inside the map.
// Note: This is exactly the same as calling naEnhanceByteMap2DWithRecti with
// a size of (1, 1). Returns a mutable pointer to the NAByte at pos.
NA_IAPI NAByte* naEnhanceByteMap2DAtPosi(NAByteMap2D* map2d, NAPosi pos);

// Clamps the map to the given rect. All data outside the rect will be
// discarded. The given rect must be completely inside the existing map.
NA_IAPI void naClampByteMap2DToRecti(NAByteMap2D* map2d, NARecti rect);


// Fills the map with the given value in the given rect. The rect must be fully
// contained in the map!
NA_IAPI void naFillByteMap2DWithValueInRecti(         NAByteMap2D* map2d,
                                                           NARecti rect,
                                                            NAByte value);
// Does the same thing but uses a mask. For each position, the mask defines,
// if the value shall be set or not. A mask-value of 0 will leave the map2d
// untouched, any other value will cause the value to be written to that
// position in the map2d.
// rect must be fully contained in both the map2d and the mask!
NA_IAPI void naFillByteMap2DWithValueInMaskedRecti(   NAByteMap2D* map2d,
                                                           NARecti rect,
                                                const NAByteMap2D* mask,
                                                            NAByte value);
// Does the same thing but uses the values stored in the given values-map.
// rect must be fully contained in all given maps!
// all maps must have the same orientation to produce desired results!
NA_IAPI void naFillByteMap2DWithByteMapInMaskedRecti( NAByteMap2D* map2d,
                                                           NARecti rect,
                                                const NAByteMap2D* mask,
                                                const NAByteMap2D* values);
// Does the same thing but without the mask.
NA_IAPI void naFillByteMap2DWithByteMapInRecti(NAByteMap2D* map2d,
                                                    NARecti rect,
                                         const NAByteMap2D* values);

// Replaces all originalvalue with newvalue in rect.
NA_IAPI void naReplaceByteMap2DValueWithValue( NAByteMap2D* map2d,
                                                    NARecti rect,
                                                     NAByte originalvalue,
                                                     NAByte newvalue);

// Returns a pointer to the very first byte of the map. Warning: result is
// garbage if the map is empty. Notice: This function is speedy.
//
// You can use this function to iterate through the whole map. There are
// width * height number of elements in the map. The elements are ordered
// primary-dimension-first.
NA_IAPI const NAByte* naGetByteMap2DConstPointer  (const NAByteMap2D* map2d);
NA_IAPI       NAByte* naGetByteMap2DMutablePointer(      NAByteMap2D* map2d);

// Returns a POINTER to the byte at the given position. If the given position
// is not within the rect of the map, the result is dangerous. There is no
// bounding-check, you have to do that manually in advance, if needed. Use the
// naIsPosiInByteMap2D function for that.
// Warning: result of this function is garbage if the map is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetByteMap2DPointer or
// this function and use pointer arithmetic afterwards.
NA_IAPI const NAByte* naGetByteMap2DConstByte  (const NAByteMap2D* map2d,
                                                            NAPosi pos);
NA_IAPI       NAByte* naGetByteMap2DMutableByte(      NAByteMap2D* map2d,
                                                            NAPosi pos);

// Returns NA_TRUE, if the given position is contained in the given map. Note:
// Returns NA_FALSE for empty maps.
NA_IAPI NABool naIsPosiInByteMap2D(NAPosi pos, const NAByteMap2D* map2d);
// Returns the NARecti of the map. Note: Does NOT emits a warning if the map is
// empty!
NA_IAPI NARecti naGetByteMap2DRecti(const NAByteMap2D* map2d);
// Returns NA_TRUE if the map is empty.
NA_IAPI NABool  naIsByteMap2DEmpty( const NAByteMap2D* map2d);


// Moves the given map to the given absolute position or by the given relative
// size.
NA_IAPI void naMoveByteMap2DToPosi(NAByteMap2D* map2d, NAPosi pos);
NA_IAPI void naMoveByteMap2DBySizei(NAByteMap2D* map2d, NASizei size);







// Inline implementations are in a separate file:
#include "NAStruct/NAByteMap2DII.h"



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_BYTE_MAP_2D_INCLUDED


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


