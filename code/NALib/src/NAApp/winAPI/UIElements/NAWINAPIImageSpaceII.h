
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!




NAWINAPICallbackInfo naImageSpaceWINAPIDrawItem(void* uiElement) {
  NAWINAPIImageSpace* imageSpace = (NAWINAPIImageSpace*)uiElement;

  if(imageSpace->imageSpace.imageSet) {
    PAINTSTRUCT paintStruct;
    BeginPaint(naGetUIElementNativePtr(uiElement), &paintStruct);

    // Create a memory device context to do image blending.
    HDC hMemDC = CreateCompatibleDC(paintStruct.hdc);

    //NAWINAPIColor* bgWinapiColor = na_GetSpaceBackgroundWINAPIColor(naGetUIElementParentSpaceMutable(uiElement));
    //FillRect(paintStruct.hdc, &paintStruct.rcPaint, bgWinapiColor->brush);

    double uiScale = naGetUIElementUIScale(NA_NULL);

    NASizes size1x = naGetImageSet1xSize(imageSpace->imageSpace.imageSet);
    size1x.width = (size_t)(size1x.width * uiScale);
    size1x.height = (size_t)(size1x.height * uiScale);

    NASizei32 spaceSize = naMakeSizei32(
      paintStruct.rcPaint.right - paintStruct.rcPaint.left,
      paintStruct.rcPaint.bottom - paintStruct.rcPaint.top);
    NAPosi32 offset = naMakePosi32(
      (spaceSize.width - (int32)size1x.width) / 2,
      (spaceSize.height - (int32)size1x.height) / 2);

    const NAImage* foreImage = na_GetImageSetSubImage(
      imageSpace->imageSpace.imageSet,
      NA_UI_RESOLUTION_1x * uiScale,
      NA_SKIN_PLAIN,
      NA_IMAGE_SET_INTERACTION_NONE,
      NA_FALSE);

    // We store the background where the image will be placed.
    NAByte* backBuffer = naMalloc(size1x.width * size1x.height * 4);
    HBITMAP hBackBitmap = CreateBitmap(
      (int)size1x.width,
      (int)size1x.height,
      1,
      32,
      backBuffer);

    HBITMAP hOldBitmap = SelectObject(hMemDC, hBackBitmap);
    BitBlt(
      hMemDC,
      0,
      0,
      (int)size1x.width,
      (int)size1x.height,
      paintStruct.hdc,
      (int)offset.x,
      (int)offset.y,
      SRCCOPY);
    NAImage* backImage = naCreateImageWithNativeImage(hBackBitmap);

    // Now we blend manually the foreground to the background.
    NAImage* blendedImage = naCreateImageWithBlend(
      backImage,
      foreImage,
      NA_BLEND_OVERLAY,
      1.f,
      naMakePosi32(0, 0));
    NAByte* blendedBuffer = naMalloc(size1x.width * size1x.height * 4);
    naConvertImageTou8(
      blendedImage,
      blendedBuffer,
      NA_TRUE,
      NA_COLOR_BUFFER_BGR0);
    HBITMAP hBlendedBitmap = CreateBitmap(
      (int)size1x.width,
      (int)size1x.height,
      1,
      32,
      blendedBuffer);

    // Finally, we put the blended image onscreen.
    SelectObject(hMemDC, hBlendedBitmap);
    BitBlt(
      paintStruct.hdc,
      (int)offset.x,
      (int)offset.y,
      (int)size1x.width,
      (int)size1x.height,
      hMemDC,
      0,
      0,
      SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);

    // Deleting the blended objects and buffers
    DeleteObject(hBlendedBitmap);
    naFree(blendedBuffer);
    naRelease(blendedImage);

    // Deleting background objects and buffers
    DeleteObject(hBackBitmap);
    naFree(backBuffer);
    naRelease(backImage);

    // Deleting device contexts
    ReleaseDC(naGetUIElementNativePtr(uiElement), paintStruct.hdc);
    DeleteDC(hMemDC);

    EndPaint(naGetUIElementNativePtr(uiElement), &paintStruct);
  }

  NAWINAPICallbackInfo info = {
    .hasBeenHandeled = NA_TRUE,
    .result = 0
  };
  return info;
}



