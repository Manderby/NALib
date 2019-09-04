
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


//#include "NAUICocoa.h"
//#include <objc/message.h>


#include "NAUICore.h"
#include "NAMemory.h"
#include "NACoord.h"
#include "NAThreading.h"
#include "NATranslator.h"


typedef struct NACocoaApplication NACocoaApplication;
typedef struct NACocoaWindow      NACocoaWindow;
typedef struct NACocoaSpace       NACocoaSpace;
typedef struct NACocoaImageSpace  NACocoaImageSpace;
typedef struct NACocoaOpenGLSpace NACocoaOpenGLSpace;
typedef struct NACocoaButton      NACocoaButton;
typedef struct NACocoaRadio       NACocoaRadio;
typedef struct NACocoaCheckBox    NACocoaCheckBox;
typedef struct NACocoaLabel       NACocoaLabel;
typedef struct NACocoaTextField   NACocoaTextField;
typedef struct NACocoaTextBox     NACocoaTextBox;


// ////////////////
// The bridging structs
//
// Native:       Adapter for the native implementation (in a specific language)
// Cocoa/WINAPI: Adapter for the technology (for any language)
// Core:         Adapter for NALib
// UIElement:    Direct pointer back to the native struct.
//
// Native:       Compiled with Objective-C - native class inheriting from Cocoa
// Cocoa/WINAPI: Compiled with Objective-C - C struct inheriting Core
// Core:         Compiled with C           - C struct inheriting UIElement
// UIElement:    Compiled with C           - C struct pointer to native.
//
// Native:       Compiled with Swift       - native class inheriting from Cocoa
// Cocoa/WINAPI: Compiled with Swift       - C struct inheriting Core
// Core:         Compiled with C           - C struct inheriting UIElement
// UIElement:    Compiled with C           - C struct pointer to native.

struct NACocoaApplication{
  NACoreApplication coreapplication;
};

struct NACocoaWindow{
  NACoreWindow corewindow;
};

struct NACocoaSpace{
  NACoreSpace corespace;
};

struct NACocoaImageSpace{
  NACoreImageSpace coreimagespace;
};

struct NACocoaOpenGLSpace{
  NACoreOpenGLSpace coreopenglspace;
};

struct NACocoaButton{
  NACoreButton corebutton;
};

struct NACocoaRadio{
  NACoreRadio coreradio;
};

struct NACocoaCheckBox{
  NACoreCheckBox corecheckbox;
};

struct NACocoaLabel{
  NACoreLabel corelabel;
};

struct NACocoaTextField{
  NACoreTextField coretextfield;
};

struct NACocoaTextBox{
  NACoreTextBox coretextbox;
};



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

NA_HAPI void naRenewWindowMouseTracking(NACocoaWindow* cocoawindow);
NA_HAPI void naClearWindowMouseTracking(NACocoaWindow* cocoawindow);


@interface NANativeApplicationDelegate : NSObject <NSApplicationDelegate>{
  NACocoaApplication* cocoaapplication;
}
@end

@interface NANativeWindow : NSWindow <NSWindowDelegate>{
  NACocoaWindow* cocoawindow;
  NAUInt trackingcount;
  NSTrackingArea* trackingarea;
}
@end

@interface NANativeSpace : NSView{
  NACocoaSpace* cocoaspace;
  NSTrackingArea* trackingarea;
}
@end

@interface NANativeImageSpace : NSImageView{
  NACocoaImageSpace* cocoaimagespace;
}
@end

#if (NA_CONFIG_COMPILE_OPENGL == 1)
  @interface NANativeOpenGLSpace : NSOpenGLView{
    NACocoaOpenGLSpace* cocoaopenglspace;
    NAMutator initFunc;
    void* initData;
  }
  @end
#endif

@interface NANativeButton : NSButton{
  NACocoaButton* cocoabutton;
}
@end

@interface NANativeRadio : NSButton{
  NACocoaRadio* cocoaradio;
  // Cocoa thinks it's smart by doing things automatically. Unfortunately, we
  // have to encapsulate the radio into its own view to get the behaviour
  // we need.
  NSView* containingview;
}
- (NSView*) getContainingView;
@end

@interface NANativeCheckBox : NSButton{
  NACocoaCheckBox* cocoacheckbox;
}
@end

@interface MDVerticallyCenteredTextFieldCell : NSTextFieldCell{
}
@end

@interface NANativeLabel : NSTextField{
  NACocoaLabel* cocoalabel;
}
@end

