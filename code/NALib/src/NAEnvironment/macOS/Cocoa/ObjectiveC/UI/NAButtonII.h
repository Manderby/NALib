
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



NSImage* naCreateResolutionIndependentNSImage(NSView* viewOnScreen, const NAUIImage* uiImage, NAUIImageKind imageKind){
  NSImage* image = nil; // todo: this must be implemented before macOS 10.8

  if([NSImage respondsToSelector:@selector(imageWithSize:flipped:drawingHandler:)]){
    NA_MACOS_AVAILABILITY_GUARD_10_8(
      NSSize imageSize = NSMakeSize(naGetUIImage1xSize(uiImage).width, naGetUIImage1xSize(uiImage).height);
      image = [NSImage imageWithSize:imageSize flipped:NO drawingHandler:^BOOL(NSRect dstRect) {
        CGContextRef context = NA_NULL;
        CGImageRef cocoaimage;
        
        NAUIImageSkin skin = NA_UIIMAGE_SKIN_PLAIN;
        if(uiImage->tintMode != NA_BLEND_ZERO){
          skin = naGetSkinForCurrentAppearance();
        }

        context = naGetCGContextRef([NSGraphicsContext currentContext]);
        NAUIImageResolution resolution = naGetWindowBackingScaleFactor([viewOnScreen window]) == 2. ? NA_UIIMAGE_RESOLUTION_2x : NA_UIIMAGE_RESOLUTION_1x;

        cocoaimage = na_GetUIImageNativeImage(uiImage, resolution, imageKind, skin);
        if(!cocoaimage){
          cocoaimage = na_GetUIImageNativeImage(uiImage, NA_UIIMAGE_RESOLUTION_1x, imageKind, skin);
        }
        CGContextDrawImage(context, dstRect, cocoaimage);
        return YES;
      }];
    ) // end NA_MACOS_AVAILABILITY_GUARD_10_8
  }else{
    image = NA_COCOA_PTR_C_TO_OBJC(naAllocNativeImageWithUIImage(uiImage, imageKind, NA_UIIMAGE_SKIN_LIGHT));
  }
  return image;
}


void naTellNSButtonSetUIImage(void* nsButton, const NAUIImage* uiImage){
  NSButton* button = (NA_COCOA_BRIDGE NSButton*)(nsButton);

  [button setImage:naCreateResolutionIndependentNSImage(button, uiImage, NA_UIIMAGE_KIND_MAIN)];
  [button setAlternateImage:naCreateResolutionIndependentNSImage(button, uiImage, NA_UIIMAGE_KIND_ALT)];
  [[button cell] setImageScaling:NSImageScaleProportionallyUpOrDown];
  // OptionButton: NSBezelStyleShadowlessSquare
  // NSBezelStyleRegularSquare : 5 5 5 5
  // NSBezelStyleShadowlessSquare : 3 3 3 3
  // NSBezelStyleSmallSquare : 2 1 2 1
}



// Push (Text only) (24px height fixed)
// Option (Text / Image) (3px padding on all sides)
// Borderless (Image only) (0px padding on all sides)

@implementation NACocoaButton
- (id) initWithButton:(NAButton*)newButton bezelStyle:(NSBezelStyle)bezelStyle frame:(NSRect)frame{
  self = [super initWithFrame:frame];
  [self setButtonType:(bezelStyle == NABezelStyleRounded) ? NAButtonTypeMomentaryLight : NAButtonTypePushOnPushOff];
  // NSBezelStyleShadowlessSquare is used to have a transparent background. The option 0 has a grey background.
  [self setBezelStyle:bezelStyle ? bezelStyle : NABezelStyleShadowlessSquare]; 
  [self setBordered:bezelStyle ? YES : NO];
  button = newButton;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];
  return self;
}
- (void) setButtonText:(const NAUTF8Char*)text{
  [self setTitle:[NSString stringWithUTF8String:text]];
}
- (void) setUIImage:(NAUIImage*)uiImage{
  naTellNSButtonSetUIImage((NA_COCOA_BRIDGE void*)self, uiImage);
}
- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  na_DispatchUIElementCommand((NA_UIElement*)button, NA_UI_COMMAND_PRESSED);
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
  NAButton* button = naAlloc(NAButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithButton:button bezelStyle:NABezelStyleRounded frame:frameRect];
  na_InitButton(button, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setTag: (NSInteger)button];
  [cocoaButton setButtonText:text];
  
  return button;
}



NA_DEF NAButton* naNewTextOptionButton(const NAUTF8Char* text, NASize size){
  NAButton* button = naAlloc(NAButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithButton:button bezelStyle:NABezelStyleShadowlessSquare frame:frameRect];
  na_InitButton(button, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setButtonText:text];
  
  return button;
}



NA_DEF NAButton* naNewImageOptionButton(NAUIImage* uiImage, NASize size){
  NAButton* button = naAlloc(NAButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithButton:button bezelStyle:NABezelStyleShadowlessSquare frame:frameRect];
  na_InitButton(button, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setUIImage:uiImage];
  
  return button;
}



NA_DEF NAButton* naNewImageButton(NAUIImage* uiImage, NASize size){
  NAButton* button = naAlloc(NAButton);

  NSRect frameRect = NSMakeRect((CGFloat)0., (CGFloat)0., (CGFloat)size.width, (CGFloat)size.height);
  NACocoaButton* cocoaButton = [[NACocoaButton alloc] initWithButton:button bezelStyle:0 frame:frameRect];
  na_InitButton(button, NA_COCOA_PTR_OBJC_TO_C(cocoaButton));
  [cocoaButton setUIImage:uiImage];
  
  return button;
}



NA_DEF void na_DestructButton(NAButton* button){
  na_ClearButton(button);
}



NA_DEF void naSetButtonState(NAButton* button, NABool state){
  naDefineCocoaObject(NACocoaButton, cocoaButton, button);
  [cocoaButton setButtonState:state];
}



NA_DEF void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, void* controller){
  naDefineCocoaObject(NACocoaButton, cocoaButton, button);
  [cocoaButton setDefaultButton:NA_TRUE];
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER), handler, controller);
}



NA_DEF void naSetButtonAbort(NAButton* button, NAReactionHandler handler, void* controller){
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
