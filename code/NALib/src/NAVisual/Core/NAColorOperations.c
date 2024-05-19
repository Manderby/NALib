
#include "../NAABYColor.h"
#include "../../NAMath/NAMathOperators.h"


NA_DEF void naConvertHSVToHSL(float out[3], const float in[3]){
  out[0] = in[0];
  out[1] = in[1] * in[2];
  out[2] = in[2] - 0.5f * out[1];
}

NA_DEF void naConvertHSLToHSV(float out[3], const float in[3]){
  out[0] = in[0];
  out[2] = in[2] + .5f * in[1];
  if(out[2] == 0.f){
    out[1] = in[1] / NA_SINGULARITYf;
  }else{
    out[1] = in[1] / out[2];
  }
}

void naFillHSVWithABYColor(float out[3], const NAABYColor* in){
  float min;
  float range;
  float rgb[3];
  rgb[0] = in->a;
  rgb[1] = in->y;
  rgb[2] = in->b;
  if(rgb[0] < rgb[1]){min = rgb[0]; out[2] = rgb[1];}else{min = rgb[1]; out[2] = rgb[0];}
  if(rgb[2] < min){min = rgb[2];}
  if(rgb[2] > out[2]){out[2] = rgb[2];}
  range = out[2] - min;
  if(out[2] == 0.f){
    out[1] = 0.f;
  }else{
    out[1] = range / out[2];
  }
  if(naAlmostZero(range)){
    out[0] = 0.f;
  }else{
    float invrange = naInvf(range);
    if(out[2] == rgb[0])      {out[0] = 60.f * (0.f + (rgb[1]-rgb[2]) * invrange);}
    else if(out[2] == rgb[1]) {out[0] = 60.f * (2.f + (rgb[2]-rgb[0]) * invrange);}
    else                      {out[0] = 60.f * (4.f + (rgb[0]-rgb[1]) * invrange);}
    if(out[0] < 0.f){out[0] += 360.f;}
  }
}

void naFillABYColorWithHSV(NAABYColor* out, const float in[3]){
  float hsv[3];
  hsv[0] = in[0];
  hsv[1] = in[1];
  hsv[2] = in[2];
  size_t h0 = (size_t)floorf(hsv[0] / 60.f);
  uint8 h1 = (uint8)((h0 % 6) + 6) % 6;
  float f = (hsv[0] / 60.f) - h0;
  float range = hsv[2] * hsv[1];
  float min = hsv[2] - range;
  float inc = f * range;
  float dec = (1.f - f) * range;
  inc += min;
  dec += min;
  switch(h1){
  case 0: out->a = hsv[2] ; out->y = inc    ; out->b = min    ; break;
  case 1: out->a = dec    ; out->y = hsv[2] ; out->b = min    ; break;
  case 2: out->a = min    ; out->y = hsv[2] ; out->b = inc    ; break;
  case 3: out->a = min    ; out->y = dec    ; out->b = hsv[2] ; break;
  case 4: out->a = inc    ; out->y = min    ; out->b = hsv[2] ; break;
  case 5: out->a = hsv[2] ; out->y = min    ; out->b = dec    ; break;
  default:
    out->a = 0.f ; out->y = 0.f ; out->b = 0.f; break;
  }
  out->alpha = 1.;
}



