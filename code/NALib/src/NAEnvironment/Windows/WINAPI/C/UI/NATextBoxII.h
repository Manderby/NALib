
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPITextBox NAWINAPITextBox;
struct NAWINAPITextBox {
  NACoreTextBox coretextbox;
  NAUIElement* nextTabStop;
  NAUIElement* prevTabStop;
};



NAWINAPICallbackInfo naTextBoxWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  default:
    //printf("Uncaught TextBox message" NA_NL);
    break;
  }
  
  return info;
}



NABool naHandleTextBoxTabOrder(NAReaction reaction){
  NAWINAPITextBox* winapitextbox = (NAWINAPITextBox*)reaction.uielement;
  if(winapitextbox->nextTabStop){
    SetFocus(naGetUIElementNativeID(winapitextbox->nextTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NABool naHandleTextBoxReverseTabOrder(NAReaction reaction){
  NAWINAPITextBox* winapitextbox = (NAWINAPITextBox*)reaction.uielement;
  if(winapitextbox->prevTabStop){
    SetFocus(naGetUIElementNativeID(winapitextbox->prevTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NATextBox* naNewTextBox(NASize size){
  HWND hWnd;
  DWORD style;

  NAWINAPITextBox* winapitextbox = naAlloc(NAWINAPITextBox);

  style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;

	hWnd = CreateWindow(
		TEXT("EDIT"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naInitCoreTextBox(&(winapitextbox->coretextbox), hWnd);
  winapitextbox->nextTabStop = winapitextbox;
  winapitextbox->prevTabStop = winapitextbox;

   naAddUIKeyboardShortcut(winapitextbox, naMakeKeybardStatus(0, NA_KEYCODE_TAB), naHandleTextBoxTabOrder, NA_NULL);
 naAddUIKeyboardShortcut(winapitextbox, naMakeKeybardStatus(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB), naHandleTextBoxReverseTabOrder, NA_NULL);

  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NATextBox*)winapitextbox;
}



NA_DEF void naDestructTextBox(NATextBox* textbox){
  NAWINAPITextBox* winapitextbox = (NAWINAPITextBox*)textbox;
  naClearCoreTextBox(&(winapitextbox->coretextbox));
}



NA_DEF void naSetTextBoxText(NATextBox* textbox, const NAUTF8Char* text){
  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text);
  SendMessage(naGetUIElementNativeID(textbox), WM_SETTEXT, 0, (LPARAM)systemtext);
  naFree(systemtext);
}



NA_DEF void naSetTextBoxTextAlignment(NATextBox* textbox, NATextAlignment alignment){
  long style = (long)GetWindowLongPtr(naGetUIElementNativeID(textbox), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativeID(textbox), GWL_STYLE, style);
}



NA_DEF void naSetTextBoxFontKind(NATextBox* textbox, NAFontKind kind){
  NAWINAPITextBox* winapitextbox = (NAWINAPITextBox*)textbox;
  SendMessage(naGetUIElementNativeID(winapitextbox), WM_SETFONT, (WPARAM)getFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_DEF void naSetTextBoxEditable(NATextBox* textbox, NABool editable){
  SendMessage(naGetUIElementNativeID(textbox), EM_SETREADONLY, (WPARAM)!editable, 0);
}



NA_HDEF NAUIElement** naGetTextBoxNextTabReference(NATextBox* textbox){
  NAWINAPITextBox* winapitextbox = (NAWINAPITextBox*)textbox;
  return &(winapitextbox->nextTabStop);
}



NA_HDEF NAUIElement** naGetTextBoxPrevTabReference(NATextBox* textbox){
  NAWINAPITextBox* winapitextbox = (NAWINAPITextBox*)textbox;
  return &(winapitextbox->prevTabStop);
}



NA_HDEF NARect naGetTextBoxAbsoluteInnerRect(NACoreUIElement* textbox){
  NA_UNUSED(textbox);
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