@interface NANativeTextField : NSTextField <NSTextFieldDelegate>{
  NACocoaTextField* cocoatextfield;
}
@end

@interface NANativeTextBox : NSTextView{
  NACocoaTextBox* cocoatextbox;
  NSScrollView* scrollview;
}
- (NSView*) getContainingView;
@end




NA_HDEF void naSetUIElementNextTabElement(NAUIElement* elem, NAUIElement* nextelem){
  [((NA_COCOA_BRIDGE NANativeTextField*)naGetUIElementNativeID(elem)) setNextKeyView:naGetUIElementNativeID(nextelem)];
}


NA_HDEF NABool naInterceptKeyboardShortcut(NSEvent* event){
  NABool retvalue = NA_FALSE;
  if([event type] == NSEventTypeKeyDown){
    
    // Search for the native first responder which is represented in NALib.
    NACoreUIElement* elem = NA_NULL;
    NSWindow* keyWindow = [NSApp keyWindow];
    if(keyWindow){
      NSResponder* firstResponder = [keyWindow firstResponder];
      if(firstResponder){
        while(!elem && firstResponder){
          elem = naGetUINALibEquivalent(firstResponder);
          if(!elem){
            if(firstResponder == keyWindow){
              elem = naGetApplication();
            }else{
              firstResponder = [(NSView*)firstResponder superview];
            }
          }
        }
      }else{
        elem = naGetUINALibEquivalent(keyWindow);
      }
    }else{
      elem = naGetApplication();
    }
    
    // Search for a matching keyboard shortcut by bubbling.
    while(!retvalue && elem){
      NAListIterator iter = naMakeListAccessor(&(elem->shortcuts));
      while(!retvalue && naIterateList(&iter)){
        const NAKeyboardShortcutReaction* shortcutReaction = naGetListCurConst(&iter);
        NAUIKeyCode keyCode = [event keyCode];
        if(shortcutReaction->shortcut.keyCode == keyCode){
          NSEventModifierFlags flags = [event modifierFlags];
          NABool needsShift   = (shortcutReaction->shortcut.modifiers & NA_MODIFIER_FLAG_SHIFT)   != 0;
          NABool needsControl = (shortcutReaction->shortcut.modifiers & NA_MODIFIER_FLAG_CONTROL) != 0;
          NABool needsOption  = (shortcutReaction->shortcut.modifiers & NA_MODIFIER_FLAG_OPTION)  != 0;
          NABool needsCommand = (shortcutReaction->shortcut.modifiers & NA_MODIFIER_FLAG_COMMAND) != 0;
          NABool hasShift     = (flags & NAEventModifierFlagShift)   != 0;
          NABool hasControl   = (flags & NAEventModifierFlagControl) != 0;
          NABool hasOption    = (flags & NAEventModifierFlagOption)  != 0;
          NABool hasCommand   = (flags & NAEventModifierFlagCommand) != 0;
          if(needsShift   == hasShift
          && needsControl == hasControl
          && needsOption  == hasOption
          && needsCommand == hasCommand){
            retvalue = shortcutReaction->handler(shortcutReaction->controller, na_app, NA_UI_COMMAND_KEYBOARD_SHORTCUT, &keyCode);
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
  [((NA_COCOA_BRIDGE NSView*)naGetUIElementNativeID(uielement)) setNeedsDisplay:YES];
}


NA_DEF void naSetUIElementParent (NAUIElement* uielement, NAUIElement* parent){
  NACoreUIElement* coreelement = (NACoreUIElement*)uielement;
  // todo: remove from old parent
  coreelement->parent = parent;
}










NSFont* getNSFontWithKind(NAFontKind kind){
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
      naError("Unknown font kind");
      break;
  }
  return font;
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
  mainframe = [[NSScreen mainScreen] frame];
  frame = [(NA_COCOA_BRIDGE NSScreen*)(naGetUIElementNativeID((NAUIElement*)screen)) frame];
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
    case NA_ALERT_BOX_INFO:    alert.alertStyle = NSInformationalAlertStyle; break;
    case NA_ALERT_BOX_WARNING: alert.alertStyle = NSAlertStyleWarning; break;
    case NA_ALERT_BOX_ERROR:   alert.alertStyle = NSAlertStyleCritical; break;
    }
    alert.messageText = [NSString stringWithUTF8String:titleText];
    alert.informativeText = [NSString stringWithUTF8String:infoText];
    [alert runModal];
    [alert release];
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
