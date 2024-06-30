
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../Core/NAAppCore.h"
#include "../../../NAUtility/NAMemory.h"
#include "../../../NAMath/NACoord.h"
#include "../../../NAUtility/NAThreading.h"
#include "../../../NAUtility/NATranslator.h"
#include "../../../NAUtility/NAValueHelper.h"

// the following import is needed for UTType definitions.
#import <UniformTypeIdentifiers/UTType.h>
#import <Carbon/Carbon.h>

NA_HAPI NARect na_GetNativeWindowAbsoluteInnerRect(const NSWindow* window);



#define naDefineCocoaObject(cocoatype, var, uiElement)\
  cocoatype* var = (NA_COCOA_BRIDGE cocoatype*)(naGetUIElementNativePtr(uiElement))
  
#define naDefineCocoaObjectConst(cocoatype, var, uiElement)\
  cocoatype* var = (NA_COCOA_BRIDGE cocoatype*)(naGetUIElementNativePtrConst(uiElement))


NA_HDEF void na_ClearUINativePtr(NANativePtr nativePtr) {
  NA_COCOA_RELEASE(NA_COCOA_PTR_C_TO_OBJC(nativePtr));
}



NA_HDEF void na_SetUIElementParent(NA_UIElement* uiElement, void* parent, NABool isElementAttachable) {
  NA_UNUSED(isElementAttachable);
  NA_UIElement* elem = (NA_UIElement*)uiElement;
  // todo: remove from old parent
  elem->parent = parent;
}


#ifndef NSAppKitVersionNumber11_0
  #define NSAppKitVersionNumber11_0 2022
#endif



NA_HDEF double na_GetUIElementXOffset(const NA_UIElement* elem) {
  if(naGetUIElementType(elem) == NA_UI_BUTTON) {
    if(naIsButtonBordered((const NAButton*)elem)) {
      if(isAtLeastMacOSVersion(11, 0)) {
        // On newer systems bordered buttons are 5 units shorter than expected on
        // the left and right. Therefore, we add 10 units and in naAddSpaceChild we
        // move the button 5 units to the left.
        return -5.;
      }
    }
  }
  return 0.;
}



NA_HDEF double na_GetUIElementYOffset(const NA_UIElement* elem) {
  // Line height is considered to be 25 for an optimal display. In this
  // function, the UI elements are shifted in Y direction such that text
  // always is displayed on a common baseline. The reference element is
  // a stateful text button.
  // All spaces and stateful/image buttons have offset 0.
  
  if(NSAppKitVersionNumber < NSAppKitVersionNumber11_0) {
    switch(naGetUIElementType(elem)) {
    case NA_UI_APPLICATION:  return  0.;
    case NA_UI_BUTTON:       return  0.;
    case NA_UI_CHECKBOX:     return +5.;
    case NA_UI_IMAGE_SPACE:  return  0.;
    case NA_UI_LABEL:        return +6.;
    case NA_UI_MENU:         return  0.;
    case NA_UI_MENUITEM:     return  0.;
    case NA_UI_METAL_SPACE:  return  0.;
    case NA_UI_OPENGL_SPACE: return  0.;
    case NA_UI_RADIO:        return +5.;
    case NA_UI_SCREEN:       return  0.;
    case NA_UI_SELECT:       return +2.;
    case NA_UI_SLIDER:       return +1.;
    case NA_UI_SPACE:        return  0.;
    case NA_UI_TEXTBOX:      return +1.;
    case NA_UI_TEXTFIELD:    return +4.;
    case NA_UI_WINDOW:       return  0.;
    default: return 0.;
    }
  }else{
    switch(naGetUIElementType(elem)) {
    case NA_UI_APPLICATION:  return  0.;
    case NA_UI_BUTTON:       return  0.;
    case NA_UI_CHECKBOX:     return +5.;
    case NA_UI_IMAGE_SPACE:  return  0.;
    case NA_UI_LABEL:        return +5.;
    case NA_UI_MENU:         return  0.;
    case NA_UI_MENUITEM:     return  0.;
    case NA_UI_METAL_SPACE:  return  0.;
    case NA_UI_OPENGL_SPACE: return  0.;
    case NA_UI_RADIO:        return +5.;
    case NA_UI_SCREEN:       return  0.;
    case NA_UI_SELECT:       return -1.;
    case NA_UI_SLIDER:       return -1.;
    case NA_UI_SPACE:        return  0.;
    case NA_UI_TEXTBOX:      return +1.;
    case NA_UI_TEXTFIELD:    return +3.;
    case NA_UI_WINDOW:       return  0.;
    default: return 0.;
    }
  }
}



