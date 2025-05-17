

// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!



NA_HDEF void na_InitWindow(
  NAWindow* window,
  void* nativePtr,
  void* nativeScreenPtr,
  NASpace* contentSpace,
  NABool fullScreen,
  NABool resizeable,
  NARect windowedFrame)
{
  na_InitCoreUIElement(&window->uiElement, NA_UI_WINDOW, nativePtr);
  naAddListLastMutable(&naGetApplication()->windows, window);
  window->storageTag = 0;
  window->contentSpace = contentSpace;
  window->coreFlags = NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS;
  if(fullScreen) { window->coreFlags |= NA_CORE_WINDOW_FLAG_FULLSCREEN; }
  if(resizeable) { window->coreFlags |= NA_CORE_WINDOW_FLAG_RESIZEABLE; }
  window->windowedFrame = windowedFrame;
  
  NAScreen* screen = na_GetApplicationScreenWithNativePtr(nativeScreenPtr);
  na_AddScreenWindow(
    screen,
    window);
  na_SetUIElementParent((NA_UIElement*)window, screen, NA_TRUE);
}



NA_HDEF void na_ClearWindow(NAWindow* window) {
  naRemoveListData(&naGetApplication()->windows, window);
  
  if(window->contentSpace)
    naDelete(window->contentSpace);
  
  na_ClearCoreUIElement(&window->uiElement);
}



NA_HDEF void na_UpdateWindowScreen(NAWindow* window, void* nativeScreenPtr) {
  NAScreen* screen = na_GetApplicationScreenWithNativePtr(nativeScreenPtr);
  NAScreen* oldScreen = naGetUIElementParentMutable(window);
  if(screen && screen != oldScreen) {
    na_RemoveScreenWindow(oldScreen, window);
    na_AddScreenWindow(screen, window);
    na_SetUIElementParent((NA_UIElement*)window, screen, NA_TRUE);
  }
}



NA_HDEF void na_RememberWindowPosition(const NAWindow* window) {
  if(window->storageTag) {
    NARect rect = na_GetWindowAbsoluteInnerRect(&window->uiElement);
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, window->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, window->storageTag);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(naIsWindowResizeable(window)) {
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, window->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, window->storageTag);
      naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
      naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
}



NA_DEF const NAScreen* naGetWindowScreen(const NAWindow* window) {
  return naGetUIElementParent(window);
}



NA_DEF void naPreventWindowClosing(NAWindow* window, NABool prevent) {
  #if NA_DEBUG
    if(!naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE))
      naError("This function is only allowed during a \"CLOSES\" event");
  #endif
  naSetFlagu32(&window->coreFlags, NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, prevent);
}

NA_DEF NABool naIsWindowFullscreen(NAWindow* window) {
  return naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_FULLSCREEN);
}

NA_DEF NABool naIsWindowResizeable(const NAWindow* window) {
  return naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_RESIZEABLE);
}

NA_DEF void naSetWindowStorageTag(NAWindow* window, size_t storageTag) {
  NARect rect = na_GetWindowRect(&window->uiElement);
  NABool resizeable = naIsWindowResizeable(window);
  
  window->storageTag = storageTag;
  if(window->storageTag) {
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, window->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, window->storageTag);
    rect.pos.x = naInitPreferencesDouble(
      naGetStringUTF8Pointer(prefPosXString),
      rect.pos.x,
      -NA_INFINITY,
      NA_INFINITY);
    rect.pos.y = naInitPreferencesDouble(
      naGetStringUTF8Pointer(prefPosYString),
      rect.pos.y,
      -NA_INFINITY,
      NA_INFINITY);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(resizeable) {
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, window->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, window->storageTag);
      rect.size.width = naInitPreferencesDouble(
        naGetStringUTF8Pointer(prefSizeWidthString),
        rect.size.width,
        -NA_INFINITY,
        NA_INFINITY);
      rect.size.height = naInitPreferencesDouble(
        naGetStringUTF8Pointer(prefSizeHeightString),
        rect.size.height,
        -NA_INFINITY,
        NA_INFINITY);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
    
  na_SetWindowRect(&window->uiElement, rect);
}

NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window) {
  return window->contentSpace;
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
