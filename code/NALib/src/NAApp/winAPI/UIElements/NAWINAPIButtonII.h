
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



#define NA_WINAPI_BUTTON_IMAGE      0x04
#define NA_WINAPI_BUTTON_STATE      0x08



NAWINAPICallbackInfo naButtonWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NA_UNUSED(lParam);
  NA_UNUSED(wParam);
  NA_UNUSED(uiElement);
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {
  case WM_SETFONT:
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_PAINT:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
  case WM_GETTEXTLENGTH:
  case WM_GETTEXT:
  case WM_NCHITTEST:
  case WM_SETCURSOR:
  case WM_MOUSEACTIVATE:
  case WM_LBUTTONDOWN:
  case WM_IME_SETCONTEXT:
  case WM_CANCELMODE:
  case WM_CAPTURECHANGED:
  case WM_STYLECHANGING:
  case WM_STYLECHANGED:
  case WM_IME_NOTIFY:
  case WM_LBUTTONUP:
  case WM_LBUTTONDBLCLK:
  case WM_DESTROY:
  case WM_NCDESTROY:
  case BM_SETSTATE:
  case BM_SETCHECK:
  case BM_GETSTATE:
  case WM_WINDOWPOSCHANGED:
    break;

  // Menu messages
  case WM_ENTERMENULOOP:
  case WM_INITMENU:
  case WM_INITMENUPOPUP:
  case 0x93:  // undocumented
  case 0x94:  // undocumented
  case WM_ENTERIDLE:
  case WM_MENUSELECT:
  case WM_UNINITMENUPOPUP:
  case WM_EXITMENULOOP:
  case WM_ENABLE:
  case WM_SETTEXT:
  case WM_UPDATEUISTATE:
    break;

  // Handeled in naUIElementWINAPIPreProc:
  case WM_MOUSELEAVE:
  case WM_SETFOCUS:
  case WM_KILLFOCUS:
  break;

  default:
    //printf("Uncaught Button message" NA_NL);
    break;
  }
  
  return info;
}



NA_HDEF NABool na_GetButtonState(const NAWINAPIButton* winapiButton) {
  return naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_STATE);
}



NA_HDEF void na_SetButtonState(NAWINAPIButton* winapiButton, NABool state) {
  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_STATE, state);
}



NA_HDEF const NAUTF8Char* currentText(NAWINAPIButton* winapiButton) {
  NABool secondaryState = na_GetButtonState(winapiButton);
  const NAUTF8Char* text = secondaryState
    ? winapiButton->button.text2
    : winapiButton->button.text; 
  if(secondaryState && !text) {
    text = winapiButton->button.text;
  }
  return text;
}



NA_HDEF void updateButtonText(NAWINAPIButton* winapiButton) {
  const NAUTF8Char* text = currentText(winapiButton);

  TCHAR* systemText;
  if(text) {
    systemText = naAllocSystemStringWithUTF8String(text);
  }else{
    systemText = naAllocSystemStringWithUTF8String("");
  }
  SendMessage(naGetUIElementNativePtr(winapiButton), WM_SETTEXT, 0, (LPARAM) systemText);
  naFree(systemText);
}



NABool naButtonWINAPINotify(void* uiElement, WORD notificationCode) {
  NABool hasBeenHandeled = NA_FALSE;
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)uiElement;

  switch(notificationCode) {
  case BN_CLICKED:
  case BN_DOUBLECLICKED:
    if(naGetFlagu32(winapiButton->button.flags, NA_BUTTON_STATEFUL)) {
      // flip the state
      na_SetButtonState(winapiButton, !na_GetButtonState(winapiButton));
      // Set the button to unpressed:
      SendMessage(naGetUIElementNativePtr(winapiButton), BM_SETSTATE, (WPARAM)0, (LPARAM)NA_NULL);
      // Update the button content
      if(naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE)) {
      }else{
        updateButtonText(winapiButton);
      }
    }
    if(!na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_PRESSED)) {
      // no parent method to be called.
    }
    hasBeenHandeled = NA_TRUE;
    break;
  default:
    //printf("Uncaught Button notification" NA_NL);
    break;
  }
  return hasBeenHandeled;
}



