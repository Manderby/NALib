
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naTextBoxWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {

  case WM_ERASEBKGND:
  info.hasBeenHandeled = NA_TRUE;
  info.result = 1;
  break;

  default:
    //printf("Uncaught TextBox message" NA_NL);
    break;
  }
  
  return info;
}



void naHandleTextBoxTabOrder(NAReaction reaction) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)reaction.uiElement;
  if(winapiTextBox->nextTabStop) {
    SetFocus(naGetUIElementNativePtr(winapiTextBox->nextTabStop));
  }
}



void naHandleTextBoxReverseTabOrder(NAReaction reaction) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)reaction.uiElement;
  if(winapiTextBox->prevTabStop) {
    SetFocus(naGetUIElementNativePtr(winapiTextBox->prevTabStop));
  }
}



NA_DEF NATextBox* naNewTextBox(NASize size) {
  NAWINAPITextBox* winapiTextBox = naNew(NAWINAPITextBox);

  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  winapiTextBox->rect = naMakeRect(naMakePos(0., 0.), size);

	HWND nativePtr = CreateWindow(
		TEXT("EDIT"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		0,
    0, 
    (int)(winapiTextBox->rect.size.width * uiScale),
    (int)(winapiTextBox->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(), 
    NULL, 
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL );
  
  na_InitTextBox(&winapiTextBox->textBox, nativePtr);
  winapiTextBox->nextTabStop = winapiTextBox;
  winapiTextBox->prevTabStop = winapiTextBox;

  naAddUIKeyboardShortcut(
    winapiTextBox,
    naMakeKeyStroke(0, NA_KEYCODE_TAB),
    naHandleTextBoxTabOrder,
    NA_NULL);
  naAddUIKeyboardShortcut(
    winapiTextBox,
    naMakeKeyStroke(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB),
    naHandleTextBoxReverseTabOrder,
    NA_NULL);

  winapiTextBox->textBox.font = naRetain(naCreateSystemFont());
  SendMessage(
    nativePtr,
    WM_SETFONT,
    (WPARAM)naGetFontNativePointer(winapiTextBox->textBox.font),
    MAKELPARAM(TRUE, 0));

  return (NATextBox*)winapiTextBox;
}



NA_DEF void na_DestructWINAPITextBox(NAWINAPITextBox* winapiTextBox) {
  na_ClearTextBox((NATextBox*)winapiTextBox);
}



NA_DEF void naSetTextBoxVisible(NATextBox* textBox, NABool visible) {
  ShowWindow(naGetUIElementNativePtr(textBox), visible ? SW_SHOW : SW_HIDE);
}



NA_DEF void naSetTextBoxEditable(NATextBox* textBox, NABool editable) {
  SendMessage(naGetUIElementNativePtr(textBox), EM_SETREADONLY, (WPARAM)!editable, 0);
}



NA_DEF NAString* naNewStringWithTextBoxText(const NATextBox* textBox) {
  return NA_NULL;
  // todo
}



NA_DEF void naSetTextBoxText(NATextBox* textBox, const NAUTF8Char* text) {
  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);
  SendMessage(naGetUIElementNativePtr(textBox), WM_SETTEXT, 0, (LPARAM)systemText);
  naFree(systemText);
}



NA_DEF void naSetTextBoxTextAlignment(NATextBox* textBox, NATextAlignment alignment) {
  long style = (long)GetWindowLongPtr(naGetUIElementNativePtr(textBox), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativePtr(textBox), GWL_STYLE, style);
}



NA_DEF void naSetTextBoxFont(NATextBox* textBox, NAFont* font) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  SendMessage(naGetUIElementNativePtr(winapiTextBox), WM_SETFONT, (WPARAM)naGetFontNativePointer(font), MAKELPARAM(TRUE, 0));
  naRelease(textBox->font);
  textBox->font = naRetain(font);
}



NA_DEF void naSetTextBoxUseHorizontalScrolling(NATextBox* textBox) {
  // todo
}



NA_DEF void naSetTextBoxUseVerticalScrolling(NATextBox* textBox, NABool use) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  ShowScrollBar(naGetUIElementNativePtr(textBox), SB_VERT, use);
}



NA_HDEF void** na_GetTextBoxNextTabReference(NATextBox* textBox) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  return &winapiTextBox->nextTabStop;
}



NA_HDEF void** na_GetTextBoxPrevTabReference(NATextBox* textBox) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;
  return &winapiTextBox->prevTabStop;
}



NA_HDEF NARect na_GetTextBoxRect(const NA_UIElement* textBox) {
  const NAWINAPITextBox* winapiTextBox = (const NAWINAPITextBox*)textBox;
  return winapiTextBox->rect;
}



NA_HDEF void na_SetTextBoxRect(NA_UIElement* textBox, NARect rect) {
  NAWINAPITextBox* winapiTextBox = (NAWINAPITextBox*)textBox;

  winapiTextBox->rect = rect;
  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(textBox));

  SetWindowPos(
    naGetUIElementNativePtr(textBox),
    HWND_TOP,
    (int)(winapiTextBox->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiTextBox->rect.pos.y - winapiTextBox->rect.size.height) * uiScale),
    (int)(winapiTextBox->rect.size.width * uiScale),
    (int)(winapiTextBox->rect.size.height * uiScale),
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
