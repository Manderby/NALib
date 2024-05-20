
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naSelectWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_SHOWWINDOW:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEFIRST:
  case WM_MOUSELEAVE:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_IME_SETCONTEXT:
  case WM_SETFOCUS:
  case WM_KILLFOCUS:
  case WM_NCCALCSIZE:
  case WM_SIZE:
  case CB_INSERTSTRING:
  case CB_SETCURSEL:
  case WM_CTLCOLORLISTBOX:
  case WM_CAPTURECHANGED:
  case 0x131: // undocumented
  case WM_COMMAND:
  case CB_GETCURSEL:
    break;

  default:
    printf("Uncaught Select message" NA_NL);
    break;
  }
  
  return info;
}



NABool naSelectWINAPINotify(void* uiElement, WORD notificationCode){
  NAWINAPISelect* winapiSelect = (NAWINAPISelect*)uiElement;
  NABool hasBeenHandeled = NA_FALSE;
  size_t itemIndex;
  const NAMenuItem* child;

  switch(notificationCode){
  case CBN_SELCHANGE:
    itemIndex = SendMessage(
      naGetUIElementNativePtr(uiElement),
      CB_GETCURSEL, 
      0,
      0);
    child = naGetSelectItem(uiElement, itemIndex);
    na_DispatchUIElementCommand(&(child->uiElement), NA_UI_COMMAND_PRESSED);
    break;
  }
  return hasBeenHandeled;
}



NA_DEF NASelect* naNewSelect(double width){
#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  NAWINAPISelect* winapiSelect = naNew(NAWINAPISelect);

  TCHAR* systemText = naAllocSystemStringWithUTF8String("Select");

  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  winapiSelect->rect = naMakeRectS(0., 0., width, 24.);

  HWND nativePtr = CreateWindow(
    WC_COMBOBOX,
    systemText,
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS | WS_OVERLAPPED | CBS_SIMPLE,
    0,
    0,
    (int)(winapiSelect->rect.size.width * uiScale),
    (int)(winapiSelect->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  SendMessage(nativePtr, WM_SETFONT, (WPARAM)naGetFontNativePointer(naGetSystemFont()), MAKELPARAM(TRUE, 0));

  naFree(systemText);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldSelectWindowProc){
    app->oldSelectWindowProc = oldproc;
  }

  na_InitSelect((NASelect*)winapiSelect, nativePtr);

  return (NASelect*)winapiSelect;
#else
  // not a very good cast but things are broken in such a situation anyway.
  return (NASelect*)naNewLabel("Slider error", width);
#endif
}

NA_DEF void na_DestructWINAPISelect(NAWINAPISelect* winapiSelect){
  na_ClearSelect((NASelect*)winapiSelect);
}

NA_DEF void naSetSelectVisible(NASelect* select, NABool visible){
  ShowWindow(naGetUIElementNativePtr(select), visible ? SW_SHOW : SW_HIDE);
}

NA_DEF void naSetSelectEnabled(NASelect* select, NABool enabled){
  EnableWindow(naGetUIElementNativePtr(select), enabled);
}

NA_DEF void naAddSelectMenuItem(NASelect* select, NAMenuItem* item, const NAMenuItem* atItem){
  NAWINAPISelect* winapiSelect = (NAWINAPISelect*)select;
  
  // todo: allow separators. Will crash currently.
  TCHAR* itemText = naAllocSystemStringWithUTF8String(naGetMenuItemText(item));
  size_t index = naGetSelectItemIndex(select, atItem);

  SendMessage(naGetUIElementNativePtr(select), (UINT)CB_INSERTSTRING , (WPARAM)index, (LPARAM) itemText);
  if(naGetListCount(&(select->childs)) == 0){
    SendMessage(naGetUIElementNativePtr(select), CB_SETCURSEL, (WPARAM)index , (LPARAM)0);
  }

  na_AddSelectChild(select, item, atItem);
}

NA_DEF void naSetSelectIndexSelected(NASelect* select, size_t index){
  SendMessage(naGetUIElementNativePtr(select), CB_SETCURSEL, (WPARAM)index , (LPARAM)0);
}

NA_DEF void naSetSelectItemSelected(NASelect* select, const NAMenuItem* item){
  size_t index = naGetSelectItemIndex(select, item);
  SendMessage(naGetUIElementNativePtr(select), CB_SETCURSEL, (WPARAM)index , (LPARAM)0);
}

NA_HDEF NARect na_GetSelectRect(const NA_UIElement* select)
{
  const NAWINAPISelect* winapiSelect = (const NAWINAPISelect*)select;
  return winapiSelect->rect;
}

NA_HDEF void na_SetSelectRect(NA_UIElement* select, NARect rect){
  NAWINAPISelect* winapiSelect = (NAWINAPISelect*)select;

  winapiSelect->rect = rect;
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(select));

  SetWindowPos(
    naGetUIElementNativePtr(select),
    HWND_TOP,
    (int)(winapiSelect->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiSelect->rect.pos.y - winapiSelect->rect.size.height) * uiScale),
    (int)(winapiSelect->rect.size.width * uiScale),
    (int)(winapiSelect->rect.size.height * uiScale),
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
