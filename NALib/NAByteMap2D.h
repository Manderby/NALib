
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_BYTE_MAP_2D_INCLUDED
#define NA_BYTE_MAP_2D_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NAPointer.h"
#include "NABinaryData.h"

// An NAByteMap2D stores a rectangular, two-dimensional field of bytes with a
// specific origin and size. Its components can be accessed using the integer
// variants of NAPos, NASize and NARect.
//
// Maps are designed to be enhanced and shrunk. New entries are automatically
// initialized to binary zero. Maps are not designed for creating sub-maps!

// Opaque type. See explanation in readme.txt
typedef struct NAByteMap2D NAByteMap2D;
struct NAByteMap2D{
  NAByte* data;       // the pointer to the data;
  NARecti rect;       // The origin and size of the map
};
// The two size fields (width and height) denote the number of entries in the
// primary and secondary dimension. Note that there is no definition of how
// these dimensions shall be semantically interpreted in this struct. You
// therefore don't know if the primary dimension is horizontal or vertical or
// if the vertical axis goes from top to bottom or from bottom to top. You can
// use NACoordSysi (todo) for that. But note that a 2D array is always stored
// primary dimension first, just like a regular C-style array.
//
// An empty size (naIsSizeiEmpty) is perfectly valid but explicitely denotes
// an empty map.
//
// Implementation notice: When rect is empty (naIsSizeiEmpty), the map is
// considered empty. In that case, the other fields contain no useful
// information.
//
// Negative or otherwise invalid rects or sizes behave undefined and may crash
// your application!


// Creates or fills a new, EMPTY ByteMap2D and returns the pointer to the
// NAByteMap2D struct. Other than the struct itself, no additional memory will
// be allocated. The width and height field of the rect are guaranteed to be
// zero.
NA_IAPI NAByteMap2D* naCreateByteMap2D(NAByteMap2D* map2d);

// Creates or fills a new ByteMap2D with the desired rect and returns the
// pointer. Allocates memory on the heap. If the rect size is empty, an empty
// array is returned. Negative rects are invalid.
NA_IAPI NAByteMap2D* naCreateByteMap2DWithRecti( NAByteMap2D* map2d,
                                                      NARecti rect);

// Clears or Deletes the given map.
NA_IAPI void naClearByteMap2D  (NAByteMap2D* map2d);
NA_IAPI void naDestroyByteMap2D(NAByteMap2D* map2d);

// Enhances the given map such that it contains the given rect afterwards.
// All newly created content (whether it is inside the given rect or somewhere
// else) is initialized to binary zero. The old content is copied to the new
// map at the correct position. If the rect was already contained in the map,
// nothing is changed.
// Warning: Copies as soon as something needs to be enhanced!
NA_IAPI void naEnhanceByteMap2DWithRecti(NAByteMap2D* map2d, NARecti rect);
// Enhances the map such that the given position is inside the map.
// Note: This is exactly the same as calling naEnhanceByteMap2DWithRecti with
// a size of (1, 1).
NA_IAPI void naEnhanceByteMap2DAtPosi(NAByteMap2D* map2d, NAPosi pos);

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
// garbage if the array is empty. Notice: This function is speedy.
//
// You can use this function to iterate through the whole map. There are
// width * height number of elements in the map. The elements are ordered
// primary-dimension-first.
NA_IAPI const NAByte* naGetByteMap2DConstPointer  (const NAByteMap2D* map2d);
NA_IAPI       NAByte* naGetByteMap2DMutablePointer(      NAByteMap2D* map2d);

// Returns a POINTER to the byte at the given position. If the given position
// is not within the rect of the map, the result is dangerous. There is no
// bounding-check, you have to do that manually in advance, if needed. Use the
// naIsPosiInByteMap function for that.
// Warning: result of this function is garbage if the array is empty.
// Note that calling this function too often might not be a good idea with
// respect to speed. Try getting a pointer using naGetByteMap2DPointer or
// this function and use pointer arithmetic afterwards.
NA_IAPI const NAByte* naGetByteMap2DConstByte  (const NAByteMap2D* map2d,
                                                            NAPosi pos);
