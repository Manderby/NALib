
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../../../NAUICore.h"



//#include <windows.h>
//#ifdef __gl_h_
//  #include <GL/GL.h>
//#endif

#define CUB_WINDOW_IGNORE_MOUSE_WARP  0x01

typedef struct NAWINAPIApplication NAWINAPIApplication;
typedef struct NAWINAPIWindow NAWINAPIWindow;
typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;


NA_HAPI NARect naGetApplicationAbsoluteRect(void);
NA_HAPI NARect naGetScreenAbsoluteRect(NACoreUIElement* screen);
NA_HAPI NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window);
NA_HAPI NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HDEF NARect naGetImageSpaceAbsoluteInnerRect(NACoreUIElement* imagespace);
NA_HDEF NARect naGetOpenGLSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HAPI NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* button);
NA_HAPI NARect naGetRadioAbsoluteInnerRect(NACoreUIElement* radio);
NA_HAPI NARect naGetCheckBoxAbsoluteInnerRect(NACoreUIElement* checkbox);
NA_HAPI NARect naGetLabelAbsoluteInnerRect(NACoreUIElement* label);
NA_HDEF NARect naGetTextFieldAbsoluteInnerRect(NACoreUIElement* textfield);
NA_HDEF NARect naGetTextBoxAbsoluteInnerRect(NACoreUIElement* textbox);

NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window);

NA_HAPI void naRenewWindowMouseTracking(NACoreWindow* corewindow);
NA_HAPI void naClearWindowMouseTracking(NACoreWindow* corewindow);


// The following struct stores all relevant data which will then be stored in
// a list of the running NAWINAPIApplication.
typedef struct NATimerStruct NATimerStruct;
struct NATimerStruct {
  UINT key;
  NAMutator func;
  void* arg;
};

// The struct NAWINAPIApplication stores a list of timers which could otherwise
// not be done.
struct NAWINAPIApplication {
  NACoreApplication coreapplication;
  NAList timers;
};

struct NAWINAPIWindow {
  NACoreWindow corewindow;
  uint32 flags;
  NAUInt trackingcount;
  NABool fullscreen;
  NARect windowedframe;
  NASize size;
  NABounds4 bounds;
};

struct NAWINAPIOpenGLSpace {
  NACoreOpenGLSpace coreopenglspace;
  HGLRC hRC;    // The rendering context for OpenGL
};



NA_HDEF void naClearUINativeId(NANativeID nativeId){
  //NA_COCOA_DISPOSE(nativeId);
}


NA_DEF void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText){
    //NSAlert* alert = NA_COCOA_AUTORELEASE([[NSAlert alloc] init]);

    //switch(alertBoxType){
    //case NA_ALERT_BOX_INFO:    alert.alertStyle = NSInformationalAlertStyle; break;
    //case NA_ALERT_BOX_WARNING: alert.alertStyle = NSAlertStyleWarning; break;
    //case NA_ALERT_BOX_ERROR:   alert.alertStyle = NSAlertStyleCritical; break;
    //}
    //
    //alert.messageText = [NSString stringWithUTF8String:titleText];
    //alert.informativeText = [NSString stringWithUTF8String:infoText];
    //[alert runModal];
}


NA_DEF void naSetUIElementNextTabElement(NAUIElement* elem, NAUIElement* nextelem){
  //naDefineNativeCocoaObject(NANativeTextField, nativeelem, elem);
  //naDefineNativeCocoaObject(NSView, nativeview, nextelem);
  //[nativeelem setNextKeyView:nativeview];
}





// ///////////////////////////////////
// WINDOW CALLBACK
// ///////////////////////////////////

