
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


typedef struct NAWINAPIButton NAWINAPIButton;
struct NAWINAPIButton {
  NACoreButton corebutton;
  NAUIImage* image;
  NABool transparent;
};



NAWINAPICallbackInfo naButtonWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SETFONT:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_KILLFOCUS:
  case WM_IME_NOTIFY:
  case WM_LBUTTONUP:
  case BM_SETSTATE:
  case BM_SETCHECK:
  case WM_SETFOCUS:
    break;

  default:
    //printf("Uncaught Button message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naButtonWINAPINotify(NAUIElement* uielement, WORD notificationCode){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  switch(notificationCode){
    case BN_CLICKED:
      naDispatchUIElementCommand(uielement, NA_UI_COMMAND_PRESSED);
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
      break;
    default:
      //printf("Uncaught Button notification" NA_NL);
      break;
  }
  return info;
}



NAWINAPICallbackInfo naButtonWINAPIDrawItem (NAUIElement* uielement, DRAWITEMSTRUCT* drawitemstruct){
  HBITMAP hOldBitmap;
  NASizei size1x;
  NASizei buttonsize;
  NAPosi offset;
  NABabyImage* foreImage;
  NAByte* backBuffer;
  HBITMAP hBackBitmap;
  NABabyImage* backImage;
  NABabyImage* blendedImage;
  NAByte* blendedBuffer;
  HBITMAP hBlendedBitmap;

  HDC hMemDC = CreateCompatibleDC(drawitemstruct->hDC);

  NAWINAPIButton* button = (NAWINAPIButton*)uielement;
  NAWINAPICallbackInfo info = {NA_TRUE, TRUE};

  CallWindowProc(naGetApplicationOldButtonWindowProc(), naGetUIElementNativeID(uielement), WM_ERASEBKGND, (WPARAM)drawitemstruct->hDC, (LPARAM)NA_NULL);

  if(!button->transparent){
    long oldstyle = (long)GetWindowLongPtr(naGetUIElementNativeID(uielement), GWL_STYLE);
    long newstyle = (oldstyle & ~BS_OWNERDRAW) | BS_TEXT | BS_CENTER | BS_VCENTER;
    SetWindowLongPtr(naGetUIElementNativeID(uielement), GWL_STYLE, (LONG_PTR)newstyle);
    // Oh boi. That is one hell of a hidden feature. Usually, the WM_PAINT message does not
    // use wParam and lParam at all. But there are some common controls (and buttons seems to
    // be one of them) which in fact only work if you send the device context in wParam.
    CallWindowProc(naGetApplicationOldButtonWindowProc(), naGetUIElementNativeID(uielement), WM_PAINT, (WPARAM)drawitemstruct->hDC, (LPARAM)NA_NULL);
    SetWindowLongPtr(naGetUIElementNativeID(uielement), GWL_STYLE, (LONG_PTR)oldstyle);
  }

  size1x = naGetUIImage1xSize(button->image);

  buttonsize = naMakeSizei(
    drawitemstruct->rcItem.right - drawitemstruct->rcItem.left,
    drawitemstruct->rcItem.bottom - drawitemstruct->rcItem.top);
  offset = naMakePosi(
    (buttonsize.width - size1x.width) / 2,
    (buttonsize.height - size1x.height) / 2);

  foreImage = naGetUIImageBabyImage(button->image, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_LIGHT);

  // We store the background where the image will be placed.
  backBuffer = naMalloc(size1x.width * size1x.height * 4);
  hBackBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, backBuffer);
  hOldBitmap = SelectObject(hMemDC, hBackBitmap);
  BitBlt(hMemDC, 0, 0, (int)size1x.width, (int)size1x.height, drawitemstruct->hDC, (int)offset.x, (int)offset.y, SRCCOPY);
  backImage = naCreateBabyImageFromNativeImage(hBackBitmap);

  // Now we blend manually the foreground to the background.
  blendedImage = naCreateBabyImageWithBlend(backImage, foreImage, NA_BLEND_OVERLAY, 1.f);
  blendedBuffer = naMalloc(size1x.width * size1x.height * 4);
  naConvertBabyImageTou8(blendedImage, blendedBuffer, NA_TRUE, NA_COLOR_BUFFER_BGR0);
  hBlendedBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, blendedBuffer);

  // Finally, we put the blended image onscreen.
  SelectObject(hMemDC, hBlendedBitmap);
  BitBlt(drawitemstruct->hDC, (int)offset.x, (int)offset.y, (int)size1x.width, (int)size1x.height, hMemDC, 0, 0, SRCCOPY);
  SelectObject(hMemDC, hOldBitmap);

  // Deleting the blended objects and buffers
  DeleteObject(hBlendedBitmap);
  naFree(blendedBuffer);
  naReleaseBabyImage(blendedImage);

  // Deleting background objects and buffers
  DeleteObject(hBackBitmap);
  naFree(backBuffer);
  naReleaseBabyImage(backImage);

  // Deleting device contexts
  DeleteDC(hMemDC);
  ReleaseDC(drawitemstruct->hwndItem, drawitemstruct->hDC);

  return info;
}



