
#include "../NAColor.h"
#include "../../NAMath/NAMathOperators.h"



// This factor is nice and easy to remember. There is no greater mathematical
// purpose in this number. It's just nice.
#define NA_ABY_FACTOR 0.75f

NA_IDEF float naConvertToPerceptualColorValue(float value){
  return value / (NA_ABY_FACTOR * value - (1.f - NA_ABY_FACTOR));
}
NA_IDEF float naConvertToRadiometricColorValue(float value){
  return -(1.f - NA_ABY_FACTOR) * value / (1.f + NA_ABY_FACTOR * value);
}



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

void naFillHSVWithColor(float out[3], const NAColor* in){
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

void naFillColorWithHSV(NAColor* out, const float in[3]){
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



NA_HIDEF void na_ConvertToPerceptualRGB(float* outColor, const float* inColor){
  outColor[0] = naConvertToPerceptualColorValue(inColor[0]);
  outColor[1] = naConvertToPerceptualColorValue(inColor[1]);
  outColor[2] = naConvertToPerceptualColorValue(inColor[2]);
}

NA_HIDEF void na_ConvertToRadiometricRGB(float* outColor, const float* inColor){
  outColor[0] = naConvertToRadiometricColorValue(inColor[0]);
  outColor[1] = naConvertToRadiometricColorValue(inColor[1]);
  outColor[2] = naConvertToRadiometricColorValue(inColor[2]);
}



NA_HIDEF void na_BlendColorZero(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  NA_UNUSED(topPtr);
  NA_UNUSED(factor);
  if(basePtr->alpha > NA_SINGULARITYf){
    naCopyColor(dstPtr, basePtr);
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
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
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
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
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
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y;
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
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * topPtr->a * basePtr->a;
    dstPtr->b = baseBlend * basePtr->b + topBlend * topPtr->b * basePtr->b;
    dstPtr->y = baseBlend * basePtr->y + topBlend * topPtr->y * basePtr->y;
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
  if(colorSum > NA_SINGULARITYf){
    float baseBlend = baseColorFactor / colorSum;
    float topBlend = 1.f - baseBlend;
    dstPtr->a = baseBlend * basePtr->a + topBlend * (1.f - (1.f - topPtr->a) * (1.f - basePtr->a));
    dstPtr->b = baseBlend * basePtr->b + topBlend * (1.f - (1.f - topPtr->b) * (1.f - basePtr->b));
    dstPtr->y = baseBlend * basePtr->y + topBlend * (1.f - (1.f - topPtr->y) * (1.f - basePtr->y));
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
  float baseHSV[3];
  float baseHSL[3];
  naFillHSVWithColor(baseHSV, basePtr);
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
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorErodeDark(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseHSV[3];
  float baseHSL[3];
  naFillHSVWithColor(baseHSV, basePtr);
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
    naFillColorWithTransparent(dstPtr);
  }
}

NA_HIDEF void na_BlendColorEraseHue(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor)
{
  float baseHSV[3];
  float baseHSL[3];
  naFillHSVWithColor(baseHSV, basePtr);
  naConvertHSVToHSL(baseHSL, baseHSV);
  float topHSV[3];
  float topHSL[3];
  naFillHSVWithColor(topHSV, topPtr);
  naConvertHSVToHSL(topHSL, topHSV);

  float hDiff = topHSL[0] - baseHSL[0];
  if(hDiff > 180.f){hDiff -= 360.f;}
  else if(hDiff < -180.f){hDiff += 360.f;}

  if(hDiff < -60.f || hDiff > 60.f){
    // Not near the hue, leave the color as it is.
    naCopyColor(dstPtr, basePtr);
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
      naFillColorWithHSV(dstPtr, baseHSV);
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
      naFillColorWithHSV(dstPtr, baseHSV);
      dstPtr->alpha = (1.f - factorL * factor) * basePtr->alpha;
    }
  }

  if(dstPtr->alpha == 0.f){
    dstPtr->a = 0.f;
    dstPtr->b = 0.f;
    dstPtr->y = 0.f;
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
    switch(mode){
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
    if(baseIsImage){basePtr += 1;}
    if(topIsImage){topPtr += 1;}
  }
}
