
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../../../../NAPreferences.h"

typedef struct NAWINAPIWindow NAWINAPIWindow;
struct NAWINAPIWindow {
  NACoreWindow corewindow;
  //NAUInt trackingcount;
  //NABounds4 bounds;
};



NAWINAPICallbackInfo naWindowWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NACoreWindow* corewindow;
  NABool shouldClose;

  switch(message){
  case WM_SHOWWINDOW:
    // wParam: true for show, false for hide
    // lParam: status of window
    // result: 0 when handeled.
    break;

  case WM_MOVE:
    // wParam: Unused
    // lParam: (int)(short)LOWORD: x coordinate, (int)(short)HIWORD: y coordinate
    // result: 0 when handeled.
    corewindow = (NACoreWindow*)naGetUIElementWindow(uielement);
    info.hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE);
    if (info.hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW); }
    naRememberWindowPosition(corewindow);
    info.result = 0;
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // lParam: LOWORD: width, HIWORD: height
    // result: 0 when handeled.
    corewindow = (NACoreWindow*)naGetUIElementWindow(uielement);
    info.hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE);
    if (info.hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW); }
    naRememberWindowPosition(corewindow);
    info.result = 0;
    break;

  //case WM_MOUSEMOVE:
  //  // wParam: several special keys
  //  // GET_X_LPARAM(lParam): x coord relative to top left
  //  // GET_Y_LPARAM(lParam): y coord relative to top left
    // result: 0 when handeled.
  //  window = naGetUIElementWindow(uielement);
  //  //if(window->flags & CUB_WINDOW_IGNORE_MOUSE_WARP){
  //  //  window->flags &= ~CUB_WINDOW_IGNORE_MOUSE_WARP;
  //  //  hasbeenhandeled = NA_TRUE;
  //  //}else{
  //  // todo: this should be GET_X_LPARAM and GET_Y_LPARAM
  //  // but is undefined somehow.
  //  size.width = GET_X_LPARAM(lParam);
  //  size.height = GET_Y_LPARAM(lParam);
  //  rect = naGetUIElementRect(uielement, naGetApplication(), NA_FALSE);
  //  size.width += rect.pos.x;
  //  size.height += rect.pos.y;
  //  mousestatus = naGetMouseStatus();
  //  pos = naGetMousePos(mousestatus);
  //  naSetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);
  //  hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_MOUSE_MOVED);
  //  //}
  //  info.result = 0;
  //  break;

  case WM_CLOSE:
    corewindow = (NACoreWindow*)naGetUIElementWindow(uielement);
    naDispatchUIElementCommand(uielement, NA_UI_COMMAND_CLOSES);
    shouldClose = !naGetFlagi(corewindow->flags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
    naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
    if(shouldClose){naCloseWindow(corewindow);}
    info.hasbeenhandeled = NA_TRUE;
    info.result = 0;
    break;

  case WM_CHILDACTIVATE:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_SETTEXT:
  case WM_WINDOWPOSCHANGING:
  case WM_ACTIVATEAPP:
  case WM_NCACTIVATE:
  case WM_GETICON:
  case WM_ACTIVATE:
  case WM_IME_SETCONTEXT:
  case WM_WINDOWPOSCHANGED:
  case WM_SYNCPAINT:
  case WM_DWMNCRENDERINGCHANGED:
  case WM_GETMINMAXINFO:
  case WM_KILLFOCUS:
  case WM_IME_NOTIFY:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_GETFONT:
  case WM_SETFONT:
  case WM_SETFOCUS:
  case WM_NCHITTEST:
  case WM_CTLCOLORSTATIC:
  case WM_CTLCOLOREDIT:
  case WM_CTLCOLORBTN:
  case WM_PRINTCLIENT:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_SETCURSOR:
  case WM_NCMOUSEMOVE:
  case WM_NCMOUSELEAVE:
  case WM_MOUSEMOVE: // same as WM_MOUSEFIRST
  case WM_MOUSELEAVE:
  case WM_PARENTNOTIFY:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_LBUTTONUP:
  case WM_TIMER:
  case WM_KEYDOWN:  // same as WM_KEYFIRST
  case WM_CHAR:
  case WM_KEYUP:
  case WM_SYSKEYDOWN: // the alt key
  case WM_SYSKEYUP:
  case WM_SYSCOMMAND:
  case WM_ENTERMENULOOP:
  case WM_INITMENU:
  case WM_MENUSELECT:
  case WM_EXITMENULOOP:
  case WM_ENTERIDLE:
  case WM_NCCALCSIZE:
  case WM_NCLBUTTONDOWN:
  case SPI_GETDOCKMOVING:
  case WM_DESTROY:
  case WM_NCDESTROY:
    break;

  // Cases being called due to bubbling the message.
  case BM_SETCHECK:
    break;

  // Dark mode stuff
  //case WM_DWMCOLORIZATIONCOLORCHANGED:
  //  DwmGetColorizationColor(&colorization, &lParam);
  //  info.result = 0;
  //  break;
  //case WM_THEMECHANGED :
  //case WM_WININICHANGE:
  //  "hello dark mode";
  //  break;

  default:
    //printf("Uncaught Window message: %X\n", (int)message);
    //info.result = 0;
    break;
  }
  
  return info;
}

