
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!

#ifndef NA_UI_COCOA_LEGACY_INCLUDED
#define NA_UI_COCOA_LEGACY_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "Availability.h"

#if __clang_major__ >= 8  // Supposedly, clang 8 is needed for @available.
  #define NA_MACOS_AVAILABILITY_GUARD_10_6(code)  if(@available(macOS 10.6, *)) { code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_7(code)  if(@available(macOS 10.7, *)) { code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_8(code)  if(@available(macOS 10.8, *)) { code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_9(code)  if(@available(macOS 10.9, *)) { code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_10(code) if(@available(macOS 10.10, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_11(code) if(@available(macOS 10.11, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_12(code) if(@available(macOS 10.12, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_13(code) if(@available(macOS 10.13, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_14(code) if(@available(macOS 10.14, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_15(code) if(@available(macOS 10.15, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_10_16(code) if(@available(macOS 10.16, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_11_0(code)  if(@available(macOS 11.0, *)){ code }
  #define NA_MACOS_AVAILABILITY_GUARD_11_1(code)  if(@available(macOS 11.1, *)){ code }
#else
  #if defined __MAC_10_6
    #define NA_MACOS_AVAILABILITY_GUARD_10_6(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_6(code)  if(0){ }
  #endif

  #if defined __MAC_10_7
    #define NA_MACOS_AVAILABILITY_GUARD_10_7(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_7(code)  if(0){ }
  #endif

  #if defined __MAC_10_8
    #define NA_MACOS_AVAILABILITY_GUARD_10_8(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_8(code)  if(0){ }
  #endif

  #if defined __MAC_10_9
    #define NA_MACOS_AVAILABILITY_GUARD_10_9(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_9(code)  if(0){ }
  #endif

  #if defined __MAC_10_10
    #define NA_MACOS_AVAILABILITY_GUARD_10_10(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_10(code)  if(0){ }
  #endif

  #if defined __MAC_10_11
    #define NA_MACOS_AVAILABILITY_GUARD_10_11(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_11(code)  if(0){ }
  #endif

  #if defined __MAC_10_12
    #define NA_MACOS_AVAILABILITY_GUARD_10_12(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_12(code)  if(0){ }
  #endif

  #if defined __MAC_10_13
    #define NA_MACOS_AVAILABILITY_GUARD_10_13(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_13(code)  if(0){ }
  #endif

  #if defined __MAC_10_14
    #define NA_MACOS_AVAILABILITY_GUARD_10_14(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_14(code)  if(0){ }
  #endif

  #if defined __MAC_10_15
    #define NA_MACOS_AVAILABILITY_GUARD_10_15(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_15(code)  if(0){ }
  #endif

  #if defined __MAC_10_16
    #define NA_MACOS_AVAILABILITY_GUARD_10_16(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_10_16(code)  if(0){ }
  #endif

  #if defined __MAC_11_0
    #define NA_MACOS_AVAILABILITY_GUARD_11_0(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_11_0(code)  if(0){ }
  #endif

  #if defined __MAC_11_1
    #define NA_MACOS_AVAILABILITY_GUARD_11_1(code)  if(1){ code }
  #else
    #define NA_MACOS_AVAILABILITY_GUARD_11_1(code)  if(0){ }
  #endif
#endif




// Mapping of deprecated entities

// For reference, here are how the different button states behave:
// name                    recessed   stateful   toggles main/alt
// NAButtonTypeMomentaryPushIn   N          N          N
// NAButtonTypeMomentaryLight    Y          N          N
// NAButtonTypeOnOff             N          Y          N
// NAButtonTypeMomentaryChange   N          N          Y
// NAButtonTypePushOnPushOff     Y          Y          N
// NAButtonTypeToggle            N          Y          Y
// NAButtonTypeSwitch            = NAButtonTypeToggle but borderless
// NAButtonTypeRadio             = NAButtonTypeSwitch but constrained

#if defined __MAC_10_7
  #define NATextAlignmentLeft             NSTextAlignmentLeft
  #define NATextAlignmentRight            NSTextAlignmentRight
  #define NATextAlignmentCenter           NSTextAlignmentCenter

#else // deprecated definitions before 10.7
  #define NATextAlignmentLeft             NSLeftTextAlignment
  #define NATextAlignmentRight            NSRightTextAlignment
  #define NATextAlignmentCenter           NSCenterTextAlignment
#endif

#if defined __MAC_10_12
  #define NAButtonTypeMomentaryLight      NSButtonTypeMomentaryLight
#elif defined __MAC_10_9
  #define NAButtonTypeMomentaryLight      NSMomentaryLightButton
#else // deprecated definitions before 10.9
  #define NAButtonTypeMomentaryLight      NSMomentaryPushButton // yes, this was reversed with the push button
#endif

