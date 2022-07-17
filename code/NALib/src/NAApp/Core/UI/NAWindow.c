
#include "NAAppCore.h"
#include "NAPreferences.h"



NA_HDEF void na_InitWindow(NAWindow* window, void* nativePtr, NASpace* contentSpace, NABool fullScreen, NABool resizeable, NARect windowedFrame){
  na_InitUIElement(&(window->uiElement), NA_UI_WINDOW, nativePtr);
  naAddListLastMutable(&(na_App->windows), window);
  window->contentSpace = contentSpace;
  window->coreFlags = NA_CORE_WINDOW_FLAG_ACCEPTS_KEY_REACTIONS;
  if(fullScreen){window->coreFlags |= NA_CORE_WINDOW_FLAG_FULLSCREEN;}
  if(resizeable){window->coreFlags |= NA_CORE_WINDOW_FLAG_RESIZEABLE;}
  window->windowedFrame = windowedFrame;
}



NA_HDEF void na_ClearWindow(NAWindow* window){
  naRemoveListData(&(na_App->windows), window);
  if(window->contentSpace){naDelete(window->contentSpace);}
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
