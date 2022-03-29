
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAAppCore.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAThreading.h"
#include "NATranslator.h"

// the following import is needed for UTType definitions.
#import <UniformTypeIdentifiers/UTType.h>


NA_HAPI void na_RenewWindowMouseTracking(NAWindow* window);
NA_HAPI void na_ClearWindowMouseTracking(NAWindow* window);

NA_HAPI NARect na_GetNativeWindowAbsoluteInnerRect(const NSWindow* window);



#define naDefineCocoaObject(cocoatype, var, uiElement)\
  cocoatype* var = (NA_COCOA_BRIDGE cocoatype*)(naGetUIElementNativePtr(uiElement))
  
#define naDefineCocoaObjectConst(cocoatype, var, uiElement)\
  cocoatype* var = (NA_COCOA_BRIDGE cocoatype*)(naGetUIElementNativePtrConst(uiElement))


NA_HDEF void na_ClearUINativePtr(NANativePtr nativePtr){
  NA_COCOA_RELEASE(NA_COCOA_PTR_C_TO_OBJC(nativePtr));
}



NA_HDEF void na_SetUIElementParent(NA_UIElement* uiElement, void* parent, NABool isElementAttachable){
  NA_UNUSED(isElementAttachable);
  NA_UIElement* elem = (NA_UIElement*)uiElement;
  // todo: remove from old parent
  elem->parent = parent;
}


#ifndef NSAppKitVersionNumber11_0
  #define NSAppKitVersionNumber11_0 2022.00
#endif

NA_HDEF double na_GetUIElementOffsetY(NA_UIElement* elem){
  // Line height is considered to be 25 for an optimal display. In this
  // function, the UI elements are shifted in Y direction such that text
  // always is displayed on a common baseline. The text should have (without
  // underline glyph elements) a height of 9 and a distance of 8 above and
  // 8 below.
  // Note that all spaces have offset 0
  
  if(NSAppKitVersionNumber < NSAppKitVersionNumber11_0){
    switch(naGetUIElementType(elem)){
    case NA_UI_APPLICATION:  return  0.;
    case NA_UI_BUTTON:{
      NAButton* button = (NAButton*)elem;
      return naIsButtonStateful(button) ? +1. : -1.;
    }
    case NA_UI_CHECKBOX:     return +4.;
    case NA_UI_IMAGE_SPACE:  return  0.;
    case NA_UI_LABEL:        return +5.;
    case NA_UI_MENU:         return  0.;
    case NA_UI_MENUITEM:     return  0.;
    case NA_UI_METAL_SPACE:  return  0.;
    case NA_UI_OPENGL_SPACE: return  0.;
    case NA_UI_POPUP_BUTTON: return +1.;
    case NA_UI_RADIO:        return +4.;
    case NA_UI_SCREEN:       return  0.;
    case NA_UI_SLIDER:       return +0.;
    case NA_UI_SPACE:        return  0.;
    case NA_UI_TEXTBOX:      return +0.;
    case NA_UI_TEXTFIELD:    return +3.;
    case NA_UI_WINDOW:       return  0.;
    }
  }else{
    switch(naGetUIElementType(elem)){
    case NA_UI_APPLICATION:  return  0.;
    case NA_UI_BUTTON:       return +1.;
    case NA_UI_CHECKBOX:     return +6.;
    case NA_UI_IMAGE_SPACE:  return  0.;
    case NA_UI_LABEL:        return +7.;
    case NA_UI_MENU:         return  0.;
    case NA_UI_MENUITEM:     return  0.;
    case NA_UI_METAL_SPACE:  return  0.;
    case NA_UI_OPENGL_SPACE: return  0.;
    case NA_UI_POPUP_BUTTON: return +1.;
    case NA_UI_RADIO:        return +6.;
    case NA_UI_SCREEN:       return  0.;
    case NA_UI_SLIDER:       return +2.;
    case NA_UI_SPACE:        return  0.;
    case NA_UI_TEXTBOX:      return +2.;
    case NA_UI_TEXTFIELD:    return +5.;
    case NA_UI_WINDOW:       return  0.;
    }
  }
}



