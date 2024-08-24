
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#include "../../NAVisual/NAColor.h"
#include "../../NAVisual/NAImage.h"
#include "../Core/NAAppCore.h"



// NSAppearance and drawInRect exists since 10.9
#if defined __MAC_10_9

  // NSAppearanceName exists since 10.13
  #if !defined __MAC_10_13
    typedef NSString* NSAppearanceName;
  #endif

  NA_DEF NASkin naGetCurrentSkin(void) {
    NASkin skin = NA_SKIN_LIGHT;
    NSAppearanceName appearancename = NSAppearanceNameAqua;

    #if defined __MAC_11_0
      NA_MACOS_AVAILABILITY_GUARD_11_0(
        if([NSAppearance respondsToSelector:@selector(currentDrawingAppearance)]) {
          appearancename = [[NSAppearance currentDrawingAppearance] name];
        }
      )
    #else
      if([NSAppearance respondsToSelector:@selector(currentAppearance)]) {
        if([[NSAppearance currentAppearance] respondsToSelector:@selector(name)]) {
          NA_MACOS_AVAILABILITY_GUARD_10_9(
            appearancename = [[NSAppearance currentAppearance] name];
          )
        }
      }
    #endif

    NA_MACOS_AVAILABILITY_GUARD_10_10(
      if(appearancename == NSAppearanceNameVibrantDark) {
        skin = NA_SKIN_DARK;
      }
    )
    NA_MACOS_AVAILABILITY_GUARD_10_14(
      if(appearancename == NSAppearanceNameDarkAqua
      || appearancename == NSAppearanceNameAccessibilityHighContrastDarkAqua
      || appearancename == NSAppearanceNameAccessibilityHighContrastVibrantDark) {
        skin = NA_SKIN_DARK;
      }
    )

    return skin;
  }
#else
  NA_DEF NASkin naGetCurrentSkin(void) {
    return NA_SKIN_LIGHT;
  }
#endif



NA_DEF void na_FillDefaultTextColorWithSystemSkin(NAColor* color) {
  NSColor* labelColor = naGetLabelColor();
  naFillColorWithSRGB(
    color,
    (float)[labelColor redComponent],
    (float)[labelColor greenComponent],
    (float)[labelColor blueComponent],
    1.f);
}



NA_DEF void na_FillDefaultLinkColorWithSystemSkin(NAColor* color) {
  NSColor* linkColor = naGetLinkColor();
  naFillColorWithSRGB(
    color,
    (float)[linkColor redComponent],
    (float)[linkColor greenComponent],
    (float)[linkColor blueComponent],
    1.f);
}



NA_DEF void na_FillDefaultAccentColorWithSystemSkin(NAColor* color) {
  NSColor* accentColor = naGetAccentColor();
  naFillColorWithSRGB(
    color,
    (float)[accentColor redComponent],
    (float)[accentColor greenComponent],
    (float)[accentColor blueComponent],
    1.f);
}



NA_DEF NAImage* naCreateImageWithNativeImage(const void* nativeImage) {
  NAImage* image;
  
  CFDataRef rawData = CGDataProviderCopyData(CGImageGetDataProvider((CGImageRef)nativeImage));
  image = naCreateImage(naMakeSizes(
    (size_t)CGImageGetWidth((CGImageRef)nativeImage),
    (size_t)CGImageGetHeight((CGImageRef)nativeImage)), NA_NULL);
  // Note that reading PNG files directly does not premultiply alpha!
  naFillImageWithu8(image, CFDataGetBytePtr(rawData), NA_TRUE, NA_COLOR_BUFFER_RGBAPre);
  CFRelease(rawData);
  
  return image;
}



NA_DEF NAImage* naCreateImageWithFilePath(const NAUTF8Char* pathStr) {
  NAImage* image = NA_NULL;

//  // Currently, only png is possible
//  NAPNG* png = naNewPNGWithPath(pathStr);
//  NAImage* image = naCreateImageWithPNG(png);
//  return image;

//  CGDataProviderRef dataprovider = CGDataProviderCreateWithFilename(pathStr);
//  if(dataprovider) {
//  
//    CGImageRef nativeImage = CGImageCreateWithPNGDataProvider(dataprovider, NULL, NA_FALSE, kCGRenderingIntentAbsoluteColorimetric);
//    image = naCreateImageWithNativeImage(nativeImage);
//    
//    CGImageRelease(nativeImage);
//    CGDataProviderRelease(dataprovider);
//  }

  NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:pathStr]];
  NSImage* nsImage = [[NSImage alloc] initWithContentsOfURL:url];
  CGImageRef nativeImage = [nsImage CGImageForProposedRect:NA_NULL context:NA_NULL hints:NA_NULL];
  image = naCreateImageWithNativeImage(nativeImage);

  NA_COCOA_RELEASE(nsImage);
  // Important: Do not use CGImageRelease on nativeImage. It is part of nsImage.

  return image;
}



