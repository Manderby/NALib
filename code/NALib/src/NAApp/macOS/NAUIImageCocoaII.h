
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "Availability.h"
#include "../NAApp.h"
#include "../../NAVisual/NAPNG.h"



// NSAppearance and drawInRect exists since 10.9
#if defined __MAC_10_9

  // NSAppearanceName exists since 10.13
  #if !defined __MAC_10_13
    typedef NSString* NSAppearanceName;
  #endif

  NA_DEF NAUIImageSkin naGetSkinForCurrentAppearance(void){
    NAUIImageSkin skin = NA_UIIMAGE_SKIN_LIGHT;
    NSAppearanceName appearancename = NSAppearanceNameAqua;

    #if defined __MAC_11_0
      NA_MACOS_AVAILABILITY_GUARD_11_0(
        if([NSAppearance respondsToSelector:@selector(currentDrawingAppearance)]){
          appearancename = [[NSAppearance currentDrawingAppearance] name];
        }
      )
    #else
      if([NSAppearance respondsToSelector:@selector(currentAppearance)]){
        if([[NSAppearance currentAppearance] respondsToSelector:@selector(name)]){
          NA_MACOS_AVAILABILITY_GUARD_10_9(
            appearancename = [[NSAppearance currentAppearance] name];
          )
        }
      }
    #endif

    NA_MACOS_AVAILABILITY_GUARD_10_10(
      if(appearancename == NSAppearanceNameVibrantDark){skin = NA_UIIMAGE_SKIN_DARK;}
    )
    NA_MACOS_AVAILABILITY_GUARD_10_14(
      if(appearancename == NSAppearanceNameDarkAqua
      || appearancename == NSAppearanceNameAccessibilityHighContrastDarkAqua
      || appearancename == NSAppearanceNameAccessibilityHighContrastVibrantDark){
        skin = NA_UIIMAGE_SKIN_DARK;}
    )

    return skin;
  }
#else
  NA_DEF NAUIImageSkin naGetSkinForCurrentAppearance(void){
    return NA_UIIMAGE_SKIN_LIGHT;
  }
#endif



NA_DEF void na_FillDefaultTextColorWithSystemSkin(NAABYColor* color){
  NSColor* labelColor = naGetLabelColor();
  naFillABYColorWithSRGB(
    color,
    (float)[labelColor redComponent],
    (float)[labelColor greenComponent],
    (float)[labelColor blueComponent],
    1.f);
}



NA_DEF void na_FillDefaultLinkColorWithSystemSkin(NAABYColor* color){
  NSColor* linkColor = naGetLinkColor();
  naFillABYColorWithSRGB(
    color,
    (float)[linkColor redComponent],
    (float)[linkColor greenComponent],
    (float)[linkColor blueComponent],
    1.f);
}



NA_DEF void na_FillDefaultAccentColorWithSystemSkin(NAABYColor* color){
  NSColor* accentColor = naGetAccentColor();
  naFillABYColorWithSRGB(
    color,
    (float)[accentColor redComponent],
    (float)[accentColor greenComponent],
    (float)[accentColor blueComponent],
    1.f);
}



NA_DEF NAABYImage* naCreateBabyImageFromNativeImage(const void* nativeImage){
  NAABYImage* image;
  
  CFDataRef rawData = CGDataProviderCopyData(CGImageGetDataProvider((CGImageRef)nativeImage));
  image = naCreateBabyImage(naMakeSizei((NAInt)CGImageGetWidth((CGImageRef)nativeImage), (NAInt)CGImageGetHeight((CGImageRef)nativeImage)), NA_NULL);
  // Note that reading PNG files directly does not premultiply alpha!
  naFillBabyImageWithu8(image, CFDataGetBytePtr(rawData), NA_TRUE, NA_COLOR_BUFFER_RGBAPre);
  CFRelease(rawData);
  
  return image;
}



