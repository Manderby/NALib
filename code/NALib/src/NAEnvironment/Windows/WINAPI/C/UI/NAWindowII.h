
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#include "../../../../../NAPreferences.h"

typedef struct NAWINAPIWindow NAWINAPIWindow;
struct NAWINAPIWindow {
  NA_Window coreWindow;
  NAUIElement* firstResponder;
  //NAUInt trackingcount;
  //NABounds4 bounds;
};



NAWINAPICallbackInfo naWindowWINAPIProc(NAUIElement* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NA_Window* coreWindow;
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
    coreWindow = (NA_Window*)naGetUIElementWindow(uiElement);
    info.hasbeenhandeled = na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_RESHAPE);
    if (info.hasbeenhandeled) { na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_REDRAW); }
    na_RememberWindowPosition(coreWindow);
    info.result = 0;
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // lParam: LOWORD: width, HIWORD: height
    // result: 0 when handeled.
    coreWindow = (NA_Window*)naGetUIElementWindow(uiElement);
    info.hasbeenhandeled = na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_RESHAPE);
    if (info.hasbeenhandeled) { na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_REDRAW); }
    na_RememberWindowPosition(coreWindow);
    info.result = 0;
    break;

  case WM_CLOSE:
    coreWindow = (NA_Window*)naGetUIElementWindow(uiElement);
    na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_CLOSES);
    shouldClose = !naGetFlagi(coreWindow->flags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
    naSetFlagi(&(coreWindow->flags), NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
    if(shouldClose){naCloseWindow(coreWindow);}
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
  case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
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

  // Dark mode stuff. Commented out for the future.
  //case WM_DWMCOLORIZATIONCOLORCHANGED:
  //  DwmGetColorizationColor(&colorization, &lParam);
  //  info.result = 0;
  //  break;
  //case WM_THEMECHANGED :
  //case WM_WININICHANGE:
  //  "hello dark mode";
  //  break;

  default:
    //printf("Uncaught Window message: %X" NA_NL, (int)message);
    //info.result = 0;
    break;
  }
  
  return info;
}



