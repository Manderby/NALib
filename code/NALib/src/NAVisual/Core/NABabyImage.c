
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



NA_HDEF void na_BlendBabyImage(NAInt pixelCount, float* ret, const float* base, const float* top, NABlendMode mode, float blend, NABool baseIsImage, NABool topIsImage){
  NAInt i;
  //blend = naLinearizeColorValue(blend);
  for(i = 0; i < pixelCount; ++i){
    float topblend;
    switch(mode){
    case NA_BLEND_ZERO:
      naCopyV4f(ret, base);
      break;
    case NA_BLEND:
      topblend = blend;
      naFillV4f(ret,
        (1.f - topblend) * base[0] + topblend * top[0],
        (1.f - topblend) * base[1] + topblend * top[1],
        (1.f - topblend) * base[2] + topblend * top[2],
        (1.f - topblend) * base[3] + topblend * top[3]);
      break;
    case NA_BLEND_OVERLAY:
      topblend = top[3] * blend;
      naFillV4f(ret,
        (1.f - topblend) * base[0] + topblend * top[0],
        (1.f - topblend) * base[1] + topblend * top[1],
        (1.f - topblend) * base[2] + topblend * top[2],
        base[3] + (1.f - base[3]) * topblend);
      break;
    case NA_BLEND_OPAQUE:
      topblend = top[3] * blend;
      naFillV4f(ret,
        (1.f - topblend) * base[0] + topblend * top[0],
        (1.f - topblend) * base[1] + topblend * top[1],
        (1.f - topblend) * base[2] + topblend * top[2],
        base[3] * (1.f - (1.f - top[3]) * blend));
      break;
    case NA_BLEND_BLACK_GREEN:
      topblend = top[3] * blend;
      naFillV4f(ret,
        (1.f - topblend) * base[0] + topblend * top[0],
        (1.f - topblend) * base[1] + topblend * top[1],
        (1.f - topblend) * base[2] + topblend * top[2],
        (1.f - base[1]) * base[3] * (1.f - (1.f - top[3]) * blend));
      break;
    case NA_BLEND_WHITE_GREEN:
      topblend = top[3] * blend;
      naFillV4f(ret,
        (1.f - topblend) * base[0] + topblend * top[0],
        (1.f - topblend) * base[1] + topblend * top[1],
        (1.f - topblend) * base[2] + topblend * top[2],
        base[1] * base[3] * (1.f - (1.f - top[3]) * blend));
      break;
    }
    if(ret[3] == 0.f){
      naFillV3f(ret, 0.f, 0.f, 0.f);
    }
    ret += NA_BABY_COLOR_CHANNEL_COUNT;
    if(baseIsImage){base += NA_BABY_COLOR_CHANNEL_COUNT;}
    if(topIsImage){top += NA_BABY_COLOR_CHANNEL_COUNT;}
  }
}


NA_DEF NABabyImage* naCreateBabyImageWithTint(const NABabyImage* base, const NABabyColor tint, NABlendMode mode, float blend){
  NABabyImage* retimage;
  NAInt pixelCount;
  const float* baseptr;
  
  #if NA_DEBUG
    if(!base)
      naCrash("Given base image is a Null-Pointer");
    if(!tint)
      naCrash("tint is Null");
    if(tint[3] == 0.f && (tint[0] != 0.f || tint[1] != 0.f || tint[2] != 0.f))
      naError("inanoSecondure tint color given");
  #endif
  
  retimage = naCreateBabyImage(naGetBabyImageSize(base), NA_NULL);
  pixelCount = na_GetBabyImagePixelCount(base);
  
  baseptr = base->data;
  na_BlendBabyImage(pixelCount, retimage->data, baseptr, tint, mode, blend, NA_TRUE, NA_FALSE);

  return retimage;
}



NA_DEF NABabyImage* naCreateBabyImageWithBlend(const NABabyImage* base, const NABabyImage* top, NABlendMode mode, float blend){
  NABabyImage* retimage;
  NAInt pixelCount;
  
  #if NA_DEBUG
    if(!top)
      naCrash("top is Null");
    if(base && !naEqualSizei(naGetBabyImageSize(base), naGetBabyImageSize(top)))
      naError("The two images have not the same size");
  #endif
  
  retimage = naCreateBabyImage(naGetBabyImageSize(top), NA_NULL);
  pixelCount = na_GetBabyImagePixelCount(top);
    
  if(base){
    const float* baseptr = base->data;
    na_BlendBabyImage(pixelCount, retimage->data, baseptr, top->data, mode, blend, NA_TRUE, NA_TRUE);
  }else{
    NABabyColor transparent = {0.f, 0.f, 0.f, 0.f};
    na_BlendBabyImage(pixelCount, retimage->data, transparent, top->data, mode, blend, NA_FALSE, NA_TRUE);
  }

  return retimage;
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
