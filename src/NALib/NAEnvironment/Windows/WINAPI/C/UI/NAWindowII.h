
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
  NAUInt trackingcount;
  //NABounds4 bounds;
};



NABool naWindowWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWindow* window;
  //NARect rect;
  //NASize size;
  //NAPos pos;
  //const NAMouseStatus* mousestatus;
  NABool hasbeenhandeled = NA_FALSE;

  switch(message){
  case WM_SHOWWINDOW:
    // wParam: true for show, false for hide
    // lParam: status of window
    break;

  case WM_MOVE:
    // wParam: Unused
    // lParam: LOWORD: x coordinate, HIWORD: y coordinate
    window = naGetUIElementWindow(uielement);
    //rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    //rect.pos.x = (double)((int)(short)LOWORD(lParam));
    //rect.pos.y = (double)((int)(short)HIWORD(lParam));
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE);
    if (hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW); }
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // lParam: LOWORD: width, HIWORD: height
    window = naGetUIElementWindow(uielement);
    //rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    //rect.size.width = LOWORD(lParam);
    //rect.size.height = HIWORD(lParam);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE);
    if (hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW); }
    break;

  //case WM_MOUSEMOVE:
  //  // wParam: several special keys
  //  // GET_X_LPARAM(lParam): x coord relative to top left
  //  // GET_Y_LPARAM(lParam): y coord relative to top left
  //  window = naGetUIElementWindow(uielement);
  //  //if(window->flags & CUB_WINDOW_IGNORE_MOUSE_WARP){
  //  //  window->flags &= ~CUB_WINDOW_IGNORE_MOUSE_WARP;
  //  //  hasbeenhandeled = NA_TRUE;
  //  //}else{
  //  // todo: this should be GET_X_LPARAM and GET_Y_LPARAM
  //  // but is undefined somehow.
  //  size.width = LOWORD(lParam);
  //  size.height = HIWORD(lParam);
  //  rect = naGetUIElementRect(uielement, naGetApplication(), NA_FALSE);
  //  size.width += rect.pos.x;
  //  size.height += rect.pos.y;
  //  mousestatus = naGetMouseStatus();
  //  pos = naGetMousePos(mousestatus);
  //  naSetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);
  //  hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_MOUSE_MOVED);
  //  //}
  //  break;


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
  case WM_SETFOCUS:
    break;

  default:
    //printf("Uncaught Window message: %X\n", (int)message);
    break;
  }
  
  return hasbeenhandeled;
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



//#define NA_WINDOW_PREF_STRING_POS_X "NAWindow_%" NA_PRIi "_Pos_x"
//#define NA_WINDOW_PREF_STRING_POS_Y "NAWindow_%" NA_PRIi "_Pos_y"
//#define NA_WINDOW_PREF_STRING_SIZE_WIDTH "NAWindow_%" NA_PRIi "_Size_Width"
//#define NA_WINDOW_PREF_STRING_SIZE_HEIGHT "NAWindow_%" NA_PRIi "_Size_Height"