const NAImageSet* currentImage(NAWINAPIButton* winapiButton) {
  NABool secondaryState = na_GetButtonState(winapiButton);
  const NAImageSet* imageSet = secondaryState
    ? winapiButton->button.imageSet2
    : winapiButton->button.imageSet; 
  if(secondaryState && !imageSet) {
    imageSet = winapiButton->button.imageSet;
  }
  return imageSet;
}



NAWINAPICallbackInfo naButtonWINAPIDrawItem (void* uiElement, DRAWITEMSTRUCT* drawitemstruct) {
  NASizei32 buttonSize = naMakeSizei32(
    (drawitemstruct->rcItem.right - drawitemstruct->rcItem.left),
    (drawitemstruct->rcItem.bottom - drawitemstruct->rcItem.top));

  // Create an offscreen device context and buffer
  HDC hMemDC = CreateCompatibleDC(drawitemstruct->hDC);  
  NAByte* buttonBuffer = naMalloc((size_t)(buttonSize.width * buttonSize.height) * 4);
  HBITMAP hButtonBitmap = CreateBitmap((int)buttonSize.width, (int)buttonSize.height, 1, 32, buttonBuffer);
  SelectObject(hMemDC, hButtonBitmap);

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)uiElement;
  NAWINAPICallbackInfo info = {NA_TRUE, TRUE};

  if(naIsButtonBordered(&winapiButton->button)) {
    HWND hwnd = naGetUIElementNativePtr(uiElement);
    NABool customDraw = naIsButtonStateful(&winapiButton->button) && naGetButtonState(&winapiButton->button);

    NASpace* parentSpace = naGetUIElementParentSpaceMutable(uiElement);
    NAColor maskColor = {1.f, 1.f, 0.f, 1.f};
    NAUIColor* prevBgColor = NA_NULL;
    NAUIColor* tmpBgColor = NA_NULL;

    if(customDraw) {
      tmpBgColor = naAllocUIColor(&maskColor, NA_NULL);
      prevBgColor = na_SwapWINAPISpaceBackgroundColor(parentSpace, tmpBgColor);
    }

    long oldstyle = (long)GetWindowLongPtr(hwnd, GWL_STYLE);
    long newstyle = (oldstyle & ~BS_OWNERDRAW) | BS_TEXT | BS_CENTER | BS_VCENTER;
    SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)newstyle);
    // Oh boi. That is one hell of a hidden feature. Usually, the WM_PAINT
    // message does not use wParam and lParam at all. But there are some
    // common controls (and buttons seems to be one of them) which in fact
    // only work if you send the device context in wParam.
    CallWindowProc(
      na_GetApplicationOldButtonWindowProc(),
      hwnd,
      WM_PAINT,
      customDraw ? (WPARAM)hMemDC : (WPARAM)drawitemstruct->hDC,
      (LPARAM)NA_NULL);
    SetWindowLongPtr(hwnd, GWL_STYLE, (LONG_PTR)oldstyle);

    if(customDraw) {
      // We store the button as it is drawn by the system.
      BitBlt(hMemDC, 0, 0, (int)buttonSize.width, (int)buttonSize.height, hMemDC, 0, 0, SRCCOPY);
      NAImage* buttonImage = naCreateImageWithNativeImage(hButtonBitmap);

      // Now we blend manually the foreground to the background.

      NAColor backColor;
      if(parentSpace) {
        naFillSpaceBackgroundColor(&backColor, parentSpace);
      }else{
        naFillColorWithDefaultWindowBackgroundColor(&backColor);
      }
      NAColor accentColor;
      naFillColorWithSkinAccentColor(&accentColor, NA_SKIN_SYSTEM);
      NAImage* alphaImage = naCreateImageWithTint(buttonImage, &maskColor, NA_BLEND_ERASE_HUE, 1.f);
      NAImage* tintedImage = naCreateImageWithTint(alphaImage, &accentColor, NA_BLEND_MULTIPLY, .85f);
      //NAImage* blendedImage = naCreateImageCopy(tintedImage);
      NAImage* blendedImage = naCreateImageWithApply(&backColor, tintedImage, NA_BLEND_OVERLAY, 1.f);
      naRelease(alphaImage);
      naRelease(tintedImage);

      NAByte* blendedBuffer = naMalloc((size_t)(buttonSize.width * buttonSize.height) * 4);
      naConvertImageTou8(blendedImage, blendedBuffer, NA_TRUE, NA_COLOR_BUFFER_BGR0);
      HBITMAP hBlendedBitmap = CreateBitmap((int)buttonSize.width, (int)buttonSize.height, 1, 32, blendedBuffer);

      // Finally, we put the blended image onscreen.
      SelectObject(hMemDC, hBlendedBitmap);
      BitBlt(drawitemstruct->hDC, 0, 0, (int)buttonSize.width, (int)buttonSize.height, hMemDC, 0, 0, SRCCOPY);

      if(customDraw) {
        tmpBgColor = na_SwapWINAPISpaceBackgroundColor(parentSpace, prevBgColor);
        naDeallocUIColor(tmpBgColor);
      }

      // Deleting the blended objects and buffers
      naRelease(buttonImage);
      DeleteObject(hBlendedBitmap);
      naFree(blendedBuffer);
      naRelease(blendedImage);
    }

    DeleteObject(hButtonBitmap);
    naFree(buttonBuffer);
  }

  const NAImageSet* imageSet = currentImage(winapiButton);
  if(imageSet) {
    double uiScale = naGetUIElementResolutionScale(NA_NULL);
    NASizes size1x = naGetImageSet1xSize(imageSet);
    size1x.width = (size_t)(size1x.width * uiScale);
    size1x.height = (size_t)(size1x.height * uiScale);

    NAPosi32 offset = naMakePosi32(
      (buttonSize.width - (int32)size1x.width) / 2,
      (buttonSize.height - (int32)size1x.height) / 2);

    LRESULT result = SendMessage(naGetUIElementNativePtr(winapiButton), BM_GETSTATE, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
    NABool pushed = (result & BST_PUSHED) == BST_PUSHED;

    const NAImage* foreImage;
    NABool secondaryState = na_GetButtonState(winapiButton);
    if(IsWindowEnabled(naGetUIElementNativePtr(winapiButton))) {
      if(pushed) {
        foreImage = na_GetImageSetSubImage(imageSet, NA_UI_RESOLUTION_1x * uiScale, NA_SKIN_SYSTEM, NA_IMAGE_SET_INTERACTION_PRESSED, secondaryState);
      }else{
        if(na_GetUIElementMouseInside(&winapiButton->button.uiElement)) {
          foreImage = na_GetImageSetSubImage(imageSet, NA_UI_RESOLUTION_1x * uiScale, NA_SKIN_SYSTEM, NA_IMAGE_SET_INTERACTION_HOVER, secondaryState);
        }else{
          foreImage = na_GetImageSetSubImage(imageSet, NA_UI_RESOLUTION_1x * uiScale, NA_SKIN_SYSTEM, NA_IMAGE_SET_INTERACTION_NONE, secondaryState);
        }
      }
    }else{
      foreImage = na_GetImageSetSubImage(imageSet, NA_UI_RESOLUTION_1x * uiScale, NA_SKIN_SYSTEM, NA_IMAGE_SET_INTERACTION_DISABLED, secondaryState);
    }

    // We store the background where the image will be placed.
    NAByte* backBuffer = naMalloc(size1x.width * size1x.height * 4);
    HBITMAP hBackBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, backBuffer);
    HBITMAP hOldBitmap = SelectObject(hMemDC, hBackBitmap);
    BitBlt(hMemDC, 0, 0, (int)size1x.width, (int)size1x.height, drawitemstruct->hDC, (int)offset.x, (int)offset.y, SRCCOPY);
    NAImage* backImage = naCreateImageWithNativeImage(hBackBitmap);

    // Now we blend manually the foreground to the background.
    NAImage* blendedImage = naCreateImageWithBlend(
      backImage,
      foreImage,
      NA_BLEND_OVERLAY,
      1.f,
      naMakePosi32(0, 0));
    NAByte* blendedBuffer = naMalloc(size1x.width * size1x.height * 4);
    naConvertImageTou8(blendedImage, blendedBuffer, NA_TRUE, NA_COLOR_BUFFER_BGR0);
    HBITMAP hBlendedBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, blendedBuffer);

    // Finally, we put the blended image onscreen.
    SelectObject(hMemDC, hBlendedBitmap);
    BitBlt(drawitemstruct->hDC, (int)offset.x, (int)offset.y, (int)size1x.width, (int)size1x.height, hMemDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);

    // Deleting the blended objects and buffers
    DeleteObject(hBlendedBitmap);
    naFree(blendedBuffer);
    naRelease(blendedImage);

    // Deleting background objects and buffers
    DeleteObject(hBackBitmap);
    naFree(backBuffer);
    naRelease(backImage);
  }

  // Deleting device contexts
  DeleteDC(hMemDC);
  ReleaseDC(drawitemstruct->hwndItem, drawitemstruct->hDC);

  return info;
}



