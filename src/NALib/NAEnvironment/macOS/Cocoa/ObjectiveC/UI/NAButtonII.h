
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


// Push (Text only) (24px height fixed)
// Option (Text / Image) (3px padding on all sides)
// Borderless (Image only) (0px padding on all sides)

@implementation NANativeButton
- (id) initWithCocoaButton:(NACocoaButton*)newcocoabutton bezelStyle:(NSBezelStyle)bezelStyle frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  [self setButtonType:(bezelStyle == NSRoundedBezelStyle) ? NSButtonTypeMomentaryLight : NSButtonTypePushOnPushOff];
  // NSBezelStyleShadowlessSquare is used to have a transparent background. The option 0 has a grey background.
  [self setBezelStyle:bezelStyle ? bezelStyle : NSBezelStyleShadowlessSquare]; 
  [self setBordered:bezelStyle ? YES : NO];
  cocoabutton = newcocoabutton;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];
  return self;
}
- (void) setButtonText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}
- (void) setButtonImage:(const char*)imagePath{
  NSURL* url = [NSURL fileURLWithPath:[NSString stringWithUTF8String:imagePath]];
  NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
//  CGImageRef imgRef = [image CGImageForProposedRect:nil context:nil hints:nil];
  [self setImage:image];
  [self setImageScaling:NSImageScaleProportionallyUpOrDown];
  [image release];
//  [self setBezelStyle:NSBezelStyleShadowlessSquare];
  // OptionButton: NSBezelStyleShadowlessSquare
  // NSBezelStyleRegularSquare : 5 5 5 5
  // NSBezelStyleShadowlessSquare : 3 3 3 3
  // NSBezelStyleSmallSquare : 2 1 2 1
//  [self setBordered:NO];
}
- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  naDispatchUIElementCommand((NACoreUIElement*)cocoabutton, NA_UI_COMMAND_PRESSED, NA_NULL);
}
- (void) setButtonState:(NABool)state{
  [self setState:state ? NSOnState : NSOffState];
}
//- (void) simulatePress{
//  [self performClick:nil];
//}
- (void) setDefaultButton:(NABool)isDefault{
  if(isDefault){
    [self setKeyEquivalent:@"\r"];
  }else{
    [self setKeyEquivalent:@""];
  }
}
@end



NA_DEF NAButton* naNewPushButton(const NAUTF8Char* text, NARect rect){
  NACocoaButton* cocoabutton = naAlloc(NACocoaButton);

  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCocoaButton:cocoabutton bezelStyle:NSBezelStyleRounded frame:naMakeNSRectWithRect(rect)];
  naRegisterCoreUIElement(&(cocoabutton->corebutton.uielement), NA_UI_BUTTON, (void*)NA_COCOA_RETAIN(nativeButton));
  [nativeButton setButtonText:text];
  
  return (NAButton*)cocoabutton;
}



NA_DEF NAButton* naNewTextOptionButton(const NAUTF8Char* text, NARect rect){
  NACocoaButton* cocoabutton = naAlloc(NACocoaButton);

  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCocoaButton:cocoabutton bezelStyle:NSBezelStyleShadowlessSquare frame:naMakeNSRectWithRect(rect)];
  naRegisterCoreUIElement(&(cocoabutton->corebutton.uielement), NA_UI_BUTTON, (void*)NA_COCOA_RETAIN(nativeButton));
  [nativeButton setButtonText:text];
  
  return (NAButton*)cocoabutton;
}



NA_DEF NAButton* naNewImageOptionButton(const char* imagePath, NARect rect){
  NACocoaButton* cocoabutton = naAlloc(NACocoaButton);

  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCocoaButton:cocoabutton bezelStyle:NSBezelStyleShadowlessSquare frame:naMakeNSRectWithRect(rect)];
  naRegisterCoreUIElement(&(cocoabutton->corebutton.uielement), NA_UI_BUTTON, (void*)NA_COCOA_RETAIN(nativeButton));
  [nativeButton setButtonImage:imagePath];
  
  return (NAButton*)cocoabutton;
}



NA_DEF NAButton* naNewImageButton(const char* imagePath, NARect rect){
  NACocoaButton* cocoabutton = naAlloc(NACocoaButton);

  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCocoaButton:cocoabutton bezelStyle:0 frame:naMakeNSRectWithRect(rect)];
  naRegisterCoreUIElement(&(cocoabutton->corebutton.uielement), NA_UI_BUTTON, (void*)NA_COCOA_RETAIN(nativeButton));
  [nativeButton setButtonImage:imagePath];
  
  return (NAButton*)cocoabutton;
}



NA_DEF void naDestructButton(NAButton* button){
  NACocoaButton* cocoabutton = (NACocoaButton*)button;
  NA_COCOA_RELEASE(naUnregisterCoreUIElement(&(cocoabutton->corebutton.uielement)));
}



NA_HDEF void naSetButtonState(NAButton* button, NABool state){
  [((NA_COCOA_BRIDGE NANativeButton*)naGetUIElementNativeID(button)) setButtonState:state];
}



NA_HDEF void naSetButtonSubmit(NAButton* button, NAUIElement* controller, NAReactionHandler handler){
  [((NA_COCOA_BRIDGE NANativeButton*)naGetUIElementNativeID(button)) setDefaultButton:NA_TRUE];
  naAddUIKeyboardShortcut(controller, naGetUIElementWindow(button), naMakeKeybardShortcut(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER), handler);
}



NA_HDEF void naSetButtonAbort(NAButton* button, NAUIElement* controller, NAReactionHandler handler){
  naAddUIKeyboardShortcut(controller, naGetUIElementWindow(button), naMakeKeybardShortcut(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ESC), handler);
  naAddUIKeyboardShortcut(controller, naGetUIElementWindow(button), naMakeKeybardShortcut(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_PERIOD), handler);
}



//NA_HDEF void naSimulateButtonPress(NAButton* button){
//  [((NA_COCOA_BRIDGE NANativeButton*)naGetUIElementNativeID(button)) simulatePress];
//}



NA_HDEF NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* button){
  NA_UNUSED(button);
  return naMakeRectS(20, 40, 100, 50);
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