NAWINAPICallbackInfo naImageSpaceWINAPIProc(
  void* uiElement,
  UINT message,
  WPARAM wParam,
  LPARAM lParam)
{
  NA_UNUSED(wParam);
  NA_UNUSED(lParam);

  switch(message) {
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_NCPAINT:
  break;

  case WM_PAINT:
    return naImageSpaceWINAPIDrawItem(uiElement);

  default:
    //printf("Uncaught Image Space message" NA_NL);
    break;
  }
  
  NAWINAPICallbackInfo info = {
    .hasBeenHandeled = NA_FALSE,
    .result = 0
  };
  return info;
}



NA_DEF NAImageSpace* naNewImageSpace(NAImageSet* imageSet, NASize size) {
  NAWINAPIImageSpace* winapiImageSpace = naNew(NAWINAPIImageSpace);

  winapiImageSpace->rect = naMakeRect(naMakePos(0., 0.), size);
  double uiScale = naGetUIElementUIScale(NA_NULL);

	HWND nativePtr = CreateWindow(
		TEXT("NASpace"),
    TEXT("Space"),
    WS_CHILD | WS_VISIBLE,
		0,
    0,
    (LONG)(winapiImageSpace->rect.size.width * uiScale),
    (LONG)(winapiImageSpace->rect.size.height * uiScale),
    naGetApplicationOffscreenWindow(),
    NULL,
    (HINSTANCE)naGetUIElementNativePtr(naGetApplication()),
    NULL);

  na_InitImageSpace(&winapiImageSpace->imageSpace, nativePtr);

  if(imageSet) {
    winapiImageSpace->imageSpace.imageSet = naRetain(imageSet);
  }else{
    winapiImageSpace->imageSpace.imageSet = NA_NULL;
  }

  return (NAImageSpace*)winapiImageSpace;
}



NA_DEF void na_DestructWINAPIImageSpace(NAWINAPIImageSpace* winapiImageSpace) {
  naRelease(winapiImageSpace->imageSpace.imageSet);
  na_ClearImageSpace((NAImageSpace*)winapiImageSpace);
}



NA_DEF void naSetImageSpaceImage(NAImageSpace* imageSpace, NAImageSet* imageSet) {
  if(imageSpace->imageSet) {
    naRelease(imageSpace->imageSet);
  }
  if(imageSet) {
    imageSpace->imageSet = naRetain(imageSet);
  }
  else {
    imageSpace->imageSet = NA_NULL;
  }

  InvalidateRect(naGetUIElementNativePtr(imageSpace), NULL, TRUE);
}



NA_HDEF NARect na_GetImageSpaceRect(const NA_UIElement* imageSpace)
{
  const NAWINAPIImageSpace* winapiImageSpace = (const NAWINAPIImageSpace*)imageSpace;
  return winapiImageSpace->rect;
}

NA_HDEF void na_SetImageSpaceRect(NA_UIElement* imageSpace, NARect rect) {
  NAWINAPIImageSpace* winapiImageSpace = (NAWINAPIImageSpace*)imageSpace;

  winapiImageSpace->rect = rect;
  double uiScale = naGetUIElementUIScale(NA_NULL);
  NARect parentRect = naGetUIElementRect(naGetUIElementParent(imageSpace));

  SetWindowPos(
    naGetUIElementNativePtr(imageSpace),
    HWND_TOP,
    (LONG)(winapiImageSpace->rect.pos.x * uiScale),
    (LONG)((parentRect.size.height - winapiImageSpace->rect.pos.y - winapiImageSpace->rect.size.height) * uiScale),
    (LONG)(winapiImageSpace->rect.size.width * uiScale),
    (LONG)(winapiImageSpace->rect.size.height * uiScale),
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
