
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPITextField NAWINAPITextField;
struct NAWINAPITextField {
  NACoreTextField coretextfield;
};



NAWINAPICallbackInfo naTextFieldWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:            //printf("WM_SETFONT "); break;
  case WM_STYLECHANGING:      //printf("WM_STYLECHANGING "); break;
  case WM_STYLECHANGED:       //printf("WM_STYLECHANGED "); break;
  case WM_WINDOWPOSCHANGING:  //printf("WM_WINDOWPOSCHANGING "); break;
  case WM_CHILDACTIVATE:      //printf("WM_CHILDACTIVATE "); break;
  case WM_WINDOWPOSCHANGED:   //printf("WM_WINDOWPOSCHANGED "); break;
  case WM_MOVE:               //printf("WM_MOVE "); break;
  case WM_SHOWWINDOW:         //printf("WM_SHOWWINDOW "); break;
  case WM_PAINT:              //printf("WM_PAINT "); break;
  case WM_NCPAINT:            //printf("WM_NCPAINT "); break;
  case WM_ERASEBKGND:         //printf("WM_ERASEBKGND "); break;
  case WM_NCHITTEST:          //printf("WM_NCHITTEST "); break;
  case WM_SETCURSOR:          //printf("WM_SETCURSOR "); break;
  case WM_MOUSEACTIVATE:      //printf("WM_MOUSEACTIVATE "); break;
  case WM_LBUTTONDOWN:        //printf("WM_LBUTTONDOWN "); break;
  case WM_IME_SETCONTEXT:     //printf("WM_IME_SETCONTEXT "); break;
  case WM_SETFOCUS:           //printf("WM_SETFOCUS "); break;
  case WM_IME_NOTIFY:         //printf("WM_IME_NOTIFY "); break;
  case WM_CANCELMODE:         //printf("WM_CANCELMODE "); break;
  case WM_CAPTURECHANGED:     //printf("WM_CAPTURECHANGED "); break;
  case WM_KILLFOCUS:          //printf("WM_KILLFOCUS "); break;
  case WM_GETTEXTLENGTH:      //printf("WM_GETTEXTLENGTH "); break;
  case EM_GETSEL:             //printf("EM_GETSEL "); break;
  case EM_GETRECT:            //printf("EM_GETRECT "); break;
  case EM_CHARFROMPOS:        //printf("EM_CHARFROMPOS "); break;
  case WM_GETFONT:            //printf("WM_GETFONT "); break;
  case EM_LINEFROMCHAR:       //printf("EM_LINEFROMCHAR "); break;
  case EM_POSFROMCHAR:        //printf("EM_POSFROMCHAR "); break;
  case WM_LBUTTONUP:          //printf("WM_LBUTTONUP "); break;
  case 0x43C:                 //printf("0x43C "); break;
  case WM_GETTEXT:            //printf("WM_GETTEXT "); break;
  case WM_SETTEXT:            //printf("WM_SETTEXT "); break;
  case EM_LINELENGTH:         //printf("EM_LINELENGTH "); break;
  case WM_KEYUP:              //printf("WM_KEYUP "); break;
  case WM_SYSKEYUP:           //printf("WM_SYSKEYUP "); break;
  case WM_CHAR:               //printf("WM_CHAR "); break;
  case WM_KEYDOWN:  // capture enter and tab here.
  case WM_SYSKEYDOWN: // the alt key!

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



NA_DEF NATextField* naNewTextField(NASize size){
  HWND hWnd;
  DWORD style;
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPITextField* winapitextfield = naAlloc(NAWINAPITextField);

  style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;

	hWnd = CreateWindow(
		TEXT("EDIT"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldTextFieldWindowProc){app->oldTextFieldWindowProc = oldproc;}

  naInitCoreTextField(&(winapitextfield->coretextfield), hWnd);

  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NATextField*)winapitextfield;
}



NA_DEF void naDestructTextField(NATextField* textfield){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
  naClearCoreTextField(&(winapitextfield->coretextfield));
}



NA_DEF void naSetTextFieldText(NATextField* textfield, const NAUTF8Char* text){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
  TCHAR* systemtext = naAllocSystemStringWithUTF8String(text, 0);
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
    return naNewStringFromSystemString(buffer, (NAUInt)textlength);
  }else{
    return naNewString();
  }
}



NA_DEF void naSetTextFieldTextAlignment(NATextField* textfield, NATextAlignment alignment){
  long style = GetWindowLongPtr(naGetUIElementNativeID(textfield), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | getWINAPITextAlignmentWithAlignment(alignment);
  SetWindowLongPtr(naGetUIElementNativeID(textfield), GWL_STYLE, style);
}



NA_DEF void naSetTextFieldFontKind(NATextField* textfield, NAFontKind kind){
  NAWINAPITextField* winapitextfield = (NAWINAPITextField*)textfield;
  SendMessage(naGetUIElementNativeID(winapitextfield), WM_SETFONT, (WPARAM)getFontWithKind(kind), MAKELPARAM(TRUE, 0));
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