NA_HIDEF void na_BlendABYColorZero(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  NA_UNUSED(topPtr);
  NA_UNUSED(factor);
  if(basePtr->alpha > NA_SINGULARITYf){
    naCopyABYColor(dstPtr, basePtr);
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorLinear(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseColorFactor = (1.f - factor) * basePtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
    dstPtr->alpha = colorSum;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorOverlay(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseColorFactor = (1.f - factor * topPtr->alpha) * basePtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
    dstPtr->alpha = basePtr->alpha + (1.f - basePtr->alpha) * factor * topPtr->alpha;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorOpaque(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseColorFactor = basePtr->alpha * (1.f - factor * topPtr->alpha);
  float topColorFactor = basePtr->alpha * factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
    dstPtr->alpha = basePtr->alpha;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorMultiply(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseColorFactor = 1.f - factor * topPtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a * basePtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b * basePtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y * basePtr->y;
    dstPtr->alpha = basePtr->alpha;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorScreen(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseColorFactor = 1.f - topPtr->alpha * factor;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * (1.f - (1.f - topPtr->a) * (1.f - basePtr->a));
    dstPtr->b = baseBlend * basePtr->b + topBlend * (1.f - (1.f - topPtr->b) * (1.f - basePtr->b));
    dstPtr->y = baseBlend * basePtr->y + topBlend * (1.f - (1.f - topPtr->y) * (1.f - basePtr->y));
    dstPtr->alpha = basePtr->alpha;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorErodeLight(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseHSV[3];
  float baseHSL[3];
  naFillHSVWithABYColor(baseHSV, basePtr);
  naConvertHSVToHSL(baseHSL, baseHSV);

  float baseColorFactor = (1.f - factor) * topPtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
    dstPtr->alpha = (1.f - factor) * basePtr->alpha + factor * (1.f - baseHSL[2]) * basePtr->alpha * topPtr->alpha;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorErodeDark(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseHSV[3];
  float baseHSL[3];
  naFillHSVWithABYColor(baseHSV, basePtr);
  naConvertHSVToHSL(baseHSL, baseHSV);

  float baseColorFactor = (1.f - factor) * topPtr->alpha;
  float topColorFactor = factor * topPtr->alpha;
  float colorSum = baseColorFactor + topColorFactor;
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
    dstPtr->alpha = (1.f - factor) * basePtr->alpha + factor * baseHSL[2] * basePtr->alpha * topPtr->alpha;
  }else{
    naFillABYColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendABYColorEraseHue(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor)
{
  float baseHSV[3];
  float baseHSL[3];
  naFillHSVWithABYColor(baseHSV, basePtr);
  naConvertHSVToHSL(baseHSL, baseHSV);
  float topHSV[3];
  float topHSL[3];
  naFillHSVWithABYColor(topHSV, topPtr);
  naConvertHSVToHSL(topHSL, topHSV);

  float hDiff = topHSL[0] - baseHSL[0];
  if(hDiff > 180.f){hDiff -= 360.f;}
  else if(hDiff < -180.f){hDiff += 360.f;}

  if(hDiff < -60.f || hDiff > 60.f){
    // Not near the hue, leave the color as it is.
    naCopyABYColor(dstPtr, basePtr);
  }else{
    if(hDiff <= 0.){
      // fully saturated colors have L = .5
      float factorL = 2.f * (topHSL[2] - baseHSL[2]);
      if(factorL < 0.){factorL = -factorL;}
      if(factorL > 1.f){factorL = 1.f;}
      factorL = naConvertToRadiometricColorValue(1.f - factorL); // looks better with naLinearize
      float hueStrength = 1.f - -hDiff / 60.f;
      float factor = factor * topPtr->alpha * hueStrength;
      baseHSL[1] = (1.f - factor) * baseHSL[1];
      float decolorization = factor * topPtr->alpha;
      if(hueStrength < decolorization){
        baseHSL[0] = topHSL[0] + 60.f;
      }else if((1.f - decolorization) > NA_SINGULARITYf){
        float linearFactor = (hueStrength - decolorization) / (1.f - decolorization);
        baseHSL[0] = linearFactor * baseHSL[0] + (1.f - linearFactor) * (topHSL[0] + 60.f);
      }
      if(baseHSL[0] > 360.f){baseHSL[0] -= 360.f;}
      naConvertHSLToHSV(baseHSV, baseHSL);
      naFillABYColorWithHSV(dstPtr, baseHSV);
      dstPtr->alpha = (1.f - factorL * factor) * basePtr->alpha;
    }else{
      // fully saturated colors have L = .5
      float factorL = 2.f * (topHSL[2] - baseHSL[2]);
      if(factorL < 0.){factorL = -factorL;}
      if(factorL > 1.f){factorL = 1.f;}
      factorL = naConvertToRadiometricColorValue(1.f - factorL); // looks better with naLinearize
      float hueStrength = 1.f - hDiff / 60.f;
      float factor = factor * topPtr->alpha * hueStrength;
      baseHSL[1] = (1.f - factor) * baseHSL[1];
      float decolorization = factor * topPtr->alpha;
      if(hueStrength < decolorization){
        baseHSL[0] = topHSL[0] - 60.f;
      }else if((1.f - decolorization) > NA_SINGULARITYf){
        float linearFactor = (hueStrength - decolorization) / (1.f - decolorization);
        baseHSL[0] = linearFactor * baseHSL[0] + (1.f - linearFactor) * (topHSL[0] - 60.f);
      }
      if(baseHSL[0] < 0.f){baseHSL[0] += 360.f;}
      naConvertHSLToHSV(baseHSV, baseHSL);
      naFillABYColorWithHSV(dstPtr, baseHSV);
      dstPtr->alpha = (1.f - factorL * factor) * basePtr->alpha;
    }
  }
}

NA_DEF void naBlendABYColors(
  NAABYColor* dstPtr,
  const NAABYColor* basePtr,
  const NAABYColor* topPtr,
  float factor,
  NABlendMode mode,
  size_t count,
  NABool baseIsImage,
  NABool topIsImage)
{
  for(size_t i = 0; i < count; ++i)
  {
    switch(mode){
    case NA_BLEND_ZERO:
      na_BlendABYColorZero(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_LINEAR:
      na_BlendABYColorLinear(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_OVERLAY:
      na_BlendABYColorOverlay(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_OPAQUE:
      na_BlendABYColorOpaque(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_MULTIPLY:
      na_BlendABYColorMultiply(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_SCREEN:
      na_BlendABYColorScreen(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_ERODE_LIGHT:
      na_BlendABYColorErodeLight(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_ERODE_DARK:
      na_BlendABYColorErodeDark(dstPtr, basePtr, topPtr, factor);
      break;
    case NA_BLEND_ERASE_HUE:
      na_BlendABYColorEraseHue(dstPtr, basePtr, topPtr, factor);
      break;
    }

    if(dstPtr->alpha == 0.f){
      dstPtr->a = 0.f;
      dstPtr->b = 0.f;
      dstPtr->y = 0.f;
    }

    dstPtr += 1;
    if(baseIsImage){basePtr += 1;}
    if(topIsImage){topPtr += 1;}
  }
}
