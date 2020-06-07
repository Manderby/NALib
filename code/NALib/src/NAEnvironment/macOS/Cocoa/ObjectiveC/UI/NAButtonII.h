
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


// Push (Text only) (24px height fixed)
// Option (Text / Image) (3px padding on all sides)
// Borderless (Image only) (0px padding on all sides)

@implementation NACocoaButton
- (id) initWithCoreButton:(NA_Button*)newcoreButton bezelStyle:(NSBezelStyle)bezelStyle frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  [self setButtonType:(bezelStyle == NABezelStyleRounded) ? NAButtonTypeMomentaryLight : NAButtonTypePushOnPushOff];
  // NSBezelStyleShadowlessSquare is used to have a transparent background. The option 0 has a grey background.
  [self setBezelStyle:bezelStyle ? bezelStyle : NABezelStyleShadowlessSquare]; 
  [self setBordered:bezelStyle ? YES : NO];
  coreButton = newcoreButton;
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
      NSSize imageSize = NSMakeSize(naGetUIImage1xSize(uiimage).width, naGetUIImage1xSize(uiimage).height);
      image = [NSImage imageWithSize:imageSize flipped:NO drawingHandler:^BOOL(NSRect dstRect) {
        NAUIImageResolution resolution;
        CGContextRef context = NA_NULL;
        CGImageRef cocoaimage;
        
        NAUIImageSkin skin = NA_UIIMAGE_SKIN_PLAIN;
        if(uiimage->tintMode != NA_BLEND_ZERO){
          skin = naGetSkinForCurrentAppearance();
        }

        resolution = naGetWindowUIResolution(naGetUIElementWindow(&(self->coreButton->uiElement)));
        context = naGetCGContextRef([NSGraphicsContext currentContext]);

        cocoaimage = na_GetUIImageNativeImage(uiimage, resolution, NA_UIIMAGE_KIND_MAIN, skin);
        if(!cocoaimage){
          cocoaimage = na_GetUIImageNativeImage(uiimage, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, skin);
        }
        CGContextDrawImage(context, dstRect, cocoaimage);
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
  na_DispatchUIElementCommand((NA_UIElement*)coreButton, NA_UI_COMMAND_PRESSED);
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
  NA_Button* coreButton = naAlloc(NA_Button);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithCoreButton:coreButton bezelStyle:NABezelStyleRounded frame:frameRect];
  na_InitButton(coreButton, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setButtonText:text];
  
  return (NAButton*)coreButton;
}



NA_DEF NAButton* naNewTextOptionButton(const NAUTF8Char* text, NASize size){
  NA_Button* coreButton = naAlloc(NA_Button);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithCoreButton:coreButton bezelStyle:NABezelStyleShadowlessSquare frame:frameRect];
  na_InitButton(coreButton, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setButtonText:text];
  
  return (NAButton*)coreButton;
}



NA_DEF NAButton* naNewImageOptionButton(NAUIImage* uiimage, NASize size){
  NA_Button* coreButton = naAlloc(NA_Button);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithCoreButton:coreButton bezelStyle:NABezelStyleShadowlessSquare frame:frameRect];
  na_InitButton(coreButton, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setButtonImage:uiimage];
  
  return (NAButton*)coreButton;
}



NA_DEF NAButton* naNewImageButton(NAUIImage* uiimage, NASize size){
  NA_Button* coreButton = naAlloc(NA_Button);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithCoreButton:coreButton bezelStyle:0 frame:frameRect];
  na_InitButton(coreButton, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setButtonImage:uiimage];
  
  return (NAButton*)coreButton;
}



NA_DEF void na_DestructButton(NAButton* button){
  NA_Button* coreButton = (NA_Button*)button;
  na_ClearButton(coreButton);
}



NA_DEF void naSetButtonState(NAButton* button, NABool state){
  naDefineCocoaObject(NACocoaButton, cocoaButton, button);
  [cocoaButton setButtonState:state];
}



NA_DEF void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, NAUIElement* controller){
  naDefineCocoaObject(NACocoaButton, cocoaButton, button);
  [cocoaButton setDefaultButton:NA_TRUE];
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER), handler, controller);
}



NA_DEF void naSetButtonAbort(NAButton* button, NAReactionHandler handler, NAUIElement* controller){
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ESC), handler, controller);
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_COMMAND, NA_KEYCODE_PERIOD), handler, controller);
}



NA_HDEF NARect na_GetButtonAbsoluteInnerRect(NA_UIElement* button){
  NA_UNUSED(button);
  return naMakeRectS(20, 40, 100, 50);
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
