
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPITextBox NAWINAPITextBox;
struct NAWINAPITextBox {
  NA_TextBox coreTextBox;
  NAUIElement* nextTabStop;
  NAUIElement* prevTabStop;
};



NAWINAPICallbackInfo naTextBoxWINAPIProc(NAUIElement* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  default:
    //printf("Uncaught TextBox message" NA_NL);
    break;
  }
  
  return info;
}



NABool naHandleTextBoxTabOrder(NAReaction reaction){
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)reaction.uiElement;
  if(winapiTextBox->nextTabStop){
    SetFocus(naGetUIElementNativeID(winapiTextBox->nextTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NABool naHandleTextBoxReverseTabOrder(NAReaction reaction){
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)reaction.uiElement;
  if(winapiTextBox->prevTabStop){
    SetFocus(naGetUIElementNativeID(winapiTextBox->prevTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NATextBox* naNewTextBox(NASize size){
  HWND hWnd;
  DWORD style;

  NAWINAPITextBox* winapiTextBox = naAlloc(NAWINAPITextBox);

  style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;

	hWnd = CreateWindow(
		TEXT("EDIT"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  na_InitTextBox(&(winapiTextBox->coreTextBox), hWnd);
  winapiTextBox->nextTabStop = winapiTextBox;
  winapiTextBox->prevTabStop = winapiTextBox;

   naAddUIKeyboardShortcut(winapiTextBox, naMakeKeybardStatus(0, NA_KEYCODE_TAB), naHandleTextBoxTabOrder, NA_NULL);
 naAddUIKeyboardShortcut(winapiTextBox, naMakeKeybardStatus(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB), naHandleTextBoxReverseTabOrder, NA_NULL);

  SendMessage(hWnd, WM_SETFONT, (WPARAM)na_GetFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NATextBox*)winapiTextBox;
}



NA_DEF void na_DestructTextBox(NATextBox* textBox){
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  na_ClearTextBox(&(winapiTextBox->coreTextBox));
}



NA_DEF void naSetTextBoxText(NATextBox* textBox, const NAUTF8Char* text){
  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text);
  SendMessage(naGetUIElementNativeID(textBox), WM_SETTEXT, 0, (LPARAM)systemtext);
  naFree(systemtext);
}



NA_DEF void naSetTextBoxTextAlignment(NATextBox* textBox, NATextAlignment alignment){
  long style = (long)GetWindowLongPtr(naGetUIElementNativeID(textBox), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativeID(textBox), GWL_STYLE, style);
}



NA_DEF void naSetTextBoxFontKind(NATextBox* textBox, NAFontKind kind){
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  SendMessage(naGetUIElementNativeID(winapiTextBox), WM_SETFONT, (WPARAM)na_GetFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_DEF void naSetTextBoxEditable(NATextBox* textBox, NABool editable){
  SendMessage(naGetUIElementNativeID(textBox), EM_SETREADONLY, (WPARAM)!editable, 0);
}



NA_HDEF NAUIElement** na_GetTextBoxNextTabReference(NATextBox* textBox){
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  return &(winapiTextBox->nextTabStop);
}



NA_HDEF NAUIElement** na_GetTextBoxPrevTabReference(NATextBox* textBox){
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  return &(winapiTextBox->prevTabStop);
}



NA_HDEF NARect na_GetTextBoxAbsoluteInnerRect(NA_UIElement* textBox){
  NA_UNUSED(textBox);
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
