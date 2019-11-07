
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "NAUICore.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAThreading.h"
#include "NATranslator.h"



// Mapping of deprecated entities
#if !defined __MAC_10_12
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
  #define NAAlertStyleInfo                NSAlertStyleWarning
  #define NSAlertStyleError               NSAlertStyleCritical
#else
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
#endif

#if !defined __MAC_10_14
  #define NABezelStyleRounded             NSRoundedBezelStyle
  #define NABezelStyleShadowlessSquare    NSShadowlessSquareBezelStyle

  #define NAStateOn                       NSOnState
  #define NAStateOff                      NSOffState
#else
  #define NABezelStyleRounded             NSBezelStyleRounded
  #define NABezelStyleShadowlessSquare    NSBezelStyleShadowlessSquare

  #define NAStateOn                       NSControlStateValueOn
  #define NAStateOff                      NSControlStateValueOff
#endif


NA_HAPI NARect naGetApplicationAbsoluteRect(void);
NA_HAPI NARect naGetScreenAbsoluteRect(NACoreUIElement* screen);
NA_HAPI NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window);
NA_HAPI NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HDEF NARect naGetImageSpaceAbsoluteInnerRect(NACoreUIElement* imagespace);
NA_HDEF NARect naGetOpenGLSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HAPI NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* button);
NA_HAPI NARect naGetRadioAbsoluteInnerRect(NACoreUIElement* radio);
NA_HAPI NARect naGetCheckBoxAbsoluteInnerRect(NACoreUIElement* checkbox);
NA_HAPI NARect naGetLabelAbsoluteInnerRect(NACoreUIElement* label);
NA_HDEF NARect naGetTextFieldAbsoluteInnerRect(NACoreUIElement* textfield);
NA_HDEF NARect naGetTextBoxAbsoluteInnerRect(NACoreUIElement* textbox);

NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window);

NA_HAPI void naRenewWindowMouseTracking(NACoreWindow* corewindow);
NA_HAPI void naClearWindowMouseTracking(NACoreWindow* corewindow);



@interface NANativeApplicationDelegate : NSObject <NSApplicationDelegate>{
  NACoreApplication* coreapplication;
}
@end

@interface NANativeWindow : NSWindow <NSWindowDelegate>{
  NACoreWindow* corewindow;
  NAUInt trackingcount;
  NSTrackingArea* trackingarea;
}
@end

@interface NANativeSpace : NSView{
  NACoreSpace* corespace;
  NSTrackingArea* trackingarea;
}
@end

@interface NANativeImageSpace : NSImageView{
  NACoreImageSpace* coreimagespace;
}
@end

#if (NA_CONFIG_COMPILE_OPENGL == 1)
  @interface NANativeOpenGLSpace : NSOpenGLView{
    NACoreOpenGLSpace* coreopenglspace;
    NAMutator initFunc;
    void* initData;
  }
  @end
#endif

@interface NANativeButton : NSButton{
  NACoreButton* corebutton;
}
@end

@interface NANativeRadio : NSButton{
  NACoreRadio* coreradio;
  // Core thinks it's smart by doing things automatically. Unfortunately, we
  // have to encapsulate the radio into its own view to get the behaviour
  // we need.
  NSView* containingview;
}
- (NSView*) getContainingView;
@end

@interface NANativeCheckBox : NSButton{
  NACoreCheckBox* corecheckbox;
}
@end

@interface MDVerticallyCenteredTextFieldCell : NSTextFieldCell{
}
@end

@interface NANativeLabel : NSTextField{
  NACoreLabel* corelabel;
}
@end

@interface NANativeTextField : NSTextField <NSTextFieldDelegate>{
  NACoreTextField* coretextfield;
}
@end

@interface NANativeTextBox : NSTextView{
  NACoreTextBox* coretextbox;
  NSScrollView* scrollview;
}
- (NSView*) getContainingView;
@end



#define naDefineNativeCocoaObject(nativetype, var, uielement)\
  nativetype* var = (NA_COCOA_BRIDGE nativetype*)(naGetUIElementNativeID((NAUIElement*)uielement))


