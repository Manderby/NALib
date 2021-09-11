
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NACocoaMetalSpace NACocoaMetalSpace;
struct NACocoaMetalSpace{
  NAMetalSpace metalSpace;
};

NA_HAPI void na_DestructCocoaMetalSpace(NACocoaMetalSpace* cocoaMetalSpace);
NA_RUNTIME_TYPE(NACocoaMetalSpace, na_DestructCocoaMetalSpace, NA_FALSE);

#if (NA_COMPILE_METAL == 1)
#ifdef __MAC_10_12  // Metal is only available since macOS 10.11, CALayerDelegate since 10.12

  #import <QuartzCore/CAMetalLayer.h>
  #include <Metal/Metal.h>

  @interface NACocoaNativeMetalSpace : NSView<CALayerDelegate>{
    NACocoaMetalSpace* cocoaMetalSpace;
    NSTrackingArea*     trackingArea;
    NAMutator           initFunc;
    void*               initData;
  }
  @end



  @implementation NACocoaNativeMetalSpace
  
  - (id)initWithMetalSpace:(NACocoaMetalSpace*)newCocoaMetalSpace frame:(NSRect)frameRect initFunc:(NAMutator)newinitFunc initData:(void*)newinitData{
    self = [super initWithFrame:frameRect];

    if (@available(macOS 10.11, *)) {
      CAMetalLayer* metalLayer = [[CAMetalLayer alloc] init];
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
      
      //    context = karoAllocDrawContext(
      //      KARO_DRAW_TARGET_METAL,
      //      NA_COCOA_PTR_OBJC_TO_C(metalLayer),
      //      newRaster,
      //      NA_FALSE);
      //      
      //    karoSetDrawContextScaleFactor(context, [KaroNSApplication getUIScaleFactorForWindow:[self window]]);
      
      [self setLayer:metalLayer];
      [self setLayerContentsRedrawPolicy:NSViewLayerContentsRedrawOnSetNeedsDisplay];
      
    }

    // todo: make this dependent on whether tracking is needed or not.
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
        options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow
        owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];

    cocoaMetalSpace = newCocoaMetalSpace;
    initFunc = newinitFunc;
    initData = newinitData;
    return self;
  }
  
  - (void)dealloc{
//    NA_COCOA_RELEASE(trackingArea);
    NA_COCOA_SUPER_DEALLOC();
  }
  
  - (BOOL)acceptsFirstResponder{
    return YES; // This is required to get keyboard input.
  }
  
//  - (void)prepareMetal{
//    // When entering this function, the opengl context is set.
//    [super prepareMetal];
//    // Make sure Metal always swaps the buffers of the default frameBuffer. If
//    // this is not done, sometimes, the double buffer will not work properly.
//    GLint swapInt = 1;
//    
//    [[self metalContext] setValues:&swapInt forParameter:NAMetalContextParameterSwapInterval];
//
//    // Now the Metal context is created and current. We can initialize it
//    // if necessary.
//    if(initFunc){
//      initFunc(initData);
//    }
//  }
  
- (void) displayLayer:(CALayer *)layer {
    NA_UNUSED(layer);
//    [[self metalContext] makeCurrentContext];
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_REDRAW);
  }
