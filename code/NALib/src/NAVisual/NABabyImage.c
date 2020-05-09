
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "../NABabyImage.h"
#include "../NAVectorAlgebra.h"


struct NABabyImage{
  NARefCount refcount;
  int32 width;
  int32 height;
  float* data;
};




NA_DEF NAInt naGetBabyImageValuesPerLine(const NABabyImage* image){
  #ifndef NDEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (NAInt)image->width * NA_BABY_COLOR_CHANNEL_COUNT;
}



NA_HIDEF NAInt naGetBabyImagePixelCount(const NABabyImage* image){
  #ifndef NDEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (NAInt)image->width * (NAInt)image->height;
}



NA_HIDEF NAInt naGetBabyImageDataSize(const NABabyImage* image){
  #ifndef NDEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return (NAInt)image->width * (NAInt)image->height * NA_BABY_COLOR_BYTES_PER_COMPONENT;
}



NA_DEF float* naGetBabyImageData(const NABabyImage* image){
  #ifndef NDEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return image->data;
}



NA_DEF NASizei naGetBabyImageSize(const NABabyImage* image){
  #ifndef NDEBUG
    if(!image)
      naCrash("Given image is a Null-Pointer");
  #endif
  return naMakeSizei(image->width, image->height);
}



NA_DEF NABabyImage* naCreateBabyImage(NASizei size, const NABabyColor color){
  NABabyImage* image;
  #ifndef NDEBUG
    if(size.width <= 0 || size.height <= 0)
      naError("size must be > 0");
    if(size.width > NA_MAX_i32 || size.height > NA_MAX_i32)
      naError("size is too big");
    if(color && color[3] == 0.f && (color[0] != 0.f || color[1] != 0.f || color[2] != 0.f))
      naError("insecure color given");
  #endif
  image = naAlloc(NABabyImage);
  naInitRefCount(&image->refcount);
  image->width = (int32)size.width;
  image->height = (int32)size.height;
  image->data = naMalloc(naGetBabyImageDataSize(image));
  if(color){
    NAInt i;
    NAInt pixelCount = naGetBabyImagePixelCount(image);
    float* ptr = image->data;
    for(i = 0; i < pixelCount; i++){
      naCopyV4f(ptr, color);
      ptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
  }
  return image;
}



NA_HDEF void naBlendBabyImage(NAInt pixelCount, float* ret, const float* base, const float* top, NABlendMode mode, float blend, NABool baseIsImage, NABool topIsImage){
  NAInt i;
  blend = naLinearizeColorValue(blend);
  for(i = 0; i < pixelCount; i++){
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
  
  #ifndef NDEBUG
    if(!base)
      naCrash("Given base image is a Null-Pointer");
    if(!tint)
      naCrash("tint is Null");
    if(tint[3] == 0.f && (tint[0] != 0.f || tint[1] != 0.f || tint[2] != 0.f))
      naError("insecure tint color given");
  #endif
  
  retimage = naCreateBabyImage(naGetBabyImageSize(base), NA_NULL);
  pixelCount = naGetBabyImagePixelCount(base);
  
  baseptr = base->data;
  naBlendBabyImage(pixelCount, retimage->data, baseptr, tint, mode, blend, NA_TRUE, NA_FALSE);

  return retimage;
}



NA_DEF NABabyImage* naCreateBabyImageWithBlend(const NABabyImage* base, const NABabyImage* top, NABlendMode mode, float blend){
  NABabyImage* retimage;
  NAInt pixelCount;
  
  #ifndef NDEBUG
    if(!top)
      naCrash("top is Null");
    if(base && !naEqualSizei(naGetBabyImageSize(base), naGetBabyImageSize(top)))
      naError("The two images have not the same size");
  #endif
  
  retimage = naCreateBabyImage(naGetBabyImageSize(top), NA_NULL);
  pixelCount = naGetBabyImagePixelCount(top);
    
  if(base){
    const float* baseptr = base->data;
    naBlendBabyImage(pixelCount, retimage->data, baseptr, top->data, mode, blend, NA_TRUE, NA_TRUE);
  }else{
    NABabyColor transparent = {0.f, 0.f, 0.f, 0.f};
    naBlendBabyImage(pixelCount, retimage->data, transparent, top->data, mode, blend, NA_FALSE, NA_TRUE);
  }

  return retimage;
}



NA_DEF NABabyImage* naCreateBabyImageWithHalfSize(const NABabyImage* image){
  NASizei halfsize;
  NAInt x, y;
  NABabyImage* outimage;
  NAInt valuesPerLine;
  float* inptr1;
  float* inptr2;
  float* outdataptr;
  
  #ifndef NDEBUG
    if((image->width % 2) || (image->height % 2))
      naError("Width or height not divisible by 2");
  #endif
  halfsize = naMakeSizei(image->width / 2, image->height / 2);

  outimage = naCreateBabyImage(halfsize, NA_NULL);
  valuesPerLine = naGetBabyImageValuesPerLine(image);
  
  inptr1 = image->data;
  inptr2 = image->data + valuesPerLine;
  outdataptr = outimage->data;
  for(y = 0; y < image->height; y += 2){
    for(x = 0; x < image->width; x += 2){
      outdataptr[0] = inptr1[0] * inptr1[3] + inptr1[4] * inptr1[7];
      outdataptr[1] = inptr1[1] * inptr1[3] + inptr1[5] * inptr1[7];
      outdataptr[2] = inptr1[2] * inptr1[3] + inptr1[6] * inptr1[7];
      outdataptr[3] = inptr1[3] + inptr1[7];
      inptr1 += 8;
      outdataptr[0] += inptr2[0] * inptr2[3] + inptr2[4] * inptr2[7];
      outdataptr[1] += inptr2[1] * inptr2[3] + inptr2[5] * inptr2[7];
      outdataptr[2] += inptr2[2] * inptr2[3] + inptr2[6] * inptr2[7];
      outdataptr[3] += inptr2[3] + inptr2[7];
      inptr2 += 8;
      if(outdataptr[3] != 0.f){
        float invweight = naInvf(outdataptr[3]);
        outdataptr[0] *= invweight;
        outdataptr[1] *= invweight;
        outdataptr[2] *= invweight;
        outdataptr[3] *= .25f;
      }
      outdataptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
    // Each line has advanced till the last pixel of the line, so we only
    // have to overjump one more line.
    inptr1 += valuesPerLine;
    inptr2 += valuesPerLine;
  }
  return outimage;
}



NA_HDEF void naDestroyBabyImage(NABabyImage* image){
  naFree(image->data);
  naFree(image);
}

NA_API NABabyImage* naRetainBabyImage(NABabyImage* image){
  return (NABabyImage*)naRetainRefCount(&image->refcount);
}

NA_DEF void naReleaseBabyImage(NABabyImage* image){
  naReleaseRefCount(&image->refcount, image, (NAMutator)naDestroyBabyImage);
}



NA_DEF void naFillBabyImageWithu8(NABabyImage* image, const void* data, NABool toptobottom, NAColorBufferType bufferType){
  float* imgptr = image->data;
  const uint8* u8ptr;

  if(toptobottom){
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
    pixelCount = naGetBabyImagePixelCount(image);
    for(i = 0; i < pixelCount; i++){
      naFillBabyColorWithu8(imgptr, u8ptr, bufferType);
      imgptr += NA_BABY_COLOR_CHANNEL_COUNT;
      u8ptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
  }
}



NA_DEF void naConvertBabyImageTou8(const NABabyImage* image, void* data, NABool toptobottom, NAColorBufferType bufferType){
  const float* imgptr = image->data;
  uint8* u8ptr;

  if(toptobottom){
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
    pixelCount = naGetBabyImagePixelCount(image);
    for(i = 0; i < pixelCount; i++){
      naFillu8WithBabyColor(u8ptr, imgptr, bufferType);
      imgptr += NA_BABY_COLOR_CHANNEL_COUNT;
      u8ptr += NA_BABY_COLOR_CHANNEL_COUNT;
    }
  }
}



// Copyright (c) NALib, Tobias Stamm
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
