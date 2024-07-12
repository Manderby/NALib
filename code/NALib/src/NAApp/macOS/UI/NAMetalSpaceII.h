
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#if(NA_COMPILE_METAL == 1) && defined __MAC_10_12
// Metal is only available since macOS 10.11, CALayerDelegate since 10.12

  @implementation NACocoaNativeMetalSpace
  
  - (id)initWithMetalSpace:(NACocoaMetalSpace*)newCocoaMetalSpace frame:(NSRect)frameRect{
    self = [super initWithFrame:frameRect];

    if(@available(macOS 10.11, *)) {
      CAMetalLayer* metalLayer = NA_COCOA_AUTORELEASE([[CAMetalLayer alloc] init]);
      metalLayer.frame = [self frame];
      metalLayer.device = MTLCreateSystemDefaultDevice();
      metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
      metalLayer.framebufferOnly = true;
      metalLayer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
      
      // See documentation of setWantsLayer: This is a hosting-layer situation
      // where setting the layer first and then setting wantsLayer is crucial!
      [self setLayer: metalLayer];
      [self setWantsLayer:YES];
      // Do not reorder these two lines!
      
      [metalLayer setDelegate:self];
            
      [self setLayerContentsRedrawPolicy:NSViewLayerContentsRedrawOnSetNeedsDisplay];
    }

    cocoaMetalSpace = newCocoaMetalSpace;
    return self;
  }
  
  - (BOOL)acceptsFirstResponder{
    return YES; // This is required to get keyboard input.
  }
  
  - (void) displayLayer:(CALayer *)layer {
    NA_UNUSED(layer);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_REDRAW)) {
      // If it is not handeled, just do nothing. Note that calling displayLayer
      // of the CALayerDelegate would just recursively call this method again
      // leading to an infinite recursion.
    }
  }
  
  - (void)adjustLayerFrame{
    double uiScale = naGetWindowBackingScaleFactor([self window]);
    NSRect frame = [self frame];
    frame.size.width *= uiScale;
    frame.size.height *= uiScale;
    #if NA_DEBUG
      if(frame.size.width == 0 || frame.size.height == 0)
        naError("Frame size is zero");
    #endif
    NA_MACOS_AVAILABILITY_GUARD_10_11(
      [(CAMetalLayer*)[self layer] setDrawableSize:frame.size];
      [(CAMetalLayer*)[self layer] setContentsScale:uiScale];
    )
    [self setNeedsDisplay:YES];
  }

  - (void)setFrame:(NSRect) frame{
    [super setFrame:frame];
    [self adjustLayerFrame];
  }

  - (void)viewDidChangeBackingProperties{
    [super viewDidChangeBackingProperties];
    if([self layer]) {
      [self adjustLayerFrame];
    }
  }

  - (void)mouseMoved:(NSEvent*)event{
    na_SetMouseMovedTo(naMakePosWithNSPoint([NSEvent mouseLocation]));
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
      [super mouseMoved:event];
    }
    [NSEvent setMouseCoalescingEnabled:NO];
  }

  - (void)mouseDown:(NSEvent*)event{
    na_SetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
    na_SetMouseButtonPressed(
      NA_MOUSE_BUTTON_LEFT,
      ([NSEvent pressedMouseButtons] & (1 << 0)) != 0);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_DOWN)) {
      [super mouseDown:event];
    }
  }

  - (void)mouseUp:(NSEvent*)event{
    na_SetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
    na_SetMouseButtonPressed(
      NA_MOUSE_BUTTON_LEFT,
      ([NSEvent pressedMouseButtons] & (1 << 0)) != 0);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_UP)) {
      [super mouseUp:event];
    }
  }
 
  - (void)mouseDragged:(NSEvent*)event{
    na_SetMouseMovedTo(naMakePosWithNSPoint([NSEvent mouseLocation]));
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
      [super mouseDragged:event];
    }
    [NSEvent setMouseCoalescingEnabled:NO];
  }
  
  - (void)rightMouseDown:(NSEvent*)event{
    na_SetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
    na_SetMouseButtonPressed(
      NA_MOUSE_BUTTON_RIGHT,
      ([NSEvent pressedMouseButtons] & (1 << 1)) != 0);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_DOWN)) {
      [super rightMouseDown:event];
    }
  }

  - (void)rightMouseUp:(NSEvent*)event{
    na_SetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
    na_SetMouseButtonPressed(
      NA_MOUSE_BUTTON_RIGHT,
      ([NSEvent pressedMouseButtons] & (1 << 1)) != 0);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_UP)) {
      [super rightMouseUp:event];
    }
  }

  - (void)rightMouseDragged:(NSEvent*)event{
    na_SetMouseMovedTo(naMakePosWithNSPoint([NSEvent mouseLocation]));
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
      [super rightMouseDragged:event];
    }
    [NSEvent setMouseCoalescingEnabled:NO];
  }

  - (void)otherMouseDown:(NSEvent*)event{
    na_SetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
    na_SetMouseButtonPressed(
      NA_MOUSE_BUTTON_MIDDLE,
      ([NSEvent pressedMouseButtons] & (1 << 2)) != 0);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_DOWN)) {
      [super otherMouseDown:event];
    }
  }

  - (void)otherMouseUp:(NSEvent*)event{
    na_SetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
    na_SetMouseButtonPressed(
      NA_MOUSE_BUTTON_MIDDLE,
      ([NSEvent pressedMouseButtons] & (1 << 2)) != 0);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_UP)) {
      [super otherMouseUp:event];
    }
  }

  - (void)otherMouseDragged:(NSEvent*)event{
    na_SetMouseMovedTo(naMakePosWithNSPoint([NSEvent mouseLocation]));
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_MOVED)) {
      [super otherMouseDragged:event];
    }
    [NSEvent setMouseCoalescingEnabled:NO];
  }

  - (void)scrollWheel:(NSEvent*)event{
    na_SetMouseScrolledByDiff([event deltaX], [event deltaY]);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_SCROLLED)) {
      [super scrollWheel:event];
    }
  }

  - (void)keyDown:(NSEvent*)event{
    na_CaptureKeyboardStatus(event);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_KEY_DOWN)) {
      [super keyDown:event];
    }
  }
  
  - (void)keyUp:(NSEvent*)event{
    na_CaptureKeyboardStatus(event);
    if(!na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_KEY_UP)) {
      [super keyUp:event];
    }
  }
  
  - (void)flagsChanged:(NSEvent*)event{
    NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)   ? NA_TRUE : NA_FALSE;
    NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)  ? NA_TRUE : NA_FALSE;
    NABool control = ([event modifierFlags] & NAEventModifierFlagControl) ? NA_TRUE : NA_FALSE;
    NABool command = ([event modifierFlags] & NAEventModifierFlagCommand) ? NA_TRUE : NA_FALSE;
    
    NABool handeled = NA_FALSE;
    handeled |= na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (shift ? NA_UI_COMMAND_KEY_DOWN : NA_UI_COMMAND_KEY_UP));
    handeled |= na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (alt ? NA_UI_COMMAND_KEY_DOWN : NA_UI_COMMAND_KEY_UP));
    handeled |= na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (control ? NA_UI_COMMAND_KEY_DOWN : NA_UI_COMMAND_KEY_UP));
    handeled |= na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (command ? NA_UI_COMMAND_KEY_DOWN : NA_UI_COMMAND_KEY_UP));
    
    if(!handeled) {
      [super flagsChanged:event];
    }
  }
  
  @end



  NA_DEF NAMetalSpace* naNewMetalSpace(NASize size) {
    NACocoaMetalSpace* cocoaMetalSpace = naNew(NACocoaMetalSpace);

    NACocoaNativeMetalSpace* nativePtr = [[NACocoaNativeMetalSpace alloc]
      initWithMetalSpace:cocoaMetalSpace
      frame:naMakeNSRectWithSize(size)];
      
    na_InitMetalSpace((NAMetalSpace*)cocoaMetalSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

    return (NAMetalSpace*)cocoaMetalSpace;
  }



  NA_API NABool naIsMetalTechnologyAvailable(NAMetalSpace* metalSpace) {
    naDefineCocoaObjectConst(NACocoaNativeMetalSpace, nativePtr, metalSpace);
    // Security check for Sierra and High Sierra systems.
    // Damnit Apple, get your shit together!
    return [nativePtr layer] != nil;
  }



  NA_DEF void na_DestructCocoaMetalSpace(NACocoaMetalSpace* cocoaMetalSpace) {
    na_ClearMetalSpace((NAMetalSpace*)cocoaMetalSpace);
  }


  NA_DEF void* naGetMetalSpaceSystemContext(const NAMetalSpace* metalSpace) {
    naDefineCocoaObjectConst(NACocoaNativeMetalSpace, nativePtr, metalSpace);
    return (NA_COCOA_BRIDGE void*)[nativePtr layer];
  }


  NA_DEF void naSetMetalSpaceVisible(NAMetalSpace* metalSpace, NABool visible) {
    naDefineCocoaObject(NACocoaNativeMetalSpace, nativePtr, metalSpace);
    [nativePtr setHidden:visible ? NO : YES];
  }



  NA_DEF void naSetMetalSpaceInnerRect(NAMetalSpace* metalSpace, NARect bounds) {
    naDefineCocoaObject(NACocoaNativeMetalSpace, nativePtr, metalSpace);
    NSRect frame = naMakeNSRectWithRect(bounds);
    frame.origin = NSMakePoint(0, 0);
    [nativePtr setFrame: frame];
  }



NA_HDEF NARect na_GetMetalSpaceRect(const NA_UIElement* metalSpace) {
  naDefineCocoaObjectConst(NACocoaNativeMetalSpace, nativePtr, metalSpace);
  return naMakeRectWithNSRect([nativePtr frame]);
}

NA_HDEF void na_SetMetalSpaceRect(NA_UIElement* metalSpace, NARect rect) {
  naDefineCocoaObject(NACocoaNativeMetalSpace, nativePtr, metalSpace);
  [nativePtr setFrame:naMakeNSRectWithRect(rect)];
}



#else

  NA_DEF NAMetalSpace* naNewMetalSpace(NASize size) {
    NA_UNUSED(size);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
    return NA_NULL;
  }

  NA_API NABool naIsMetalTechnologyAvailable(NAMetalSpace* metalSpace) {
    return NA_FALSE;
  }

  NA_DEF void na_DestructCocoaMetalSpace(NACocoaMetalSpace* cocoaMetalSpace) {
    NA_UNUSED(cocoaMetalSpace);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
  }

  NA_DEF void* naGetMetalSpaceSystemContext(const NAMetalSpace* metalSpace) {
    NA_UNUSED(metalSpace);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
    return NA_NULL;
  }

  NA_DEF void naSetMetalSpaceInnerRect(NAMetalSpace* metalSpace, NARect bounds) {
    NA_UNUSED(metalSpace);
    NA_UNUSED(bounds);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
  }

  NA_HDEF NARect na_GetMetalSpaceRect(const NA_UIElement* metalSpace) {
    NA_UNUSED(metalSpace);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
    return naMakeRectS(0., 0., 1., 1.);
  }

  NA_HDEF void na_SetMetalSpaceRect(NA_UIElement* metalSpace, NARect rect) {
    NA_UNUSED(metalSpace);
    NA_UNUSED(rect);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
  }

#endif  // NA_COMPILE_METAL && defined __MAC_10_12



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
