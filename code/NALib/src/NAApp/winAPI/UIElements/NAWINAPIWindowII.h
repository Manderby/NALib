
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#include "../../../NAApp/NAPreferences.h"



#define NA_WINAPI_WINDOW_KEEP_POS 0x0100
#if (NA_WINAPI_WINDOW_KEEP_POS < NA_WINDOW_FLAG_FIRST_SYSTEM_DEPENDENT_FLAG)
  #error "System specific flags overlap with core flags"
#endif

NAWINAPICallbackInfo naWindowWINAPIProc(
  void* uiElement,
  UINT message,
  WPARAM wParam,
  LPARAM lParam)
{
  #if NA_DEBUG
    if(!uiElement)
      naError("uiElement is nullptr");
  #endif

  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NAWINAPIWindow* windowMutable;
  NASpace* contentSpace;
  NARect contentRect;
  NABool shouldClose;
  NARect screenRect;
  double uiScale;
  double oldHeight;
  
  switch(message) {
  case WM_SHOWWINDOW:
    // We force the content space to redraw the proper background colors.
    windowMutable = (NAWINAPIWindow*)naGetUIElementWindowMutable(uiElement);
    contentSpace = naGetWindowContentSpace(&windowMutable->window);

    if(wParam) {
      na_ForceWINAPISpaceToRedrawBackground(&contentSpace->uiElement);
    }

    // wParam: true for show, false for hide
    // lParam: status of window
    // result: 0 when handeled.
    break;

  case WM_MOVE:
    // wParam: Unused
    // lParam: (int)(short)LOWORD: x coordinate, (int)(short)HIWORD: y coordinate
    // result: 0 when handeled.
    windowMutable = (NAWINAPIWindow*)naGetUIElementWindowMutable(uiElement);
    if(!naGetFlagu32(windowMutable->window.flags, NA_WINAPI_WINDOW_KEEP_POS)) {
      screenRect = naGetMainScreenRect();
      uiScale = naGetUIElementUIScale(NA_NULL);
      windowMutable->rect.pos.x = (double)LOWORD(lParam) / uiScale;
      windowMutable->rect.pos.y = screenRect.size.height - (double)HIWORD(lParam) / uiScale - windowMutable->rect.size.height;

      if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_RESHAPE)) {
        // no parent method to be called.
      }
      if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_REDRAW)) {
        // no parent method to be called.
      }
      na_RememberWindowPosition(&windowMutable->window);
      info.result = 0;
      //printf("move %f, %f\n", windowMutable->rect.pos.x, windowMutable->rect.pos.y);
      //printf("size %f, %f\n", windowMutable->rect.size.width, windowMutable->rect.size.height);
    }else{
      info.result = 0;
      info.hasBeenHandeled = NA_TRUE;
    }
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // lParam: LOWORD: width, HIWORD: height
    // result: 0 when handeled.
    windowMutable = (NAWINAPIWindow*)naGetUIElementWindowMutable(uiElement);
    uiScale = naGetUIElementUIScale(NA_NULL);
    oldHeight = windowMutable->rect.size.height;
    windowMutable->rect.size.width = (double)LOWORD(lParam) / uiScale;
    windowMutable->rect.size.height = (double)HIWORD(lParam) / uiScale;
    windowMutable->rect.pos.y -= (windowMutable->rect.size.height - oldHeight);

    // We need to trigger a resizing of the content space. The position does not change,
    // it is always (0,0).
    contentSpace = naGetWindowContentSpace(&windowMutable->window);
    contentRect = naMakeRectS(
      0.,
      0.,
      windowMutable->rect.size.width,
      windowMutable->rect.size.height);
    naSetUIElementRect(contentSpace, contentRect);

    if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_RESHAPE)) {
      // no parent method to be called.
    }
    if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_REDRAW)) {
      // no parent method to be called.
    }
    na_RememberWindowPosition(&windowMutable->window);
    info.result = 0;
    break;

  case WM_CLOSE:
    windowMutable = (NAWINAPIWindow*)naGetUIElementWindowMutable(uiElement);
    naSetFlagu32(&windowMutable->window.flags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE, NA_TRUE);
    if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_CLOSES)) {
      // no parent method to be called.
    }
    shouldClose = !naGetFlagu32(windowMutable->window.flags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING);
    naSetFlagu32(&windowMutable->window.flags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE | NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, NA_FALSE);
    if(shouldClose) {
      naCloseWindow(&windowMutable->window);
    }
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
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
    break;

  case WM_WINDOWPOSCHANGED:
    // //wParam: Unused
    // //lParam: pointer to WINDOWPOS structure
    // //result: 0 when handeled.
    // We let the default window procedure do its job which will emit the
    // WM_MOVE and WM_SIZE messages in turn.
    info.result = DefWindowProc(naGetUIElementNativePtr(uiElement), message, wParam, lParam);
    info.hasBeenHandeled = NA_TRUE;

    break;

  case WM_SYNCPAINT:
  case WM_DWMNCRENDERINGCHANGED:
  case WM_GETMINMAXINFO:
  case WM_IME_NOTIFY:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_GETFONT:
  case WM_SETFONT:
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
  case WM_PARENTNOTIFY:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_LBUTTONUP:
  case WM_CHAR:
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
  case WM_TIMER:
  case WM_NCDESTROY:
  case BM_SETSTATE:
  case 0xC0D6: // undocumented
  case WM_ENTERSIZEMOVE:
  case WM_MOVING:
  case WM_EXITSIZEMOVE:
    break;

  case WM_ERASEBKGND: // wParam: Device context, return != 0 if erasing, 0 otherwise
    // We pretend to have erased, although it will be the content space which
    // erases the background.
    info.result = 1;
    info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // labels and textFields to not display a cursor or selection.
    info = naUIElementWINAPIDefaultProc(naGetUIElementNativePtr(uiElement), message, wParam, lParam);
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



