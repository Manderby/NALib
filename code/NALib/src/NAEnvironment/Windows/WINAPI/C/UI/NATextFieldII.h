
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPITextField NAWINAPITextField;
struct NAWINAPITextField {
 NACoreTextField coretextfield;
 NAUIElement* nextTabStop;
 NAUIElement* prevTabStop;
};



NAWINAPICallbackInfo naTextFieldWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
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
 case WM_SETFOCUS:
 case WM_IME_NOTIFY:
 case WM_CANCELMODE:
 case WM_CAPTURECHANGED:
 case WM_KILLFOCUS:
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
 case WM_MOUSEMOVE:
 case WM_MOUSELEAVE:

 // note that any change of the edit control is captured in naWINAPINotificationProc.
 break;

 default:
   //printf("Uncaught TextField message\n");
  break;
 }
 
 return info;
}



NAWINAPICallbackInfo naTextFieldWINAPINotify(NAUIElement* uielement, WORD notificationCode){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  switch(notificationCode){
  case EN_CHANGE:
    naDispatchUIElementCommand(uielement, NA_UI_COMMAND_EDITED);
    info.hasbeenhandeled = NA_TRUE;
    info.result = 0;
    break;
  }
  return info;
}



NABool naHandleTextFieldTabOrder(NAReaction reaction){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)reaction.uielement;
  if(winapitextfield->nextTabStop){
    SetFocus(naGetUIElementNativeID(winapitextfield->nextTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NABool naHandleTextFieldReverseTabOrder(NAReaction reaction){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)reaction.uielement;
  if(winapitextfield->prevTabStop){
    SetFocus(naGetUIElementNativeID(winapitextfield->prevTabStop));
    return NA_TRUE;
  }
  return NA_FALSE;
}



NA_DEF NATextField* naNewTextField(NASize size){
 HWND hWnd;
 DWORD style;
 NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

 NAWINAPITextField* winapitextfield = naAlloc(NAWINAPITextField);

 // WS_TABSTOP and WS_GROUP seem not to work... strange. I solved it using the
 // naInterceptKeyboardShortcut function. 
 style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;

	hWnd = CreateWindow(
		TEXT("EDIT"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
 
 WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
 if(!app->oldTextFieldWindowProc){app->oldTextFieldWindowProc = oldproc;}

 naInitCoreTextField(&(winapitextfield->coretextfield), hWnd);
 winapitextfield->nextTabStop = winapitextfield;
 winapitextfield->prevTabStop = winapitextfield;

 naAddUIKeyboardShortcut(winapitextfield, naMakeKeybardStatus(0, NA_KEYCODE_TAB), naHandleTextFieldTabOrder, NA_NULL);
 naAddUIKeyboardShortcut(winapitextfield, naMakeKeybardStatus(NA_MODIFIER_FLAG_SHIFT, NA_KEYCODE_TAB), naHandleTextFieldReverseTabOrder, NA_NULL);

 SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

 return (NATextField*)winapitextfield;
}



NA_DEF void naDestructTextField(NATextField* textfield){
 NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
 naClearCoreTextField(&(winapitextfield->coretextfield));
}



NA_DEF void naSetTextFieldText(NATextField* textfield, const NAUTF8Char* text){
 NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
 TCHAR* systemtext = naAllocSystemStringWithUTF8String(text);
 naBlockUIElementNotifications(&(winapitextfield->coretextfield.uielement));
 SendMessage(naGetUIElementNativeID(textfield), WM_SETTEXT, 0, (LPARAM)systemtext);
 naAllowUIElementNotifications(&(winapitextfield->coretextfield.uielement));
 naFree(systemtext);
}



NA_DEF NAString* naNewStringWithTextFieldText(NATextField* textfield){
 LRESULT textlength = SendMessage(naGetUIElementNativeID(textfield), WM_GETTEXTLENGTH, 0, 0);
 if(textlength){
 TCHAR* buffer = naMalloc((textlength + 1) * sizeof(TCHAR));
 SendMessage(naGetUIElementNativeID(textfield), WM_GETTEXT, textlength + 1, (LPARAM)buffer);
 return naNewStringFromSystemString(buffer);
 }else{
 return naNewString();
 }
}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textfield, NATextAlignment alignment){
 long style = (long)GetWindowLongPtr(naGetUIElementNativeID(textfield), GWL_STYLE);
 style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
 SetWindowLongPtr(naGetUIElementNativeID(textfield), GWL_STYLE, style);
}



NA_DEF void naSetTextFieldFontKind(NATextField* textfield, NAFontKind kind){
 NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
 SendMessage(naGetUIElementNativeID(winapitextfield), WM_SETFONT, (WPARAM)getFontWithKind(kind), MAKELPARAM(TRUE, 0));
}



NA_HDEF NAUIElement** naGetTextFieldNextTabReference(NATextField* textfield){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
  return &(winapitextfield->nextTabStop);
}



NA_HDEF NAUIElement** naGetTextFieldPrevTabReference(NATextField* textfield){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
  return &(winapitextfield->prevTabStop);
}



NA_HDEF NARect naGetTextFieldAbsoluteInnerRect(NACoreUIElement* textfield){
 NA_UNUSED(textfield);
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