NA_HDEF void naClearUINativeId(NANativeID nativeId){
  NA_COCOA_DISPOSE(nativeId);
}


NA_HDEF void naSetUIElementParent(NAUIElement* uielement, NAUIElement* parent){
  NACoreUIElement* coreelement = (NACoreUIElement*)uielement;
  // todo: remove from old parent
  coreelement->parent = parent;
}


NA_DEF void naSetUIElementNextTabElement(NAUIElement* elem, NAUIElement* nextelem){
  naDefineNativeCocoaObject(NANativeTextField, nativeelem, elem);
  naDefineNativeCocoaObject(NSView, nativeview, nextelem);
  [nativeelem setNextKeyView:nativeview];
}



NA_HDEF void naCaptureKeyboardStatus(NSEvent* event){  
  NAUIKeyCode keyCode = [event keyCode];
  na_app->keyboardStatus.keyCode = keyCode;
  NSEventModifierFlags flags = [event modifierFlags];
  NABool hasShift     = (flags & NAEventModifierFlagShift)   != 0;
  NABool hasControl   = (flags & NAEventModifierFlagControl) != 0;
  NABool hasOption    = (flags & NAEventModifierFlagOption)  != 0;
  NABool hasCommand   = (flags & NAEventModifierFlagCommand) != 0;
  na_app->keyboardStatus.modifiers = 0;
  na_app->keyboardStatus.modifiers |= hasShift * NA_MODIFIER_FLAG_SHIFT;
  na_app->keyboardStatus.modifiers |= hasControl * NA_MODIFIER_FLAG_CONTROL;
  na_app->keyboardStatus.modifiers |= hasOption * NA_MODIFIER_FLAG_OPTION;
  na_app->keyboardStatus.modifiers |= hasCommand * NA_MODIFIER_FLAG_COMMAND;
}



NA_HDEF NABool naInterceptKeyboardShortcut(NSEvent* event){
  NABool retvalue = NA_FALSE;
  if([event type] == NSEventTypeKeyUp){
    naCaptureKeyboardStatus(event);
  }else if([event type] == NSEventTypeKeyDown){
    naCaptureKeyboardStatus(event);
    
    // Search for the native first responder which is represented in NALib.
    NACoreUIElement* elem = NA_NULL;
    NSWindow* keyWindow = [NSApp keyWindow];
    if(keyWindow){
      NSResponder* firstResponder = [keyWindow firstResponder];
      if(firstResponder){
        while(!elem && firstResponder){
          elem = naGetUINALibEquivalent((NA_COCOA_BRIDGE NANativeID)(firstResponder));
          if(!elem){
            if(firstResponder == keyWindow){
              elem = naGetApplication();
            }else{
              firstResponder = [(NSView*)firstResponder superview];
            }
          }
        }
      }else{
        elem = naGetUINALibEquivalent((NA_COCOA_BRIDGE NANativeID)(keyWindow));
      }
    }else{
      elem = naGetApplication();
    }
    
    // Search for a matching keyboard shortcut by bubbling.
    while(!retvalue && elem){
      NAListIterator iter = naMakeListAccessor(&(elem->shortcuts));
      while(!retvalue && naIterateList(&iter)){
        const NACoreKeyboardShortcutReaction* corereaction = naGetListCurConst(&iter);
        if(corereaction->shortcut.keyCode == na_app->keyboardStatus.keyCode){
          NABool needsShift   = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool needsControl = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool needsOption  = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool needsCommand = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_COMMAND);
          NABool hasShift   = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool hasControl = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool hasOption  = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool hasCommand = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_COMMAND);
          if(needsShift   == hasShift
          && needsControl == hasControl
          && needsOption  == hasOption
          && needsCommand == hasCommand){
            NAReaction reaction = {na_app, NA_UI_COMMAND_KEYBOARD_SHORTCUT, corereaction->controller};
            retvalue = corereaction->handler(reaction);
          }
        }
      }
      naClearListIterator(&iter);
      elem = naGetUIElementParent(elem);
    }
  }
  return retvalue;
}


// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////