void naHandleWindowTabOrder(NAReaction reaction) {
  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)reaction.uiElement;
  if(winapiWindow->firstResponder) {
    SetFocus(naGetUIElementNativePtr(winapiWindow->firstResponder));
  }
}



NA_DEF NAWindow* naNewWindow(
  const NAUTF8Char* title,
  NARect rect,
  uint32 flags)
{
  NAWINAPIWindow* winapiWindow = naNew(NAWINAPIWindow);

  NABool resizeable = naGetFlagu32(flags, NA_WINDOW_RESIZEABLE);
  NABool titleless = naGetFlagu32(flags, NA_WINDOW_TITLELESS);
  NABool noncloseable = naGetFlagu32(flags, NA_WINDOW_NON_CLOSEABLE);
  NABool nonminiaturizeable = naGetFlagu32(flags, NA_WINDOW_NON_MINIATURIZEABLE);
  NABool auxiliary = naGetFlagu32(flags, NA_WINDOW_AUXILIARY);

  DWORD style = WS_OVERLAPPEDWINDOW;
  if(!resizeable) {
    style &= ~WS_THICKFRAME;
    style &= ~WS_MAXIMIZEBOX;
  }
  if(nonminiaturizeable) {
    style &= ~WS_MINIMIZEBOX;
  }
  if(!resizeable & nonminiaturizeable & noncloseable) {
    style &= ~WS_SYSMENU;
  }
  if(titleless) {
    style = WS_POPUPWINDOW;
  }
  if(auxiliary) {
    // todo
  }

  const NAScreen* screen = naGetApplicationScreenWithPos(naGetRectCenter(rect));
  if(screen) {
    NARect screenRect = na_GetScreenRect(&screen->uiElement);
    rect.pos.x -= screenRect.pos.x;
    rect.pos.y -= screenRect.pos.y;
  }

  winapiWindow->rect = rect;
	double uiScale = naGetUIElementUIScale(NA_NULL);

  NARect screenRect = naGetMainScreenRect();

  RECT winapiRect;
  winapiRect.top = (LONG)((screenRect.size.height - rect.pos.y - rect.size.height) * uiScale);
  winapiRect.right = (LONG)((rect.pos.x + rect.size.width) * uiScale);
  winapiRect.bottom = (LONG)((screenRect.size.height - rect.pos.y) * uiScale);
  winapiRect.left = (LONG)(rect.pos.x * uiScale);
  AdjustWindowRect(&winapiRect, style, NA_FALSE);

  TCHAR* systemTitle = naAllocSystemStringWithUTF8String(title);

	HWND nativePtr = CreateWindow(
		TEXT("NAWindow"),
    systemTitle,
    style,
    winapiRect.left,
    winapiRect.top,
    winapiRect.right - winapiRect.left,
    winapiRect.bottom - winapiRect.top,
		NULL,
    NULL,
    naGetUIElementNativePtr(naGetApplication()),
    NULL);

  if(noncloseable) {
    EnableMenuItem(GetSystemMenu(nativePtr, FALSE), SC_CLOSE,
      MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
  }

  HICON hIcon = na_GetWINAPIApplicationIcon();
  if(hIcon) {   
    SendMessage(nativePtr, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(nativePtr, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  }

  naFree(systemTitle);

  NAScreen* actualScreen = na_GetApplicationScreenWithNativePtr(MonitorFromWindow(nativePtr, MONITOR_DEFAULTTONEAREST));

  na_InitWindow(
    &winapiWindow->window,
    nativePtr,
    actualScreen,
    NA_NULL,
    NA_FALSE,
    resizeable,
    rect);
  winapiWindow->firstResponder = NA_NULL;
  winapiWindow->window.flags = flags;

  naAddUIKeyboardShortcut(
    winapiWindow,
    naNewKeyStroke(NA_KEYCODE_TAB, NA_KEY_MODIFIER_NONE),
    naHandleWindowTabOrder,
    NA_NULL);
  naAddUIKeyboardShortcut(
    winapiWindow,
    naNewKeyStroke(NA_KEYCODE_TAB, NA_KEY_MODIFIER_SHIFT),
    naHandleWindowTabOrder,
    NA_NULL);

  NASpace* contentSpace = naNewSpace(naMakeSize(rect.size.width, rect.size.height));
  if(titleless) {
    naSetSpaceDragsWindow(contentSpace, NA_TRUE);
  }

  naSetWindowContentSpace(&winapiWindow->window, contentSpace);

  return (NAWindow*)winapiWindow;
}



NA_DEF void na_DestructWINAPIWindow(NAWINAPIWindow* winapiWindow) {
  na_ClearWindow((NAWindow*)winapiWindow);
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, void* space) {
  #if NA_DEBUG
    if(!window)
      naCrash("window is nullptr");
    if(!space)
      naCrash("space is nullptr");
  #endif

  #if NA_DEBUG
    if((naGetUIElementType(space) != NA_UI_SPACE) &&
      (naGetUIElementType(space) != NA_UI_IMAGE_SPACE) &&
      (naGetUIElementType(space) != NA_UI_OPENGL_SPACE) &&
      (naGetUIElementType(space) != NA_UI_METAL_SPACE))
      naError("Require a space, not any arbitrary ui element.");
  #endif

  if(space == window->contentSpace) {
    #if NA_DEBUG
      naError("Setting the same content space again. Nothing will happen");
    #endif
    return;
  }

  if(window->contentSpace) { naDelete(window->contentSpace); }
  window->contentSpace = space;

  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
  naSetFlagu32(&winapiWindow->window.flags, NA_WINAPI_WINDOW_KEEP_POS, NA_TRUE);
  na_SetUIElementParent(space, window, NA_TRUE);
  naSetFlagu32(&winapiWindow->window.flags, NA_WINAPI_WINDOW_KEEP_POS, NA_FALSE);
}



NA_DEF void naShowWindow(const NAWindow* window) {
  ShowWindow(naGetUIElementNativePtrConst(window), SW_SHOW);
  BringWindowToTop(naGetUIElementNativePtrConst(window));
}

NA_DEF void naShowWindowModal(NAWindow* window, NAWindow* parentWindow) {
  NA_UNUSED(parentWindow);
  // todo: modal windows do not work yet on windows
  //EnableWindow(naGetUIElementNativePtr(parentWindow), FALSE);
  naShowWindow(window);
}

NA_DEF void naCloseWindowModal(NAWindow* window) {
  // todo: modal windows do not work yet on windows
  //EnableWindow(naGetUIElementNativePtr(parentWindow), TRUE);
  naCloseWindow(window);
}



NA_DEF void naCloseWindow(const NAWindow* window) {
  ShowWindow(naGetUIElementNativePtrConst(window), SW_HIDE);
}



NA_DEF void naMarkWindowChanged(NAWindow* window, NABool changed) {
  NA_UNUSED(changed);
  NA_UNUSED(window);
  // todo
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullScreen) {
  #if NA_DEBUG
    if(!window)
      naCrash("window is nullptr");
  #endif

  DWORD style;
  NARect newRect;
  NARect screenRect;
  double uiScale = naGetUIElementUIScale(NA_NULL);

  if(fullScreen != naIsWindowFullscreen(window)) {
    screenRect = naGetMainScreenRect();
    if(fullScreen) {
      DEVMODE screenSettings;
      window->windowedFrame = naGetUIElementRectAbsolute(window);

      newRect = naGetMainScreenRect();

      memset(&screenSettings, 0, sizeof(screenSettings)); // set everything to 0
      screenSettings.dmSize = sizeof(screenSettings);
      //memcpy(screenSettings.dmDeviceName, fullscreendevicename, CCHDEVICENAME * sizeof(WCHAR));
      screenSettings.dmPelsWidth = (DWORD)newRect.size.width;
      screenSettings.dmPelsHeight = (DWORD)newRect.size.height;
      screenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

      style = WS_POPUP;
      SetWindowLongPtr(naGetUIElementNativePtr(window), GWL_STYLE, style);
      SetWindowPos(
        naGetUIElementNativePtr(window),
        HWND_TOPMOST,
        (LONG)(screenRect.pos.x * uiScale),
        (LONG)((screenRect.pos.y - screenRect.pos.y) * uiScale),
        (LONG)(screenRect.size.width * uiScale),
        (LONG)(screenRect.size.height * uiScale),
        SWP_SHOWWINDOW);
      // Commented out for future use. Note: Incorporate resolution depencence zoom.
      //ChangeDisplaySettings(NULL, 0);
      //ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
    }else{
      newRect = window->windowedFrame;
      style = WS_OVERLAPPEDWINDOW;
      SetWindowLongPtr(naGetUIElementNativePtr(window), GWL_STYLE, style);
      SetWindowPos(
        naGetUIElementNativePtr(window),
        HWND_NOTOPMOST,
        (LONG)(window->windowedFrame.pos.x * uiScale),
        (LONG)((screenRect.size.height - window->windowedFrame.pos.y - window->windowedFrame.size.height) * uiScale),
        (LONG)(window->windowedFrame.size.width * uiScale),
        (LONG)(window->windowedFrame.size.height * uiScale),
        SWP_SHOWWINDOW);
      //ChangeDisplaySettings(NULL, 0);
    }

    naSetFlagu32(&window->flags, NA_CORE_WINDOW_FLAG_FULLSCREEN, fullScreen);
  }
}



NA_DEF void naSetWindowTitle(NAWindow* window, const NAUTF8Char* title) {
  TCHAR* systemTitle = naAllocSystemStringWithUTF8String(title);
  SetWindowText(naGetUIElementNativePtr(window), systemTitle);
  naFree(systemTitle);
}



//NA_DEF void naSetWindowRect(NAWindow* window, NARect rect) {
//  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
//}



NA_DEF void naKeepWindowOnTop(NAWindow* window, NABool keepOnTop) {
  if(keepOnTop) {
    SetWindowPos(naGetUIElementNativePtr(window), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  }else{
    SetWindowPos(naGetUIElementNativePtr(window), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  }
}



NA_DEF void naSetWindowAcceptsKeyboardReactions(NAWindow* window, NABool accepts) {
  NA_UNUSED(accepts);
  NA_UNUSED(window);
  // todo
}



NA_DEF void* naGetWindowFirstTabElement(NAWindow* window) {
  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
  return winapiWindow->firstResponder;
}



NA_DEF void naSetWindowFirstTabElement(NAWindow* window, void* firstTabElem) {
  #if NA_DEBUG
    if(!window)
      naCrash("window is nullptr");
    if(firstTabElem && naGetUIElementWindow(firstTabElem) != window)
      naError("Element is not part of the window.");
  #endif

  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
  winapiWindow->firstResponder = firstTabElem;
}



NA_HDEF NARect na_GetWindowAbsoluteInnerRect(const NA_UIElement* window) {
  NARect rect;
  NARect screenRect;
  RECT clientRect;
  POINT testPoint = {0, 0};

  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;
  double uiScale = naGetUIElementUIScale(NA_NULL);

  GetClientRect(window->nativePtr, &clientRect);
  ClientToScreen(window->nativePtr, &testPoint);

  screenRect = naGetMainScreenRect();

  rect.pos.x = testPoint.x / uiScale;
  rect.pos.y = (double)screenRect.size.height - (double)testPoint.y / uiScale - ((double)clientRect.bottom - (double)clientRect.top) / uiScale;
  rect.size = winapiWindow->rect.size;
  return rect;
}



NA_HAPI NARect na_GetWindowRect(const NA_UIElement* window)
{
  const NAWINAPIWindow* winapiWindow = (const NAWINAPIWindow*)window;
  return winapiWindow->rect;
}

static NARect na_convertInnerToOuterRect(const NAWINAPIWindow* winapiWindow, NARect rect, NARect screenRect) {
  NARect outerRect = rect;
  RECT clientRect;
  RECT windowRect;
  POINT testPoint = {0, 0};
  double uiScale = naGetUIElementUIScale(NA_NULL);

  HWND nativePtr = naGetUIElementNativePtrConst(&winapiWindow->window);
  GetClientRect(nativePtr, &clientRect);
  GetWindowRect(nativePtr, &windowRect);
  ClientToScreen(nativePtr, &testPoint);

  LONG leftdiff = (testPoint.x - windowRect.left);
  LONG topdiff =  (testPoint.y - windowRect.top);
  LONG rightdiff =  (windowRect.right - (testPoint.x + clientRect.right));
  LONG bottomdiff =  (windowRect.bottom - (testPoint.y + clientRect.bottom));
  outerRect.pos.x -= leftdiff / uiScale;
  outerRect.pos.y -= bottomdiff / uiScale;
  outerRect.size.width += (leftdiff + rightdiff) / uiScale;
  outerRect.size.height += (bottomdiff + topdiff) / uiScale;

  return outerRect;
}

NA_HDEF void na_SetWindowRect(NA_UIElement* window, NARect rect) {
  NAWINAPIWindow* winapiWindow = (NAWINAPIWindow*)window;

  NARect screenRect = naGetMainScreenRect();
  winapiWindow->rect = rect;
  double uiScale = naGetUIElementUIScale(NA_NULL);

  NARect outerRect = na_convertInnerToOuterRect(winapiWindow, rect, screenRect);

  MoveWindow(
    naGetUIElementNativePtr(winapiWindow),
    (LONG)(outerRect.pos.x * uiScale),
    (LONG)((screenRect.size.height - outerRect.pos.y - outerRect.size.height) * uiScale),
    (LONG)(outerRect.size.width * uiScale),
    (LONG)(outerRect.size.height * uiScale),
    NA_FALSE);

  na_UpdateMouseTracking(&winapiWindow->window.uiElement);

  // We need to trigger a resizing of the content space. The position does not change,
  // it is always (0,0).
  NASpace* contentSpace = naGetWindowContentSpace(&winapiWindow->window);
  NARect contentRect = naMakeRectS(
    0.,
    0.,
    rect.size.width,
    rect.size.height);
  naSetUIElementRect(contentSpace, contentRect);
}


NA_DEF NARect naGetWindowOuterRect(const NAWindow* window) {
  const NAWINAPIWindow* winapiWindow = (const NAWINAPIWindow*)window;

  NARect screenRect = naGetMainScreenRect();
  return na_convertInnerToOuterRect(winapiWindow, winapiWindow->rect, screenRect);
}

NA_DEF void naSetWindowOuterRect(NAWindow* window, NARect rect) {
  //naDefineCocoaObject(NACocoaNativeWindow, nativePtr, window);
  //[nativePtr setFrame:naMakeNSRectWithRect(rect)];
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
