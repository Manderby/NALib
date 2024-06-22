
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naLabelWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {
  case WM_SETFONT:
  case WM_STYLECHANGING:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_STYLECHANGED:
  case WM_SETTEXT:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_GETFONT:
  case WM_DESTROY:
  case WM_NCDESTROY:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_IME_NOTIFY:
  case WM_LBUTTONUP:
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // labels to not display a selection.
    info = naUIElementWINAPIDefaultProc(naGetUIElementNativePtr(uiElement), message, wParam, lParam);
    break;

  case WM_ERASEBKGND:
    info.hasBeenHandeled = NA_TRUE;
    info.result = 1;
    break;

  default:
    //printf("Uncaught Label message" NA_NL);
    break;
  }
  
  return info;
}



NABool naLabelWINAPINotify(void* uiElement, WORD notificationCode) {
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)uiElement;
  NABool hasBeenHandeled = NA_FALSE;
  switch(notificationCode) {
    case EN_SETFOCUS:
      if(winapiLabel->href) {
        system(naGetStringUTF8Pointer(winapiLabel->href));
        hasBeenHandeled = NA_TRUE;
      }
      break;
  }
  return hasBeenHandeled;
}



NA_DEF NALabel* naNewLabel(const NAUTF8Char* text, double width) {
  NAWINAPILabel* winapiLabel = naNew(NAWINAPILabel);

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

  winapiLabel->rect = naMakeRectS(0., 0., width, 16.);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  // We need a read only edit control here, otherwise on windows, the user is not able to select text.
	HWND nativePtr = CreateWindow(
		TEXT("EDIT"),
    systemText,
    WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE,
    0,
    0,
    (int)(winapiLabel->rect.size.width * uiScale),
    (int)(winapiLabel->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);
  
  naFree(systemText);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldLabelWindowProc) {
    app->oldLabelWindowProc = oldproc;
  }

  na_InitLabel(&(winapiLabel->label), nativePtr);

  winapiLabel->enabled = NA_TRUE;
  winapiLabel->href = NA_NULL;

  winapiLabel->label.font = naRetain(naGetSystemFont());
  SendMessage(
    nativePtr,
    WM_SETFONT,
    (WPARAM)naGetFontNativePointer(winapiLabel->label.font),
    MAKELPARAM(TRUE, 0));

  return (NALabel*)winapiLabel;
}



NA_DEF void na_DestructWINAPILabel(NAWINAPILabel* winapiLabel) {
  if(winapiLabel->href) {
    naDelete(winapiLabel->href);
  }
  na_ClearLabel((NALabel*)winapiLabel);
}



NA_DEF void naSetLabelVisible(NALabel* label, NABool visible) {
  ShowWindow(naGetUIElementNativePtr(label), visible ? SW_SHOW : SW_HIDE);
}



NA_DEF NABool naIsLabelEnabled(const NALabel* label) {
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  return winapiLabel->enabled;
}



NA_DEF void naSetLabelEnabled(NALabel* label, NABool enabled) {
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  winapiLabel->enabled = enabled;
  naRefreshUIElement(label, 0);
}



NA_DEF void naSetLabelText(NALabel* label, const NAUTF8Char* text) {
  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);
  SendMessage(naGetUIElementNativePtr(label), WM_SETTEXT, 0, (LPARAM)systemText);
  naFree(systemText);
}



NA_DEF void naSetLabelLink(NALabel* label, const NAUTF8Char* url) {
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  #if NA_DEBUG
    if(!url || !*url)
      naError("url must be something useful. Deleting a Link is not possible yet.");
  #endif

  NAFont* underlineFont = naCreateFont(
    naGetStringUTF8Pointer(naGetFontName(label->font)),
    naGetFontFlags(label->font) | NA_FONT_FLAG_UNDERLINE,
    naGetFontSize(label->font));

  naSetLabelFont(label, underlineFont);

  naRelease(underlineFont);

  if(winapiLabel->href)
    naDelete(winapiLabel->href);
    
  winapiLabel->href = naNewStringWithFormat("start %s", url);
}



NA_DEF void naSetLabelSelectable(NALabel* label, NABool selectable) {
  // todo
}



NA_DEF void naSetLabelHeight(NALabel* label, double height) {
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  winapiLabel->rect.size.height = height;

  SetWindowPos(
    label->uiElement.nativePtr,
    HWND_TOP,
    (int)(winapiLabel->rect.pos.x * uiScale),
    (int)(naGetRectEndY(winapiLabel->rect) * uiScale),
    (int)(winapiLabel->rect.size.width * uiScale),
    (int)(winapiLabel->rect.size.height * uiScale),
    0);
}



NA_DEF void naSetLabelTextColor(NALabel* label, const NAColor* color) {
  // todo
}



NA_DEF void naSetLabelTextAlignment(NALabel* label, NATextAlignment alignment) {
  long style = (long)GetWindowLongPtr(naGetUIElementNativePtr(label), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativePtr(label), GWL_STYLE, style);
}



NA_DEF void naSetLabelFont(NALabel* label, NAFont* font) {
  SendMessage(naGetUIElementNativePtr(label), WM_SETFONT, (WPARAM)naGetFontNativePointer(font), MAKELPARAM(TRUE, 0));
  naRelease(label->font);
  label->font = naRetain(font);
}



NA_HDEF NARect na_GetLabelRect(const NA_UIElement* label)
{
  const NAWINAPILabel* winapiLabel = (const NAWINAPILabel*)label;
  return winapiLabel->rect;
}

NA_HDEF void na_SetLabelRect(NA_UIElement* label, NARect rect) {
  NAWINAPILabel* winapiLabel = (NAWINAPILabel*)label;

  winapiLabel->rect = rect;
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(label));

  SetWindowPos(
    naGetUIElementNativePtr(label),
    HWND_TOP,
    (int)(winapiLabel->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiLabel->rect.pos.y - winapiLabel->rect.size.height) * uiScale),
    (int)(winapiLabel->rect.size.width * uiScale),
    (int)(winapiLabel->rect.size.height * uiScale),
    0);
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
