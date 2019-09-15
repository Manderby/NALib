
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAUIImage.h"


void naFillBabyColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skincolor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
    skincolor[0] = 16;
    skincolor[1] = 16;
    skincolor[2] = 16;
    skincolor[3] = 255;
    break;
  case NA_UIIMAGE_SKIN_DARK:
    skincolor[0] = 240;
    skincolor[1] = 240;
    skincolor[2] = 240;
    skincolor[3] = 255;
    break;
  default:
    #ifndef NDEBUG
      naError("Cannot provide color for plain skin");
    #endif
    break;
  }
  naFillBabyColorWithUInt8(color, skincolor, NA_FALSE);
}


NABabyImage* naAllocBabyImageFromImageRef(const void* imageref){
  NABabyImage* image;
  
  CFDataRef rawData = CGDataProviderCopyData(CGImageGetDataProvider((CGImageRef)imageref));
  image = naAllocBabyImage(naMakeSizei((NAInt)CGImageGetWidth((CGImageRef)imageref), (NAInt)CGImageGetHeight((CGImageRef)imageref)), NA_NULL);
  // Note that reading PNG files directly does not premultiply alpha!
  naFillBabyImageWithUInt8(image, CFDataGetBytePtr(rawData), NA_FALSE);
  CFRelease(rawData);
  
  return image;
}



NABabyImage* naAllocBabyImageFromFilePath(const NAUTF8Char* pathStr){
  NABabyImage* image = NA_NULL;
  
  CGDataProviderRef dataprovider = CGDataProviderCreateWithFilename(pathStr);
  if(dataprovider){
  
    CGImageRef imageref = CGImageCreateWithPNGDataProvider(dataprovider, NULL, NA_FALSE, kCGRenderingIntentAbsoluteColorimetric);
    image = naAllocBabyImageFromImageRef(imageref);
    
    CGImageRelease(imageref);
    CGDataProviderRelease(dataprovider);
  }

  return image;
}



NA_DEF CGImageRef naCreateCGImageWithBabyImage(const NABabyImage* image){
  CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
  NASizei imageSize = naGetBabyImageSize(image);
  CGContextRef cgcontext = CGBitmapContextCreateWithData(NULL, (size_t)imageSize.width, (size_t)imageSize.height, 8, (size_t)naGetBabyImageValuesPerLine(image), colorSpace, kCGImageAlphaPremultipliedLast, NULL, NULL);

  uint8* imgdata = CGBitmapContextGetData(cgcontext);
  naConvertBabyImageToUInt8(image, imgdata, NA_TRUE);
  
  CGImageRef imageref = CGBitmapContextCreateImage(cgcontext);
  CGContextRelease(cgcontext);
  CGColorSpaceRelease(colorSpace);
  return imageref;
}



NA_HIDEF void* naGetUIImageRef(NAUIImage* image, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  void* retimg = image->imgref[(resolution * NA_UIIMAGE_KIND_COUNT + kind) * NA_UIIMAGE_SKIN_COUNT + skin];
  if(!retimg && skin != NA_UIIMAGE_SKIN_PLAIN){
    void* plainimg = image->imgref[(resolution * NA_UIIMAGE_KIND_COUNT + kind) * NA_UIIMAGE_SKIN_COUNT + NA_UIIMAGE_SKIN_PLAIN];
    if(plainimg){
      NABabyImage* plainbabyimg = naAllocBabyImageFromImageRef(plainimg);
      
      NABabyColor skincolor;
      naFillBabyColorWithSkin(skincolor, skin);
      NABabyImage* skinnedImage = naAllocBabyImageWithTint(plainbabyimg, skincolor, image->tintMode, 1.f);
      retimg = naCreateCGImageWithBabyImage(skinnedImage);
      naSetUIImageRef(image, retimg, resolution, kind, skin);
      naDeallocBabyImage(skinnedImage);
      naDeallocBabyImage(plainbabyimg);
    }
  }
  return retimg;
}



