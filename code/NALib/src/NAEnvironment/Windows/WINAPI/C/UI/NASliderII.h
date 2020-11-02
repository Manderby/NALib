
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPISlider NAWINAPISlider;
struct NAWINAPISlider {
  NASlider slider;
};



NAWINAPICallbackInfo naSliderWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
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
  case WM_MOUSEMOVE:
  case WM_MOUSELEAVE:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_KILLFOCUS:
  case WM_IME_SETCONTEXT:
  case WM_DESTROY:
  case WM_NCDESTROY:
    break;

  default:
    printf("Uncaught Slider message" NA_NL);
    break;
  }
  
  return info;
}



NA_DEF NASlider* naNewSlider(NASize size){
  HWND hWnd;
  DWORD style;
  TCHAR* systemtext;
  WNDPROC oldproc;

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPISlider* winapiSlider = naAlloc(NAWINAPISlider);

  style = WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE;

  systemtext = naAllocSystemStringWithUTF8String("Slider");

	hWnd = CreateWindow(
		TRACKBAR_CLASS, systemtext, style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeId(naGetApplication()), NULL );
  
  naFree(systemtext);

  SendMessage(hWnd, TBM_SETRANGE, 
    (WPARAM) TRUE,
    (LPARAM) MAKELONG(NA_ZERO_u16, NA_MAX_u16));

  SendMessage(hWnd, TBM_SETPAGESIZE, 
      0, (LPARAM) 4);                  // new page size 

  SendMessage(hWnd, TBM_SETSEL, 
      (WPARAM) FALSE,                  // redraw flag 
      (LPARAM) MAKELONG(0x3000, 0x7000)); 
        
  SendMessage(hWnd, TBM_SETPOS, 
      (WPARAM) TRUE,                   // redraw flag 
      (LPARAM) 0x5000); 

  SetFocus(hWnd); 

  oldproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldRadioWindowProc){app->oldRadioWindowProc = oldproc;}

  na_InitSlider(&(winapiSlider->slider), hWnd);
  SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)&(winapiSlider->slider));

  return (NASlider*)winapiSlider;
}



NA_DEF void na_DestructSlider(NASlider* slider){
  NAWINAPISlider* winapiSlider = (NAWINAPISlider*)slider;
  na_ClearSlider(&(winapiSlider->slider));
}



NA_HDEF NARect na_GetSliderAbsoluteInnerRect(NASlider* slider){
  NA_UNUSED(slider);
  return naMakeRectS(20, 40, 100, 50);
}



NA_API void naSetSliderTickCount(NASlider* slider, NAInt tickCount){
}



NA_API double naGetSliderValue(NASlider* slider){
    return 0;
}



NA_API void naSetSliderValue(NASlider* slider, double value){
}

//NA_DEF NABool naGetRadioState(NARadio* radio){
//  LPARAM state = SendMessage(naGetUIElementNativeId(radio), BM_GETSTATE, 0, 0);
//  return (state & BST_CHECKED) == BST_CHECKED;
//}
//
//
//
//NA_DEF void naSetRadioState(NARadio* radio, NABool state){
//  LPARAM lParam = state ? BST_CHECKED : BST_UNCHECKED;
//  SendMessage(naGetUIElementNativeId(radio), BM_SETCHECK, lParam, 0);
//}



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