NA_IAPI       NAByte* naGetByteMap2DMutableByte(      NAByteMap2D* map2d,
                                                            NAPosi pos);

// Returns NA_TRUE, if the given position is contained in the given map. Note:
// Returns NA_FALSE for empty maps.
NA_IAPI NABool naIsPosiInByteMap(NAPosi pos, const NAByteMap2D* map2d);
// Returns the NARecti of the map. Note: Does NOT emits a warning if the map is
// empty!
NA_IAPI NARecti naGetByteMap2DRect(const NAByteMap2D* map2d);
















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////



NA_IAPI NAByteMap2D* naCreateByteMap2D(NAByteMap2D* map2d){
  map2d = (NAByteMap2D*)naAllocateIfNull(map2d, sizeof(NAByteMap2D));
  map2d->rect.size.width  = 0;
  map2d->rect.size.height = 0;
  return map2d;
}


NA_IAPI NAByteMap2D* naCreateByteMap2DWithRecti(NAByteMap2D* map2d,
                                                     NARecti rect){
  #ifndef NDEBUG
    if(!naIsRectiValid(rect))
      naError("naCreateByteMap2DWithRecti", "rect is invalid.");
  #endif
  NAInt totalsize = naGetRectiIndexCount(rect);
  if(!totalsize){  // if total size is zero
    map2d = naCreateByteMap2D(map2d);
  }else{
    map2d = (NAByteMap2D*)naAllocateIfNull(map2d, sizeof(NAByteMap2D));
    #ifndef NDEBUG
      if(naIsRectiValid(rect) && (totalsize < 0))
        naError("naCreateByteMap2DWithRecti", "Total size exceeds int range");
    #endif
    map2d->data = (NAByte*)naAllocate(totalsize);
    naNulln(map2d->data, totalsize);
    map2d->rect = rect;
  }
  return map2d;
}



NA_IAPI void naClearByteMap2D(NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d)
      {naCrash("naClearByteMap2D", "map2d is Null-Pointer."); return;}
  #endif
  if(!naIsRectiEmpty(map2d->rect)){free(map2d->data);}
}


NA_IAPI void naDestroyByteMap2D(NAByteMap2D* map2d){
  naClearByteMap2D(map2d);
  free(map2d);
}


NA_IAPI void naEnhanceByteMap2DWithRecti(NAByteMap2D* map2d, NARecti rect){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naEnhanceByteMap2DWithRecti", "map2d is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(map2d->rect))
      naError("naEnhanceByteMap2DWithRecti",  "rect of map2d is invalid. "
                                              "Uninitialized struct?");
    if(!naIsRectiValid(rect))
      naError("naEnhanceByteMap2DWithRecti", "rect is invalid.");
  #endif
  if(naIsRectiEmpty(map2d->rect)){
    // the storage is empty. Create a new one.
    naClearByteMap2D(map2d);
    naCreateByteMap2DWithRecti(map2d, rect);
  }else{
    NARecti enhancedrect = naMakeRectiWithRectiAndRecti(map2d->rect, rect);  
    NAInt totalsize = naGetRectiIndexCount(enhancedrect);
    #ifndef NDEBUG
      if(!naIsRectiValid(enhancedrect))
        naError("naEnhanceByteMap2DWithRecti", "enhanced rect is invalid");
      if(totalsize < 0)
        naError("naEnhanceByteMap2DWithRecti", "Total size exceeds int range");
    #endif


    if(!naEqualRecti(enhancedrect, map2d->rect)){
      // If the enhanced rect is different than the current rect, enhance!
      NAByte* olddataptr = map2d->data;
      NAByte* newdata = naAllocate(totalsize);
      NAByte* newdataptr = newdata;
      
      // Compute the bounds.
      NAInt leadbound1 =     map2d->rect.pos.x
                           - enhancedrect.pos.x;
      NAInt trailbound1 =    naGetRectiEndX(enhancedrect)
                           - naGetRectiEndX(map2d->rect);
      NAInt leadbound2 =    (map2d->rect.pos.y - enhancedrect.pos.y)
                           * enhancedrect.size.width;
      NAInt trailbound2 =   (naGetRectiEndY(enhancedrect)
                           - naGetRectiEndY(map2d->rect))
                           * enhancedrect.size.width;
      // The bounds of dimension 2 are premultiplied with the width.
      
      // Fill the contents.
      // Fill the top bound with binary zero.
      if(leadbound2){
        naNulln(newdataptr, leadbound2);
        newdataptr += leadbound2;
      }
      // Fill the middle
      if(leadbound1 || trailbound1){
        // The map has been horizontally enhanded. We need to copy line by line.
        for(NAInt v2 = 0; v2 < map2d->rect.size.height; v2++){
          if(leadbound1){
            naNulln(newdataptr, leadbound1);
            newdataptr += leadbound1;
          }
          naCpyn(newdataptr, olddataptr, map2d->rect.size.width);
          newdataptr += map2d->rect.size.width;
          olddataptr += map2d->rect.size.width;
          if(trailbound1){
            naNulln(newdataptr, trailbound1);
            newdataptr += trailbound1;
          }
        }
      }else{
        // The map has NOT been horizontally enhanced. Just copy the whole old
        // data block to the new one.
        NAInt totaloldsize = naGetRectiIndexCount(map2d->rect);
        naCpyn(newdataptr, olddataptr, totaloldsize);
        newdataptr += totaloldsize;
        olddataptr += totaloldsize;
      }
      // Fill the bottom bound with binary zero.
      if(trailbound2){
        naNulln(newdataptr, trailbound2);
        newdataptr += trailbound2;
      }
            
      // Delete the old data and attach the new one.
      free(map2d->data);
      map2d->data = newdata;
      map2d->rect = enhancedrect;
    }
  }
}


