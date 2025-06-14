
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#if NA_COMPILE_OPENGL == 1

#include "../../../NAUtility/NADateTime.h"

static NAList* redrawList;
static int redrawCount = 0;
void na_drawAllOpenGLSpaces(void* data) {
  NA_UNUSED(data);
  printf("Redraw: %d\n", redrawCount);
  redrawCount = 0;
}



NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NA_UNUSED(lParam);
  NAWINAPIOpenGLSpace* winapiOpenGLSpace;
  int wheelDelta;

  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {
  case WM_NCHITTEST:
  case WM_SHOWWINDOW:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SETCURSOR:
  case WM_MOUSEFIRST:
  case WM_UPDATEUISTATE:
  case WM_MOUSEACTIVATE:
    break;

  case WM_LBUTTONDOWN:
    break;

  case WM_LBUTTONUP:
  case WM_NCPAINT:
  case WM_NCCALCSIZE:
  break;

  case WM_ERASEBKGND: // wParam: Device context, return != 0 if erasing, 0 otherwise
    // not called.
    //info.result = 0;
    //info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_MOUSEWHEEL:

    winapiOpenGLSpace = (NAWINAPIOpenGLSpace*)uiElement;
    wheelDelta = (int)(-8. * GET_WHEEL_DELTA_WPARAM(wParam) / (double)WHEEL_DELTA);
    naSetOpenGLSpaceTranslation(&winapiOpenGLSpace->openGLSpace, 0, wheelDelta);
    if(!na_DispatchUIElementCommand((NA_UIElement*)winapiOpenGLSpace, NA_UI_COMMAND_TRANSFORMED)) {
      // no parent method to be called.
    }
    naResetOpenGLSpaceTransformation(&winapiOpenGLSpace->openGLSpace);
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_PAINT:

    winapiOpenGLSpace = (NAWINAPIOpenGLSpace*)uiElement;

    // Activating the redraw list is using a timer to update all openGL spaces
    // at once.
    naAddOpenGLSpaceToRedrawList(winapiOpenGLSpace);

    RECT updateRegion;
    GetClientRect(naGetUIElementNativePtr(uiElement), &updateRegion);
    ValidateRect(naGetUIElementNativePtr(uiElement), &updateRegion);
  
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  default:
    //printf("Uncaught OpenGL Space message" NA_NL);
    break;
  }
  
  return info;
}


typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);

NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NASize size, NAMutator initFunc, void* initData) {
  NAWINAPIOpenGLSpace* winapiOpenGLSpace = naNew(NAWINAPIOpenGLSpace);

  winapiOpenGLSpace->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementUIScale(NA_NULL);

	HWND nativePtr = CreateWindow(
		TEXT("NAOpenGLSpace"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | ES_READONLY,
    0,
    0,
    (LONG)(winapiOpenGLSpace->rect.size.width * uiScale),
    (LONG)(winapiOpenGLSpace->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()), 
    NULL);
    
  HDC hDC = GetDC(nativePtr);

  // Expected to be called when initializing. Do not multithread!
	// define pixel format for device context
 	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cAlphaBits = 8;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat( GetDC(nativePtr), &pfd );

	SetPixelFormat( hDC, format, &pfd );
	
	// make render context with this device context.
	winapiOpenGLSpace->hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, winapiOpenGLSpace->hRC);

  //glfwWindowHint(GLFW_SAMPLES, 4);

	//const char* extensions = (char*)glGetString(GL_EXTENSIONS);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(wglSwapIntervalEXT) {
    wglSwapIntervalEXT(1);
  }

  na_InitOpenGLSpace(&winapiOpenGLSpace->openGLSpace, nativePtr);

  // Now the OpenGL context is created and current. We can initialize it
  // if necessary.
  if(initFunc) {
    initFunc(initData);
  }

	//glewInit();
  return (NAOpenGLSpace*)winapiOpenGLSpace;
}



NA_DEF void na_DestructWINAPIOpenGLSpace(NAWINAPIOpenGLSpace* winapiOpenGLSpace) {
  na_ClearOpenGLSpace((NAOpenGLSpace*)winapiOpenGLSpace);
}



NA_DEF void naSetOpenGLSpaceVisible(NAOpenGLSpace* openGLSpace, NABool visible) {
  ShowWindow(naGetUIElementNativePtr(openGLSpace), visible ? SW_SHOW : SW_HIDE);
}



NA_DEF void naSwapOpenGLSpaceBuffer(NAOpenGLSpace* openGLSpace) {
  NAWINAPIOpenGLSpace* winapiOpenGLSpace = (NAWINAPIOpenGLSpace*)openGLSpace;
  SwapBuffers(GetDC((HWND)naGetUIElementNativePtr(&winapiOpenGLSpace->openGLSpace.uiElement)));
}



NA_API NARect na_GetOpenGLSpaceRect(const NA_UIElement* openGLSpace)
{
  const NAWINAPIOpenGLSpace* winapiOpenGLSpace = (const NAWINAPIOpenGLSpace*)openGLSpace;
  return winapiOpenGLSpace->rect;
}

NA_API void na_SetOpenGLSpaceRect(NA_UIElement* openGLSpace, NARect rect) {
  NAWINAPIOpenGLSpace* winapiOpenGLSpace = (NAWINAPIOpenGLSpace*)openGLSpace;

  winapiOpenGLSpace->rect = rect;
  double uiScale = naGetUIElementUIScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(openGLSpace));

  SetWindowPos(
    naGetUIElementNativePtr(openGLSpace),
    HWND_TOP,
    (LONG)(winapiOpenGLSpace->rect.pos.x * uiScale),
    (LONG)((parentRect.size.height - winapiOpenGLSpace->rect.pos.y - winapiOpenGLSpace->rect.size.height) * uiScale),
    (LONG)(winapiOpenGLSpace->rect.size.width * uiScale),
    (LONG)(winapiOpenGLSpace->rect.size.height * uiScale),
    SWP_NOREDRAW);
}



#else

  NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
    NAWINAPICallbackInfo info = {NA_FALSE, 0};
    return info;
  }

  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NASize size, NAMutator initFunc, void* initData) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
    return NA_NULL;
  }

  NA_DEF void na_DestructOpenGLSpace(NAOpenGLSpace* openGLSpace) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
  }

  NA_DEF void naSetOpenGLSpaceVisible(NAOpenGLSpace* openGLSpace, NABool visible) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
  } 

  NA_DEF void naSwapOpenGLSpaceBuffer(NAOpenGLSpace* openGLSpace) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
  }

  //NA_DEF void naSetOpenGLSpaceInnerRect(NAOpenGLSpace* openGLSpace, NARect bounds) {
  //  #if NA_DEBUG
  //    naError("OpenGL has not been configured. See NAConfiguration.h");
  //  #endif
  //}

  NA_DEF NARect na_GetOpenGLSpaceRect(const NA_UIElement* openGLSpace) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
    return naMakeRectZero();
  }

  NA_DEF void na_SetOpenGLSpaceRect(NA_UIElement* openGLSpace, NARect rect) {
    #if NA_DEBUG
      naError("OpenGL has not been configured. See NAConfiguration.h");
    #endif
  }

#endif  // NA_COMPILE_OPENGL == 1



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
