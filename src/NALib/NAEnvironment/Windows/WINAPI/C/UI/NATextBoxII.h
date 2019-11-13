
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPITextBox NAWINAPITextBox;
struct NAWINAPITextBox {
  NACoreTextBox coretextbox;
};



NAWINAPICallbackInfo naTextBoxWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  default:
    //printf("Uncaught TextBox message\n");
    break;
  }
  
  return info;
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



NA_HDEF NARect naGetTextBoxAbsoluteInnerRect(NACoreUIElement* textbox){
  NA_UNUSED(textbox);
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
