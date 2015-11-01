
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.



#include "NAUI.h"
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
// Now, we are sure, we compile with Objective-C and on MacOSX. The two
// #if directives will be closed at the very bottom of this file.

#include "NAMemory.h"



// The following are just comments to see what the original types should be:
// typedef ???? NAUIKeyCode;
// typedef HWND NANativeUIID;



#include "NACoord.h"
#include "NAThreading.h"
#include <stdio.h>






struct NAWindow{
  NAUIElement uielement;
  NABool fullscreen;
  NARect windowedframe;
  //HDC hDC;    // Device context
//  HGLRC hRC;  // Rendering context
//  NAList elements;
//  Rect previouswinrect;
//  TCHAR fullscreendevicename[CCHDEVICENAME];
//  Rect fullscreenrect;
//  MBController* controller;
//  Rect winrect;
//  Rect viewrect;
//  MBBool erasebackground;
//  MBBool quitonclose;
//  void show();
//  void close();
//  void enableContext();
//  virtual void prepareOpenGL(); // loads all needed stuff for best OpenGL experience
//  virtual void startOpenGLDrawing(); // opens the context
//  virtual void endOpenGLDrawing();  // swaps the buffer
//  virtual void draw();  // default implementation does nothing
//  virtual void resize(); // default implementation stores the position and size
//  virtual void keyDown(int32 key);  // default  does nothing.
//  void setPos(int posx, int posy);
//
//  void setFullscreenRect(WCHAR szDevice[CCHDEVICENAME], Rect rect);
//
//  void addElement(MBUIElement* newelement);
//  void removeElement(MBUIElement* oldelement);
//  MBUIElement* getUIElement(HWND handle);
//  
//  // The background of a window is usually filled with the default windows
//  // background color. Set it to false, if you want to prevent that.
//  MBBool eraseBackground();
//  void setEraseBackground(MBBool erase);
//
//  // The background of a window is usually filled with the default windows
//  // background color. Set it to false, if you want to prevent that.
//  MBBool quitOnClose();
//  void setQuitOnClose(MBBool quit);
};

struct NAOpenGLView{
  NAUIElement uielement;
  HGLRC hRC;    // The rendering context for OpenGL
};








#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleOnWindows(){
    int outHandle, errHandle, inHandle;
    FILE *outFile, *errFile, *inFile;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    AllocConsole();
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 9999;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    outHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    errHandle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
    inHandle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );

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



LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
  NAWindow* window;
  NARect rect;
	PAINTSTRUCT ps; // Paint information. Needed for WM_PAINT messages
  NABool hasbeenhandeled = NA_FALSE;
  NAUIKeyCode keycode;
  NASize size;

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
    rect = naGetWindowRect(window);
    rect.pos.x = LOWORD(lParam);
    rect.pos.y = HIWORD(lParam);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE, &rect);
    if(hasbeenhandeled){naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL);}
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // LOWORD(lParam): width
    // HIWORD(lParam): height
    window = naGetUIElementWindow(uielement);
    rect = naGetWindowRect(window);
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
        hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL);
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
  //  window->dispatchCommand(WM_TIMER, 0);
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
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYDOWN, &keycode);
    break;

  case WM_KEYUP:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYUP, &keycode);
    break;

  case WM_MOUSELEAVE:
  //  window->mouseLeave();
    break;

  case WM_MOUSEMOVE:
    // wParam: several special keys
    // LOWORD(lParam): x coord relative to top left
    // HIWORD(lParam): y coord relative to top left
    size.width = LOWORD(lParam);
    size.height = HIWORD(lParam);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_MOUSEMOVED, &size);
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
    //printf("oh well.");
    break;

  }

  if(hasbeenhandeled){
    return 0;
  }else{
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
}







NA_DEF void naStartDefaultApplication(void){
  // On windows, we do nothing.
}






NA_DEF void naInitUI(void){
  WNDCLASS wc;
  NAApplication* app;
  ATOM atom;

  naInitBareUI();

  app = naAlloc(NAApplication);
  naInitUIElement(app, NA_NULL, NA_UI_APPLICATION, GetModuleHandle(NULL));

  // Register the window class

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


}