//@implementation NANativeWindow
//- (id) initWithCoreWindow:(NACoreWindow*)newcorewindow contentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen{
//  self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
//  corewindow = newcorewindow;
//  trackingcount = 0;
//  trackingarea = nil;
//  self.releasedWhenClosed = NO;
//  return self;
//}
//- (NACoreWindow*) corewindow{
//  return corewindow;
//}
//- (NSTrackingArea*) trackingarea{
//  return trackingarea;
//}
//- (NAUInt) trackingcount{
//  return trackingcount;
//}
//- (BOOL)windowShouldClose:(id)sender{
//  NA_UNUSED(sender);
//  naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE, NA_TRUE);
//  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_CLOSES);
//  NABool shouldClose = !naGetFlagi(corewindow->flags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
//  naSetFlagi(&(corewindow->flags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
//  return (BOOL)shouldClose;
//}
//- (void)setContentRect:(NARect)rect{
//  NSRect frame = [NSWindow frameRectForContentRect:naMakeNSRectWithRect(rect) styleMask:[self styleMask]];
//  [super setFrame:frame display:YES];
//  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_RESHAPE);
//}
//- (void)setWindowTitle:(const NAUTF8Char*) title{
//  [self setTitle:[NSString stringWithUTF8String:title]];
//}
//- (void)setKeepOnTop:(NABool) keepOnTop{
//  if(keepOnTop){
//    [self setLevel:NSFloatingWindowLevel];
//  }else{
//    [self setLevel:NSNormalWindowLevel];
//  }
//}
//- (void)renewMouseTracking{
//  trackingarea = [[NSTrackingArea alloc] initWithRect:[[self contentView] bounds]
//      options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
//      owner:self userInfo:nil];
//  [[self contentView] addTrackingArea:trackingarea];
//}
//- (void)clearMouseTracking{
//  [[self contentView] removeTrackingArea:trackingarea];
//  #if __has_feature(objc_arc)
//    // TrackingArea will be released automatically when ARC is turned on.
//  #else
//    [trackingarea release];
//  #endif
//  trackingarea = nil;
//}
//- (void)retainMouseTracking{
//  trackingcount++;
//  if(trackingcount == 1){
//    [self setAcceptsMouseMovedEvents:YES];
//    naRenewWindowMouseTracking(corewindow);
//  }
//}
//- (void)releaseMouseTracking{
//  trackingcount--;
//  if(trackingcount == 0){
//    [self setAcceptsMouseMovedEvents:NO];
//    naClearWindowMouseTracking(corewindow);
//  }
//}
//- (void)mouseMoved:(NSEvent*)event{
////  double deltaX = [event deltaX];
////  double deltaY = [event deltaY];
//  naSetMouseMovedByDiff([event deltaX], [event deltaY]);
//  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_MOUSE_MOVED);
////  [NSEvent setMouseCoalescingEnabled:NO];
//}
//- (void)mouseEntered:(NSEvent*)event{
//  NA_UNUSED(event);
//  naSetMouseEnteredAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
//  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_MOUSE_ENTERED);
//}
//- (void)mouseExited:(NSEvent*)event{
//  NA_UNUSED(event);
//  naSetMouseExitedAtPos(naMakePosWithNSPoint([NSEvent mouseLocation]));
//  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_MOUSE_EXITED);
//}
//- (void)keyUp:(NSEvent*)event{
//  NA_UNUSED(event);
//  naDispatchUIElementCommand((NACoreUIElement*)corewindow, NA_UI_COMMAND_KEYUP);
//}
//- (void)windowDidResize:(NSNotification *)notification{
//  NA_UNUSED(notification);
//  naRememberWindowPosition(corewindow);
//}
//- (void)windowDidMove:(NSNotification *)notification{
//  NA_UNUSED(notification);
//  naRememberWindowPosition(corewindow);
//}
//@end




