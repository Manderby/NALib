
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!

#include "NAUICocoaLegacy.h"


// Damnit Apple or clang or whoever responsible for this mess!
#if defined __clang_major__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif



NA_DEF CGContextRef naGetCGContextRef(NSGraphicsContext* graphicsContext){
  CGContextRef cgContext = nil;

  NA_MACOS_AVAILABILITY_GUARD_10_14(
    if([NSGraphicsContext instancesRespondToSelector:@selector(CGContext)]){
      cgContext = [graphicsContext CGContext];
    }
  )
  if(!cgContext){
    cgContext = [graphicsContext graphicsPort];
  }
  return cgContext;
}



NA_DEF CGFloat naGetUIElementBackingScaleFactor(NSView* uiElement){
  return naGetWindowBackingScaleFactor([uiElement window]);
}



NA_DEF CGFloat naGetWindowBackingScaleFactor(NSWindow* window){
  CGFloat uiScale = (CGFloat)1.;

  NA_MACOS_AVAILABILITY_GUARD_10_7(
    if([NSWindow instancesRespondToSelector:@selector(backingScaleFactor)]){
      uiScale = [window backingScaleFactor];
    }
  )
  if(uiScale == 0.){
    uiScale = [window userSpaceScaleFactor];
  }
  if(uiScale == 0.){
    uiScale = 1.;
  }
  
  return uiScale;
}



NA_DEF NABool naLoadNib(const NAUTF8Char* nibName, void* owner){
  NABool loaded = NA_FALSE;
  id cocoaOwner = owner ? (NA_COCOA_BRIDGE id)owner : NSApp;

  NA_MACOS_AVAILABILITY_GUARD_10_8(
    if([NSBundle instancesRespondToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]){
      NSArray * topLevelObjects;
      loaded = [[NSBundle mainBundle] loadNibNamed:[NSString stringWithUTF8String:nibName] owner:cocoaOwner topLevelObjects:&topLevelObjects];
      // Yes, we are retaining the topLevelObjects just like that. Upon closing the app,
      // these will be a leak but who cares at this point.
      (void)NA_COCOA_RETAIN(topLevelObjects);
    }
  )
  if(!loaded){
    loaded = [NSBundle loadNibNamed:[NSString stringWithUTF8String:nibName] owner:cocoaOwner];
  }
  return loaded;
}



NA_DEF void naSwitchApplicationToGraphiteAppearance(void){
  [[NSUserDefaults standardUserDefaults] setVolatileDomain:@{@"AppleAquaColorVariant": @6} forName:NSArgumentDomain];
}



NA_DEF NSColor* naGetLabelColor(){
  NSColor* sRGBColor = nil;
  
  NA_MACOS_AVAILABILITY_GUARD_10_10(
    if([NSColor instancesRespondToSelector:@selector(labelColor)]){
      NSColor* color = [NSColor labelColor];
      sRGBColor = [color colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
    }
  )
  if(!sRGBColor){
    NSColor* color = [NSColor controlTextColor];
    sRGBColor = [color colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
  }
  return sRGBColor;
}

NA_DEF NSColor* naGetLinkColor(){
  NSColor* sRGBColor = nil;

  // documentation says available since 10.10 but that is not true. It is at least 10.12 but maybe even higher.
  NA_MACOS_AVAILABILITY_GUARD_10_12(
    if([NSColor instancesRespondToSelector:@selector(linkColor)]){
      NSColor* color = [NSColor linkColor];
      sRGBColor = [color colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
    }
  )
  if(!sRGBColor){
    NSColor* color = [NSColor blueColor];
    sRGBColor = [color colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
  }
  return sRGBColor;
}

NA_DEF NSColor* naGetAccentColor(){
  NSColor* sRGBColor = nil;

  NA_MACOS_AVAILABILITY_GUARD_10_14(
    if([NSColor respondsToSelector:@selector(controlAccentColor)]){
      NSColor* color = [NSColor controlAccentColor];
      sRGBColor = [color colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
    }
  )
  if(!sRGBColor){
    NSColor* color = [NSColor blueColor];
    sRGBColor = [color colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
  }
  return sRGBColor;
}

#if defined __clang_major__
  #pragma clang diagnostic pop
#endif


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
