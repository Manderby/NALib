
#include "../NAColor.h"
#include "../NAImage.h"
#include "../../NAMath/NAVectorAlgebra.h"


struct NAImage{
  NARefCount refCount;
  uint32 width;
  uint32 height;
  NAColor* data;
};

#if NA_SIZE_T_BITS < 32
  #error "Size of size_t is too small for the NAImage struct"
#endif


NA_HIDEF size_t na_GetImagePixelCount(const NAImage* image) {
#if NA_DEBUG
  if(!image)
    naCrash("Given image is a Null-Pointer");
#endif
  return (size_t)image->width * (size_t)image->height;
}



NA_HIDEF size_t na_GetImageDataSize(const NAImage* image) {
#if NA_DEBUG
  if(!image)
    naCrash("Given image is a Null-Pointer");
#endif
  return na_GetImagePixelCount(image) * sizeof(NAColor);
}



NA_DEF NAColor* naGetImageData(const NAImage* image) {
#if NA_DEBUG
  if(!image)
    naCrash("Given image is a Null-Pointer");
#endif
  return image->data;
}



NA_DEF NASizes naGetImageSize(const NAImage* image) {
#if NA_DEBUG
  if(!image)
    naCrash("Given image is a Null-Pointer");
#endif
  return naMakeSizes(image->width, image->height);
}



NA_DEF NAImage* naCreateImage(NASizes size, const NAColor* color) {
  NAImage* image;
#if NA_DEBUG
  if(size.width <= 0 || size.height <= 0)
    naError("size must be > 0");
  if(size.width > NA_MAX_i32 || size.height > NA_MAX_i32)
    naError("size is too big");
#endif
  image = naAlloc(NAImage);
  naInitRefCount(&image->refCount);
  image->width = (uint32)size.width;
  image->height = (uint32)size.height;
  image->data = naMalloc(na_GetImageDataSize(image));
  if(color) {
    size_t pixelCount = na_GetImagePixelCount(image);
    NAColor* ptr = image->data;
    for(size_t i = 0; i < pixelCount; ++i) {
      naFillColorWithCopy(ptr, color);
      ptr += 1;
    }
  }
  return image;
}



NA_HDEF NAImage* naCreateImageCopy(const NAImage* image) {
  NAImage* newImage;
#if NA_DEBUG
  if(!image)
    naCrash("given image is a Null pointer");
#endif
  newImage = naAlloc(NAImage);
  naInitRefCount(&newImage->refCount);
  newImage->width = image->width;
  newImage->height = image->height;
  size_t dataSize = na_GetImageDataSize(newImage);
  newImage->data = naMalloc(dataSize);
  naCopyn(newImage->data, image->data, dataSize);
  return newImage;
}








