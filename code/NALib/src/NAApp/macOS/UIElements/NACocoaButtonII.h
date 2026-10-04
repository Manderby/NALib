
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#define NA_COCOA_DEFAULT_BUTTON_HEIGHT 24



struct NACocoaButton{
  NAButton button;
};
NA_HAPI void na_DestructCocoaButton(NACocoaButton* cocoaButton);
NA_RUNTIME_TYPE(NACocoaButton, na_DestructCocoaButton, NA_FALSE);


@implementation NACocoaNativeButton

- (id) initWithButton:(NACocoaButton*)newCocoaButton flags:(uint32)flags isImage:(bool)newIsImage frame:(NSRect)frame{
  self = [super initWithFrame:frame];

  isImage = newIsImage;

  if(naGetFlagu32(flags, NA_BUTTON_BORDERED)) {
//    [self setBezelStyle:naGetFlagu32(flags, NA_BUTTON_STATEFUL) ? NSBezelStyleShadowlessSquare : NABezelStylePush]; 
    [self setBezelStyle:naGetFlagu32(flags, NA_BUTTON_STATEFUL) ? NABezelStylePush : NABezelStylePush]; 
    [self setBordered:YES];
  }else{
    if(!isImage && naGetFlagu32(flags, NA_BUTTON_STATEFUL)) {
      [self setBezelStyle:NABezelStyleInline]; 
      [self setBordered:YES];
    }else{
      [self setBezelStyle:NABezelStylePush]; 
      [self setBordered:NO];
    }
  }

  if(isImage) {
    [self setButtonType:naGetFlagu32(flags, NA_BUTTON_STATEFUL) ? NAButtonTypePushOnPushOff : NSButtonTypeMomentaryChange];
    [[self cell] setImageScaling:NSImageScaleNone];
  }else{
    [self setButtonType:naGetFlagu32(flags, NA_BUTTON_STATEFUL) ? NAButtonTypePushOnPushOff : NAButtonTypeMomentaryLight];
  }
    
  cocoaButton = newCocoaButton;
  [self setTarget:self];
  [self setAction:@selector(onPressed:)];

  return self;
}

- (void)drawRect:(NSRect)dirtyRect{
  #if NA_DEBUG
    na_DrawLayoutDebugging([self frame], &cocoaButton->button.uiElement);
  #endif // NA_DEBUG
  [super drawRect:dirtyRect];
}

//- (BOOL)isAccessibilityElement
//{
//    return YES;
//}
//
//- (NSString *)accessibilityHelp
//{
//  if(cocoaButton->button.hint) {
//    return [NSString stringWithUTF8String:cocoaButton->button.hint];
//  }else{
//    return @"";
//  }
//}

- (bool) isImage{
  return isImage;
}

- (const NAUTF8Char*) currentText{
  NABool secondaryState = [self getButtonState];
  const NAUTF8Char* text = secondaryState
    ? cocoaButton->button.text2
    : cocoaButton->button.text; 
  if(secondaryState && !text) {
    text = cocoaButton->button.text;
  }
  return text;
}

- (void) updateButtonText{
  const NAUTF8Char* text = [self currentText];

  if(text) {
    [self setTitle:[NSString stringWithUTF8String:text]];
  }else{
    [self setTitle:@""];
  }
}

- (void) updateButtonBackground{
  NA_MACOS_AVAILABILITY_GUARD_10_14(
    // setBezelColor is 10.12.2 or newer
    // controlAccentColor is 10.14 or newer
    if([NSButton instancesRespondToSelector:@selector(setBezelColor:)]
      && [NSColor respondsToSelector:@selector(controlAccentColor)]) {
      if([self getButtonState]) {
        [self setBezelColor:[NSColor controlAccentColor]];
      }else{
        [self setBezelColor:nil];
      }
    }
  )
}

- (void) onPressed:(id)sender{
  NA_UNUSED(sender);
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaButton, NA_UI_COMMAND_PRESSED)) {
    // no super method to be called.
  }
  [self updateButtonBackground];
  [self updateButtonText];
  [self updateImages];
}

- (const NAImageSet*) currentImage{
  NABool secondaryState = [self getButtonState];
  const NAImageSet* imageSet = secondaryState
    ? cocoaButton->button.imageSet2
    : cocoaButton->button.imageSet; 
  if(secondaryState && !imageSet) {
    imageSet = cocoaButton->button.imageSet;
  }
  return imageSet;
}

