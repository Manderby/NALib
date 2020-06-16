
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../../NAUIImage.h"
#include "../../../../NAPNG.h"


NABabyImage* naCreateBabyImageFromNativeImage(const void* nativeImage){
  NABabyImage* image;
  
  CFDataRef rawData = CGDataProviderCopyData(CGImageGetDataProvider((CGImageRef)nativeImage));
  image = naCreateBabyImage(naMakeSizei((NAInt)CGImageGetWidth((CGImageRef)nativeImage), (NAInt)CGImageGetHeight((CGImageRef)nativeImage)), NA_NULL);
  // Note that reading PNG files directly does not premultiply alpha!
  naFillBabyImageWithu8(image, CFDataGetBytePtr(rawData), NA_TRUE, NA_COLOR_BUFFER_RGBAPre);
  CFRelease(rawData);
  
  return image;
}



NABabyImage* naCreateBabyImageFromFilePath(const NAUTF8Char* pathStr){
  NABabyImage* image = NA_NULL;

//  // Currently, only png is possible
//  NAPNG* png = naNewPNGWithPath(pathStr);
//  NABabyImage* babyImage = naCreateBabyImageFromPNG(png);
//  return babyImage;

//  CGDataProviderRef dataprovider = CGDataProviderCreateWithFilename(pathStr);
//  if(dataprovider){
//  
//    CGImageRef nativeImage = CGImageCreateWithPNGDataProvider(dataprovider, NULL, NA_FALSE, kCGRenderingIntentAbsoluteColorimetric);
//    image = naCreateBabyImageFromNativeImage(nativeImage);
//    
//    CGImageRelease(nativeImage);
//    CGDataProviderRelease(dataprovider);
//  }

  NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:pathStr]];
  NSImage* nsImage = [[NSImage alloc] initWithContentsOfURL:url];
  CGImageRef nativeImage = [nsImage CGImageForProposedRect:NA_NULL context:NA_NULL hints:NA_NULL];
  image = naCreateBabyImageFromNativeImage(nativeImage);

  NA_COCOA_RELEASE(nsImage);
  // Important: Do not use CGImageRelease on nativeImage. It is part of nsImage.

  return image;
}



NA_DEF void* naAllocNativeImageWithBabyImage(const NABabyImage* image){
  CGImageRef nativeImage;
  CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
  NASizei imageSize = naGetBabyImageSize(image);
  CGContextRef cgContext = CGBitmapContextCreateWithData(NULL, (size_t)imageSize.width, (size_t)imageSize.height, 8, (size_t)naGetBabyImageValuesPerLine(image), colorSpace, kCGImageAlphaPremultipliedLast, NULL, NULL);

  uint8* imgData = CGBitmapContextGetData(cgContext);
  naConvertBabyImageTou8(image, imgData, NA_TRUE, NA_COLOR_BUFFER_RGBAPre);
  
  nativeImage = CGBitmapContextCreateImage(cgContext);
  CGContextRelease(cgContext);
  CGColorSpaceRelease(colorSpace);
  return nativeImage;
}




void* naAllocNativeImageWithUIImage(NAUIImage* uiimage, NAUIImageKind kind, NAUIImageSkin skin){
  NASizei imageSize = naGetUIImage1xSize(uiimage);
  NSImage* image = [[NSImage alloc] initWithSize:NSMakeSize(imageSize.width, imageSize.height)];

  CGImageRef img1x = na_GetUIImageNativeImage(uiimage, NA_UIIMAGE_RESOLUTION_1x, kind, skin);
  CGImageRef img2x = na_GetUIImageNativeImage(uiimage, NA_UIIMAGE_RESOLUTION_2x, kind, skin);
  if(img1x){
    NSBitmapImageRep* rep = NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img1x]);
    [image addRepresentation:rep];
  }
  if(img2x){
    NSBitmapImageRep* rep = NA_COCOA_AUTORELEASE([[NSBitmapImageRep alloc] initWithCGImage:img2x]);
    [image addRepresentation:rep];
  }
  
  return NA_COCOA_PTR_OBJC_TO_C(image);
}



void naDeallocNativeImage(void* nativeImage){
  CGImageRelease(nativeImage);
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
