
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPILabel NAWINAPILabel;
struct NAWINAPILabel {
  NALabel label;
  NABool enabled;
  NAString* href;
};


NAWINAPICallbackInfo naLabelWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
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



NAWINAPICallbackInfo naLabelWINAPINotify(void* uiElement, WORD notificationCode){
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)uiElement;
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  switch(notificationCode){
    case EN_SETFOCUS:
      if(winapiLabel->href){
        system(naGetStringUTF8Pointer(winapiLabel->href));
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

  NAWINAPILabel* winapiLabel = naAlloc(NAWINAPILabel);

  // We need a read only edit control here, otherwise on windows, the user is not able to select text.
  style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE;

  systemtext = naAllocSystemStringWithUTF8String(text);

	hWnd = CreateWindow(
		TEXT("EDIT"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeId(naGetApplication()), NULL );
  
  naFree(systemtext);

  oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldLabelWindowProc){app->oldLabelWindowProc = oldproc;}

  na_InitLabel(&(winapiLabel->label), hWnd);
  SetWindowLongPtrA(hWnd, GWLP_USERDATA, &(winapiLabel->label));

  winapiLabel->enabled = NA_TRUE;
  winapiLabel->href = NA_NULL;
  SendMessage(hWnd, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NALabel*)winapiLabel;
}



NA_DEF void na_DestructLabel(NALabel* label){
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  if(winapiLabel->href){naDelete(winapiLabel->href);}
  na_ClearLabel(&(winapiLabel->label));
}



NA_DEF void naSetLabelText(NALabel* label, const NAUTF8Char* text){
  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text);
  SendMessage(naGetUIElementNativeId(label), WM_SETTEXT, 0, (LPARAM)systemtext);
  naFree(systemtext);
}



NA_DEF void naSetLabelLink(NALabel* label, const NAUTF8Char* url){
  HFONT hFont;
  HFONT hOriginalFont;
  LOGFONT lf;

  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  #ifndef NDEBUG
    if(!url || !*url)
      naError("url must be something useful. Deleting a Link is not possible yet.");
  #endif
  hOriginalFont = (HFONT)SendMessage(naGetUIElementNativeId(label), WM_GETFONT, 0, 0);
  GetObject(hOriginalFont, sizeof(LOGFONT), &lf);
  lf.lfUnderline = NA_TRUE;
  hFont = CreateFontIndirect(&lf);
  SendMessage(naGetUIElementNativeId(label), WM_SETFONT, (WPARAM)hFont, NA_FALSE);

  if(winapiLabel->href){naDelete(winapiLabel->href);}
  winapiLabel->href = naNewStringWithFormat("start %s", url);
}



NA_DEF NABool naIsLabelEnabled(NALabel* label){
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  return winapiLabel->enabled;
}



NA_DEF void naSetLabelEnabled(NALabel* label, NABool enabled){
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  winapiLabel->enabled = enabled;
  naRefreshUIElement(label, 0);
}



NA_DEF void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment){
  long style = (long)GetWindowLongPtr(naGetUIElementNativeId(label), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativeId(label), GWL_STYLE, style);
}



NA_DEF void naSetLabelFontKind(NALabel* label, NAFontKind kind){
  SendMessage(naGetUIElementNativeId(label), WM_SETFONT, (WPARAM)na_GetFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_HDEF NARect na_GetLabelAbsoluteInnerRect(NA_UIElement* label){
  NA_UNUSED(label);
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
