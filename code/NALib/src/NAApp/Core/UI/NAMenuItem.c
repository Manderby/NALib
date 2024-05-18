
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitMenuItem(NAMenuItem* menuItem, void* nativePtr, NA_UIElement* parent){
  // Note that the nativePtr in this case is a pointer to the menuItem itself,
  // as there does not exist a native pointer for a MenuItem in WINAPI at all.
  na_InitUIElement(&(menuItem->uiElement), NA_UI_MENUITEM, nativePtr);
  na_SetUIElementParent(&(menuItem->uiElement), parent, NA_FALSE);
}



NA_HDEF void na_ClearMenuItem(NAMenuItem* menuItem){
  na_ClearUIElement(&(menuItem->uiElement));
}



NA_HDEF NARect na_GetMenuItemRect(const NA_UIElement* menuItem)
{
  NA_UNUSED(menuItem);
  return naMakeRectS(0, 0, 1, 1);
}



NA_HDEF void na_SetMenuItemRect(NA_UIElement* menuItem, NARect rect){
  NA_UNUSED(menuItem);
  NA_UNUSED(rect);
  #if NA_DEBUG
    naError("MenuItem rects can not be set.");
  #endif
}



#endif // NA_COMPILE_GUI == 1
