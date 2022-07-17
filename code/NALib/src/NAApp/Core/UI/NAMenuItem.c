
#include "NAAppCore.h"



NA_HDEF void na_InitMenuItem(NAMenuItem* menuItem, void* nativePtr, NA_UIElement* parent){
  // Note that the nativePtr in this case is a pointer to the menuItem itself,
  // as there does not exist a native pointer for a MenuItem in WINAPI at all.
  na_InitUIElement(&(menuItem->uiElement), NA_UI_MENUITEM, nativePtr);
  na_SetUIElementParent(&(menuItem->uiElement), parent, NA_FALSE);
}



NA_HDEF void na_ClearMenuItem(NAMenuItem* menuItem){
  na_ClearUIElement(&(menuItem->uiElement));
}



NA_HDEF void na_SetMenuItemId(NAMenuItem* menuItem, uint32 id){
  menuItem->id = id;
}



NA_HDEF uint32 na_GetMenuItemId(const NAMenuItem* menuItem){
  return menuItem->id;
}