NA_DEF NAButton* naNewTextPushButton(const NAUTF8Char* text, double width) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_BORDERED;

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    systemText,
    WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON,
    0,
    0,
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  naFree(systemText);

  const NAFont* systemFont = na_GetApplicationSystemFont(&app->application);
  SendMessage(
    nativePtr,
    WM_SETFONT,
    (WPARAM)naGetFontNativePointer(systemFont),
    MAKELPARAM(TRUE, 0));

  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc) {
    app->oldButtonWindowProc = oldproc;
  }

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    text,
    NA_NULL,
    NA_NULL,
    NA_NULL,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_IMAGE, NA_FALSE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewTextStateButton(const NAUTF8Char* text, const NAUTF8Char* text2, double width) {
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_BORDERED | NA_BUTTON_STATEFUL;

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    systemText,
    WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON | BS_OWNERDRAW,
    0,
    0,
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  naFree(systemText);

  const NAFont* systemFont = na_GetApplicationSystemFont(&app->application);
  SendMessage(
    nativePtr,
    WM_SETFONT,
    (WPARAM)naGetFontNativePointer(systemFont),
    MAKELPARAM(TRUE, 0));

  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc) {
    app->oldButtonWindowProc = oldproc;
  }

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    text,
    text2,
    NA_NULL,
    NA_NULL,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_IMAGE, NA_FALSE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewIconPushButton(const NAImageSet* icon, double width) {
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_BORDERED;

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc) {
    app->oldButtonWindowProc = oldproc;
  }

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    icon,
    NA_NULL,
    flags);
  na_IncCoreUIElementHoverTrackingCount(&winapiButton->button.uiElement);

  winapiButton->state = 0;

  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewIconStateButton(const NAImageSet* icon, const NAImageSet* icon2, double width) {
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_STATEFUL | NA_BUTTON_BORDERED;

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  // Note: We need a copy because the tinting might be different.
  NAImageSet* imageSet2 = icon2
    ? NA_NULL
    : naRecreateImageSet(icon);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc) {
    app->oldButtonWindowProc = oldproc;
  }

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    icon,
    icon2 ? icon2 : imageSet2,
    flags);
  na_IncCoreUIElementHoverTrackingCount(&winapiButton->button.uiElement);

  if(imageSet2) {
    naRelease(imageSet2);
  }

  winapiButton->state = 0;

  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewImagePushButton(const NAImageSet* imageSet, NASize size) {
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = 0;

  winapiButton->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc) {
    app->oldButtonWindowProc = oldproc;
  }

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    imageSet,
    NA_NULL,
    flags);
  na_IncCoreUIElementHoverTrackingCount(&winapiButton->button.uiElement);

  winapiButton->state = 0;

  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewImageStateButton(const NAImageSet* imageSet, const NAImageSet* imageSet2, NASize size) {
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_STATEFUL;

  winapiButton->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc) {
    app->oldButtonWindowProc = oldproc;
  }

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    imageSet,
    imageSet2,
    flags);
  na_IncCoreUIElementHoverTrackingCount(&winapiButton->button.uiElement);

  winapiButton->state = 0;

  naSetFlagu32(&winapiButton->state, NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF void na_DestructWINAPIButton(NAWINAPIButton* winapiButton) {
  na_ClearButton((NAButton*)winapiButton);
}



NA_DEF void naSetButtonVisible(NAButton* button, NABool visible) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  ShowWindow(naGetUIElementNativePtr(button), visible ? SW_SHOW : SW_HIDE);
}