NA_HDEF void na_CaptureKeyboardStatus(NSEvent* event){
  NSUInteger flags;
  NABool hasShift;
  NABool hasControl;
  NABool hasOption;
  NABool hasCommand;
  NAUIKeyCode keyCode = [event keyCode];
  na_App->curKeyStroke.keyCode = keyCode;
  [event modifierFlags];
  flags = (NSUInteger)[event modifierFlags];
  hasShift     = (flags & NAEventModifierFlagShift)   != 0;
  hasControl   = (flags & NAEventModifierFlagControl) != 0;
  hasOption    = (flags & NAEventModifierFlagOption)  != 0;
  hasCommand   = (flags & NAEventModifierFlagCommand) != 0;
  na_App->curKeyStroke.modifiers = 0;
  na_App->curKeyStroke.modifiers |= (uint32)hasShift * NA_MODIFIER_FLAG_SHIFT;
  na_App->curKeyStroke.modifiers |= (uint32)hasControl * NA_MODIFIER_FLAG_CONTROL;
  na_App->curKeyStroke.modifiers |= (uint32)hasOption * NA_MODIFIER_FLAG_OPTION;
  na_App->curKeyStroke.modifiers |= (uint32)hasCommand * NA_MODIFIER_FLAG_COMMAND;
}



