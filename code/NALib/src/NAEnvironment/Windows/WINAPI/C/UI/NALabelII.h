
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPILabel NAWINAPILabel;
struct NAWINAPILabel {
  NACoreLabel corelabel;
  NABool enabled;
  NAString* href;
};


NAWINAPICallbackInfo naLabelWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:
  case WM_STYLECHANGING:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_STYLECHANGED:
  case WM_WINDOWPOSCHANGED:
  case WM_SETTEXT:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_GETFONT:
  case WM_DESTROY:
  case WM_NCDESTROY:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_SETFOCUS:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_KILLFOCUS:
  case WM_IME_NOTIFY:
  case WM_LBUTTONUP:
    break;

  case WM_ERASEBKGND:
    info.hasbeenhandeled = NA_TRUE;
    info.result = 1;
    break;

  default:
    //printf("Uncaught Label message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naLabelWINAPINotify(NAUIElement* uielement, WORD notificationCode){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)uielement;
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  switch(notificationCode){
    case EN_SETFOCUS:
      if(winapilabel->href){
        system(naGetStringUTF8Pointer(winapilabel->href));
        info.hasbeenhandeled = NA_TRUE;
        info.result = 0;
      }
      break;
  }
  return info;
}



NA_DEF NALabel* naNewLabel(const NAUTF8Char* text, NASize size){
  HWND hWnd;
  DWORD style;
  TCHAR* systemtext;
  WNDPROC oldproc;

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPILabel* winapilabel = naAlloc(NAWINAPILabel);

  // We need a read only edit control here, otherwise on windows, the user is not able to select text.
  style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE;

  systemtext = naAllocSystemStringWithUTF8String(text);

	hWnd = CreateWindow(
		TEXT("EDIT"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naFree(systemtext);

  oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldLabelWindowProc){app->oldLabelWindowProc = oldproc;}

  naInitCoreLabel(&(winapilabel->corelabel), hWnd);

  winapilabel->enabled = NA_TRUE;
  winapilabel->href = NA_NULL;
  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NALabel*)winapilabel;
}



NA_DEF void naDestructLabel(NALabel* label){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  if(winapilabel->href){naDelete(winapilabel->href);}
  naClearCoreLabel(&(winapilabel->corelabel));
}



NA_DEF void naSetLabelText(NALabel* label, const NAUTF8Char* text){
  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text);
  SendMessage(naGetUIElementNativeID(label), WM_SETTEXT, 0, (LPARAM)systemtext);
  naFree(systemtext);
}



NA_DEF void naSetLabelLink(NALabel* label, const NAUTF8Char* url){
  HFONT hFont;
  HFONT hOriginalFont;
  LOGFONT lf;

  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  #ifndef NDEBUG
    if(!url || !*url)
      naError("url must be something useful. Deleting a Link is not possible yet.");
  #endif
  hOriginalFont = (HFONT)SendMessage(naGetUIElementNativeID(label), WM_GETFONT, 0, 0);
  GetObject(hOriginalFont, sizeof(LOGFONT), &lf);
  lf.lfUnderline = NA_TRUE;
  hFont = CreateFontIndirect(&lf);
  SendMessage(naGetUIElementNativeID(label), WM_SETFONT, (WPARAM)hFont, NA_FALSE);

  if(winapilabel->href){naDelete(winapilabel->href);}
  winapilabel->href = naNewStringWithFormat("start %s", url);
}



NA_DEF NABool naIsLabelEnabled(NALabel* label){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  return winapilabel->enabled;
}



NA_DEF void naSetLabelEnabled(NALabel* label, NABool enabled){
  NAWINAPILabel* winapilabel = (NAWINAPILabel*)label;
  winapilabel->enabled = enabled;
  naRefreshUIElement(label, 0);
}



NA_DEF void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment){
  long style = (long)GetWindowLongPtr(naGetUIElementNativeID(label), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativeID(label), GWL_STYLE, style);
}



NA_DEF void naSetLabelFontKind(NALabel* label, NAFontKind kind){
  SendMessage(naGetUIElementNativeID(label), WM_SETFONT, (WPARAM)getFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_HDEF NARect naGetLabelAbsoluteInnerRect(NACoreUIElement* label){
  NA_UNUSED(label);
  return naMakeRectS(20, 40, 100, 50);
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
