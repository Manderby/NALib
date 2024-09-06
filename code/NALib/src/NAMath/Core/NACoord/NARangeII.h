
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NARange naMakeRange(double origin, double length) {
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(origin) && naIsLengthValueValid(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueUseful(length))
      naError("length is not useful.");
  #endif
  NARange newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARangef naMakeRangef(float origin, float length) {
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(origin) && naIsLengthValueValidf(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueUsefulf(length))
      naError("length is not useful.");
  #endif
  NARangef newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARangei32 naMakeRangei32(int32 origin, int32 length) {
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(origin) && naIsLengthValueValidi32(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueUsefuli32(length))
      naError("length is not useful.");
  #endif
  NARangei32 newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARangei64 naMakeRangei64(int64 origin, int64 length) {
  #if NA_DEBUG
  if(!(naIsOffsetValueValidi64(origin) && naIsLengthValueValidi64(length)))
    naError("Invalid values given.");
  if(!naIsLengthValueUsefuli64(length))
    naError("length is not useful.");
  #endif
  NARangei64 newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARanges naMakeRanges(size_t origin, size_t length) {
  #if NA_DEBUG
  if(!(naIsOffsetValueValids(origin) && naIsLengthValueValids(length)))
    naError("Invalid values given.");
  if(!naIsLengthValueUsefuls(length))
    naError("length is not useful.");
  #endif
  NARanges newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}



NA_IDEF NARange naMakeRangeE(double origin, double length) {
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(origin) && naIsLengthValueValid(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueValid(length))
      naError("length is not valid.");
  #endif
  NARange newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARangef naMakeRangefE(float origin, float length) {
  #if NA_DEBUG
  if(!(naIsOffsetValueValidf(origin) && naIsLengthValueValidf(length)))
    naError("Invalid values given.");
  if(!naIsLengthValueValidf(length))
    naError("length is not valid.");
  #endif
  NARangef newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARangei32 naMakeRangei32E(int32 origin, int32 length) {
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(origin) && naIsLengthValueValidi32(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueValidi32(length))
      naError("length is not valid.");
  #endif
  NARangei32 newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARangei64 naMakeRangei64E(int64 origin, int64 length) {
  #if NA_DEBUG
  if(!(naIsOffsetValueValidi64(origin) && naIsLengthValueValidi64(length)))
    naError("Invalid values given.");
  if(!naIsLengthValueValidi64(length))
    naError("length is not valid.");
  #endif
  NARangei64 newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}

NA_IDEF NARanges naMakeRangesE(size_t origin, size_t length) {
  #if NA_DEBUG
  if(!(naIsOffsetValueValids(origin) && naIsLengthValueValids(length)))
    naError("Invalid values given.");
  if(!naIsLengthValueValids(length))
    naError("length is not valid.");
  #endif
  NARanges newRange;
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}



NA_IDEF NARange naMakeRangeZero() {
  NARange newRange;
  newRange.origin = 0.;
  newRange.length = 0.;
  return newRange;
}

NA_IDEF NARangef naMakeRangefZero() {
  NARangef newRange;
  newRange.origin = 0.f;
  newRange.length = 0.f;
  return newRange;
}

NA_IDEF NARangei32 naMakeRangei32Zero() {
  NARangei32 newRange;
  newRange.origin = 0;
  newRange.length = 0;
  return newRange;
}

NA_IDEF NARangei64 naMakeRangei64Zero() {
  NARangei64 newRange;
  newRange.origin = 0;
  newRange.length = 0;
  return newRange;
}

NA_IDEF NARanges naMakeRangesZero() {
  NARanges newRange;
  newRange.origin = 0;
  newRange.length = 0;
  return newRange;
}



NA_IDEF void naFillRangeWithCopy(NARange* ranged, const NARange* ranges) {
  ranged->origin = ranges->origin;
  ranged->length = ranges->length;
}
NA_IDEF void naFillRangefWithCopy(NARangef* ranged, const NARangef* ranges) {
  ranged->origin = ranges->origin;
  ranged->length = ranges->length;
}
NA_IDEF void naFillRangei32WithCopy(NARangei32* ranged, const NARangei32* ranges) {
  ranged->origin = ranges->origin;
  ranged->length = ranges->length;
}
NA_IDEF void naFillRangei64WithCopy(NARangei64* ranged, const NARangei64* ranges) {
  ranged->origin = ranges->origin;
  ranged->length = ranges->length;
}
NA_IDEF void naFillRangesWithCopy(NARanges* ranged, const NARanges* ranges) {
  ranged->origin = ranges->origin;
  ranged->length = ranges->length;
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
