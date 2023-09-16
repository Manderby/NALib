
#include "../NABabyImage.h"
#include "../../NAMath/NAVectorAlgebra.h"


struct NABabyImage{
  NARefCount refCount;
  int32 width;
  int32 height;
  float* data;
};




NA_DEF NAInt naGetBabyImageValuesPerLine(const NABabyImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (NAInt)image->width * NA_BABY_COLOR_CHANNEL_COUNT;
}



NA_HIDEF NAInt na_GetBabyImagePixelCount(const NABabyImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (NAInt)image->width * (NAInt)image->height;
}



NA_HIDEF size_t na_GetBabyImageDataSize(const NABabyImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (size_t)image->width * (size_t)image->height * NA_BABY_COLOR_BYTES_PER_COMPONENT;
}



NA_DEF float* naGetBabyImageData(const NABabyImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return image->data;
}



NA_DEF NASizei naGetBabyImageSize(const NABabyImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return naMakeSizei(image->width, image->height);
}



NA_DEF NABabyImage* naCreateBabyImage(NASizei size, const NABabyColor color){
  NABabyImage* image;
  #if NA_DEBUG
    if(size.width <= 0 || size.height <= 0)
      naError("size must be > 0");
    if(size.width > NA_MAX_i32 || size.height > NA_MAX_i32)
      naError("size is too big");
    if(color && color[3] == 0.f && (color[0] != 0.f || color[1] != 0.f || color[2] != 0.f))
      naError("inanoSecondure color given");
  #endif
  image = naAlloc(NABabyImage);
  naInitRefCount(&image->refCount);
  image->width = (int32)size.width;
  image->height = (int32)size.height;
  image->data = naMalloc(na_GetBabyImageDataSize(image));
  if(color){
    NAInt i;
    NAInt pixelCount = na_GetBabyImagePixelCount(image);
    float* ptr = image->data;
    for(i = 0; i < pixelCount; ++i){
      naCopyV4f(ptr, color);
      ptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
  }
  return image;
}



NA_HDEF NABabyImage* naCreateBabyImageCopy(const NABabyImage* image){
  NABabyImage* newImage;
  #if NA_DEBUG
  if(!image)
    naCrash("given image is a Null pointer");
  #endif
  newImage = naAlloc(NABabyImage);
  naInitRefCount(&newImage->refCount);
  newImage->width = image->width;
  newImage->height = image->height;
  size_t dataSize = na_GetBabyImageDataSize(newImage);
  newImage->data = naMalloc(dataSize);
  naCopyn(newImage->data, image->data, dataSize);
  return newImage;
}



void na_ConvertRGBToHSV(float out[3], const float in[3]){
  float min;
  float range;
  float rgb[3];
  naCopyV3f(rgb, in);
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

void na_ConvertHSVToRGB(float out[3], const float in[3]){
  float hsv[3];
  naCopyV3f(hsv, in);
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
  case 0: out[0] = hsv[2] ; out[1] = inc    ; out[2] = min    ; break;
  case 1: out[0] = dec    ; out[1] = hsv[2] ; out[2] = min    ; break;
  case 2: out[0] = min    ; out[1] = hsv[2] ; out[2] = inc    ; break;
  case 3: out[0] = min    ; out[1] = dec    ; out[2] = hsv[2] ; break;
  case 4: out[0] = inc    ; out[1] = min    ; out[2] = hsv[2] ; break;
  case 5: out[0] = hsv[2] ; out[1] = min    ; out[2] = dec    ; break;
  default:
    out[0] = 0.f; out[1] = 0.f; out[2] = 0.f;
  }
}



void na_ConvertHSVToHSL(float out[3], const float in[3]){
  out[0] = in[0];
  out[1] = in[1] * in[2];
  out[2] = in[2] - 0.5f * out[1];
}

void na_ConvertHSLToHSV(float out[3], const float in[3]){
  out[0] = in[0];
  out[2] = in[2] + .5f * in[1];
  if(out[2] == 0.f){
    out[1] = in[1] / NA_SINGULARITYf;
  }else{
    out[1] = in[1] / out[2];
  }
}



NABabyImage* na_CreateBlendedBabyImage(
  const float* base,
  NASizei baseSize,
  const float* top,
  NASizei topSize,
  NABlendMode mode,
  float blend,
  NAPosi offset)
{
  NABool baseIsImage = !naIsSizeiEmpty(baseSize);
  NABool topIsImage = !naIsSizeiEmpty(topSize);

  // First, define the return size and inner rect.
  NASizei retSize;
  NARecti innerRect;
  if(topIsImage && baseIsImage){
    retSize = baseSize;
    innerRect = naClampRectiToRect(naMakeRecti(offset, topSize), naMakeRecti(naMakePosiZero(), retSize));
    if(!naIsRectiUseful(innerRect)){innerRect = naMakeRectiZero();}
  }else if(topIsImage){
    retSize = topSize;
    innerRect = naMakeRecti(naMakePosiZero(), retSize);
  }else{
    retSize = baseSize;
    innerRect = naMakeRecti(naMakePosiZero(), retSize);
  }
  NAInt innerEndY = naIsRectiEmpty(innerRect) ? 0 : naGetRectiEndY(innerRect);
  NAInt innerEndX = naIsRectiEmpty(innerRect) ? 0 : naGetRectiEndX(innerRect);

  // Create the actual image to be filled.
  NABabyImage* retImage = naCreateBabyImage(retSize, NA_NULL);
  float* ret = naGetBabyImageData(retImage);

  // In case we have two images, fill up the trivial vertial parts.
  if(topIsImage && baseIsImage){
    // Simply copy the lower part of the base image
    NAInt bottomPixelCount = innerRect.pos.y;
    if(bottomPixelCount > 0){
      naCopyn(
        ret,
        base,
        bottomPixelCount * retSize.width * NA_BABY_COLOR_CHANNEL_COUNT * sizeof(float));
    }
    // Simply copy the upper part of the base image
    NAInt topPixelCount = naIsRectiEmpty(innerRect) ? baseSize.height : baseSize.height - innerEndY;
    if(topPixelCount > 0){
      naCopyn(
        &ret[(retSize.height - topPixelCount) * retSize.width * NA_BABY_COLOR_CHANNEL_COUNT],
        &base[(baseSize.height - topPixelCount) * baseSize.width * NA_BABY_COLOR_CHANNEL_COUNT],
        topPixelCount * retSize.width * NA_BABY_COLOR_CHANNEL_COUNT * sizeof(float));
    }
  }

  // Go through the inner part vertically.
  for(NAInt y = innerRect.pos.y; y < innerEndY; ++y){

    // In case we have two images, fill up the trivial horizontal parts.
    if(topIsImage && baseIsImage){
      // Simply copy the left part of the base image
      NAInt leftPixelCount = innerRect.pos.x;
      if(leftPixelCount > 0){
        if(leftPixelCount > baseSize.width){leftPixelCount = baseSize.width;}
        naCopyn(
          &ret[y * retSize.width * NA_BABY_COLOR_CHANNEL_COUNT],
          &base[y * baseSize.width * NA_BABY_COLOR_CHANNEL_COUNT],
          leftPixelCount * NA_BABY_COLOR_CHANNEL_COUNT * sizeof(float));
      }
      // Simply copy the right part of the base image
      NAInt rightPixelCount = baseSize.width - innerEndX;
      if(rightPixelCount > 0){
        if(rightPixelCount > baseSize.width){rightPixelCount = baseSize.width;}
        naCopyn(
          &ret[(y * retSize.width + retSize.width - rightPixelCount) * NA_BABY_COLOR_CHANNEL_COUNT],
          &base[(y * baseSize.width + baseSize.width - rightPixelCount) * NA_BABY_COLOR_CHANNEL_COUNT],
          rightPixelCount * NA_BABY_COLOR_CHANNEL_COUNT * sizeof(float));
      }
    }

    // Now for the actual interesting part of the image.

    // Define the source and destination pointers
    float* retPtr;    
    const float* basePtr;
    const float* topPtr;
    if(topIsImage && baseIsImage){
      retPtr = &ret[(y * retSize.width + innerRect.pos.x) * NA_BABY_COLOR_CHANNEL_COUNT];
      basePtr = &base[(y * baseSize.width + innerRect.pos.x) * NA_BABY_COLOR_CHANNEL_COUNT];
      NAInt offsetX = 0;
      NAInt offsetY = 0;
      if(offset.x < 0) {offsetX = -offset.x;}
      if(offset.y < 0) {offsetY = -offset.y;}
      topPtr = &top[(((y - innerRect.pos.y + offsetY) * topSize.width) + offsetX) * NA_BABY_COLOR_CHANNEL_COUNT];
    }else if(topIsImage){
      retPtr = &ret[(y * retSize.width) * NA_BABY_COLOR_CHANNEL_COUNT];
      basePtr = base;
      topPtr = &top[(y * topSize.width) * NA_BABY_COLOR_CHANNEL_COUNT];
    }else{
      retPtr = &ret[(y * retSize.width) * NA_BABY_COLOR_CHANNEL_COUNT];
      basePtr = &base[(y * baseSize.width) * NA_BABY_COLOR_CHANNEL_COUNT];
      topPtr = top;
    }

    for(NAInt x = innerRect.pos.x; x < innerEndX; ++x){

      float topBlend;
      float bottomBlend;
      float sumBlend;
      switch(mode){
      case NA_BLEND_ZERO:
        naCopyV4f(retPtr, basePtr);
        retPtr[0] = naUnlinearizeColorValue(retPtr[0]);
        retPtr[1] = naUnlinearizeColorValue(retPtr[1]);
        retPtr[2] = naUnlinearizeColorValue(retPtr[2]);
        break;
      case NA_BLEND:
        sumBlend = (1.f - blend) * basePtr[3] + blend * topPtr[3];
        if(sumBlend > NA_SINGULARITYf){
          bottomBlend = (1.f - blend) * basePtr[3] / sumBlend;
          topBlend = blend * topPtr[3] / sumBlend;
          naFillV4f(retPtr,
            bottomBlend * naUnlinearizeColorValue(basePtr[0]) + topBlend * naUnlinearizeColorValue(topPtr[0]),
            bottomBlend * naUnlinearizeColorValue(basePtr[1]) + topBlend * naUnlinearizeColorValue(topPtr[1]),
            bottomBlend * naUnlinearizeColorValue(basePtr[2]) + topBlend * naUnlinearizeColorValue(topPtr[2]),
            sumBlend);
        }else{
          naFillV4f(retPtr, 0., 0., 0., 0.);
        }
        break;
      case NA_BLEND_OVERLAY:
        sumBlend = basePtr[3] * (1.f - blend * topPtr[3]) + blend * topPtr[3];
        if(sumBlend > NA_SINGULARITYf){
          bottomBlend = (basePtr[3] * (1.f - blend * topPtr[3])) / sumBlend;
          topBlend = (blend * topPtr[3]) / sumBlend;
          naFillV4f(retPtr,
            bottomBlend * naUnlinearizeColorValue(basePtr[0]) + topBlend * naUnlinearizeColorValue(topPtr[0]),
            bottomBlend * naUnlinearizeColorValue(basePtr[1]) + topBlend * naUnlinearizeColorValue(topPtr[1]),
            bottomBlend * naUnlinearizeColorValue(basePtr[2]) + topBlend * naUnlinearizeColorValue(topPtr[2]),
            basePtr[3] + (1.f - basePtr[3]) * blend * topPtr[3]);
        }else{
          naFillV4f(retPtr, 0., 0., 0., 0.);
        }
        break;
      case NA_BLEND_OPAQUE:
        sumBlend = basePtr[3] * (1.f - blend * topPtr[3]) + blend * topPtr[3];
        if(sumBlend > NA_SINGULARITYf){
          bottomBlend = (basePtr[3] * (1.f - blend * topPtr[3])) / sumBlend;
          topBlend = (blend * topPtr[3]) / sumBlend;
          naFillV4f(retPtr,
            bottomBlend * naUnlinearizeColorValue(basePtr[0]) + topBlend * naUnlinearizeColorValue(topPtr[0]),
            bottomBlend * naUnlinearizeColorValue(basePtr[1]) + topBlend * naUnlinearizeColorValue(topPtr[1]),
            bottomBlend * naUnlinearizeColorValue(basePtr[2]) + topBlend * naUnlinearizeColorValue(topPtr[2]),
            basePtr[3]);
        }else{
          naFillV4f(retPtr, 0., 0., 0., 0.);
        }
        break;
      case NA_BLEND_BLACK_GREEN:
        topBlend = topPtr[3] * blend;
        naFillV4f(retPtr,
          (1.f - topBlend) * naUnlinearizeColorValue(basePtr[0]) + topBlend * naUnlinearizeColorValue(topPtr[0]),
          (1.f - topBlend) * naUnlinearizeColorValue(basePtr[1]) + topBlend * naUnlinearizeColorValue(topPtr[1]),
          (1.f - topBlend) * naUnlinearizeColorValue(basePtr[2]) + topBlend * naUnlinearizeColorValue(topPtr[2]),
          (1.f - basePtr[1]) * basePtr[3] * (1.f - (1.f - topPtr[3]) * blend));
        break;
      case NA_BLEND_WHITE_GREEN:
      topBlend = topPtr[3] * blend;
      naFillV4f(retPtr,
        (1.f - topBlend) * naUnlinearizeColorValue(basePtr[0]) + topBlend * naUnlinearizeColorValue(topPtr[0]),
        (1.f - topBlend) * naUnlinearizeColorValue(basePtr[1]) + topBlend * naUnlinearizeColorValue(topPtr[1]),
        (1.f - topBlend) * naUnlinearizeColorValue(basePtr[2]) + topBlend * naUnlinearizeColorValue(topPtr[2]),
        basePtr[1] * basePtr[3] * (1.f - (1.f - topPtr[3]) * blend));
      break;
        case NA_BLEND_MULTIPLY:
        topBlend = topPtr[3] * blend;
        naFillV4f(retPtr,
          (1.f - topBlend) * naUnlinearizeColorValue(basePtr[0]) + topBlend * naUnlinearizeColorValue(topPtr[0]) * naUnlinearizeColorValue(basePtr[0]),
          (1.f - topBlend) * naUnlinearizeColorValue(basePtr[1]) + topBlend * naUnlinearizeColorValue(topPtr[1]) * naUnlinearizeColorValue(basePtr[1]),
          (1.f - topBlend) * naUnlinearizeColorValue(basePtr[2]) + topBlend * naUnlinearizeColorValue(topPtr[2]) * naUnlinearizeColorValue(basePtr[2]),
          basePtr[3]);
        break;
      case NA_BLEND_ERASE_HUE:
        {
          float baseRGB[3];
          float baseHSV[3];
          float baseHSL[3];
          baseRGB[0] = naUnlinearizeColorValue(basePtr[0]);
          baseRGB[1] = naUnlinearizeColorValue(basePtr[1]);
          baseRGB[2] = naUnlinearizeColorValue(basePtr[2]);
          na_ConvertRGBToHSV(baseHSV, baseRGB);
          na_ConvertHSVToHSL(baseHSL, baseHSV);
          float topRGB[3];
          float topHSV[3];
          float topHSL[3];
          topRGB[0] = naUnlinearizeColorValue(topPtr[0]);
          topRGB[1] = naUnlinearizeColorValue(topPtr[1]);
          topRGB[2] = naUnlinearizeColorValue(topPtr[2]);
          na_ConvertRGBToHSV(topHSV, topRGB);
          na_ConvertHSVToHSL(topHSL, topHSV);

          if(baseHSV[1] != 1.)
          {
            int asdf = 1234;
          }
          float hDiff = topHSL[0] - baseHSL[0];
          if(hDiff > 180.f){hDiff -= 360.f;}
          else if(hDiff < -180.f){hDiff += 360.f;}

          if(hDiff < -60.f || hDiff > 60.f){
            // Not near the hue, leave the color as it is.
            naCopyV3f(retPtr, baseRGB);
            retPtr[3] = 1.;
          }else{
            if(hDiff < 0.){
              float factor = -hDiff / 60.f;
              baseHSL[0] = topHSL[0] + 60.f;
              if(baseHSL[0] > 360.f){baseHSL[0] -= 360.f;}
              retPtr[3] = 1.f - baseHSL[1];
              baseHSL[1] *= factor;
              na_ConvertHSLToHSV(baseHSV, baseHSL);
              na_ConvertHSVToRGB(retPtr, baseHSV);
            }else{
              float factor = hDiff / 60.f;
              baseHSL[0] = topHSL[0] - 60.f + 360.f;
              if(baseHSL[0] > 360.f){baseHSL[0] -= 360.f;}
              retPtr[3] = 1.f - baseHSL[1];
              baseHSL[1] *= factor;
              na_ConvertHSLToHSV(baseHSV, baseHSL);
              na_ConvertHSVToRGB(retPtr, baseHSV);
            }
          }
        }
        break;
      }

      if(retPtr[3] == 0.f){
        naFillV3f(retPtr, 0.f, 0.f, 0.f);
      }
      retPtr[0] = naLinearizeColorValue(retPtr[0]);
      retPtr[1] = naLinearizeColorValue(retPtr[1]);
      retPtr[2] = naLinearizeColorValue(retPtr[2]);

      retPtr += NA_BABY_COLOR_CHANNEL_COUNT;
      if(baseIsImage){basePtr += NA_BABY_COLOR_CHANNEL_COUNT;}
      if(topIsImage){topPtr += NA_BABY_COLOR_CHANNEL_COUNT;}
    }
  }

  return retImage;
}


NA_DEF NABabyImage* naCreateBabyImageWithTint(
  const NABabyImage* base,
  const NABabyColor tint,
  NABlendMode mode,
  float blend){
  #if NA_DEBUG
    if(!base)
      naCrash("Given base image is a Null-Pointer");
    if(!tint)
      naCrash("tint is Null");
    if(!naIsBabyColorSecure(tint))
      naError("insecure tint color given");
  #endif
  
  return na_CreateBlendedBabyImage(
    base->data,
    naGetBabyImageSize(base),
    tint,
    naMakeSizeiEmpty(),
    mode,
    blend,
    naMakePosi(0, 0));
}



NA_DEF NABabyImage* naCreateBabyImageWithBlend(
  const NABabyImage* base,
  const NABabyImage* top,
  NABlendMode mode,
  float blend,
  NAPosi offset)
{
  #if NA_DEBUG
  if(!base)
    naCrash("base is Null");
  if(!top)
    naCrash("top is Null");
  #endif

  return na_CreateBlendedBabyImage(
    base->data,
    naGetBabyImageSize(base),
    top->data,
    naGetBabyImageSize(top),
    mode,
    blend,
    offset);
}



NA_DEF NABabyImage* naCreateBabyImageWithApply(const NABabyColor ground, const NABabyImage* top, NABlendMode mode, float blend){
  #if NA_DEBUG
  if(!ground)
    naCrash("ground is Null");
  if(!top)
    naCrash("top is Null");
  if(!naIsBabyColorSecure(ground))
    naError("insecure ground color given");
  #endif

  const float* topptr = top->data;
  return na_CreateBlendedBabyImage(
    ground,
    naMakeSizeiEmpty(),
    topptr,
    naGetBabyImageSize(top),
    mode,
    blend,
    naMakePosi(0, 0));
}



NA_DEF NABabyImage* naCreateBabyImageWithHalfSize(const NABabyImage* image){
  NASizei halfSize;
  NAInt x, y;
  NABabyImage* outImage;
  NAInt valuesPerLine;
  float* inPtr1;
  float* inPtr2;
  float* outDataPtr;
  
  #if NA_DEBUG
    if((image->width % 2) || (image->height % 2))
      naError("Width or height not divisible by 2");
  #endif
  halfSize = naMakeSizei(image->width / 2, image->height / 2);

  outImage = naCreateBabyImage(halfSize, NA_NULL);
  valuesPerLine = naGetBabyImageValuesPerLine(image);
  
  inPtr1 = image->data;
  inPtr2 = image->data + valuesPerLine;
  outDataPtr = outImage->data;
  for(y = 0; y < image->height; y += 2){
    for(x = 0; x < image->width; x += 2){
      outDataPtr[0] = inPtr1[0] * inPtr1[3] + inPtr1[4] * inPtr1[7];
      outDataPtr[1] = inPtr1[1] * inPtr1[3] + inPtr1[5] * inPtr1[7];
      outDataPtr[2] = inPtr1[2] * inPtr1[3] + inPtr1[6] * inPtr1[7];
      outDataPtr[3] = inPtr1[3] + inPtr1[7];
      inPtr1 += 8;
      outDataPtr[0] += inPtr2[0] * inPtr2[3] + inPtr2[4] * inPtr2[7];
      outDataPtr[1] += inPtr2[1] * inPtr2[3] + inPtr2[5] * inPtr2[7];
      outDataPtr[2] += inPtr2[2] * inPtr2[3] + inPtr2[6] * inPtr2[7];
      outDataPtr[3] += inPtr2[3] + inPtr2[7];
      inPtr2 += 8;
      if(outDataPtr[3] != 0.f){
        float invweight = naInvf(outDataPtr[3]);
        outDataPtr[0] *= invweight;
        outDataPtr[1] *= invweight;
        outDataPtr[2] *= invweight;
        outDataPtr[3] *= .25f;
      }
      outDataPtr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
    // Each line has advanced till the last pixel of the line, so we only
    // have to overjump one more line.
    inPtr1 += valuesPerLine;
    inPtr2 += valuesPerLine;
  }
  return outImage;
}


NA_HDEF void naAccumulateResizeLine(float* out, const float* in, int32 outY, int32 inY, int32 outWidth, int32 inWidth, float factorY){
  float* outPtr = &out[outY * outWidth * NA_BABY_COLOR_CHANNEL_COUNT];
  const float* inPtr = &in[inY * inWidth * NA_BABY_COLOR_CHANNEL_COUNT];

  int32 inX = 0;
  float subX = 0.f;
  float factorX = (float)inWidth / (float)outWidth;
  float remainerX = factorX;
  for(int32 outX = 0; outX < outWidth; outX += 1){
    float counterSubX = 1.f - subX;
    while(counterSubX <= remainerX){
      float accumulateFactor = factorY * counterSubX;
      outPtr[0] += accumulateFactor * inPtr[0];
      outPtr[1] += accumulateFactor * inPtr[1];
      outPtr[2] += accumulateFactor * inPtr[2];
      outPtr[3] += accumulateFactor * inPtr[3];
      remainerX -= counterSubX;
      inPtr += NA_BABY_COLOR_CHANNEL_COUNT;
      inX++;
      counterSubX = 1.f;
    }
    subX = 1.f - counterSubX;
    if(inX < inWidth){
      float accumulateFactor = factorY * remainerX;
      outPtr[0] += accumulateFactor * inPtr[0];
      outPtr[1] += accumulateFactor * inPtr[1];
      outPtr[2] += accumulateFactor * inPtr[2];
      outPtr[3] += accumulateFactor * inPtr[3];
    }
    subX += remainerX;
    outPtr += NA_BABY_COLOR_CHANNEL_COUNT;
    remainerX = factorX;
    remainerX = (outX + 2) * factorX - (inX + subX);
  }
}



NA_DEF NABabyImage* naCreateBabyImageWithResize(const NABabyImage* image, NASizei newSize){
  NABabyImage* outImage;
  NAInt valuesPerLine;
  
  #if NA_DEBUG
    if(!naIsSizeiUseful(newSize))
      naError("Given size is not useful");
  #endif

  NABabyColor blank = {0.f, 0.f, 0.f, 0.f};
  outImage = naCreateBabyImage(newSize, blank);
  valuesPerLine = naGetBabyImageValuesPerLine(image);
  
  int32 inY = 0;
  float subY = 0.f;
  float factorY = (float)image->height / (float)newSize.height;
  float remainerY = factorY;
  for(int32 outY = 0; outY < newSize.height; outY += 1){
    float counterSubY = 1.f - subY;
    while(counterSubY <= remainerY){
      naAccumulateResizeLine(outImage->data, image->data, outY, inY, (int32)newSize.width, image->width, counterSubY);
      remainerY -= counterSubY;
      inY++;
      counterSubY = 1.f;
    }
    subY = 1.f - counterSubY;
    if(inY < image->height){
      naAccumulateResizeLine(outImage->data, image->data, outY, inY, (int32)newSize.width, image->width, remainerY);
    }
    subY += remainerY;
    remainerY = factorY;
    remainerY = (outY + 2) * factorY - (inY + subY);
  }

  // Normalize the whole output image;
  float divisor = (1.f / factorY) * (float)newSize.width / (float)image->width;
  float* outPtr = outImage->data;
  for(uint32 i = 0; i < (uint32)(newSize.width * newSize.height) * NA_BABY_COLOR_CHANNEL_COUNT; i += 1){
    *outPtr++ *= divisor;
  }


  return outImage;
}



NA_HDEF void na_DestroyBabyImage(NABabyImage* image){
  naFree(image->data);
  naFree(image);
}

NA_API NABabyImage* naRetainBabyImage(const NABabyImage* image){
  NABabyImage* mutableImage = (NABabyImage*)image; 
  return (NABabyImage*)naRetainRefCount(&mutableImage->refCount);
}

NA_DEF void naReleaseBabyImage(const NABabyImage* image){
  NABabyImage* mutableImage = (NABabyImage*)image; 
  naReleaseRefCount(&mutableImage->refCount, mutableImage, (NAMutator)na_DestroyBabyImage);
}



NA_DEF void naFillBabyImageWithu8(NABabyImage* image, const void* data, NABool topToBottom, NAColorBufferType bufferType){
  float* imgptr = image->data;
  const uint8* u8ptr;

  if(topToBottom){
    NAInt x, y;
    NASizei size = naGetBabyImageSize(image);
    for(y = 0; y < size.height; y++){
      u8ptr = &(((uint8*)data)[(size.height - y - 1) * naGetBabyImageValuesPerLine(image)]);
      for(x = 0; x < size.width; x++){
        naFillBabyColorWithu8(imgptr, u8ptr, bufferType);
        imgptr += NA_BABY_COLOR_CHANNEL_COUNT;
        u8ptr += NA_BABY_COLOR_CHANNEL_COUNT;
      }
    }
  }else{
    NAInt pixelCount;
    NAInt i;
    u8ptr = data;
    pixelCount = na_GetBabyImagePixelCount(image);
    for(i = 0; i < pixelCount; ++i){
      naFillBabyColorWithu8(imgptr, u8ptr, bufferType);
      imgptr += NA_BABY_COLOR_CHANNEL_COUNT;
      u8ptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
  }
}



NA_DEF void naConvertBabyImageTou8(const NABabyImage* image, void* data, NABool topToBottom, NAColorBufferType bufferType){
  const float* imgptr = image->data;
  uint8* u8ptr;

  if(topToBottom){
    NAInt x, y;
    NASizei size = naGetBabyImageSize(image);
    for(y = 0; y < size.height; y++){
      u8ptr = &(((uint8*)data)[(size.height - y - 1) * naGetBabyImageValuesPerLine(image)]);
      for(x = 0; x < size.width; x++){
        naFillu8WithBabyColor(u8ptr, imgptr, bufferType);
        imgptr += NA_BABY_COLOR_CHANNEL_COUNT;
        u8ptr += NA_BABY_COLOR_CHANNEL_COUNT;
      }
    }
  }else{
    NAInt pixelCount;
    NAInt i;
    u8ptr = data;
    pixelCount = na_GetBabyImagePixelCount(image);
    for(i = 0; i < pixelCount; ++i){
      naFillu8WithBabyColor(u8ptr, imgptr, bufferType);
      imgptr += NA_BABY_COLOR_CHANNEL_COUNT;
      u8ptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
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