NA_DEF void naRefreshUIElementNow(NAUIElement* uielement){
  naDefineNativeCocoaObject(NSView, nativeview, uielement);
  [nativeview setNeedsDisplay:YES];
}











NAFont getFontWithKind(NAFontKind kind){
  NSFont* font;
  CGFloat systemSize = [NSFont systemFontSize];
  NSFontDescriptor* descriptor;
  switch(kind){
    case NA_FONT_KIND_SYSTEM:
      font = [NSFont systemFontOfSize:systemSize];
      break;
    case NA_FONT_KIND_TITLE:
      font = [NSFont boldSystemFontOfSize:systemSize];
      break;
    case NA_FONT_KIND_MONOSPACE:
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:@{
                                       NSFontFamilyAttribute : @"Courier", 
                                       NSFontFaceAttribute : @"Regular"}];
      font = [NSFont fontWithDescriptor:descriptor size:systemSize];
      break;
    case NA_FONT_KIND_PARAGRAPH:
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:@{
                                       NSFontFamilyAttribute : @"Palatino", 
                                       NSFontFaceAttribute : @"Regular"}];
      font = [NSFont fontWithDescriptor:descriptor size:systemSize + 1];
      break;
    case NA_FONT_KIND_MATH:
      descriptor = [NSFontDescriptor fontDescriptorWithFontAttributes:@{
                                       NSFontFamilyAttribute : @"Times New Roman", 
                                       NSFontFaceAttribute : @"Italic"}];
      font = [NSFont fontWithDescriptor:descriptor size:systemSize];
      break;
    default:
      #ifndef NDEBUG
        naError("Unknown font kind");
      #endif
      font = [NSFont systemFontOfSize:systemSize];
      break;
  }
  return (NAFont)font;
}



NSTextAlignment getNSTextAlignmentWithAlignment(NATextAlignment alignment){
  NSTextAlignment nsalignment;
  switch(alignment){
  case NA_TEXT_ALIGNMENT_LEFT: nsalignment = NSTextAlignmentLeft; break;
  case NA_TEXT_ALIGNMENT_RIGHT: nsalignment = NSTextAlignmentRight; break;
  case NA_TEXT_ALIGNMENT_CENTER: nsalignment = NSTextAlignmentCenter; break;
  default:
    #ifndef NDEBUG
      naError("Invalid alignment enumeration");
    #endif
    break;
  }
  return nsalignment;
}





NA_HDEF NARect naGetApplicationAbsoluteRect(void){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NACoreUIElement* screen){
  NARect rect;
  NSRect frame;
  NSRect mainframe;
  naDefineNativeCocoaObject(NSScreen, nativescreen, screen);
  mainframe = [[NSScreen mainScreen] frame];
  frame = [nativescreen frame];
  rect.pos.x = frame.origin.x;
  rect.pos.y = mainframe.size.height - frame.size.height - frame.origin.y;
  rect.size.width = frame.size.width;
  rect.size.height = frame.size.height;
  return rect;
}









// ///////////////////


NA_DEF void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText){
    NSAlert* alert = NA_COCOA_AUTORELEASE([[NSAlert alloc] init]);

    switch(alertBoxType){
    case NA_ALERT_BOX_INFO:    alert.alertStyle = NAAlertStyleWarning; break;
    case NA_ALERT_BOX_WARNING: alert.alertStyle = NAAlertStyleInfo; break;
    case NA_ALERT_BOX_ERROR:   alert.alertStyle = NSAlertStyleError; break;
    }
    
    alert.messageText = [NSString stringWithUTF8String:titleText];
    alert.informativeText = [NSString stringWithUTF8String:infoText];
    [alert runModal];
}



NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NA_UNUSED(sendmovemessage);
  NARect spacerect;
  NSRect screenframe;
  CGPoint centerpos;
  spacerect = naGetUIElementRect(uielement, (NAUIElement*)naGetApplication(), includebounds);
  screenframe = [[NSScreen mainScreen] frame];