NA_DEF NAUInt naRunUI(){
  MSG message;
  while(1){
    BOOL response = GetMessage(&message, 0, 0, 0);
    if(response == 0){break;}
    if (response == -1){
      //printf("message error\n");
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
  return 0;
}






NA_DEF void naRefreshUIElement(void* uielement, double timediff){
  naCallUIElementInSeconds(NA_NULL, naGetNativeID(uielement), timediff);
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
  NAWindow* window = naAlloc(NAWindow);
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


	hWnd = CreateWindow( 
		TEXT("NAWindow"), title, 
		style,
		(int)posx, (int)posy, (int)width, (int)height,
		NULL, NULL, naGetNativeID(naGetApplication()), NULL);

  //lasterror = GetLastError();
	//hDC = GetDC(hWnd);

  naInitUIElement(window, (NAUIElement*)naGetApplication(), NA_UI_WINDOW, hWnd);

  return window;
}



NA_DEF void naClearWindow(NAWindow* window){
	DestroyWindow(naGetNativeID(window));
}



NA_DEF void naShowWindow(NAWindow* window){
  ShowWindow(naGetNativeID(window), SW_SHOW);
}



NA_DEF void naSetWindowContentView(NAWindow* window, void* uielement){
  //NAUIElement* element = (NAUIElement*)uielement;
  //[(NANativeWindow*)(window->uielement.nativeID) setContentView:element->nativeID];
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  //if(fullscreen == window->fullscreen){return;}
  //if(fullscreen){
  //  window->windowedframe = naMakeRectWithNSRect([(NSWindow*)(window->uielement.nativeID) frame]);
  //  [(NSWindow*)(window->uielement.nativeID) setStyleMask:NSBorderlessWindowMask];
  //  [(NSWindow*)(window->uielement.nativeID) setFrame:[[NSScreen mainScreen] frame] display:YES];
  //  [(NSWindow*)(window->uielement.nativeID) setLevel:kCGScreenSaverWindowLevel];
  //}else{
  //  [(NSWindow*)(window->uielement.nativeID) setStyleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask];
  //  [(NSWindow*)(window->uielement.nativeID) setFrame:naMakeNSRectWithRect(window->windowedframe) display:YES];
  //  [(NSWindow*)(window->uielement.nativeID) setLevel:NSNormalWindowLevel];
  //}
  //window->fullscreen = fullscreen;
  //// Setting the first responder again is necessary as otherwise the first
  //// responder is lost.
  //[(NSWindow*)(window->uielement.nativeID) makeFirstResponder:[(NSWindow*)(window->uielement.nativeID) contentView]];
}



NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  return window->fullscreen;
}



NA_DEF NARect naGetWindowRect(NAWindow* window){
  NARect rect;
  WINDOWINFO windowinfo;
  windowinfo.cbSize = sizeof(WINDOWINFO);
  GetWindowInfo(naGetNativeID(window), &windowinfo);
  rect.size.width = windowinfo.rcWindow.right - windowinfo.rcWindow.left;
  rect.size.height = windowinfo.rcWindow.bottom - windowinfo.rcWindow.top;
  return rect;
}


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
		naGetNativeID(window), NULL, naGetNativeID(naGetApplication()), NULL );

  openglview = naAlloc(NAOpenGLView);
  naInitUIElement(openglview, (NAUIElement*)window, NA_UI_OPENGLVIEW, hWnd);

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


NA_DEF void naStartOpenGLDrawing(NAOpenGLView* openglview){
  wglMakeCurrent(GetDC(naGetNativeID(&(openglview->uielement))), openglview->hRC);
}

NA_DEF void naEndOpenGLDrawing(NAOpenGLView* openglview){
  SwapBuffers(GetDC(naGetNativeID(&(openglview->uielement))));
}



NA_DEF void naCenterMouseInView(NAOpenGLView* openglview){
//  NSRect windowframe = [(NANativeWindow*)(naGetUIElementWindow(openglview)->uielement.nativeID) frame];
//  CGPoint centerpos = {windowframe.origin.x + windowframe.size.width * .5f, windowframe.origin.y + windowframe.size.height * .5f};
//
//  //NSRect viewrect = [self convertRect:[self bounds] toView:nil];
////  NSRect screenrect = [[self window] convertRectToScreen:viewrect];
////  CGPoint centerpos = {screenrect.origin.x + screenrect.size.width * .5f, screenrect.origin.y + screenrect.size.height * .5f};
////  centerpos.y = [[self window] screen].frame.size.height - centerpos.y;
////  
//  //  // deprecated method for Snow leopard:
//  //  NSPoint centerpos = [[self window] convertBaseToScreen:NSMakePoint(400, 400)];
//  //  centerpos.y = [[self window] screen].frame.size.height - centerpos.y;
//  
//  CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, centerpos);
}



NA_DEF void naHideMouse(){
  //CGDisplayHideCursor(kCGDirectMainDisplay);
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