NA_IAPI void naEnhanceByteMap2DAtPosi(NAByteMap2D* map2d, NAPosi pos){
  #ifndef NDEBUG
    if(!map2d)
      {naCrash("naEnhanceByteMap2DAtPosi", "map2d is Null-Pointer."); return;}
    if(!naIsRectiValid(map2d->rect))
      naError("naEnhanceByteMap2DAtPosi", "rect of map2d is invalid. "
                                          "Uninitialized struct?");
    if(!naIsPosiValid(pos))
      naError("naEnhanceByteMap2DAtPosi", "pos is invalid.");
  #endif
  naEnhanceByteMap2DWithRecti(map2d, naMakeRecti(pos, naMakeSizei(1, 1)));
}


NA_IAPI void naFillByteMap2DWithValueInRecti( NAByteMap2D* map2d,
                                                   NARecti rect,
                                                    NAByte value){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithValueInRecti", "map2d is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(map2d->rect))
      naError("naFillByteMap2DWithValueInRecti",  "rect of map2d is invalid. "
                                                  "Uninitialized struct?");
    if(!naIsRectiValid(rect))
      naError("naFillByteMap2DWithValueInRecti", "rect is invalid.");
    if(!naIsRectiInRecti(rect, map2d->rect))
      naError("naFillByteMap2DWithValueInRecti", "rect is not fully on map.");
  #endif
  if(rect.size.width == map2d->rect.size.width){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, rect.pos);
    memset(dstptr, value, naGetRectiIndexCount(rect));
  }else{
    NAPosi leadpos = rect.pos;
    for(int v2 = 0; v2 < rect.size.height; v2++){
      NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
      memset(dstptr, value, rect.size.width);
      leadpos.y++;
    }
  }
}


