
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


typedef struct NAWINAPICheckBox NAWINAPICheckBox;
struct NAWINAPICheckBox {
  NACheckBox checkBox;
};

NA_HAPI void na_DestructWINAPICheckBox(NAWINAPICheckBox* winapiCheckBox);
NA_RUNTIME_TYPE(NAWINAPICheckBox, na_DestructWINAPICheckBox, NA_FALSE);



NAWINAPICallbackInfo naCheckBoxWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case BM_SETCHECK:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case BM_SETSTATE:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_IME_NOTIFY:
  case BM_GETSTATE:
  case WM_LBUTTONUP:
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // We do not display any caret.
    info.hasBeenHandeled = NA_TRUE;
    info.result = 0;
    break;

  default:
    //printf("Uncaught CheckBox message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naCheckBoxWINAPINotify(void* uiElement, WORD notificationCode){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NABool check;

  switch(notificationCode){
    case BN_CLICKED:
      ReleaseCapture();
      check = naGetCheckBoxState(uiElement);
      naSetCheckBoxState(uiElement, !check);
      na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_PRESSED);
      info.hasBeenHandeled = NA_TRUE;
      info.result = 0;
      break;
  }

  return info;
}



NA_DEF NACheckBox* naNewCheckBox(const NAUTF8Char* text, NASize size){
  NAWINAPICheckBox* winapiCheckBox = naNew(NAWINAPICheckBox);

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

	HWND nativePtr = CreateWindow(
		TEXT("BUTTON"),
    systemText,
    WS_CHILD | WS_VISIBLE | BS_LEFT | BS_VCENTER | BS_TEXT | BS_CHECKBOX,
		0,
    0,
    (int)size.width,
    (int)size.height,
		naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  naFree(systemText);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldCheckBoxWindowProc){app->oldCheckBoxWindowProc = oldproc;}

  na_InitCheckBox(&(winapiCheckBox->checkBox), nativePtr);

  SendMessage(nativePtr, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NACheckBox*)winapiCheckBox;
}



NA_DEF void na_DestructWINAPICheckBox(NAWINAPICheckBox* winapiCheckBox){
  na_ClearCheckBox((NACheckBox*)winapiCheckBox);
}



NA_HDEF NARect na_GetCheckBoxAbsoluteInnerRect(NA_UIElement* checkBox){
  NA_UNUSED(checkBox);
  return naMakeRectS(20, 40, 100, 50);
}



NA_DEF NABool naGetCheckBoxState(NACheckBox* checkBox){
  LPARAM state = SendMessage(naGetUIElementNativePtr(checkBox), BM_GETSTATE, 0, 0);
  return (state & BST_CHECKED) == BST_CHECKED;
}



NA_DEF void naSetCheckBoxState(NACheckBox* checkBox, NABool state){
  LPARAM lParam = state ? BST_CHECKED : BST_UNCHECKED;
  SendMessage(naGetUIElementNativePtr(checkBox), BM_SETCHECK, lParam, 0);
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