- (void) updateImages{
  const NAImageSet* imageSet = [self currentImage];
  NABool secondaryState = [self getButtonState];
  double uiScale = naGetUIElementUIScale(&cocoaButton->button);

  if(imageSet) {
    if([self isEnabled]) {
      [self setImage:na_CreateResolutionIndependentNativeImage(
        imageSet,
        NA_IMAGE_SET_INTERACTION_NONE,
        secondaryState,
        uiScale,
        NA_FALSE)];
        
      [self setAlternateImage:na_CreateResolutionIndependentNativeImage(
        imageSet,
        NA_IMAGE_SET_INTERACTION_PRESSED,
        secondaryState,
        uiScale,
        NA_FALSE)];
    }else{
      [self setImage:na_CreateResolutionIndependentNativeImage(
        imageSet,
        NA_IMAGE_SET_INTERACTION_DISABLED,
        secondaryState,
        uiScale,
        NA_FALSE)];
        
      [self setAlternateImage:nil];
    }
  }else{
    [self setImage:nil];
    [self setAlternateImage:nil];
  }
}

- (void) mouseEntered:(NSEvent*)event{
  NA_UNUSED(event);
  const NAImageSet* imageSet = [self currentImage];
  NABool secondaryState = [self getButtonState];

  if(imageSet && [self isEnabled]) {
    double uiScale = naGetUIElementUIScale(&cocoaButton->button);
    [self setImage:na_CreateResolutionIndependentNativeImage(
      imageSet,
      NA_IMAGE_SET_INTERACTION_HOVER,
      secondaryState,
      uiScale,
        NA_FALSE)];
  }
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaButton, NA_UI_COMMAND_MOUSE_ENTERED)) {
    [super mouseEntered:event];
  }
}

- (void) mouseExited:(NSEvent*)event{
  NA_UNUSED(event);
  const NAImageSet* imageSet = [self currentImage];
  NABool secondaryState = [self getButtonState];

  if(imageSet) {
    double uiScale = naGetUIElementUIScale(&cocoaButton->button);
    [self setImage:na_CreateResolutionIndependentNativeImage(
      imageSet,
      NA_IMAGE_SET_INTERACTION_NONE,
      secondaryState,
      uiScale,
        NA_FALSE)];
  }
  if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaButton, NA_UI_COMMAND_MOUSE_EXITED)) {
    [super mouseExited:event];
  }
}

- (void) setButtonState:(NABool)state{
  [self setState:state ? NAStateOn : NAStateOff];
  [self updateButtonBackground];
  [self updateButtonText];
  [self updateImages];
}

- (NABool) getButtonState{
  return naGetFlagu32(cocoaButton->button.flags, NA_BUTTON_STATEFUL) && [self state] == NAStateOn;
}

- (void) setDefaultButton:(NABool)isDefault{
  if(isDefault) {
    [self setKeyEquivalent:@"\r"];
  }else{
    [self setKeyEquivalent:@""];
  }
}
  
- (void) setVisible:(NABool)visible{
  [self setHidden:visible ? NO : YES];
}

- (NARect) getInnerRect{
  return naMakeRectWithNSRect([self frame]);
}
@end



NA_DEF NAButton* naNewTextPushButton(const NAUTF8Char* text, double width) {
  NACocoaButton* cocoaButton = naNew(NACocoaButton);

  uint32 flags = NA_BUTTON_BORDERED;
  
  NACocoaNativeButton* nativeObj = [[NACocoaNativeButton alloc]
    initWithButton:cocoaButton
    flags:flags
    isImage:NO
    frame:naMakeNSRectWithSize(naMakeSize(
      width,
      NA_COCOA_DEFAULT_BUTTON_HEIGHT))];
      
  na_InitCoreButton(
    (NAButton*)cocoaButton,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    text,
    NA_NULL,
    NA_NULL,
    NA_NULL,
    flags);
  
  [nativeObj updateButtonText];
  
  return (NAButton*)cocoaButton;
}



NA_DEF NAButton* naNewTextStateButton(const NAUTF8Char* text, const NAUTF8Char* text2, double width) {
  
  NACocoaButton* cocoaButton = naNew(NACocoaButton);

  uint32 flags = NA_BUTTON_STATEFUL | NA_BUTTON_BORDERED;

  NACocoaNativeButton* nativeObj = [[NACocoaNativeButton alloc]
    initWithButton:cocoaButton
    flags:flags
    isImage:NO
    frame:naMakeNSRectWithSize(naMakeSize(
      width,
      NA_COCOA_DEFAULT_BUTTON_HEIGHT))];
      
  na_InitCoreButton(
    (NAButton*)cocoaButton,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    text,
    text2,
    NA_NULL,
    NA_NULL,
    flags);
  
  [nativeObj updateButtonText];
  
  return (NAButton*)cocoaButton;
}