NA_DEF NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, NABool resizeable, NAInt storageTag){
  DWORD exStyle;
  DWORD style;
  HWND hWnd;
  RECT windowrect;
  NARect screenrect;
  
  NAWINAPIWindow* winapiwindow = naAlloc(NAWINAPIWindow);

  rect = naSetWindowStorageTag(winapiwindow, storageTag, rect, resizeable);

  exStyle = WS_EX_CLIENTEDGE;
  style = WS_OVERLAPPEDWINDOW;
  if(!resizeable){style &= ~WS_THICKFRAME;}

  screenrect = naGetMainScreenRect();
  windowrect.top = (int)(screenrect.size.height - rect.pos.y - rect.size.height);
  windowrect.right = (int)(rect.pos.x + rect.size.width);
  windowrect.bottom = (int)(screenrect.size.height - rect.pos.y);
  windowrect.left = (int)rect.pos.x;
  AdjustWindowRect(&windowrect, style, NA_FALSE);

  TCHAR* systemtitle = naAllocSystemStringWithUTF8String(title);

	hWnd = CreateWindow(
		TEXT("NAWindow"), systemtitle, style,
		windowrect.left, windowrect.top, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top,
		NULL, NULL, naGetUIElementNativeID(naGetApplication()), NULL);

  NAString* iconPath = naNewApplicationIconPath();
  if(iconPath){
    NABabyImage* iconBabyImage = naCreateBabyImageFromFilePath(naGetStringUTF8Pointer(iconPath));
    HBITMAP bitmap = naAllocNativeImageWithBabyImage(iconBabyImage);
   
    HBITMAP hbmMask = CreateCompatibleBitmap(
      GetDC(NULL), 
      (int)naGetBabyImageSize(iconBabyImage).width,
      (int)naGetBabyImageSize(iconBabyImage).height);

    ICONINFO ii = {0};
    ii.fIcon    = TRUE;
    ii.hbmColor = bitmap;
    ii.hbmMask  = hbmMask;
    HICON hIcon = CreateIconIndirect(&ii);
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  }
  naDelete(iconPath);

  // todo: destroy the icon at the end of the windows lifetime.
  //DestroyIcon(hIcon);

  naFree(systemtitle);

  naInitCoreWindow(&(winapiwindow->corewindow), hWnd, NA_NULL, NA_FALSE, resizeable, rect);

  NASpace* space = naNewSpace(rect.size);
  naSetWindowContentSpace(winapiwindow, space);

  naSetUIElementParent(winapiwindow, naGetApplication());

  return (NAWindow*)winapiwindow;
}



