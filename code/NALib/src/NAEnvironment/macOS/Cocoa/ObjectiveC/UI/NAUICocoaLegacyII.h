
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#include "Availability.h"

#if __clang_major__ >= 5  // Supposedly, clang 5 is needed for @available.
  #define NA_MACOS_AVAILABILITY_GUARD_10_6(code)  if(@available(macOS 10.6, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_7(code)  if(@available(macOS 10.7, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_8(code)  if(@available(macOS 10.8, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_9(code)  if(@available(macOS 10.9, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_10(code) if(@available(macOS 10.10, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_11(code) if(@available(macOS 10.11, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_12(code) if(@available(macOS 10.12, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_13(code) if(@available(macOS 10.13, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_14(code) if(@available(macOS 10.14, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_15(code) if(@available(macOS 10.15, *)){ code }
#else
  #define NA_MACOS_AVAILABILITY_GUARD_10_6(code)  if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_7(code)  if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_8(code)  if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_9(code)  if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_10(code) if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_11(code) if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_12(code) if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_13(code) if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_14(code) if(0){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_15(code) if(0){ code }
#endif



// Mapping of deprecated entities
#if defined __MAC_10_7
  #define NAEventTypeKeyUp                NSEventTypeKeyUp
  #define NAEventTypeKeyDown              NSEventTypeKeyDown

  #define NAButtonTypeRadio               NSButtonTypeRadio
  #define NAButtonTypeMomentaryLight      NSButtonTypeMomentaryLight
  #define NAButtonTypePushOnPushOff       NSButtonTypePushOnPushOff
  #define NAButtonTypeSwitch              NSButtonTypeSwitch
  
  #define NATextAlignmentLeft             NSTextAlignmentLeft
  #define NATextAlignmentRight            NSTextAlignmentRight
  #define NATextAlignmentCenter           NSTextAlignmentCenter
#else // deprecated definitions before 10.7
  #define NAEventTypeKeyUp                NSKeyUp
  #define NAEventTypeKeyDown              NSKeyDown
  
  #define NAButtonTypeRadio               NSRadioButton
  #define NAButtonTypeMomentaryLight      NSMomentaryLightButton
  #define NAButtonTypePushOnPushOff       NSPushOnPushOffButton
  #define NAButtonTypeSwitch              NSSwitchButton

  #define NATextAlignmentLeft             NSLeftTextAlignment
  #define NATextAlignmentRight            NSRightTextAlignment
  #define NATextAlignmentCenter           NSCenterTextAlignment
#endif

#if defined __MAC_10_12
  #define NAEventMaskAny                  NSEventMaskAny
  #define NAWindowStyleMaskTitled         NSWindowStyleMaskTitled
  #define NAWindowStyleMaskClosable       NSWindowStyleMaskClosable
  #define NAWindowStyleMaskMiniaturizable NSWindowStyleMaskMiniaturizable
  #define NAWindowStyleMaskResizable      NSWindowStyleMaskResizable
  #define NAWindowStyleMaskBorderless     NSWindowStyleMaskBorderless
  #define NAEventModifierFlagShift        NSEventModifierFlagShift
  #define NAEventModifierFlagOption       NSEventModifierFlagOption
  #define NAEventModifierFlagControl      NSEventModifierFlagControl
  #define NAEventModifierFlagCommand      NSEventModifierFlagCommand

  #define NAAlertStyleWarning             NSAlertStyleInformational
  #define NAAlertStyleInfo                NSAlertStyleWarning
  #define NSAlertStyleError               NSAlertStyleCritical
