
// This file is an inline implmenentation (II) file which is included in the
// NAColor.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAColor.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_ConvertHSVToHSL(float out[3], const float in[3]) {
  out[0] = in[0];
  out[1] = in[1] * in[2];
  out[2] = in[2] - 0.5f * out[1];
}

NA_HDEF void na_ConvertHSLToHSV(float out[3], const float in[3]) {
  out[0] = in[0];
  out[2] = in[2] + .5f * in[1];
  if(out[2] == 0.f) {
    out[1] = in[1] / NA_SINGULARITYf;
  }else{
    out[1] = in[1] / out[2];
  }
}

NA_HDEF void na_FillHSVWithColor(float out[3], const NAColor* in) {
  float min;
  float range;
  float rgb[3];
  rgb[0] = in->r;
  rgb[1] = in->g;
  rgb[2] = in->b;
  if(rgb[0] < rgb[1]) {
    min = rgb[0]; out[2] = rgb[1];
      }else{
    min = rgb[1]; out[2] = rgb[0];
  }
  if(rgb[2] < min) {
    min = rgb[2];
  }
  if(rgb[2] > out[2]) {
    out[2] = rgb[2];
  }
  range = out[2] - min;
  if(out[2] == 0.f) {
    out[1] = 0.f;
  }else{
    out[1] = range / out[2];
  }
  if(naAlmostZero(range)) {
    out[0] = 0.f;
  }else{
    float invrange = naInvf(range);
    if(out[2] == rgb[0])     { out[0] = 60.f * (0.f + (rgb[1]-rgb[2]) * invrange); }
    else if(out[2] == rgb[1]) { out[0] = 60.f * (2.f + (rgb[2]-rgb[0]) * invrange); }
    else                     { out[0] = 60.f * (4.f + (rgb[0]-rgb[1]) * invrange); }
    if(out[0] < 0.f) {
      out[0] += 360.f;
    }
  }
}

NA_HDEF void na_FillColorWithHSV(NAColor* out, const float in[3]) {
  float hsv[3];
  hsv[0] = in[0];
  hsv[1] = in[1];
  hsv[2] = in[2];
//  if(hsv[0] > 360.f){hsv[0] -= 360.f;}
//  if(hsv[0] < 0.f){hsv[0] += 360.f;}
  size_t h0 = (size_t)floorf(hsv[0] / 60.f);
  uint8 h1 = (uint8)((h0 % 6) + 6) % 6;
  float f = (hsv[0] / 60.f) - h0;
  float range = hsv[2] * hsv[1];
  float min = hsv[2] - range;
  float inc = f * range;
  float dec = (1.f - f) * range;
  inc += min;
  dec += min;
  switch(h1) {
  case 0: out->r = hsv[2] ; out->g = inc    ; out->b = min    ; break;
  case 1: out->r = dec    ; out->g = hsv[2] ; out->b = min    ; break;
  case 2: out->r = min    ; out->g = hsv[2] ; out->b = inc    ; break;
  case 3: out->r = min    ; out->g = dec    ; out->b = hsv[2] ; break;
  case 4: out->r = inc    ; out->g = min    ; out->b = hsv[2] ; break;
  case 5: out->r = hsv[2] ; out->g = min    ; out->b = dec    ; break;
  default:
    out->r = 0.f ; out->g = 0.f ; out->b = 0.f; break;
  }
  out->alpha = 1.;
}



