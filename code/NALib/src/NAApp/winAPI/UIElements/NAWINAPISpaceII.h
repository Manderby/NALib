
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  RECT spaceRect;
  NA_UIElement* childElement;
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)uiElement;
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAColor fgColor;
  NAColor bgColor;
  NAWINAPIColor* winapiFgColor;

  switch(message) {
  case WM_SHOWWINDOW:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_SETTEXT:
  case WM_GETFONT:
  case WM_SETFONT:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_PARENTNOTIFY:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_IME_NOTIFY:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_LBUTTONUP:
  case WM_CHAR:
  case 0x43c: // undocumented
  case WM_NCCALCSIZE:
  case 0x14e: // undocumented
  case 0x405: // undocumented
  case WM_ENABLE:
  case WM_NCMOUSEMOVE:
  case 0xf2: // undocumented
  case 0xf3: // undocumented
  case WM_TIMER:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_COMMAND:
  case WM_CTLCOLORLISTBOX:
  case 0x131: // undocumented
  case CB_GETCURSEL:
  break;

  case WM_SIZE: // todo: never called? why handeled?
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // Do not change the default behaviour of focus. Otherwise, this would cause
    // labels and textFields to not display a cursor or selection.
    info = naUIElementWINAPIDefaultProc(naGetUIElementNativePtr(uiElement), message, wParam, lParam);
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

  case WM_CTLCOLORBTN: // Button
    {
      const NA_UIElement* uiElement = na_GetUINALibEquivalent((void*)lParam);
      if(naGetUIElementType(uiElement) == NA_UI_BUTTON) {
        const NAButton* button = (const NAButton*)uiElement;
        if(naIsButtonBordered(button) && naIsButtonStateful(button) && naGetButtonState(button)) {
          // we choose yellow as background as this is probably the last color ever
          // being used as a system UI style.
          info.result = (LRESULT)CreateSolidBrush(RGB(255, 255, 0)); // todo: memory leak.
          info.hasBeenHandeled = NA_TRUE;
        }else{
          info.result = (LRESULT)winapiSpace->curBgColor->brush;
          info.hasBeenHandeled = NA_TRUE;
        }
      }else{
        #if NA_DEBUG
          naError("Unexpected ui element type");
        #endif
        info.result = (LRESULT)CreateSolidBrush(RGB(255, 128, 0));
        info.hasBeenHandeled = NA_TRUE;
      }
    }
    break;

  case WM_PRINTCLIENT: // wParam = HDC
    GetClientRect(naGetUIElementNativePtr(uiElement), &spaceRect);
    FillRect((HDC)wParam, &spaceRect, winapiSpace->curBgColor->brush);
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_CTLCOLOREDIT: // TextBox
    //childElement = (NA_UIElement*)na_GetUINALibEquivalent((HWND)lParam);
    //SetTextColor((HDC)wParam, RGB(255, 128, 0));
    //bgColor = naGetWINAPISpaceBackgroundColor(uiElement);
    //SetBkMode( (HDC)wParam, TRANSPARENT ); 
    //SetBkColor((HDC)wParam, bgColor->color);
    //info.result = (LRESULT)bgColor->brush;
    //info.hasBeenHandeled = NA_TRUE;
    break;

  // Note that an NALabel is declared as an "EDIT" control but as it is
  // marked with "ES_READONLY", it answers to WM_CTLCOLORSTATIC rathern than
  // WM_CTLCOLOREDIT.

  case WM_CTLCOLORSTATIC: // Label, TextField, Radio, CheckBox, Select
  // Message is sent to parent space.
    // wParam: device
    // lParam HWND handle to actual control
    // return: background color brush
    childElement = (NA_UIElement*)na_GetUINALibEquivalent((HWND)lParam);
    switch(childElement->elementType) {
    case NA_UI_LABEL:
      naFillLabelTextColor(&fgColor, (NALabel*)childElement);
      if(winapiSpace->curBgColor) { naDeallocUIColor(winapiSpace->curBgColor); }
      naFillSpaceBackgroundColor(&bgColor, uiElement);
      winapiSpace->curBgColor = naAllocUIColor(&bgColor, NA_NULL);
      winapiFgColor = naAllocUIColor(&fgColor, &bgColor);
      SetTextColor((HDC)wParam, winapiFgColor->colorRef);
      naDeallocUIColor(winapiFgColor);
      SetBkColor((HDC)wParam, winapiSpace->curBgColor->colorRef);
      info.result = (LRESULT)winapiSpace->curBgColor->brush;
      info.hasBeenHandeled = NA_TRUE;
      break;
    case NA_UI_TEXTFIELD:
      // Background of a textfield or a textbox is really the back of the text,
      // not the surrounding. So, leave it as it is.
      break;
    default:
      // Radio, CheckBox, Select
      info.result = (LRESULT)winapiSpace->curBgColor->brush;
      info.hasBeenHandeled = NA_TRUE;
      break;
    }
    break;

  case WM_ERASEBKGND: // wParam: Device context, return != 0 if erasing, 0 otherwise
    GetClientRect(naGetUIElementNativePtr(uiElement), &spaceRect);
    if(winapiSpace->forceEraseBackground) {
      if(winapiSpace->curBgColor) { naDeallocUIColor(winapiSpace->curBgColor); }
      naFillSpaceBackgroundColor(&bgColor, uiElement);
      winapiSpace->curBgColor = naAllocUIColor(&bgColor, NA_NULL);
      FillRect((HDC)wParam, &spaceRect, winapiSpace->curBgColor->brush);
      winapiSpace->forceEraseBackground = NA_FALSE;

      NAListIterator iter = naMakeListModifier(&winapiSpace->space.childs);
      while(naIterateList(&iter)) {
        NA_UIElement* elem = naGetListCurMutable(&iter);
        if(elem->elementType == NA_UI_SPACE) {
          NAWINAPISpace* winapiSpace = (NAWINAPISpace*)elem;
          winapiSpace->forceEraseBackground = NA_TRUE;
          naRefreshUIElement(elem, 0);
        }
      }
      naClearListIterator(&iter);

      info.result = 1;
    }else{
      info.result = 0;
    }
    info.hasBeenHandeled = NA_TRUE;
    break;

  default:
    //printf("Uncaught Space message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF NASpace* naNewSpace(NASize size) {
  NAWINAPISpace* winapiSpace = naNew(NAWINAPISpace);

  winapiSpace->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

	HWND nativePtr = CreateWindow(
		TEXT("NASpace"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE,
    0,
    0,
    (int)(winapiSpace->rect.size.width * uiScale),
    (int)(winapiSpace->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  na_InitSpace(&winapiSpace->space, nativePtr);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  winapiSpace->curBgColor = NA_NULL;

  winapiSpace->forceEraseBackground = NA_FALSE;

  winapiSpace->space.alternateBackground = NA_FALSE;

  return (NASpace*)winapiSpace;
}



NA_DEF void na_DestructWINAPISpace(NAWINAPISpace* winapiSpace) {
  if(winapiSpace->curBgColor) { naDeallocUIColor(winapiSpace->curBgColor); }
  na_ClearSpace((NASpace*)winapiSpace);
}



NA_DEF void naAddSpaceChild(NASpace* space, void* child, NAPos pos) {
  na_AddSpaceChild(space, child);
  double offsetY = na_GetUIElementYOffset(child);

  if(naGetUIElementType(child) == NA_UI_SPACE) {
    NAWINAPISpace* childElem = (NAWINAPISpace*)child;
    childElem->forceEraseBackground = NA_TRUE;
  }

  NARect childRect = naGetUIElementRect(child);
  childRect.pos = naMakePos(pos.x, pos.y + offsetY);
  naSetUIElementRect(child, childRect);
}



NA_DEF void naSetSpaceBackgroundColor(NASpace* space, const NAColor* color) {
  na_SetSpaceBackgroundColor(space, color);
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;
  winapiSpace->forceEraseBackground = NA_TRUE;
  naRefreshUIElement(space, 0);
}



NA_DEF void naSetSpaceAlternateBackground(NASpace* space, NABool alternate) {
  space->alternateBackground = alternate;
  ((NAWINAPISpace*)space)->forceEraseBackground = NA_TRUE;
  naRefreshUIElement(space, 0.);
}



NA_DEF void naRemoveSpaceChild(NASpace* space, void* child) {
  NAListIterator iter = naMakeListModifier(&space->childs);
  NABool found = naLocateListData(&iter, child);
  naClearListIterator(&iter);
  if(found) {
    na_RemoveSpaceChild(space, child);
    ((NAWINAPISpace*)space)->forceEraseBackground = NA_TRUE;
  }else{
    #if NA_DEBUG
    naError("Child UI element not found in given space.");
    #endif
  }
}



NA_DEF void naRemoveAllSpaceChilds(NASpace* space) {
  while(!naIsListEmpty(&space->childs)) {
    void* child = naGetListFirstMutable(&space->childs);
    na_RemoveSpaceChild(space, child);
  }
  ((NAWINAPISpace*)space)->forceEraseBackground = NA_TRUE;
}



NA_DEF void naShiftSpaceChilds(NASpace* space, NAPos shift) {
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;

  NAListIterator childIt = naMakeListMutator(&space->childs);
  while(naIterateList(&childIt)) {
    void* child = naGetListCurMutable(&childIt);
    NARect elementRect = naGetUIElementRect(child);
    elementRect.pos.x += shift.x;
    elementRect.pos.y += shift.y;
    naSetUIElementRect(child, elementRect);
  }
  naClearListIterator(&childIt);
}



NA_DEF void naSetSpaceVisible(NASpace* space, NABool visible) {
  ShowWindow(naGetUIElementNativePtr(space), visible ? SW_SHOW : SW_HIDE);
}



NA_HDEF void naSetSpaceDragsWindow(NASpace* space, NABool isDraggable) {
  // todo
}



NA_HDEF NARect na_GetSpaceRect(const NA_UIElement* space) {
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;
  return winapiSpace->rect;
}



NA_HDEF void na_SetSpaceRect(NA_UIElement* space, NARect rect) {
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;

  double prevHeight = winapiSpace->rect.size.height;
  winapiSpace->rect = rect;
  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(space));

  double test = parentRect.size.height - winapiSpace->rect.pos.y - winapiSpace->rect.size.height;

  winapiSpace->forceEraseBackground = NA_TRUE;

  SetWindowPos(
    naGetUIElementNativePtr(space),
    HWND_TOP,
    (int)(winapiSpace->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiSpace->rect.pos.y - winapiSpace->rect.size.height) * uiScale),
    (int)(winapiSpace->rect.size.width * uiScale),
    (int)(winapiSpace->rect.size.height * uiScale),
    SWP_NOZORDER);

  // We need to trigger a repositioning because in WinAPI, the coordinates
  // are computed from the top left instead of the bottom right. Simply
  // setting the rect anew does the trick.
  if(prevHeight != rect.size.height) {
    naShiftSpaceChilds(&winapiSpace->space, naMakePos(0., 0.));
  }
}



NA_HDEF void na_ForceWINAPISpaceToEraseBackground(NASpace* space) {
  NAWINAPISpace* winapiSpace = (NAWINAPISpace*)space;
  winapiSpace->forceEraseBackground = NA_TRUE;
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