NA_IAPI void naFillByteMap2DWithValueInMaskedRecti(   NAByteMap2D* map2d,
                                                           NARecti rect,
                                                const NAByteMap2D* mask,
                                                            NAByte value){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithValueInMaskedRecti",
              "map2d is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(map2d->rect))
      naError("naFillByteMap2DWithValueInMaskedRecti",
              "rect of map2d is invalid. Uninitialized struct?");
    if(!naIsRectiValid(rect))
      naError("naFillByteMap2DWithValueInMaskedRecti",
              "rect is invalid.");
    if(!naIsRectiInRecti(rect, map2d->rect))
      naError("naFillByteMap2DWithValueInMaskedRecti",
              "rect is not fully on map.");
    if(!mask){
      naCrash("naFillByteMap2DWithValueInMaskedRecti",
              "mask is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(mask->rect))
      naError("naFillByteMap2DWithValueInMaskedRecti",
              "rect of mask is invalid. Uninitialized struct?");
    if(!naIsRectiInRecti(rect, mask->rect))
      naError("naFillByteMap2DWithValueInMaskedRecti",
              "rect is not fully in mask.");
  #endif
  NAPosi leadpos = rect.pos;
  for(int v2 = 0; v2 < rect.size.height; v2++){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
    const NAByte* maskptr = naGetByteMap2DConstByte(mask, leadpos);
    for(int v1 = 0; v1 < rect.size.width; v1++){
      if(*maskptr){*dstptr = value;}
      dstptr++;
      maskptr++;
    }
    leadpos.y++;
  }
}


NA_IAPI void naFillByteMap2DWithByteMapInMaskedRecti( NAByteMap2D* map2d,
                                                           NARecti rect,
                                                const NAByteMap2D* mask,
                                                const NAByteMap2D* values){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti",
              "map2d is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(map2d->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect of map2d is invalid. Uninitialized struct?");
    if(!naIsRectiValid(rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is invalid.");
    if(!naIsRectiInRecti(rect, map2d->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is not fully on map.");
    if(!mask){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti",
              "mask is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(mask->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect of mask is invalid. Uninitialized struct?");
    if(!naIsRectiInRecti(rect, mask->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is not fully in mask.");
    if(!values){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti",
              "values is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(values->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect of values is invalid. Uninitialized struct?");
    if(!naIsRectiInRecti(rect, values->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is not fully in values.");
  #endif
  NAPosi leadpos = rect.pos;
  for(int v2 = 0; v2 < rect.size.height; v2++){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
    const NAByte* maskptr = naGetByteMap2DConstByte(mask, leadpos);
    const NAByte* valueptr = naGetByteMap2DConstByte(values, leadpos);
    for(int v1 = 0; v1 < rect.size.width; v1++){
      if(*maskptr){*dstptr = *valueptr;}
      dstptr++;
      maskptr++;
      valueptr++;
    }
    leadpos.y++;
  }
}


NA_IAPI void naFillByteMap2DWithByteMapInRecti(NAByteMap2D* map2d,
                                                    NARecti rect,
                                         const NAByteMap2D* values){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti",
              "map2d is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(map2d->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect of map2d is invalid. Uninitialized struct?");
    if(!naIsRectiValid(rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is invalid.");
    if(!naIsRectiInRecti(rect, map2d->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is not fully on map.");
    if(!values){
      naCrash("naFillByteMap2DWithByteMapInMaskedRecti",
              "values is Null-Pointer.");
      return;
    }
    if(!naIsRectiValid(values->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect of values is invalid. Uninitialized struct?");
    if(!naIsRectiInRecti(rect, values->rect))
      naError("naFillByteMap2DWithByteMapInMaskedRecti",
              "rect is not fully in values.");
  #endif
  NAPosi leadpos = rect.pos;
  for(int v2 = 0; v2 < rect.size.height; v2++){
    NAByte* dstptr = naGetByteMap2DMutableByte(map2d, leadpos);
    const NAByte* valueptr = naGetByteMap2DConstByte(values, leadpos);
    naCpyn(dstptr, valueptr, rect.size.width);
    leadpos.y++;
  }
}


NA_IAPI void naReplaceByteMap2DValueWithValue( NAByteMap2D* map2d,
                                                    NARecti rect,
                                                     NAByte originalvalue,
                                                     NAByte newvalue){
  #ifndef NDEBUG
    if(!map2d){ 
      naCrash("naReplaceByteMap2DValueWithValue", "map2d is Null-Pointer.");
      return;
    }
    if(naIsRectiEmpty(map2d->rect))
      naCrash("naReplaceByteMap2DValueWithValue", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naReplaceByteMap2DValueWithValue", "rect of map2d is invalid. "
                                                  "Uninitialized struct?");
    if(naIsRectiEmpty(rect))
      naError("naReplaceByteMap2DValueWithValue", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naReplaceByteMap2DValueWithValue", "rect is invalid.");
    if(!naIsRectiInRecti(rect, map2d->rect))
      naError("naReplaceByteMap2DValueWithValue", "rect is not fully in map.");
  #endif
  NAPosi leadpos = rect.pos;
  for(int v2=0; v2<rect.size.height; v2++){
    NAByte* dataptr = naGetByteMap2DMutableByte(map2d, leadpos);
    for(int v1=0; v1<rect.size.width; v1++){
      if(*dataptr == originalvalue){*dataptr = newvalue;}
      dataptr++;
    }
    leadpos.y++;
  }
}


NA_IAPI const NAByte* naGetByteMap2DConstPointer(const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DConstPointer", "map2d is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsRectiEmpty(map2d->rect))
      naCrash("naGetByteMap2DConstPointer", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naGetByteMap2DConstPointer",  "rect of map2d is invalid. "
                                             "Uninitialized struct?");
  #endif
  return map2d->data;
}


NA_IAPI NAByte* naGetByteMap2DMutablePointer(NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DMutablePointer", "map2d is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsRectiEmpty(map2d->rect))
      naCrash("naGetByteMap2DMutablePointer", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naGetByteMap2DMutablePointer",  "rect of map2d is invalid. "
                                               "Uninitialized struct?");
  #endif
  return map2d->data;
}



NA_IAPI const NAByte* naGetByteMap2DConstByte(const NAByteMap2D* map2d,
                                                          NAPosi pos){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DConstByte", "map2d is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsRectiEmpty(map2d->rect))
      naCrash("naGetByteMap2DConstByte", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naGetByteMap2DConstByte", "rect of map2d is invalid. "
                                         "Uninitialized struct?");
    if(!naIsPosiInRecti(pos, map2d->rect))
      naError("naGetByteMap2DConstByte", "pos outside of map");
  #endif
  return &(map2d->data[naGetRectiIndexOfPosi(map2d->rect, pos)]);
}


NA_IAPI NAByte* naGetByteMap2DMutableByte(NAByteMap2D* map2d, NAPosi pos){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DMutableByte", "map2d is Null-Pointer.");
      return NA_NULL;
    }
    if(naIsRectiEmpty(map2d->rect))
      naCrash("naGetByteMap2DMutableByte", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naGetByteMap2DMutableByte", "rect of map2d is invalid. "
                                         "Uninitialized struct?");
    if(!naIsPosiInRecti(pos, map2d->rect))
      naError("naGetByteMap2DMutableByte", "pos outside of map");
    naIsPosiInRecti(pos, map2d->rect);
  #endif
  return &(map2d->data[naGetRectiIndexOfPosi(map2d->rect, pos)]);
}



NA_IAPI NABool naIsPosiInByteMap(NAPosi pos, const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naIsPosiInByteMap", "map2d is Null-Pointer.");
      return NA_FALSE;
    }
    if(naIsRectiEmpty(map2d->rect))
      naCrash("naIsPosiInByteMap", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naIsPosiInByteMap", "rect of map2d is invalid. "
                                         "Uninitialized struct?");
  #endif
  return naIsPosiInRecti(pos, map2d->rect);
}


NA_IAPI NARecti naGetByteMap2DRect(const NAByteMap2D* map2d){
  #ifndef NDEBUG
    if(!map2d){
      naCrash("naGetByteMap2DRect", "map2d is Null-Pointer.");
      return naMakeRecti(naMakePosi(0, 0), naMakeSizei(0, 0));
    }
    // Note that the following empty-check has been removed. It is a warning
    // which is very annoying. At first, there existed a separate function
    // naIsByteMap2DEmpty to solve that but the author himself found this to
    // be very stupid and it lead to bad programming style. If an empty rect
    // is used later on, errors will be emitted anyway.
    // if(naIsRectiEmpty(map2d->rect))
    //   naError("naGetByteMap2DRect", "map2d is Empty.");
    if(!naIsRectiValid(map2d->rect))
      naError("naGetByteMap2DRect", "rect of map2d is invalid. "
                                         "Uninitialized struct?");
  #endif
  return map2d->rect;
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


