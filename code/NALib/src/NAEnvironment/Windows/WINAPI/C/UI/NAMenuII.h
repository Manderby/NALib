
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPIMenu NAWINAPIMenu;
struct NAWINAPIMenu {
  NAMenu   menu;
};

NA_HAPI void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu);
NA_RUNTIME_TYPE(NAWINAPIMenu, na_DestructWINAPIMenu, NA_FALSE);



typedef struct NAWINAPIMenuItem NAWINAPIMenuItem;
struct NAWINAPIMenuItem {
  NAMenuItem   menuItem;
};

NA_HAPI void na_DestructWINAPIMenuItem(NAWINAPIMenuItem* winapiMenuItem);
NA_RUNTIME_TYPE(NAWINAPIMenuItem, na_DestructWINAPIMenuItem, NA_FALSE);



NAWINAPICallbackInfo naMenuWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  //case WM_SETFONT:
  //case WM_STYLECHANGING:
  //case WM_WINDOWPOSCHANGING:
  //case WM_CHILDACTIVATE:
  //case WM_MOVE:
  //case WM_SHOWWINDOW:
  //case WM_STYLECHANGED:
  //case WM_WINDOWPOSCHANGED:
  //case WM_SETTEXT:
  //case WM_PAINT:
  //case WM_NCPAINT:
  //case WM_GETFONT:
  //case WM_DESTROY:
  //case WM_NCDESTROY:
  //case WM_NCHITTEST:
  //case WM_SETCURSOR:
  //case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  //case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  //case WM_MOUSEACTIVATE:
  //case WM_LBUTTONDOWN:
  //case WM_IME_SETCONTEXT:
  //case WM_SETFOCUS:
  //case WM_CANCELMODE:
  //case WM_CAPTURECHANGED:
  //case WM_KILLFOCUS:
  //case WM_IME_NOTIFY:
  //case WM_LBUTTONUP:
  //  break;

  //case WM_ERASEBKGND:
  //  info.hasBeenHandeled = NA_TRUE;
  //  info.result = 1;
  //  break;

  default:
    //printf("Uncaught Label message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naMenuItemWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  //case WM_SETFONT:
  //case WM_STYLECHANGING:
  //case WM_WINDOWPOSCHANGING:
  //case WM_CHILDACTIVATE:
  //case WM_MOVE:
  //case WM_SHOWWINDOW:
  //case WM_STYLECHANGED:
  //case WM_WINDOWPOSCHANGED:
  //case WM_SETTEXT:
  //case WM_PAINT:
  //case WM_NCPAINT:
  //case WM_GETFONT:
  //case WM_DESTROY:
  //case WM_NCDESTROY:
  //case WM_NCHITTEST:
  //case WM_SETCURSOR:
  //case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  //case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  //case WM_MOUSEACTIVATE:
  //case WM_LBUTTONDOWN:
  //case WM_IME_SETCONTEXT:
  //case WM_SETFOCUS:
  //case WM_CANCELMODE:
  //case WM_CAPTURECHANGED:
  //case WM_KILLFOCUS:
  //case WM_IME_NOTIFY:
  //case WM_LBUTTONUP:
  //  break;

  //case WM_ERASEBKGND:
  //  info.hasBeenHandeled = NA_TRUE;
  //  info.result = 1;
  //  break;

  default:
    //printf("Uncaught Label message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF NAMenu* naNewMenu(void* parent){
  NAWINAPIMenu* winapiMenu = naNew(NAWINAPIMenu);

  HMENU nativePtr = CreateMenu();
  // todo: how about DestroyMenu?

 // NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
 // WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
 // if(!app->oldLabelWindowProc){app->oldLabelWindowProc = oldproc;}

  na_InitMenu(&(winapiMenu->menu), nativePtr, parent);

 // winapiLabel->enabled = NA_TRUE;
 // winapiLabel->href = NA_NULL;
 // SendMessage(nativePtr, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NAMenu*)winapiMenu;
}



NA_DEF NAMenuItem* naNewMenuItem(NAMenu* menu, const NAUTF8Char* text, NAMenuItem* atItem){
  NAWINAPIMenuItem* winapiMenuItem = naNew(NAWINAPIMenuItem);

	HWND nativePtr = CreateWindow(
		TEXT("EDIT"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE,
		0,
    0,
    10,
    10,
		naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

 // NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
 // WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
 // if(!app->oldLabelWindowProc){app->oldLabelWindowProc = oldproc;}

  na_InitMenuItem(&(winapiMenuItem->menuItem), nativePtr);

 // winapiLabel->enabled = NA_TRUE;
 // winapiLabel->href = NA_NULL;
 // SendMessage(nativePtr, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF NAMenuItem* naNewMenuSeparator(NAMenu* menu, NAMenuItem* atItem){
  NAWINAPIMenuItem* winapiMenuItem = naNew(NAWINAPIMenuItem);

	HWND nativePtr = CreateWindow(
		TEXT("EDIT"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE,
		0,
    0,
    10,
    10,
		naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

 // NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
 // WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
 // if(!app->oldLabelWindowProc){app->oldLabelWindowProc = oldproc;}

  na_InitMenuItem(&(winapiMenuItem->menuItem), nativePtr);

 // winapiLabel->enabled = NA_TRUE;
 // winapiLabel->href = NA_NULL;
 // SendMessage(nativePtr, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NAMenuItem*)winapiMenuItem;
}

NA_DEF int32 naGetMenuItemIndex(NAMenu* menu, NAMenuItem* item){
  return 0;
}

NA_DEF void naPresentMenu(NAMenu* menu, NAPos pos){
  // todo
}



NA_DEF void na_DestructWINAPIMenu(NAWINAPIMenu* winapiMenu){
  na_ClearMenu((NAMenu*)winapiMenu);
}

NA_DEF void na_DestructWINAPIMenuItem(NAWINAPIMenuItem* winapiMenuItem){
  na_ClearMenuItem((NAMenuItem*)winapiMenuItem);
}



NA_HDEF NARect na_GetMenuAbsoluteInnerRect(NA_UIElement* menu){
  NA_UNUSED(menu);
  return naMakeRectS(20, 40, 100, 50);
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