NA_HDEF NABool na_InterceptKeyboardShortcut(NSEvent* event){
  NABool retValue = NA_FALSE;
  if([event type] == NAEventTypeKeyDown || [event type] == NAEventTypeFlagsChanged){
    NA_UIElement* elem;
    NSWindow* focusWindow;
    na_CaptureKeyboardStatus(event);
    
    // Search for the first responder which is represented in NALib.
    elem = NA_NULL;
    focusWindow = [NSApp keyWindow];
    if(focusWindow){
      NSResponder* firstResponder = [focusWindow firstResponder];
      if(firstResponder){
        while(!elem && firstResponder){
          elem = na_GetUINALibEquivalent((NA_COCOA_BRIDGE NANativePtr)(firstResponder));
          if(!elem){
            if(firstResponder == focusWindow){
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
    while(!retValue && elem){
      NAListIterator iter = naMakeListAccessor(&(elem->shortcuts));
      while(!retValue && naIterateList(&iter)){
        const NAKeyboardShortcutReaction* keyReaction = naGetListCurConst(&iter);
        if(keyReaction->shortcut.keyCode == na_App->curKeyStroke.keyCode){
          NABool needsShift   = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool needsControl = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool needsOption  = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool needsCommand = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_COMMAND);
          NABool hasShift   = naGetFlagu32(na_App->curKeyStroke.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool hasControl = naGetFlagu32(na_App->curKeyStroke.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool hasOption  = naGetFlagu32(na_App->curKeyStroke.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool hasCommand = naGetFlagu32(na_App->curKeyStroke.modifiers, NA_MODIFIER_FLAG_COMMAND);
          if(needsShift   == hasShift
          && needsControl == hasControl
          && needsOption  == hasOption
          && needsCommand == hasCommand){
            NAReaction reaction;
            reaction.uiElement = na_App;
            reaction.command = NA_UI_COMMAND_KEYBOARD_SHORTCUT;
            reaction.controller = keyReaction->controller;
            retValue = keyReaction->handler(reaction);
          }
        }
      }
      naClearListIterator(&iter);
      elem = naGetUIElementParent(elem);
    }
  }
  return retValue;
}


// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////


NA_DEF void na_RefreshUIElementNow(void* uiElement){
  naDefineCocoaObjectConst(NSView, cocoaView, uiElement);
  [cocoaView setNeedsDisplay:YES];
}




NA_DEF void naSetUIElementNextTabElement(void* uiElement, const void* nextTabElem){
  if(  naGetUIElementType(uiElement) != NA_UI_TEXTFIELD
    && naGetUIElementType(uiElement) != NA_UI_TEXTBOX){
    #if NA_DEBUG
      naError("elem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  if(  naGetUIElementType(nextTabElem) != NA_UI_TEXTFIELD
    && naGetUIElementType(nextTabElem) != NA_UI_TEXTBOX){
    #if NA_DEBUG
      naError("nextTabElem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  naDefineCocoaObject(NSView, cocoaCurTabElem, uiElement);
  naDefineCocoaObjectConst(NSView, cocoaNextTabElem, nextTabElem);
  [cocoaCurTabElem setNextKeyView:cocoaNextTabElem];
}




NA_DEF double naGetUIElementResolutionFactor(void* uiElement){
  if(naGetUIElementType(uiElement) == NA_UI_APPLICATION){return 1.;}
  if(naGetUIElementType(uiElement) == NA_UI_SCREEN){
    #if NA_DEBUG
      naError("Sorry, was too lazy to implement this. I mean, it's not hard, but as screens are not used anyway at the moment, who cares.");
    #endif
    return 1.;
  }
  
  void* parent = naGetUIElementParent(uiElement);
  while(naGetUIElementType(uiElement) != NA_UI_WINDOW && parent){
    uiElement = parent;
    parent = naGetUIElementParent(uiElement);
  }
  
  if(naGetUIElementType(uiElement) == NA_UI_WINDOW){
    return naGetWindowBackingScaleFactor(NA_COCOA_PTR_C_TO_OBJC(naGetUIElementNativePtr(uiElement)));
  }else{
    return naGetUIElementBackingScaleFactor(NA_COCOA_PTR_C_TO_OBJC(naGetUIElementNativePtr(uiElement)));
  }
}






NAFont na_GetFontWithKindAndSize(NAFontKind kind, NAFontSize size){
  CGFloat baseSize;
  switch(size){
  case NA_FONT_SIZE_SMALL: baseSize = 11; break;
  case NA_FONT_SIZE_DEFAULT: baseSize = [NSFont systemFontSize]; break;
  case NA_FONT_SIZE_BIG: baseSize = 18; break;
  case NA_FONT_SIZE_HUGE: baseSize = 24; break;
  default: baseSize = [NSFont systemFontSize]; break;
  }

  NSFont* font;
  NSFontDescriptor* descriptor;
  NSDictionary* dict;
  switch(kind){
    case NA_FONT_KIND_SYSTEM:
      font = [NSFont systemFontOfSize:baseSize];
      break;
    case NA_FONT_KIND_TITLE:
      font = [NSFont boldSystemFontOfSize:baseSize];
      break;
    case NA_FONT_KIND_MONOSPACE:
      dict = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"Courier", NSFontFamilyAttribute, 
                            @"Regular", NSFontFaceAttribute, nil];
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
      font = [NSFont fontWithDescriptor:descriptor size:baseSize];
      break;
    case NA_FONT_KIND_PARAGRAPH:
      dict = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"Palatino", NSFontFamilyAttribute, 
                            @"Regular", NSFontFaceAttribute, nil];
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
      font = [NSFont fontWithDescriptor:descriptor size:baseSize + 1];
      break;
    case NA_FONT_KIND_MATH:
      dict = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"Times New Roman", NSFontFamilyAttribute, 
                            @"Italic", NSFontFaceAttribute, nil];
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
      font = [NSFont fontWithDescriptor:descriptor size:baseSize];
      break;
    default:
      #if NA_DEBUG
        naError("Unknown font kind");
      #endif
      font = [NSFont systemFontOfSize:baseSize];
      break;
  }
  return NA_COCOA_PTR_OBJC_TO_C(font);
}



NAFont na_GetCustomFont(const NAUTF8Char* fontName, uint32 flags, double size){
  NA_UNUSED(flags);
  NSFont* font;

  NSString* attrStr = @"Regular";
//  if(flags == NA_FONT_FLAG_BOLD){ attrStr = @"Bold"; }
//  if(flags == NA_FONT_FLAG_ITALIC){ attrStr = @"Italic"; }
//  if(flags == NA_FONT_FLAG_BOLD | NA_FONT_FLAG_ITALIC){ attrStr = @"Bold Italic"; }

  NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
                        [NSString stringWithUTF8String: fontName], NSFontFamilyAttribute, 
                        attrStr, NSFontFaceAttribute, nil];
  NSFontDescriptor* descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
  font = [NSFont fontWithDescriptor:descriptor size:size];

  if(!font){
    #if NA_DEBUG
      naError("Font can not be found");
    #endif
    font = [NSFont systemFontOfSize:size];
  }
  return NA_COCOA_PTR_OBJC_TO_C(font);
}



NSTextAlignment getNSTextAlignmentWithAlignment(NATextAlignment alignment){
  NSTextAlignment nsalignment;
  switch(alignment){
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



NA_HDEF NARect na_GetScreenAbsoluteRect(const NA_UIElement* screen){
  NARect rect;
  NSRect frame;
  NSRect mainframe;
  naDefineCocoaObjectConst(NSScreen, cocoascreen, screen);
  mainframe = [[NSScreen mainScreen] frame];
  frame = [cocoascreen frame];
  rect.pos.x = frame.origin.x;
  rect.pos.y = mainframe.size.height - frame.size.height - frame.origin.y;
  rect.size.width = frame.size.width;
  rect.size.height = frame.size.height;
  return rect;
}









// ///////////////////


NA_DEF void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText){
    NSAlert* alert = [[NSAlert alloc] init];

    switch(alertBoxType){
    case NA_ALERT_BOX_INFO:    alert.alertStyle = NAAlertStyleWarning; break;
    case NA_ALERT_BOX_WARNING: alert.alertStyle = NAAlertStyleInfo; break;
    case NA_ALERT_BOX_ERROR:   alert.alertStyle = NSAlertStyleError; break;
    }
    
    alert.messageText = [NSString stringWithUTF8String:titleText];
    alert.informativeText = [NSString stringWithUTF8String:infoText];
    [alert runModal];
    NA_COCOA_RELEASE(alert);
}



NA_DEF void naPresentFilePanel(void* window, NABool load, const NAUTF8Char* fileName, const NAUTF8Char* allowedFileSuffix, FilePanelCallback callback){
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
  
  [savepanel beginSheetModalForWindow:NA_COCOA_PTR_C_TO_OBJC(window) completionHandler:^(NSInteger result){
    #if defined __MAC_10_9
      NABool doPerform = result != NSModalResponseCancel;
    #else
      NABool doPerform = result != NSFileHandlingPanelCancelButton;
    #endif
    callback(doPerform, [[[savepanel URL] path] UTF8String]);
  }];
}



NA_DEF void naCenterMouse(void* uiElement, NABool includeBorder){
  NARect spacerect;
  NSRect screenframe;
  CGPoint centerpos;
  spacerect = naGetUIElementRect(uiElement, naGetApplication(), includeBorder);
  screenframe = [[NSScreen mainScreen] frame];
  centerpos.x = (CGFloat)spacerect.pos.x + (CGFloat)spacerect.size.width * .5f;
  centerpos.y = (CGFloat)screenframe.size.height - (CGFloat)(spacerect.pos.y + spacerect.size.height * .5f);

  CGWarpMouseCursorPosition(centerpos);
}



NA_DEF void naShowMouse(){
  NAApplication* app = naGetApplication();
  if(!(app->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE)){
    CGDisplayShowCursor(kCGDirectMainDisplay);
    app->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


NA_DEF void naHideMouse(){
  NAApplication* app = naGetApplication();
  if(app->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE){
    CGDisplayHideCursor(kCGDirectMainDisplay);
    app->flags &= ~NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}




NA_DEF NARect naGetUIElementRect(const void* uiElement, const void* relativeuiElement, NABool includeBorder){
  NARect rect;
  NARect relRect;
  NA_UIElement* element;
  NA_UIElement* relElement;
  NAApplication* app;

  element = (NA_UIElement*)uiElement;
  relElement = (NA_UIElement*)relativeuiElement;
  app = naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(element == (NA_UIElement*)app){
    #if NA_DEBUG
      if(relElement && (relElement != (NA_UIElement*)app))
        naError("The relative element is invalid for the given uiElement, which seems to be the application.");
    #endif
    return na_GetApplicationAbsoluteRect();
  }

  switch(element->elementType){
  case NA_UI_APPLICATION:  rect = na_GetApplicationAbsoluteRect(); break;
  case NA_UI_BUTTON:       rect = na_GetButtonAbsoluteInnerRect(element); break;
  case NA_UI_CHECKBOX:     rect = na_GetCheckBoxAbsoluteInnerRect(element); break;
  case NA_UI_IMAGE_SPACE:  rect = na_GetImageSpaceAbsoluteInnerRect(element); break;
  case NA_UI_LABEL:        rect = na_GetLabelAbsoluteInnerRect(element); break;
  case NA_UI_MENU:         rect = na_GetMenuAbsoluteInnerRect(element); break;
  case NA_UI_MENUITEM:     rect = na_GetMenuItemAbsoluteInnerRect(element); break;
  case NA_UI_METAL_SPACE:  rect = na_GetMetalSpaceAbsoluteInnerRect(element); break;
  case NA_UI_OPENGL_SPACE: rect = na_GetOpenGLSpaceAbsoluteInnerRect(element); break;
  case NA_UI_POPUP_BUTTON: rect = na_GetPopupButtonAbsoluteInnerRect(element); break;
  case NA_UI_RADIO:        rect = na_GetRadioAbsoluteInnerRect(element); break;
  case NA_UI_SCREEN:       rect = na_GetScreenAbsoluteRect(element); break;
  case NA_UI_SLIDER:       rect = na_GetSliderAbsoluteInnerRect(element); break;
  case NA_UI_SPACE:        rect = na_GetSpaceAbsoluteInnerRect(element); break;
  case NA_UI_TEXTBOX:      rect = na_GetTextBoxAbsoluteInnerRect(element); break;
  case NA_UI_TEXTFIELD:    rect = na_GetTextFieldAbsoluteInnerRect(element); break;
  case NA_UI_WINDOW:
    if(includeBorder){
      rect = na_GetWindowAbsoluteOuterRect(element);
    }else{
      rect = na_GetWindowAbsoluteInnerRect(element);
    }
    break;
  }
  rect.pos.y -= na_GetUIElementOffsetY(element);

  // Now, we find the appropriate relative element.
  if(!relElement){relElement = (NA_UIElement*)naGetUIElementParent(element);}

  if(relElement){
    switch(relElement->elementType){
    case NA_UI_APPLICATION:  relRect = na_GetApplicationAbsoluteRect(); break;
    case NA_UI_BUTTON:       relRect = na_GetButtonAbsoluteInnerRect(relElement); break;
    case NA_UI_CHECKBOX:     relRect = na_GetCheckBoxAbsoluteInnerRect(relElement); break;
    case NA_UI_IMAGE_SPACE:  relRect = na_GetImageSpaceAbsoluteInnerRect(relElement); break;
    case NA_UI_LABEL:        relRect = na_GetLabelAbsoluteInnerRect(relElement); break;
    case NA_UI_MENU:         relRect = na_GetMenuAbsoluteInnerRect(relElement); break;
    case NA_UI_MENUITEM:     relRect = na_GetMenuItemAbsoluteInnerRect(relElement); break;
    case NA_UI_METAL_SPACE:  relRect = na_GetMetalSpaceAbsoluteInnerRect(relElement); break;
    case NA_UI_OPENGL_SPACE: relRect = na_GetOpenGLSpaceAbsoluteInnerRect(relElement); break;
    case NA_UI_POPUP_BUTTON: relRect = na_GetPopupButtonAbsoluteInnerRect(relElement); break;
    case NA_UI_RADIO:        relRect = na_GetRadioAbsoluteInnerRect(relElement); break;
    case NA_UI_SCREEN:       relRect = na_GetScreenAbsoluteRect(relElement); break;
    case NA_UI_SLIDER:       relRect = na_GetSliderAbsoluteInnerRect(relElement); break;
    case NA_UI_SPACE:        relRect = na_GetSpaceAbsoluteInnerRect(relElement); break;
    case NA_UI_TEXTBOX:      relRect = na_GetTextBoxAbsoluteInnerRect(relElement); break;
    case NA_UI_TEXTFIELD:    relRect = na_GetTextFieldAbsoluteInnerRect(relElement); break;
    case NA_UI_WINDOW:       relRect = na_GetWindowAbsoluteInnerRect(relElement); break;
    }
    relRect.pos.y -= na_GetUIElementOffsetY(relElement);

    rect.pos.x = rect.pos.x - relRect.pos.x;
    rect.pos.y = rect.pos.y - relRect.pos.y;
  }

//  rect.size.width = rect.size.width;
//  rect.size.height = rect.size.height;

  // Convert the rect into absolute coordinates.

  return rect;
}


NA_API void naOpenURLInBrowser(const NAUTF8Char* url){
  [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
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