// This is the one and only, master of destruction, defender of chaos and
// pimp of the century function handling all and everything using the most
// beautiful and highly pattern-absent spaghetti coding structure ever
// seen to mankind.
//
// Send me a fax if you have a better idea. It works.

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWindow* window;
  NARect rect;
  PAINTSTRUCT ps; // Paint information. Needed for WM_PAINT messages
  NABool hasbeenhandeled = NA_FALSE;
  LRESULT retvalue;
  NAUIKeyCode keycode;
  NAUIKeyCode scancode;   // used as UINT, converted to NAUIKeyCode
  NASize size;
  NAPos pos;
  const NAMouseStatus* mousestatus;

  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(hWnd);
  if (!uielement) {
    // If we do not have the hWnd registered in NALib UI, we do not process it. Simply
    // deletage it to the default window procedure and return.
    return DefWindowProc(hWnd, message, wParam, lParam);
  }


  //if((message >= WM_USER) && (message <= 0x7fff)){
  //  // User defined message
  //  naDispatchUIElementCommand(uielement, ???, wParam);
  //  return 0;
  //}

  switch (message) {
  case WM_SHOWWINDOW:
    // wParam: true for show, false for hide
    // lParam: status of window
    break;

  case WM_MOVE:
    // wParam: Unused
    // LOWORD(lParam): x coordinate
    // HIWORD(lParam): y coordinate
    window = naGetUIElementWindow(uielement);
    rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    rect.pos.x = (double)((int)(short)LOWORD(lParam));
    rect.pos.y = (double)((int)(short)HIWORD(lParam));
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE);
    if (hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW); }
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // LOWORD(lParam): width
    // HIWORD(lParam): height
    window = naGetUIElementWindow(uielement);
    rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    rect.size.width = LOWORD(lParam);
    rect.size.height = HIWORD(lParam);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE);
    if (hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW); }
    break;

  case WM_PAINT:
    // wParam: Unused
    // lParam: Unused
    if (uielement->elementtype == NA_UI_OPENGLSPACE) {
      BeginPaint(hWnd, &ps);

      //if(uielement is opengl)
      //        wglMakeCurrent(GetDC(naGetUIElementNativeID(&(openglspace->uielement))), openglspace->hRC);

      hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW);
      //uielement->refreshrequested = NA_FALSE;
      EndPaint(hWnd, &ps);
    }
    break;

  case WM_ERASEBKGND:
    hasbeenhandeled = NA_TRUE;
    //  if(window->eraseBackground()){
    //    DefWindowProc(hWnd, message, wParam, lParam);
    //  }else{
    //  }
    break;

  case WM_TIMER:
    // Note: Does not work with hWnd == NULL. Will not be called here.
    // wParam: timer identifier as an UINT
    // lParam: callback function. Unused in NALib
    //hasbeenhandeled = naExecuteApplicationTimer((UINT)wParam);
    break;

  case WM_HSCROLL:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(LOWORD(wParam), 0);} // note that this is the loword
    break;

  case WM_KEYDOWN:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = (NAUIKeyCode)MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYDOWN);
    break;

  case WM_KEYUP:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYUP);
    break;

  case WM_MOUSELEAVE:
    //  window->mouseLeave();
    break;

  case WM_MOUSEMOVE:
    // wParam: several special keys
    // GET_X_LPARAM(lParam): x coord relative to top left
    // GET_Y_LPARAM(lParam): y coord relative to top left
    window = naGetUIElementWindow(uielement);
    //if(window->flags & CUB_WINDOW_IGNORE_MOUSE_WARP){
    //  window->flags &= ~CUB_WINDOW_IGNORE_MOUSE_WARP;
    //  hasbeenhandeled = NA_TRUE;
    //}else{
    // todo: this should be GET_X_LPARAM and GET_Y_LPARAM
    // but is undefined somehow.
    size.width = LOWORD(lParam);
    size.height = HIWORD(lParam);
    rect = naGetUIElementRect(uielement, naGetApplication(), NA_FALSE);
    size.width += rect.pos.x;
    size.height += rect.pos.y;
    mousestatus = naGetMouseStatus();
    pos = naGetMousePos(mousestatus);
    naSetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_MOUSE_MOVED);
    //}
    break;

  case WM_LBUTTONUP:
    //  window->leftMouseUp(LOWORD(lParam), HIWORD(lParam), wParam);
    break;

  case WM_COMMAND:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(HIWORD(wParam), 0);}
    break;

  case WM_NOTIFY:
    //  uielement = window->getUIElement(((LPNMHDR)lParam)->hwndFrom);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(((LPNMHDR)lParam)->code, lParam);}
    break;

  case WM_USER:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(HIWORD(wParam), 0);}
    break;

  case WM_CLOSE:
    //  window->close();
    //  if(!::gui->numwindows){PostQuitMessage(0);}
    break;

  default:
    break;

  }

  if(hasbeenhandeled) {
    retvalue = 0;
  }else{
    retvalue = DefWindowProc(hWnd, message, wParam, lParam);
  }
  return retvalue;
}