NA_DEF NAButton* naNewPushButton(const NAUTF8Char* text, NASize size){
  HWND hWnd;
  DWORD style;
  TCHAR* systemtext;
  WNDPROC oldproc;

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPIButton* winapibutton = naAlloc(NAWINAPIButton);

  style = WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON;

  systemtext = naAllocSystemStringWithUTF8String(text);

	hWnd = CreateWindow(
		TEXT("BUTTON"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL);
  
  naFree(systemtext);

  oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  naInitCoreButton(&(winapibutton->corebutton), hWnd);
  winapibutton->image = NA_NULL;

  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NAButton*)winapibutton;
}



NA_DEF NAButton* naNewTextOptionButton(const NAUTF8Char* text, NASize size){
  HWND hWnd;
  DWORD style;
  TCHAR* systemtext;

  NAWINAPIButton* winapibutton = naAlloc(NAWINAPIButton);

  style = WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON;

  systemtext = naAllocSystemStringWithUTF8String(text);

	hWnd = CreateWindow(
		TEXT("BUTTON"), systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naFree(systemtext);

  naInitCoreButton(&(winapibutton->corebutton), hWnd);
  winapibutton->image = NA_NULL;

  SendMessage(hWnd, WM_SETFONT, (WPARAM)getFontWithKind(NA_FONT_KIND_SYSTEM), MAKELPARAM(TRUE, 0));

  return (NAButton*)winapibutton;
}



NA_DEF NAButton* naNewImageOptionButton(NAUIImage* uiimage, NASize size){
  HWND hWnd;
  DWORD style;

  NAWINAPIButton* winapibutton = naAlloc(NAWINAPIButton);

  style = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON;

	hWnd = CreateWindow(
		TEXT("BUTTON"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naInitCoreButton(&(winapibutton->corebutton), hWnd);
  winapibutton->image = uiimage;
  winapibutton->transparent = NA_FALSE;

  return (NAButton*)winapibutton;
}



NA_DEF NAButton* naNewImageButton(NAUIImage* uiimage, NASize size){
  HWND hWnd;
  DWORD style;

  NAWINAPIButton* winapibutton = naAlloc(NAWINAPIButton);

  style = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW;

	hWnd = CreateWindow(
		TEXT("BUTTON"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naInitCoreButton(&(winapibutton->corebutton), hWnd);
  winapibutton->image = uiimage;
  winapibutton->transparent = NA_TRUE;

  return (NAButton*)winapibutton;
}



NA_DEF void naDestructButton(NAButton* button){
  NAWINAPIButton* winapibutton = (NAWINAPIButton*)button;
  naClearCoreButton(&(winapibutton->corebutton));
}



NA_HDEF void naSetButtonState(NAButton* button, NABool state){
  NAWINAPIButton* winapibutton = (NAWINAPIButton*)button;
  SendMessage(naGetUIElementNativeID(winapibutton), BM_SETSTATE, (WPARAM)state, (LPARAM)NA_NULL);
}



NA_HDEF void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, NAUIElement* controller){
  NAWINAPIButton* winapibutton = (NAWINAPIButton*)button;
  long style = (long)GetWindowLongPtr(naGetUIElementNativeID(winapibutton), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | BS_DEFPUSHBUTTON;
  SetWindowLongPtr(naGetUIElementNativeID(winapibutton), GWL_STYLE, (LONG_PTR)style);
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER), handler, controller);
}



NA_HDEF void naSetButtonAbort(NAButton* button, NAReactionHandler handler, NAUIElement* controller){
  naAddUIKeyboardShortcut(naGetUIElementWindow(button), naMakeKeybardStatus(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ESC), handler, controller);
}



NA_HDEF NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* button){
  NA_UNUSED(button);
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
