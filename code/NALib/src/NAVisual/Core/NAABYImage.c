
#include "../NAABYColor.h"
#include "../NAABYImage.h"
#include "../../NAMath/NAVectorAlgebra.h"


struct NAABYImage{
  NARefCount refCount;
  uint32 width;
  uint32 height;
  NAABYColor* data;
};




NA_HIDEF size_t na_GetABYImagePixelCount(const NAABYImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (size_t)image->width * (size_t)image->height;
}



NA_HIDEF size_t na_GetABYImageDataSize(const NAABYImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return na_GetABYImagePixelCount(image) * sizeof(NAABYColor);
}



NA_DEF NAABYColor* naGetBabyImageData(const NAABYImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return image->data;
}



NA_DEF NASizei naGetBabyImageSize(const NAABYImage* image){
  #if NA_DEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return naMakeSizei((NAInt)image->width, (NAInt)image->height);
}



NA_DEF NAABYImage* naCreateBabyImage(NASizei size, const NAABYColor* color){
  NAABYImage* image;
  #if NA_DEBUG
    if(size.width <= 0 || size.height <= 0)
      naError("size must be > 0");
    if(size.width > NA_MAX_i32 || size.height > NA_MAX_i32)
      naError("size is too big");
    if(color && !naIsABYColorUseful(color))
      naError("color is not useful");
  #endif
  image = naAlloc(NAABYImage);
  naInitRefCount(&image->refCount);
  image->width = (int32)size.width;
  image->height = (int32)size.height;
  image->data = naMalloc(na_GetABYImageDataSize(image));
  if(color){
    size_t pixelCount = na_GetABYImagePixelCount(image);
    NAABYColor* ptr = image->data;
    for(size_t i = 0; i < pixelCount; ++i){
      naCopyABYColor(ptr, color);
      ptr += 1;
    }
  }
  return image;
}



NA_HDEF NAABYImage* naCreateBabyImageCopy(const NAABYImage* image){
  NAABYImage* newImage;
  #if NA_DEBUG
  if(!image)
    naCrash("given image is a Null pointer");
  #endif
  newImage = naAlloc(NAABYImage);
  naInitRefCount(&newImage->refCount);
  newImage->width = image->width;
  newImage->height = image->height;
  size_t dataSize = na_GetABYImageDataSize(newImage);
  newImage->data = naMalloc(dataSize);
  naCopyn(newImage->data, image->data, dataSize);
  return newImage;
}








NAABYImage* na_CreateBlendedBabyImage(
  const NAABYColor* base,
  NASizei baseSize,
  const NAABYColor* top,
  NASizei topSize,
  NABlendMode mode,
  float factor,
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
  NAABYImage* retImage = naCreateBabyImage(retSize, NA_NULL);
  NAABYColor* ret = naGetBabyImageData(retImage);

  // In case we have two images, fill up the trivial vertical parts.
  if(topIsImage && baseIsImage){
    // Simply copy the lower part of the base image
    NAInt basePixelCount = innerRect.pos.y;
    if(basePixelCount > 0){
      naCopyn(
        ret,
        base,
        (size_t)basePixelCount * (size_t)retSize.width * sizeof(NAABYColor));
    }
    // Simply copy the upper part of the base image
    NAInt topPixelCount = naIsRectiEmpty(innerRect) ? baseSize.height : baseSize.height - innerEndY;
    if(topPixelCount > 0){
      naCopyn(
        &ret[(retSize.height - topPixelCount) * retSize.width],
        &base[(baseSize.height - topPixelCount) * baseSize.width],
        (size_t)topPixelCount * (size_t)retSize.width * sizeof(NAABYColor));
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
          &ret[y * retSize.width],
          &base[y * baseSize.width],
          (size_t)leftPixelCount * sizeof(NAABYColor));
      }
      // Simply copy the right part of the base image
      NAInt rightPixelCount = baseSize.width - innerEndX;
      if(rightPixelCount > 0){
        if(rightPixelCount > baseSize.width){rightPixelCount = baseSize.width;}
        naCopyn(
          &ret[(y * retSize.width + retSize.width - rightPixelCount)],
          &base[(y * baseSize.width + baseSize.width - rightPixelCount)],
          (size_t)rightPixelCount * sizeof(NAABYColor));
      }
    }

    // Now for the actual interesting part of the image.

    // Define the source and destination pointers
    NAABYColor* retPtr;    
    const NAABYColor* basePtr;
    const NAABYColor* topPtr;
    if(topIsImage && baseIsImage){
      retPtr = &ret[(y * retSize.width + innerRect.pos.x)];
      basePtr = &base[(y * baseSize.width + innerRect.pos.x)];
      NAInt offsetX = 0;
      NAInt offsetY = 0;
      if(offset.x < 0) {offsetX = -offset.x;}
      if(offset.y < 0) {offsetY = -offset.y;}
      topPtr = &top[(((y - innerRect.pos.y + offsetY) * topSize.width) + offsetX)];
    }else if(topIsImage){
      retPtr = &ret[(y * retSize.width)];
      basePtr = base;
      topPtr = &top[(y * topSize.width)];
    }else{
      retPtr = &ret[(y * retSize.width)];
      basePtr = &base[(y * baseSize.width)];
      topPtr = top;
    }

    naBlendABYColors(
      retPtr,
      basePtr,
      topPtr,
      factor,
      mode,
      innerEndX - innerRect.pos.x,
      baseIsImage,
      topIsImage);
  }

  return retImage;
}


