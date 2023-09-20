
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!

#define NA_WINAPI_BUTTON_IMAGE      0x04
#define NA_WINAPI_BUTTON_STATE      0x08



NAWINAPICallbackInfo naButtonWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
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
  case WM_IME_NOTIFY:
  case WM_LBUTTONUP:
  case WM_LBUTTONDBLCLK:
  case WM_DESTROY:
  case WM_NCDESTROY:
  case BM_SETSTATE:
  case BM_SETCHECK:
  case BM_GETSTATE:
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

  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // We do not display any caret.
    info.hasBeenHandeled = NA_TRUE;
    info.result = 0;
    break;

  default:
    printf("Uncaught Button message" NA_NL);
    break;
  }
  
  return info;
}



NA_HDEF NABool na_GetButtonState(const NAWINAPIButton* winapiButton){
  return naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_STATE);
}



NA_HDEF void na_SetButtonState(NAWINAPIButton* winapiButton, NABool state){
  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_STATE, state);
}



NA_HDEF const NAUTF8Char* currentText(NAWINAPIButton* winapiButton){
  NABool secondaryState = na_GetButtonState(winapiButton);
  const NAUTF8Char* text = secondaryState
    ? winapiButton->button.text2
    : winapiButton->button.text; 
  if(secondaryState && !text){
    text = winapiButton->button.text;
  }
  return text;
}



NA_HDEF void updateButtonText(NAWINAPIButton* winapiButton){
  const NAUTF8Char* text = currentText(winapiButton);

  TCHAR* systemText;
  if(text){
    systemText = naAllocSystemStringWithUTF8String(text);
  }else{
    systemText = naAllocSystemStringWithUTF8String("");
  }
  SendMessage(naGetUIElementNativePtr(winapiButton), WM_SETTEXT, 0, (LPARAM) systemText);
  naFree(systemText);
}



NABool naButtonWINAPINotify(void* uiElement, WORD notificationCode){
  NABool hasBeenHandeled = NA_FALSE;
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)uiElement;

  switch(notificationCode){
  case BN_CLICKED:
  case BN_DOUBLECLICKED:
    if(naGetFlagu32(winapiButton->button.flags, NA_BUTTON_STATEFUL)){
      // flip the state
      na_SetButtonState(winapiButton, !na_GetButtonState(winapiButton));
      // Set the button to unpressed:
      SendMessage(naGetUIElementNativePtr(winapiButton), BM_SETSTATE, (WPARAM)0, (LPARAM)NA_NULL);
      // Update the button content
      if(naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE)){
      }else{
        updateButtonText(winapiButton);
      }
    }
    na_DispatchUIElementCommand(uiElement, NA_UI_COMMAND_PRESSED);
    hasBeenHandeled = NA_TRUE;
    break;
  default:
    printf("Uncaught Button notification" NA_NL);
    break;
  }
  return hasBeenHandeled;
}



const NAUIImage* currentImage(NAWINAPIButton* winapiButton){
  NABool secondaryState = na_GetButtonState(winapiButton);
  const NAUIImage* uiImage = secondaryState
    ? winapiButton->button.uiImage2
    : winapiButton->button.uiImage; 
  if(secondaryState && !uiImage){
    uiImage = winapiButton->button.uiImage;
  }
  return uiImage;
}



