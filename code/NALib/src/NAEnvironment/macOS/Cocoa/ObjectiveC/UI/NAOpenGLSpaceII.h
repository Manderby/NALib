
// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#if (NA_COMPILE_OPENGL == 1)

  #pragma GCC diagnostic push 
  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

  @implementation NACocoaNativeOpenGLSpace
  
  - (id)initWithOpenGLSpace:(NAOpenGLSpace*)newOpenGLSpace frame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)pixelformat initFunc:(NAMutator)newinitFunc initData:(void*)newinitData{
    self = [super initWithFrame:frameRect pixelFormat:pixelformat];

    // todo: make this dependent on whether tracking is needed or not.
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
        options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow
        owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];

    openGLSpace = newOpenGLSpace;
    initFunc = newinitFunc;
    initData = newinitData;
    return self;
  }
  
  - (BOOL)acceptsFirstResponder{
    return YES; // This is required to get keyboard input.
  }
  
  - (void)prepareOpenGL{
    // When entering this function, the opengl context is set.
    [super prepareOpenGL];
    // Make sure OpenGL always swaps the buffers of the default framebuffer. If
    // this is not done, sometimes, the double buffer will not work properly.
    GLint swapInt = 1;
    
    [[self openGLContext] setValues:&swapInt forParameter:NAOpenGLContextParameterSwapInterval];

    // Now the OpenGL context is created and current. We can initialize it
    // if necessary.
    if(initFunc){
      initFunc(initData);
    }
  }
  
  - (void)drawRect:(NSRect)dirtyRect{
    NA_UNUSED(dirtyRect);
    [[self openGLContext] makeCurrentContext];
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, NA_UI_COMMAND_REDRAW);
  }
  
  - (void)reshape{
    [super reshape];
    [[self openGLContext] update];
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, NA_UI_COMMAND_RESHAPE);
  }
  
  - (void)mouseMoved:(NSEvent*)event{
    na_SetMouseMovedByDiff([event deltaX], -[event deltaY]);
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, NA_UI_COMMAND_MOUSE_MOVED);
    //[NSEvent setMouseCoalescingEnabled:NO];
  }
  
  - (void)keyDown:(NSEvent*)event{
    NA_UNUSED(event);
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, NA_UI_COMMAND_KEYDOWN);
  }
  
  - (void)keyUp:(NSEvent*)event{
    NA_UNUSED(event);
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, NA_UI_COMMAND_KEYUP);
  }
  
  - (void)flagsChanged:(NSEvent*)event{
    NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)    ?NA_TRUE:NA_FALSE;
    NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)   ?NA_TRUE:NA_FALSE;
    NABool control = ([event modifierFlags] & NAEventModifierFlagControl)  ?NA_TRUE:NA_FALSE;
    NABool command = ([event modifierFlags] & NAEventModifierFlagCommand)  ?NA_TRUE:NA_FALSE;

    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, (shift ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, (alt ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, (control ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
    na_DispatchUIElementCommand((NA_UIElement*)openGLSpace, (command ? NA_UI_COMMAND_KEYDOWN : NA_UI_COMMAND_KEYUP));
  }
  
  @end



  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NASize size, NAMutator initFunc, void* initData){
    NAOpenGLSpace* openGLSpace = naAlloc(NAOpenGLSpace);

    // Configure the OpenGL context and initialize this object.
    NSOpenGLPixelFormatAttribute attr[] = {
      NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAColorSize, 24,
      NSOpenGLPFADepthSize, 64,
      NSOpenGLPFAAllowOfflineRenderers, // lets OpenGL know this context is offline renderer aware
      0 };
    NSOpenGLPixelFormat *pixelformat = NA_COCOA_AUTORELEASE([[NSOpenGLPixelFormat alloc] initWithAttributes:attr]);

    NACocoaNativeOpenGLSpace* nativePtr = [[NACocoaNativeOpenGLSpace alloc]
      initWithOpenGLSpace:openGLSpace
      frame:naMakeNSRectWithSize(size)
      pixelFormat:pixelformat
      initFunc:initFunc
      initData:initData];
    na_InitOpenGLSpace(openGLSpace, NA_COCOA_PTR_OBJC_TO_C(nativePtr));

    if([nativePtr respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]){
      NA_MACOS_AVAILABILITY_GUARD_10_7(
        [nativePtr setWantsBestResolutionOpenGLSurface:YES];
      )
    }
    
    return openGLSpace;
  }



  NA_DEF void na_DestructOpenGLSpace(NAOpenGLSpace* openGLSpace){
    na_ClearOpenGLSpace(openGLSpace);
  }



  NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openGLSpace){
    [[(NA_COCOA_BRIDGE NACocoaNativeOpenGLSpace*)(openGLSpace->uiElement.nativePtr) openGLContext] flushBuffer];
  }



  NA_DEF void naSetOpenGLInnerRect(NAOpenGLSpace* openGLSpace, NARect bounds){
    naDefineCocoaObject(NACocoaNativeOpenGLSpace, nativePtr, openGLSpace);
    NSRect frame = naMakeNSRectWithRect(bounds);
    frame.origin = NSMakePoint(0, 0);
    [nativePtr setFrame: frame];
  }
  
  #pragma GCC diagnostic pop

#else

  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NASize size, NAMutator initFunc, void* initData){
    naError("OpenGL has not been configured. See NAConfiguration.h");
    return NA_NULL;
  }

  NA_DEF void na_DestructOpenGLSpace(NAOpenGLSpace* openGLSpace){
    naError("OpenGL has not been configured. See NAConfiguration.h");
  }

  NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openGLSpace){
    naError("OpenGL has not been configured. See NAConfiguration.h");
  }

  NA_DEF void naSetOpenGLInnerRect(NAOpenGLSpace* openGLSpace, NARect bounds){
    naError("OpenGL has not been configured. See NAConfiguration.h");
  }

#endif  // NA_COMPILE_OPENGL



NA_HDEF NARect na_GetOpenGLSpaceAbsoluteInnerRect(NA_UIElement* openGLSpace){
  return na_GetSpaceAbsoluteInnerRect(openGLSpace);
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