#if defined __MAC_10_12
  #define NABezelStyleRegularSquare       NSBezelStyleRegularSquare

  #define NAEventTypeKeyUp                NSEventTypeKeyUp
  #define NAEventTypeKeyDown              NSEventTypeKeyDown
  #define NAEventTypeFlagsChanged         NSEventTypeFlagsChanged

  #define NAEventMaskAny                  NSEventMaskAny
  #define NAWindowStyleMaskTitled         NSWindowStyleMaskTitled
  #define NAWindowStyleMaskClosable       NSWindowStyleMaskClosable
  #define NAWindowStyleMaskMiniaturizable NSWindowStyleMaskMiniaturizable
  #define NAWindowStyleMaskResizable      NSWindowStyleMaskResizable
  #define NAWindowStyleMaskBorderless     NSWindowStyleMaskBorderless
  #define NAWindowStyleMaskHUDWindow      NSWindowStyleMaskHUDWindow
  #define NAWindowStyleMaskFullScreen     NSWindowStyleMaskFullScreen
  #define NAWindowStyleMaskNonactivatingPanel NSWindowStyleMaskNonactivatingPanel // Only supported for NSPanel
  #define NAWindowStyleMaskUtilityWindow  NSWindowStyleMaskUtilityWindow // Only supported for NSPanel
  #define NAEventModifierFlagShift        NSEventModifierFlagShift
  #define NAEventModifierFlagOption       NSEventModifierFlagOption
  #define NAEventModifierFlagControl      NSEventModifierFlagControl
  #define NAEventModifierFlagCommand      NSEventModifierFlagCommand

  #define NAAlertStyleWarning             NSAlertStyleInformational
  #define NAAlertStyleInfo                NSAlertStyleWarning
  #define NSAlertStyleError               NSAlertStyleCritical
#else // deprecated definitions before 10.12
  #define NABezelStyleRegularSquare       NSRegularSquareBezelStyle

  #define NAEventTypeKeyUp                NSKeyUp
  #define NAEventTypeKeyDown              NSKeyDown
  #define NAEventTypeFlagsChanged         NSFlagsChanged

  #define NAEventMaskAny                  NSAnyEventMask
  #define NAWindowStyleMaskTitled         NSTitledWindowMask
  #define NAWindowStyleMaskClosable       NSClosableWindowMask
  #define NAWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
  #define NAWindowStyleMaskResizable      NSResizableWindowMask
  #define NAWindowStyleMaskBorderless     NSBorderlessWindowMask
  #define NAWindowStyleMaskHUDWindow      1 << 13
  #define NAWindowStyleMaskFullScreen     NSFullScreenWindowMask
  #define NAEventModifierFlagShift        NSShiftKeyMask
  #define NAEventModifierFlagOption       NSAlternateKeyMask
  #define NAEventModifierFlagControl      NSControlKeyMask
  #define NAEventModifierFlagCommand      NSCommandKeyMask

  #define NAAlertStyleWarning             NSInformationalAlertStyle
  #define NAAlertStyleInfo                NSWarningAlertStyle
  #define NSAlertStyleError               NSCriticalAlertStyle
#endif

#if defined __MAC_10_14
  #define NAButtonTypeRadio               NSButtonTypeRadio
  #define NAButtonTypeSwitch              NSButtonTypeSwitch
  #define NAButtonTypePushOnPushOff       NSButtonTypePushOnPushOff
  #define NAButtonTypeToggle              NSButtonTypeToggle
  #define NAButtonTypeMomentaryChange     NSButtonTypeMomentaryChange
  #define NAButtonTypeOnOff               NSButtonTypeOnOff
  #define NAButtonTypeMomentaryPushIn     NSButtonTypeMomentaryPushIn

  #define NABezelStyleRounded             NSBezelStyleRounded
  #define NABezelStyleShadowlessSquare    NSBezelStyleShadowlessSquare

  #define NAStateOn                       NSControlStateValueOn
  #define NAStateOff                      NSControlStateValueOff
  
  #define NAOpenGLContextParameterSwapInterval  NSOpenGLContextParameterSwapInterval
#else // deprecated definitions before 10.14
  #define NAButtonTypeRadio               NSRadioButton
  #define NAButtonTypeSwitch              NSSwitchButton
  #define NAButtonTypePushOnPushOff       NSPushOnPushOffButton
  #define NAButtonTypeToggle              NSToggleButton
  #define NAButtonTypeMomentaryChange     NSMomentaryChangeButton

  #define NABezelStyleRounded             NSRoundedBezelStyle
  #define NABezelStyleShadowlessSquare    NSShadowlessSquareBezelStyle

  #define NAStateOn                       NSOnState
  #define NAStateOff                      NSOffState

#define NAOpenGLContextParameterSwapInterval  NSOpenGLCPSwapInterval
#endif




NA_API CGContextRef naGetCGContextRef(NSGraphicsContext* graphicsContext);
NA_API CGFloat naGetUIElementBackingScaleFactor(NSView* uiElement);
NA_API CGFloat naGetWindowBackingScaleFactor(NSWindow* window);
NA_API NABool naLoadNib(const NAUTF8Char* nibName);
NA_API NSColor* naGetLabelColor(void);
NA_API NSColor* naGetLinkColor(void);



#ifdef __cplusplus
  }
#endif
#endif // NA_UI_COCOA_LEGACY_INCLUDED


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
