
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "NAUIHiddenAPI.h"
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.

#include "NAMemory.h"






#include "NACoord.h"
#include "NAThreading.h"
#include <stdio.h>
#include <windows.h>




#define CUB_WINDOW_IGNORE_MOUSE_WARP  0x01

struct NAApplication{
  NAUIElement uielement;
  NAList timers;
  NABool ismousevisible;
};

struct NAWindow{
  NAUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
  NAUInt flags;
  TRACKMOUSEEVENT  trackingevent;
  NAUInt trackingcount;
};

struct NAOpenGLView{
  NAUIElement uielement;
  HGLRC hRC;    // The rendering context for OpenGL
};




#if (NA_SYSTEM_ADDRESS_BITS == 64)
  typedef intptr_t NAWINAPIHANDLE;
#elif (NA_SYSTEM_ADDRESS_BITS == 32)
  typedef long NAWINAPIHANDLE;
#else
  #error "Undefined system address size"
#endif


#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleWindow(){
    int outHandle, errHandle, inHandle;
    FILE *outFile, *errFile, *inFile;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    AllocConsole();
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 9999;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    outHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    errHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
    inHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );

    outFile = _fdopen(outHandle, "w" );
    errFile = _fdopen(errHandle, "w");
    inFile =  _fdopen(inHandle, "r");

    *stdout = *outFile;
    *stderr = *errFile;
    *stdin = *inFile;

    setvbuf( stdout, NULL, _IONBF, 0 );
    setvbuf( stderr, NULL, _IONBF, 0 );
    setvbuf( stdin, NULL, _IONBF, 0 );
}



// On windows, we need to reroute a timer function using a specific callback.
struct NATimerStruct{
  UINT key;
  NAFunc func;
  void* arg;
};

NA_HAPI void naAddApplicationTimer(NATimerStruct* timerstruct);
NA_HAPI NABool naExecuteApplicationTimer(UINT timerid);

NA_HDEF static VOID CALLBACK naTimerCallbackFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
  //todo something is wrong here with the type.
  naExecuteApplicationTimer((UINT)idEvent);
}


NA_HDEF void naAddApplicationTimer(NATimerStruct* timerstruct){
  NAApplication* app = naGetApplication();
  naAddListLastMutable(&(app->timers), timerstruct);
}


NA_HDEF NABool naExecuteApplicationTimer(UINT timerid){
  NATimerStruct* curstruct;
  NAApplication* app = naGetApplication();
  while((curstruct = naIterateListMutable(&(app->timers), 1))){
    if(curstruct->key == timerid){
      naRemoveListPrevMutable(&(app->timers));
      curstruct->func(curstruct->arg);
      naFree(curstruct);
      return NA_TRUE;
    }
  }
  return NA_FALSE;
}


NA_DEF void naCallFunctionInSeconds(NAFunc function, void* arg, double timediff){
  NATimerStruct* timerstruct = naAlloc(NATimerStruct);
  timerstruct->func = function;
  timerstruct->arg = arg;
  //todo something is wrong here with the type.
  timerstruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), naTimerCallbackFunction);
  naAddApplicationTimer(timerstruct);
}



LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
  NAWindow* window;
  NARect rect;
	PAINTSTRUCT ps; // Paint information. Needed for WM_PAINT messages
  NABool hasbeenhandeled = NA_FALSE;
  NAUIKeyCode keycode;
  UINT scancode;
  NASize size;
  NAPos pos;
  const NACursorInfo* cursorinfo;

  NAUIElement* uielement = naGetUINALibEquivalent(hWnd);
  if(!uielement){
    // If we do not have the hWnd registered in NALib UI, we do not process it. Simply
    // deletage it to the default window procedure and return.
    return DefWindowProc(hWnd, message, wParam, lParam);
  }


  //if((message >= WM_USER) && (message <= 0x7fff)){
  //  // User defined message
  //  naDispatchUIElementCommand(uielement, ???, wParam);
  //  return 0;
  //}

  switch (message){
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
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE, &rect);
    if(hasbeenhandeled){naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL);}
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // LOWORD(lParam): width
    // HIWORD(lParam): height
    window = naGetUIElementWindow(uielement);
    rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    rect.size.width = LOWORD(lParam);
    rect.size.height = HIWORD(lParam);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE, &rect);
    if(hasbeenhandeled){naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL);}
    break;

  case WM_PAINT:
    // wParam: Unused
    // lParam: Unused
    if(uielement->elementtype == NA_UI_OPENGLVIEW){
      BeginPaint(hWnd, &ps);

        //if(uielement is opengl)
//        wglMakeCurrent(GetDC(naGetUINativeID(&(openglview->uielement))), openglview->hRC);

        hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL);
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
  //  if(!uielement){return 0;}
  //  uielement->dispatchCommand(LOWORD(wParam), 0); // note that this is the loword
    break;

  case WM_KEYDOWN:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    //printf("%x\n", scancode);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYDOWN, &scancode);
    break;

  case WM_KEYUP:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYUP, &scancode);
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
      cursorinfo = naGetMouseInfo();
      pos = naGetCursorPos(cursorinfo);
      naSetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);
      hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_MOUSE_MOVED, &size);
    //}
    break;

  case WM_LBUTTONUP:
  //  window->leftMouseUp(LOWORD(lParam), HIWORD(lParam), wParam);
    break;

  case WM_COMMAND:
  //  uielement = window->getUIElement((HWND)lParam);
  //  if(!uielement){return 0;}
  //  uielement->dispatchCommand(HIWORD(wParam), 0);
    break;

  case WM_NOTIFY:
  //  uielement = window->getUIElement(((LPNMHDR)lParam)->hwndFrom);
  //  if(!uielement){return 0;}
  //  uielement->dispatchCommand(((LPNMHDR)lParam)->code, lParam);
    break;

  case WM_USER:
  //  uielement = window->getUIElement((HWND)lParam);
  //  if(!uielement){return 0;}
  //  uielement->dispatchCommand(HIWORD(wParam), 0);
    break;

  case WM_CLOSE:
  //  window->close();
  //  if(!::gui->numwindows){PostQuitMessage(0);}
    break;
 
  default:
    break;

  }

  if(hasbeenhandeled){
    return 0;
  }else{
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}