//  - (void)drawRect:(NSRect)dirtyRect{
//    NA_UNUSED(dirtyRect);
////    [[self metalContext] makeCurrentContext];
////    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_REDRAW);
//  }
  
  - (void)reshape{
    [super reshape];
//    [[self metalContext] update];
//    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_RESHAPE);
  }
  
  - (void)mouseMoved:(NSEvent*)event{
    na_SetMouseMovedByDiff([event deltaX], -[event deltaY]);
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_MOUSE_MOVED);
    //[NSEvent setMouseCoalescingEnabled:NO];
  }
  
  - (void)keyDown:(NSEvent*)event{
    NA_UNUSED(event);
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_KEYDOWN);
  }
  
  - (void)keyUp:(NSEvent*)event{
    NA_UNUSED(event);
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, NA_UI_COMMAND_KEYUP);
  }
  
  - (void)flagsChanged:(NSEvent*)event{
    NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)    ?NA_TRUE:NA_FALSE;
    NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)   ?NA_TRUE:NA_FALSE;
    NABool control = ([event modifierFlags] & NAEventModifierFlagControl)  ?NA_TRUE:NA_FALSE;
    NABool command = ([event modifierFlags] & NAEventModifierFlagCommand)  ?NA_TRUE:NA_FALSE;

    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (shift ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (alt ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (control ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
    na_DispatchUIElementCommand((NA_UIElement*)cocoaMetalSpace, (command ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
  }
  
  @end



  NA_DEF NAMetalSpace* naNewMetalSpace(NASize size, NAMutator initFunc, void* initData){
    NACocoaMetalSpace* cocoaMetalSpace = naNew(NACocoaMetalSpace);

    NACocoaNativeMetalSpace* nativePtr = [[NACocoaNativeMetalSpace alloc]
      initWithMetalSpace:cocoaMetalSpace
      frame:naMakeNSRectWithSize(size)
      initFunc:initFunc
      initData:initData];
    na_InitMetalSpace((NAMetalSpace*)cocoaMetalSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

//    NA_COCOA_RELEASE(pixelFormat);
//
//    if([nativePtr respondsToSelector:@selector(setWantsBestResolutionMetalSurface:)]){
//      NA_MACOS_AVAILABILITY_GUARD_10_7(
//        [nativePtr setWantsBestResolutionMetalSurface:YES];
//      )
//    }
    
    return (NAMetalSpace*)cocoaMetalSpace;
  }



  NA_DEF void na_DestructCocoaMetalSpace(NACocoaMetalSpace* cocoaMetalSpace){
    na_ClearMetalSpace((NAMetalSpace*)cocoaMetalSpace);
  }


  NA_DEF void* naGetMetalSystemContext(NAMetalSpace* metalSpace){
    naDefineCocoaObject(NACocoaNativeMetalSpace, nativePtr, metalSpace);
//    return [nativePtr metalContext];
    return [nativePtr layer];
    return NA_NULL;
  }


  NA_DEF void naSwapMetalBuffer(NAMetalSpace* metalSpace){
//    [[(NA_COCOA_BRIDGE NACocoaNativeMetalSpace*)(metalSpace->uiElement.nativePtr) metalContext] flushBuffer];
  }



  NA_DEF void naSetMetalInnerRect(NAMetalSpace* metalSpace, NARect bounds){
    naDefineCocoaObject(NACocoaNativeMetalSpace, nativePtr, metalSpace);
    NSRect frame = naMakeNSRectWithRect(bounds);
    frame.origin = NSMakePoint(0, 0);
    [nativePtr setFrame: frame];
  }

#else

  NA_DEF NAMetalSpace* naNewMetalSpace(NASize size, NAMutator initFunc, void* initData){
    NA_UNUSED(size);
    NA_UNUSED(initFunc);
    NA_UNUSED(initData);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
    return NA_NULL;
  }

  NA_DEF void na_DestructCocoaMetalSpace(NACocoaMetalSpace* cocoaMetalSpace){
    NA_UNUSED(cocoaMetalSpace);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
  }

  NA_DEF void* naGetMetalSystemContext(NAMetalSpace* metalSpace){
    NA_UNUSED(metalSpace);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
    return NA_NULL;
  }

  NA_DEF void naSwapMetalBuffer(NAMetalSpace* metalSpace){
    NA_UNUSED(metalSpace);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
  }

  NA_DEF void naSetMetalInnerRect(NAMetalSpace* metalSpace, NARect bounds){
    NA_UNUSED(metalSpace);
    NA_UNUSED(bounds);
    #if NA_DEBUG
      naError("Metal has not been configured. See NAConfiguration.h");
    #endif
  }

#endif // __MAC_10_12
#endif  // NA_COMPILE_METAL



NA_HDEF NARect na_GetMetalSpaceAbsoluteInnerRect(NA_UIElement* metalSpace){
  return na_GetSpaceAbsoluteInnerRect(metalSpace);
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