NA_HDEF void na_CaptureKeyboardStatus(NSEvent* event) {
  NSUInteger flags;
  NABool hasShift;
  NABool hasControl;
  NABool hasOption;
  NABool hasCommand;
  NAUIKeyCode keyCode = [event keyCode];
  naGetApplication()->curKeyStroke.keyCode = keyCode;
  [event modifierFlags];
  flags = (NSUInteger)[event modifierFlags];
  hasShift     = (flags & NAEventModifierFlagShift)   != 0;
  hasControl   = (flags & NAEventModifierFlagControl) != 0;
  hasOption    = (flags & NAEventModifierFlagOption)  != 0;
  hasCommand   = (flags & NAEventModifierFlagCommand) != 0;
  naGetApplication()->curKeyStroke.modifiers = 0;
  naGetApplication()->curKeyStroke.modifiers |= (uint32)hasShift * NA_MODIFIER_FLAG_SHIFT;
  naGetApplication()->curKeyStroke.modifiers |= (uint32)hasControl * NA_MODIFIER_FLAG_CONTROL;
  naGetApplication()->curKeyStroke.modifiers |= (uint32)hasOption * NA_MODIFIER_FLAG_OPTION;
  naGetApplication()->curKeyStroke.modifiers |= (uint32)hasCommand * NA_MODIFIER_FLAG_COMMAND;
}



