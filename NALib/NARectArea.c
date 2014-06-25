
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


#include <stdarg.h>
#include "NARectArea.h"
#include "NAHeap.h"
#include "NAMinMax.h"
#include "NAPointer.h"
#include "NAArray.h"
#include "NACoord.h"


NA_API NAArray* naCreateRectiAreaArray(NAArray* areaarray, NAInt numhabitants, ...){
  NAGrowingSpace minmax0space;
  NAGrowingSpace minmax1space;
  naCreateGrowingSpace(&minmax0space, sizeof(NAMinMax1i), numhabitants);
  naCreateGrowingSpace(&minmax1space, sizeof(NAMinMax1i), numhabitants);
  
  NAInt masks[NA_SYSTEM_ADDRESS_BITS];
  naNulln(masks, NA_SYSTEM_ADDRESS_BITS * sizeof(NAInt));
  
  NAInt usefulhabitants = 0;
  va_list argumentlist;
  va_start(argumentlist, numhabitants);
  for(NAInt h=0; h<numhabitants; h++){
    NARecti rect = va_arg(argumentlist, NARecti);
    if(naIsRectiUseful(rect)){
      NAMinMax1i newminmax0 = naMakeMinMax1iWithPosAndSize(rect.pos.x, rect.size.width);
      NAMinMax1i newminmax1 = naMakeMinMax1iWithPosAndSize(rect.pos.y, rect.size.height);
      naAddGrowingSpaceElement(&minmax0space, &newminmax0);
      naAddGrowingSpaceElement(&minmax1space, &newminmax1);
      masks[usefulhabitants] = 1<<h;
      usefulhabitants++;
    }
  }
  va_end(argumentlist);

  NAArray minmax0array;
  NAArray minmax1array;
  naCreateArrayOutOfGrowingSpace(&minmax0array, &minmax0space, NA_FALSE);
  naCreateArrayOutOfGrowingSpace(&minmax1array, &minmax1space, NA_FALSE);

  NAArray segments0;
  NAArray segments1;
  naCreateAreasWithMinMax1iFromMinMax1iArray(&segments0, &minmax0array);
  naCreateAreasWithMinMax1iFromMinMax1iArray(&segments1, &minmax1array);
 
  NAInt segmentcount0 = naGetArrayCount(&segments0);
  NAInt segmentcount1 = naGetArrayCount(&segments1);
  NAGrowingSpace space;
  naCreateGrowingSpace(&space, sizeof(NARectiArea), segmentcount0 * segmentcount1);

  const NAMinMax1i* cursegment1 = naGetArrayConstPointer(&segments1);
  for(NAInt s1=0; s1<segmentcount1; s1++){
    const NAMinMax1i* cursegment0 = naGetArrayConstPointer(&segments0);
    for(NAInt s0=0; s0<segmentcount0; s0++){
      NARectiArea newarea;
      newarea.rect = naMakeRectiFromMinMax1i(*cursegment0, *cursegment1);
      newarea.habitants = 0;
      NAMinMax1i* curminmax0 = (NAMinMax1i*)naGetArrayConstPointer(&minmax0array);
      NAMinMax1i* curminmax1 = (NAMinMax1i*)naGetArrayConstPointer(&minmax1array);
      NAInt* curmask = masks;
      for(NAInt h=0; h<usefulhabitants; h++){
        if((*curmask) && (naIsInsideMinMax1i(newarea.rect.pos.x, (*curminmax0))) && naIsInsideMinMax1i(newarea.rect.pos.y, (*curminmax1))){
          newarea.habitants |= *curmask;
        }
        curminmax0++;
        curminmax1++;
        curmask++;
      }
      naAddGrowingSpaceElement(&space, &newarea);
      cursegment0++;
    }
    cursegment1++;
  }

  areaarray = naCreateArrayOutOfGrowingSpace(areaarray, &space, NA_FALSE);

  naClearArray(&segments0, NA_NULL);
  naClearArray(&segments1, NA_NULL);
  naClearArray(&minmax0array, NA_NULL);
  naClearArray(&minmax1array, NA_NULL);
  naClearGrowingSpace(&minmax0space, NA_NULL);
  naClearGrowingSpace(&minmax1space, NA_NULL);
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
