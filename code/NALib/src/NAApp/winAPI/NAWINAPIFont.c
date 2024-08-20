
#include "../../NABase/NABase.h"
#include "../NAFont.h"
#include "../Core/NAAppCore.h"

#include <windows.h>

#include "../UIElements/NAUIElement.h"

NA_HDEF void na_DestructFontNativePtr(void* nativePtr) {
  DeleteObject(nativePtr);
}

NA_DEF NAFont* naCreateFont(const NAUTF8Char* fontFamilyName, uint32 flags, double size) {
  TCHAR* systemFontName = naAllocSystemStringWithUTF8String(fontFamilyName);

  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  HFONT nativePtr = CreateFont(
    (int)(size * uiScale),
    0,
    0,
    0,
    naGetFlagu32(flags, NA_FONT_FLAG_BOLD) ? FW_BOLD : FW_NORMAL,
    naGetFlagu32(flags, NA_FONT_FLAG_ITALIC),
    naGetFlagu32(flags, NA_FONT_FLAG_UNDERLINE),
    NA_FALSE,
    DEFAULT_CHARSET,
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    systemFontName);

  NAString* name = naNewStringWithFormat("%s", fontFamilyName);

  naFree(systemFontName);

  return na_CreateFont(nativePtr, name, flags, size);
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
