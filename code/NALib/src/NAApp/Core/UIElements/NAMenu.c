
#include "../NAAppCore.h"

#if NA_COMPILE_GUI == 1



NA_HDEF void na_InitMenu(NAMenu* menu, void* nativePtr, NA_UIElement* parent) {
  na_InitUIElement(&menu->uiElement, NA_UI_MENU, nativePtr);
  naInitList(&menu->childs);
  na_SetUIElementParent(&menu->uiElement, parent, NA_FALSE);
}



NA_HDEF void na_ClearMenu(NAMenu* menu) {
  naForeachListMutable(&menu->childs, (NAMutator)naDelete);
  naClearList(&menu->childs);
  // todo
  // na_ClearUIElement(&menu->uiElement);
}



NA_HDEF void na_AddMenuChild(NAMenu* menu, NAMenuItem* child, const NAMenuItem* itemAt) {
  NAListIterator iter = naMakeListModifier(&menu->childs);
  naLocateListData(&iter, itemAt);
  naAddListBeforeMutable(&iter, child);
  naClearListIterator(&iter);
  na_SetUIElementParent(&child->uiElement, menu, NA_FALSE);
}


NA_DEF size_t naGetMenuItemIndex(const NAMenu* menu, const NAMenuItem* item) {
  return naGetListElemIndex(&menu->childs, item);
}



NA_DEF size_t naGetMenuItemCount(const NAMenu* menu) {
  return naGetListCount(&menu->childs);
}



NA_HDEF NARect na_GetMenuRect(const NA_UIElement* menu)
{
  NA_UNUSED(menu);
  return naMakeRectS(0, 0, 1, 1);
}



NA_HDEF void na_SetMenuRect(NA_UIElement* menu, NARect rect) {
  NA_UNUSED(menu);
  NA_UNUSED(rect);
  #if NA_DEBUG
  naError("Menu rects can not be set.");
  #endif
}


#endif // NA_COMPILE_GUI == 1