#else // deprecated definitions before 10.12
  #define NAEventMaskAny                  NSAnyEventMask
  #define NAWindowStyleMaskTitled         NSTitledWindowMask
  #define NAWindowStyleMaskClosable       NSClosableWindowMask
  #define NAWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
  #define NAWindowStyleMaskResizable      NSResizableWindowMask
  #define NAWindowStyleMaskBorderless     NSBorderlessWindowMask
  #define NAEventModifierFlagShift        NSShiftKeyMask
  #define NAEventModifierFlagOption       NSAlternateKeyMask
  #define NAEventModifierFlagControl      NSControlKeyMask
  #define NAEventModifierFlagCommand      NSCommandKeyMask

  #define NAAlertStyleWarning             NSInformationalAlertStyle
  #define NAAlertStyleInfo                NSWarningAlertStyle
  #define NSAlertStyleError               NSCriticalAlertStyle
#endif

#if defined __MAC_10_14
  #define NABezelStyleRounded             NSBezelStyleRounded
  #define NABezelStyleShadowlessSquare    NSBezelStyleShadowlessSquare

  #define NAStateOn                       NSControlStateValueOn
  #define NAStateOff                      NSControlStateValueOff
  
  #define NAOpenGLContextParameterSwapInterval  NSOpenGLContextParameterSwapInterval
#else // deprecated definitions before 10.14
  #define NABezelStyleRounded             NSRoundedBezelStyle
  #define NABezelStyleShadowlessSquare    NSShadowlessSquareBezelStyle

  #define NAStateOn                       NSOnState
  #define NAStateOff                      NSOffState

#define NAOpenGLContextParameterSwapInterval  NSOpenGLCPSwapInterval
#endif



// Damnit Apple or clang or whoever responsible for this mess!
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"



CGContextRef naGetCGContextRef(NSGraphicsContext* graphicsContext){
  if([graphicsContext respondsToSelector:@selector(CGContext)]){
    NA_MACOS_AVAILABILITY_GUARD_10_14(
      return [graphicsContext CGContext];
    )else{return nil;}
  }else{
    return [graphicsContext graphicsPort];
  }
}



NAUIImageSkin naGetSkinForCurrentAppearance(void){
  NAUIImageSkin skin = NA_UIIMAGE_SKIN_LIGHT;
  if([NSAppearance respondsToSelector:@selector(currentAppearance)]){
    NSAppearanceName appearancename = NSAppearanceNameAqua;
    if([[NSAppearance currentAppearance] respondsToSelector:@selector(name)]){
      NA_MACOS_AVAILABILITY_GUARD_10_9(
        appearancename = [[NSAppearance currentAppearance] name];
      )
    }
    NA_MACOS_AVAILABILITY_GUARD_10_10(
      if(appearancename == NSAppearanceNameVibrantDark){skin = NA_UIIMAGE_SKIN_DARK;}
    )
    NA_MACOS_AVAILABILITY_GUARD_10_14(
      if(appearancename == NSAppearanceNameDarkAqua
      || appearancename == NSAppearanceNameAccessibilityHighContrastDarkAqua
      || appearancename == NSAppearanceNameAccessibilityHighContrastVibrantDark){
        skin = NA_UIIMAGE_SKIN_DARK;}
    )
  }
  return skin;
}



CGFloat naGetWindowBackingScaleFactor(NSWindow* window){
  CGFloat res = (CGFloat)1.;

  if([window respondsToSelector:@selector(backingScaleFactor)]){
    NA_MACOS_AVAILABILITY_GUARD_10_7(
      res = [window backingScaleFactor];
    )
  }else{
    res = [window userSpaceScaleFactor];
  }
  return res;
}



NABool naLoadNib(NAUTF8Char* nibName){
  if([NSBundle respondsToSelector:@selector(loadNibNamed:owner:topLevelObjects:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_8(
      return [[NSBundle mainBundle] loadNibNamed:[NSString stringWithUTF8String:nibName] owner:NSApp topLevelObjects:nil];
    )else{return NA_FALSE;}
  }else{
    return [NSBundle loadNibNamed:[NSString stringWithUTF8String:nibName] owner:NSApp];
  }
}


#pragma clang diagnostic pop


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
