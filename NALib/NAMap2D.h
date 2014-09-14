
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












NA_IAPI NAMap2D* naCreateMap2D(NAMap2D* map2d, NAInt typesize){
  map2d = (NAMap2D*)naAllocateIfNull(map2d, sizeof(NAMap2D));
  map2d->typesize = typesize;
  naCreateByteMap2D(&(map2d->bytemap));
  return map2d;
}


NA_IAPI NAMap2D* naCreateMap2DWithRecti(NAMap2D* map2d,
                                          NAInt typesize,
                                         NARecti rect){
  map2d = (NAMap2D*)naAllocateIfNull(map2d, sizeof(NAMap2D));
  NARecti xrect = rect;
  xrect.size.width *= typesize;
  map2d->typesize = typesize;
  naCreateByteMap2DWithRecti(&(map2d->bytemap), xrect);
  return map2d;
}



NA_IAPI void naClearMap2D(NAMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d)
      {naCrash("naClearMap2D", "map2d is Null-Pointer."); return;}
  #endif
  naClearByteMap2D(&(map2d->bytemap));
}


NA_IAPI void naDestroyMap2D(NAMap2D* map2d){
  naClearMap2D(map2d);
  free(map2d);
}


NA_IAPI void naEmptyMap2D(NAMap2D* map2d){
  naClearMap2D(map2d);
  naCreateMap2D(map2d, map2d->typesize);
}



NA_IAPI void naFillMap2DWithNullInRecti( NAMap2D* map2d,
                                           NARecti rect){
  NARecti xrect = rect;
  xrect.size.width *= map2d->typesize;
  xrect.pos.x = naGetByteMap2DRecti(&(map2d->bytemap)).pos.x;
  xrect.pos.x += (rect.pos.x - xrect.pos.x) * map2d->typesize;  
  naFillByteMap2DWithValueInRecti(&(map2d->bytemap), xrect, 0);
}




NA_IAPI void* naGetMap2DMutableValue(NAMap2D* map2d, NAPosi pos){
  NAInt indx;
  NAPosi xpos = pos;
  xpos.x = naGetByteMap2DRecti(&(map2d->bytemap)).pos.x;
  xpos.x += (pos.x - xpos.x) * map2d->typesize;
  return naGetByteMap2DMutableByte(&(map2d->bytemap), xpos);
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


