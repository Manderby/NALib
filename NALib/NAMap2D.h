
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_MAP_2D_INCLUDED
#define NA_MAP_2D_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NAByteMap2D.h"


typedef struct NAMap2D NAMap2D;
struct NAMap2D{
  NAByteMap2D bytemap;       // the underlying bytemap;
  NAInt typesize;             // The size of the type in bytes.
};



// Creates or fills a new, EMPTY NAMap2D and returns the pointer to the
// NAMap2D struct. Other than the struct itself, no additional memory will
// be allocated. An empty map is defined such that it does not need to be
// cleared. No harm is done if you do it anyway.
// The width and height field of the underlying NAByteMap2D's rect field are
// guaranteed to be zero.
NA_IAPI NAMap2D* naCreateMap2D(NAMap2D* map2d, NAInt typesize);

// Creates or fills a new NAMap2D with the desired rect and returns the
// pointer. Allocates memory on the heap. If the rect size is empty, an empty
// array is returned. Negative rects are invalid.
NA_IAPI NAMap2D* naCreateMap2DWithRecti(NAMap2D* map2d,
                                          NAInt typesize,
                                         NARecti rect);

// Clears or Deletes the given map.
NA_IAPI void naClearMap2D(NAMap2D* map2d);
NA_IAPI void naDestroyMap2D(NAMap2D* map2d);

// Clears the given map and reinitializes it as an empty map
NA_IAPI void naEmptyMap2D(NAMap2D* map2d);

// Fills the memory in the given rect with binary zero.
NA_IAPI void naFillMap2DWithNullInRecti( NAMap2D* map2d,
                                           NARecti rect);

// Returns a pointer to the very first byte of the map. Warning: result is
// garbage if the map is empty. Notice: This function is speedy.
//
// You can use this function to iterate through the whole map. There are
// width * height number of elements in the map. When properly casted, you can
// use pointer arithmetics to traverse the whole map. The elements are ordered
// primary-dimension-first.
NA_IAPI const void* naGetMap2DConstPointer  (const NAMap2D* map2d);
NA_IAPI       void* naGetMap2DMutablePointer(      NAMap2D* map2d);

// Returns a POINTER to the object at the given position. If the given position
// is not within the rect of the map, the result is dangerous. There is no
// bounding-check, you have to do that manually in advance, if needed. Use the
// naIsPosiInMap function for that.
// Warning: result of this function is garbage if the map is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetMap2DPointer or
// this function and use pointer arithmetic afterwards.
NA_IAPI const void* naGetMap2DConstValue(const NAMap2D* map2d, NAPosi pos);
NA_IAPI void* naGetMap2DMutableValue(NAMap2D* map2d, NAPosi pos);

// Returns NA_TRUE, if the given position is contained in the given map. Note:
// Returns NA_FALSE for empty maps.
NA_IAPI NABool naIsPosiInMap2D(NAPosi pos, const NAMap2D* map2d);









NA_IDEF NAMap2D* naCreateMap2D(NAMap2D* map2d, NAInt typesize){
  map2d = naAllocNALibStruct(map2d, NAMap2D);
  map2d->typesize = typesize;
  naCreateByteMap2D(&(map2d->bytemap));
  return map2d;
}


NA_IDEF NAMap2D* naCreateMap2DWithRecti(NAMap2D* map2d,
                                          NAInt typesize,
                                         NARecti rect){
  map2d = naAllocNALibStruct(map2d, NAMap2D);
  NARecti xrect = rect;
  xrect.size.width *= typesize;
  map2d->typesize = typesize;
  naCreateByteMap2DWithRecti(&(map2d->bytemap), xrect);
  return map2d;
}



NA_IDEF void naClearMap2D(NAMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naClearMap2D", "map2d is Null-Pointer.");
      return;
    }
  #endif
  naClearByteMap2D(&(map2d->bytemap));
}


NA_IDEF void naDestroyMap2D(NAMap2D* map2d){
  naClearMap2D(map2d);
  free(map2d);
}


NA_IDEF void naEmptyMap2D(NAMap2D* map2d){
  naClearMap2D(map2d);
  naCreateMap2D(map2d, map2d->typesize);
}



NA_IDEF void naFillMap2DWithNullInRecti( NAMap2D* map2d,
                                           NARecti rect){
  NARecti xrect = rect;
  xrect.size.width *= map2d->typesize;
  xrect.pos.x = naGetByteMap2DRecti(&(map2d->bytemap)).pos.x;
  xrect.pos.x += (rect.pos.x - xrect.pos.x) * map2d->typesize;  
  naFillByteMap2DWithValueInRecti(&(map2d->bytemap), xrect, 0);
}


NA_IDEF const void* naGetMap2DConstPointer(const NAMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetMap2DConstPointer", "map2d is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteMap2DConstPointer(&(map2d->bytemap));
}


NA_IDEF void* naGetMap2DMutablePointer(NAMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetMap2DMutablePointer", "map2d is Null-Pointer.");
      return NA_NULL;
    }
  #endif
  return naGetByteMap2DMutablePointer(&(map2d->bytemap));
}


NA_IDEF const void* naGetMap2DConstValue(const NAMap2D* map2d, NAPosi pos){
  NAInt indx;
  NAPosi xpos = pos;
  xpos.x = naGetByteMap2DRecti(&(map2d->bytemap)).pos.x;
  xpos.x += (pos.x - xpos.x) * map2d->typesize;
  return naGetByteMap2DConstByte(&(map2d->bytemap), xpos);
}


NA_IDEF void* naGetMap2DMutableValue(NAMap2D* map2d, NAPosi pos){
  NAInt indx;
  NAPosi xpos = pos;
  xpos.x = naGetByteMap2DRecti(&(map2d->bytemap)).pos.x;
  xpos.x += (pos.x - xpos.x) * map2d->typesize;
  return naGetByteMap2DMutableByte(&(map2d->bytemap), xpos);
}


NA_IDEF NABool naIsPosiInMap2D(NAPosi pos, const NAMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naIsPosiInMap", "map2d is Null-Pointer.");
      return NA_FALSE;
    }
  #endif
  return naIsPosiInByteMap2D(pos, &(map2d->bytemap));
}



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


