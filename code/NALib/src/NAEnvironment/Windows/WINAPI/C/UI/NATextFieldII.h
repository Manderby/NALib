
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPITextField NAWINAPITextField;
struct NAWINAPITextField {
  NATextField textField;
  void*       nextTabStop;
  void*       prevTabStop;
};

NA_HAPI void na_DestructWINAPITextField(NAWINAPITextField* winapiTextField);
NA_RUNTIME_TYPE(NAWINAPITextField, na_DestructWINAPITextField, NA_FALSE);



NAWINAPICallbackInfo naTextFieldWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_IME_NOTIFY:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_GETTEXTLENGTH:
  case EM_GETSEL:
  case EM_GETRECT:
  case EM_CHARFROMPOS:
  case WM_GETFONT:
  case EM_LINEFROMCHAR:
  case EM_POSFROMCHAR:
  case WM_LBUTTONUP:
  case 0x43C:  // undocumented
  case WM_GETTEXT:
  case WM_SETTEXT:
  case EM_LINELENGTH:
  case WM_KEYUP:
  case WM_SYSKEYUP:
  case WM_CHAR:
  case WM_KEYDOWN: // capture enter and tab here.
  case WM_SYSKEYDOWN: // the alt key!
  case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  // note that any change of the edit control is captured in naWINAPINotificationProc.
  break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // textfields to not display a cursor or selection.
    break;

  default:
    //printf("Uncaught TextField message" NA_NL);
  break;
  }
 
  return info;
}



NAWINAPICallbackInfo naTextFieldWINAPINotify(void* uiElement, WORD notificationCode){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  switch(notificationCode){
  case EN_CHANGE:
    na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_EDITED);
    info.hasBeenHandeled = NA_TRUE;
    info.result = 0;
    break;
  }
  return info;
}



NABool naHandleTextFieldTabOrder(NAReaction reaction){
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)reaction.uiElement;
  if(winapiTextField->nextTabStop){
    SetFocus(naGetUIElementNativePtr(winapiTextField->nextTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NABool naHandleTextFieldReverseTabOrder(NAReaction reaction){
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)reaction.uiElement;
  if(winapiTextField->prevTabStop){
    SetFocus(naGetUIElementNativePtr(winapiTextField->prevTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NATextField* naNewTextField(NASize size){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPITextField* winapiTextField = naNew(NAWINAPITextField);

  HWND nativePtr = CreateWindow(
	  TEXT("EDIT"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
	  0,
    0,
    (int)size.width,
    (int)size.height,
	  naGetApplicationOffscreenWindow(),
    NULL, 
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);
 
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldTextFieldWindowProc){app->oldTextFieldWindowProc = oldproc;}

  na_InitTextField(&(winapiTextField->textField), nativePtr);
  winapiTextField->nextTabStop = winapiTextField;
  winapiTextField->prevTabStop = winapiTextField;

  naAddUIKeyboardShortcut(
    winapiTextField,
    naMakeKeybardStatus(0, NA_KEYCODE_TAB),
    naHandleTextFieldTabOrder,
    NA_NULL);
  naAddUIKeyboardShortcut(
    winapiTextField,
    naMakeKeybardStatus(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB),
    naHandleTextFieldReverseTabOrder,
    NA_NULL);

  SendMessage(nativePtr, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NATextField*)winapiTextField;
}



NA_DEF void na_DestructWINAPITextField(NAWINAPITextField* winapiTextField){
  na_ClearTextField((NATextField*)winapiTextField);
}



NA_DEF void naSetTextFieldText(NATextField* textField, const NAUTF8Char* text){
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);
  na_BlockUIElementNotifications(&(winapiTextField->textField.uiElement));
  SendMessage(naGetUIElementNativePtr(textField), WM_SETTEXT, 0, (LPARAM)systemText);
  na_AllowUIElementNotifications(&(winapiTextField->textField.uiElement));
  naFree(systemText);
}



NA_DEF NAString* naNewStringWithTextFieldText(NATextField* textField){
  LRESULT textlength = SendMessage(naGetUIElementNativePtr(textField), WM_GETTEXTLENGTH, 0, 0);
  if(textlength){
    TCHAR* buffer = naMalloc((textlength + 1) * sizeof(TCHAR));
    SendMessage(naGetUIElementNativePtr(textField), WM_GETTEXT, textlength + 1, (LPARAM)buffer);
    return naNewStringFromSystemString(buffer);
  }else{
    return naNewString();
  }
}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textField, NATextAlignment alignment){
 long style = (long)GetWindowLongPtr(naGetUIElementNativePtr(textField), GWL_STYLE);
 style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
 SetWindowLongPtr(naGetUIElementNativePtr(textField), GWL_STYLE, style);
}



NA_DEF void naSetTextFieldFontKind(NATextField* textField, NAFontKind kind){
 NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
 SendMessage(naGetUIElementNativePtr(winapiTextField), WM_SETFONT, (WPARAM)na_GetFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_HDEF void** na_GetTextFieldNextTabReference(NATextField* textField){
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
  return &(winapiTextField->nextTabStop);
}



NA_HDEF void** na_GetTextFieldPrevTabReference(NATextField* textField){
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
  return &(winapiTextField->prevTabStop);
}



NA_HDEF NARect na_GetTextFieldAbsoluteInnerRect(NA_UIElement* textField){
 NA_UNUSED(textField);
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
