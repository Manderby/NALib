
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naTextFieldWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {
  case WM_SETFONT:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
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
  case WM_SYSKEYUP:
  case WM_CHAR:
  case WM_SYSKEYDOWN: // the alt key!
  // note that any change of the edit control is captured in naWINAPINotificationProc.
  break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // textFields to not display a cursor or selection.
    info = naUIElementWINAPIDefaultProc(naGetUIElementNativePtr(uiElement), message, wParam, lParam);
    break;

  default:
    //printf("Uncaught TextField message" NA_NL);
  break;
  }
 
  return info;
}



NABool naTextFieldWINAPINotify(void* uiElement, WORD notificationCode) {
  NABool hasBeenHandeled = NA_FALSE;
  switch(notificationCode) {
  case EN_CHANGE:
    if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_EDITED)) {
      // don't know what to do.
    }
    hasBeenHandeled = NA_TRUE;
    break;
  // todo: add code for NA_UI_COMMAND_EDIT_FINISHED
  }
  return hasBeenHandeled;
}



void naHandleTextFieldEnter(NAReaction reaction) {
  if(!na_DispatchUIElementCommand(reaction.uiElement, NA_UI_COMMAND_EDIT_FINISHED)) {
    // don't know what to do.
  }
}



void naHandleTextFieldTabOrder(NAReaction reaction) {
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)reaction.uiElement;
  if(winapiTextField->nextTabStop) {
    naHandleTextFieldEnter(reaction);
    SetFocus(naGetUIElementNativePtr(winapiTextField->nextTabStop));
  }
}



void naHandleTextFieldReverseTabOrder(NAReaction reaction) {
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)reaction.uiElement;
  if(winapiTextField->prevTabStop) {
    naHandleTextFieldEnter(reaction);
    SetFocus(naGetUIElementNativePtr(winapiTextField->prevTabStop));
  }
}



NA_DEF NATextField* naNewTextField(double width) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPITextField* winapiTextField = naNew(NAWINAPITextField);

  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  winapiTextField->rect = naMakeRectS(0., 0., width, 20.);

  // If this is used before Win XP, it might fail. Use CreateWindow() instead
  // and omit the first argument.
  HWND nativePtr = CreateWindowEx(
    WS_EX_CLIENTEDGE,
	  TEXT("EDIT"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
	  0,
    0,
    (int)(winapiTextField->rect.size.width * uiScale),
    (int)(winapiTextField->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL, 
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);
 
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldTextFieldWindowProc) {
    app->oldTextFieldWindowProc = oldproc;
  }

  na_InitTextField(&winapiTextField->textField, nativePtr);
  winapiTextField->nextTabStop = winapiTextField;
  winapiTextField->prevTabStop = winapiTextField;

  naAddUIKeyboardShortcut(
    winapiTextField,
    naMakeKeyStroke(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER),
    naHandleTextFieldEnter,
    NA_NULL);
  naAddUIKeyboardShortcut(
    winapiTextField,
    naMakeKeyStroke(0, NA_KEYCODE_TAB),
    naHandleTextFieldTabOrder,
    NA_NULL);
  naAddUIKeyboardShortcut(
    winapiTextField,
    naMakeKeyStroke(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB),
    naHandleTextFieldReverseTabOrder,
    NA_NULL);

  winapiTextField->textField.font = naRetain(naCreateSystemFont());
  SendMessage(
    nativePtr,
    WM_SETFONT,
    (WPARAM)naGetFontNativePointer(winapiTextField->textField.font),
    MAKELPARAM(TRUE, 0));

  return (NATextField*)winapiTextField;
}



NA_DEF void na_DestructWINAPITextField(NAWINAPITextField* winapiTextField) {
  na_ClearTextField((NATextField*)winapiTextField);
}



NA_DEF void naSetTextFieldEnabled(NATextField* textField, NABool enabled) {
  EnableWindow(naGetUIElementNativePtr(textField), enabled);
}



NA_DEF void naSetTextFieldText(NATextField* textField, const NAUTF8Char* text) {
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);
  na_SetUIElementWINAPINotificationsBlocked(&winapiTextField->textField.uiElement, NA_TRUE);
  SendMessage(naGetUIElementNativePtr(textField), WM_SETTEXT, 0, (LPARAM)systemText);
  na_SetUIElementWINAPINotificationsBlocked(&winapiTextField->textField.uiElement, NA_FALSE);
  naFree(systemText);
}



NA_DEF NAString* naNewStringWithTextFieldText(const NATextField* textField) {
  LRESULT textlength = SendMessage(naGetUIElementNativePtrConst(textField), WM_GETTEXTLENGTH, 0, 0);
  if(textlength) {
    TCHAR* buffer = naMalloc((textlength + 1) * sizeof(TCHAR));
    SendMessage(naGetUIElementNativePtrConst(textField), WM_GETTEXT, textlength + 1, (LPARAM)buffer);
    return naNewStringWithSystemString(buffer);
  }else{
    return naNewString();
  }
}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textField, NATextAlignment alignment) {
 long style = (long)GetWindowLongPtr(naGetUIElementNativePtr(textField), GWL_STYLE);
 style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
 SetWindowLongPtr(naGetUIElementNativePtr(textField), GWL_STYLE, style);
}



NA_DEF void naSetTextFieldFont(NATextField* textField, NAFont* font) {
 NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
 SendMessage(naGetUIElementNativePtr(winapiTextField), WM_SETFONT, (WPARAM)naGetFontNativePointer(font), MAKELPARAM(TRUE, 0));
 naRelease(textField->font);
 textField->font = naRetain(font);
}



NA_HDEF void** na_GetTextFieldNextTabReference(NATextField* textField) {
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
  return &winapiTextField->nextTabStop;
}



NA_HDEF void** na_GetTextFieldPrevTabReference(NATextField* textField) {
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;
  return &winapiTextField->prevTabStop;
}



NA_HAPI NARect na_GetTextFieldRect(const NA_UIElement* textField)
{
  const NAWINAPITextField* winapiTextField = (const NAWINAPITextField*)textField;
  return winapiTextField->rect;
}



NA_HDEF void na_SetTextFieldRect(NA_UIElement* textField, NARect rect) {
  NAWINAPITextField* winapiTextField = (NAWINAPITextField*)textField;

  winapiTextField->rect = rect;
  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(textField));

  SetWindowPos(
    naGetUIElementNativePtr(textField),
    HWND_TOP,
    (int)(winapiTextField->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiTextField->rect.pos.y - winapiTextField->rect.size.height) * uiScale),
    (int)(winapiTextField->rect.size.width * uiScale),
    (int)(winapiTextField->rect.size.height * uiScale),
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
