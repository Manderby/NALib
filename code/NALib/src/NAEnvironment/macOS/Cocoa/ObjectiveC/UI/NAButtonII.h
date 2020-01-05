
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
- (id) initWithCoreButton:(NACoreButton*)newcorebutton bezelStyle:(NSBezelStyle)bezelStyle frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  [self setButtonType:(bezelStyle == NABezelStyleRounded) ? NAButtonTypeMomentaryLight : NAButtonTypePushOnPushOff];
  // NSBezelStyleShadowlessSquare is used to have a transparent background. The option 0 has a grey background.
  [self setBezelStyle:bezelStyle ? bezelStyle : NABezelStyleShadowlessSquare]; 
  [self setBordered:bezelStyle ? YES : NO];
  corebutton = newcorebutton;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];
  return self;
}
- (void) setButtonText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}
- (void) setButtonImage:(NAUIImage*)uiimage{
  NSImage* image = nil; // todo: this must be implemented before macOS 10.8

  if([NSImage respondsToSelector:@selector(imageWithSize:flipped:drawingHandler:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_8(
      NSSize imagesize = NSMakeSize(naGetUIImage1xSize(uiimage).width, naGetUIImage1xSize(uiimage).height);
      image = [NSImage imageWithSize:imagesize flipped:NO drawingHandler:^BOOL(NSRect dstRect) {
        NAUIImageResolution resolution;
        CGContextRef context = NA_NULL;
        CGImageRef nativeimage;
        
        NAUIImageSkin skin = NA_UIIMAGE_SKIN_PLAIN;
        if(uiimage->tintMode != NA_BLEND_ZERO){
          skin = naGetSkinForCurrentAppearance();
        }

        resolution = naGetWindowUIResolution(naGetUIElementWindow(&(self->corebutton->uielement)));
        context = naGetCGContextRef([NSGraphicsContext currentContext]);

        nativeimage = naGetUIImageNativeImage(uiimage, resolution, NA_UIIMAGE_KIND_MAIN, skin);
        if(!nativeimage){
          nativeimage = naGetUIImageNativeImage(uiimage, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, skin);
        }
        CGContextDrawImage(context, dstRect, nativeimage);
        return YES;
      }];
    ) // end NA_MACOS_AVAILABILITY_GUARD_10_8
  }else{
    image = NA_COCOA_PTR_C_TO_OBJC(naAllocNativeImageWithUIImage(uiimage, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_LIGHT));
  }

  [self setImage:image];
  [[self cell] setImageScaling:NSImageScaleProportionallyUpOrDown];
  // OptionButton: NSBezelStyleShadowlessSquare
  // NSBezelStyleRegularSquare : 5 5 5 5
  // NSBezelStyleShadowlessSquare : 3 3 3 3
  // NSBezelStyleSmallSquare : 2 1 2 1
}
- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  naDispatchUIElementCommand((NACoreUIElement*)corebutton, NA_UI_COMMAND_PRESSED);
}
- (void) setButtonState:(NABool)state{
  [self setState:state ? NAStateOn : NAStateOff];
}
- (void) setDefaultButton:(NABool)isDefault{
  if(isDefault){
    [self setKeyEquivalent:@"\r"];
  }else{
    [self setKeyEquivalent:@""];
  }
}
@end



NA_DEF NAButton* naNewPushButton(const NAUTF8Char* text, NASize size){
  NACoreButton* corebutton = naAlloc(NACoreButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCoreButton:corebutton bezelStyle:NABezelStyleRounded frame:frameRect];
  naInitCoreButton(corebutton, NA_COCOA_PTR_OBJC_TO_C(nativeButton));
  [nativeButton setButtonText:text];
  
  return (NAButton*)corebutton;
}



NA_DEF NAButton* naNewTextOptionButton(const NAUTF8Char* text, NASize size){
  NACoreButton* corebutton = naAlloc(NACoreButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCoreButton:corebutton bezelStyle:NABezelStyleShadowlessSquare frame:frameRect];
  naInitCoreButton(corebutton, NA_COCOA_PTR_OBJC_TO_C(nativeButton));
  [nativeButton setButtonText:text];
  
  return (NAButton*)corebutton;
}



NA_DEF NAButton* naNewImageOptionButton(NAUIImage* uiimage, NASize size){
  NACoreButton* corebutton = naAlloc(NACoreButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCoreButton:corebutton bezelStyle:NABezelStyleShadowlessSquare frame:frameRect];
  naInitCoreButton(corebutton, NA_COCOA_PTR_OBJC_TO_C(nativeButton));
  [nativeButton setButtonImage:uiimage];
  
  return (NAButton*)corebutton;
}



NA_DEF NAButton* naNewImageButton(NAUIImage* uiimage, NASize size){
  NACoreButton* corebutton = naAlloc(NACoreButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NANativeButton* nativeButton = [[NANativeButton alloc] initWithCoreButton:corebutton bezelStyle:0 frame:frameRect];
  naInitCoreButton(corebutton, NA_COCOA_PTR_OBJC_TO_C(nativeButton));
  [nativeButton setButtonImage:uiimage];
  
  return (NAButton*)corebutton;
}



NA_DEF void naDestructButton(NAButton* button){
  NACoreButton* corebutton = (NACoreButton*)button;
  naClearCoreButton(corebutton);
}



NA_HDEF void naSetButtonState(NAButton* button, NABool state){
  naDefineNativeCocoaObject(NANativeButton, nativebutton, button);
  [nativebutton setButtonState:state];
}



NA_HDEF void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, NAUIElement* controller){
  naDefineNativeCocoaObject(NANativeButton, nativebutton, button);
  [nativebutton setDefaultButton:NA_TRUE];
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER), handler, controller);
}



NA_HDEF void naSetButtonAbort(NAButton* button, NAReactionHandler handler, NAUIElement* controller){
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ESC), handler, controller);
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_PERIOD), handler, controller);
}



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