NA_HDEF NABool na_InterceptKeyboardShortcut(NSEvent* event) {
  NABool retValue = NA_FALSE;
  if([event type] == NAEventTypeKeyDown || [event type] == NAEventTypeFlagsChanged) {
    NA_UIElement* elem;
    NSWindow* focusWindow;
    na_CaptureKeyboardStatus(event);
    
    // Search for the first responder which is represented in NALib.
    elem = NA_NULL;
    focusWindow = [NSApp keyWindow];
    if(focusWindow) {
      NSResponder* firstResponder = [focusWindow firstResponder];
      if(firstResponder) {
        while(!elem && firstResponder) {
          elem = na_GetUINALibEquivalent((NA_COCOA_BRIDGE NANativePtr)(firstResponder));
          if(!elem) {
            if(firstResponder == focusWindow) {
              elem = &(naGetApplication()->uiElement);
            }else{
              firstResponder = [(NSView*)firstResponder superview];
            }
          }
        }
      }else{
        elem = na_GetUINALibEquivalent((NA_COCOA_BRIDGE NANativePtr)(focusWindow));
      }
    }else{
      elem = &(naGetApplication()->uiElement);
    }
    
    // Search for a matching keyboard shortcut by bubbling.
    while(!retValue && elem) {
      NAListIterator iter = naMakeListAccessor(&(elem->shortcuts));
      while(!retValue && naIterateList(&iter)) {
        const NAKeyboardShortcutReaction* keyReaction = naGetListCurConst(&iter);
        if(keyReaction->shortcut.keyCode == naGetApplication()->curKeyStroke.keyCode) {
          NABool needsShift   = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool needsControl = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool needsOption  = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool needsCommand = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_COMMAND);
          NABool hasShift   = naGetFlagu32(naGetApplication()->curKeyStroke.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool hasControl = naGetFlagu32(naGetApplication()->curKeyStroke.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool hasOption  = naGetFlagu32(naGetApplication()->curKeyStroke.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool hasCommand = naGetFlagu32(naGetApplication()->curKeyStroke.modifiers, NA_MODIFIER_FLAG_COMMAND);
          if(needsShift   == hasShift
          && needsControl == hasControl
          && needsOption  == hasOption
          && needsCommand == hasCommand) {
            NAReaction reaction;
            reaction.uiElement = naGetApplication();
            reaction.command = NA_UI_COMMAND_KEYBOARD_SHORTCUT;
            reaction.controller = keyReaction->controller;
            keyReaction->callback(reaction);
            retValue = NA_TRUE;
          }
        }
      }
      naClearListIterator(&iter);
      elem = naGetUIElementParent(elem);
    }
  }
  return retValue;
}



NAString* naNewKeyPressString(uint32 modifiers, NAUIKeyCode keyCode) {
  TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardLayoutInputSource();
  CFDataRef layoutData = TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
  const UCKeyboardLayout* keyboardLayout = (const UCKeyboardLayout*)CFDataGetBytePtr(layoutData);
  
  uint32 keysDown = 0;
  UniChar chars[4];
  UniCharCount realLength;
                
  UInt32 modifierKeyState = 0;
  if(modifiers & NA_MODIFIER_FLAG_SHIFT)  { modifierKeyState |= shiftKey; }
  if(modifiers & NA_MODIFIER_FLAG_CONTROL) { modifierKeyState |= controlKey; }
  if(modifiers & NA_MODIFIER_FLAG_OPTION) { modifierKeyState |= optionKey; }
  if(modifiers & NA_MODIFIER_FLAG_COMMAND) { modifierKeyState |= cmdKey; }

  UCKeyTranslate(
    keyboardLayout,
    (UInt16)keyCode,
    kUCKeyActionDisplay,
    modifierKeyState >> 8,
    LMGetKbdType(),
    kUCKeyTranslateNoDeadKeysBit,
    &keysDown,
    sizeof(chars) / sizeof(chars[0]),
    &realLength,
    chars);
  CFRelease(currentKeyboard);
    
  NAUTF8Char utf8String[10];
  CFStringRef letterCFString = CFStringCreateWithCharacters(kCFAllocatorDefault, chars, 1);
  CFStringGetCString(letterCFString, utf8String, 10, kCFStringEncodingUTF8);
  CFRelease(letterCFString);
  return naNewStringWithFormat("%s", utf8String);
}



// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////


NA_DEF void na_RefreshUIElementNow(void* uiElement) {
  naDefineCocoaObjectConst(NSView, cocoaView, uiElement);
  [cocoaView setNeedsDisplay:YES];
}




NA_DEF void naSetUIElementNextTabElement(void* uiElement, const void* nextTabElem) {
  if(  naGetUIElementType(uiElement) != NA_UI_TEXTFIELD
    && naGetUIElementType(uiElement) != NA_UI_TEXTBOX) {
    #if NA_DEBUG
      naError("elem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  if(  naGetUIElementType(nextTabElem) != NA_UI_TEXTFIELD
    && naGetUIElementType(nextTabElem) != NA_UI_TEXTBOX) {
    #if NA_DEBUG
      naError("nextTabElem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  naDefineCocoaObject(NSView, cocoaCurTabElem, uiElement);
  naDefineCocoaObjectConst(NSView, cocoaNextTabElem, nextTabElem);
  [cocoaCurTabElem setNextKeyView:cocoaNextTabElem];
}




NA_DEF double naGetUIElementResolutionFactor(const void* uiElement) {
  if(naGetUIElementType(uiElement) == NA_UI_APPLICATION)
    return 1.;
    
  if(naGetUIElementType(uiElement) == NA_UI_SCREEN) {
    #if NA_DEBUG
      naError("Sorry, was too lazy to implement this. I mean, it's not hard, but as screens are not used anyway at the moment, who cares.");
    #endif
    return 1.;
  }
  
  const void* parent = naGetUIElementParentConst(uiElement);
  while(naGetUIElementType(uiElement) != NA_UI_WINDOW && parent) {
    uiElement = parent;
    parent = naGetUIElementParentConst(uiElement);
  }
  
  if(naGetUIElementType(uiElement) == NA_UI_WINDOW) {
    return naGetWindowBackingScaleFactor(NA_COCOA_PTR_C_TO_OBJC(naGetUIElementNativePtrConst(uiElement)));
  }else{
    return naGetUIElementBackingScaleFactor(NA_COCOA_PTR_C_TO_OBJC(naGetUIElementNativePtrConst(uiElement)));
  }
}



NA_HDEF void na_DestructFont(NAFont* font) {
  NA_COCOA_RELEASE(NA_COCOA_PTR_C_TO_OBJC(font->nativePtr));
  naDelete(font->name);
}

NA_DEF NAFont* naCreateFont(const NAUTF8Char* fontFamilyName, uint32 flags, double size) {
  NAFont* font = naCreate(NAFont);
  NSString* systemFontName = [NSString stringWithUTF8String: fontFamilyName];

  NSFont* systemFont = [NSFont systemFontOfSize:[NSFont systemFontSize]];
  NSFont* retFont;

  if(systemFontName == [systemFont familyName]) {
    retFont = (naGetFlagu32(flags, NA_FONT_FLAG_BOLD)) ?
      [NSFont systemFontOfSize:size] :
      [NSFont boldSystemFontOfSize:size];
  }else{
    NSFontTraitMask traits = 0;
    if(naGetFlagu32(flags, NA_FONT_FLAG_BOLD)) { traits |= NSBoldFontMask; }
    if(naGetFlagu32(flags, NA_FONT_FLAG_ITALIC)) { traits |= NSItalicFontMask; }
    if(naGetFlagu32(flags, NA_FONT_FLAG_UNDERLINE)) { }

    retFont = [[NSFontManager sharedFontManager]
      fontWithFamily:systemFontName
      traits:traits
      weight:5  // ignored if NSBoldFontMask is set.
      size:size];
  }
  font->nativePtr = NA_COCOA_PTR_OBJC_TO_C(NA_COCOA_RETAIN(retFont));

  font->name = naNewStringWithFormat("%s", fontFamilyName);
  font->flags = flags;
  font->size = size;
  
  return font;
}

NAFont* naCreateFontWithPreset(NAFontKind kind, NAFontSize fontSize) {
  CGFloat baseSize;
  switch(fontSize) {
  case NA_FONT_SIZE_SMALL: baseSize = 11; break;
  case NA_FONT_SIZE_DEFAULT: baseSize = [NSFont systemFontSize]; break;
  case NA_FONT_SIZE_BIG: baseSize = 18; break;
  case NA_FONT_SIZE_HUGE: baseSize = 24; break;
  default: baseSize = [NSFont systemFontSize]; break;
  }

  NSFont* systemFont = [NSFont systemFontOfSize:[NSFont systemFontSize]];

  NAFont* retFont;
  switch(kind) {
    case NA_FONT_KIND_SYSTEM:
      retFont = naCreateFont([[systemFont familyName] UTF8String], NA_FONT_FLAG_REGULAR, baseSize);
      break;
    case NA_FONT_KIND_TITLE:
      retFont = naCreateFont([[systemFont familyName] UTF8String], NA_FONT_FLAG_BOLD, baseSize);
      break;
    case NA_FONT_KIND_MONOSPACE:
      retFont = naCreateFont("Courier", NA_FONT_FLAG_REGULAR, baseSize);
      break;
    case NA_FONT_KIND_PARAGRAPH:
      retFont = naCreateFont("Palatino", NA_FONT_FLAG_REGULAR, baseSize);
      break;
    case NA_FONT_KIND_MATH:
      // Note: Times new roman would be more traditional but unicode support is worse.
      retFont = naCreateFont("STIX Two Math", NA_FONT_FLAG_REGULAR, baseSize);
      //retFont = naCreateFont("STIX Two Text", NA_FONT_FLAG_ITALIC, baseSize);
      break;
    default:
      #if NA_DEBUG
        naError("Unknown font kind");
      #endif
      retFont = naCreateFont("San Francisco", NA_FONT_FLAG_REGULAR, baseSize);
      break;
  }
  
  return retFont;
}



NSTextAlignment getNSTextAlignmentWithAlignment(NATextAlignment alignment) {
  NSTextAlignment nsalignment;
  switch(alignment) {
    case NA_TEXT_ALIGNMENT_LEFT: nsalignment = NATextAlignmentLeft; break;
    case NA_TEXT_ALIGNMENT_RIGHT: nsalignment = NATextAlignmentRight; break;
    case NA_TEXT_ALIGNMENT_CENTER: nsalignment = NATextAlignmentCenter; break;
  default:
    #if NA_DEBUG
      naError("Invalid alignment enumeration");
    #endif
    break;
  }
  return nsalignment;
}





// ///////////////////


NA_DEF void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText) {
    NSAlert* alert = NA_COCOA_AUTORELEASE([[NSAlert alloc] init]);

    switch(alertBoxType) {
    case NA_ALERT_BOX_INFO:    alert.alertStyle = NAAlertStyleWarning; break;
    case NA_ALERT_BOX_WARNING: alert.alertStyle = NAAlertStyleInfo; break;
    case NA_ALERT_BOX_ERROR:   alert.alertStyle = NAAlertStyleError; break;
    }
    
    alert.messageText = [NSString stringWithUTF8String:titleText];
    alert.informativeText = [NSString stringWithUTF8String:infoText];
    [alert runModal];
}



NA_DEF void naPresentFilePanel(
  void* window,
  NABool load,
  const NAUTF8Char* fileName,
  const NAUTF8Char* allowedFileSuffix,
  NAFilePanelCallback callback,
  const void* data)
{
  #if NA_DEBUG
    if(load)
      naError("Load panels are not implemented yet.");
  #else
    NA_UNUSED(load);
  #endif

  NSSavePanel* savepanel = [NSSavePanel savePanel];

  [savepanel setNameFieldStringValue:[NSString stringWithUTF8String:fileName]];
  [savepanel setExtensionHidden:NO];
  #if defined __MAC_11_0
    NA_MACOS_AVAILABILITY_GUARD_11_0(
      [savepanel setAllowedContentTypes:[NSArray arrayWithObject:[UTType typeWithFilenameExtension:[NSString stringWithUTF8String:allowedFileSuffix]]]];
    )
  #else
    [savepanel setAllowedFileTypes:[NSArray arrayWithObject:[NSString stringWithUTF8String:allowedFileSuffix]]];
  #endif
  
  [savepanel beginSheetModalForWindow:NA_COCOA_PTR_C_TO_OBJC(window) completionHandler:^(NSInteger result) {
    #if defined __MAC_10_9
      NABool doPerform = result != NSModalResponseCancel;
    #else
      NABool doPerform = result != NSFileHandlingPanelCancelButton;
    #endif
    callback(doPerform, [[[savepanel URL] path] UTF8String], data);
  }];
}


NA_HDEF NARect na_GetScreenRect(const NA_UIElement* screen) {
  NA_UNUSED(screen);
  NARect rect = {{0, 0}, {1, 1}};
  return rect;
}

NA_HDEF void na_SetScreenRect(NA_UIElement* screen, NARect rect) {
  NA_UNUSED(screen);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("A screen can not be resized by software.");
  #endif
}


NA_DEF void naCenterMouse(void* uiElement) {
  NARect spaceRect;
  NSRect screenframe;
  CGPoint centerPos;
  spaceRect = naGetUIElementRectAbsolute(uiElement);
  screenframe = [[NSScreen mainScreen] frame];
  centerPos.x = (CGFloat)spaceRect.pos.x + (CGFloat)spaceRect.size.width * .5f;
  centerPos.y = (CGFloat)screenframe.size.height - (CGFloat)(spaceRect.pos.y + spaceRect.size.height * .5f);

  CGWarpMouseCursorPosition(centerPos);
}



NA_DEF void naShowMouse() {
  NAApplication* app = naGetApplication();
  if(!(app->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE)) {
    CGDisplayShowCursor(kCGDirectMainDisplay);
    naSetFlagu32(&app->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE, NA_TRUE);
  }
}


NA_DEF void naHideMouse() {
  NAApplication* app = naGetApplication();
  if(app->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE) {
    CGDisplayHideCursor(kCGDirectMainDisplay);
    naSetFlagu32(&app->flags, NA_APPLICATION_FLAG_MOUSE_VISIBLE, NA_FALSE);
  }
}



NA_DEF void naHideMouseUntilMovement(NABool hide) {
  [NSCursor setHiddenUntilMouseMoves:hide ? YES : NO];
}



NA_DEF NACursorImage naAllocCursorImage(const NAUIImage* uiImage, NAPos hotspot) {
  NSImage* nsImage = na_CreateResolutionIndependentNativeImage(
    NA_NULL,
    uiImage,
    NA_UIIMAGE_INTERACTION_NONE,
    NA_FALSE);
  return NA_COCOA_PTR_OBJC_TO_C([[NSCursor alloc] initWithImage:nsImage hotSpot:naMakeNSPointWithPos(hotspot)]);
}



NA_API void naOpenURLInBrowser(const NAUTF8Char* url) {
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}



NA_HDEF void* na_AddMouseTracking(NA_UIElement* uiElement) {
  naDefineCocoaObject(NSView, nativePtr, uiElement);
  NSTrackingArea* trackingArea = [[NSTrackingArea alloc]
    initWithRect:naMakeNSRectWithRect(naGetUIElementRect(uiElement))
    options:(NSTrackingAreaOptions)(NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | /*NSTrackingActiveWhenFirstResponder | NSTrackingActiveInKeyWindow |*/ NSTrackingActiveInActiveApp)
    owner:nativePtr
    userInfo:nil];
  [nativePtr addTrackingArea:trackingArea];
  
  return trackingArea;
}



NA_HDEF void na_ClearMouseTracking(NA_UIElement* uiElement, void* mouseTracking) {
  naDefineCocoaObject(NSView, nativePtr, uiElement);
  [nativePtr removeTrackingArea:mouseTracking];
  NA_COCOA_RELEASE(mouseTracking);
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