NA_API void naStartApplication(  NAFunc prestartup,
                                 NAFunc poststartup,
                                  void* arg){
  NAApplication* app;
  WNDCLASS wc;
  ATOM atom;
  MSG message;

  naInitBareUI();
  app = naAlloc(NAApplication);
  naInitUIElement((NAUIElement*)app, NA_NULL, NA_UI_APPLICATION, GetModuleHandle(NULL));
  naInitList(&(app->timers));
  app->ismousevisible = NA_TRUE;

  if(prestartup){prestartup(arg);}

  // Register the window classes
  naNulln(&wc, sizeof(WNDCLASS));
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowCallback;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("NAWindow");
	atom = RegisterClass( &wc );

  naNulln(&wc, sizeof(WNDCLASS));
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowCallback;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("NAView");
	RegisterClass( &wc );

  if(poststartup){poststartup(arg);}


  while(1){
    BOOL response = GetMessage(&message, 0, 0, 0);
    if(response == 0){break;}
    if (response == -1){
      // handle the error and possibly exit
      break;
    }else{
      if(message.message == WM_QUIT){
        break;
      }
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

}



NA_DEF void cubFlushGarbageMemory(){
  // On windows, we do nothing.
}







NA_HDEF void naRefreshUIElementNow (void* uielement){
  PostMessage(naGetUINativeID(uielement), WM_PAINT, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
}







NA_HDEF NARect naGetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NAUIElement* screen){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NAUIElement* window){
  NARect rect;
  NARect screenrect;
  RECT contentrect;
  POINT testpoint = {0, 0};

  GetClientRect(window->nativeID, &contentrect);
  ClientToScreen(window->nativeID, &testpoint);

  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = screenrect.size.height - (testpoint.y + (contentrect.bottom - contentrect.top));
  rect.size.width = contentrect.right - contentrect.left;
  rect.size.height = contentrect.bottom - contentrect.top;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteOuterRect(NAUIElement* window){
  NARect rect;
  NARect screenrect;
  RECT windowrect;

  GetWindowRect(window->nativeID, &windowrect);
  screenrect = naGetMainScreenRect();

  rect.pos.x = windowrect.left;
  rect.pos.y = screenrect.size.height - windowrect.bottom;
  rect.size.width = windowrect.right - windowrect.left;
  rect.size.height = windowrect.bottom - windowrect.top;
  return rect;
}



NA_HDEF NARect naGetViewAbsoluteInnerRect(NAUIElement* view){
  NARect rect;
  NARect screenrect;
  RECT contentrect;
  POINT testpoint = {0, 0};

  GetClientRect(view->nativeID, &contentrect);
  ClientToScreen(view->nativeID, &testpoint);
  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = screenrect.size.height - (testpoint.y + (contentrect.bottom - contentrect.top));
  rect.size.width = contentrect.right - contentrect.left;
  rect.size.height = contentrect.bottom - contentrect.top;
  return rect;
}



NA_DEF NARect naGetUIElementRect(void* uielement, void* relativeelement, NABool includebounds){
  NARect rect;
  NARect relrect;
  NAUIElement* element;
  NAUIElement* relelement;
  NAApplication* app;
  
  element = (NAUIElement*)uielement;
  relelement = (NAUIElement*)relativeelement;
  app = naGetApplication();
  
  // First, let's handle the root case: Returning the application rect.
  if(element == (NAUIElement*)app){
    #ifndef NDEBUG
      if(relativeelement && (relativeelement != app))
        naError("naGetUIElementRect", "The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }
  
  // Now, we find the appropriate relative element.
  if(!relelement){relelement = naGetUIElementParent(element);}
  
  switch(element->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(element); break;
  case NA_UI_WINDOW:
    if(includebounds){
      rect = naGetWindowAbsoluteOuterRect(element);
    }else{
      rect = naGetWindowAbsoluteInnerRect(element);
    }
    break;
  case NA_UI_OPENGLVIEW:  rect = naGetViewAbsoluteInnerRect(element); break;
  }
  
  switch(relelement->elementtype){
  case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(relelement); break;
  case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(relelement); break;
  case NA_UI_OPENGLVIEW:  relrect = naGetViewAbsoluteInnerRect(relelement); break;
  }
  
  rect.pos.x = rect.pos.x - relrect.pos.x;
  rect.pos.y = rect.pos.y - relrect.pos.y;
  rect.size.width = rect.size.width;
  rect.size.height = rect.size.height;
  
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
  rect.pos.x = screeninfo.rcMonitor.left;;
  rect.pos.y = screeninfo.rcMonitor.bottom - screeninfo.rcMonitor.top;
  rect.size.width = screeninfo.rcMonitor.right - screeninfo.rcMonitor.left;
  rect.size.height = screeninfo.rcMonitor.bottom - screeninfo.rcMonitor.top;
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






NA_DEF NAWindow* naNewWindow(const char* title, double posx, double posy, double width, double height, NABool resizeable){
  DWORD style;
  HWND hWnd;
  RECT windowrect;
  NAWindow* window = naAlloc(NAWindow);
  NARect screenrect;
  //DWORD lasterror;

	//hRC = NULL;
	//hDC = NULL;
 // elements = NULL;
 // numelements = 0;
 // winrect.posx = newposx;
 // winrect.posy = newposy;
 // winrect.width = newwidth;
 // winrect.height = newheight;
 // erasebackground = MB_TRUE;
 // quitonclose = MB_FALSE;
 // fullscreen = MB_FALSE;
 // controller = newcontroller;

  style = WS_OVERLAPPEDWINDOW;
  if(!resizeable){style &= ~WS_THICKFRAME;}
  screenrect = naGetMainScreenRect();

  windowrect.top = (int)(screenrect.size.height - posy - height);
  windowrect.right = (int)(posx + width);
  windowrect.bottom = (int)(screenrect.size.height - posy);
  windowrect.left = (int)posx;
  AdjustWindowRect(&windowrect, style, NA_FALSE);

	hWnd = CreateWindow( 
		TEXT("NAWindow"), title, 
		style,
		windowrect.left, windowrect.top, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top,
		NULL, NULL, naGetUINativeID(naGetApplication()), NULL);

  //lasterror = GetLastError();
	//hDC = GetDC(hWnd);

  naInitUIElement((NAUIElement*)window, (NAUIElement*)naGetApplication(), NA_UI_WINDOW, hWnd);

  window->flags = 0;
  window->trackingcount = 0;
  window->fullscreen = NA_FALSE;

  return window;
}




NA_HDEF void naRenewWindowMouseTracking(NAWindow* window){
  //window->trackingarea = [[NSTrackingArea alloc] initWithRect:[[(NSWindow*)naGetUINativeID(window) contentView] bounds]
  //    options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
  //    owner:(NSWindow*)naGetUINativeID(window) userInfo:nil];
  //[[(NSWindow*)naGetUINativeID(window) contentView] addTrackingArea:window->trackingarea];
}



NA_HDEF void naClearWindowMouseTracking(NAWindow* window){
  //[[(NSWindow*)naGetUINativeID(window) contentView] removeTrackingArea:window->trackingarea];
  //[window->trackingarea release];
  //window->trackingarea = nil;
}



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  window->trackingcount++;
  //if(window->trackingcount == 1){
  //  [(NSWindow*)naGetUINativeID(window) setAcceptsMouseMovedEvents:YES];
  //  naRenewWindowMouseTracking(window);
  //}
}



NA_HDEF void naReleaseWindowMouseTracking(NAWindow* window){
  window->trackingcount--;
  //if(window->trackingcount == 0){
  //  [(NSWindow*)naGetUINativeID(window) setAcceptsMouseMovedEvents:NO];
  //  naClearWindowMouseTracking(window);
  //}
}

NA_DEF void naClearWindow(NAWindow* window){
	DestroyWindow(naGetUINativeID(window));
}



NA_DEF void naShowWindow(NAWindow* window){
  ShowWindow(naGetUINativeID(window), SW_SHOW);
}



NA_DEF void naSetWindowContentView(NAWindow* window, void* uielement){
  naAddListLastMutable(&(window->uielement.childs), uielement); // todo: this is a hack just for now.
  //NAUIElement* element = (NAUIElement*)uielement;
  //[(NANativeWindow*)(window->uielement.nativeID) setContentView:element->nativeID];
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  DWORD style;
  //NABool hasbeenhandeled;
  NARect newrect;
  NARect screenrect;

  if(fullscreen == window->fullscreen){return;}
  window->fullscreen = fullscreen;

  //HWND taskbar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
  //HWND startbutton = FindWindow(TEXT("Button"), NULL);

  screenrect = naGetMainScreenRect();

  if(fullscreen){
    DEVMODE screenSettings;
    window->windowedframe = naGetUIElementRect(window, naGetApplication(), NA_TRUE);

    newrect = naGetMainScreenRect(NA_NULL);
    
    memset(&screenSettings, 0, sizeof(screenSettings)); // set everything to 0
    screenSettings.dmSize = sizeof(screenSettings);
    //memcpy(screenSettings.dmDeviceName, fullscreendevicename, CCHDEVICENAME * sizeof(WCHAR));
    screenSettings.dmPelsWidth = (DWORD)newrect.size.width;
    screenSettings.dmPelsHeight = (DWORD)newrect.size.height;
    screenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

    style = WS_POPUP;
    SetWindowLongPtr(naGetUINativeID(window), GWL_STYLE, style);
    SetWindowPos(naGetUINativeID(window), HWND_TOPMOST, (int)screenrect.pos.x, (int)(screenrect.pos.y - screenrect.pos.y), (int)screenrect.size.width, (int)screenrect.size.height, SWP_SHOWWINDOW);
    //ChangeDisplaySettings(NULL, 0);
    ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
  }else{
    newrect = window->windowedframe;
    style = WS_OVERLAPPEDWINDOW;
    SetWindowLongPtr(naGetUINativeID(window), GWL_STYLE, style);
    SetWindowPos(naGetUINativeID(window), HWND_NOTOPMOST, (int)window->windowedframe.pos.x, (int)(screenrect.size.height - window->windowedframe.pos.y), (int)window->windowedframe.size.width, (int)window->windowedframe.size.height, SWP_SHOWWINDOW);
    ChangeDisplaySettings(NULL, 0);
  }

  //hasbeenhandeled = naDispatchUIElementCommand(window, NA_UI_COMMAND_RESHAPE, &newrect);
  //if(hasbeenhandeled){naDispatchUIElementCommand(window, NA_UI_COMMAND_REDRAW, NA_NULL);}
}



NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  return window->fullscreen;
}



//NA_DEF NARect naGetWindowRect(NAWindow* window){
//  NARect rect;
//  WINDOWINFO windowinfo;
//  windowinfo.cbSize = sizeof(WINDOWINFO);
//  GetWindowInfo(naGetUINativeID(window), &windowinfo);
//  rect.size.width = windowinfo.rcWindow.right - windowinfo.rcWindow.left;
//  rect.size.height = windowinfo.rcWindow.bottom - windowinfo.rcWindow.top;
//  return rect;
//}

#ifdef __gl_h_

NA_DEF NAOpenGLView* naNewOpenGLView(NAWindow* window, double width, double height){
	
  HWND hWnd;
  HDC hDC;
 	PIXELFORMATDESCRIPTOR pfd;
  int format;
  NAOpenGLView* openglview;

	hWnd = CreateWindow( 
		TEXT("NAView"), "OpenGL View", 
		WS_CHILD | WS_VISIBLE | ES_READONLY,
		0, 0, (int)width, (int)height,
		naGetUINativeID(window), NULL, naGetUINativeID(naGetApplication()), NULL );

  openglview = naAlloc(NAOpenGLView);
  naInitUIElement((NAUIElement*)openglview, (NAUIElement*)window, NA_UI_OPENGLVIEW, hWnd);

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
	openglview->hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, openglview->hRC);

	//glewInit();
  return openglview;
}


NA_DEF void naSwapOpenGLBuffer(NAOpenGLView* openglview){
  SwapBuffers(GetDC(naGetUINativeID(&(openglview->uielement))));
}



NA_API void naSetOpenGLInnerRect(NAOpenGLView* openglview, NARect bounds){
  //NARect windowrect = naGetUIElementRect(naGetUIElementParent(openglview), naGetApplication(), NA_FALSE);
  SetWindowPos(naGetUINativeID(openglview), HWND_TOP, 0, 0, (int)bounds.size.width, (int)bounds.size.height, SWP_NOREDRAW);
}


#endif


NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NARect viewrect;
  NARect screenframe;
  NAPos centerpos;
  viewrect = naGetUIElementRect(uielement, naGetApplication(), includebounds);
  // todo: screen not defined
  screenframe = naGetMainScreenRect();
  centerpos.x = viewrect.pos.x + viewrect.size.width * .5f;
  centerpos.y = viewrect.pos.y + viewrect.size.height * .5f;
  
//  naGetUIElementWindow(uielement)->flags |= CUB_WINDOW_IGNORE_MOUSE_WARP;
  naSetMouseWarpedTo(centerpos);
  SetCursorPos((int)centerpos.x, (int)screenframe.size.height - (int)centerpos.y);
}



NA_DEF void naShowMouse(){
  NAApplication* app = naGetApplication();
  if(!app->ismousevisible){
    ShowCursor(1);
    app->ismousevisible = NA_TRUE;
  }
}


NA_DEF void naHideMouse(){
  NAApplication* app = naGetApplication();
  if(app->ismousevisible){
    ShowCursor(0);
    app->ismousevisible = NA_FALSE;
  }
}


#endif // NA_SYSTEM == NA_SYSTEM_WINDOWS

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