NA_DEF NAABYImage* naCreateBabyImageFromFilePath(const NAUTF8Char* pathStr){
  NAABYImage* image = NA_NULL;

//  // Currently, only png is possible
//  NAPNG* png = naNewPNGWithPath(pathStr);
//  NAABYImage* abyImage = naCreateBabyImageFromPNG(png);
//  return abyImage;

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



NA_DEF void* naAllocNativeImageWithBabyImage(const NAABYImage* image){
  CGImageRef nativeImage;
  CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
  NASizei imageSize = naGetBabyImageSize(image);
  CGContextRef cgContext = CGBitmapContextCreateWithData(
    NULL,
    (size_t)imageSize.width,
    (size_t)imageSize.height,
    NA_BITS_PER_BYTE,
    (size_t)naGetBabyImageSize(image).width * NA_ABY_COLOR_CHANNEL_COUNT,
    colorSpace,
    kCGImageAlphaPremultipliedLast,
    NULL,
    NULL);

  uint8* imgData = CGBitmapContextGetData(cgContext);
  naConvertBabyImageTou8(image, imgData, NA_TRUE, NA_COLOR_BUFFER_RGBAPre);
  
  nativeImage = CGBitmapContextCreateImage(cgContext);
  CGContextRelease(cgContext);
  CGColorSpaceRelease(colorSpace);
  return nativeImage;
}


NA_HDEF BOOL na_drawFixedResolutionImage(const NAUIImage* uiImage, double resolution, NAUIImageInteraction interaction, NABool secondaryState, NSSize imageSize, NSRect dstRect){
  NAUIImageSkin skin = NA_UIIMAGE_SKIN_SYSTEM;
  if(uiImage->tintMode != NA_BLEND_ZERO){
    skin = naGetSkinForCurrentAppearance();
  }
  
  CGImageRef cocoaimage = na_GetUIImageNativeImage(uiImage, resolution, skin, interaction, secondaryState);

  // Yes, we create a new NSImage which we draw into the NSImage which
  // calls this callback. It is unknown to me exactly why I need to do
  // that but otherwise the context just isn't there on all systems.
  // Potentially this has to do with threading which can only allocate
  // memory in its own region as this callback may be called in a thread.
  [NSGraphicsContext saveGraphicsState];
  NSImage* drawImage = [[NSImage alloc] initWithSize:imageSize];
  [drawImage lockFocus];
    CGContextDrawImage(naGetCGContextRef([NSGraphicsContext currentContext]), dstRect, cocoaimage);
  [drawImage unlockFocus];
  [NSGraphicsContext restoreGraphicsState];

  [drawImage drawInRect:dstRect];

  return YES;
}



NA_DEF NSImage* na_CreateResolutionIndependentNativeImage(
  const NSView* containingView,
  const NAUIImage* uiImage,
  NAUIImageInteraction interaction,
  NABool secondaryState)
{
  NSImage* image = nil;

  // modern method: Create an image which redraws itself automatically.
  // This is commented out as there have been severe problems with this working
  // on different computers. The context sometimes simply does not seem to be
  // there and returns null, resulting in empty images.
  if(containingView && [NSImage respondsToSelector:@selector(imageWithSize:flipped:drawingHandler:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_8(
      NSSize imageSize = NSMakeSize(naGetUIImage1xSize(uiImage).width, naGetUIImage1xSize(uiImage).height);
      image = [NSImage imageWithSize:imageSize flipped:NO drawingHandler:^BOOL(NSRect dstRect)
      {
        double resolution = naGetWindowBackingScaleFactor([containingView window]) * NA_UIIMAGE_RESOLUTION_SCREEN_1x;
        return na_drawFixedResolutionImage(uiImage, resolution, interaction, secondaryState, imageSize, dstRect);
      }];
    ) // end NA_MACOS_AVAILABILITY_GUARD_10_8
  }
  
  // old method: Just create an image with multiple representations.
  if(!image){
    NASizei imageSize = naGetUIImage1xSize(uiImage);
    image = [[NSImage alloc] initWithSize:NSMakeSize(imageSize.width, imageSize.height)];

    NAUIImageSkin skin = NA_UIIMAGE_SKIN_PLAIN;
    if(uiImage->tintMode != NA_BLEND_ZERO){
      skin = naGetSkinForCurrentAppearance();
    }

    CGImageRef img1x = na_GetUIImageNativeImage(uiImage, NA_UIIMAGE_RESOLUTION_SCREEN_1x, skin, interaction, secondaryState);
    CGImageRef img2x = na_GetUIImageNativeImage(uiImage, NA_UIIMAGE_RESOLUTION_SCREEN_2x, skin, interaction, secondaryState);
    if(img1x){
      NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithCGImage:img1x];
      [image addRepresentation:rep];
      NA_COCOA_RELEASE(rep);
    }
    if(img2x){
      NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithCGImage:img2x];
      [image addRepresentation:rep];
      NA_COCOA_RELEASE(rep);
    }
  }
  
  return image;
}



NA_DEF void naDeallocNativeImage(void* nativeImage){
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