NA_HIDEF void na_BlendColorZero(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  NA_UNUSED(topPtr);
  NA_UNUSED(factor);
  if(basePtr->alpha > NA_SINGULARITYf) {
    naFillColorWithCopy(dstPtr, basePtr);
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorLinear(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseColorFactor = (1.f - factor) * basePtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * topPtr->r;
    dstPtr->g = baseBlend * basePtr->g + topBlend * topPtr->g;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->alpha = colorSum;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorOverlay(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseColorFactor = (1.f - factor * topPtr->alpha) * basePtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * topPtr->r;
    dstPtr->g = baseBlend * basePtr->g + topBlend * topPtr->g;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->alpha = basePtr->alpha + (1.f - basePtr->alpha) * factor * topPtr->alpha;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorOpaque(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseColorFactor = basePtr->alpha * (1.f - factor * topPtr->alpha);
  float topColorFactor = basePtr->alpha * factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * topPtr->r;
    dstPtr->g = baseBlend * basePtr->g + topBlend * topPtr->g;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->alpha = basePtr->alpha;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorMultiply(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseColorFactor = 1.f - factor * topPtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * topPtr->r * basePtr->r;
    dstPtr->g = baseBlend * basePtr->g + topBlend * topPtr->g * basePtr->g;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b * basePtr->b;
    dstPtr->alpha = basePtr->alpha;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorScreen(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseColorFactor = 1.f - topPtr->alpha * factor;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * (1.f - (1.f - topPtr->r) * (1.f - basePtr->r));
    dstPtr->g = baseBlend * basePtr->g + topBlend * (1.f - (1.f - topPtr->g) * (1.f - basePtr->g));
    dstPtr->b = baseBlend * basePtr->b + topBlend * (1.f - (1.f - topPtr->b) * (1.f - basePtr->b));
    dstPtr->alpha = basePtr->alpha;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorErodeLight(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float HSV[3];
  float baseHSL[3];
  na_FillHSVWithColor(HSV, basePtr);
  na_ConvertHSVToHSL(baseHSL, HSV);

  float baseColorFactor = (1.f - factor) * topPtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * topPtr->r;
    dstPtr->g = baseBlend * basePtr->g + topBlend * topPtr->g;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->alpha = (1.f - factor) * basePtr->alpha + factor * (1.f - baseHSL[2]) * basePtr->alpha * topPtr->alpha;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorErodeDark(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float HSV[3];
  float baseHSL[3];
  na_FillHSVWithColor(HSV, basePtr);
  na_ConvertHSVToHSL(baseHSL, HSV);

  float baseColorFactor = (1.f - factor) * topPtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf) {
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->r = baseBlend * basePtr->r + topBlend * topPtr->r;
    dstPtr->g = baseBlend * basePtr->g + topBlend * topPtr->g;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->alpha = (1.f - factor) * basePtr->alpha + factor * baseHSL[2] * basePtr->alpha * topPtr->alpha;
  }else{
    naFillColorWithTransparent(dstPtr);
  }
}

#include <stdio.h>

NA_HIDEF void na_BlendColorEraseHue(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float HSV[3];
  float baseHSL[3];
  float topHSL[3];
  na_FillHSVWithColor(HSV, basePtr);
  na_ConvertHSVToHSL(baseHSL, HSV);
  na_FillHSVWithColor(HSV, topPtr);
  na_ConvertHSVToHSL(topHSL, HSV);

  float hDiff = topHSL[0] - baseHSL[0];
  if(hDiff > 180.f) {
    hDiff -= 360.f;
  }else if(hDiff < -180.f) {
    hDiff += 360.f;
  }

  if(hDiff < -60.f || hDiff > 60.f) {
    // Not near the hue, leave the color as it is.
    naFillColorWithCopy(dstPtr, basePtr);
  }else{
    float hueCloseness = 1.f - naAbsf(hDiff / 60.f);
    
    float discolorizationHue = baseHSL[0];
  
    if(hDiff < 0.f){
      // distance to the safe color
      float safeDist = (1.f + hDiff / 60.f);
      
      if(safeDist < factor || factor > NA_SUB_NORMf){
        discolorizationHue = topHSL[0] + 60.f;
      }else{
        float linFactor = (1.f - safeDist) / (1.f - factor);
        discolorizationHue = linFactor * (topHSL[0] + 60.f) + (1.f - linFactor) * baseHSL[0];
      }
    }else{
      // distance to the safe color
      float safeDist = (1.f - hDiff / 60.f);
      
      if(safeDist < factor || factor > NA_SUB_NORMf){
        discolorizationHue = topHSL[0] - 60.f;
        if(discolorizationHue < 0.f && baseHSL[0] > 180.f)
          discolorizationHue += 360.f;
      }else{
        float linFactor = (1.f - safeDist) / (1.f - factor);
        float discolorHue = topHSL[0] - 60.f;
        if(discolorHue < 0.f && baseHSL[0] > 180.f)
          discolorHue += 360.f;
        discolorizationHue = linFactor * discolorHue + (1.f - linFactor) * baseHSL[0];
      }
    }
    
    float prevSaturation = baseHSL[1];
    
    baseHSL[0] = factor * discolorizationHue + (1.f - factor) * baseHSL[0];
    baseHSL[1] *= (1.f - hueCloseness * factor);
    
    na_ConvertHSLToHSV(HSV, baseHSL);
    na_FillColorWithHSV(dstPtr, HSV);

    dstPtr->alpha = basePtr->alpha * (1.f - prevSaturation * hueCloseness * factor);
  }
   
   if(dstPtr->alpha == 0.f) {
    dstPtr->r = 0.f;
    dstPtr->g = 0.f;
    dstPtr->b = 0.f;
  }
}

NA_DEF void naBlendColors(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor,
  NABlendMode mode,
  size_t count,
  NABool baseIsImage,
  NABool topIsImage)
{
  for(size_t i = 0; i < count; ++i)
  {
    switch(mode) {
    case NA_BLEND_ZERO:
      na_BlendColorZero(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_LINEAR:
      na_BlendColorLinear(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_OVERLAY:
      na_BlendColorOverlay(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_OPAQUE:
      na_BlendColorOpaque(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_MULTIPLY:
      na_BlendColorMultiply(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_SCREEN:
      na_BlendColorScreen(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_ERODE_LIGHT:
      na_BlendColorErodeLight(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_ERODE_DARK:
      na_BlendColorErodeDark(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_ERASE_HUE:
      na_BlendColorEraseHue(dstPtr, basePtr, topPtr, factor);
      break;
    }

    dstPtr += 1;
    if(baseIsImage)
      basePtr += 1;
    if(topIsImage)
      topPtr += 1;
  }
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