NA_DEF void naDestructWindow(NAWindow* window){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  DestroyWindow(naGetUIElementNativeID(window));
  naReleaseUIElement(corewindow->contentspace);
  naClearCoreWindow(corewindow);
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  [nativewindow setWindowTitle:title];
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  [nativewindow setKeepOnTop:keepOnTop];
}



NA_DEF void naSetWindowRect(NAWindow* window, NARect rect){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;
  NARect currect = naGetUIElementRect(&(winapiwindow->corewindow.uielement), NA_NULL, NA_FALSE);
  if(!naEqualRect(currect, rect)){
    POINT testpoint = {0, 0};
    RECT clientrect;
    RECT windowrect;
    NARect screenrect = naGetMainScreenRect();
    GetClientRect(naGetUIElementNativeID(window), &clientrect);
    GetWindowRect(naGetUIElementNativeID(window), &windowrect);
    ClientToScreen(naGetUIElementNativeID(window), &testpoint);

    LONG leftdiff = (testpoint.x - windowrect.left);
    LONG topdiff =  (testpoint.y - windowrect.top);
    LONG rightdiff =  ((windowrect.right - windowrect.left) - (clientrect.right - clientrect.left) - leftdiff);
    LONG bottomdiff =  ((windowrect.bottom - windowrect.top) - (clientrect.bottom - clientrect.top) - topdiff);
    rect.pos.x -= leftdiff;
    rect.pos.y -= bottomdiff;
    rect.size.width += leftdiff;
    rect.size.height += bottomdiff;
    rect.size.width += rightdiff;
    rect.size.height += topdiff;

    MoveWindow(
      naGetUIElementNativeID(winapiwindow),
      (LONG)rect.pos.x,
      (LONG)(screenrect.size.height - rect.pos.y - rect.size.height),
      (LONG)rect.size.width,
      (LONG)rect.size.height,
      NA_FALSE);
  }
}



NA_DEF NAUIImageResolution naGetWindowUIResolution(NAWindow* window){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  CGFloat res = 1.;
//  if(NSAppKitVersionNumber >= NSAppKitVersionNumber10_7){
//    #ifdef __MAC_10_7
//      res = [nativewindow backingScaleFactor];
//    #endif
//  }else{
//    #ifndef __MAC_10_7
//      res = [nativewindow userSpaceScaleFactor];
//    #endif
//  }
//  return (res == 1.) ? NA_UIIMAGE_RESOLUTION_1x : NA_UIIMAGE_RESOLUTION_2x;
  return NA_UIIMAGE_RESOLUTION_1x;
}



NA_HDEF void naSetWindowFirstTabElement(NAWindow* window, NAUIElement* firstElem){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  naDefineNativeCocoaObject(NSView, nativefirst, firstElem);
//  [nativewindow setInitialFirstResponder:nativefirst];
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window){
  NARect rect;
  NARect screenrect;
  RECT clientrect;
  RECT windowrect;
  POINT testpoint = {0, 0};
  NACoreUIElement* corewindow;

  corewindow = (NACoreUIElement*)window;

  GetClientRect(corewindow->nativeID, &clientrect);
  GetWindowRect(corewindow->nativeID, &windowrect);
  ClientToScreen(corewindow->nativeID, &testpoint);

  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = (double)screenrect.size.height - testpoint.y - ((double)clientrect.bottom - (double)clientrect.top);
  rect.size.width = (double)clientrect.right - (double)clientrect.left;
  rect.size.height = (double)clientrect.bottom - (double)clientrect.top;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window){
  NARect rect;
  NARect screenrect;
  RECT windowrect;
  NACoreUIElement* corewindow;

  corewindow = (NACoreUIElement*)window;

  GetWindowRect(corewindow->nativeID, &windowrect);
  screenrect = naGetMainScreenRect();

  rect.pos.x = windowrect.left;
  rect.pos.y = screenrect.size.height - windowrect.bottom;
  rect.size.width = (double)windowrect.right - (double)windowrect.left;
  rect.size.height = (double)windowrect.bottom - (double)windowrect.top;

  return rect;
}



