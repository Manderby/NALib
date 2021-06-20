
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAUICore.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAThreading.h"
#include "NATranslator.h"



NA_HAPI void na_RenewWindowMouseTracking(NAWindow* window);
NA_HAPI void na_ClearWindowMouseTracking(NAWindow* window);

NA_HAPI NARect na_GetNativeWindowAbsoluteInnerRect(NSWindow* window);

// Cocoa thinks it's smart by doing things automatically. Unfortunately, we
// have to encapsulate some ui elements like NARadio or NATextBox into its own
// view to get the behaviour we need.
@protocol NACocoaNativeEncapsulatedElement
@required
- (NSView*) getEncapsulatingView;
@end



#define naDefineCocoaObject(cocoatype, var, uiElement)\
  cocoatype* var = (NA_COCOA_BRIDGE cocoatype*)(naGetUIElementNativePtr(uiElement))


NA_HDEF void na_ClearUINativePtr(NANativePtr nativePtr){
  NA_COCOA_RELEASE(nativePtr);
}



NA_HDEF void na_SetUIElementParent(NA_UIElement* uiElement, void* parent){
  NA_UIElement* elem = (NA_UIElement*)uiElement;
  // todo: remove from old parent
  elem->parent = parent;
}



NA_HDEF void na_CaptureKeyboardStatus(NSEvent* event){
  NSUInteger flags;
  NABool hasShift;
  NABool hasControl;
  NABool hasOption;
  NABool hasCommand;
  NAUIKeyCode keyCode = [event keyCode];
  na_App->keyboardStatus.keyCode = keyCode;
  [event modifierFlags];
  flags = (NSUInteger)[event modifierFlags];
  hasShift     = (flags & NAEventModifierFlagShift)   != 0;
  hasControl   = (flags & NAEventModifierFlagControl) != 0;
  hasOption    = (flags & NAEventModifierFlagOption)  != 0;
  hasCommand   = (flags & NAEventModifierFlagCommand) != 0;
  na_App->keyboardStatus.modifiers = 0;
  na_App->keyboardStatus.modifiers |= (uint32)hasShift * NA_MODIFIER_FLAG_SHIFT;
  na_App->keyboardStatus.modifiers |= (uint32)hasControl * NA_MODIFIER_FLAG_CONTROL;
  na_App->keyboardStatus.modifiers |= (uint32)hasOption * NA_MODIFIER_FLAG_OPTION;
  na_App->keyboardStatus.modifiers |= (uint32)hasCommand * NA_MODIFIER_FLAG_COMMAND;
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
        if(keyReaction->shortcut.keyCode == na_App->keyboardStatus.keyCode){
          NABool needsShift   = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool needsControl = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool needsOption  = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool needsCommand = naGetFlagu32(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_COMMAND);
          NABool hasShift   = naGetFlagu32(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool hasControl = naGetFlagu32(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool hasOption  = naGetFlagu32(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool hasCommand = naGetFlagu32(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_COMMAND);
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
  naDefineCocoaObject(NSView, cocoaView, uiElement);
  [cocoaView setNeedsDisplay:YES];
}




NA_DEF void naSetUIElementNextTabElement(void* elem, void* nextTabElem){
  if(  naGetUIElementType(elem) != NA_UI_TEXTFIELD
    && naGetUIElementType(elem) != NA_UI_TEXTBOX){
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

  naDefineCocoaObject(NSView, cocoaCurTabElem, elem);
  naDefineCocoaObject(NSView, cocoaNextTabElem, nextTabElem);
  [cocoaCurTabElem setNextKeyView:cocoaNextTabElem];
}







NAFont na_GetFontWithKind(NAFontKind kind){
  NSFont* font;
  CGFloat systemSize = [NSFont systemFontSize];
  NSFontDescriptor* descriptor;
  NSDictionary* dict;
  switch(kind){
    case NA_FONT_KIND_SYSTEM:
      font = [NSFont systemFontOfSize:systemSize];
      break;
    case NA_FONT_KIND_TITLE:
      font = [NSFont boldSystemFontOfSize:systemSize];
      break;
    case NA_FONT_KIND_MONOSPACE:
      dict = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"Courier", NSFontFamilyAttribute, 
                            @"Regular", NSFontFaceAttribute, nil];
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
      font = [NSFont fontWithDescriptor:descriptor size:systemSize];
      break;
    case NA_FONT_KIND_PARAGRAPH:
      dict = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"Palatino", NSFontFamilyAttribute, 
                            @"Regular", NSFontFaceAttribute, nil];
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
      font = [NSFont fontWithDescriptor:descriptor size:systemSize + 1];
      break;
    case NA_FONT_KIND_MATH:
      dict = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"Times New Roman", NSFontFamilyAttribute, 
                            @"Italic", NSFontFaceAttribute, nil];
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:dict];
      font = [NSFont fontWithDescriptor:descriptor size:systemSize];
      break;
    default:
      #if NA_DEBUG
        naError("Unknown font kind");
      #endif
      font = [NSFont systemFontOfSize:systemSize];
      break;
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



NA_HDEF NARect na_GetScreenAbsoluteRect(NA_UIElement* screen){
  NARect rect;
  NSRect frame;
  NSRect mainframe;
  naDefineCocoaObject(NSScreen, cocoascreen, screen);
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



NA_DEF void naCenterMouse(void* uiElement, NABool includebounds, NABool sendmovemessage){
  NARect spacerect;
  NSRect screenframe;
  CGPoint centerpos;
  NA_UNUSED(sendmovemessage);
  spacerect = naGetUIElementRect(uiElement, naGetApplication(), includebounds);
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




NA_DEF NARect naGetUIElementRect(void* uiElement, void* relativeuiElement, NABool includeborder){
  NARect rect;
  NARect relrect;
  NA_UIElement* element;
  NA_UIElement* relelement;
  NAApplication* app;

  element = (NA_UIElement*)uiElement;
  relelement = (NA_UIElement*)relativeuiElement;
  app = naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(element == (NA_UIElement*)app){
    #if NA_DEBUG
      if(relelement && (relelement != (NA_UIElement*)app))
        naError("The relative element is invalid for the given uiElement, which seems to be the application.");
    #endif
    return na_GetApplicationAbsoluteRect();
  }

  switch(element->elementType){
  case NA_UI_APPLICATION: rect = na_GetApplicationAbsoluteRect(); break;
  case NA_UI_BUTTON:      rect = na_GetButtonAbsoluteInnerRect(element); break;
  case NA_UI_CHECKBOX:    rect = na_GetCheckBoxAbsoluteInnerRect(element); break;
  case NA_UI_IMAGESPACE:  rect = na_GetImageSpaceAbsoluteInnerRect(element); break;
  case NA_UI_LABEL:       rect = na_GetLabelAbsoluteInnerRect(element); break;
  case NA_UI_MENU:        rect = na_GetMenuAbsoluteInnerRect(element); break;
  case NA_UI_MENUITEM:    rect = na_GetMenuItemAbsoluteInnerRect(element); break;
  case NA_UI_OPENGLSPACE: rect = na_GetOpenGLSpaceAbsoluteInnerRect(element); break;
  case NA_UI_RADIO:       rect = na_GetRadioAbsoluteInnerRect(element); break;
  case NA_UI_SCREEN:      rect = na_GetScreenAbsoluteRect(element); break;
  case NA_UI_SLIDER:      rect = na_GetSliderAbsoluteInnerRect(element); break;
  case NA_UI_SPACE:       rect = na_GetSpaceAbsoluteInnerRect(element); break;
  case NA_UI_TEXTBOX:     rect = na_GetTextBoxAbsoluteInnerRect(element); break;
  case NA_UI_TEXTFIELD:   rect = na_GetTextFieldAbsoluteInnerRect(element); break;
  case NA_UI_WINDOW:
    if(includeborder){
      rect = na_GetWindowAbsoluteOuterRect(element);
    }else{
      rect = na_GetWindowAbsoluteInnerRect(element);
    }
    break;
  }

  // Now, we find the appropriate relative element.
  if(!relelement){relelement = (NA_UIElement*)naGetUIElementParent(element);}

  if(relelement){
    switch(relelement->elementType){
    case NA_UI_APPLICATION: relrect = na_GetApplicationAbsoluteRect(); break;
    case NA_UI_BUTTON:      relrect = na_GetButtonAbsoluteInnerRect(relelement); break;
    case NA_UI_CHECKBOX:    relrect = na_GetCheckBoxAbsoluteInnerRect(relelement); break;
    case NA_UI_IMAGESPACE:  relrect = na_GetImageSpaceAbsoluteInnerRect(relelement); break;
    case NA_UI_LABEL:       relrect = na_GetLabelAbsoluteInnerRect(relelement); break;
    case NA_UI_MENU:        relrect = na_GetMenuAbsoluteInnerRect(relelement); break;
    case NA_UI_MENUITEM:    relrect = na_GetMenuItemAbsoluteInnerRect(relelement); break;
    case NA_UI_OPENGLSPACE: relrect = na_GetOpenGLSpaceAbsoluteInnerRect(relelement); break;
    case NA_UI_RADIO:       relrect = na_GetRadioAbsoluteInnerRect(relelement); break;
    case NA_UI_SCREEN:      relrect = na_GetScreenAbsoluteRect(relelement); break;
    case NA_UI_SLIDER:      relrect = na_GetSliderAbsoluteInnerRect(relelement); break;
    case NA_UI_SPACE:       relrect = na_GetSpaceAbsoluteInnerRect(relelement); break;
    case NA_UI_TEXTBOX:     relrect = na_GetTextBoxAbsoluteInnerRect(relelement); break;
    case NA_UI_TEXTFIELD:   relrect = na_GetTextFieldAbsoluteInnerRect(relelement); break;
    case NA_UI_WINDOW:      relrect = na_GetWindowAbsoluteInnerRect(relelement); break;
    }

    rect.pos.x = rect.pos.x - relrect.pos.x;
    rect.pos.y = rect.pos.y - relrect.pos.y;
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