NA_HIDEF void naSetUIImageRef(NAUIImage* image, void* imgref, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  image->imgref[(resolution * NA_UIIMAGE_KIND_COUNT + kind) * NA_UIIMAGE_SKIN_COUNT + skin] = imgref;
}



NA_DEF NAUIImage* naAllocUIImage(NABabyImage* main, NABabyImage* alt, NAUIImageResolution resolution, NABlendMode tintMode){
  #ifndef NDEBUG
    if(!main)
      naError("There mus be a main image");
    if(alt && !naEqualSizei(naGetBabyImageSize(main), naGetBabyImageSize(alt)))
      naError("Both images must have the same size.");
  #endif
  NAUIImage* uiImage = naAlloc(NAUIImage);
  
  uiImage->size1x = naGetBabyImageSize(main);
  uiImage->tintMode = tintMode;
  naZeron(uiImage->imgref, NA_UIIMAGE_RESOLUTION_COUNT * NA_UIIMAGE_KIND_COUNT * NA_UIIMAGE_SKIN_COUNT * naSizeof(CGImageRef));
  
  switch(resolution){
  case NA_UIIMAGE_RESOLUTION_1x:
    naSetUIImageRef(uiImage, naCreateCGImageWithBabyImage(main), NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    if(alt){
      naSetUIImageRef(uiImage, naCreateCGImageWithBabyImage(alt), NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
    }
    break;
  case NA_UIIMAGE_RESOLUTION_2x:
    #ifndef NDEBUG
      if(uiImage->size1x.width % 2 || uiImage->size1x.height % 2)
        naError("Image size is not divisable by 2");
    #endif
    uiImage->size1x.width /= 2;
    uiImage->size1x.height /= 2;
    naSetUIImageRef(uiImage, naCreateCGImageWithBabyImage(main), NA_UIIMAGE_RESOLUTION_2x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    NABabyImage* main1x = naAllocBabyImageWithHalfSize(main);
    naSetUIImageRef(uiImage, naCreateCGImageWithBabyImage(main1x), NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    naDeallocBabyImage(main1x);
    if(alt){
      naSetUIImageRef(uiImage, naCreateCGImageWithBabyImage(alt), NA_UIIMAGE_RESOLUTION_2x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
      NABabyImage* alt1x = naAllocBabyImageWithHalfSize(alt);
      naSetUIImageRef(uiImage, naCreateCGImageWithBabyImage(alt1x), NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
      naDeallocBabyImage(alt1x);
    }
    break;
  default:
    #ifndef NDEBUG
      naError("Unknown resolution");
    #endif
    break;
  }

  return uiImage;
}



NA_API void naDeallocUIImage(NAUIImage* uiimage){
  for(NAInt i = 0; i < NA_UIIMAGE_RESOLUTION_COUNT * NA_UIIMAGE_KIND_COUNT * NA_UIIMAGE_SKIN_COUNT; i++){
    if(uiimage->imgref[i]){CGImageRelease(uiimage->imgref[i]);}
  }
  naFree(uiimage);
}



void* naAllocNativeImageWithUIImage(NAUIImage* uiimage, NAUIImageKind kind, NAUIImageSkin skin){
  NASizei imagesize = naGetUIImage1xSize(uiimage);
  NSImage* image = [[NSImage alloc] initWithSize:NSMakeSize(imagesize.width, imagesize.height)];

  CGImageRef img1x = naGetUIImageRef(uiimage, NA_UIIMAGE_RESOLUTION_1x, kind, skin);
  CGImageRef img2x = naGetUIImageRef(uiimage, NA_UIIMAGE_RESOLUTION_2x, kind, skin);
  if(img1x){[image addRepresentation:NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img1x])];}
  if(img2x){[image addRepresentation:NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img2x])];}
  
  return NA_COCOA_TAKE_OWNERSHIP(image);
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
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
