
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



NAWINAPICallbackInfo naSliderWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NA_UNUSED(lParam);
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NASlider* slider = (NASlider*)uiElement;
  const NASpace* space;
  NAColor bgColor;
  NAWINAPIColor* bgWinapiColor;
  RECT sliderRect;

  switch(message) {
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_NCPAINT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_IME_SETCONTEXT:
  case WM_DESTROY:
  case WM_NCDESTROY:
  case WM_CAPTURECHANGED:
  case WM_IME_NOTIFY:
  case WM_TIMER:
  case WM_CANCELMODE:
  case WM_UPDATEUISTATE:
  case WM_ENABLE:
  case WM_MOUSEFIRST:
  case WM_MOUSEWHEEL:
  case WM_KEYFIRST:
  case WM_WINDOWPOSCHANGED:

#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  case TBM_SETPOS:  // (WM_USER + 5)
  case TBM_GETPOS: // (WM_USER + 0)
  case TBM_GETTHUMBRECT:
  case TBM_GETTHUMBLENGTH:
#endif

    break;

  case WM_PAINT:
    space = naGetUIElementParentSpace(uiElement);
    GetClientRect(naGetUIElementNativePtr(uiElement), &sliderRect);

    naFillSpaceBackgroundColor(&bgColor, space);
    bgWinapiColor = naAllocUIColor(&bgColor, NA_NULL);
    FillRect((HDC)wParam, &sliderRect, bgWinapiColor->brush);
    naDeallocUIColor(bgWinapiColor);
    break;

  case WM_ERASEBKGND: // wParam: Device context, return != 0 if erasing, 0 otherwise
    //space = naGetUIElementParentSpace(uiElement);
    //GetClientRect(naGetUIElementNativePtr(uiElement), &sliderRect);

    //naFillSpaceBackgroundColor(&bgColor, space);
    //bgWinapiColor = naAllocUIColor(&bgColor);
    //FillRect((HDC)wParam, &sliderRect, bgWinapiColor->brush);
    //naDeallocUIColor(bgWinapiColor);

    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_LBUTTONDOWN:
    slider->sliderInMovement = NA_TRUE;
    info.result = 0;
    break;

  case WM_LBUTTONUP:
    slider->sliderInMovement = NA_TRUE;
    info.result = 0;
    break;

  default:
    //printf("Uncaught Slider message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naSliderWINAPIScroll(void* uiElement, WPARAM wParam) {
  NA_UNUSED(wParam);
  NAWINAPICallbackInfo info = {NA_TRUE, 0};

  naSetSliderValue(uiElement, naGetSliderValue(uiElement));
  if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_EDITED)) {
    // no parent method to be called.
  }

  return info;
}



NA_DEF NASlider* naNewSlider(double width) {
#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  NAWINAPISlider* winapiSlider = naNew(NAWINAPISlider);

  double uiScale = naGetUIElementUIScale(NA_NULL);
  winapiSlider->rect = naMakeRectS(0., 0., width, 24.);

  // TBS_TRANSPARENTBKGND is necessary to call WM_PRINTCLIENT of the parent
  // space.

	HWND nativePtr = CreateWindow(
    TRACKBAR_CLASS,
    NULL,
    WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_TRANSPARENTBKGND,
    0,
    0,
    (LONG)(winapiSlider->rect.size.width * uiScale),
    (LONG)(winapiSlider->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);
  
  SendMessage(nativePtr, TBM_SETRANGEMIN , 
    (WPARAM) TRUE,
    (LPARAM) NA_ZERO_u32);
  SendMessage(nativePtr, TBM_SETRANGEMAX, 
    (WPARAM) TRUE,
    (LPARAM) NA_MAX_i32);

  //SendMessage(hWnd, TBM_SETPAGESIZE, 
  //    0, (LPARAM) 4);                  // new page size 

  //SendMessage(hWnd, TBM_SETSEL, 
  //    (WPARAM) FALSE,                  // redraw flag 
  //    (LPARAM) MAKELONG(0x3000, 0x7000)); 
  //      

  //SetFocus(hWnd); 

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldSliderWindowProc) {
    app->oldSliderWindowProc = oldproc;
  }

  na_InitSlider(&winapiSlider->slider, nativePtr);

  return (NASlider*)winapiSlider;
#else
  // not a very good cast but things are broken in such a situation anyway.
  return naNewLabel("Slider error", width);
#endif
}