NA_DEF void* naAllocNativeImageWithImage(const NAImage* image) {
  CGImageRef nativeImage;
  CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
  NASizes imageSize = naGetImageSize(image);
  CGContextRef cgContext = CGBitmapContextCreateWithData(
    NULL,
    (size_t)imageSize.width,
    (size_t)imageSize.height,
    NA_BITS_PER_BYTE,
    (size_t)naGetImageSize(image).width * 4, // 4 channels: RGBA
    colorSpace,
    kCGImageAlphaPremultipliedLast,
    NULL,
    NULL);

  uint8* imgData = CGBitmapContextGetData(cgContext);
  naConvertImageTou8(image, imgData, NA_TRUE, NA_COLOR_BUFFER_RGBAPre);
  
  nativeImage = CGBitmapContextCreateImage(cgContext);
  CGContextRelease(cgContext);
  CGColorSpaceRelease(colorSpace);
  return nativeImage;
}


NA_HDEF BOOL na_drawFixedResolutionImage(const NAImageSet* imageSet, double resolution, NAImageSetInteraction interaction, NABool secondaryState, NSSize imageSize, NSRect dstRect) {
  NASkin skin = NA_SKIN_SYSTEM;
  if(naGetImageSetTinting(imageSet) != NA_BLEND_ZERO) {
    skin = naGetCurrentSkin();
  }
  
  CGImageRef cocoaImage = na_GetImageSetNativeSubImage(imageSet, resolution, skin, interaction, secondaryState);

  // Yes, we create a new NSImage which we draw into the NSImage which
  // calls this callback. It is unknown to me exactly why I need to do
  // that but otherwise the context just isn't there on all systems.
  // Potentially this has to do with threading which can only allocate
  // memory in its own region as this callback may be called in a thread.
  [NSGraphicsContext saveGraphicsState];
  NSImage* drawImage = [[NSImage alloc] initWithSize:imageSize];
  [drawImage lockFocus];
    CGContextDrawImage(naGetCGContextRef([NSGraphicsContext currentContext]), dstRect, cocoaImage);
  [drawImage unlockFocus];
  [NSGraphicsContext restoreGraphicsState];

  [drawImage drawInRect:dstRect];

  return YES;
}



NA_DEF NSImage* na_CreateResolutionIndependentNativeImage(
  const NSView* containingView,
  const NAImageSet* imageSet,
  NAImageSetInteraction interaction,
  NABool secondaryState)
{
  NSImage* image = nil;

  // modern method: Create an image which redraws itself automatically.
  // This is commented out as there have been severe problems with this working
  // on different computers. The context sometimes simply does not seem to be
  // there and returns null, resulting in empty images.
  if(containingView && [NSImage respondsToSelector:@selector(imageWithSize:flipped:drawingHandler:)]) {
    NA_MACOS_AVAILABILITY_GUARD_10_8(
      NSSize imageSize = NSMakeSize(naGetImageSet1xSize(imageSet).width, naGetImageSet1xSize(imageSet).height);
      image = [NSImage imageWithSize:imageSize flipped:NO drawingHandler:^BOOL(NSRect dstRect)
      {
        double resolution = naGetCocoaBackingScaleFactor(containingView) * NA_UI_RESOLUTION_1x;
        return na_drawFixedResolutionImage(imageSet, resolution, interaction, secondaryState, imageSize, dstRect);
      }];
    ) // end NA_MACOS_AVAILABILITY_GUARD_10_8
  }
  
  // old method: Just create an image with multiple representations.
  if(!image) {
    NASizes imageSize = naGetImageSet1xSize(imageSet);
    image = [[NSImage alloc] initWithSize:NSMakeSize(imageSize.width, imageSize.height)];

    NASkin skin = NA_SKIN_PLAIN;
    if(naGetImageSetTinting(imageSet) != NA_BLEND_ZERO) {
      skin = naGetCurrentSkin();
    }

    CGImageRef img1x = na_GetImageSetNativeSubImage(imageSet, NA_UI_RESOLUTION_1x, skin, interaction, secondaryState);
    CGImageRef img2x = na_GetImageSetNativeSubImage(imageSet, NA_UI_RESOLUTION_2x, skin, interaction, secondaryState);
    if(img1x) {
      NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithCGImage:img1x];
      [image addRepresentation:rep];
      NA_COCOA_RELEASE(rep);
    }
    if(img2x) {
      NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithCGImage:img2x];
      [image addRepresentation:rep];
      NA_COCOA_RELEASE(rep);
    }
  }
  
  return image;
}



NA_DEF void naDeallocNativeImage(void* nativeImage) {
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
