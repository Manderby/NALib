
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


typedef struct NAWINAPICheckBox NAWINAPICheckBox;
struct NAWINAPICheckBox {
  NACoreCheckBox corecheckbox;
};



NAWINAPICallbackInfo naCheckBoxWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
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
  case WM_SETFOCUS:
  case BM_SETSTATE:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_KILLFOCUS:
  case WM_IME_NOTIFY:
  case BM_GETSTATE:
  case WM_LBUTTONUP:
    break;

  default:
    //printf("Uncaught CheckBox message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naCheckBoxWINAPINotify(NAUIElement* uielement, WORD notificationCode){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NABool check;

  switch(notificationCode){
    case BN_CLICKED:
      ReleaseCapture();
      check = naGetCheckBoxState(uielement);
      naSetCheckBoxState(uielement, !check);
      naDispatchUIElementCommand(uielement, NA_UI_COMMAND_PRESSED);
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
      break;
  }

  return info;
}



NA_DEF NACheckBox* naNewCheckBox(const NAUTF8Char* text, NASize size){
  HWND hWnd;
  DWORD style;
  TCHAR* systemtext;
  WNDPROC oldproc;

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPICheckBox* winapicheckbox = naAlloc(NAWINAPICheckBox);

  style = WS_CHILD | WS_VISIBLE | BS_LEFT | BS_VCENTER | BS_TEXT | BS_CHECKBOX;

  systemtext = naAllocSystemStringWithUTF8String(text);

	hWnd = CreateWindow(
		TEXT("BUTTON"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );

  naFree(systemtext);

  oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldCheckBoxWindowProc){app->oldCheckBoxWindowProc = oldproc;}

  naInitCoreCheckBox(&(winapicheckbox->corecheckbox), hWnd);

  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NACheckBox*)winapicheckbox;
}



NA_DEF void naDestructCheckBox(NACheckBox* checkbox){
  NAWINAPICheckBox* winapicheckbox = (NAWINAPICheckBox*)checkbox;
  naClearCoreCheckBox(&(winapicheckbox->corecheckbox));
}



NA_HDEF NARect naGetCheckBoxAbsoluteInnerRect(NACoreUIElement* checkbox){
  NA_UNUSED(checkbox);
  return naMakeRectS(20, 40, 100, 50);
}



NA_HDEF NABool naGetCheckBoxState(NACheckBox* checkbox){
  LPARAM state = SendMessage(naGetUIElementNativeID(checkbox), BM_GETSTATE, 0, 0);
  return (state & BST_CHECKED) == BST_CHECKED;
}



NA_HDEF void naSetCheckBoxState(NACheckBox* checkbox, NABool state){
  LPARAM lParam = state ? BST_CHECKED : BST_UNCHECKED;
  SendMessage(naGetUIElementNativeID(checkbox), BM_SETCHECK, lParam, 0);
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