NA_HDEF void naRenewWindowMouseTracking(NACoreWindow* corewindow){
  //window->trackingarea = [[NSTrackingArea alloc] initWithRect:[[(NSWindow*)naGetUIElementNativeID(window) contentView] bounds]
  //    options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
  //    owner:(NSWindow*)naGetUIElementNativeID(window) userInfo:nil];
  //[[(NSWindow*)naGetUIElementNativeID(window) contentView] addTrackingArea:window->trackingarea];
}



NA_HDEF void naClearWindowMouseTracking(NACoreWindow* corewindow){
  //[[(NSWindow*)naGetUIElementNativeID(window) contentView] removeTrackingArea:window->trackingarea];
  //[window->trackingarea release];
  //window->trackingarea = nil;
}



NA_DEF void naClearWindow(NAWindow* window){
	DestroyWindow(naGetUIElementNativeID(window));
}



NA_DEF void naShowWindow(NAWindow* window){
  ShowWindow(naGetUIElementNativeID(window), SW_SHOW);
}



NA_DEF void naCloseWindow(NAWindow* window){
  ShowWindow(naGetUIElementNativeID(window), SW_HIDE);
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  corewindow->contentspace = (NACoreSpace*)uielement;
  naSetUIElementParent(uielement, window);
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  //DWORD style;
  //NABool hasbeenhandeled;
  //NARect newrect;
  //NARect screenrect;
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;

  //if(fullscreen != winapiwindow->fullscreen){
  //  winapiwindow->fullscreen = fullscreen;

  //  //HWND taskbar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
  //  //HWND startbutton = FindWindow(TEXT("Button"), NULL);

  //  screenrect = naGetMainScreenRect();

  //  if(fullscreen){
  //    DEVMODE screenSettings;
  //    winapiwindow->windowedframe = naGetUIElementRect(window, naGetApplication(), NA_TRUE);

  //    newrect = naGetMainScreenRect();

  //    memset(&screenSettings, 0, sizeof(screenSettings)); // set everything to 0
  //    screenSettings.dmSize = sizeof(screenSettings);
  //    //memcpy(screenSettings.dmDeviceName, fullscreendevicename, CCHDEVICENAME * sizeof(WCHAR));
  //    screenSettings.dmPelsWidth = (DWORD)newrect.size.width;
  //    screenSettings.dmPelsHeight = (DWORD)newrect.size.height;
  //    screenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

  //    style = WS_POPUP;
  //    SetWindowLongPtr(naGetUIElementNativeID(window), GWL_STYLE, style);
  //    SetWindowPos(naGetUIElementNativeID(window), HWND_TOPMOST, (int)screenrect.pos.x, (int)(screenrect.pos.y - screenrect.pos.y), (int)screenrect.size.width, (int)screenrect.size.height, SWP_SHOWWINDOW);
  //    //ChangeDisplaySettings(NULL, 0);
  //    ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
  //  }else{
  //    newrect = winapiwindow->windowedframe;
  //    style = WS_OVERLAPPEDWINDOW;
  //    SetWindowLongPtr(naGetUIElementNativeID(window), GWL_STYLE, style);
  //    SetWindowPos(naGetUIElementNativeID(window), HWND_NOTOPMOST, (int)winapiwindow->windowedframe.pos.x, (int)(screenrect.size.height - winapiwindow->windowedframe.pos.y), (int)winapiwindow->windowedframe.size.width, (int)winapiwindow->windowedframe.size.height, SWP_SHOWWINDOW);
  //    ChangeDisplaySettings(NULL, 0);
  //  }

  //  //hasbeenhandeled = naDispatchUIElementCommand(window, NA_UI_COMMAND_RESHAPE, &newrect);
  //  //if(hasbeenhandeled){naDispatchUIElementCommand(window, NA_UI_COMMAND_REDRAW, NA_NULL);}
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
