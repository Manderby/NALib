
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


typedef struct NAWINAPIImageSpace NAWINAPIImageSpace;
struct NAWINAPIImageSpace {
  NA_ImageSpace coreImageSpace;
  NAUIImage* image;
};


NAWINAPICallbackInfo naImageSpaceWINAPIDrawItem (NAUIElement* uiElement);



NAWINAPICallbackInfo naImageSpaceWINAPIProc(NAUIElement* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_WINDOWPOSCHANGING:
  case WM_CHILDACTIVATE:
  case WM_WINDOWPOSCHANGED:
  case WM_MOVE:
  case WM_SHOWWINDOW:
  case WM_NCPAINT:
  case WM_ERASEBKGND:
    break;

  case WM_PAINT:
    naImageSpaceWINAPIDrawItem(uiElement);
    info.hasbeenhandeled = NA_TRUE;
    break;

  default:
    //printf("Uncaught Image Space message" NA_NL);
    break;
  }
  
  return info;
}



NAWINAPICallbackInfo naImageSpaceWINAPIDrawItem (NAUIElement* uiElement){
  PAINTSTRUCT paintStruct;
  HBITMAP hOldBitmap;
  HDC hMemDC;
  NAWINAPIImageSpace* imageSpace;
  NAWINAPICallbackInfo info = {NA_TRUE, TRUE};
  NASizei size1x;
  NASizei spacesize;
  NAPosi offset;
  NABabyImage* foreImage;
  NAByte* backBuffer;
  HBITMAP hBackBitmap;
  NABabyImage* backImage;
  NABabyImage* blendedImage;
  NAByte* blendedBuffer;
  HBITMAP hBlendedBitmap;

  BeginPaint(naGetUIElementNativeID(uiElement), &paintStruct);
  hMemDC = CreateCompatibleDC(paintStruct.hdc);

  imageSpace = (NAWINAPIImageSpace*)uiElement;

  CallWindowProc(naGetApplicationOldButtonWindowProc(), naGetUIElementNativeID(uiElement), WM_ERASEBKGND, (WPARAM)paintStruct.hdc, (LPARAM)NA_NULL);

  size1x = naGetUIImage1xSize(imageSpace->image);

  spacesize = naMakeSizei(
    paintStruct.rcPaint.right - paintStruct.rcPaint.left,
    paintStruct.rcPaint.bottom - paintStruct.rcPaint.top);
  offset = naMakePosi(
    (spacesize.width - size1x.width) / 2,
    (spacesize.height - size1x.height) / 2);

  foreImage = na_GetUIImageBabyImage(imageSpace->image, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_LIGHT);

  // We store the background where the image will be placed.
  backBuffer = naMalloc(size1x.width * size1x.height * 4);
  hBackBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, backBuffer);
  hOldBitmap = SelectObject(hMemDC, hBackBitmap);
  BitBlt(hMemDC, 0, 0, (int)size1x.width, (int)size1x.height, paintStruct.hdc, (int)offset.x, (int)offset.y, SRCCOPY);
  backImage = naCreateBabyImageFromNativeImage(hBackBitmap);

  // Now we blend manually the foreground to the background.
  blendedImage = naCreateBabyImageWithBlend(backImage, foreImage, NA_BLEND_OVERLAY, 1.f);
  blendedBuffer = naMalloc(size1x.width * size1x.height * 4);
  naConvertBabyImageTou8(blendedImage, blendedBuffer, NA_TRUE, NA_COLOR_BUFFER_BGR0);
  hBlendedBitmap = CreateBitmap((int)size1x.width, (int)size1x.height, 1, 32, blendedBuffer);

  // Finally, we put the blended image onscreen.
  SelectObject(hMemDC, hBlendedBitmap);
  BitBlt(paintStruct.hdc, (int)offset.x, (int)offset.y, (int)size1x.width, (int)size1x.height, hMemDC, 0, 0, SRCCOPY);
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
  ReleaseDC(naGetUIElementNativeID(uiElement), paintStruct.hdc);
  EndPaint(naGetUIElementNativeID(uiElement), &paintStruct);

  return info;
}



NA_DEF NAImageSpace* naNewImageSpace(NAUIImage* uiimage, NASize size){
  HWND hWnd;
  DWORD exStyle;
  DWORD style;
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();

  NAWINAPIImageSpace* winapiImageSpace = naAlloc(NAWINAPIImageSpace);

  exStyle = 0;
  style = WS_CHILD | WS_VISIBLE;

	hWnd = CreateWindow(
		TEXT("NASpace"), TEXT("Space"), style,
		0, 0, (int)size.width, (int)size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );

  na_InitCoreImageSpace(&(winapiImageSpace->coreImageSpace), hWnd);
  winapiImageSpace->image = uiimage;

  return (NAImageSpace*)winapiImageSpace;
}



NA_DEF void naDestructImageSpace(NAImageSpace* imageSpace){
  NAWINAPIImageSpace* winapiImageSpace = (NAWINAPIImageSpace*)imageSpace;
  na_ClearCoreImageSpace(&(winapiImageSpace->coreImageSpace));
}



NA_HDEF NARect na_GetImageSpaceAbsoluteInnerRect(NA_UIElement* imageSpace){
  NA_UNUSED(imageSpace);
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
