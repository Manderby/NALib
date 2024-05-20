
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1

#include "../../NAPreferences.h"



NA_HDEF void na_InitWindow(NAWindow* window, void* nativePtr, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame){
  na_InitUIElement(&(window->uiElement), NA_UI_WINDOW, nativePtr);
  naAddListLastMutable(&(naGetApplication()->windows), window);
  window->contentSpace = contentSpace;
  window->coreFlags = NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS;
  if(fullScreen){ window->coreFlags |= NA_CORE_WINDOW_FLAG_FULLSCREEN; }
  if(resizeable){ window->coreFlags |= NA_CORE_WINDOW_FLAG_RESIZEABLE; }
  window->windowedFrame = windowedFrame;
}



NA_HDEF void na_ClearWindow(NAWindow* window){
  naRemoveListData(&(naGetApplication()->windows), window);
  
  if(window->contentSpace)
    naDelete(window->contentSpace);
  
  na_ClearUIElement(&(window->uiElement));
}



NA_HDEF void na_RememberWindowPosition(const NAWindow* window){
  if(window->storageTag){
    NARect rect = na_GetWindowAbsoluteInnerRect(&(window->uiElement));
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, (int)window->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, (int)window->storageTag);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosXString), rect.pos.x);
    naSetPreferencesDouble(naGetStringUTF8Pointer(prefPosYString), rect.pos.y);
    naDelete(prefPosXString);
    naDelete(prefPosYString);
    if(naIsWindowResizeable(window)){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, (int)window->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, (int)window->storageTag);
      naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeWidthString), rect.size.width);
      naSetPreferencesDouble(naGetStringUTF8Pointer(prefSizeHeightString), rect.size.height);
      naDelete(prefSizeWidthString);
      naDelete(prefSizeHeightString);
    }
  }
}



NA_DEF void naPreventWindowFromClosing(NAWindow* window, NABool prevent){
  #if NA_DEBUG
    if(!naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_TRIES_TO_CLOSE))
      naError("This function is only allowed during a \"CLOSES\" event");
  #endif
  naSetFlagu32(&(window->coreFlags), NA_CORE_WINDOW_FLAG_PREVENT_FROM_CLOSING, prevent);
}

NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  return naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_FULLSCREEN);
}

NA_DEF NABool naIsWindowResizeable(const NAWindow* window){
  return naGetFlagu32(window->coreFlags, NA_CORE_WINDOW_FLAG_RESIZEABLE);
}

NA_DEF NASpace* naGetWindowContentSpace(NAWindow* window){
  return window->contentSpace;
}



NA_DEF NARect naSetWindowStorageTag(NAWindow* window, NAInt storageTag, NARect rect, NABool resizeable){
  window->storageTag = storageTag;
  if(window->storageTag){
    NAString* prefPosXString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_X, (int)window->storageTag);
    NAString* prefPosYString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_POS_Y, (int)window->storageTag);
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
    if(resizeable){
      NAString* prefSizeWidthString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_WIDTH, (int)window->storageTag);
      NAString* prefSizeHeightString = naNewStringWithFormat(NA_WINDOW_PREF_STRING_SIZE_HEIGHT, (int)window->storageTag);
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
  return rect;
}



#endif // NA_COMPILE_GUI == 1
