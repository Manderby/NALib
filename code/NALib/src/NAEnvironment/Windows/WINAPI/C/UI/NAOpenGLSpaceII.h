
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#if (NA_CONFIG_COMPILE_OPENGL == 1)


typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;
struct NAWINAPIOpenGLSpace {
  NACoreOpenGLSpace coreopenglspace;
};



NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NABool hasbeenhandeled = NA_FALSE;

  switch(message){
  default:
    //printf("Uncaught OpenGL Space message\n");
    break;
  }
  
  return hasbeenhandeled;
}


//  @implementation NACocoaOpenGLSpace
//  - (id)initWithCoreOpenGLSpace:(NAOpenGLSpace*)newcoreopenglspace frame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)pixelformat initFunc:(NAMutator)newinitFunc initData:(void*)newinitData{
//    self = [super initWithFrame:frameRect pixelFormat:pixelformat];
//    coreopenglspace = newcoreopenglspace;
//    initFunc = newinitFunc;
//    initData = newinitData;
//    return self;
//  }
//  - (BOOL)acceptsFirstResponder{
//    return YES; // This is required to get keyboard input.
//  }
//  - (void)prepareOpenGL{
//    // When entering this function, the opengl context is set.
//    [super prepareOpenGL];
//    // Make sure OpenGL always swaps the buffers of the default framebuffer. If
//    // this is not done, sometimes, the double buffer will not work properly.
//    GLint swapInt = 1;
//    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
//
//    // Now the OpenGL context is created and current. We can initialize it
//    // if necessary.
//    if(initFunc){
//      initFunc(initData);
//    }
//  }
//  - (void)drawRect:(NSRect)dirtyRect{
//    [[self openGLContext] makeCurrentContext];
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_REDRAW, NA_NULL);
//  }
//  - (void)reshape{
//    [super reshape];
//    [[self openGLContext] update];
//    NARect bounds = naMakeRectWithNSRect([self bounds]);
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_RESHAPE, NA_NULL);
//  }
//  - (void)keyDown:(NSEvent*)event{
//    NAUIKeyCode keyCode = [event keyCode];
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_KEYDOWN, &keyCode);
//  }
//  - (void)keyUp:(NSEvent*)event{
//    NAUIKeyCode keyCode = [event keyCode];
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, NA_UI_COMMAND_KEYUP, &keyCode);
//  }
//  - (void)flagsChanged:(NSEvent*)event{
//    NAUIKeyCode keyCode;
//    NABool shift   = ([event modifierFlags] & NAEventModifierFlagShift)    ?NA_TRUE:NA_FALSE;
//    NABool alt     = ([event modifierFlags] & NAEventModifierFlagOption)   ?NA_TRUE:NA_FALSE;
//    NABool control = ([event modifierFlags] & NAEventModifierFlagControl)  ?NA_TRUE:NA_FALSE;
//    NABool command = ([event modifierFlags] & NAEventModifierFlagCommand)  ?NA_TRUE:NA_FALSE;
//
////    [event modifierFlags]; NSEventModifierFlagCapsLock;
////    let isLeftShift = event.modifierFlags.rawValue & UInt(NX_DEVICELSHIFTKEYMASK) != 0
////    let isRightShift = event.modifierFlags.rawValue & UInt(NX_DEVICERSHIFTKEYMASK) != 0
//
//    keyCode = NA_KEYCODE_SHIFT;
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (shift?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//    keyCode = NA_KEYCODE_OPTION;
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (alt?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//    keyCode = NA_KEYCODE_CONTROL;
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (control?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//    keyCode = NA_KEYCODE_LEFT_COMMAND;
//    naDispatchUIElementCommand((NACoreUIElement*)coreopenglspace, (command?NA_UI_COMMAND_KEYDOWN:NA_UI_COMMAND_KEYUP), &keyCode);
//  }
//  @end



  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
	
    HWND hWnd;
    HDC hDC;
 	  PIXELFORMATDESCRIPTOR pfd;
    int format;
    NAWINAPIOpenGLSpace* openglspace;
    DWORD style;

    style = WS_CHILD | WS_VISIBLE | ES_READONLY;

	  hWnd = CreateWindow(
		  TEXT("NAOpenGLSpace"), "OpenGL Space", style,
		  0, 0, (int)size.width, (int)size.height,
		  (HWND)naGetUIElementNativeID(window), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );

    openglspace = naAlloc(NAWINAPIOpenGLSpace);
    
    // Bugfix: Probably remove window as parent
    naRegisterCoreUIElement((NACoreUIElement*)openglspace, (NACoreUIElement*)window, NA_UI_OPENGLSPACE, hWnd);

    hDC = GetDC(hWnd);

    // Expected to be called when initializing. Do not multithread!
	  // define pixel format for device context
	  ZeroMemory( &pfd, sizeof( pfd ) );
	  pfd.nSize = sizeof( pfd );
	  pfd.nVersion = 1;
	  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	  pfd.iPixelType = PFD_TYPE_RGBA;
	  pfd.cAlphaBits = 8;
	  pfd.cColorBits = 24;
	  pfd.cDepthBits = 16;
	  pfd.iLayerType = PFD_MAIN_PLANE;
	  format = ChoosePixelFormat( GetDC(hWnd), &pfd );

	  SetPixelFormat( hDC, format, &pfd );
	
	  // make render context with this device context.
	  openglspace->hRC = wglCreateContext(hDC);
	  wglMakeCurrent(hDC, openglspace->hRC);

	  typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	  PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	  const char *extensions = (char*)glGetString(GL_EXTENSIONS);
	  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	  if (wglSwapIntervalEXT){wglSwapIntervalEXT(1);}


    // Now the OpenGL context is created and current. We can initialize it
    // if necessary.
    if(initfunc){
      initfunc(initdata);
    }

	  //glewInit();
    return openglspace;
  }


  NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace){
    NAWINAPIOpenGLSpace* winapiopenglspace = (NAWINAPIOpenGLSpace*)openglspace;
    SwapBuffers(GetDC((HWND)naGetUIElementNativeID(&(winapiopenglspace->coreopenglspace.uielement))));
  }



  NA_API void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds){
    //NARect windowrect = naGetUIElementRect(naGetUIElementParent(openglspace), naGetApplication(), NA_FALSE);
    SetWindowPos((HWND)naGetUIElementNativeID(openglspace), HWND_TOP, 0, 0, (int)bounds.size.width, (int)bounds.size.height, SWP_NOREDRAW);
  }




  NA_DEF void naDestructOpenGLSpace(NAOpenGLSpace* openglspace){
//    NACoreOpenGLSpace* coreopenglspace = (NACoreOpenGLSpace*)openglspace;
//    naClearCoreOpenGLSpace(&(coreopenglspace->coreopenglspace));
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