NAImage* na_CreateBlendedImage(
  const NAColor* base,
  NASizes baseSize,
  const NAColor* top,
  NASizes topSize,
  NABlendMode mode,
  float factor,
  NAPosi32 offset)
{
  NABool baseIsImage = !naIsSizesEmpty(baseSize);
  NABool topIsImage = !naIsSizesEmpty(topSize);
  
  // First, define the return size and inner rect.
  NASizei32 retSize;
  NARecti32 innerRect;
  if(topIsImage && baseIsImage) {
    retSize = naMakeSizei32((int32)baseSize.width, (int32)baseSize.height);
    innerRect = naClampRecti32(naMakeRecti32(offset, naMakeSizei32((int32)topSize.width, (int32)topSize.height)), naMakeRecti32(naMakePosi32Zero(), retSize));
    if(!naIsRecti32Useful(innerRect)) {
      innerRect = naMakeRecti32Zero();
    }
  }else if(topIsImage) {
    retSize = naMakeSizei32((int32)topSize.width, (int32)topSize.height);
    innerRect = naMakeRecti32(naMakePosi32Zero(), retSize);
  }else{
    retSize = naMakeSizei32((int32)baseSize.width, (int32)baseSize.height);
    innerRect = naMakeRecti32(naMakePosi32Zero(), retSize);
  }
  int32 innerEndY = naIsRecti32Empty(innerRect) ? 0 : naGetRecti32EndY(innerRect);
  int32 innerEndX = naIsRecti32Empty(innerRect) ? 0 : naGetRecti32EndX(innerRect);
  
  // Create the actual image to be filled.
  NAImage* retImage = naCreateImage(naMakeSizes((size_t)retSize.width, (size_t)retSize.height), NA_NULL);
  NAColor* ret = naGetImageData(retImage);
  
  // In case we have two images, fill up the trivial vertical parts.
  if(topIsImage && baseIsImage) {
    // Simply copy the lower part of the base image
    NAInt basePixelCount = innerRect.pos.y;
    if(basePixelCount > 0) {
      naCopyn(
        ret,
        base,
        (size_t)basePixelCount * (size_t)retSize.width * sizeof(NAColor));
    }
    // Simply copy the upper part of the base image
    int32 topPixelCount = naIsRecti32Empty(innerRect) ? (int32)baseSize.height : (int32)baseSize.height - innerEndY;
    if(topPixelCount > 0) {
      naCopyn(
        &ret[(retSize.height - topPixelCount) * retSize.width],
        &base[(baseSize.height - (size_t)topPixelCount) * baseSize.width],
        (size_t)topPixelCount * (size_t)retSize.width * sizeof(NAColor));
    }
  }
  
  // Go through the inner part vertically.
  for(int32 y = innerRect.pos.y; y < innerEndY; ++y) {
    
    // In case we have two images, fill up the trivial horizontal parts.
    if(topIsImage && baseIsImage) {
      // Simply copy the left part of the base image
      int32 leftPixelCount = innerRect.pos.x;
      if(leftPixelCount > 0) {
        if(leftPixelCount > (int32)baseSize.width) {
          leftPixelCount = (int32)baseSize.width;
        }
        naCopyn(
          &ret[y * retSize.width],
          &base[(size_t)y * baseSize.width],
          (size_t)leftPixelCount * sizeof(NAColor));
      }
      // Simply copy the right part of the base image
      int32 rightPixelCount = (int32)baseSize.width - innerEndX;
      if(rightPixelCount > 0) {
        if(rightPixelCount > (int32)baseSize.width) {
          rightPixelCount = (int32)baseSize.width;
        }
        naCopyn(
          &ret[(y * retSize.width + retSize.width - rightPixelCount)],
          &base[((size_t)y * baseSize.width + baseSize.width - (size_t)rightPixelCount)],
          (size_t)rightPixelCount * sizeof(NAColor));
      }
    }
    
    // Now for the actual interesting part of the image.
    
    // Define the source and destination pointers
    NAColor* retPtr;    
    const NAColor* basePtr;
    const NAColor* topPtr;
    if(topIsImage && baseIsImage) {
      retPtr = &ret[(y * retSize.width + innerRect.pos.x)];
      basePtr = &base[((size_t)y * baseSize.width + (size_t)innerRect.pos.x)];
      size_t offsetX = 0;
      size_t offsetY = 0;
      if(offset.x < 0) { offsetX = (size_t)-offset.x; }
      if(offset.y < 0) { offsetY = (size_t)-offset.y; }
      topPtr = &top[((((size_t)(y - innerRect.pos.y) + offsetY) * topSize.width) + offsetX)];
    }else if(topIsImage) {
      retPtr = &ret[((size_t)y * (size_t)retSize.width)];
      basePtr = base;
      topPtr = &top[((size_t)y * topSize.width)];
    }else{
      retPtr = &ret[((size_t)y * (size_t)retSize.width)];
      basePtr = &base[((size_t)y * baseSize.width)];
      topPtr = top;
    }
    
    naBlendColors(
      retPtr,
      basePtr,
      topPtr,
      factor,
      mode,
      (size_t)(innerEndX - innerRect.pos.x),
      baseIsImage,
      topIsImage);
  }
  
  return retImage;
}


NA_DEF NAImage* naCreateImageWithTint(
  const NAImage* base,
  const NAColor* tint,
  NABlendMode mode,
  float factor)
{
#if NA_DEBUG
  if(!base)
    naCrash("Given base image is a Null-Pointer");
  if(!tint)
    naCrash("tint is Null");
#endif
  
  return na_CreateBlendedImage(
    base->data,
    naGetImageSize(base),
    tint,
    naMakeSizesZero(),
    mode,
    factor,
    naMakePosi32(0, 0));
}



NA_DEF NAImage* naCreateImageWithBlend(
  const NAImage* base,
  const NAImage* top,
  NABlendMode mode,
  float factor,
  NAPosi32 offset)
{
#if NA_DEBUG
  if(!base)
    naCrash("base is Null");
  if(!top)
    naCrash("top is Null");
#endif
  
  return na_CreateBlendedImage(
    base->data,
    naGetImageSize(base),
    top->data,
    naGetImageSize(top),
    mode,
    factor,
    offset);
}



NA_DEF NAImage* naCreateImageWithApply(const NAColor* ground, const NAImage* top, NABlendMode mode, float factor) {
#if NA_DEBUG
  if(!ground)
    naCrash("ground is Null");
  if(!top)
    naCrash("top is Null");
#endif
  
  const NAColor* topPtr = top->data;
  return na_CreateBlendedImage(
    ground,
    naMakeSizesZero(),
    topPtr,
    naGetImageSize(top),
    mode,
    factor,
    naMakePosi32(0, 0));
}