NA_DEF NAButton* naNewIconPushButton(const NAImageSet* icon, double width) {
  NACocoaButton* cocoaButton = naNew(NACocoaButton);
  
  uint32 flags = NA_BUTTON_BORDERED;

  NACocoaNativeButton* nativeObj = [[NACocoaNativeButton alloc]
    initWithButton:cocoaButton
    flags:flags
    isImage:YES
    frame:naMakeNSRectWithSize(naMakeSize(
      width,
      NA_COCOA_DEFAULT_BUTTON_HEIGHT))];
      
  na_InitCoreButton(
    (NAButton*)cocoaButton,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    NA_NULL,
    NA_NULL,
    icon,
    NA_NULL,
    flags);
  
  [nativeObj updateImages];
  
  // For hovering effects:
  na_RetainMouseTracking(&cocoaButton->button.uiElement);

  return (NAButton*)cocoaButton;
}



NA_DEF NAButton* naNewIconStateButton(const NAImageSet* icon, const NAImageSet* icon2, double width) {
  NACocoaButton* cocoaButton = naNew(NACocoaButton);
  
  uint32 flags = NA_BUTTON_STATEFUL | NA_BUTTON_BORDERED;

  NACocoaNativeButton* nativeObj = [[NACocoaNativeButton alloc]
    initWithButton:cocoaButton
    flags:flags
    isImage:YES
    frame:naMakeNSRectWithSize(naMakeSize(
      width,
      NA_COCOA_DEFAULT_BUTTON_HEIGHT))];
      
  na_InitCoreButton(
    (NAButton*)cocoaButton,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    NA_NULL,
    NA_NULL,
    icon,
    icon2 ? icon2 : icon,
    flags);
      
  [nativeObj updateImages];

  // For hovering effects:
  na_RetainMouseTracking(&cocoaButton->button.uiElement);

  return (NAButton*)cocoaButton;
}



NA_DEF NAButton* naNewImagePushButton(const NAImageSet* imageSet, NASize size) {
  NACocoaButton* cocoaButton = naNew(NACocoaButton);
  
  uint32 flags = 0;

  NACocoaNativeButton* nativeObj = [[NACocoaNativeButton alloc]
    initWithButton:cocoaButton
    flags:flags
    isImage:YES
    frame:naMakeNSRectWithSize(size)];
    
  na_InitCoreButton(
    (NAButton*)cocoaButton,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    NA_NULL,
    NA_NULL,
    imageSet,
    NA_NULL,
    flags);
  
  [nativeObj updateImages];

  // For hovering effects:
  na_RetainMouseTracking(&cocoaButton->button.uiElement);

  return (NAButton*)cocoaButton;
}



NA_DEF NAButton* naNewImageStateButton(const NAImageSet* imageSet, const NAImageSet* imageSet2, NASize size) {
  NACocoaButton* cocoaButton = naNew(NACocoaButton);
  
  uint32 flags = NA_BUTTON_STATEFUL;

  NACocoaNativeButton* nativeObj = [[NACocoaNativeButton alloc]
    initWithButton:cocoaButton
    flags:flags
    isImage:YES
    frame:naMakeNSRectWithSize(size)];
    
  na_InitCoreButton(
    (NAButton*)cocoaButton,
    NA_COCOA_PTR_OBJC_TO_C(nativeObj),
    NA_NULL,
    NA_NULL,
    imageSet,
    imageSet2,
    flags);
  
  [nativeObj updateImages];

  // For hovering effects:
  na_RetainMouseTracking(&cocoaButton->button.uiElement);

  return (NAButton*)cocoaButton;
}



NA_DEF void na_DestructCocoaButton(NACocoaButton* cocoaButton) {
  if(cocoaButton->button.imageSet) {
    na_ReleaseMouseTracking(&cocoaButton->button.uiElement);
  }
  na_ClearCoreButton((NAButton*)cocoaButton);
}



NA_DEF void naSetButtonEnabled(NAButton* button, NABool enabled) {
  #if NA_DEBUG
  if(!button)
    naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);

  [nativeObj setEnabled:(BOOL)enabled];
  [nativeObj updateImages];
}



NA_DEF void naSetButtonText(NAButton* button, const NAUTF8Char* text) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  
  #if NA_DEBUG
    if([nativeObj isImage])
      naError("This is not a text button");
  #endif

  na_setButtonText(button, text);
  [nativeObj updateButtonText];
}



