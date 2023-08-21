
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

  // Elements introduced in 10.7. No replacement possible.
  #define NABezelStyleInline              NSBezelStyleInline
  #define NAWindowCollectionBehaviorFullScreenAuxiliary NSWindowCollectionBehaviorFullScreenAuxiliary

#else // deprecated definitions before 10.7
  #define NATextAlignmentLeft             NSLeftTextAlignment
  #define NATextAlignmentRight            NSRightTextAlignment
  #define NATextAlignmentCenter           NSCenterTextAlignment

  // Elements introduced in 10.7. No replacement possible.
  #define NABezelStyleInline              NABezelStyleRounded
  #define NAWindowCollectionBehaviorFullScreenAuxiliary 0
#endif



#if defined __MAC_10_12
  #define NAButtonTypeMomentaryLight      NSButtonTypeMomentaryLight
#elif defined __MAC_10_9
  #define NAButtonTypeMomentaryLight      NSMomentaryLightButton
#else // deprecated definitions before 10.9
  // yes, this was reversed with the push button
  #define NAButtonTypeMomentaryLight      NSMomentaryPushButton
#endif



#if defined __MAC_10_12
  #define NAAlertStyleError               NSAlertStyleCritical
  #define NAAlertStyleInfo                NSAlertStyleWarning
  #define NAAlertStyleWarning             NSAlertStyleInformational
  #define NABezelStyleRegularSquare       NSBezelStyleRegularSquare
  #define NACompositePlusDarker           NSCompositingOperationPlusDarker
  #define NADeviceIndependentModifierFlagsMask NSEventModifierFlagDeviceIndependentFlagsMask
  #define NAEventMaskAny                  NSEventMaskAny
  #define NAEventModifierFlagCommand      NSEventModifierFlagCommand
  #define NAEventModifierFlagControl      NSEventModifierFlagControl
  #define NAEventModifierFlagFunction     NSEventModifierFlagFunction
  #define NAEventModifierFlagOption       NSEventModifierFlagOption
  #define NAEventModifierFlagShift        NSEventModifierFlagShift
  #define NAEventTypeFlagsChanged         NSEventTypeFlagsChanged
  #define NAEventTypeLeftMouseUp          NSEventTypeLeftMouseUp
  #define NAEventTypeKeyDown              NSEventTypeKeyDown
  #define NAEventTypeKeyUp                NSEventTypeKeyUp
  #define NAWindowStyleMaskBorderless     NSWindowStyleMaskBorderless
  #define NAWindowStyleMaskClosable       NSWindowStyleMaskClosable
  #define NAWindowStyleMaskFullScreen     NSWindowStyleMaskFullScreen
  #define NAWindowStyleMaskHUDWindow      NSWindowStyleMaskHUDWindow
  #define NAWindowStyleMaskMiniaturizable NSWindowStyleMaskMiniaturizable
  #define NAWindowStyleMaskNonactivatingPanel NSWindowStyleMaskNonactivatingPanel // Only supported for NSPanel
  #define NAWindowStyleMaskResizable      NSWindowStyleMaskResizable
  #define NAWindowStyleMaskTitled         NSWindowStyleMaskTitled
  #define NAWindowStyleMaskUtilityWindow  NSWindowStyleMaskUtilityWindow // Only supported for NSPanel
#else // deprecated definitions before 10.12
  #define NAAlertStyleError               NSCriticalAlertStyle
  #define NAAlertStyleInfo                NSWarningAlertStyle
  #define NAAlertStyleWarning             NSInformationalAlertStyle
  #define NABezelStyleRegularSquare       NSRegularSquareBezelStyle
  #define NACompositePlusDarker           NSCompositePlusDarker
  #define NADeviceIndependentModifierFlagsMask NSDeviceIndependentModifierFlagsMask
  #define NAEventMaskAny                  NSAnyEventMask
  #define NAEventModifierFlagCommand      NSCommandKeyMask
  #define NAEventModifierFlagControl      NSControlKeyMask
  #define NAEventModifierFlagFunction     NSFunctionKeyMask
  #define NAEventModifierFlagOption       NSAlternateKeyMask
  #define NAEventModifierFlagShift        NSShiftKeyMask
  #define NAEventTypeFlagsChanged         NSFlagsChanged
  #define NAEventTypeLeftMouseUp          NSLeftMouseUp
  #define NAEventTypeKeyDown              NSKeyDown
  #define NAEventTypeKeyUp                NSKeyUp
  #define NAWindowStyleMaskBorderless     NSBorderlessWindowMask
  #define NAWindowStyleMaskClosable       NSClosableWindowMask
  #define NAWindowStyleMaskFullScreen     NSFullScreenWindowMask
  #define NAWindowStyleMaskHUDWindow      1 << 13
  #define NAWindowStyleMaskMiniaturizable NSMiniaturizableWindowMask
  #define NAWindowStyleMaskResizable      NSResizableWindowMask
  #define NAWindowStyleMaskTitled         NSTitledWindowMask
#endif



#if defined __MAC_10_14
  #define NABezelStyleRounded             NSBezelStyleRounded
  #define NABezelStyleShadowlessSquare    NSBezelStyleShadowlessSquare
  #define NAButtonTypeMomentaryChange     NSButtonTypeMomentaryChange
  #define NAButtonTypeMomentaryPushIn     NSButtonTypeMomentaryPushIn
  #define NAButtonTypeOnOff               NSButtonTypeOnOff
  #define NAButtonTypePushOnPushOff       NSButtonTypePushOnPushOff
  #define NAButtonTypeRadio               NSButtonTypeRadio
  #define NAButtonTypeSwitch              NSButtonTypeSwitch
  #define NAButtonTypeToggle              NSButtonTypeToggle
  #define NAOpenGLContextParameterSwapInterval  NSOpenGLContextParameterSwapInterval
  #define NAStateOff                      NSControlStateValueOff
  #define NAStateOn                       NSControlStateValueOn
#else // deprecated definitions before 10.14
  #define NABezelStyleRounded             NSRoundedBezelStyle
  #define NABezelStyleShadowlessSquare    NSShadowlessSquareBezelStyle
  #define NAButtonTypeMomentaryChange     NSMomentaryChangeButton
  #define NAButtonTypePushOnPushOff       NSPushOnPushOffButton
  #define NAButtonTypeRadio               NSRadioButton
  #define NAButtonTypeSwitch              NSSwitchButton
  #define NAButtonTypeToggle              NSToggleButton
  #define NAOpenGLContextParameterSwapInterval  NSOpenGLCPSwapInterval
  #define NAStateOff                      NSOffState
  #define NAStateOn                       NSOnState
#endif



NA_API CGContextRef naGetCGContextRef(NSGraphicsContext* graphicsContext);
NA_API CGFloat naGetUIElementBackingScaleFactor(NSView* uiElement);
NA_API CGFloat naGetWindowBackingScaleFactor(NSWindow* window);
NA_API NSColor* naGetLabelColor(void);
NA_API NSColor* naGetLinkColor(void);
NA_API NSColor* naGetAccentColor(void);



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