NA_DEF NAABYImage* naCreateBabyImageWithTint(
  const NAABYImage* base,
  const NAABYColor* tint,
  NABlendMode mode,
  float blend){
  #if NA_DEBUG
    if(!base)
      naCrash("Given base image is a Null-Pointer");
    if(!tint)
      naCrash("tint is Null");
    if(!naIsABYColorUseful(tint))
      naError("tint color is not useful");
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



NA_DEF NAABYImage* naCreateBabyImageWithBlend(
  const NAABYImage* base,
  const NAABYImage* top,
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



NA_DEF NAABYImage* naCreateBabyImageWithApply(const NAABYColor* ground, const NAABYImage* top, NABlendMode mode, float blend){
  #if NA_DEBUG
  if(!ground)
    naCrash("ground is Null");
  if(!top)
    naCrash("top is Null");
  if(!naIsABYColorUseful(ground))
    naError("ground color is not useful");
  #endif

  const NAABYColor* topPtr = top->data;
  return na_CreateBlendedBabyImage(
    ground,
    naMakeSizeiEmpty(),
    topPtr,
    naGetBabyImageSize(top),
    mode,
    blend,
    naMakePosi(0, 0));
}



NA_DEF NAABYImage* naCreateBabyImageWithHalfSize(const NAABYImage* image){
  NASizei halfSize;
  NAInt x, y;
  NAABYImage* outImage;
  NAABYColor* inPtr1;
  NAABYColor* inPtr2;
  NAABYColor* inPtr3;
  NAABYColor* inPtr4;
  NAABYColor* outDataPtr;
  
  #if NA_DEBUG
    if((image->width % 2) || (image->height % 2))
      naError("Width or height not divisible by 2");
  #endif
  halfSize = naMakeSizei(image->width / 2, image->height / 2);

  outImage = naCreateBabyImage(halfSize, NA_NULL);
  
  inPtr1 = image->data;
  inPtr2 = inPtr1 + 1;
  inPtr3 = image->data + image->width;
  inPtr4 = inPtr3 + 1;
  outDataPtr = outImage->data;
  for(y = 0; y < image->height; y += 2){
    for(x = 0; x < image->width; x += 2){
      outDataPtr->a = inPtr1->a * inPtr1->alpha + inPtr2->a * inPtr2->alpha;
      outDataPtr->b = inPtr1->b * inPtr1->alpha + inPtr2->b * inPtr2->alpha;
      outDataPtr->y = inPtr1->y * inPtr1->alpha + inPtr2->y * inPtr2->alpha;
      outDataPtr->alpha = inPtr1->alpha + inPtr2->alpha;
      inPtr1 += 2;
      outDataPtr->a += inPtr3->a * inPtr3->alpha + inPtr4->a * inPtr4->alpha;
      outDataPtr->b += inPtr3->b * inPtr3->alpha + inPtr4->b * inPtr4->alpha;
      outDataPtr->y += inPtr3->y * inPtr3->alpha + inPtr4->y * inPtr4->alpha;
      outDataPtr->alpha += inPtr3->alpha + inPtr4->alpha;
      inPtr3 += 2;
      if(outDataPtr->alpha > NA_SINGULARITYf){
        float invweight = naInvf(outDataPtr->alpha);
        outDataPtr->a *= invweight;
        outDataPtr->b *= invweight;
        outDataPtr->y *= invweight;
        outDataPtr->alpha *= .25f;
      }else{
        outDataPtr->a = 0.f;
        outDataPtr->b = 0.f;
        outDataPtr->y = 0.f;
      }
      outDataPtr += NA_ABY_COLOR_CHANNEL_COUNT;
    }
    // Each line has advanced till the last pixel of the line, so we only
    // have to overjump one more line.
    inPtr1 += image->width;
    inPtr3 += image->width;
  }
  return outImage;
}


NA_HDEF void naAccumulateResizeLine(NAABYColor* out, const NAABYColor* in, int32 outY, int32 inY, int32 outWidth, int32 inWidth, float factorY){
  NAABYColor* outPtr = &out[outY * outWidth];
  const NAABYColor* inPtr = &in[inY * inWidth];

  int32 inX = 0;
  float subX = 0.f;
  float factorX = (float)inWidth / (float)outWidth;
  float remainerX = factorX;
  for(int32 outX = 0; outX < outWidth; outX += 1){
    float counterSubX = 1.f - subX;
    while(counterSubX <= remainerX){
      float accumulateFactor = factorY * counterSubX;
      outPtr->a += accumulateFactor * inPtr->a;
      outPtr->b += accumulateFactor * inPtr->b;
      outPtr->y += accumulateFactor * inPtr->y;
      outPtr->alpha += accumulateFactor * inPtr->alpha;
      remainerX -= counterSubX;
      inPtr += 1;
      inX++;
      counterSubX = 1.f;
    }
    subX = 1.f - counterSubX;
    if(inX < inWidth){
      float accumulateFactor = factorY * remainerX;
      outPtr->a += accumulateFactor * inPtr->a;
      outPtr->b += accumulateFactor * inPtr->b;
      outPtr->y += accumulateFactor * inPtr->y;
      outPtr->alpha += accumulateFactor * inPtr->alpha;
    }
    subX += remainerX;
    outPtr += 1;
    remainerX = factorX;
    remainerX = (outX + 2) * factorX - (inX + subX);
  }
}



NA_DEF NAABYImage* naCreateBabyImageWithResize(const NAABYImage* image, NASizei newSize){
  NAABYImage* outImage;
  NAInt valuesPerLine;
  
  #if NA_DEBUG
    if(!naIsSizeiUseful(newSize))
      naError("Given size is not useful");
  #endif

  NAABYColor blank = {0.f, 0.f, 0.f, 0.f};
  outImage = naCreateBabyImage(newSize, &blank);
  valuesPerLine = naGetBabyImageSize(image).width * NA_ABY_COLOR_CHANNEL_COUNT;
  
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
  NAABYColor* outPtr = outImage->data;
  for(uint32 i = 0; i < (uint32)(newSize.width * newSize.height); i += 1){
    outPtr->a *= divisor;
    outPtr->b *= divisor;
    outPtr->y *= divisor;
    outPtr->alpha *= divisor;
    outPtr += 1;
  }

  return outImage;
}



NA_HDEF void na_DestroyBabyImage(NAABYImage* image){
  naFree(image->data);
  naFree(image);
}

NA_API NAABYImage* naRetainBabyImage(const NAABYImage* image){
  NAABYImage* mutableImage = (NAABYImage*)image; 
  return (NAABYImage*)naRetainRefCount(&mutableImage->refCount);
}

NA_DEF void naReleaseBabyImage(const NAABYImage* image){
  NAABYImage* mutableImage = (NAABYImage*)image; 
  naReleaseRefCount(&mutableImage->refCount, mutableImage, (NAMutator)na_DestroyBabyImage);
}



NA_DEF void naFillBabyImageWithu8(NAABYImage* image, const void* data, NABool topToBottom, NAColorBufferType bufferType){
  NAABYColor* imgPtr = image->data;
  const uint8* u8Ptr;

  if(topToBottom){
    NAInt x, y;
    NASizei size = naGetBabyImageSize(image);
    for(y = 0; y < size.height; y++){
      u8Ptr = &(((uint8*)data)[(size.height - y - 1) * naGetBabyImageSize(image).width * NA_ABY_COLOR_CHANNEL_COUNT]);
      for(x = 0; x < size.width; x++){
        naFillABYColorWithSRGBu8(imgPtr, u8Ptr, bufferType);
        imgPtr += 1;
        u8Ptr += NA_ABY_COLOR_CHANNEL_COUNT;
      }
    }
  }else{
    u8Ptr = data;
    size_t pixelCount = na_GetABYImagePixelCount(image);
    for(size_t i = 0; i < pixelCount; ++i){
      naFillABYColorWithSRGBu8(imgPtr, u8Ptr, bufferType);
      imgPtr += 1;
      u8Ptr += NA_ABY_COLOR_CHANNEL_COUNT;
    }
  }
}



NA_DEF void naConvertBabyImageTou8(const NAABYImage* image, void* data, NABool topToBottom, NAColorBufferType bufferType){
  const NAABYColor* imgPtr = image->data;
  uint8* u8Ptr;

  if(topToBottom){
    NAInt x, y;
    NASizei size = naGetBabyImageSize(image);
    for(y = 0; y < size.height; y++){
      u8Ptr = &(((uint8*)data)[(size.height - y - 1) * naGetBabyImageSize(image).width * NA_ABY_COLOR_CHANNEL_COUNT]);
      for(x = 0; x < size.width; x++){
        naFillSRGBu8WithABYColor(u8Ptr, imgPtr, bufferType);
        imgPtr += 1;
        u8Ptr += NA_ABY_COLOR_CHANNEL_COUNT;
      }
    }
  }else{
    u8Ptr = data;
    size_t pixelCount = na_GetABYImagePixelCount(image);
    for(size_t i = 0; i < pixelCount; ++i){
      naFillSRGBu8WithABYColor(u8Ptr, imgPtr, bufferType);
      imgPtr += 1;
      u8Ptr += NA_ABY_COLOR_CHANNEL_COUNT;
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
