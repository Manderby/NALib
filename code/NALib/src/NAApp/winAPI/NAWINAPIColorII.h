
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPIColor NAWINAPIColor;
struct NAWINAPIColor {
  COLORREF colorRef;
  HBRUSH   brush;
};

NA_DEF NAUIColor* naAllocUIColor(const NAColor* color, const NAColor* bgColor) {
  NAWINAPIColor* winapiColor = naAlloc(NAWINAPIColor);

  if(bgColor) {
    NAColor blendedColor;
    naBlendColors(&blendedColor, bgColor, color, 1.f, NA_BLEND_OVERLAY, 1, NA_FALSE, NA_FALSE);
    naFillColorRefWithColor(&winapiColor->colorRef, &blendedColor);
  }else{
    naFillColorRefWithColor(&winapiColor->colorRef, color);
  }
  winapiColor->brush = CreateSolidBrush(winapiColor->colorRef);
  return winapiColor;
}

NA_DEF NAUIColor* naAllocUIColorWithColorRef(const void* colorRef) {
  NAWINAPIColor* winapiColor = naAlloc(NAWINAPIColor);
  const COLORREF* nativeRef = (const COLORREF*)colorRef;
  winapiColor->colorRef = *nativeRef;
  winapiColor->brush = CreateSolidBrush(winapiColor->colorRef);
  return winapiColor;
}

NA_DEF void naDeallocUIColor(NAUIColor* uiColor) {
  NAWINAPIColor* winapiColor = (NAWINAPIColor*)uiColor;
  DeleteObject(winapiColor->brush);
  naFree(winapiColor);
}




#include <wingdi.h>

NA_DEF void naFillColorWithColorRef(NAColor* color, const void* colorRef) {
  const COLORREF* nativeRef = (const COLORREF*)colorRef;
  naFillColorWithSRGB(
    color,
    GetRValue(*nativeRef) / 255.f,
    GetGValue(*nativeRef) / 255.f,
    GetBValue(*nativeRef) / 255.f,
    1.f);
}



NA_DEF void naFillColorRefWithColor(void* colorRef, const NAColor* color) {
  COLORREF* nativeRef = (COLORREF*)colorRef;
  *nativeRef = RGB(
    (int)(color->r * 255),
    (int)(color->g * 255),
    (int)(color->b * 255));
}



NA_DEF void naFillColorWithDefaultWindowBackgroundColor(NAColor* color) {
  // Although officially in the documentation, COLOR_WINDOW is the background
  // color of choice, the acutally used color is the BTNFACE color.
  COLORREF colorRef = GetSysColor(COLOR_BTNFACE);
  naFillColorWithColorRef(color, &colorRef);
}

NA_DEF void naFillColorWithDefaultTextColor(NAColor* color) {
  COLORREF colorRef = GetSysColor(COLOR_BTNTEXT);
  naFillColorWithColorRef(color, &colorRef);
}

NA_DEF void naFillColorWithDefaultLinkColor(NAColor* color) {
  COLORREF colorRef = GetSysColor(COLOR_HOTLIGHT);
  naFillColorWithColorRef(color, &colorRef);
}

NA_DEF void naFillColorWithDefaultAccentColor(NAColor* color) {
  COLORREF colorRef;
  if(!na_GetWINRegistryFixedEntry(
    HKEY_CURRENT_USER,
    "Software\\Microsoft\\Windows\\DWM",
    "AccentColor",
    &colorRef,
    sizeof(COLORREF)))
  {
    colorRef = GetSysColor(COLOR_HIGHLIGHT);
  }

  naFillColorWithColorRef(color, &colorRef);
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