NA_DEF void naSetButtonEnabled(NAButton* button, NABool enabled) {
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  EnableWindow(naGetUIElementNativePtr(button), enabled);
}



NA_DEF NABool naGetButtonState(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  const NAWINAPIButton* winapiButton = (const NAWINAPIButton*)button;
  
  #if NA_DEBUG
    if(!naGetFlagu32(winapiButton->button.flags, NA_BUTTON_STATEFUL))
      naError("This is not a stateful button");
  #endif

  return na_GetButtonState(winapiButton);
}



NA_DEF void naSetButtonState(NAButton* button, NABool state) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  
  // Note that BM_SETSTATE only changes the visual highlight, not the state of the
  // WINAPI button. Therefore, we need a separate state boolean.
  if(naGetFlagu32(winapiButton->button.flags, NA_BUTTON_STATEFUL)) {
    na_SetButtonState(winapiButton, state);
    updateButtonText(winapiButton);
    InvalidateRect(naGetUIElementNativePtr(winapiButton), NULL, TRUE);
  }else{
    #if NA_DEBUG
      naError("This is not a stateful button");
    #endif
  }
}



NA_DEF void naSetButtonText(NAButton* button, const NAUTF8Char* text) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  
  #if NA_DEBUG
    if(naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE))
      naError("This is not a text button");
  #endif

  na_setButtonText(button, text);
  updateButtonText(winapiButton);
}



