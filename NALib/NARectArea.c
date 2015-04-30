
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include <stdarg.h>
#include "NARectArea.h"
#include "NAHeap.h"
#include "NAMinMax.h"
#include "NAPointer.h"
#include "NAArray.h"
#include "NACoord.h"
#include "NAGrowingSpace.h"


NA_API NAArray* naCreateRectiAreaArray(NAArray* areaarray, NAInt numhabitants, ...){
  NAGrowingSpace minmax0space;
  NAGrowingSpace minmax1space;  
  NAInt masks[NA_SYSTEM_ADDRESS_BITS];
  NAInt usefulhabitants;
  va_list argumentlist;
  NAInt h;
  NAArray minmax0array;
  NAArray minmax1array;
  NAArray segments0;
  NAArray segments1;
  NAUInt segmentcount0;
  NAUInt segmentcount1;
  NAGrowingSpace space;
  const NAMinMax1i* cursegment1;
  const NAMinMax1i* cursegment0;
  NAUInt s0;
  NAUInt s1;
  NAMinMax1i* curminmax0;
  NAMinMax1i* curminmax1;
  NAInt* curmask;

  naCreateGrowingSpace(&minmax0space, sizeof(NAMinMax1i), NA_NULL);
  naCreateGrowingSpace(&minmax1space, sizeof(NAMinMax1i), NA_NULL);
  naNulln(masks, NA_SYSTEM_ADDRESS_BITS * sizeof(NAInt));
  
  usefulhabitants = 0;
  va_start(argumentlist, numhabitants);
  for(h=0; h<numhabitants; h++){
    NARecti rect = va_arg(argumentlist, NARecti);
    if(naIsRectiUseful(rect)){
      NAMinMax1i* newminmax0; // Declaration before Implementation.
      NAMinMax1i* newminmax1;
      newminmax0 = naNewGrowingSpaceElement(&minmax0space);
      naFillMinMax1iWithPosAndSize(newminmax0, rect.pos.x, rect.size.width);
      newminmax1 = naNewGrowingSpaceElement(&minmax1space);
      naFillMinMax1iWithPosAndSize(newminmax1, rect.pos.y, rect.size.height);
      masks[usefulhabitants] = 1<<h;
      usefulhabitants++;
    }
  }
  va_end(argumentlist);

  naCreateArrayOutOfGrowingSpace(&minmax0array, &minmax0space);
  naCreateArrayOutOfGrowingSpace(&minmax1array, &minmax1space);

  naCreateAreasWithMinMax1iFromMinMax1iArray(&segments0, &minmax0array);
  naCreateAreasWithMinMax1iFromMinMax1iArray(&segments1, &minmax1array);
 
  segmentcount0 = naGetArrayCount(&segments0);
  segmentcount1 = naGetArrayCount(&segments1);
  naCreateGrowingSpace(&space, sizeof(NARectiArea), NA_NULL);

  cursegment1 = naGetArrayConstPointer(&segments1);
  for(s1=0; s1<segmentcount1; s1++){
    cursegment0 = naGetArrayConstPointer(&segments0);
    for(s0=0; s0<segmentcount0; s0++){
      NARectiArea* newarea = naNewGrowingSpaceElement(&space);
      newarea->rect = naMakeRectiWithMinMax1i(*cursegment0, *cursegment1);
      newarea->habitants = 0;
      curminmax0 = (NAMinMax1i*)naGetArrayConstPointer(&minmax0array);
      curminmax1 = (NAMinMax1i*)naGetArrayConstPointer(&minmax1array);
      curmask = masks;
      for(h=0; h<usefulhabitants; h++){
        if((*curmask) && (naInsideMinMax1i((*curminmax0), newarea->rect.pos.x)) && naInsideMinMax1i((*curminmax1), newarea->rect.pos.y)){
          newarea->habitants |= *curmask;
        }
        curminmax0++;
        curminmax1++;
        curmask++;
      }
      cursegment0++;
    }
    cursegment1++;
  }

  areaarray = naCreateArrayOutOfGrowingSpace(areaarray, &space);

  naClearArray(&segments0, NA_NULL);
  naClearArray(&segments1, NA_NULL);
  naClearArray(&minmax0array, NA_NULL);
  naClearArray(&minmax1array, NA_NULL);
  naClearGrowingSpace(&minmax0space, NA_NULL);
  naClearGrowingSpace(&minmax1space, NA_NULL);
  naClearGrowingSpace(&space, NA_NULL);
  return areaarray;
}


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