NABool naHandleWindowTabOrder(NAReaction reaction){
  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)reaction.uiElement;
  if(winapiWindow->firstResponder){
    SetFocus(naGetUIElementNativeID(winapiWindow->firstResponder));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NAWindow* naNewWindow(const NAUTF8Char* title, NARect rect, NABool resizeable, NAInt storageTag){
  DWORD style;
  HWND hWnd;
  RECT windowrect;
  NARect screenrect;
  TCHAR* systemtitle;
  HICON hIcon;
  NASpace* space;
  
  NAWINAPIWindow* winapiWindow = naAlloc(NAWINAPIWindow);

  rect = naSetWindowStorageTag(winapiWindow, storageTag, rect, resizeable);

  style = WS_OVERLAPPEDWINDOW;
  if(!resizeable){
    style &= ~WS_THICKFRAME;
    style &= ~WS_MAXIMIZEBOX;
  }

  screenrect = naGetMainScreenRect();
  windowrect.top = (int)(screenrect.size.height - rect.pos.y - rect.size.height);
  windowrect.right = (int)(rect.pos.x + rect.size.width);
  windowrect.bottom = (int)(screenrect.size.height - rect.pos.y);
  windowrect.left = (int)rect.pos.x;
  AdjustWindowRect(&windowrect, style, NA_FALSE);

  systemtitle = naAllocSystemStringWithUTF8String(title);

	hWnd = CreateWindow(
		TEXT("NAWindow"), systemtitle, style,
		windowrect.left, windowrect.top, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top,
		NULL, NULL, naGetUIElementNativeID(naGetApplication()), NULL);

  hIcon = naGetWINAPIApplicationIcon();
  if(hIcon){   
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  }

  naFree(systemtitle);

  na_InitWindow(&(winapiWindow->coreWindow), hWnd, NA_NULL, NA_FALSE, resizeable, rect);
  winapiWindow->firstResponder = NA_NULL;

  naAddUIKeyboardShortcut(winapiWindow, naMakeKeybardStatus(0, NA_KEYCODE_TAB), naHandleWindowTabOrder, NA_NULL);
  naAddUIKeyboardShortcut(winapiWindow, naMakeKeybardStatus(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB), naHandleWindowTabOrder, NA_NULL);

  space = naNewSpace(rect.size);
  naSetWindowContentSpace(winapiWindow, space);

  na_SetUIElementParent(winapiWindow, naGetApplication());

  return (NAWindow*)winapiWindow;
}



NA_DEF void na_DestructWindow(NAWindow* window){
  NA_Window* coreWindow = (NA_Window*)window;
  DestroyWindow(naGetUIElementNativeID(window));
  naReleaseUIElement(coreWindow->contentspace);
  na_ClearWindow(coreWindow);
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title){
  NA_Window* coreWindow = (NA_Window*)window;
  TCHAR* systemtitle = naAllocSystemStringWithUTF8String(title);
  SetWindowText(naGetUIElementNativeID(window), systemtitle);
  naFree(systemtitle);
}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop){
  if(keepOnTop){
    SetWindowPos(naGetUIElementNativeID(window), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  }else{
    SetWindowPos(naGetUIElementNativeID(window), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  }
}



NA_DEF void naSetWindowRect(NAWindow* window, NARect rect){
  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
  NARect currect = naGetUIElementRect(&(winapiWindow->coreWindow.uiElement), NA_NULL, NA_FALSE);
  if(!naEqualRect(currect, rect)){
    POINT testpoint = {0, 0};
    RECT clientrect;
    RECT windowrect;
    LONG leftdiff;
    LONG topdiff;
    LONG rightdiff;
    LONG bottomdiff;

    NARect screenrect = naGetMainScreenRect();
    GetClientRect(naGetUIElementNativeID(window), &clientrect);
    GetWindowRect(naGetUIElementNativeID(window), &windowrect);
    ClientToScreen(naGetUIElementNativeID(window), &testpoint);

    leftdiff = (testpoint.x - windowrect.left);
    topdiff =  (testpoint.y - windowrect.top);
    rightdiff =  ((windowrect.right - windowrect.left) - (clientrect.right - clientrect.left) - leftdiff);
    bottomdiff =  ((windowrect.bottom - windowrect.top) - (clientrect.bottom - clientrect.top) - topdiff);
    rect.pos.x -= leftdiff;
    rect.pos.y -= bottomdiff;
    rect.size.width += leftdiff;
    rect.size.height += bottomdiff;
    rect.size.width += rightdiff;
    rect.size.height += topdiff;

    MoveWindow(
      naGetUIElementNativeID(winapiWindow),
      (LONG)rect.pos.x,
      (LONG)(screenrect.size.height - rect.pos.y - rect.size.height),
      (LONG)rect.size.width,
      (LONG)rect.size.height,
      NA_FALSE);
  }
}



NA_DEF NAUIImageResolution naGetWindowUIResolution(NAWindow* window){
  // Currently, NALib for windows GUI is not resolution aware. Be patient.
  return NA_UIIMAGE_RESOLUTION_1x;
}



NA_DEF void naSetWindowFirstTabElement(NAWindow* window, NAUIElement* firstTabElem){
  NAWINAPIWindow* winapiWindow;
  
  #ifndef NDEBUG
    if(naGetUIElementWindow(firstTabElem) != window)
      naError("Element is not part of the window.");
  #endif
  winapiWindow = (NAWINAPIWindow*)window;
  winapiWindow->firstResponder = firstTabElem;
}



NA_DEF NAUIElement* naGetWindowFirstTabElement(NAWindow* window){
  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
  return winapiWindow->firstResponder;
}



NA_HDEF NARect na_GetWindowAbsoluteInnerRect(NA_UIElement* window){
  NARect rect;
  NARect screenrect;
  RECT clientrect;
  RECT windowrect;
  POINT testpoint = {0, 0};
  NA_UIElement* coreWindow;

  coreWindow = (NA_UIElement*)window;

  GetClientRect(coreWindow->nativeID, &clientrect);
  GetWindowRect(coreWindow->nativeID, &windowrect);
  ClientToScreen(coreWindow->nativeID, &testpoint);

  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = (double)screenrect.size.height - testpoint.y - ((double)clientrect.bottom - (double)clientrect.top);
  rect.size.width = (double)clientrect.right - (double)clientrect.left;
  rect.size.height = (double)clientrect.bottom - (double)clientrect.top;
  return rect;
}



NA_HDEF NARect na_GetWindowAbsoluteOuterRect(NA_UIElement* window){
  NARect rect;
  NARect screenrect;
  RECT windowrect;
  NA_UIElement* coreWindow;

  coreWindow = (NA_UIElement*)window;

  GetWindowRect(coreWindow->nativeID, &windowrect);
  screenrect = naGetMainScreenRect();

  rect.pos.x = windowrect.left;
  rect.pos.y = screenrect.size.height - windowrect.bottom;
  rect.size.width = (double)windowrect.right - (double)windowrect.left;
  rect.size.height = (double)windowrect.bottom - (double)windowrect.top;

  return rect;
}



NA_DEF void naClearWindow(NAWindow* window){
	DestroyWindow(naGetUIElementNativeID(window));
}



NA_DEF void naShowWindow(NAWindow* window){
  ShowWindow(naGetUIElementNativeID(window), SW_SHOW);
  BringWindowToTop(naGetUIElementNativeID(window));
}



NA_DEF void naCloseWindow(NAWindow* window){
  ShowWindow(naGetUIElementNativeID(window), SW_HIDE);
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, NAUIElement* uiElement){
  NA_Window* coreWindow = (NA_Window*)window;
  coreWindow->contentspace = (NA_Space*)uiElement;
  na_SetUIElementParent(uiElement, window);
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  DWORD style;
  NARect newrect;
  NARect screenrect;
  NA_Window* coreWindow = (NA_Window*)window;
  
  if(fullscreen != naIsWindowFullscreen(window)){
    screenrect = naGetMainScreenRect();
    if(fullscreen){
      DEVMODE screenSettings;
      coreWindow->windowedFrame = naGetUIElementRect(window, naGetApplication(), NA_TRUE);

      newrect = naGetMainScreenRect();

      memset(&screenSettings, 0, sizeof(screenSettings)); // set everything to 0
      screenSettings.dmSize = sizeof(screenSettings);
      //memcpy(screenSettings.dmDeviceName, fullscreendevicename, CCHDEVICENAME * sizeof(WCHAR));
      screenSettings.dmPelsWidth = (DWORD)newrect.size.width;
      screenSettings.dmPelsHeight = (DWORD)newrect.size.height;
      screenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

      style = WS_POPUP;
      SetWindowLongPtr(naGetUIElementNativeID(window), GWL_STYLE, style);
      SetWindowPos(naGetUIElementNativeID(window), HWND_TOPMOST, (int)screenrect.pos.x, (int)(screenrect.pos.y - screenrect.pos.y), (int)screenrect.size.width, (int)screenrect.size.height, SWP_SHOWWINDOW);
      // Commented out for future use. Note: Incorporate resolution depencence zoom.
      //ChangeDisplaySettings(NULL, 0);
      //ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
    }else{
      newrect = coreWindow->windowedFrame;
      style = WS_OVERLAPPEDWINDOW;
      SetWindowLongPtr(naGetUIElementNativeID(window), GWL_STYLE, style);
      SetWindowPos(
        naGetUIElementNativeID(window),
        HWND_NOTOPMOST,
        (int)coreWindow->windowedFrame.pos.x,
        (int)(screenrect.size.height - coreWindow->windowedFrame.pos.y - coreWindow->windowedFrame.size.height),
        (int)coreWindow->windowedFrame.size.width,
        (int)coreWindow->windowedFrame.size.height,
        SWP_SHOWWINDOW);
      //ChangeDisplaySettings(NULL, 0);
    }

    naSetFlagi(&(coreWindow->flags), NA_CORE_WINDOW_FLAG_FULLSCREEN, fullscreen);
  }
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