NA_DEF NAImage* naCreateImageWithHalfSize(const NAImage* image) {
  NASizes halfSize;
  NAInt x, y;
  NAImage* outImage;
  NAColor* inPtr1;
  NAColor* inPtr2;
  NAColor* inPtr3;
  NAColor* inPtr4;
  NAColor* outDataPtr;
  
#if NA_DEBUG
  if((image->width % 2) || (image->height % 2))
    naError("Width or height not divisible by 2");
#endif

  halfSize = naMakeSizes(image->width / 2, image->height / 2);
  
  outImage = naCreateImage(halfSize, NA_NULL);
  
  // Create radiometric copy of image.
  NAImage* radioImage = naCreateImageCopy(image);
  size_t pixelCount = na_GetImagePixelCount(radioImage);
  NAColor* inPtr = image->data;
  NAColor* radioPtr = radioImage->data;
  for(size_t i = 0; i < pixelCount; ++i) {
    naFillColorWithCopy(radioPtr, inPtr);
    na_ConvertToRadiometricRGB(radioPtr, radioPtr);
    radioPtr++;
    inPtr++;
  }
  
  inPtr1 = radioImage->data;
  inPtr2 = inPtr1 + 1;
  inPtr3 = radioImage->data + radioImage->width;
  inPtr4 = inPtr3 + 1;
  outDataPtr = outImage->data;
  for(y = 0; y < radioImage->height; y += 2) {
    for(x = 0; x < radioImage->width; x += 2) {
      outDataPtr->a = inPtr1->a * inPtr1->alpha + inPtr2->a * inPtr2->alpha;
      outDataPtr->b = inPtr1->b * inPtr1->alpha + inPtr2->b * inPtr2->alpha;
      outDataPtr->y = inPtr1->y * inPtr1->alpha + inPtr2->y * inPtr2->alpha;
      outDataPtr->alpha = inPtr1->alpha + inPtr2->alpha;
      inPtr1 += 2;
      inPtr2 += 2;
      outDataPtr->a += inPtr3->a * inPtr3->alpha + inPtr4->a * inPtr4->alpha;
      outDataPtr->b += inPtr3->b * inPtr3->alpha + inPtr4->b * inPtr4->alpha;
      outDataPtr->y += inPtr3->y * inPtr3->alpha + inPtr4->y * inPtr4->alpha;
      outDataPtr->alpha += inPtr3->alpha + inPtr4->alpha;
      inPtr3 += 2;
      inPtr4 += 2;
      if(outDataPtr->alpha > NA_SINGULARITYf) {
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

      na_ConvertToPerceptualRGB(outDataPtr, outDataPtr);
      outDataPtr += 1;
    }
    // Each line has advanced till the last pixel of the line, so we only
    // have to overjump one more line.
    inPtr1 += radioImage->width;
    inPtr2 += radioImage->width;
    inPtr3 += radioImage->width;
    inPtr4 += radioImage->width;
  }
  naReleaseImage(radioImage);
  
  return outImage;
}

NA_HDEF void naAccumulateResizeLine(
  NAColor* out,
  const NAColor* in,
  size_t outY,
  size_t inY,
  size_t outWidth,
  size_t inWidth,
  float factorY)
{
  NAColor* outPtr = &out[outY * outWidth];
  const NAColor* inPtr = &in[inY * inWidth];
  
  int32 inX = 0;
  float subX = 0.f;
  float factorX = (float)inWidth / (float)outWidth;
  float remainerX = factorX;
  for(int32 outX = 0; outX < (int32)outWidth; outX += 1) {
    float counterSubX = 1.f - subX;
    while(counterSubX <= remainerX) {
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
    if(inX < (int32)inWidth) {
      float accumulateFactor = factorY * remainerX;
      outPtr->a += accumulateFactor * inPtr->a;
      outPtr->b += accumulateFactor * inPtr->b;
      outPtr->y += accumulateFactor * inPtr->y;
      outPtr->alpha += accumulateFactor * inPtr->alpha;
    }
    subX += remainerX;
    outPtr += 1;
    remainerX = (outX + 2) * factorX - (inX + subX);
  }
}



NA_DEF NAImage* naCreateImageWithResize(const NAImage* image, NASizes newSize) {
  NAImage* outImage;
  
#if NA_DEBUG
  if(!naIsSizesUseful(newSize))
    naError("Given size is not useful");
#endif
  
  NAColor blank = {0.f, 0.f, 0.f, 0.f};
  outImage = naCreateImage(newSize, &blank);
  
  // Create radiometric copy of image.
  NAImage* radioImage = naCreateImageCopy(image);
  size_t pixelCount = na_GetImagePixelCount(radioImage);
  NAColor* inPtr = image->data;
  NAColor* radioPtr = radioImage->data;
  for(size_t i = 0; i < pixelCount; ++i) {
    naFillColorWithCopy(radioPtr, inPtr);
    na_ConvertToRadiometricRGB(radioPtr, radioPtr);
    radioPtr++;
    inPtr++;
  }

  size_t inY = 0;
  float subY = 0.f;
  float factorY = (float)image->height / (float)newSize.height;
  float remainerY = factorY;
  for(size_t outY = 0; outY < newSize.height; outY += 1) {
    float counterSubY = 1.f - subY;
    while(counterSubY <= remainerY) {
      naAccumulateResizeLine(
        outImage->data,
        radioImage->data,
        outY,
        inY,
        newSize.width,
        image->width,
        counterSubY);
      remainerY -= counterSubY;
      inY++;
      counterSubY = 1.f;
    }
    subY = 1.f - counterSubY;
    if(inY < (size_t)image->height) {
      naAccumulateResizeLine(
        outImage->data,
        radioImage->data,
        outY,
        inY,
        newSize.width,
        image->width,
        remainerY);
    }
    subY += remainerY;
    remainerY = (outY + 2) * factorY - (inY + subY);
  }
  
  // Normalize the whole output image;
  float divisor = (1.f / factorY) * (float)newSize.width / (float)image->width;
  NAColor* outPtr = outImage->data;
  for(size_t i = 0; i < (newSize.width * newSize.height); i += 1) {
    outPtr->a *= divisor;
    outPtr->b *= divisor;
    outPtr->y *= divisor;
    outPtr->alpha *= divisor;

    na_ConvertToPerceptualRGB(outPtr, outPtr);

    outPtr += 1;
  }
  
  naReleaseImage(radioImage);
  
  return outImage;
}



NA_HDEF void na_DestroyImage(NAImage* image) {
  naFree(image->data);
  naFree(image);
}

NA_API NAImage* naRetainImage(const NAImage* image) {
  NAImage* mutableImage = (NAImage*)image; 
  return (NAImage*)naRetainRefCount(&mutableImage->refCount);
}

NA_DEF void naReleaseImage(const NAImage* image) {
  NAImage* mutableImage = (NAImage*)image; 
  naReleaseRefCount(&mutableImage->refCount, mutableImage, (NAMutator)na_DestroyImage);
}



#define NA_RGBA_COLOR_CHANNEL_COUNT 4

NA_DEF void naFillImageWithu8(NAImage* image, const void* data, NABool topToBottom, NAColorBufferType bufferType) {
  NAColor* imgPtr = image->data;
  const uint8* u8Ptr;
  
  if(topToBottom) {
    NASizes size = naGetImageSize(image);
    for(size_t y = 0; y < size.height; y++) {
      u8Ptr = &(((uint8*)data)[(size.height - y - 1) * naGetImageSize(image).width * NA_RGBA_COLOR_CHANNEL_COUNT]);
      for(size_t x = 0; x < size.width; x++) {
        naFillColorWithSRGBu8v(imgPtr, u8Ptr, bufferType);
        imgPtr += 1;
        u8Ptr += NA_RGBA_COLOR_CHANNEL_COUNT;
      }
    }
  }else{
    u8Ptr = data;
    size_t pixelCount = na_GetImagePixelCount(image);
    for(size_t i = 0; i < pixelCount; ++i) {
      naFillColorWithSRGBu8v(imgPtr, u8Ptr, bufferType);
      imgPtr += 1;
      u8Ptr += NA_RGBA_COLOR_CHANNEL_COUNT;
    }
  }
}



NA_DEF void naConvertImageTou8(const NAImage* image, void* data, NABool topToBottom, NAColorBufferType bufferType) {
  const NAColor* imgPtr = image->data;
  uint8* u8Ptr;
  
  if(topToBottom) {
    NASizes size = naGetImageSize(image);
    for(size_t y = 0; y < size.height; y++) {
      u8Ptr = &(((uint8*)data)[(size.height - y - 1) * naGetImageSize(image).width * NA_RGBA_COLOR_CHANNEL_COUNT]);
      for(size_t x = 0; x < size.width; x++) {
        naFillSRGBu8WithColor(u8Ptr, imgPtr, bufferType);
        imgPtr += 1;
        u8Ptr += NA_RGBA_COLOR_CHANNEL_COUNT;
      }
    }
  }else{
    u8Ptr = data;
    size_t pixelCount = na_GetImagePixelCount(image);
    for(size_t i = 0; i < pixelCount; ++i) {
      naFillSRGBu8WithColor(u8Ptr, imgPtr, bufferType);
      imgPtr += 1;
      u8Ptr += NA_RGBA_COLOR_CHANNEL_COUNT;
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
