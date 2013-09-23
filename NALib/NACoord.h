//
//// This file is part of NALib, a collection of C and C++ source code
//// intended for didactical purposes. Full license notice at the bottom.
//
//#ifndef NA_COORD_INCLUDED
//#define NA_COORD_INCLUDED
//#ifdef __cplusplus 
//  extern "C"{
//#endif
//
////#include "NASystem.h"
//#include "NARange.h"
//
//typedef struct NAIntPos NAIntPos;
//struct NAIntPos{
//  int32 x;
//  int32 y;
//};
//
//typedef struct NAIntSize NAIntSize;
//struct NAIntSize{
//  int32 width;
//  int32 height;
//};
//
//typedef struct NAIntRect NAIntRect;
//struct NAIntRect{
//  NAIntPos pos;
//  NAIntSize size;
//};
//
//
//
//
//NAIntPos makeIntPos(int32 x, int32 y){
//  NAIntPos newpos = {
//    x,
//    y
//  };
//  return newpos;
//}
//
//NABool equalIntPos(NAIntPos a, NAIntPos b){
//  return (a.x == b.x) && (a.y == b.y);
//}
//
//NAIntPos addIntPosIntSize(NAIntPos a, NAIntSize s){
//  NAIntPos newpos = {
//    a.x + s.width,
//    a.y + s.height
//  };
//  return newpos;
//}
//
//NAIntPos clampIntPosToIntRect(NAIntPos a, NAIntRect r){
//  NAIntPos newpos = a;
////  naClampToRangei32(a.x, , <#double b#>)
////  if(a.x <  clamprect.pos.x) {x = clamprect.pos.x;}
////  if(x >= clamprect.endX()){x = clamprect.maxX();}
////  if(y <  clamprect.pos.y) {y = clamprect.pos.y;}
////  if(y >= clamprect.endY()){y = clamprect.maxY();}
//  return newpos;
//}
//
//#ifdef __cplusplus 
//  } // extern "C"
//#endif
//#endif // NA_COORD_INCLUDED
//
//// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the source-code.
////
//// In case the source-code of this software is inaccessible to the end-user,
//// the above copyright notice and this permission notice shall be included
//// in any source-code which is dependent on this software and is accessible
//// to the end-user.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