NAWINAPICallbackInfo naButtonWINAPIDrawItem (void* uiElement, DRAWITEMSTRUCT* drawitemstruct){
  NASizei buttonsize = naMakeSizei(
    (NAInt)drawitemstruct->rcItem.right - (NAInt)drawitemstruct->rcItem.left,
    (NAInt)drawitemstruct->rcItem.bottom - (NAInt)drawitemstruct->rcItem.top);

  // Create an offscreen device context and buffer
  HDC hMemDC = CreateCompatibleDC(drawitemstruct->hDC);  
  NAByte* buttonBuffer = naMalloc(buttonsize.width * buttonsize.height * 4);
  HBITMAP hButtonBitmap = CreateBitmap((int)buttonsize.width, (int)buttonsize.height, 1, 32, buttonBuffer);
  SelectObject(hMemDC, hButtonBitmap);

  NAWINAPIButton* winapiButton = (NAWINAPIButton*)uiElement;
  NAWINAPICallbackInfo info = {NA_TRUE, TRUE};

  //CallWindowProc(na_GetApplicationOldButtonWindowProc(), naGetUIElementNativePtr(uiElement), WM_ERASEBKGND, (WPARAM)drawitemstruct->hDC, (LPARAM)NA_NULL);

  if(naIsButtonBordered(&winapiButton->button)){
    HWND hwnd = naGetUIElementNativePtr(uiElement);
    NABool customDraw = naIsButtonStateful(&winapiButton->button) && naGetButtonState(&winapiButton->button);

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

    if(customDraw){
      // We store the button as it is drawn by the system.
      BitBlt(hMemDC, 0, 0, (int)buttonsize.width, (int)buttonsize.height, hMemDC, 0, 0, SRCCOPY);
      NABabyImage* buttonImage = naCreateBabyImageFromNativeImage(hButtonBitmap);

      // Now we blend manually the foreground to the background.
      NABabyColor backColor = {.8f, .8f, .8f, 1.f};
      NABabyColor maskColor = {1.f, 1.f, 0.f, 1.f};
      NABabyColor accentColor;
      naFillDefaultAccentColorWithSkin(accentColor, NA_UIIMAGE_SKIN_LIGHT);
      NABabyImage* alphaImage = naCreateBabyImageWithTint(buttonImage, maskColor, NA_BLEND_ERASE_HUE, 1.f);
      NABabyImage* tintedImage = naCreateBabyImageWithTint(alphaImage, accentColor, NA_BLEND_MULTIPLY, .85f);
      //NABabyImage* blendedImage = naCreateBabyImageCopy(tintedImage);
      NABabyImage* blendedImage = naCreateBabyImageWithApply(backColor, tintedImage, NA_BLEND_OVERLAY, 1.f);
      naReleaseBabyImage(alphaImage);
      naReleaseBabyImage(tintedImage);

      NAByte* blendedBuffer = naMalloc(buttonsize.width * buttonsize.height * 4);
      naConvertBabyImageTou8(blendedImage, blendedBuffer, NA_TRUE, NA_COLOR_BUFFER_BGR0);
      HBITMAP hBlendedBitmap = CreateBitmap((int)buttonsize.width, (int)buttonsize.height, 1, 32, blendedBuffer);

      // Finally, we put the blended image onscreen.
      SelectObject(hMemDC, hBlendedBitmap);
      BitBlt(drawitemstruct->hDC, 0, 0, (int)buttonsize.width, (int)buttonsize.height, hMemDC, 0, 0, SRCCOPY);

      // Deleting the blended objects and buffers
      naReleaseBabyImage(buttonImage);
      DeleteObject(hBlendedBitmap);
      naFree(blendedBuffer);
      naReleaseBabyImage(blendedImage);
    }

    DeleteObject(hButtonBitmap);
    naFree(buttonBuffer);
  }

  const NAUIImage* uiImage = currentImage(winapiButton);
  if(uiImage){
    double uiScale = naGetUIElementResolutionFactor(NA_NULL);
    NASizei size1x = naGetUIImage1xSize(uiImage);
    size1x.width = (NAInt)(size1x.width * uiScale);
    size1x.height = (NAInt)(size1x.height * uiScale);

    NAPosi offset = naMakePosi(
      (buttonsize.width - size1x.width) / 2,
      (buttonsize.height - size1x.height) / 2);

    LRESULT result = SendMessage(naGetUIElementNativePtr(winapiButton), BM_GETSTATE, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
    NABool pushed = (result & BST_PUSHED) == BST_PUSHED;

    const NABabyImage* foreImage;
    NABool secondaryState = na_GetButtonState(winapiButton);
    if(IsWindowEnabled(naGetUIElementNativePtr(winapiButton))){
      if(pushed){
        foreImage = na_GetUIImageBabyImage(uiImage, NA_UIIMAGE_RESOLUTION_SCREEN_1x * uiScale, NA_UIIMAGE_SKIN_LIGHT, NA_UIIMAGE_INTERACTION_PRESSED, secondaryState);
      }else{
        if(winapiButton->button.uiElement.mouseInside){
          foreImage = na_GetUIImageBabyImage(uiImage, NA_UIIMAGE_RESOLUTION_SCREEN_1x * uiScale, NA_UIIMAGE_SKIN_LIGHT,  NA_UIIMAGE_INTERACTION_HOVER, secondaryState);
        }else{
          foreImage = na_GetUIImageBabyImage(uiImage, NA_UIIMAGE_RESOLUTION_SCREEN_1x * uiScale, NA_UIIMAGE_SKIN_LIGHT,  NA_UIIMAGE_INTERACTION_NONE, secondaryState);
        }
      }
    }else{
      foreImage = na_GetUIImageBabyImage(uiImage, NA_UIIMAGE_RESOLUTION_SCREEN_1x * uiScale, NA_UIIMAGE_SKIN_LIGHT,  NA_UIIMAGE_INTERACTION_DISABLED, secondaryState);
    }

    // We store the background where the image will be placed.
    NAByte* backBuffer = naMalloc(size1x.width * size1x.height * 4);
    HBITMAP hBackBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, backBuffer);
    HBITMAP hOldBitmap = SelectObject(hMemDC, hBackBitmap);
    BitBlt(hMemDC, 0, 0, (int)size1x.width, (int)size1x.height, drawitemstruct->hDC, (int)offset.x, (int)offset.y, SRCCOPY);
    NABabyImage* backImage = naCreateBabyImageFromNativeImage(hBackBitmap);

    // Now we blend manually the foreground to the background.
    NABabyImage* blendedImage = naCreateBabyImageWithBlend(
      backImage,
      foreImage,
      NA_BLEND_OVERLAY,
      1.f,
      naMakePosi(0, 0));
    NAByte* blendedBuffer = naMalloc(size1x.width * size1x.height * 4);
    naConvertBabyImageTou8(blendedImage, blendedBuffer, NA_TRUE, NA_COLOR_BUFFER_BGR0);
    HBITMAP hBlendedBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, blendedBuffer);

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
  }

  // Deleting device contexts
  DeleteDC(hMemDC);
  ReleaseDC(drawitemstruct->hwndItem, drawitemstruct->hDC);

  return info;
}