NA_DEF void na_DestructWINAPISlider(NAWINAPISlider* winapiSlider) {
  na_ClearSlider((NASlider*)winapiSlider);
}



NA_DEF void naSetSliderVisible(NASlider* slider, NABool visible) {
  ShowWindow(naGetUIElementNativePtr(slider), visible ? SW_SHOW : SW_HIDE);
}



NA_DEF void naSetSliderEnabled(NASlider* slider, NABool enabled) {
  EnableWindow(naGetUIElementNativePtr(slider), enabled);
}



NA_API double naGetSliderValue(const NASlider* slider) {
#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  int32 sliderValue = (int32)SendMessage(naGetUIElementNativePtrConst(slider), TBM_GETPOS, 0, 0); 
  double plainValue = (double)sliderValue / (double)NA_MAX_i32;
  return plainValue * (slider->max - slider->min) + slider->min;
#else
  return 0.;
#endif
}



NA_DEF double naGetSliderStaticValue(const NASlider* slider) {
  return slider->staticValue;
}



NA_API void naSetSliderValue(NASlider* slider, double value) {
#if NA_USE_WINDOWS_COMMON_CONTROLS_6 == 1
  double plainValue = (value - slider->min) / (slider->max - slider->min);
  int32 sliderValue = (int32)(plainValue * (double)NA_MAX_i32);
  SendMessage(naGetUIElementNativePtr(slider), TBM_SETPOS, 
    (WPARAM) TRUE, // redraw flag 
    (LPARAM) sliderValue);
  if(!slider->sliderInMovement) {
    slider->staticValue = value;
  }
#endif
}



NA_API void naSetSliderRange(NASlider* slider, double min, double max, size_t tickCount) {

  SendMessage(naGetUIElementNativePtr(slider), TBM_CLEARTICS, TRUE, 0); // true = redraw
  if(tickCount) {
    SetWindowLongPtr(naGetUIElementNativePtr(slider), GWL_STYLE, WS_CHILD | WS_VISIBLE | TBS_TRANSPARENTBKGND);
    for(size_t i = 1; i < tickCount - 1; ++i) {
      SendMessage(naGetUIElementNativePtr(slider), TBM_SETTIC , 0, i * NA_MAX_i32 / (tickCount - 1));
    }
  }else{
    SetWindowLongPtr(naGetUIElementNativePtr(slider), GWL_STYLE, WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_TRANSPARENTBKGND);
  }

  slider->min = min;
  slider->max = max;
}



NA_HDEF NARect na_GetSliderRect(const NA_UIElement* slider)
{
  const NAWINAPISlider* winapiSlider = (const NAWINAPISlider*)slider;
  return winapiSlider->rect;
}

NA_HDEF void na_SetSliderRect(NA_UIElement* slider, NARect rect) {
  NAWINAPISlider* winapiSlider = (NAWINAPISlider*)slider;

  winapiSlider->rect = rect;
  double uiScale = naGetUIElementUIScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(slider));

  SetWindowPos(
    naGetUIElementNativePtr(slider),
    HWND_TOP,
    (LONG)(winapiSlider->rect.pos.x * uiScale),
    (LONG)((parentRect.size.height - winapiSlider->rect.pos.y - winapiSlider->rect.size.height) * uiScale),
    (LONG)(winapiSlider->rect.size.width * uiScale),
    (LONG)(winapiSlider->rect.size.height * uiScale),
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