NA_DEF NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, NABool resizeable, NAInt storageTag){
  DWORD exStyle;
  DWORD style;
  HWND hWnd;
  RECT windowrect;
  NARect screenrect;
  
  NAWINAPIWindow* winapiwindow = naAlloc(NAWINAPIWindow);

  //corewindow->storageTag = storageTag;
  //if(corewindow->storageTag){
  //  NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, corewindow->storageTag);
  //  NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, corewindow->storageTag);
  //  rect.pos.x = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
  //  rect.pos.y = naInitPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
  //  naDelete(prefPosXString);
  //  naDelete(prefPosYString);
  //  if(resizeable){
  //    NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, corewindow->storageTag);
  //    NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, corewindow->storageTag);
  //    rect.size.width = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
  //    rect.size.height = naInitPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
  //    naDelete(prefSizeWidthString);
  //    naDelete(prefSizeHeightString);
  //  }
  //}

  exStyle = WS_EX_CLIENTEDGE;
  style = WS_OVERLAPPEDWINDOW;
  if(!resizeable){style &= ~WS_THICKFRAME;}
  screenrect = naGetMainScreenRect();

  windowrect.top = (int)(screenrect.size.height - rect.pos.y - rect.size.height);
  windowrect.right = (int)(rect.pos.x + rect.size.width);
  windowrect.bottom = (int)(screenrect.size.height - rect.pos.y);
  windowrect.left = (int)rect.pos.x;
  AdjustWindowRect(&windowrect, style, NA_FALSE);

	hWnd = CreateWindow(
		TEXT("NAWindow"), title, style,
		windowrect.left, windowrect.top, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top,
		NULL, NULL, naGetUIElementNativeID(naGetApplication()), NULL);

  //DWORD lasterror = GetLastError();
	//hDC = GetDC(hWnd);

  naInitCoreWindow(&(winapiwindow->corewindow), hWnd, NA_NULL, NA_FALSE, resizeable, rect);
  winapiwindow->trackingcount = 0;

  NARect contentFrame = naMakeRect(naMakePos(0., 0.), rect.size);
  NASpace* space = naNewSpace(contentFrame);
  naSetWindowContentSpace(winapiwindow, space);

  naSetUIElementParent(winapiwindow, naGetApplication());

  return (NAWindow*)winapiwindow;
}



NA_HDEF void naRememberWindowPosition(NACoreWindow* corewindow){
//  NARect rect = naGetWindowAbsoluteInnerRect(&(corewindow->uielement));
//  NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, corewindow->storageTag);
//  NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, corewindow->storageTag);
//  naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
//  naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
//  naDelete(prefPosXString);
//  naDelete(prefPosYString);
//  if(naIsWindowResizeable(corewindow)){
//    NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, corewindow->storageTag);
//    NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, corewindow->storageTag);
//    naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
//    naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
//    naDelete(prefSizeWidthString);
//    naDelete(prefSizeHeightString);
//  }
}



NA_DEF void naDestructWindow(NAWindow* window){
//  NACoreWindow* corewindow = (NACoreWindow*)window;
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  [nativewindow close];
//  naReleaseUIElement(corewindow->contentspace);
//  naClearCoreWindow(corewindow);
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
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  [nativewindow setContentRect:rect];
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
  RECT contentrect;
  POINT testpoint = {0, 0};
  NACoreUIElement* corewindow;

  corewindow = (NACoreUIElement*)window;

  GetClientRect(corewindow->nativeID, &contentrect);
  ClientToScreen(corewindow->nativeID, &testpoint);

  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = screenrect.size.height - ((double)testpoint.y + (contentrect.bottom - contentrect.top));
  rect.size.width = (double)contentrect.right - (double)contentrect.left;
  rect.size.height = (double)contentrect.bottom - (double)contentrect.top;
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



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;
  winapiwindow->trackingcount++;
  //if(winapiwindow->trackingcount == 1){
  //  [(NSWindow*)naGetUIElementNativeID(window) setAcceptsMouseMovedEvents:YES];
  //  naRenewWindowMouseTracking(window);
  //}
}



NA_HDEF void naReleaseWindowMouseTracking(NACoreWindow* corewindow){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)corewindow;
  winapiwindow->trackingcount--;
  //if(winapiwindow->trackingcount == 0){
  //  [(NSWindow*)naGetUIElementNativeID(window) setAcceptsMouseMovedEvents:NO];
  //  naClearWindowMouseTracking(window);
  //}
}



NA_DEF void naClearWindow(NAWindow* window){
	DestroyWindow(naGetUIElementNativeID(window));
}



NA_DEF void naShowWindow(NAWindow* window){
  ShowWindow(naGetUIElementNativeID(window), SW_SHOW);
}



NA_DEF void naCloseWindow(NAWindow* window){
//  naDefineNativeCocoaObject(NANativeWindow, nativewindow, window);
//  [nativewindow performClose:NA_NULL];
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uielement){
  NACoreWindow* corewindow = (NACoreWindow*)window;
  //naAddListLastMutable(&(corewindow->uielement.childs), uielement); // todo: this is a hack just for now.
  //NAUIElement* element = (NAUIElement*)uielement;
  //[(NANativeWindow*)(window->uielement.nativeID) setContentView:element->nativeID];

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