NA_DEF NAButton* naNewTextPushButton(const NAUTF8Char* text, double width){
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_BORDERED;

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    systemText,
    WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON,
    0,
    0,
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  naFree(systemText);

  SendMessage(nativePtr, WM_SETFONT, (WPARAM)naGetFontNativePointer(naGetSystemFont()), MAKELPARAM(TRUE, 0));

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    text,
    NA_NULL,
    NA_NULL,
    NA_NULL,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_IMAGE, NA_FALSE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewTextStateButton(const NAUTF8Char* text, const NAUTF8Char* text2, double width){
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_BORDERED | NA_BUTTON_STATEFUL;

  TCHAR* systemText = naAllocSystemStringWithUTF8String(text);

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    systemText,
    WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_TEXT | BS_PUSHBUTTON | BS_OWNERDRAW,
    0,
    0,
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  naFree(systemText);

  SendMessage(nativePtr, WM_SETFONT, (WPARAM)naGetFontNativePointer(naGetSystemFont()), MAKELPARAM(TRUE, 0));

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    text,
    text2,
    NA_NULL,
    NA_NULL,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_IMAGE, NA_FALSE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewIconPushButton(const NAUIImage* icon, double width){
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_BORDERED;

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    icon,
    NA_NULL,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewIconStateButton(const NAUIImage* icon, const NAUIImage* icon2, double width){
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_STATEFUL | NA_BUTTON_BORDERED;

  NAUIImage* secondaryIcon = NA_NULL;
  if(!icon2){
    secondaryIcon = naRecreateUIImage(icon);
  }

  winapiButton->rect = naMakeRectS(0., 0., width, 24.);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    icon,
    icon2 ? icon2 : secondaryIcon,
    flags);
  winapiButton->state = 0;

  if(secondaryIcon){
    naRelease(secondaryIcon);
  }

  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewImagePushButton(const NAUIImage* uiImage, NASize size){
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = 0;

  winapiButton->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    uiImage,
    NA_NULL,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF NAButton* naNewImageStateButton(const NAUIImage* uiImage, const NAUIImage* uiImage2, NASize size){
  NAWINAPIButton* winapiButton = naNew(NAWINAPIButton);

  uint32 flags = NA_BUTTON_STATEFUL;

  winapiButton->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);

  HWND nativePtr = CreateWindow(
    TEXT("BUTTON"),
    TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0,
    0,
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  WNDPROC oldproc = (WNDPROC)SetWindowLongPtr(nativePtr, GWLP_WNDPROC, (LONG_PTR)naWINAPIWindowCallback);
  if(!app->oldButtonWindowProc){app->oldButtonWindowProc = oldproc;}

  na_InitButton(
    (NAButton*)winapiButton,
    nativePtr,
    NA_NULL,
    NA_NULL,
    uiImage,
    uiImage2,
    flags);
  winapiButton->state = 0;

  naSetFlagu32(&(winapiButton->state), NA_WINAPI_BUTTON_IMAGE, NA_TRUE); 

  return (NAButton*)winapiButton;
}



NA_DEF void na_DestructWINAPIButton(NAWINAPIButton* winapiButton){
  na_ClearButton((NAButton*)winapiButton);
}



NA_DEF void naSetButtonVisible(NAButton* button, NABool visible){
  //todo
}



NA_DEF void naSetButtonEnabled(NAButton* button, NABool enabled){
  const NAWINAPIButton* winapiButton = (const NAWINAPIButton*)button;
  EnableWindow(naGetUIElementNativePtr(button), enabled);
}



NA_DEF NABool naGetButtonState(const NAButton* button){
  const NAWINAPIButton* winapiButton = (const NAWINAPIButton*)button;
  #if NA_DEBUG
    if(!naGetFlagu32(winapiButton->button.flags, NA_BUTTON_STATEFUL))
      naError("This is not a stateful button");
  #endif
  return na_GetButtonState(winapiButton);
}



NA_DEF void naSetButtonState(NAButton* button, NABool state){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  // Note that BM_SETSTATE only changes the visual highlight, not the state of the
  // WINAPI button. Therefore, we need a separate state boolean.
  if(naGetFlagu32(winapiButton->button.flags, NA_BUTTON_STATEFUL)){
    na_SetButtonState(winapiButton, state);
    updateButtonText(winapiButton);
    InvalidateRect(naGetUIElementNativePtr(winapiButton), NULL, TRUE);
  }else{
    #if NA_DEBUG
      naError("This is not a stateful button");
    #endif
  }
}



NA_DEF void naSetButtonText(NAButton* button, const NAUTF8Char* text){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  #if NA_DEBUG
  if(naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE))
    naError("This is not a text button");
  #endif
  na_setButtonText(button, text);
  updateButtonText(winapiButton);
}



NA_DEF void naSetButtonText2(NAButton* button, const NAUTF8Char* text){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  #if NA_DEBUG
  if(naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE))
    naError("This is not a text button");
  #endif
  na_setButtonText2(button, text);
  updateButtonText(winapiButton);
}



NA_DEF void naSetButtonImage(NAButton* button, const NAUIImage* uiImage){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  #if NA_DEBUG
    if(!naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE))
      naError("This is not a image button");
  #endif
  // todo
}



NA_DEF NABool naIsButtonStateful(const NAButton* button){
  return naGetFlagu32(button->flags, NA_BUTTON_STATEFUL);
}



NA_DEF NABool naIsButtonBordered(const NAButton* button){
  return naGetFlagu32(button->flags, NA_BUTTON_BORDERED);
}



NA_DEF NABool naIsButtonTextual(const NAButton* button){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  return naGetFlagu32(winapiButton->state, NA_WINAPI_BUTTON_IMAGE);
}



NA_DEF void naSetButtonSubmit(NAButton* button, NAReactionHandler handler, void* controller){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;
  long style = (long)GetWindowLongPtr(naGetUIElementNativePtr(winapiButton), GWL_STYLE);
  style = (style & ~SS_TYPEMASK) | BS_DEFPUSHBUTTON;
  SetWindowLongPtr(naGetUIElementNativePtr(winapiButton), GWL_STYLE, (LONG_PTR)style);

  na_setButtonSubmit(button);
  naAddUIKeyboardShortcut(
    naGetUIElementWindow(button),
    naMakeKeyStroke(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ENTER),
    handler,
    controller);
  // Windows can not distinguish between ENTER and NUMPAD_ENTER. So we do not
  // install another keystroke listener.
  //naAddUIKeyboardShortcut(
  //  naGetUIElementWindow(button),
  //  naMakeKeyStroke(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_NUMPAD_ENTER),
  //  handler,
  //  controller);
}



NA_DEF void naSetButtonAbort(NAButton* button, NAReactionHandler handler, void* controller){
  na_setButtonAbort(button);
  naAddUIKeyboardShortcut(
    naGetUIElementWindow(button),
    naMakeKeyStroke(NA_MODIFIER_FLAG_NONE, NA_KEYCODE_ESC),
    handler,
    controller);
}



NA_HDEF NARect na_GetButtonRect(const NA_UIElement* button){
  const NAWINAPIButton* winapiButton = (const NAWINAPIButton*)button;
  return winapiButton->rect;
}



NA_HDEF void na_SetButtonRect(NA_UIElement* button, NARect rect){
  NAWINAPIButton* winapiButton = (NAWINAPIButton*)button;

  winapiButton->rect = rect;
  double uiScale = naGetUIElementResolutionFactor(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(button));

  SetWindowPos(
    naGetUIElementNativePtr(button),
    HWND_TOP,
    (int)(winapiButton->rect.pos.x * uiScale),
    (int)((parentRect.size.height - winapiButton->rect.pos.y - winapiButton->rect.size.height) * uiScale),
    (int)(winapiButton->rect.size.width * uiScale),
    (int)(winapiButton->rect.size.height * uiScale),
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