NA_DEF void naSetButtonText2(NAButton* button, const NAUTF8Char* text) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif
  
  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  
  #if NA_DEBUG
    if([nativeObj isImage])
      naError("This is not a text button");
  #endif
  
  na_setButtonText2(button, text);
  [nativeObj updateButtonText];
}



NA_DEF void naSetButtonImage(NAButton* button, const NAImageSet* imageSet) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);

  #if NA_DEBUG
    if(![nativeObj isImage])
      naError("This is not an image button.");
  #endif

  na_setButtonImage(button, imageSet);
  [nativeObj updateImages];
}



NA_DEF void naSetButtonImage2(NAButton* button, const NAImageSet* imageSet) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);

  #if NA_DEBUG
    if(![nativeObj isImage])
      naError("This is not an image button.");
  #endif

  na_setButtonImage2(button, imageSet);
  [nativeObj updateImages];
}



NA_DEF NABool naIsButtonStateful(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  return naGetFlagu32(button->flags, NA_BUTTON_STATEFUL);
}



NA_DEF NABool naIsButtonBordered(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  return naGetFlagu32(button->flags, NA_BUTTON_BORDERED);
}



NA_DEF NABool naIsButtonTextual(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObjConst(NACocoaNativeButton, nativeObj, button);
  return ![nativeObj isImage];
}



NA_DEF NABool naGetButtonState(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObjConst(const NACocoaNativeButton, nativeObj, button);
  #if NA_DEBUG
  if(!naGetFlagu32(button->flags, NA_BUTTON_STATEFUL))
    naError("This is not a stateful button");
  #endif

  return [nativeObj getButtonState];
}



NA_DEF void naSetButtonState(NAButton* button, NABool state) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  if(naGetFlagu32(button->flags, NA_BUTTON_STATEFUL)) {
    [nativeObj setButtonState:state];
  }else{
    #if NA_DEBUG
    naError("This is not a stateful button");
    #endif
  }
}



NA_DEF void naSetButtonSubmit(
  NAButton* button,
  NAReactionCallback callback,
  void* controller)
{
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
    if(naGetFlagu32(button->flags, NA_BUTTON_STATEFUL))
      naError("Abort functionality only works reliably for push buttons");
  #endif // NA_DEBUG

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  [nativeObj setDefaultButton:NA_TRUE];
  
  NAWindow* window = naGetUIElementWindowMutable(button);
  if(window) {
    naAddUIKeyboardShortcut(
      window,
      naNewKeyStroke(NA_KEYCODE_ENTER, NA_KEY_MODIFIER_NONE),
      callback,
      controller);
    naAddUIKeyboardShortcut(
      window,
      naNewKeyStroke(NA_KEYCODE_NUMPAD_ENTER, NA_KEY_MODIFIER_NONE),
      callback,
      controller);
  }else{
    #if NA_DEBUG
      naError("Submit button requires NAWindow to work.");
    #endif
  }
}



NA_DEF void naSetButtonAbort(
  NAButton* button,
  NAReactionCallback callback,
  void* controller)
{
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
    if(naGetFlagu32(button->flags, NA_BUTTON_STATEFUL))
      naError("Abort functionality only works reliably for push buttons");
  #endif
  
  NAWindow* window = naGetUIElementWindowMutable(button);
  if(window) {
    naAddUIKeyboardShortcut(
      window,
      naNewKeyStroke(NA_KEYCODE_ESCAPE, NA_KEY_MODIFIER_NONE),
      callback,
      controller);
    naAddUIKeyboardShortcut(
      window,
      naNewKeyStroke(NA_KEYCODE_PERIOD, NA_KEY_MODIFIER_COMMAND),
      callback,
      controller);
  }
}



NA_API void naSetButtonVisible(NAButton* button, NABool visible) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  [nativeObj setVisible:visible];
}


NA_HDEF NARect na_GetButtonRect(const NA_UIElement* button) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObjConst(NACocoaNativeButton, nativeObj, button);
  return naMakeRectWithNSRect([nativeObj frame]);
}

NA_HDEF void na_SetButtonRect(NA_UIElement* button, NARect rect) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  [nativeObj setFrame:naMakeNSRectWithRect(rect)];
  na_UpdateMouseTracking(button);
}

NA_HDEF void na_UpdateButtonUIScale(NA_UIElement* button) {
  naDefineUIElementNativeCocoaObj(NACocoaNativeButton, nativeObj, button);
  [nativeObj updateImages];
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