//  centerpos.x = spacerect.pos.x + spacerect.size.width * .5f;
//  centerpos.y = spacerect.pos.y + spacerect.size.height * .5f;
  centerpos.x = (CGFloat)spacerect.pos.x + (CGFloat)spacerect.size.width * .5f;
  centerpos.y = (CGFloat)screenframe.size.height - (CGFloat)(spacerect.pos.y + spacerect.size.height * .5f);

  CGWarpMouseCursorPosition(centerpos);
//  CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, centerpos);
}



NA_DEF void naShowMouse(){
  NACoreApplication* coreapp = (NACoreApplication*)naGetApplication();
  if(!(coreapp->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE)){
    CGDisplayShowCursor(kCGDirectMainDisplay);
    coreapp->flags |= NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}


NA_DEF void naHideMouse(){
  NACoreApplication* coreapp = (NACoreApplication*)naGetApplication();
  if(coreapp->flags & NA_APPLICATION_FLAG_MOUSE_VISIBLE){
    CGDisplayHideCursor(kCGDirectMainDisplay);
    coreapp->flags &= ~NA_APPLICATION_FLAG_MOUSE_VISIBLE;
  }
}




NA_DEF NARect naGetUIElementRect(NAUIElement* uielement, NAUIElement* relativeuielement, NABool includeborder){
  NARect rect;
  NARect relrect;
  NACoreUIElement* element;
  NACoreUIElement* relelement;

  element = (NACoreUIElement*)uielement;
  relelement = (NACoreUIElement*)relativeuielement;
  NAApplication* app = naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(element == (NACoreUIElement*)app){
    #ifndef NDEBUG
      if(relelement && (relelement != (NACoreUIElement*)app))
        naError("The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }

  switch(element->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(element); break;
  case NA_UI_WINDOW:
    if(includeborder){
      rect = naGetWindowAbsoluteOuterRect(element);
    }else{
      rect = naGetWindowAbsoluteInnerRect(element);
    }
    break;
  case NA_UI_SPACE:       rect = naGetSpaceAbsoluteInnerRect(element); break;
  case NA_UI_IMAGESPACE:  rect = naGetImageSpaceAbsoluteInnerRect(element); break;
  case NA_UI_OPENGLSPACE: rect = naGetOpenGLSpaceAbsoluteInnerRect(element); break;
  case NA_UI_BUTTON:      rect = naGetButtonAbsoluteInnerRect(element); break;
  case NA_UI_RADIO:       rect = naGetRadioAbsoluteInnerRect(element); break;
  case NA_UI_CHECKBOX:    rect = naGetCheckBoxAbsoluteInnerRect(element); break;
  case NA_UI_LABEL:       rect = naGetLabelAbsoluteInnerRect(element); break;
  case NA_UI_TEXTFIELD:   rect = naGetTextFieldAbsoluteInnerRect(element); break;
  case NA_UI_TEXTBOX:     rect = naGetTextBoxAbsoluteInnerRect(element); break;
  }

  // Now, we find the appropriate relative element.
  if(!relelement){relelement = (NACoreUIElement*)naGetUIElementParent((NAUIElement*)element);}

  if(relelement){
    switch(relelement->elementtype){
    case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
    case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(relelement); break;
    case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(relelement); break;
    case NA_UI_SPACE:       relrect = naGetSpaceAbsoluteInnerRect(relelement); break;
    case NA_UI_IMAGESPACE:  relrect = naGetImageSpaceAbsoluteInnerRect(relelement); break;
    case NA_UI_OPENGLSPACE: relrect = naGetOpenGLSpaceAbsoluteInnerRect(relelement); break;
    case NA_UI_BUTTON:      relrect = naGetButtonAbsoluteInnerRect(relelement); break;
    case NA_UI_RADIO:       relrect = naGetRadioAbsoluteInnerRect(relelement); break;
    case NA_UI_CHECKBOX:    relrect = naGetCheckBoxAbsoluteInnerRect(relelement); break;
    case NA_UI_LABEL:       relrect = naGetLabelAbsoluteInnerRect(relelement); break;
    case NA_UI_TEXTFIELD:   relrect = naGetTextFieldAbsoluteInnerRect(relelement); break;
    case NA_UI_TEXTBOX:     relrect = naGetTextBoxAbsoluteInnerRect(relelement); break;
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
