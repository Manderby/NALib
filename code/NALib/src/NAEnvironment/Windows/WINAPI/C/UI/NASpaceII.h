
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPISpace NAWINAPISpace;
struct NAWINAPISpace {
  NASpace        space;
  NAWINAPIColor* lastBgColor;
};

NA_HAPI void na_DestructWINAPISpace(NAWINAPISpace* winapiSpace);
NA_RUNTIME_TYPE(NAWINAPISpace, na_DestructWINAPISpace, NA_FALSE);


NAWINAPIColor* naGetWINAPISpaceBackgroundColor(NAWINAPISpace* winapiSpace);



NAWINAPICallbackInfo naSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  RECT spacerect;
  NA_UIElement* childelement;
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)uiElement;
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAWINAPIColor* bgColor;

  switch(message){
  case WM_SHOWWINDOW:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_PRINTCLIENT:
  case WM_CTLCOLORBTN:
  case WM_CTLCOLOREDIT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_SETTEXT:
  case WM_GETFONT:
  case WM_SETFONT:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_MOUSEMOVE: // captured in naUIElementWINAPIProc
  case WM_MOUSELEAVE: // captured in naUIElementWINAPIProc
  case WM_PARENTNOTIFY:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_IME_NOTIFY:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_LBUTTONUP:
  case WM_KEYDOWN:
  case WM_CHAR:
  case WM_KEYUP:
  case 0x43c: // undocumented
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // labels and textfields to not display a cursor or selection.
    break;

  // Dialog messages
  case WM_UPDATEUISTATE:
  case WM_GETDLGCODE:
  case WM_CHANGEUISTATE:

  // Cases being called due to bubbling the message.
  case BM_SETCHECK:
  case EM_GETSEL:
  case EM_GETRECT:
  case EM_CHARFROMPOS:
  case EM_LINEFROMCHAR:
  case EM_POSFROMCHAR:
  case EM_SETSEL:
    break;

  case WM_CTLCOLORSTATIC:
    // Message is sent to parent space.
    // wParam: device context
    // lParam HWND handle to actual control
    // return: background color brush
    childelement = (NA_UIElement*)na_GetUINALibEquivalent((HWND)lParam);
    switch(childelement->elementType){
    case NA_UI_LABEL:
      if(naIsLabelEnabled((NALabel*)childelement)){
        SetTextColor((HDC)wParam, app->fgColor.color);
      }else{
        SetTextColor((HDC)wParam, app->fgColorDisabled.color);
      }
      bgColor = naGetWINAPISpaceBackgroundColor(uiElement);
      SetBkColor((HDC)wParam, bgColor->color);
      info.result = (LRESULT)bgColor->brush;
      info.hasBeenHandeled = NA_TRUE;
      break;
    }
    break;

  case WM_ERASEBKGND: // wParam: Device context, return > 1 if erasing, 0 otherwise
    GetClientRect(naGetUIElementNativePtr(uiElement), &spacerect);
    bgColor = naGetWINAPISpaceBackgroundColor(uiElement);
    if(bgColor != winapiSpace->lastBgColor){ // Only draw if changed
      FillRect((HDC)wParam, &spacerect, bgColor->brush);
      winapiSpace->lastBgColor = bgColor;
    }
    info.hasBeenHandeled = NA_TRUE;
    info.result = 1;
    break;

  default:
    //printf("Uncaught Space message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPIColor* naGetWINAPISpaceBackgroundColor(NAWINAPISpace* winapiSpace){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAWINAPIColor* retcolor;
  NAInt alternateLevel = 0;
  void* parent = winapiSpace;
  while(parent){
    if(naGetSpaceAlternateBackground(parent)){alternateLevel++;}
    parent = naGetUIElementParentSpace(parent);
  }
  switch(alternateLevel){
  case 0: retcolor = &(app->bgColor); break;
  case 1: retcolor = &(app->bgColorAlternate); break;
  case 2:
  default:
    retcolor = &(app->bgColorAlternate2); break;
  }
  return retcolor;
}



NA_DEF NASpace* naNewSpace(NASize size){
  NAWINAPISpace* winapiSpace = naNew(NAWINAPISpace);

	HWND nativePtr = CreateWindow(
		TEXT("NASpace"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE,
		0,
    0,
    (int)size.width,
    (int)size.height,
		naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  na_InitSpace(&(winapiSpace->space), nativePtr);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  winapiSpace->lastBgColor = &(app->bgColor);
  winapiSpace->space.alternatebackground = NA_FALSE;

  return (NASpace*)winapiSpace;
}



NA_DEF void na_DestructWINAPISpace(NAWINAPISpace* winapiSpace){
  na_ClearSpace((NASpace*)winapiSpace);
}



NA_DEF void naSetSpaceRect(NASpace* space, NARect rect){
  SetWindowPos(naGetUIElementNativePtr(space), HWND_TOP, 0, 0, (int)rect.size.width, (int)rect.size.height, SWP_NOMOVE | SWP_NOZORDER);
}



NA_DEF void naAddSpaceChild(NASpace* space, void* child, NAPos pos){
  RECT spacerect;
  RECT childrect;
  int spaceheight;
  int childheight;

  GetClientRect(naGetUIElementNativePtr(space), &spacerect);
  GetClientRect(naGetUIElementNativePtr(child), &childrect);
  spaceheight = spacerect.bottom - spacerect.top;
  childheight = childrect.bottom - childrect.top;
  SetWindowPos(naGetUIElementNativePtr(child), HWND_TOP, (int)pos.x, spaceheight - (int)pos.y - childheight, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  na_SetUIElementParent(child, space);
}



NA_HDEF NARect na_GetSpaceAbsoluteInnerRect(NA_UIElement* space){
  NARect rect;
  NARect screenRect;
  RECT contentRect;
  POINT testpoint = {0, 0};

  GetClientRect(space->nativePtr, &contentRect);
  ClientToScreen(space->nativePtr, &testpoint);
  screenRect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = (double)screenRect.size.height - ((double)testpoint.y + ((double)contentRect.bottom - (double)contentRect.top));
  rect.size.width = (double)contentRect.right - (double)contentRect.left;
  rect.size.height = (double)contentRect.bottom - (double)contentRect.top;
  return rect;
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate){
  space->alternatebackground = alternate;
  naRefreshUIElement(space, 0.);
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
