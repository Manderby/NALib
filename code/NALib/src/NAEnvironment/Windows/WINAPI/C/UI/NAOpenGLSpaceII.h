
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#if (NA_COMPILE_OPENGL == 1)

#include "../../../../../NADateTime.h"

typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;
struct NAWINAPIOpenGLSpace {
  NAOpenGLSpace openGLSpace;
  HGLRC         hRC;    // The rendering context for OpenGL
};

NA_HAPI void na_DestructWINAPIOpenGLSpace(NAWINAPIOpenGLSpace* winapiOpenGLSpace);
NA_RUNTIME_TYPE(NAWINAPIOpenGLSpace, na_DestructWINAPIOpenGLSpace, NA_FALSE);



NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_PAINT:

    info.hasBeenHandeled = na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_REDRAW);
    RECT updateRegion;
    GetUpdateRect(naGetUIElementNativePtr(uiElement), &updateRegion, NA_FALSE);
    ValidateRect(naGetUIElementNativePtr(uiElement), &updateRegion);

    info.hasBeenHandeled = NA_TRUE;
    info.result = 0;
    break;

  default:
    //printf("Uncaught OpenGL Space message" NA_NL);
    break;
  }
  
  return info;
}


typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);

NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NASize size, NAMutator initFunc, void* initData){
	
  HWND hWnd;
  HDC hDC;
 	PIXELFORMATDESCRIPTOR pfd;
  int format;
  DWORD style;
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
  const char *extensions;

  NAWINAPIOpenGLSpace* winapiOpenGLSpace = naNew(NAWINAPIOpenGLSpace);

  style = WS_CHILD | WS_VISIBLE | ES_READONLY;
  
	hWnd = CreateWindow(
		TEXT("NAOpenGLSpace"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativePtr(naGetApplication()), NULL );
    
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
	winapiOpenGLSpace->hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, winapiOpenGLSpace->hRC);

	extensions = (char*)glGetString(GL_EXTENSIONS);
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT){wglSwapIntervalEXT(1);}

  na_InitOpenGLSpace(&(winapiOpenGLSpace->openGLSpace), hWnd);

  // Now the OpenGL context is created and current. We can initialize it
  // if necessary.
  if(initFunc){
    initFunc(initData);
  }

	//glewInit();
  return (NAOpenGLSpace*)winapiOpenGLSpace;
}


NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openGLSpace){
  NAWINAPIOpenGLSpace* winapiOpenGLSpace = (NAWINAPIOpenGLSpace*)openGLSpace;
  SwapBuffers(GetDC((HWND)naGetUIElementNativePtr(&(winapiOpenGLSpace->openGLSpace.uiElement))));
}



NA_API void naSetOpenGLInnerRect(NAOpenGLSpace* openGLSpace, NARect bounds){
  SetWindowPos((HWND)naGetUIElementNativePtr(openGLSpace), HWND_TOP, 0, 0, (int)bounds.size.width, (int)bounds.size.height, SWP_NOREDRAW);
}




NA_DEF void na_DestructWINAPIOpenGLSpace(NAWINAPIOpenGLSpace* winapiOpenGLSpace){
  na_ClearOpenGLSpace((NAOpenGLSpace*)winapiOpenGLSpace);
}



#else

  NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
    naError("OpenGL has not been configured. See NAConfiguration.h");
    NAWINAPICallbackInfo info = {NA_FALSE, 0};
    return info;
  }

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

#endif  // NA_COMPILE_OPENGL == 1

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
