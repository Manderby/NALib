
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitScreen(NAScreen* screen, void* nativePtr) {
  na_InitCoreUIElement(&screen->uiElement, NA_UI_SCREEN, nativePtr);
}



NA_HDEF void na_ClearScreen(NAScreen* screen) {
  na_ClearCoreUIElement(&screen->uiElement);
}



NA_HDEF NARect na_GetScreenRect(const NA_UIElement* screen) {
  NA_UNUSED(screen);
  NARect rect = {{0, 0}, {1, 1}};
  return rect;
}



NA_HDEF void na_SetScreenRect(NA_UIElement* screen, NARect rect) {
  NA_UNUSED(screen);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("A screen can not be resized by software.");
  #endif
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
