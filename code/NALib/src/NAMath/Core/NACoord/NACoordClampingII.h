
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NARange naClampRange(NARange range, NARange clampRange) {
  double value;
  NARange newRange;
  #if NA_DEBUG
    if(naIsRangeEmpty(range))
      naError("range is empty.");
    if(!naIsRangeValid(range))
      naError("range is invalid.");
    if(naIsRangeEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangeValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0.) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangeEnd(clampRange);
  if(naGetRangeEnd(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}

NA_IDEF NARangef naClampRangef(NARangef range, NARangef clampRange) {
  float value;
  NARangef newRange;
  #if NA_DEBUG
    if(naIsRangefEmpty(range))
      naError("range is empty.");
    if(!naIsRangefValid(range))
      naError("range is invalid.");
    if(naIsRangefEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangefValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0.f) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangefEnd(clampRange);
  if(naGetRangefEnd(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}

NA_IDEF NARangei32 naClampRangei32(NARangei32 range, NARangei32 clampRange) {
  int32 value;
  NARangei32 newRange;
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
    if(naIsRangei32Empty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangei32Valid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangei32End(clampRange);
  if(naGetRangei32End(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}

NA_IDEF NARangei64 naClampRangei64(NARangei64 range, NARangei64 clampRange) {
  int64 value;
  NARangei64 newRange;
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
    if(naIsRangei64Empty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangei64Valid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangei64End(clampRange);
  if(naGetRangei64End(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}

NA_IDEF NARanges naClampRanges(NARanges range, NARanges clampRange) {
  int64 value;
  NARanges newRange;
  #if NA_DEBUG
    if(naIsRangesEmpty(range))
      naError("range is empty.");
    if(!naIsRangesValid(range))
      naError("range is invalid.");
    if(naIsRangesEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangesValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangesEnd(clampRange);
  if(naGetRangesEnd(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}



NA_IDEF double naClampPointToRange(double point, NARange clampRange) {
  double end;
  double newPoint;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(point))
      naError("point is invalid.");
    if(naIsRangeEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangeValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newPoint = point;
  if(newPoint < clampRange.origin) { newPoint = clampRange.origin; }
  end = naGetRangeEnd(clampRange);
  if(newPoint > end) {
    newPoint = end;
  }
  return newPoint;
}

NA_IDEF float naClampPointToRangef(float point, NARangef clampRange) {
  float end;
  float newPoint;
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(point))
      naError("point is invalid.");
    if(naIsRangefEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangefValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newPoint = point;
  if(newPoint < clampRange.origin) { newPoint = clampRange.origin; }
  end = naGetRangefEnd(clampRange);
  if(newPoint > end) {
    newPoint = end;
  }
  return newPoint;
}

NA_IDEF int32 naClampPointToRangei32(int32 point, NARangei32 clampRange) {
  int32 max;
  int32 newPoint;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(point))
      naError("point is invalid.");
    if(naIsRangei32Empty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangei32Valid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newPoint = point;
  if(newPoint <  clampRange.origin) { newPoint = clampRange.origin; }
  max = naGetRangei32Max(clampRange);
  if(newPoint > max) {
    newPoint = max;
  }
  return newPoint;
}

NA_IDEF int64 naClampPointToRangei64(int64 point, NARangei64 clampRange) {
  int64 max;
  int64 newPoint;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(point))
      naError("point is invalid.");
    if(naIsRangei64Empty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangei64Valid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newPoint = point;
  if(newPoint <  clampRange.origin) { newPoint = clampRange.origin; }
  max = naGetRangei64Max(clampRange);
  if(newPoint > max) {
    newPoint = max;
  }
  return newPoint;
}

NA_IDEF size_t naClampPointToRanges(size_t point, NARanges clampRange) {
  size_t max;
  size_t newPoint;
  #if NA_DEBUG
    if(!naIsOffsetValueValids(point))
      naError("point is invalid.");
    if(naIsRangesEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangesValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newPoint = point;
  if(newPoint <  clampRange.origin) { newPoint = clampRange.origin; }
  max = naGetRangesMax(clampRange);
  if(newPoint > max) {
    newPoint = max;
  }
  return newPoint;
}



NA_IDEF NARect naClampRect(NARect rect, NARect clampRect) {
  // Todo: use helper functions in this function.
  double value;
  NARect newRect;
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectValid(rect))
      naError("rect is invalid.");
    if(naIsRectEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newRect = rect;
  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRectEndX(clampRect);
  if(naGetRectEndXE(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRectEndY(clampRect);
  if(naGetRectEndYE(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
  }
  return newRect;
}

NA_IDEF NARectf naClampRectf(NARectf rect, NARectf clampRect) {
  // Todo: use helper functions in this function.
  float value;
  NARectf newRect;
  #if NA_DEBUG
    if(naIsRectfEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectfValid(rect))
      naError("rect is invalid.");
    if(naIsRectfEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectfValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newRect = rect;
  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRectfEndX(clampRect);
  if(naGetRectfEndXE(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRectfEndY(clampRect);
  if(naGetRectfEndYE(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
  }
  return newRect;
}

NA_IDEF NARecti32 naClampRecti32(NARecti32 rect, NARecti32 clampRect) {
  // Todo: use helper functions in this function.
  int32 value;
  NARecti32 newRect;
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(naIsRecti32EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti32Valid(clampRect))
      naError("clampRect is invalid.");
  #endif

  newRect = rect;

  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    if(newRect.size.width <= 0) {
      return newRect;
    }
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRecti32EndX(clampRect);
  if(naGetRecti32EndX(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
    if(newRect.size.width <= 0) {
      return newRect;
    }
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    if(newRect.size.height <= 0) {
      return newRect;
    }
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRecti32EndY(clampRect);
  if(naGetRecti32EndY(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
    if(newRect.size.height <= 0) {
      return newRect;
    }
  }
  return newRect;
}

NA_IDEF NARecti64 naClampRecti64(NARecti64 rect, NARecti64 clampRect) {
  // Todo: use helper functions in this function.
  int64 value;
  NARecti64 newRect;
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(naIsRecti64EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti64Valid(clampRect))
      naError("clampRect is invalid.");
  #endif

  newRect = rect;

  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    if(newRect.size.width <= 0) {
      return newRect;
    }
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRecti64EndX(clampRect);
  if(naGetRecti64EndX(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
    if(newRect.size.width <= 0) {
      return newRect;
    }
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    if(newRect.size.height <= 0) {
      return newRect;
    }
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRecti64EndY(clampRect);
  if(naGetRecti64EndY(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
    if(newRect.size.height <= 0) {
      return newRect;
    }
  }
  return newRect;
}

NA_IDEF NARects naClampRects(NARects rect, NARects clampRect) {
  // Todo: use helper functions in this function.
  size_t value;
  NARects newRect;
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
    if(naIsRectsEmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectsValid(clampRect))
      naError("clampRect is invalid.");
  #endif

  newRect = rect;

  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    if(newRect.size.width <= 0) {
      return newRect;
    }
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRectsEndX(clampRect);
  if(naGetRectsEndX(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
    if(newRect.size.width <= 0) {
      return newRect;
    }
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    if(newRect.size.height <= 0) {
      return newRect;
    }
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRectsEndY(clampRect);
  if(naGetRectsEndY(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
    if(newRect.size.height <= 0) {
      return newRect;
    }
  }
  return newRect;
}



NA_IDEF NAPos naClampPointToRect(NAPos point, NARect clampRect) {
  double end;
  NAPos newPoint;
  #if NA_DEBUG
    if(!naIsPosValid(point))
      naError("point is invalid.");
    if(naIsRectEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x < clampRect.pos.x) { newPoint.x = clampRect.pos.x; }
  end = naGetRectEndX(clampRect);
  if(newPoint.x > end) {
    newPoint.x = end;
  }
  if(newPoint.y < clampRect.pos.y) { newPoint.y = clampRect.pos.y; }
  end = naGetRectEndY(clampRect);
  if(newPoint.y > end) {
    newPoint.y = end;
  }
  return newPoint;
}

NA_IDEF NAPosf naClampPointToRectf(NAPosf point, NARectf clampRect) {
  float end;
  NAPosf newPoint;
  #if NA_DEBUG
    if(!naIsPosfValid(point))
      naError("point is invalid.");
    if(naIsRectfEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectfValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x < clampRect.pos.x) { newPoint.x = clampRect.pos.x; }
  end = naGetRectfEndX(clampRect);
  if(newPoint.x > end) {
    newPoint.x = end;
  }
  if(newPoint.y < clampRect.pos.y) { newPoint.y = clampRect.pos.y; }
  end = naGetRectfEndY(clampRect);
  if(newPoint.y > end) {
    newPoint.y = end;
  }
  return newPoint;
}

NA_IDEF NAPosi32 naClampPointToRecti32(NAPosi32 point, NARecti32 clampRect) {
  int32 max;
  NAPosi32 newPoint;
  #if NA_DEBUG
    if(!naIsPosi32Valid(point))
      naError("point is invalid.");
    if(naIsRecti32EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti32Valid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x <  clampRect.pos.x) { newPoint.x = clampRect.pos.x; }
  max = naGetRecti32MaxX(clampRect);
  if(newPoint.x > max) {
    newPoint.x = max;
  }
  if(newPoint.y <  clampRect.pos.y) { newPoint.y = clampRect.pos.y; }
  max = naGetRecti32MaxY(clampRect);
  if(newPoint.y > max) {
    newPoint.y = max;
  }
  return newPoint;
}

NA_IDEF NAPosi64 naClampPointToRecti64(NAPosi64 point, NARecti64 clampRect) {
  int64 max;
  NAPosi64 newPoint;
  #if NA_DEBUG
    if(!naIsPosi64Valid(point))
      naError("point is invalid.");
    if(naIsRecti64EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti64Valid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x <  clampRect.pos.x) { newPoint.x = clampRect.pos.x; }
  max = naGetRecti64MaxX(clampRect);
  if(newPoint.x > max) {
    newPoint.x = max;
  }
  if(newPoint.y <  clampRect.pos.y) { newPoint.y = clampRect.pos.y; }
  max = naGetRecti64MaxY(clampRect);
  if(newPoint.y > max) {
    newPoint.y = max;
  }
  return newPoint;
}

NA_IDEF NAPoss naClampPointToRects(NAPoss point, NARects clampRect) {
  size_t max;
  NAPoss newPoint;
  #if NA_DEBUG
    if(!naIsPossValid(point))
      naError("point is invalid.");
    if(naIsRectsEmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectsValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x <  clampRect.pos.x) { newPoint.x = clampRect.pos.x; }
  max = naGetRectsMaxX(clampRect);
  if(newPoint.x > max) {
    newPoint.x = max;
  }
  if(newPoint.y <  clampRect.pos.y) { newPoint.y = clampRect.pos.y; }
  max = naGetRectsMaxY(clampRect);
  if(newPoint.y > max) {
    newPoint.y = max;
  }
  return newPoint;
}



NA_IDEF NABox naClampBox(NABox box, NABox clampBox) {
  // Todo: use helper functions in this function.
  double value;
  NABox newBox;
  #if NA_DEBUG
    if(naIsBoxEmpty(box))
      naError("box is empty.");
    if(!naIsBoxValid(box))
      naError("box is invalid.");
    if(naIsBoxEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newBox = box;
  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxEndX(clampBox);
  if(naGetBoxEndXE(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxEndY(clampBox);
  if(naGetBoxEndYE(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxEndZ(clampBox);
  if(naGetBoxEndZE(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
  }
  return newBox;
}

NA_IDEF NABoxf naClampBoxf(NABoxf box, NABoxf clampBox) {
  // Todo: use helper functions in this function.
  float value;
  NABoxf newBox;
  #if NA_DEBUG
    if(naIsBoxfEmpty(box))
      naError("box is empty.");
    if(!naIsBoxfValid(box))
      naError("box is invalid.");
    if(naIsBoxfEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxfValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newBox = box;
  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxfEndX(clampBox);
  if(naGetBoxfEndXE(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxfEndY(clampBox);
  if(naGetBoxfEndYE(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxfEndZ(clampBox);
  if(naGetBoxfEndZE(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
  }
  return newBox;
}

NA_IDEF NABoxi32 naClampBoxi32(NABoxi32 box, NABoxi32 clampBox) {
  // Todo: use helper functions in this function.
  int32 value;
  NABoxi32 newBox;
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(naIsBoxi32EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi32Valid(clampBox))
      naError("clampBox is invalid.");
  #endif

  newBox = box;

  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxi32EndX(clampBox);
  if(naGetBoxi32EndX(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxi32EndY(clampBox);
  if(naGetBoxi32EndY(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxi32EndZ(clampBox);
  if(naGetBoxi32EndZ(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
  }
  return newBox;
}

NA_IDEF NABoxi64 naClampBoxi64(NABoxi64 box, NABoxi64 clampBox) {
  // Todo: use helper functions in this function.
  int64 value;
  NABoxi64 newBox;
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(naIsBoxi64EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi64Valid(clampBox))
      naError("clampBox is invalid.");
  #endif

  newBox = box;

  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxi64EndX(clampBox);
  if(naGetBoxi64EndX(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxi64EndY(clampBox);
  if(naGetBoxi64EndY(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxi64EndZ(clampBox);
  if(naGetBoxi64EndZ(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
  }
  return newBox;
}

NA_IDEF NABoxs naClampBoxs(NABoxs box, NABoxs clampBox) {
  // Todo: use helper functions in this function.
  size_t value;
  NABoxs newBox;
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
    if(naIsBoxsEmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxsValid(clampBox))
      naError("clampBox is invalid.");
  #endif

  newBox = box;

  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxsEndX(clampBox);
  if(naGetBoxsEndX(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxsEndY(clampBox);
  if(naGetBoxsEndY(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxsEndZ(clampBox);
  if(naGetBoxsEndZ(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
  }
  return newBox;
}



NA_IDEF NAVertex naClampPointToBox(NAVertex point, NABox clampBox) {
  double end;
  NAVertex newPoint;
  #if NA_DEBUG
    if(!naIsVertexValid(point))
      naError("point is invalid.");
    if(naIsBoxEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x < clampBox.vertex.x) { newPoint.x = clampBox.vertex.x; }
  end = naGetBoxEndX(clampBox);
  if(newPoint.x > end) {
    newPoint.x = end;
  }
  if(newPoint.y < clampBox.vertex.y) { newPoint.y = clampBox.vertex.y; }
  end = naGetBoxEndY(clampBox);
  if(newPoint.y > end) {
    newPoint.y = end;
  }
  if(newPoint.z < clampBox.vertex.z) { newPoint.z = clampBox.vertex.z; }
  end = naGetBoxEndZ(clampBox);
  if(newPoint.z > end) {
    newPoint.z = end;
  }
  return newPoint;
}

NA_IDEF NAVertexf naClampPointToBoxf(NAVertexf point, NABoxf clampBox) {
  float end;
  NAVertexf newPoint;
  #if NA_DEBUG
    if(!naIsVertexfValid(point))
      naError("point is invalid.");
    if(naIsBoxfEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxfValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x < clampBox.vertex.x) { newPoint.x = clampBox.vertex.x; }
  end = naGetBoxfEndX(clampBox);
  if(newPoint.x > end) {
    newPoint.x = end;
  }
  if(newPoint.y < clampBox.vertex.y) { newPoint.y = clampBox.vertex.y; }
  end = naGetBoxfEndY(clampBox);
  if(newPoint.y > end) {
    newPoint.y = end;
  }
  if(newPoint.z < clampBox.vertex.z) { newPoint.z = clampBox.vertex.z; }
  end = naGetBoxfEndZ(clampBox);
  if(newPoint.z > end) {
    newPoint.z = end;
  }
  return newPoint;
}

NA_IDEF NAVertexi32 naClampPointToBoxi32(NAVertexi32 point, NABoxi32 clampBox) {
  int32 max;
  NAVertexi32 newPoint;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(point))
      naError("point is invalid.");
    if(naIsBoxi32EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi32Valid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x <  clampBox.vertex.x) { newPoint.x = clampBox.vertex.x; }
  max = naGetBoxi32MaxX(clampBox);
  if(newPoint.x > max) {
    newPoint.x = max;
  }
  if(newPoint.y <  clampBox.vertex.y) { newPoint.y = clampBox.vertex.y; }
  max = naGetBoxi32MaxY(clampBox);
  if(newPoint.y > max) {
    newPoint.y = max;
  }
  if(newPoint.z <  clampBox.vertex.z) { newPoint.z = clampBox.vertex.z; }
  max = naGetBoxi32MaxZ(clampBox);
  if(newPoint.z > max) {
    newPoint.z = max;
  }
  return newPoint;
}

NA_IDEF NAVertexi64 naClampPointToBoxi64(NAVertexi64 point, NABoxi64 clampBox) {
  int64 max;
  NAVertexi64 newPoint;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(point))
      naError("point is invalid.");
    if(naIsBoxi64EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi64Valid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x <  clampBox.vertex.x) { newPoint.x = clampBox.vertex.x; }
  max = naGetBoxi64MaxX(clampBox);
  if(newPoint.x > max) {
    newPoint.x = max;
  }
  if(newPoint.y <  clampBox.vertex.y) { newPoint.y = clampBox.vertex.y; }
  max = naGetBoxi64MaxY(clampBox);
  if(newPoint.y > max) {
    newPoint.y = max;
  }
  if(newPoint.z <  clampBox.vertex.z) { newPoint.z = clampBox.vertex.z; }
  max = naGetBoxi64MaxZ(clampBox);
  if(newPoint.z > max) {
    newPoint.z = max;
  }
  return newPoint;
}

NA_IDEF NAVertexs naClampPointToBoxs(NAVertexs point, NABoxs clampBox) {
  size_t max;
  NAVertexs newPoint;
  #if NA_DEBUG
    if(!naIsVertexsValid(point))
      naError("point is invalid.");
    if(naIsBoxsEmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxsValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newPoint = point;
  if(newPoint.x <  clampBox.vertex.x) { newPoint.x = clampBox.vertex.x; }
  max = naGetBoxsMaxX(clampBox);
  if(newPoint.x > max) {
    newPoint.x = max;
  }
  if(newPoint.y <  clampBox.vertex.y) { newPoint.y = clampBox.vertex.y; }
  max = naGetBoxsMaxY(clampBox);
  if(newPoint.y > max) {
    newPoint.y = max;
  }
  if(newPoint.z <  clampBox.vertex.z) { newPoint.z = clampBox.vertex.z; }
  max = naGetBoxsMaxZ(clampBox);
  if(newPoint.z > max) {
    newPoint.z = max;
  }
  return newPoint;
}


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
