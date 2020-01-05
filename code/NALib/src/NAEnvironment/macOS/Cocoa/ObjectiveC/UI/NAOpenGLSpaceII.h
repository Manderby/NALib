
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#if (NA_CONFIG_COMPILE_OPENGL == 1)

  @implementation NACocoaOpenGLSpace
  - (id)initWithCoreOpenGLSpace:(NAOpenGLSpace*)newcoreopenglspace frame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)pixelformat initFunc:(NAMutator)newinitFunc initData:(void*)newinitData{
    self = [super initWithFrame:frameRect pixelFormat:pixelformat];
    coreopenglspace = newcoreopenglspace;
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
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_REDRAW);
  }
  - (void)reshape{
    [super reshape];
    [[self openGLContext] update];
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_RESHAPE);
  }
  - (void)keyDown:(NSEvent*)event{
    NA_UNUSED(event);
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_KEYDOWN);
  }
  - (void)keyUp:(NSEvent*)event{
NA_UNUSED(event);
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_KEYUP);
  }
  - (void)flagsChanged:(NSEvent*)event{
    NAUIKeyCode keyCode;
    NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)    ?NA_TRUE:NA_FALSE;
    NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)   ?NA_TRUE:NA_FALSE;
    NABool control = ([event modifierFlags] & NAEventModifierFlagControl)  ?NA_TRUE:NA_FALSE;
    NABool command = ([event modifierFlags] & NAEventModifierFlagCommand)  ?NA_TRUE:NA_FALSE;

//    [event modifierFlags]; NSEventModifierFlagCapsLock;
//    let isLeftShift = event.modifierFlags.rawValue & UInt(NX_DEVICELSHIFTKEYMASK) != 0
//    let isRightShift = event.modifierFlags.rawValue & UInt(NX_DEVICERSHIFTKEYMASK) != 0

    keyCode = NA_KEYCODE_LEFT_SHIFT;
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (shift?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP));
    keyCode = NA_KEYCODE_LEFT_OPTION;
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (alt?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP));
    keyCode = NA_KEYCODE_CONTROL;
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (control?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP));
    keyCode = NA_KEYCODE_LEFT_COMMAND;
    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP));
  }
  @end



  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
    NA_UNUSED(window);
    NACoreOpenGLSpace* coreopenglspace = naAlloc(NACoreOpenGLSpace);

    // Configure the OpenGL Context and initialize this object.
    NSOpenGLPixelFormatAttribute attr[] = {
      NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAColorSize, 24,
  //    NSOpenGLPFAAlphaSize, 8,
      NSOpenGLPFADepthSize, 64,
      NSOpenGLPFAAllowOfflineRenderers, // lets OpenGL know this context is offline renderer aware
      0 };
    NSOpenGLPixelFormat *pixelformat = NA_COCOA_AUTORELEASE([[NSOpenGLPixelFormat alloc] initWithAttributes:attr]);
    
    NSRect frameRect = NSMakeRect(0.f, 0.f, (CGFloat)size.width, (CGFloat)size.height);
    NACocoaOpenGLSpace* cocoaSpace = [[NACocoaOpenGLSpace alloc] initWithCoreOpenGLSpace:coreopenglspace frame:frameRect pixelFormat:pixelformat initFunc:initfunc initData:initdata];

    if([self respondsToSelector:@selector(setWantsBestResolutionOpenGLSurface:)]){
      NA_MACOS_AVAILABILITY_GUARD_10_7(
        [self setWantsBestResolutionOpenGLSurface:YES];
      )
    }

    naInitCoreOpenGLSpace(coreopenglspace, NA_COCOA_PTR_OBJC_TO_C(cocoaSpace));
    return coreopenglspace;
  }



  NA_DEF void naDestructOpenGLSpace(NAOpenGLSpace* openglspace){
    NACoreOpenGLSpace* coreopenglspace = (NACoreOpenGLSpace*)openglspace;
    naClearCoreOpenGLSpace(coreopenglspace);
  }



  NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace){
    NACoreOpenGLSpace* coreopenglspace = (NACoreOpenGLSpace*)openglspace;
    [[(NACocoaOpenGLSpace*)NA_COCOA_PTR_C_TO_OBJC(coreopenglspace->uielement.nativeID) openGLContext] flushBuffer];
  }



  NA_DEF void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds){
    NA_UNUSED(openglspace);
    NA_UNUSED(bounds);
  }
  
#endif  // NA_CONFIG_COMPILE_OPENGL

NA_HDEF NARect naGetOpenGLSpaceAbsoluteInnerRect(NACoreUIElement* openglspace){
  return naGetSpaceAbsoluteInnerRect(openglspace);
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
