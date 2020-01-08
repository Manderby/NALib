
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
  HGLRC hRC;    // The rendering context for OpenGL
};



NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_PAINT:
    info.hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW);
    info.result = 0;
    break;

  default:
    //printf("Uncaught OpenGL Space message\n");
    break;
  }
  
  return info;
}


typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);

NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
	
  HWND hWnd;
  HDC hDC;
 	PIXELFORMATDESCRIPTOR pfd;
  int format;
  DWORD style;
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
  const char *extensions;

  NAWINAPIOpenGLSpace* winapiopenglspace = naAlloc(NAWINAPIOpenGLSpace);

  style = WS_CHILD | WS_VISIBLE | ES_READONLY;

	hWnd = CreateWindow(
		TEXT("NASpace"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		(HWND)naGetUIElementNativeID(window), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
    
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
	winapiopenglspace->hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, winapiopenglspace->hRC);

	extensions = (char*)glGetString(GL_EXTENSIONS);
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT){wglSwapIntervalEXT(1);}

  naInitCoreOpenGLSpace(&(winapiopenglspace->coreopenglspace), hWnd);

  // Now the OpenGL context is created and current. We can initialize it
  // if necessary.
  if(initfunc){
    initfunc(initdata);
  }

	//glewInit();
  return (NAOpenGLSpace*)winapiopenglspace;
}


NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace){
  NAWINAPIOpenGLSpace* winapiopenglspace = (NAWINAPIOpenGLSpace*)openglspace;
  SwapBuffers(GetDC((HWND)naGetUIElementNativeID(&(winapiopenglspace->coreopenglspace.uielement))));
}



NA_API void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds){
  SetWindowPos((HWND)naGetUIElementNativeID(openglspace), HWND_TOP, 0, 0, (int)bounds.size.width, (int)bounds.size.height, SWP_NOREDRAW);
}




NA_DEF void naDestructOpenGLSpace(NAOpenGLSpace* openglspace){
  NACoreOpenGLSpace* coreopenglspace = (NACoreOpenGLSpace*)openglspace;
  naClearCoreOpenGLSpace(coreopenglspace);
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