NA_DEF void naSetButtonText2(NAButton* button, const NAUTF8Char* text) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  
  #if NA_DEBUG
    if(naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE))
      naError("This is not a text button");
  #endif

  na_setButtonText2(button, text);
  updateButtonText(winapiButton);
}



NA_DEF void naSetButtonImage(NAButton* button, const NAImageSet* imageSet) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;

  #if NA_DEBUG
    if(!naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE))
      naError("This is not a image button");
  #endif

  // todo
  NA_UNUSED(imageSet);
}



NA_DEF NABool naIsButtonStateful(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  return naGetFlagu32(button->flags, NA_BUTTON_STATEFUL);
}



NA_DEF NABool naIsButtonBordered(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  return naGetFlagu32(button->flags, NA_BUTTON_BORDERED);
}



NA_DEF NABool naIsButtonTextual(const NAButton* button) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  return naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE);
}



NA_DEF void naSetButtonSubmit(
  NAButton* button,
  NAReactionCallback callback,
  void* controller)
{
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  long style = (long)GetWindowLongPtr(naGetUIElementNativePtr(winapiButton), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | BS_DEFPUSHBUTTON;
  SetWindowLongPtr(naGetUIElementNativePtr(winapiButton), GWL_STYLE, (LONG_PTR)style);

  naAddUIKeyboardShortcut(
    naGetUIElementWindowMutable(button),
    naNewKeyStroke(NA_KEYCODE_ENTER, NA_KEY_MODIFIER_NONE),
    callback,
    controller);

  // Windows can not distinguish between ENTER and NUMPAD_ENTER. So we do not
  // install another keystroke listener. really? todo.
  //naAddUIKeyboardShortcut(
  //  naGetUIElementWindowMutable(button),
  //  naNewKeyStroke(NA_KEYCODE_NUMPAD_ENTER, NA_KEY_MODIFIER_NONE),
  //  callback,
  //  controller);
}



NA_DEF void naSetButtonAbort(
  NAButton* button,
  NAReactionCallback callback,
  void* controller)
{
  #if NA_DEBUG
    if(!button)
      naError("button is nullptr");
  #endif

  naAddUIKeyboardShortcut(
    naGetUIElementWindowMutable(button),
    naNewKeyStroke(NA_KEYCODE_ESCAPE, NA_KEY_MODIFIER_NONE),
    callback,
    controller);
}



NA_HDEF NARect na_GetButtonRect(const NA_UIElement* button) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  const NAWINAPIButton* winapiButton = (const NAWINAPIButton*)button;
  return winapiButton->rect;
}



NA_HDEF void na_SetButtonRect(NA_UIElement* button, NARect rect) {
  #if NA_DEBUG
    if(!button)
      naCrash("button is nullptr");
  #endif

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;

  winapiButton->rect = rect;
  double uiScale = naGetUIElementResolutionScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(button));

  SetWindowPos(
    naGetUIElementNativePtr(button),
    HWND_TOP,
    (LONG)(winapiButton->rect.pos.x * uiScale),
    (LONG)((parentRect.size.height - winapiButton->rect.pos.y - winapiButton->rect.size.height) * uiScale),
    (LONG)(winapiButton->rect.size.width * uiScale),
    (LONG)(winapiButton->rect.size.height * uiScale),
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