// ///////////////////////////////////
// TIMER
// ///////////////////////////////////


// This is the native WINAPI callback function. It finds the corresponding
// registered timer struct of the application and executes the function
// stored in that struct with the stored argument.
//
// Definitely not the fastest and best method. But as for now, it's ok. todo.
NA_HDEF static VOID CALLBACK naTimerCallbackFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
  //todo something is wrong here with the type.
  NAWINAPIApplication* app;
  NAListIterator iter;

  UINT timerkey = (UINT)idEvent;
  app = (NAWINAPIApplication*)naGetApplication();

  naBeginListModifierIteration(NATimerStruct* timerstruct, &(app->timers), iter);
    if(timerstruct->key == timerkey) {
      naRemoveListCurMutable(&iter, NA_FALSE);
      KillTimer(hwnd, idEvent);
      timerstruct->func(timerstruct->arg);
      naFree(timerstruct);
      break;
    }
  naEndListIteration(iter);
}




// ///////////////////////////////////
// APPLICATION
// ///////////////////////////////////




#if (NA_SYSTEM_ADDRESS_BITS == 64)
  typedef intptr_t NAWINAPIHANDLE;
#elif (NA_SYSTEM_ADDRESS_BITS == 32)
  typedef long NAWINAPIHANDLE;
#else
  #error "Undefined system address bytesize"
#endif







// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_HDEF void naRefreshUIElementNow(NAUIElement* uielement){
  PostMessage(naGetUIElementNativeID(uielement), WM_PAINT, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
}






