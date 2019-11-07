
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../../NAUIImage.h"


NABabyImage* naCreateBabyImageFromNativeImage(const void* nativeimage){
  NABabyImage* image;
  
  CFDataRef rawData = CGDataProviderCopyData(CGImageGetDataProvider((CGImageRef)nativeimage));
  image = naCreateBabyImage(naMakeSizei((NAInt)CGImageGetWidth((CGImageRef)nativeimage), (NAInt)CGImageGetHeight((CGImageRef)nativeimage)), NA_NULL);
  // Note that reading PNG files directly does not premultiply alpha!
  naFillBabyImageWithUInt8(image, CFDataGetBytePtr(rawData), NA_TRUE, NA_FALSE);
  CFRelease(rawData);
  
  return image;
}



NABabyImage* naCreateBabyImageFromFilePath(const NAUTF8Char* pathStr){
  NABabyImage* image = NA_NULL;
  
  CGDataProviderRef dataprovider = CGDataProviderCreateWithFilename(pathStr);
  if(dataprovider){
  
    CGImageRef nativeimage = CGImageCreateWithPNGDataProvider(dataprovider, NULL, NA_FALSE, kCGRenderingIntentAbsoluteColorimetric);
    image = naCreateBabyImageFromNativeImage(nativeimage);
    
    CGImageRelease(nativeimage);
    CGDataProviderRelease(dataprovider);
  }

  return image;
}



NA_DEF void* naAllocNativeImageWithBabyImage(const NABabyImage* image){
  CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
  NASizei imageSize = naGetBabyImageSize(image);
  CGContextRef cgcontext = CGBitmapContextCreateWithData(NULL, (size_t)imageSize.width, (size_t)imageSize.height, 8, (size_t)naGetBabyImageValuesPerLine(image), colorSpace, kCGImageAlphaPremultipliedLast, NULL, NULL);

  uint8* imgdata = CGBitmapContextGetData(cgcontext);
  naConvertBabyImageToUInt8(image, imgdata, NA_TRUE, NA_TRUE);
  
  CGImageRef nativeimage = CGBitmapContextCreateImage(cgcontext);
  CGContextRelease(cgcontext);
  CGColorSpaceRelease(colorSpace);
  return nativeimage;
}




//void* naAllocNativeImageWithUIImage(NAUIImage* uiimage, NAUIImageKind kind, NAUIImageSkin skin){
//  NASizei imagesize = naGetUIImage1xSize(uiimage);
//  NSImage* image = [[NSImage alloc] initWithSize:NSMakeSize(imagesize.width, imagesize.height)];
//
//  CGImageRef img1x = naGetUIImageNativeImage(uiimage, NA_UIIMAGE_RESOLUTION_1x, kind, skin);
//  CGImageRef img2x = naGetUIImageNativeImage(uiimage, NA_UIIMAGE_RESOLUTION_2x, kind, skin);
//  if(img1x){[image addRepresentation:NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img1x])];}
//  if(img2x){[image addRepresentation:NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img2x])];}
//  
//  return NA_COCOA_TAKE_OWNERSHIP(image);
//}



void naDeallocNativeImage(void* nativeimage){
  CGImageRelease(nativeimage);
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