NA_HDEF NARect naGetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NACoreUIElement* screen){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_DEF NARect naGetUIElementRect(NACoreUIElement* uielement, NAUIElement* relativeelement, NABool includebounds){
  NARect rect;
  NARect relrect;
  NACoreUIElement* coreelement;
  NACoreUIElement* corerelelement;
  NAWINAPIApplication* app;

  coreelement = (NACoreUIElement*)uielement;
  corerelelement = (NACoreUIElement*)relativeelement;
  app = (NAWINAPIApplication*)naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(coreelement == (NACoreUIElement*)app){
    #ifndef NDEBUG
      if(relativeelement && (relativeelement != app))
        naError("The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }

  // Now, we find the appropriate relative element.
  if(!corerelelement){corerelelement = naGetUIElementParent(coreelement);}

  switch(coreelement->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(coreelement); break;
  case NA_UI_WINDOW:
    if(includebounds){
      rect = naGetWindowAbsoluteOuterRect(coreelement);
    }else{
      rect = naGetWindowAbsoluteInnerRect(coreelement);
    }
    break;
  case NA_UI_OPENGLSPACE:  rect = naGetSpaceAbsoluteInnerRect(coreelement); break;
  default:
    #ifndef NDEBUG
      naError("Invalid UI type");
    #endif
    rect = naMakeRectSE(0., 0., 0., 0.);
    break;
  }

  if(corerelelement){
    switch(corerelelement->elementtype){
    case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
    case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(corerelelement); break;
    case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(corerelelement); break;
    case NA_UI_OPENGLSPACE:  relrect = naGetSpaceAbsoluteInnerRect(corerelelement); break;
    default:
      #ifndef NDEBUG
        naError("Invalid UI type");
      #endif
      relrect = naMakeRectSE(0., 0., 0., 0.);
      break;
    }

    rect.pos.x = rect.pos.x - relrect.pos.x;
    rect.pos.y = rect.pos.y - relrect.pos.y;
    rect.size.width = rect.size.width;
    rect.size.height = rect.size.height;
  }
  // Convert the rect into absolute coordinates.

  return rect;
}



NA_API NARect naGetMainScreenRect(){
  HMONITOR screen;
  MONITORINFO screeninfo;
  NARect rect;
  screen = MonitorFromWindow(NA_NULL, MONITOR_DEFAULTTOPRIMARY);
  screeninfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(screen, &screeninfo);
  rect.pos.x = screeninfo.rcMonitor.left;
  rect.pos.y = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  rect.size.width = (double)screeninfo.rcMonitor.right - (double)screeninfo.rcMonitor.left;
  rect.size.height = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  return rect;
}


//void MBUIElement::setEnabled(MBBool enabled){
//  SendMessage(hWnd, WM_ENABLE, (WPARAM)enabled, 0);
//}
//
//
//
//void MBUIElement::leftMouseUp(int32 x, int32 y, int32 modifierkeys){
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseMove(int32 x, int32 y, int32 modifierkeys){
//  if(mousex == -1){
//    TRACKMOUSEEVENT tme;
//    tme.cbSize = sizeof(TRACKMOUSEEVENT);
//    tme.dwFlags = TME_LEAVE;
//    tme.hwndTrack = hWnd;
//    tme.dwHoverTime = HOVER_DEFAULT;
//    TrackMouseEvent(&tme);
//  }
//
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseLeave(){
//  mousex = -1;
//  mousey = -1;
//
//  TRACKMOUSEEVENT tme;
//  tme.cbSize = sizeof(TRACKMOUSEEVENT);
//  tme.dwFlags = TME_CANCEL;
//  tme.hwndTrack = hWnd;
//  tme.dwHoverTime = HOVER_DEFAULT;
//  TrackMouseEvent(&tme);
//}




//NA_DEF NARect naGetWindowRect(NAWindow* window){
//  NARect rect;
//  WINDOWINFO windowinfo;
//  windowinfo.cbSize = sizeof(WINDOWINFO);
//  GetWindowInfo(naGetUIElementNativeID(window), &windowinfo);
//  rect.size.width = windowinfo.rcWindow.right - windowinfo.rcWindow.left;
//  rect.size.height = windowinfo.rcWindow.bottom - windowinfo.rcWindow.top;
//  return rect;
//}

#if NA_CONFIG_COMPILE_OPENGL == 1
  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
	
    HWND hWnd;
    HDC hDC;
 	  PIXELFORMATDESCRIPTOR pfd;
    int format;
    NAWINAPIOpenGLSpace* openglspace;

	  hWnd = CreateWindow(
		  TEXT("NASpace"), "OpenGL Space",
		  WS_CHILD | WS_VISIBLE | ES_READONLY,
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

#endif


NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NARect spacerect;
  NARect screenframe;
  NAPos centerpos;
  spacerect = naGetUIElementRect(uielement, naGetApplication(), includebounds);
  // todo: screen not defined
  screenframe = naGetMainScreenRect();
  centerpos.x = spacerect.pos.x + spacerect.size.width * .5f;
  centerpos.y = spacerect.pos.y + spacerect.size.height * .5f;

//  naGetUIElementWindow(uielement)->flags |= CUB_WINDOW_IGNORE_MOUSE_WARP;
  naSetMouseWarpedTo(centerpos);
  SetCursorPos((int)centerpos.x, (int)screenframe.size.height - (int)centerpos.y);
}



NA_DEF void naShowMouse(){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  //if(!app->ismousevisible){
  //  ShowCursor(1);
  //  app->ismousevisible = NA_TRUE;
  //}
}


NA_DEF void naHideMouse(){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  //if(app->ismousevisible){
  //  ShowCursor(0);
  //  app->ismousevisible = NA_FALSE;
  //}
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
